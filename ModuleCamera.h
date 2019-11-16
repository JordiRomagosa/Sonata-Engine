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
	void SetAspectRatio(float height, float width);
	void TranslateCamera(int x, int y, int z, bool shift);
	void RotateCamera(int pitch, int yaw);

private:
	Frustum frustum;
	float3 cameraRight;
	float3 cameraAdvance;
	
	float currentPitch = 0;
	float cameraMovementSpeed = 0.01f;
	int shiftSpeedMultiplier = 2;
	float cameraRotationSpeed = 0.05f;
};

#endif __ModuleCamera_H__