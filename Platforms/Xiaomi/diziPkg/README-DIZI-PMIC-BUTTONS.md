# DIZI native EDK2 selector / PMIC buttons

This is the **native** integration of the tested DIZI GOP menu and PMIC
button fallback. The selector sources are in
`Platforms/Xiaomi/DiziBootPkg/BootSelector/`, compiled from the dizi DSC
and embedded in FVMAIN in the dizi FDF. The DIZI-only BDS library invokes
it after initializing the console. No external
`dizi_IpadOS_maybe/tools/overlay_ksn_dizi.py` or borrowed SPMIDxe is needed.

The Linux kernel/device-tree-derived PMIC inputs are:
- PMK8350 SID0 PON peripheral 0x13, real-time status `0x1310`.
  **GEN3** KPDPWR = bit7 (0x80), RESIN/Volume Down = bit6 (0x40).
- PM6450 SID1 GPIO1 peripheral 0x88, input status `0x8808`,
  active-low bit0 for Volume Up.
- Qualcomm SPMI Arbiter v7 (read on actual dizi as `0x70000000`).
  Observed APIDs 408 (PON) and 432 (GPIO1), on bus0.

The fallback verifies detected PPIDs and issues only read commands through
the observer channel with a bounded timeout. A read command **does write
to the Arbiter observer CMD register**, but never to PMIC data registers.
No Type-C role or PHY changes, no PMIC writes. The menu shows raw state and
read errors. UEFI boots Android/XNU only once real EFI loaders exist;
they are currently missing. USB host remains unresolved.

The native build stamp is `R4-MU-NATIVE`. Build for dizi with:
```
python3 build_uefi.py -d dizi -r RELEASE -c
```
Temporary test only:
```
fastboot boot Mu-dizi.img
```
**Do not use `fastboot flash` on experimental boot or firmware images.**

Source ancestry: `vildangil/dizi_IpadOS_maybe`, branch
`dizi/pmic-spmi-buttons-readonly-20261025`, plus the pinned dizi
device-specific BDS/console fixes from Mu commit
`39fbbc20885325044cfeaf8428efb7267e0bbe35`.
The newer GEN3 key masks have not yet been confirmed in a successful
physical test.
