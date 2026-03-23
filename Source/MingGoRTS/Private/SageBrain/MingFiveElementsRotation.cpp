#include "MingFiveElementsRotation.h"
#include "MingSupremeSageCommandSystem.h"

UMingFiveElementsRotation::UMingFiveElementsRotation()
    : CurrentPhase(EFiveElementPhase::None)
    , bEvilForcesSealed(false)
{
}

bool UMingFiveElementsRotation::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("五行輪轉器初始化"));
    
    CurrentPhase = EFiveElementPhase::None;
    bEvilForcesSealed = false;
    PhaseHistory.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("✓ 五行輪轉器初始化完成"));
    
    return true;
}

bool UMingFiveElementsRotation::StartWoodPhase(const FWoodPhaseConfig& Config)
{
    WoodConfig = Config;
    
    UE_LOG(LogTemp, Log, TEXT("=== 進入木階段（立名） ==="));
    UE_LOG(LogTemp, Log, TEXT("階段名稱: %s"), *Config.PhaseName);
    UE_LOG(LogTemp, Log, TEXT("預計時長: %.0f秒"), Config.Duration);
    UE_LOG(LogTemp, Log, TEXT("正當性目標: %.2f"), Config.LegitimacyTarget);
    
    CurrentPhase = EFiveElementPhase::Wood;
    PhaseStartTime = FDateTime::Now();
    bEvilForcesSealed = false;
    
    // 初始化木階段指標
    CurrentMetrics.Phase = EFiveElementPhase::Wood;
    CurrentMetrics.CurrentLegitimacy = 0.0f;
    
    PhaseHistory.Add(EFiveElementPhase::Wood);
    
    return true;
}

bool UMingFiveElementsRotation::StartFirePhase(const FFirePhaseConfig& Config)
{
    FireConfig = Config;
    
    UE_LOG(LogTemp, Log, TEXT("=== 進入火階段（造勢） ==="));
    UE_LOG(LogTemp, Log, TEXT("階段名稱: %s"), *Config.PhaseName);
    UE_LOG(LogTemp, Log, TEXT("造勢目標: %.2f"), Config.MomentumTarget);
    UE_LOG(LogTemp, Log, TEXT("邪燃料強度: %.2f"), Config.EvilFuelIntensity);
    
    // 檢查是否從木階段過渡
    if (CurrentPhase != EFiveElementPhase::Wood)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 未經木階段直接進入火階段，正當性可能不足"));
    }
    
    CurrentPhase = EFiveElementPhase::Fire;
    PhaseStartTime = FDateTime::Now();
    bEvilForcesSealed = false;
    
    CurrentMetrics.Phase = EFiveElementPhase::Fire;
    CurrentMetrics.CurrentMomentum = 0.0f;
    
    PhaseHistory.Add(EFiveElementPhase::Fire);
    
    return true;
}

bool UMingFiveElementsRotation::StartEarthPhase(const FEarthPhaseConfig& Config)
{
    EarthConfig = Config;
    
    UE_LOG(LogTemp, Log, TEXT("=== 進入土階段（收權） ==="));
    UE_LOG(LogTemp, Log, TEXT("階段名稱: %s"), *Config.PhaseName);
    
    // 檢查火是否太旺
    if (IsFireTooStrong())
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 火勢過旺，必須立即進入土階段封存"));
    }
    
    CurrentPhase = EFiveElementPhase::Earth;
    PhaseStartTime = FDateTime::Now();
    
    // 土階段關鍵：封存邪兵
    if (Config.bSealEvilForces)
    {
        SealEvilForces();
        UE_LOG(LogTemp, Log, TEXT("✓ 邪兵已封存"));
    }
    
    if (Config.bStopMomentum)
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 造勢已停止"));
    }
    
    if (Config.bConsolidatePower)
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 開始收權"));
    }
    
    CurrentMetrics.Phase = EFiveElementPhase::Earth;
    CurrentMetrics.PowerConsolidation = 0.0f;
    
    PhaseHistory.Add(EFiveElementPhase::Earth);
    
    UE_LOG(LogTemp, Log, TEXT("土階段目標: 收權 %.0f%%"), Config.PowerConsolidationTarget * 100);
    
    return true;
}

