#pragma once
#include "Grid.hpp"
#include "Sprite.hpp"

class GameMap;
class DirectionInput;

// A Person is the C++ equivalent of the JS GameObject + Character: a grid-locked
// actor that walks one tile at a time. The player is a Person with isPlayer set;
// NPCs reuse the same class but drive themselves with a simple idle behaviour
// loop (turning in place) instead of keyboard input.
class Person {
public:
    Person(int x, int y, Sprite sprite, bool isPlayer);

    void update(DirectionInput& input, const GameMap& map);
    void draw(int camX, int camY) const;

    int x() const { return x_; }
    int y() const { return y_; }

private:
    void startWalk(grid::Dir dir, const GameMap& map);
    void updatePosition();
    void updateSprite();
    void updateIdleBehaviour();

    int x_;
    int y_;
    grid::Dir direction_ = grid::Dir::Down;
    int movingProgress_ = 0; // pixels left in the current step (16 -> 0)
    bool isPlayer_;
    Sprite sprite_;

    int idleTimer_ = 0;
    static constexpr int kIdleTurnInterval = 90; // ~1.5s at 60fps between turns
};
