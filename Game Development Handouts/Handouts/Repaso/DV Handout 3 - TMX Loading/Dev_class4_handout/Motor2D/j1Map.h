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
	p2SString tileset_name; //Name of the tileset.
	char img_source[128]; //The name of the TSX file where the tileset is stored at.
	uint img_width = 0u; //Width of the source image.
	uint img_height = 0u; //Height of the source image.

	uint first_gid = 0u; //The first tile ID of the whole tileset.
	uint max_tile_width = 0u; //Maximum width of the tiles in this tileset.
	uint max_tile_height = 0u; //Maximum height of the tiles in this tileset.
	uint spacing = 0u; //The space in pixels between the tiles in this tileset.
	uint margin = 0u; //The margin around the tiles in this tileset (applies to tileset image)

	uint offset_x = 0u;
	uint offset_y = 0u;

	SDL_Texture* texture; //Declares a texture pointer that can point to any of the variables inside texture.
	uint texture_width = 0u; //Image width in pixels.
	uint texture_height = 0u; //Image height in pixels.
	uint tile_max_width = 0u; //Max tile width in pixels.
	uint tile_max_height = 0u; //Max tile height in pixels.
};


// TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	//Example: "class" keeps the names used inside the struct in the same namespace so there is no name collision with other variables. New way of avoiding name collisions.
	enum class MapType //"enum" creates a "list" of expected inputs/variable names. Used to reduce memory usage and loading time, only use when what will be recieved is known.
	{
		error, //Declared error as an expected input so if something goes wrong with the code it can be seen where it falters.
		orthogonal,
		isometric,
		staggered,
		hexagonal
	};

	//Adding a prefix, OR_ in this case, to the names used inside the struct is the old way of avoiding name collisions. 
	enum orientation
	{
		OR_ERROR = 0,
		OR_ORTHOGONAL,
		OR_ISOMETRIC,
		OR_STAGGERED,
		OR_HEXAGONAL
	};
	
	//Adding a prefix, RO_ in this case, to the names used inside the struct is the old way of avoiding name collisions. 
	enum renderorder
	{
		RO_ERROR = 0,
		RO_RIGHT_DOWN, //Standard rendering order. Starts at top left and ends bottom right.
		RO_RIGHT_UP,
		RO_LEFT_DOWN,
		RO_LEFT_UP
		
	};
	
	// uint = 0 due to the fact that if there is any kind of error we can see where the code falters.
	//"u" (standing for unsigned int) tells the compiler we only need an unsigned number. Unless it is specified otherwise, the compiler will allocate the maximum amount of memory it can at that moment (in this case int).
	uint width = 0u; //Map width in tiles
	uint height = 0u; //Map height in tiles.
	uint tile_width = 0u; //The width of a tile.
	uint tile_height = 0u; //The height of at tile.
	uint nextObjectId = 0u; // Stores the next available ID for new objects. Prevents reuse of used IDs after object removal.

	orientation orientation_type; //Allows to access the enum with the MapData struct (data.type ...)
	renderorder render_order_type; //Allows to access the enum with the MapData struct (data.type ...)
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
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet * set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet * set);

public:

	// TODO 1: Add your struct for map info as public for now
	MapData data; //Added so it is possible to call from the j1Map.cpp
	TileSet tileSetData; //Added so it is possible to call from the j1Map.cpp
private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__