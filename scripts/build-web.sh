#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p build/web
cp web/index.html build/web/index.html
em++ cpp/tetris.cpp -O2 -sLEGACY_GL_EMULATION=1 -sGL_FFP_ONLY=1 -sEXPORTED_RUNTIME_METHODS=FS -o build/web/tetris.js
