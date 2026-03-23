# BMAD模式 - 至聖者指揮系統API需求規劃書

## 概述

基於《至聖者指揮學》七卷內容，將其指揮哲學轉化為可實現的C++ API系統。

---

## 一、核心概念映射

### 1.1 三權模型 (Three Authorities Model)
```cpp
UENUM(BlueprintType)
enum class ESupremeAuthorityType : uint8
{
    DaoAuthority,       // 道權 - 防墮監督
    StrategyAuthority,  // 策權 - 正逆切換
    MilitaryAuthority   // 兵權 - 執行載體
};
```

### 1.2 五行階段 (Five Elements Phases)
```cpp
UENUM(BlueprintType)
enum class EFiveElementPhase : uint8
{
    Wood,   // 木 - 立名正當性
    Fire,   // 火 - 造勢加速
    Earth,  // 土 - 收權止亂
    Metal,  // 金 - 裁斷清除
    Water   // 水 - 留白不動
};
```

### 1.3 正邪兵種 (Righteous/Evil Forces as Troop Types)
```cpp
UENUM(BlueprintType)
enum class EForceType : uint8
{
    Righteous,    // 正兵 - 公開可承
    Evil,         // 邪兵 - 隱蔽短期
    Hybrid        // 混合 - 正邪並用
};
```

### 1.4 六伐策略 (Six Conquest Strategies)
```cpp
UENUM(BlueprintType)
enum class ESixConquestStrategy : uint8
{
    ConquerMind,    // 伐心 - 奪其志
    ConquerSpirit,  // 伐氣 - 挫其銳
    ConquerMomentum,// 伐勢 - 逆其時
    ConquerLife,    // 伐命 - 斷其根
    ConquerShadow,  // 伐影 - 亂其象
    ConquerNothing  // 伐無 - 藏其跡
};
```

### 1.5 正逆六策 (Positive/Reverse Six Strategies)
```cpp
UENUM(BlueprintType)
enum class ETwelveStrategies : uint8
{
    // 正六策
    EstablishNation,    // 立國 - 伐心正道
    EstablishSystem,    // 立制 - 伐勢伐命正道
    EstablishPeople,    // 立人 - 伐氣伐影伐無正道
    
    // 逆六策
    BreakSituation,     // 破局 - 伐勢伐氣逆道
    BreakStructure,     // 破結構 - 伐命伐影逆道
    BreakNotPerson      // 不破人 - 伐心伐無逆道
};
```

---

## 二、API系統架構

### 2.1 核心類設計

#### UMingSupremeSageCommandSystem (至聖者指揮系統)
```cpp
UCLASS()
class MINGGORTS_API UMingSupremeSageCommandSystem : public UObject
{
    // ========== 三權管理 ==========
    UFUNCTION(BlueprintCallable)
    bool InitializeThreeAuthorities();
    
    UFUNCTION(BlueprintCallable)
    bool SetAuthorityActive(ESupremeAuthorityType AuthorityType, bool bActive);
    
    UFUNCTION(BlueprintCallable)
    FAuthorityStatus GetAuthorityStatus(ESupremeAuthorityType AuthorityType);
    
    // ========== 五行輪轉 ==========
    UFUNCTION(BlueprintCallable)
    bool EnterPhase(EFiveElementPhase Phase, const FPhaseContext& Context);
    
    UFUNCTION(BlueprintCallable)
    bool TransitionToNextPhase();
    
    UFUNCTION(BlueprintCallable)
    EFiveElementPhase GetCurrentPhase();
    
    UFUNCTION(BlueprintCallable)
    bool DeployForceInPhase(EForceType ForceType, const FDeploymentConfig& Config);
    
    // ========== 六伐執行 ==========
    UFUNCTION(BlueprintCallable)
    FConquestResult ExecuteSixConquest(
        ESixConquestStrategy Strategy, 
        EStrategyApproach Approach,
        const FConquestTarget& Target
    );
    
    // ========== 正逆策略 ==========
    UFUNCTION(BlueprintCallable)
    FStrategyResult ExecuteTwelveStrategy(
        ETwelveStrategies Strategy,
        const FStrategyContext& Context
    );
    
    // ========== 防墮機制 ==========
    UFUNCTION(BlueprintCallable)
    FCorruptionCheckResult PerformCorruptionCheck();
    
    UFUNCTION(BlueprintCallable)
    bool IsCorruptionSignPresent(ECorruptionSign Sign);
    
    UFUNCTION(BlueprintCallable)
    FSelfAuditReport GenerateSelfAuditReport();
    
    UFUNCTION(BlueprintCallable)
    bool ShouldTransferCommand();
};
```

