#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "EulerIntegratorAndAimBot.h"

using namespace std;
#define PI 3.1416f
#define GRAVITY -9.8f
#define ORIGIN vec3d({0.0f, 0.0f, 0.0f})
#define ITERATIONS 1000
#define MAX_ANGLE 180.0f
#define MAX_WIND_VEL 10.0f
#define POS_MAX_RAND 10

int main()
{
	// --- Integrator Test
	//LoadIntegratorTestVariables();															//Time that will be applied to the object / objects.
	//RunIntegratorTest();

	// --- Monte-Carlo Test
	InitSimulation();
	Monte_Carlo(ITERATIONS, projectile, target);

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
		EulerIntegratorCore(eulerProjectile.position, eulerProjectile.speed, eulerProjectile.acceleration, 1.0f);		//EulerIntegrator() function declaration.

		cout << "fpos is: (" << eulerProjectile.position.x << " " << eulerProjectile.position.y << " " << eulerProjectile.position.z << ")";
		cout << "	fvel is: (" << eulerProjectile.speed.x << " " << eulerProjectile.speed.y << " " << eulerProjectile.speed.y << ")" << endl;
	}

	//cout << "Final position: (" << ipos.x << " " << ipos.y << " " << ipos.z << ")" << endl;
	cout << "Final position: (" << eulerProjectile.position.x << " " << eulerProjectile.position.y << " " << eulerProjectile.position.z << ")" << endl;
}
// ----------------------------------------------------------------------------------------------------------

// ------------------------------------------ AIMBOT / MONTE-CARLO ------------------------------------------
void AimBotEulerIntegrator(Particle& projectile, Particle& target)
{
	cout << "Fluid Velocity: (" << world.fluidVelocity.x << " " << world.fluidVelocity.y << " " << world.fluidVelocity.z << ")" << endl;

	for (int i = 0; i < world.total_time; i++)
	{
		world.fg = projectile.mass * world.gravity;													//Calculates the gravitational force applied to the projectile.

		if (projectile.speed.x > 100.0f)
		{
			projectile.speed.x = 100.0f;
		}

		world.totalVel = projectile.speed.x - world.fluidVelocity.x;								//Calculates the total velocity for the X Axis (Could be also applied to the other 2 axis).

		world.fd = 0.5f * world.fluidDensity * world.totalVel * world.totalVel * projectile.dragCoefficient * projectile.surface;	//Calculates the drag force applied to the projectile.

		projectile.acceleration.x = world.fd / projectile.mass;
		projectile.acceleration.y = world.fg / projectile.mass;
		projectile.acceleration.z = projectile.acceleration.z;

		//y = yo + vo * dt
		//v = vo + a * dt
		projectile.position.x = projectile.position.x + projectile.speed.x * world.dt;				//Gets the object's final position in the X axis.
		projectile.position.y = projectile.position.y + projectile.speed.y * world.dt;				//Gets the object's final position in the Y axis.
		projectile.position.z = projectile.position.z + projectile.speed.z * world.dt;				//Gets the object's final position in the Z axis.

		projectile.speed.x = projectile.speed.x + projectile.acceleration.x * world.dt;				//Gets the object's final velocity in the X axis.
		projectile.speed.y = projectile.speed.y + projectile.acceleration.y * world.dt;				//Gets the object's final velocity in the Y axis.
		projectile.speed.z = projectile.speed.z + projectile.acceleration.z * world.dt;				//Gets the object's final velocity in the Z axis.

		//cout << "(" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
		
		CheckRebound(projectile);																	//Checks whehter or not the projectile has collided against a wall. 
																									//If there has been a collision the velocity vector will be flipped/inverted.

		if (CheckHit(projectile, target))															//Checks whether or not the projectile has collided against the target.
		{
			aimbot.targetWasHit = true;																//If the target was hit, the targetWasHit flag will be set to true and the loop will be terminated.
			break;
		}

		//cout << "fpos is: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")";
		//cout << "	fvel is: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
	}
}

