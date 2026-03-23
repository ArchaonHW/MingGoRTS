#include "SageBrain/MingNegativeStrategies.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UMingNegativeStrategies::UMingNegativeStrategies()
{
    BaseSuccessRate = 0.6f;
    CovertOperationBonus = 0.15f;
    InitializeDefaultValues();
}

void UMingNegativeStrategies::InitializeDefaultValues()
{
    BreakSituationHistory.Empty();
    BreakStructureHistory.Empty();
    PreservePeopleHistory.Empty();
}

FBreakSituationResult UMingNegativeStrategies::ExecuteBreakSituation(const FBreakSituationParams& Params)
{
    FBreakSituationResult Result;
    
    OnBreakSituationStarted(Params);
    
    // 檢查是否可以執行
    if (!CanBreakSituation(Params))
    {
        Result.ExecutionLog = TEXT("條件不足，無法破局");
        return Result;
    }
    
    // 計算破局潛力
    float DisruptionPotential = CalculateDisruptionPotential(Params);
    float WeaknessExploitation = CalculateWeaknessExploitation(Params.Weaknesses);
    
    // 隱蔽行動加成
    float CovertBonus = Params.bCovertOperation ? CovertOperationBonus : 0.0f;
    
    // 判斷成功與否
    float SuccessChance = BaseSuccessRate * (DisruptionPotential * 0.4f + WeaknessExploitation * 0.4f + CovertBonus * 0.2f);
    Result.bSuccess = (FMath::FRand() < SuccessChance);
    
    if (Result.bSuccess)
    {
        Result.DisruptionEffect = DisruptionPotential * (0.7f + FMath::FRand() * 0.6f);
        Result.TargetStabilityLoss = Result.DisruptionEffect * 0.8f;
        Result.DestroyedStructures = IdentifyTargets(Params);
        Result.ResourceCost = Params.ResourceInvestment * (1.0f + FMath::FRand() * 0.5f);
        Result.ReputationImpact = Params.bCovertOperation ? -0.05f : -0.15f;
        Result.ExecutionLog = FString::Printf(TEXT("成功破局：%s"), *Params.TargetSituationId);
        
        // 記錄歷史
        FString RecordId = FString::Printf(TEXT("BreakSit_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S")));
        BreakSituationHistory.Add(RecordId, Result);
        
        UE_LOG(LogTemp, Log, TEXT("Situation broken: %s"), *Params.TargetSituationId);
    }
    else
    {
        Result.ResourceCost = Params.ResourceInvestment * 0.5f;
        Result.ReputationImpact = Params.bCovertOperation ? -0.02f : -0.08f;
        Result.ExecutionLog = FString::Printf(TEXT("破局失敗：%s"), *Params.TargetSituationId);
        UE_LOG(LogTemp, Warning, TEXT("Failed to break situation: %s"), *Params.TargetSituationId);
    }
    
    OnSituationBroken.Broadcast(Result);
    return Result;
}

FBreakStructureResult UMingNegativeStrategies::ExecuteBreakStructure(const FBreakStructureParams& Params)
{
    FBreakStructureResult Result;
    
    OnBreakStructureStarted(Params);
    
    // 檢查是否可以執行
    if (!CanBreakStructure(Params))
    {
        Result.ExecutionLog = TEXT("條件不足，無法破結構");
        return Result;
    }
    
    // 計算破壞潛力
    float DestructionPotential = CalculateDestructionPotential(Params);
    float PrecisionBonus = CalculatePrecisionBonus(Params.bPrecisionStrike, Params.KeyPoints);
    
    // 判斷成功與否
    float SuccessChance = BaseSuccessRate * (DestructionPotential * 0.6f + PrecisionBonus * 0.4f);
    Result.bSuccess = (FMath::FRand() < SuccessChance);
    
    if (Result.bSuccess)
    {
        Result.StructureIntegrityLoss = DestructionPotential * (0.6f + FMath::FRand() * 0.8f);
        Result.FunctionalDamage = Result.StructureIntegrityLoss * 0.9f;
        Result.bCompleteDestruction = Result.StructureIntegrityLoss > 0.95f;
        Result.MilitaryEffort = Params.RequiredForce * (0.8f + FMath::FRand() * 0.4f);
        Result.CollateralDamage = AssessCollateralDamage(Params);
        Result.ExecutionLog = FString::Printf(TEXT("成功破結構：%s"), *Params.TargetStructureId);
        
        // 記錄歷史
        FString RecordId = FString::Printf(TEXT("BreakStruct_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S")));
        BreakStructureHistory.Add(RecordId, Result);
        
        UE_LOG(LogTemp, Log, TEXT("Structure broken: %s"), *Params.TargetStructureId);
    }
    else
    {
        Result.MilitaryEffort = Params.RequiredForce * 0.3f;
        Result.ExecutionLog = FString::Printf(TEXT("破結構失敗：%s"), *Params.TargetStructureId);
        UE_LOG(LogTemp, Warning, TEXT("Failed to break structure: %s"), *Params.TargetStructureId);
    }
    
    OnStructureBroken.Broadcast(Result);
    return Result;
}

