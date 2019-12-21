#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"

#define MAX_KEYBOARD_KEYS 256

j1Input::j1Input() : j1Module()
{
	name.create("input");
}

// Destructor
j1Input::~j1Input()
{
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	bool ret = true;
    
    LOG("Init input event system");

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	return true;
}

void j1Input::GetMousePosition(int& x, int& y)
{
    // Calling raylib functions equivalent
	x = GetMouseX();
	y = GetMouseY();
}

// TODO: Call raylib equivalent functions from this class
// raylib API available funcs:
/*
// Input-related functions: keyboard
bool IsKeyPressed(int key);                             // Detect if a key has been pressed once
bool IsKeyDown(int key);                                // Detect if a key is being pressed
bool IsKeyReleased(int key);                            // Detect if a key has been released once
bool IsKeyUp(int key);                                  // Detect if a key is NOT being pressed
void SetExitKey(int key);                               // Set a custom key to exit program (default is ESC)
int GetKeyPressed(void);                                // Get key pressed, call it multiple times for chars queued

// Input-related functions: gamepads
bool IsGamepadAvailable(int gamepad);                   // Detect if a gamepad is available
bool IsGamepadName(int gamepad, const char *name);      // Check gamepad name (if available)
const char *GetGamepadName(int gamepad);                // Return gamepad internal name id
bool IsGamepadButtonPressed(int gamepad, int button);   // Detect if a gamepad button has been pressed once
bool IsGamepadButtonDown(int gamepad, int button);      // Detect if a gamepad button is being pressed
bool IsGamepadButtonReleased(int gamepad, int button);  // Detect if a gamepad button has been released once
bool IsGamepadButtonUp(int gamepad, int button);        // Detect if a gamepad button is NOT being pressed
int GetGamepadButtonPressed(void);                      // Get the last gamepad button pressed
int GetGamepadAxisCount(int gamepad);                   // Return gamepad axis count for a gamepad
float GetGamepadAxisMovement(int gamepad, int axis);    // Return axis movement value for a gamepad axis

// Input-related functions: mouse
bool IsMouseButtonPressed(int button);                  // Detect if a mouse button has been pressed once
bool IsMouseButtonDown(int button);                     // Detect if a mouse button is being pressed
bool IsMouseButtonReleased(int button);                 // Detect if a mouse button has been released once
bool IsMouseButtonUp(int button);                       // Detect if a mouse button is NOT being pressed
int GetMouseX(void);                                    // Returns mouse position X
int GetMouseY(void);                                    // Returns mouse position Y
Vector2 GetMousePosition(void);                         // Returns mouse position XY
void SetMousePosition(int x, int y);                    // Set mouse position XY
void SetMouseOffset(int offsetX, int offsetY);          // Set mouse offset
void SetMouseScale(float scaleX, float scaleY);         // Set mouse scaling
int GetMouseWheelMove(void);                            // Returns mouse wheel movement Y

// Input-related functions: touch
int GetTouchX(void);                                    // Returns touch position X for touch point 0 (relative to screen size)
int GetTouchY(void);                                    // Returns touch position Y for touch point 0 (relative to screen size)
Vector2 GetTouchPosition(int index);                    // Returns touch position XY for a touch point index (relative to screen size)
*/