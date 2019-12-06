#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"

// TODO 1: ...and the 3 libraries based on how we compile (Debug or Release)
// use the _DEBUG preprocessor define

#if _DEBUG
	
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")

#else
	
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")

#endif


ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : 
	Module(app, start_enabled)
	, world(nullptr)					//Initializing all pointers to nullptr in the constructor so at constructor call they are initialized.
	, dispatcher(nullptr)
	, broadphase(nullptr)
	, solver(nullptr)
	, collision_config(nullptr)
{
	debug_draw = NULL;
	debug = true;

	// TODO 2: Create collision configuration, dispacher
	// broad _phase and solver
	collision_config = new btDefaultCollisionConfiguration();	//Declares a new collision configuration and allocates memory for it. It's of the btDefaultCollisionConfiguration type.
	dispatcher = new btCollisionDispatcher(collision_config);	//Declares a new collision dispatcher and allocates memory for it. It's of the btCollisionDispatcher type
	broadphase = new btDbvtBroadphase();						//Declares a new broadphase and allocates memory for it. It's of the btDbvtBroadphase type.
	solver = new btSequentialImpulseConstraintSolver();			//Declares a new constraint solver and allocates memory for it. It's of the btSequentialImpulseConstraintSolver type.

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);	//Creates the world where the physics simulation.

	//TODO 4: Uncomment the creation of the DebugDrawer
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;

	// TODO 2: and destroy them!
	delete world;
	delete solver;				//Deleted in reverse order of declaration.
	delete broadphase;			
	delete dispatcher;			//If collision_config was deleted before dispatcher, the game
	delete collision_config;	//would crash as dispatcher receives collision_config as argument.
}

// ---------------------------------------------------------

bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	// TODO 3: Create the world and set default gravity
	// Have gravity defined in a macro!
	world->setGravity(GRAVITY);																//Sets the world's gravity to -9.8f in the Y axis. Defined with the GRAVITY macro in Globals.h 

	//TODO 4: Uncomment and link the debug Drawer with our newly created Physics world
	world->setDebugDrawer(debug_draw);

	{
		// TODO 6: Create a big rectangle as ground
		// Big rectangle as ground
		mass = 0.0f;							//An object with a mass of value 0 will be static.

		btVector3 ground(200, 1, 200);

		motionState = new btDefaultMotionState();
		collisionShape = new btBoxShape(ground);

		btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape);

		rigidBody = new btRigidBody(constructionInfo);

		world->addRigidBody(rigidBody);
	} 

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	// TODO 5: step the world
	world->stepSimulation(dt);		//Setting the world step with dt. If a timestep is larger than 60Hz, more than 1 simulation step can be performed during each "stepSimulation".

	//Holds all the information about a rigid body.

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
			mass = 1.0f;		//As we want the sphere to be dynamic, we add a mass != 0.
			radius = 1.0f;

			mat4x4 matrix = IdentityMatrix;
			matrix.translate(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

			btTransform transform;
			transform.setFromOpenGLMatrix(&matrix);

			motionState = new btDefaultMotionState(transform);
			collisionShape = new btSphereShape(radius);

			btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape);
			
			rigidBody = new btRigidBody(constructionInfo);

			//rigidBody->applyImpulse(btVector3(10, 2, 10), transform.getOrigin());		//Applies an impulse to the created sphere in the given direction (vector).

			//mass*500 ensures that the force applied corresponds with the mass so the same force impulse is appreciated.
			rigidBody->applyForce(btVector3(mass*500, 0, 0), transform.getOrigin());		//Applies a force to the created sphere in the given direction (vector).

			//rigidBody->getLocalInertia();

			//rigidBody->applyTorqueImpulse(btVector3(1000, 2, 10));

			world->addRigidBody(rigidBody);
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
//	delete rigidBody;
	delete motionState;
	delete collisionShape;

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
