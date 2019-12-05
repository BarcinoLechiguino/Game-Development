#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update()
{
	// OnKeys WASD keys -----------------------------------

	// TODO 3: Make the camera go up/down when pressing R (up) F(down)
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
	{
		Position.y += 0.1f;									//Increases the position of the camera in the y axis by 0.1 units per frame.
		Reference.y += 0.1f;								//Increases the position of the camera's point of reference in the y axis by 0.1 units per frame.
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
	{
		Position.y -= 0.1f;									//Decreases the position of the camera in the y axis by 0.1 units per frame.
		Reference.y -= 0.1f;								//Decreases the position of the camera's point of reference in the y axis by 0.1 units per frame.
	}

	// TODO 4: Make the camera go forward (w) and backward with (s)
	// Note that the vectors X/Y/Z contain the current axis of the camera
	// you can read them to modify Position
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		//Position.z -= 0.1f;								//Decreases the position of the camera in the z axis by 0.1 units per frame.
		//Z;												//It is the position of the Z vector of the camera in 0.1 units per frame. The Z vector defines where in the z axis the camera is placed/looks at.
		//Reference.z -= 0.1f;								//Decreases the position of the camera's point of reference in the z axis by 0.1 units per frame.

		//Move() receives a movement vector and updates both 
		//the position of the camera and the reference point. 
		//Moreover it recalculates the view matrix (rotations).
		Move(-Z);											//Moves the camera forward along the z axis while taking into account the point of reference and the Z vector of the camera.
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		Move(Z);											//Moves the camera backward along the z axis while taking into account the poing of reference and the Z vector of the camera.
	}

	// TODO 5: Make the camera go left (a) and right with (d)
	// Note that the vectors X/Y/Z contain the current axis of the camera
	// you can read them to modify Position
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		Move(-X);											//Moves the camera to the left along the x axis while taking into account the point of reference and the X vector of the camera.
	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		Move(X);											//Moves the camera to the right along the x axis while taking into account the point of reference and the X vector of the camera.
	}

	// Mouse motion ----------------
	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		// TODO (Homework): Rotate the camera with the mouse
		vec3 Forward = -Z;							//Sets a new Vec3 that with the data of the camera's Z vector. -Z represents the vector that determines where the camera is looking directly. Imagine as a bar going through the camera horizontally.
		
		//In rotate():
		//"u" is the vector to rotate (the Z axis of the camera) 
		//"angle" is the value to rotate (the movement in pixels of the mouse) 
		//"v" is the axis of rotation (fixed axis around which the camera will rotate).
		Forward = rotate(Forward, dx, Y);			//The movement of the mouse in the x axis will be translated to rotation of the Z vector around the Y axis.
		Forward = rotate(Forward, dy, X);			//The movement of the mouse in the y axis will be translated to rotation of the Z vector around the X axis.

		LookAt(Reference - Forward);				//Changes the camera's point of reference according to the camera's -Z vector after it has been rotated according to mouse movement.

		//Look(Forward, Reference - Forward, true);	//Rotates the camera around a position and taking into account a point of recerence. It hass a bool that if set to true makes the camera rotate around the given reference.
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