#### UMingThreeAuthoritiesManager (三權管理器)
```cpp
UCLASS()
class MINGGORTS_API UMingThreeAuthoritiesManager : public UObject
{
    // 道權 - 防墮監督
    UFUNCTION(BlueprintCallable)
    FDaoAuthorityDecision DaoAuthorityCheck(const FCommandSituation& Situation);
    
    // 策權 - 正逆切換決策
    UFUNCTION(BlueprintCallable)
    FStrategySwitchDecision StrategyAuthorityDecideSwitch(
        EFiveElementPhase CurrentPhase,
        const FSituationAssessment& Assessment
    );
    
    // 兵權 - 執行管理
    UFUNCTION(BlueprintCallable)
    bool MilitaryAuthorityExecute(const FExecutionOrder& Order);
};
```

#### UMingFiveElementsRotation (五行輪轉器)
```cpp
UCLASS()
class MINGGORTS_API UMingFiveElementsRotation : public UObject
{
    // 階段管理
    UFUNCTION(BlueprintCallable)
    bool StartWoodPhase(const FWoodPhaseConfig& Config);  // 木階段 - 立名
    UFUNCTION(BlueprintCallable)
    bool StartFirePhase(const FFirePhaseConfig& Config);  // 火階段 - 造勢
    UFUNCTION(BlueprintCallable)
    bool StartEarthPhase(const FEarthPhaseConfig& Config);  // 土階段 - 收權
    UFUNCTION(BlueprintCallable)
    bool StartMetalPhase(const FMetalPhaseConfig& Config);  // 金階段 - 裁斷
    UFUNCTION(BlueprintCallable)
    bool StartWaterPhase(const FWaterPhaseConfig& Config);  // 水階段 - 留白
    
    // 正邪輪轉
    UFUNCTION(BlueprintCallable)
    bool RotateRighteousAndEvil(
        EFiveElementPhase Phase,
        const FRotationConfig& Config
    );
    
    // 檢查階段轉換時機
    UFUNCTION(BlueprintCallable)
    bool IsPhaseTransitionAppropriate(EFiveElementPhase From, EFiveElementPhase To);
};
```

#### UMingCorruptionPrevention (防墮系統)
```cpp
UCLASS()
class MINGGORTS_API UMingCorruptionPrevention : public UObject
{
    // 三大墮落徵象檢測
    UFUNCTION(BlueprintCallable)
    bool CheckRationalizationOfEvil();      // 將非常手段合理化
    UFUNCTION(BlueprintCallable)
    bool CheckFollowerDeification();        // 將追隨者神聖化
    UFUNCTION(BlueprintCallable)
    bool CheckOppositionDemonization();     // 將反對者妖魔化
    
    // 最高戒律檢查
    UFUNCTION(BlueprintCallable)
    bool CheckIAmJusticeSyndrome();         // 「我即正義」徵象
    
    // 戒令發布
    UFUNCTION(BlueprintCallable)
    bool IssueWarningCommand(EWarningLevel Level, const FWarningContent& Content);
    
    // 權力交接判斷
    UFUNCTION(BlueprintCallable)
    bool EvaluateCommandTransferNecessity();
};
```

---

## 三、API優先級與實現順序

### P0 - 最高優先級 (核心骨架)
| 功能 | 估計工期 | 依賴 |
|------|---------|------|
| 三權模型基礎架構 | 2天 | 無 |
| 五行階段狀態機 | 2天 | 無 |
| 正邪兵種切換機制 | 2天 | 五行階段 |
| 防墮監測基礎 | 2天 | 三權模型 |

### P1 - 高優先級 (核心功能)
| 功能 | 估計工期 | 依賴 |
|------|---------|------|
| 六伐策略執行器 | 3天 | 五行階段 |
| 正逆十二策實現 | 3天 | 六伐策略 |
| 三權決策邏輯 | 3天 | 三權模型 |
| 五行輪轉自動化 | 2天 | 五行階段 |

