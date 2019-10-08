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
	//We set the pointer of the list we declared previously to the first item in the TileSet struct.
	p2List_item<TileSet*>* item = data.tilesets.start;

	while (item != NULL)
	{
		//The RELEASE macro checks again if what it is given is null and then proceeds to delete.
		//delete item->data; //Deletes a buffer. //delete[] item->data; //Deletes an array.
		RELEASE(item->data); //The RELEASE macro deletes the memory allocated to what it is given.
		item = item->next;
	}

	data.tilesets.clear(); //Destroys and frees all memory related to the tilesets.

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
		ret == LoadMap();
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	

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
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		data.nextObjectId = map.attribute("nextobjectid").as_int();

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = orthohonal;
		}
		else if (orientation == "isometric")
		{

		}
		else if (orientation == "staggered")
		{

		}
		else if (orientation == "hexagonal")
		{

		}

		p2SString renderorder(map.attribute("renderorder").as_string());

		if (renderorder == "right-up")
		{
			data.RO_right_up = renderorder;
		}
		else if (renderorder == "right-down")
		{

		}
		else if (renderorder == "left-up")
		{

		}
		else if (renderorder == "left-down")
		{

		}

	}
}

