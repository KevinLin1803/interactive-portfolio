#pragma once
#include "Grid.hpp"
#include "raylib.h"

// Mirrors the JS Sprite: a 4x4 sheet of 32x32 frames (columns = animation
// frame, rows = direction). Idle uses column 0; walking cycles 1,2,3,0.
// Textures are NON-owning handles (owned by AssetStore).
class Sprite {
public:
    Sprite() = default;
    Sprite(Texture2D sheet, Texture2D shadow);

    void setDirection(grid::Dir d);
    void setMoving(bool moving);
    void update();                                  // advance the animation
    void draw(int worldX, int worldY, int camX, int camY) const;

private:
    Texture2D sheet_{};
    Texture2D shadow_{};
    bool hasShadow_ = false;
    grid::Dir dir_ = grid::Dir::Down;
    bool moving_ = false;

    int frameIndex_ = 0; // index into the current animation
    int frameProgress_ = 8;
    static constexpr int kFrameLimit = 8; // ticks each frame is shown
    static constexpr int kSize = 32;      // frame size in px
};
