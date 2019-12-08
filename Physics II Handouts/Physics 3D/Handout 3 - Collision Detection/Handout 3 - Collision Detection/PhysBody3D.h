#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2DynArray.h"

class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;
class Module;

class Sphere;
class Primitive;

class PhysBody3D
{
public:
	PhysBody3D();
	~PhysBody3D();

	void InitBody(Sphere* primitive, float mass);
	bool HasBody() const;

	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);

private:
	//TODO 1: Store all "new" created values
	btRigidBody*			body;
	btCollisionShape*		colShape;			//Pointer of a PhysBody's collision shape.
	btDefaultMotionState*	motionState;		//Pointer of a PhysBody's motion state.

public:
	Primitive* parentPrimitive;
	p2DynArray<Module*> collision_listeners;	//Any module can register as a listener.
};

#endif // __PhysBody3D_H__