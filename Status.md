# DIZI-only UEFI hardware status

**Device:** Xiaomi Redmi Pad Pro / Redmi Pad Pro 5G (dizi, Snapdragon 7s Gen 2 / SM7435)

This fork builds **dizi only**. Historical platform source files are retained
for reference and as submodule dependencies, but are not part of its release
matrix. The original upstream status is available in Git history.

## UEFI status

| Feature | Observed | State |
|---|---|---|
| Display | User screenshot; framebuffer output visible | ✅ |
| Internal storage | UFS stack included, mounting/boot access not yet confirmed | ❔ |
| Side buttons | No key events observed; Qualcomm binary is device-derived | ❌ |
| USB host | Not working according to hardware test | ❌ |
| USB device | Reported working | ✅ |
| USB Power Delivery | Not working according to hardware test | ❌ |
| USB Mass Storage | Reported working | ✅ |
| Windows Boot | Not working | ❌ |
| Linux Boot | Unverified | ❔ |

## Button diagnostics

- **Volume Up:** PM6450 **SPMI GPIO1**, active low. Not a TLMM GPIO.
- **Power:** PMK8350 PON/SPMI `pwrkey`.
- **Resin / Volume Down:** PMK8350 PON/SPMI `resin` according to the device tree.
- The external `Binaries/dizi` firmware came from the Redmi Note 13 Pro 5G
  device family. Its `ButtonsDxe.inf` selects `ButtonsDxe.patched.efi`, whose
  documented patch changes the Qualcomm Power scan code to Enter. This does
  **not** prove Power or Volume inputs work on the Redmi Pad Pro.
- A DIZI-specific `PcdConInConnectOnDemand=FALSE` experiment makes the
  console connection eager rather than using a USB-only on-demand path.
- The DIZI-specific `DiziDeviceBootManagerLib` attempts to append existing
  Qualcomm `SimpleTextIn`/`SimpleTextInEx` button handles to UEFI `ConIn`.
  It does **not** poll PMIC hardware; physical input still needs validation.

`python3 tools/dizi_buttons_preflight.py` checks DTS and build dependencies.
It does not probe or write PMIC registers.

**Release gate:** do not publish a flashable release until the device reliably
recognizes Power/Volume keys, can exit UEFI/reboot safely, and avoids regression
of Android boot. USB host/PD, UFS root and Linux remain separate investigations.

## Safe test policy

Only test an experimental image with an already verified **temporary boot**
method and working stock recovery path. Do not flash to `boot_a`,
`boot_b`, `vendor_boot`, `userdata`, or repartition UFS.
