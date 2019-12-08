#include "PhysBody3D.h"
#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ---------------------------------------------------------
PhysBody3D::PhysBody3D()
	: body(nullptr)
	, colShape(nullptr)
	, motionState(nullptr)
	, parentPrimitive(nullptr)
	, collision_listeners()
{
	
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	//TODO 2: And delete them!
	//Make sure there's actually something to delete, check before deleting
	if (body != nullptr)
	{
		delete body;
	}
	if (colShape != nullptr)
	{
		delete colShape;
	}
	if (motionState != nullptr)
	{
		delete motionState;
	}
}

void PhysBody3D::InitBody(Sphere* primitive, float mass)
{
	//TODO 1: Store all "new" created values
	assert(HasBody() == false);
	parentPrimitive = primitive;

	primitive->transform.translate(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);		//Spawn spheres at camera position.

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&primitive->transform);

	motionState = new btDefaultMotionState(startTransform);
	colShape = new btSphereShape(primitive->radius);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);

	body = new btRigidBody(rbInfo);

	//body->applyForce(btVector3(1000, 0, 0), btVector3(App->camera->X.x, App->camera->X.y, App->camera->X.z));

	body->setUserPointer(this);				//This PhysBody3D.
	App->physics->AddBodyToWorld(body);
}

bool PhysBody3D::HasBody() const
{
	return body != nullptr;
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if (HasBody() == false)
		return;
	//TODO 5: Set the Physical Body transform into "matrix"
	
	//getWorldTransform gets the transform of the body itself, which holds all variables and methods related with the transform of a PhysBody.
	body->getWorldTransform().getOpenGLMatrix(matrix);			//Gets the transform of the body.
	
	body->activate();											//Activates the body.
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if (HasBody() == false)
		return;

	//TODO 5: Set the Physical Body transform to equal "matrix"
	body->getWorldTransform().setFromOpenGLMatrix(matrix);		//Sets the transform of the body.

	body->activate();											//Activates the body.
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	if (HasBody() == false)
		return;

	//TODO 5: Set the Physical Body position to x, y, z. Make sure not to change the rotation!
	btVector3 origin(x, y, z);
	body->getWorldTransform().setOrigin(origin);				//Sets the origin position of the body.

	body->activate();											//Activates the body.
}
