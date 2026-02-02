#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <executable> [args...]"
  exit 1
fi

exe="$1"
shift || true

if [[ ! -x "$exe" ]]; then
  echo "Error: '$exe' is not executable"
  exit 1
fi

if ! command -v xterm >/dev/null 2>&1; then
  echo "Error: xterm not found. Install it with:"
  echo "  sudo apt install xterm xfonts-base"
  exit 1
fi

# Build a safely-quoted command string for bash -lc.
quote() { printf "%q" "$1"; }
cmd="$(quote "$exe")"
for a in "$@"; do
  cmd+=" $(quote "$a")"
done
cmd+="; echo; read -n1 -s -r -p 'Press any key to close...'"

exec xterm -fa "Monospace" -fs 12 -e bash -lc "$cmd"
