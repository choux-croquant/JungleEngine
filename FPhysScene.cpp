#include "FPhysScene.h"

FPhysScene::FPhysScene()
{

}

void FPhysScene::Update()
{
}

void FPhysScene::LogRender()
{
}

void FPhysScene::setSampleCube(const UCubeComp& uCubeComp)
{
	cubes.push_back(uCubeComp);
}