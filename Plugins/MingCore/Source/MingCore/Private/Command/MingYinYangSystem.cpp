#include "MingYinYangSystem.h"
#include "Engine/World.h"

UMingYinYangSystem::UMingYinYangSystem()
{
    YinYangThreshold = 70.0f;
    BalanceThreshold = 20.0f;
    StabilityThreshold = 60.0f;
}

void UMingYinYangSystem::InitializeYinYangSystem()
{
    UE_LOG(LogTemp, Log, TEXT("初始化陰陽系統..."));

    // 初始化陰陽平衡
    CurrentBalance = FYinYangBalance();
    CurrentBalance.YinLevel = 50.0f;
    CurrentBalance.YangLevel = 50.0f;
    CurrentBalance.BalanceRatio = 1.0f;
    CurrentBalance.bIsBalanced = true;
    CurrentBalance.StabilityIndex = 100.0f;

    UE_LOG(LogTemp, Log, TEXT("陰陽系統初始化完成"));
}

FYinYangSituation UMingYinYangSystem::AnalyzeYinYangSituation()
{
    FYinYangSituation Situation;

    // 分析透明度（陽性屬性）
    Situation.TransparencyLevel = CurrentBalance.YangLevel;
    
    // 分析混亂度（陰性屬性）
    Situation.ChaosLevel = 100.0f - CurrentBalance.StabilityIndex;
    
    // 分析緊急度（陰性屬性）
    Situation.UrgencyLevel = FMath::Max(0.0f, 100.0f - CurrentBalance.YangLevel);
    
    // 分析穩定性（陽性屬性）
    Situation.StabilityLevel = CurrentBalance.StabilityIndex;

    // 判斷是否需要陰性或陽性方法
    Situation.bRequiresYinApproach = RequiresYinApproach(Situation);
    Situation.bRequiresYangApproach = !Situation.bRequiresYinApproach;

    // 確定主導屬性
    Situation.DominantAttributes.Empty();
    
    if (Situation.TransparencyLevel > YinYangThreshold)
    {
        Situation.DominantAttributes.Add(EYinYangAttribute::Visible);
        Situation.DominantAttributes.Add(EYinYangAttribute::Public);
    }
    else
    {
        Situation.DominantAttributes.Add(EYinYangAttribute::Invisible);
        Situation.DominantAttributes.Add(EYinYangAttribute::Hidden);
    }

    if (Situation.ChaosLevel > YinYangThreshold)
    {
        Situation.DominantAttributes.Add(EYinYangAttribute::Chaos);
    }
    else
    {
        Situation.DominantAttributes.Add(EYinYangAttribute::Order);
    }

    if (Situation.UrgencyLevel > YinYangThreshold)
    {
        Situation.DominantAttributes.Add(EYinYangAttribute::Urgent);
    }
    else
    {
        Situation.DominantAttributes.Add(EYinYangAttribute::Sustainable);
    }

    return Situation;
}

FYinYangBalance UMingYinYangSystem::CalculateBalance()
{
    // 計算平衡比率
    if (CurrentBalance.YangLevel > 0.0f)
    {
        CurrentBalance.BalanceRatio = CurrentBalance.YinLevel / CurrentBalance.YangLevel;
    }
    else
    {
        CurrentBalance.BalanceRatio = 0.0f;
    }

    // 判斷是否平衡
    float Difference = FMath::Abs(CurrentBalance.YinLevel - CurrentBalance.YangLevel);
    CurrentBalance.bIsBalanced = (Difference <= BalanceThreshold);

    // 計算穩定性指數
    CurrentBalance.StabilityIndex = CalculateStability();

    return CurrentBalance;
}

bool UMingYinYangSystem::ExecuteYinYangAction(const FYinYangAction& Action)
{
    // 檢查動作是否兼容當前局勢
    FYinYangSituation CurrentSituation = AnalyzeYinYangSituation();
    if (!IsActionCompatible(Action, CurrentSituation))
    {
        UE_LOG(LogTemp, Warning, TEXT("陰陽動作與當前局勢不兼容"));
        return false;
    }

    // 執行動作
    CurrentBalance.YinLevel += Action.YinImpact;
    CurrentBalance.YangLevel += Action.YangImpact;

    // 限制範圍
    CurrentBalance.YinLevel = FMath::Clamp(CurrentBalance.YinLevel, 0.0f, 100.0f);
    CurrentBalance.YangLevel = FMath::Clamp(CurrentBalance.YangLevel, 0.0f, 100.0f);

    // 更新平衡
    UpdateBalance();

    // 記錄動作
    RecordAction(Action);

    UE_LOG(LogTemp, Log, TEXT("執行陰陽動作：%s，陰影響：%.2f，陽影響：%.2f"), 
        *Action.Description, Action.YinImpact, Action.YangImpact);

    return true;
}

bool UMingYinYangSystem::CanMaintainPureYang(const FYinYangSituation& Situation)
{
    // 檢查是否可以保持純陽（全透明）
    // 純陽只在穩定、低混亂、低緊急的情況下可行
    if (Situation.StabilityLevel < StabilityThreshold)
    {
        return false;
    }

    if (Situation.ChaosLevel > YinYangThreshold)
    {
        return false;
    }

    if (Situation.UrgencyLevel > YinYangThreshold)
    {
        return false;
    }

    // 純陽會導致失去應變能力
    UE_LOG(LogTemp, Warning, TEXT("警告：純陽狀態缺乏應變能力，高級謊言風險"));
    return true;
}

