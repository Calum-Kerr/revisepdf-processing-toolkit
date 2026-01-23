$tools = @('convert', 'compress', 'merge', 'split', 'rotate')
foreach ($tool in $tools) {
  Write-Host "Testing /api/$tool..."
  try {
    $response = Invoke-WebRequest -Uri "http://localhost:8080/api/$tool" -Method Post -InFile 'test.pdf' -ContentType 'application/pdf' -UseBasicParsing -ErrorAction Stop
    $json = $response.Content | ConvertFrom-Json
    Write-Host "  Status: $($json.status)"
    Write-Host "  Mode: $($json.mode)"
  } catch {
    Write-Host "  Error: $_"
  }
}

