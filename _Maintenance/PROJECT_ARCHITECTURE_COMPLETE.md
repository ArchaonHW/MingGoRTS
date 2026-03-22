# MingGoRTS 項目架構完善計劃

## 基於現有技術文檔的完整開發事項

### 1. 核心系統架構

#### 1.1 單位管理系統 (基於 PROJECT_PLAN.md T1-T5)
- **RTSUnit 基類** (T1 - 8小時)
  ```cpp
  class MINGGORTS_API ARTSUnit : public AActor
  {
      UPROPERTY(EditAnywhere, BlueprintReadWrite)
      FString UnitID;
      
      UPROPERTY(EditAnywhere, BlueprintReadWrite)
      float MaxHealth;
      
      UPROPERTY(EditAnywhere, BlueprintReadWrite)
      float CurrentHealth;
  };
  ```
  
- **RTSPlayerController** (T2 - 12小時)
  ```cpp
  class MINGGORTS_API ARTSPlayerController : public APlayerController
  {
      UFUNCTION(BlueprintCallable)
      void SelectUnit(ARTSUnit* Unit);
      
      UFUNCTION(BlueprintCallable)
      void MoveSelectedUnits(const FVector& TargetLocation);
  };
  ```

#### 1.2 事件系統集成 (基於 api_spec.md)
- **單位事件**
  ```cpp
  class MINGCORE_API MingUnitSelectedEvent : public MingEvent
  {
  public:
      UPROPERTY(EditAnywhere, BlueprintReadWrite)
      FString UnitID;
      
      UPROPERTY(EditAnywhere, BlueprintReadWrite)
      FVector SelectionPosition;
  };
  ```
  
- **戰鬥事件**
  ```cpp
  class MINGCORE_API MingUnitDamagedEvent : public MingEvent
  {
  public:
      UPROPERTY(EditAnywhere, BlueprintReadWrite)
      FString TargetUnitID;
      
      UPROPERTY(EditAnywhere, BlueprintReadWrite)
      float DamageAmount;
  };
  ```

### 2. 資源管理系統 (基於 PROJECT_PLAN.md T6-T9)

#### 2.1 資源類型定義
```cpp
UENUM(BlueprintType)
enum class EResourceType : uint8
{
    Gold,
    Food,
    Wood,
    Stone
};

USTRUCT(BlueprintType)
struct FResourceAmount
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EResourceType ResourceType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Amount;
};
```

#### 2.2 資源管理器
```cpp
class MINGGORTS_API UResourceManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable)
    bool HasEnoughResources(const TArray<FResourceAmount>& RequiredResources);
    
    UFUNCTION(BlueprintCallable)
    void ConsumeResources(const TArray<FResourceAmount>& Resources);
    
    UFUNCTION(BlueprintCallable)
    void AddResources(const TArray<FResourceAmount>& Resources);
};
```

### 3. 建築系統 (基於 PROJECT_PLAN.md T7-T9)

#### 3.1 建築基類
```cpp
class MINGGORTS_API ARTSBuilding : public AActor
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ARTSUnit> ProductionUnit;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FResourceAmount> BuildCost;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BuildTime;
};
```

#### 3.2 建築管理器
```cpp
class MINGGORTS_API UBuildingManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable)
    bool CanBuildBuilding(TSubclassOf<ARTSBuilding> BuildingClass);
    
    UFUNCTION(BlueprintCallable)
    void StartBuilding(TSubclassOf<ARTSBuilding> BuildingClass, const FVector& Location);
};
```

### 4. AI系統 (基於 PROJECT_PLAN.md T10-T12)

#### 4.1 AI控制器
```cpp
class MINGGORTS_API ARTSAIController : public AAIController
{
protected:
    virtual void OnPossess(APawn* InPawn) override;
    
    UFUNCTION(BlueprintCallable)
    void ExecuteAttack(AActor* Target);
    
    UFUNCTION(BlueprintCallable)
    void MoveToLocation(const FVector& TargetLocation);
};
```

### 5. UI系統 (基於 PROJECT_PLAN.md T12)

#### 5.1 RTS HUD
```cpp
class MINGGORTS_API ARTSHUD : public AHUD
{
public:
    UFUNCTION(BlueprintCallable)
    void UpdateResourceDisplay(const TArray<FResourceAmount>& Resources);
    
    UFUNCTION(BlueprintCallable)
    void ShowUnitSelection(int32 SelectedCount);
};
```

## 實現優先級

### 高優先級 (Sprint 1)
1. **MingCore事件系統測試** - 驗證事件發布/訂閱
2. **RTSUnit基類** - 基礎單位實現
3. **RTSPlayerController** - 玩家控制器
4. **基礎攝影機** - RTS風格攝影機

### 中優先級 (Sprint 2)
1. **資源管理系統** - 黃金、食物、木材、石頭
2. **建築系統** - 基礎建築功能
3. **戰鬥系統** - 傷害計算和生命值
4. **UI資源顯示** - HUD資源資訊

### 低優先級 (Sprint 3+)
1. **AI系統** - 基礎AI行為
2. **進階建築** - 升級系統
3. **多人模式** - 網絡功能
4. **音效和音樂** - AIVA集成

## 技術要求

### 代碼標準
- 使用MingCore事件總線進行模組間通信
- 所有C++類使用MINGGORTS_API宏
- Blueprint可調用函數使用UFUNCTION宏
- 遵循UE5編碼規範

### 性能要求
- 單位數量支持：1000+ 單位
- 地圖大小：1024x1024
- 幀率目標：60 FPS
- 記憶使用：< 4GB

### 測試要求
- 每個功能模組需要單元測試
- 集成測試驗證模組間交互
- 性能測試確保目標幀率
- 用戶接受度測試
