#include "entity.hpp"
#include "libtcod.hpp"
#include <string>
#include "map.hpp"

Entity::Entity(int x, int y, std::string character, tcod::ColorRGB color, bool ai, int maxHp, GameMap& map): map(map) {
    this->x = x;
    this->y = y;
    this->ai = ai;
    this->character = character;
    this->color = color;
    this->maxHp = maxHp;
    this->hp = maxHp;
    this->living = true;
    this->acted = false;
}

void Entity::move(int dx, int dy) {
    if (!acted) {
        if (map.isWalkable(x + dx, y + dy)) {
            x += dx;
            y += dy;
        }
        acted = true;
    }
}

void Entity::render(tcod::Console& rconsole) {
    if (map.fmap.isInFov(x, y)) {
        tcod::print(rconsole, {x, y}, character, color, std::nullopt);
    }
}

void Entity::update() {
    if (hp <= 0) {
        living = false;
        color = {255, 0, 0};
    } else if (ai) {
        auto* path = new TCODPath(&map.fmap);
        if (!(x == map.entities[0].x && y == map.entities[0].y)) {
            path->compute(x, y, map.entities[0].x, map.entities[0].y); //entities[0] is the player
            int x;
            int y;
            path->walk(&x, &y, true);
            this->move(x - this->x, y - this->y);
        }
    }
    acted = false;
}

void Entity::spawn() {
    TCODRandom* randomizer = TCODRandom::getInstance();
    while (!map.isWalkable(x, y)) {
        x += randomizer->getInt(-1, 1);
        y += randomizer->getInt(-1, 1);
        if (x >= 80) x = 80;
        if (y >= 45) y = 45;
    }
}
