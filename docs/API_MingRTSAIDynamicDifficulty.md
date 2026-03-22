# MingRTSAIDynamicDifficulty 系統文檔 / MingRTSAIDynamicDifficulty System Documentation

## 概述 / Overview

MingRTSAIDynamicDifficulty 是一個AI難度動態平衡系統，根據玩家技能和表現自動調整遊戲難度，提供個性化的遊戲體驗。

MingRTSAIDynamicDifficulty is an AI dynamic difficulty balancing system that automatically adjusts game difficulty based on player skill and performance, providing a personalized gaming experience.

## 核心功能 / Core Features

### 1. 難度等級 / Difficulty Levels
系統支持7種難度等級 / System supports 7 difficulty levels：
- **VeryEasy**: 非常簡單 / Very Easy - 適合新手玩家 / For new players
- **Easy**: 簡單 / Easy - 降低挑戰 / Reduced challenge
- **Normal**: 正常 / Normal - 標準體驗 / Standard experience
- **Hard**: 困難 / Hard - 增加挑戰 / Increased challenge
- **VeryHard**: 非常困難 / Very Hard - 高難度挑戰 / High difficulty challenge
- **Nightmare**: 噩夢級 / Nightmare - 極限挑戰 / Extreme challenge
- **Adaptive**: 自適應 / Adaptive - 根據玩家表現動態調整 / Dynamically adjusts based on performance

### 2. 玩家技能評估 / Player Skill Assessment
跟踪10項玩家技能指標 / Tracks 10 player skill metrics：
- **ActionsPerMinute (APM)**: 每分鐘操作數 / Actions per minute
- **ReactionTime**: 反應時間 / Reaction time
- **StrategicThinking**: 戰略思維能力 / Strategic thinking ability
- **ResourceManagement**: 資源管理能力 / Resource management ability
- **TacticalExecution**: 戰術執行能力 / Tactical execution ability
- **Adaptability**: 適應能力 / Adaptability
- **UnitControl**: 單位控制能力 / Unit control ability
- **MapAwareness**: 地圖意識 / Map awareness
- **EconomyManagement**: 經濟管理 / Economy management
- **CombatEfficiency**: 戰鬥效率 / Combat efficiency

### 3. 動態調整類型 / Dynamic Adjustment Types
- **AIIntelligence**: AI智能等級 / AI intelligence level
- **ResourceBonus**: AI資源加成 / AI resource bonus
- **ProductionSpeed**: 生產速度 / Production speed
- **CombatBonus**: 戰鬥加成 / Combat bonus
- **TechProgression**: 科技進度 / Tech progression
- **DefensiveStrength**: 防禦強度 / Defensive strength
- **Aggressiveness**: AI侵略性 / AI aggressiveness
- **UnitEfficiency**: 單位效率 / Unit efficiency
- **StrategicDepth**: 戰略深度 / Strategic depth

## API 參考 / API Reference

### 系统初始化

```cpp
// 初始化难度系统
void InitializeDifficultySystem();

// 关闭难度系统
void ShutdownDifficultySystem();
```

### 玩家评估

```cpp
// 更新玩家技能评估
void UpdatePlayerSkillAssessment(const FString& PlayerID, 
    const FPlayerSkillMetrics& Metrics);

// 获取玩家技能等级
float GetPlayerSkillLevel(const FString& PlayerID) const;

// 获取玩家难度等级
EDifficultyLevel GetPlayerDifficultyLevel(const FString& PlayerID) const;

// 设置玩家难度等级
void SetPlayerDifficultyLevel(const FString& PlayerID, EDifficultyLevel Level);
```

### 动态调整

```cpp
// 应用自适应调整
void ApplyAdaptiveAdjustments(const FString& PlayerID);

// 获取调整强度
float GetAdjustmentIntensity(const FString& PlayerID) const;

// 设置调整强度上限
void SetAdjustmentCap(float MaxAdjustment);

// 获取调整因子
float GetAdjustmentFactor(EDifficultyAdjustmentType Type, const FString& PlayerID) const;
```

### 会话管理

```cpp
// 开始追踪会话
void StartSessionTracking(const FString& PlayerID);

// 更新会话追踪
void UpdateSessionTracking(const FString& PlayerID, float CurrentPerformance);

// 结束会话追踪
FSessionStatistics EndSessionTracking(const FString& PlayerID);

// 获取会话统计数据
FSessionStatistics GetSessionStatistics(const FString& PlayerID) const;
```

### 挑战评估

```cpp
// 记录挑战
void RecordChallenge(const FString& PlayerID, const FChallengeData& Challenge);

// 评估挑战应对
void EvaluateChallengeResponse(const FString& PlayerID, 
    const FString& ChallengeID, bool bSuccess);

// 获取挑战记录
TArray<FChallengeRecord> GetChallengeHistory(const FString& PlayerID) const;
```

### 性能评估

