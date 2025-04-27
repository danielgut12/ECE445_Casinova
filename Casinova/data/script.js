let playerId = localStorage.getItem("playerId");
let captureCooldown = false;
let isReady = false;

// Poker Stuff
// let lookup = { rank: {}, flush: {} };

// Load lookup.json on page load
// fetch('/lookup.json')
//   .then(response => response.json())
//   .then(data => {
//     lookup = data;
//     console.log('Lookup loaded!');
//     document.getElementById('calculateBtn').disabled = false; // enable button
//   })
//   .catch(error => {
//     console.error('Failed to load lookup.json:', error);
//   });


// Web Sockets
const socket = new WebSocket(`ws://${location.host}/ws`);


// ON SOCKET OPEN (basically boot up)
socket.onopen = function () {
  console.log("WebSocket connected");


  socket.send(JSON.stringify({ type: "identify", id: playerId }));
  // After connection, fetch players manually once
  fetchPlayers();
  fetchGamestate();
  // fetchHand();
};


// on socket update (whenever server pushes new update)
socket.onmessage = function(event) {
  const data = JSON.parse(event.data);

  console.log("WebSocket message:", data); // Always good for debugging

  if (data.players) {
    const ul = document.getElementById("players-ul");
    ul.innerHTML = "";
    data.players.forEach(p => {
      const li = document.createElement("li");
      li.textContent = `${p.id} ${p.ready ? "Ready" : ""}`;
      ul.appendChild(li);
    });
  }

  if (data.event === "newHand") {
    console.log("Received new hand event!");
    fetchHand();  // fetch your updated hand automatically
  }

  if (data.event === "winner") {
    console.log("Received winner event!");
    const winnerArea = document.getElementById("winner-announcement");
    winnerArea.innerText = `${data.winner} wins the hand!`;
    winnerArea.style.display = "block";
  }

  if (data.phase && data.community) {
    console.log("Received game state update!");
    console.log("Current Phase:", data.phase);
    console.log("Community Cards:", data.community);

    document.getElementById("phase").textContent = `Next Phase: ${data.phase}`;

    const communityDiv = document.getElementById("community-cards");
    communityDiv.innerHTML = "";
    data.community.forEach(card => {
      const cardElem = document.createElement("div");
      cardElem.textContent = card;
      communityDiv.appendChild(cardElem);
    });

    if (data.phase === "Waiting") {
      document.getElementById("winner-announcement").innerText = "";
      document.getElementById("winner-announcement").style.display = "none";
    }
  }

  if (data.type === "distance") {
    console.log("TOF Distance:", data.distance, "mm");

    // Update the page if you want
    const distanceLabel = document.getElementById("distance-label");
    if (distanceLabel) {
      distanceLabel.textContent = `Distance: ${data.distance.toFixed(2)} mm`;
    }
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
      // .then(msg => alert(msg));
}


function autoEject() {
  fetch('/autoEject')
      .then(response => response.text())
      // .then(msg => alert(msg));
}

function queueCard() {
  fetch('/queueCard')
      .then(response => response.text())
      // .then(msg => alert(msg));
}


function rotate() {
  const angle = document.getElementById('angleInput').value;
  fetch('/rotate?degrees=' + angle)
      .then(response => response.text())
      // .then(msg => alert(msg));
}
    
async function ensurePlayer() {
  playerID = localStorage.getItem("playerId");

  if (playerId) {
    // Reuse existing playerId by sending it as a name
    const url = `/join?name=${encodeURIComponent(playerId)}`;
    const res = await fetch(url);
    isReady = false;

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


// unused
// function sendAction(action) {
//   fetch('/action', {
//     method: 'POST',
//     headers: { "Content-Type": "application/json" },
//     body: JSON.stringify({ id: playerId, action: action })
//   })
//   .then(res => res.text())
//   .then(msg => {
//     console.log("Action result:", msg);
//     alert(msg);
//   })
//   .catch(err => {
//     console.error("Action failed:", err);
//     alert("Failed to send action.");
//   });
// }

function advanceGame() {
  fetch('/nextPhase')
    .then(res => res.text())
    .then(msg => {
      const phaseStatus = document.getElementById('phase-status');

      // If waiting for players, display, else leave empty
      if (msg.trim() === "WAITING") {
        phaseStatus.innerText = "Waiting for players...";
      } else {
        phaseStatus.innerText = "";
      }
    })
    .catch(err => {
      console.error('Failed to advance phase:', err);
      alert("Error advancing phase.");
    });
}

function toggleReady(btn) {
  // btn.disabled = true;
  
  fetch("/ready", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ id: playerId, ready: !isReady })  // send the *next* state
  })
  .then(res => res.text())
  .then(msg => {
    isReady = !isReady;  // Flip state only after success
    btn.innerText = isReady ? "Ready" : "I'm Ready";
    btn.disabled = false;
    // alert(msg);
  })
  .catch(err => {
    btn.disabled = false;
    console.error("Ready failed:", err);
    alert("Failed to toggle ready.");
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
        li.textContent = `${p.id} ${p.ready ? "Ready" : ""}`;
        ul.appendChild(li);
      });
    })
    .catch(err => console.error("Failed to fetch players", err));
}

function fetchHand() {
  fetch(`/hand?id=${encodeURIComponent(playerId)}`)
    .then(res => res.json())
    .then(data => {
      console.log("[DEBUG] Hand data:", data); 
      const handDiv = document.getElementById("hand");
      handDiv.innerHTML = "";

      if (data.card1) {
        const span1 = document.createElement("span");
        span1.textContent = data.card1;
        handDiv.appendChild(span1);
      }
      if (data.card2) {
        const span2 = document.createElement("span");
        span2.textContent = " " + data.card2;
        handDiv.appendChild(span2);
      }
    })
    .catch(err => {
      console.error("Failed to fetch hand:", err);
    });
}

function fold() {
  // const playerId = document.getElementById("player-id").innerText.replace("Logged in as ", "").trim();
  console.log("Attempting to fold player:", playerId);

  fetch("/fold", {
    method: "POST",
    headers: { "Content-Type": "application/x-www-form-urlencoded" },
    body: `id=${encodeURIComponent(playerId)}`
  })
  .then(res => res.text())
  .then(msg => {
    console.log("Fold response:", msg);
    // alert("You folded!");
  })
  .catch(err => {
    console.error("Error folding:", err);
    alert("Fold failed!");
  });
}



window.addEventListener("DOMContentLoaded", () => {
  ensurePlayer();
  // setInterval(fetchGamestate, 2000);

  document.getElementById("captureBtn")?.addEventListener("click", capture);
  document.getElementById("ejectBtn")?.addEventListener("click", eject);
  document.getElementById("autoEjectBtn")?.addEventListener("click", autoEject);
  document.getElementById("queueCardBtn")?.addEventListener("click", queueCard);
  document.getElementById("rotateBtn")?.addEventListener("click", rotate);
  document.getElementById("advanceGameBtn")?.addEventListener("click", advanceGame);
  document.getElementById("foldBtn")?.addEventListener("click", fold);
  document.getElementById("readyBtn")?.addEventListener("click", toggleReady);
});

// TO UPLOAD THESE FILES, use "pio run --target uploadfs"