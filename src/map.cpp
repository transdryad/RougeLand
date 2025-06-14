#include "map.hpp"
#include "libtcod.hpp"
#include <cmath>
#include <random>

Point GameMap::pointInCircle(int radius) {
    double t = 2*M_PI*(this->distrib(this->gen));
    double u = this->distrib(this->gen)+this->distrib(this->gen);
    double r = 0.0;
    if (u > 1) {
	r = 2 - u;
    } else {
	r = u;
    }
    return {radius*r*cos(t), radius*r*sin(t)};
}

GameMap::GameMap() {
    std::random_device rd;
    this->gen = std::mt19937(rd());
    this->distrib = std::uniform_int_distribution<>(0, 1);
    

    for (const auto& row : this->tiles) {
	for (MapTile tile : row) {
	    tile.solid = false;
	    tile.walkable = true;
	    tile.character = "";
	    tile.color = {0, 0, 0};
	}
    }
}

bool GameMap::isWalkable(int x, int y) {
    return this->tiles[x][y].walkable;
}

bool GameMap::isSolid(int x, int y) {
    return this->tiles[x][y].solid;
}

void GameMap::render(tcod::Console& rconsole) {
    int x = 0;
    int y = 0;
    for (const auto& row : this->tiles) {
	for (MapTile tile : row) {
	    tcod::print(rconsole, {x, y}, tile.character, tile.color, std::nullopt);
	    ++y;
	}
	++x;
    }
}
