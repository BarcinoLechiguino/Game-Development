#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
enum class UI_ELEMENT
{
	IMAGE,
	TEXT,
	BUTTON,
	TOGGLE,
	SCROLLBAR,
	CHECKBOX
};

class Gui
{
public:
	//virtual ~Gui();

	bool Update();

	void Draw();

public:
	iPoint position;
};

class GuiImage : public Gui
{
public:
	GuiImage(int x, int y, SDL_Rect rect);
	~GuiImage();

	bool Update();

	void Draw(SDL_Texture* atlas);

private:
	SDL_Rect imgRect;
};

class GuiText : public Gui
{
public:
	GuiText(int x, int y, p2SString string);
	~GuiText();

	bool Update();

	void Draw();

private:
	SDL_Texture* text;
};
// ---------------------------------------------------

class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	GuiImage* CreateGuiImage(int x, int y, SDL_Rect rect);
	GuiText* CreateGuiText(int x, int y, SDL_Rect rect, p2SString string);

	const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<Gui*> elements;					//This is where the elements are added.
};

#endif // __j1GUI_H__