```cpp
// 获取难度调整记录
TArray<FDifficultyAdjustmentRecord> GetAdjustmentHistory(const FString& PlayerID) const;

// 获取性能趋势
float GetPerformanceTrend(const FString& PlayerID) const;

// 是否应该调整难度
bool ShouldAdjustDifficulty(const FString& PlayerID) const;
```

### 全局设置

```cpp
// 设置调整间隔
void SetAdjustmentInterval(float IntervalSeconds);

// 设置平滑因子
void SetSmoothingFactor(float SmoothingFactor);

// 设置最小难度变化
void SetMinDifficultyChange(float MinChange);

// 设置最大难度变化
void SetMaxDifficultyChange(float MaxChange);
```

## 数据结构

### FPlayerSkillMetrics
```cpp
struct FPlayerSkillMetrics
{
    float ActionsPerMinute;             // 每分钟操作数
    float ReactionTime;                 // 反应时间(秒)
    float StrategicThinking;            // 战略思维(0-1)
    float ResourceManagement;           // 资源管理(0-1)
    float TacticalExecution;            // 战术执行(0-1)
    float Adaptability;                 // 适应能力(0-1)
    float UnitControl;                  // 单位控制(0-1)
    float MapAwareness;                 // 地图意识(0-1)
    float EconomyManagement;            // 经济管理(0-1)
    float CombatEfficiency;             // 战斗效率(0-1)
    
    // 计算综合技能分数
    float CalculateOverallSkill() const;
};
```

### FSessionStatistics
```cpp
struct FSessionStatistics
{
    FString SessionID;                  // 会话ID
    FString PlayerID;                   // 玩家ID
    float StartTime;                    // 开始时间
    float EndTime;                      // 结束时间
    float AveragePerformance;           // 平均表现
    float PeakPerformance;              // 峰值表现
    float LowestPerformance;            // 最低表现
    int32 DifficultyAdjustments;        // 难度调整次数
    float FinalDifficulty;              // 最终难度
    float SkillProgression;             // 技能进步
    TArray<float> PerformanceHistory;   // 表现历史
};
```

### FChallengeData
```cpp
struct FChallengeData
{
    FString ChallengeID;                // 挑战ID
    FString ChallengeType;              // 挑战类型
    float DifficultyRating;             // 难度评级
    float ExpectedSuccessRate;          // 预期成功率
    TMap<FString, float> Parameters;    // 挑战参数
};
```

### FChallengeRecord
```cpp
struct FChallengeRecord
{
    FString ChallengeID;                // 挑战ID
    FString ChallengeType;              // 挑战类型
    bool bSuccessful;                   // 是否成功
    float AttemptDuration;              // 尝试时长
    float PlayerPerformance;              // 玩家表现
    float Timestamp;                      // 时间戳
    FString FailureReason;              // 失败原因(如适用)
};
```

### FDifficultyAdjustmentRecord
```cpp
struct FDifficultyAdjustmentRecord
{
    float Timestamp;                    // 时间戳
    EDifficultyLevel OldLevel;          // 旧难度等级
    EDifficultyLevel NewLevel;          // 新难度等级
    TMap<EDifficultyAdjustmentType, float> Adjustments; // 具体调整
    FString Reason;                     // 调整原因
    float PlayerPerformance;          // 玩家表现
};
```

## 事件委托

```cpp
// 难度调整事件
FOnDifficultyAdjusted OnDifficultyAdjusted;

// 玩家技能评估事件
FOnPlayerSkillAssessed OnPlayerSkillAssessed;

// 会话统计事件
FOnSessionStatistics OnSessionStatistics;
```

## 使用示例

### 初始化系统
```cpp
UMingRTSAIDynamicDifficulty* DifficultySystem = NewObject<UMingRTSAIDynamicDifficulty>();
DifficultySystem->InitializeDifficultySystem();
```

### 注册玩家
```cpp
FPlayerSkillMetrics InitialMetrics;
InitialMetrics.ActionsPerMinute = 60.0f;
InitialMetrics.ReactionTime = 1.5f;
InitialMetrics.StrategicThinking = 0.5f;
// ... 设置其他指标

DifficultySystem->UpdatePlayerSkillAssessment(TEXT("Player1"), InitialMetrics);
DifficultySystem->SetPlayerDifficultyLevel(TEXT("Player1"), EDifficultyLevel::Normal);
```

### 开始会话追踪
```cpp
DifficultySystem->StartSessionTracking(TEXT("Player1"));

// 绑定事件
DifficultySystem->OnDifficultyAdjusted.AddDynamic(this, &AMyGameMode::OnDifficultyChanged);
```

### 实时更新玩家表现
```cpp
void AMyGameMode::OnPlayerAction(const FString& PlayerID, float PerformanceScore)
{
    DifficultySystem->UpdateSessionTracking(PlayerID, PerformanceScore);
    
    // 如果需要，应用自适应调整
    if (DifficultySystem->ShouldAdjustDifficulty(PlayerID))
    {
        DifficultySystem->ApplyAdaptiveAdjustments(PlayerID);
    }
}
```

