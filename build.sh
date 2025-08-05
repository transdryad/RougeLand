#!/bin/bash
rm -rf buildDir
rm -rf subprojects/libtcod/
meson setup --prefer-static --default-library=static buildDir
meson compile -C buildDir
