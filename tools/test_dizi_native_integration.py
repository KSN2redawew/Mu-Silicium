#!/usr/bin/env python3
"""Offline invariant checks for native Redmi Pad Pro (dizi) EDK2 wiring.

No hardware access or third-party packages required.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "Platforms/Xiaomi"
SRC = BASE / "DiziBootPkg/BootSelector"
PLAT = BASE / "diziPkg"
DSC = (PLAT / "dizi.dsc").read_text()
FDF = (PLAT / "dizi.fdf").read_text()
BDS = (PLAT / "Library/DiziDeviceBootManagerLib/DiziBootManager.c").read_text()
BDS_INF = (PLAT / "Library/DiziDeviceBootManagerLib/DiziDeviceBootManagerLib.inf").read_text()
INF = (SRC / "BootSelector.inf").read_text()
C = (SRC / "BootSelector.c").read_text()
KEYS = (SRC / "DiziPmicKeys.c").read_text()
ARB = (SRC / "DiziSpmiArbProbe.c").read_text()
OBS = (SRC / "DiziSpmiArbRead.c").read_text()


class NativeDiziSelectorTests(unittest.TestCase):
    def test_build_and_bds_autostart_are_native(self):
        self.assertIn("  DiziBootPkg/BootSelector/BootSelector.inf", DSC)
        self.assertIn("  INF DiziBootPkg/BootSelector/BootSelector.inf", FDF)
        self.assertIn("  DiziLaunchEmbeddedSelector ();", BDS)
        self.assertIn("  DiziInitUsbHostEarly ();", BDS)
        self.assertIn("  gEfiFirmwareVolume2ProtocolGuid", BDS_INF)
        self.assertIn("  gEfiPciRootBridgeIoProtocolGuid", BDS_INF)
        self.assertIn("  DiziSpmiArbRead.c", INF)
        self.assertIn("  DiziPmicKeys.c", INF)
        self.assertIn('#define DIZI_BUILD_STAMP "R4-MU-NATIVE"', C)
        self.assertIn("DiziPmicSampleKeys (&Input.PmicKeys, &Input.Arbiter", C)
        self.assertNotIn("overlay_ksn_dizi", DSC + FDF + BDS)

    def test_gen3_keys_and_single_bounded_spmi_read_command(self):
        self.assertIn("DIZI_PON_GEN3_POWER_MASK 0x80U", KEYS)
        self.assertIn("DIZI_PON_GEN3_RESIN_MASK 0x40U", KEYS)
        self.assertIn("Keys->VolUp = (RawGpio & 0x01U) == 0", KEYS)
        self.assertNotIn("Keys->PowerDown = (RawPon & 0x01U)", KEYS)
        self.assertNotIn("Keys->VolDown = (RawPon & 0x02U)", KEYS)
        self.assertIn("Result->Version >> 28U", ARB)
        self.assertIn("Result->MapSamplesValid = TRUE;", ARB)
        self.assertIn("DIZI_SPMI_TIMEOUT_US 1000U", OBS)
        self.assertEqual(OBS.count("MmioWrite32 ("), 1)
        self.assertIn(
            "MmioWrite32 (DIZI_OBSERVER_BASE + Offset + DIZI_SPMI_CMD, Cmd)",
            OBS)
        for forbidden in ("SpmiWrite", "InstallProtocolInterface",
                          "MmioWrite8", "MmioWrite16", "MmioWrite64"):
            self.assertNotIn(forbidden, OBS)

    def test_unnecessary_foreign_spmi_firmware_is_not_added(self):
        dxe = (PLAT / "Include/DXE.inc").read_text()
        self.assertNotIn("SPMIDxe/SPMIDxe.inf", dxe)
        self.assertNotIn("PmicDxeLa/PmicDxeLa.inf", dxe)
        self.assertIn("DiziDeviceBootManagerLib", DSC)


if __name__ == "__main__":
    unittest.main()
