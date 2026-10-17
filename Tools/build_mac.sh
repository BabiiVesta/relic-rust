#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 2 ]]; then
  echo "Usage: UE_ROOT=/path/to/UE_5.x ./Tools/build_mac.sh /path/to/Project.uproject ProjectEditor" >&2
  exit 2
fi

PROJECT="$1"
TARGET="$2"
if [[ -z "${UE_ROOT:-}" ]]; then
  echo "Set UE_ROOT to your Unreal Engine installation directory." >&2
  exit 2
fi
BUILD_SCRIPT="$UE_ROOT/Engine/Build/BatchFiles/Mac/Build.sh"
if [[ ! -x "$BUILD_SCRIPT" ]]; then
  echo "Build script not found: $BUILD_SCRIPT" >&2
  exit 2
fi
if [[ ! -f "$PROJECT" ]]; then
  echo "Project file not found: $PROJECT" >&2
  exit 2
fi
"$BUILD_SCRIPT" "$TARGET" Mac Development "-project=$PROJECT" -waitmutex