FPreservePeopleResult UMingNegativeStrategies::ExecutePreservePeople(const FPreservePeopleParams& Params)
{
    FPreservePeopleResult Result;
    
    OnPreservePeopleStarted(Params);
    
    // 檢查是否可以執行
    if (!CanPreservePeople(Params))
    {
        Result.ExecutionLog = TEXT("條件不足，無法保護人民");
        return Result;
    }
    
    // 計算保護效率
    float PreservationEfficiency = CalculatePreservationEfficiency(Params);
    float ProtectionCoverage = CalculateProtectionCoverage(Params.SafeZones);
    
    // 判斷成功與否
    float SuccessChance = BaseSuccessRate * (PreservationEfficiency * 0.5f + ProtectionCoverage * 0.3f + Params.ProtectionLevel * 0.2f);
    Result.bSuccess = (FMath::FRand() < SuccessChance);
    
    if (Result.bSuccess)
    {
        Result.ProtectedPeople = FMath::FloorToInt(Params.ProtectedPeopleIds.Num() * PreservationEfficiency * (0.8f + FMath::FRand() * 0.4f));
        Result.PreservationRate = static_cast<float>(Result.ProtectedPeople) / FMath::Max(Params.ProtectedPeopleIds.Num(), 1);
        Result.PopulationLoyaltyGain = Result.PreservationRate * 0.3f;
        Result.MoralAuthorityGain = Result.PreservationRate * 0.4f;
        Result.RecruitedTalents = RecruitTalents(Params.ProtectedPeopleIds);
        Result.ExecutionLog = FString::Printf(TEXT("成功保護人民：%d人"), Result.ProtectedPeople);
        
        // 記錄歷史
        FString RecordId = FString::Printf(TEXT("Preserve_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S")));
        PreservePeopleHistory.Add(RecordId, Result);
        
        UE_LOG(LogTemp, Log, TEXT("People preserved: %d"), Result.ProtectedPeople);
    }
    else
    {
        Result.ProtectedPeople = FMath::FloorToInt(Params.ProtectedPeopleIds.Num() * 0.2f);
        Result.PreservationRate = static_cast<float>(Result.ProtectedPeople) / FMath::Max(Params.ProtectedPeopleIds.Num(), 1);
        Result.ExecutionLog = TEXT("保護人民部分失敗");
        UE_LOG(LogTemp, Warning, TEXT("Partially failed to preserve people"));
    }
    
    OnPeoplePreserved.Broadcast(Result);
    return Result;
}

// 分析功能實現
TArray<FString> UMingNegativeStrategies::AnalyzeSituationWeaknesses(const FString& SituationId) const
{
    TArray<FString> Weaknesses;
    
    // 模擬分析弱點
    if (FMath::FRand() < 0.7f) Weaknesses.Add(TEXT("防禦薄弱"));
    if (FMath::FRand() < 0.6f) Weaknesses.Add(TEXT("補給線過長"));
    if (FMath::FRand() < 0.5f) Weaknesses.Add(TEXT("士氣低落"));
    if (FMath::FRand() < 0.4f) Weaknesses.Add(TEXT("指揮不統一"));
    if (FMath::FRand() < 0.3f) Weaknesses.Add(TEXT("情報洩漏"));
    
    return Weaknesses;
}

