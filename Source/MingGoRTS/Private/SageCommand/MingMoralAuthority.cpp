// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingMoralAuthority.h"

UMingMoralAuthority::UMingMoralAuthority()
    : bIsInitialized(false)
    , HighRiskThreshold(70)
    , MaxActiveSymptoms(5)
    , SymptomExpirationTime(300.0f)
    , CachedFallValue(0)
    , CachedFallThreshold(100)
{
}

void UMingMoralAuthority::InitializeMoralAuthority()
{
    if (bIsInitialized)
    {
        return;
    }

    ActiveSymptoms.Empty();
    SymptomHistory.Empty();

    bIsInitialized = true;
}

bool UMingMoralAuthority::PerformMoralCheck()
{
    if (!bIsInitialized)
    {
        return false;
    }

    // 清理過期徵象
    FDateTime CurrentTime = FDateTime::Now();
    for (int32 i = ActiveSymptoms.Num() - 1; i >= 0; --i)
    {
        FTimespan Elapsed = CurrentTime - ActiveSymptoms[i].DetectionTime;
        if (Elapsed.GetTotalSeconds() > SymptomExpirationTime)
        {
            ActiveSymptoms.RemoveAt(i);
        }
    }

    // 檢查系統健康狀況
    if (!IsHealthy())
    {
        OnMoralWarning.Broadcast(TEXT("道德權威系統處於異常狀態，建議立即進行正策修復。"));
        return false;
    }

    return true;
}

TArray<FFallSymptomData> UMingMoralAuthority::DetectFallSymptoms(int32 CurrentFallValue, int32 FallThreshold)
{
    CachedFallValue = CurrentFallValue;
    CachedFallThreshold = FallThreshold;

    TArray<FFallSymptomData> NewSymptoms;

    // 計算風險值
    int32 RiskValue = CalculateRiskValue(CurrentFallValue, FallThreshold);

    // 根據風險值檢測各種徵象
    if (RiskValue >= 30)
    {
        // 檢測內部始疑 (風險達到30%時可能出現)
        if (DetectInternalDoubt(0, RiskValue / 10.0f))
        {
            FFallSymptomData Symptom;
            Symptom.SymptomType = EFallSymptomType::InternalDoubt;
            Symptom.Description = GetSymptomDescription(EFallSymptomType::InternalDoubt);
            Symptom.DetectionTime = FDateTime::Now();
            Symptom.Severity = FMath::Clamp(RiskValue / 10, 1, 10);
            NewSymptoms.Add(Symptom);
            UpdateSymptomsList(EFallSymptomType::InternalDoubt, Symptom.Description, Symptom.Severity);
        }
    }

    if (RiskValue >= 50)
    {
        // 檢測敵轉異常 (風險達到50%時可能出現)
        if (DetectEnemyAbnormal(RiskValue >= 60))
        {
            FFallSymptomData Symptom;
            Symptom.SymptomType = EFallSymptomType::EnemyAbnormal;
            Symptom.Description = GetSymptomDescription(EFallSymptomType::EnemyAbnormal);
            Symptom.DetectionTime = FDateTime::Now();
            Symptom.Severity = FMath::Clamp(RiskValue / 10, 1, 10);
            NewSymptoms.Add(Symptom);
            UpdateSymptomsList(EFallSymptomType::EnemyAbnormal, Symptom.Description, Symptom.Severity);
        }
    }

    if (RiskValue >= 70)
    {
        // 檢測事過露跡 (風險達到70%時可能出現)
        if (DetectEvidenceExposed(RiskValue / 100.0f))
        {
            FFallSymptomData Symptom;
            Symptom.SymptomType = EFallSymptomType::EvidenceExposed;
            Symptom.Description = GetSymptomDescription(EFallSymptomType::EvidenceExposed);
            Symptom.DetectionTime = FDateTime::Now();
            Symptom.Severity = FMath::Clamp(RiskValue / 10, 1, 10);
            NewSymptoms.Add(Symptom);
            UpdateSymptomsList(EFallSymptomType::EvidenceExposed, Symptom.Description, Symptom.Severity);
        }
    }

    if (RiskValue >= 80)
    {
        // 檢測勢反過盛和身人心變 (風險達到80%時可能出現)
        if (DetectOverConfidence(0, 0))
        {
            FFallSymptomData Symptom;
            Symptom.SymptomType = EFallSymptomType::OverConfidence;
            Symptom.Description = GetSymptomDescription(EFallSymptomType::OverConfidence);
            Symptom.DetectionTime = FDateTime::Now();
            Symptom.Severity = FMath::Clamp(RiskValue / 10, 1, 10);
            NewSymptoms.Add(Symptom);
            UpdateSymptomsList(EFallSymptomType::OverConfidence, Symptom.Description, Symptom.Severity);
        }

        if (DetectHeartChange(RiskValue / 100.0f))
        {
            FFallSymptomData Symptom;
            Symptom.SymptomType = EFallSymptomType::HeartChange;
            Symptom.Description = GetSymptomDescription(EFallSymptomType::HeartChange);
            Symptom.DetectionTime = FDateTime::Now();
            Symptom.Severity = FMath::Clamp(RiskValue / 10, 1, 10);
            NewSymptoms.Add(Symptom);
            UpdateSymptomsList(EFallSymptomType::HeartChange, Symptom.Description, Symptom.Severity);
        }
    }

    // 廣播新檢測到的徵象
    for (const FFallSymptomData& Symptom : NewSymptoms)
    {
        OnFallSymptomDetected.Broadcast(Symptom);
    }

    return NewSymptoms;
}

