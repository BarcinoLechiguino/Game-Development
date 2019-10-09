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
	char tileset_name[64]; //Name of the tileset.
	char img_source[128]; //The name of the TSX file where the tileset is stored at.
	uint img_width = 0u; //Width of the source image.
	uint img_height = 0u; //Height of the source image.

	uint first_gid = 0u; //The first tile ID of the whole tileset.
	uint tile_max_width = 0u; //Maximum width of the tiles in this tileset.
	uint tile_max_height = 0u; //Maximum height of the tiles in this tileset.
	uint spacing = 0u; //The space in pixels between the tiles in this tileset.
	uint margin = 0u; //The margin around the tiles in this tileset (applies to tileset image)

};

enum MapType  //Had to create it outside bc I do not know how to call the enum class orientation elements.
{
	MAPTYPE_UKNOWN,
	MAPTYPE_ORTOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED,
	MAPTYPE_HEXAGONAL
};

// TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	//class here separates the variables so there are no naming collisions. New way of avoiding name collisions.
	enum class orientation //Enum defines a closed list of possible inputs/output. Use only when knowing exactly what you will recieve.
	{
		error, //We declare this so if there is any error in the code we can see where it comes from.
		orthogonal,
		isometric,
		staggered,
		hexagonal
	};

	//Instead of using the class namespace(?) we add a prefix to the names so they are separated entities. It's the old school way of avoiding naming collisions.
	enum renderorder
	{
		RO_error,
		RO_right_down, //This is the default type of rendering. Begins at the pixel the furthest at the bottom right.
		RO_right_up,
		RO_left_down,
		RO_left_up
	};

	// The compilator, when given a variable, will allocate the maximum amount of memory it can at that moment, in this case int. To prevent that, we put the type of variable we want it to be after we give it a value.
	uint width = 0u; //Map width in tiles
	uint height = 0u; //Map height in tiles.
	uint tile_width = 0u; //The width of a tile.
	uint tile_height = 0u; //The height of at tile.
	uint nextObjectId = 0u; // Stores the next available ID for new objects. Prevents reuse of used IDs after object removal.

	MapType type; //Allows to access the enum with the MapData struct (data. ...)
	p2List<TileSet*> TileSet_list; //Creates a list that grants access to the variables stored in the TileSet struct. It can be iterated through with the p2List_Item pointer.
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
	MapData data; //Declares the struct in the module so it can be called from all over.

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__