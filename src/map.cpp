#include "map.hpp"

#include "libtcod.hpp"
#include <random>
#include <fmt/core.h>
#include <cstdlib>
#include <cmath>
#include "entity.hpp"
#include "item.hpp"

Hallway::Hallway(const int x1, const int x2, const int y1, const int y2, const int corner_x, const int corner_y) {
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->corner_x = corner_x;
    this->corner_y = corner_y;
}

void Hallway::draw(GameMap& map) const {
    int x = x1;
    int y = y1;
    TCODLine::init(x, y, corner_x, corner_y);

    do {
        map.tiles[x][y] = {false, true, false, ".", {255, 255, 255}};
    } while (!TCODLine::step(&x, &y));

    x = x2;
    y = y2;
    TCODLine::init(this->corner_x, this->corner_y, x, y);

    do {
        map.tiles[x][y] = {false, true, false, ".", {255, 255, 255}};
    } while (!TCODLine::step(&x, &y));
}

RectRoom GameMap::roomFromNode(TCODBsp* node) const {
    TCODBsp* originalNode = node;
    for (auto & room : rooms) {
        node = originalNode;
        while (node->level < room.node->level) {
            node = node->getLeft();
            if (node == nullptr) break;
        }
        if (room.node == node) return room;
    }
    exit(1);
}

RectRoom::RectRoom(const int x, const int y, const int xx, const int yy, TCODBsp* node) {
    this->x = x;
    this->y = y;
    this->xx = xx;
    this->yy = yy;
    this->node = node;
    this->cx = static_cast<int>(std::round(x + xx) / 2);
    this->cy = static_cast<int>(std::round(y + yy) / 2);
}

void RectRoom::draw(GameMap& map) const {
    printf("Drawing room: %d, %d to %d, %d", x,y, xx, yy);
    for (int i = y; i < yy; i++) {
        for (int j = x; j < xx; j++) {
            map.tiles[j][i] = {false, true, false, ".", {255, 255, 255}};
        }
    }
    printf("Done\n");
}

void GameMap::drawInBounds(const int x, const int y, const int nx, const int ny, TCODBsp* node) {
    //printf("Carving");
    TCODRandom* random = TCODRandom::getInstance();
    const int min_x = x;
    const int min_y = y;
    const int max_x = nx;
    const int max_y = ny;
    const int top_x = random->getInt(min_x + 1, abs(max_x - 4));
    const int top_y = random->getInt(min_y + 1, abs(max_y - 4));
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
    int inum = random->getInt(0, 3);
    for (int i = 0; i < inum; i++) {
        int ix = random->getInt(top_x, bottom_x);
        int iy = random->getInt(top_y, bottom_y);
        auto itype = static_cast<ItemType>(random->getInt(0, 2));
        switch (itype) {
            case SWORD:
                items.emplace_back(Item(itype, ix, iy, "!", {204, 204, 204}, *this)); break;
            case HELMET:
                items.emplace_back(Item(itype, ix, iy, "^", {119, 119, 119}, *this)); break;
            default: break;
        }
    }
    rooms.emplace_back(top_x, top_y, bottom_x, bottom_y, node);
    printf("done\n");
}

void GameMap::connect(TCODBsp* left, TCODBsp* right) { // Partially from RogueLikeTuroials v2022
    const RectRoom lroom = this->roomFromNode(left);
    const RectRoom rroom = this->roomFromNode(right);
    const int x1 = lroom.cx;
    const int x2 = rroom.cx;
    const int y1 = lroom.cy;
    const int y2 = rroom.cy;
    int corner_x;
    int corner_y;
    if (TCODRandom::getInstance()->getFloat(0, 1) < 0.5) {
        corner_x = x2;
        corner_y = y1;
    } else {
        corner_x = x1;
        corner_y = y2;
    }
    halls.emplace_back(x1, x2, y1, y2, corner_x, corner_y);
}

class NodeCallback final : public ITCODBspCallback {
    public:
        GameMap& mapref;
        explicit NodeCallback(GameMap& map): mapref(map) {}

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
    wipe();

    bsptree = TCODBsp(0, 0, 80, 45);
    bsptree.splitRecursive(TCODRandom::getInstance(), 4, 8, 8, 1.5, 1.5);
    bsptree.traversePostOrder(new NodeCallback(*this), nullptr);

    compute();
}


bool GameMap::isWalkable(const int x, const int y) const {
    return tiles[x][y].walkable;
}

bool GameMap::isSolid(const int x, const int y) const {
    return tiles[x][y].solid;
}

void GameMap::render(tcod::Console& rconsole) {
    for (int y = 0; y < 45; y++) {
        for (int x = 0; x < 80; x++) {
            //printf("%s", tiles[x][y].character.c_str());
            if (fmap.isInFov(x, y)) {
                tcod::print(rconsole, {x, y}, tiles[x][y].character, tiles[x][y].color, std::nullopt);
                tiles[x][y].explored = true;
            } else if (this->tiles[x][y].explored) {
                tcod::print(rconsole, {x, y}, tiles[x][y].character, {{75, 75, 75}}, std::nullopt);
            } else {
                tcod::print(rconsole, {x, y}, " ", {{255, 255, 255}}, std::nullopt);
            }
        }
        //printf("\n");
    }
    //printf("\n");
}

void GameMap::wipe() {
    for (int y = 0; y < 45; y++) {
        for (auto & tile : tiles) {
            tile[y] = {true, false, false, "#", {255, 255, 255}};
        }
    }
}

void GameMap::compute() {
    this->wipe();
    for (RectRoom& room : rooms) {
        room.draw(*this);
    }
    for (Hallway& hall : halls) {
        hall.draw(*this);
    }
    for (int y = 0; y < 45; y++) {
        for (int x = 0; x < 80; x++) {
            MapTile& tile = tiles[x][y];
            fmap.setProperties(x, y, !tile.solid, tile.walkable);
        }
    }
}

GameMap::GameMap(std::vector<Creature>& entities, std::vector<Item>& items) : fmap(80, 45), entities(entities), items(items), tiles{} {
}
