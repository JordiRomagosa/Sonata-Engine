#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"


ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	float aspect = float(w) / float(h);

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0, 1, -2);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);

	return true;
}

math::float4x4 ModuleCamera::GetViewMatrix() {
	return frustum.ViewMatrix();
}

math::float4x4 ModuleCamera::GetProjectionMatrix() {
	return frustum.ProjectionMatrix();
}

void ModuleCamera::SetAspectRatio(float height, float width)
{
	float aspect = height / width;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
}