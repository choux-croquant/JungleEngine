#include "AActor.h"
#include "UActorComponent.h"

AActor::AActor() {}
AActor::~AActor() {}

void AActor::AddComponent(UActorComponent* Component)
{
    if (Component)
    {
        Component->Owner = this;
        Components.push_back(Component);
    }
}

void AActor::Tick(float DeltaTime)
{
    for (UActorComponent* Component : Components)
    {
        if (Component && Component->bIsActive)
        {
            Component->TickComponent(DeltaTime);
        }
    }
}
