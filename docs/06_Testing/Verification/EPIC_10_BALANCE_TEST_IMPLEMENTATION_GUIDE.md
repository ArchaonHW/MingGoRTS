# Epic 10 势力平衡测试实施总指南

## 概述

**目标**: 建立完整的12势力平衡测试系统，确保游戏平衡性  
**适用范围**: 数值平衡、AI难度平衡、经济平衡、战斗平衡  
**开发周期**: 约2个月（持续迭代）  
**团队配置**: 建议1名平衡设计师 + 2名QA测试 + 数据分析支持

---

## 第一部分: 平衡测试系统架构

### 1.1 核心测试架构

```
MingBalanceTestSystem (平衡测试系统)
  ├─ 数据收集器 (DataCollector)
  │   ├─ 实时对战数据
  │   ├─ 经济数据
  │   ├─ 战斗数据
  │   └─ AI行为数据
  ├─ 分析引擎 (AnalysisEngine)
  │   ├─ 胜率分析
  │   ├─ 经济平衡分析
  │   ├─ 单位效能分析
  │   └─ 时间曲线分析
  ├─ 报告生成器 (ReportGenerator)
  │   ├─ 平衡报告
  │   ├─ 问题诊断
  │   └─ 调整建议
  └─ 自动平衡器 (AutoBalancer)
      ├─ 参数调整
      ├─ A/B测试
      └─ 回归验证
```

### 1.2 测试数据模型

```cpp
USTRUCT()
struct FBalanceTestData
{
    GENERATED_BODY()
    
    // 对局信息
    UPROPERTY()
    FString MatchID;
    
    UPROPERTY()
    TArray<EFactionType> ParticipatingFactions;
    
    UPROPERTY()
    EFactionType Winner;
    
    UPROPERTY()
    float MatchDuration;  // 分钟
    
    // 经济数据
    UPROPERTY()
    TMap<EFactionType, float> TotalResourcesGathered;
    
    UPROPERTY()
    TMap<EFactionType, float> ResourceGatherRate;
    
    UPROPERTY()
    TMap<EFactionType, int32> BuildingsConstructed;
    
    // 战斗数据
    UPROPERTY()
    TMap<EFactionType, int32> UnitsProduced;
    
    UPROPERTY()
    TMap<EFactionType, int32> UnitsLost;
    
    UPROPERTY()
    TMap<EFactionType, int32> UnitsKilled;
    
    UPROPERTY()
    TMap<EFactionType, float> CombatEfficiency;  // 杀敌/损失比
    
    // 时间曲线数据
    UPROPERTY()
    TArray<FTimeSliceData> TimeSlices;
};

USTRUCT()
struct FTimeSliceData
{
    UPROPERTY()
    float Timestamp;  // 游戏时间
    
    UPROPERTY()
    TMap<EFactionType, float> MilitaryPower;  // 军事实力指数
    
    UPROPERTY()
    TMap<EFactionType, float> EconomicPower;  // 经济实力指数
    
    UPROPERTY()
    TMap<EFactionType, int32> TerritoryControl;  // 控制领土数
};
```

---

## 第二部分: 平衡指标体系

### 2.1 胜率平衡指标

#### 目标胜率分布
```
理想状态:
- 所有势力胜率: 45%-55%
- 标准差: < 5%
- 方差: < 0.0025

警告阈值:
- 单一势力胜率 > 60% 或 < 40%
- 连续20局单一势力连胜/连败
```

