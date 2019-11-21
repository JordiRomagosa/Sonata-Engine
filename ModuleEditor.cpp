#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"

#include <GL/glew.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl3.h>

ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->glcontext);
	ImGui_ImplOpenGL3_Init("#version 130");

	/*caps = "";
	if (SDL_Has3DNow())
		caps += "3DNow, ";
	if (SDL_HasAVX())
		caps += "AVX, ";
	if (SDL_HasAVX2())
		caps += "AVX2, ";
	if (SDL_HasAltiVec())
		caps += "AltiVec, ";
	if (SDL_HasMMX())
		caps += "MMX, ";
	if (SDL_HasRDTSC())
		caps += "RDTSC, ";
	if (SDL_HasSSE())
		caps += "SSE, ";
	if (SDL_HasSSE2())
		caps += "SSE2, ";
	if (SDL_HasSSE3())
		caps += "SSE3, ";
	if (SDL_HasSSE41())
		caps += "SSE41, ";
	if (SDL_HasSSE42())
		caps += "SSE42, ";
	if (caps.size() > 0)  caps.resize(caps.size() - 2);

	lastSecond = std::chrono::steady_clock::now();

	fullscreen = FULLSCREEN;
	resizable = RESIZABLE;

	verticalPOV = math::pi / 4.0f;
	App->camera->SetFOV(verticalPOV);*/

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	if (MainMenu() != UPDATE_CONTINUE)
		return UPDATE_STOP;

	if (showAbout)
		ShowAboutTab();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::MainMenu()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Menu"))
	{
		if (ImGui::MenuItem("About"))
			showAbout = !showAbout;

		if (ImGui::MenuItem("Github Link"))
			App->RequestBrowser("https://github.com/JordiRomagosa/Sonata-Engine");

		char * labelText = "Enable Editor";
		if (editorIsEnabled)
			labelText = "Disable Editor";
		if (ImGui::MenuItem(labelText))
			editorIsEnabled = !editorIsEnabled;

		if (ImGui::MenuItem("Quit"))
			return UPDATE_STOP;

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	return UPDATE_CONTINUE;
}

void ModuleEditor::ShowAboutTab()
{
	ImGui::Begin("About");

	ImGui::Text("SAEGE: the Super Awersome and Experimental Game Engine");
	ImGui::Text("Used for learning. This engine is under construction.");
	ImGui::Text("Author: Jordi Romagosa Mellado");
	ImGui::Text("Libraries used: Glew 2.1.0, SDL v2.0.10, IMGUI v1.73, MathGeoLib v1.5");

	ImGui::Text("License:\n"
		"\tMIT License - Copyright(c)2019 - Jordi Romagosa Mellado\n\n"

		"\tPermission is hereby granted, free of charge, to any person obtaining a copy\n"
		"\tof this software and associated documentation files(the \"Software\"), to deal\n"
		"\tin the Software without restriction, including without limitation the rights\n"
		"\tto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
		"\tcopies of the Software, and to permit persons to whom the Software is\n"
		"\tfurnished to do so, subject to the following conditions :\n\n"

		"\tThe above copyright notice and this permission notice shall be included in all\n"
		"\tcopies or substantial portions of the Software.\n\n"

		"\tTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
		"\tIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
		"\tFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\n"
		"\tAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
		"\tLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
		"\tOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
		"\tSOFTWARE.");

	ImGui::End();
}
