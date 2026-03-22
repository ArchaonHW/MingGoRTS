#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingCombatDamageManager.generated.h"

// ?�害類�?
UENUM(BlueprintType)
enum class EMingDamageType : uint8
{
    Kinetic,           // ?�能?�害 (槍�X�砲�?
    Explosive,         // ?�炸?�害 (?�榴彈、炸�?
    Fire,              // ?�焰?�害
    Chemical,          // ?�學?�害
    Psychological,      // 心�X�害
    Environmental      // ?��X�害
};

// 裝�?類�?
UENUM(BlueprintType)
enum class EMingEquipmentType : uint8
{
    Rifle,             // 步�?
    MachineGun,         // 機�?
    Artillery,         // ?�兵
    Mortar,            // 迫�X    Grenade,           // ?�榴�?    Bayonet,           // ?��?
    Tank,              // ?��?
    Aircraft,          // 飛�?
    NavalGun,          // ?�砲
    Melee              // 近戰武器
};

// 補給類�?
UENUM(BlueprintType)
enum class EMingSupplyType : uint8
{
    Ammunition,        // 彈藥
    Fuel,              // ?��?
    Food,              // 糧�?
    Medical,           // ?��X��?
    Equipment,         // 裝�?
    Reinforcements     // 增援?��?
};

