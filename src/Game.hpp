#pragma once
#include "AssetStore.hpp"
#include "DirectionInput.hpp"
#include "GameMap.hpp"
#include "GameObject.hpp"
#include "ui/KeyboardMenu.hpp"
#include "ui/TextMessage.hpp"
#include "raylib.h"
#include <memory>
#include <string>

// Owns the window, the offscreen render target, and the world. `frame()` runs
// one tick+draw and is safe to call from both the native loop and Emscripten's
// main-loop callback.
class Game {
public:
    // If screenshotPath is non-empty, the game saves a screenshot after
    // `screenshotFrames` frames and then requests quit (used for verification).
    explicit Game(std::string screenshotPath = "", int screenshotFrames = 90);
    ~Game();

    void frame();
    bool shouldQuit() const { return quit_; }

private:
    RenderTexture2D target_{};
    AssetStore assets_;
    std::unique_ptr<GameMap> map_;
    std::unique_ptr<Person> player_;
    std::unique_ptr<Person> npc_;
    DirectionInput input_;
    TextMessage message_;
    KeyboardMenu menu_;

    std::string screenshotPath_;
    int screenshotFrames_;
    int frameCount_ = 0;
    bool quit_ = false;
};
