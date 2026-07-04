// Generates placeholder Gen-4-palette art for the foundation build so the
// engine has something to render and verify. 100% original/CC0. The overnight
// run swaps these for a curated free Gen-4-style pack (see ASSET_CREDITS.md).
//
// Uses only raylib's CPU-side Image API, so it runs headless (no GL context).
#include "DemoMap.hpp"
#include "Grid.hpp"
#include "HubMap.hpp"
#include "raylib.h"
#include <filesystem>

namespace {
constexpr int T = grid::TILE; // 16

// Kenney "Roguelike/RPG Pack" (CC0, see ASSET_CREDITS.md). Sheet cells are
// 16x16 with a 1px margin, so each cell occupies a 17x17 stride.
constexpr int SHEET_CELL = T + 1;
constexpr int WATER_SHEET_COL = 0;
constexpr int WATER_SHEET_ROW = 0;
constexpr int GRASS_SHEET_COL = 5;
constexpr int GRASS_SHEET_ROW = 0;
constexpr int PATH_SHEET_COL = 6;
constexpr int PATH_SHEET_ROW = 0;
constexpr int HUB_FLOOR_A_SHEET_COL = 21;
constexpr int HUB_FLOOR_A_SHEET_ROW = 17;
constexpr int HUB_FLOOR_B_SHEET_COL = 26;
constexpr int HUB_FLOOR_B_SHEET_ROW = 18;

void blitSheetTile(Image* dst, Image* sheet, int sheetCol, int sheetRow, int ox, int oy) {
    Rectangle src{float(sheetCol * SHEET_CELL), float(sheetRow * SHEET_CELL), float(T), float(T)};
    Rectangle dstRect{float(ox), float(oy), float(T), float(T)};
    ImageDraw(dst, *sheet, src, dstRect, WHITE);
}

const Color GRASS_A{104, 168, 72, 255};
const Color WATER_A{96, 144, 224, 255};
const Color WATER_B{128, 176, 240, 255};
const Color CANOPY{56, 128, 72, 255};
const Color CANOPY_D{32, 88, 52, 255};
const Color TRUNK{120, 80, 48, 255};
const Color FLOWER_R{224, 96, 96, 255};
const Color FLOWER_Y{240, 216, 96, 255};
const Color DOOR_FRAME{120, 72, 40, 255};
const Color WALL_COLOR{96, 80, 64, 255};
const Color EXIT_COLOR{216, 192, 128, 255};

void drawGrass(Image* img, Image* tileSheet, int ox, int oy) {
    blitSheetTile(img, tileSheet, GRASS_SHEET_COL, GRASS_SHEET_ROW, ox, oy);
}

void drawTileLower(Image* img, Image* tileSheet, int gx, int gy) {
    const int ox = gx * T, oy = gy * T;
    switch (demomap::tileAt(gx, gy)) {
        case demomap::GRASS:
            drawGrass(img, tileSheet, ox, oy);
            break;
        case demomap::FLOWER:
            drawGrass(img, tileSheet, ox, oy);
            ImageDrawRectangle(img, ox + 5, oy + 5, 2, 2, FLOWER_R);
            ImageDrawRectangle(img, ox + 9, oy + 8, 2, 2, FLOWER_Y);
            break;
        case demomap::PATH:
            blitSheetTile(img, tileSheet, PATH_SHEET_COL, PATH_SHEET_ROW, ox, oy);
            break;
        case demomap::WATER:
            blitSheetTile(img, tileSheet, WATER_SHEET_COL, WATER_SHEET_ROW, ox, oy);
            break;
        case demomap::TREE:
            drawGrass(img, tileSheet, ox, oy);
            ImageDrawRectangle(img, ox + 6, oy + 9, 4, 6, TRUNK);
            break;
        case demomap::DOOR:
            blitSheetTile(img, tileSheet, PATH_SHEET_COL, PATH_SHEET_ROW, ox, oy);
            ImageDrawRectangle(img, ox + 3, oy + 2, T - 6, T - 4, DOOR_FRAME);
            break;
    }
}

// Hub room: checkered floor (real sheet tiles), solid walls, and a distinct
// exit tile (walls/exit are still procedural placeholders).
void drawHubTile(Image* img, Image* tileSheet, int gx, int gy) {
    const int ox = gx * T, oy = gy * T;
    switch (hubmap::tileAt(gx, gy)) {
        case hubmap::WALL:
            ImageDrawRectangle(img, ox, oy, T, T, WALL_COLOR);
            break;
        case hubmap::EXIT:
            ImageDrawRectangle(img, ox, oy, T, T, EXIT_COLOR);
            break;
        case hubmap::FLOOR:
            if ((gx + gy) % 2 == 0)
                blitSheetTile(img, tileSheet, HUB_FLOOR_A_SHEET_COL, HUB_FLOOR_A_SHEET_ROW, ox, oy);
            else
                blitSheetTile(img, tileSheet, HUB_FLOOR_B_SHEET_COL, HUB_FLOOR_B_SHEET_ROW, ox, oy);
            break;
    }
}

// Tree canopy lives on the upper layer, drawn after the player each frame, so
// the player appears to walk behind the tree trunk while the leaves stay on
// top. Every other tile has nothing on the upper layer.
void drawTileUpper(Image* img, int gx, int gy) {
    if (demomap::tileAt(gx, gy) != demomap::TREE) return;
    const int ox = gx * T, oy = gy * T;
    ImageDrawCircle(img, ox + 8, oy + 6, 7, CANOPY_D);
    ImageDrawCircle(img, ox + 8, oy + 6, 5, CANOPY);
}

struct PersonPalette {
    Color skin;
    Color cap;
    Color shirt;
    Color legs;
};

constexpr PersonPalette HERO_PALETTE{
    {248, 208, 168, 255}, {216, 64, 64, 255}, {64, 112, 208, 255}, {56, 56, 72, 255}};
constexpr PersonPalette NPC_PALETTE{
    {248, 208, 168, 255}, {96, 176, 96, 255}, {224, 176, 80, 255}, {72, 56, 40, 255}};

// One 32x32 person frame at cell (col,row). row = direction, col = walk frame.
void drawPerson(Image* img, int col, int row, grid::Dir dir, const PersonPalette& pal) {
    const int ox = col * 32, oy = row * 32;
    const Color skin = pal.skin;
    const Color cap = pal.cap;
    const Color shirt = pal.shirt;
    const Color legs = pal.legs;
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

    Image tileSheet = LoadImage("assets/tiles/kenney_roguelike_sheet.png");

    // --- map lower layer ---
    Image lower = GenImageColor(demomap::W * T, demomap::H * T, GRASS_A);
    for (int gy = 0; gy < demomap::H; ++gy)
        for (int gx = 0; gx < demomap::W; ++gx) drawTileLower(&lower, &tileSheet, gx, gy);
    ExportImage(lower, "assets/maps/demo_lower.png");
    UnloadImage(lower);

    // --- map upper layer: tree canopies only, drawn over the player ---
    Image upper = GenImageColor(demomap::W * T, demomap::H * T, BLANK);
    for (int gy = 0; gy < demomap::H; ++gy)
        for (int gx = 0; gx < demomap::W; ++gx) drawTileUpper(&upper, gx, gy);
    ExportImage(upper, "assets/maps/demo_upper.png");
    UnloadImage(upper);

    // --- hub room: checkered floor (real tiles) + procedural walls/exit ---
    Image hubLower = GenImageColor(hubmap::W * T, hubmap::H * T, WALL_COLOR);
    for (int gy = 0; gy < hubmap::H; ++gy)
        for (int gx = 0; gx < hubmap::W; ++gx) drawHubTile(&hubLower, &tileSheet, gx, gy);
    ExportImage(hubLower, "assets/maps/hub_lower.png");
    UnloadImage(hubLower);
    UnloadImage(tileSheet);

    Image hubUpper = GenImageColor(hubmap::W * T, hubmap::H * T, BLANK);
    ExportImage(hubUpper, "assets/maps/hub_upper.png");
    UnloadImage(hubUpper);

    // --- character sheets: 4 cols (frames) x 4 rows (directions) of 32px ---
    const grid::Dir dirs[4] = {grid::Dir::Down, grid::Dir::Right, grid::Dir::Up,
                               grid::Dir::Left};
    Image hero = GenImageColor(128, 128, BLANK);
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col) drawPerson(&hero, col, row, dirs[row], HERO_PALETTE);
    ExportImage(hero, "assets/characters/hero.png");
    UnloadImage(hero);

    Image npc = GenImageColor(128, 128, BLANK);
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col) drawPerson(&npc, col, row, dirs[row], NPC_PALETTE);
    ExportImage(npc, "assets/characters/npc.png");
    UnloadImage(npc);

    // --- shadow ---
    Image shadow = GenImageColor(32, 32, BLANK);
    ImageDrawCircle(&shadow, 16, 27, 6, Color{0, 0, 0, 90});
    ExportImage(shadow, "assets/characters/shadow.png");
    UnloadImage(shadow);

    return 0;
}
