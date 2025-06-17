#include "entity.hpp"
#include "libtcod.hpp"
#include <string>
#include "map.hpp"

Entity::Entity(int x, int y, std::string character, tcod::ColorRGB color, GameMap& map): map(map) {
    this->x = x;
    this->y = y;
    this->character = character;
    this->color = color;
}

void Entity::move(int dx, int dy) {
    if (this->map.isWalkable(this->x + dx, this->y + dy)) {
        this->x += dx;
	this->y += dy;
    }
}

void Entity::render(tcod::Console& rconsole) {
    tcod::print(rconsole, {this->x, this->y}, this->character, this->color, std::nullopt);
}
