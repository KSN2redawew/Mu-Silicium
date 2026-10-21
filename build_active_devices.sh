#!/usr/bin/env bash
# DIZI-only compatibility wrapper. Do not parse the historical monorepo
# Status.md and accidentally build/publish unrelated hardware images.
set -euo pipefail

MODE=RELEASE
SECUREBOOT=0

usage() {
    echo "Usage: $0 [-r DEBUG|RELEASE] [-s|--enable-secureboot]"
    echo "Build only Mu-dizi firmware. Experimental, NOT flash validated."
}

while (( $# > 0 )); do
    case "$1" in
        -r|--release)
            if (( $# < 2 )); then usage >&2; exit 2; fi
            MODE="${2^^}"
            shift 2
            ;;
        -s|--enable-secureboot)
            SECUREBOOT=1
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            usage >&2
            exit 2
            ;;
    esac
done

case "$MODE" in
    DEBUG|RELEASE) ;;
    *) echo "Invalid mode: $MODE" >&2; exit 2 ;;
esac

args=(-d dizi -r "$MODE" -c)
if (( SECUREBOOT )); then
    args+=(-s)
fi
exec python3 build_uefi.py "${args[@]}"
