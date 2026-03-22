#pragma once

#include "CoreMinimal.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerCondition.generated.h"

/**
 * Ê¢ù‰ª∂?ç‰?Á¨? */
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
 * ?èËºØ?ç‰?Á¨? */
UENUM(BlueprintType)
enum class ELogicOperator : uint8
{
    AND                 UMETA(DisplayName = "AND"),
    OR                  UMETA(DisplayName = "OR")
};

/**
 * ?ÆÂÄãÊ?‰ª∂Â?Áæ? */
USTRUCT(BlueprintType)
struct FSingleCondition
{
    GENERATED_BODY()
    
    // Ê¢ù‰ª∂?çÁ®± (?®ÊñºË™øË©¶)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionName;
    
    // ?ÆÊ?Â∞çË±° (Ë≥áÊ?È°ûÂ??ÅË??≤ID?ÅÈ??≤Á??ãÁ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetKey;
    
    // ?ç‰?Á¨?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EConditionOperator Operator;
    
    // ?ÆÊ??∏ÂÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetValue;
    
    // ?ØÂê¶?∫Â?Á¨¶‰∏≤ÊØîË?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bStringComparison;
    
    // Â≠óÁ¨¶‰∏≤ÁõÆÊ®ôÂÄ?(Â¶ÇÊ??®ÊñºÂ≠óÁ¨¶‰∏≤Ê?Ëº?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StringTargetValue;
    
    FSingleCondition()
        : Operator(EConditionOperator::Equal)
        , TargetValue(0.0f)
        , bStringComparison(false)
    {}
};

/**
 * Ê¢ù‰ª∂Áµ?(?ØÊ?Ë§áÂ?Ê¢ù‰ª∂)
 */
USTRUCT(BlueprintType)
struct FConditionGroup
{
    GENERATED_BODY()
    
    // ÁµÑÂÖßÊ¢ù‰ª∂?óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSingleCondition> Conditions;
    
    // ÁµÑÂÖß?èËºØ?ç‰?Á¨?(AND/OR)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ELogicOperator GroupOperator;
    
    FConditionGroup()
        : GroupOperator(ELogicOperator::AND)
    {}
};

/**
 * Ê¢ù‰ª∂Ëß∏Áôº?? * ?∫Êñº?äÊà≤?Ä?ãÊ?‰ª∂Ëß∏?º‰?‰ª? */
UCLASS()
class MINGSTRATEGIC_API UMingStrategicEventCondition : public UMingEventTrigger
{
    GENERATED_BODY()

public:
    UMingStrategicEventCondition();

    // Ê∑ªÂ??ÆÂÄãÊ?‰ª?    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void AddCondition(const FSingleCondition& Condition);

    // Ê∑ªÂ?Ê¢ù‰ª∂Áµ?    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void AddConditionGroup(const FConditionGroup& Group);

    // Ë®≠ÁΩÆ?®Â??èËºØ?ç‰?Á¨?    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void SetGlobalLogicOperator(ELogicOperator Operator);

    // ?¥Êñ∞Ê¢ù‰ª∂??(?±È??≤Á≥ªÁµ±Ë™ø??
    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void UpdateConditionValue(const FString& Key, float Value);

    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void UpdateConditionStringValue(const FString& Key, const FString& Value);

    // Ê∏ÖÈô§?Ä?âÊ?‰ª∂ÂÄ?    UFUNCTION(BlueprintCallable, Category = "Condition Trigger")
    void ClearConditionValues();

    // ?≤Â?Ê¢ù‰ª∂Ë©ï‰º∞ÁµêÊ? (?®ÊñºË™øË©¶)
    UFUNCTION(BlueprintPure, Category = "Condition Trigger")
    bool EvaluateCondition(const FSingleCondition& Condition) const;

protected:
    // Ê¢ù‰ª∂ÁµÑÂ?Ë°?    UPROPERTY()
    TArray<FConditionGroup> ConditionGroups;

    // ?®Â??èËºØ?ç‰?Á¨?(ÁµÑË?ÁµÑ‰???
    UPROPERTY()
    ELogicOperator GlobalOperator;

    // ?∂Â?Ê¢ù‰ª∂??(?∏ÂÄ?
    UPROPERTY()
    TMap<FString, float> NumericValues;

    // ?∂Â?Ê¢ù‰ª∂??(Â≠óÁ¨¶‰∏?
    UPROPERTY()
    TMap<FString, FString> StringValues;

    // ?çÂØ´?∫È??πÊ?
    virtual bool PerformTrigger() override;
    virtual bool CheckTriggerCondition() const override;

    // Ë©ï‰º∞Ê¢ù‰ª∂Áµ?    bool EvaluateConditionGroup(const FConditionGroup& Group) const;

    // Ë©ï‰º∞?ÆÂÄãÊ?‰ª?    bool EvaluateSingleCondition(const FSingleCondition& Condition) const;

    // ?∏ÂÄºÊ?Ëº?    bool CompareValues(float Value1, float Value2, EConditionOperator Op) const;

    // Â≠óÁ¨¶‰∏≤Ê?Ëº?    bool CompareStrings(const FString& Value1, const FString& Value2, EConditionOperator Op) const;
};
