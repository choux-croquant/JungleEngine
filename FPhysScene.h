#pragma once
#include "UCubeComp.h"
#include "Types.h"
class FPhysScene
{
public:
	FPhysScene();
	void setSampleCube(const UCubeComp& uCubeComp);
private:
	TArray<UPrimitiveComponent> cubes;
};

