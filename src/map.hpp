#include <string>
#include "libtcod.hpp"

struct MapTile{
    bool solid;
    bool walkable;
    std::string character;
    tcod::ColorRGB color;
};

class GameMap{
    public:
	GameMap();
	MapTile tiles[80][45];
	bool isWalkable(int x, int y);
	bool isSolid(int x, int y);
	void render(tcod::Console& rconsole);
};
