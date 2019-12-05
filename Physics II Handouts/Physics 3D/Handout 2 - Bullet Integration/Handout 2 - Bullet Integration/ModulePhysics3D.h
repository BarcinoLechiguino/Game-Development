#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "Primitive.h"

// TODO 1: Add Bullet common include btBulletDynamicsCommon.h
#include "Bullet/include/btBulletDynamicsCommon.h"

class DebugDrawer;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:

	//Here we need to create the pointers to the mother class, then in the cpp the specific classes need to be called.
	btDiscreteDynamicsWorld*	world;				//Physics world. Space where the physics will happen / be calculated.
	btCollisionDispatcher*		dispatcher;			//Detects collisions and finds contact points. Finds the right algorithm to calculate the collision between a pair of objects.
	btBroadphaseInterface*		broadphase;			//Does a first pass to detect objects that "may collide". Simplifies all objects to spheres or cubes.
	btConstraintSolver*			solver;				//Calculates how the constraints affect an object attached to  them and "solves" how the object and it's restraints interact.
	btCollisionConfiguration*	collision_config;	//Contains the default setup for bullet, with memory and collision setups.

	btMotionState*				motionState;		//Holds the position, velocity, inertia... of the body.
	btCollisionShape*			collisionShape;		//Defines the shape of the body. We can use a big.
	btRigidBody*				rigidBody;			//Body of an object in the world.

	bool debug;
	DebugDrawer* debug_draw;
};

//TODO 4: Uncomment the declaration of the Debug Drawer

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line line;
	Primitive point;
};