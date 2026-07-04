#include "KeyboardMenu.hpp"
#include "raylib.h"

void KeyboardMenu::open(std::vector<std::string> options) {
    options_ = std::move(options);
    selected_ = 0;
    open_ = true;
}

void KeyboardMenu::close() { open_ = false; }

void KeyboardMenu::moveUp() {
    if (!open_ || options_.empty()) return;
    selected_ = (selected_ - 1 + static_cast<int>(options_.size())) % static_cast<int>(options_.size());
}

void KeyboardMenu::moveDown() {
    if (!open_ || options_.empty()) return;
    selected_ = (selected_ + 1) % static_cast<int>(options_.size());
}

void KeyboardMenu::draw() const {
    if (!open_) return;
    constexpr int lineH = 14;
    const int boxW = 120;
    const int boxH = 10 + static_cast<int>(options_.size()) * lineH;
    const int boxX = 352 - boxW - 8;
    const int boxY = 8;
    DrawRectangle(boxX, boxY, boxW, boxH, Color{20, 20, 30, 230});
    DrawRectangleLines(boxX, boxY, boxW, boxH, WHITE);
    for (int i = 0; i < static_cast<int>(options_.size()); ++i) {
        const int textY = boxY + 5 + i * lineH;
        if (i == selected_) {
            DrawText(">", boxX + 6, textY, 10, WHITE);
        }
        DrawText(options_[i].c_str(), boxX + 18, textY, 10, WHITE);
    }
}