#### 胜率分析实现
```cpp
UCLASS()
class UMingWinRateAnalyzer : public UObject
{
    UFUNCTION()
    FWinRateAnalysis AnalyzeWinRates(const TArray<FBalanceTestData>& MatchData);
    
    UFUNCTION()
    bool IsFactionOverpowered(EFactionType Faction, float Threshold = 0.6f);
    
    UFUNCTION()
    bool IsFactionUnderpowered(EFactionType Faction, float Threshold = 0.4f);
    
    UFUNCTION()
    TMap<EFactionType, float> CalculateWinRates(
        const TArray<FBalanceTestData>& MatchData,
        EMatchupType Matchup = EMatchupType::All);
};

USTRUCT()
struct FWinRateAnalysis
{
    UPROPERTY()
    TMap<EFactionType, float> WinRates;  // 各势力胜率
    
    UPROPERTY()
    float StandardDeviation;  // 标准差
    
    UPROPERTY()
    EFactionType StrongestFaction;  // 最强势力
    
    UPROPERTY()
    EFactionType WeakestFaction;  // 最弱势力
    
    UPROPERTY()
    bool bIsBalanced;  // 整体是否平衡
    
    UPROPERTY()
    TArray<FString> ImbalanceIssues;  // 不平衡问题列表
};
```

### 2.2 经济平衡指标

#### 经济发展曲线
```cpp
// 理想经济曲线 (归一化)
// 时间    经济强度
// 0-5min   0.1-0.2  (起步阶段)
// 5-10min  0.2-0.4  (发展阶段)
// 10-15min 0.4-0.7  (扩张阶段)
// 15-20min 0.7-1.0  (成熟阶段)
// 20min+   1.0+     (后期阶段)

// 势力间经济差异阈值
const float MaxEconomicGap = 0.3f;  // 最大经济差距30%

USTRUCT()
struct FEconomicBalanceAnalysis
{
    UPROPERTY()
    TMap<EFactionType, float> AvgEconomicPower;  // 平均经济实力
    
    UPROPERTY()
    TMap<EFactionType, float> EconomicGrowthRate;  // 经济增长率
    
    UPROPERTY()
    float EconomicVariance;  // 经济方差
    
    UPROPERTY()
    TArray<EFactionType> OverEconomicFactions;  // 经济过强势力
    
    UPROPERTY()
    TArray<EFactionType> UnderEconomicFactions;  // 经济过弱势力
};
```

#### 资源采集平衡
```cpp
UFUNCTION()
FResourceBalanceAnalysis AnalyzeResourceBalance(
    const TArray<FBalanceTestData>& MatchData)
{
    FResourceBalanceAnalysis Result;
    
    for (auto Faction : AllFactions)
    {
        float AvgGatherRate = CalculateAverageGatherRate(Faction, MatchData);
        float ExpectedRate = GetExpectedGatherRate(Faction);
        float Deviation = (AvgGatherRate - ExpectedRate) / ExpectedRate;
        
        // 偏差超过15%视为不平衡
        if (FMath::Abs(Deviation) > 0.15f)
        {
            if (Deviation > 0)
                Result.OverGatheringFactions.Add(Faction);
            else
                Result.UnderGatheringFactions.Add(Faction);
        }
    }
    
    return Result;
}
```

### 2.3 单位效能平衡

#### 单位性价比指标
```cpp
USTRUCT()
struct FUnitEfficiencyMetrics
{
    UPROPERTY()
    TSubclassOf<AActor> UnitClass;
    
    UPROPERTY()
    float Cost;  // 生产成本
    
    UPROPERTY()
    float AvgDamageDealt;  // 平均造成伤害
    
    UPROPERTY()
    float AvgDamageTaken;  // 平均承受伤害
    
    UPROPERTY()
    float KillDeathRatio;  // 击杀死亡比
    
    UPROPERTY()
    float SurvivalTime;  // 平均存活时间
    
    UPROPERTY()
    float CostEfficiency;  // 性价比 (伤害/成本)
    
    UPROPERTY()
    EFactionType Faction;
};

// 单位效能平衡目标
// 所有单位性价比差异 < 20%
// 没有单位被完全弃用 (使用率 > 5%)
// 各单位有明确克制关系
```

