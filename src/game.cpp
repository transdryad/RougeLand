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
#include <sstream>

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
                if (ui) {
                    switch (event.key.scancode) {
                        case SDL_SCANCODE_I:
                            fmt::print("UI off.\n");
                            ui = false;
                        default: player.acted = true; break;
                    }
                } else {
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
                            levels[level].compute(); break;
                        case SDL_SCANCODE_I:
                            fmt::print("UI on.\n");
                            ui = true; break;
                        default: break;
                    }
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

void Game::draw_text(tcod::Console& rconsole, std::string text, int x, int y, int length, tcod::ColorRGB topc, tcod::ColorRGB bottomc) {
    int tlength = text.length();
    std::istringstream iss(text);
    std::vector<std::string> words;
    do {
        std::string word;
        iss >> word;
        words.push_back(word);
    } while (iss);
    int current_width = 0;
    int ox = x;
    for (int j = 0; j < words.size(); j++) {
        std::string word = words[j];
        if ((current_width + word.length() + 1) <= length) {
            for (int i = 0; i < word.length(); i++) {
                char ch = word.at(i);
                tcod::print(rconsole, {x, y}, std::string(1, ch), topc, bottomc);
                ++x;
            }
            ++x;
            current_width += word.length() + 1;
        } else { ++y; x = ox; current_width = 0; --j;}
    }
    //fmt::print("{}\n", tlength);
    //int ch = 0;
    //for (int j = 0; j < tlength / static_cast<double>(length); j++) {
        //printf("line");
        //fmt::print("j: {}, tlength: {}, length: {}, t/l: {}", j, tlength, length, tlength/length);
        //for (int i = 0; i < length; i++) {
            //if (ch >= tlength) break;
            //char character = text.at(ch);
            //tcod::print(rconsole, {x + i, y + j}, std::string(1, character), topc, bottomc);
            //++ch;
        //}
    //}
}

void Game::render_ui() {
    console.clear();
    static constexpr std::array<int, 9> LEGEND = {0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23};
    tcod::draw_frame(console, {1, 1, 78, 48}, LEGEND, {{255, 255, 255}}, {{0, 0, 0}}, TCOD_BKGND_DEFAULT, false);
    context.present(console);
}

void Game::new_level() {
    levels.emplace_back(creatures, items, *this);
    ++level;
    levels[level].init();
    creatures[0].spawn();
    creatures[0].level += 1;
}

void Game::render_game() {
    Creature& player = creatures[0];
    console.clear();
    levels[level].render(console);
    for (Creature& entity : creatures) {
        entity.render(console);
    }
    for (Item& item : items) {
        logger->info("Rendering Item");
        item.render(console);
    }

    draw_bar(console, player.hp, player.maxHp, 24, {0, 255, 0}, {255, 0, 0}, 0, 46); //hp
    tcod::print(console, {1, 46}, fmt::format("HP: {}/{}", player.hp, player.maxHp), {{255, 255, 255}}, std::nullopt);

    draw_bar(console, player.xp, 1000, 24, {10, 242, 95}, {140, 166, 109}, 0, 47); //xp
    tcod::print(console, {1, 47}, fmt::format("{}: {}/{}", player.xlevel, player.xp, 5000), {{255, 255, 255}}, std::nullopt);

    if (messages.size() > 0) {
        draw_text(console, messages.front(), 25, 46, 35, {255,255,255}, {0,0,0});
    }

    tcod::print(console, {1, 48}, fmt::format("ATK: {}, AC: {}", player.attack, player.ac), {{255, 255, 255}}, std::nullopt);

    context.present(console);
}

void Game::spawn(const CreatureType etype) {
    TCODRandom* random = TCODRandom::getInstance();
    const int x = random->getInt(0, 80);
    const int y = random->getInt(0, 45);
    switch (etype) {
        case PLAYER:
            creatures.emplace_back(Creature(x, y, "@", {210, 210, 255}, false, 20, true, 50, *this, 2, level));
            fmt::print("Level: {}", level);
            logger->info("Spawning Player"); break;
        case ORC:
            creatures.emplace_back(Creature(x, y, "o", {0, 200, 0}, true, 10, false, 25, *this, 3, level)); break;
        default: break;
    }
    creatures.back().spawn();
}

Game::Game(const int argc, char* argv[]) {
    ui = false;
    try {
        const time_t now = time(nullptr);
        struct tm tstruct{};
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

    level = 0;
    levels.emplace_back(creatures, items, *this);
    levels[level].init();

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
    fmt::print("Computing FOV.\n");
    levels[level].fmap.computeFov(creatures[0].x, creatures[0].y, 10);
    fmt::print("Done.\n");
    messages.push_back("Welcome to RougeLand. I know it's misspelled.");
    //messages.push_back("This is a test message that tests the line wrapping of the message system.");
}

[[noreturn]] void Game::run() {
    while (true) {
        if (ui) {
            render_ui();
        } else {
            render_game();
        }
        handle_events(); // Input event from player/os
        if (!ui) {
            levels[level].fmap.computeFov(creatures[0].x, creatures[0].y, 10);
            if (randomizer->getFloat(0, 1) < 0.3) {
                spawn(ORC);
            }
            for (Entity& entity : creatures) { // Do monster ai/check for death
                if (levels[level].fmap.isInFov(entity.x, entity.y)) {
                    entity.update();
                }
            }
        } else {
            creatures[0].update();
        }
    }
}

