#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"

j1Window::j1Window() : j1Module()
{
	name.create("window");
}

// Destructor
j1Window::~j1Window()
{
}

// Called before render is available
bool j1Window::Awake(pugi::xml_node& config)
{
	bool ret = true;

	unsigned int flags = 0;
	bool fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
	bool borderless = config.child("borderless").attribute("value").as_bool(false);
	bool resizable = config.child("resizable").attribute("value").as_bool(false);
	//bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(false);
    bool vsync = config.child("vsync").attribute("value").as_bool(true);

	width = config.child("resolution").attribute("width").as_int(640);
	height = config.child("resolution").attribute("height").as_int(480);
	scale = config.child("resolution").attribute("scale").as_int(1);

	if (fullscreen == true) flags |= FLAG_FULLSCREEN_MODE;

	if (borderless == true) flags |= FLAG_WINDOW_UNDECORATED;

	if (resizable == true) flags |= FLAG_WINDOW_RESIZABLE;

    if (vsync == true) flags |= FLAG_VSYNC_HINT;

    LOG("Init raylib window");

    SetConfigFlags(flags);
    InitWindow(width, height, "title");

	return ret;
}

// Called each loop iteration
bool j1Window::PreUpdate()
{
    if (WindowShouldClose()) windowEvents[WINDOW_EVENT_QUIT] = true;
    if (IsWindowMinimized()) windowEvents[WINDOW_EVENT_HIDE] = true;
    //if (IsWindowRestored()) windowEvents[WINDOW_EVENT_SHOW] = true;   // Not available
    if (IsWindowResized()) windowEvents[WINDOW_EVENT_RESIZE] = true;

    return true;
}

// Called before quitting
bool j1Window::CleanUp()
{
	LOG("Close raylib window");

    CloseWindow();

	return true;
}

// Set new window title
void j1Window::SetTitle(const char* new_title)
{
	SetWindowTitle(new_title);
}

uint j1Window::GetWidth() const
{
	return width;
}

uint j1Window::GetHeight() const
{
    return height;
}

uint j1Window::GetScale() const
{
	return scale;
}

bool j1Window::GetWindowEvent(j1WindowEvent ev)
{
    return windowEvents[ev];
}