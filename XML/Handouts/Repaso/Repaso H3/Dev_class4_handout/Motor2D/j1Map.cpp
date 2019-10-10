#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map
	//p2List_item declares a pointer that can iterate through a given struct/class/list. In this case it points to the first item in the TileSet struct.
	p2List_item<TileSet*>* item = data.TileSet_list.start; //data stands for the MapData struct, TileSet_list stands for the list containing the variables of the TS struct and start is a pointer that points to the first member of the list that calls it. 

	while (item != NULL)
	{
		//The RELEASE macro deletes a given buffer. It first re-checks that what it is given is not NULL.
		RELEASE(item->data); //item->data gives the RELEASE macro the allocated memory to be deleted.
		item = item->next; //Changes the where the pointer is pointing at and directs it to the next node.
		//delete item->data would also delete all memory allocated to tilesets and maps.
	}

	data.TileSet_list.clear(); //The clear() method deletes and frees all memory. In this case it deletes the memory allocated for the struct.
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		ret = LoadMap();
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tileset;


	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map"); //We set the handle of the node to the map tag.

	if (map == NULL)
	{
		LOG("Error parsing the map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		//Allocating the variables gotten from the MapData struct to the map xml node.
		data.width = map.append_attribute("width").as_uint();
		data.height = map.append_attribute("height").as_uint();
		data.tile_width = map.append_attribute("tilewidth").as_uint();
		data.tile_height = map.append_attribute("tileheight").as_uint();

		//Declares a string that gets the type of orientation and sets it as a string.
		p2SString orientation(map.attribute("orientation").as_string());
		
		//Going through each enum case to ascertain which has been specified by the file.
		//data.type is used to et the
		if (orientation == "ortogonal") 
		{
			//If both strings are equal then the type of orientation is set to whatever type it matched to.
			data.type = MAPTYPE_ORTOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else if (orientation == "hexagonal")
		{
			data.type = MAPTYPE_HEXAGONAL;
		}
		else
		{
			data.type = MAPTYPE_UKNOWN;
		}
	}

	return ret;
}

