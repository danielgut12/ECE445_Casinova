function capture() {
  const img = document.getElementById('snapshot');
  img.src = '/capture?t=' + new Date().getTime(); // prevent caching
}

function eject() {
  const dist = document.getElementById('distanceInput').value;
  fetch('/eject?distance=' + dist)
      .then(response => response.text())
      .then(msg => alert(msg));
}

function rotate() {
  const angle = document.getElementById('angleInput').value;
  fetch('/rotate?degrees=' + angle)
      .then(response => response.text())
      .then(msg => alert(msg));
}


// TO UPLOAD THESE FILES, use "pio run --target uploadfs"