#pragma once

#include <string>
#include "libtcod.hpp"

class Entity {
    public:
	int x = 0;
	int y = 0;
	std::string character = "@";
	tcod::ColorRGB fg;
	void move(int dx, int dy);
	void render(tcod::Console& rconsole);
};
