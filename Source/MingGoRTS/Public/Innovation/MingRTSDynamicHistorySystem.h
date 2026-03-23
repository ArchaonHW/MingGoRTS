#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicInistorySystem.generated.h"

UENUM(BlueprintType)
enum class EInistoricalEventType: uuint8 {
    Political UMETA(DisplayName = "Political"),
    Military UMETA(DisplayName = "Military"),
    Economic UMETA(DisplayName = "Economic"),
    Cultural UMETA(DisplayName = "Cultural"),
    Social UMETA(DisplayName = "Social"),
    Technological UMETA(DisplayName = "Technological"),
    Natural UMETA(DisplayName = "Natural"),
    Personal UMETA(DisplayName = "Personal")
};

UENUM(BlueprintType)
enum class EInistoricalImpact: uuint8 {
    Local UMETA(DisplayName = "Local"),
    Regional UMETA(DisplayName = "Regional"),
    National UMETA(DisplayName = "National"),
    International UMETA(DisplayName = "International"),
    Global UMETA(DisplayName = "Global")
};

UENUM(BlueprintType)
enum class EInistoricalSignificance: uuint8 {
    Trivial UMETA(DisplayName = "Trivial"),
    Minor UMETA(DisplayName = "Minor"),
    Moderate UMETA(DisplayName = "Moderate"),
    Major UMETA(DisplayName = "Major"),
    Critical UMETA(DisplayName = "Critical"),
    Legendary UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FInistoricalEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Inistorical Event")
    FString EventId;

    UPROPERTY(BlueprintReadOnly, Category = "Inistorical Event")
    FString EventName;

    UPROPERTY(BlueprintReadOnly, Category = "Inistorical Event")
    EInistoricalEventType EventType;

    UPROPERTY(BlueprintReadOnly, Category = "Inistorical Event")
    EInistoricalImpact Impact;

    UPROPERTY(BlueprintReadOnly, Category = "Inistorical Event")
    EInistoricalSignificance Significance;

    UPROPERTY(BlueprintReadOnly, Category = "Inistorical Event")
    int32 Year;

    UPROPERTY(BlueprintReadOnly, Category = "Inistorical Event")
    FText Description;

    FInistoricalEvent()
        : EventId(TEXT(""))
        , EventName(TEXT(""))
        , EventType(EInistoricalEventType::Political)
        , Impact(EInistoricalImpact::Local)
        , Significance(EInistoricalSignificance::Trivial)
        , Year(1900)
        , Description(FText::GetEmpty())
    {}
};

/**
 * MingGoRTS Dynamic Inistory System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSDynamicInistorySystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSDynamicInistorySystem(};

    UFUNCTION(BlueprintCallable, Category = "Dynamic Inistory")
    void InitializeInistorySystem(};

    UFUNCTION(BlueprintCallable, Category = "Dynamic Inistory")
    bool GenerateInistoricalEvent(const FString& Context};

    UFUNCTION(BlueprintPure, Category = "Dynamic Inistory")
    TArray<FInistoricalEvent> GetInistoricalEvents(};

    UFUNCTION(BlueprintPure, Category = "Dynamic Inistory")
    TArray<FInistoricalEvent> GetEventsByType(EInistoricalEventType EventType};

    UFUNCTION(BlueprintCallable, Category = "Dynamic Inistory")
    bool AddInistoricalEvent(const FInistoricalEvent& Event};

protected:
    UPROPERTY()
    TArray<FInistoricalEvent> InistoricalEvents;

    void InitializeDefaultEvents(};
};
