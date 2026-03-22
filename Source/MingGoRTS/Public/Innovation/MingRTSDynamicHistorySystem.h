#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicHistorySystem.generated.h"

UENUM(BlueprintType)
enum class EHistoricalEventType : uint8
{
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
enum class EHistoricalImpact : uint8
{
    Local UMETA(DisplayName = "Local"),
    Regional UMETA(DisplayName = "Regional"),
    National UMETA(DisplayName = "National"),
    International UMETA(DisplayName = "International"),
    Global UMETA(DisplayName = "Global")
};

UENUM(BlueprintType)
enum class EHistoricalSignificance : uint8
{
    Trivial UMETA(DisplayName = "Trivial"),
    Minor UMETA(DisplayName = "Minor"),
    Moderate UMETA(DisplayName = "Moderate"),
    Major UMETA(DisplayName = "Major"),
    Critical UMETA(DisplayName = "Critical"),
    Legendary UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FHistoricalEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Historical Event")
    FString EventId;

    UPROPERTY(BlueprintReadOnly, Category = "Historical Event")
    FString EventName;

    UPROPERTY(BlueprintReadOnly, Category = "Historical Event")
    EHistoricalEventType EventType;

    UPROPERTY(BlueprintReadOnly, Category = "Historical Event")
    EHistoricalImpact Impact;

    UPROPERTY(BlueprintReadOnly, Category = "Historical Event")
    EHistoricalSignificance Significance;

    UPROPERTY(BlueprintReadOnly, Category = "Historical Event")
    int32 Year;

    UPROPERTY(BlueprintReadOnly, Category = "Historical Event")
    FText Description;

    FHistoricalEvent()
        : EventId(TEXT(""))
        , EventName(TEXT(""))
        , EventType(EHistoricalEventType::Political)
        , Impact(EHistoricalImpact::Local)
        , Significance(EHistoricalSignificance::Trivial)
        , Year(1900)
        , Description(FText::GetEmpty())
    {}
};

/**
 * MingGoRTS Dynamic History System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSDynamicHistorySystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSDynamicHistorySystem();

    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void InitializeHistorySystem();

    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    bool GenerateHistoricalEvent(const FString& Context);

    UFUNCTION(BlueprintPure, Category = "Dynamic History")
    TArray<FHistoricalEvent> GetHistoricalEvents();

    UFUNCTION(BlueprintPure, Category = "Dynamic History")
    TArray<FHistoricalEvent> GetEventsByType(EHistoricalEventType EventType);

    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    bool AddHistoricalEvent(const FHistoricalEvent& Event);

protected:
    UPROPERTY()
    TArray<FHistoricalEvent> HistoricalEvents;

    void InitializeDefaultEvents();
};
