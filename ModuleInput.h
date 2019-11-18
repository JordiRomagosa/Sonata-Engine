#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "SDL/SDL.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	void ControlCameraInputKeys();
	void ControlCameraEvents(SDL_Event & event);

private:
	const Uint8 *keyboard = NULL;
};

#endif __ModuleInput_H__