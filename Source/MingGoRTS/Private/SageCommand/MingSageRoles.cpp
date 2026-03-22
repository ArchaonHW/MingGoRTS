#include "SageCommand/MingSageRoles.h"
#include "SageCommand/MingMoralAuthority.h"
#include "SageCommand/MingYinYangFiveElements.h"
#include "SageCommand/MingSixStrategies.h"
#include "SageCommand/MingCommandAuthority.h"
#include "SageCommand/MingFiveElementsRotation.h"

UMingSageRoles::UMingSageRoles()
{
    bSystemInitialized = false;
}

bool UMingSageRoles::Initialize()
{
    if (bSystemInitialized)
    {
        return true;
    }

    // 初始化角色規範
    InitializeRoleStandards();
    
    // 初始化角色特徵
    InitializeRoleCharacteristics();

    bSystemInitialized = true;
    return true;
}

FSageRoleCharacteristics UMingSageRoles::DefineTrueSage()
{
    FSageRoleCharacteristics TrueSage;
    TrueSage.RoleType = ESageRoleType::TrueSage;
    TrueSage.RoleName = TEXT("至聖者");
    TrueSage.RoleDescription = TEXT("完美體現聖者指揮學的至高境界，道德與智慧的化身");
    TrueSage.StandardLevel = ERoleStandardLevel::Perfect;

    // 道德權威特徵 - 完美道德
    TrueSage.MoralAuthorityLevel = 100.0f;
    TrueSage.LegitimacyScore = 100.0f;
    TrueSage.bHasFallPrevention = true;

    // 陰陽五行特徵 - 完美平衡
    TrueSage.PrimaryAttribute = EYinYangAttribute::BalancedYinYang;
    TrueSage.PrimaryElement = EFiveElements::Earth;
    TrueSage.HarmonyLevel = 100.0f;
    TrueSage.bUsesEvilRotation = false;

    // 六策特徵 - 大師級洞察
    TrueSage.PreferredStrategy = ESixStrategyType::HeavenEarthHumanStrategy;
    TrueSage.MaxComplexity = EStrategyComplexity::Master;
    TrueSage.StrategicInsight = 100.0f;

    // 指揮權力特徵 - 道德權威為主
    TrueSage.PrimaryAuthority = ECommandAuthorityType::MoralAuthority;
    TrueSage.CommandEffectiveness = 100.0f;
    TrueSage.InfluenceRadius = 100.0f;

    return TrueSage;
}

FSageRoleCharacteristics UMingSageRoles::DefineFalseSage()
{
    FSageRoleCharacteristics FalseSage;
    FalseSage.RoleType = ESageRoleType::FalseSage;
    FalseSage.RoleName = TEXT("偽聖者");
    FalseSage.RoleDescription = TEXT("表面聖者，內心邪惡；以道德之名行邪惡之實");
    FalseSage.StandardLevel = ERoleStandardLevel::Corrupted;

    // 道德權威特徵 - 虛假道德
    FalseSage.MoralAuthorityLevel = 30.0f;  // 表面高，實際低
    FalseSage.LegitimacyScore = 60.0f;     // 依靠欺騙維持
    FalseSage.bHasFallPrevention = false;   // 無防墮機制

    // 陰陽五行特徵 - 表面平衡，實則陰盛
    FalseSage.PrimaryAttribute = EYinYangAttribute::Evil;  // 內心邪惡
    FalseSage.PrimaryElement = EFiveElements::Water;        // 陰性元素
    FalseSage.HarmonyLevel = 25.0f;                       // 假和諧
    FalseSage.bUsesEvilRotation = true;                    // 使用邪惡輪轉

    // 六策特徵 - 善於陰謀
    FalseSage.PreferredStrategy = ESixStrategyType::HumanStrategy;  // 善用人心
    FalseSage.MaxComplexity = EStrategyComplexity::Complex;        // 複雜但不達大師
    FalseSage.StrategicInsight = 70.0f;                          // 有洞察但有限

    // 指揮權力特徵 - 依靠魅力和欺騙
    FalseSage.PrimaryAuthority = ECommandAuthorityType::CharismaticAuthority;
    FalseSage.CommandEffectiveness = 60.0f;  // 表面有效，實則有限
    FalseSage.InfluenceRadius = 80.0f;       // 影響範圍較大但不穩定

    return FalseSage;
}

