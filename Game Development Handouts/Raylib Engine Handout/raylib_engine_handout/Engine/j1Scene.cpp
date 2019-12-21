#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Fonts.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if(App->map->CreateWalkabilityMap(w, h, &data)) App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

    selector = LoadTexture("maps/path2.png");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->renderer->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update()
{
	if(IsKeyPressed(KEY_L)) App->LoadGame("save_game.xml");
	if(IsKeyPressed(KEY_S)) App->SaveGame("save_game.xml");

    // TODO: Consider delta time on camera movement
	if(IsKeyDown(KEY_UP)) App->renderer->camera.y += 1;
	if(IsKeyDown(KEY_DOWN)) App->renderer->camera.y -= 1;
	if (IsKeyDown(KEY_LEFT)) App->renderer->camera.x += 1;
	if (IsKeyDown(KEY_RIGHT)) App->renderer->camera.x -= 1;

	App->map->Draw();
/*
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->renderer->camera.x, y - App->renderer->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	App->window->SetTitle(title.GetString());
*/
	// Debug pathfinding ------------------------------
    int x, y;
	App->input->GetMousePosition(x, y);
    iPoint p = App->renderer->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->renderer->Draw(selector, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);

		App->renderer->Draw(selector, pos.x, pos.y);
	}

    App->renderer->DrawText("THIS is an EFFICIENT TEXT render!", 10, 10, App->fonts->GetFont(), 2, GREEN);

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(IsKeyPressed(KEY_ESCAPE)) ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
