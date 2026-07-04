#include "DirectionInput.hpp"
#include "raylib.h"
#include <algorithm>

namespace {
// Is any key bound to this direction currently held down?
bool dirDown(grid::Dir d) {
    switch (d) {
        case grid::Dir::Up:    return IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
        case grid::Dir::Down:  return IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
        case grid::Dir::Left:  return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
        case grid::Dir::Right: return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    }
    return false;
}
} // namespace

void DirectionInput::update() {
    const grid::Dir dirs[4] = {grid::Dir::Down, grid::Dir::Right,
                               grid::Dir::Up, grid::Dir::Left};
    for (grid::Dir d : dirs) {
        const int i = static_cast<int>(d);
        const bool now = dirDown(d);
        if (now && !down_[i]) {
            // freshly pressed -> becomes the active direction
            held_.insert(held_.begin(), d);
        } else if (!now && down_[i]) {
            held_.erase(std::remove(held_.begin(), held_.end(), d), held_.end());
        }
        down_[i] = now;
    }
}
