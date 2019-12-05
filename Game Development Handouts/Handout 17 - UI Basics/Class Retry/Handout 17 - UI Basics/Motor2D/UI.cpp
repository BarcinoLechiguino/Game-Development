#include "j1Gui.h"
#include "UI.h"

UI::UI(UI_ELEMENT element, int x, int y, SDL_Rect rect) : element(element), position(x, y), rectangle(rect)
{
	
}

UI::~UI()
{

}

bool UI::Update()
{
	return true;
}

bool UI::Draw()
{
	return true;
}