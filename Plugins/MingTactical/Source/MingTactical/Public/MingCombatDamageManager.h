#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingCombatDamageManager.generated.h"

// ?·å®³é¡å?
UENUM(BlueprintType)
enum class EMingDamageType : uint8
{
    Kinetic,           // ?•èƒ½?·å®³ (æ§å??ç ²å½?
    Explosive,         // ?†ç‚¸?·å®³ (?‹æ¦´å½ˆã€ç‚¸å½?
    Fire,              // ?«ç„°?·å®³
    Chemical,          // ?–å­¸?·å®³
    Psychological,      // å¿ƒç??·å®³
    Environmental      // ?°å??·å®³
};

// è£å?é¡å?
UENUM(BlueprintType)
enum class EMingEquipmentType : uint8
{
    Rifle,             // æ­¥æ?
    MachineGun,         // æ©Ÿæ?
    Artillery,         // ?²å…µ
    Mortar,            // è¿«æ???    Grenade,           // ?‹æ¦´å½?    Bayonet,           // ?ºå?
    Tank,              // ?¦å?
    Aircraft,          // é£›æ?
    NavalGun,          // ?¦ç ²
    Melee              // è¿‘æˆ°æ­¦å™¨
};

// è£œçµ¦é¡å?
UENUM(BlueprintType)
enum class EMingSupplyType : uint8
{
    Ammunition,        // å½ˆè—¥
    Fuel,              // ?ƒæ?
    Food,              // ç³§é?
    Medical,           // ?«ç??¨å?
    Equipment,         // è£å?
    Reinforcements     // å¢æ´?¨é?
};

