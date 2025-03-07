#pragma once
#include "Types.h"

class UEngineStatics
{
public:
	static uint32 GenUUID()
	{
		return NextUUID++;
	}

	static uint32 NextUUID;
};

uint32 UEngineStatics::NextUUID = 0;