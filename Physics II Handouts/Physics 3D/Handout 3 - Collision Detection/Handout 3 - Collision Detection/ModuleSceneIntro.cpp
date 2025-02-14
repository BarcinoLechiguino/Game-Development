#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (int n = 0; n < primitives.Count(); n++)
	{
		delete primitives[n];
	}
	primitives.Clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	if (App->debug == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			//TODO 3: Create a "new" sphere, and add it to the "primitives" DynArray
			Primitive* sphere = new Sphere(1.0f, 1.0f);		//Creates a new sphere with the Primitive* pointer.
			
			primitives.PushBack(sphere);

			//TODO 9: Push ModuleSceneIntro to the sphere collision listeners
			sphere->physBody.collision_listeners.PushBack(this);
		}
	}


	//TODO 3: Nothing to do here. But it's good to know where all primitives are being updated
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Update();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	//TODO 3: Nothing to do here. But it's good to know where all primitives are being rendered
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}

//TODO 9: And change the color of the colliding bodies, so we can visualize it working!
void ModuleSceneIntro::OnCollision(PhysBody3D* PB1, PhysBody3D* PB2)
{
	/*Color color = { 255, 0, 0 };
	
	PB1->parentPrimitive->color = color;
	PB2->parentPrimitive->color = color;*/

	PB1->parentPrimitive->color.Set(255, 0, 0, 1.0f);
	PB2->parentPrimitive->color.Set(0, 0, 255, 1.0f);
}