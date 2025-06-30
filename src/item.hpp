#pragma once

#include "entity.hpp"
#include "libtcod.hpp"
#include "map.hpp"

class Entity;
class GameMap;

enum ItemType {
    SWORD, HELMET
};

class Item : public Entity {
    public:
        ItemType itype;
        int value;
        bool equipped;
        Item(ItemType type, int x, int y, int value, GameMap& map);
        void render(tcod::Console& console) override;
};

