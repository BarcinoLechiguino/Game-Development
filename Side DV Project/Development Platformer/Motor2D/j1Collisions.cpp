#include "p2Defs.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1Render.h"

//With the constructor call collider_debug (draw colliders on screen) is set to false.
j1Collisions::j1Collisions() : j1Module()
{
	name.create("collisions");
	collider_debug = false;		//F9 key sets it to true or false.
}

bool j1Collisions::Init()
{

	return true;
};

bool j1Collisions::Awake(pugi::xml_node& config)
{
	return true;
};

//Loads all objects from the tmx file, adds a collider to them and assigns them a type
bool j1Collisions::Start() 
{
	LoadColliderFromMap();

	return true;
};

//Calculates the colliders's collisions and deletes from memory any collider that has been set to be deleted.
bool j1Collisions::PreUpdate() 
{
	p2List_item<Collider*>* collider_iterator = collider_list.start;
	
	//This loop deletes from memory any collider that has been set to be deleted before calculating any new collisions.
	while (collider_iterator != NULL)
	{
		if (collider_iterator->data->delete_collider == true)	//If the delete_collider is set to true this will be run.
		{
			collider_list.del(collider_iterator);				//Using the list's properties all colliders set to delete will be deleted from memory.
		}
		collider_iterator = collider_iterator->next;			//Gets the next collider that will be deleted.
	}

	//------------------------------------------ From this point onwards collider collisions will be calculated ------------------------------------------
	Collider* C1;		//Declares the first collider's pointer.
	Collider* C2;		//Declares the second collider's pointer.
	
	// Declares a new list item pointer that will be associated with the first collider and sets it to the start of the list in case any colliders have been deleted.
	p2List_item<Collider*>* C1_iterator = collider_list.start;
	while (C1_iterator != NULL)
	{
		C1 = C1_iterator->data;	//Sets the data members of the first collider to the data members of the collider being iterated.

		p2List_item<Collider*>* C2_iterator = collider_list.start;	//Declares a new list item pointer that will be associated with the second collider for the same purposes as C1_iterator.
		while (C2_iterator != NULL)
		{
			C2 = C2_iterator->data;	//Sets the data members of the second collider to the data members of the collider being iterated.

			if (C1 != C2)	//If data members of C1 are different from the data members of C2, then this will be run.
			{
				if (C1->Check_Collision(C2->collider) == true && (C1->type == PLAYER || C2->type == PLAYER)) //Will be run if there is a collision and any of the colliders are of the type PLAYER.
				{
					if (C1->callback)
					{
						C1->callback->OnCollision(C1, C2);
					}
					else if (C1->callback)
					{
						C2->callback->OnCollision(C2, C1);
					}
				}
			}
			C2_iterator = C2_iterator->next; //Gets the next collider 2 that will have its collisions calculated.
		}
		C1_iterator = C1_iterator->next; //Gets the next collider 1 that will have its collisions calculated.
	}

	return true;
};

bool j1Collisions::Update(float dt) 
{
	Collider_Debug();
	return true;
};

//Iterates all the colliders and assigns each type a Draw_Quad() with an identifying colour.
void j1Collisions::Collider_Debug()
{
	if (collider_debug != true)
	{
		return;
	}

	else
	{
		p2List_item<Collider*>* collider_iterator = collider_list.start;	//Declares a list item pointer that iterates a given list, in this case the collider list.
		while (collider_iterator != NULL)
		{	
			switch (collider_iterator->data->type)	//We declare a switch that will consider collider types as possible cases.
			{
			//Declaring a DrawQuad() with a set colour depending of the type of the object/collider that is being iterated at that moment. ALPHA is the transparency value.

			case PLAYER:	//PLAYER collider will be GREEN.
				App->render->DrawQuad(collider_iterator->data->collider, 0, 255, 0, ALPHA);
				break;

			case SOLID:	//SOLID collider will be BLUE.
				App->render->DrawQuad(collider_iterator->data->collider, 0, 0, 255, ALPHA);
				break;

			case PLATFORM: //PLATFORM collider will be WHITE.
				App->render->DrawQuad(collider_iterator->data->collider, 255, 255, 255, ALPHA);

			case HAZARD: //HAZARD collider will be RED
				App->render->DrawQuad(collider_iterator->data->collider, 255, 0, 0, ALPHA);
				break;

			case ITEM: //ITEM collider will be ORANGE
				App->render->DrawQuad(collider_iterator->data->collider, 255, 150, 0, ALPHA);
				break;

			case DESACTIVABLE: //DEACTIVABLE collider will be YELLOW
				App->render->DrawQuad(collider_iterator->data->collider, 255, 255, 0, ALPHA);
				break;

			case RESPAWN: //RESPAWN collider will be PURPLE
				App->render->DrawQuad(collider_iterator->data->collider, 255, 0, 255, ALPHA);
				break;

			case GOAL: //GOAL collider will be PINK
				App->render->DrawQuad(collider_iterator->data->collider, 255, 0, 150, ALPHA);
				break;
			}
			collider_iterator = collider_iterator->next; //Gets the next collider that will be assigned a type.
		}
	}
}

bool j1Collisions::PostUpdate() 
{
	return true;
};

bool j1Collisions::CleanUp() 
{
	return true;
};

//Generates a new collider with the given arguments and allocates it in memory.
Collider* j1Collisions::AddCollider(SDL_Rect collider, Object_Type type, j1Module* callback)
{
	Collider* hitbox = new Collider;	//Allocates memory for the new collider.

	hitbox->collider = collider;		//Sets the new collider's rect to the given collider's rect.
	hitbox->type = type;				//Sets the new collider's type to the given collider's type.
	hitbox->callback = callback;		//Sets the new collider's callback to the given collider's callback.

	collider_list.add(hitbox);			//Adds the new collider to collider_list.

	return hitbox;
}

//Loads all the objects that are in the tmx map file and iterates through them generating a new collider for each one of them.
void j1Collisions::LoadColliderFromMap() // Remember to put in fade to black.
{
	p2List_item<ObjectGroup*>* object_iterator = App->map->data.objectGroups.start; //Declares a list item pointer that iterates through the ObjectGroup list and sets it starting position to the first objectgroup in the list.  
	while (object_iterator != NULL)
	{
		for (int i = 0; i < object_iterator->data->num_objects; i++)	//This loop will iterate as many times as objects the objectgroup being iterated has. Done like this to avoid wasting memory.
		{
			AddCollider(*object_iterator->data->object[i].collider, object_iterator->data->object[i].type, NULL); //Adds a new collider for each object that is iterated.
		}
		object_iterator = object_iterator->next;	//Gets the next objectGroup that will be iterated through to load all its objects.
	}
}

//Checks all possible collider collisions
bool Collider::Check_Collision(const SDL_Rect& r) const //Main collider calls the method and given collider is passed as an argument.
{
	return ((r.x + r.w > collider.x) &&				// Collision going from right to left.
			(r.x < collider.x + collider.w) &&		// Collison going from left to right.
			(r.y + r.h > collider.y) &&				// Collision going down up. Jumping collision (With player, ground, platform...).
			(r.y < collider.y + collider.h));		// Collision going up down. Falling collision (With player, ground, platform...).
}

//Method that sets a collider's data members with the values of the data members of a given object. 
Collider::Collider(ObjectData object)
{
	collider = *object.collider;
	type = object.type;
}