bool UMingFiveElementsRotation::StartMetalPhase(const FMetalPhaseConfig& Config)
{
    MetalConfig = Config;
    
    UE_LOG(LogTemp, Log, TEXT("=== 進入金階段（裁斷） ==="));
    UE_LOG(LogTemp, Log, TEXT("階段名稱: %s"), *Config.PhaseName);
    
    // 檢查是否有收尾設計
    if (!Config.bColdJudgment)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 金階段必須使用冷裁斷"));
    }
    
    CurrentPhase = EFiveElementPhase::Metal;
    PhaseStartTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("清除目標數: %d"), Config.EliminationTargets.Num());
    UE_LOG(LogTemp, Log, TEXT("裁斷嚴厲程度: %.2f"), Config.JudgmentSeverity);
    
    CurrentMetrics.Phase = EFiveElementPhase::Metal;
    CurrentMetrics.ClarityLevel = 0.0f;
    
    PhaseHistory.Add(EFiveElementPhase::Metal);
    
    return true;
}

bool UMingFiveElementsRotation::StartWaterPhase(const FWaterPhaseConfig& Config)
{
    WaterConfig = Config;
    
    UE_LOG(LogTemp, Log, TEXT("=== 進入水階段（留白） ==="));
    UE_LOG(LogTemp, Log, TEXT("階段名稱: %s"), *Config.PhaseName);
    
    CurrentPhase = EFiveElementPhase::Water;
    PhaseStartTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("不作為程度: %.2f"), Config.InactionLevel);
    
    if (Config.bHideStrength)
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 藏鋒模式啟動"));
    }
    
    if (Config.bLetEnemyExhaust)
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 讓敵自竭模式啟動"));
    }
    
    CurrentMetrics.Phase = EFiveElementPhase::Water;
    CurrentMetrics.HiddenStrength = 1.0f;
    
    PhaseHistory.Add(EFiveElementPhase::Water);
    
    return true;
}

bool UMingFiveElementsRotation::EndCurrentPhase()
{
    if (CurrentPhase == EFiveElementPhase::None)
    {
        return false;
    }
    
    FDateTime Now = FDateTime::Now();
    FTimespan Duration = Now - PhaseStartTime;
    
    UE_LOG(LogTemp, Log, TEXT("結束%s階段，持續時間: %.0f秒"),
        *UEnum::GetDisplayValueAsText(CurrentPhase).ToString(),
        Duration.GetTotalSeconds());
    
    // 土階段結束時確保邪兵封存
    if (CurrentPhase == EFiveElementPhase::Earth && !bEvilForcesSealed)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 土階段結束但邪兵未封存，強制封存"));
        SealEvilForces();
    }
    
    return true;
}

bool UMingFiveElementsRotation::ForceTransitionToPhase(EFiveElementPhase TargetPhase)
{
    UE_LOG(LogTemp, Warning, TEXT("強制轉換至%s階段"),
        *UEnum::GetDisplayValueAsText(TargetPhase).ToString());
    
    EndCurrentPhase();
    
    // 強制轉換跳過正常檢查
    CurrentPhase = TargetPhase;
    PhaseStartTime = FDateTime::Now();
    
    PhaseHistory.Add(TargetPhase);
    
    return true;
}

bool UMingFiveElementsRotation::RotateRighteousAndEvil(const FRotationConfig& Config)
{
    UE_LOG(LogTemp, Log, TEXT("正邪輪轉配置:"));
    UE_LOG(LogTemp, Log, TEXT("  正兵比例: %.0f%%"), Config.RighteousRatio * 100);
    UE_LOG(LogTemp, Log, TEXT("  邪兵比例: %.0f%%"), Config.EvilRatio * 100);
    
    // 檢查比例是否合理
    if (!IsForceRatioAppropriate())
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 正邪比例異常"));
    }
    
    // 根據當前階段調整
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        UE_LOG(LogTemp, Log, TEXT("木階段：正為旗，邪為輔"));
        break;
    case EFiveElementPhase::Fire:
        UE_LOG(LogTemp, Log, TEXT("火階段：正為旗，邪為燃料"));
        break;
    case EFiveElementPhase::Earth:
        UE_LOG(LogTemp, Log, TEXT("土階段：只用正兵，邪兵封存"));
        break;
    case EFiveElementPhase::Metal:
        UE_LOG(LogTemp, Log, TEXT("金階段：正兵冷裁斷"));
        break;
    case EFiveElementPhase::Water:
        UE_LOG(LogTemp, Log, TEXT("水階段：正兵養勢，邪兵深藏"));
        break;
    }
    
    return true;
}

