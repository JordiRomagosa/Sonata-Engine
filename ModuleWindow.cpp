#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include <IMGUI/imgui.h>

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		int width = DM.w * 0.95;
		int height = DM.h * 0.9;
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		fullscreen = FULLSCREEN;
		resizable = RESIZABLE;
		fullscreenDesktop = false;
		bordered = true;

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		glcontext = SDL_GL_CreateContext(window);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetFullScreen() const
{
	if (fullscreenDesktop)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else if (fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetResizable() const
{
	if (resizable)
		SDL_SetWindowResizable(window, SDL_TRUE);
	else
		SDL_SetWindowResizable(window, SDL_FALSE);
}

void ModuleWindow::SetBordered() const
{
	if (bordered)
		SDL_SetWindowBordered(window, SDL_TRUE);
	else
		SDL_SetWindowBordered(window, SDL_FALSE);
}

void ModuleWindow::SetBrightness() const
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::ShowWindowProperties()
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	if (ImGui::SliderInt("Width", &width, 100, 1000, "%d"))
		SDL_SetWindowSize(window, width, height);
	if (ImGui::SliderInt("Height", &height, 100, 800, "%d"))
		SDL_SetWindowSize(window, width, height);

	if (ImGui::SliderFloat("Brightness", &brightness, 0.0, 1.0, "%.2f"))
		SetBrightness();

	ImGui::Separator();
	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		SetFullScreen();
	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &resizable))
		SetResizable();
	
	if (ImGui::Checkbox("Fullscreen Desktop", &fullscreenDesktop))
		SetFullScreen();
	ImGui::SameLine();
	if (ImGui::Checkbox("Bordered", &bordered))
		SetBordered();
}

