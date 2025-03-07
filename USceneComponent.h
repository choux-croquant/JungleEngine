#pragma once
#include "UObject.h"
#include "Vector.h"

class USceneComponent : public UObject
{
public:
	USceneComponent(FVector Position, FVector Rotation, FVector Scale)
		: RelativeLocation(Position), RelativeRotation(Rotation), RelativeScale3D(Scale) {
	};

	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;

	FVector GetWorldLocation() const { return RelativeLocation; }
	FVector GetWorldRotation() const { return RelativeRotation; }
	FVector GetWorldScale3D() const { return RelativeScale3D; }
};