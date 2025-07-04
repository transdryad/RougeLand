#pragma once

#include <string>
#include "libtcod.hpp"
#include <random>
#include <vector>
//#include "game.hpp"

//#include "creature.hpp"
//#include "item.hpp"

class Item;
class Creature;
class Game;

class Entity;

struct MapTile{
    bool solid;
    bool walkable;
    bool explored;
    std::string character;
    tcod::ColorRGB color;
};

struct Point{
    double x;
    double y;
};

class GameMap;

class RectRoom{
    public:
        int x;
        int y;
        int xx;
        int yy;
        int cx;
        int cy;
        TCODBsp* node;
        RectRoom(int x, int y, int xx, int yy, TCODBsp* node);
        void draw(GameMap& map) const;
};

class Hallway{
    public:
        Hallway(int x1, int x2, int y1, int y2, int corner_x, int corner_y);
        void draw(GameMap& map) const;
        int x1;
        int x2;
        int y1;
        int y2;
        int corner_x;
        int corner_y;
};

class GameMap{
    public:
        GameMap(std::vector<Creature>& entities, std::vector<Item>& items, Game& game);
        void init();
        TCODBsp bsptree;
        TCODMap fmap;
        std::vector<Creature>& entities;
        std::vector<Item>& items;
        MapTile tiles[80][45];
        void connect(TCODBsp* left, TCODBsp* right);
        Game& game;
        std::vector<RectRoom> rooms;
        std::vector<Hallway> halls;
        bool isWalkable(int x, int y) const;
        bool isSolid(int x, int y) const;
        void render(tcod::Console& rconsole);
        void compute();
        void drawInBounds(int x, int y, int nx, int ny, TCODBsp* node);
        void wipe();
        RectRoom roomFromNode(TCODBsp* node) const;
};
