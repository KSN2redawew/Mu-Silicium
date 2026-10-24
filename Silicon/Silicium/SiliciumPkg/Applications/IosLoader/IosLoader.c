/** @file
  iOS 16.6 XNU Kernel UEFI Loader application for SM8650 / OnePlus Ace 5.

  Copyright (c) 2026 Project Silicium authors.
  SPDX-License-Identifier: MIT
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>

#include <Protocol/SimpleFileSystem.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>

#include "BootArgs.h"
#include "MachO.h"

typedef VOID (*XNU_ENTRY_FUNC)(UINT64 BootArgsPhys);

STATIC
EFI_STATUS
LoadFileFromVolume (
  IN  EFI_FILE_PROTOCOL *Volume,
  IN  CHAR16            *FileName,
  OUT VOID             **Buffer,
  OUT UINTN             *BufferSize
  )
{
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL *FileHandle;
  EFI_FILE_INFO     *FileInfo;
  UINTN              InfoSize;

  Status = Volume->Open (Volume, &FileHandle, FileName, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  InfoSize = 0;
  Status   = FileHandle->GetInfo (FileHandle, &gEfiFileInfoGuid, &InfoSize, NULL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    FileHandle->Close (FileHandle);
    return Status;
  }

  FileInfo = AllocateZeroPool (InfoSize);
  if (FileInfo == NULL) {
    FileHandle->Close (FileHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = FileHandle->GetInfo (FileHandle, &gEfiFileInfoGuid, &InfoSize, FileInfo);
  if (EFI_ERROR (Status)) {
    FreePool (FileInfo);
    FileHandle->Close (FileHandle);
    return Status;
  }

  *BufferSize = (UINTN)FileInfo->FileSize;
  FreePool (FileInfo);

  *Buffer = AllocateZeroPool (*BufferSize);
  if (*Buffer == NULL) {
    FileHandle->Close (FileHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = FileHandle->Read (FileHandle, BufferSize, *Buffer);
  FileHandle->Close (FileHandle);

  return Status;
}

EFI_STATUS
EFIAPI
IosLoaderMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                         Status;
  UINTN                              HandleCount;
  EFI_HANDLE                        *Handles;
  UINTN                              Index;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *Fs;
  EFI_FILE_PROTOCOL                 *Root;
  VOID                              *KernelBuf = NULL;
  UINTN                              KernelSize = 0;
  VOID                              *DtBuf = NULL;
  UINTN                              DtSize = 0;
  EFI_GRAPHICS_OUTPUT_PROTOCOL      *Gop = NULL;
  EFI_PHYSICAL_ADDRESS               KernelEntry = 0;
  EFI_PHYSICAL_ADDRESS               KernelPhys = 0;
  UINT64                             KernelVirt = 0;
  UINT64                             KernelSpan = 0;
  boot_args                         *BootArgs;
  EFI_PHYSICAL_ADDRESS               BootArgsPhys;
  UINTN                              MapKey = 0;
  UINTN                              DescriptorSize = 0;
  UINT32                             DescriptorVersion = 0;
  UINTN                              MemoryMapSize = 0;
  EFI_MEMORY_DESCRIPTOR             *MemoryMap = NULL;

  Print (L"\n=======================================================\n");
  Print (L" Project Silicium: iOS 16.6 XNU Kernel Loader (SM8650) \n");
  Print (L" Target: OnePlus Ace 5 (giulia) Cortex-X4 / A720       \n");
  Print (L"=======================================================\n\n");

  /* Locate GOP */
  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&Gop);
  if (EFI_ERROR (Status)) {
    Print (L"[-] Warning: GOP not located (Status = %r)\n", Status);
  } else {
    Print (L"[+] GOP Active: %ux%u, FB=0x%lx, ScanLine=%u\n",
           Gop->Mode->Info->HorizontalResolution,
           Gop->Mode->Info->VerticalResolution,
           Gop->Mode->FrameBufferBase,
           Gop->Mode->Info->PixelsPerScanLine);
  }

  /* Enumerate Simple File System volumes */
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR (Status) || HandleCount == 0) {
    Print (L"[-] No file system volumes located!\n");
    return EFI_NOT_FOUND;
  }

  Print (L"[*] Scanning %lu storage volume(s)...\n", (UINT64)HandleCount);

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gEfiSimpleFileSystemProtocolGuid,
                    (VOID **)&Fs
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = Fs->OpenVolume (Fs, &Root);
    if (EFI_ERROR (Status)) {
      continue;
    }

    /* Probe kernel candidates */
    if (KernelBuf == NULL) {
      Status = LoadFileFromVolume (Root, L"\\EFI\\BOOT\\BOOTAA64.EFI", &KernelBuf, &KernelSize);
      if (EFI_ERROR (Status)) {
        Status = LoadFileFromVolume (Root, L"\\kernelcache.release.iphone14.macho", &KernelBuf, &KernelSize);
      }
      if (EFI_ERROR (Status)) {
        Status = LoadFileFromVolume (Root, L"\\ios\\kernelcache.release.iphone14.macho", &KernelBuf, &KernelSize);
      }
      if (!EFI_ERROR (Status)) {
        Print (L"[+] Found iOS Kernel (%lu bytes)\n", (UINT64)KernelSize);
      }
    }

    /* Probe DeviceTree candidate */
    if (DtBuf == NULL) {
      Status = LoadFileFromVolume (Root, L"\\EFI\\BOOT\\DeviceTree.d63ap.bin", &DtBuf, &DtSize);
      if (EFI_ERROR (Status)) {
        Status = LoadFileFromVolume (Root, L"\\DeviceTree.d63ap.bin", &DtBuf, &DtSize);
      }
      if (EFI_ERROR (Status)) {
        Status = LoadFileFromVolume (Root, L"\\ios\\DeviceTree.d63ap.bin", &DtBuf, &DtSize);
      }
      if (!EFI_ERROR (Status)) {
        Print (L"[+] Found Apple DeviceTree ADT (%lu bytes)\n", (UINT64)DtSize);
      }
    }

    Root->Close (Root);
    if (KernelBuf != NULL && DtBuf != NULL) {
      break;
    }
  }

  FreePool (Handles);

  if (KernelBuf == NULL) {
    Print (L"[-] Error: iOS Mach-O kernelcache not found on any volume!\n");
    return EFI_NOT_FOUND;
  }

  /* Parse and load Mach-O */
  Print (L"[*] Parsing Mach-O 64-bit arm64e segments...\n");
  Status = ParseAndLoadMachO (
             KernelBuf,
             KernelSize,
             &KernelEntry,
             &KernelPhys,
             &KernelVirt,
             &KernelSpan
             );
  if (EFI_ERROR (Status)) {
    Print (L"[-] Failed to load Mach-O segments: %r\n", Status);
    return Status;
  }

  Print (L"[+] Mach-O loaded! Base: 0x%lx, Entry: 0x%lx\n", KernelPhys, KernelEntry);

  /* Allocate and construct boot_args */
  BootArgsPhys = 0x87F00000;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiLoaderData,
                  1,
                  &BootArgsPhys
                  );
  if (EFI_ERROR (Status)) {
    BootArgsPhys = (EFI_PHYSICAL_ADDRESS)(UINTN)AllocateZeroPool (sizeof (boot_args));
  }

  BootArgs = (boot_args *)(UINTN)BootArgsPhys;
  ZeroMem (BootArgs, sizeof (boot_args));

  BootArgs->Revision        = 1;
  BootArgs->Version         = 2;
  BootArgs->virtBase        = KernelVirt;
  BootArgs->physBase        = KernelPhys;
  BootArgs->memSize         = 0x400000000ULL; /* 16GB SM8650 RAM */
  BootArgs->topOfKernelData = KernelPhys + KernelSpan;
  BootArgs->machineType     = 0;

  if (Gop != NULL && Gop->Mode != NULL && Gop->Mode->Info != NULL) {
    BootArgs->video.v_baseAddr = Gop->Mode->FrameBufferBase;
    BootArgs->video.v_width    = Gop->Mode->Info->HorizontalResolution;
    BootArgs->video.v_height   = Gop->Mode->Info->VerticalResolution;
    BootArgs->video.v_rowBytes = Gop->Mode->Info->PixelsPerScanLine * 4;
    BootArgs->video.v_depth    = 32;
  }

  if (DtBuf != NULL) {
    BootArgs->deviceTreeP      = DtBuf;
    BootArgs->deviceTreeLength = (UINT32)DtSize;
  }

  AsciiStrCpyS (BootArgs->CommandLine, BOOT_LINE_LENGTH, "debug=0x14e serial=3 -v keepsyms=1");

  Print (L"[+] boot_args prepared at physical 0x%lx\n", BootArgsPhys);
  Print (L"[!] Jumping to iOS XNU entry point 0x%lx...\n", KernelEntry);

  /* Get Memory Map and ExitBootServices */
  MemoryMapSize = 0;
  Status = gBS->GetMemoryMap (&MemoryMapSize, NULL, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    MemoryMap = AllocatePool (MemoryMapSize + 4096);
    if (MemoryMap != NULL) {
      Status = gBS->GetMemoryMap (&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
      if (!EFI_ERROR (Status)) {
        gBS->ExitBootServices (ImageHandle, MapKey);
      }
    }
  }

  /* Branch to entry point */
  ((XNU_ENTRY_FUNC)KernelEntry)((UINT64)BootArgsPhys);

  return EFI_SUCCESS;
}
