#pragma once

#include <string>
#include "libtcod.hpp"
#include "map.hpp"

class Entity {
    public:
	int x;
	int y;
	std::string character;
	tcod::ColorRGB color;
	GameMap& map;
	Entity(int x, int y, std::string character, tcod::ColorRGB color, GameMap& map);
	void move(int dx, int dy);
	void render(tcod::Console& rconsole);
};
