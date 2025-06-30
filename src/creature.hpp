#pragma once
#include "map.hpp"
#include "entity.hpp"
#include "item.hpp"
#include <string>

enum CreatureType {
    PLAYER, ORC, GOBLIN
};

class Creature : public Entity {
    public:
        bool ai;
        int hp;
        int maxHp;
        int attack;
        int level;
        bool living;
        bool acted;
        bool player;
        int xp;
        int xpval;
        int ac;
        std::vector<Item> items;
        Creature(int x, int y, const std::string &character, tcod::ColorRGB color, bool ai, int maxHp, bool player, int xpval, GameMap& map);
        void move(int dx, int dy);
        void damage(int ar, int damage);
        void experience(int exp);
        void update();
        void render(tcod::Console& console) override;
};
