#include "creature.hpp"

#include "libtcod.hpp"
#include <string>
#include "map.hpp"
#include "item.hpp"

void Creature::render(tcod::Console& rconsole) {
    if (map.get().fmap.isInFov(x, y)) {
        tcod::print(rconsole, {x, y}, character, color, std::nullopt);
    }
}

Creature::Creature(const int x, const int y, const std::string &character, const tcod::ColorRGB color, const bool ai, const int maxHp, const bool player, const int xpval, GameMap& map, int attack): Entity(x, y, character, color,map){
    this->x = x;
    this->y = y;
    this->ai = ai;
    this->attack = attack;
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
    this->ac = 10;
}

void Creature::experience(const int exp) {
    if (xp + exp >= 1000) {
        ++level;
        xp = 0;
    } else {
        xp += exp;
    }
}

void Creature::damage(int ar, const int damage) {
    if (ar >= ac) {
        hp -= damage;
    }
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
            for (auto& c : map.get().entities) {
                if (c.x == x + dx && c.y == y + dy) {
                    if (c.living) {                   
                        occupied = true;
                        c.damage(TCODRandom::getInstance()->getInt(1, 20), attack);
                        if (!c.living) {
                            experience(c.xpval);
                            c.xpval = 0;
                        }
                    }
                }
            }
            for (int i = 0; i < map.get().items.size(); i++) {
                if (Item it = map.get().items[i]; it.x == x + dx && it.y == y + dy) {
                    switch (it.itype) {
                        case (SWORD):
                            attack += it.value; break;
                        case (HELMET):
                            ac += it.value; break;
                        default: break;
                    }
                    it.x = 0;
                    it.y = 0;
                    it.equipped = true;
                    map.get().items.erase(map.get().items.begin() + i);
                    items.emplace_back(it);
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

void Creature::update() {
    if (!living && player) {
        printf("You died with %d xp!\n", xp);
        map.get().wipe();
        exit(0);
    }
    if (ai) {
        auto* path = new TCODPath(&map.get().fmap);
        if (!(x == map.get().entities[0].x && y == map.get().entities[0].y)) {
            path->compute(x, y, map.get().entities[0].x, map.get().entities[0].y); //entities[0] is the player
            int nx;
            int ny;
            path->walk(&nx, &ny, true);
            this->move(nx - x, ny - y);
        }
    }
    acted = false;
}
