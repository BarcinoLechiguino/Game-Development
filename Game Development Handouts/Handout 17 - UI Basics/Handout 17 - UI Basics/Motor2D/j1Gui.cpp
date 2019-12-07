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

	ui_debug = false;

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
	for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
	{
		element_iterator->data->Draw();
	}

	Debug_UI();

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	//Iterate the elements list and frees all allocated memory.
	for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
	{
		RELEASE(element_iterator->data);
	}

	elements.clear();

	return true;
}

// const getter for atlas
/*const*/ SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

UI* j1Gui::CreateElement(UI_Element element, int x, int y, SDL_Rect* rect, p2SString* string)
{
	UI* elem = nullptr;

	switch (element)
	{
	case UI_Element::IMAGE:
		elem = new UI_Image(element, x, y, *rect);			//In *rect the "*" is used to access rect pointer's data members.
		break;

	case UI_Element::TEXT:
		elem = new UI_Text(element, x, y, *rect, string);
		break;

	case UI_Element::BUTTON:
		break;

	case UI_Element::SCROLLBAR:
		break;

	case UI_Element::INPUTBOX:
		break;
	}

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

void j1Gui::Debug_UI()
{
	if (ui_debug == true)
	{
		for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
		{	
			App->render->DrawQuad(element_iterator->data->GetHitbox(), 255, 0, 0, 255, false, false);
		}
	}
}

// class Gui ---------------------------------------------------

