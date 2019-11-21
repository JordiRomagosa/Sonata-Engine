#ifndef __Application_H__
#define __Application_H__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleInput;
class ModuleCamera;
class ModuleProgram;
class ModuleModelLoader;
class ModuleTexture;
class ModuleEditor;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleProgram* program = nullptr;
	ModuleModelLoader* modelLoader = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleEditor* editor = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;

#endif __Application_H__