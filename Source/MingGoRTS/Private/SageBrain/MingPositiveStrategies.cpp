#include "SageBrain/MingPositiveStrategies.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UMingPositiveStrategies::UMingPositiveStrategies()
{
    BaseSuccessRate = 0.7f;
    InitializeDefaultValues();
}

void UMingPositiveStrategies::InitializeDefaultValues()
{
    NationHistory.Empty();
    SystemHistory.Empty();
    PeopleHistory.Empty();
}

FEstablishNationResult UMingPositiveStrategies::ExecuteEstablishNation(const FEstablishNationParams& Params)
{
    FEstablishNationResult Result;
    
    OnEstablishNationStarted(Params);
    
    // 檢查是否可以執行
    if (!CanEstablishNation(Params))
    {
        Result.ExecutionLog = TEXT("條件不足，無法立國");
        return Result;
    }
    
    // 計算各項潛力
    float TerritoryValue = CalculateTerritoryValue(Params.InitialTerritories);
    float ResourceEfficiency = CalculateResourceEfficiency(Params.StartingResources, Params.InitialPopulation);
    float Stability = CalculateNationStability(Params);
    
    // 判斷成功與否
    float SuccessChance = BaseSuccessRate * (Stability * 0.5f + ResourceEfficiency * 0.3f + TerritoryValue * 0.2f);
    Result.bSuccess = (FMath::FRand() < SuccessChance);
    
    if (Result.bSuccess)
    {
        // 生成國家ID
        Result.EstablishedNationId = FString::Printf(TEXT("Nation_%s_%s"), 
            *Params.NationName, *FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S")));
        
        Result.StabilityScore = Stability * 100.0f;
        Result.EconomicPotential = ResourceEfficiency * 100.0f;
        Result.MilitaryPotential = (TerritoryValue * 0.6f + ResourceEfficiency * 0.4f) * 100.0f;
        Result.EstablishedInstitutions = GenerateInstitutions(Params);
        Result.ExecutionLog = FString::Printf(TEXT("成功建立國家：%s"), *Params.NationName);
        
        // 記錄歷史
        NationHistory.Add(Result.EstablishedNationId, Result);
        
        UE_LOG(LogTemp, Log, TEXT("Nation established: %s (ID: %s)"), 
               *Params.NationName, *Result.EstablishedNationId);
    }
    else
    {
        Result.ExecutionLog = FString::Printf(TEXT("立國失敗：%s"), *Params.NationName);
        UE_LOG(LogTemp, Warning, TEXT("Failed to establish nation: %s"), *Params.NationName);
    }
    
    OnNationEstablished.Broadcast(Result);
    return Result;
}

FEstablishSystemResult UMingPositiveStrategies::ExecuteEstablishSystem(const FEstablishSystemParams& Params)
{
    FEstablishSystemResult Result;
    
    OnEstablishSystemStarted(Params);
    
    // 檢查是否可以執行
    if (!CanEstablishSystem(Params))
    {
        Result.ExecutionLog = TEXT("條件不足，無法立制");
        return Result;
    }
    
    // 計算效率
    float Efficiency = CalculateSystemEfficiency(Params);
    float Compatibility = CalculateSystemCompatibility(TEXT("君主制"), Params.SystemType); // 假設當前政府類型
    
    // 判斷成功與否
    float SuccessChance = BaseSuccessRate * (Efficiency * 0.6f + Compatibility * 0.4f);
    Result.bSuccess = (FMath::FRand() < SuccessChance);
    
    if (Result.bSuccess)
    {
        // 生成系統ID
        Result.EstablishedSystemId = FString::Printf(TEXT("System_%s_%s"), 
            *Params.SystemType, *FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S")));
        
        Result.ActualEfficiency = Efficiency * (0.8f + FMath::FRand() * 0.4f); // 實際效率有浮動
        Result.PublicAcceptance = 0.5f + (Efficiency * 0.5f);
        Result.AdministrativeCost = Params.ImplementationCost * (1.0f / Result.ActualEfficiency);
        Result.ImplementedLaws = GenerateLaws(Params);
        Result.ExecutionLog = FString::Printf(TEXT("成功建立制度：%s"), *Params.SystemType);
        
        // 記錄歷史
        SystemHistory.Add(Result.EstablishedSystemId, Result);
        
        UE_LOG(LogTemp, Log, TEXT("System established: %s (ID: %s)"), 
               *Params.SystemType, *Result.EstablishedSystemId);
    }
    else
    {
        Result.ExecutionLog = FString::Printf(TEXT("立制失敗：%s"), *Params.SystemType);
        UE_LOG(LogTemp, Warning, TEXT("Failed to establish system: %s"), *Params.SystemType);
    }
    
    OnSystemEstablished.Broadcast(Result);
    return Result;
}

FEstablishPeopleResult UMingPositiveStrategies::ExecuteEstablishPeople(const FEstablishPeopleParams& Params)
{
    FEstablishPeopleResult Result;
    
    OnEstablishPeopleStarted(Params);
    
    // 檢查是否可以執行
    if (!CanEstablishPeople(Params))
    {
        Result.ExecutionLog = TEXT("條件不足，無法立人");
        return Result;
    }
    
    // 計算發展潛力
    float Development = CalculatePeopleDevelopment(Params);
    float TeachingEffect = CalculateTeachingEffectiveness(Params.MoralTeachings);
    
    // 判斷成功與否
    float SuccessChance = BaseSuccessRate * (Development * 0.5f + TeachingEffect * 0.3f + Params.LoyaltyBuilding * 0.2f);
    Result.bSuccess = (FMath::FRand() < SuccessChance);
    
    if (Result.bSuccess)
    {
        Result.TrainedPeople = FMath::Min(Params.RecruitmentQuota, 
            static_cast<int32>(Params.TargetPeopleIds.Num() * (0.7f + FMath::FRand() * 0.3f)));
        
        Result.AverageSkillLevel = Development * (0.6f + FMath::FRand() * 0.4f) * 100.0f;
        Result.AverageLoyalty = Params.LoyaltyBuilding * (0.8f + FMath::FRand() * 0.4f) * 100.0f;
        Result.DevelopedTalents = IdentifyTalents(Params.TargetPeopleIds);
        Result.SocialStabilityImpact = (Result.AverageLoyalty * 0.6f + Result.AverageSkillLevel * 0.4f) / 100.0f;
        Result.ExecutionLog = FString::Printf(TEXT("成功培養人才：%d人"), Result.TrainedPeople);
        
        // 記錄歷史
        FString RecordId = FString::Printf(TEXT("People_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S")));
        PeopleHistory.Add(RecordId, Result);
        
        UE_LOG(LogTemp, Log, TEXT("People development completed: %d trained"), Result.TrainedPeople);
    }
    else
    {
        Result.ExecutionLog = TEXT("立人失敗");
        UE_LOG(LogTemp, Warning, TEXT("Failed to establish people development"));
    }
    
    OnPeopleEstablished.Broadcast(Result);
    return Result;
}

// 計算功能實現
float UMingPositiveStrategies::CalculateNationStability(const FEstablishNationParams& Params) const
{
    float BaseStability = 0.5f;
    
    // 領地越多越穩定
    float TerritoryBonus = FMath::Min(Params.InitialTerritories.Num() * 0.05f, 0.3f);
    
    // 資源與人口比例
    float ResourcePerCapita = Params.StartingResources / FMath::Max(Params.InitialPopulation, 1);
    float ResourceBonus = FMath::Clamp(ResourcePerCapita / 100.0f, 0.0f, 0.2f);
    
    // 政策影響
    float PolicyBonus = Params.NationalPolicies.Num() * 0.02f;
    
    return FMath::Clamp(BaseStability + TerritoryBonus + ResourceBonus + PolicyBonus, 0.0f, 1.0f);
}

float UMingPositiveStrategies::CalculateSystemEfficiency(const FEstablishSystemParams& Params) const
{
    float BaseEfficiency = Params.ExpectedEfficiency;
    
    // 官員數量影響
    float OfficialFactor = FMath::Clamp(Params.RequiredOfficials / 20.0f, 0.5f, 1.5f);
    
    // 系統組件完整度
    float ComponentBonus = Params.SystemComponents.Num() * 0.05f;
    
    // 法律支持
    float LawBonus = Params.SupportingLaws.Num() * 0.03f;
    
    return FMath::Clamp(BaseEfficiency * OfficialFactor + ComponentBonus + LawBonus, 0.0f, 1.0f);
}

float UMingPositiveStrategies::CalculatePeopleDevelopment(const FEstablishPeopleParams& Params) const
{
    float BaseDevelopment = 0.4f;
    
    // 投資影響
    float InvestmentBonus = FMath::Clamp(Params.TrainingInvestment / 1000.0f, 0.0f, 0.3f);
    
    // 技能優先級明確度
    float SkillBonus = Params.SkillPriorities.Num() * 0.04f;
    
    // 道德教育
    float MoralBonus = Params.MoralTeachings.Num() * 0.03f;
    
    return FMath::Clamp(BaseDevelopment + InvestmentBonus + SkillBonus + MoralBonus, 0.0f, 1.0f);
}

// 驗證功能實現
bool UMingPositiveStrategies::CanEstablishNation(const FEstablishNationParams& Params) const
{
    // 檢查基本條件
    if (Params.NationName.IsEmpty() || Params.CapitalCity.IsEmpty())
    {
        return false;
    }
    
    // 需要有初始領地
    if (Params.InitialTerritories.Num() == 0)
    {
        return false;
    }
    
    // 資源和人口必須為正數
    if (Params.StartingResources <= 0 || Params.InitialPopulation <= 0)
    {
        return false;
    }
    
    return true;
}

bool UMingPositiveStrategies::CanEstablishSystem(const FEstablishSystemParams& Params) const
{
    // 檢查基本條件
    if (Params.SystemType.IsEmpty() || Params.TargetNationId.IsEmpty())
    {
        return false;
    }
    
    // 需要有系統組件
    if (Params.SystemComponents.Num() == 0)
    {
        return false;
    }
    
    // 成本必須為正數
    if (Params.ImplementationCost <= 0)
    {
        return false;
    }
    
    return true;
}

bool UMingPositiveStrategies::CanEstablishPeople(const FEstablishPeopleParams& Params) const
{
    // 檢查基本條件
    if (Params.TargetPeopleIds.Num() == 0)
    {
        return false;
    }
    
    // 投資必須為正數
    if (Params.TrainingInvestment <= 0)
    {
        return false;
    }
    
    return true;
}

// 輔助計算功能
float UMingPositiveStrategies::CalculateTerritoryValue(const TArray<FString>& Territories) const
{
    float BaseValue = 0.3f;
    float SizeBonus = FMath::Min(Territories.Num() * 0.1f, 0.5f);
    return BaseValue + SizeBonus;
}

float UMingPositiveStrategies::CalculateResourceEfficiency(float Resources, int32 Population) const
{
    if (Population <= 0)
    {
        return 0.0f;
    }
    
    float PerCapita = Resources / Population;
    return FMath::Clamp(PerCapita / 0.1f, 0.1f, 1.0f); // 假設理想比例為0.1資源/人
}

float UMingPositiveStrategies::CalculateSystemCompatibility(const FString& GovernmentType, const FString& SystemType) const
{
    // 簡化的兼容性計算
    if (GovernmentType == TEXT("君主制"))
    {
        if (SystemType == TEXT("行政系統") || SystemType == TEXT("軍事系統"))
        {
            return 0.9f;
        }
    }
    else if (GovernmentType == TEXT("共和制"))
    {
        if (SystemType == TEXT("議會系統") || SystemType == TEXT("選舉系統"))
        {
            return 0.9f;
        }
    }
    
    return 0.6f; // 默認兼容性
}

float UMingPositiveStrategies::CalculateTeachingEffectiveness(const TArray<FString>& MoralTeachings) const
{
    float BaseEffectiveness = 0.3f;
    float TeachingBonus = MoralTeachings.Num() * 0.08f;
    return FMath::Clamp(BaseEffectiveness + TeachingBonus, 0.0f, 1.0f);
}

// 生成功能
TArray<FString> UMingPositiveStrategies::GenerateInstitutions(const FEstablishNationParams& Params) const
{
    TArray<FString> Institutions;
    
    Institutions.Add(TEXT("中央政府"));
    Institutions.Add(TEXT("地方政府"));
    Institutions.Add(TEXT("軍事機構"));
    
    if (Params.StartingResources > 5000)
    {
        Institutions.Add(TEXT("經濟部門"));
    }
    
    if (Params.InitialPopulation > 50000)
    {
        Institutions.Add(TEXT("教育部門"));
        Institutions.Add(TEXT("文化機構"));
    }
    
    return Institutions;
}

TArray<FString> UMingPositiveStrategies::GenerateLaws(const FEstablishSystemParams& Params) const
{
    TArray<FString> Laws;
    
    Laws.Add(TEXT("基本法"));
    Laws.Add(TEXT("行政法"));
    
    if (Params.SystemType == TEXT("行政系統"))
    {
        Laws.Add(TEXT("官僚任命法"));
        Laws.Add(TEXT":"政效率法"));
    }
    else if (Params.SystemType == TEXT("軍事系統"))
    {
        Laws.Add(TEXT("徵兵法"));
        Laws.Add(TEXT("軍事指揮法"));
    }
    
    // 添加支持法律
    for (const FString& Law : Params.SupportingLaws)
    {
        if (!Laws.Contains(Law))
        {
            Laws.Add(Law);
        }
    }
    
    return Laws;
}

TArray<FString> UMingPositiveStrategies::IdentifyTalents(const TArray<FString>& PeopleIds) const
{
    TArray<FString> Talents;
    
    // 隨機識別人才（約30%的機率）
    for (const FString& PersonId : PeopleIds)
    {
        if (FMath::FRand() < 0.3f)
        {
            Talents.Add(PersonId);
        }
    }
    
    return Talents;
}
