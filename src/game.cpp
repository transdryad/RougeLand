#include "game.hpp"

#include "libtcod.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>
#include <fmt/core.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "entity.hpp"
#include <functional>
#include "creature.hpp"
#include "map.hpp"
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <cstring>

void Game::handle_events() {
    //logger->info(fmt::format("Player.acted = %s.\n", player.acted ? "true" : "false"));
    Creature& player = *dynamic_cast<Creature*>(&creatures[0]);
    while (!player.acted) {
        SDL_Event event;
        SDL_PollEvent(&event);
        context.convert_event_coordinates(event);
        switch (event.type) {
            case SDL_EVENT_QUIT:
                exit(0);
            case SDL_EVENT_KEY_DOWN:
                //printf("KeyPress\n");
                switch (event.key.scancode) {
                    case SDL_SCANCODE_W:
                        player.move(0, -1); break;
                    case SDL_SCANCODE_A:
                        player.move(-1, 0); break;
                    case SDL_SCANCODE_S:
                        player.move(0, 1); break;
                    case SDL_SCANCODE_D:
                        player.move(1, 0); break;
                    case SDL_SCANCODE_Q:
                        player.move(-1, -1); break;
                    case SDL_SCANCODE_E:
                        player.move(1, -1); break;
                    case SDL_SCANCODE_Z:
                        player.move(-1, 1); break;
                    case SDL_SCANCODE_C:
                        player.move(1, 1); break;
                    case SDL_SCANCODE_F8:
                        printf("Recomputing world.");
                        map.compute(); break;
                    default: break;
                }
            default: break;
        }
    }
}

void Game::draw_bar(tcod::Console& rconsole, const int curVal, const int maxVal, const int width, tcod::ColorRGB topc, tcod::ColorRGB bottomc, const int x, const int y) {
    const int bar_width = static_cast<double>(curVal) / maxVal * width;
    tcod::draw_rect(rconsole, {x, y, width, 1}, 1, std::nullopt, bottomc);
    if (bar_width > 0) {
        tcod::draw_rect(rconsole, {x, y, bar_width, 1}, 1, std::nullopt, topc);
    }
}

void Game::render() {
    Creature& player = creatures[0];
    console.clear();
    map.render(console);
    for (Creature& entity : creatures) {
        entity.render(console);
    }
    for (Item& item : items) {
        item.render(console);
    }

    draw_bar(console, player.hp, player.maxHp, 24, {0, 255, 0}, {255, 0, 0}, 0, 46); //hp
    tcod::print(console, {1, 46}, fmt::format("HP: {}/{}", player.hp, player.maxHp), {{255, 255, 255}}, std::nullopt);

    draw_bar(console, player.xp, 1000, 24, {10, 242, 95}, {140, 166, 109}, 0, 47); //xp
    tcod::print(console, {1, 47}, fmt::format("{}: {}/{}", player.level, player.xp, 1000), {{255, 255, 255}}, std::nullopt);

    context.present(console);
}

void Game::spawn(const CreatureType etype) {
    TCODRandom* random = TCODRandom::getInstance();
    const int x = random->getInt(0, 80);
    const int y = random->getInt(0, 45);
    switch (etype) {
        case PLAYER:
            creatures.emplace_back(Creature(x, y, "@", {210, 210, 255}, false, 20, true, 50, map)); break;
        case ORC:
            creatures.emplace_back(Creature(x, y, "o", {0, 200, 0}, true, 10, false, 25, map)); break;
        default: break;
    }
    creatures.back().spawn();
}

Game::Game(const int argc, char* argv[]): map(creatures, items, *this) {
    try {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        std::string fname = "logs/";
        fname += buf;
        fname.append("-log.txt");
        logger = spdlog::basic_logger_mt("file", fname);
        spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %v");
    } catch (const spdlog::spdlog_ex &ex) {
        fmt::print(stderr, "Log init failed: {}\n", ex.what());
        exit(EXIT_FAILURE);
    }

    logger->info("Welcome to RougeLand!");
    logger->info("Initializing libTCOD.");

    map.init();

    spawn(PLAYER);
    spawn(ORC);

    console = tcod::Console{80, 50};
    auto params = TCOD_ContextParams{};

    params.console = console.get();
    params.window_title = "RougeLand";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;
    params.argv = argv;
    params.tileset = tileset.get();

    context = tcod::Context(params);

    map.fmap.computeFov(creatures[0].x, creatures[0].y, 10);
}

[[noreturn]] void Game::run() {
    while (true) {
        render();
        handle_events(); // Input event from player/os
        map.fmap.computeFov(creatures[0].x, creatures[0].y, 10);
        if (randomizer->getFloat(0, 1) > 0.01) {
            spawn(ORC);
        }
        for (Entity& entity : creatures) { // Do monster ai/check for death
            if (map.fmap.isInFov(entity.x, entity.y)) {
                entity.update();
            }
        }
    }
}

