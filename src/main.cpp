#include "libtcod.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>
#include <fmt/core.h>
#include <cstdio>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(const int argc, char* argv[]) {
    std::shared_ptr<spdlog::logger> logger;
    try {
        logger = spdlog::basic_logger_mt("file", "log.txt");
        spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %v");
    } catch (const spdlog::spdlog_ex &ex) {
        fmt::print(stderr, "Log init failed: {}\n", ex.what());
        return EXIT_FAILURE;
    }
    logger->info("Welcome to RougeLand!");
    logger->info("Initializing libTCOD.");

    auto tileset = tcod::load_tilesheet("src/tileset.png",{16, 16}, tcod::CHARMAP_CP437);

    auto console = tcod::Console{80, 45};
    auto params = TCOD_ContextParams{};

    params.console = console.get();
    params.window_title = "RougeLand";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;
    params.argv = argv;
    params.tileset = tileset.get();

    auto context = tcod::Context(params);

    logger->info("Entering main loop.");
    while (true) {
        console.clear();
        tcod::print(console, {0, 0}, "Testing RougeLand", std::nullopt, std::nullopt);
        context.present(console);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            context.convert_event_coordinates(event);
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    return EXIT_SUCCESS;
            }
        }
    }

}
