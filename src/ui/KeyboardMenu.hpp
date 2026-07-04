#pragma once
#include <string>
#include <vector>

// A simple pause menu: a vertical list of text options navigated with
// up/down and confirmed with Space, drawn in native 352x198 screen space
// (UI, not affected by the world camera).
class KeyboardMenu {
public:
    void open(std::vector<std::string> options);
    void close();
    void moveUp();
    void moveDown();
    void draw() const;

    bool isOpen() const { return open_; }
    int selectedIndex() const { return selected_; }

private:
    std::vector<std::string> options_;
    int selected_ = 0;
    bool open_ = false;
};
