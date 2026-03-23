#include "SageBrain/MingMoralAuthority.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingMoralAuthority::UMingMoralAuthority()
{
    bIsInitialized = false;
    MaxMoralAuthority = 100.0f;
    BaseInfluenceRadius = 100.0f;
    MoralRestoreRate = 1.0f;
    PublicTrustInfluenceFactor = 1.0f;
    MoralBalanceLevel = 50.0f;
    MaxFollowers = 1000;
    
    // 初始化道德行動
    InitializeMoralActions();
}

void UMingMoralAuthority::InitializeMoralAuthority()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Moral Authority already initialized"));
        return;
    }

    // 初始化道德狀態
    CurrentMoralStatus = FMoralStatus();
    CurrentMoralStatus.MoralAuthority = 50.0f;
    CurrentMoralStatus.RighteousPower = 50.0f;
    CurrentMoralStatus.InfluenceRadius = BaseInfluenceRadius;
    CurrentMoralStatus.MoralAura = 1.0f;
    CurrentMoralStatus.PublicTrust = 50.0f;
    
    bIsInitialized = true;
    
    // 觸發初始化事件
    OnMoralAuthorityInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Moral Authority initialized successfully"));
}

void UMingMoralAuthority::SetMoralAuthority(float Authority)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Moral Authority not initialized"));
        return;
    }

    float OldAuthority = CurrentMoralStatus.MoralAuthority;
    CurrentMoralStatus.MoralAuthority = FMath::Clamp(Authority, 0.0f, MaxMoralAuthority);
    
    // 更新道德光環
    UpdateMoralAura();
    
    // 觸發權威更改事件
    OnMoralAuthorityChanged.Broadcast(CurrentMoralStatus.MoralAuthority);
    
    UE_LOG(LogTemp, Log, TEXT("Moral Authority changed from %.2f to %.2f"), 
           OldAuthority, CurrentMoralStatus.MoralAuthority);
}

bool UMingMoralAuthority::ApplyMoralInfluence(EMoralInfluenceType InfluenceType, float Intensity, const FString& Target)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Moral Authority not initialized"));
        return false;
    }

    // 檢查道德權威是否足夠
    float RequiredAuthority = Intensity * 10.0f;
    if (CurrentMoralStatus.MoralAuthority < RequiredAuthority)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient moral authority for influence"));
        return false;
    }

    // 計算影響效果
    float Effect = CalculateInfluenceEffect(InfluenceType, Intensity);
    
    // 消耗道德權威
    CurrentMoralStatus.MoralAuthority -= RequiredAuthority;
    
    // 應用影響
    ActiveInfluences.Add(InfluenceType, Effect);
    
    // 更新相關狀態
    switch (InfluenceType)
    {
        case EMoralInfluenceType::Inspiration:
            CurrentMoralStatus.FollowerCount += FMath::RoundToInt(Effect * 5);
            break;
        case EMoralInfluenceType::Judgment:
            CurrentMoralStatus.RighteousPower += Effect * 2;
            break;
        case EMoralInfluenceType::Guidance:
            CurrentMoralStatus.PublicTrust += Effect;
            break;
        case EMoralInfluenceType::Purification:
            CurrentMoralStatus.MoralAura += Effect * 0.1f;
            break;
        case EMoralInfluenceType::Blessing:
            CurrentMoralStatus.InfluenceRadius += Effect * 10;
            break;
        default:
            break;
    }
    
    // 更新道德光環
    UpdateMoralAura();
    
    // 觸發影響應用事件
    OnMoralInfluenceApplied.Broadcast(InfluenceType, Effect);
    OnMoralInfluenceStarted.Broadcast(InfluenceType, Target);
    
    UE_LOG(LogTemp, Log, TEXT("Applied moral influence %s to %s with effect %.2f"), 
           *UEnum::GetValueAsString(InfluenceType), *Target, Effect);
    
    return true;
}

