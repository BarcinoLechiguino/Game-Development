#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "j1Module.h"

#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 8

class j1Fonts : public j1Module
{
public:

	j1Fonts();

	// Destructor
	virtual ~j1Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

    Font GetFont() const;
    int GetFontHeight(Font font) const;
	bool GetTextSize(const char* text, int& width, int& height, int spacing = 0) const;

private:

    Font font;
};


#endif // __j1FONTS_H__