bool UMingFiveElementsRotation::IsForceRatioAppropriate() const
{
    // 檢查當前正邪比例是否適合當前階段
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Earth:
        // 土階段應該純正
        return true;  // 邪兵應該已封存
    case EFiveElementPhase::Metal:
        // 金階段應該以正為主
        return true;
    default:
        return true;
    }
}

FRotationConfig UMingFiveElementsRotation::GetRecommendedRotationConfig() const
{
    FRotationConfig Config;
    
    // 根據當前階段推薦配置
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        Config.RighteousRatio = 0.8f;
        Config.EvilRatio = 0.2f;
        break;
    case EFiveElementPhase::Fire:
        Config.RighteousRatio = 0.7f;
        Config.EvilRatio = 0.3f;
        break;
    case EFiveElementPhase::Earth:
        Config.RighteousRatio = 1.0f;
        Config.EvilRatio = 0.0f;
        break;
    case EFiveElementPhase::Metal:
        Config.RighteousRatio = 0.9f;
        Config.EvilRatio = 0.1f;
        break;
    case EFiveElementPhase::Water:
        Config.RighteousRatio = 0.8f;
        Config.EvilRatio = 0.2f;
        break;
    }
    
    return Config;
}

bool UMingFiveElementsRotation::SealEvilForces()
{
    if (bEvilForcesSealed)
    {
        UE_LOG(LogTemp, Log, TEXT("邪兵已封存"));
        return true;
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== 封存邪兵 ==="));
    UE_LOG(LogTemp, Log, TEXT("斷開單線聯絡"));
    UE_LOG(LogTemp, Log, TEXT("銷毀痕跡"));
    UE_LOG(LogTemp, Log, TEXT("自毀機制啟動"));
    
    bEvilForcesSealed = true;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 邪兵已完全封存"));
    
    return true;
}

bool UMingFiveElementsRotation::UnsealEvilForces()
{
    // 只有在新的木火階段才能解封
    if (CurrentPhase != EFiveElementPhase::Wood && 
        CurrentPhase != EFiveElementPhase::Fire)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 非木火階段不能解封邪兵"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== 解封邪兵 ==="));
    UE_LOG(LogTemp, Log, TEXT("重新建立單線聯絡"));
    UE_LOG(LogTemp, Log, TEXT("啟動邪燃料"));
    
    bEvilForcesSealed = false;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 邪兵已解封"));
    
    return true;
}

bool UMingFiveElementsRotation::CanTransitionTo(EFiveElementPhase TargetPhase) const
{
    // 正常相生順序
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        return TargetPhase == EFiveElementPhase::Fire;
    case EFiveElementPhase::Fire:
        // 火可以正常到土，或者強制到土
        return TargetPhase == EFiveElementPhase::Earth;
    case EFiveElementPhase::Earth:
        return TargetPhase == EFiveElementPhase::Metal;
    case EFiveElementPhase::Metal:
        return TargetPhase == EFiveElementPhase::Water;
    case EFiveElementPhase::Water:
        return TargetPhase == EFiveElementPhase::Wood;
    default:
        return TargetPhase == EFiveElementPhase::Wood;
    }
}

FPhaseMetrics UMingFiveElementsRotation::GetCurrentPhaseMetrics() const
{
    return CurrentMetrics;
}

bool UMingFiveElementsRotation::IsCurrentPhaseComplete() const
{
    // 檢查當前階段目標是否達成
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        return CurrentMetrics.CurrentLegitimacy >= WoodConfig.LegitimacyTarget;
    case EFiveElementPhase::Fire:
        return CurrentMetrics.CurrentMomentum >= FireConfig.MomentumTarget;
    case EFiveElementPhase::Earth:
        return CurrentMetrics.PowerConsolidation >= EarthConfig.PowerConsolidationTarget;
    case EFiveElementPhase::Metal:
        return CurrentMetrics.ClarityLevel >= 0.8f;
    case EFiveElementPhase::Water:
        return true;  // 水階段自然過渡
    default:
        return false;
    }
}

EFiveElementPhase UMingFiveElementsRotation::GetRecommendedNextPhase() const
{
    return CalculateNaturalFlow();
}

