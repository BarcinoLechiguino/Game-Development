#include "j1App.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "UI.h"
#include "UI_Text.h"

UI_Text::UI_Text(UI_Element type, int x, int y, SDL_Rect hitbox, p2SString* string) : UI(UI_Element::TEXT, x, y, hitbox)
{
	this->string = string;
	
	font = App->font->Load("fonts/open_sans/OpenSans-SemiboldItalic.ttf");			//Loads the font.

	SDL_Color font_colour = { 255, 255, 255, 255 };									//Colour of the font, set to white and with no transparence.

	tex = App->font->Print(string->GetString(), font_colour, font);					//Generates a texture with the given string with the a font and a colour.
}

bool UI_Text::Draw()
{	
	BlitElement(tex, GetPosition().x, GetPosition().y, NULL);

	return true;
}