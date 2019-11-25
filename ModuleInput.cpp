#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"

#include <IMGUI/imgui.h>
#include <string>

using namespace std;

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
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

// Called every draw update
update_status ModuleInput::Update()
{
	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(NULL);

	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			return UPDATE_STOP;

		case SDL_WINDOWEVENT:
			if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
			break;

		case SDL_DROPFILE:
			char * filePath = sdlEvent.drop.file;
			DroppedFile(filePath);
			break;
		}
		ControlMouseEvents(sdlEvent);
	}

	if (keyboard[SDL_SCANCODE_ESCAPE])
		return UPDATE_STOP;

	ControlCameraInputKeys();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::ControlCameraInputKeys() const
{
	bool shift = keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT];
	
	//Translate the camera with WASD + QE
	float moveX = 0, moveY = 0, moveZ = 0;

	if (keyboard[SDL_SCANCODE_D] && !keyboard[SDL_SCANCODE_A])
		moveX = 1;
	else if (keyboard[SDL_SCANCODE_A] && !keyboard[SDL_SCANCODE_D])
		moveX = -1;
	if (keyboard[SDL_SCANCODE_W] && !keyboard[SDL_SCANCODE_S])
		moveZ = 1;
	else if (keyboard[SDL_SCANCODE_S] && !keyboard[SDL_SCANCODE_W])
		moveZ = -1;
	if (keyboard[SDL_SCANCODE_Q] && !keyboard[SDL_SCANCODE_E])
		moveY = -1;
	else if (keyboard[SDL_SCANCODE_E] && !keyboard[SDL_SCANCODE_Q])
		moveY = 1;

	if (rightMousePressed)
		App->camera->TranslateCamera(moveX, moveY, moveZ, shift);

	//Rotate camera with arrows
	float pitch = 0, yaw = 0;
	if (keyboard[SDL_SCANCODE_UP] && !keyboard[SDL_SCANCODE_DOWN])
		pitch = 1;
	else if (keyboard[SDL_SCANCODE_DOWN] && !keyboard[SDL_SCANCODE_UP])
		pitch = -1;

	if (keyboard[SDL_SCANCODE_RIGHT] && !keyboard[SDL_SCANCODE_LEFT])
		yaw = 1;
	else if (keyboard[SDL_SCANCODE_LEFT] && !keyboard[SDL_SCANCODE_RIGHT])
		yaw = -1;

	App->camera->RotateCamera(pitch, yaw);

	if (keyboard[SDL_SCANCODE_F])
		App->camera->FocusCameraOnModel();
}

void ModuleInput::ControlMouseEvents(const SDL_Event & event)
{
	bool shift = keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT];
	bool alt = keyboard[SDL_SCANCODE_LALT] || keyboard[SDL_SCANCODE_RALT];

	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		App->camera->ZoomCamera(event.wheel.y > 0, shift);
		break;

	case SDL_MOUSEBUTTONDOWN:
		lastMouseX = (float)event.button.x;
		lastMouseY = (float)event.button.y;

		if (event.button.button == SDL_BUTTON_RIGHT)
			rightMousePressed = true;
		else if (event.button.button == SDL_BUTTON_LEFT)
			leftMousePressed = true;
		break;

	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_RIGHT)
			rightMousePressed = false;
		if (event.button.button == SDL_BUTTON_LEFT)
			leftMousePressed = false;
		break;

	case SDL_MOUSEMOTION:
		currentMouseX = (float)event.button.x;
		currentMouseY = (float)event.button.y;

		if (rightMousePressed)
			App->camera->RotateCamera(lastMouseY - currentMouseY, currentMouseX - lastMouseX);

		else if (leftMousePressed && alt)
			App->camera->OrbitCamera(lastMouseX - currentMouseX, lastMouseY - currentMouseY);

		lastMouseX = currentMouseX;
		lastMouseY = currentMouseY;
		break;
	}
}

void ModuleInput::DroppedFile(const char * path) const
{
	string sPath = path;
	size_t pos = sPath.find_last_of('.');
	string extension = sPath.substr(pos, string::npos);

	if (extension == ".fbx")
		App->modelLoader->loadModel(path);
	else
		App->modelLoader->loadTexture(path);
}

void ModuleInput::ShowInputProperties() const
{
	ImGui::Text("Mouse Position: x:%.2f y:%.2f", currentMouseX, currentMouseY);

	string showText = leftMousePressed ? "True" : "False";
	ImGui::Text("Mouse Left: %s", showText.c_str());
	showText = rightMousePressed ? "True" : "False";
	ImGui::Text("Mouse Right: %s", showText.c_str());

	showText = "";
	if (keyboard[SDL_SCANCODE_UP]) showText += "Up, ";
	if (keyboard[SDL_SCANCODE_DOWN]) showText += "Down, ";
	if (keyboard[SDL_SCANCODE_LEFT]) showText += "Left, ";
	if (keyboard[SDL_SCANCODE_RIGHT]) showText += "Right, ";

	if (keyboard[SDL_SCANCODE_W]) showText += "W, ";
	if (keyboard[SDL_SCANCODE_S]) showText += "S, ";
	if (keyboard[SDL_SCANCODE_A]) showText += "A, ";
	if (keyboard[SDL_SCANCODE_D]) showText += "D, ";
	if (keyboard[SDL_SCANCODE_Q]) showText += "Q, ";
	if (keyboard[SDL_SCANCODE_E]) showText += "E, ";

	if (keyboard[SDL_SCANCODE_LSHIFT]) showText += "LShift, ";
	if (keyboard[SDL_SCANCODE_RSHIFT]) showText += "RShift, ";
	if (keyboard[SDL_SCANCODE_LALT]) showText += "LAlt, ";
	if (keyboard[SDL_SCANCODE_RALT]) showText += "RAlt, ";

	if (keyboard[SDL_SCANCODE_F]) showText += "F, ";

	if (showText.size() > 0)
		showText = showText.substr(0, showText.size()-2);

	ImGui::Text("Keys Pressed: %s", showText.c_str());
}
