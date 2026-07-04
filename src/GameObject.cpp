#include "GameObject.hpp"
#include "GameMap.hpp"
#include "DirectionInput.hpp"

Person::Person(int x, int y, Sprite sprite, bool isPlayer)
    : x_(x), y_(y), isPlayer_(isPlayer), sprite_(sprite) {}

void Person::update(DirectionInput& input, const GameMap& map) {
    if (movingProgress_ > 0) {
        updatePosition();
    } else if (isPlayer_ && input.hasDirection()) {
        startWalk(input.direction(), map);
    } else if (!isPlayer_) {
        updateIdleBehaviour();
    }
    updateSprite();
}

void Person::updateIdleBehaviour() {
    idleTimer_ += 1;
    if (idleTimer_ >= kIdleTurnInterval) {
        idleTimer_ = 0;
        direction_ = static_cast<grid::Dir>((static_cast<int>(direction_) + 1) % 4);
    }
}

void Person::startWalk(grid::Dir dir, const GameMap& map) {
    direction_ = dir;
    if (map.isCollided(x_, y_, dir)) {
        return; // blocked: face that way but don't move
    }
    movingProgress_ = grid::TILE; // 16px, one tile, at 1px/frame
}

void Person::updatePosition() {
    const grid::Vec2i next = grid::nextPosition(x_, y_, direction_);
    // step one pixel toward the target cell
    if (next.x > x_) x_ += 1; else if (next.x < x_) x_ -= 1;
    if (next.y > y_) y_ += 1; else if (next.y < y_) y_ -= 1;
    movingProgress_ -= 1;
}

void Person::updateSprite() {
    sprite_.setDirection(direction_);
    sprite_.setMoving(movingProgress_ > 0);
    sprite_.update();
}

void Person::draw(int camX, int camY) const {
    sprite_.draw(x_, y_, camX, camY);
}
