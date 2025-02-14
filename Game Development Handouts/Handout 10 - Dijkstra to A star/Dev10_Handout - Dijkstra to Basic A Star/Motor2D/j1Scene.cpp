#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
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
	App->map->Load("iso_walk.tmx");
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		App->map->ResetPath();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		App->map->PropagateBFS();

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
		App->map->PropagateBFS();

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
		App->map->PropagateDijkstra();

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		App->map->PropagateDijkstra();

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->map->PropagateAStar();

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
		App->map->PropagateAStar();
	
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		iPoint p;
		App->input->GetMousePosition(p.x, p.y);
		App->map->Path(p.x - App->render->camera.x, p.y - App->render->camera.y);

		//Gets the position of the mouse, translates its coordinates to Map coordinates (tiles) and then sets the goal as that position.
		App->map->goal = App->map->WorldToMap(p.x - App->render->camera.x, p.y - App->render->camera.y);	
	}

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
