#include "GameMap.hpp"

GameMap::GameMap(Texture2D lower, Texture2D upper)
    : lower_(lower), upper_(upper) {}

void GameMap::addWall(int gridX, int gridY) {
    walls_.insert(key(gridX * grid::TILE, gridY * grid::TILE));
}

bool GameMap::isCollided(int x, int y, grid::Dir dir) const {
    const grid::Vec2i next = grid::nextPosition(x, y, dir);
    return walls_.count(key(next.x, next.y)) > 0;
}

void GameMap::drawLower(int camX, int camY) const {
    DrawTexture(lower_, grid::CAM_OFFSET_X - camX, grid::CAM_OFFSET_Y - camY, WHITE);
}

void GameMap::drawUpper(int camX, int camY) const {
    if (upper_.id != 0) {
        DrawTexture(upper_, grid::CAM_OFFSET_X - camX, grid::CAM_OFFSET_Y - camY, WHITE);
    }
}
