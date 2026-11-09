// Run after build-web.sh with @playwright/test available on NODE_PATH.
const { chromium } = require('@playwright/test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const http = require('node:http');
const path = require('node:path');

const root = path.resolve(__dirname, '../build/web');
const server = http.createServer((req, res) => {
  const pathname = new URL(req.url, 'http://localhost').pathname;
  const file = path.join(root, pathname === '/' ? 'index.html' : pathname);
  res.setHeader('Content-Type', file.endsWith('.wasm') ? 'application/wasm' : file.endsWith('.js') ? 'text/javascript' : file.endsWith('.css') ? 'text/css' : 'text/html');
  fs.createReadStream(file).on('error', () => { res.statusCode = 404; res.end(); }).pipe(res);
});

async function main() {
  await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
  const url = `http://127.0.0.1:${server.address().port}/`;
  const browser = await chromium.launch({ executablePath: process.env.TETRIS_CHROMIUM, args: ['--enable-unsafe-swiftshader'] });
  try {
    const context = await browser.newContext({ locale: 'pl-PL', hasTouch: true, viewport: { width: 390, height: 844 } });
    const page = await context.newPage();
    const errors = [];
    page.on('pageerror', error => errors.push(error.message));
    await page.goto(url);
    await page.waitForFunction(() => !document.querySelector('#controls').disabled);
    assert.equal(await page.locator('#boot').isVisible(), false);
    assert.ok(await page.locator('#led').evaluate(e => e.classList.contains('on')));
    for (const width of [320, 390, 768, 1280]) {
      await page.setViewportSize({ width, height: 900 });
      assert.ok(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth), `Overflow at ${width}px`);
      const geometry = await page.locator('#canvas').boundingBox();
      const textGeometry = await page.locator('#text').boundingBox();
      assert.deepEqual(geometry, textGeometry, 'Game and text must remain aligned');
      assert.equal(geometry.width, geometry.height);
      for (const button of await page.locator('#controls button').all()) {
        const rect = await button.boundingBox();
        assert.ok(rect.width >= 44 && rect.height >= 44, `Touch target too small at ${width}px`);
      }
      if (process.env.TETRIS_EVIDENCE) {
        fs.mkdirSync(process.env.TETRIS_EVIDENCE, { recursive: true });
        await page.screenshot({ path: path.join(process.env.TETRIS_EVIDENCE, `crt-${width}.png`), fullPage: true });
      }
    }
    await page.setViewportSize({ width: 390, height: 844 });
    // Observe real calls while keeping the WASM engine connected.
    await page.evaluate(() => {
      const send = Module._web_key;
      window.keys = [];
      Module._web_key = (key, special) => { keys.push([key, special]); return send(key, special); };
    });
    await page.getByRole('button', { name: 'Wybierz', exact: true }).tap();
    await page.getByRole('button', { name: 'Pauza', exact: true }).tap();
    assert.deepEqual(await page.evaluate(() => keys), [[32, 0], [112, 0]], 'Touch must activate once');
    const pause = page.getByRole('button', { name: 'Pauza', exact: true });
    await pause.focus();
    await page.keyboard.press('Enter');
    await page.keyboard.press('Space');
    assert.equal(await page.evaluate(() => keys.length), 4, 'Keyboard activation must not double-fire');
    const left = page.getByRole('button', { name: 'W lewo', exact: true });
    const rect = await left.boundingBox();
    await page.mouse.move(rect.x + rect.width / 2, rect.y + rect.height / 2);
    await page.mouse.down();
    // A second finger on the same button must not create an orphan repeat timer.
    await left.dispatchEvent('pointerdown', { pointerId: 99, button: 0, pointerType: 'touch' });
    await left.dispatchEvent('pointerup', { pointerId: 99, button: 0, pointerType: 'touch' });
    await page.waitForFunction(() => keys.filter(([key]) => key === 100).length >= 3);
    await page.evaluate(() => window.dispatchEvent(new Event('blur')));
    const stopped = await page.evaluate(() => keys.length);
    await page.waitForTimeout(550);
    assert.equal(await page.evaluate(() => keys.length), stopped, 'Blur must stop held movement');
    await page.mouse.up();
    assert.equal(await left.evaluate(e => e.classList.contains('pressed')), false);
    const scores = 'Ada\n100\nBen\n80\nCez\n60\nDan\n40\nEwa\n20\n';
    await page.evaluate(scores => localStorage.setItem('tetris-results', scores), scores);
    await page.locator('[data-language="en"]').click();
    await page.getByRole('button', { name: 'Restart game', exact: true }).click();
    await page.waitForFunction(() => !document.querySelector('#controls').disabled);
    assert.equal(await page.evaluate(() => document.documentElement.lang), 'en');
    assert.equal(await page.evaluate(() => Module.FS.readFile('wyniki.txt', { encoding: 'utf8' })), scores);
    assert.deepEqual(errors, []);
    await context.close();
    // A failed game download leaves a usable localized retry button.
    const failure = await browser.newContext({ locale: 'pl-PL' });
    const failedPage = await failure.newPage();
    await failedPage.route('**/tetris.js', route => route.abort());
    await failedPage.goto(url);
    await failedPage.getByRole('button', { name: 'Spróbuj ponownie' }).waitFor();
    assert.equal(await failedPage.locator('#controls').evaluate(e => e.disabled), true);
    assert.equal(await failedPage.locator('#led').evaluate(e => e.classList.contains('on')), false);
    await failedPage.unroute('**/tetris.js');
    await failedPage.getByRole('button', { name: 'Spróbuj ponownie' }).click();
    await failedPage.waitForFunction(() => !document.querySelector('#controls').disabled);
    await failure.close();
    // Storage restrictions must not prevent playing or changing the language.
    const privateContext = await browser.newContext();
    await privateContext.addInitScript(() => {
      Storage.prototype.getItem = Storage.prototype.setItem = () => { throw new DOMException('Blocked', 'SecurityError'); };
    });
    const privatePage = await privateContext.newPage();
    await privatePage.goto(url);
    await privatePage.waitForFunction(() => !document.querySelector('#controls').disabled);
    await privatePage.locator('[data-language="pl"]').click();
    assert.equal(await privatePage.evaluate(() => document.documentElement.lang), 'pl');
    await privatePage.getByRole('button', { name: 'Wybierz', exact: true }).click();
    await privateContext.close();
    console.log('CRT: layouts, canvas alignment, touch, keyboard, held input, restart, scores, retry and blocked storage PASS');
  } finally {
    await browser.close();
    server.close();
  }
}
main().catch(error => { console.error(error); server.close(); process.exitCode = 1; });
