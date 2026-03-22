#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LayerIntegration/MingFourLayerTypes.h"
#include "IMingGameLayer.generated.h"

// ?äÊà≤Â±§Á?‰ªãÈù¢
UINTERFACE(MinimalAPI, BlueprintType)
class UMingGameLayer : public UInterface
{
    GENERATED_BODY()
};

/**
 * ?äÊà≤Â±§Á?‰ªãÈù¢
 * ?Ä?âÂ?Â±§Á≥ªÁµ±Á??∫Á?‰ªãÈù¢
 */
class MINGSTRATEGIC_API IMingGameLayer
{
    GENERATED_BODY()

public:
    // ?ùÂ??ñÂ±§Á¥?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void InitializeLayer();

    // ?¥Êñ∞Â±§Á?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void UpdateLayer(float DeltaTime);

    // ?≤Â?Â±§Á?È°ûÂ?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    EMingGameLayer GetLayerType() const;

    // ?≤Â?Â±§Á??çÁ®±
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    FString GetLayerName() const;

    // ?≤Â?Â±§Á??™Â?Á¥?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    float GetLayerPriority() const;

    // ?ïÁ?Â±§Á?‰∫ã‰ª∂
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void HandleLayerEvent(const FMingGameEvent& Event);

    // ?ïÁ?Â±§Á?Ê±∫Á?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void HandleLayerDecision(const FMingGameDecision& Decision);

    // ?≤Â?Â±§Á?Ë≥áÊ??ÄÊ±?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TMap<EMingResourceType, float> GetResourceRequirements() const;

    // ?≤Â?Â±§Á?Ë≥áÊ??ê‰?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TMap<EMingResourceType, float> GetResourceContributions() const;

    // ?≤Â?Â±§Á??Ä??    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    FString GetLayerStatus() const;

    // Ê™¢Êü•Â±§Á?Ë°ùÁ?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TArray<FMingLayerConflict> CheckLayerConflicts() const;

    // Ëß?±∫Â±§Á?Ë°ùÁ?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void ResolveLayerConflicts(const TArray<FMingLayerConflict>& Conflicts);

    // ?îË™ø?áÂÖ∂‰ªñÂ±§Á¥?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void CoordinateWithOtherLayers(const TArray<EMingGameLayer>& OtherLayers);

    // ?≤Â?Â±§Á??ßËÉΩÁµ±Ë?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    float GetLayerPerformance() const;

    // ?çÁΩÆÂ±§Á?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void ResetLayer();
};