bool UMingMoralAuthority::ExecuteMoralAction(const FString& ActionName)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Moral Authority not initialized"));
        return false;
    }

    if (!MoralActionsMap.Contains(ActionName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Moral action not found: %s"), *ActionName);
        return false;
    }

    FMoralAction Action = MoralActionsMap[ActionName];
    
    // 檢查行動可行性
    if (!CanExecuteMoralAction(ActionName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot execute moral action: %s"), *ActionName);
        OnMoralActionCompleted.Broadcast(ActionName, false);
        return false;
    }

    // 檢查行動條件
    if (!CheckActionConditions(Action))
    {
        UE_LOG(LogTemp, Warning, TEXT("Action conditions not met: %s"), *ActionName);
        OnMoralActionCompleted.Broadcast(ActionName, false);
        return false;
    }

    // 消耗道德權威
    if (CurrentMoralStatus.MoralAuthority < Action.MoralCost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient moral authority for action: %s"), *ActionName);
        OnMoralActionCompleted.Broadcast(ActionName, false);
        return false;
    }

    CurrentMoralStatus.MoralAuthority -= Action.MoralCost;
    
    // 應用行動效果
    ApplyActionEffects(Action);
    
    // 記錄行動
    RecordMoralAction(Action);
    
    // 觸發完成事件
    OnMoralActionCompleted.Broadcast(ActionName, true);
    
    UE_LOG(LogTemp, Log, TEXT("Executed moral action: %s"), *ActionName);
    return true;
}

TArray<FMoralAction> UMingMoralAuthority::GetAvailableMoralActions() const
{
    TArray<FMoralAction> AvailableActions;
    
    for (const auto& ActionPair : MoralActionsMap)
    {
        const FMoralAction& Action = ActionPair.Value;
        if (CanExecuteMoralAction(Action.ActionName) && CheckActionConditions(Action))
        {
            AvailableActions.Add(Action);
        }
    }
    
    return AvailableActions;
}

bool UMingMoralAuthority::InspireFollowers(const FString& Message, float InfluenceRadius)
{
    return ApplyMoralInfluence(EMoralInfluenceType::Inspiration, InfluenceRadius / 10.0f, Message);
}

bool UMingMoralAuthority::ExecuteMoralJudgment(const FString& Target, float JudgmentPower)
{
    return ApplyMoralInfluence(EMoralInfluenceType::Judgment, JudgmentPower, Target);
}

bool UMingMoralAuthority::GuidePublicOpinion(const FString& Direction, float GuidanceStrength)
{
    return ApplyMoralInfluence(EMoralInfluenceType::Guidance, GuidanceStrength, Direction);
}

bool UMingMoralAuthority::PurifyEvil(const FString& Target, float PurificationPower)
{
    return ApplyMoralInfluence(EMoralInfluenceType::Purification, PurificationPower, Target);
}

bool UMingMoralAuthority::BestowBlessing(const FString& Target, float BlessingPower)
{
    return ApplyMoralInfluence(EMoralInfluenceType::Blessing, BlessingPower, Target);
}

float UMingMoralAuthority::CalculateInfluenceRadius() const
{
    float BaseRadius = BaseInfluenceRadius;
    float AuthorityBonus = CurrentMoralStatus.MoralAuthority * 2.0f;
    float FollowerBonus = CurrentMoralStatus.FollowerCount * 0.1f;
    float AuraBonus = CurrentMoralStatus.MoralAura * 50.0f;
    
    return BaseRadius + AuthorityBonus + FollowerBonus + AuraBonus;
}

bool UMingMoralAuthority::CanExecuteMoralAction(const FString& ActionName) const
{
    if (!MoralActionsMap.Contains(ActionName))
    {
        return false;
    }

    const FMoralAction& Action = MoralActionsMap[ActionName];
    return CurrentMoralStatus.MoralAuthority >= Action.MoralCost;
}

float UMingMoralAuthority::GetMoralEfficiency() const
{
    float AuthorityRatio = CurrentMoralStatus.MoralAuthority / MaxMoralAuthority;
    float TrustRatio = CurrentMoralStatus.PublicTrust / 100.0f;
    float AuraBonus = CurrentMoralStatus.MoralAura;
    
    float Efficiency = AuthorityRatio * TrustRatio * AuraBonus;
    
    // 應用修飾符
    for (const auto& Modifier : CurrentMoralStatus.MoralModifiers)
    {
        Efficiency *= Modifier.Value;
    }
    
    return FMath::Clamp(Efficiency, 0.0f, 2.0f);
}

void UMingMoralAuthority::ApplyMoralModifier(const FString& ModifierName, float ModifierValue)
{
    CurrentMoralStatus.MoralModifiers.Add(ModifierName, ModifierValue);
    UpdateMoralAura();
    
    UE_LOG(LogTemp, Log, TEXT("Applied moral modifier %s with value %.2f"), 
           *ModifierName, ModifierValue);
}

void UMingMoralAuthority::RemoveMoralModifier(const FString& ModifierName)
{
    if (CurrentMoralStatus.MoralModifiers.Remove(ModifierName) > 0)
    {
        UpdateMoralAura();
        UE_LOG(LogTemp, Log, TEXT("Removed moral modifier %s"), *ModifierName);
    }
}

void UMingMoralAuthority::UpdatePublicTrust(float TrustChange, const FString& Reason)
{
    float OldTrust = CurrentMoralStatus.PublicTrust;
    CurrentMoralStatus.PublicTrust = FMath::Clamp(
        CurrentMoralStatus.PublicTrust + TrustChange * PublicTrustInfluenceFactor, 
        0.0f, 100.0f
    );
    
    // 觸發信任更改事件
    OnPublicTrustChanged.Broadcast(CurrentMoralStatus.PublicTrust, Reason);
    
    UE_LOG(LogTemp, Log, TEXT("Public trust changed from %.2f to %.2f (%s)"), 
           OldTrust, CurrentMoralStatus.PublicTrust, *Reason);
}

TArray<FMoralAction> UMingMoralAuthority::GetMoralInfluenceHistory() const
{
    return MoralInfluenceHistory;
}

void UMingMoralAuthority::CheckMoralBalance()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 計算道德平衡
    float AuthorityRatio = CurrentMoralStatus.MoralAuthority / MaxMoralAuthority;
    float TrustRatio = CurrentMoralStatus.PublicTrust / 100.0f;
    float RighteousRatio = CurrentMoralStatus.RighteousPower / 100.0f;
    
    MoralBalanceLevel = (AuthorityRatio + TrustRatio + RighteousRatio) / 3.0f * 100.0f;
    
    // 觸發平衡更改事件
    OnMoralBalanceChanged(MoralBalanceLevel);
    
    UE_LOG(LogTemp, Log, TEXT("Moral balance checked: %.2f"), MoralBalanceLevel);
}

