#include "item.hpp"

Item::Item(const ItemType type, const int x, const int y, const std::string &character, const tcod::ColorRGB color): Entity(Entity(x, y, character, color,map)) {
    this->type = type;
}
