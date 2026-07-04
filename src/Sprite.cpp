#include "Sprite.hpp"

namespace {
// Walk cycle columns (idle is always column 0), matching the JS animations.
constexpr int kWalkCols[4] = {1, 2, 3, 0};
} // namespace

Sprite::Sprite(Texture2D sheet, Texture2D shadow)
    : sheet_(sheet), shadow_(shadow), hasShadow_(shadow.id != 0) {}

void Sprite::setDirection(grid::Dir d) {
    if (dir_ != d) {
        dir_ = d;
        frameIndex_ = 0;
        frameProgress_ = kFrameLimit;
    }
}

void Sprite::setMoving(bool moving) {
    if (moving_ != moving) {
        moving_ = moving;
        frameIndex_ = 0;
        frameProgress_ = kFrameLimit;
    }
}

void Sprite::update() {
    if (!moving_) return; // idle stays on frame 0
    if (frameProgress_ > 0) {
        frameProgress_ -= 1;
        return;
    }
    frameProgress_ = kFrameLimit;
    frameIndex_ = (frameIndex_ + 1) % 4;
}

void Sprite::draw(int worldX, int worldY, int camX, int camY) const {
    // -8 / -18 centers the 32px sprite on its 16px grid cell (from the JS draw).
    const int screenX = worldX - 8 + grid::CAM_OFFSET_X - camX;
    const int screenY = worldY - 18 + grid::CAM_OFFSET_Y - camY;

    if (hasShadow_) {
        DrawTexture(shadow_, screenX, screenY, WHITE);
    }

    const int col = moving_ ? kWalkCols[frameIndex_] : 0;
    const int row = static_cast<int>(dir_);
    const Rectangle src{static_cast<float>(col * kSize),
                        static_cast<float>(row * kSize),
                        static_cast<float>(kSize), static_cast<float>(kSize)};
    DrawTextureRec(sheet_, src,
                   Vector2{static_cast<float>(screenX), static_cast<float>(screenY)},
                   WHITE);
}