FSageRoleCharacteristics UMingSageRoles::DefineDemonKing()
{
    FSageRoleCharacteristics DemonKing;
    DemonKing.RoleType = ESageRoleType::DemonKing;
    DemonKing.RoleName = TEXT("魔王");
    DemonKing.RoleDescription = TEXT("公開邪惡，追求絕對權力；以暴力和恐懼統治");
    DemonKing.StandardLevel = ERoleStandardLevel::Corrupted;

    // 道德權威特徵 - 完全邪惡
    DemonKing.MoralAuthorityLevel = 0.0f;      // 無道德權威
    DemonKing.LegitimacyScore = 10.0f;         // 僅靠暴力維持
    DemonKing.bHasFallPrevention = false;       // 無防墮機制

    // 陰陽五行特徵 - 純邪惡
    DemonKing.PrimaryAttribute = EAlignmentAttribute::Evil;  // 純邪惡
    DemonKing.PrimaryElement = EFiveElements::Fire;         // 火主攻擊
    DemonKing.HarmonyLevel = 0.0f;                        // 完全不和諧
    DemonKing.bUsesEvilRotation = true;                   // 使用邪惡輪轉

    // 六策特徵 - 專注攻擊
    DemonKing.PreferredStrategy = ESixStrategyType::HeavenStrategy;  // 天道暴力
    DemonKing.MaxComplexity = EStrategyComplexity::Moderate;         // 簡單直接
    DemonKing.StrategicInsight = 40.0f;                            // 有限洞察

    // 指揮權力特徵 - 純粹暴力
    DemonKing.PrimaryAuthority = ECommandAuthorityType::SituationalAuthority;  // 情境權威
    DemonKing.CommandEffectiveness = 80.0f;  // 高效但殘酷
    DemonKing.InfluenceRadius = 90.0f;       // 廣泛影響但基於恐懼

    return DemonKing;
}

FSageRoleStandards UMingSageRoles::GetRoleStandards(ESageRoleType RoleType)
{
    if (RoleStandardsMap.Contains(RoleType))
    {
        return RoleStandardsMap[RoleType];
    }
    
    // 返回默認規範
    FSageRoleStandards DefaultStandards;
    DefaultStandards.RoleType = RoleType;
    DefaultStandards.StandardName = TEXT("未定義規範");
    return DefaultStandards;
}

TArray<FSageRoleStandards> UMingSageRoles::GetAllRoleStandards()
{
    TArray<FSageRoleStandards> AllStandards;
    
    for (const auto& RolePair : RoleStandardsMap)
    {
        AllStandards.Add(RolePair.Value);
    }
    
    return AllStandards;
}

FSageRoleEvaluation UMingSageRoles::EvaluateRoleCompliance(ESageRoleType RoleType, const FSageRoleCharacteristics& Characteristics)
{
    FSageRoleEvaluation Evaluation;
    Evaluation.EvaluatedRole = RoleType;
    
    // 獲取角色規範
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    
    // 評估各項合規性
    Evaluation.MoralCompliance = EvaluateMoralCompliance(Characteristics, Standards);
    Evaluation.StrategicCompliance = EvaluateStrategicCompliance(Characteristics, Standards);
    Evaluation.AuthorityCompliance = EvaluateAuthorityCompliance(Characteristics, Standards);
    
    // 計算總體分數
    Evaluation.OverallScore = (Evaluation.MoralCompliance + Evaluation.StrategicCompliance + Evaluation.AuthorityCompliance) / 3.0f;
    
    // 確定合規等級
    Evaluation.ComplianceLevel = DetermineComplianceLevel(Evaluation.OverallScore);
    
    // 生成優勢和弱點
    GenerateStrengthsAndWeaknesses(Evaluation, Characteristics, Standards);
    
    // 生成建議
    GenerateRecommendations(Evaluation, Characteristics, Standards);
    
    // 生成評估總結
    Evaluation.EvaluationSummary = FString::Printf(TEXT("%s角色評估：總體分數%.1f，合規等級：%s"), 
        *GetRoleTypeName(RoleType), Evaluation.OverallScore, *GetStandardLevelName(Evaluation.ComplianceLevel));
    
    return Evaluation;
}

