#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "EulerIntegratorAndAimBot.h"

using namespace std;
#define ITERATIONS 1000
#define POS_MAX_RAND 10
#define GRAVITY -9.8f
#define ORIGIN vec3d({0.0f, 0.0f, 0.0f})
#define MAX_WIND_VEL 10

int main()
{
	// --- Integrator Test
	//LoadIntegratorTestVariables();															//Time that will be applied to the object / objects.
	//RunIntegratorTest();

	// --- Monte-Carlo Test
	InitSimulation();
	Monte_Carlo(ITERATIONS, target);

	system("pause");
	return 0;
}

// -------------------------------------------- EULER INTEGRATOR --------------------------------------------
void EulerIntegratorCore(vec3d& iposition, vec3d& ivelocity, vec3d& acceleration, float dt) //The value we want to "return" changed must be passed as reference so the variable passed as argument is changed.
{
	//y = yo + vo * dt
	//v = vo + a * dt
	iposition.x = iposition.x + ivelocity.x * dt;					//Gets the object's final position in the X axis.
	iposition.y = iposition.y + ivelocity.y * dt;					//Gets the object's final position in the Y axis.
	iposition.z = iposition.z + ivelocity.z * dt;					//Gets the object's final position in the Z axis.

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
		EulerIntegratorCore(eulerProjectile.position, eulerProjectile.speed, eulerProjectile.acceleration, 1);		//EulerIntegrator() function declaration.

		cout << "fpos is: (" << eulerProjectile.position.x << " " << eulerProjectile.position.y << " " << eulerProjectile.position.z << ")";
		cout << "	fvel is: (" << eulerProjectile.speed.x << " " << eulerProjectile.speed.y << " " << eulerProjectile.speed.y << ")" << endl;
	}

	//cout << "Final position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;
	cout << "Final position: (" << eulerProjectile.position.x << " " << eulerProjectile.position.y << " " << eulerProjectile.position.z << ")" << endl;
}
// ----------------------------------------------------------------------------------------------------------

// ------------------------------------------ AIMBOT / MONTE-CARLO ------------------------------------------
void AimBotEulerIntegrator(vec3d& iposition, vec3d& ivelocity, vec3d& acceleration, float dt)
{	
	
	
	for (int i = 0; i < total_time; i++)
	{
		//y = yo + vo * dt
		//v = vo + a * dt
		iposition.x = iposition.x + ivelocity.x * dt;					//Gets the object's final position in the X axis.
		iposition.y = iposition.y + ivelocity.y * dt;					//Gets the object's final position in the Y axis.
		iposition.z = iposition.z + ivelocity.z * dt;					//Gets the object's final position in the Z axis.

		ivelocity.x = ivelocity.x + acceleration.x * dt;				//Gets the object's final velocity in the X axis.
		ivelocity.y = ivelocity.y + acceleration.y * dt;				//Gets the object's final velocity in the Y axis.
		ivelocity.z = ivelocity.z + acceleration.z * dt;				//Gets the object's final velocity in the Z axis.

		CheckRebound();													//Checks whehter or not the projectile has collided against a wall. 
																		//If there has been a collision the velocity vector will be flipped/inverted.


		if (CheckHit())													//Checks whether or not the projectile has collided against the target.
		{
			targetWasHit = true;										//If the target was hit, the targetWasHit flag will be set to true and the loop will be terminated.
			break;
		}
	}
}

void Monte_Carlo(int iterations, const Particle& target)
{
	for (int i = 0; i < iterations; i++)
	{
		cout << "Monte-Carlo " << i << endl;
		
		projectile.position = ORIGIN;												//Resetting the projectile's position back to ORIGIN.

		RandomizeVariables();														//Randomizing all relevant variables.

		//MonteCarloTest();															//Running the integrator to propagate the state of the projectile.

		PropagateAll(projectile.speed, angle);										//Running the integrator to propagate the state of the projectile.

		if (targetWasHit)
		{	
			cout << endl;

			cout << "Target at (" << target.position.x << " " << target.position.y << " " << target.position.z <<
				") was hit at iteration " << i << " of the Monte-Carlo method." << endl;

			cout << "Initial Speed: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
			cout << "Throwing Angle: " << angle << endl;

			break;
		}
	}
}

