#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

class vec3d
{
public:
	float x;
	float y;
	float z;
};

class particle
{
public:
	vec3d position;
	vec3d speed;
	vec3d acceleration;
};

void EulerIntegrator(vec3d& iposition, vec3d& ivelocity, vec3d& fposition, vec3d& fvelocity, float acceleration, float dt) //The value we want to "return" changed must be passed as reference so the variable passed as argument is changed.
{
	//y = yo + vo * dt
	//v = vo + a * dt
	fposition.x = iposition.x + ivelocity.x * dt;
	fposition.y = iposition.y + ivelocity.y * dt;
	fposition.z = iposition.z + ivelocity.z * dt;

	iposition.x = fposition.x;
	iposition.y = fposition.y;
	iposition.z = fposition.z;

	fvelocity.x = ivelocity.x + acceleration * dt;
	fvelocity.y = ivelocity.y + acceleration * dt;
	fvelocity.z = ivelocity.z + acceleration * dt;

	ivelocity.x = fvelocity.x;
	ivelocity.x = fvelocity.x;
	ivelocity.x = fvelocity.x;
}

int main()
{
	vec3d ipos;
	ipos.x = 0.0f;
	ipos.y = 0.0f;
	ipos.z = 0.0f;

	vec3d ivel;
	ivel.x = 5; 
	ivel.y = 0;
	ivel.z = 0;

	vec3d fpos, fvel;

	float dt = 1.0f / 30.0f;


	for (int i = 0; i < 4; i++)
	{
		EulerIntegrator(ipos, ivel, fpos, fvel, 1.0f, dt);

		cout << "fpos is: (" << fpos.x << " " << fpos.y << " " << fpos.z << ")" << endl;
		cout << "fvel is: (" << fvel.x << " " << fvel.y << " " << fvel.z << ")" << endl;
	}

	cout << "Initial position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;

	/*while (1)
	{
		EulerIntegrator(ipos, ivel, fpos, fvel, 1.0f, dt);

		cout << "fpos is: (" << fpos.x << " " << fpos.y << " " << fpos.z << ")" << endl;
		cout << "fvel is: (" << fvel.x << " " << fvel.y << " " << fvel.z << ")" << endl;
	}*/

	system("pause");
	return 0;
}