/** @file
  Mach-O 64-bit arm64e Parser and Segment Loader implementation.

  Copyright (c) 2026 Project Silicium authors.
  SPDX-License-Identifier: MIT
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>

#include "MachO.h"

EFI_STATUS
ParseAndLoadMachO (
  IN  VOID                  *FileBuffer,
  IN  UINTN                  FileSize,
  OUT EFI_PHYSICAL_ADDRESS  *KernelEntry,
  OUT EFI_PHYSICAL_ADDRESS  *KernelPhysBase,
  OUT UINT64                *KernelVirtBase,
  OUT UINT64                *KernelSize
  )
{
  MACH_HEADER_64     *Header;
  UINT8              *Cursor;
  UINT32              Index;
  SEGMENT_COMMAND_64 *Seg;
  UINT64              LowestVirt;
  UINT64              HighestVirt;
  EFI_STATUS          Status;
  EFI_PHYSICAL_ADDRESS AllocatedBase;
  UINTN               TotalPages;
  UINT64              Span;

  if (FileBuffer == NULL || FileSize < sizeof (MACH_HEADER_64)) {
    return EFI_INVALID_PARAMETER;
  }

  Header = (MACH_HEADER_64 *)FileBuffer;
  if (Header->magic != MH_MAGIC_64) {
    DEBUG ((DEBUG_ERROR, "[IosLoader] Invalid Mach-O 64 magic: 0x%08x\n", Header->magic));
    return EFI_LOAD_ERROR;
  }

  if (Header->cputype != CPU_TYPE_ARM64) {
    DEBUG ((DEBUG_ERROR, "[IosLoader] Invalid CPUType: 0x%08x\n", Header->cputype));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "[IosLoader] Valid Mach-O arm64e kernel found. Commands: %u\n", Header->ncmds));

  LowestVirt  = 0xFFFFFFFFFFFFFFFFULL;
  HighestVirt = 0;
  Cursor      = (UINT8 *)FileBuffer + sizeof (MACH_HEADER_64);

  /* First pass: determine virtual memory span */
  for (Index = 0; Index < Header->ncmds; Index++) {
    UINT32 Cmd     = *(UINT32 *)Cursor;
    UINT32 CmdSize = *(UINT32 *)(Cursor + 4);

    if (Cmd == LC_SEGMENT_64) {
      Seg = (SEGMENT_COMMAND_64 *)Cursor;
      if (Seg->vmsize > 0) {
        if (Seg->vmaddr < LowestVirt) {
          LowestVirt = Seg->vmaddr;
        }
        if (Seg->vmaddr + Seg->vmsize > HighestVirt) {
          HighestVirt = Seg->vmaddr + Seg->vmsize;
        }
      }
    }
    Cursor += CmdSize;
  }

  if (LowestVirt >= HighestVirt) {
    return EFI_NOT_FOUND;
  }

  Span = HighestVirt - LowestVirt;
  TotalPages = EFI_SIZE_TO_PAGES ((UINTN)Span);

  DEBUG ((DEBUG_INFO, "[IosLoader] Virtual span: 0x%lx - 0x%lx (Span: 0x%lx, Pages: %lu)\n",
          LowestVirt, HighestVirt, Span, (UINT64)TotalPages));

  /* Allocate physical pages in memory */
  AllocatedBase = 0x88000000; /* Safe base in SM8650 high RAM */
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiLoaderData,
                  TotalPages,
                  &AllocatedBase
                  );
  if (EFI_ERROR (Status)) {
    Status = gBS->AllocatePages (
                    AllocateAnyPages,
                    EfiLoaderData,
                    TotalPages,
                    &AllocatedBase
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[IosLoader] Failed to allocate %lu pages for kernel!\n", (UINT64)TotalPages));
      return Status;
    }
  }

  ZeroMem ((VOID *)(UINTN)AllocatedBase, TotalPages * EFI_PAGE_SIZE);

  /* Second pass: copy segments to physical memory */
  Cursor = (UINT8 *)FileBuffer + sizeof (MACH_HEADER_64);
  *KernelEntry = 0;

  for (Index = 0; Index < Header->ncmds; Index++) {
    UINT32 Cmd     = *(UINT32 *)Cursor;
    UINT32 CmdSize = *(UINT32 *)(Cursor + 4);

    if (Cmd == LC_SEGMENT_64) {
      Seg = (SEGMENT_COMMAND_64 *)Cursor;
      if (Seg->filesize > 0 && Seg->fileoff + Seg->filesize <= FileSize) {
        UINTN  SegOffset = (UINTN)(Seg->vmaddr - LowestVirt);
        VOID  *Dest      = (VOID *)((UINTN)AllocatedBase + SegOffset);
        VOID  *Src       = (VOID *)((UINTN)FileBuffer + (UINTN)Seg->fileoff);

        CopyMem (Dest, Src, (UINTN)Seg->filesize);
        DEBUG ((DEBUG_INFO, "[IosLoader] Loaded segment %a to phys 0x%lx (size: 0x%lx)\n",
                Seg->segname, (UINT64)(UINTN)Dest, Seg->filesize));

        /* If this is __TEXT_EXEC, record entry address */
        if (AsciiStrnCmp (Seg->segname, "__TEXT_EXEC", 11) == 0 && *KernelEntry == 0) {
          *KernelEntry = (EFI_PHYSICAL_ADDRESS)(UINTN)Dest;
        }
      }
    } else if (Cmd == LC_UNIXTHREAD) {
      /* ARM_THREAD_STATE64 has pc at offset 16 + 32*8 */
      UINT64 *ThreadState = (UINT64 *)(Cursor + 16);
      UINT64  Pc          = ThreadState[32];
      if (Pc >= LowestVirt && Pc < HighestVirt) {
        *KernelEntry = AllocatedBase + (Pc - LowestVirt);
        DEBUG ((DEBUG_INFO, "[IosLoader] Entry point extracted from LC_UNIXTHREAD: phys 0x%lx\n", *KernelEntry));
      }
    }
    Cursor += CmdSize;
  }

  if (*KernelEntry == 0) {
    *KernelEntry = AllocatedBase;
  }

  *KernelPhysBase = AllocatedBase;
  *KernelVirtBase = LowestVirt;
  *KernelSize     = Span;

  return EFI_SUCCESS;
}
