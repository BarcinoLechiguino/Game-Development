

class vec3d		//Class that will be used to declare 3D Vectors.
{
public:
	vec3d();
	vec3d(float x, float y, float z);

	float x;
	float y;
	float z;
};

class Particle	//Class that will be used to declare objects / particles.
{
public:
	vec3d position;
	vec3d speed;
	vec3d acceleration;
};

//The value we want to "return" changed must be passed as reference so the variable passed as argument is changed.
void EulerIntegrator(vec3d& iposition, vec3d& ivelocity, vec3d& fposition, vec3d& fvelocity, vec3d& acceleration, float dt);
void PropagateAll(const vec3d& velocity, float angle);

//Particle projectile;
float angle;