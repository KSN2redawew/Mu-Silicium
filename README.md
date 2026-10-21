# Mu-Silicium — DIZI experimental fork

DIZI-focused UEFI bring-up for **Xiaomi Redmi Pad Pro (SM7435)**, with
future Android/iPadOS dualboot as a separate target.

**Not a production bootloader, no released dizi firmware yet.**
The existing upstream devices and binary dependencies remain in Git history
and the source tree; CI and the public status list now target **dizi only**.

- [DIZI hardware status](Status.md)
- [Build documentation](Building.md)
- [UEFI diagnostic build workflow](.github/workflows/active_devices.yml)

## Build

```sh
python3 build_uefi.py -d dizi -r DEBUG -c
```

Or use **Actions → DIZI UEFI diagnostic build → Run workflow**.

The new build evaluates DIZI's key mapping:
- Volume Up uses **PM6450 SPMI GPIO1**, active low.
- Power and resin use **PMK8350 PON/SPMI**.
- Firmware binaries are **not** guaranteed to have Redmi Pad Pro-specific
  key mappings. The preflight is static, not hardware confirmation.

Do not flash experimental UEFI; check Power/Volume input and recovery via a
reversible boot method first. Do not treat a passing CI build as evidence
that USB host, PD, Linux/Windows, storage, or dualboot work.

Upstream: [Project Silicium](https://github.com/Project-Silicium/Mu-Silicium).
Code and licensing remain governed by [LICENSE](LICENSE).
