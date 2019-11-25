#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	void SetFullScreen() const;
	void SetResizable() const;
	void SetBordered() const;
	void SetBrightness() const;
	void ShowWindowProperties();

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	//The OpenGL context associated with the window
	SDL_GLContext glcontext = NULL;

private:
	bool fullscreen, resizable, fullscreenDesktop, bordered;
	float brightness = 1.0f;
};

#endif __ModuleWindow_H__