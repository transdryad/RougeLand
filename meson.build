project('RougeLand', 'cpp',
        version : '1.0.0',
        default_options : ['default_library=static', 'warning_level=3', 'cpp_std=c++20'])

cmake = import('cmake')
opt_var = cmake.subproject_options()

opt_var.add_cmake_defines({'LIBTCOD_LODEPNG': 'vendored'})
opt_var.add_cmake_defines({'LIBTCOD_UTF8PROC': 'vendored'})
opt_var.add_cmake_defines({'LIBTCOD_STB': 'vendored'})

sub_pro = cmake.subproject('libtcod', options: opt_var)

spdlog = dependency('spdlog')
fmt = dependency('fmt')
libtcod = sub_pro.dependency('libtcod')

RougeLand = executable('RougeLand', 'src/main.cpp', install : true, dependencies: [fmt, spdlog, libtcod])
