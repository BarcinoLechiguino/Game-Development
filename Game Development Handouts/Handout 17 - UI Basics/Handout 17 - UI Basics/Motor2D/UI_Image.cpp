#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UI.h"
#include "UI_Image.h"


UI_Image::UI_Image(UI_Element element, int x, int y, SDL_Rect rect, UI_Image* elementCallback, UI* parent) : UI(UI_Element::IMAGE, x, y, rect, parent) //When a UI_Image's constructor is called, a UI element is initialized as a IMAGE element.
{
	tex = App->gui->GetAtlas();		//The atlas already has the path to the atlas spritesheet. Check how to work around the const

	interactible = false;			//A UI_Image type of UI Element will never be interactible.

	imgCallback = elementCallback;

	//focused = false;
}

bool UI_Image::Draw()
{
	BlitElement(tex, GetPosition().x, GetPosition().y, &GetRect());		//GetPosition() is used as the position variable in the UI parent class will be initialized with the values of the UI_Image instance at constructor call
	
	return true;
}