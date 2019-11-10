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
	ResetPath();

	return ret;
}

bool j1Map::Start()
{
	tile_x = App->tex->Load("maps/x.png");
	return true;
}

void j1Map::ResetPath()
{
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
	frontier.Push(iPoint(19, 4), 0);
	visited.add(iPoint(19, 4));
	breadcrumbs.add(iPoint(19, 4));
	memset(cost_so_far, 0, sizeof(uint) * COST_MAP * COST_MAP);

	goalReached = false;
}

void j1Map::Path(int x, int y)
{
	path.Clear();
	iPoint goal = WorldToMap(x, y);

	iPoint current = goal;							//Minor change to improve code readability.
	path.PushBack(current);							//Adds to the path dyn array the goal tile (So that the algorithm also takes into account from where the path starts backtracking)

	if (visited.find(current) != -1)				//If the current tile is within the map and is in the visited list.
	{
		while (current != visited.start->data)		//While the data members of the current point are different from the data members of the first point/tile in the visited list.
		{
			int i = visited.find(current);			//The index of the current point in the visited list will be the same index of the parent node in the breadcrumbs list. (if index of current is 8, the 8th position in the breadcrumbs list will be the parent of current)
			current = breadcrumbs[i];				//Assigns to current the data members of the parent node. (So the algorithm backtracks the breadcrumbs list)
			path.PushBack(current);					//Adds to the path dyn array the position/tile data members of the parent node so the path continues from there.
		}
		path.PushBack(visited.start->data);			//Adds to the path dyn array the starting position/tile of the path.
	}
}

// TODO 2: Create a PropagateAStar() as a copy from Dijkstra
// Add a neighbour in the frontier and add the distance to the final cost
// The distance heuristic should be one of three: Manhattan, Square root, Distance.
void j1Map::PropagateAStar()
{
	iPoint current;

	if (goalReached == false)
	{
		if (frontier.Count() != 0)
		{
			frontier.Pop(current);

			iPoint neighbour[8];

			neighbour[0] = { current.x + 1, current.y + 0 };	//RIGHT neighbour
			neighbour[1] = { current.x + 0, current.y + 1 };	//UP neighbour
			neighbour[2] = { current.x - 1, current.y + 0 };	//LEFT neighbour
			neighbour[3] = { current.x + 0, current.y - 1 };	//DOWN neighbour
			neighbour[4] = { current.x + 1, current.y + 1 };	//RIGHT UP neighbour
			neighbour[5] = { current.x + 1, current.y - 1 };	//RIGHT DOWN neighbour
			neighbour[6] = { current.x - 1, current.y + 1 };	//LEFT UP neighbour
			neighbour[7] = { current.x - 1, current.y - 1 };	//LEFT DOWN neighbour

			if (current == goal)
			{
				goalReached = true;
				return;
			}

			for (int i = 0; i < 8; i++)
			{
				if (MovementCost(neighbour[i].x, neighbour[i].y) >= 0)
				{
					int new_cost = cost_so_far[current.x][current.y] + MovementCost(neighbour[i].x, neighbour[i].y);
					//int distance = goal.DistanceNoSqrt(neighbour[i]);
					int distance = Distance(goal, neighbour[i]);						//Calculates the distance between the neighbour being iterated and the goal.

					if (cost_so_far[neighbour[i].x][neighbour[i].y] == NULL || new_cost < cost_so_far[neighbour[i].x][neighbour[i].y])
					{
						if (visited.find(neighbour[i]) == -1)
						{
							cost_so_far[neighbour[i].x][neighbour[i].y] = new_cost;
							priority = new_cost + distance;								//Calculates the priority of this neighbour by taking into account the new cost and distance between the neighbour and the goal.
							
							LOG("This tile's priority is: %d", priority);
							
							frontier.Push(neighbour[i], priority);
							visited.add(neighbour[i]);
							breadcrumbs.add(current);
						}
					}
				}
			}
		}
	}
}