ERoleStandardLevel UMingSageRoles::DetermineComplianceLevel(float Score)
{
    if (Score >= 95.0f) return ERoleStandardLevel::Perfect;
    if (Score >= 85.0f) return ERoleStandardLevel::Excellent;
    if (Score >= 70.0f) return ERoleStandardLevel::Good;
    if (Score >= 50.0f) return ERoleStandardLevel::Average;
    if (Score >= 30.0f) return ERoleStandardLevel::Poor;
    return ERoleStandardLevel::Corrupted;
}

TArray<FString> UMingSageRoles::CompareRoles(ESageRoleType Role1, ESageRoleType Role2)
{
    return GenerateRoleComparison(Role1, Role2);
}

FSageRoleCharacteristics UMingSageRoles::GetOptimalRoleForSituation(const FString& Situation)
{
    // 根據情況分析最佳角色
    if (Situation.Contains("道德") || Situation.Contains("正義") || Situation.Contains("和平"))
    {
        return DefineTrueSage();
    }
    else if (Situation.Contains("欺騙") || Situation.Contains("陰謀") || Situation.Contains("權術"))
    {
        return DefineFalseSage();
    }
    else if (Situation.Contains("戰爭") || Situation.Contains("暴力") || Situation.Contains("征服"))
    {
        return DefineDemonKing();
    }
    else
    {
        // 默認情況下返回至聖者
        return DefineTrueSage();
    }
}

bool UMingSageRoles::IsActionCompliant(ESageRoleType RoleType, const FString& Action)
{
    // 檢查道德約束
    if (!CheckMoralConstraint(RoleType, Action))
    {
        return false;
    }
    
    // 檢查戰略指導
    if (!CheckStrategicGuideline(RoleType, Action))
    {
        return false;
    }
    
    // 檢查行為規則
    if (!CheckBehavioralRule(RoleType, Action))
    {
        return false;
    }
    
    return true;
}

TArray<FString> UMingSageRoles::GetCompliantActions(ESageRoleType RoleType, const FString& Context)
{
    TArray<FString> CompliantActions;
    
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    
    // 根據規範生成合規行動
    for (const FString& Guideline : Standards.StrategicGuidelines)
    {
        CompliantActions.Add(Guideline);
    }
    
    return CompliantActions;
}

TArray<FString> UMingSageRoles::GetProhibitedActions(ESageRoleType RoleType)
{
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    return Standards.ProhibitedActions;
}

bool UMingSageRoles::CanTransformRole(ESageRoleType FromRole, ESageRoleType ToRole)
{
    // 定義角色轉換規則
    switch (FromRole)
    {
    case ESageRoleType::TrueSage:
        // 至聖者可以轉換為任何角色（但通常不會）
        return true;
        
    case ESageRoleType::FalseSage:
        // 偽聖者可以轉換為魔王（完全墮落）或至聖者（悔悟）
        return (ToRole == ESageRoleType::DemonKing || ToRole == ESageRoleType::TrueSage);
        
    case ESageRoleType::DemonKing:
        // 魔王很難轉換，但可以轉換為偽偽聖者（偽裝）
        return ToRole == ESageRoleType::FalseSage;
        
    default:
        return false;
    }
}

