# MingRTSSelfLearningSystem 系统文档

## 概述

MingRTSSelfLearningSystem 是一个自我学习系统，利用机器学习算法从玩家行为、游戏指标和系统性能中学习，提供个性化的游戏体验和智能AI适应。

## 核心功能

### 1. 学习数据类型
支持8种学习数据类型：
- **UserBehavior**: 用户行为 - 点击、移动、选择等操作
- **GameMetrics**: 游戏指标 - 胜率、分数、时长等
- **CulturalPreferences**: 文化偏好 - 语言、地区设置
- **AudioPreferences**: 音频偏好 - 音量、音效类型
- **UIPreferences**: UI偏好 - 界面布局、主题
- **ContentPreferences**: 内容偏好 - 喜欢/不喜欢的内容
- **AIPlayerStrategies**: AI玩家策略 - 战术模式识别
- **SystemPerformance**: 系统性能 - FPS、内存使用等

### 2. 机器学习算法
支持7种算法：
- **ReinforcementLearning**: 强化学习 - 基于奖励的学习
- **NeuralNetwork**: 神经网络 - 深度模式识别
- **Clustering**: 聚类 - 用户分群
- **DecisionTree**: 决策树 - 规则提取
- **Bayesian**: 贝叶斯 - 概率推理
- **GeneticAlgorithm**: 遗传算法 - 进化优化
- **CollaborativeFiltering**: 协同过滤 - 推荐系统

### 3. 学习信心水平
5种信心水平：
- **VeryLow**: 很低 - < 30%
- **Low**: 低 - 30-50%
- **Medium**: 中等 - 50-70%
- **High**: 高 - 70-85%
- **VeryHigh**: 很高 - > 85%

## API 参考

### 系统初始化

```cpp
// 初始化学习系统
void InitializeSelfLearningSystem();

// 关闭学习系统
void ShutdownSelfLearningSystem();
```

### 数据收集

```cpp
// 收集用户行为数据
void CollectUserBehaviorData(const FUserBehaviorData& Data);

// 收集游戏指标数据
void CollectGameMetricsData(const FGameMetricsData& Data);

// 收集性能数据
void CollectPerformanceData(const FSystemPerformanceData& Data);

// 批量收集数据
void CollectDataBatch(const TArray<FLearningDataPoint>& DataPoints);

// 获取数据历史
TArray<FLearningDataPoint> GetDataHistory(ELearningDataType Type, 
    int32 MaxCount = 100) const;
```

### 模型管理

```cpp
// 创建学习模型
FString CreateLearningModel(const FString& ModelName, const FLearningModelConfig& Config);

// 删除学习模型
void DeleteLearningModel(const FString& ModelID);

// 训练模型
bool TrainLearningModel(const FString& ModelID);

// 保存模型
bool SaveModel(const FString& ModelID, const FString& FilePath);

// 加载模型
FString LoadModel(const FString& FilePath);

// 获取模型信息
FLearningModelInfo GetModelInfo(const FString& ModelID) const;

// 获取所有模型
TArray<FString> GetAllModelIDs() const;
```

### 预测与推荐

```cpp
// 进行预测
FPredictionResult Predict(const FString& ModelID, const TArray<float>& InputData);

// 获取推荐
TArray<FRecommendation> GetRecommendations(ELearningDataType Type, 
    const FString& TargetID, int32 Count = 5);

// 获取个性化设置
TMap<FString, float> GetPersonalizedSettings(const FString& UserID);

// 预测玩家策略
EPlayerStrategy PredictPlayerStrategy(const FString& PlayerID);
```

### 适应控制

```cpp
// 应用适应
void ApplyAdaptation(const FString& UserID, EAdaptationType Type);

// 获取适应参数
FAdaptationParameters GetAdaptationParameters(const FString& UserID) const;

// 设置适应强度
void SetAdaptationIntensity(float Intensity);

// 重置用户适应
void ResetUserAdaptation(const FString& UserID);
```

### 系统集成

```cpp
// 集成到AI系统
void IntegrateWithAISystem(UObject* AISystem);

// 集成到文化系统
void IntegrateWithCulturalSystem(UObject* CulturalSystem);

// 集成到音频系统
void IntegrateWithAudioSystem(UObject* AudioSystem);

// 集成到UI系统
void IntegrateWithUISystem(UObject* UISystem);

// 集成到内容系统
void IntegrateWithContentSystem(UObject* ContentSystem);
```

## 数据结构

