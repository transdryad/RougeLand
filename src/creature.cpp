#include "creature.hpp"

#include "libtcod.hpp"
#include <string>
#include "map.hpp"
#include "item.hpp"

Creature::Creature(const int x, const int y, const std::string &character, const tcod::ColorRGB color, const bool ai, const int maxHp, const bool player, const int xpval, GameMap& map): Entity(x, y, character, color,map){
    this->x = x;
    this->y = y;
    this->ai = ai;
    this->attack = 1;
    this->player = player;
    this->character = character;
    this->color = color;
    this->maxHp = maxHp;
    this->hp = maxHp;
    this->xpval = xpval;
    this->xp = 0;
    this->level = 1;
    this->living = true;
    this->acted = false;
}

void Creature::experience(const int exp) {
    if (xp + exp >= 1000) {
        ++level;
        xp = 0;
    } else {
        xp += exp;
    }
}

void Creature::damage(const int damage) {
    hp -= damage;
    if (hp <= 0) {
        living = false;
        ai = false;
        color = {255, 0, 0};
    }
}

void Creature::move(const int dx, const int dy) {
    if (!acted) {
        if (map.get().isWalkable(x + dx, y + dy)) {
            bool occupied = false;
            for (int i = 0; i < map.get().entities.size(); i++) {
                if (Creature c = map.get().entities[i]; c.x == x + dx && c.y == y + dy) {
                    if (c.living) {
                        occupied = true;
                        c.damage(attack);
                        if (!c.living) {
                            experience(c.xpval);
                            c.xpval = 0;
                        }
                    }
                }
            }
            for (int i = 0; i < map.get().items.size(); i++) {
                Item it = map.get().items[i];
                it.x = 0;
                it.y = 0;
                map.get().entities.erase(map.get().entities.begin() + i);
                items.emplace_back(it);
            }
            if (!occupied) {
                x += dx;
                y += dy;
            }
        }
        acted = true;
    }
}

void Creature::update() {
    if (!living && player) {
        printf("You died with %d xp!", xp);
        map.get().wipe();
        exit(0);
    }
    if (ai) {
        auto* path = new TCODPath(&map.get().fmap);
        if (!(x == map.get().entities[0].x && y == map.get().entities[0].y)) {
            path->compute(x, y, map.get().entities[0].x, map.get().entities[0].y); //entities[0] is the player
            int x;
            int y;
            path->walk(&x, &y, true);
            this->move(x - this->x, y - this->y);
        }
    }
    acted = false;
}