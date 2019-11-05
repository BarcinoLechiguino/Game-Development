#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

int iterator = 0;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	iterator = 0;

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		//App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 50);
		//objectArray[iterator]->objectBody = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 50)->objectBody;
		
		//The Circle argument will be passed as a pointer.
		Object body = *App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25);
		bodyList.add(body);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		Object body = *App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 50, 25);
		bodyList.add(body);
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		b2Vec2 chainVectors[12];

		int chainPoints[24] =
		{
			-38, 80,
			-44, -54,
			-16, -60,
			-16, -17,
			19, -19,
			19, -79,
			61, -77,
			57, 73,
			17, 78,
			20, 16,
			-25, 13,
			-9, 72
		};

		Object* body = App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), chainVectors, 12, chainPoints, 24);
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		b2Vec2 Im_Rick[40];
		int rick_head[80] = {
			47, 132,
			35, 137,
			40, 126,
			23, 125,
			32, 115,
			11, 103,
			30, 91,
			0, 77,
			30, 63,
			14, 37,
			43, 40,
			41, 0,
			57, 13,
			69, 24,
			75, 33,
			83, 18,
			88, 4,
			93, 20,
			94, 41,
			112, 36,
			102, 58,
			105, 64,
			117, 67,
			109, 74,
			111, 82,
			110, 90,
			104, 94,
			111, 101,
			102, 106,
			100, 117,
			104, 120,
			106, 123,
			103, 126,
			98, 126,
			96, 136,
			92, 142,
			81, 149,
			68, 149,
			57, 145,
			50, 138
		};

		Object* body = App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), Im_Rick, 40, rick_head, 80);
		bodyList.add(*body);
	}

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		b2Vec2 Lliure[12];
		int Palestina[24] = {
			22, 247,
			58, 185,
			23, 121,
			95, 120,
			129, 58,
			165, 121,
			235, 119,
			200, 185,
			236, 244,
			164, 243,
			129, 306,
			96, 244,
			/*22, 247*/ //The chains cannot be closed, they will be completed automatically later
		};

		Object* body = App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), Lliure, 12, Palestina, 24);
	}

	// TODO 7: Draw all the circles using "circle" texture
	if (bodyList.getFirst() != NULL)
	{
		p2List_item<Object>* body_iterator = bodyList.getFirst();
		while (body_iterator != NULL)
		{
			if (body_iterator->data.objectBody->GetFixtureList()->GetShape()->GetType() == 0) //type 0 refers to circles.
			{
				App->renderer->Blit(circle, body_iterator->data.GetPosition()->x - 25, body_iterator->data.GetPosition()->y - 25, NULL, 0, body_iterator->data.GetRotation());	
			}
			else if (body_iterator->data.objectBody->GetFixtureList()->GetShape()->GetType() == 2) //type 2 type refers to poligons.
			{
				App->renderer->Blit(box, body_iterator->data.GetPosition()->x - 50, body_iterator->data.GetPosition()->y - 25, NULL, 0, body_iterator->data.GetRotation());
			}
			else if (body_iterator->data.objectBody->GetFixtureList()->GetShape()->GetType() == 3) //type 3 refers to chains.
			{
				App->renderer->Blit(rick, body_iterator->data.GetPosition()->x, body_iterator->data.GetPosition()->y, NULL, 0, body_iterator->data.GetRotation(), 0, 0);
			}
			body_iterator = body_iterator->next;
		}
	}
	return UPDATE_CONTINUE;
}
	