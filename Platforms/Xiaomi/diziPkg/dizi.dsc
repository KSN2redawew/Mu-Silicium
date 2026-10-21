##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = dizi
  PLATFORM_GUID                  = 5B4C270A-700B-48F8-BA4F-AE78FB1BD171
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/diziPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = diziPkg/dizi.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM7435
  # 1 = SM6450
  #
  SOC_TYPE                       = 0

!include NetraniPkg/NetraniPkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # DDR Memory
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xA760D000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Pad Pro"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"dizi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"2405CRPFDL"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"dizi"

  #
  # Simple Frame Buffer
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1600
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2560
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  #
  # Console input
  #
  # Explicit DIZI-only experiment: enumerate console input at boot instead of
  # deferring it to the USB-only OnDemandConInConnect hook. The shipped
  # ButtonsDxe is a Qualcomm binary and still needs an on-device test; this
  # setting cannot make the PM6450/PMK8350 electrical inputs work by itself.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|FALSE

  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|diziPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|diziPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf

[Components]
  #
  # ACPI Tables
  #
  #dizi/AcpiTables.inf
