#include "UEngineStatics.h"

uint32 UEngineStatics::NextUUID = 0;

uint32 UEngineStatics::GenUUID()
{
    return NextUUID++;
}
