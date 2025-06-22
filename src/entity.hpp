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
        int attack;
        bool living;
        bool acted;
        bool player;
        std::string character;
        tcod::ColorRGB color;
        GameMap& map;
        Entity(int x, int y, const std::string &character, tcod::ColorRGB color, bool ai, int maxHp, bool player, GameMap& map);
        void move(int dx, int dy);
        void damage(int damage);
        void render(tcod::Console& rconsole);
        void update();
        void spawn();
};
