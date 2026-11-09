// Keep the native game and browser controls on the same input path.
var Module = {
  canvas: document.getElementById('canvas'),
  preRun: [function () {
    try {
      const saved = localStorage.getItem('tetris-results');
      if (saved !== null) FS.writeFile('wyniki.txt', saved);
    } catch (error) { console.warn('Could not load results:', error); }
  }],
  onRuntimeInitialized: function () {
    document.getElementById('boot').hidden = true;
    document.getElementById('status').textContent = '';
    delete document.getElementById('status').dataset.message;
    document.getElementById('controls').disabled = false;
    document.getElementById('led').classList.add('on');
  },
  onAbort: function () {
    document.getElementById('boot').hidden = false;
    document.getElementById('controls').disabled = true;
    document.getElementById('led').classList.remove('on');
    const status = document.getElementById('status');
    status.dataset.message = 'The game could not start. Please reload the page.';
    status.textContent = TetrisI18n.t(status.dataset.message);
    document.getElementById('retry').hidden = false;
    window.dispatchEvent(new Event('blur'));
  }
};
// GLUT listens on window in capture phase; leave form and button keys to the browser.
for (const type of ['keydown', 'keyup']) {
  window.addEventListener(type, function (event) {
    if (event.target.closest?.('footer, #name-form, .hardware, .boot') ||
        (event.target.closest?.('#controls button') && (event.key === ' ' || event.key === 'Enter'))) {
      event.stopImmediatePropagation();
    }
  }, true);
}
const controls = document.getElementById('controls');
const sendKey = button => { if (!controls.disabled) Module._web_key(Number(button.dataset.key), Number(button.dataset.special || 0)); };
for (const button of controls.querySelectorAll('button')) {
  let delay, repeat, activePointer = null;
  const stop = event => {
    if (event?.pointerId !== undefined && event.pointerId !== activePointer) return;
    clearTimeout(delay);
    clearInterval(repeat);
    activePointer = null;
    button.classList.remove('pressed');
  };
  window.addEventListener('blur', stop);
  window.addEventListener('pagehide', stop);
  document.addEventListener('visibilitychange', () => { if (document.hidden) stop(); });
  button.addEventListener('pointerdown', event => {
    if (event.button !== 0 || controls.disabled || activePointer !== null) return;
    event.preventDefault();
    activePointer = event.pointerId;
    button.classList.add('pressed');
    button.setPointerCapture(event.pointerId);
    sendKey(button);
    if (button.hasAttribute('data-repeat')) {
      delay = setTimeout(() => { repeat = setInterval(() => sendKey(button), 100); }, 350);
    }
  });
  for (const type of ['pointerup', 'pointercancel', 'lostpointercapture']) button.addEventListener(type, stop);
  // Physical pointers are handled above; native keyboard/AT activation uses click.
  button.addEventListener('click', event => { if (event.detail === 0) sendKey(button); });
}
document.getElementById('name-form').addEventListener('submit', event => {
  event.preventDefault();
  const name = document.getElementById('player-name').value.replace(/[^a-zA-Z0-9]/g, '').slice(0, 11);
  Module._web_name_char(0, 1);
  for (const character of name) Module._web_name_char(character.charCodeAt(0), 0);
  Module._web_key(27, 0);
  document.getElementById('player-name').blur();
});

document.getElementById('restart').addEventListener('click', () => location.reload());
document.getElementById('retry').addEventListener('click', () => location.reload());