void UMingMoralAuthority::RestoreMoralAuthority(float Amount)
{
    if (!bIsInitialized)
    {
        return;
    }

    float OldAuthority = CurrentMoralStatus.MoralAuthority;
    CurrentMoralStatus.MoralAuthority = FMath::Clamp(
        CurrentMoralStatus.MoralAuthority + Amount * MoralRestoreRate, 
        0.0f, MaxMoralAuthority
    );
    
    // 更新道德光環
    UpdateMoralAura();
    
    // 觸發權威更改事件
    OnMoralAuthorityChanged.Broadcast(CurrentMoralStatus.MoralAuthority);
    
    UE_LOG(LogTemp, Log, TEXT("Restored %.2f moral authority (from %.2f to %.2f)"), 
           Amount * MoralRestoreRate, OldAuthority, CurrentMoralStatus.MoralAuthority);
}

void UMingMoralAuthority::InitializeMoralActions()
{
    // 感召行動
    FMoralAction Inspiration;
    Inspiration.ActionName = TEXT("道德感召");
    Inspiration.InfluenceType = EMoralInfluenceType::Inspiration;
    Inspiration.MoralCost = 20.0f;
    Inspiration.InfluenceEffect = 30.0f;
    Inspiration.Duration = 60.0f;
    Inspiration.RequiredConditions = {TEXT("MinFollowers:10")};
    Inspiration.Effects = {TEXT("GainFollowers"), TEXT("IncreaseTrust")};
    MoralActionsMap.Add(TEXT("道德感召"), Inspiration);

    // 審判行動
    FMoralAction Judgment;
    Judgment.ActionName = TEXT("正義審判");
    Judgment.InfluenceType = EMoralInfluenceType::Judgment;
    Judgment.MoralCost = 30.0f;
    Judgment.InfluenceEffect = 40.0f;
    Judgment.Duration = 30.0f;
    Judgment.RequiredConditions = {TEXT("MinAuthority:60")};
    Judgment.Effects = {TEXT("PunishEvil"), TEXT("IncreaseRighteousness")};
    MoralActionsMap.Add(TEXT("正義審判"), Judgment);

    // 引導行動
    FMoralAction Guidance;
    Guidance.ActionName = TEXT("民心引導");
    Guidance.InfluenceType = EMoralInfluenceType::Guidance;
    Guidance.MoralCost = 15.0f;
    Guidance.InfluenceEffect = 25.0f;
    Guidance.Duration = 45.0f;
    Guidance.RequiredConditions = {TEXT("MinTrust:40")};
    Guidance.Effects = {TEXT("GuideOpinion"), TEXT("IncreaseInfluence")};
    MoralActionsMap.Add(TEXT("民心引導"), Guidance);

    // 淨化行動
    FMoralAction Purification;
    Purification.ActionName = TEXT("邪惡淨化");
    Purification.InfluenceType = EMoralInfluenceType::Purification;
    Purification.MoralCost = 25.0f;
    Purification.InfluenceEffect = 35.0f;
    Purification.Duration = 40.0f;
    Purification.RequiredConditions = {TEXT("MinRighteousness:50")};
    Purification.Effects = {TEXT("PurifyTarget"), TEXT("IncreaseAura")};
    MoralActionsMap.Add(TEXT("邪惡淨化"), Purification);

    // 祝福行動
    FMoralAction Blessing;
    Blessing.ActionName = TEXT("神聖祝福");
    Blessing.InfluenceType = EMoralInfluenceType::Blessing;
    Blessing.MoralCost = 35.0f;
    Blessing.InfluenceEffect = 45.0f;
    Blessing.Duration = 90.0f;
    Blessing.RequiredConditions = {TEXT("MinAuthority:70"), TEXT("MinTrust:60")};
    Blessing.Effects = {TEXT("BlessTarget"), TEXT("IncreaseRadius")};
    MoralActionsMap.Add(TEXT("神聖祝福"), Blessing);
}

