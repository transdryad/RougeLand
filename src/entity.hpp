#pragma once

#include <string>
#include "libtcod.hpp"
#include "map.hpp"
#include <functional>

class GameMap;

class Entity {
    public:
        virtual ~Entity() = default;
        int x;
        int y;
        std::string character;
        tcod::ColorRGB color;
        std::reference_wrapper<GameMap> map;
        Entity(int x, int y, const std::string &character, tcod::ColorRGB color, GameMap& map);
        void render(tcod::Console& rconsole);
        void spawn();
        virtual void update();
};