FSageRoleCharacteristics UMingSageRoles::TransformRole(const FSageRoleCharacteristics& CurrentRole, ESageRoleType TargetRole)
{
    if (!CanTransformRole(CurrentRole.RoleType, TargetRole))
    {
        return CurrentRole; // 無法轉換，返回原角色
    }
    
    // 創建新角色特徵
    FSageRoleCharacteristics NewRole;
    
    switch (TargetRole)
    {
    case ESageRoleType::TrueSage:
        NewRole = DefineTrueSage();
        break;
    case ESageRoleType::FalseSage:
        NewRole = DefineFalseSage();
        break;
    case ESageRoleType::DemonKing:
        NewRole = DefineDemonKing();
        break;
    }
    
    // 保留部分原有特徵（記憶）
    NewRole.RoleDescription += FString::Printf(TEXT("（由%s轉換而來）"), *GetRoleTypeName(CurrentRole.RoleType));
    
    return NewRole;
}

void UMingSageRoles::ApplyRoleStandardsToProject(ESageRoleType RoleType)
{
    // 將角色規範應用到專案
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    
    // 這裡可以添加具體的專案應用邏輯
    // 例如：設置遊戲規則、AI行為、UI主題等
}

TArray<FString> UMingSageRoles::GetProjectGuidelines(ESageRoleType RoleType)
{
    TArray<FString> Guidelines;
    
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    
    // 添加核心原則
    for (const FString& Principle : Standards.CorePrinciples)
    {
        Guidelines.Add(Principle);
    }
    
    // 添加戰略指導
    for (const FString& Guideline : Standards.StrategicGuidelines)
    {
        Guidelines.Add(Guideline);
    }
    
    return Guidelines;
}

bool UMingSageRoles::ValidateProjectCompliance(ESageRoleType RoleType)
{
    // 驗證專案是否符合角色規範
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    
    // 這裡可以添加具體的驗證邏輯
    // 例如：檢查遊戲內容、AI行為、系統設置等
    
    return true; // 簡化實現
}

// 私有方法實現

