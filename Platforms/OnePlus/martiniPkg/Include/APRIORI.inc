APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/martini/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/martini/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/martini/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/martini/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/martini/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF Binaries/martini/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/martini/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/martini/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/martini/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/martini/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/martini/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/martini/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/martini/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/martini/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/OcdtDxe/OppoProject.inf
  INF Binaries/martini/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/martini/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/martini/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/martini/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/martini/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/martini/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/martini/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
