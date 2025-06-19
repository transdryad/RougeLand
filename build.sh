#!/bin/bash
rm -rf buildDir
meson setup --prefer-static --default-library=static buildDir
meson compile -C buildDir
