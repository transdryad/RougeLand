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

void Game::draw_bar(tcod::Console& rconsole, const int curVal, const int maxVal, const int width, tcod::ColorRGB topc, tcod::ColorRGB bottomc, const int x, const int y) {
    const int bar_width = static_cast<double>(curVal) / maxVal * width;
    tcod::draw_rect(rconsole, {x, y, width, 1}, 1, std::nullopt, bottomc);
    if (bar_width > 0) {
        tcod::draw_rect(rconsole, {x, y, bar_width, 1}, 1, std::nullopt, topc);
    }
}

void Game::render() {
    console.clear();
    map.render(console);
    for (Entity& entity : entities) {
        entity.render(console);
    }

    draw_bar(console, entities[0].hp, entities[0].maxHp, 30, {0, 255, 0}, {255, 0, 0}, 0, 46); //hp
    tcod::print(console, {1, 46}, fmt::format("HP: {}/{}", entities[0].hp, entities[0].maxHp), {{255, 255, 255}}, std::nullopt);

    draw_bar(console, entities[0].xp, 1000, 30, {10, 242, 95}, {140, 166, 109}, 0, 47); //xp
    tcod::print(console, {1, 47}, fmt::format("{}: {}/{}", entities[0].level, entities[0].xp, 1000), {{255, 255, 255}}, std::nullopt);

    context.present(console);
}

void Game::spawn(const EntityType etype) {
    TCODRandom* random = TCODRandom::getInstance();
    const int x = random->getInt(0, 80);
    const int y = random->getInt(0, 45);
    switch (etype) {
        case PLAYER:
            entities.emplace_back(Entity(x, y, "@", {210, 210, 255}, false, 20, true, 50, map)); break;
        case ORC:
            entities.emplace_back(Entity(x, y, "o", {0, 200, 0}, true, 10, false, 25, map)); break;
        default: break;
    }
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

    map.fmap.computeFov(entities[0].x, entities[0].y, 10);
}

[[noreturn]] void Game::run() {
    while (true) {
        render();
        handle_events(); // Input event from player/os
        map.fmap.computeFov(entities[0].x, entities[0].y, 10);
        if (randomizer->getFloat(0, 1) > 0.01) {
            spawn(ORC);
        }
        for (Entity& entity : entities) { // Do monster ai/check for death
            if (map.fmap.isInFov(entity.x, entity.y)) {
                entity.update();
            }
        }
    }
}

