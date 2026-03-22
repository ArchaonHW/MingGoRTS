# 聖者角色控制器使用指南

## 概述

`AMingSageRoleController` 是 MingGoRTS 中的核心角色管理控制器，負責管理玩家的角色選擇、發展、轉換和能力系統。它與聖者指揮學系統深度集成，提供完整的角色扮演體驗。

## 系統架構

### 核心組件

1. **AMingSageRoleController** - 主控制器類
2. **UMingSageRoles** - 角色定義和評估系統
3. **UMingSageCommandSystem** - 聖者指揮學系統
4. **FSageRoleDevelopment** - 角色發展數據結構
5. **FSageRoleAbility** - 角色能力數據結構

### 角色類型

- **至聖者 (TrueSage)** - 具有完美道德權威的聖者
- **偽聖者 (FalseSage)** - 善於偽裝和欺騙的假聖者
- **魔王 (DemonKing)** - 具有絕對力量但缺乏道德的統治者

## 角色狀態系統

### 角色狀態枚舉

```cpp
enum class ESageRoleState : uint8
{
    RoleSelection = 0,      // 角色選擇階段
    RoleDevelopment = 1,     // 角色發展階段
    RoleTransformation = 2,  // 角色轉換階段
    RoleMastery = 3,        // 角色精通階段
    RoleCorruption = 4       // 角色墮落階段
};
```

### 狀態轉換規則

1. **RoleSelection** → **RoleDevelopment** - 選擇角色後
2. **RoleDevelopment** → **RoleTransformation** - 開始角色轉換
3. **RoleTransformation** → **RoleDevelopment** - 轉換完成
4. **RoleDevelopment** → **RoleMastery** - 等級達到50且合規分數≥80
5. **任何狀態** → **RoleCorruption** - 合規分數<30

## 角色發展系統

### 經驗值系統

```cpp
// 獲得經驗值
void AddExperience(float Amount);

// 檢查升級
bool LevelUp();

// 經驗值計算公式
int32 RequiredExperience = CurrentLevel * 100;
```

### 合規分數系統

```cpp
// 更新合規分數
void UpdateRoleCompliance(float ComplianceChange);

// 合規分數影響
// - ≥80: 角色精通狀態
// - 30-79: 正常發展狀態  
// - <30: 墮落狀態
```

### 等級系統

- **等級範圍**: 1-100
- **升級獎勵**: 解鎖新能力
- **精通等級**: 50級達到角色精通

## 角色轉換系統

### 轉換條件

```cpp
bool CanTransformToRole(ESageRoleType TargetRole);
bool StartRoleTransformation(ESageRoleType TargetRole);
```

### 轉換規則

1. **至聖者** → **偽聖者**: 合規分數下降到60-79
2. **至聖者** → **魔王**: 合規分數下降到<30
3. **偽聖者** → **至聖者**: 提升道德權威到完美
4. **偽聖者** → **魔王**: 進一步墮落
5. **魔王** → **偽聖者**: 部分恢復道德
6. **魔王** → **至聖者**: 完全贖罪

### 轉換效果

每種轉換都有特定的效果：

- **至聖者轉換**: 恢復100%合規分數
- **偽聖者轉換**: 設置60%合規分數，增加偽裝能力
- **魔王轉換**: 設置10%合規分數，大幅增加力量

## 能力系統

### 能力類型

每個角色都有獨特的能力組合：

#### 至聖者能力
- **moral_guidance** (道德指引)
- **divine_protection** (神聖保護)
- **wisdom_insight** (智慧洞察)

#### 偽聖者能力
- **deception** (欺騙)
- **manipulation** (操控)
- **infiltration** (滲透)

#### 魔王能力
- **dominance** (支配)
- **fear_aura** (恐懼光環)
- **destruction** (破壞)

### 能力管理

```cpp
// 解鎖能力
bool UnlockAbility(const FString& AbilityID);

// 激活能力
bool ActivateAbility(const FString& AbilityID);

// 停用能力
void DeactivateAbility(const FString& AbilityID);

// 檢查能力狀態
bool IsAbilityUnlocked(const FString& AbilityID) const;
bool IsAbilityActive(const FString& AbilityID) const;
```

