#pragma once
#include "Grid.hpp"

// Shared, deterministic definition of the demo map. BOTH the runtime engine
// (for collisions) and the asset generator (for drawing the map image) use
// `tileAt`, so the picture and the walls can never drift out of sync.
//
// This is placeholder content for the foundation. The overnight run replaces it
// with real Gen-4-style tilesets and hand-authored portfolio rooms.

namespace demomap {

constexpr int W = 40; // tiles wide
constexpr int H = 26; // tiles tall

enum Tile { GRASS, PATH, WATER, TREE, FLOWER, DOOR };

// The door to the portfolio hub sits on the east arm of the horizontal path,
// well clear of the pond and the NPC's spot next to spawn.
constexpr int DOOR_X = W / 2 + 8;
constexpr int DOOR_Y = H / 2;

inline Tile tileAt(int x, int y) {
    if (x == DOOR_X && y == DOOR_Y) return DOOR;                   // hub entrance
    if (x <= 0 || y <= 0 || x >= W - 1 || y >= H - 1) return TREE; // tree border
    if (x >= 6 && x <= 9 && y >= 6 && y <= 9) return WATER;         // a small pond
    if (x == W / 2 || y == H / 2) return PATH;                      // cross paths
    if ((x * 3 + y * 5) % 23 == 0) return TREE;                     // scattered trees
    if ((x * 7 + y * 13) % 17 == 0) return FLOWER;                  // flower patches
    return GRASS;
}

inline bool isWall(Tile t) { return t == TREE || t == WATER; }

// Pixel spawn point: the center path intersection (always walkable).
constexpr int SPAWN_X = (W / 2) * grid::TILE;
constexpr int SPAWN_Y = (H / 2) * grid::TILE;

// Where the player reappears after walking back out of the hub: one tile
// west of the door, so stepping out doesn't immediately re-trigger the warp.
constexpr int DOOR_RETURN_X = (DOOR_X - 1) * grid::TILE;
constexpr int DOOR_RETURN_Y = DOOR_Y * grid::TILE;

} // namespace demomap
