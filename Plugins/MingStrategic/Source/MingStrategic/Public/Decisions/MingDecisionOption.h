#pragma once

#include "CoreMinimal.h"
#include "MingDecisionEffect.h"
#include "MingStrategicDecisionOption.generated.h"

/**
 * 決策選項結構
 * 定義決策的一個選項及其後果
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicDecisionOption
{
    GENERATED_BODY()
    
    // 選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString OptionId;
    
    // 選項標題
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionTitle;
    
    // 選項描述 (詳細說明)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionDescription;
    
    // 選項預覽 (簡短說明顯示在按鈕上)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionPreview;
    
    // 選項圖標
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    UTexture2D* OptionIcon;
    
    // 選項效果列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    TArray<FMingDecisionEffect> Effects;
    
    // 後續決策ID (選擇此選項後觸發的下一個決策)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString NextDecisionId;
    
    // 是否結束當前決策鏈
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    bool bEndDecisionChain;
    
    // 選項可用性條件 (可留空表示始終可用)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString AvailabilityCondition;
    
    FMingDecisionOption()
        : OptionIcon(nullptr)
        , bEndDecisionChain(false)
    {}
    
    // 檢查選項是否可用
    bool IsAvailable() const;
};
