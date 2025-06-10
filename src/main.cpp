#include <iostream>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>

using namespace ftxui;

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
    Element document = hbox({
        text("left")   | border,
        text("Hello Worlds!") | border | flex,
        text("right")  | border,});
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
}
