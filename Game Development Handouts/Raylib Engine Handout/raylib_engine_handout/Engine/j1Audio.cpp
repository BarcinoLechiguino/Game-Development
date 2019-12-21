#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"

#include "raylib.h"

#define MAX_FX_SOUNDS   64

j1Audio::j1Audio() : j1Module()
{
	name.create("audio");

    fx = new Sound[MAX_FX_SOUNDS];
    fx_count = 0;
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	bool ret = true;

    LOG("Loading raylib audio system");

    InitAudioDevice();

	return ret;
}

bool j1Audio::Update()
{
    if (music.stream.buffer != NULL) UpdateMusicStream(music);

    // TODO: Fade music volume using raylib available functionality:
/*
void SetMusicVolume(Music music, float volume);     // Set volume for music (1.0 is max level)
void SetMusicPitch(Music music, float pitch);       // Set pitch for a music (1.0 is base level)
float GetMusicTimeLength(Music music);              // Get music time length (in seconds)
float GetMusicTimePlayed(Music music);              // Get current music time played (in seconds)
*/
    return true;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active) return true;

    // Unload sounds
    for (unsigned int i = 0; i < fx_count; i++) UnloadSound(fx[i]);

    RELEASE(fx);

    // Unload music
    if (music.stream.buffer != NULL)
    {
        StopMusicStream(music);
        UnloadMusicStream(music);
    }

    CloseAudioDevice();

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active) return false;
    
    StopMusicStream(music);
    music = LoadMusicStream(path);
    
    if (music.stream.buffer != NULL) PlayMusicStream(music);

	LOG("Successfully playing %s", path);

	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active) return 0;

	Sound sound = LoadSound(path);

	if(sound.stream.buffer == NULL)
	{
		LOG("Cannot load sound: %s", path);
	}
	else
	{
        fx[fx_count++] = sound;
		ret = fx_count;
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active) return false;

	if(id < fx_count) PlaySound(fx[id]);

	return ret;
}