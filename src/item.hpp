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
        Item(ItemType type, int x, int y, GameMap& map);
        void render(tcod::Console& console) override;
};

