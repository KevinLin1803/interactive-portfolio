#pragma once
#include "Grid.hpp"
#include "raylib.h"
#include <unordered_set>

// Mirrors the JS OverworldMap: a lower image (ground), an upper image (things
// drawn on top of the player, e.g. tree canopies), and a set of blocked cells.
class GameMap {
public:
    GameMap(Texture2D lower, Texture2D upper);

    void addWall(int gridX, int gridY);                 // grid coords
    bool isCollided(int x, int y, grid::Dir dir) const; // pixel coords + dir

    void drawLower(int camX, int camY) const;
    void drawUpper(int camX, int camY) const;

private:
    static long long key(int px, int py) {
        return (static_cast<long long>(px) << 32) ^ (py & 0xffffffffLL);
    }

    Texture2D lower_{};
    Texture2D upper_{};
    std::unordered_set<long long> walls_; // keyed by pixel coords of blocked cells
};
