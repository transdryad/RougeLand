#include "entity.hpp"
#include "libtcod.hpp"
#include <string>

Entity::Entity(int x, int y, std::string character, tcod::ColorRGB color) {
    this->x = x;
    this->y = y;
    this->character = character;
    this->color = color;
}

void Entity::move(int dx, int dy) {
    this->x += dx;
    this->y += dy;
}

void Entity::render(tcod::Console& rconsole) {
    tcod::print(rconsole, {this->x, this->y}, this->character, this->color, std::nullopt);
}
