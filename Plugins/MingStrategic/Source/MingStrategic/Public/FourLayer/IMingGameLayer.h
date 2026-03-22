#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingFourLayerTypes.h"
#include "IMingGameLayer.generated.h"

// 遊戲層級介面
UINTERFACE(MinimalAPI, BlueprintType)
class UMingGameLayer : public UInterface
{
    GENERATED_BODY()
};

/**
 * 遊戲層級介面
 * 所有四層系統的基礎介面
 */
class MINGSTRATEGIC_API IMingGameLayer
{
    GENERATED_BODY()

public:
    // 初始化層級
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void InitializeLayer();

    // 更新層級
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void UpdateLayer(float DeltaTime);

    // 獲取層級類型
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    EMingGameLayer GetLayerType() const;

    // 獲取層級名稱
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    FString GetLayerName() const;

    // 獲取層級優先級
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    float GetLayerPriority() const;

    // 處理層級事件
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void HandleLayerEvent(const FMingGameEvent& Event);

    // 處理層級決策
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void HandleLayerDecision(const FMingGameDecision& Decision);

    // 獲取層級資源需求
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TMap<EMingResourceType, float> GetResourceRequirements() const;

    // 獲取層級資源提供
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TMap<EMingResourceType, float> GetResourceContributions() const;

    // 獲取層級狀態
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    FString GetLayerStatus() const;

    // 檢查層級衝突
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    TArray<FMingLayerConflict> CheckLayerConflicts() const;

    // 解決層級衝突
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void ResolveLayerConflicts(const TArray<FMingLayerConflict>& Conflicts);

    // 協調與其他層級
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void CoordinateWithOtherLayers(const TArray<EMingGameLayer>& OtherLayers);

    // 獲取層級性能統計
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    float GetLayerPerformance() const;

    // 重置層級
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Layer")
    void ResetLayer();
};
