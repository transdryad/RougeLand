#include "entity.hpp"
#include "libtcod.hpp"

void Entity::move(int dx, int dy) {
    this->x += dx;
    this->y += dy;
}

void Entity::render(tcod::Console& rconsole) {
    tcod::print(rconsole, {this->x, this->y}, this->character, this->fg, std::nullopt);
}
