#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct TileSet
{
	//All int = 0 bc in case sth goes wrong we see where the code falters.
	//We put u to tell the compiler that we only require an unsigned number. If you just put a number it will take the most amount of memory it can, in this case int.
	uint firstGid = 0u;
	char name[64];
	uint tileSet = 0u;
	uint tileWidth = 0u;
	uint spacing = 0u;
	uint margin = 0u;
	char imgSource[128];
	uint imgWidth = 0u;
	uint imgHeight = 0u;
};

// TODO 1: Create a struct needed to hold the information to Map node
struct mapNode
{
	enum class orientation //Keeps the names used inside the struct in the same namespace so there is no name collision with other variables.
	{
		error, //In case sth goes wrong we see where the code falters.
		orthogonal,
		isometric,
		staggered,
		hexagonal
	};
	enum renderorder
	{
		//The old school way to avoid name collisions was and is to put a prefix, in this case RO_.
		RO_error, //In case sth goes wrong we see where the code falters.
		RO_right_down,
		RO_left_up,
		RO_left_down,
		RO_left_up
	};

	//All int = 0 bc in case sth goes wrong we see where the code falters.
	//We put u to tell the compiler that we only require an unsigned number. If you just put a number it will take the most amount of memory it can, in this case int.
	uint width = 0u; 
	uint height = 0u; 
	uint tilewidth = 0u; 
	uint tileheight = 0u; 
	uint nextobjectid = 0u; 
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:


public:

	// TODO 1: Add your struct for map info as public for now
	mapNode Map;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__