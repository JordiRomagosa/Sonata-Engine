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

public:
	bool editorIsEnabled = true;

private:
	ImGuiIO io;
	bool showAbout = false;

private:
	update_status MainMenu();
	void ShowAboutTab();
};

#endif __ModuleEditor_H__