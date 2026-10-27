(() => {
  const translations = {
  "Tetris game": "Gra Tetris",
  "High score! Enter your name (letters and numbers):": "Rekord! Podaj imię (litery i cyfry):",
  "Save score": "Zapisz wynik",
  "Game controls": "Sterowanie grą",
  "Back / Save": "Powrót / Zapisz",
  "Up or rotate": "W górę lub obrót",
  "Pause": "Pauza",
  "Move left": "W lewo",
  "Move down": "W dół",
  "Move right": "W prawo",
  "Faster": "Szybciej",
  "Select": "Wybierz",
  "Slower": "Wolniej",
  "Help": "Pomoc",
  "Language": "Język",
  "Use the buttons or arrow keys to move and rotate. Select starts a game; Back returns to the menu.": "Używaj przycisków lub strzałek, aby poruszać i obracać klocki. Wybierz rozpoczyna grę, a Powrót wraca do menu.",
  "Loading game…": "Wczytywanie gry…",
  "The game could not start. Please reload the page.": "Nie udało się uruchomić gry. Odśwież stronę."
};
  const key = 'tetris-language';
  let saved;
  try { saved = localStorage.getItem(key); } catch { /* Storage may be blocked. */ }
  const preferred = (navigator.languages?.length ? navigator.languages : [navigator.language])
    .map(value => value.toLowerCase().split(/[-_]/)[0]).find(value => value === 'pl' || value === 'en');
  let language = saved === 'pl' || saved === 'en' ? saved : preferred || 'en';
  const labels = [...document.querySelectorAll('[aria-label]')].map(element => [element, element.getAttribute('aria-label')]);
  const text = [...document.querySelectorAll('[data-i18n]')].map(element => [element, element.textContent]);
  const languageButtons = document.querySelectorAll('[data-language]');
  function t(source) { return language === 'pl' ? translations[source] || source : source; }
  function render() {
    document.documentElement.lang = language;
    for (const button of languageButtons) button.setAttribute('aria-pressed', String(button.dataset.language === language));
    for (const [element, source] of labels) element.setAttribute('aria-label', t(source));
    for (const [element, source] of text) element.textContent = t(source);
    const status = document.getElementById('status');
    if (status.dataset.message) status.textContent = t(status.dataset.message);
  }
  function setLanguage(value, fromGame = false) {
    if (value !== 'pl' && value !== 'en') return;
    language = value;
    try { localStorage.setItem(key, language); } catch { /* Switching still works. */ }
    render();
    if (!fromGame && window.Module?._web_set_language) window.Module._web_set_language(language === 'pl' ? 1 : 0);
  }
  window.TetrisI18n = { get language() { return language; }, setLanguage, t };
  for (const button of languageButtons) button.addEventListener('click', () => setLanguage(button.dataset.language));
  render();
})();
