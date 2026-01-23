if ('serviceWorker' in navigator) {
  navigator.serviceWorker.register('/service-worker.js').catch(err => console.log('SW registration failed:', err));
}
document.querySelectorAll('.tool-btn').forEach(btn => {
  btn.addEventListener('click', () => {
    document.querySelectorAll('.tool-btn').forEach(b => b.classList.remove('active'));
    document.querySelectorAll('.tool-section').forEach(s => s.classList.remove('active'));
    btn.classList.add('active');
    const tool = btn.dataset.tool;
    document.getElementById(tool).classList.add('active');
  });
});
document.querySelectorAll('.upload-area').forEach(area => {
  area.addEventListener('click', () => {
    const tool = area.id.replace('upload-', '');
    document.getElementById(`file-${tool}`).click();
  });
  area.addEventListener('dragover', (e) => {
    e.preventDefault();
    area.classList.add('drag-over');
  });
  area.addEventListener('dragleave', () => {
    area.classList.remove('drag-over');
  });
  area.addEventListener('drop', (e) => {
    e.preventDefault();
    area.classList.remove('drag-over');
    const files = e.dataTransfer.files;
    if (files.length > 0) {
      const tool = area.id.replace('upload-', '');
      document.getElementById(`file-${tool}`).files = files;
      handleFileUpload(tool, files[0]);
    }
  });
});
document.querySelectorAll('input[type="file"]').forEach(input => {
  input.addEventListener('change', (e) => {
    const tool = e.target.id.replace('file-', '');
    if (e.target.files.length > 0) {
      handleFileUpload(tool, e.target.files[0]);
    }
  });
});
async function handleFileUpload(tool, file) {
  const resultDiv = document.getElementById(`result-${tool}`);
  resultDiv.innerHTML = '<p>Processing...</p>';
  resultDiv.classList.add('active');
  try {
    let result = null;
    if (window.Module && window.Module.ccall) {
      try {
        const fileData = await file.arrayBuffer();
        const wasmResult = window.Module.ccall(tool, 'string', ['array', 'number'], [new Uint8Array(fileData), fileData.byteLength]);
        result = JSON.parse(wasmResult);
      } catch (wasmErr) {
        console.log('WASM failed, falling back to API:', wasmErr);
      }
    }
    if (!result) {
      const response = await fetch(`/api/${tool}`, {
        method: 'POST',
        body: file
      });
      const contentType = response.headers.get('content-type') || '';
      if (contentType.includes('application/pdf') || contentType.includes('image/jpeg')) {
        const blob = await response.blob();
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        const filename = response.headers.get('content-disposition')?.split('filename=')[1]?.replace(/"/g, '') || `${tool}-output`;
        a.download = filename;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
        resultDiv.innerHTML = `<p>Downloaded: ${filename}</p>`;
        return;
      } else {
        result = await response.json();
      }
    }
    resultDiv.innerHTML = `<p>Mode: ${result.mode}</p><p>Status: ${result.status}</p>`;
  } catch (err) {
    resultDiv.innerHTML = `<p>Error: ${err.message}</p>`;
  }
}

