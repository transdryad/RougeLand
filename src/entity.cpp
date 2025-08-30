#include "entity.hpp"
#include "libtcod.hpp"
#include <string>
#include <algorithm>

#include "creature.hpp"
#include "map.hpp"

Entity::Entity(const int x, const int y, const std::string &character, const tcod::ColorRGB color, GameMap& map): map(map) {
    this->x = x;
    this->y = y;
    this->character = character;
    this->color = color;
}

void Entity::spawn() {
    TCODRandom* randomizer = TCODRandom::getInstance();
    while (!map.get().isWalkable(x, y)) {
        x += randomizer->getInt(-1, 1);
        y += randomizer->getInt(-1, 1);
        std::clamp(x, 0, 80);
        std::clamp(y, 0, 45);
        //if (x >= 80) x = 80;
        //if (y >= 45) y = 45;
    }
}

void Entity::update() {}
void Entity::render(tcod::Console& console) {}
