async function update() {
  try {
      const res = await fetch('/distance');
      const mm = await res.text();
      document.getElementById('dist').innerText = mm + ' mm';
  } catch (e) {
      document.getElementById('dist').innerText = 'error';
  }
}

setInterval(update, 500);
window.onload = update;
