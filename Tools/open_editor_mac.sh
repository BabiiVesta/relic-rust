#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
  echo "Usage: UE_ROOT=/path/to/UE_5.8 ./Tools/open_editor_mac.sh /path/to/RelicRust.uproject" >&2
  exit 2
fi

PROJECT="$1"
EDITOR="${UE_ROOT:-}/Engine/Binaries/Mac/UnrealEditor"
if [[ ! -x "$EDITOR" ]]; then
  echo "Set UE_ROOT to the Unreal Engine installation directory; editor not found: $EDITOR" >&2
  exit 2
fi
if [[ ! -f "$PROJECT" ]]; then
  echo "Project file not found: $PROJECT" >&2
  exit 2
fi
"$EDITOR" "$PROJECT" -log
