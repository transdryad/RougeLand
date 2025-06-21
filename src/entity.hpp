#pragma once

#include <string>
#include "libtcod.hpp"
#include "map.hpp"

class GameMap;

class Entity {
    public:
	int x;
	int y;
	bool ai;
	int hp;
	int maxHp;
	bool living;
	bool acted;
	std::string character;
	tcod::ColorRGB color;
	GameMap& map;
	Entity(int x, int y, std::string character, tcod::ColorRGB color, bool ai, int maxhp, GameMap& map);
	void move(int dx, int dy);
	void render(tcod::Console& rconsole);
	void update();
	void spawn();
};
