// Generates placeholder Gen-4-palette art for the foundation build so the
// engine has something to render and verify. 100% original/CC0. The overnight
// run swaps these for a curated free Gen-4-style pack (see ASSET_CREDITS.md).
//
// Uses only raylib's CPU-side Image API, so it runs headless (no GL context).
#include "DemoMap.hpp"
#include "Grid.hpp"
#include "raylib.h"
#include <filesystem>

namespace {
constexpr int T = grid::TILE; // 16

const Color GRASS_A{104, 168, 72, 255};
const Color GRASS_B{120, 184, 88, 255};
const Color PATH_A{216, 192, 128, 255};
const Color PATH_B{198, 172, 108, 255};
const Color WATER_A{96, 144, 224, 255};
const Color WATER_B{128, 176, 240, 255};
const Color CANOPY{56, 128, 72, 255};
const Color CANOPY_D{32, 88, 52, 255};
const Color TRUNK{120, 80, 48, 255};
const Color FLOWER_R{224, 96, 96, 255};
const Color FLOWER_Y{240, 216, 96, 255};

void drawGrass(Image* img, int ox, int oy) {
    ImageDrawRectangle(img, ox, oy, T, T, GRASS_A);
    for (int y = 0; y < T; y += 2)
        for (int x = 0; x < T; x += 2)
            if (((x + y) / 2) % 3 == 0) ImageDrawPixel(img, ox + x, oy + y, GRASS_B);
}

void drawTile(Image* img, int gx, int gy) {
    const int ox = gx * T, oy = gy * T;
    switch (demomap::tileAt(gx, gy)) {
        case demomap::GRASS:
            drawGrass(img, ox, oy);
            break;
        case demomap::FLOWER:
            drawGrass(img, ox, oy);
            ImageDrawRectangle(img, ox + 5, oy + 5, 2, 2, FLOWER_R);
            ImageDrawRectangle(img, ox + 9, oy + 8, 2, 2, FLOWER_Y);
            break;
        case demomap::PATH:
            ImageDrawRectangle(img, ox, oy, T, T, PATH_A);
            for (int i = 0; i < T; i += 3)
                ImageDrawPixel(img, ox + (i * 5) % T, oy + i, PATH_B);
            break;
        case demomap::WATER:
            ImageDrawRectangle(img, ox, oy, T, T, WATER_A);
            for (int y = 2; y < T; y += 5)
                ImageDrawRectangle(img, ox + 2, oy + y, 8, 1, WATER_B);
            break;
        case demomap::TREE:
            drawGrass(img, ox, oy);
            ImageDrawRectangle(img, ox + 6, oy + 9, 4, 6, TRUNK);
            ImageDrawCircle(img, ox + 8, oy + 6, 7, CANOPY_D);
            ImageDrawCircle(img, ox + 8, oy + 6, 5, CANOPY);
            break;
    }
}

// One 32x32 hero frame at cell (col,row). row = direction, col = walk frame.
void drawHero(Image* img, int col, int row, grid::Dir dir) {
    const int ox = col * 32, oy = row * 32;
    const Color skin{248, 208, 168, 255};
    const Color cap{216, 64, 64, 255};
    const Color shirt{64, 112, 208, 255};
    const Color legs{56, 56, 72, 255};
    const int cx = ox + 16;

    // legs shift per walk frame to fake a stride
    const int stride = (col == 1) ? -2 : (col == 3) ? 2 : 0;
    ImageDrawRectangle(img, cx - 5, oy + 24 + stride, 4, 6, legs);
    ImageDrawRectangle(img, cx + 1, oy + 24 - stride, 4, 6, legs);
    // torso
    ImageDrawRectangle(img, cx - 6, oy + 14, 12, 11, shirt);
    // head
    ImageDrawCircle(img, cx, oy + 9, 6, skin);
    // cap
    ImageDrawRectangle(img, cx - 6, oy + 3, 12, 4, cap);
    ImageDrawRectangle(img, cx - 6, oy + 7, 12, 1, cap);
    // face accents by direction
    const Color eye{40, 40, 48, 255};
    if (dir == grid::Dir::Down) {
        ImageDrawRectangle(img, cx - 3, oy + 10, 2, 2, eye);
        ImageDrawRectangle(img, cx + 1, oy + 10, 2, 2, eye);
    } else if (dir == grid::Dir::Right) {
        ImageDrawRectangle(img, cx + 2, oy + 10, 2, 2, eye);
    } else if (dir == grid::Dir::Left) {
        ImageDrawRectangle(img, cx - 4, oy + 10, 2, 2, eye);
    } // Up = back of head, no eyes
}
} // namespace

int main() {
    std::filesystem::create_directories("assets/maps");
    std::filesystem::create_directories("assets/characters");

    // --- map lower layer ---
    Image lower = GenImageColor(demomap::W * T, demomap::H * T, GRASS_A);
    for (int gy = 0; gy < demomap::H; ++gy)
        for (int gx = 0; gx < demomap::W; ++gx) drawTile(&lower, gx, gy);
    ExportImage(lower, "assets/maps/demo_lower.png");
    UnloadImage(lower);

    // --- map upper layer (nothing on top yet: fully transparent) ---
    Image upper = GenImageColor(demomap::W * T, demomap::H * T, BLANK);
    ExportImage(upper, "assets/maps/demo_upper.png");
    UnloadImage(upper);

    // --- hero sheet: 4 cols (frames) x 4 rows (directions) of 32px ---
    const grid::Dir dirs[4] = {grid::Dir::Down, grid::Dir::Right, grid::Dir::Up,
                               grid::Dir::Left};
    Image hero = GenImageColor(128, 128, BLANK);
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col) drawHero(&hero, col, row, dirs[row]);
    ExportImage(hero, "assets/characters/hero.png");
    UnloadImage(hero);

    // --- shadow ---
    Image shadow = GenImageColor(32, 32, BLANK);
    ImageDrawCircle(&shadow, 16, 27, 6, Color{0, 0, 0, 90});
    ExportImage(shadow, "assets/characters/shadow.png");
    UnloadImage(shadow);

    return 0;
}
