// Run against the separate readability-fixture.cpp build, never deployed to Pages.
const { chromium } = require("@playwright/test");
const { createRequire } = require("node:module");
const path = require("node:path");
const coreRequire = createRequire(
  createRequire(require.resolve("@playwright/test")).resolve("playwright"),
);
const { PNG } = require(
  path.join(
    path.dirname(coreRequire.resolve("playwright-core/package.json")),
    "lib/utilsBundle.js",
  ),
);
const fs = require("node:fs");
const http = require("node:http");
const assert = require("node:assert/strict");
const root = path.resolve(__dirname, "../build/readability");
const server = http.createServer((req, res) => {
  const file = path.join(
    root,
    new URL(req.url, "http://localhost").pathname === "/"
      ? "index.html"
      : new URL(req.url, "http://localhost").pathname,
  );
  res.setHeader(
    "Content-Type",
    file.endsWith(".wasm")
      ? "application/wasm"
      : file.endsWith(".js")
        ? "text/javascript"
        : "text/html",
  );
  fs.createReadStream(file)
    .on("error", () => {
      res.statusCode = 404;
      res.end();
    })
    .pipe(res);
});
function lum(rgb) {
  return rgb
    .map((x) => {
      x /= 255;
      return x <= 0.04045 ? x / 12.92 : ((x + 0.055) / 1.055) ** 2.4;
    })
    .reduce((s, v, i) => s + v * [0.2126, 0.7152, 0.0722][i], 0);
}
function contrast(a, b) {
  let x = lum(a),
    y = lum(b);
  return (Math.max(x, y) + 0.05) / (Math.min(x, y) + 0.05);
}
function histogram(png, left, right) {
  const m = new Map();
  for (let y = 20; y < png.height - 20; y++)
    for (let x = left; x < right; x++) {
      const i = (y * png.width + x) * 4;
      const c = [...png.data.slice(i, i + 3)];
      const k = c.join(",");
      m.set(k, (m.get(k) || 0) + 1);
    }
  return m;
}
(async () => {
  await new Promise((r) => server.listen(0, "127.0.0.1", r));
  const browser = await chromium.launch({
    executablePath: process.env.TETRIS_CHROMIUM,
    args: ["--enable-unsafe-swiftshader"],
  });
  try {
    for (const mobile of [false, true]) {
      const page = await browser.newPage({
        viewport: { width: mobile ? 390 : 1280, height: mobile ? 844 : 1000 },
        isMobile: mobile,
        hasTouch: mobile,
      });
      const errors = [];
      page.on("pageerror", (e) => errors.push(e.message));
      await page.goto(`http://127.0.0.1:${server.address().port}`);
      await page.waitForFunction(
        () => !document.querySelector("#controls").disabled,
      );
      const colors = [
        [0, 255, 255],
        [255, 102, 102],
        [255, 255, 0],
        [255, 102, 255],
        [0, 255, 0],
        [64, 102, 255],
        [128, 128, 128],
      ];
      const bgs = [
        [0, 26, 0],
        [0, 0, 26],
        [26, 26, 0],
        [26, 0, 26],
      ];
      for (let code = 0; code < 7; code++) {
        await page.evaluate(
          (code) => Module._readability_fixture(code, 22),
          code,
        );
        await page.waitForFunction(() =>
          document
            .querySelector("#text")
            .getContext("2d")
            .getImageData(335, 160, 190, 20)
            .data.some((v, i) => i % 4 === 3 && v),
        );
        await page
          .locator("#text")
          .evaluate((e) => (e.style.visibility = "hidden"));
        const png = PNG.sync.read(await page.locator("#canvas").screenshot());
        await page.locator("#text").evaluate((e) => (e.style.visibility = ""));
        const expected = colors[code];
        for (const [left, right] of [
          [20, Math.floor(png.width / 2)],
          [Math.ceil(png.width / 2), png.width - 20],
        ]) {
          const h = histogram(png, left, right);
          assert.ok(
            (h.get(expected.join(",")) || 0) > 100,
            `figure ${code} missing in ${left}-${right}`,
          );
        }
        for (const bg of bgs)
          assert.ok(contrast(expected, bg) >= 3, `figure ${code} contrast`);
        // Pause strip in actual render, away from text: must be neutral white.
        const x = Math.round((png.width * 340) / 600),
          y = Math.round((png.height * 165) / 600),
          i = (y * png.width + x) * 4;
        assert.ok(
          png.data[i] > 240 && png.data[i + 1] > 240 && png.data[i + 2] > 240,
          "pause plaque is not white",
        );
        if (process.env.TETRIS_EVIDENCE) {
          fs.mkdirSync(process.env.TETRIS_EVIDENCE, { recursive: true });
          for (const gray of [false, true]) {
            await page
              .locator(".game")
              .evaluate(
                (e, g) => (e.style.filter = g ? "grayscale(1)" : ""),
                gray,
              );
            const rendered = PNG.sync.read(
              await page
                .locator(".game")
                .screenshot({
                  path: path.join(
                    process.env.TETRIS_EVIDENCE,
                    `${mobile ? "phone" : "desktop"}-figure${code + 1}-${gray ? "gray" : "color"}.png`,
                  ),
                }),
            );
            if (gray) {
              const values = [expected, ...bgs].map((rgb) => {
                for (let i = 0; i < png.data.length; i += 4)
                  if (rgb.every((v, j) => png.data[i + j] === v))
                    return [...rendered.data.slice(i, i + 3)];
                throw new Error("missing rendered palette");
              });
              for (const bg of values.slice(1))
                assert.ok(
                  contrast(values[0], bg) >= 3,
                  `grayscale contrast figure ${code}`,
                );
              console.log(
                `figure${code + 1} min color ${Math.min(...bgs.map((bg) => contrast(expected, bg))).toFixed(2)}, gray ${Math.min(...values.slice(1).map((bg) => contrast(values[0], bg))).toFixed(2)}`,
              );
            }
          }
          await page.locator(".game").evaluate((e) => (e.style.filter = ""));
        }
      }
      for (const language of ["pl", "en"]) {
        await page
          .locator(`footer button[data-language="${language}"]`)
          .click();
        await page.evaluate(() => Module._readability_fixture(5, 22));
        const text = await page.locator("#text").screenshot();
        assert.ok(text.length > 1000);
      }
      await page.evaluate(() => Module._readability_fixture(0, 11));
      await page.locator("#canvas").focus();
      await page.keyboard.press("Escape");
      await page.keyboard.press("Space");
      await page.keyboard.press("p");
      await page.keyboard.press("p");
      for (const width of [320, 390, 1440]) {
        await page.setViewportSize({ width, height: 900 });
        assert.ok(
          await page.evaluate(
            () => document.documentElement.scrollWidth <= innerWidth,
          ),
        );
      }
      assert.deepEqual(errors, []);
      console.log(
        `${mobile ? "touch" : "desktop"}: seven current/preview shapes, pause, contrast, layouts PASS`,
      );
      await page.close();
    }
  } finally {
    await browser.close();
    server.close();
  }
})().catch((e) => {
  console.error(e);
  server.close();
  process.exitCode = 1;
});