float UMingMoralAuthority::CalculateInfluenceEffect(EMoralInfluenceType InfluenceType, float Intensity) const
{
    float BaseEffect = Intensity * 10.0f;
    float Efficiency = GetMoralEfficiency();
    
    // 根據影響類型調整效果
    switch (InfluenceType)
    {
        case EMoralInfluenceType::Inspiration:
            return BaseEffect * Efficiency * 1.2f;
        case EMoralInfluenceType::Judgment:
            return BaseEffect * Efficiency * 1.5f;
        case EMoralInfluenceType::Guidance:
            return BaseEffect * Efficiency * 1.0f;
        case EMoralInfluenceType::Purification:
            return BaseEffect * Efficiency * 1.3f;
        case EMoralInfluenceType::Blessing:
            return BaseEffect * Efficiency * 1.8f;
        default:
            return BaseEffect * Efficiency;
    }
}

void UMingMoralAuthority::UpdateMoralAura()
{
    float AuthorityRatio = CurrentMoralStatus.MoralAuthority / MaxMoralAuthority;
    float TrustRatio = CurrentMoralStatus.PublicTrust / 100.0f;
    float FollowerBonus = FMath::Min(CurrentMoralStatus.FollowerCount / 100.0f, 2.0f);
    
    CurrentMoralStatus.MoralAura = 1.0f + AuthorityRatio + TrustRatio * 0.5f + FollowerBonus * 0.3f;
    
    // 應用修飾符
    for (const auto& Modifier : CurrentMoralStatus.MoralModifiers)
    {
        CurrentMoralStatus.MoralAura *= Modifier.Value;
    }
    
    CurrentMoralStatus.MoralAura = FMath::Clamp(CurrentMoralStatus.MoralAura, 0.1f, 3.0f);
}

