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

	math::float4x4 GetViewMatrix();
	math::float4x4 GetProjectionMatrix();
	void SetFov(float verticalFov);
	void SetAspectRatio(float height, float width);
	void TranslateCamera(float x, float y, float z, bool shift);
	void RotateCamera(int pitch, int yaw);
	void ZoomCamera(bool zoomIn, bool shift);
	void OrbitCamera(float x, float y);
	void LookAt(float3 target);
	void FocusCameraOnModel();

	void ShowCameraProperties();

private:
	Frustum frustum;
	float3 cameraRight;
	float3 cameraAdvance;

	float currentPitch = 0;
	float cameraMovementSpeed = 0.1f;
	float shiftSpeedMultiplier = 2;
	float cameraRotationSpeed = 0.5f;
	float verticalFov;
};

#endif __ModuleCamera_H__