void UMingSageRoles::InitializeRoleStandards()
{
    // 初始化至聖者規範
    FSageRoleStandards TrueSageStandards;
    TrueSageStandards.RoleType = ESageRoleType::TrueSage;
    TrueSageStandards.StandardName = TEXT("至聖者規範");
    
    TrueSageStandards.CorePrinciples.Add(TEXT("道德至上，以德服人"));
    TrueSageStandards.CorePrinciples.Add(TEXT("陰陽平衡，五行和諧"));
    TrueSageStandards.CorePrinciples.Add(TEXT("智慧決策，洞察天機"));
    TrueSageStandards.CorePrinciples.Add(TEXT("仁愛眾生，保護無辜"));
    
    TrueSageStandards.BehavioralRules.Add(TEXT("永不傷害無辜者"));
    TrueSageStandards.BehavioralRules.Add(TEXT("堅守道德底線"));
    TrueSageStandards.BehavioralRules.Add(TEXT("追求長遠和諧"));
    TrueSageStandards.BehavioralRules.Add(TEXT("以智慧解決衝突"));
    
    TrueSageStandards.StrategicGuidelines.Add(TEXT("運用天地人三策"));
    TrueSageStandards.StrategicGuidelines.Add(TEXT("保持陰陽動態平衡"));
    TrueSageStandards.StrategicGuidelines.Add(TEXT("善用五行相生相克"));
    TrueSageStandards.StrategicGuidelines.Add(TEXT("注重長遠戰略規劃"));
    
    TrueSageStandards.MoralConstraints.Add(TEXT("絕不使用邪惡手段"));
    TrueSageStandards.MoralConstraints.Add(TEXT("避免玉石俱焚"));
    TrueSageStandards.MoralConstraints.Add(TEXT("尊重生命價值"));
    TrueSageStandards.MoralConstraints.Add(TEXT("維護正義秩序"));
    
    TrueSageStandards.DecisionCriteria.Add(TEXT("是否符合道德"));
    TrueSageStandards.DecisionCriteria.Add(TEXT("是否促進和諧"));
    TrueSageStandards.DecisionCriteria.Add(TEXT("是否長遠有益"));
    TrueSageStandards.DecisionCriteria.Add(TEXT("是否保護無辜"));
    
    TrueSageStandards.ProhibitedActions.Add(TEXT("屠殺無辜"));
    TrueSageStandards.ProhibitedActions.Add(TEXT("使用禁術"));
    TrueSageStandards.ProhibitedActions.Add(TEXT("背叛信任"));
    TrueSageStandards.ProhibitedActions.Add(TEXT("追求絕對權力"));
    
    RoleStandardsMap.Add(ESageRoleType::TrueSage, TrueSageStandards);
    
    // 初始化偽聖者規範
    FSageRoleStandards FalseSageStandards;
    FalseSageStandards.RoleType = ESageRoleType::FalseSage;
    FalseSageStandards.StandardName = TEXT("偽聖者規範");
    
    FalseSageStandards.CorePrinciples.Add(TEXT("表面道德，內心邪惡"));
    FalseSageStandards.CorePrinciples.Add(TEXT("善於偽裝，欺騙大眾"));
    FalseSageStandards.CorePrinciples.Add(TEXT("利用道德謀取私利"));
    FalseSageStandards.CorePrinciples.Add(TEXT("暗中破壞秩序"));
    
    FalseSageStandards.BehavioralRules.Add(TEXT("偽裝善良"));
    FalseSageStandards.BehavioralRules.Add(TEXT("暗中陰謀"));
    FalseSageStandards.BehavioralRules.Add(TEXT("利用他人弱點"));
    FalseSageStandards.BehavioralRules.Add(TEXT("避免暴露真實意圖"));
    
    FalseSageStandards.StrategicGuidelines.Add(TEXT("善用人心策略"));
    FalseSageStandards.StrategicGuidelines.Add(TEXT("製造內部分裂"));
    FalseSageStandards.StrategicGuidelines.Add(TEXT("利用道德綁架"));
    FalseSageStandards.StrategicGuidelines.Add(TEXT("暗中削弱敵人"));
    
    FalseSageStandards.MoralConstraints.Add(TEXT("避免公開邪惡"));
    FalseSageStandards.MoralConstraints.Add(TEXT("維護表面形象"));
    FalseSageStandards.MoralConstraints.Add(TEXT("避免直接傷害無辜"));
    FalseSageStandards.MoralConstraints.Add(TEXT("保持偽裝一致性"));
    
    FalseSageStandards.DecisionCriteria.Add(TEXT("是否有利於私利"));
    FalseSageStandards.DecisionCriteria.Add(TEXT("是否能夠偽裝"));
    FalseSageStandards.DecisionCriteria.Add(TEXT("是否避免暴露"));
    FalseSageStandards.DecisionCriteria.Add(TEXT("是否長遠有利"));
    
    FalseSageStandards.ProhibitedActions.Add(TEXT("公開邪惡行為"));
    FalseSageStandards.ProhibitedActions.Add(TEXT("暴露真實意圖"));
    FalseSageStandards.ProhibitedActions.Add(TEXT("直接傷害盟友"));
    FalseSageStandards.ProhibitedActions.Add(TEXT("破壞自身形象"));
    
    RoleStandardsMap.Add(ESageRoleType::FalseSage, FalseSageStandards);
    
    // 初始化魔王規範
    FSageRoleStandards DemonKingStandards;
    DemonKingStandards.RoleType = ESageRoleType::DemonKing;
    DemonKingStandards.StandardName = TEXT("魔王規範");
    
    DemonKingStandards.CorePrinciples.Add(TEXT("力量至上，弱肉強食"));
    DemonKingStandards.CorePrinciples.Add(TEXT("絕對權力，無所顧忌"));
    DemonKingStandards.CorePrinciples.Add(TEXT("恐懼統治，暴力解決"));
    DemonKingStandards.CorePrinciples.Add(TEXT("征服一切，毀滅反對者"));
    
    DemonKingStandards.BehavioralRules.Add(TEXT("使用暴力"));
    DemonKingStandards.BehavioralRules.Add(TEXT("製造恐懼"));
    DemonKingStandards.BehavioralRules.Add(TEXT("無視道德"));
    DemonKingStandards.BehavioralRules.Add(TEXT("追求絕對控制"));
    
    DemonKingStandards.StrategicGuidelines.Add(TEXT("直接攻擊"));
    DemonKingStandards.StrategicGuidelines.Add(TEXT("製造混亂"));
    DemonKingStandards.StrategicGuidelines.Add(TEXT("利用恐懼"));
    DemonKingStandards.StrategicGuidelines.Add(TEXT("快速征服"));
    
    DemonKingStandards.MoralConstraints.Add(TEXT("無道德約束"));
    DemonKingStandards.MoralConstraints.Add(TEXT("無限制手段"));
    DemonKingStandards.MoralConstraints.Add(TEXT("無禁忌行動"));
    DemonKingStandards.MoralConstraints.Add(TEXT("無後顧之憂"));
    
    DemonKingStandards.DecisionCriteria.Add(TEXT("是否增加權力"));
    DemonKingStandards.DecisionCriteria.Add(TEXT("是否製造恐懼"));
    DemonKingStandards.DecisionCriteria.Add(TEXT("是否快速有效"));
    DemonKingStandards.DecisionCriteria.Add(TEXT("是否絕對控制"));
    
    DemonKingStandards.ProhibitedActions.Add(TEXT("示弱"));
    DemonKingStandards.ProhibitedActions.Add(TEXT("仁慈"));
    DemonKingStandards.ProhibitedActions.Add(TEXT("妥協"));
    DemonKingStandards.ProhibitedActions.Add(TEXT("退讓"));
    
    RoleStandardsMap.Add(ESageRoleType::DemonKing, DemonKingStandards);
}