bool UMingMoralAuthority::CheckMoralBoundaries(const FMoralBoundary& ProposedAction)
{
    bool bAllRespected = true;

    if (!ProposedAction.bNoHarmToInnocents)
    {
        OnMoralWarning.Broadcast(TEXT("警告：行為可能傷害無辜者，違反道德邊界。"));
        bAllRespected = false;
    }

    if (!ProposedAction.bNoTotalAnnihilation)
    {
        OnMoralWarning.Broadcast(TEXT("警告：行為可能導致玉石俱焚，違反道德邊界。"));
        bAllRespected = false;
    }

    if (!ProposedAction.bNoViolationOfHeavenlyPrinciples)
    {
        OnMoralWarning.Broadcast(TEXT("警告：行為違反天地大義，可能失去天時。"));
        bAllRespected = false;
    }

    if (!ProposedAction.bNoPersonalGainAbuse)
    {
        OnMoralWarning.Broadcast(TEXT("警告：行為可能為私利濫用權力，違反道德邊界。"));
        bAllRespected = false;
    }

    if (!bAllRespected)
    {
        OnMoralBoundaryViolated.Broadcast(ProposedAction);
    }

    return bAllRespected;
}

int32 UMingMoralAuthority::AssessFallRisk(int32 CurrentFallValue, int32 FallThreshold, int32 ConsecutiveEvilUses)
{
    int32 BaseRisk = CalculateRiskValue(CurrentFallValue, FallThreshold);
    
    // 連續使用逆策增加風險
    int32 ConsecutiveBonus = FMath::Min(ConsecutiveEvilUses * 5, 30);
    
    // 當前徵象數量影響風險
    int32 SymptomBonus = ActiveSymptoms.Num() * 3;
    
    return FMath::Clamp(BaseRisk + ConsecutiveBonus + SymptomBonus, 0, 100);
}

bool UMingMoralAuthority::ResolveSymptom(EFallSymptomType SymptomType)
{
    for (FFallSymptomData& Symptom : ActiveSymptoms)
    {
        if (Symptom.SymptomType == SymptomType && !Symptom.bIsResolved)
        {
            Symptom.bIsResolved = true;
            
            // 移動到歷史記錄
            SymptomHistory.Add(Symptom);
            
            return true;
        }
    }
    
    return false;
}

bool UMingMoralAuthority::IsHealthy() const
{
    // 系統健康狀況檢查
    // 1. 徵象數量不超過上限
    // 2. 沒有嚴重徵象（嚴重度>=8）
    
    if (ActiveSymptoms.Num() >= MaxActiveSymptoms)
    {
        return false;
    }

    for (const FFallSymptomData& Symptom : ActiveSymptoms)
    {
        if (Symptom.Severity >= 8 && !Symptom.bIsResolved)
        {
            return false;
        }
    }

    return true;
}

bool UMingMoralAuthority::IsFallRiskHigh() const
{
    int32 RiskValue = CalculateRiskValue(CachedFallValue, CachedFallThreshold);
    return RiskValue >= HighRiskThreshold;
}

