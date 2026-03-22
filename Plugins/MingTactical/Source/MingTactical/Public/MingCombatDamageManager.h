#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingCombatDamageManager.generated.h"

// 傷害類型
UENUM(BlueprintType)
enum class EMingDamageType : uint8
{
    Kinetic,           // 動能傷害 (槍彈、砲彈)
    Explosive,         // 爆炸傷害 (手榴彈、炸彈)
    Fire,              // 火焰傷害
    Chemical,          // 化學傷害
    Psychological,      // 心理傷害
    Environmental      // 環境傷害
};

// 裝備類型
UENUM(BlueprintType)
enum class EMingEquipmentType : uint8
{
    Rifle,             // 步槍
    MachineGun,         // 機槍
    Artillery,         // 砲兵
    Mortar,            // 迫擊砲
    Grenade,           // 手榴彈
    Bayonet,           // 刺刀
    Tank,              // 坦克
    Aircraft,          // 飛機
    NavalGun,          // 艦砲
    Melee              // 近戰武器
};

// 補給類型
UENUM(BlueprintType)
enum class EMingSupplyType : uint8
{
    Ammunition,        // 彈藥
    Fuel,              // 燃料
    Food,              // 糧食
    Medical,           // 醫療用品
    Equipment,         // 裝備
    Reinforcements     // 增援部隊
};

// 傷害計算參數
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDamageCalculation
{
    GENERATED_BODY()

    // 基礎傷害
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage;

    // 傷害類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDamageType DamageType;

    // 裝備類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipmentType EquipmentType;

    // 攻擊距離
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackDistance;

    // 命中部位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HitLocation;

    // 護甲值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmorValue;

    // 護甲穿透
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmorPenetration;

    // 暴擊概率
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalChance;

    // 暴擊倍數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalMultiplier;

    // 傷害衰減係數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageFalloff;

    // 彈藥類型修正
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AmmoTypeModifier;

    // 地形影響因子
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainModifier;

    // 天氣影響因子
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WeatherModifier;

    // 士氣影響因子
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoraleModifier;

    FMingDamageCalculation()
    {
        BaseDamage = 0.0f;
        DamageType = EMingDamageType::Kinetic;
        EquipmentType = EMingEquipmentType::Rifle;
        AttackDistance = 0.0f;
        HitLocation = TEXT("");
        ArmorValue = 0.0f;
        ArmorPenetration = 0.0f;
        CriticalChance = 0.1f;
        CriticalMultiplier = 2.0f;
        DamageFalloff = 1.0f;
        AmmoTypeModifier = 1.0f;
        TerrainModifier = 1.0f;
        WeatherModifier = 1.0f;
        MoraleModifier = 1.0f;
    }
};

// 補給數據
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingSupplyData
{
    GENERATED_BODY()

    // 補給類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSupplyType SupplyType;

    // 當前數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentAmount;

    // 最大容量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxCapacity;

    // 消耗速率
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConsumptionRate;

    // 補給優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SupplyPriority;

    // 補給來源
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SupplySourceID;

    // 最後補給時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastSupplyTime;

    // 補給狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SupplyStatus;

    // 緊急補給閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmergencyThreshold;

    FMingSupplyData()
    {
        SupplyType = EMingSupplyType::Ammunition;
        CurrentAmount = 100.0f;
        MaxCapacity = 100.0f;
        ConsumptionRate = 1.0f;
        SupplyPriority = 1;
        SupplySourceID = -1;
        LastSupplyTime = FDateTime::Now();
        SupplyStatus = TEXT("normal");
        EmergencyThreshold = 20.0f;
    }
};

