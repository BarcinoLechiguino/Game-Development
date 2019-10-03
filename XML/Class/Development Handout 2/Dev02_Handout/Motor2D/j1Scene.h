#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	/*void saveGame() { Save = true; }
	void loadGame() { Load = true; }*/

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load = false;
	bool Save = false;

private:
	SDL_Texture* img;
	/*bool Load = false;
	bool Save = false;*/
};

#endif // __j1SCENE_H__