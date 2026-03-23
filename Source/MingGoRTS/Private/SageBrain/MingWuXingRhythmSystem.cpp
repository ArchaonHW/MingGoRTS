#include "SageBrain/MingWuXingRhythmSystem.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingWuXingRhythmSystem::UMingWuXingRhythmSystem()
{
    bIsInitialized = false;
    MaxWuXingPower = 100.0f;
    PhaseTransitionThreshold = 1.0f;
    
    InitializeDefaultWuXingElements();
    InitializeWuXingRelations();
}

void UMingWuXingRhythmSystem::InitializeWuXingRhythmSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("WuXing Rhythm System already initialized"));
        return;
    }

    // 初始化所有五行元素
    WuXingStatusMap.Empty();
    
    for (uint8 i = static_cast<uint8>(EWuXingType::Wood); 
         i <= static_cast<uint8>(EWuXingType::Water); ++i)
    {
        EWuXingType ElementType = static_cast<EWuXingType>(i);
        FWuXingStatus Status;
        Status.WuXingType = ElementType;
        Status.CurrentPhase = EWuXingPhase::Birth;
        Status.PhaseProgress = 0.0f;
        Status.PowerLevel = 20.0f;
        Status.Stability = 0.5f;
        Status.bIsActive = false;
        
        WuXingStatusMap.Add(ElementType, Status);
    }
    
    ActiveCombinations.Empty();
    ActiveRhythmEffects.Empty();
    
    bIsInitialized = true;
    
    OnWuXingRhythmSystemInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("WuXing Rhythm System initialized successfully"));
}

void UMingWuXingRhythmSystem::ActivateWuXingElement(EWuXingType ElementType)
{
    if (!bIsInitialized || ElementType == EWuXingType::None)
    {
        return;
    }

    FWuXingStatus* Status = WuXingStatusMap.Find(ElementType);
    if (Status)
    {
        Status->bIsActive = true;
        Status->CurrentPhase = EWuXingPhase::Birth;
        Status->PhaseProgress = 0.0f;
        
        ApplyBirthPhaseEffect(ElementType);
        
        UE_LOG(LogTemp, Log, TEXT("WuXing element %d activated"), static_cast<int32>(ElementType));
        
        // 更新元素關係
        UpdateElementRelations();
    }
}

void UMingWuXingRhythmSystem::DeactivateWuXingElement(EWuXingType ElementType)
{
    if (!bIsInitialized)
    {
        return;
    }

    FWuXingStatus* Status = WuXingStatusMap.Find(ElementType);
    if (Status)
    {
        Status->bIsActive = false;
        Status->CurrentPhase = EWuXingPhase::None;
        Status->PhaseProgress = 0.0f;
        
        UE_LOG(LogTemp, Log, TEXT("WuXing element %d deactivated"), static_cast<int32>(ElementType));
    }
}

void UMingWuXingRhythmSystem::AdvancePhase(EWuXingType ElementType, float ProgressDelta)
{
    if (!bIsInitialized)
    {
        return;
    }

    FWuXingStatus* Status = WuXingStatusMap.Find(ElementType);
    if (!Status || !Status->bIsActive)
    {
        return;
    }

    EWuXingPhase OldPhase = Status->CurrentPhase;
    
    Status->PhaseProgress += ProgressDelta;
    
    // 檢查是否達到階段轉換條件
    if (Status->PhaseProgress >= PhaseTransitionThreshold)
    {
        EWuXingPhase NextPhase = GetNextPhase(Status->CurrentPhase);
        
        if (CanTransitionToPhase(ElementType, NextPhase))
        {
            OnPhaseTransitionCompleted(ElementType, OldPhase, NextPhase);
            
            Status->CurrentPhase = NextPhase;
            Status->PhaseProgress = 0.0f;
            
            // 應用新階段效果
            switch (NextPhase)
            {
            case EWuXingPhase::Birth:
                ApplyBirthPhaseEffect(ElementType);
                break;
            case EWuXingPhase::Growth:
                ApplyGrowthPhaseEffect(ElementType);
                break;
            case EWuXingPhase::Stabilization:
                ApplyStabilizationPhaseEffect(ElementType);
                break;
            case EWuXingPhase::Contraction:
                ApplyContractionPhaseEffect(ElementType);
                break;
            case EWuXingPhase::Storage:
                ApplyStoragePhaseEffect(ElementType);
                break;
            default:
                break;
            }
            
            OnWuXingPhaseChanged.Broadcast(ElementType, NextPhase);
        }
    }
}

