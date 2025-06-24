#pragma once

#include "libtcod.hpp"
#include "spdlog/spdlog.h"
#include <vector>

#include "creature.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "item.hpp"

class Game {
    public:
        std::shared_ptr<spdlog::logger> logger;
        std::vector<Entity> entities;
        std::vector<Item> items;
        GameMap map;
        TCODRandom* randomizer = TCODRandom::getInstance();
        tcod::Console console;
        tcod::Context context;
        tcod::Tileset tileset = tcod::load_tilesheet("src/tileset.png", {16, 16}, tcod::CHARMAP_CP437);
        [[noreturn]] void run();
        void handle_events();
        static void draw_bar(tcod::Console& rconsole, int curVal, int maxVal, int width, tcod::ColorRGB topc, tcod::ColorRGB bottomc, int x, int y);
        void render();
        void spawn(CreatureType etype);
        Game(int argc, char* argv[]);
};