#### 单位使用率分析
```cpp
UFUNCTION()
FUnitUsageAnalysis AnalyzeUnitUsage(const TArray<FBalanceTestData>& MatchData)
{
    FUnitUsageAnalysis Result;
    
    for (auto Faction : AllFactions)
    {
        TMap<TSubclassOf<AActor>, float> UnitUsageRates;
        
        for (auto UnitType : GetFactionUnits(Faction))
        {
            float UsageRate = CalculateUnitUsageRate(UnitType, MatchData);
            UnitUsageRates.Add(UnitType, UsageRate);
            
            // 使用率低于5%视为问题单位
            if (UsageRate < 0.05f)
            {
                Result.UnderusedUnits.Add(UnitType);
            }
            // 使用率过高可能意味着过强
            else if (UsageRate > 0.5f)
            {
                Result.OverusedUnits.Add(UnitType);
            }
        }
        
        Result.FactionUnitUsage.Add(Faction, UnitUsageRates);
    }
    
    return Result;
}
```

### 2.4 时间曲线平衡

#### 游戏时长分布
```cpp
// 目标游戏时长分布
// 短局 (< 15min): 20%
// 中局 (15-25min): 50%
// 长局 (25-40min): 25%
// 超长 (> 40min): 5%

USTRUCT()
struct FGameDurationAnalysis
{
    UPROPERTY()
    float AverageDuration;  // 平均时长
    
    UPROPERTY()
    float ShortGamePercentage;  // 短局比例
    
    UPROPERTY()
    float MediumGamePercentage;  // 中局比例
    
    UPROPERTY()
    float LongGamePercentage;  // 长局比例
    
    UPROPERTY()
    bool bDurationBalanced;  // 时长是否平衡
    
    UPROPERTY()
    FString DurationIssue;  // 时长问题描述
};
```

---

## 第三部分: 测试自动化系统

### 3.1 AI对战模拟

```cpp
UCLASS()
class UMingAIBattleSimulator : public UObject
{
    UFUNCTION()
    void StartAutomatedTesting(int32 MatchCount, ETestMode Mode);
    
    UFUNCTION()
    void SimulateMatch(EFactionType FactionA, EFactionType FactionB, EDifficulty Difficulty);
    
    UFUNCTION()
    void OnMatchCompleted(FBalanceTestData MatchData);
    
    UPROPERTY()
    int32 TotalMatchesToRun;
    
    UPROPERTY()
    int32 CompletedMatches;
    
    UPROPERTY()
    TArray<FBalanceTestData> CollectedData;
};

// 测试模式
UENUM()
enum class ETestMode
{
    AllFactionPairs,      // 所有势力组合
    MirrorMatches,        // 同势力对战
    RandomFactions,       // 随机势力
    FocusOnImbalanced     // 专注不平衡组合
};
```

### 3.2 持续集成测试

```cpp
UCLASS()
class UMingCIBalanceTest : public UObject
{
    // 每日自动运行测试
    UFUNCTION()
    void RunDailyBalanceTest();
    
    // 版本提交前测试
    UFUNCTION()
    void RunPreCommitTest();
    
    // 生成平衡报告
    UFUNCTION()
    FBalanceReport GenerateBalanceReport();
    
    // 如果发现问题，发送警告
    UFUNCTION()
    void SendBalanceAlert(const FString& Issue);
};
```

### 3.3 A/B测试系统

```cpp
UCLASS()
class UMingABTestSystem : public UObject
{
    // 创建参数变体
    UFUNCTION()
    void CreateParameterVariant(FString ParameterName, float ValueA, float ValueB);
    
    // 运行A/B测试
    UFUNCTION()
    void RunABTest(int32 MatchesPerVariant);
    
    // 分析结果
    UFUNCTION()
    FABTestResult AnalyzeABTestResults();
};

USTRUCT()
struct FABTestResult
{
    UPROPERTY()
    FString ParameterName;
    
    UPROPERTY()
    float WinRateA;
    
    UPROPERTY()
    float WinRateB;
    
    UPROPERTY()
    float StatisticalSignificance;  // 统计显著性
    
    UPROPERTY()
    bool bVariantBIsBetter;
    
    UPROPERTY()
    FString Recommendation;
};
```

---

