#include "creature.hpp"

#include "libtcod.hpp"
#include <string>
#include "map.hpp"
#include "item.hpp"
#include "game.hpp"

void Creature::render(tcod::Console& rconsole) {
    if (game.get().levels[game.get().level].fmap.isInFov(x, y) && level == game.get().level) {
        tcod::print(rconsole, {x, y}, character, color, std::nullopt);
    }
}

Creature::Creature(const int x, const int y, const std::string &character, const tcod::ColorRGB color, const bool ai, const int maxHp, const bool player, const int xpval, Game& game, int attack, int level): Entity(x, y, character, color, game, level){
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
    this->xlevel = 1;
    this->living = true;
    this->acted = false;
    this->ac = 10;
}

void Creature::experience(const int exp) {
    if (xp + exp >= 1000) {
        ++xlevel;
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
        if (game.get().levels[game.get().level].isWalkable(x + dx, y + dy)) {
            bool occupied = false;
            for (auto& c : game.get().levels[game.get().level].entities) {
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
            for (int i = 0; i < game.get().levels[game.get().level].items.size(); i++) {
                if (Item it = game.get().levels[game.get().level].items[i]; it.x == x + dx && it.y == y + dy) {
                    switch (it.itype) {
                        case (SWORD):
                            attack += it.value; break;
                        case (HELMET):
                            ac += it.value; break;
                        case (POTION):
                            hp += it.value; maxHp += 1; if (hp > maxHp) hp = maxHp; break;
                        default: break;
                    }
                    it.x = 0;
                    it.y = 0;
                    it.equipped = true;
                    it.level = 0;
                    game.get().levels[game.get().level].items.erase(game.get().levels[game.get().level].items.begin() + i);
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
        game.get().levels[game.get().level].wipe();
        exit(0);
    }
    if (ai) {
        auto* path = new TCODPath(&game.get().levels[game.get().level].fmap);
        if (!(x == game.get().levels[game.get().level].entities[0].x && y == game.get().levels[game.get().level].entities[0].y)) {
            path->compute(x, y, game.get().levels[game.get().level].entities[0].x, game.get().levels[game.get().level].entities[0].y); //entities[0] is the player
            int nx;
            int ny;
            path->walk(&nx, &ny, true);
            this->move(nx - x, ny - y);
        }
    }
    acted = false;
}
