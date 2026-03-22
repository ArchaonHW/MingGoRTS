#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCulturalAdaptationSystem.generated.h"

UENUM(BlueprintType)
enum class ECulturalRegion : uint8
{
    EastAsia, UMETA(DisplayName = "East Asia"),
    SoutheastAsia, UMETA(DisplayName = "Southeast Asia"),
    SouthAsia, UMETA(DisplayName = "South Asia"),
    MiddleEast, UMETA(DisplayName = "Middle East"),
    WesternEurope, UMETA(DisplayName = "Western Europe"),
    EasternEurope, UMETA(DisplayName = "Eastern Europe"),
    NorthAmerica, UMETA(DisplayName = "North America"),
    LatinAmerica, UMETA(DisplayName = "Latin America"),
    Oceania, UMETA(DisplayName = "Oceania"),
    Africa, UMETA(DisplayName = "Africa"),
    Global, UMETA(DisplayName = "Global")
};

UENUM(BlueprintType)
enum class EContentVariant : uint8
{
    Default, UMETA(DisplayName = "Default"),
    Regional, UMETA(DisplayName = "Regional"),
    Cultural, UMETA(DisplayName = "Cultural"),
    Religious, UMETA(DisplayName = "Religious"),
    Historical, UMETA(DisplayName = "Historical"),
    Custom, UMETA(DisplayName = "Custom")
};

/**
 * MingGoRTS Cultural Adaptation System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCulturalAdaptationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCulturalAdaptationSystem();

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void InitializeCulturalSystem();

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void SetPlayerRegion(ECulturalRegion Region);

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    ECulturalRegion GetCurrentRegion() const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    FString GetAdaptedContent(const FString& ContentKey, EContentVariant Variant);

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    bool ApplyRegionalGameplaySettings();

protected:
    UPROPERTY()
    ECulturalRegion CurrentRegion;

    UPROPERTY()
    TMap<FString, FString> AdaptedContent;

    void InitializeRegionalContent();
    FString AdaptContentForRegion(const FString& Content, ECulturalRegion Region);
};