FWuXingStatus UMingWuXingRhythmSystem::GetWuXingStatus(EWuXingType ElementType) const
{
    if (const FWuXingStatus* Status = WuXingStatusMap.Find(ElementType))
    {
        return *Status;
    }
    
    return FWuXingStatus();
}

void UMingWuXingRhythmSystem::SetWuXingPower(EWuXingType ElementType, float PowerLevel)
{
    if (!bIsInitialized)
    {
        return;
    }

    FWuXingStatus* Status = WuXingStatusMap.Find(ElementType);
    if (Status)
    {
        Status->PowerLevel = FMath::Clamp(PowerLevel, 0.0f, MaxWuXingPower);
        
        // 更新穩定性
        Status->Stability = Status->PowerLevel / MaxWuXingPower;
        
        UE_LOG(LogTemp, Log, TEXT("WuXing element %d power set to %.2f"), 
               static_cast<int32>(ElementType), Status->PowerLevel);
    }
}

EWuXingRelation UMingWuXingRhythmSystem::CalculateWuXingRelation(EWuXingType Source, EWuXingType Target) const
{
    if (Source == EWuXingType::None || Target == EWuXingType::None)
    {
        return EWuXingRelation::None;
    }

    const TMap<EWuXingType, EWuXingRelation>* Relations = WuXingRelationMatrix.Find(Source);
    if (Relations)
    {
        if (const EWuXingRelation* Relation = Relations->Find(Target))
        {
            return *Relation;
        }
    }
    
    return EWuXingRelation::None;
}

FRhythmEffect UMingWuXingRhythmSystem::GetRhythmEffect(EWuXingType Source, EWuXingType Target) const
{
    FRhythmEffect Effect;
    Effect.SourceElement = Source;
    Effect.TargetElement = Target;
    Effect.RelationType = CalculateWuXingRelation(Source, Target);
    
    const FWuXingStatus* SourceStatus = WuXingStatusMap.Find(Source);
    const FWuXingStatus* TargetStatus = WuXingStatusMap.Find(Target);
    
    if (SourceStatus && TargetStatus)
    {
        float SourcePower = SourceStatus->PowerLevel;
        float TargetPower = TargetStatus->PowerLevel;
        
        switch (Effect.RelationType)
        {
        case EWuXingRelation::Generate:
            Effect.EffectStrength = SourcePower * 0.3f;
            Effect.bIsBeneficial = true;
            Effect.EffectDescription = TEXT("相生效果：增強目標元素");
            break;
        case EWuXingRelation::Restrain:
            Effect.EffectStrength = SourcePower * 0.4f;
            Effect.bIsBeneficial = false;
            Effect.EffectDescription = TEXT("相剋效果：抑制目標元素");
            break;
        case EWuXingRelation::Overcome:
            Effect.EffectStrength = SourcePower * 0.5f;
            Effect.bIsBeneficial = false;
            Effect.EffectDescription = TEXT("相乘效果：過度克制");
            break;
        case EWuXingRelation::Insult:
            Effect.EffectStrength = TargetPower * 0.3f;
            Effect.bIsBeneficial = false;
            Effect.EffectDescription = TEXT("相侮效果：反被克制");
            break;
        default:
            Effect.EffectStrength = 0.0f;
            Effect.bIsBeneficial = true;
            Effect.EffectDescription = TEXT("無特殊關係");
            break;
        }
        
        Effect.Duration = 10.0f; // 預設持續10秒
    }
    
    return Effect;
}