### P2 - 中優先級 (高級功能)
| 功能 | 估計工期 | 依賴 |
|------|---------|------|
| 三大墮落徵象AI檢測 | 3天 | 防墮基礎 |
| 最高戒律判斷系統 | 2天 | 防墮基礎 |
| 自我審核報告生成 | 2天 | 三權決策 |
| 歷史指揮記錄分析 | 2天 | 自我審核 |

### P3 - 低優先級 (優化功能)
| 功能 | 估計工期 | 依賴 |
|------|---------|------|
| 智能建議系統 | 3天 | 全部P0-P2 |
| 預測分析系統 | 3天 | 智能建議 |
| 至聖者評分系統 | 2天 | 歷史分析 |
| 多人指揮協調 | 4天 | 全部基礎 |

---

## 四、數據結構定義

### 4.1 核心配置結構
```cpp
// 階段上下文
USTRUCT(BlueprintType)
struct FPhaseContext
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString PhaseName;
    
    UPROPERTY()
    float DurationEstimate;
    
    UPROPERTY()
    EForceType PrimaryForce;
    
    UPROPERTY()
    TArray<FString> Objectives;
    
    UPROPERTY()
    FDateTime ExpectedEndTime;
};

// 三權狀態
USTRUCT(BlueprintType)
struct FAuthorityStatus
{
    GENERATED_BODY()
    
    UPROPERTY()
    ESupremeAuthorityType AuthorityType;
    
    UPROPERTY()
    bool bIsActive;
    
    UPROPERTY()
    float HealthLevel;  // 0-1
    
    UPROPERTY()
    float CorruptionRisk;  // 0-1
    
    UPROPERTY()
    FString LastAction;
    
    UPROPERTY()
    FDateTime LastCheckTime;
};

// 征服目標
USTRUCT(BlueprintType)
struct FConquestTarget
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString TargetID;
    
    UPROPERTY()
    FString TargetName;
    
    UPROPERTY()
    TMap<FString, float> WeaknessMap;  // 弱點分析
    
    UPROPERTY()
    float ResistanceLevel;  // 抵抗程度
    
    UPROPERTY()
    float StrategicValue;   // 戰略價值
};

// 征服結果
USTRUCT(BlueprintType)
struct FConquestResult
{
    GENERATED_BODY()
    
    UPROPERTY()
    bool bSuccess;
    
    UPROPERTY()
    float Effectiveness;  // 0-1
    
    UPROPERTY()
    float MoralCost;      // 道德成本
    
    UPROPERTY()
    float ReputationImpact;
    
    UPROPERTY()
    FString OutcomeDescription;
    
    UPROPERTY()
    TArray<FString> SideEffects;
};

// 墮落徵象檢測結果
USTRUCT(BlueprintType)
struct FCorruptionCheckResult
{
    GENERATED_BODY()
    
    UPROPERTY()
    bool bRationalizationOfEvil;
    
    UPROPERTY()
    bool bFollowerDeification;
    
    UPROPERTY()
    bool bOppositionDemonization;
    
    UPROPERTY()
    bool bIAmJusticeSyndrome;
    
    UPROPERTY()
    float OverallCorruptionRisk;  // 0-1
    
    UPROPERTY()
    FString WarningMessage;
    
    UPROPERTY()
    bool bShouldTransferCommand;
};

// 自我審核報告
USTRUCT(BlueprintType)
struct FSelfAuditReport
{
    GENERATED_BODY()
    
    UPROPERTY()
    FDateTime AuditTime;
    
    UPROPERTY()
    int32 TotalCommandsIssued;
    
    UPROPERTY()
    int32 EvilForceUsages;
    
    UPROPERTY()
    int32 PhaseTransitions;
    
    UPROPERTY()
    float AverageDecisionQuality;
    
    UPROPERTY()
    TArray<FString> CriticalDecisions;
    
    UPROPERTY()
    FCorruptionCheckResult CorruptionStatus;
    
    UPROPERTY()
    FString Recommendations;
};
```

---

## 五、事件系統

