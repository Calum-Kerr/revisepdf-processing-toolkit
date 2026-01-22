#!/bin/bash
set -e
WASM_DIR="frontend"
SRC_FILE="src/engine-wasm.cpp"
if [ ! -f "$SRC_FILE" ]; then
  echo "Error: $SRC_FILE not found"
  exit 1
fi
echo "Building WASM with emscripten..."
emcc -std=c++17 "$SRC_FILE" -o "$WASM_DIR/algorithms.js" \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s EXPORTED_FUNCTIONS="['_pdf_to_jpg','_pdf_to_pdfa','_compress','_merge','_split','_add_page_numbers','_rotate_pages']" \
  -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']" \
  -O3
echo "WASM build complete"
WASM_FILE="$WASM_DIR/algorithms.wasm"
if [ -f "$WASM_FILE" ]; then
  HASH=$(sha256sum "$WASM_FILE" | awk '{print $1}')
  echo "WASM hash: $HASH"
fi

