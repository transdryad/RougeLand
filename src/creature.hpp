#pragma once
#include "map.hpp"
#include "game.hpp"
#include "entity.hpp"
#include "item.hpp"
#include <string>

enum CreatureType : int{
    PLAYER, ORC, GOBLIN
};

class Creature : public Entity {
    public:
        bool ai;
        int hp;
        int maxHp;
        int attack;
        int xlevel;
        bool living;
        bool acted;
        bool player;
        int xp;
        int xpval;
        int ac;
        std::vector<Item> items;
        Creature(int x, int y, const std::string &character, tcod::ColorRGB color, bool ai, int maxHp, bool player, int xpval, Game& game, int attack, int level);
        void move(int dx, int dy);
        void damage(int ar, int damage);
        void experience(int exp);
        void update() override;
        void render(tcod::Console& console) override;
};
