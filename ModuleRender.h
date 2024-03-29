#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned const width, unsigned const height);

	void ShowRenderProperties();

private:
	void* context;
	int windowWidth = 0;
	int windowHeight = 0;

	bool showGrid = true;
	bool showModel = true;

	void RenderGrid(math::float4x4 & model, math::float4x4 & view, math::float4x4 & proj) const;
};

#endif __ModuleRender_H__