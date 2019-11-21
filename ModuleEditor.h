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
	ImGuiTextBuffer consoleBuffer;
	bool editorIsEnabled = true;
	bool showConsole = true;

private:
	ImGuiIO io;
	bool showAbout = false;

private:
	update_status MainMenu();
	void ViewMenu();
	void ShowAboutWindow();
	void ShowConsoleWindow();
};

#endif __ModuleEditor_H__