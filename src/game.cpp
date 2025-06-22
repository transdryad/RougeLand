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
#include "map.hpp"

void Game::handle_events() {
    //printf("Player.acted = %s.\n", player.acted ? "true" : "false");
    while (!entities[0].acted) {
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
                        entities[0].move(0, -1); break;
                    case SDL_SCANCODE_A:
                        entities[0].move(-1, 0); break;
                    case SDL_SCANCODE_S:
                        entities[0].move(0, 1); break;
                    case SDL_SCANCODE_D:
                        entities[0].move(1, 0); break;
                    case SDL_SCANCODE_Q:
                        entities[0].move(-1, -1); break;
                    case SDL_SCANCODE_E:
                        entities[0].move(1, -1); break;
                    case SDL_SCANCODE_Z:
                        entities[0].move(-1, 1); break;
                    case SDL_SCANCODE_C:
                        entities[0].move(1, 1); break;
                    case SDL_SCANCODE_F8:
                        printf("Recomputing world.");
                        map.compute(); break;
                    default: break;
                }
            default: break;
        }
    }
}

void Game::render() {
    console.clear();
    map.render(console);
    for (Entity& entity : entities) {
        entity.render(console);
    }
    context.present(console);
}

void Game::spawn(std::string character, tcod::ColorRGB color, bool ai, int maxHp, bool player) {
    TCODRandom* random = TCODRandom::getInstance();
    int x = random->getInt(0, 80);
    int y = random->getInt(0, 45);
    entities.emplace_back(x, y, character, color, ai, maxHp, player, map);
    entities.back().spawn();
}

Game::Game(const int argc, char* argv[]): map(entities) {
    try {
        logger = spdlog::basic_logger_mt("file", "log.txt");
        spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %v");
    } catch (const spdlog::spdlog_ex &ex) {
        fmt::print(stderr, "Log init failed: {}\n", ex.what());
        exit(EXIT_FAILURE);
    }

    logger->info("Welcome to RougeLand!");
    logger->info("Initializing libTCOD.");

    map.init();

    spawn("@", {210, 210, 255}, false, 12, true); //player
    spawn("o", {0, 200, 0}, true, 10, false);

    console = tcod::Console{80, 45};
    auto params = TCOD_ContextParams{};

    params.console = console.get();
    params.window_title = "RougeLand";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;
    params.argv = argv;
    params.tileset = tileset.get();

    context = tcod::Context(params);

    map.fmap.computeFov(entities[0].x, entities[0].y, 10);

    while (true) {
        render();
        handle_events(); // Input event from player/os
        map.fmap.computeFov(entities[0].x, entities[0].y, 10);
        if (randomizer->getFloat(0, 1) > 0.05) {
            spawn("o", {0, 200, 0}, true, 10, false);
        }
        for (Entity& entity : entities) { // Do monster ai/check for death
            if (map.fmap.isInFov(entity.x, entity.y)) {
                entity.update();
            }
        }
    }
}
