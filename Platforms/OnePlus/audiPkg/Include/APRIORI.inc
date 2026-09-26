APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF QcomPkg/Drivers/IortDtDxe/IortDtDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/audi/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/ProjectInfoDxe/ProjectInfoDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/audi/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/audi/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/audi/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/audi/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/audi/QcomPkg/Drivers/FvUtilsDxe/FvUtilsEnhancedDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/audi/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/audi/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/audi/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/audi/QcomPkg/Drivers/RscDxe/RscDxe.inf
  INF Binaries/audi/QcomPkg/Drivers/RmVmDxe/RmVmDxe.inf
}
