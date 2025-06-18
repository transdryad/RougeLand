#include "map.hpp"

#include "libtcod.hpp"
#include <random>
#include <fmt/base.h>
#include <cstdlib>

RectRoom::RectRoom(int x, int y, int xx, int yy) {
    this->x = x;
    this->y = y;
    this->xx = xx;
    this->yy = yy;
}

void RectRoom::draw(GameMap* map) {
    printf("Drawing room: %d, %d to %d, %d", this->x, this->y, this->xx, this->yy);
    for (int i = this->y; i < this->yy; i++) {
	for (int j = this->x; j < this->xx; j++) {
	    map->tiles[j][i] = {false, true, " ", {255, 255, 255}};
	}
    }
    printf("Done\n");
}

int GameMap::distance(int x, int y, int tx, int ty) {
    return abs(tx - x) + abs(ty - y);
}

void GameMap::drawInBounds(int nx, int ny, int w, int h) {
    //printf("Carving");
    TCODRandom* random = TCODRandom::getInstance();
    int min_x = nx;
    int min_y = ny;
    int max_x = nx + w;
    int max_y = ny + h;
    int top_x = random->getInt(min_x + 1, max_x - 4);
    int top_y = random->getInt(min_y + 1, max_y - 4);
    int bottom_x = top_x + random->getInt(3, 25, 16);
    int bottom_y = top_y + random->getInt(3, 25, 16);
    if (bottom_x >= w) bottom_x = max_x - 1;
    if (bottom_y >= h) bottom_y = max_y - 1;
    printf("Drawing room from %d,%d to %d,%d.\n", top_x, top_y, bottom_x, bottom_y);
    if (bottom_x < top_x) exit(1);
    if (bottom_y < top_y) exit(1);
    RectRoom room(top_x, top_y, bottom_x, bottom_y);
    this->rooms.push_back(room);
    //int otop_x = top_x;
    //for (; top_y < bottom_y; top_y++) {
      //  top_x = otop_x;
        //for (; top_x < bottom_x; top_x++) {
          //  this->tiles[top_x][top_y] = {false, true, " ", {255,255,255}};
        //}
    //}
}

void GameMap::connect(const TCODBsp* left, const TCODBsp* right) {
    int distances[left->x + left->w][left->y + left->h][right->x + right->w][right->y + right->h];
    int s[] = {0, 0, 0, 0};
    distances[0][0][0][0] = 500;
    for (int x = left->w; x < left->x + left->w; x++) {
        for (int y = left->y; y < left->y + left->h; y++) {
            for (int xx = right->x; x < right->x + right->w; xx++) {
                for (int yy = right->y; y < right->y + right->h; yy++) {
                    if (this->isWalkable(x, y) && this->isWalkable(xx, yy) && !this->isSolid(x, y) && !this->isSolid(xx, yy)) {
                        distances[x][y][xx][yy] = this->distance(x, y, xx, yy);
                        if (distances[x][y][xx][yy] < distances[s[0]][s[1]][s[2]][s[3]]) {
                            s[0] = x;
                            s[1] = y;
                            s[2] = xx;
                            s[3] = yy;
                        }
                    }
                }
            }
        }
    }
    printf("Shortest path between rooms: %d,%d to %d,%d.\n", s[0], s[1], s[2], s[3]);
}

class NodeCallback final : public ITCODBspCallback {
public:
    GameMap& mapref;
    NodeCallback(GameMap& map): mapref(map) {}

    bool visitNode(TCODBsp *node, void *userData) override {
        if (!node->isLeaf()) {
	    TCODBsp* left = node->getLeft();
	    TCODBsp* right = node->getRight();
	    //mapref.connect(left, right);
            return true;
        }
        printf("node pos %d,%d to %d,%d level %d: ", node->x,node->y,node->w + node->x, node->h + node->y, node->level);
        mapref.drawInBounds(node->x,node->y,node->w,node->h);
	return true;
    }
};

GameMap::GameMap() {
    this->wipe();
     
    this->bsptree = TCODBsp(0, 0, 80, 45);
    this->bsptree.splitRecursive(TCODRandom::getInstance(), 4, 3, 3, 1.5, 1.5);
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
            tcod::print(rconsole, {x, y}, this->tiles[x][y].character, this->tiles[x][y].color, std::nullopt);
            //printf(this->tiles[x][y].character.c_str());
        }
        //printf("%d\n", y);
    }
    //printf("\n");
}

void GameMap::wipe() {
    for (int y = 0; y < 45; y++) {
	for (auto & tile : this->tiles) {
	    tile[y] = {true, false, "#", {255, 255, 255}};
	}
    }
}

void GameMap::compute() {
    this->wipe();
    for (RectRoom& room : this->rooms) {
	room.draw(this);	
    }
}
