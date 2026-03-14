#pragma once

#include "CoreMinimal.h"
#include "EventType.generated.h"

UENUM(BlueprintType)
enum class EEventType : uint8
{
    Default     UMETA(DisplayName = "Default"),
    Quest       UMETA(DisplayName = "Quest"),
    Combat      UMETA(DisplayName = "Combat")
};
