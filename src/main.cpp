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

auto logger = spdlog::basic_logger_mt("file", "log.txt");
std::vector<std::reference_wrapper<Entity>> entities;
GameMap map;
Entity player(39, 21, "@", {255, 255, 255}, false, 12, map);
TCODRandom* randomizer = TCODRandom::getInstance();

void handle_events(tcod::Context& context) {
    SDL_Event event;
    while (!player.acted) {
    SDL_PollEvent(&event);
    context.convert_event_coordinates(event);
    switch (event.type) {
        case SDL_EVENT_QUIT:
            exit(0); break;
	case SDL_EVENT_KEY_DOWN:
	    //printf("KeyPress");
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
		default:
		    break; 
	    }
    }
    }
}

void render(tcod::Console& console, tcod::Context& context) {
    console.clear();
    map.render(console);
    for (Entity& entity : entities) {
	entity.render(console);
    }
    context.present(console);
}

int main(const int argc, char* argv[]) {
    try {
        //auto logger = spdlog::basic_logger_mt("file", "log.txt");
        //spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %v");
    } catch (const spdlog::spdlog_ex &ex) {
        fmt::print(stderr, "Log init failed: {}\n", ex.what());
        return EXIT_FAILURE;
    }
    //logger->info("Welcome to RougeLand!");
    //logger->info("Initializing libTCOD.");

    auto tileset = tcod::load_tilesheet("src/tileset.png", {16, 16}, tcod::CHARMAP_CP437);
    
    //logger->info("Loading Entities in vectors.");

    entities.push_back(player);
    Entity npc(30, 20, "!", {0, 255, 0}, true, 10, map);
    entities.push_back(npc);

    tcod::Console console = tcod::Console{80, 45};
    auto params = TCOD_ContextParams{};

    params.console = console.get();
    params.window_title = "RougeLand";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;
    params.argv = argv;
    params.tileset = tileset.get();

    auto context = tcod::Context(params);

    map.init();

    player.spawn();
    npc.spawn();

    //logger->info("Entering main loop.");

    while (true) {
	render(console, context);

	handle_events(context); // Input event from player/os
	
	for (Entity& entity : entities) { // Do monster ai/check for death
	    entity.update();
	}
    }

}