### 能力冷卻系統

- **冷卻時間**: 每個能力獨立的冷卻時間
- **自動更新**: 系統自動管理冷卻時間
- **冷卻檢查**: 激活前檢查冷卻狀態

## 聖者指揮學集成

### 系統同步

```cpp
// 應用角色到聖者指揮學系統
void ApplyRoleToSageCommandSystem();

// 同步系統狀態
void SyncWithSageCommandSystem();
```

### 角色配置

每種角色對應不同的聖者指揮學配置：

#### 至聖者配置
- **主要權威**: 道德權威 (MoralAuthority)
- **屬性**: 正義 (Righteous)
- **元素親和**: 土 (Earth)

#### 偽聖者配置
- **主要權威**: 魅力權威 (CharismaticAuthority)
- **屬性**: 邪惡 (Evil)
- **元素親和**: 水 (Water)

#### 魔王配置
- **主要權威**: 情境權威 (SituationalAuthority)
- **屬性**: 邪惡 (Evil)
- **元素親和**: 火 (Fire)

## 事件系統

### 主要事件

```cpp
// 角色選擇事件
UPROPERTY(BlueprintAssignable)
FOnRoleSelected OnRoleSelected;

// 角色轉換事件
UPROPERTY(BlueprintAssignable)
FOnRoleTransformationStarted OnRoleTransformationStarted;
UPROPERTY(BlueprintAssignable)
FOnRoleTransformationCompleted OnRoleTransformationCompleted;

// 能力事件
UPROPERTY(BlueprintAssignable)
FOnAbilityUnlocked OnAbilityUnlocked;
UPROPERTY(BlueprintAssignable)
FOnAbilityActivated OnAbilityActivated;

// 合規分數變化事件
UPROPERTY(BlueprintAssignable)
FOnRoleComplianceChanged OnRoleComplianceChanged;
```

### 事件處理

```cpp
// 綁定事件
OnRoleSelected.AddDynamic(this, &AMyActor::OnRoleSelected);

// 處理事件
UFUNCTION()
void AMyActor::OnRoleSelected(ESageRoleType SelectedRole)
{
    // 處理角色選擇邏輯
}
```

## 使用示例

### 基礎設置

```cpp
// 在遊戲模式中創建角色控制器
AMingSageRoleController* RoleController = GetWorld()->SpawnActor<AMingSageRoleController>();

// 初始化系統
RoleController->BeginPlay();
```

### 角色選擇

```cpp
// 選擇角色
bool bSuccess = RoleController->SelectRole(ESageRoleType::TrueSage);

if (bSuccess)
{
    UE_LOG(LogTemp, Log, TEXT("成功選擇至聖者角色"));
}
```

### 角色發展

```cpp
// 獲得經驗值
RoleController->AddExperience(100.0f);

// 檢查角色狀態
FSageRoleDevelopment Development = RoleController->GetRoleDevelopment();
UE_LOG(LogTemp, Log, TEXT("當前等級: %d, 經驗值: %.1f"), 
    Development.Level, Development.ExperiencePoints);
```

### 角色轉換

```cpp
// 檢查是否可以轉換
if (RoleController->CanTransformToRole(ESageRoleType::DemonKing))
{
    // 開始轉換
    RoleController->StartRoleTransformation(ESageRoleType::DemonKing);
}
```

### 能力管理

```cpp
// 解鎖能力
if (RoleController->UnlockAbility(TEXT("moral_guidance")))
{
    UE_LOG(LogTemp, Log, TEXT("成功解鎖道德指引能力"));
}

// 激活能力
if (RoleController->ActivateAbility(TEXT("moral_guidance")))
{
    UE_LOG(LogTemp, Log, TEXT("道德指引能力已激活"));
}
```

## 角色評估系統

### 評估方法

