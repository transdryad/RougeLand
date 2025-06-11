project('RougeLand', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

spdlog = dependency('spdlog')
fmt = dependency('fmt')

RougeLand = executable('RougeLand', 'src/main.cpp', install : true, dependencies: [fmt, spdlog])