FString UMingMoralAuthority::GetSymptomDescription(EFallSymptomType SymptomType) const
{
    switch (SymptomType)
    {
    case EFallSymptomType::InternalDoubt:
        return TEXT("內部始疑：己方眾心離散，互生猜忌，逆氣已洩。建議使用正策恢復部隊忠誠。");
    case EFallSymptomType::EnemyAbnormal:
        return TEXT("敵轉異常：敵忽靜、忽和、忽讓，已察我逆手，反設陷阱。建議改變策略組合。");
    case EFallSymptomType::EvidenceExposed:
        return TEXT("事過露跡：舊逆之事漸為人知、為敵所證，陰轉陽而禍起。建議進行情報掩蓋。");
    case EFallSymptomType::OverConfidence:
        return TEXT("勢反過盛：逆勝過多，眾以為常，欲永用逆，正道盡喪。建議立即轉用正策固本。");
    case EFallSymptomType::HeartChange:
        return TEXT("身人心變：用逆者心漸酷、漸孤、漸無忌，天理已失。這是最危險的徵象！");
    default:
        return TEXT("未知徵象");
    }
}

FString UMingMoralAuthority::GetMoralAdvice() const
{
    if (ActiveSymptoms.Num() == 0)
    {
        return TEXT("道權監測正常，當前無墮落風險。請繼續保持正逆平衡。");
    }

    FString Advice = TEXT("道權發出警告：\n");
    
    for (const FFallSymptomData& Symptom : ActiveSymptoms)
    {
        if (!Symptom.bIsResolved)
        {
            Advice += FString::Printf(TEXT("• %s (嚴重度: %d/10)\n"), 
                *GetSymptomDescription(Symptom.SymptomType), Symptom.Severity);
        }
    }

    Advice += TEXT("\n建議：立即執行正策修復，必要時進行贖罪。");

    return Advice;
}

bool UMingMoralAuthority::DetectInternalDoubt(int32 ConsecutiveEvilUses, float LoyaltyDrop)
{
    // 簡化的檢測邏輯：基於連續使用逆策和忠誠度下降
    float DetectionChance = (ConsecutiveEvilUses * 0.1f) + (LoyaltyDrop * 0.5f);
    return FMath::FRand() < DetectionChance;
}

bool UMingMoralAuthority::DetectEnemyAbnormal(bool bEnemyDefensiveStance)
{
    // 檢測敵人是否進入防禦態勢
    return bEnemyDefensiveStance;
}

bool UMingMoralAuthority::DetectEvidenceExposed(float ExposureRisk)
{
    // 檢測逆策被揭露的風險
    return FMath::FRand() < ExposureRisk;
}

bool UMingMoralAuthority::DetectOverConfidence(int32 TotalEvilUses, int32 TotalRighteousUses)
{
    // 檢測是否過度依賴逆策
    if (TotalEvilUses + TotalRighteousUses == 0)
    {
        return false;
    }
    
    float EvilRatio = (float)TotalEvilUses / (TotalEvilUses + TotalRighteousUses);
    return EvilRatio > 0.7f && TotalEvilUses > 5;
}

bool UMingMoralAuthority::DetectHeartChange(float MoralDeviation)
{
    // 檢測指揮者心態變化
    return MoralDeviation > 0.8f;
}

void UMingMoralAuthority::UpdateSymptomsList(EFallSymptomType SymptomType, const FString& Description, int32 Severity)
{
    // 檢查是否已存在相同類型的未解決徵象
    for (FFallSymptomData& ExistingSymptom : ActiveSymptoms)
    {
        if (ExistingSymptom.SymptomType == SymptomType && !ExistingSymptom.bIsResolved)
        {
            // 更新嚴重程度
            ExistingSymptom.Severity = FMath::Max(ExistingSymptom.Severity, Severity);
            ExistingSymptom.DetectionTime = FDateTime::Now();
            return;
        }
    }

    // 添加新徵象
    if (ActiveSymptoms.Num() < MaxActiveSymptoms)
    {
        FFallSymptomData NewSymptom;
        NewSymptom.SymptomType = SymptomType;
        NewSymptom.Description = Description;
        NewSymptom.DetectionTime = FDateTime::Now();
        NewSymptom.Severity = Severity;
        NewSymptom.bIsResolved = false;
        
        ActiveSymptoms.Add(NewSymptom);
    }
}

int32 UMingMoralAuthority::CalculateRiskValue(int32 CurrentFallValue, int32 FallThreshold) const
{
    if (FallThreshold <= 0)
    {
        return 0;
    }

    return FMath::Clamp((CurrentFallValue * 100) / FallThreshold, 0, 100);
}
