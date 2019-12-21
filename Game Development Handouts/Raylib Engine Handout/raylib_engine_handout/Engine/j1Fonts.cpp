#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"

j1Fonts::j1Fonts() : j1Module()
{
	name.create("fonts");
}

// Destructor
j1Fonts::~j1Fonts()
{}

// Called after window is available
bool j1Fonts::Awake(pugi::xml_node& conf)
{
	bool ret = true;

    LOG("Init default font");

	const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
	int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);

    font = LoadFontEx(path, size, NULL, 0);

	return ret;
}

// Called before quitting
bool j1Fonts::CleanUp()
{
	LOG("Freeing font");

    UnloadFont(font);

	return true;
}

Font j1Fonts::GetFont() const
{
    return font;
}

// calculate the height of a font type
int j1Fonts::GetFontHeight(Font font) const
{
    return font.baseSize;
}

// calculate size of a text
bool j1Fonts::GetTextSize(const char* text, int& width, int& height, int spacing) const
{
	bool ret = false;

    MeasureTextEx(font, text, font.baseSize, spacing);

	return ret;
}

