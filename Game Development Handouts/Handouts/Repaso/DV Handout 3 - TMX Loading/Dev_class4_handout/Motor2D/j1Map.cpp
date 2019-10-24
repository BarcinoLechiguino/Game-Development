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
	p2List_item<TileSet*>* item = data.TileSet_list.start;
	TileSet * set = item->data;

	while (item != NULL)
	{
		for (int y = 0; y < data.height; y++)
		{
			for (int x = 0; x < data.width; x++)
			{
				/*App->render->Blit(set->texture, x, y);
				item = item->next;*/
			}
		}
	}

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map
	//We set the pointer of the list we declared previously to the first item in the TileSet struct.
	p2List_item<TileSet*>* item = data.TileSet_list.start;

	while (item != NULL)
	{
		//The RELEASE macro checks again if what it is given is null and then proceeds to delete.
		//delete item->data; //Deletes a buffer. //delete[] item->data; //Deletes an array.
		RELEASE(item->data); //The RELEASE macro deletes the memory allocated to what it is given.
		item = item->next;
	}

	data.TileSet_list.clear(); //Destroys and frees all memory related to the tilesets.

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

	//At the beginning of the loop the handler node is at the first position of the tileset tag details, and ,until either tileset == NULL or ret != true, the loop will keep iterating the different tilesets.
	for (tileset = map_file.child("map").child("tileset"); tileset && ret ; tileset = tileset.next_sibling("tileset"))
	{
		TileSet * set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.TileSet_list.add(set); //Per each cycle we call the add() method to Load a whole new tileset that has been filled.
	}

	//LOGing all the data loaded of all the tilesets.
	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		LOG("Successfully parsed the map XML File: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);
		
		p2List_item<TileSet*>* item = data.TileSet_list.start;

		while (item != NULL)
		{
			//TileSet * s = item->data --> s->tileset_name... || item->data->tileset_name... || Declaring the struct in j1Map.h
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", tileSetData.tileset_name.GetString(), tileSetData.first_gid);
			LOG("tile width: %d tile height: %d", tileSetData.max_tile_width, tileSetData.max_tile_height);
			LOG("spacing: %d margin: %d", tileSetData.spacing, tileSetData.margin);

			item = item->next;
		}
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadMap() 
{
	//Fill all the map data into the xml file.
	bool ret = true;
	//The node handle is pointed to the map tag of the file.
	pugi::xml_node map = map_file.child("map");

	//As we only need to pass the information once we do not need to declare a list and iterate it.
	if (map == NULL)
	{
		LOG("Error in parsing the xml map file. 'map' tag not found."); //Reports what went wrong when loading the map.
		ret = false;
	}
	else
	{
		//Need to go through each one individually to save it in the file. Could be iterated by a list?
		//.attribute() gives us access to the variables/tags inside the already declared tag.
		data.width = map.attribute("width").as_uint();
		data.height = map.attribute("height").as_uint();
		data.tile_width = map.attribute("tilewidth").as_uint();
		data.tile_height = map.attribute("tileheight").as_uint();
		data.nextObjectId = map.attribute("nextobjectid").as_uint();

		//A string is needed to compare which orientation and render order is needed.
		//After accessing the desired tag through the handle node, we set that tag as a string. 
		p2SString orientation(map.attribute("orientation").as_string());

		//If enum orientation is declared outside the MapData struct, the second "data." is not needed.
		if (orientation == "orthogonal")
		{
			data.orientation_type = data.OR_ORTHOGONAL; //Sets the type of orientation in the struct at the xml file.
		}
		else if (orientation == "isometric")
		{
			data.orientation_type = data.OR_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.orientation_type = data.OR_STAGGERED;
		}
		else if (orientation == "hexagonal")
		{
			data.orientation_type = data.OR_HEXAGONAL;
		}
		else
		{
			data.orientation_type = data.OR_ERROR;
		}

		//After accessing the desired tag through the handle node, we set that tag as a string.
		p2SString render_order(map.attribute("renderorder").as_string());

		//If enum renderorder is declared outside the MapData struct, the second "data." is not needed.
		if (render_order == "right-down")
		{
			data.render_order_type = data.RO_RIGHT_DOWN; //Sets the type of rendering order in the struct at the xml file.
		}
		else if (render_order == "right-up")
		{
			data.render_order_type = data.RO_RIGHT_UP;
		}
		else if (render_order == "left-down")
		{
			data.render_order_type = data.RO_LEFT_DOWN;
		}
		else if (render_order == "left-up")
		{
			data.render_order_type = data.RO_LEFT_UP;
		}
		else
		{
			data.render_order_type = data.RO_ERROR;
		}

		ret = true;
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet * set)
{
	bool ret = true;

	//With the TileSet pointer we can add each detail wherever they have to be added, and with the handler node we can access the nodes/tags.
	set->tileset_name.create(tileset_node.attribute("name").as_string());
	set->first_gid = tileset_node.attribute("firstgid").as_uint();
	set->max_tile_width = tileset_node.attribute("tilewidth").as_uint();
	set->max_tile_height = tileset_node.attribute("tileheight").as_uint();
	set->spacing = tileset_node.attribute("spacing").as_uint();
	set->margin = tileset_node.attribute("margin").as_uint();
	
	//As tile offset is sth that can be there or not, it is necessary to solve the two cases: With offset and without.
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_uint();
		set->offset_y = offset.attribute("y").as_uint();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet * set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error in parsing the map tmx file. Could not find 'image' tag.");
		ret = false;
	}
	else
	{
		//Fills the texture's variables. tex is the texture module and Load() gets the path to the texture (folder, file).
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		
		//SDL_QueryTexture retrieves the size of a given texture.
		int width, height;
		SDL_QueryTexture(set->texture, NULL, NULL, &width, &height);

		set->texture_width = image.attribute("width").as_uint();

		//In case a negative value is recieved, QueryTexture retrieves the true size.
		if (set->texture_width <= 0)
		{
			set->texture_width = width;
		}

		set->texture_height = image.attribute("height").as_uint();

		if (set->texture_height <= 0)
		{
			set->texture_height = height;
		}

		//Sets the width limit in pixels for the tile.
		set->tile_max_width = set->texture_width / set->max_tile_width;

		//Sets the height limit in pixelsfor the tile.
		set->tile_max_height = set->texture_height / set->max_tile_height;
	}

	return ret;
}


