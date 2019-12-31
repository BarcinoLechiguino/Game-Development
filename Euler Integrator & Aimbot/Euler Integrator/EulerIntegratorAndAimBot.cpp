#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "EulerIntegratorAndAimBot.h"

using namespace std;
#define ITERATIONS 1000
#define POS_MAX_RAND 10

int main()
{
	// --- Integrator Test
	//LoadIntegratorTestVariables();															//Time that will be applied to the object / objects.
	//RunIntegratorTest();

	// --- Monte-Carlo Test
	targetWasHit = false;
	target.position = { (float)(std::rand() % POS_MAX_RAND), (float)(std::rand() % POS_MAX_RAND), (float)(std::rand() % POS_MAX_RAND) };
	Monte_Carlo(ITERATIONS, target);

	system("pause");
	return 0;
}

// --- vec3d class constructor definitions
vec3d::vec3d(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3d::vec3d()
{

}

// --- EULER INTEGRATOR
void EulerIntegrator(vec3d& iposition, vec3d& ivelocity, vec3d& acceleration, /*float angle,*/ float dt) //The value we want to "return" changed must be passed as reference so the variable passed as argument is changed.
{
	//y = yo + vo * dt
	//v = vo + a * dt
	iposition.x = iposition.x + ivelocity.x * cos(45) * dt;					//Gets the object's final position in the X axis.
	iposition.y = iposition.y + ivelocity.y * cos(45) * dt;					//Gets the object's final position in the Y axis.
	iposition.z = iposition.z + ivelocity.z * cos(45) * dt;					//Gets the object's final position in the Z axis.

	ivelocity.x = ivelocity.x + acceleration.x * dt;				//Gets the object's final velocity in the X axis.
	ivelocity.y = ivelocity.y + acceleration.y * dt;				//Gets the object's final velocity in the Y axis.
	ivelocity.z = ivelocity.z + acceleration.z * dt;				//Gets the object's final velocity in the Z axis.
}

void LoadIntegratorTestVariables()
{
	eulerProjectile.position		= { 0.0f, 0.0f, 0.0f };
	eulerProjectile.speed			= { 5.0f, 0.0f, 0.0f };
	eulerProjectile.acceleration	= { 1.0f, 0.0f, 0.0f };

	float dt = 1.0f / 60.0f;										//60 FPS
}

void RunIntegratorTest()
{
	//cout << "Initial position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;
	cout << "Initial position: (" << eulerProjectile.position.x << " " << eulerProjectile.position.y << " " << eulerProjectile.position.z << ")" << endl;

	for (int i = 0; i < 4; i++)
	{
		EulerIntegrator(eulerProjectile.position, eulerProjectile.speed, eulerProjectile.acceleration, 1);		//EulerIntegrator() function declaration.

		cout << "fpos is: (" << eulerProjectile.position.x << " " << eulerProjectile.position.y << " " << eulerProjectile.position.z << ")";
		cout << "	fvel is: (" << eulerProjectile.speed.x << " " << eulerProjectile.speed.y << " " << eulerProjectile.speed.y << ")" << endl;
	}

	//cout << "Final position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;
	cout << "Final position: (" << eulerProjectile.position.x << " " << eulerProjectile.position.y << " " << eulerProjectile.position.z << ")" << endl;
}

// ---  AIMBOT / MONTE-CARLO
void Monte_Carlo(int iterations, const Particle& target)
{
	//target.position				= { 10, 10, 0 };

	for (int i = 0; i < iterations; i++)
	{
		cout << "Monte-Carlo " << i << endl;
		
		// --- Randomizing the initial Position, velocity and acceleration.
		projectile.position			= { 0, 0, 0 };
		//projectile.speed			= { (float)(std::rand() % 10), (float)(std::rand() % 10), (float)(std::rand() % 10) };
		//projectile.acceleration	= { (float)(std::rand() % 1), (float)(std::rand() % 1), (float)(std::rand() % 1) };
		projectile.speed			= { (float)(std::rand() % 100), (float)(std::rand() % 100), (float)(std::rand() % 100) };
		projectile.acceleration		= { 0.0f, -9.8f, 0.0f };
		angle						= std::rand() % 360;

		// --- Running the integrator to propagate the state of the projectile.
		MonteCarloTest();

		//PropagateAll(projectile.speed, angle);

		if (targetWasHit)
		{	
			cout << "Target at (" << target.position.x << " " << target.position.y << " " << target.position.z <<
				") was hit at iteration " << i << " of the Monte-Carlo method." << endl;

			cout << "Initial Speed: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
			cout << "Throwing Angle: " << angle << endl;

			break;
		}

		//cout << endl;
	}
}

void PropagateAll(vec3d& velocity, float angle)
{
	EulerIntegrator(projectile.position, velocity, projectile.acceleration, 1);
	
	if (CheckHit())
	{
		targetWasHit = true;
	}
}

bool CheckHit()
{
	if (projectile.position.x >= target.position.x && projectile.position.x <= target.position.x + 1
		&& projectile.position.y >= target.position.y && projectile.position.y <= target.position.y + 1)
	{
		return true;
	}

	return false;
}

void MonteCarloTest()
{
	cout << "Initial position: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")" << endl;
	cout << "Initial velocity: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
	cout << "Initial acceleration: (" << projectile.acceleration.x << " " << projectile.acceleration.y << " " << projectile.acceleration.z << ")" << endl;
	cout << "Initial angle: " << angle << endl;
	cout << "Target position: (" << target.position.x << " " << target.position.y << " " << target.position.z << ")" << endl;

	for (int j = 0; j < 5; j++)
	{
		EulerIntegrator(projectile.position, projectile.speed, projectile.acceleration, 1);

		cout << "fpos is: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")";
		cout << "	fvel is: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z<< ")" << endl;

		if (CheckHit())
		{
			targetWasHit = true;
		}
	}

	cout << "Final position: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")" << endl;

	cout << endl;
}
