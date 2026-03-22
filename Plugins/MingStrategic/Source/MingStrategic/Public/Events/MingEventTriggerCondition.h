#pragma once

#include "CoreMinimal.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerCondition.generated.h"

/**
 * 條件操作符
 */
UENUM(BlueprintType)
enum class EConditionOperator : uint8
{
    Equal               UMETA(DisplayName = "=="),
    NotEqual            UMETA(DisplayName = "!="),
    Greater             UMETA(DisplayName = ">"),
    GreaterEqual        UMETA(DisplayName = ">="),
    Less                UMETA(DisplayName = "<"),
    LessEqual           UMETA(DisplayName = "<=")
};

/**
 * 邏輯操作符
 */
UENUM(BlueprintType)
enum class ELogicOperator : uint8
{
    AND                 UMETA(DisplayName = "AND"),
    OR                  UMETA(DisplayName = "OR")
};

/**
 * 單個條件定義
 */
USTRUCT(BlueprintType)
struct FSingleCondition
{
    GENERATED_BODY()
    
    // 條件名稱 (用於調試)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionName;
    
    // 目標對象 (資源類型、角色ID、遊戲狀態等)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetKey;
    
    // 操作符
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EConditionOperator Operator;
    
    // 目標數值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetValue;
    
    // 是否為字符串比較
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bStringComparison;
    
    // 字符串目標值 (如果用於字符串比較)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StringTargetValue;
    
    FSingleCondition()
        : Operator(EConditionOperator::Equal)
        , TargetValue(0.0f)
        , bStringComparison(false)
    {}
};

/**
 * 條件組 (支持複合條件)
 */
USTRUCT(BlueprintType)
struct FConditionGroup
{
    GENERATED_BODY()
    
    // 組內條件列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSingleCondition> Conditions;
    
    // 組內邏輯操作符 (AND/OR)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ELogicOperator GroupOperator;
    
    FConditionGroup()
        : GroupOperator(ELogicOperator::AND)
    {}
};

/**
 * 條件觸發器
 * 基於遊戲狀態條件觸發事件
 */
UCLASS()
class MINGSTRATEGIC_API UMingStrategicEventCondition : public UMingEventTrigger
{
    GENERATED_BODY()

public:
    UMingStrategicEventCondition();

    // 添加單個條件
    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void AddCondition(const FSingleCondition& Condition);

    // 添加條件組
    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void AddConditionGroup(const FConditionGroup& Group);

    // 設置全局邏輯操作符
    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void SetGlobalLogicOperator(ELogicOperator Operator);

    // 更新條件值 (由遊戲系統調用)
    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void UpdateConditionValue(const FString& Key, float Value);

    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void UpdateConditionStringValue(const FString& Key, const FString& Value);

    // 清除所有條件值
    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void ClearConditionValues();

    // 獲取條件評估結果 (用於調試)
    UFUNCTION(BlueprintPure, Category = "Condition Trigger")
    bool EvaluateCondition(const FSingleCondition& Condition) const;

protected:
    // 條件組列表
    UPROPERTY()
    TArray<FConditionGroup> ConditionGroups;

    // 全局邏輯操作符 (組與組之間)
    UPROPERTY()
    ELogicOperator GlobalOperator;

    // 當前條件值 (數值)
    UPROPERTY()
    TMap<FString, float> NumericValues;

    // 當前條件值 (字符串)
    UPROPERTY()
    TMap<FString, FString> StringValues;

    // 重寫基類方法
    virtual bool PerformTrigger() override;
    virtual bool CheckTriggerCondition() const override;

    // 評估條件組
    bool EvaluateConditionGroup(const FConditionGroup& Group) const;

    // 評估單個條件
    bool EvaluateSingleCondition(const FSingleCondition& Condition) const;

    // 數值比較
    bool CompareValues(float Value1, float Value2, EConditionOperator Op) const;

    // 字符串比較
    bool CompareStrings(const FString& Value1, const FString& Value2, EConditionOperator Op) const;
};
