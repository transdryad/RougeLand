#include "map.hpp"
#include "libtcod.hpp"

GameMap::GameMap() {
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
