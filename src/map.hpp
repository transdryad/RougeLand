#pragma once

#include <string>
#include "libtcod.hpp"
#include <random>

struct MapTile{
    bool solid;
    bool walkable;
    std::string character;
    tcod::ColorRGB color;
};

struct Point{
    double x;
    double y;
};

class GameMap{
    public:
    	GameMap();
	TCODBsp bsptree;
	MapTile tiles[80][45];
	void connect(const TCODBsp* left, const TCODBsp* right);
	bool isWalkable(int x, int y) const;
	bool isSolid(int x, int y) const;
	void render(tcod::Console& rconsole);
	void drawInBounds(int x, int y, int w, int h);
	int distance(int x, int y, int tx, int ty);
};