void j1Map::PropagateDijkstra()
{
	// TODO 1: We want to stop propagation as soon as we find the goal tile. 
	// When clicked remember in a property the goal position in tile coordinates.
	// Stop propagation as soon as the goal is found. Test with Dijkstra
	iPoint current;											//No need to declare the points of current as .Pop sets it's data members to the data members of the first item in the queue (first element in the frontier queue), regardless of which data members it had before. 

	if (goalReached == false)
	{
		if (frontier.Count() != 0)
		{
			frontier.Pop(current);

			iPoint neighbour[8];

			neighbour[0] = { current.x + 1, current.y + 0 };	//RIGHT neighbour
			neighbour[1] = { current.x + 0, current.y + 1 };	//UP neighbour
			neighbour[2] = { current.x - 1, current.y + 0 };	//LEFT neighbour
			neighbour[3] = { current.x + 0, current.y - 1 };	//DOWN neighbour
			neighbour[4] = { current.x + 1, current.y + 1 };	//RIGHT UP neighbour
			neighbour[5] = { current.x + 1, current.y - 1 };	//RIGHT DOWN neighbour
			neighbour[6] = { current.x - 1, current.y + 1 };	//LEFT UP neighbour
			neighbour[7] = { current.x - 1, current.y - 1 };	//LEFT DOWN neighbour

			LOG("Goal Tile: (%d, %d)", goal.x, goal.y);
			LOG("Current Tile: (%d, %d)", current.x, current.y);

			if (current == goal)
			{
				goalReached = true;
				return;
			}

			for (int i = 0; i < 8; i++)
			{
				if (MovementCost(neighbour[i].x, neighbour[i].y) >= 0)																	//Checks that the position of the neighbour being iterated is inide the map's bounds.
				{
					int new_cost = cost_so_far[current.x][current.y] + MovementCost(neighbour[i].x, neighbour[i].y);					//Calculates the cost of moving to the neighbour being iterated.

					if (cost_so_far[neighbour[i].x][neighbour[i].y] == NULL || new_cost < cost_so_far[neighbour[i].x][neighbour[i].y])	//Checks that neighbour has not already been calculated in the current cost (cost_so_far) and that the new_cost is less than the previously calculated cost_so_far)
					{
						if (visited.find(neighbour[i]) == -1)																			//Could be also be added to the above if() as if(() && visited.find(neighbour[i]) == -1), but it was separated for readability.
						{
							cost_so_far[neighbour[i].x][neighbour[i].y] = new_cost;														//Assigns the new_cost to the cost so far at the position of the neighbour currently being iterated.
							frontier.Push(neighbour[i], new_cost);																		//Adds the neighbour to the frontier queue along with the calculated cost of moving to that neighbour.
							visited.add(neighbour[i]);																					//Adds the neighbour being iterated to the visited list.
							breadcrumbs.add(current);																					//Adds the current tile to the breadcrumbs list (parent tile/node)
						}
					}
				}
				LOG("Neighbour Tile: (%d, %d)", neighbour[i].x, neighbour[i].y);
			}
		}
	}
}

int j1Map::MovementCost(int x, int y) const
{
	int ret = -1;

	if (x >= 0 && x < data.width && y >= 0 && y < data.height)
	{
		int id = data.layers.start->next->data->Get(x, y);

		if (id == 0)
			ret = 3;
		else
			ret = 0;
	}

	return ret;
}

int j1Map::Distance(iPoint goal, iPoint current)
{
	return  goal.DistanceNoSqrt(current);
}

void j1Map::PropagateBFS()
{
	iPoint current;
	
	if (goalReached != true)
	{
		if (frontier.Pop(current))
		{
			iPoint neighbour[8];
			neighbour[0].create(current.x + 1, current.y + 0);	//RIGHT neighbour
			neighbour[1].create(current.x + 0, current.y + 1);	//UP neighbour
			neighbour[2].create(current.x - 1, current.y + 0);	//LEFT neighbour
			neighbour[3].create(current.x + 0, current.y - 1);	//DOWN  neighbour
			neighbour[4].create(current.x + 1, current.y + 1);	//RIGHT UP neighbour
			neighbour[5].create(current.x + 1, current.y - 1);	//RIGHT DOWN neighbour
			neighbour[6].create(current.x - 1, current.y + 1);	//LEFT UP neighbour
			neighbour[7].create(current.x - 1, current.y - 1);	//LEFT DOWN neighbour

			if (current == goal)
			{
				goalReached = true;
				return;
			}

			for (uint i = 0; i < 8; ++i)
			{
				if (MovementCost(neighbour[i].x, neighbour[i].y) > 0)
				{
					if (visited.find(neighbour[i]) == -1)
					{
						frontier.Push(neighbour[i], 0);
						visited.add(neighbour[i]);
						breadcrumbs.add(current);
					}
				}
			}
		}
	}
}

void j1Map::DrawPath()
{
	iPoint point;

	// Draw visited
	p2List_item<iPoint>* item = visited.start;

	while(item)
	{
		point = item->data;
		TileSet* tileset = GetTilesetFromTileId(26);

		SDL_Rect r = tileset->GetTileRect(26);
		iPoint pos = MapToWorld(point.x, point.y);

		App->render->Blit(tileset->texture, pos.x, pos.y, &r);

		item = item->next;
	}

	// Draw frontier
	for (uint i = 0; i < frontier.Count(); ++i)
	{
		point = *(frontier.Peek(i));
		TileSet* tileset = GetTilesetFromTileId(25);

		SDL_Rect r = tileset->GetTileRect(25);
		iPoint pos = MapToWorld(point.x, point.y);

		App->render->Blit(tileset->texture, pos.x, pos.y, &r);
	}

	// Draw path
	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = MapToWorld(path[i].x, path[i].y);
		App->render->Blit(tile_x, pos.x, pos.y);
	}
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	p2List_item<MapLayer*>* item = data.layers.start;

	for(; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if(layer->properties.Get("Nodraw") != 0)
			continue;

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if(tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					App->render->Blit(tileset->texture, pos.x, pos.y, &r);
				}
			}
		}
	}

	DrawPath();
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while(item)
	{
		if(item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while(item)
	{
		if(id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = (x / data.tile_width);
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		
		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int( (x / half_width + y / half_height) / 2) - 1;
		ret.y = int( (y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
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

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.add(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if(data != NULL)
	{
		pugi::xml_node prop;

		for(prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.add(p);
		}
	}

	return ret;
}