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
	uint firstgid = 0u;
	char name[64];
	uint tileWidth = 0u;
	uint tileHeight = 0u;
	uint spacing = 0u;
	uint margin = 0u;

	char imgSource[128];
	uint imgWidth = 0u;
	uint imgHeight = 0u;
};

// TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	//"class" keeps the names used inside the struct in the same namespace so there is no name collision with other variables. New way of avoiding name collisions.
	enum class orientation //"enum" creates a "list" of expected inputs/variable names. Used to reduce memory usage and loading time, only use when what will be recieved is known.
	{
		error, //Declared error as an expected input so if something goes wrong with the code it can be seen where it falters.
		orthogonal,
		isometric,
		staggered,
		hexagonal
	};

	//Adding a prefix, RO_ in this case, to the names used inside the struct is the old way of avoiding name collisions. 
	enum renderorder
	{
		RO_error,
		RO_right_up,
		RO_right_down,
		RO_left_up,
		RO_left_down
	};

	// uint = 0 due to the fact that if there is any kind of error we can see where the code falters.
	//"u" (standing for unsigned int) tells the compiler we only need an unsigned number. Unless it is specified, the compiler will take the most amount of memory it can, int in this case.
	uint width = 0u;
	uint height = 0u;
	uint tileWidth = 0u;
	uint tileHeight = 0u;
	uint nextObjectId = 0u;

	p2List<TileSet*> tilesets; // List of the TileSet struct we can iterate through.
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
	bool LoadMap();

public:

	// TODO 1: Add your struct for map info as public for now
	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__