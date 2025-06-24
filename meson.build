project('RougeLand', 'cpp',
        version : '1.0.0',
        default_options : ['default_library=static', 'prefer_static=true', 'warning_level=3', 'cpp_std=c++20'])

cmake = import('cmake')
opt_var = cmake.subproject_options()

opt_var.add_cmake_defines({'LIBTCOD_LODEPNG': 'vendored'})
opt_var.add_cmake_defines({'LIBTCOD_UTF8PROC': 'vendored'})
opt_var.add_cmake_defines({'LIBTCOD_STB': 'vendored'})

sub_pro = cmake.subproject('libtcod', options: opt_var)

spdlog = dependency('spdlog')
fmt = dependency('fmt')
libtcod = sub_pro.dependency('libtcod')

add_project_arguments('-g', language : 'cpp')

RougeLand = executable('RougeLand', 'src/main.cpp', 'src/entity.cpp', 'src/map.cpp', 'src/game.cpp', 'src/item.cpp', 'src/creature.cpp', install : true, dependencies: [fmt, spdlog, libtcod])
