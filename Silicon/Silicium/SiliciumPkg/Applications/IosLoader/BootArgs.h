/** @file
  Apple Silicon XNU boot_args definition for SM8650 IosLoader.

  Copyright (c) 2026 Project Silicium authors.
  SPDX-License-Identifier: MIT
**/

#ifndef _BOOT_ARGS_H_
#define _BOOT_ARGS_H_

#include <Uefi.h>

#define BOOT_LINE_LENGTH 256

#pragma pack(1)
typedef struct {
  UINT64 v_baseAddr;
  UINT64 v_display;
  UINT64 v_rowBytes;
  UINT64 v_width;
  UINT64 v_height;
  UINT64 v_depth;
} Boot_Video;

typedef struct {
  UINT16     Revision;           /* 1 */
  UINT16     Version;            /* 2 */
  UINT64     virtBase;           /* 0xfffffff007004000 */
  UINT64     physBase;           /* 0x80000000 (SM8650 DDR Base) */
  UINT64     memSize;            /* Available physical RAM */
  UINT64     topOfKernelData;    /* End of mapped kernel segments */
  Boot_Video video;
  UINT32     machineType;        /* Target board ID (0) */
  VOID      *deviceTreeP;        /* Pointer to loaded DeviceTree.d63ap.bin */
  UINT32     deviceTreeLength;   /* Length of ADT */
  CHAR8      CommandLine[BOOT_LINE_LENGTH]; /* Kernel command line */
  UINT64     bootFlags;
  UINT64     memSizeActual;
} boot_args;
#pragma pack()

#endif /* _BOOT_ARGS_H_ */