bool UMingWuXingRhythmSystem::IsRhythmBalanced(const TArray<EWuXingType>& ActiveElements) const
{
    if (ActiveElements.Num() < 2)
    {
        return false;
    }
    
    float TotalPower = 0.0f;
    float MaxPower = 0.0f;
    float MinPower = MaxWuXingPower;
    
    for (EWuXingType Element : ActiveElements)
    {
        const FWuXingStatus* Status = WuXingStatusMap.Find(Element);
        if (Status)
        {
            TotalPower += Status->PowerLevel;
            MaxPower = FMath::Max(MaxPower, Status->PowerLevel);
            MinPower = FMath::Min(MinPower, Status->PowerLevel);
        }
    }
    
    float AveragePower = TotalPower / ActiveElements.Num();
    float PowerVariance = (MaxPower - MinPower) / AveragePower;
    
    // 如果差異小於30%，認為是平衡的
    return PowerVariance < 0.3f;
}

FWuXingRhythmCombination UMingWuXingRhythmSystem::FormWuXingCombination(const TArray<EWuXingType>& Elements)
{
    FWuXingRhythmCombination Combination;
    Combination.ActiveElements = Elements;
    
    if (Elements.Num() < 2)
    {
        return Combination;
    }
    
    // 計算和諧度
    Combination.RhythmHarmony = CalculateHarmonyScore(Elements);
    Combination.bIsBalanced = IsRhythmBalanced(Elements);
    
    // 計算總威力
    float TotalPower = 0.0f;
    for (EWuXingType Element : Elements)
    {
        const FWuXingStatus* Status = WuXingStatusMap.Find(Element);
        if (Status)
        {
            TotalPower += Status->PowerLevel;
        }
    }
    Combination.TotalPower = TotalPower;
    
    // 生成組合效果
    for (int32 i = 0; i < Elements.Num(); ++i)
    {
        for (int32 j = i + 1; j < Elements.Num(); ++j)
        {
            FRhythmEffect Effect = GetRhythmEffect(Elements[i], Elements[j]);
            if (Effect.RelationType != EWuXingRelation::None)
            {
                Combination.ActiveEffects.Add(Effect);
            }
        }
    }
    
    // 設置組合名稱
    if (Elements.Num() == 5)
    {
        Combination.CombinationName = TEXT("五行完備");
    }
    else if (Elements.Num() >= 3)
    {
        Combination.CombinationName = TEXT("多元組合");
    }
    else
    {
        Combination.CombinationName = TEXT("雙行組合");
    }
    
    ActiveCombinations.Add(Combination);
    
    OnWuXingCombinationFormed.Broadcast(Combination);
    
    UE_LOG(LogTemp, Log, TEXT("WuXing combination formed: %s (Harmony: %.2f, Power: %.2f)"),
           *Combination.CombinationName, Combination.RhythmHarmony, Combination.TotalPower);
    
    return Combination;
}

TArray<FWuXingRhythmCombination> UMingWuXingRhythmSystem::GetRecommendedCombinations() const
{
    TArray<FWuXingRhythmCombination> Recommendations;
    
    // 推薦五行完備組合
    TArray<EWuXingType> AllElements = {
        EWuXingType::Wood, EWuXingType::Fire, EWuXingType::Earth,
        EWuXingType::Metal, EWuXingType::Water
    };
    
    // 檢查是否可以形成五行完備
    bool CanFormComplete = true;
    for (EWuXingType Element : AllElements)
    {
        const FWuXingStatus* Status = WuXingStatusMap.Find(Element);
        if (!Status || !Status->bIsActive)
        {
            CanFormComplete = false;
            break;
        }
    }
    
    if (CanFormComplete)
    {
        // 這裡只是返回現有組合，不創建新組合
        for (const auto& Combination : ActiveCombinations)
        {
            if (Combination.ActiveElements.Num() == 5)
            {
                Recommendations.Add(Combination);
            }
        }
    }
    
    // 按和諧度排序
    Recommendations.Sort([](const FWuXingRhythmCombination& A, const FWuXingRhythmCombination& B)
    {
        return A.RhythmHarmony > B.RhythmHarmony;
    });
    
    return Recommendations;
}

