# 至聖者指揮學系統性能優化指南

**版本**: v1.0  
**適用系統**: MingGoRTS - 至聖者指揮學 v0.9.0+  
**最後更新**: 2026-03-24

---

## 📋 目錄

1. [性能指標和目標](#性能指標和目標)
2. [內存優化](#內存優化)
3. [CPU性能優化](#cpu性能優化)
4. [渲染和幀率優化](#渲染和幀率優化)
5. [網絡同步優化](#網絡同步優化)
6. [數據結構優化](#數據優化)
7. [事件系統優化](#事件系統優化)
8. [最佳實踐](#最佳實踐)
9. [性能監控工具](#性能監控工具)
10. [常見性能問題](#常見性能問題)

---

## 🎯 性能指標和目標

### 目標性能指標

| 指標 | 目標值 | 可接受範圍 | 警告閾值 |
|------|--------|------------|----------|
| **平均幀時間** | < 16.67ms | < 20ms | > 25ms |
| **最小幀率** | 60 FPS | 50 FPS | 40 FPS |
| **內存使用** | < 200MB | < 300MB | > 400MB |
| **CPU使用率** | < 20% | < 30% | > 40% |
| **策略執行延遲** | < 50ms | < 100ms | > 200ms |
| **五行節奏更新** | < 10ms | < 20ms | > 30ms |
| **AI決策時間** | < 100ms | < 200ms | > 500ms |

### 性能測試場景

1. **基準測試**: 單玩家，基礎策略執行
2. **壓力測試**: 多AI對手，複雜策略組合
3. **極限測試**: 最大並發策略，頻繁權力轉移
4. **長時間測試**: 持續運行1小時以上

---

## 💾 內存優化

### 1. 對象池化

```cpp
// 使用對象池減少內存分配
class FStrategyEffectPool
{
private:
    TArray<FStrategyEffectDetails> AvailableEffects;
    TArray<FStrategyEffectDetails> UsedEffects;
    
public:
    FStrategyEffectDetails* AcquireEffect()
    {
        if (AvailableEffects.Num() > 0)
        {
            FStrategyEffectDetails Effect = AvailableEffects.Pop();
            UsedEffects.Add(Effect);
            return &UsedEffects.Last();
        }
        
        // 創建新效果
        UsedEffects.Add(FStrategyEffectDetails());
        return &UsedEffects.Last();
    }
    
    void ReleaseEffect(FStrategyEffectDetails* Effect)
    {
        // 重置並回收到池
        Effect->Reset();
        AvailableEffects.Add(*Effect);
    }
};
```

### 2. 數據結構選擇

```cpp
// 優化前：使用 TArray 頻繁查找
TArray<FStrategyData> StrategyArray; // O(n) 查找

// 優化後：使用 TMap 快速查找
TMap<ESixStrategyType, FStrategyData> StrategyMap; // O(1) 查找

// 優化前：大量小對象
TArray<FString> EventListeners; // 內存碎片

// 優化後：預分配和緊湊存儲
TArray<FName> CompactListeners; // FName 比 FString 更省內存
```

### 3. 智能指針使用

```cpp
// 避免內存洩漏
class UMingThreePowerSystem
{
private:
    // 使用智能指針管理子系統
    TSharedPtr<UMingMoralAuthority> MoralAuthority;
    TSharedPtr<UMingStrategyAuthority> StrategyAuthority;
    TSharedPtr<UMingMilitaryAuthority> MilitaryAuthority;
    
public:
    void CleanupUnusedSystems()
    {
        // 自動清理未使用的系統
        if (MoralAuthority.IsValid() && !MoralAuthority->IsActive())
        {
            MoralAuthority.Reset();
        }
    }
};
```

### 4. 內存清理策略

```cpp
void OptimizeMemoryUsage()
{
    // 1. 定期清理過期數據
    AntiCorruptionSystem->ClearOldPhenomena(3600.0f); // 1小時前的數據
    
    // 2. 限制歷史記錄數量
    if (TestResultHistory.Num() > 100)
    {
        TestResultHistory.RemoveAt(0, TestResultHistory.Num() - 100);
    }
    
    // 3. 壓縮事件歷史
    if (EventHistory.Num() > MAX_EVENT_HISTORY)
    {
        // 只保留重要事件
        EventHistory.RemoveAll([](const FWuXingRhythmEvent& Event) {
            return Event.EventStrength < 0.5f;
        });
    }
    
    // 4. 強制垃圾回收（謹慎使用）
    // CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}
```

---

## ⚡ CPU性能優化

### 1. Tick頻率優化

```cpp
// 優化前：每幀更新
void Tick(float DeltaTime)
{
    UpdateAllSystems(); // 每幀都更新，浪費CPU
}

// 優化後：按需更新
void Tick(float DeltaTime)
{
    AccumulatedTime += DeltaTime;
    
    // 三權系統：每秒更新一次
    if (AccumulatedTime >= 1.0f)
    {
        ThreePowerSystem->Update();
        AccumulatedTime = 0.0f;
    }
    
    // 五行節奏：每0.1秒更新一次
    if (RhythmAccumulatedTime >= 0.1f)
    {
        WuXingSystem->UpdateRhythmState(RhythmAccumulatedTime);
        RhythmAccumulatedTime = 0.0f;
    }
}
```

### 2. 延遲計算

```cpp
// 優化前：立即計算
void ExecuteStrategy(ESixStrategyType StrategyType)
{
    CalculateImmediateEffects();      // 立即計算
    CalculateLongTermEffects();       // 立即計算
    UpdateAllDependentSystems();      // 立即更新
}

// 優化後：分批計算
void ExecuteStrategy(ESixStrategyType StrategyType)
{
    // 立即計算關鍵效果
    CalculateImmediateEffects();
    
    // 延遲計算非關鍵效果
    PendingCalculations.Add(StrategyType);
}

void ProcessPendingCalculations()
{
    // 每幀處理一部分
    int32 ProcessCount = FMath::Min(PendingCalculations.Num(), 5);
    for (int32 i = 0; i < ProcessCount; ++i)
    {
        CalculateLongTermEffects(PendingCalculations[i]);
    }
    PendingCalculations.RemoveAt(0, ProcessCount);
}
```

### 3. 並行計算

```cpp
// 使用多線程處理獨立計算
void ParallelStrategyCalculations()
{
    ParallelFor(Strategies.Num(), [&](int32 Index)
    {
        FStrategyData& Strategy = Strategies[Index];
        
        // 獨立計算每個策略的效果
        Strategy.CalculatedEffect = CalculateStrategyEffect(Strategy);
    });
}
```

### 4. 緩存和預計算

```cpp
class UMingSixStrategiesSystem
{
private:
    // 緩存常用的計算結果
    TMap<ESixStrategyType, float> CachedEffectiveness;
    TMap<EFiveElementPhase, float> CachedWuXingBonuses;
    
public:
    float GetWuXingBonus(ESixStrategyType StrategyType, EFiveElementPhase Phase)
    {
        // 檢查緩存
        TPair<ESixStrategyType, EFiveElementPhase> Key = MakeTuple(StrategyType, Phase);
        if (CachedWuXingBonuses.Contains(Key))
        {
            return CachedWuXingBonuses[Key];
        }
        
        // 計算並緩存
        float Bonus = CalculateWuXingBonusInternal(StrategyType, Phase);
        CachedWuXingBonuses.Add(Key, Bonus);
        
        return Bonus;
    }
    
    void InvalidateCache()
    {
        // 當配置改變時清除緩存
        CachedEffectiveness.Empty();
        CachedWuXingBonuses.Empty();
    }
};
```

---

## 🎨 渲染和幀率優化

### 1. UI更新優化

```cpp
// 優化前：每幀更新UI
void Tick(float DeltaTime)
{
    UpdateStrategyUI();    // 每幀更新
    UpdatePowerUI();       // 每幀更新
    UpdateRhythmUI();      // 每幀更新
}

// 優化後：按需更新
void Tick(float DeltaTime)
{
    // 只有數據改變時才更新UI
    if (bStrategyDataDirty)
    {
        UpdateStrategyUI();
        bStrategyDataDirty = false;
    }
}

// 數據改變時標記
void OnStrategyExecuted(ESixStrategyType StrategyType, bool bSuccess)
{
    bStrategyDataDirty = true;
}
```

### 2. 可見性判斷

```cpp
void UpdateVisualElements()
{
    // 只更新可見的策略效果
    for (auto& Effect : StrategyEffects)
    {
        if (Effect.bIsVisible && Effect.bIsOnScreen)
        {
            Effect.UpdateVisuals();
        }
        else
        {
            // 暫停不可見元素的更新
            Effect.PauseUpdates();
        }
    }
}
```

### 3. LOD（細節層次）系統

```cpp
void UpdateStrategyVisuals()
{
    float DistanceToCamera = GetDistanceToPlayer();
    
    if (DistanceToCamera < 1000.0f)
    {
        // 近距離：完整細節
        UpdateFullVisuals();
    }
    else if (DistanceToCamera < 5000.0f)
    {
        // 中距離：簡化細節
        UpdateSimplifiedVisuals();
    }
    else
    {
        // 遠距離：只顯示圖標
        UpdateIconOnly();
    }
}
```

---

## 🌐 網絡同步優化

### 1. 數據壓縮

```cpp
// 優化前：完整數據傳輸
struct FStrategyData
{
    ESixStrategyType StrategyType;    // 1 byte
    float EffectIntensity;             // 4 bytes
    FDateTime StartTime;               // 8 bytes
    TArray<FString> Targets;          // 變長
    FString Description;              // 變長
}; // 總計: 13+ bytes

// 優化後：壓縮傳輸
struct FCompressedStrategyData
{
    uint8 StrategyTypeIndex;           // 1 byte (索引而非枚舉)
    uint8 IntensityPercent;          // 1 byte (0-100)
    uint32 StartTimeUnix;            // 4 bytes (Unix時間戳)
    TArray<uint16> TargetIDs;         // 2 bytes per target
}; // 總計: 6+ bytes (節省50%+)
```

### 2. 增量同步

```cpp
void SyncStrategyData()
{
    // 只同步改變的數據
    for (auto& Strategy : ActiveStrategies)
    {
        if (Strategy.bHasChanged)
        {
            SendStrategyUpdate(Strategy);
            Strategy.bHasChanged = false;
        }
    }
    
    // 批量發送
    if (PendingUpdates.Num() > 10)
    {
        SendBatchUpdate(PendingUpdates);
        PendingUpdates.Empty();
    }
}
```

### 3. 優先級隊列

```cpp
void NetworkUpdate()
{
    // 按優先級排序
    PendingSyncs.Sort([](const FSyncData& A, const FSyncData& B) {
        return A.Priority > B.Priority;
    });
    
    // 只發送高優先級數據
    int32 SendCount = 0;
    for (auto& Sync : PendingSyncs)
    {
        if (SendCount >= MAX_SYNC_PER_FRAME) break;
        
        SendData(Sync);
        SendCount++;
    }
    
    // 剩餘的低優先級數據延遲到下一幀
}
```

---

## 📊 數據優化

### 1. 數據結構優化

```cpp
// 優化前：大量使用 FString
struct FStrategyEffect
{
    FString EffectName;       // 可變長，內存開銷大
    FString Description;
    FString TargetID;
};

// 優化後：使用 FName 和緊湊結構
struct FOptimizedStrategyEffect
{
    FName EffectName;         // 固定大小，內存友好
    FText Description;        // 本地化支持
    uint32 TargetID;          // 數字ID更高效
};
```

### 2. 數組預分配

```cpp
// 優化前：動態擴容
TArray<FStrategyEffect> Effects;
for (int32 i = 0; i < 1000; ++i)
{
    Effects.Add(FStrategyEffect()); // 頻繁重新分配
}

// 優化後：預分配
TArray<FStrategyEffect> Effects;
Effects.Reserve(1000); // 一次性分配
for (int32 i = 0; i < 1000; ++i)
{
    Effects.Emplace(FStrategyEffect()); // 更高效
}
```

### 3. 使用緊湊的位域

```cpp
// 使用位域節省內存
struct FCompactStrategyFlags
{
    uint16 bIsActive : 1;
    uint16 bIsVisible : 1;
    uint16 bIsPermanent : 1;
    uint16 bIsReversible : 1;
    uint16 Priority : 4;        // 0-15
    uint16 Category : 8;        // 0-255
}; // 總計: 2 bytes
```

---

## 📡 事件系統優化

### 1. 事件批處理

```cpp
// 優化前：每個事件單獨處理
void OnStrategyEffectApplied(EStrategyEffectType EffectType, float Value)
{
    UpdateUI(EffectType, Value);      // 立即更新
    PlaySound(EffectType);              // 立即播放
    LogEffect(EffectType, Value);       // 立即記錄
}

// 優化後：批量處理
void OnStrategyEffectApplied(EStrategyEffectType EffectType, float Value)
{
    // 添加到批處理隊列
    PendingEffects.Add(MakeTuple(EffectType, Value));
}

void ProcessPendingEffects()
{
    if (PendingEffects.Num() == 0) return;
    
    // 合併相似效果
    TMap<EStrategyEffectType, float> MergedEffects;
    for (auto& [Type, Value] : PendingEffects)
    {
        if (MergedEffects.Contains(Type))
        {
            MergedEffects[Type] += Value; // 合併數值
        }
        else
        {
            MergedEffects.Add(Type, Value);
        }
    }
    
    // 批量更新UI
    UpdateUIBatch(MergedEffects);
    
    PendingEffects.Empty();
}
```

### 2. 事件過濾

```cpp
void OnWuXingPhaseChanged(EFiveElementPhase NewPhase)
{
    // 過濾不重要的事件
    if (NewPhase == CurrentPhase) return; // 忽略無變化
    if (GetDistanceToPlayer() > 10000.0f) return; // 忽略遠處事件
    
    // 處理重要事件
    ProcessPhaseChange(NewPhase);
}
```

---

## ✅ 最佳實踐

### 1. 初始化優化

```cpp
void OptimizedInitialization()
{
    // 1. 按重要性排序初始化
    InitializeCriticalSystems();    // 首先初始化關鍵系統
    InitializeSecondarySystems(); // 然後初始化次要系統
    InitializeOptionalSystems();    // 最後初始化可選系統
    
    // 2. 異步初始化
    AsyncTask(ENamedThreads::AnyBackgroundThread, [=]()
    {
        InitializeHeavySystems(); // 在後台線程初始化重型系統
    });
}
```

### 2. 資源管理

```cpp
// 使用RAII管理資源
class FScopedStrategyExecution
{
private:
    UMingSixStrategiesSystem* System;
    ESixStrategyType StrategyType;
    
public:
    FScopedStrategyExecution(UMingSixStrategiesSystem* InSystem, ESixStrategyType InType)
        : System(InSystem), StrategyType(InType)
    {
        System->BeginStrategyExecution(StrategyType);
    }
    
    ~FScopedStrategyExecution()
    {
        System->EndStrategyExecution(StrategyType);
    }
};

// 使用
{
    FScopedStrategyExecution ScopedExec(StrategySystem, ESixStrategyType::EstablishNation);
    // 執行策略...
} // 自動清理
```

### 3. 配置優化

```ini
; 引擎配置優化
[/Script/Engine.Engine]
; 限制Tick頻率
MaxTickRate=60

[/Script/Engine.GameEngine]
; 優化垃圾回收
TimeBetweenPurgingPendingKillObjects=60

[/Script/MingGoRTS.MingSageCommandSystem]
; 系統特定配置
MaxConcurrentStrategies=3
StrategyUpdateInterval=0.5
WuXingRhythmUpdateInterval=0.1
AntiCorruptionCheckInterval=5.0
AIAdaptationUpdateInterval=1.0
```

---

## 🔧 性能監控工具

### 1. 內置性能監控

```cpp
// 使用UE5的內置分析器
DECLARE_CYCLE_STAT(TEXT("Strategy Execution"), STAT_StrategyExecution, STATGROUP_MingGoRTS);
DECLARE_CYCLE_STAT(TEXT("WuXing Update"), STAT_WuXingUpdate, STATGROUP_MingGoRTS);
DECLARE_CYCLE_STAT(TEXT("AI Decision"), STAT_AIDecision, STATGROUP_MingGoRTS);

void ExecuteStrategy(ESixStrategyType StrategyType)
{
    SCOPE_CYCLE_COUNTER(STAT_StrategyExecution);
    
    // 策略執行代碼...
}
```

### 2. 自定義性能監控

```cpp
class FPerformanceMonitor
{
public:
    void BeginSection(const FString& SectionName)
    {
        SectionStartTimes.Add(SectionName, FPlatformTime::Seconds());
    }
    
    void EndSection(const FString& SectionName)
    {
        if (SectionStartTimes.Contains(SectionName))
        {
            double StartTime = SectionStartTimes[SectionName];
            double Duration = FPlatformTime::Seconds() - StartTime;
            
            UE_LOG(LogTemp, Log, TEXT("[%s] 耗時: %.3fms"), 
                   *SectionName, Duration * 1000.0f);
            
            SectionStartTimes.Remove(SectionName);
        }
    }
    
private:
    TMap<FString, double> SectionStartTimes;
};
```

### 3. 內存監控

```cpp
void LogMemoryUsage()
{
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    
    UE_LOG(LogTemp, Log, TEXT("內存使用統計:"));
    UE_LOG(LogTemp, Log, TEXT("  已用物理內存: %.1f MB"), 
           MemoryStats.UsedPhysical / 1024.0f / 1024.0f);
    UE_LOG(LogTemp, Log, TEXT("  可用物理內存: %.1f MB"), 
           MemoryStats.AvailablePhysical / 1024.0f / 1024.0f);
    UE_LOG(LogTemp, Log, TEXT("  已用虛擬內存: %.1f MB"), 
           MemoryStats.UsedVirtual / 1024.0f / 1024.0f);
}
```

---

## 🐛 常見性能問題

### 問題1: 幀率下降

**症狀**: 幀率從60FPS下降到30FPS以下

**診斷**:
```cpp
void DiagnoseFrameRateIssues()
{
    // 1. 檢查Tick函數
    float TickTime = GetTickTime();
    if (TickTime > 16.67f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tick耗時過長: %.2fms"), TickTime);
    }
    
    // 2. 檢查策略數量
    int32 ActiveStrategies = SixStrategiesSystem->GetActiveStrategies().Num();
    if (ActiveStrategies > 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("活動策略過多: %d"), ActiveStrategies);
    }
    
    // 3. 檢查內存使用
    float MemoryUsage = GetMemoryUsage();
    if (MemoryUsage > 300.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("內存使用過高: %.1fMB"), MemoryUsage);
    }
}
```

**解決方案**:
- 減少並發策略數量
- 優化Tick更新頻率
- 清理過期數據

### 問題2: 內存洩漏

**症狀**: 內存使用持續增長，不下降

**診斷**:
```cpp
void CheckMemoryLeaks()
{
    static float LastMemoryUsage = 0.0f;
    float CurrentMemoryUsage = GetMemoryUsage();
    
    if (CurrentMemoryUsage > LastMemoryUsage + 50.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("內存增長異常: %.1fMB -> %.1fMB"),
               LastMemoryUsage, CurrentMemoryUsage);
        
        // 檢查對象數量
        int32 ActiveObjects = GetActiveObjectCount();
        UE_LOG(LogTemp, Warning, TEXT("活動對象數量: %d"), ActiveObjects);
    }
    
    LastMemoryUsage = CurrentMemoryUsage;
}
```

**解決方案**:
- 確保正確釋放對象
- 使用智能指針管理生命週期
- 定期運行垃圾回收

### 問題3: AI決策緩慢

**症狀**: AI反應時間超過500ms

**診斷**:
```cpp
void DiagnoseAISlowness()
{
    double StartTime = FPlatformTime::Seconds();
    
    FAIAdaptationDecision Decision = AIAdaptationSystem->GenerateAdaptationDecision(
        TEXT("Player_001"), ESixStrategyType::EstablishNation);
    
    double Duration = FPlatformTime::Seconds() - StartTime;
    
    if (Duration > 0.5f) // 超過500ms
    {
        UE_LOG(LogTemp, Error, TEXT("AI決策過慢: %.3f秒"), Duration);
        
        // 檢查學習數據量
        FAdaptationLearningData Data = AIAdaptationSystem->GetLearningData(TEXT("Player_001"));
        UE_LOG(LogTemp, Warning, TEXT("學習數據樣本數: %d"), Data.EncounterCount);
    }
}
```

**解決方案**:
- 限制學習數據樣本數
- 使用簡化決策邏輯
- 緩存常見決策結果

---

## 📈 性能優化檢查清單

### 開發階段

- [ ] 使用Unreal Insights進行性能分析
- [ ] 驗證Tick函數執行時間
- [ ] 檢查內存分配模式
- [ ] 測試網絡同步效率
- [ ] 優化數據結構選擇
- [ ] 實現對象池化
- [ ] 配置適當的更新頻率

### 測試階段

- [ ] 運行基準性能測試
- [ ] 運行壓力測試
- [ ] 運行長時間穩定性測試
- [ ] 驗證內存使用穩定
- [ ] 測試網絡延遲
- [ ] 驗證幀率穩定性

### 發布階段

- [ ] 啟用編譯器優化
- [ ] 配置發布參數
- [ ] 移除調試代碼
- [ ] 優化資源加載
- [ ] 配置CDN緩存

---

## 🎓 學習資源

### 官方文檔
- [Unreal Engine 性能指南](https://docs.unrealengine.com/performance-guidelines)
- [UE5 多線程編程](https://docs.unrealengine.com/multithreading)
- [網絡優化最佳實踐](https://docs.unrealengine.com/network-optimization)

### 社區資源
- Unreal Slackers Discord
- Unreal Engine Forums
- GitHub 性能優化示例

---

## 📞 技術支持

- **性能問題報告**: 使用 `MingSageCommandIntegrationTest` 生成報告
- **文檔反饋**: `f:\MingGoRTS\docs\`
- **最新更新**: 查看專案Git倉庫

---

**版權所有**: MingGoRTS Project Team  
**許可證**: MIT License  
**最後更新**: 2026-03-24
