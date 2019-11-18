#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"


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
		}
		ControlCameraEvents(sdlEvent);
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

void ModuleInput::ControlCameraInputKeys()
{
	bool shift = keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT];
	
	//Translate the camera with WASD + QE
	int moveX = 0, moveY = 0, moveZ = 0;

	if (keyboard[SDL_SCANCODE_D] && !keyboard[SDL_SCANCODE_A])
		moveX = 1;
	else if (keyboard[SDL_SCANCODE_A] && !keyboard[SDL_SCANCODE_D])
		moveX = -1;
	if (keyboard[SDL_SCANCODE_W] && !keyboard[SDL_SCANCODE_S])
		moveZ = 1;
	else if (keyboard[SDL_SCANCODE_S] && !keyboard[SDL_SCANCODE_W])
		moveZ = -1;
	if (keyboard[SDL_SCANCODE_Q] && !keyboard[SDL_SCANCODE_E])
		moveY = 1;
	else if (keyboard[SDL_SCANCODE_E] && !keyboard[SDL_SCANCODE_Q])
		moveY = -1;

	App->camera->TranslateCamera(moveX, moveY, moveZ, shift);

	//Rotate camera with arrows
	int pitch = 0, yaw = 0;
	if (keyboard[SDL_SCANCODE_UP] && !keyboard[SDL_SCANCODE_DOWN])
		pitch = 1;
	else if (keyboard[SDL_SCANCODE_DOWN] && !keyboard[SDL_SCANCODE_UP])
		pitch = -1;

	if (keyboard[SDL_SCANCODE_RIGHT] && !keyboard[SDL_SCANCODE_LEFT])
		yaw = 1;
	else if (keyboard[SDL_SCANCODE_LEFT] && !keyboard[SDL_SCANCODE_RIGHT])
		yaw = -1;

	App->camera->RotateCamera(pitch, yaw);
}

void ModuleInput::ControlCameraEvents(SDL_Event & event)
{
	bool shift = keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT];

	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		App->camera->ZoomCamera(event.wheel.y > 0, shift);
		break;
	}
}
