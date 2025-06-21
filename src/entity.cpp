#include "entity.hpp"
#include "libtcod.hpp"
#include <string>
#include "map.hpp"

Entity::Entity(int x, int y, std::string character, tcod::ColorRGB color, bool ai, int maxHp, GameMap& map): map(map) {
    this->x = x;
    this->y = y;
    this->ai = ai;
    this->character = character;
    this->color = color;
    this->maxHp = maxHp;
    this->hp = maxHp;
}

void Entity::move(int dx, int dy) {
    if (!this->acted) {
	if (this->map.isWalkable(this->x + dx, this->y + dy)) {
	    this->x += dx;
	    this->y += dy;
	}
	this->acted = true;
    }
}

void Entity::render(tcod::Console& rconsole) {
    bool veiwable = this->map.fmap.isInFov(this->x, this->y);
    if (veiwable) {
	tcod::print(rconsole, {this->x, this->y}, this->character, this->color, std::nullopt);
    }
}

void Entity::update() {
    if (this->hp <= 0) {
	this->living = false;
	this->color = {255, 0, 0};
    } else if (this->ai) {
	TCODPath* path = new TCODPath(&this->map.fmap);
	if (this->x == this->map.entities[0].get().x && this->y == this->map.entities[0].get().y) {
	    
	} else {
	path->compute(this->x, this->y, this->map.entities[0].get().x, this->map.entities[0].get().y); //entities[0] is always the player
	int x;
	int y;
	path->walk(&x, &y, true);
	this->move(x - this->x, y - this->y);
	}
    }
    this->acted = false;
}

void Entity::spawn() {
    TCODRandom* randomizer = TCODRandom::getInstance();
    while (!this->map.isWalkable(this->x, this->y)) {
	this->x += randomizer->getInt(-1, 1);
	this->y += randomizer->getInt(-1, 1);
	if (this->x >= 80) this->x = 80;
	if (this->y >= 45) this->y = 45;
    }
}
