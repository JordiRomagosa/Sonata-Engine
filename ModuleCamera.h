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

private:
	Frustum frustum;
};

#endif __ModuleCamera_H__