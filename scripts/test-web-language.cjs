// Run after build-web.sh with @playwright/test available on NODE_PATH.
const { chromium } = require('@playwright/test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const { createRequire } = require('node:module');
const coreRequire = createRequire(createRequire(require.resolve('@playwright/test')).resolve('playwright'));
const { PNG } = require(require('node:path').join(require('node:path').dirname(coreRequire.resolve('playwright-core/package.json')), 'lib/utilsBundle.js'));
const http = require('node:http');
const path = require('node:path');

const root = path.resolve(__dirname, '../build/web');
const server = http.createServer((req, res) => {
  const pathname = new URL(req.url, 'http://localhost').pathname;
  const file = path.join(root, pathname === '/' ? 'index.html' : pathname);
  res.setHeader('Content-Type', file.endsWith('.wasm') ? 'application/wasm' : file.endsWith('.js') ? 'text/javascript' : 'text/html');
  fs.createReadStream(file).on('error', () => { res.statusCode = 404; res.end(); }).pipe(res);
});

async function main() {
  await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
  const browser = await chromium.launch({ executablePath: process.env.TETRIS_CHROMIUM, args: ['--enable-unsafe-swiftshader'] });
  try {
    for (const locale of ['en-GB', 'pl-PL', 'fr-FR']) {
      const context = await browser.newContext({ locale, viewport: { width: 390, height: 844 } });
      const page = await context.newPage();
      const errors = [];
      page.on('pageerror', error => errors.push(error.message));
      await page.addInitScript(() => {
        const original = CanvasRenderingContext2D.prototype.fillText;
        CanvasRenderingContext2D.prototype.fillText = function (text, ...args) {
          if (this.canvas.id === 'text') {
            window.gameText ||= [];
            window.gameText.push(text);
            if (window.gameText.length > 300) window.gameText.splice(0, 200);
          }
          return original.call(this, text, ...args);
        };
      });
      await page.goto(`http://127.0.0.1:${server.address().port}/`);
      await page.waitForFunction(() => !document.querySelector('#controls').disabled);
      const initial = locale.startsWith('pl') ? 'pl' : 'en';
      const other = initial === 'pl' ? 'en' : 'pl';
      const languageButton = code => page.locator(`footer button[data-language="${code}"]`);
      assert.equal(await languageButton(initial).getAttribute("aria-pressed"), "true");
      assert.equal(await page.evaluate(() => localStorage.getItem('tetris-language')), null,
        'An inferred browser language must not be saved as a manual choice');
      await page.waitForFunction(text => window.gameText?.includes(text), initial === 'pl' ? 'Nowa Gra' : 'New Game');
      await page.getByRole('button', { name: initial === 'pl' ? 'Wybierz' : 'Select', exact: true }).click();
      await page.getByRole('button', { name: initial === 'pl' ? 'Pauza' : 'Pause', exact: true }).click();
      await page.waitForFunction(text => window.gameText?.slice(-25).some(value => value.startsWith(text)), initial === 'pl' ? 'Pauza' : 'Pause');
      const pausePixels = PNG.sync.read(await page.locator('#canvas').screenshot());
      const plaqueOffset = (Math.round(pausePixels.height * 181 / 600) * pausePixels.width + Math.round(pausePixels.width * 340 / 600)) * 4;
      assert.ok([0, 1, 2].every(channel => pausePixels.data[plaqueOffset + channel] > 240), 'Pause plaque must be white in the rendered game');
      // Element screenshots also include the text canvas layered above the board.
      await page.locator('#text').evaluate(element => { element.style.visibility = 'hidden'; });
      const pausedBoard = await page.locator('#canvas').screenshot();
      await languageButton(other).click();
      await page.waitForFunction(text => window.gameText?.slice(-25).includes(text), other === 'pl' ? 'Punkty' : 'Points');
      assert.ok(pausedBoard.equals(await page.locator('#canvas').screenshot()), 'Language switching changed the paused board');
      await page.locator('#canvas').focus();
      await page.keyboard.press('l');
      assert.equal(await languageButton(initial).getAttribute('aria-pressed'), 'true', 'The L key did not update the footer language links');
      await languageButton(other).click();
      await page.reload();
      await page.waitForFunction(() => !document.querySelector('#controls').disabled);
      assert.equal(await languageButton(other).getAttribute("aria-pressed"), "true");
      await page.waitForFunction(text => window.gameText?.includes(text), other === 'pl' ? 'Nowa Gra' : 'New Game');
      for (const width of [320, 390, 1440]) {
        await page.setViewportSize({ width, height: 900 });
        assert.ok(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth), `Overflow at ${width}px`);
      }
      assert.deepEqual(errors, []);
      console.log(`${locale}: browser preference, native menu, live switching, L key, persistence and layouts passed`);
      await context.close();
    }
  } finally {
    await browser.close();
    server.close();
  }
}
main().catch(error => { console.error(error); server.close(); process.exitCode = 1; });
