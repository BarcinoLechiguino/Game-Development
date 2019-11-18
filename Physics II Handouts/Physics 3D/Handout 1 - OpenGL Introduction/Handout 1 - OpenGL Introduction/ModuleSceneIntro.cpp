#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	// TODO 2: Place the camera one unit up in Y and one unit to the right
	// experiment with different camera placements, then use LookAt()
	// to make it look at the center
	vec3 center = { 0, 0, 0 };				//Declares a vector (from glmath.h) that stores the position of the center of the world (rendering context).

	App->camera->Position.Set(2, 2, 2);		//Sets the starting position of the camera. x moves it along the X axis, y along the Y axis and z moves it along the Z axis. It moves along the grid like it's on rails. 
	App->camera->LookAt(center);			//Sets the point where the camera will be looking at/focused on. Needs to be given a 3D vector.

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 1: Create a Plane primitive. This uses the plane formula
	// so you have to express the normal of the plane to create 
	// a plane centered around 0,0. Make that it draw the axis for reference
	Plane plane(0, 0, 0, 1);				//Creates an infinite plane with the specified parameters (x, y, z and d). We can ignore d completely.
	plane.axis = true;						//Enables the axis so it shows in render time.
	plane.Render();							//Renders the plane in the rendering context.

	// TODO 6: Draw a sphere of 0.5f radius around the center
	// Draw somewhere else a cube and a cylinder in wireframe
	Sphere sphere(0.5f);					//Creates a sphere of 0.5 units of radius. As its position has not been defined it has been set to the origin's position (0, 0, 0).
	sphere.Render();						//Renders the sphere in the rendering context.

	//Now only the wireframe is shown.
	Sphere sphereWireframe(0.5f);			//Creates a sphere of 0.5 units of radius.
	sphere.SetPos(-1, 0, 1);				//Sets the spehere's position in the rendering context.
	sphere.wire = true;						//Enables the spere's wireframe so it can be seen in render time.
	sphere.Render();						//Renders the object in the rendering context.

	Cube cube(1, 1, 1);						//Creates a cube of 1 unit of width, 1 unit of height and 1 unit of depth.
	cube.SetPos(1, 0, -1);					//Sets the cube's position in the rendering context.
	cube.wire = true;						//Enables the cube's wireframe so it can be seen in render time.
	cube.Render();							//Rendes the cube in the rendering context.

	Cylinder cylinder(0.5f, 1.0f);			//Creates a cylinder of 0.5 units of radius and 1.0 units of height.
	cylinder.SetPos(-1, 0, -1);				//Sets the cylinder's position in the rendering context.
	cylinder.SetRotation(120, (0, 0, 1));	//Rotates the cylinder a determinate amount of degrees along a given axis. (Angle = 120º, Axis: z)
	cylinder.wire = true;					//Enables the cylinder's wireframe so it can be seen in render time.
	cylinder.Render();						//Renders the cylinder in the rendering context.


	return UPDATE_CONTINUE;
}

