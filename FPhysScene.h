#pragma once
#include "UCubeComp.h"
#include "Types.h"
class FPhysScene
{
public:
	FPhysScene();
	void setSampleCube(const UCubeComp& uCubeComp);
	void Update();
	void LogRender();
private:
	TArray<UPrimitiveComponent> cubes;
};

