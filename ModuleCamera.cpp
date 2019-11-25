#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"

#include <IMGUI/imgui.h>

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
	frustum.pos = float3(0, 3, -2);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);

	verticalFov = frustum.verticalFov;

	cameraRight = frustum.up.Cross(frustum.front); cameraRight.Normalize();
	return true;
}

math::float4x4 ModuleCamera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

math::float4x4 ModuleCamera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

void ModuleCamera::SetFov(float verticalFov)
{
	float aspect = frustum.AspectRatio();

	frustum.verticalFov = verticalFov;
	frustum.horizontalFov = frustum.verticalFov *aspect;
}

void ModuleCamera::SetAspectRatio(const float height, const float width)
{
	float aspect = height / width;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
}

void ModuleCamera::TranslateCamera(const float x, const float y, const float z, const bool shift)
{
	float speedMult = 1;
	if (shift)
		speedMult = shiftSpeedMultiplier;

	if (x != 0)
		frustum.pos -= x * cameraRight * cameraMovementSpeed * speedMult;
	if (y != 0)
		frustum.pos -= y * float3(0, 1, 0) * cameraMovementSpeed * speedMult;
	if (z != 0)
		frustum.pos += z * frustum.front * cameraMovementSpeed * speedMult;
}

void ModuleCamera::RotateCamera(const float pitch, const float yaw)
{
	math::float3x3 rotationMatrix = math::float3x3::identity;
	float anglePitch = math::DegToRad(-pitch * cameraRotationSpeed);
	float angleYaw = math::DegToRad(-yaw * cameraRotationSpeed);
	
	if (yaw != 0)
	{
		rotationMatrix = rotationMatrix * math::float3x3::RotateY(angleYaw);
	}

	if (pitch != 0)
	{
		if ((currentPitch < 89 || pitch < 0) && (currentPitch > -89 || pitch > 0))
		{
			currentPitch += pitch * cameraRotationSpeed;
			rotationMatrix = rotationMatrix * math::float3x3::RotateAxisAngle(cameraRight, anglePitch);
		}
	}

	float3 mult = rotationMatrix * frustum.front;
	frustum.front = mult;
	mult = rotationMatrix * frustum.up;
	frustum.up = mult;

	cameraRight = frustum.up.Cross(frustum.front); cameraRight.Normalize();
}

void ModuleCamera::ZoomCamera(const bool zoomIn, const bool shift)
{
	float3 cameraFront = frustum.front; cameraFront.Normalize();
	float speedMult = 5;
	if (shift)
		speedMult *= 2 * shiftSpeedMultiplier;

	if (!zoomIn)
		speedMult *= -1;
	
	frustum.pos += cameraFront * cameraMovementSpeed * speedMult;
}

void ModuleCamera::OrbitCamera(const float yaw, const float pitch)
{
	float pitchMod = pitch;
	float3 center = App->modelLoader->GetModelCenter();

	if (yaw != 0.0f)
	{
		float3x3 rot = float3x3::RotateY(math::DegToRad(yaw * cameraRotationSpeed));
		frustum.pos = rot.Transform(frustum.pos - center) + center;
	}

	if (pitch != 0.0f)
	{
		if (currentPitch + pitch > 89)
		{
			pitchMod = 89 - currentPitch;
			currentPitch = 89;
		}
			
		else if (currentPitch + pitch < -89)
		{
			pitchMod = -89 - currentPitch;
			currentPitch = -89;
		}
		else
			currentPitch += pitchMod * cameraRotationSpeed;

		float3x3 rot = float3x3::RotateAxisAngle(cameraRight, math::DegToRad(-1.0 * pitchMod * cameraRotationSpeed));
		frustum.pos = rot.Transform(frustum.pos - center) + center;
	}

	LookAt(center);
}

void ModuleCamera::LookAt(const float3 target)
{
	float3 newFront = target - frustum.pos; newFront.Normalize();
	float3x3 rot = float3x3::LookAt(frustum.front, newFront, frustum.up, float3::unitY);
	frustum.front = rot.Transform(frustum.front).Normalized();
	frustum.up = rot.Transform(frustum.up).Normalized();

	cameraRight = frustum.up.Cross(frustum.front); cameraRight.Normalize();
}

void ModuleCamera::FocusCameraOnModel()
{
	if (!App->modelLoader->isModelLoaded)
		return;
	float3 modelCenter = App->modelLoader->GetModelCenter();
	LookAt(modelCenter);

	float maxModelAxis = App->modelLoader->GetModelAABB().Size().MaxElement();
	frustum.pos = modelCenter - frustum.front * (1.5 * maxModelAxis);
}

void ModuleCamera::ShowCameraProperties()
{
	ImGui::Text("Camera Position: x:%.2f y:%.2f z:%.2f", frustum.pos.x, frustum.pos.y, frustum.pos.z);
	ImGui::Text("Camera Front: x:%.2f y:%.2f z:%.2f", frustum.front.x, frustum.front.y, frustum.front.z);
	ImGui::Text("Camera Up: x:%.2f y:%.2f z:%.2f", frustum.up.x, frustum.up.y, frustum.up.z);

	ImGui::Separator();
	if (ImGui::SliderFloat("Vertical FOV", &verticalFov, 0.01, math::pi - 0.01, "%.3f"))
		SetFov(verticalFov);
	ImGui::SliderFloat("Near Plane", &frustum.nearPlaneDistance, 1, 100, "%.0f");
	ImGui::SliderFloat("Far Plane", &frustum.farPlaneDistance, 10, 1000, "%.0f");
	if (frustum.nearPlaneDistance > frustum.farPlaneDistance)
		frustum.farPlaneDistance = frustum.nearPlaneDistance + 0.1;

	ImGui::SliderFloat("Movement Speed", &cameraMovementSpeed, 0.01, 1, "%.2f");
	ImGui::SliderFloat("Rotation Speed", &cameraRotationSpeed, 0.01, 1, "%.2f");
	ImGui::SliderFloat("Shift Speed Modifier", &shiftSpeedMultiplier, 1, 5, "%.1f");
}