void UMingSageRoles::InitializeRoleCharacteristics()
{
    // 初始化角色特徵
    RoleCharacteristicsMap.Add(ESageRoleType::TrueSage, DefineTrueSage());
    RoleCharacteristicsMap.Add(ESageRoleType::FalseSage, DefineFalseSage());
    RoleCharacteristicsMap.Add(ESageRoleType::DemonKing, DefineDemonKing());
}

float UMingSageRoles::EvaluateMoralCompliance(const FSageRoleCharacteristics& Characteristics, const FSageRoleStandards& Standards)
{
    float Score = 0.0f;
    
    // 評估道德權威
    if (Characteristics.MoralAuthorityLevel >= 80.0f)
    {
        Score += 30.0f;
    }
    else if (Characteristics.MoralAuthorityLevel >= 50.0f)
    {
        Score += 20.0f;
    }
    else
    {
        Score += 10.0f;
    }
    
    // 評估防墮機制
    if (Characteristics.bHasFallPrevention)
    {
        Score += 20.0f;
    }
    else
    {
        Score += 0.0f;
    }
    
    // 評估陰陽平衡
    if (Characteristics.HarmonyLevel >= 80.0f)
    {
        Score += 30.0f;
    }
    else if (Characteristics.HarmonyLevel >= 50.0f)
    {
        Score += 20.0f;
    }
    else
    {
        Score += 10.0f;
    }
    
    // 評估邪惡輪轉
    if (!Characteristics.bUsesEvilRotation)
    {
        Score += 20.0f;
    }
    else
    {
        Score += 0.0f;
    }
    
    return Score;
}

