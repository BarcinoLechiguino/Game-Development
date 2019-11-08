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

int dt = 1 / 30;

void EulerIntegrator(vec3d iposition, vec3d ivelocity, vec3d fposition, float acceleration, int dt)
{
	//y = yo + vo * dt
	//v = vo + a * dt
	fposition.x = iposition.x + ivelocity.x * dt;
	fposition.y = iposition.y + ivelocity.y * dt;
	fposition.z = iposition.z + ivelocity.z * dt;


	vec3d vel;
	/*vel.x = velocity.x + acceleration * dt;
	vel.y = velocity.y + acceleration * dt;
	vel.z = velocity.z + acceleration * dt;*/
}

int main()
{
	vec3d ipos;
	ipos.x = 0;
	ipos.y = 0;
	ipos.z = 0;

	vec3d ivel;
	ivel.x = 5; 
	ivel.y = 0;
	ivel.z = 0;

	vec3d fpos, fvel;

	EulerIntegrator(ipos, ivel, fpos, 0.0f, dt);

	cout << "Initial position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;



	system("pause");
	return 0;
}