### FLearningModelConfig
```cpp
struct FLearningModelConfig
{
    ELearningAlgorithm Algorithm;         // 算法类型
    ELearningDataType TargetDataType;     // 目标数据类型
    TArray<FString> FeatureNames;         // 特征名称
    int32 HiddenLayers;                   // 隐藏层数（神经网络）
    int32 NeuronsPerLayer;                // 每层神经元数
    float LearningRate;                   // 学习率
    int32 TrainingEpochs;               // 训练轮数
    float ConvergenceThreshold;           // 收敛阈值
    int32 BatchSize;                    // 批量大小
    bool bUseGPU;                       // 是否使用GPU
};
```

### FUserBehaviorData
```cpp
struct FUserBehaviorData
{
    FString UserID;                       // 用户ID
    FString ActionType;                   // 动作类型
    FString TargetObject;               // 目标对象
    FVector Location;                   // 位置
    float Timestamp;                      // 时间戳
    float Duration;                     // 持续时间
    FString Context;                    // 上下文
    TMap<FString, FString> Metadata;      // 元数据
};
```

### FGameMetricsData
```cpp
struct FGameMetricsData
{
    FString UserID;                       // 用户ID
    FString MatchID;                      // 比赛ID
    float WinRate;                      // 胜率
    float AverageScore;                 // 平均分数
    float AverageDuration;              // 平均时长
    int32 TotalMatches;                 // 总比赛数
    TMap<FString, float> DetailedMetrics; // 详细指标
    float Timestamp;                      // 时间戳
};
```

### FSystemPerformanceData
```cpp
struct FSystemPerformanceData
{
    float FPS;                          // 帧率
    float FrameTime;                    // 帧时间
    float MemoryUsage;                  // 内存使用(MB)
    float CPUUsage;                     // CPU使用率
    float GPUUsage;                     // GPU使用率
    int32 DrawCalls;                    // 绘制调用数
    int32 TriangleCount;                // 三角形数
    FString HardwareProfile;              // 硬件配置
    float Timestamp;                      // 时间戳
};
```

### FLearningDataPoint
```cpp
struct FLearningDataPoint
{
    FString DataID;                     // 数据ID
    ELearningDataType DataType;         // 数据类型
    TMap<FString, float> Features;        // 特征值
    float Label;                        // 标签值
    float Weight;                       // 权重
    float Timestamp;                      // 时间戳
    bool bIsLabeled;                    // 是否有标签
};
```

### FLearningModelInfo
```cpp
struct FLearningModelInfo
{
    FString ModelID;                    // 模型ID
    FString ModelName;                  // 模型名称
    ELearningAlgorithm Algorithm;       // 算法
    ELearningDataType DataType;         // 数据类型
    ELearningConfidence ConfidenceLevel; // 信心水平
    float Accuracy;                     // 准确率
    float TrainingTime;                 // 训练时间
    int32 TrainingSamples;              // 训练样本数
    bool bIsTrained;                    // 是否已训练
    FString ModelPath;                  // 模型路径
};
```

### FPredictionResult
```cpp
struct FPredictionResult
{
    float PredictedValue;               // 预测值
    float Confidence;                   // 置信度
    TArray<float> Probabilities;        // 概率分布
    ELearningConfidence ConfidenceLevel; // 信心水平
    FString ModelID;                    // 使用的模型ID
};
```

### FRecommendation
```cpp
struct FRecommendation
{
    FString ItemID;                     // 推荐项ID
    FString ItemName;                   // 推荐项名称
    float RelevanceScore;               // 相关度分数
    ERecommendationReason Reason;         // 推荐理由
    FString Explanation;                // 解释
};
```

### FAdaptationParameters
```cpp
struct FAdaptationParameters
{
    float DifficultyMultiplier;         // 难度乘数
    float ResourceMultiplier;           // 资源乘数
    float AIAggressiveness;             // AI侵略性
    float ContentPersonalization;       // 内容个性化度
    float UIPreferenceScore;            // UI偏好分数
    float AudioPreferenceScore;         // 音频偏好分数
    TMap<FString, float> CustomParams;  // 自定义参数
};
```

## 事件委托

```cpp
// 数据收集
FOnDataCollected OnDataCollected;

// 模型训练完成
FOnModelTrained OnModelTrained;

// 预测完成
FOnPredictionMade OnPredictionMade;

// 适应应用
FOnAdaptationApplied OnAdaptationApplied;
```

## 使用示例

### 初始化系统
```cpp
UMingRTSSelfLearningSystem* LearningSystem = NewObject<UMingRTSSelfLearningSystem>();
LearningSystem->InitializeSelfLearningSystem();
```

