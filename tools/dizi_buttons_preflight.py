#!/usr/bin/env python3
"""Static preflight for DIZI power/volume button wiring and console chain.

Only reads checked-in DTS, DSC, DXE and optionally the external binary INF.
It does not touch PMIC registers or assert that physical buttons work.
"""
import argparse
import json
from pathlib import Path
import re


def check(root: Path, *, require_binary: bool = False) -> dict:
    dts = (root / "Resources/DTBs/dizi.dts").read_text()
    dsc = (root / "Platforms/Xiaomi/diziPkg/dizi.dsc").read_text()
    dxe = (root / "Platforms/Xiaomi/diziPkg/Include/DXE.inc").read_text()
    apriori = (root / "Platforms/Xiaomi/diziPkg/Include/APRIORI.inc").read_text()
    inf_path = root / "Binaries/dizi/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf"
    # The PM6450 GPIO controller, NOT the MSM TLMM MMIO controller, owns Vol+.
    gpio = re.search(
        r'pinctrl@8800\s*\{[^}]*?compatible\s*=\s*"qcom,pm6450-gpio";'
        r'[^}]*?phandle\s*=\s*<0x4b2>;',
        dts, re.S
    )
    vol = re.search(
        r'gpio_keys\s*\{.{0,500}?vol_up\s*\{.{0,200}?'
        r'gpios\s*=\s*<0x4b2\s+0x0*1\s+0x0*1>;',
        dts, re.S
    )
    pmk = re.search(r'qcom,pmk8350@0\s*\{.{0,350}?compatible\s*=\s*"qcom,spmi-pmic";', dts, re.S)
    pwr = re.search(r'pwrkey\s*\{\s*compatible\s*=\s*"qcom,pmk8350-pwrkey";', dts)
    resin = re.search(r'resin\s*\{\s*compatible\s*=\s*"qcom,pmk8350-resin";', dts)
    if not all([gpio, vol, pmk, pwr, resin]):
        raise ValueError("DIZI PM6450 Vol+ or PMK8350 Power/Resin mapping changed")
    if 'gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|FALSE' not in dsc:
        raise ValueError("DIZI console input still deferred to USB-only path")
    if dxe.count('INF Binaries/dizi/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf') != 1:
        raise ValueError("DIZI Qualcomm ButtonsDxe missing or included twice")
    if 'DeviceBootManagerLib|diziPkg/Library/DiziDeviceBootManagerLib/DiziDeviceBootManagerLib.inf' not in dsc:
        raise ValueError("DIZI BDS is not bound to the input-enumeration library")
    bds_path = root / 'Platforms/Xiaomi/diziPkg/Library/DiziDeviceBootManagerLib/DiziBootManager.c'
    bds = bds_path.read_text()
    if 'EfiBootManagerUpdateConsoleVariable (ConIn, Path, NULL)' not in bds:
        raise ValueError("DIZI BDS does not append existing button handles to ConIn")
    for label, source in (("DXE", dxe), ("APRIORI", apriori)):
        gpio = 'INF Binaries/dizi/QcomPkg/Drivers/GpiDxe/GpiDxe.inf'
        buttons = 'INF Binaries/dizi/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf'
        pmic = 'INF Binaries/dizi/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf'
        spmi = 'INF Binaries/dizi/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf'
        pmic_core = 'INF Binaries/dizi/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf'
        providers = (spmi, pmic_core, pmic, gpio, buttons)
        if any(source.count(name) != 1 for name in providers):
            raise ValueError(f"{label} missing/duplicate SPMI/PmicDxe/GLink/GPIO/Buttons driver")
        if not (source.index(spmi) < source.index(pmic_core) < source.index(buttons)
                and source.index(pmic) < source.index(buttons)
                and source.index(gpio) < source.index(buttons)):
            raise ValueError(f"{label}: ButtonsDxe starts before SPMI or PMIC GPIO/PON providers")
    if require_binary:
        for name in ("SPMIDxe/SPMIDxe.inf", "PmicDxe/PmicDxeLa.inf"):
            p = root / "Binaries/dizi/QcomPkg/Drivers" / name
            if not p.is_file():
                raise ValueError("Missing PMIC provider firmware " + name)
    if require_binary and not inf_path.is_file():
        raise ValueError("Binaries submodule is absent: cannot verify actual ButtonsDxe image")
    chosen = None
    if inf_path.is_file():
        inf = inf_path.read_text()
        if "PE32|ButtonsDxe.patched.efi|*" not in inf:
            raise ValueError("DIZI ButtonsDxe INF no longer loads patched Power=Enter binary")
        if not (inf_path.parent / "ButtonsDxe.patched.efi").is_file():
            raise ValueError("Patched ButtonsDxe.efi is absent")
        chosen = "ButtonsDxe.patched.efi"
    return {
        "device": "Xiaomi Redmi Pad Pro (dizi, SM7435)",
        "volume_up": {"controller": "PM6450 GPIO", "pin": 1, "active_low": True, "uefi_runtime_verified": False},
        "power": {"controller": "PMK8350 PON/SPMI", "uefi_runtime_verified": False},
        "volume_down_or_resin": {"controller": "PMK8350 PON/SPMI", "uefi_runtime_verified": False},
        "binary_origin": "Redmi Note 13 Pro 5G firmware; button mappings may differ",
        "buttons_dxe": chosen or "external Binaries submodule not checked",
        "driver_init_order": "SPMIDxe, PmicDxe, PmicGlinkDxe and GpiDxe before ButtonsDxe",
        "conin_eager_connect": True,
        "conin_driver_binding": "DiziDeviceBootManagerLib",
        "button_protocol_runtime_verified": False,
        "usb_host_mode_verified": False,
        "usb_power_delivery_verified": False,
        "internal_storage_verified": False,
        "suitable_for_release": False,
        "next_hardware_test": "Read EFI console input events for Power, Vol+, Vol- and inspect button protocol registration; NEVER flash.",
    }


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    p.add_argument("--require-binary", action="store_true")
    p.add_argument("--json-out", type=Path)
    args = p.parse_args()
    result = check(args.root, require_binary=args.require_binary)
    data = json.dumps(result, indent=2, sort_keys=True) + "\n"
    print(data, end="")
    if args.json_out:
        args.json_out.parent.mkdir(parents=True, exist_ok=True)
        args.json_out.write_text(data)


if __name__ == "__main__":
    main()