TArray<FString> UMingNegativeStrategies::IdentifyStructuralWeaknesses(const FString& StructureId) const
{
    TArray<FString> Weaknesses;
    
    // 模擬識別結構弱點
    if (FMath::FRand() < 0.8f) Weaknesses.Add(TEXT("基礎不穩"));
    if (FMath::FRand() < 0.6f) Weaknesses.Add(TEXT("材料老化"));
    if (FMath::FRand() < 0.5f) Weaknesses.Add(TEXT("設計缺陷"));
    if (FMath::FRand() < 0.4f) Weaknesses.Add(TEXT("維護不足"));
    if (FMath::FRand() < 0.3f) Weaknesses.Add(TEXT("關鍵節點暴露"));
    
    return Weaknesses;
}

float UMingNegativeStrategies::CalculatePreservationValue(const TArray<FString>& PeopleIds) const
{
    if (PeopleIds.Num() == 0)
    {
        return 0.0f;
    }
    
    // 基礎價值
    float BaseValue = 0.5f;
    
    // 人才越多價值越高
    float QuantityBonus = FMath::Min(PeopleIds.Num() * 0.02f, 0.3f);
    
    return BaseValue + QuantityBonus;
}

// 計算功能實現
float UMingNegativeStrategies::CalculateDisruptionPotential(const FBreakSituationParams& Params) const
{
    float BasePotential = 0.4f;
    
    // 破壞強度影響
    float IntensityBonus = Params.DisruptionIntensity * 0.3f;
    
    // 弱點數量影響
    float WeaknessBonus = FMath::Min(Params.Weaknesses.Num() * 0.08f, 0.2f);
    
    // 投資影響
    float InvestmentBonus = FMath::Clamp(Params.ResourceInvestment / 1000.0f, 0.0f, 0.1f);
    
    return FMath::Clamp(BasePotential + IntensityBonus + WeaknessBonus + InvestmentBonus, 0.0f, 1.0f);
}

float UMingNegativeStrategies::CalculateDestructionPotential(const FBreakStructureParams& Params) const
{
    float BasePotential = 0.5f;
    
    // 破壞強度影響
    float IntensityBonus = Params.DestructionIntensity * 0.3f;
    
    // 結構弱點影響
    float WeaknessBonus = FMath::Min(Params.StructuralWeaknesses.Num() * 0.1f, 0.3f);
    
    // 精確打擊加成
    float PrecisionBonus = Params.bPrecisionStrike ? 0.1f : 0.0f;
    
    // 關鍵點數量
    float KeyPointBonus = FMath::Min(Params.KeyPoints.Num() * 0.05f, 0.15f);
    
    return FMath::Clamp(BasePotential + IntensityBonus + WeaknessBonus + PrecisionBonus + KeyPointBonus, 0.0f, 1.0f);
}

float UMingNegativeStrategies::CalculatePreservationEfficiency(const FPreservePeopleParams& Params) const
{
    float BaseEfficiency = 0.5f;
    
    // 保護等級影響
    float LevelBonus = Params.ProtectionLevel * 0.3f;
    
    // 資源分配影響
    float ResourceBonus = FMath::Clamp(Params.ResourceAllocation / 1000.0f, 0.0f, 0.15f);
    
    // 疏散計畫加成
    float EvacuationBonus = Params.bEvacuationPlan ? 0.1f : 0.0f;
    
    // 政治赦免加成
    float AmnestyBonus = Params.bPoliticalAmnesty ? 0.05f : 0.0f;
    
    return FMath::Clamp(BaseEfficiency + LevelBonus + ResourceBonus + EvacuationBonus + AmnestyBonus, 0.0f, 1.0f);
}

// 驗證功能實現
bool UMingNegativeStrategies::CanBreakSituation(const FBreakSituationParams& Params) const
{
    // 檢查基本條件
    if (Params.TargetSituationId.IsEmpty())
    {
        return false;
    }
    
    // 需要有弱點情報
    if (Params.Weaknesses.Num() == 0)
    {
        return false;
    }
    
    // 投資必須為正數
    if (Params.ResourceInvestment <= 0)
    {
        return false;
    }
    
    return true;
}

