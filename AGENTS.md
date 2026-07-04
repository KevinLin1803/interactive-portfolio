# AGENTS.md

Guidance for coding agents (including overnight gnhf runs) working in this repo.

## What this is

A C++/raylib re-implementation of the top-down overworld portfolio engine from
[bryanleezh/www](https://github.com/bryanleezh/www) (vanilla-JS `<canvas>`,
"Pizza Legends" style). It must build and run as **both** a native desktop app
and a **WebAssembly** web app. Visual target: **Generation-4 Pokémon**, using
only **free, legally-licensed assets** (never ripped Nintendo art).

## Golden rules

1. **Keep both builds green.** Every change must compile native AND web. Verify
   before committing (see below).
2. **One small, committed slice per iteration.** Don't rewrite; extend.
3. **Preserve the architecture.** Small, single-purpose files with clear
   boundaries. Match the existing style.
4. **Assets must stay license-clean.** Only CC0 / CC-BY / MIT. Record every
   external asset in `ASSET_CREDITS.md`.
5. **No em dashes in prose. Use a regular hyphen.**

## Engine model (mirrors the JS original)

- 16px grid; internal resolution 352x198, scaled up nearest-neighbor.
- Camera centers the player at (168, 96) = `withGrid(10.5), withGrid(6)`.
- Sprites: 32x32 frames, 4 columns (frames) x 4 rows (directions: down, right,
  up, left). Idle = column 0; walk cycles columns 1,2,3,0.
- Movement is grid-locked: 16 frames per tile at 1px/frame.
- Maps draw as lower layer -> entities -> upper layer, plus a wall set.

## Module map

| File | Responsibility |
| --- | --- |
| `src/Grid.hpp` | grid constants, `Dir`, `nextPosition` |
| `src/DirectionInput.*` | keyboard -> held-direction stack |
| `src/Sprite.*` | sprite-sheet animation + draw |
| `src/GameObject.*` | `Person` (player/NPC): grid-locked walking |
| `src/GameMap.*` | lower/upper layers + collisions |
| `src/AssetStore.*` | owns/caches textures |
| `src/Game.*` | window, render target, world, main tick |
| `src/main.cpp` | arg parsing, native loop + Emscripten loop |
| `src/DemoMap.hpp` | shared, deterministic demo-map definition |
| `tools/gen_assets.cpp` | headless placeholder-art generator |

## Build & verify

```sh
# native
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target gen_assets -j && ./build/gen_assets
cmake --build build --target portfolio -j
./build/portfolio --screenshot /tmp/shot.png --frames 90   # visual check

# web (assets must be generated first)
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web --target portfolio -j
```

A change is "done" only when: native compiles, web compiles, and the native
`--screenshot` still renders the map + character.

## Roadmap (expand in this order)

1. Swap placeholder art for a curated free Gen-4-style pack (`ASSET_CREDITS.md`).
2. Upper-layer tree canopies (draw over the player) + refined tileset.
3. NPCs with behaviour loops (reuse `Person`).
4. Text-box system with typewriter reveal (`ui/TextMessage`).
5. Keyboard menu + pause menu (`ui/KeyboardMenu`).
6. Portfolio rooms (hub, projects, work, skills) as maps + content data, with
   map transitions.

Content to port lives in the original repo under `data/`, `texts/`, `menus/`,
and `overworld/OverworldMap.js`.
