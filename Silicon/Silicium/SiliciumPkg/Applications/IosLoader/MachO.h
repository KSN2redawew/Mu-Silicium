/** @file
  Mach-O 64-bit arm64e Parser definitions for iOS 16.6 XNU.

  Copyright (c) 2026 Project Silicium authors.
  SPDX-License-Identifier: MIT
**/

#ifndef _MACHO_H_
#define _MACHO_H_

#include <Uefi.h>

#define MH_MAGIC_64          0xFEEDFACF
#define MH_CIGAM_64          0xCFFAEDFE

#define CPU_TYPE_ARM64        0x0100000C
#define CPU_SUBTYPE_ARM64E    0x00000002

#define LC_SEGMENT_64        0x19
#define LC_UNIXTHREAD        0x5
#define LC_MAIN              0x80000028

#pragma pack(1)
typedef struct {
  UINT32 magic;
  UINT32 cputype;
  UINT32 cpusubtype;
  UINT32 filetype;
  UINT32 ncmds;
  UINT32 sizeofcmds;
  UINT32 flags;
  UINT32 reserved;
} MACH_HEADER_64;

typedef struct {
  UINT32 cmd;
  UINT32 cmdsize;
  CHAR8  segname[16];
  UINT64 vmaddr;
  UINT64 vmsize;
  UINT64 fileoff;
  UINT64 filesize;
  UINT32 maxprot;
  UINT32 initprot;
  UINT32 nsects;
  UINT32 flags;
} SEGMENT_COMMAND_64;
#pragma pack()

EFI_STATUS
ParseAndLoadMachO (
  IN  VOID                  *FileBuffer,
  IN  UINTN                  FileSize,
  OUT EFI_PHYSICAL_ADDRESS  *KernelEntry,
  OUT EFI_PHYSICAL_ADDRESS  *KernelPhysBase,
  OUT UINT64                *KernelVirtBase,
  OUT UINT64                *KernelSize
  );

#endif /* _MACHO_H_ */
