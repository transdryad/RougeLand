#include "map.hpp"

#include <map>

#include "libtcod.hpp"
#include <random>
#include <fmt/base.h>

void GameMap::drawInBounds(int nx, int ny, int w, int h) {
    //printf("Carving");
    TCODRandom* random = TCODRandom::getInstance();
    int min_x = nx;
    int min_y = ny;
    int max_x = nx + w;
    int max_y = ny + h;
    int top_x = random->getInt(min_x, max_x);
    int top_y = random->getInt(min_y, max_y);
    int bottom_x = top_x + random->getInt(1, 15);
    int bottom_y = top_y + random->getInt(1, 15);
    if (bottom_x >= w) bottom_x = w - 1;
    if (bottom_y >= h) bottom_y = h - 1;
    printf("Drawing room from %d,%d to %d,%d.\n", top_x, top_y, bottom_x, bottom_y);
    for (; top_y < bottom_y; top_y++) {
        if (top_y > 44) break;
        for (; top_x < bottom_x; top_x++) {
            if (top_x > 79) break;
            this->tiles[top_x][top_y] = {false, true, " ", {255,255,255}};
        }
    }
}


class NodeCallback final : public ITCODBspCallback {
public:
    GameMap& mapref;
    NodeCallback(GameMap& map): mapref(map) {}

    bool visitNode(TCODBsp *node, void *userData) override {
        if (node->level != 5) {
            return true;
        }
        printf("node pos %d,%d to %d,%d level %d: ", node->x,node->y,node->w + node->x, node->h + node->y,node->level);
        mapref.drawInBounds(node->x,node->y,node->w,node->h);
    return true;
    }
};

GameMap::GameMap() {
    for (int y = 0; y < 45; y++) {
        for (auto & tile : this->tiles) {
            tile[y] = {true, false, "#", {255,255,255}};
        }
    }

    this->bsptree = TCODBsp(0, 0, 80, 45);
    this->bsptree.splitRecursive(TCODRandom::getInstance(), 5, 3, 3, 1.5, 1.5);
    this->bsptree.traversePostOrder(new NodeCallback(*this), nullptr);
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
            tcod::print(rconsole, {x, y}, this->tiles[x][y].character, this->tiles[x][y].color, std::nullopt);
            //printf(this->tiles[x][y].character.c_str());
        }
        //printf("%d\n", y);
    }
    //printf("\n");
}
