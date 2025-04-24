let captureCooldown = false;

function capture() {
  if (captureCooldown) return;
  captureCooldown = true;

  fetch('/capture')
    .then(res => res.blob())
    .then(blob => {
      const img = document.getElementById("snapshot");
      img.src = URL.createObjectURL(blob);
      img.style.transform = "rotate(270deg)"
      img.style.transformOrigin = "center"
    })
    .finally(() => {
      setTimeout(() => captureCooldown = false, 500);  // 500ms cooldown
    });
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