#include "item.hpp"
#include "map.hpp"
#include <fmt/core.h>
#include "game.hpp"

void Item::render(tcod::Console& console) {
    game.get().logger->info(fmt::format("Rendering item at {}, {}", x, y));
    if (game.get().levels[game.get().level].fmap.isInFov(x, y) && level == game.get().level) {
        tcod::print(console, {x, y}, character, color, std::nullopt);
    }
}


Item::Item(const ItemType type, const int x, const int y, int value, Game& game, int level): Entity(Entity(x, y, " ", {0, 0, 0}, game, level)) {
    this->itype = type;
    this->value = value;
    this->equipped = false;
    switch (itype) {
        case SWORD:
            character = "!";
            color = {204, 204, 204}; break;
        case HELMET:
            character = "^";
            color = {119, 119, 119}; break;
        case POTION:
            character = "♠︎";
            //character = "p";
            color = {255, 215, 0}; break;
        default: break;
    }
}
