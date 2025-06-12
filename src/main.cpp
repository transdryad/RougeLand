#include "libtcod.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>
#include <fmt/core.h>
#include <cstdio>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Player{
    public:
	int x = 0;
	int y = 0;
	std::string character = "@";
	void move(int dx, int dy) {
	    x += dx;
	    y += dy;
	}
	void render(tcod::Console& rconsole) {
	    tcod::print(rconsole, {x, y}, character, {{255, 255, 255}}, std::nullopt);
	}
};

Player player;
auto logger = spdlog::basic_logger_mt("file", "log.txt");

int main(const int argc, char* argv[]) {
    try {
        //auto logger = spdlog::basic_logger_mt("file", "log.txt");
        spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %v");
    } catch (const spdlog::spdlog_ex &ex) {
        fmt::print(stderr, "Log init failed: {}\n", ex.what());
        return EXIT_FAILURE;
    }
    logger->info("Welcome to RougeLand!");
    logger->info("Initializing libTCOD.");

    auto tileset = tcod::load_tilesheet("src/tileset.png",{16, 16}, tcod::CHARMAP_CP437);

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

    player.x = 39;
    player.y = 21;

    logger->info("Entering main loop.");
    while (true) {
        console.clear();
        tcod::print(console, {0, 0}, "Testing RougeLand!", std::nullopt, std::nullopt);
	player.render(console);
        context.present(console);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            context.convert_event_coordinates(event);
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    return EXIT_SUCCESS;
		case SDL_EVENT_KEY_DOWN:
		    switch (event.key.scancode) {
			case SDL_SCANCODE_W:
			    player.move(0, -1); break;
			case SDL_SCANCODE_A:
			    player.move(-1, 0); break;
			case SDL_SCANCODE_S:
			    player.move(0, 1); break;
			case SDL_SCANCODE_D:
			    player.move(1, 0); break;
			default:
			    break;
		   } 
            }
        }
    }

}
