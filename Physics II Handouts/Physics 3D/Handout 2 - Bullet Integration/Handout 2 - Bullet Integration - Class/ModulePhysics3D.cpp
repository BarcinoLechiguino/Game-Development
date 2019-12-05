#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"

//#define GRAVITY btVector3(0.0f, -9.8f, 0.0f) //We can define a macro with a class...

// TODO 1: ...and the 3 libraries based on how we compile (Debug or Release)
// use the _DEBUG preprocessor define
#if _DEBUG
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")	
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else 
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")	
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) 
	: Module(app, start_enabled),
	world(nullptr),			//Initializes the value of the pointers to nullptr when the constructor is called. A small optimization over initializing them in the .h
	dispatcher(nullptr),
	broadphase(nullptr),
	solver(nullptr),
	config(nullptr)
{
	debug_draw = NULL;
	debug = true;

	// TODO 2: Create collision configuration, dispacher
	// broad _phase and solver
	//Here we need to assign the pointers to the specific classes we need.
	config = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(config);		//Collision dispatcher requires to be passed a collision configuration as argument.
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);

	//TODO 4: Uncomment the creation of the DebugDrawer
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;

	// TODO 2: and destroy them!
	delete world;					//Needs to be deleted first.
	delete dispatcher;				//Using pointers makes memory leak handling far easier.
	delete broadphase;
	delete solver;
	delete config;

	
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	// TODO 3: Create the world and set default gravity
	// Have gravity defined in a macro!

	//World is created in the constructor and destroyed int the destructor

	world->setGravity(GRAVITY);

	//TODO 4: Uncomment and link the debug Drawer with our newly created Physics world
	world->setDebugDrawer(debug_draw);
	{
		// TODO 6: Create a big rectangle as ground
		// Big rectangle as ground
		mass = 0.0f;
		mState = new btDefaultMotionState();
		cShape = new btBoxShape(btVector3(200.0f, 1.0f, 200.0f));

		btRigidBody::btRigidBodyConstructionInfo construction_info(mass, mState, cShape);
		
		body = new btRigidBody(construction_info);		//Creates the rigid body

		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	// TODO 5: step the world
	world->stepSimulation(dt);

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		//TODO 4: Uncomment the render of the debug render
		world->debugDrawWorld();
		
		if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			// TODO 7: Create a Solid Sphere when pressing 1 on camera position
			mat4x4 matrix = IdentityMatrix;																	//First we create the matrix.
			matrix.translate(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);	//Translate actualizes the position of the matrix (x, y, z).

			//btTransform transform = new btTransform;														//Declaring a transform so it can be set with the matrix's data members.
			btTransform transform;
			transform.setFromOpenGLMatrix(&matrix);															//Sets the transform with the data from the matrix.

			mass = 1.0f;
			radius = 1.0f;
			//mState = new btDefaultMotionState();															//Default motion initializes everything to (0, 0, 0)
			mState = new btDefaultMotionState(transform);
			cShape = new btSphereShape(radius);

			btRigidBody::btRigidBodyConstructionInfo construction_info(mass, mState, cShape);

			body = new btRigidBody(construction_info);

			world->addRigidBody(body);
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// TODO 3: ... and destroy the world here!
	//Destroyed in the destructor as world was declared in the constructor.

	delete mState;
	delete cShape;
	//delete transform;

	return true;
}

// =============================================
//TODO 4: Uncomment the definition of the Debug Drawer

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
