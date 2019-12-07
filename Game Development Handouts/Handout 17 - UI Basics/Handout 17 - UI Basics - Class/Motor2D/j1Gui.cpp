#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}


//------------------------------------------------ GUI METHODS ------------------------------------------------
GuiImage::GuiImage(int x, int y, SDL_Rect rect)
{
	position.x = x;
	position.y = y;

	imgRect = rect;
}

GuiText::GuiText(int x, int y, p2SString string)
{
	App->font->Print(string.GetString());
	
	position.x = x;
	position.y = y;
}

GuiImage* j1Gui::CreateGuiImage(int x, int y, SDL_Rect rect)
{
	GuiImage* gui_image = new GuiImage(x, y, rect);

	elements.add(gui_image);

	return gui_image;
}

GuiText* j1Gui::CreateGuiText(int x, int y, SDL_Rect rect, p2SString string)
{
	GuiText* gui_text = new GuiText(x, y, string);

	elements.add(gui_text);

	return gui_text;
}

// class Gui ---------------------------------------------------

