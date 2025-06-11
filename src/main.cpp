#include "libtcod.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>
#include <fmt/core.h>
#include <cstdio>

int main() {
    std::shared_ptr<spdlog::logger> logger;
    try {
        logger = spdlog::basic_logger_mt("file", "log.txt");
        spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %v");
    } catch (const spdlog::spdlog_ex &ex) {
        fmt::print(stderr, "Log init failed: {}\n", ex.what());
        return EXIT_FAILURE;
    }
    logger->info("Welcome to RougeLand!");
    logger->info("Initializing libtcod.");
    TCODConsole::initRoot(80,50,"RougeLand",false);
    while (!TCODConsole::isWindowClosed()) {
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,null);
	TCODConsole::root->clear();
	TCODConsole::root->putChar(40,25,'@');
	TCODConsole::flush();
    }
    return EXIT_SUCCESS;
}
