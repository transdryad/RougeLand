#include <iostream>
#include "include/libtcod.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>
#include <fmt/core.h>

int main() {
    std::shared_ptr<spdlog::logger> logger;
    try {
        logger = spdlog::basic_logger_mt("file", "log.txt");
        spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %v");
    } catch (const spdlog::spdlog_ex &ex) {
        std::cerr << "Log init failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    logger->info("Welcome to RougeLand!");
    return EXIT_SUCCESS;
}
