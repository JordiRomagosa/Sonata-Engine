#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleModelLoader.h"
#include "SDL/SDL.h"

#include <GL/glew.h>
#include <IMGUI/imgui.h>

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	if (VSYNC)
		SDL_GL_SetSwapInterval(1);
	
	GLenum err = glewInit();
	if (err)
	{
		LOG("Failed to initialize OpenGL loader!\n", SDL_GetError());
		return false;
	}
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	SDL_GetWindowSize(App->window->window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glViewport(0, 0, windowWidth, windowHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	math::float4x4 model = App->modelLoader->model;
	math::float4x4 view = App->camera->GetViewMatrix();
	math::float4x4 proj = App->camera->GetProjectionMatrix();

	if (showGrid)
		RenderGrid(model, view, proj);

	if (showModel)
	{
		glUseProgram(App->program->modelProgram);

		glUniformMatrix4fv(glGetUniformLocation(App->program->modelProgram,
			"model"), 1, GL_TRUE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->program->modelProgram,
			"view"), 1, GL_TRUE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->program->modelProgram,
			"proj"), 1, GL_TRUE, &proj[0][0]);

		App->modelLoader->Draw(App->program->gridProgram);
		glUseProgram(0);
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	return true;
}

void ModuleRender::WindowResized(unsigned const width, unsigned const height)
{
	windowWidth = width;
	windowHeight = height;
	App->camera->SetAspectRatio((float)width, (float)height);
}

void ModuleRender::RenderGrid(math::float4x4 & model, math::float4x4 & view, math::float4x4 & proj) const
{
	glUseProgram(App->program->gridProgram);

	glUniformMatrix4fv(glGetUniformLocation(App->program->gridProgram,
		"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->gridProgram,
		"view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->gridProgram,
		"proj"), 1, GL_TRUE, &proj[0][0]);

	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	// red X
	//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y
	//glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z
	//glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
	glLineWidth(1.0f);

	glUseProgram(0);
}

void ModuleRender::ShowRenderProperties()
{
	if (ImGui::Checkbox("Show Grid", &showGrid)) {}
	ImGui::SameLine();
	if (ImGui::Checkbox("Show Model", &showModel)) {}
}
