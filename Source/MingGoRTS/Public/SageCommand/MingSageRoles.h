#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageRoles.generated.h"

// 聖者角色類型枚舉
UENUM(BlueprintType)
enum class ESageRoleType: uint8 {
    TrueSage = 0,        // 至聖者 - 完美體現聖者指揮學
    FalseSage = 1,       // 偽聖者 - 表面聖者，內心邪惡
    DemonKing = 2        // 魔王 - 公開邪惡，追求絕對權力
};

// 角色規範等級
UENUM(BlueprintType)
enum class ERoleStandardLevel: uint8 {
    Perfect = 0,         // 完美級 - 完全符合角色規範
    Excellent = 1,       // 優秀級 - 高度符合規範
    Good = 2,           // 良好級 - 基本符合規範
    Average = 3,         // 平均級 - 部分符合規範
    Poor = 4,           // 較差級 - 較少符合規範
    Corrupted = 5        // 墮落級 - 完全偏離規範
};

// 指揮風格特徵
USTRUCT(BlueprintType)
struct FSageRoleCharacteristics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ESageRoleType RoleType = ESageRoleType::TrueSage;

    UPROPERTY(BlueprintReadOnly)
    FString RoleName;

    UPROPERTY(BlueprintReadOnly)
    FString RoleDescription;

    UPROPERTY(BlueprintReadOnly)
    ERoleStandardLevel StandardLevel = ERoleStandardLevel::Perfect;

    // 道德權威特徵
    UPROPERTY(BlueprintReadOnly)
    float MoralAuthorityLevel = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    float LegitimacyScore = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bHasFallPrevention = true;

    // 陰陽五行特徵
    UPROPERTY(BlueprintReadOnly)
    EYinYangAttribute PrimaryAttribute = EYinYangAttribute::BalancedYinYang;

    UPROPERTY(BlueprintReadOnly)
    EFiveElements PrimaryElement = EFiveElements::Earth;

    UPROPERTY(BlueprintReadOnly)
    float HarmonyLevel = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bUsesEvilRotation = false;

    // 六策特徵
    UPROPERTY(BlueprintReadOnly)
    ESixStrategyType PreferredStrategy = ESixStrategyType::HeavenEarthHumanStrategy;

    UPROPERTY(BlueprintReadOnly)
    EStrategyComplexity MaxComplexity = EStrategyComplexity::Master;

    UPROPERTY(BlueprintReadOnly)
    float StrategicInsight = 100.0f;

    // 指揮權力特徵
    UPROPERTY(BlueprintReadOnly)
    ECommandAuthorityType PrimaryAuthority = ECommandAuthorityType::MoralAuthority;

    UPROPERTY(BlueprintReadOnly)
    float CommandEffectiveness = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    float InfluenceRadius = 100.0f;
};

// 角色行為規範
USTRUCT(BlueprintType)
struct FSageRoleStandards
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ESageRoleType RoleType;

    UPROPERTY(BlueprintReadOnly)
    FString StandardName;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CorePrinciples;        // 核心原則

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> BehavioralRules;       // 行為規則

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> StrategicGuidelines;    // 戰略指導

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> MoralConstraints;       // 道德約束

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> DecisionCriteria;       // 決策標準

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ProhibitedActions;      // 禁止行動
};

// 角色評估結果
USTRUCT(BlueprintType)
struct FSageRoleEvaluation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ESageRoleType EvaluatedRole;

    UPROPERTY(BlueprintReadOnly)
    ERoleStandardLevel ComplianceLevel = ERoleStandardLevel::Perfect;

    UPROPERTY(BlueprintReadOnly)
    float OverallScore = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    float MoralCompliance = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    float StrategicCompliance = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    float AuthorityCompliance = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Strengths;             // 優勢

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Weaknesses;            // 弱點

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Recommendations;        // 建議

    UPROPERTY(BlueprintReadOnly)
    FString EvaluationSummary;             // 評估總結
};

/**
 * 聖者角色管理系統
 * 管理至聖者、偽聖者、魔王三種角色的規範和行為
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (SageCommand))
class MINGRTS_API UMingSageRoles : public UObject
{
    GENERATED_BODY()

public:
    UMingSageRoles();

    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    bool Initialize();

    // 角色定義
    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    FSageRoleCharacteristics DefineTrueSage();

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    FSageRoleCharacteristics DefineFalseSage();

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    FSageRoleCharacteristics DefineDemonKing();

    // 規範定義
    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    FSageRoleStandards GetRoleStandards(ESageRoleType RoleType);

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    TArray<FSageRoleStandards> GetAllRoleStandards();

    // 角色評估
    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    FSageRoleEvaluation EvaluateRoleCompliance(ESageRoleType RoleType, const FSageRoleCharacteristics& Characteristics);

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    ERoleStandardLevel DetermineComplianceLevel(float Score);

    // 角色比較
    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    TArray<FString> CompareRoles(ESageRoleType Role1, ESageRoleType Role2);

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    FSageRoleCharacteristics GetOptimalRoleForSituation(const FString& Situation);

    // 規範檢查
    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    bool IsActionCompliant(ESageRoleType RoleType, const FString& Action);

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    TArray<FString> GetCompliantActions(ESageRoleType RoleType, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    TArray<FString> GetProhibitedActions(ESageRoleType RoleType);

    // 角色轉換
    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    bool CanTransformRole(ESageRoleType FromRole, ESageRoleType ToRole);

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    FSageRoleCharacteristics TransformRole(const FSageRoleCharacteristics& CurrentRole, ESageRoleType TargetRole);

    // 專案規範應用
    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    void ApplyRoleStandardsToProject(ESageRoleType RoleType);

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    TArray<FString> GetProjectGuidelines(ESageRoleType RoleType);

    UFUNCTION(BlueprintCallable, Category = "Sage Roles")
    bool ValidateProjectCompliance(ESageRoleType RoleType);

protected:
    // 內部數據
    UPROPERTY(BlueprintReadOnly, Category = "Sage Roles")
    TMap<ESageRoleType, FSageRoleStandards> RoleStandardsMap;

    UPROPERTY(BlueprintReadOnly, Category = "Sage Roles")
    TMap<ESageRoleType, FSageRoleCharacteristics> RoleCharacteristicsMap;

    UPROPERTY(BlueprintReadOnly, Category = "Sage Roles")
    bool bSystemInitialized = false;

private:
    // 初始化方法
    void InitializeRoleStandards();
    void InitializeRoleCharacteristics();

    // 評估輔助方法
    float EvaluateMoralCompliance(const FSageRoleCharacteristics& Characteristics, const FSageRoleStandards& Standards);
    float EvaluateStrategicCompliance(const FSageRoleCharacteristics& Characteristics, const FSageRoleStandards& Standards);
    float EvaluateAuthorityCompliance(const FSageRoleCharacteristics& Characteristics, const FSageRoleStandards& Standards);

    // 規範檢查方法
    bool CheckMoralConstraint(ESageRoleType RoleType, const FString& Action);
    bool CheckStrategicGuideline(ESageRoleType RoleType, const FString& Action);
    bool CheckBehavioralRule(ESageRoleType RoleType, const FString& Action);

    // 輔助方法
    FString GetRoleTypeName(ESageRoleType RoleType) const;
    FString GetStandardLevelName(ERoleStandardLevel Level) const;
    TArray<FString> GenerateRoleComparison(ESageRoleType Role1, ESageRoleType Role2);
};
