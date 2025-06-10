project('RougeLand', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

ftxui = dependency('ftxui')
spdlog = dependency('spdlog')

RougeLand = executable('RougeLand', 'src/main.cpp', install : true, dependencies: [ftxui, spdlog])
