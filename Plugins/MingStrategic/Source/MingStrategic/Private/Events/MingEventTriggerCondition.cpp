#include "Events/MingEventTriggerCondition.h"

UMingEventTriggerCondition::UMingEventTriggerCondition()
    : GlobalOperator(ELogicOperator::AND)
{
    TriggerType = EEventTriggerType::Condition;
}

void UMingEventTriggerCondition::AddCondition(const FSingleCondition& Condition)
{
    // 創建一個只包含單個條件的組
    FConditionGroup Group;
    Group.Conditions.Add(Condition);
    Group.GroupOperator = ELogicOperator::AND;
    
    ConditionGroups.Add(Group);
    
    UE_LOG(LogTemp, Verbose, TEXT("ConditionTrigger %s added condition: %s"),
        *TriggerId, *Condition.ConditionName);
}

void UMingEventTriggerCondition::AddConditionGroup(const FConditionGroup& Group)
{
    ConditionGroups.Add(Group);
    
    UE_LOG(LogTemp, Verbose, TEXT("ConditionTrigger %s added condition group with %d conditions"),
        *TriggerId, Group.Conditions.Num());
}

void UMingEventTriggerCondition::SetGlobalLogicOperator(ELogicOperator Operator)
{
    GlobalOperator = Operator;
}

void UMingEventTriggerCondition::UpdateConditionValue(const FString& Key, float Value)
{
    NumericValues.Add(Key, Value);
    
    UE_LOG(LogTemp, Verbose, TEXT("ConditionTrigger %s updated numeric value: %s = %f"),
        *TriggerId, *Key, Value);
}

void UMingEventTriggerCondition::UpdateConditionStringValue(const FString& Key, const FString& Value)
{
    StringValues.Add(Key, Value);
    
    UE_LOG(LogTemp, Verbose, TEXT("ConditionTrigger %s updated string value: %s = %s"),
        *TriggerId, *Key, *Value);
}

void UMingEventTriggerCondition::ClearConditionValues()
{
    NumericValues.Empty();
    StringValues.Empty();
}

bool UMingEventTriggerCondition::EvaluateCondition(const FSingleCondition& Condition) const
{
    return EvaluateSingleCondition(Condition);
}

bool UMingEventTriggerCondition::PerformTrigger()
{
    // 條件觸發器的觸發邏輯在 CheckTriggerCondition 中處理
    return true;
}

bool UMingEventTriggerCondition::CheckTriggerCondition() const
{
    if (ConditionGroups.Num() == 0)
    {
        return false;
    }
    
    // 評估所有條件組
    if (GlobalOperator == ELogicOperator::AND)
    {
        // 所有組都必須為真
        for (const auto& Group : ConditionGroups)
        {
            if (!EvaluateConditionGroup(Group))
            {
                return false;
            }
        }
        return true;
    }
    else // OR
    {
        // 至少一個組為真
        for (const auto& Group : ConditionGroups)
        {
            if (EvaluateConditionGroup(Group))
            {
                return true;
            }
        }
        return false;
    }
}

bool UMingEventTriggerCondition::EvaluateConditionGroup(const FConditionGroup& Group) const
{
    if (Group.Conditions.Num() == 0)
    {
        return true;
    }
    
    if (Group.GroupOperator == ELogicOperator::AND)
    {
        // 組內所有條件都必須為真
        for (const auto& Condition : Group.Conditions)
        {
            if (!EvaluateSingleCondition(Condition))
            {
                return false;
            }
        }
        return true;
    }
    else // OR
    {
        // 組內至少一個條件為真
        for (const auto& Condition : Group.Conditions)
        {
            if (EvaluateSingleCondition(Condition))
            {
                return true;
            }
        }
        return false;
    }
}

bool UMingEventTriggerCondition::EvaluateSingleCondition(const FSingleCondition& Condition) const
{
    if (Condition.bStringComparison)
    {
        // 字符串比較
        FString* CurrentValue = StringValues.Find(Condition.TargetKey);
        if (CurrentValue)
        {
            return CompareStrings(*CurrentValue, Condition.StringTargetValue, Condition.Operator);
        }
        else
        {
            // 如果沒有找到值，視為空字符串
            return CompareStrings(FString(), Condition.StringTargetValue, Condition.Operator);
        }
    }
    else
    {
        // 數值比較
        float* CurrentValue = NumericValues.Find(Condition.TargetKey);
        float ValueToCompare = CurrentValue ? *CurrentValue : 0.0f;
        
        return CompareValues(ValueToCompare, Condition.TargetValue, Condition.Operator);
    }
}

bool UMingEventTriggerCondition::CompareValues(float Value1, float Value2, EConditionOperator Op) const
{
    switch (Op)
    {
    case EConditionOperator::Equal:
        return FMath::IsNearlyEqual(Value1, Value2, KINDA_SMALL_NUMBER);
        
    case EConditionOperator::NotEqual:
        return !FMath::IsNearlyEqual(Value1, Value2, KINDA_SMALL_NUMBER);
        
    case EConditionOperator::Greater:
        return Value1 > Value2;
        
    case EConditionOperator::GreaterEqual:
        return Value1 >= Value2;
        
    case EConditionOperator::Less:
        return Value1 < Value2;
        
    case EConditionOperator::LessEqual:
        return Value1 <= Value2;
        
    default:
        return false;
    }
}

bool UMingEventTriggerCondition::CompareStrings(const FString& Value1, const FString& Value2, EConditionOperator Op) const
{
    switch (Op)
    {
    case EConditionOperator::Equal:
        return Value1.Equals(Value2, ESearchCase::IgnoreCase);
        
    case EConditionOperator::NotEqual:
        return !Value1.Equals(Value2, ESearchCase::IgnoreCase);
        
    case EConditionOperator::Greater:
        return Value1.Compare(Value2, ESearchCase::IgnoreCase) > 0;
        
    case EConditionOperator::GreaterEqual:
        return Value1.Compare(Value2, ESearchCase::IgnoreCase) >= 0;
        
    case EConditionOperator::Less:
        return Value1.Compare(Value2, ESearchCase::IgnoreCase) < 0;
        
    case EConditionOperator::LessEqual:
        return Value1.Compare(Value2, ESearchCase::IgnoreCase) <= 0;
        
    default:
        return false;
    }
}
