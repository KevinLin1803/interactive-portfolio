#include "Game.hpp"
#include "DemoMap.hpp"
#include <cmath>

#if !defined(PLATFORM_WEB)
#include <cstdlib> // std::exit path helpers not needed, kept minimal
#endif

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
    map_ = std::make_unique<GameMap>(lower, upper);

    // Build collisions from the shared, deterministic map definition.
    for (int gy = 0; gy < demomap::H; ++gy) {
        for (int gx = 0; gx < demomap::W; ++gx) {
            if (demomap::isWall(demomap::tileAt(gx, gy))) {
                map_->addWall(gx, gy);
            }
        }
    }

    const Texture2D hero = assets_.load("assets/characters/hero.png");
    const Texture2D shadow = assets_.load("assets/characters/shadow.png");
    player_ = std::make_unique<Person>(demomap::SPAWN_X, demomap::SPAWN_Y,
                                       Sprite(hero, shadow), /*isPlayer=*/true);
}

Game::~Game() {
    UnloadRenderTexture(target_);
    CloseWindow();
}

void Game::frame() {
    // --- update ---
    input_.update();
    player_->update(input_, *map_);
    const int camX = player_->x();
    const int camY = player_->y();

    // --- draw world at native 352x198 ---
    BeginTextureMode(target_);
    ClearBackground(Color{40, 50, 40, 255});
    map_->drawLower(camX, camY);
    player_->draw(camX, camY);
    map_->drawUpper(camX, camY);
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
