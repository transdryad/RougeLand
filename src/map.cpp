#include "map.hpp"

#include <map>

#include "libtcod.hpp"
#include <random>

class NodeCallback final : public ITCODBspCallback {
public:
    GameMap* map;
    NodeCallback(GameMap* map) {
        this->map = map;
    }
    bool visitNode(TCODBsp *node, void *userData) override {
        TCODRandom* random = TCODRandom::getInstance();
        if (node->level != 5) {
            return true;
        }
        int room_width = random->getInt(4, 30);
        int room_height = random->getInt(4, 30);
        int x = random->getInt(node->x, node->x + node->w);
        int y = random->getInt(node->y, node->y + node->h);
        int oldX = x;
        int oldY = y;
        for (; y < room_height + oldY; y++) {
            for (; x < room_width + oldX; x++) {
                map->tiles[x][y].character = " ";
                map->tiles[x][y].solid = false;
                map->tiles[x][y].walkable = true;
                printf("Carving");
            }
        }
        printf("node pos %dx%d size %dx%d level %d\n", node->x,node->y,node->w,node->h,node->level);
    return true;
    }
};

GameMap::GameMap() {
    for (const auto& row : this->tiles) { //fill with walls
        for (MapTile tile : row) {
            tile.solid = true;
            tile.walkable = false;
            tile.character = "#";
            tile.color = {255, 255, 255};
        }
    }

    this->bsptree = TCODBsp(0, 0, 80, 45);
    this->bsptree.splitRecursive(TCODRandom::getInstance(), 5, 3, 3, 1.5, 1.5);
    this->bsptree.traversePostOrder(new NodeCallback(this), nullptr);
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
