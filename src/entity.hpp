#pragma once

#include <string>
#include "libtcod.hpp"
#include "map.hpp"
#include <functional>

class GameMap;
class Game;

class Entity {
    public:
        virtual ~Entity() = default;
        int x;
        int y;
        int level;
        std::string character;
        tcod::ColorRGB color;
        std::reference_wrapper<Game> game;
        Entity(int x, int y, const std::string &character, tcod::ColorRGB color, Game& game, int level);
        virtual void render(tcod::Console& rconsole);
        void spawn();
        virtual void update();
};