// 五行階段效果實現
void UMingWuXingRhythmSystem::ApplyBirthPhaseEffect(EWuXingType Element)
{
    FWuXingStatus* Status = WuXingStatusMap.Find(Element);
    if (Status)
    {
        Status->PowerLevel = FMath::Min(Status->PowerLevel + 5.0f, MaxWuXingPower);
        UE_LOG(LogTemp, Log, TEXT("Birth phase effect applied to element %d"), static_cast<int32>(Element));
    }
}

void UMingWuXingRhythmSystem::ApplyGrowthPhaseEffect(EWuXingType Element)
{
    FWuXingStatus* Status = WuXingStatusMap.Find(Element);
    if (Status)
    {
        Status->PowerLevel = FMath::Min(Status->PowerLevel + 10.0f, MaxWuXingPower);
        Status->Stability = FMath::Min(Status->Stability + 0.1f, 1.0f);
        UE_LOG(LogTemp, Log, TEXT("Growth phase effect applied to element %d"), static_cast<int32>(Element));
    }
}

void UMingWuXingRhythmSystem::ApplyStabilizationPhaseEffect(EWuXingType Element)
{
    FWuXingStatus* Status = WuXingStatusMap.Find(Element);
    if (Status)
    {
        Status->Stability = FMath::Min(Status->Stability + 0.2f, 1.0f);
        UE_LOG(LogTemp, Log, TEXT("Stabilization phase effect applied to element %d"), static_cast<int32>(Element));
    }
}

void UMingWuXingRhythmSystem::ApplyContractionPhaseEffect(EWuXingType Element)
{
    FWuXingStatus* Status = WuXingStatusMap.Find(Element);
    if (Status)
    {
        Status->PowerLevel = FMath::Max(Status->PowerLevel - 5.0f, 0.0f);
        UE_LOG(LogTemp, Log, TEXT("Contraction phase effect applied to element %d"), static_cast<int32>(Element));
    }
}

void UMingWuXingRhythmSystem::ApplyStoragePhaseEffect(EWuXingType Element)
{
    FWuXingStatus* Status = WuXingStatusMap.Find(Element);
    if (Status)
    {
        Status->Stability = FMath::Min(Status->Stability + 0.3f, 1.0f);
        // 儲存階段累積能量
        Status->PowerLevel = FMath::Min(Status->PowerLevel + 15.0f, MaxWuXingPower);
        UE_LOG(LogTemp, Log, TEXT("Storage phase effect applied to element %d"), static_cast<int32>(Element));
    }
}

// 五行生剋應用
void UMingWuXingRhythmSystem::ApplyGeneratingRelation(EWuXingType Source, EWuXingType Target)
{
    FRhythmEffect Effect = GetRhythmEffect(Source, Target);
    if (Effect.RelationType == EWuXingRelation::Generate)
    {
        FWuXingStatus* TargetStatus = WuXingStatusMap.Find(Target);
        if (TargetStatus)
        {
            TargetStatus->PowerLevel = FMath::Min(
                TargetStatus->PowerLevel + Effect.EffectStrength, 
                MaxWuXingPower
            );
            
            ActiveRhythmEffects.Add(Effect);
            OnRhythmEffectTriggered.Broadcast(Effect);
            
            UE_LOG(LogTemp, Log, TEXT("Generating relation applied: %d -> %d"),
                   static_cast<int32>(Source), static_cast<int32>(Target));
        }
    }
}

