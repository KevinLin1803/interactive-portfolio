#pragma once
#include "Grid.hpp"
#include <vector>

// Mirrors the JS DirectionInput: keeps a stack of held directions so the most
// recently pressed key wins, matching the feel of the original engine.
class DirectionInput {
public:
    void update(); // poll raylib key state each frame
    bool hasDirection() const { return !held_.empty(); }
    grid::Dir direction() const { return held_.front(); }

private:
    std::vector<grid::Dir> held_;
    bool down_[4] = {false, false, false, false}; // previous frame key state
};
