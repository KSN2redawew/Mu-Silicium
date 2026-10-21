"""No hardware access: verify DIZI buttons DTS and console build wiring."""
from pathlib import Path
import sys
import tempfile
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from dizi_buttons_preflight import check

ROOT = Path(__file__).resolve().parents[1]


class DiziButtonsTests(unittest.TestCase):
    def test_real_sources_match_spmi_and_gpio_topology(self):
        result = check(ROOT)
        self.assertEqual(result["volume_up"]["controller"], "PM6450 GPIO")
        self.assertTrue(result["volume_up"]["active_low"])
        self.assertFalse(result["suitable_for_release"])

    def test_fail_closed_when_binary_unavailable(self):
        if not (ROOT / "Binaries/dizi/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf").is_file():
            with self.assertRaisesRegex(ValueError, "submodule"):
                check(ROOT, require_binary=True)


if __name__ == "__main__":
    unittest.main()