bool UMingNegativeStrategies::CanBreakStructure(const FBreakStructureParams& Params) const
{
    // 檢查基本條件
    if (Params.TargetStructureId.IsEmpty() || Params.StructureType.IsEmpty())
    {
        return false;
    }
    
    // 需要有結構弱點情報
    if (Params.StructuralWeaknesses.Num() == 0)
    {
        return false;
    }
    
    // 需要足夠的力量
    if (Params.RequiredForce <= 0)
    {
        return false;
    }
    
    return true;
}

bool UMingNegativeStrategies::CanPreservePeople(const FPreservePeopleParams& Params) const
{
    // 檢查基本條件
    if (Params.TargetNationId.IsEmpty() || Params.ProtectedPeopleIds.Num() == 0)
    {
        return false;
    }
    
    // 需要有安全區域
    if (Params.SafeZones.Num() == 0)
    {
        return false;
    }
    
    // 資源分配必須為正數
    if (Params.ResourceAllocation <= 0)
    {
        return false;
    }
    
    return true;
}

// 輔助計算功能
float UMingNegativeStrategies::CalculateWeaknessExploitation(const TArray<FString>& Weaknesses) const
{
    float BaseExploitation = 0.3f;
    float WeaknessBonus = FMath::Min(Weaknesses.Num() * 0.1f, 0.4f);
    return BaseExploitation + WeaknessBonus;
}

float UMingNegativeStrategies::CalculatePrecisionBonus(bool bPrecisionStrike, const TArray<FString>& KeyPoints) const
{
    if (!bPrecisionStrike)
    {
        return 0.0f;
    }
    
    float BaseBonus = 0.1f;
    float KeyPointBonus = FMath::Min(KeyPoints.Num() * 0.05f, 0.15f);
    return BaseBonus + KeyPointBonus;
}

float UMingNegativeStrategies::CalculateProtectionCoverage(const TArray<FString>& SafeZones) const
{
    float BaseCoverage = 0.3f;
    float ZoneBonus = FMath::Min(SafeZones.Num() * 0.15f, 0.5f);
    return BaseCoverage + ZoneBonus;
}

// 輔助功能
TArray<FString> UMingNegativeStrategies::IdentifyTargets(const FBreakSituationParams& Params) const
{
    TArray<FString> Targets;
    
    // 根據弱點識別目標
    for (const FString& Weakness : Params.Weaknesses)
    {
        if (Weakness == TEXT("防禦薄弱"))
        {
            Targets.Add(TEXT("前線要塞"));
        }
        else if (Weakness == TEXT("補給線過長"))
        {
            Targets.Add(TEXT("補給站"));
        }
        else if (Weakness == TEXT("士氣低落"))
        {
            Targets.Add(TEXT("指揮中心"));
        }
    }
    
    // 添加破壞目標
    for (const FString& SabotageTarget : Params.SabotageTargets)
    {
        if (!Targets.Contains(SabotageTarget))
        {
            Targets.Add(SabotageTarget);
        }
    }
    
    return Targets;
}

TArray<FString> UMingNegativeStrategies::AssessCollateralDamage(const FBreakStructureParams& Params) const
{
    TArray<FString> Damage;
    
    // 如果不是精確打擊，可能會有附帶損害
    if (!Params.bPrecisionStrike)
    {
        if (FMath::FRand() < 0.5f) Damage.Add(TEXT("周邊建築"));
        if (FMath::FRand() < 0.4f) Damage.Add(TEXT("平民設施"));
        if (FMath::FRand() < 0.3f) Damage.Add(TEXT("環境損害"));
    }
    else
    {
        // 精確打擊減少附帶損害
        if (FMath::FRand() < 0.1f) Damage.Add(TEXT("輕微周邊損害"));
    }
    
    return Damage;
}

TArray<FString> UMingNegativeStrategies::RecruitTalents(const TArray<FString>& ProtectedPeople) const
{
    TArray<FString> Talents;
    
    // 從受保護人口中招募人才（約20%的機率）
    for (const FString& Person : ProtectedPeople)
    {
        if (FMath::FRand() < 0.2f)
        {
            Talents.Add(Person);
        }
    }
    
    return Talents;
}
