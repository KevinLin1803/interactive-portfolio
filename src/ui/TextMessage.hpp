#pragma once
#include <string>

// A Pokemon-style dialogue box: reveals a message one character at a time
// (typewriter effect), then waits for confirmation to close. Drawn directly
// in the native 352x198 screen space (UI, not affected by the world camera).
class TextMessage {
public:
    void show(const std::string& message);
    void update();
    // Confirm/advance input: skips to the full message if still revealing,
    // otherwise closes the box.
    void confirm();
    void draw() const;

    bool isActive() const { return active_; }

private:
    std::string message_;
    int revealedChars_ = 0;
    int frameCounter_ = 0;
    bool active_ = false;

    static constexpr int kFramesPerChar = 2; // typewriter speed
};
