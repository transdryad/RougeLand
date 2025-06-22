#pragma once

#include "libtcod.hpp"
#include "spdlog/spdlog.h"
#include <vector>
#include "entity.hpp"
#include <string>
#include "map.hpp"


class Game {
    public:
        std::shared_ptr<spdlog::logger> logger;
        std::vector<Entity> entities;
        GameMap map;
        TCODRandom* randomizer = TCODRandom::getInstance();
        tcod::Console console;
        tcod::Context context;
        tcod::Tileset tileset = tcod::load_tilesheet("src/tileset.png", {16, 16}, tcod::CHARMAP_CP437);
        void handle_events();
        void render();
        void spawn(std::string character, tcod::ColorRGB color, bool ai, int maxHp);
        Game(int argc, char* argv[]);
};
