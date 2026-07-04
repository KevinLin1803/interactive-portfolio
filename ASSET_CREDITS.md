# Asset Credits

## Current assets (foundation)

Most art in `assets/` is still **procedurally generated** by
`tools/gen_assets.cpp` in this repo. It is original work, released under CC0
(public domain). It is intentionally simple placeholder art in a Gen-4 palette,
present so the engine has something to render and verify.

- `assets/maps/demo_lower.png`, `demo_upper.png` - generated demo map (WATER,
  GRASS, and PATH tiles are now real art, see below; tree canopy, trunk,
  flowers, and door frame overlay are still procedural)
- `assets/maps/hub_lower.png`, `hub_upper.png` - generated portfolio hub room
- `assets/characters/hero.png` - generated 4-direction walk sheet
- `assets/characters/npc.png` - generated 4-direction walk sheet, NPC palette
- `assets/characters/shadow.png` - generated shadow

## Curated free Gen-4-style assets (in progress)

The goal is a Generation-4 Pokémon *look* using only legally-licensed assets.
**No ripped Nintendo/Game Freak assets.**

### In use

- **Roguelike/RPG Pack** by Kenney Vleugels for Kenney (www.kenney.nl), with
  help by Lynn Evers. Source: https://kenney.nl/assets/roguelike-rpg-pack.
  License: **CC0** (public domain).
  File: `assets/tiles/kenney_roguelike_sheet.png` (the pack's
  `Spritesheet/roguelikeSheet_transparent.png`, copied in unmodified).
  Currently used for the demo map's WATER, GRASS, and PATH tiles (16x16 cells
  stamped in by `tools/gen_assets.cpp`'s `blitSheetTile`): WATER at sheet
  column 0 row 0, GRASS at column 5 row 0, PATH at column 6 row 0. Cell
  identity was inferred without visually inspecting the sheet, by sampling
  average RGB and per-channel standard deviation over each 16x16 cell: GRASS
  is a near-solid green cell (avg ~140,195,52, std < 3) and PATH a near-solid
  brown cell (avg ~178,130,84, std < 6), both distinct from the WATER blue and
  from each other with high confidence. Trees, flowers, and the door frame
  overlay are still procedural placeholders pending future iterations that
  pick further cells from this same sheet.

### Candidates for future iterations

| Pack | Source | License | Notes |
| --- | --- | --- | --- |
| Tiny Town / Tiny Dungeon | kenney.nl | **CC0** | Clean 16px tiles, no water/tree tiles found |
| Tiny 16: Basic (Sharm) | opengameart.org | CC-BY 3.0 | Attribute the author |
| Liberated Pixel Cup (LPC) | opengameart.org | CC-BY-SA 3.0 / GPL | Character generator; share-alike |
| Pipoya Free RPG Character Sprites | itch.io | Free w/ credit | 4-direction top-down, Pokémon-like |

When more of a pack is wired in: drop files under `assets/`, record the exact
pack name, author, URL, and license here, and keep any required attribution.
