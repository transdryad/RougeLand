# RougeLand
RougeLand is a simple, minimalism roguelike game. There are monsters to fight, loot to gather, and potions to increase your health. In an infinite dungeon, what more could you want?

For controls, it uses WASD and QEZC for diagonals (assumes US layout).
### Explanation of Tiles
"#" - Wall

" " - Empty/Unexplored

"^" - Helmet

"!" - Sword

"o" - Orc/Enemy

'lightbulb' - Health Potion
## Installation
If available, download the latest release from releases. Alternatively, compile from source:

RougeLand requires SDL3, ZLIB, Meson, CMake, and a C++ compiler & standard library to be installed and available on the system for compilation.
Clone this git repo, and then cd into it in the terminal.

```bash
meson setup --prefer-static --default-library=static build
meson compile -C build
```
Once compilation completes, the executable will be located inside the build folder, and be named RougeLand.
## Why
I made RougeLand to gain experiece with C++ and game development, and to do so in a graphically unintensive way.
## How
RougeLand relys on three main libraries: libTCOD for rendering, fmt for formatting strings, and spdlog for logging. I made it over the course of several months, working on and off. It was made almost entirely in NeoVim, and uses object-oriented C++. I chose meson for the build system for its simplicity and interoperability. The spritesheet was taken from a list of alternative free spritesheets for dwarf fortress.
## Reflection
During this project, I often struggled with memory usage. I had a lot of segfaults. However, I now feel mostly confident in the C++ language, even if I'm not fully versed in the specifics or the standard library.