## 第四部分: 问题诊断与修复

### 4.1 自动问题检测

```cpp
UCLASS()
class UMingBalanceIssueDetector : public UObject
{
    UFUNCTION()
    TArray<FBalanceIssue> DetectIssues(const FBalanceTestData& Data);
    
    UFUNCTION()
    bool CheckWinRateImbalance(const TArray<FBalanceTestData>& Data);
    
    UFUNCTION()
    bool CheckEconomicImbalance(const TArray<FBalanceTestData>& Data);
    
    UFUNCTION()
    bool CheckUnitImbalance(const TArray<FBalanceTestData>& Data);
    
    UFUNCTION()
    bool CheckSnowballEffect(const TArray<FBalanceTestData>& Data);
};

USTRUCT()
struct FBalanceIssue
{
    UPROPERTY()
    EIssueType Type;
    
    UPROPERTY()
    EFactionType AffectedFaction;
    
    UPROPERTY()
    FString Description;
    
    UPROPERTY()
    float Severity;  // 0-1
    
    UPROPERTY()
    TArray<FString> SuggestedFixes;
};
```

### 4.2 自动平衡调整

```cpp
UCLASS()
class UMingAutoBalancer : public UObject
{
    UFUNCTION()
    void ApplyAutoBalance(const TArray<FBalanceIssue>& Issues);
    
    UFUNCTION()
    void AdjustFactionParameter(EFactionType Faction, FString Parameter, float Delta);
    
    UFUNCTION()
    void AdjustUnitStats(TSubclassOf<AActor> UnitClass, FStatAdjustment Adjustment);
    
    UFUNCTION()
    void AdjustEconomicParameter(EFactionType Faction, EResourceType Resource, float Delta);
};

// 自动平衡规则
// 1. 胜率偏差 > 10% -> 调整单位成本或属性
// 2. 经济偏差 > 15% -> 调整采集效率或建筑成本
// 3. 单位使用率 < 5% -> 降低单位成本或增强能力
// 4. 滚雪球效应 -> 增加 comeback 机制
```

### 4.3 平衡调整建议系统

```cpp
UCLASS()
class UMingBalanceAdvisor : public UObject
{
    UFUNCTION()
    TArray<FBalanceSuggestion> GenerateSuggestions(const FBalanceIssue& Issue);
    
    UFUNCTION()
    FBalanceSuggestion GenerateWinRateFixSuggestion(EFactionType Faction, float CurrentWinRate);
    
    UFUNCTION()
    FBalanceSuggestion GenerateEconomicFixSuggestion(EFactionType Faction, float EconomicGap);
    
    UFUNCTION()
    FBalanceSuggestion GenerateUnitFixSuggestion(TSubclassOf<AActor> UnitClass, float UsageRate);
};

USTRUCT()
struct FBalanceSuggestion
{
    UPROPERTY()
    FString Title;
    
    UPROPERTY()
    FString Description;
    
    UPROPERTY()
    TArray<FParameterChange> Changes;
    
    UPROPERTY()
    float ExpectedImpact;  // 预期改善程度
    
    UPROPERTY()
    EChangeRiskLevel RiskLevel;
};
```

---

## 第五部分: 报告与可视化

### 5.1 平衡报告生成

```cpp
UCLASS()
class UMingBalanceReportGenerator : public UObject
{
    UFUNCTION()
    FBalanceReport GenerateFullReport(const TArray<FBalanceTestData>& Data);
    
    UFUNCTION()
    FString ExportReportToJSON(const FBalanceReport& Report);
    
    UFUNCTION()
    void ExportReportToHTML(const FBalanceReport& Report, FString FilePath);
    
    UFUNCTION()
    void GenerateTrendGraph(const TArray<FBalanceTestData>& Data, EGraphType Type);
};

USTRUCT()
struct FBalanceReport
{
    UPROPERTY()
    FDateTime GeneratedTime;
    
    UPROPERTY()
    int32 TotalMatchesAnalyzed;
    
    UPROPERTY()
    FWinRateAnalysis WinRateAnalysis;
    
    UPROPERTY()
    FEconomicBalanceAnalysis EconomicAnalysis;
    
    UPROPERTY()
    FUnitUsageAnalysis UnitAnalysis;
    
    UPROPERTY()
    FGameDurationAnalysis DurationAnalysis;
    
    UPROPERTY()
    TArray<FBalanceIssue> Issues;
    
    UPROPERTY()
    TArray<FBalanceSuggestion> Suggestions;
    
    UPROPERTY()
    float OverallBalanceScore;  // 0-100
};
```

