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
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Menu"))
	{
		if (ImGui::MenuItem("Quit"))
			return UPDATE_STOP;

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();


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
