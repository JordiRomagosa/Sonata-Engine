#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"


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

	cameraRight = frustum.up.Cross(frustum.front); cameraRight.Normalize();
	cameraAdvance = cameraRight.Cross(float3(0, 1, 0)); cameraAdvance.Normalize();
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

void ModuleCamera::TranslateCamera(float x, float y, float z, bool shift)
{
	float speedMult = 1;
	if (shift)
		speedMult = shiftSpeedMultiplier;

	if (x != 0)
		frustum.pos -= x * cameraRight * cameraMovementSpeed * speedMult;
	if (y != 0)
		frustum.pos -= y * float3(0, 1, 0) * cameraMovementSpeed * speedMult;
	if (z != 0)
		frustum.pos += z * cameraAdvance * cameraMovementSpeed * speedMult;
}

void ModuleCamera::RotateCamera(int pitch, int yaw)
{
	math::float3x3 rotationMatrix;
	float angle = math::DegToRad(cameraRotationSpeed);
	if (pitch > 0 || yaw > 0)
		angle *= -1;

	if (yaw != 0)
	{
		rotationMatrix = math::float3x3::RotateY(angle);

		float3 mult = rotationMatrix * frustum.front;
		frustum.front = mult;
		mult = rotationMatrix * frustum.up;
		frustum.up = mult;
	}

	if (pitch != 0)
	{
		if ((currentPitch < 89 || pitch < 0) && (currentPitch > -89 || pitch > 0))
		{
			currentPitch += pitch * cameraRotationSpeed;
			rotationMatrix = math::float3x3::RotateAxisAngle(cameraRight, angle);

			float3 mult = rotationMatrix * frustum.front;
			frustum.front = mult;
			mult = rotationMatrix * frustum.up;
			frustum.up = mult;
		}
	}

	cameraRight = frustum.up.Cross(frustum.front); cameraRight.Normalize();
	cameraAdvance = cameraRight.Cross(float3(0, 1, 0)); cameraAdvance.Normalize();
}

void ModuleCamera::ZoomCamera(bool zoomIn, bool shift)
{
	float3 cameraFront = frustum.front; cameraFront.Normalize();
	float speedMult = 5;
	if (shift)
		speedMult *= 2 * shiftSpeedMultiplier;

	if (!zoomIn)
		speedMult *= -1;
	
	frustum.pos += cameraFront * cameraMovementSpeed * speedMult;
}

void ModuleCamera::OrbitCamera(float yaw, float pitch)
{
	float3 center(0, 0, 0);


	if (yaw != 0.0f)
	{
		float3x3 rot = float3x3::RotateY(math::DegToRad(yaw * cameraRotationSpeed));
		frustum.pos = rot.Transform(frustum.pos - center) + center;
	}

	if (pitch != 0.0f)
	{
		if (currentPitch + pitch > 89)
		{
			pitch = 89 - currentPitch;
			currentPitch = 89;
		}
			
		else if (currentPitch + pitch < -89)
		{
			pitch = -89 - currentPitch;
			currentPitch = -89;
		}
		else
			currentPitch += pitch * cameraRotationSpeed;

		float3x3 rot = float3x3::RotateAxisAngle(cameraRight, math::DegToRad(-1.0 * pitch * cameraRotationSpeed));
		frustum.pos = rot.Transform(frustum.pos - center) + center;
	}

	LookAt(center);
}

void ModuleCamera::LookAt(float3 target)
{
	float3 newFront = target - frustum.pos; newFront.Normalize();
	float3x3 rot = float3x3::LookAt(frustum.front, newFront, frustum.up, float3::unitY);
	frustum.front = rot.Transform(frustum.front).Normalized();
	frustum.up = rot.Transform(frustum.up).Normalized();

	cameraRight = frustum.up.Cross(frustum.front); cameraRight.Normalize();
	cameraAdvance = cameraRight.Cross(float3(0, 1, 0)); cameraRight.Normalize();
}

void ModuleCamera::FocusCameraOnModel()
{
	frustum.pos = App->modelLoader->GetFocusModelPoint();
	LookAt(App->modelLoader->GetModelCenter());
}
