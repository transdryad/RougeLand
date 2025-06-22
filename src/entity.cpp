#include "entity.hpp"
#include "libtcod.hpp"
#include <string>
#include "map.hpp"

Entity::Entity(const int x, const int y, const std::string &character, const tcod::ColorRGB color, const bool ai, const int maxHp, const bool player, GameMap& map): map(map) {
    this->x = x;
    this->y = y;
    this->ai = ai;
    this->attack = 1;
    this->player = player;
    this->character = character;
    this->color = color;
    this->maxHp = maxHp;
    this->hp = maxHp;
    this->living = true;
    this->acted = false;
}

void Entity::damage(const int damage) {
    hp -= damage;
    if (hp <= 0) {
        living = false;
        ai = false;
        color = {255, 0, 0};
    }
}


void Entity::move(const int dx, const int dy) {
    if (!acted) {
        if (map.isWalkable(x + dx, y + dy)) {
            bool occupied = false;
            for (Entity& e : map.entities) {
                if (e.x == x + dx && e.y == y + dy) {
                    if (e.living) {
                        occupied = true;
                        e.damage(attack);
                    }
                }
            }
            if (!occupied) {
                x += dx;
                y += dy;
            }
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
    if (!living && player) {
        printf("You Died!");
        map.wipe();
        exit(0);
    }
    if (ai) {
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
