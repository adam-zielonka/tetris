# Readability verification — 2026-10-29 UTC

Implementation SHA: f0fb4b79027c58292a2954af72ce1e09932fb8ca. No package version is declared.
Chromium 156.0.8078.4 headless, real WebGL/WASM rendering, viewport 1280×1000
and 390×844 with touch. Existing dark theme only. Screenshots crop the game.
CSS `filter: grayscale(1)` is a browser simulation, not a physical panel test.
All data is synthetic (empty game, default anonymous high score).

`production-*` views use the ordinary production build with normal random
selection. The other 28 images use a **separate test-only WASM fixture** which
selects a known current/next figure and pause state, using the unchanged game
renderer. The fixture is not included in the Pages artifact. Four cells and
existing shapes are retained; no production RNG, collision or input changes.

Runtime checks: menu/new game/pause/resume/game over, keyboard arrows/Space/P,
phone controls, live PL/EN and preserved paused board; no JS errors or horizontal
overflow at 320/390/1440px. Native g++ build passed with existing GL sysroot;
native GUI NOT verified: no DISPLAY/Xvfb, freeglut failed to open display.
No host packages were installed. Browser languages en-GB/pl-PL/fr-FR passed.

Rendered palette minimum contrast to all four board/preview backgrounds:

| Figure | Color | CSS grayscale |
| --- | ---: | ---: |
| Square / cyan | 14.05 | 10.72 |
| Line / red | 6.16 | 4.94 |
| L / yellow | 16.41 | 15.17 |
| Mirrored L / magenta | 7.21 | 5.71 |
| Z / green | 12.84 | 8.76 |
| Mirrored Z / blue | 3.83 | 3.23 |
| Nose / gray | 4.46 | 4.50 |

Pause is black text on white, 21:1. Red and magenta also needed lightening:
their original color contrast passed, but their actual grayscale render did not.
Other text uses of palette 6 retain readable black text on the lighter blue.
This is a bounded regression check, not a full WCAG certification.

## Production pause

![Production desktop, en, color](desktop-production-en-color.png)

![Production desktop, en, gray](desktop-production-en-gray.png)

![Production desktop, pl, color](desktop-production-pl-color.png)

![Production desktop, pl, gray](desktop-production-pl-gray.png)

![Production phone, en, color](phone-production-en-color.png)

![Production phone, en, gray](phone-production-en-gray.png)

![Production phone, pl, color](phone-production-pl-color.png)

![Production phone, pl, gray](phone-production-pl-gray.png)

## All seven current / next shapes (test fixture)

### Figure 1

![Fixture figure 1, desktop, color](desktop-figure1-color.png)

![Fixture figure 1, desktop, gray](desktop-figure1-gray.png)

![Fixture figure 1, phone, color](phone-figure1-color.png)

![Fixture figure 1, phone, gray](phone-figure1-gray.png)

### Figure 2

![Fixture figure 2, desktop, color](desktop-figure2-color.png)

![Fixture figure 2, desktop, gray](desktop-figure2-gray.png)

![Fixture figure 2, phone, color](phone-figure2-color.png)

![Fixture figure 2, phone, gray](phone-figure2-gray.png)

### Figure 3

![Fixture figure 3, desktop, color](desktop-figure3-color.png)

![Fixture figure 3, desktop, gray](desktop-figure3-gray.png)

![Fixture figure 3, phone, color](phone-figure3-color.png)

![Fixture figure 3, phone, gray](phone-figure3-gray.png)

### Figure 4

![Fixture figure 4, desktop, color](desktop-figure4-color.png)

![Fixture figure 4, desktop, gray](desktop-figure4-gray.png)

![Fixture figure 4, phone, color](phone-figure4-color.png)

![Fixture figure 4, phone, gray](phone-figure4-gray.png)

### Figure 5

![Fixture figure 5, desktop, color](desktop-figure5-color.png)

![Fixture figure 5, desktop, gray](desktop-figure5-gray.png)

![Fixture figure 5, phone, color](phone-figure5-color.png)

![Fixture figure 5, phone, gray](phone-figure5-gray.png)

### Figure 6

![Fixture figure 6, desktop, color](desktop-figure6-color.png)

![Fixture figure 6, desktop, gray](desktop-figure6-gray.png)

![Fixture figure 6, phone, color](phone-figure6-color.png)

![Fixture figure 6, phone, gray](phone-figure6-gray.png)

### Figure 7

![Fixture figure 7, desktop, color](desktop-figure7-color.png)

![Fixture figure 7, desktop, gray](desktop-figure7-gray.png)

![Fixture figure 7, phone, color](phone-figure7-color.png)

![Fixture figure 7, phone, gray](phone-figure7-gray.png)