void UMingWuXingRhythmSystem::ApplyRestrainingRelation(EWuXingType Source, EWuXingType Target)
{
    FRhythmEffect Effect = GetRhythmEffect(Source, Target);
    if (Effect.RelationType == EWuXingRelation::Restrain)
    {
        FWuXingStatus* TargetStatus = WuXingStatusMap.Find(Target);
        if (TargetStatus)
        {
            TargetStatus->PowerLevel = FMath::Max(
                TargetStatus->PowerLevel - Effect.EffectStrength, 
                0.0f
            );
            
            ActiveRhythmEffects.Add(Effect);
            OnRhythmEffectTriggered.Broadcast(Effect);
            
            UE_LOG(LogTemp, Log, TEXT("Restraining relation applied: %d -> %d"),
                   static_cast<int32>(Source), static_cast<int32>(Target));
        }
    }
}

// 私有方法實現
void UMingWuXingRhythmSystem::InitializeDefaultWuXingElements()
{
    // 默認構造函數已經初始化基本狀態
}

void UMingWuXingRhythmSystem::InitializeWuXingRelations()
{
    WuXingRelationMatrix.Empty();
    
    // 定義五行相生關係：木生火，火生土，土生金，金生水，水生木
    TMap<EWuXingType, EWuXingRelation> WoodRelations;
    WoodRelations.Add(EWuXingType::Fire, EWuXingRelation::Generate);
    WoodRelations.Add(EWuXingType::Earth, EWuXingRelation::Restrain);
    WuXingRelationMatrix.Add(EWuXingType::Wood, WoodRelations);
    
    TMap<EWuXingType, EWuXingRelation> FireRelations;
    FireRelations.Add(EWuXingType::Earth, EWuXingRelation::Generate);
    FireRelations.Add(EWuXingType::Metal, EWuXingRelation::Restrain);
    WuXingRelationMatrix.Add(EWuXingType::Fire, FireRelations);
    
    TMap<EWuXingType, EWuXingRelation> EarthRelations;
    EarthRelations.Add(EWuXingType::Metal, EWuXingRelation::Generate);
    EarthRelations.Add(EWuXingType::Water, EWuXingRelation::Restrain);
    WuXingRelationMatrix.Add(EWuXingType::Earth, EarthRelations);
    
    TMap<EWuXingType, EWuXingRelation> MetalRelations;
    MetalRelations.Add(EWuXingType::Water, EWuXingRelation::Generate);
    MetalRelations.Add(EWuXingType::Wood, EWuXingRelation::Restrain);
    WuXingRelationMatrix.Add(EWuXingType::Metal, MetalRelations);
    
    TMap<EWuXingType, EWuXingRelation> WaterRelations;
    WaterRelations.Add(EWuXingType::Wood, EWuXingRelation::Generate);
    WaterRelations.Add(EWuXingType::Fire, EWuXingRelation::Restrain);
    WuXingRelationMatrix.Add(EWuXingType::Water, WaterRelations);
}

float UMingWuXingRhythmSystem::CalculatePhaseEffectiveness(EWuXingType Element, EWuXingPhase Phase) const
{
    const FWuXingStatus* Status = WuXingStatusMap.Find(Element);
    if (!Status)
    {
        return 0.0f;
    }
    
    float BaseEffectiveness = 0.5f;
    
    // 根據元素和階段的配合調整效果
    switch (Element)
    {
    case EWuXingType::Wood:
        // 木元素在生發和壯大階段效果最佳
        if (Phase == EWuXingPhase::Birth || Phase == EWuXingPhase::Growth)
        {
            BaseEffectiveness += 0.3f;
        }
        break;
    case EWuXingType::Fire:
        // 火元素在壯大和穩固階段效果最佳
        if (Phase == EWuXingPhase::Growth || Phase == EWuXingPhase::Stabilization)
        {
            BaseEffectiveness += 0.3f;
        }
        break;
    case EWuXingType::Earth:
        // 土元素在穩固階段效果最佳
        if (Phase == EWuXingPhase::Stabilization)
        {
            BaseEffectiveness += 0.4f;
        }
        break;
    case EWuXingType::Metal:
        // 金元素在收斂和儲存階段效果最佳
        if (Phase == EWuXingPhase::Contraction || Phase == EWuXingPhase::Storage)
        {
            BaseEffectiveness += 0.3f;
        }
        break;
    case EWuXingType::Water:
        // 水元素在儲存和生發階段效果最佳
        if (Phase == EWuXingPhase::Storage || Phase == EWuXingPhase::Birth)
        {
            BaseEffectiveness += 0.3f;
        }
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseEffectiveness, 0.0f, 1.0f);
}