void PropagateAll(vec3d& velocity, float angle)
{
	velocity.x = velocity.x * cos(angle);
	velocity.y = velocity.y * sin(angle);

	cout << "Initial position: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")" << endl;
	cout << "Initial velocity: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
	cout << "Initial acceleration: (" << projectile.acceleration.x << " " << projectile.acceleration.y << " " << projectile.acceleration.z << ")" << endl;
	cout << "Initial angle: " << angle << endl;
	cout << "Target position: (" << target.position.x << " " << target.position.y << " " << target.position.z << ")" << endl;
	
	for (int j = 0; j < 5; j++)
	{
		AimBotEulerIntegrator(projectile.position, velocity, projectile.acceleration, world.dt);

		cout << "fpos is: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")";
		cout << "	fvel is: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
		
		if (CheckHit())
		{
			targetWasHit = true;
			break;
		}
	}

	cout << "Final position: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")" << endl;

	cout << endl;
}

// ------------------------------------- SIMULATION CHECKS --------------------------------------
bool CheckHit()
{
	/*if (projectile.position.x >= target.position.x && projectile.position.x <= target.position.x + 1
		&& projectile.position.y >= target.position.y && projectile.position.y <= target.position.y + 1)
	{
		return true;
	}*/

	if (DistBetweenElements(projectile.position, target.position) <= target.radius)
	{
		return true;
	}

	return false;
}

float DistBetweenElements(vec3d projectilePos, vec3d targetPos)
{
	vec3d distBuffer = { projectilePos.x - targetPos.x,
						projectilePos.y - targetPos.y,
						projectilePos.z - targetPos.z};

	float distNoSqrt = distBuffer.x * distBuffer.x + distBuffer.y * distBuffer.y + distBuffer.z * distBuffer.z;

	float dist = sqrt(distNoSqrt);

	return dist;
	//return distNoSqrt;
}

