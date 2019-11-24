#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "IMGUI/imgui.h"

#include <vector>
#include <chrono>
#include <string>

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
	bool showConsole = false;
	bool showConfiguration = false;
	bool showModelProperties = false;

private:
	ImGuiIO io;
	std::string caps = "";
	std::vector<float> fps_log;
	std::chrono::steady_clock::time_point lastSecond;
	bool showAbout = false;

private:
	update_status GeneralMenu();
	void ViewMenu();
	void ShowAboutWindow();
	void ShowConsoleWindow();
	void ShowConfigurationWindow();
	void ShowModelPropertiesWindow();
	void UpdateFrameBuffer();
};

#endif __ModuleEditor_H__