#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p build/readability
cp web/index.html web/i18n.js build/readability/
em++ scripts/readability-fixture.cpp -O2 -sLEGACY_GL_EMULATION=1 -sGL_FFP_ONLY=1 -sEXPORTED_RUNTIME_METHODS=FS -o build/readability/tetris.js