```cpp
// 評估當前角色
FSageRoleEvaluation Evaluation = RoleController->EvaluateCurrentRole();

// 獲取角色建議
TArray<FString> Recommendations = RoleController->GetRoleRecommendations();

// 獲取角色優劣勢
TArray<FString> Strengths = RoleController->GetRoleStrengths();
TArray<FString> Weaknesses = RoleController->GetRoleWeaknesses();
```

### 評估指標

- **合規等級**: Perfect, Excellent, Good, Average, Poor, Corrupted
- **道德合規分數**: 0-100分
- **戰略合規分數**: 0-100分
- **權威合規分數**: 0-100分

## 配置參數

### 可配置參數

```cpp
// 經驗值倍數
UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sage Role Controller")
float ExperienceMultiplier = 1.0f;

// 合規分數衰減率
UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sage Role Controller")
float ComplianceDecayRate = 0.1f;

// 最大等級
UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sage Role Controller")
int32 MaxLevel = 100;

// 轉換持續時間
UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sage Role Controller")
float TransformationDuration = 10.0f;
```

### 參數調整建議

- **ExperienceMultiplier**: 0.5-2.0，影響經驗值獲得速度
- **ComplianceDecayRate**: 0.05-0.2，影響合規分數衰減速度
- **MaxLevel**: 50-200，影響最大等級上限
- **TransformationDuration**: 5-30秒，影響轉換動畫持續時間

## 性能優化

### 系統優化

1. **定時器管理**: 使用高效的定時器系統
2. **事件系統**: 優化事件觸發和處理
3. **數據緩存**: 緩存角色數據和能力信息
4. **異步處理**: 轉換和升級使用異步處理

### 內存管理

1. **智能清理**: 自動清理過期能力冷卻
2. **對象池**: 重用能力對象減少GC壓力
3. **數據壓縮**: 壓縮角色數據節省內存

## 調試和測試

### 調試工具

```cpp
// 獲取詳細角色信息
FSageRoleDevelopment Development = RoleController->GetRoleDevelopment();
FSageRoleEvaluation Evaluation = RoleController->EvaluateCurrentRole();

// 檢查能力狀態
TArray<FSageRoleAbility> Abilities = RoleController->GetUnlockedAbilities();
for (const FSageRoleAbility& Ability : Abilities)
{
    UE_LOG(LogTemp, Log, TEXT("能力: %s, 激活: %s"), 
        *Ability.AbilityName, Ability.bIsActive ? TEXT("是") : TEXT("否"));
}
```

### 測試建議

1. **角色轉換測試**: 測試所有可能的轉換路徑
2. **能力系統測試**: 測試能力解鎖、激活、冷卻
3. **經驗值測試**: 測試經驗值獲得和升級機制
4. **事件系統測試**: 測試所有事件的觸發和處理

## 最佳實踐

### 開發建議

1. **事件驅動**: 充分利用事件系統進行解耦
2. **狀態機**: 使用狀態機管理角色狀態轉換
3. **數據驗證**: 在關鍵操作前進行數據驗證
4. **錯誤處理**: 提供完善的錯誤處理機制

### 設計建議

1. **角色平衡**: 確保三種角色的平衡性
2. **轉換成本**: 設置合理的轉換成本和收益
3. **能力設計**: 設計有趣且平衡的能力系統
4. **用戶反饋**: 提供清晰的用戶反饋

## 未來擴展

### 計劃功能

1. **更多角色類型**: 支持更多角色類型
2. **角色自定義**: 允許玩家自定義角色特徵
3. **多人角色**: 支持多人遊戲中的角色系統
4. **AI角色**: 支持AI控制的角色

### 技術改進

1. **網絡同步**: 支持網絡遊戲中的角色同步
2. **持久化**: 支持角色數據的持久化存儲
3. **模組化**: 進一步模組化系統架構
4. **性能優化**: 持續的性能優化和改進

## 總結

聖者角色控制器為 MingGoRTS 提供了完整的角色管理系統，支持角色選擇、發展、轉換和能力管理。它與聖者指揮學系統深度集成，為玩家提供豐富的角色扮演體驗。

通過合理使用本指南，開發者可以充分利用角色控制器的功能，創造出有趣且平衡的遊戲體驗。
