let playerId = localStorage.getItem("playerId");
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
    
async function ensurePlayer() {
    if (!playerId) {
        const name = prompt("Enter your name (or leave blank for auto-assigned):");
        const url = name ? `/join?name=${encodeURIComponent(name)}` : '/join';
        const res = await fetch(url);
        playerId = await res.text();
        localStorage.setItem("playerId", playerId);
        console.log("Joined as", playerId);
        document.getElementById("player-id").innerText = `Logged in as: ${playerId}`;
    }
}

ensurePlayer();

// TO UPLOAD THESE FILES, use "pio run --target uploadfs"