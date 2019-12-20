#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Scene.h"

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
	escape = true;

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		PassFocus();
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (focusedElement != nullptr && focusedElement->isInteractible)
		{
			focusedElement->ui_event = UI_Event::UNCLICKED;
			OnEventCall(focusedElement, focusedElement->ui_event);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{

	}
	
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{	
	//escape = true;
	
	for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
	{
		switch (element_iterator->data->element)
		{
		case UI_Element::IMAGE:
			element_iterator->data->Draw();
			break;

		case UI_Element::TEXT:
			element_iterator->data->Draw();
			break;

		case UI_Element::BUTTON:
			element_iterator->data->Draw();

			break;

		case UI_Element::SCROLLBAR:
			break;

		case UI_Element::INPUTBOX:
			break;
		}
	}

	Debug_UI();

	return escape;
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

//----------------------------------- UI ELEMENT CREATION METHODS -----------------------------------
UI* j1Gui::CreateImage(UI_Element element, int x, int y, SDL_Rect hitbox, bool isInteractible, bool isDraggable, UI* parent)
{
	UI* elem = nullptr;

	elem = new UI_Image(element, x, y, hitbox, isInteractible, isDraggable, parent);

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

UI* j1Gui::CreateText(UI_Element element, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, bool isInteractible, bool isDraggable, UI* parent, p2SString* string,
					p2SString* hoverString, p2SString* focusString, p2SString* leftClickString, p2SString* rightClickString)
{
	UI* elem = nullptr;

	elem = new UI_Text(element, x, y, hitbox, font, fontColour, isInteractible, isDraggable, parent, string, hoverString, focusString, leftClickString, rightClickString);

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

UI* j1Gui::CreateButton(UI_Element element, int x, int y, bool isInteractible, bool isDraggable, UI* parent, SDL_Rect* idle, SDL_Rect* hover, SDL_Rect* clicked)
{
	UI* elem = nullptr;

	elem = new UI_Button(element, x, y, isInteractible, isDraggable, parent, idle, hover, clicked);

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

UI* j1Gui::CreateUI_Window(UI_Element element, int x, int y, SDL_Rect hitbox, bool isInteractible, bool isDraggable, UI* parent)
{
	UI* elem = nullptr;

	elem = new UI_Window(element, x, y, hitbox, isInteractible, isDraggable, parent);

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

//--------------------------------- INPUT PROCESSING METHODS ---------------------------------
void j1Gui::OnEventCall(UI* element, UI_Event ui_event)
{
	if (element == App->scene->button && ui_event == UI_Event::UNCLICKED)		//If the pointer received is the UI_Button* button pointer of Scene.h and event = clicked. 
	{
		App->gui->ui_debug = !App->gui->ui_debug;							//Enables / Disables UI Debug Mode.
	}

	if (element == App->scene->escButton && ui_event == UI_Event::UNCLICKED)	//If the pointer received is the UI_Button* escbutton pointer of Scene.h and event = clicked.
	{
		escape = false;
	}

	if (element == App->scene->draggableButton && ui_event == UI_Event::UNCLICKED)
	{
		App->gui->ui_debug = !App->gui->ui_debug;
	}

	if (element == App->scene->interactibleText && ui_event == UI_Event::UNCLICKED)
	{
		App->gui->ui_debug = !App->gui->ui_debug;
	}
}

//----------------------------------- FOCUS MANAGEMENT METHODS -----------------------------------
void j1Gui::PassFocus()
{
	if (iteratedElement == nullptr)
	{
		iteratedElement = elements.start;
	}
	
	if (focusedElement == nullptr)
	{
		for (iteratedElement; iteratedElement != NULL; iteratedElement = iteratedElement->next)		//Loop that is used to find the first interactible element of the elments list.
		{
			if (iteratedElement->data->isInteractible && ElementCanBeFocused(iteratedElement->data))												//If the element being iterated is interactible.
			{
				focusedElement = iteratedElement->data;												//UI* focusedElement is set with the UI* of the element being iterated.
				break;																				//The loop is stopped.
			}
		}

		return;																						//Stops the function here so the focus remains in the first interactible element.
	}

	for (iteratedElement; iteratedElement != NULL; iteratedElement = iteratedElement->next)			//Loop that is used to find the next interactible element of the elments list.
	{
		if (iteratedElement->next != NULL)															//If the next element of the list is not NULL.
		{
			if (iteratedElement->next->data->isInteractible && ElementCanBeFocused(iteratedElement->next->data))											//If the next element of the list is interactible.
			{
				focusedElement = iteratedElement->next->data;										//UI* focusedElement is set with the UI* of the element next to the element being iterated. 
				iteratedElement = iteratedElement->next;											//The element being iterated is set to the next element in the list.
				break;																				//The loop is stopped so the focus remains in the interactible element that now has the focus.
			}
		}
		else																						//If the next element of the list is NULL.
		{
			iteratedElement = nullptr;																//The list_item is set to nullptr.
			focusedElement = nullptr;																//The UI* focused element is set to nullptr, which efectively disables the focus.
			break;																					//The loop is stopped so no element regains the focus.
		}
	}
}

bool j1Gui::ElementCanBeFocused(UI* focusElement)
{
	bool ret = false;

	if (focusElement->element == UI_Element::BUTTON || focusElement->element == UI_Element::SCROLLBAR)
	{
		ret = true;
	}

	return ret;
}

// --------------------------- SHOW & HIDE UI ELEMENTS METHODS --------------------------
void j1Gui::ShowElement(UI* parentElement)
{
	// Set the parent element's isVisible/isInteractible to true.
	// Do the same for any element that has the parent Element as a parent.
}

void j1Gui::HideElement(UI* parentElement)
{
	// Set the parent element's isVisible/isInteractible to false.
	// Do the ame for any element that has the parent element as a parent.
}


//----------------------------------- UI DEBUG METHOD -----------------------------------
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

