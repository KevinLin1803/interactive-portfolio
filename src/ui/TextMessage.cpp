#include "TextMessage.hpp"
#include "raylib.h"

void TextMessage::show(const std::string& message) {
    message_ = message;
    revealedChars_ = 0;
    frameCounter_ = 0;
    active_ = true;
}

void TextMessage::update() {
    if (!active_ || revealedChars_ >= static_cast<int>(message_.size())) return;
    frameCounter_ += 1;
    if (frameCounter_ >= kFramesPerChar) {
        frameCounter_ = 0;
        revealedChars_ += 1;
    }
}

void TextMessage::confirm() {
    if (!active_) return;
    if (revealedChars_ < static_cast<int>(message_.size())) {
        revealedChars_ = static_cast<int>(message_.size()); // reveal instantly
    } else {
        active_ = false;
    }
}

void TextMessage::draw() const {
    if (!active_) return;
    constexpr int boxX = 8, boxY = 140, boxW = 336, boxH = 50;
    DrawRectangle(boxX, boxY, boxW, boxH, Color{20, 20, 30, 230});
    DrawRectangleLines(boxX, boxY, boxW, boxH, WHITE);
    const std::string visible = message_.substr(0, revealedChars_);
    DrawText(visible.c_str(), boxX + 8, boxY + 8, 10, WHITE);
}
