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
	MapTile tiles[80][45];
	std::mt19937 gen;
	std::uniform_int_distribution<> distrib;
	bool isWalkable(int x, int y);
	bool isSolid(int x, int y);
	void render(tcod::Console& rconsole);
	Point pointInCircle(int radius);
};
