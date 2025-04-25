let playerId = localStorage.getItem("playerId");
let captureCooldown = false;


// Web Sockets
const socket = new WebSocket(`ws://${location.host}/ws`);


// ON SOCKET OPEN (basically boot up)
socket.onopen = function () {
  console.log("WebSocket connected");

  // After connection, fetch players manually once
  fetchPlayers();
};


// on socket update (whenever server pushes new update)
socket.onmessage = function (event) {
  const data = JSON.parse(event.data);
  if (data.players) {
    const ul = document.getElementById("players-ul");
    ul.innerHTML = "";
    data.players.forEach(p => {
      const li = document.createElement("li");
      li.textContent = `${p.id} ${p.ready ? "ready" : "not ready"}`;
      ul.appendChild(li);
    });
  }
};



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
      setTimeout(() => captureCooldown = false, 300);  // 500ms cooldown
    });
    console.log("hello");
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
  playerID = localStorage.getItem("playerId");

  if (playerId) {
    // Reuse existing playerId by sending it as a name
    const url = `/join?name=${encodeURIComponent(playerId)}`;
    const res = await fetch(url);

    if (res.status === 200) {
      playerId = await res.text();
      localStorage.setItem("playerId", playerId);
      console.log("Rejoined as", playerId);
    } else {
      console.warn("Join failed, clearing local storage...");
      localStorage.removeItem("playerId");
      playerId = null;
    }
  }

  if (!playerId) {
      const name = prompt("Enter your name (or leave blank for auto-assigned):");
      const url = name ? `/join?name=${encodeURIComponent(name)}` : '/join';
      const res = await fetch(url);
      playerId = await res.text();
      localStorage.setItem("playerId", playerId);
      console.log("Joined as", playerId);
  }

  const idElem = document.getElementById("player-id");
  if (idElem) {
      idElem.innerText = `Logged in as: ${playerId}`;
  }
}

function fetchGamestate() {
  fetch('/gamestate')
    .then(res => res.json())
    .then(data => {
      const cardsDiv = document.getElementById('cards');
      cardsDiv.innerHTML = "";  // clear previous cards
      data.community.forEach(card => {
        const span = document.createElement("span");
        span.textContent = card + " ";
        span.style.marginRight = "10px";
        cardsDiv.appendChild(span);
      });
    });
}

function sendAction(action) {
  fetch('/action', {
    method: 'POST',
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ id: playerId, action: action })
  })
  .then(res => res.text())
  .then(msg => {
    console.log("Action result:", msg);
    alert(msg);
  })
  .catch(err => {
    console.error("Action failed:", err);
    alert("Failed to send action.");
  });
}

function advanceGame() {
  fetch('/nextPhase')
    .then(res => res.text())
    .then(msg => {
      document.getElementById('phase-status').innerText = msg;
    })
    .catch(err => {
      console.error('Failed to advance phase:', err);
      alert("Error advancing phase.");
    });
}

function sendReady() {
  const btn = document.getElementById("readyBtn");
  btn.disabled = true;
  btn.innerText = "Waiting...";

  fetch("/ready", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ id: playerId })
  })
  .then(res => res.text())
  .then(msg => {
    btn.innerText = "Ready";
    alert(msg);
  })
  .catch(err => {
    btn.disabled = false;
    btn.innerText = "I'm Ready";
    console.error("Ready failed:", err);
    alert("Failed to mark as ready.");
  });
}

function fetchPlayers() {
  fetch("/players")
    .then(res => res.json())
    .then(data => {
      const ul = document.getElementById("players-ul");
      ul.innerHTML = "";
      data.players.forEach(p => {
        const li = document.createElement("li");
        li.textContent = `${p.id} ${p.ready ? "yes" : "no"}`;
        ul.appendChild(li);
      });
    })
    .catch(err => console.error("Failed to fetch players", err));
}



window.addEventListener("DOMContentLoaded", () => {
  console.log("thisran1")
  ensurePlayer();
  console.log("thisran2")
  // setInterval(fetchGamestate, 2000);

  document.getElementById("captureBtn")?.addEventListener("click", capture);
  document.getElementById("ejectBtn")?.addEventListener("click", eject);
  document.getElementById("rotateBtn")?.addEventListener("click", rotate);
  document.getElementById("advanceGameBtn")?.addEventListener("click", advanceGame);
  document.getElementById("checkBtn")?.addEventListener("click", () => sendAction("check"));
  document.getElementById("raiseBtn")?.addEventListener("click", () => sendAction("raise"));
  document.getElementById("foldBtn")?.addEventListener("click", () => sendAction("fold"));
  document.getElementById("readyBtn")?.addEventListener("click", sendReady);


});

// TO UPLOAD THESE FILES, use "pio run --target uploadfs"