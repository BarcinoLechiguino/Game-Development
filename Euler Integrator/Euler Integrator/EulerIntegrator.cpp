#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

class vec3d		//Class that will be used to declare 3D Vectors.
{
public:
	float x;
	float y;
	float z;
};

class particle	//Class that will be used to declare objects / particles.
{
public:
	vec3d position;
	vec3d speed;
	vec3d acceleration;
};

void EulerIntegrator(vec3d& iposition, vec3d& ivelocity, vec3d& fposition, vec3d& fvelocity, vec3d acceleration, float dt) //The value we want to "return" changed must be passed as reference so the variable passed as argument is changed.
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

	float dt = 1.0f / 30.0f;		//Time that will be applied to the object / objects.

	cout << "Initial position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;

	for (int i = 0; i < 4; i++)
	{
		EulerIntegrator(ipos, ivel, fpos, fvel, acceleration, 1);								//EulerIntegrator() function declaration.

		cout << "fpos is: (" << fpos.x << " " << fpos.y << " " << fpos.z << ")";
		cout << "	fvel is: (" << fvel.x << " " << fvel.y << " " << fvel.z << ")" << endl;
	}

	cout << "Final position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;

	system("pause");
	return 0;
}