#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1PathFinding;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	//Tools	
	j1Timer*			timer = NULL;				//Creates a pointer to j1Timer tool. Gives acces to j1Timer's elements. Used to keep track of time since app start.
	j1PerfTimer*		perf_timer = NULL;			//Creates a pointer to j1PerfTimer tool. Gives access to j1PerfTimer's elements. Used to keep track of time since app start.
	j1PerfTimer*		last_second_timer = NULL;	//Creates a pointer to j1PerfTimer tool. Used to calculate variables in spans of one second.

	// Modules
	j1Window*			win = NULL;
	j1Input*			input = NULL;
	j1Render*			render = NULL;
	j1Textures*			tex = NULL;
	j1Audio*			audio = NULL;
	j1Scene*			scene = NULL;
	j1Map*				map = NULL;
	j1PathFinding*		pathfinding = NULL;

private:

	p2List<j1Module*>	modules;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	p2SString			load_game;
	mutable p2SString	save_game;

	//Framerate
	int					frames;					//Calcultes the amount of frames per second. frames++ after each Update() loop.
	float				seconds;				//Calculates the amount of seconds that have passed since execution start.
	float				average_frames;			//Calculates the average rate of frames per second.
	j1Timer				frame_timer;			//Keeps track of everything time related in the span of a frame.
	uint32				last_update_ms;			//Calculates the amount of milliseconds that the last update spent running.
	uint32				frames_last_second;		//Calculates the amount of frames that where processed the last second.
	uint32				prev_sec_frames;
};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif