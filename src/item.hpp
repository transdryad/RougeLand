#pragma once

#include "entity.hpp"
#include "libtcod.hpp"
#include "map.hpp"

class Entity;
class GameMap;

enum ItemType {
    SWORD, HELMET, POTION
};

class Item : public Entity {
    public:
        ItemType itype;
        int value;
        bool equipped;
        Item(ItemType type, int x, int y, int value, Game& game, int level);
        void render(tcod::Console& console) override;
};

