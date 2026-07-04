#pragma once
#include "Grid.hpp"

// Deterministic definition of the "hub" room: the first real portfolio room,
// reached through the door on the demo map. Later iterations branch project,
// work, and skills rooms off this same hub.

namespace hubmap {

constexpr int W = 14; // tiles wide
constexpr int H = 10; // tiles tall

enum Tile { FLOOR, WALL, EXIT };

inline Tile tileAt(int x, int y) {
    if (x == W / 2 && y == H - 1) return EXIT;                      // south door
    if (x <= 0 || y <= 0 || x >= W - 1 || y >= H - 1) return WALL; // room walls
    return FLOOR;
}

inline bool isWall(Tile t) { return t == WALL; }

// Pixel spawn point: just inside the south exit, facing into the room.
constexpr int SPAWN_X = (W / 2) * grid::TILE;
constexpr int SPAWN_Y = (H - 2) * grid::TILE;

} // namespace hubmap