### 5.1 核心事件委派
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EFiveElementPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAuthoritySwitch, ESupremeAuthorityType, From, ESupremeAuthorityType, To);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCorruptionSignDetected, ECorruptionSign, Sign);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWarningCommandIssued, FWarningContent, Warning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyExecuted, FStrategyResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCommandTransferRequired);
```

### 5.2 事件觸發條件
- **階段轉換**: 當五行階段發生變化時
- **三權切換**: 當決策權在道/策/兵之間轉移時
- **墮落徵象**: 當檢測到三大墮落徵象之一時
- **戒令發布**: 當道權發出警告或命令時
- **策略執行**: 當六伐或十二策執行完成時
- **權力交接**: 當系統判斷應當移交指揮權時

---

## 六、使用範例

### 6.1 基礎使用流程
```cpp
// 初始化至聖者指揮系統
UMingSupremeSageCommandSystem* SupremeSystem = 
    NewObject<UMingSupremeSageCommandSystem>();
SupremeSystem->InitializeThreeAuthorities();

// 進入木階段 (立名)
FPhaseContext WoodContext;
WoodContext.PhaseName = TEXT("建立正當性");
WoodContext.PrimaryForce = EForceType::Righteous;
SupremeSystem->EnterPhase(EFiveElementPhase::Wood, WoodContext);

// 在木火階段使用正旗邪燃料
SupremeSystem->DeployForceInPhase(EForceType::Righteous, RighteousConfig);
SupremeSystem->DeployForceInPhase(EForceType::Evil, EvilConfig);

// 執行伐心策略
FConquestTarget Target;
Target.TargetName = TEXT("敵方士氣");
FConquestResult Result = SupremeSystem->ExecuteSixConquest(
    ESixConquestStrategy::ConquerMind,
    EStrategyApproach::Righteous,
    Target
);

// 定期防墮檢查
FCorruptionCheckResult Check = SupremeSystem->PerformCorruptionCheck();
if (Check.bShouldTransferCommand)
{
    // 執行權力交接
}
```

---

## 七、集成計劃

### 7.1 與現有SageBrain系統集成
```cpp
// 在MingSageBrainBridge中添加至聖者系統接口
class UMingSageBrainBridge
{
    UPROPERTY()
    UMingSupremeSageCommandSystem* SupremeSageSystem;
    
    UFUNCTION(BlueprintCallable, Category = "SupremeSage")
    bool InitializeSupremeSageSystem();
    
    UFUNCTION(BlueprintCallable, Category = "SupremeSage")
    FCommandRecommendation GetSageRecommendation(const FSituationAnalysis& Situation);
};
```

### 7.2 與AI系統集成
- 將至聖者指揮邏輯注入AI決策系統
- AI單位執行六伐策略
- 動態歷史事件影響指揮決策

### 7.3 與任務系統集成
- 基於五行階段生成對應任務
- 三權決策影響任務類型
- 防墮機制觸發特殊任務

---

## 八、測試計劃

### 8.1 單元測試清喉
- [ ] 三權初始化與切換
- [ ] 五行階段轉換
- [ ] 正邪兵種部署
- [ ] 六伐策略執行
- [ ] 墮落徵象檢測
- [ ] 最高戒律判斷

### 8.2 集成測試
- [ ] 完整指揮流程
- [ ] 多階段輪轉
- [ ] 三權協作
- [ ] 防墮機制觸發

### 8.3 壓力測試
- [ ] 長時間運行穩定性
- [ ] 高頻決策處理
- [ ] 並發指揮操作

---

## 九、風險評估

| 風險 | 概率 | 影響 | 對策 |
|------|------|------|------|
| 哲學概念難以量化 | 高 | 中 | 建立評分模型和閾值 |
| 防墮機制過於敏感 | 中 | 高 | 可調整參數和校準 |
| 三權決策衝突 | 中 | 中 | 建立優先級規則 |
| 性能開銷過大 | 低 | 中 | 異步計算和緩存 |

---

## 十、里程碑

### 里程碑1: 核心骨架 (P0完成)
- 三權模型運行
- 五行階段轉換
- 基礎防墮檢測

### 里程碑2: 功能完整 (P0-P1完成)
- 六伐策略可執行
- 正逆十二策實現
- 自動輪轉運作

### 里程碑3: 防墮完善 (P0-P2完成)
- AI檢測墮落徵象
- 自動戒令發布
- 權力交接判斷

### 里程碑4: 系統成熟 (P0-P3完成)
- 智能建議系統
- 預測分析功能
- 多人協調機制

---

**規劃完成時間**: 預計12週完成全部功能
**關鍵路徑**: 三權模型 → 五行輪轉 → 六伐執行 → 防墮完善
