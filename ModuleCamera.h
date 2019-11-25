#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "MathGeoLib/MathGeoLib.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();

	math::float4x4 GetViewMatrix() const;
	math::float4x4 GetProjectionMatrix() const;
	void SetFov(const float verticalFov);
	void SetAspectRatio(const float height, const float width);
	void TranslateCamera(const float x, const float y, const float z, const bool shift);
	void RotateCamera(const float pitch, const float yaw);
	void ZoomCamera(const bool zoomIn, const bool shift);
	void OrbitCamera(const float yaw, const float pitch);
	void LookAt(const float3 target);
	void FocusCameraOnModel();

	void ShowCameraProperties();

private:
	Frustum frustum;
	float3 cameraRight;

	float currentPitch = 0;
	float cameraMovementSpeed = 0.1f;
	float shiftSpeedMultiplier = 2;
	float cameraRotationSpeed = 0.5f;
	float verticalFov;
};

#endif __ModuleCamera_H__