bool UMingWuXingRhythmSystem::CanTransitionToPhase(EWuXingType Element, EWuXingPhase TargetPhase) const
{
    const FWuXingStatus* Status = WuXingStatusMap.Find(Element);
    if (!Status)
    {
        return false;
    }
    
    // 檢查是否有足夠的穩定性來轉換階段
    return Status->Stability >= 0.3f;
}

EWuXingPhase UMingWuXingRhythmSystem::GetNextPhase(EWuXingPhase CurrentPhase) const
{
    switch (CurrentPhase)
    {
    case EWuXingPhase::Birth:
        return EWuXingPhase::Growth;
    case EWuXingPhase::Growth:
        return EWuXingPhase::Stabilization;
    case EWuXingPhase::Stabilization:
        return EWuXingPhase::Contraction;
    case EWuXingPhase::Contraction:
        return EWuXingPhase::Storage;
    case EWuXingPhase::Storage:
        return EWuXingPhase::Birth; // 循環
    default:
        return EWuXingPhase::Birth;
    }
}

float UMingWuXingRhythmSystem::CalculateHarmonyScore(const TArray<EWuXingType>& Elements) const
{
    if (Elements.Num() < 2)
    {
        return 0.0f;
    }
    
    float HarmonyScore = 0.0f;
    int32 RelationCount = 0;
    
    for (int32 i = 0; i < Elements.Num(); ++i)
    {
        for (int32 j = i + 1; j < Elements.Num(); ++j)
        {
            EWuXingRelation Relation = CalculateWuXingRelation(Elements[i], Elements[j]);
            
            if (Relation == EWuXingRelation::Generate)
            {
                HarmonyScore += 1.0f;
            }
            else if (Relation == EWuXingRelation::Restrain)
            {
                HarmonyScore -= 0.5f;
            }
            
            RelationCount++;
        }
    }
    
    if (RelationCount > 0)
    {
        HarmonyScore = HarmonyScore / RelationCount;
    }
    
    // 調整到 0-1 範圍
    return FMath::Clamp((HarmonyScore + 1.0f) / 2.0f, 0.0f, 1.0f);
}

void UMingWuXingRhythmSystem::UpdateElementRelations()
{
    // 重新計算所有活躍元素之間的關係效果
    TArray<EWuXingType> ActiveElements;
    
    for (const auto& Pair : WuXingStatusMap)
    {
        if (Pair.Value.bIsActive)
        {
            ActiveElements.Add(Pair.Key);
        }
    }
    
    // 觸發相關的節奏效果
    for (int32 i = 0; i < ActiveElements.Num(); ++i)
    {
        for (int32 j = i + 1; j < ActiveElements.Num(); ++j)
        {
            FRhythmEffect Effect = GetRhythmEffect(ActiveElements[i], ActiveElements[j]);
            if (Effect.RelationType == EWuXingRelation::Generate)
            {
                ApplyGeneratingRelation(ActiveElements[i], ActiveElements[j]);
            }
        }
    }
}

void UMingWuXingRhythmSystem::TriggerRhythmEffects()
{
    // 遍歷所有活躍效果，檢查是否過期
    for (int32 i = ActiveRhythmEffects.Num() - 1; i >= 0; --i)
    {
        ActiveRhythmEffects[i].Duration -= 1.0f; // 假設每秒調用一次
        
        if (ActiveRhythmEffects[i].Duration <= 0.0f)
        {
            ActiveRhythmEffects.RemoveAt(i);
        }
    }
}
