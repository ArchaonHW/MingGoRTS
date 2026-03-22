#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LayerIntegration/MingFourLayerTypes.h"
#include "IMingGameLayer.generated.h"

// ?�戲層�?介面
UINTERFACE(MinimalAPI, BlueprintType)
class UMingGameLayer : public UInterface
{
    GENERATED_BODY()
};

/**
 * ?�戲層�?介面
 * ?�?��?層系統�X��?介面
 */
class MINGSTRATEGIC_API IMingGameLayer
{
    GENERATED_BODY()

public:
    // ?��X�層�?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void InitializeLayer();

    // ?�新層�?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void UpdateLayer(float DeltaTime};

    // ?��?層�?類�?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    EMingGameLayer GetLayerType() const;

    // ?��?層�X�稱
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    FString GetLayerName() const;

    // ?��?層�X��?�?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    float GetLayerPriority() const;

    // ?��?層�?事件
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void HandleLayerEvent(const FMingGameEvent& Event};

    // ?��?層�?決�?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void HandleLayerDecision(const FMingGameDecision& Decision};

    // ?��?層�?資�X��?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TMap<EMingResourceType, float> GetResourceRequirements() const;

    // ?��?層�?資�X��?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TMap<EMingResourceType, float> GetResourceContributions() const;

    // ?��?層�X�X    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    FString GetLayerStatus() const;

    // 檢查層�?衝�?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TArray<FMingLayerConflict> CheckLayerConflicts() const;

    // �?��層�?衝�?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void ResolveLayerConflicts(const TArray<FMingLayerConflict>& Conflicts};

    // ?�調?�其他層�?    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void CoordinateWithOtherLayers(const TArray<EMingGameLayer>& OtherLayers};

    // ?��?層�X�能統�?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    float GetLayerPerformance() const;

    // ?�置層�?
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void ResetLayer();
};