void Monte_Carlo(int iterations, Particle& projectile, Particle& target)
{
	for (int i = 0; i < iterations; i++)
	{
		cout << "Monte-Carlo " << i << endl;
		
		projectile.position			= ORIGIN;											//Resetting the projectile's position back to ORIGIN.
		//projectile.acceleration		= ORIGIN;											//Resetting the projectile's position back to ORIGIN.

		RandomizeVelocityAndAngle();

		//MonteCarloTest();															//Running the integrator to propagate the state of the projectile.

		PropagateAll(projectile, target, aimbot.velModule, aimbot.angle);			//Running the integrator to propagate the state of the projectile.

		if (aimbot.targetWasHit)
		{	
			cout << endl;

			cout << "Target at (" << target.position.x << " " << target.position.y << " " << target.position.z <<
				") was hit at iteration " << i << " of the Monte-Carlo method." << endl;

			cout << "Initial Speed: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
			cout << "Throwing Angle: " << aimbot.angle << endl;

			break;
		}
	}
}

void PropagateAll(Particle& projectile, Particle& target, float velModule, float angle)
{
	projectile.speed.x = velModule * cos(angle);
	projectile.speed.y = velModule * sin(angle);
	projectile.speed.z = (float)(std::rand() % 50);

	cout << "Initial position: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")" << endl;
	cout << "Initial velocity: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;
	cout << "Initial acceleration: (" << projectile.acceleration.x << " " << projectile.acceleration.y << " " << projectile.acceleration.z << ")" << endl;
	cout << "Initial angle: " << angle << endl;
	cout << "Target position: (" << target.position.x << " " << target.position.y << " " << target.position.z << ")" << endl;
	
	//AimBotEulerIntegrator(projectile.position, projectile.speed, projectile.acceleration, world.dt);
	AimBotEulerIntegrator(projectile, target);

	cout << "fpos is: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")";
	cout << "	fvel is: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z << ")" << endl;

	if (CheckHit(projectile, target))									// This check here makes the projectile be like a grenade, as the check happens after the projectile has stopped being propagated.
	{
		aimbot.targetWasHit = true;
	}

	cout << "Final position: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")" << endl;

	cout << endl;
}

