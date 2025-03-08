#include "FPhysScene.h"

FPhysScene::FPhysScene()
{

}

void FPhysScene::setSampleCube(const UCubeComp& uCubeComp)
{
	cubes.push_back(uCubeComp);
}