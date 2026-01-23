$ErrorActionPreference = "Stop"
$wasmDir = "frontend"
$srcFile = "src/engine-wasm.cpp"
if (-not (Test-Path $srcFile)) {
  Write-Host "Error: $srcFile not found"
  exit 1
}
Write-Host "Building WASM with emscripten..."
emcc -std=c++17 $srcFile -o "$wasmDir/algorithms.js" `
  -s WASM=1 `
  -s ALLOW_MEMORY_GROWTH=1 `
  -s EXPORTED_FUNCTIONS="['_pdf_to_jpg','_pdf_to_pdfa','_compress','_merge','_split','_add_page_numbers','_rotate_pages']" `
  -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']" `
  -O3
if ($LASTEXITCODE -ne 0) {
  Write-Host "WASM build failed"
  exit 1
}
Write-Host "WASM build complete"
$wasmFile = "$wasmDir/algorithms.wasm"
if (Test-Path $wasmFile) {
  $hash = (Get-FileHash $wasmFile -Algorithm SHA256).Hash
  Write-Host "WASM hash: $hash"
}

