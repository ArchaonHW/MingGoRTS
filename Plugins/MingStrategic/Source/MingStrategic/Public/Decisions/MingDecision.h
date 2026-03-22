#pragma once

#include "CoreMinimal.h"
#include "MingDecisionOption.h"
#include "MingDecision.generated.h"

/**
 * 決�?觸發類�?
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
 * 決�X��?�? */
UENUM(BlueprintType)
enum class EDecisionPriority : uint8
{
    Low                 UMETA(DisplayName = "Low"),
    Normal              UMETA(DisplayName = "Normal"),
    High                UMETA(DisplayName = "High"),
    Critical            UMETA(DisplayName = "Critical")
};

/**
 * 決�X��?結�?
 * 定義一?��X��?歷史決�?
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingDecision
{
    GENERATED_BODY()
    
    // 決�?ID (?��?標�?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString DecisionId;
    
    // 決�?標�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FText DecisionTitle;
    
    // 決�X�景?�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FText BackgroundDescription;
    
    // 決�X�景?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    UTexture2D* BackgroundImage;
    
    // 決�X��X�表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    TArray<FMingDecisionOption> Options;
    
    // 觸發類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    EDecisionTriggerType TriggerType;
    
    // ?��?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    EDecisionPriority Priority;
    
    // 觸發條件 (JSON?��X�自定義字符�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString TriggerCondition;
    
    // ?��X��? (?�戲?��X��?-1表示不�X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    float ExpirationTime;
    
    // ?�否?�以延�X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    bool bCanPostpone;
    
    // 決�?類別 (?�於?��?顯示)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString DecisionCategory;
    
    // 歷史?��? (如�?1924-1927 ?��X��?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString HistoricalPeriod;
    
    // ?��?角色ID?�表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    TArray<FString> RelatedCharacters;
    
    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString RelatedLocation;
    
    FMingDecision()
        : BackgroundImage(nullptr)
        , TriggerType(EDecisionTriggerType::Manual)
        , Priority(EDecisionPriority::Normal)
        , ExpirationTime(-1.0f)
        , bCanPostpone(true)
    {}
    
    // 檢查決�X�否?��?
    bool IsExpired(float CurrentGameTime) const;
    
    // ?��X�用?��X�表
    TArray<FMingDecisionOption> GetAvailableOptions() const;
};

