#pragma once

#include <string>
#include "libtcod.hpp"
#include <random>
#include <vector>
#include <functional>

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

class GameMap;

class RectRoom{
    public:
	int x;
	int y;
	int xx;
	int yy;
	RectRoom(int x, int y, int xx, int yy);
	void draw(GameMap* map);
};

class GameMap{
    public:
    	GameMap();
	TCODBsp bsptree;
	MapTile tiles[80][45];
	void connect(const TCODBsp* left, const TCODBsp* right);
	std::vector<RectRoom> rooms;
	bool isWalkable(int x, int y) const;
	bool isSolid(int x, int y) const;
	void render(tcod::Console& rconsole);
	void compute();
	void drawInBounds(int x, int y, int w, int h);
	void wipe();
	int distance(int x, int y, int tx, int ty);
};