// ?�害計�X�數
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDamageCalculation
{
    GENERATED_BODY()

    // ?��X�害
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage;

    // ?�害類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDamageType DamageType;

    // 裝�?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipmentType EquipmentType;

    // ?��?距離
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackDistance;

    // ?�中?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HitLocation;

    // 護甲X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmorValue;

    // 護甲穿�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmorPenetration;

    // ?��?概�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalChance;

    // ?��X�數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalMultiplier;

    // ?�害衰�?係數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageFalloff;

    // 彈藥類�?修正
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AmmoTypeModifier;

    // ?�形影響?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainModifier;

    // 天氣影響?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WeatherModifier;

    // 士氣影響?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoraleModifier;

    FMingDamageCalculation()
    {
        BaseDamage = 0.0f;
        DamageType = EMingDamageType::Kinetic;
        EquipmentType = EMingEquipmentType::Rifle;
        AttackDistance = 0.0f;
        HitLocation = TEXT(""};
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

// 補給?��?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingSupplyData
{
    GENERATED_BODY()

    // 補給類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSupplyType SupplyType;

    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentAmount;

    // ?�大容X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxCapacity;

    // 消耗速�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConsumptionRate;

    // 補給?��?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SupplyPriority;

    // 補給來�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SupplySourceID;

    // ?�後�?給�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastSupplyTime;

    // 補給?�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SupplyStatus;

    // 緊急�?給閾X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmergencyThreshold;

    FMingSupplyData()
    {
        SupplyType = EMingSupplyType::Ammunition;
        CurrentAmount = 100.0f;
        MaxCapacity = 100.0f;
        ConsumptionRate = 1.0f;
        SupplyPriority = 1;
        SupplySourceID = -1;
        LastSupplyTime = FDateTime::Now(};
        SupplyStatus = TEXT("normal"};
        EmergencyThreshold = 20.0f;
    }
};

// 後勤車�X��?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingLogisticsConvoy
{
    GENERATED_BODY()

    // 車�?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConvoyID;

    // 車�X�稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyName;

    // 車�?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyType;

    // ?��?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentPosition;

    // ?��?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPosition;

    // 補給?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingSupplyData> Cargo;

    // 移�X�度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    // 護�?等�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProtectionLevel;

    // ?�發?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DepartureTime;

    // ?��X��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EstimatedArrivalTime;

    // 車�X�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyStatus;

    // 護�X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EscortUnits;

    // 風險評估
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RiskAssessment;

    FMingLogisticsConvoy()
    {
        ConvoyID = -1;
        ConvoyName = TEXT(""};
        ConvoyType = TEXT(""};
        CurrentPosition = FVector::ZeroVector;
        TargetPosition = FVector::ZeroVector;
        Cargo.Empty(};
        MovementSpeed = 20.0f;
        ProtectionLevel = 1.0f;
        DepartureTime = FDateTime::Now(};
        EstimatedArrivalTime = FDateTime::Now(};
        ConvoyStatus = TEXT("preparing"};
        EscortUnits.Empty(};
        RiskAssessment = 0.5f;
    }
};

// ?�害結�?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDamageResult
{
    GENERATED_BODY()

    // ?��X�ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackerID;

    // ?�禦?�ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefenderID;

    // 實�X�害
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ActualDamage;

    // ?��X�害
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage;

    // ?�害類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDamageType DamageType;

    // ?�否?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCriticalHit;

    // ?�否穿透護X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bArmorPenetrated;

    // ?�害?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DamageLocation;

    // ?�害計�?詳�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DamageCalculationDetails;

    // 補給消�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingSupplyType, float> SupplyConsumption;

    // ?�鬥?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CombatTime;

    // ?�鬥位置
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
        DamageLocation = TEXT(""};
        DamageCalculationDetails = TEXT(""};
        SupplyConsumption.Empty(};
        CombatTime = FDateTime::Now(};
        CombatLocation = FVector::ZeroVector;
    }
};

// ?�鬥?�害事件委�?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageCalculated, const FMingDamageResult&, DamageResult};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSupplyConsumed, int32, UnitID, const TMap<EMingSupplyType, float>&, ConsumedSupplies};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSupplyDelivered, const FMingLogisticsConvoy&, Convoy};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSupplyStatusChanged, int32, UnitID, EMingSupplyType, SupplyType, float, NewAmount};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConvoyStatusChanged, int32, ConvoyID, FString, NewStatus};

/**
 * ?�鬥?�害管�X * 負責計�X�實?�戰鬥傷害�?管�?後勤補給系統
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingCombatDamageManager : public UObject
{
    GENERATED_BODY()

public:
    UMingCombatDamageManager(};

    // ?��X�傷害系�?    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool InitializeDamageSystem(};

    // 計�X�鬥?�害
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    FMingDamageResult CalculateCombatDamage(const FMingDamageCalculation& DamageCalc};

    // 註�X��?補給
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool RegisterUnitSupply(int32 UnitID, const FMingSupplyData& SupplyData};

    // ?�新?��?補給
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool UpdateUnitSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount};

    // ?��X��?補給
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    FMingSupplyData GetUnitSupply(int32 UnitID, EMingSupplyType SupplyType) const;

    // 消耗�?�?    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool ConsumeSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount};

    // ?�建後勤車�?
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    int32 CreateLogisticsConvoy(const TArray<FMingSupplyData>& Cargo, const FVector& TargetLocation};

    // 派遣後勤車�?
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool DispatchConvoy(int32 ConvoyID};

    // ?��?車�?信息
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    FMingLogisticsConvoy GetConvoyInfo(int32 ConvoyID) const;

    // ?�新車�?位置
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool UpdateConvoyPosition(int32 ConvoyID, const FVector& NewPosition};

    // ?��?補給?��?    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    TArray<FMingSupplyData> GetSupplyDemands() const;

    // 計�?補給?��?�?    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    int32 CalculateSupplyPriority(int32 UnitID, EMingSupplyType SupplyType};

    // ?��X�害統�?
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    TMap<EMingDamageType, int32> GetDamageStatistics() const;

    // 保�X�害?��?
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool SaveDamageData(};

    // 載入?�害?��?
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool LoadDamageData(};

    // 清除?�?�傷害數X    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    void ClearAllDamageData(};

    // 事件委�?
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

    // ?��?補給?��?�?- 注�?：�?�?TMap 不支XUPROPERTY
    TMap<int32, TMap<EMingSupplyType, FMingSupplyData>> UnitSupplyDatabase;

    // 後勤車�X��?�?    UPROPERTY()
    TMap<int32, FMingLogisticsConvoy> ConvoyDatabase;

    // ?�害歷史記�?
    UPROPERTY()
    TArray<FMingDamageResult> DamageHistory;

    // 裝�X�害�?    UPROPERTY()
    TMap<EMingEquipmentType, float> EquipmentDamageTable;

    // 彈藥?�害修正�?    UPROPERTY()
    TMap<FString, float> AmmoDamageModifiers;

    // ?�形?�害修正�?    UPROPERTY()
    TMap<FString, float> TerrainDamageModifiers;

    // 天氣?�害修正�?    UPROPERTY()
    TMap<FString, float> WeatherDamageModifiers;

    // ?�否已�?始�?
    UPROPERTY()
    bool bInitialized;

private:
    // 載入?�設?�害�?    void LoadDefaultDamageTables(};

    // 計�X��X�害
    float CalculateBaseDamage(const FMingDamageCalculation& DamageCalc) const;

    // 計�?護甲減傷
    float CalculateArmorReduction(const FMingDamageCalculation& DamageCalc) const;

    // 計�?距離衰�?
    float CalculateDistanceFalloff(const FMingDamageCalculation& DamageCalc) const;

    // 計�X��X�害
    float CalculateCriticalDamage(float BaseDamage, const FMingDamageCalculation& DamageCalc) const;

    // 計�X�形影響
    float CalculateTerrainEffect(const FMingDamageCalculation& DamageCalc) const;

    // 計�?天氣影響
    float CalculateWeatherEffect(const FMingDamageCalculation& DamageCalc) const;

    // 計�?士氣影響
    float CalculateMoraleEffect(const FMingDamageCalculation& DamageCalc) const;

    // 計�?補給消�?    TMap<EMingSupplyType, float> CalculateSupplyConsumption(const FMingDamageCalculation& DamageCalc) const;

    // ?�新?��X�能
    void UpdateUnitPerformance(int32 UnitID, const TMap<EMingSupplyType, FMingSupplyData>& Supplies};

    // ?��?後勤車�?移�?
    void ProcessConvoyMovement(float DeltaTime};

    // 檢查補給?��?    void CheckSupplyDemands(};

    // 計�?車�?風險
    float CalculateConvoyRisk(const FMingLogisticsConvoy& Convoy) const;

    // ?��?補給路�?
    TArray<FVector> GenerateSupplyRoute(const FVector& Start, const FVector& End) const;

    // 驗�X�害計�?
    bool ValidateDamageCalculation(const FMingDamageCalculation& DamageCalc) const;

    // 驗�?補給?��?
    bool ValidateSupplyData(const FMingSupplyData& SupplyData) const;

    // 計�X�害?��?
    FString CalculateHitLocation(const FVector& ImpactPoint) const;

    // ?��?補給緊急�?�?    void HandleSupplyEmergency(int32 UnitID, EMingSupplyType SupplyType};

    // ?�新車�X�X    void UpdateConvoyStatus(int32 ConvoyID, const FString& NewStatus};

    // 計�?補給?��?
    float CalculateSupplyEfficiency(int32 UnitID) const;
};

