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

	const int SnakeLength = 7;
	const float StartingSize = 0.5f;
	const float SizeIncrement = 0.2f;
	const float BallDistance = 0.3f;

	float XPos = 0.f;
	float Size = StartingSize;

	Sphere* previousSphere = nullptr;		//Buffer where the previously iterated sphere will be kept.
	float prevSize = StartingSize;			//Stores the previous sphere's size.

	for (int n = 0; n < SnakeLength; n++)
	{
		Sphere* s = new Sphere(Size);
		primitives.PushBack(s);
		s->SetPos(XPos, 10.f, 2.5f);

		//int sphereMidPosX = Size + (BallDistance / 2);			//Position in X of the center between 2 spheres in this setup.
		btVector3 center = { Size + (BallDistance / 2), 0, 0 };		//Center vector between 2 spheres.

		btScalar prevCenterDist = (prevSize + (BallDistance / 2));
		btScalar centerDist = (Size + (BallDistance / 2));

		//TODO 2: Link all the spheres with your P2P constraints
		if (primitives.Count() > 1 && previousSphere != nullptr)		//If there are more than 2 primitives in the primitives list and previousSphere is not NULL.
		{
			//App->physics->AddConstraintP2P(*s, *previousSphere, center, -center);
			App->physics->AddConstraintP2P(*s, *previousSphere, 
				btVector3(centerDist, 0, 0), btVector3(-prevCenterDist, 0, 0));
		}

		previousSphere = s;					//The new sphere is set in the buffer.
		prevSize = Size;					//Previous size of the 

		XPos += Size + Size + SizeIncrement + BallDistance;
		Size += SizeIncrement;
	}

	//TODO 4: Link some other spheres with your Hinge constraint
	//Variables for the hinge constraint spheres. The first letter of each variable's name is in lower case.
	const float startingSize = 0.5f;
	const float sizeIncrement = 0.2f;
	const float ballDistance = 0.3f;

	float xPos = 0.f;
	float size = startingSize;

	Sphere* previousHinge = nullptr;
	float previousSize = startingSize;

	for (int n = 0; n < SnakeLength; n++)
	{
		Sphere* hinge_S = new Sphere(size);
		primitives.PushBack(hinge_S);
		hinge_S->SetPos(xPos, 10.f, 10.0f);

		//btScalar centerDist = (previousSize + ballDistance + size) / 2;
		btScalar prevCenterDist = (previousSize + (ballDistance / 2));
		btScalar centerDist = (size + (ballDistance / 2));

		//TODO 2: Link all the spheres with your P2P constraints
		if (previousHinge != nullptr)			//If there are more than 2 primitives in the primitives list and previousSphere is not NULL.
		{
			App->physics->AddConstraintHinge(*hinge_S, *previousHinge, btVector3(centerDist, 0, 0), 
				btVector3(-prevCenterDist, 0, 0), btVector3(0, 1, 0), btVector3(0, 1, 0));
		}

		previousHinge = hinge_S;					//The new sphere is set in the buffer.
		previousSize = size;						//The previousSize is set with the size of the current sphere.

		xPos += size + size + sizeIncrement + ballDistance;
		size += sizeIncrement;
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::HandleDebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		DebugSpawnPrimitive(new Sphere());
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		DebugSpawnPrimitive(new Cube());
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		DebugSpawnPrimitive(new Cylinder());
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->body.Push(vec3((float)(std::rand() % 500) - 250, 500, (float)(std::rand() % 500) - 250));

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//TODO: NEW CODE
		//A snippet of new code that may be useful for you. Nothing to do here really

		//Get a vector indicating the direction from the camera viewpoint to the "mouse"
		const vec2 mousePos(((float)App->input->GetMouseX() / (float)App->window->Width()) * 2.f - 1.f,
			-((float)App->input->GetMouseY() / (float)App->window->Height()) * 2.f + 1.f);
		const vec4 rayEye = inverse(App->renderer3D->ProjectionMatrix) * vec4(mousePos.x, mousePos.y, -1.f, 1.f);
		const vec4 rayWorld(inverse(App->camera->GetViewMatrix()) * vec4(rayEye.x, rayEye.y, -1.f, 0.f));

		vec3 Dir(rayWorld.x, rayWorld.y, rayWorld.z);
		//Cast a ray from the camera, in the "mouse" direction
		PhysBody3D* body = App->physics->RayCast(App->camera->Position, Dir);
		if (body)
		{
			//Change the color of the clicked primitive
			body->parentPrimitive->color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);
		}
	}
}

void ModuleSceneIntro::DebugSpawnPrimitive(Primitive * p)
{
	primitives.PushBack(p);
	p->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	p->body.collision_listeners.PushBack(this);
	p->body.Push(-App->camera->Z * 1000.f);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	if (App->debug == true)
		HandleDebugInput();

	for (uint n = 0; n < primitives.Count(); n++)
		primitives[n]->Update();

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);

	body1->parentPrimitive->color = color;
	body2->parentPrimitive->color = color;

}