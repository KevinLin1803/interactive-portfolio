#pragma once
// Grid + direction helpers, mirroring the original JS `utilities` module.
// Everything is laid out on a 16px grid; the internal render resolution is
// 352x198 and the camera centers the player at (withGrid(10.5), withGrid(6)).

namespace grid {

constexpr int TILE = 16;
constexpr int SCREEN_W = 352;
constexpr int SCREEN_H = 198;

// Camera offset that keeps the followed person centered (from the JS engine).
constexpr int CAM_OFFSET_X = 168; // withGrid(10.5)
constexpr int CAM_OFFSET_Y = 96;  // withGrid(6)

inline int withGrid(double n) { return static_cast<int>(n * TILE); }

// Row order matches the sprite sheet: down, right, up, left.
enum class Dir { Down = 0, Right = 1, Up = 2, Left = 3 };

struct Vec2i {
    int x;
    int y;
};

// Next grid cell (in pixels) after stepping one tile in `dir`.
inline Vec2i nextPosition(int x, int y, Dir dir) {
    switch (dir) {
        case Dir::Left:  return {x - TILE, y};
        case Dir::Right: return {x + TILE, y};
        case Dir::Up:    return {x, y - TILE};
        case Dir::Down:  return {x, y + TILE};
    }
    return {x, y};
}

} // namespace grid
