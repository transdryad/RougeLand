#pragma once

#include "libtcod.hpp"
#include "spdlog/spdlog.h"
#include <vector>
#include <string>
#include <list>

#include "creature.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "item.hpp"

enum CreatureType : int;

class Game {
    public:
        std::shared_ptr<spdlog::logger> logger;
        std::vector<Creature> creatures;
        std::vector<Item> items;
        std::list<std::string> messages;
        //GameMap map;
        int level;
        int explored_level;
        std::vector<GameMap> levels;
        bool ui;
        TCODRandom* randomizer = TCODRandom::getInstance();
        tcod::Console console;
        tcod::Context context;
        tcod::Tileset tileset = tcod::load_tilesheet("src/tileset.png", {16, 16}, tcod::CHARMAP_CP437);
        [[noreturn]] void run();
        void handle_events();
        static void draw_bar(tcod::Console& rconsole, int curVal, int maxVal, int width, tcod::ColorRGB topc, tcod::ColorRGB bottomc, int x, int y);
        void down_level();
        void draw_text(tcod::Console& rconsole, std::string text, int x, int y, int length, tcod::ColorRGB topc, tcod::ColorRGB bottomc);
        void new_level();
        void render_game();
        void render_ui();
        void spawn(CreatureType etype);
        void up_level();
        Game(int argc, char* argv[]);
};
