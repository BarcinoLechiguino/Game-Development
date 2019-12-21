#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"

#define VSYNC true

j1Render::j1Render() : j1Module()
{
	name.create("renderer");

	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 255;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	bool ret = true;

    camera.x = 590;     // TODO: Center map properly -> maths!
    camera.y = -20;     // TODO: Center map properly -> maths!
	camera.width = App->window->GetWidth();
	camera.height = App->window->GetHeight();

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
    ClearBackground(background);

    // NOTE: This function setups render batching system for
    // maximum performance, all consecutive Draw() calls are
    // not processed until EndDrawing() is called
    BeginDrawing();

	return true;
}

bool j1Render::PostUpdate()
{
    // Draw everything in our batch!
    EndDrawing();

	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void j1Render::SetBackgroundColor(Color color)
{
	background = color;
}

iPoint j1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->window->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

// Draw to screen
bool j1Render::Draw(Texture2D texture, int x, int y, const Rectangle* section, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

	uint scale = App->window->GetScale();
    Vector2 position = { (float)x, (float)y };
    Rectangle rect = { 0, 0, texture.width, texture.height };

    if (section != NULL) rect = *section;

    position.x = (float)x * scale + camera.x;
    position.y = (float)y * scale + camera.y;

	rect.width *= scale;
	rect.height *= scale;

    DrawTextureRec(texture, rect, position, WHITE);

	return ret;
}

bool j1Render::DrawText(const char * text, int x, int y, Font font, int spacing, Color tint) const
{
    bool ret = true;

    Vector2 position = { x, y };

    DrawTextEx(font, text, position, font.baseSize, spacing, tint);

    return ret;
}
