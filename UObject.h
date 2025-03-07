#pragma once
#include "Types.h"
#include "UEngineStatics.h"

class UObject
{
public:
	UObject()
	{
		UUID = UEngineStatics::GenUUID();
	}

	uint32 UUID;
	uint32 InternalIndex; // Index of GUObjectArray
};

TArray<UObject*> GUObjectArray;