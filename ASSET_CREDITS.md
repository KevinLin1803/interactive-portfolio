# Asset Credits

## Current assets (foundation)

All art currently in `assets/` is **procedurally generated** by
`tools/gen_assets.cpp` in this repo. It is original work, released under CC0
(public domain). It is intentionally simple placeholder art in a Gen-4 palette,
present so the engine has something to render and verify.

- `assets/maps/demo_lower.png`, `demo_upper.png` - generated demo map
- `assets/maps/hub_lower.png`, `hub_upper.png` - generated portfolio hub room
- `assets/characters/hero.png` - generated 4-direction walk sheet
- `assets/characters/npc.png` - generated 4-direction walk sheet, NPC palette
- `assets/characters/shadow.png` - generated shadow

## Planned: curated free Gen-4-style assets (to swap in)

The goal is a Generation-4 Pokémon *look* using only legally-licensed assets.
**No ripped Nintendo/Game Freak assets.** Candidate packs (verify each license
before committing the files):

| Pack | Source | License | Notes |
| --- | --- | --- | --- |
| Roguelike/RPG Pack | kenney.nl | **CC0** | Safest; top-down tiles + characters |
| Tiny Town / Tiny Dungeon | kenney.nl | **CC0** | Clean 16px tiles |
| Tiny 16: Basic (Sharm) | opengameart.org | CC-BY 3.0 | Attribute the author |
| Liberated Pixel Cup (LPC) | opengameart.org | CC-BY-SA 3.0 / GPL | Character generator; share-alike |
| Pipoya Free RPG Character Sprites | itch.io | Free w/ credit | 4-direction top-down, Pokémon-like |

When a pack is added: download it, drop files under `assets/`, record the exact
pack name, author, URL, and license here, and keep any required attribution.
