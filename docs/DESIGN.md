# Interactive Portfolio (C++ / raylib) - Design Spec

Date: 2026-07-05
Status: Approved (foundation build in progress)

## Goal

Port [bryanleezh/www](https://github.com/bryanleezh/www) - an interactive,
Pokémon-style top-down "overworld" portfolio originally written in vanilla
JavaScript (`<canvas>`) - into **C++** using the **raylib** game library, so it
runs both:

- as a **native desktop game** (macOS/Linux/Windows), and
- as a **web app** compiled to **WebAssembly** via Emscripten (hosted at a URL,
  like the original).

Visual direction: **Generation-4 Pokémon** aesthetic, using **only free,
legally-licensed assets** (CC0 / CC-BY / MIT look-alike packs). No ripped
Nintendo assets.

## Attribution / License

- Original engine design and portfolio concept: **MIT © Bryan Lee**
  (bryanleezh). This port preserves that credit (see `LICENSE`).
- All art/audio assets are tracked with their licenses in `ASSET_CREDITS.md`.

## Source engine → C++ mapping

The original is the classic "Pizza Legends"-style engine:

- 16px grid; internal resolution **352×198**, scaled up with nearest-neighbor.
- Camera centers the player at `withGrid(10.5), withGrid(6)` (168, 96).
- Sprites are **32×32** frames on a sheet: 4 animation frames × 4 directions
  (idle/walk per direction). Sprite drawn offset (-8, -18) to sit on a 16px cell.
- Movement is grid-locked: a step takes **16 frames** at 1px/frame.
- Maps render as **lower layer → entities (y-sorted) → upper layer**, plus a
  wall set for collisions.

| JS original | C++ module (this repo) |
| --- | --- |
| `init.js`, game loop | `src/Game.{hpp,cpp}` |
| `utilities/DirectionInput.js` | `src/DirectionInput.{hpp,cpp}` |
| `sprites/Sprite.js` | `src/Sprite.{hpp,cpp}` |
| `sprites/GameObject.js`, `sprites/Character.js` | `src/GameObject.{hpp,cpp}` (GameObject + Person) |
| `overworld/OverworldMap.js`, `fullmap.js` | `src/GameMap.{hpp,cpp}` + `assets/maps/*` |
| `utilities/utilities.js` (grid math) | `src/Grid.hpp` |
| `texts/TextMessage.js`, `menus/*` | *(future - added by overnight run)* |
| `overworld/OverworldEvent.js`, `progress.js` | *(future)* |

## Foundation (hand-built, must be green before any overnight run)

1. CMake project; raylib pulled via `FetchContent`.
2. Builds **native** (system clang) **and web** (`emcmake cmake` + Emscripten).
3. Internal 352×198 `RenderTexture2D`, scaled to window with nearest-neighbor.
4. One demo map (lower/upper layers) drawn from a Gen-4-palette tileset.
5. A **player sprite that walks** (arrow keys / WASD), grid-locked, animated,
   with a camera that follows and simple wall collisions.
6. `--screenshot <path>` mode: render N frames headless-ish and
   `TakeScreenshot`, so the build can be verified visually in CI/terminal.
7. Docs: `README.md`, `LICENSE`, `ASSET_CREDITS.md`, `AGENTS.md`.

## Verification loop

- **Native:** `cmake --build build` compiles with no errors; the binary runs
  with `--screenshot out.png --frames 120` and the PNG shows the map + sprite.
- **Web:** `emcmake` build produces `.html/.js/.wasm`; served locally it loads
  with no console errors and the canvas paints.

## Overnight (gnhf) plan - expand, don't rewrite

Foundation stays green; gnhf adds one committed slice per iteration:

1. Swap the placeholder art for a curated free Gen-4-style pack (candidates in
   `ASSET_CREDITS.md`).
2. Collisions from a wall map / tiled data.
3. NPCs with behaviour loops.
4. Text-box system (`TextMessage`) + typewriter reveal.
5. Keyboard menu + pause menu.
6. Each portfolio "room" (hub, projects, work, skills) as its own map + content.

Run config: `--agent claude` on **Sonnet**, `--max-tokens 5000000`,
`--prevent-sleep on`, on a `gnhf/*` branch. Build must stay green (native+web),
one commit per successful iteration.

## Non-goals

- No 1:1 pixel-perfect reproduction of Bryan's exact maps.
- No ripped Nintendo assets.
- No networking/multiplayer.
