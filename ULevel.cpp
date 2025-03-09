#include "ULevel.h"
#include "UPrimitiveComponent.h"
#include "Types.h"

using uint32 = unsigned int;

void ULevel::AddPrimitiveComponent(UPrimitiveComponent* Component)
{
    {
        if (Component) {
            PrimitiveComponents.push_back(Component);
        }
    }
}

void ULevel::Tick(float DeltaTime)
{
    for (UPrimitiveComponent* Component : PrimitiveComponents) {
        if (Component) {
            Component->TickComponent(DeltaTime);
        }
    }
}
