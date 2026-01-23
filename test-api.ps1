$response = Invoke-WebRequest -Uri 'http://localhost:8080/api/compress' -Method Post -InFile 'test.pdf' -ContentType 'application/pdf' -UseBasicParsing
Write-Host $response.Content

