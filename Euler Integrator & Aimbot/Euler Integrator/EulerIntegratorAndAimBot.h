

class vec3d		//Class that will be used to declare 3D Vectors.
{
public:
	vec3d();
	vec3d(float x, float y, float z);

	float	x;
	float	y;
	float	z;
};

class Particle	//Class that will be used to declare objects / particles.
{
public:
	Particle();
	Particle(vec3d position, vec3d speed, vec3d acceleration, float mass, float surface, float dragCoefficient, float restitutionCoefficient);
	
	vec3d	position;								//Position of the particle in space.
	vec3d	speed;									//Speed of the particle.
	vec3d	acceleration;							//Acceleration of the particle.
	float	mass;									//Mass of the particle.
	float	surface;								//Front surface area of a particle.
	float	dragCoefficient;						//Drag coefficient of the particle.
	float	restitutionCoefficient;					//Restitution coefficient of the particle.
};

class World		//Class that will be used to define the simulation world's base properties.
{
public:
	World();
	World(float gravity, int worldWidth, int worldHeight, vec3d fluidVelocity, float fluidDensity, float dt);
	
	float	gravity;								//Gravity of the simulation world.
	int		worldWidth;								//Width of the simulation world.
	int		worldHeight;							//Height of the simulation world.
	vec3d	fluidVelocity;							//Fluid velocity in the simulation world. (Air, Water...)
	float	fluidDensity;							//Fluid density in the simulation world. (Air, Water...)
	float	dt;										//Timestep of the world (1/fps --> 1/60 = 0.016s)
};

// --- EULER INTEGRATOR
void EulerIntegrator(vec3d& iposition, vec3d& ivelocity, vec3d& acceleration, float dt);		//The value we want to "return" changed must be passed as reference so the variable passed as argument is changed.
void LoadIntegratorTestVariables();
void RunIntegratorTest();

Particle eulerProjectile;

// --- AIMBOT
void ParabolicEulerIntegrator(vec3d& iposition, vec3d& ivelocity, vec3d& acceleration, float angle, float dt);
void Monte_Carlo(int iterations, const Particle& target);
void MonteCarloTest();
void PropagateAll(vec3d& velocity, float angle);
bool CheckHit();

Particle	projectile;
Particle	target;
float		angle;
bool		targetWasHit;