#include "item.hpp"
#include "map.hpp"

void Item::render(tcod::Console& rconsole) {
    if (map.get().fmap.isInFov(x, y)) {
        tcod::print(rconsole, {x, y}, character, color, std::nullopt);
    }
}


Item::Item(const ItemType type, const int x, const int y, const std::string &character, const tcod::ColorRGB color, GameMap& map): Entity(Entity(x, y, character, color, map)) {
    this->itype = type;
}
