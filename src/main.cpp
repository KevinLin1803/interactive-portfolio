#include "Game.hpp"
#include "raylib.h"
#include <cstdlib>
#include <memory>
#include <string>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace {
std::unique_ptr<Game> g_game;
void updateDrawFrame() { g_game->frame(); }
} // namespace

int main(int argc, char** argv) {
    std::string screenshotPath;
    int screenshotFrames = 90;

    // --screenshot <path> [--frames <n>] : used by the verification loop.
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--screenshot" && i + 1 < argc) {
            screenshotPath = argv[++i];
        } else if (arg == "--frames" && i + 1 < argc) {
            screenshotFrames = std::atoi(argv[++i]);
        }
    }

    g_game = std::make_unique<Game>(screenshotPath, screenshotFrames);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(updateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose() && !g_game->shouldQuit()) {
        updateDrawFrame();
    }
    g_game.reset(); // triggers window/texture cleanup before exit
#endif
    return 0;
}
