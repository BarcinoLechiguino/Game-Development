#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXELS 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXELS * p)

class b2World;
class b2Body;

// TODO 6: Create a small class that keeps a pointer to tghe b2Body
// and has a method to request the position
// then write the implementation in the .cpp
// Then make your circle creation function to return a pointer to that class


class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains
	//Creation of a circle on screen.
	void CreateCircle(int posX, int posY, int radius)
	{
		b2BodyDef bodyCircle; //Declares the frame for the geometrical form.
		bodyCircle.type = b2_dynamicBody; //Defines the type of the frame/body as a dynamic body.
		float radi = PIXEL_TO_METERS(radius); //Defines the radius variable of the circle. PIXEL_TO_METERS converts pixels to meters. In this environment we have set 50 pixels per meter.
		bodyCircle.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY)); //Sets the position where the shape will be created.

		b2Body* b = world->CreateBody(&bodyCircle); //Creates a body given a definition.

		b2CircleShape shape; //We define that we want to create a circle and name it shape
		shape.m_radius = radi; //We set the radius with the radi variable we have declared before and translated to meters.
		b2FixtureDef fixture; //Definition of the fixture. This has multiple elements (shape(circle), density(weight) and friction(how it drags along other surfaces))
		fixture.shape = &shape; //Defines which shape will the fixture have.

		b->CreateFixture(&fixture); //Fixture definition automatically updates the mass of the body. As many fixtures as wanted can be added.
	}

	//Creation of a rectangle on screen
	void CreateRectangle(int posX, int posY, int width, int height)
	{
		// TODO 1: When pressing 2, create a box on the mouse position
		b2BodyDef bodyBox;
		bodyBox.type = b2_dynamicBody;
		bodyBox.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));

		b2Body* bBox = world->CreateBody(&bodyBox);

		b2PolygonShape shapeBox;
		shapeBox.SetAsBox(PIXEL_TO_METERS(width), PIXEL_TO_METERS(height));
		b2FixtureDef fixture;
		fixture.shape = &shapeBox;

		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
		bBox->CreateFixture(&shapeBox, 1.0f);
	}

	//Creation of a chain on screen
	void CreateChain(int posX, int posY, b2Vec2 chainVectors[], const int numVectors, int chainPoints[], const int numPoints)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!
		/*b2Vec2 chainVectors[12]; //b2Vec2 Array passed as function argument

		int chainPoints[24] =
		{
			-38, 80,
			-44, -54,
			-16, -60,
			-16, -17,
			19, -19,
			19, -79,
			61, -77,
			57, 73,
			17, 78,
			20, 16,
			-25, 13,
			-9, 72
		};*/

		chainVectors[numVectors];

		int ChainPoints[256];
		
		for (int i = 0; i < numPoints; i++)
		{
			ChainPoints[i] = chainPoints[i];
		}

		//chainPoints[numPoints] = {};

		for (int i = 0; i < numVectors; i++)
		{
			chainVectors[i].Set(PIXEL_TO_METERS(chainPoints[i * 2]), PIXEL_TO_METERS(chainPoints[i * 2 + 1]));
		}

		b2BodyDef chainBody;
		chainBody.type = b2_dynamicBody;
		chainBody.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));

		b2Body* b_chain = world->CreateBody(&chainBody);

		b2ChainShape chainShape;
		b2FixtureDef chainFixture;
		chainFixture.shape = &chainShape;

		chainShape.CreateLoop(chainVectors, numVectors); //Adjusts the connectivity of the vertexs

		b_chain->CreateFixture(&chainShape, 1.0f);
	}

private:

	bool debug;
	b2World* world;
};