void CheckRebound()
{
	// --- Projectile hits the walls on the X Axis.
	if (projectile.position.x <= 0.0f || projectile.position.x >= world.worldWidth)
	{
		projectile.speed.x = -(projectile.restitutionCoefficient * projectile.speed.x);		//The component x of the vector speed will be flipped/inverted.
	}

	// --- Projectile hits the walls on the Y Axis.
	if (projectile.position.y <= 0.0f || projectile.position.y >= world.worldHeight)
	{
		projectile.speed.y = -(projectile.restitutionCoefficient * projectile.speed.y);		//The component y of the vector speed will be flipped/inverted.
	}
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
		AimBotEulerIntegrator(projectile.position, projectile.speed, projectile.acceleration, 1);

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

// ---------------------------- INITIALIZING ALL SIMULATION ELEMENTS ----------------------------
void InitSimulation()
{
	InitSimulationWorld();
	InitSimulationElements();

	targetWasHit		= false;
	simulation_fps		= 60;
	simulation_time		= 5;
	total_time			= simulation_fps * simulation_time;
}

void InitSimulationWorld()
{
	vec3d fluidSpeed	= ORIGIN;

	// Gravity will be -9.8f.
	// World Width will be 20m.
	// World Height will be 10m.
	// Fluid velocity will be randomized (-10.0f ~ 10.0f) at the MonteCarlo() method.
	// Fluid Density will be 1.2f (Air, 20º at 1 atm).
	// Dt will be 0.016f (1 / 60).
	
	world = World(GRAVITY, 20, 10, fluidSpeed, 1.2f, 0.016f);
}

void InitSimulationElements()
{
	// --- INITIALIZING THE PROJECTILE
	vec3d originPos				= ORIGIN;
	vec3d originSpeed			= ORIGIN;
	vec3d originAcceleration	= { 0.0f, world.gravity, 0.0f };

	// Initial Position will be set at ORIGIN ({0.0f, 0.0f, 0.0f})
	// Velocity will be randomized (0.0f ~ 50.0f) at the MonteCarlo() method.
	// The Acceleration applied to the projectile will be the World's gravity.
	// Mass will be set to 1 Kg.
	// Radius will be set to 0.5m.
	// As the projectile will have a radius of 0.5m, the Surface Area will be 0.78 m^2.
	// The Drag Coefficient (Cd) of a sphere is 0.47.
	// The Restitution Coefficient of the projectile will be set to 0.95 (0 ~ 1)
	
	projectile = Particle(originPos, originSpeed, originAcceleration, 1.0f, 0.5f, 0.78f, 0.47f, 0.95f);

	cout << projectile.position.x << endl;

	
	// --- INITIALIZING THE TARGET
	vec3d targetPos				= { (float)(std::rand() % POS_MAX_RAND), (float)(std::rand() % POS_MAX_RAND), (float)(std::rand() % POS_MAX_RAND) };
	vec3d targetSpeed			= ORIGIN;
	vec3d targetAcceleration	= ORIGIN;

	// Initial Target Position will be randomized (for now).
	// Target Radius will be set to 0.5m.
	// As the target will be static:
	//	- Speed will be NIL.
	//	- Acceration will be NIL.
	//	- All other variables will be set to 0.0f (default).
	
	target = Particle(targetPos, targetSpeed, targetAcceleration, 0.0f, 0.5f);

	targetWasHit = false;												//If this bool will be set to true when the projectile hits the target. Stops the Monte-Carlo.
}
// ----------------------------------------------------------------------------------------------

// ---------------------------- RANDOMIZING MONTE-CARLO'S VARIABLES -----------------------------
void RandomizeVariables()
{
	RandomizeVelocityAndAngle();
	RandomizeWindVelocity();
}

void RandomizeVelocityAndAngle()
{
	// --- Randomizing the velocity & throwing angle of the projectile.
	projectile.speed	= { (float)(std::rand() % 50), (float)(std::rand() % 50), (float)(std::rand() % 50) };
	angle				= std::rand() % 90;
}

void RandomizeWindVelocity()
{
	// --- Randomizing the wind's velocity.
	world.fluidVelocity = { (float)(std::rand() % 20), 0.0f, 0.0f };

	// --- To be able to have random negative numbers:
	if (world.fluidVelocity.x <= MAX_WIND_VEL)
	{
		world.fluidVelocity.x = -world.fluidVelocity.x;					// If rand is lower than the maximum wind velocity (10 m/s) then velocity will be negative. 
	}
	else
	{
		world.fluidVelocity.x = world.fluidVelocity.x - MAX_WIND_VEL; 	// If rand is higher than MAX, then it will be positive. However it will be subtracted MAX_WIND_VEL so as to keep it within the limits.
	}
}
// ----------------------------------------------------------------------------------------------

// ------------------------------------- CLASS CONSTRUCTORS -------------------------------------
vec3d::vec3d()
{

}

vec3d::vec3d(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Particle::Particle()
{

}

Particle::Particle(vec3d position, vec3d speed, vec3d acceleration, float mass, float radius, float surface, float dragCoefficient, float restitutionCoefficient)
{
	this->position					= position;
	this->speed						= speed;
	this->acceleration				= acceleration;
	this->mass						= mass;
	this->radius					= radius;
	this->surface					= surface;
	this->dragCoefficient			= dragCoefficient;
	this->restitutionCoefficient	= restitutionCoefficient;
}

World::World()
{

}

World::World(float gravity, int worldWidth, int worldHeight, vec3d fluidVelocity, float fluidDensity, float dt)
{
	this->gravity					= gravity;
	this->worldWidth				= worldWidth;
	this->worldHeight				= worldHeight;
	this->fluidVelocity				= fluidVelocity;
	this->fluidDensity				= fluidDensity;
	this->dt						= dt;
}
// ----------------------------------------------------------------------------------------------