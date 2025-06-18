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
	int cx;
	int cy;
	TCODBsp* node;
	RectRoom(int x, int y, int xx, int yy, TCODBsp* node);
	void draw(GameMap& map);
};

class Hallway{
    public:
	Hallway(int x1, int x2, int y1, int y2, int corner_x, int corner_y);
	void draw(GameMap& map);
	int x1;
	int x2;
	int y1;
	int y2;
	int corner_x;
	int corner_y;
};

class GameMap{
    public:
	void init();
	TCODBsp bsptree;
	MapTile tiles[80][45];
	void connect(TCODBsp* left, TCODBsp* right);
	std::vector<RectRoom> rooms;
	std::vector<Hallway> halls;
	bool isWalkable(int x, int y) const;
	bool isSolid(int x, int y) const;
	void render(tcod::Console& rconsole);
	void compute();
	void drawInBounds(int x, int y, int nx, int ny, TCODBsp* node);
	void wipe();
	int distance(int x, int y, int tx, int ty);
	RectRoom roomFromNode(TCODBsp* node);
};
