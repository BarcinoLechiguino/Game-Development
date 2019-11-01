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

// TODO 6: Create a small class that keeps a pointer to the b2Body
// and has a method to request the position
// then write the implementation in the .cpp
// Then make your circle creation function to return a pointer to that class
class Object
{	
public:
	b2Body*	objectBody;				//Pointer to b2Body
	b2Vec2*	GetPosition();			//Has to return a vector
	b2Vec2*	GetCenterPosition();
	float	GetRotation();
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	Object* object;

	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains
	//Creation of a circle on screen.

	//We have the CreateCircle method return a pointer to the object class.
	Object* CreateCircle(int posX, int posY, int radius)
	{	
		Object* Circle = new Object;
		
		b2BodyDef bodyCircle;													//Declares the frame for the geometrical form.
		bodyCircle.type = b2_dynamicBody;										//Defines the type of the frame/body as a dynamic body.
		float radi = PIXEL_TO_METERS(radius);									//Defines the radius variable of the circle. PIXEL_TO_METERS converts pixels to meters. In this environment we have set 50 pixels per meter.
		bodyCircle.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));	//Sets the position where the shape will be created.

		Circle->objectBody = world->CreateBody(&bodyCircle);					//Creates a body given a definition.

		b2CircleShape shapeCircle;												//We define that we want to create a circle and name it shape
		shapeCircle.m_radius = radi;											//We set the radius with the radi variable we have declared and translated to meters.
		b2FixtureDef fixture;													//Definition of the fixture. This has multiple elements (shape(circle), density(weight) and friction(how it drags along other surfaces))
		fixture.shape = &shapeCircle;											//Defines which shape will the fixture have.

		Circle->objectBody->CreateFixture(&shapeCircle, 1.0f);					//Fixture definition automatically updates the mass of the body. As many fixtures as wanted can be added.
		
		return Circle;
	}

	//Creation of a rectangle on screen
	Object* CreateRectangle(int posX, int posY, int width, int height)
	{
		// TODO 1: When pressing 2, create a box on the mouse position
		Object* Box  = new Object;

		b2BodyDef bodyBox;
		bodyBox.type = b2_dynamicBody;
		bodyBox.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));

		Box->objectBody = world->CreateBody(&bodyBox);

		b2PolygonShape shapeBox;
		shapeBox.SetAsBox(PIXEL_TO_METERS(width), PIXEL_TO_METERS(height));
		b2FixtureDef fixture;
		fixture.shape = &shapeBox;

		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
		Box->objectBody->CreateFixture(&shapeBox, 1.0f);

		return Box;
	}

	//Creation of a chain on screen
	Object* CreateChain(int posX, int posY, b2Vec2 chainVectors[], const int numVectors, int chainPoints[], const int numPoints)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!
		chainVectors[numVectors];

		int ChainPoints[512];							//Revise if size can be passed as an argument.
		
		for (int i = 0; i < numPoints; i++)				//Loop that iterates to fill an array with the passed argument's array of values. Had to be done this way due to the fact that an array index cannot be passed as an argument individually and then matched in the function.
		{
			ChainPoints[i] = chainPoints[i];
		}

		for (int i = 0; i < numVectors; i++)			//Iterates the array of vectors to assign each one of them a position on screen.
		{
			chainVectors[i].Set(PIXEL_TO_METERS(chainPoints[i * 2]), PIXEL_TO_METERS(chainPoints[i * 2 + 1])); 
		}

		Object*	Chain = new Object;

		b2BodyDef	chainBody;													//Holds ALL the data required to construct a rigid body. In this case a chain.
		chainBody.type = b2_dynamicBody;										//Sets the chain body type to dynamic (interacts with the different forcess in the environment).
		chainBody.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));	//Sets the position of the whole body when it is created on screen.

		Chain->objectBody = world->CreateBody(&chainBody);		//Creates a rigid body given a body definition (the definition the chain body in this case.)

		b2ChainShape chainShape;								//Defines a Chain shape and name it as shown.
		b2FixtureDef chainFixture;								//Definition of the fixture. This has multiple elements (shape(circle), density(weight) and friction(how it drags along other surfaces)) 
		//chainFixture.shape = &chainShape;						//Sets the fixture shape to the aforementioned chainShape. Called later in the Create fixture function

		chainShape.CreateLoop(chainVectors, numVectors);		//Adjusts the connectivity of the vertexs

		Chain->objectBody->CreateFixture(&chainShape, 1.0f);	//Fixture definition automatically updates the mass of the body. As many fixtures as wanted can be added.

		return Chain;
	}

private:

	bool debug;
	b2World* world;
};