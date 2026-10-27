# Tetris

In June 2015, I was finishing my second year of my studies and I prepared this game to pass classes from ***Computer graphics and visualization*** (Polish: *Grafika komputerowa i wizualizacja*).

## Requirements

To build this game, you need: `g++` and `OpenGL` libraries (`gl.h`, `glu.h`, `glut.h`).

```shell
$ sudo add-apt-repository universe
$ sudo apt update && sudo apt install g++ mesa-common-dev libglu1-mesa-dev freeglut3-dev -y
```

This command has been checked in Ubuntu 18.10.

## Build & Run

```shell
$ cd cpp
$ g++ -o tetris.out tetris.cpp -lGL -lGLU -lglut -lm
$ ./tetris.out
```

## Run on WSL

On Windows:
- Install & Run [VcXsrv](https://sourceforge.net/projects/vcxsrv/)
- Choose: Multiple windows
- Display number: 0
- Start no client
- Disable: Native OpenGL
- (WSL2) Enable: Disable access control

After this, on WSL1:

```shell
$ export DISPLAY=localhost:0
$ ./tetris.out
```

on WSL2
```shell
$ export DISPLAY=$(grep -m 1 nameserver /etc/resolv.conf | awk '{print $2}'):0.0
$ ./tetris.out
```

You can find more on this link: [Does WSL support openGL?](https://github.com/microsoft/WSL/issues/2855#issuecomment-358861903) and [Can't use X-Server in WSL 2
](https://github.com/microsoft/WSL/issues/4106)

## Screens

![](./docs/tetris-menu.png)
![](./docs/tetris-game.png)
![](./docs/tetris-end.png)
![](./docs/tetris-results.png)
![](./docs/tetris-help.png)
![](./docs/tetris-settings.png)

## License
MIT

## Play in a browser

The WebAssembly version is available at https://adam-zielonka.github.io/tetris/.
Use the arrow keys to move and rotate, Space to select a menu item, P to pause,
Esc to return, and L to change language. On a phone, use the on-screen controls;
hold left, right or down to repeat a move. High scores are stored in your browser
on this device.

To build it locally, install and activate [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) 6.0.13, then run:

```shell
./scripts/build-web.sh
python3 -m http.server --directory build/web 8000
```

Open `http://localhost:8000/`. Serve the files over HTTP rather than opening the
HTML directly. On Linux, the native executable requires the OpenGL, GLU and
freeglut runtime libraries and a graphical desktop.

GitHub Actions builds both versions on pushes and pull requests. A push to
`main` deploys the web build to GitHub Pages. Pushing a `v*` tag builds both
archives and publishes a GitHub release.


## Language / Język

The browser version supports Polish and English. On the first visit it uses the
first supported language from the browser’s preferences, falling back to English.
The discreet English / Polski links in the footer save an explicit choice
in this application’s local storage.
Switching language preserves the current game. If storage is blocked, language switching
still works for the current visit.
The L key and the in-game language control stay synchronized with the footer language links.

The Pages workflow runs `scripts/test-web-language.cjs` against the built WebAssembly
game using Playwright. It checks browser preferences, both directions of language
synchronization, persistence, the paused board and 320–1440 px layouts. For a local
run, make `@playwright/test` available on `NODE_PATH` and run the script after the web build.
