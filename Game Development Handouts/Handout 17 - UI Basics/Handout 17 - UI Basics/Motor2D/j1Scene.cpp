#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
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
	if (App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	// TODO 3: Create the banner (rect {485, 829, 328, 103}) as a UI element
	SDL_Rect rect{ 485, 829, 328, 103 };
	SDL_Rect winRect{ 0, 512, 483, 512 };

	window = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, 225, 25, winRect, true, false, true, NULL);		//OnCallEvent Overlap
	banner = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, 300, 100, rect, true, false, false, NULL);

	// TODO 4: Create the text "Hello World" as a UI element
	SDL_Rect textHitbox{ 432, 75, 65, 20};
	SDL_Rect interactibleTextHitbox{ 432, 75, 134, 20};
	p2SString bufferString = "Hello World";
	p2SString interactibleString = "This string is interactible.";
	_TTF_Font* font = App->font->Load("fonts/open_sans/OpenSans-SemiboldItalic.ttf");
	SDL_Color fontRgb = { 255, 255, 255, 255 };

	interactibleText = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 280, 75, interactibleTextHitbox, font, fontRgb, true, true, false, NULL, &interactibleString);
	text = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 432, 75, textHitbox, font, fontRgb, true, false, false, window, &bufferString);

	
	//Creating a button:
	SDL_Rect buttonHitbox	= { 642,169,229,69 };
	SDL_Rect idle			= { 642,169,229,69 };
	SDL_Rect hover			= { 0,113,229,69 };
	SDL_Rect clicked		= { 411,169,229,69 };

	SDL_Rect buttonLabelHitbox{ 432, 75, 45, 18 };
	p2SString buttonString = "BUTTON";

	button = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 205, true, true, false, NULL, &idle, &hover, &clicked);
	escButton = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 280, true, true, false, NULL, &idle, &hover, &clicked);
	
	draggableButton = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 355, true, true, true, window, &idle, &hover, &clicked);
	buttonLabel = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 442, 380, buttonLabelHitbox, font, fontRgb, true, false, false, draggableButton, &buttonString);

	draggableButton2 = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 425, true, true, true, window, &idle, &hover, &clicked);

	LOG("Screen Pos Drag Button: (%d, %d)", draggableButton->GetScreenPos().x, draggableButton->GetScreenPos().y);
	LOG("Local Pos Drag Button: (%d, %d)", draggableButton->GetLocalPos().x, draggableButton->GetLocalPos().y);
	LOG("Local Pos Drag Button 2: (%d, %d)", draggableButton2->GetLocalPos().x, draggableButton2->GetLocalPos().y);

	//draggableButton->SetLocalPos(iPoint(-100, -100));

	LOG("Screen Pos Drag Button: (%d, %d)", draggableButton->GetScreenPos().x, draggableButton->GetScreenPos().y);
	LOG("Local Pos Drag Button : (%d, %d)", draggableButton->GetLocalPos().x, draggableButton->GetLocalPos().y);


	//button = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 100, 205, NULL,&idle, &hover, &clicked);					//Testing the focus.
	
	//text = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 525, 75, textHitbox, font, fontRgb, text, NULL, &bufferString);		//Testing the focus.
	
	//button = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 600, 205, NULL, &idle, &hover, &clicked);					//Testing the focus

	//text = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 350, 75, textHitbox, font, fontRgb, text, NULL, &bufferString);		//Testing the focus.

	//escButton = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 360, NULL, &idle, &hover, &clicked);				//Testing the focus.

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
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
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
bool j1Scene::Update(float dt)
{
	// Gui ---
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		if (App->gui->ui_debug == false)
		{
			App->gui->ui_debug = true;
		}
		else
		{
			App->gui->ui_debug = false;
		}
	}
	// -------
	
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);
	

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

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