float UMingSageRoles::EvaluateStrategicCompliance(const FSageRoleCharacteristics& Characteristics, const FSageRoleStandards& Standards)
{
    float Score = 0.0f;
    
    // 評估戰略洞察
    if (Characteristics.StrategicInsight >= 80.0f)
    {
        Score += 40.0f;
    }
    else if (Characteristics.StrategicInsight >= 50.0f)
    {
        Score += 25.0f;
    }
    else
    {
        Score += 10.0f;
    }
    
    // 評估策略複雜度
    switch (Characteristics.MaxComplexity)
    {
    case EStrategyComplexity::Master:
        Score += 30.0f;
        break;
    case EStrategyComplexity::Complex:
        Score += 20.0f;
        break;
    case EStrategyComplexity::Moderate:
        Score += 10.0f;
        break;
    case EStrategyComplexity::Simple:
        Score += 5.0f;
        break;
    }
    
    // 評估策略偏好
    if (Characteristics.PreferredStrategy == ESixStrategyType::HeavenEarthHumanStrategy)
    {
        Score += 30.0f;
    }
    else if (Characteristics.PreferredStrategy == ESixStrategyType::HeavenEarthStrategy ||
             Characteristics.PreferredStrategy == ESixStrategyType::HeavenHumanStrategy ||
             Characteristics.PreferredStrategy == ESixStrategyType::EarthHumanStrategy)
    {
        Score += 20.0f;
    }
    else
    {
        Score += 10.0f;
    }
    
    return Score;
}

float UMingSageRoles::EvaluateAuthorityCompliance(const FSageRoleCharacteristics& Characteristics, const FSageRoleStandards& Standards)
{
    float Score = 0.0f;
    
    // 評估指揮權威類型
    if (Characteristics.PrimaryAuthority == ECommandAuthorityType::MoralAuthority)
    {
        Score += 40.0f;
    }
    else if (Characteristics.PrimaryAuthority == ECommandAuthorityType::LegalAuthority ||
             Characteristics.PrimaryAuthority == ECommandAuthorityType::TraditionalAuthority)
    {
        Score += 30.0f;
    }
    else if (Characteristics.PrimaryAuthority == ECommandAuthorityType::CharismaticAuthority)
    {
        Score += 20.0f;
    }
    else
    {
        Score += 10.0f;
    }
    
    // 評估指揮有效性
    if (Characteristics.CommandEffectiveness >= 80.0f)
    {
        Score += 30.0f;
    }
    else if (Characteristics.CommandEffectiveness >= 50.0f)
    {
        Score += 20.0f;
    }
    else
    {
        Score += 10.0f;
    }
    
    // 評估影響範圍
    if (Characteristics.InfluenceRadius >= 80.0f)
    {
        Score += 30.0f;
    }
    else if (Characteristics.InfluenceRadius >= 50.0f)
    {
        Score += 20.0f;
    }
    else
    {
        Score += 10.0f;
    }
    
    return Score;
}

void UMingSageRoles::GenerateStrengthsAndWeaknesses(FSageRoleEvaluation& Evaluation, const FSageRoleCharacteristics& Characteristics, const FSageRoleStandards& Standards)
{
    // 根據評估結果生成優勢和弱點
    if (Evaluation.MoralCompliance >= 80.0f)
    {
        Evaluation.Strengths.Add(TEXT("道德權威強大"));
    }
    else if (Evaluation.MoralCompliance < 50.0f)
    {
        Evaluation.Weaknesses.Add(TEXT("道德權威不足"));
    }
    
    if (Evaluation.StrategicCompliance >= 80.0f)
    {
        Evaluation.Strengths.Add(TEXT("戰略洞察深刻"));
    }
    else if (Evaluation.StrategicCompliance < 50.0f)
    {
        Evaluation.Weaknesses.Add(TEXT("戰略能力有限"));
    }
    
    if (Evaluation.AuthorityCompliance >= 80.0f)
    {
        Evaluation.Strengths.Add(TEXT("指揮權威穩固"));
    }
    else if (Evaluation.AuthorityCompliance < 50.0f)
    {
        Evaluation.Weaknesses.Add(TEXT("指揮權威薄弱"));
    }
}

void UMingSageRoles::GenerateRecommendations(FSageRoleEvaluation& Evaluation, const FSageRoleCharacteristics& Characteristics, const FSageRoleStandards& Standards)
{
    // 根據評估結果生成建議
    if (Evaluation.MoralCompliance < 70.0f)
    {
        Evaluation.Recommendations.Add(TEXT("建議加強道德權威建設"));
    }
    
    if (Evaluation.StrategicCompliance < 70.0f)
    {
        Evaluation.Recommendations.Add(TEXT("建議提升戰略洞察力"));
    }
    
    if (Evaluation.AuthorityCompliance < 70.0f)
    {
        Evaluation.Recommendations.Add(TEXT("建議增強指揮權威"));
    }
    
    if (Evaluation.OverallScore < 50.0f)
    {
        Evaluation.Recommendations.Add(TEXT("建議全面重新評估角色定位"));
    }
}

