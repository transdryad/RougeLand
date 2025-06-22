#include "map.hpp"

#include "libtcod.hpp"
#include <random>
#include <fmt/core.h>
#include <cstdlib>
#include <cmath>
#include "entity.hpp"

Hallway::Hallway(int x1, int x2, int y1, int y2, int corner_x, int corner_y) {
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->corner_x = corner_x;
    this->corner_y = corner_y;
}

void Hallway::draw(GameMap& map) {
    int x = this->x1;
    int y = this->y1;
    TCODLine::init(x, y, this->corner_x, this->corner_y);
    do {
	map.tiles[x][y] = {false, true, false, ".", {255, 255, 255}};
    } while (!TCODLine::step(&x, &y));
    x = this->x2;
    y = this->y2;
    TCODLine::init(this->corner_x, this->corner_y, x, y);
    do {
	map.tiles[x][y] = {false, true, false, ".", {255, 255, 255}};
    } while (!TCODLine::step(&x, &y));
}

RectRoom GameMap::roomFromNode(TCODBsp* node) {
    TCODBsp* originalNode = node;
    for (auto & room : this->rooms) {
        node = originalNode;
	    while (node->level < room.node->level) {
	        node = node->getLeft();
	        if (node == nullptr) break;
	    }
	    if (room.node == node) return room;
    }
    exit(1);
}

RectRoom::RectRoom(int x, int y, int xx, int yy, TCODBsp* node) {
    this->x = x;
    this->y = y;
    this->xx = xx;
    this->yy = yy;
    this->node = node;
    this->cx = static_cast<int>(std::round(x + xx) / 2);
    this->cy = static_cast<int>(std::round(y + yy) / 2);
}

void RectRoom::draw(GameMap& map) {
    printf("Drawing room: %d, %d to %d, %d", this->x, this->y, this->xx, this->yy);
    for (int i = this->y; i < this->yy; i++) {
	for (int j = this->x; j < this->xx; j++) {
	    map.tiles[j][i] = {false, true, false, ".", {255, 255, 255}};
	}
    }
    printf("Done\n");
}

void GameMap::drawInBounds(int x, int y, int nx, int ny, TCODBsp* node) {
    //printf("Carving");
    TCODRandom* random = TCODRandom::getInstance();
    int min_x = x;
    int min_y = y;
    int max_x = nx;
    int max_y = ny;
    int top_x = random->getInt(min_x + 1, abs(max_x - 4));
    int top_y = random->getInt(min_y + 1, abs(max_y - 4));
    int bottom_x = top_x + random->getInt(3, 25, 24);
    int bottom_y = top_y + random->getInt(3, 25, 24);
    if (bottom_x >= max_x) bottom_x = max_x - 1;
    if (bottom_y >= max_y) bottom_y = max_y - 1;
    printf("Drawing room from %d,%d to %d,%d.\n", top_x, top_y, bottom_x, bottom_y);
    if (bottom_x < top_x) {
        printf("Bottom x %d is less than top %d, ie rand was %d.\n", bottom_x, top_x, bottom_x - top_x);
        exit(1);
    }
    if (bottom_y < top_y) {
        printf("Bottom y %d is less than top %d, ie rand was %d.\n", bottom_y, top_y, bottom_y - top_y);
        exit(1);
    }
    RectRoom room(top_x, top_y, bottom_x, bottom_y, node);
    this->rooms.push_back(room);
}

void GameMap::connect(TCODBsp* left, TCODBsp* right) { // Partially from RogueLikeTuroials v2022
    RectRoom lroom = this->roomFromNode(left);
    RectRoom rroom = this->roomFromNode(right);
    int x1 = lroom.cx;
    int x2 = rroom.cx;
    int y1 = lroom.cy;
    int y2 = rroom.cy;
    int corner_x;
    int corner_y;
    if (TCODRandom::getInstance()->getFloat(0, 1) < 0.5) {
	corner_x = x2;
	corner_y = y1;
    } else {
	corner_x = x1;
	corner_y = y2;
    }
    Hallway hall(x1, x2, y1, y2, corner_x, corner_y);
    this->halls.push_back(hall);
}

class NodeCallback final : public ITCODBspCallback {
public:
    GameMap& mapref;
    NodeCallback(GameMap& map): mapref(map) {}

    bool visitNode(TCODBsp *node, void *userData) override {
        if (!node->isLeaf()) {
	    TCODBsp* left = node->getLeft();
	    TCODBsp* right = node->getRight();
	    mapref.connect(left, right);
            return true;
        }
        printf("node pos %d,%d to %d,%d level %d: ", node->x,node->y,node->w + node->x, node->h + node->y, node->level);
        mapref.drawInBounds(node->x,node->y,node->w + node->x, node->h + node->y, node);
	return true;
    }
};

void GameMap::init() {
    this->wipe();

    this->bsptree = TCODBsp(0, 0, 80, 45);
    this->bsptree.splitRecursive(TCODRandom::getInstance(), 4, 8, 8, 1.5, 1.5);
    this->bsptree.traversePostOrder(new NodeCallback(*this), nullptr);

    this->compute();
}


bool GameMap::isWalkable(int x, int y) const {
    return this->tiles[x][y].walkable;
}

bool GameMap::isSolid(int x, int y) const {
    return this->tiles[x][y].solid;
}

void GameMap::render(tcod::Console& rconsole) {
    for (int y = 0; y < 45; y++) {
        for (int x = 0; x < 80; x++) {
        printf("%s", tiles[x][y].character.c_str());
        if (this->fmap.isInFov(x, y)) {
		tcod::print(rconsole, {x, y}, this->tiles[x][y].character, this->tiles[x][y].color, std::nullopt);
		this->tiles[x][y].explored = true;
	    } else if (this->tiles[x][y].explored) {
		tcod::print(rconsole, {x, y}, this->tiles[x][y].character, {{75, 75, 75}}, std::nullopt);
	    } else {
		tcod::print(rconsole, {x, y}, " ", {{255, 255, 255}}, std::nullopt);
	    }
        }
        printf("\n");
    }
    printf("\n");
}

void GameMap::wipe() {
    for (int y = 0; y < 45; y++) {
	for (auto & tile : this->tiles) {
	    tile[y] = {true, false, false, "#", {255, 255, 255}};
	}
    }
}

void GameMap::compute() {
    this->wipe();
    for (RectRoom& room : this->rooms) {
	room.draw(*this);
    }
    for (Hallway& hall : this->halls) {
	hall.draw(*this);
    }
    for (int y = 0; y < 45; y++) {
	for (int x = 0; x < 80; x++) {
	    MapTile& tile = this->tiles[x][y];
	    this->fmap.setProperties(x, y, !tile.solid, tile.walkable);
	}
    }
}

GameMap::GameMap(std::vector<std::reference_wrapper<Entity>>& entities) : fmap(80, 45), entities(entities) {
    return;
}
