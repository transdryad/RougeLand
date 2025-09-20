#!/bin/bash
rm -rf build
rm -rf subprojects/libtcod/
meson setup --prefer-static --default-library=static build
meson compile -C build