// 後勤車隊數據
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingLogisticsConvoy
{
    GENERATED_BODY()

    // 車隊ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConvoyID;

    // 車隊名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyName;

    // 車隊類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyType;

    // 當前位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentPosition;

    // 目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPosition;

    // 補給物資
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingSupplyData> Cargo;

    // 移動速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    // 護衛等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProtectionLevel;

    // 出發時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DepartureTime;

    // 預計到達時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EstimatedArrivalTime;

    // 車隊狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyStatus;

    // 護衛單位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EscortUnits;

    // 風險評估
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RiskAssessment;

    FMingLogisticsConvoy()
    {
        ConvoyID = -1;
        ConvoyName = TEXT("");
        ConvoyType = TEXT("");
        CurrentPosition = FVector::ZeroVector;
        TargetPosition = FVector::ZeroVector;
        Cargo.Empty();
        MovementSpeed = 20.0f;
        ProtectionLevel = 1.0f;
        DepartureTime = FDateTime::Now();
        EstimatedArrivalTime = FDateTime::Now();
        ConvoyStatus = TEXT("preparing");
        EscortUnits.Empty();
        RiskAssessment = 0.5f;
    }
};

// 傷害結果
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDamageResult
{
    GENERATED_BODY()

    // 攻擊者ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackerID;

    // 防禦者ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefenderID;

    // 實際傷害
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ActualDamage;

    // 基礎傷害
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage;

    // 傷害類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDamageType DamageType;

    // 是否暴擊
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCriticalHit;

    // 是否穿透護甲
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bArmorPenetrated;

    // 傷害部位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DamageLocation;

    // 傷害計算詳情
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DamageCalculationDetails;

    // 補給消耗
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingSupplyType, float> SupplyConsumption;

    // 戰鬥時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CombatTime;

    // 戰鬥位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CombatLocation;

    FMingDamageResult()
    {
        AttackerID = -1;
        DefenderID = -1;
        ActualDamage = 0.0f;
        BaseDamage = 0.0f;
        DamageType = EMingDamageType::Kinetic;
        bIsCriticalHit = false;
        bArmorPenetrated = false;
        DamageLocation = TEXT("");
        DamageCalculationDetails = TEXT("");
        SupplyConsumption.Empty();
        CombatTime = FDateTime::Now();
        CombatLocation = FVector::ZeroVector;
    }
};

// 戰鬥傷害事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageCalculated, const FMingDamageResult&, DamageResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSupplyConsumed, int32, UnitID, const TMap<EMingSupplyType, float>&, ConsumedSupplies);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSupplyDelivered, const FMingLogisticsConvoy&, Convoy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSupplyStatusChanged, int32, UnitID, EMingSupplyType, SupplyType, float, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConvoyStatusChanged, int32, ConvoyID, FString, NewStatus);