// ------------------------------------- SIMULATION CHECKS --------------------------------------
bool CheckHit(const Particle& projectile, const Particle& target)
{
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

void CheckRebound(Particle& projectile)
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
	cout << "Initial angle: " << aimbot.angle << endl;
	cout << "Target position: (" << target.position.x << " " << target.position.y << " " << target.position.z << ")" << endl;

	for (int j = 0; j < 5; j++)
	{
		//AimBotEulerIntegrator(projectile.position, projectile.speed, projectile.acceleration, 1.0f);

		cout << "fpos is: (" << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << ")";
		cout << "	fvel is: (" << projectile.speed.x << " " << projectile.speed.y << " " << projectile.speed.z<< ")" << endl;

		if (CheckHit(projectile, target))
		{
			aimbot.targetWasHit = true;
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

	RandomizeWindVelocity();					//Revise this, maybe set wind speed somewhere else?
}

void InitSimulationWorld()
{
	world.gravity			= GRAVITY;													// Gravity will be -9.8f.
	world.worldWidth		= 20;														// World Width will be 20m.
	world.worldHeight		= 10;														// World Height will be 10m.
	world.fluidVelocity		= ORIGIN;													// Fluid velocity will be a random value (-10.0f ~ 10.0f)
	world.fluidDensity		= 1.2f;														// Fluid Density will be 1.2f (Air, 20º at 1 atm).
	world.simulation_fps	= 60;														// The simulation will run at 60 FPS. Change later to match real time (syncronized with the game).	
	world.dt				= 1 / world.simulation_fps;									// Dt will be 0.016f (1 / 60).	
	world.simulation_time	= 5;														// For each simulation case, the simulation will run for 5 seconds. (5s of propagation).
	world.total_time		= world.simulation_fps * world.simulation_time;				// Total amount of frames that the AimBot will propagate the state of the projectile for. 
	
	//world = World(GRAVITY, 20, 10, fluidSpeed, 1.2f, 60, 5);
}

void InitSimulationElements()
{
	// --- INITIALIZING THE PROJECTILE

	projectile.position					= ORIGIN;										// Initial Position will be set at ORIGIN ({0.0f, 0.0f, 0.0f})
	projectile.speed					= ORIGIN;										// Velocity will be randomized (0.0f ~ 50.0f) at the MonteCarlo() method.
	projectile.acceleration				= ORIGIN;										// The Acceleration applied to the projectile will be the World's gravity.
	projectile.mass						= 1.0f;											// Mass will be set to 1 Kg.
	projectile.radius					= 0.5f;											// Radius will be set to 0.5m.
	//projectile.surface				= 0.78f;										// As the projectile will have a radius of 0.5m, the Surface Area will be 0.78 m^2.
	projectile.surface					= PI * projectile.radius * projectile.radius;	// As the projectile will have a radius of 0.5m, the Surface Area will be 0.78 m^2.
	projectile.dragCoefficient			= 0.47f;										// The Drag Coefficient (Cd) of a sphere is 0.47.
	projectile.restitutionCoefficient	= 0.95f;										// The Restitution Coefficient of the projectile will be set to 0.95 (0 ~ 1)
	
	/*vec3d originPos = ORIGIN;
	vec3d originSpeed = ORIGIN;
	vec3d originAcceleration = { 0.0f, world.gravity, 0.0f };

	projectile = Particle(originPos, originSpeed, originAcceleration, 1.0f, 0.5f, 0.78f, 0.47f, 0.95f);*/

	cout << projectile.position.x << endl;

	
	// --- INITIALIZING THE TARGET
	vec3d targetPos				= { (float)(std::rand() % POS_MAX_RAND), (float)(std::rand() % POS_MAX_RAND), (float)(std::rand() % POS_MAX_RAND) };
	vec3d targetSpeed			= ORIGIN;
	vec3d targetAcceleration	= ORIGIN;

	// Initial Target Position will be randomized (for now).
	// Target Radius will be set to 1.0m.
	// As the target will be static:
	//	- Speed will be NIL.
	//	- Acceration will be NIL.
	//	- All other variables will be set to 0.0f (default).
	
	target = Particle(targetPos, targetSpeed, targetAcceleration);

	aimbot.targetWasHit = false;												//If this bool will be set to true when the projectile hits the target. Stops the Monte-Carlo.
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
	aimbot.velModule	= (float)(std::rand() % 50);
	aimbot.angle		= -MAX_ANGLE + (float)(std::rand() % 360);
}

void RandomizeWindVelocity()
{
	// --- Randomizing the wind's velocity.
	world.fluidVelocity = { -MAX_WIND_VEL + (float)(std::rand() % 20), 0.0f, 0.0f }; //-MAX_WIND_VEL allows to have negative random numbers (-10.0f + rand % 20 = -10.0f + 5.0f = -5.0f)
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

AimBotVariables::AimBotVariables()
{

}

AimBotVariables::AimBotVariables(float velModule, float angle, bool targetWasHit)
{
	this->velModule					= velModule;
	this->angle						= angle;
	this->targetWasHit				= targetWasHit;
}

World::World()
{

}

World::World(float gravity, int worldWidth, int worldHeight, vec3d fluidVelocity, float fluidDensity, float simulation_fps, float simulation_time)
{
	this->gravity					= gravity;
	this->worldWidth				= worldWidth;
	this->worldHeight				= worldHeight;
	this->fluidVelocity				= fluidVelocity;
	this->fluidDensity				= fluidDensity;

	this->simulation_fps			= simulation_fps;
	this->dt						= 1 / simulation_fps;
	this->simulation_time			= simulation_time;
	this->total_time				= simulation_fps * simulation_time;
}
// ----------------------------------------------------------------------------------------------