bool UMingYinYangSystem::RequiresYinApproach(const FYinYangSituation& Situation)
{
    // 判斷是否需要陰性方法
    // 高混亂、高緊急、低穩定性時需要陰性方法
    if (Situation.ChaosLevel > YinYangThreshold)
    {
        return true;
    }

    if (Situation.UrgencyLevel > YinYangThreshold)
    {
        return true;
    }

    if (Situation.StabilityLevel < StabilityThreshold)
    {
        return true;
    }

    return false;
}

TArray<EYinYangAttribute> UMingYinYangSystem::GetRecommendedAttributes(const FYinYangSituation& Situation)
{
    TArray<EYinYangAttribute> Recommendations;

    // 根據局勢推薦陰陽屬性
    if (Situation.bRequiresYinApproach)
    {
        Recommendations.Add(EYinYangAttribute::Invisible);
        Recommendations.Add(EYinYangAttribute::Hidden);
        Recommendations.Add(EYinYangAttribute::Chaos);
        Recommendations.Add(EYinYangAttribute::Urgent);
    }
    else
    {
        Recommendations.Add(EYinYangAttribute::Visible);
        Recommendations.Add(EYinYangAttribute::Public);
        Recommendations.Add(EYinYangAttribute::Order);
        Recommendations.Add(EYinYangAttribute::Sustainable);
    }

    return Recommendations;
}

void UMingYinYangSystem::BalanceYinYang(float TargetYin, float TargetYang)
{
    // 逐步調整陰陽平衡
    float YinDelta = (TargetYin - CurrentBalance.YinLevel) * 0.1f;
    float YangDelta = (TargetYang - CurrentBalance.YangLevel) * 0.1f;

    CurrentBalance.YinLevel += YinDelta;
    CurrentBalance.YangLevel += YangDelta;

    // 限制範圍
    CurrentBalance.YinLevel = FMath::Clamp(CurrentBalance.YinLevel, 0.0f, 100.0f);
    CurrentBalance.YangLevel = FMath::Clamp(CurrentBalance.YangLevel, 0.0f, 100.0f);

    UpdateBalance();

    UE_LOG(LogTemp, Log, TEXT("陰陽平衡調整：陰=%.2f，陽=%.2f"), 
        CurrentBalance.YinLevel, CurrentBalance.YangLevel);
}

float UMingYinYangSystem::CalculateTransitionCost(EYinYangAttribute From, EYinYangAttribute To)
{
    // 計算陰陽屬性轉換成本
    // 陰陽之間轉換成本較高
    bool FromIsYin = (From == EYinYangAttribute::Invisible || 
                     From == EYinYangAttribute::Chaos || 
                     From == EYinYangAttribute::Hidden || 
                     From == EYinYangAttribute::Urgent);

    bool ToIsYin = (To == EYinYangAttribute::Invisible || 
                   To == EYinYangAttribute::Chaos || 
                   To == EYinYangAttribute::Hidden || 
                   To == EYinYangAttribute::Urgent);

    if (FromIsYin != ToIsYin)
    {
        return 50.0f; // 陰陽轉換成本高
    }

    return 10.0f; // 同屬性轉換成本低
}

bool UMingYinYangSystem::IsYinYangCoherent(const TArray<EYinYangAttribute>& Attributes)
{
    // 檢查陰陽屬性是否協調
    int32 YinCount = 0;
    int32 YangCount = 0;

    for (EYinYangAttribute Attribute : Attributes)
    {
        if (Attribute == EYinYangAttribute::Invisible || 
            Attribute == EYinYangAttribute::Chaos || 
            Attribute == EYinYangAttribute::Hidden || 
            Attribute == EYinYangAttribute::Urgent)
        {
            YinCount++;
        }
        else
        {
            YangCount++;
        }
    }

    // 完全偏向一方不協調
    if (YinCount == 0 || YangCount == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("陰陽屬性不協調：完全偏向一方"));
        return false;
    }

    // 比例失衡不協調
    float Ratio = (float)YinCount / (float)YangCount;
    if (Ratio > 3.0f || Ratio < 0.33f)
    {
        UE_LOG(LogTemp, Warning, TEXT("陰陽屬性不協調：比例失衡"));
        return false;
    }

    return true;
}

void UMingYinYangSystem::UpdateBalance()
{
    // 更新平衡狀態
    CalculateBalance();
}

float UMingYinYangSystem::CalculateStability()
{
    // 計算穩定性指數
    // 穩定性基於陰陽平衡程度
    float Difference = FMath::Abs(CurrentBalance.YinLevel - CurrentBalance.YangLevel);
    float Stability = 100.0f - (Difference * 2.0f);
    
    // 考慮歷史動作的影響
    for (const FYinYangAction& Action : ActionHistory)
    {
        if (!Action.bIsReversible)
        {
            Stability -= 5.0f; // 不可逆動作降低穩定性
        }
    }

    return FMath::Clamp(Stability, 0.0f, 100.0f);
}

bool UMingYinYangSystem::IsActionCompatible(const FYinYangAction& Action, const FYinYangSituation& Situation)
{
    // 檢查動作與局勢的兼容性
    if (Situation.bRequiresYinApproach)
    {
        // 需要陰性方法時，陰性動作更兼容
        return (Action.YinImpact > Action.YangImpact);
    }
    else
    {
        // 需要陽性方法時，陽性動作更兼容
        return (Action.YangImpact > Action.YinImpact);
    }
}

void UMingYinYangSystem::RecordAction(const FYinYangAction& Action)
{
    ActionHistory.Add(Action);

    // 限制歷史記錄數量
    if (ActionHistory.Num() > 100)
    {
        ActionHistory.RemoveAt(0);
    }
}
