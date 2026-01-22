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
    const formData = new FormData();
    formData.append('file', file);
    const response = await fetch(`/api/${tool}`, {
      method: 'POST',
      body: formData
    });
    const result = await response.json();
    resultDiv.innerHTML = `<p>Mode: ${result.mode}</p><p>Status: ${result.status}</p>`;
  } catch (err) {
    resultDiv.innerHTML = `<p>Error: ${err.message}</p>`;
  }
}