### 记录挑战
```cpp
FChallengeData Challenge;
Challenge.ChallengeID = TEXT("BossBattle1");
Challenge.ChallengeType = TEXT("BossBattle");
Challenge.DifficultyRating = 0.8f;
Challenge.ExpectedSuccessRate = 0.6f;

DifficultySystem->RecordChallenge(TEXT("Player1"), Challenge);
```

### 评估挑战结果
```cpp
void AMyGameMode::OnBossBattleComplete(const FString& PlayerID, 
    const FString& ChallengeID, bool bVictory)
{
    DifficultySystem->EvaluateChallengeResponse(PlayerID, ChallengeID, bVictory);
}
```

### 获取调整因子
```cpp
float CombatBonus = DifficultySystem->GetAdjustmentFactor(
    EDifficultyAdjustmentType::CombatBonus, TEXT("Player1"));

float AIIntelligence = DifficultySystem->GetAdjustmentFactor(
    EDifficultyAdjustmentType::AIIntelligence, TEXT("Player1"));
```

### 结束会话
```cpp
FSessionStatistics Stats = DifficultySystem->EndSessionTracking(TEXT("Player1"));

UE_LOG(LogTemp, Log, TEXT("Session Summary:"));
UE_LOG(LogTemp, Log, TEXT("  Average Performance: %.2f"), Stats.AveragePerformance);
UE_LOG(LogTemp, Log, TEXT("  Difficulty Adjustments: %d"), Stats.DifficultyAdjustments);
UE_LOG(LogTemp, Log, TEXT("  Skill Progression: %.2f"), Stats.SkillProgression);
```

## 配置参数

### 默认设置
```cpp
// 调整间隔(秒)
constexpr float DefaultAdjustmentInterval = 300.0f;  // 5分钟

// 平滑因子(0-1，越高越平滑)
constexpr float DefaultSmoothingFactor = 0.3f;

// 最小难度变化(防止微小调整)
constexpr float DefaultMinDifficultyChange = 0.05f;

// 最大难度变化(防止突变)
constexpr float DefaultMaxDifficultyChange = 0.3f;

// 调整上限
constexpr float DefaultAdjustmentCap = 2.0f;
```

### 难度等级数值
```cpp
// 难度等级对应数值
VeryEasy   = 0.25f;
Easy       = 0.5f;
Normal     = 1.0f;
Hard       = 1.5f;
VeryHard   = 2.0f;
Nightmare  = 3.0f;
Adaptive   = 动态计算;
```

### 调整因子计算
```cpp
// AI智能等级
AIIntelligence = BaseValue * (1.0f + (DifficultyLevel - 1.0f) * 0.5f);

// 资源加成
ResourceBonus = BaseValue * DifficultyLevel;

// 生产速度
ProductionSpeed = BaseValue * (1.0f + (DifficultyLevel - 1.0f) * 0.3f);

// 战斗加成
CombatBonus = BaseValue * DifficultyLevel;

// 科技进度
TechProgression = BaseValue * (1.0f + (DifficultyLevel - 1.0f) * 0.4f);

// 防御强度
DefensiveStrength = BaseValue * DifficultyLevel;

// AI侵略性
Aggressiveness = BaseValue * (0.5f + DifficultyLevel * 0.5f);

// 单位效率
UnitEfficiency = BaseValue * (0.8f + DifficultyLevel * 0.2f);

// 战略深度
StrategicDepth = BaseValue * DifficultyLevel;
```

## 性能考虑

- **更新频率**: 建议每5-10分钟评估一次，避免过于频繁的调整
- **玩家数量**: 系统可以同时跟踪最多1000名玩家
- **内存使用**: 会话数据在结束后自动清理
- **平滑处理**: 使用平滑因子防止难度突变，确保渐进式调整

## 最佳实践

1. **初始评估**: 在游戏开始时进行初始技能评估
2. **渐进调整**: 允许玩家逐渐适应难度变化
3. **透明沟通**: 告知玩家难度正在调整（可选）
4. **多维度评估**: 综合考虑多种技能指标，而非单一指标
5. **挑战平衡**: 确保挑战既不太简单也不太困难
6. **反馈循环**: 根据玩家反馈调整评估算法

## 调试工具

```cpp
// 获取完整调整历史
TArray<FDifficultyAdjustmentRecord> History = 
    DifficultySystem->GetAdjustmentHistory(PlayerID);

// 获取性能趋势
float Trend = DifficultySystem->GetPerformanceTrend(PlayerID);
// Trend > 0: 玩家正在进步
// Trend < 0: 玩家表现下降
// Trend ≈ 0: 表现稳定

// 是否应该调整
bool ShouldAdjust = DifficultySystem->ShouldAdjustDifficulty(PlayerID);
```

## 扩展性

系统支持以下扩展：
- 添加新的玩家技能指标
- 自定义难度等级
- 实现新的调整算法
- 添加新的调整类型
- 集成外部数据源（如眼动追踪、心率等）
