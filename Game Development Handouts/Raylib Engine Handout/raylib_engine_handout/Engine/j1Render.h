#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "p2Point.h"
#include "j1Module.h"

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Utils
	iPoint ScreenToWorld(int x, int y) const;

    // Set background color
    void SetBackgroundColor(Color color);

	// Draw texture
	bool Draw(Texture2D texture, int x, int y, const Rectangle* section = NULL, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;

    // Draw text
    bool DrawText(const char* text, int x, int y, Font font, int spacing, Color tint) const;

public:

	Color background;
    Rectangle camera;
};

#endif // __j1RENDER_H__