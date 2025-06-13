#pragma once

#include <string>
#include "libtcod.hpp"

class Entity {
    public:
	int x;
	int y;
	std::string character;
	tcod::ColorRGB color;
	Entity(int x, int y, std::string character, tcod::ColorRGB color);
	void move(int dx, int dy);
	void render(tcod::Console& rconsole);
};