bool UMingSageRoles::CheckMoralConstraint(ESageRoleType RoleType, const FString& Action)
{
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    
    for (const FString& Constraint : Standards.MoralConstraints)
    {
        if (Action.Contains(Constraint))
        {
            return false;
        }
    }
    
    return true;
}

bool UMingSageRoles::CheckStrategicGuideline(ESageRoleType RoleType, const FString& Action)
{
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    
    for (const FString& Guideline : Standards.StrategicGuidelines)
    {
        if (Action.Contains(Guideline))
        {
            return true;
        }
    }
    
    return false;
}

bool UMingSageRoles::CheckBehavioralRule(ESageRoleType RoleType, const FString& Action)
{
    FSageRoleStandards Standards = GetRoleStandards(RoleType);
    
    for (const FString& Rule : Standards.BehavioralRules)
    {
        if (Action.Contains(Rule))
        {
            return true;
        }
    }
    
    return false;
}

FString UMingSageRoles::GetRoleTypeName(ESageRoleType RoleType) const
{
    switch (RoleType)
    {
    case ESageRoleType::TrueSage: return TEXT("至聖者");
    case ESageRoleType::FalseSage: return TEXT("偽聖者");
    case ESageRoleType::DemonKing: return TEXT("魔王");
    default: return TEXT("未知角色");
    }
}

FString UMingSageRoles::GetStandardLevelName(ERoleStandardLevel Level) const
{
    switch (Level)
    {
    case ERoleStandardLevel::Perfect: return TEXT("完美級");
    case ERoleStandardLevel::Excellent: return TEXT("優秀級");
    case ERoleStandardLevel::Good: return TEXT("良好級");
    case ERoleStandardLevel::Average: return TEXT("平均級");
    case ERoleStandardLevel::Poor: return TEXT("較差級");
    case ERoleStandardLevel::Corrupted: return TEXT("墮落級");
    default: return TEXT("未知等級");
    }
}

TArray<FString> UMingSageRoles::GenerateRoleComparison(ESageRoleType Role1, ESageRoleType Role2)
{
    TArray<FString> Comparison;
    
    FSageRoleCharacteristics Char1 = RoleCharacteristicsMap.Contains(Role1) ? RoleCharacteristicsMap[Role1] : FSageRoleCharacteristics();
    FSageRoleCharacteristics Char2 = RoleCharacteristicsMap.Contains(Role2) ? RoleCharacteristicsMap[Role2] : FSageRoleCharacteristics();
    
    Comparison.Add(FString::Printf(TEXT("角色比較：%s vs %s"), *GetRoleTypeName(Role1), *GetRoleTypeName(Role2)));
    
    // 比較道德權威
    Comparison.Add(FString::Printf(TEXT("道德權威：%s %.1f vs %s %.1f"), 
        *GetRoleTypeName(Role1), Char1.MoralAuthorityLevel, 
        *GetRoleTypeName(Role2), Char2.MoralAuthorityLevel));
    
    // 比較戰略洞察
    Comparison.Add(FString::Printf(TEXT("戰略洞察：%s %.1f vs %s %.1f"), 
        *GetRoleTypeName(Role1), Char1.StrategicInsight, 
        *GetRoleTypeName(Role2), Char2.StrategicInsight));
    
    // 比較指揮權威
    Comparison.Add(FString::Printf(TEXT("指揮權威：%s %.1f vs %s %.1f"), 
        *GetRoleTypeName(Role1), Char1.CommandEffectiveness, 
        *GetRoleTypeName(Role2), Char2.CommandEffectiveness));
    
    return Comparison;
}
