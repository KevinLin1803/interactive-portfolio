#include "Game.hpp"
#include "DemoMap.hpp"
#include "HubMap.hpp"
#include <cmath>
#include <cstdlib> // std::abs for the NPC-adjacency check

Game::Game(std::string screenshotPath, int screenshotFrames)
    : screenshotPath_(std::move(screenshotPath)),
      screenshotFrames_(screenshotFrames) {
    const int scale = 3;
    InitWindow(grid::SCREEN_W * scale, grid::SCREEN_H * scale,
               "Interactive Portfolio");

#if !defined(PLATFORM_WEB)
    // Run from the binary's directory so relative "assets/..." paths resolve.
    ChangeDirectory(GetApplicationDirectory());
#endif

    target_ = LoadRenderTexture(grid::SCREEN_W, grid::SCREEN_H);
    SetTextureFilter(target_.texture, TEXTURE_FILTER_POINT);

    const Texture2D lower = assets_.load("assets/maps/demo_lower.png");
    const Texture2D upper = assets_.load("assets/maps/demo_upper.png");
    demoMap_ = std::make_unique<GameMap>(lower, upper);

    // Build collisions from the shared, deterministic map definitions.
    for (int gy = 0; gy < demomap::H; ++gy) {
        for (int gx = 0; gx < demomap::W; ++gx) {
            if (demomap::isWall(demomap::tileAt(gx, gy))) {
                demoMap_->addWall(gx, gy);
            }
        }
    }

    const Texture2D hubLower = assets_.load("assets/maps/hub_lower.png");
    const Texture2D hubUpper = assets_.load("assets/maps/hub_upper.png");
    hubMap_ = std::make_unique<GameMap>(hubLower, hubUpper);
    for (int gy = 0; gy < hubmap::H; ++gy) {
        for (int gx = 0; gx < hubmap::W; ++gx) {
            if (hubmap::isWall(hubmap::tileAt(gx, gy))) {
                hubMap_->addWall(gx, gy);
            }
        }
    }

    activeMap_ = demoMap_.get();

    const Texture2D hero = assets_.load("assets/characters/hero.png");
    const Texture2D shadow = assets_.load("assets/characters/shadow.png");
    player_ = std::make_unique<Person>(demomap::SPAWN_X, demomap::SPAWN_Y,
                                       Sprite(hero, shadow), /*isPlayer=*/true);

    // NPC stands a few tiles east of spawn, on the walkable east-west path.
    const Texture2D npcSheet = assets_.load("assets/characters/npc.png");
    npc_ = std::make_unique<Person>(demomap::SPAWN_X + 3 * grid::TILE, demomap::SPAWN_Y,
                                    Sprite(npcSheet, shadow), /*isPlayer=*/false);
}

Game::~Game() {
    UnloadRenderTexture(target_);
    CloseWindow();
}

namespace {
// True when the player is standing exactly one tile away from the NPC
// (orthogonally adjacent), i.e. close enough to talk to it.
bool isAdjacent(const Person& a, const Person& b) {
    const int dx = std::abs(a.x() - b.x());
    const int dy = std::abs(a.y() - b.y());
    return (dx == grid::TILE && dy == 0) || (dx == 0 && dy == grid::TILE);
}

// True when the person is standing exactly on grid cell (gx, gy), in pixels.
bool atTile(const Person& p, int gx, int gy) {
    return p.x() == gx * grid::TILE && p.y() == gy * grid::TILE;
}
} // namespace

void Game::frame() {
    // --- update ---
    input_.update();
    if (currentMap_ == MapId::Demo) {
        npc_->update(input_, *activeMap_);
    }
    if (menu_.isOpen()) {
        // Pause menu is up: freeze the world and just navigate the list.
        if (IsKeyPressed(KEY_DOWN)) menu_.moveDown();
        if (IsKeyPressed(KEY_UP)) menu_.moveUp();
        if (IsKeyPressed(KEY_ESCAPE)) {
            menu_.close();
        } else if (IsKeyPressed(KEY_SPACE)) {
            if (menu_.selectedIndex() == 1) { // "Quit"
                quit_ = true;
            }
            menu_.close();
        }
    } else if (message_.isActive()) {
        // Dialogue is up: freeze player movement, drive the typewriter, and
        // let Space either fast-forward the reveal or close the box.
        message_.update();
        if (IsKeyPressed(KEY_SPACE)) {
            message_.confirm();
        }
    } else {
        player_->update(input_, *activeMap_);
        if (currentMap_ == MapId::Demo) {
            if (IsKeyPressed(KEY_SPACE) && isAdjacent(*player_, *npc_)) {
                message_.show("Hi! Welcome to my portfolio.");
            }
            if (atTile(*player_, demomap::DOOR_X, demomap::DOOR_Y)) {
                currentMap_ = MapId::Hub;
                activeMap_ = hubMap_.get();
                player_->warpTo(hubmap::SPAWN_X, hubmap::SPAWN_Y);
                message_.show("Welcome to the Hub! This is where my projects, work, and skills will live.");
            }
        } else if (atTile(*player_, hubmap::W / 2, hubmap::H - 1)) {
            currentMap_ = MapId::Demo;
            activeMap_ = demoMap_.get();
            player_->warpTo(demomap::DOOR_RETURN_X, demomap::DOOR_RETURN_Y);
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            menu_.open({"Resume", "Quit"});
        }
    }
    const int camX = player_->x();
    const int camY = player_->y();

    // --- draw world at native 352x198 ---
    BeginTextureMode(target_);
    ClearBackground(Color{40, 50, 40, 255});
    activeMap_->drawLower(camX, camY);
    player_->draw(camX, camY);
    if (currentMap_ == MapId::Demo) {
        npc_->draw(camX, camY);
    }
    activeMap_->drawUpper(camX, camY);
    message_.draw();
    menu_.draw();
    EndTextureMode();

    // --- blit scaled to the window (letterboxed, nearest-neighbor) ---
    BeginDrawing();
    ClearBackground(BLACK);
    const float scale = std::fmin(GetScreenWidth() / (float)grid::SCREEN_W,
                                  GetScreenHeight() / (float)grid::SCREEN_H);
    const float destW = grid::SCREEN_W * scale;
    const float destH = grid::SCREEN_H * scale;
    const Rectangle src{0, 0, (float)grid::SCREEN_W, -(float)grid::SCREEN_H};
    const Rectangle dst{(GetScreenWidth() - destW) / 2.0f,
                        (GetScreenHeight() - destH) / 2.0f, destW, destH};
    DrawTexturePro(target_.texture, src, dst, Vector2{0, 0}, 0.0f, WHITE);
    EndDrawing();

    // --- verification screenshot ---
    frameCount_ += 1;
    if (!screenshotPath_.empty() && frameCount_ >= screenshotFrames_) {
        TakeScreenshot(screenshotPath_.c_str());
        quit_ = true;
    }
}
