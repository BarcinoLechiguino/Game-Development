#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

// TODO 1: Include Box 2 header and library
#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment (lib,"Box2D/libx86/Debug/Box2D.lib")
#else
#pragma comment (lib,"Box2D/libx86/Release/Box2D.lib")
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{

}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it
	b2Vec2 gravity(0.0f, 9.8f);
	world = new b2World(gravity);

	// TODO 4: Create a a big static circle as "ground"
	b2BodyDef ground;
	ground.type = b2_staticBody;
	ground.position.Set(PIXELS_TO_METERS(SCREEN_WIDTH/2), PIXELS_TO_METERS(SCREEN_HEIGHT/1.5f));
	b2Body * groundBody = world->CreateBody(&ground);

	b2CircleShape globeEarth;
	globeEarth.m_radius = PIXELS_TO_METERS(SCREEN_WIDTH/3);

	b2FixtureDef fixture;
	fixture.shape = &globeEarth;
	groundBody->CreateFixture(&fixture);

	//Homework: Create a big static rectangle as 2nd "ground"
	b2BodyDef groundRectangle;
	groundRectangle.type = b2_staticBody;
	groundRectangle.position.Set(PIXELS_TO_METERS(SCREEN_WIDTH / 2), PIXELS_TO_METERS(SCREEN_HEIGHT / 1.5f));
	b2Body * groundRectangleBody = world->CreateBody(&groundRectangle);

	b2PolygonShape flatEarth;
	flatEarth.SetAsBox(PIXELS_TO_METERS(SCREEN_WIDTH / 2.5f), PIXELS_TO_METERS(SCREEN_HEIGHT / 5));

	b2FixtureDef fixtureRectangle;
	fixtureRectangle.shape = &flatEarth;
	groundRectangleBody->CreateFixture(&fixtureRectangle);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	world->Step(1.0f / 60.0f, 8, 3);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	SDL_Rect rect;

	rect.x = PIXELS_TO_METERS(SCREEN_WIDTH / 2);
	rect.y = PIXELS_TO_METERS(SCREEN_HEIGHT / 1.5f);
	rect.w = PIXELS_TO_METERS(SCREEN_WIDTH / 2.5f);
	rect.h = PIXELS_TO_METERS(SCREEN_HEIGHT / 5);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		b2BodyDef circle;
		circle.type = b2_dynamicBody;
		circle.position.Set(PIXELS_TO_METERS(App->input->GetMouseX()), PIXELS_TO_METERS(App->input->GetMouseY()));
		
		b2Body * circene = world->CreateBody(&circle);

		b2CircleShape ball;
		ball.m_radius = PIXELS_TO_METERS(100);

		b2FixtureDef fixtureII;
		fixtureII.shape = &ball;
		circene->CreateFixture(&fixtureII);
	}

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
				case b2Shape::e_polygon:
				{
					b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawQuad(rect, 255, 255, 255, false, false);
				}
				break;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	b2World *world = nullptr;

	return true;
}