### 收集数据
```cpp
// 收集用户行为
FUserBehaviorData BehaviorData;
BehaviorData.UserID = TEXT("Player123");
BehaviorData.ActionType = TEXT("UnitSelection");
BehaviorData.TargetObject = TEXT("Warrior");
BehaviorData.Location = FVector(100.0f, 200.0f, 0.0f);
BehaviorData.Duration = 0.5f;
BehaviorData.Context = TEXT("Battle");
LearningSystem->CollectUserBehaviorData(BehaviorData);

// 收集游戏指标
FGameMetricsData MetricsData;
MetricsData.UserID = TEXT("Player123");
MetricsData.WinRate = 0.75f;
MetricsData.AverageScore = 1250.0f;
MetricsData.TotalMatches = 50;
LearningSystem->CollectGameMetricsData(MetricsData);
```

### 创建和训练模型
```cpp
// 配置神经网络模型
FLearningModelConfig Config;
Config.Algorithm = ELearningAlgorithm::NeuralNetwork;
Config.TargetDataType = ELearningDataType::UserBehavior;
Config.FeatureNames = {TEXT("APM"), TEXT("ReactionTime"), TEXT("StrategyScore")};
Config.HiddenLayers = 3;
Config.NeuronsPerLayer = 64;
Config.LearningRate = 0.001f;
Config.TrainingEpochs = 1000;
Config.bUseGPU = true;

FString ModelID = LearningSystem->CreateLearningModel(TEXT("PlayerSkillModel"), Config);

// 训练模型
if (LearningSystem->TrainLearningModel(ModelID))
{
    UE_LOG(LogTemp, Log, TEXT("Model training completed!"));
    FLearningModelInfo Info = LearningSystem->GetModelInfo(ModelID);
    UE_LOG(LogTemp, Log, TEXT("Accuracy: %.2f%%"), Info.Accuracy * 100.0f);
}
```

### 进行预测
```cpp
// 准备输入数据
TArray<float> InputData = {120.0f, 0.8f, 0.75f};  // APM, ReactionTime, StrategyScore

// 预测玩家技能水平
FPredictionResult Result = LearningSystem->Predict(TEXT("PlayerSkillModel"), InputData);

UE_LOG(LogTemp, Log, TEXT("Predicted Skill Level: %.2f"), Result.PredictedValue);
UE_LOG(LogTemp, Log, TEXT("Confidence: %.2f"), Result.Confidence);
```

### 获取推荐
```cpp
// 获取个性化推荐
TArray<FRecommendation> Recommendations = LearningSystem->GetRecommendations(
    ELearningDataType::ContentPreferences, TEXT("Player123"), 5);

for (const FRecommendation& Rec : Recommendations)
{
    UE_LOG(LogTemp, Log, TEXT("Recommended: %s (Score: %.2f)"), 
        *Rec.ItemName, Rec.RelevanceScore);
}
```

### 应用适应
```cpp
// 应用难度适应
LearningSystem->ApplyAdaptation(TEXT("Player123"), EAdaptationType::Difficulty);

// 获取适应参数
FAdaptationParameters Params = LearningSystem->GetAdaptationParameters(TEXT("Player123"));

UE_LOG(LogTemp, Log, TEXT("Difficulty Multiplier: %.2f"), Params.DifficultyMultiplier);
UE_LOG(LogTemp, Log, TEXT("AI Aggressiveness: %.2f"), Params.AIAggressiveness);
```

### 系统集成
```cpp
// 集成到AI系统
LearningSystem->IntegrateWithAISystem(MyAIController);

// 集成到UI系统
LearningSystem->IntegrateWithUISystem(MyUIManager);

// 绑定事件
LearningSystem->OnModelTrained.AddDynamic(this, &AMyGameMode::OnModelTrainingComplete);
```

## 性能考虑

- **数据收集**: 每秒最多1000个数据点
- **模型训练**: 建议离线或低峰期进行
- **预测延迟**: < 10ms 每次预测
- **内存使用**: 根据模型复杂度，50-500MB

## 最佳实践

1. **数据隐私**: 确保符合数据保护法规
2. **数据质量**: 清理和验证输入数据
3. **模型选择**: 根据问题复杂度选择合适算法
4. **持续学习**: 定期重新训练模型
5. **可解释性**: 记录预测原因和推荐解释
6. **A/B测试**: 测试不同适应策略效果

## 扩展性

系统支持以下扩展：
- 自定义学习算法
- 新的数据类型
- 外部数据源集成
- 联邦学习
- 云端模型训练
