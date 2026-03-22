#pragma once

#include "CoreMinimal.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerCondition.generated.h"

/**
 * 條件?��?�? */
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
 * ?�輯?��?�? */
UENUM(BlueprintType)
enum class ELogicOperator : uint8
{
    AND                 UMETA(DisplayName = "AND"),
    OR                  UMETA(DisplayName = "OR")
};

/**
 * ?�個�?件�?�? */
USTRUCT(BlueprintType)
struct FSingleCondition
{
    GENERATED_BODY()
    
    // 條件?�稱 (?�於調試)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionName;
    
    // ?��?對象 (資�?類�X��X�ID?��X��X��?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetKey;
    
    // ?��?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EConditionOperator Operator;
    
    // ?��X��?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetValue;
    
    // ?�否?��?符串比�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bStringComparison;
    
    // 字符串目標�?(如�X�於字符串�?�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StringTargetValue;
    
    FSingleCondition()
        : Operator(EConditionOperator::Equal)
        , TargetValue(0.0f)
        , bStringComparison(false)
    {}
};

/**
 * 條件�?(?��?複�?條件)
 */
USTRUCT(BlueprintType)
struct FConditionGroup
{
    GENERATED_BODY()
    
    // 組內條件?�表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSingleCondition> Conditions;
    
    // 組內?�輯?��?�?(AND/OR)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ELogicOperator GroupOperator;
    
    FConditionGroup()
        : GroupOperator(ELogicOperator::AND)
    {}
};

/**
 * 條件觸發X * ?�於?�戲?�?��?件觸?��?�? */
UCLASS()
class MINGSTRATEGIC_API UMingStrategicEventCondition : public UMingEventTrigger
{
    GENERATED_BODY()

public:
    UMingStrategicEventCondition(};

    // 添�X�個�?�?    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void AddCondition(const FSingleCondition& Condition};

    // 添�?條件�?    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void AddConditionGroup(const FConditionGroup& Group};

    // 設置?��X�輯?��?�?    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void SetGlobalLogicOperator(ELogicOperator Operator};

    // ?�新條件X(?��X�系統調X
    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void UpdateConditionValue(const FString& Key, float Value};

    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void UpdateConditionStringValue(const FString& Key, const FString& Value};

    // 清除?�?��?件�?    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void ClearConditionValues(};

    // ?��?條件評估結�? (?�於調試)
    UFUNCTION(BlueprintPure, Category = "Condition Trigger")
    bool EvaluateCondition(const FSingleCondition& Condition) const;

protected:
    // 條件組�?�?    UPROPERTY()
    TArray<FConditionGroup> ConditionGroups;

    // ?��X�輯?��?�?(組�?組�X
    UPROPERTY()
    ELogicOperator GlobalOperator;

    // ?��?條件X(?��?
    UPROPERTY()
    TMap<FString, float> NumericValues;

    // ?��?條件X(字符�?
    UPROPERTY()
    TMap<FString, FString> StringValues;

    // ?�寫?��X��?
    virtual bool PerformTrigger() override;
    virtual bool CheckTriggerCondition() const override;

    // 評估條件�?    bool EvaluateConditionGroup(const FConditionGroup& Group) const;

    // 評估?�個�?�?    bool EvaluateSingleCondition(const FSingleCondition& Condition) const;

    // ?�值�?�?    bool CompareValues(float Value1, float Value2, EConditionOperator Op) const;

    // 字符串�?�?    bool CompareStrings(const FString& Value1, const FString& Value2, EConditionOperator Op) const;
};

