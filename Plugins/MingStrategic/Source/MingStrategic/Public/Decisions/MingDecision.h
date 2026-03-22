#pragma once

#include "CoreMinimal.h"
#include "MingDecisionOption.h"
#include "MingDecision.generated.h"

/**
 * 決策觸發類型
 */
UENUM(BlueprintType)
enum class EDecisionTriggerType : uint8
{
    Manual              UMETA(DisplayName = "Manual"),
    TimeBased           UMETA(DisplayName = "Time Based"),
    EventBased          UMETA(DisplayName = "Event Based"),
    ConditionBased      UMETA(DisplayName = "Condition Based"),
    LocationBased       UMETA(DisplayName = "Location Based")
};

/**
 * 決策優先級
 */
UENUM(BlueprintType)
enum class EDecisionPriority : uint8
{
    Low                 UMETA(DisplayName = "Low"),
    Normal              UMETA(DisplayName = "Normal"),
    High                UMETA(DisplayName = "High"),
    Critical            UMETA(DisplayName = "Critical")
};

/**
 * 決策數據結構
 * 定義一個完整的歷史決策
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingDecision
{
    GENERATED_BODY()
    
    // 決策ID (唯一標識)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString DecisionId;
    
    // 決策標題
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FText DecisionTitle;
    
    // 決策背景描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FText BackgroundDescription;
    
    // 決策場景圖片
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    UTexture2D* BackgroundImage;
    
    // 決策選項列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    TArray<FMingDecisionOption> Options;
    
    // 觸發類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    EDecisionTriggerType TriggerType;
    
    // 優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    EDecisionPriority Priority;
    
    // 觸發條件 (JSON格式或自定義字符串)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString TriggerCondition;
    
    // 過期時間 (遊戲內時間，-1表示不過期)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    float ExpirationTime;
    
    // 是否可以延後處理
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    bool bCanPostpone;
    
    // 決策類別 (用於分類顯示)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString DecisionCategory;
    
    // 歷史時期 (如：1924-1927 北伐時期)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString HistoricalPeriod;
    
    // 相關角色ID列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    TArray<FString> RelatedCharacters;
    
    // 相關地點
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString RelatedLocation;
    
    FMingDecision()
        : BackgroundImage(nullptr)
        , TriggerType(EDecisionTriggerType::Manual)
        , Priority(EDecisionPriority::Normal)
        , ExpirationTime(-1.0f)
        , bCanPostpone(true)
    {}
    
    // 檢查決策是否過期
    bool IsExpired(float CurrentGameTime) const;
    
    // 獲取可用選項列表
    TArray<FMingDecisionOption> GetAvailableOptions() const;
};