float UMingFiveElementsRotation::EvaluatePhaseEffectiveness(EFiveElementPhase Phase) const
{
    // 基於歷史數據評估階段效果
    int32 PhaseCount = 0;
    float TotalEffectiveness = 0.0f;
    
    // 簡化實現
    return 0.7f;
}

bool UMingFiveElementsRotation::ApplyRighteousFlagEvilFuelPattern()
{
    if (CurrentPhase != EFiveElementPhase::Wood && 
        CurrentPhase != EFiveElementPhase::Fire)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 木火模式只適用於木火階段"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== 應用正旗邪燃料模式 ==="));
    UE_LOG(LogTemp, Log, TEXT("檯面：高舉正旗，堂堂正正"));
    UE_LOG(LogTemp, Log, TEXT("檯面下：邪燃料暗燃，不留痕"));
    
    return true;
}

bool UMingFiveElementsRotation::ApplyEarthSealPattern()
{
    if (CurrentPhase != EFiveElementPhase::Earth)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 土封存模式只適用於土階段"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== 應用土封存模式 ==="));
    UE_LOG(LogTemp, Log, TEXT("封邪：切斷邪兵所有聯絡"));
    UE_LOG(LogTemp, Log, TEXT("止勢：停止所有造勢行動"));
    UE_LOG(LogTemp, Log, TEXT("收權：將權力收歸中樞"));
    
    return SealEvilForces();
}

bool UMingFiveElementsRotation::ApplyMetalWaterPattern()
{
    if (CurrentPhase != EFiveElementPhase::Metal && 
        CurrentPhase != EFiveElementPhase::Water)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 金水模式只適用於金水階段"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== 應用金水模式 ==="));
    UE_LOG(LogTemp, Log, TEXT("金：冷裁斷，去除冗餘"));
    UE_LOG(LogTemp, Log, TEXT("水：不作為，讓敵自竭"));
    
    return true;
}

bool UMingFiveElementsRotation::IsEarthPhaseForced() const
{
    // 檢查是否需要強制進入土階段
    return IsFireTooStrong();
}

EFiveElementPhase UMingFiveElementsRotation::CalculateNaturalFlow() const
{
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        return EFiveElementPhase::Fire;
    case EFiveElementPhase::Fire:
        return EFiveElementPhase::Earth;
    case EFiveElementPhase::Earth:
        return EFiveElementPhase::Metal;
    case EFiveElementPhase::Metal:
        return EFiveElementPhase::Water;
    case EFiveElementPhase::Water:
        return EFiveElementPhase::Wood;
    default:
        return EFiveElementPhase::Wood;
    }
}

void UMingFiveElementsRotation::UpdatePhaseMetrics()
{
    // 更新當前階段指標
    FDateTime Now = FDateTime::Now();
    FTimespan Elapsed = Now - PhaseStartTime;
    float Progress = 0.0f;
    
    switch (CurrentPhase)
    {
    case EFiveElementPhase::Wood:
        Progress = FMath::Min(1.0f, (float)(Elapsed.GetTotalSeconds() / WoodConfig.Duration));
        CurrentMetrics.CurrentLegitimacy = Progress * WoodConfig.LegitimacyTarget;
        break;
    case EFiveElementPhase::Fire:
        Progress = FMath::Min(1.0f, (float)(Elapsed.GetTotalSeconds() / FireConfig.Duration));
        CurrentMetrics.CurrentMomentum = Progress * FireConfig.MomentumTarget;
        break;
    case EFiveElementPhase::Earth:
        Progress = FMath::Min(1.0f, (float)(Elapsed.GetTotalSeconds() / EarthConfig.Duration));
        CurrentMetrics.PowerConsolidation = Progress * EarthConfig.PowerConsolidationTarget;
        break;
    }
}

bool UMingFiveElementsRotation::IsFireTooStrong() const
{
    // 檢查火勢是否過旺
    if (CurrentPhase != EFiveElementPhase::Fire)
    {
        return false;
    }
    
    FDateTime Now = FDateTime::Now();
    FTimespan Elapsed = Now - PhaseStartTime;
    
    // 火階段時間過長，或者熱度超過上限
    bool bTooLong = Elapsed.GetTotalSeconds() > FireConfig.Duration * 1.5f;
    bool bTooHot = CurrentMetrics.CurrentMomentum > FireConfig.MaxTemperature;
    
    return bTooLong || bTooHot;
}