/**
 * 戰鬥傷害管理器
 * 負責計算真實的戰鬥傷害和管理後勤補給系統
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingCombatDamageManager : public UObject
{
    GENERATED_BODY()

public:
    UMingCombatDamageManager();

    // 初始化傷害系統
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool InitializeDamageSystem();

    // 計算戰鬥傷害
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    FMingDamageResult CalculateCombatDamage(const FMingDamageCalculation& DamageCalc);

    // 註冊單位補給
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool RegisterUnitSupply(int32 UnitID, const FMingSupplyData& SupplyData);

    // 更新單位補給
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool UpdateUnitSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount);

    // 獲取單位補給
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    FMingSupplyData GetUnitSupply(int32 UnitID, EMingSupplyType SupplyType) const;

    // 消耗補給
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool ConsumeSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount);

    // 創建後勤車隊
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    int32 CreateLogisticsConvoy(const TArray<FMingSupplyData>& Cargo, const FVector& TargetLocation);

    // 派遣後勤車隊
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool DispatchConvoy(int32 ConvoyID);

    // 獲取車隊信息
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    FMingLogisticsConvoy GetConvoyInfo(int32 ConvoyID) const;

    // 更新車隊位置
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool UpdateConvoyPosition(int32 ConvoyID, const FVector& NewPosition);

    // 獲取補給需求
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    TArray<FMingSupplyData> GetSupplyDemands() const;

    // 計算補給優先級
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    int32 CalculateSupplyPriority(int32 UnitID, EMingSupplyType SupplyType);

    // 獲取傷害統計
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    TMap<EMingDamageType, int32> GetDamageStatistics() const;

    // 保存傷害數據
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool SaveDamageData();

    // 載入傷害數據
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool LoadDamageData();

    // 清除所有傷害數據
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    void ClearAllDamageData();

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnDamageCalculated OnDamageCalculated;

    UPROPERTY(BlueprintAssignable)
    FOnSupplyConsumed OnSupplyConsumed;

    UPROPERTY(BlueprintAssignable)
    FOnSupplyDelivered OnSupplyDelivered;

    UPROPERTY(BlueprintAssignable)
    FOnSupplyStatusChanged OnSupplyStatusChanged;

    UPROPERTY(BlueprintAssignable)
    FOnConvoyStatusChanged OnConvoyStatusChanged;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // 單位補給數據庫 - 注意：嵌套 TMap 不支持 UPROPERTY
    TMap<int32, TMap<EMingSupplyType, FMingSupplyData>> UnitSupplyDatabase;

    // 後勤車隊數據庫
    UPROPERTY()
    TMap<int32, FMingLogisticsConvoy> ConvoyDatabase;

    // 傷害歷史記錄
    UPROPERTY()
    TArray<FMingDamageResult> DamageHistory;

    // 裝備傷害表
    UPROPERTY()
    TMap<EMingEquipmentType, float> EquipmentDamageTable;

    // 彈藥傷害修正表
    UPROPERTY()
    TMap<FString, float> AmmoDamageModifiers;

    // 地形傷害修正表
    UPROPERTY()
    TMap<FString, float> TerrainDamageModifiers;

    // 天氣傷害修正表
    UPROPERTY()
    TMap<FString, float> WeatherDamageModifiers;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設傷害表
    void LoadDefaultDamageTables();

    // 計算基礎傷害
    float CalculateBaseDamage(const FMingDamageCalculation& DamageCalc) const;

    // 計算護甲減傷
    float CalculateArmorReduction(const FMingDamageCalculation& DamageCalc) const;

    // 計算距離衰減
    float CalculateDistanceFalloff(const FMingDamageCalculation& DamageCalc) const;

    // 計算暴擊傷害
    float CalculateCriticalDamage(float BaseDamage, const FMingDamageCalculation& DamageCalc) const;

    // 計算地形影響
    float CalculateTerrainEffect(const FMingDamageCalculation& DamageCalc) const;

    // 計算天氣影響
    float CalculateWeatherEffect(const FMingDamageCalculation& DamageCalc) const;

    // 計算士氣影響
    float CalculateMoraleEffect(const FMingDamageCalculation& DamageCalc) const;

    // 計算補給消耗
    TMap<EMingSupplyType, float> CalculateSupplyConsumption(const FMingDamageCalculation& DamageCalc) const;

    // 更新單位性能
    void UpdateUnitPerformance(int32 UnitID, const TMap<EMingSupplyType, FMingSupplyData>& Supplies);

    // 處理後勤車隊移動
    void ProcessConvoyMovement(float DeltaTime);

    // 檢查補給需求
    void CheckSupplyDemands();

    // 計算車隊風險
    float CalculateConvoyRisk(const FMingLogisticsConvoy& Convoy) const;

    // 生成補給路線
    TArray<FVector> GenerateSupplyRoute(const FVector& Start, const FVector& End) const;

    // 驗證傷害計算
    bool ValidateDamageCalculation(const FMingDamageCalculation& DamageCalc) const;

    // 驗證補給數據
    bool ValidateSupplyData(const FMingSupplyData& SupplyData) const;

    // 計算傷害部位
    FString CalculateHitLocation(const FVector& ImpactPoint) const;

    // 處理補給緊急情況
    void HandleSupplyEmergency(int32 UnitID, EMingSupplyType SupplyType);

    // 更新車隊狀態
    void UpdateConvoyStatus(int32 ConvoyID, const FString& NewStatus);

    // 計算補給效率
    float CalculateSupplyEfficiency(int32 UnitID) const;
};