bool UMingMoralAuthority::CheckActionConditions(const FMoralAction& Action) const
{
    for (const FString& Condition : Action.RequiredConditions)
    {
        TArray<FString> Parts;
        Condition.ParseIntoArray(Parts, TEXT(":"));
        
        if (Parts.Num() == 2)
        {
            FString ConditionType = Parts[0];
            float RequiredValue = FCString::Atof(*Parts[1]);
            
            if (ConditionType == TEXT("MinAuthority"))
            {
                if (CurrentMoralStatus.MoralAuthority < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinTrust"))
            {
                if (CurrentMoralStatus.PublicTrust < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinRighteousness"))
            {
                if (CurrentMoralStatus.RighteousPower < RequiredValue)
                    return false;
            }
            else if (ConditionType == TEXT("MinFollowers"))
            {
                if (CurrentMoralStatus.FollowerCount < RequiredValue)
                    return false;
            }
        }
    }
    
    return true;
}

void UMingMoralAuthority::ApplyActionEffects(const FMoralAction& Action)
{
    for (const FString& Effect : Action.Effects)
    {
        if (Effect == TEXT("GainFollowers"))
        {
            int32 NewFollowers = FMath::RoundToInt(Action.InfluenceEffect * GetMoralEfficiency());
            CurrentMoralStatus.FollowerCount = FMath::Min(CurrentMoralStatus.FollowerCount + NewFollowers, MaxFollowers);
            OnFollowerGained.Broadcast(CurrentMoralStatus.FollowerCount, Action.ActionName);
        }
        else if (Effect == TEXT("IncreaseTrust"))
        {
            UpdatePublicTrust(Action.InfluenceEffect * 0.5f, Action.ActionName);
        }
        else if (Effect == TEXT("IncreaseRighteousness"))
        {
            CurrentMoralStatus.RighteousPower = FMath::Clamp(
                CurrentMoralStatus.RighteousPower + Action.InfluenceEffect * 0.3f, 
                0.0f, 100.0f
            );
        }
        else if (Effect == TEXT("IncreaseInfluence"))
        {
            CurrentMoralStatus.InfluenceRadius += Action.InfluenceEffect * 2.0f;
        }
        else if (Effect == TEXT("IncreaseAura"))
        {
            CurrentMoralStatus.MoralAura += Action.InfluenceEffect * 0.1f;
            UpdateMoralAura();
        }
        else if (Effect == TEXT("IncreaseRadius"))
        {
            CurrentMoralStatus.InfluenceRadius += Action.InfluenceEffect * 5.0f;
        }
    }
}

void UMingMoralAuthority::RecordMoralAction(const FMoralAction& Action)
{
    MoralInfluenceHistory.Add(Action);
    
    // 限制歷史記錄數量
    if (MoralInfluenceHistory.Num() > 100)
    {
        MoralInfluenceHistory.RemoveAt(0);
    }
}
