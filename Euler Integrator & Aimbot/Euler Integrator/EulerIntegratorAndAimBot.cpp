#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "EulerIntegrator.h"
#include "AimBot.h"

using namespace std;

int main()
{
	vec3d ipos;
	ipos.x = 0.0f;
	ipos.y = 0.0f;
	ipos.z = 0.0f;

	vec3d ivel;
	ivel.x = 5.0f; 
	ivel.y = 0.0f;
	ivel.z = 0.0f;

	vec3d acceleration;
	acceleration.x = 1.0f;
	acceleration.y = 0.0f;
	acceleration.z = 0.0f;

	vec3d fpos, fvel;

	float dt = 1.0f / 30.0f;																	//Time that will be applied to the object / objects.

	cout << "Initial position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;

	for (int i = 0; i < 4; i++)
	{
		EulerIntegrator(ipos, ivel, fpos, fvel, acceleration, 1);								//EulerIntegrator() function declaration.

		cout << "fpos is: (" << fpos.x << " " << fpos.y << " " << fpos.z << ")";
		cout << "	fvel is: (" << fvel.x << " " << fvel.y << " " << fvel.z << ")" << endl;
	}

	cout << "Final position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;

	/*Monte_Carlo();*/

	angle = std::rand() % 365;
	cout << " " << angle;

	system("pause");
	return 0;
}

void EulerIntegrator(vec3d& iposition, vec3d& ivelocity, vec3d& fposition, vec3d& fvelocity, vec3d& acceleration, float dt) //The value we want to "return" changed must be passed as reference so the variable passed as argument is changed.
{
	//y = yo + vo * dt
	//v = vo + a * dt
	fposition.x = iposition.x + ivelocity.x * dt;		//Gets the object's final position in the X axis.
	fposition.y = iposition.y + ivelocity.y * dt;		//Gets the object's final position in the Y axis.
	fposition.z = iposition.z + ivelocity.z * dt;		//Gets the object's final position in the Z axis.

	iposition.x = fposition.x;							//Resets the object's initial position in the X axis to the new position.
	iposition.y = fposition.y;							//Resets the object's initial position in the Y axis to the new position.
	iposition.z = fposition.z;							//Resets the object's initial position in the Y axis to the new position.

	fvelocity.x = ivelocity.x + acceleration.x * dt;	//Gets the object's final velocity in the X axis.
	fvelocity.y = ivelocity.y + acceleration.y * dt;	//Gets the object's final velocity in the Y axis.
	fvelocity.z = ivelocity.z + acceleration.z * dt;	//Gets the object's final velocity in the Z axis.

	ivelocity.x = fvelocity.x;							//Resets the object's initial velocity in the X axis to the new velocity.
	ivelocity.y = fvelocity.y;							//Resets the object's initial velocity in the Y axis to the new velocity.
	ivelocity.z = fvelocity.z;							//Resets the object's initial velocity in the Z axis to the new velocity.
}

void PropagateAll(const vec3d& velocity, float angle)
{

}

void Monte_Carlo()
{
	for (int i = 0; i < 100; i++)
	{
		//projectile.position = vec3d(0.0f, 0.0f, 0.0f);
		
		//projectile.speed = vec3d(rand(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f)));
		angle = std::rand() % 365;
		cout << angle;
	}
}

vec3d::vec3d(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3d::vec3d()
{ }