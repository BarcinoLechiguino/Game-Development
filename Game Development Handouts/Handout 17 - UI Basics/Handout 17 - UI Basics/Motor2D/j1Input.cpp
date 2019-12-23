#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "SDL/include/SDL.h"

#define _CRT_SECURE_NO_WARNINGS
#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.create("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if(mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	TextInput();															//Enable/Disable text input depending on whether a INPUTBOX UI Element has been focused.

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_TEXTINPUT:

				AddTextInput(/*input_text2,*/ event.text.text);
				
				input_text += event.text.text;
				
				//LOG("Input String %s", input_text.GetString());

				//for (int i = 0; i < MAX_KEYS; ++i)
				//{
				//	if (keys[i] == 1)
				//	{
				//		/*strcat_s(input_text, 1, event.text.text);
				//		input_text = event.text.text;
				//		LOG("Input String %s", input_text);
				//		
				//		if (keyboard[i] == KEY_DOWN)
				//		{
				//			input_text = event.text.text;
				//			LOG("Input String %s", input_text);
				//		}*/
				//	}
				//}

				break;
			
			case SDL_TEXTEDITING:
				//composition = event.edit.text;
				//cursor = event.edit.text;
				//selection_len = event.edit.length;
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
				int scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

// ---------------------------- TEXT INPUT METHODS ----------------------------
void j1Input::TextInput()
{	
	if (App->gui->focusedElement != nullptr)
	{
		if (App->gui->focusedElement->element == UI_Element::INPUTBOX)
		{
			SDL_StartTextInput();											//If the focused element is of the INPUTBOX type, start text input.
			textInputEnabled = true;
		}
		else
		{
			SDL_StopTextInput();											//In case the focused element is not of the INPUTBOX type, stop text input.
			textInputEnabled = false;
		}
	}
	else
	{
		SDL_StopTextInput();												//In case the focused element is NULL. Used when right after focusing the inputbox, the focus changes to nullptr
		textInputEnabled = false;
	}

	if (char_InputString == nullptr)
	{
		Allocate(1);
		ClearTextInput();
	}
	
	if (textInputEnabled)
	{
		EditTextInputs();
	}

	
	// --- USING P2SSTRING.
	//App->gui->inputString = input_text;
	//LOG("inputString %s", App->gui->inputString.GetString());
}

void j1Input::EditTextInputs()
{
	if (GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		//input_text.DeleteLast();

		if (strlen(char_InputString) != 0)
		{
			DeleteTextInput();
		}
	}

	if (GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->win->SetTitle(char_InputString);
		ClearTextInput();
	}

	if (GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		//ClearTextInput();
	}

	if (GetKey(SDL_SCANCODE_HOME) == KEY_DOWN)
	{
		//ClearTextInput();
	}

	if (GetKey(SDL_SCANCODE_END) == KEY_DOWN)
	{
		//ClearTextInput();
	}

	// --- MOVING THE CURSOR AROUND
	if (GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{

	}

	if (GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{

	}

	/*if (GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		CutInputText((strlen(char_InputText) / 2) - 1, (strlen(char_InputText) / 2));
	}*/
}

p2SString* j1Input::GetTextInput()
{
	return &input_text;
}

const char* j1Input::GetInputText()
{
	return char_InputString;
}

int j1Input::InputTextLength()
{
	if (char_InputString != NULL)
	{
		return strlen(char_InputString);
	}
	else
	{
		return 0;
	}
}

// --------------------------------- TEXT EDITING METHODS ---------------------------------
void j1Input::AddTextInput(const char* origin)
{	
	if (origin != NULL)
	{
		text_size = strlen(char_InputString);
		unsigned int need_size = strlen(origin) + text_size + 1;

		if (need_size > text_size)
		{
			char* tmp = char_InputString;
			Allocate(need_size);
			strcpy_s(char_InputString, need_size, tmp);
			delete[] tmp;
		}

		strcat_s(char_InputString, text_size, origin);
	}
}

void j1Input::DeleteTextInput()
{
	text_size = strlen(char_InputString);
	unsigned int need_size = text_size - 1;

	char_InputString[text_size - 1] = '\0';

	input_text.DeleteLast();
}

void j1Input::DeleteLetter(int positionIndex)
{
	
	
	/*int size = Length();
	char* tmp = "\0";
	str[size - 1] = *tmp;*/
}

bool j1Input::CutInputText(unsigned int begin, unsigned int end)
{
	uint len = strlen(char_InputString);

	if (end >= len || end == 0)
		end = len - 1;

	if (begin > len || end <= begin)
		return false;

	char* p1 = char_InputString + begin;
	char* p2 = char_InputString + end + 1;

	while (*p1++ = *p2++);

	return true;
}

void j1Input::Allocate(int required_memory)
{
	text_size = required_memory;
	char_InputString = new char[text_size];
}

void j1Input::ClearTextInput()
{
	char_InputString[0] = '\0';
}

//void j1Input::AddTexts(char* destination, const char* origin)
//{
//	if (origin != NULL)
//	{
//		int size = strlen(destination);
//		unsigned int need_size = strlen(origin) + size + 1;
//
//		if (need_size > size)
//		{
//			char* tmp = destination;
//			Allocate(need_size);
//			strcpy_s(destination, need_size, tmp);
//			//delete[] tmp;
//		}
//
//		strcat_s(destination, size, origin);
//	}
//}