### 5.2 实时监控面板

```cpp
UCLASS()
class UMingBalanceDashboard : public UUserWidget
{
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateWinRateDisplay(const TMap<EFactionType, float>& WinRates);
    
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateEconomicGraph(const TArray<FTimeSliceData>& TimeData);
    
    UFUNCTION(BlueprintImplementableEvent)
    void ShowAlert(const FString& AlertMessage);
    
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateLiveMatchCount(int32 ActiveMatches);
};
```

---

## 第六部分: 测试计划

### 6.1 测试时程 (2个月持续迭代)

```
第1-2周: 系统搭建
  ├─ 数据收集系统实现
  ├─ 基础分析功能
  └─ 报告生成系统

第3-4周: 自动化测试
  ├─ AI对战模拟
  ├─ CI集成
  └─ 初步数据收集

第5-6周: 第一轮平衡调整
  ├─ 分析初始数据
  ├─ 识别主要不平衡
  └─ 实施第一轮调整

第7-8周: 验证与微调
  ├─ 验证调整效果
  ├─ 进一步微调
  └─ 特殊场景测试
```

### 6.2 测试场景

| 场景 | 描述 | 目标 |
|------|------|------|
| 1v1单挑 | 单势力对战 | 基础平衡 |
| 2v2团战 | 双势力合作 | 配合平衡 |
| 混战 | 多势力自由战 | 综合平衡 |
| 同势力 | 镜像对战 | 纯技术测试 |
| 随机地图 | 不同地形 | 适应性测试 |
| 极限rush | 速攻测试 | 早期平衡 |
| 后期大战 | 长期对战 | 后期平衡 |

---

## 第七部分: 平衡目标与验收标准

### 7.1 最终平衡目标

```
胜率平衡:
✓ 所有势力胜率 48%-52%
✓ 标准差 < 3%
✓ 没有势力胜率 < 45% 或 > 55%

经济平衡:
✓ 各势力经济发展曲线相似度 > 85%
✓ 资源采集速率差异 < 10%
✓ 没有明显的经济滚雪球

单位平衡:
✓ 所有单位使用率 > 5%
✓ 性价比差异 < 20%
✓ 明确的克制关系存在

游戏时长:
✓ 平均时长 20-25分钟
✓ 短局(<15min) < 15%
✓ 超长局(>40min) < 10%

AI难度:
✓ 简单AI胜率 20-30%
✓ 普通AI胜率 40-50%
✓ 困难AI胜率 60-70%
✓ 专家AI胜率 75-85%
```

### 7.2 验收测试清单

- [ ] 1000局AI对战数据收集
- [ ] 所有势力胜率在目标范围内
- [ ] 经济指标符合预期
- [ ] 单位使用率分布合理
- [ ] 游戏时长分布合理
- [ ] 没有明显的滚雪球问题
- [ ] 各种地图类型平衡
- [ ] 各种游戏模式平衡

---

## 附录

### A. 相关文档

- `12_FACTIONS_BALANCE_TEST.md` - 平衡测试设计文档
- `MingBalanceTestSystem.h` - 平衡测试系统头文件

### B. 数据分析工具推荐

- **Excel/Google Sheets**: 基础数据分析
- **Python + Pandas**: 高级数据分析
- **Tableau/Power BI**: 可视化报表
- **R**: 统计分析

---

*文档版本*: 1.0  
*创建日期*: 2026-03-23
