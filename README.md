# interactive-portfolio

A Generation-4 Pokémon-style, top-down interactive portfolio - written in
**C++** with **raylib**, playable both as a **native desktop app** and in the
**browser** (compiled to WebAssembly via Emscripten).

It is a C++ re-implementation of the "Pizza Legends"-style overworld engine from
[bryanleezh/www](https://github.com/bryanleezh/www) (MIT © Bryan Lee). See
[`LICENSE`](LICENSE) and [`ASSET_CREDITS.md`](ASSET_CREDITS.md).

> Status: **foundation**. A walking character on a demo map, building for native
> and web. Features (NPCs, text boxes, menus, portfolio rooms) are added
> incrementally - see [`docs/DESIGN.md`](docs/DESIGN.md) and [`AGENTS.md`](AGENTS.md).

## Controls

Arrow keys or WASD to move.

## Build - native (macOS / Linux / Windows)

Requires CMake and a C++17 compiler. raylib is fetched automatically.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target gen_assets -j        # build the asset generator
./build/gen_assets                                # (run from repo root) writes assets/
cmake --build build --target portfolio -j         # build the game
./build/portfolio                                 # play
```

Headless render check (writes a screenshot then exits):

```sh
./build/portfolio --screenshot shot.png --frames 90
```

## Build - web (WebAssembly)

Requires [Emscripten](https://emscripten.org/). Assets must be generated first
(run the native `gen_assets` step above), then:

```sh
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web --target portfolio -j
# serve it (WASM must be served over HTTP, not file://)
python3 -m http.server --directory build-web 8080
# open http://localhost:8080/portfolio.html
```

## Layout

```
src/            engine (see AGENTS.md for the module map)
tools/          gen_assets.cpp - generates placeholder art
assets/         maps + character sheets
web/shell.html  HTML shell for the web build
docs/DESIGN.md  design spec
```
