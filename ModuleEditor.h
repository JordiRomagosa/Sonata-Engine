#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "IMGUI/imgui.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

private:
	ImGuiIO io;
};

#endif __ModuleEditor_H__