// ?·å®³è¨ˆç??ƒæ•¸
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDamageCalculation
{
    GENERATED_BODY()

    // ?ºç??·å®³
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage;

    // ?·å®³é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDamageType DamageType;

    // è£å?é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipmentType EquipmentType;

    // ?»æ?è·é›¢
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackDistance;

    // ?½ä¸­?¨ä?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HitLocation;

    // è­·ç”²??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmorValue;

    // è­·ç”²ç©¿é€?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmorPenetration;

    // ?´æ?æ¦‚ç?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalChance;

    // ?´æ??æ•¸
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalMultiplier;

    // ?·å®³è¡°æ?ä¿‚æ•¸
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageFalloff;

    // å½ˆè—¥é¡å?ä¿®æ­£
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AmmoTypeModifier;

    // ?°å½¢å½±éŸ¿? å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainModifier;

    // å¤©æ°£å½±éŸ¿? å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WeatherModifier;

    // å£«æ°£å½±éŸ¿? å?
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

// è£œçµ¦?¸æ?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingSupplyData
{
    GENERATED_BODY()

    // è£œçµ¦é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSupplyType SupplyType;

    // ?¶å??¸é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentAmount;

    // ?€å¤§å®¹??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxCapacity;

    // æ¶ˆè€—é€Ÿç?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConsumptionRate;

    // è£œçµ¦?ªå?ç´?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SupplyPriority;

    // è£œçµ¦ä¾†æ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SupplySourceID;

    // ?€å¾Œè?çµ¦æ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastSupplyTime;

    // è£œçµ¦?€??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SupplyStatus;

    // ç·Šæ€¥è?çµ¦é–¾??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// å¾Œå‹¤è»Šé??¸æ?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingLogisticsConvoy
{
    GENERATED_BODY()

    // è»Šé?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConvoyID;

    // è»Šé??ç¨±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyName;

    // è»Šé?é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyType;

    // ?¶å?ä½ç½®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentPosition;

    // ?®æ?ä½ç½®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPosition;

    // è£œçµ¦?©è?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingSupplyData> Cargo;

    // ç§»å??Ÿåº¦
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    // è­·è?ç­‰ç?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProtectionLevel;

    // ?ºç™¼?‚é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DepartureTime;

    // ?è??°é??‚é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EstimatedArrivalTime;

    // è»Šé??€??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyStatus;

    // è­·è??®ä?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EscortUnits;

    // é¢¨éšªè©•ä¼°
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

// ?·å®³çµæ?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDamageResult
{
    GENERATED_BODY()

    // ?»æ??…ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackerID;

    // ?²ç¦¦?…ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefenderID;

    // å¯¦é??·å®³
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ActualDamage;

    // ?ºç??·å®³
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage;

    // ?·å®³é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDamageType DamageType;

    // ?¯å¦?´æ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCriticalHit;

    // ?¯å¦ç©¿é€è­·??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bArmorPenetrated;

    // ?·å®³?¨ä?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DamageLocation;

    // ?·å®³è¨ˆç?è©³æ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DamageCalculationDetails;

    // è£œçµ¦æ¶ˆè€?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingSupplyType, float> SupplyConsumption;

    // ?°é¬¥?‚é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CombatTime;

    // ?°é¬¥ä½ç½®
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

// ?°é¬¥?·å®³äº‹ä»¶å§”è?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageCalculated, const FMingDamageResult&, DamageResult};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSupplyConsumed, int32, UnitID, const TMap<EMingSupplyType, float>&, ConsumedSupplies};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSupplyDelivered, const FMingLogisticsConvoy&, Convoy};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSupplyStatusChanged, int32, UnitID, EMingSupplyType, SupplyType, float, NewAmount};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConvoyStatusChanged, int32, ConvoyID, FString, NewStatus};

/**
 * ?°é¬¥?·å®³ç®¡ç??? * è² è²¬è¨ˆç??Ÿå¯¦?„æˆ°é¬¥å‚·å®³å?ç®¡ç?å¾Œå‹¤è£œçµ¦ç³»çµ±
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingCombatDamageManager : public UObject
{
    GENERATED_BODY()

public:
    UMingCombatDamageManager(};

    // ?å??–å‚·å®³ç³»çµ?    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool InitializeDamageSystem(};

    // è¨ˆç??°é¬¥?·å®³
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    FMingDamageResult CalculateCombatDamage(const FMingDamageCalculation& DamageCalc};

    // è¨»å??®ä?è£œçµ¦
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool RegisterUnitSupply(int32 UnitID, const FMingSupplyData& SupplyData};

    // ?´æ–°?®ä?è£œçµ¦
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool UpdateUnitSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount};

    // ?²å??®ä?è£œçµ¦
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    FMingSupplyData GetUnitSupply(int32 UnitID, EMingSupplyType SupplyType) const;

    // æ¶ˆè€—è?çµ?    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool ConsumeSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount};

    // ?µå»ºå¾Œå‹¤è»Šé?
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    int32 CreateLogisticsConvoy(const TArray<FMingSupplyData>& Cargo, const FVector& TargetLocation};

    // æ´¾é£å¾Œå‹¤è»Šé?
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool DispatchConvoy(int32 ConvoyID};

    // ?²å?è»Šé?ä¿¡æ¯
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    FMingLogisticsConvoy GetConvoyInfo(int32 ConvoyID) const;

    // ?´æ–°è»Šé?ä½ç½®
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool UpdateConvoyPosition(int32 ConvoyID, const FVector& NewPosition};

    // ?²å?è£œçµ¦?€æ±?    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    TArray<FMingSupplyData> GetSupplyDemands() const;

    // è¨ˆç?è£œçµ¦?ªå?ç´?    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    int32 CalculateSupplyPriority(int32 UnitID, EMingSupplyType SupplyType};

    // ?²å??·å®³çµ±è?
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    TMap<EMingDamageType, int32> GetDamageStatistics() const;

    // ä¿å??·å®³?¸æ?
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool SaveDamageData(};

    // è¼‰å…¥?·å®³?¸æ?
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool LoadDamageData(};

    // æ¸…é™¤?€?‰å‚·å®³æ•¸??    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    void ClearAllDamageData(};

    // äº‹ä»¶å§”è?
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

    // ?®ä?è£œçµ¦?¸æ?åº?- æ³¨æ?ï¼šå?å¥?TMap ä¸æ”¯??UPROPERTY
    TMap<int32, TMap<EMingSupplyType, FMingSupplyData>> UnitSupplyDatabase;

    // å¾Œå‹¤è»Šé??¸æ?åº?    UPROPERTY()
    TMap<int32, FMingLogisticsConvoy> ConvoyDatabase;

    // ?·å®³æ­·å²è¨˜é?
    UPROPERTY()
    TArray<FMingDamageResult> DamageHistory;

    // è£å??·å®³è¡?    UPROPERTY()
    TMap<EMingEquipmentType, float> EquipmentDamageTable;

    // å½ˆè—¥?·å®³ä¿®æ­£è¡?    UPROPERTY()
    TMap<FString, float> AmmoDamageModifiers;

    // ?°å½¢?·å®³ä¿®æ­£è¡?    UPROPERTY()
    TMap<FString, float> TerrainDamageModifiers;

    // å¤©æ°£?·å®³ä¿®æ­£è¡?    UPROPERTY()
    TMap<FString, float> WeatherDamageModifiers;

    // ?¯å¦å·²å?å§‹å?
    UPROPERTY()
    bool bInitialized;

private:
    // è¼‰å…¥?è¨­?·å®³è¡?    void LoadDefaultDamageTables(};

    // è¨ˆç??ºç??·å®³
    float CalculateBaseDamage(const FMingDamageCalculation& DamageCalc) const;

    // è¨ˆç?è­·ç”²æ¸›å‚·
    float CalculateArmorReduction(const FMingDamageCalculation& DamageCalc) const;

    // è¨ˆç?è·é›¢è¡°æ?
    float CalculateDistanceFalloff(const FMingDamageCalculation& DamageCalc) const;

    // è¨ˆç??´æ??·å®³
    float CalculateCriticalDamage(float BaseDamage, const FMingDamageCalculation& DamageCalc) const;

    // è¨ˆç??°å½¢å½±éŸ¿
    float CalculateTerrainEffect(const FMingDamageCalculation& DamageCalc) const;

    // è¨ˆç?å¤©æ°£å½±éŸ¿
    float CalculateWeatherEffect(const FMingDamageCalculation& DamageCalc) const;

    // è¨ˆç?å£«æ°£å½±éŸ¿
    float CalculateMoraleEffect(const FMingDamageCalculation& DamageCalc) const;

    // è¨ˆç?è£œçµ¦æ¶ˆè€?    TMap<EMingSupplyType, float> CalculateSupplyConsumption(const FMingDamageCalculation& DamageCalc) const;

    // ?´æ–°?®ä??§èƒ½
    void UpdateUnitPerformance(int32 UnitID, const TMap<EMingSupplyType, FMingSupplyData>& Supplies};

    // ?•ç?å¾Œå‹¤è»Šé?ç§»å?
    void ProcessConvoyMovement(float DeltaTime};

    // æª¢æŸ¥è£œçµ¦?€æ±?    void CheckSupplyDemands(};

    // è¨ˆç?è»Šé?é¢¨éšª
    float CalculateConvoyRisk(const FMingLogisticsConvoy& Convoy) const;

    // ?Ÿæ?è£œçµ¦è·¯ç?
    TArray<FVector> GenerateSupplyRoute(const FVector& Start, const FVector& End) const;

    // é©—è??·å®³è¨ˆç?
    bool ValidateDamageCalculation(const FMingDamageCalculation& DamageCalc) const;

    // é©—è?è£œçµ¦?¸æ?
    bool ValidateSupplyData(const FMingSupplyData& SupplyData) const;

    // è¨ˆç??·å®³?¨ä?
    FString CalculateHitLocation(const FVector& ImpactPoint) const;

    // ?•ç?è£œçµ¦ç·Šæ€¥æ?æ³?    void HandleSupplyEmergency(int32 UnitID, EMingSupplyType SupplyType};

    // ?´æ–°è»Šé??€??    void UpdateConvoyStatus(int32 ConvoyID, const FString& NewStatus};

    // è¨ˆç?è£œçµ¦?ˆç?
    float CalculateSupplyEfficiency(int32 UnitID) const;
};

