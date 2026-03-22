#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingCombatDamageManager.generated.h"

// ??�`��??
UENUM(BlueprintType)
enum class EMingDamageType: uint8 {
    Kinetic,           // ??��??�` (�j?X?��??)
    Explosive,         // ??��??�` (??�h�u�B��??)
    Fire,              // ??�K??�`
    Chemical,          // ??��??�`
    Psychological,      // ��?X?�`
    Environmental      // ???X?`
};

// ��??��??
UENUM(BlueprintType)
enum class EMingEquipmentType : uint8
{
    Rifle,             // �B??
    MachineGun,         // ��??
    Artillery,         // ??�L
    Mortar,            // ��?X
    Grenade,           // ??�h??
    Bayonet,           // 摧毀
    Tank,              // 摧毀
    Aircraft,          // ��??
    NavalGun,          // ??��
    Melee              // ��ԪZ��
};

// �ɵ���??
UENUM(BlueprintType)
enum class EMingSupplyType: uint8 {
    Ammunition,        // �u��
    Fuel,              // 摧毀
    Food,              // ³??
    Medical,           // 目標數量
    Equipment,         // ��??
    Reinforcements     // �W��摧毀
};

// ??�`�p?X?��
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDamageCalculation
{
    GENERATED_BODY()

    // ???X?�`
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage;

    // ??�`��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDamageType DamageType;

    // ��??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipmentType EquipmentType;

    // 摧毀�Z��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackDistance;

    // ??��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HitLocation;

    // �@��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmorValue;

    // �@�Ҭ�??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmorPenetration;

    // 摧毀��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalChance;

    // ???X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalMultiplier;

    // ??�`�I??�Y��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageFalloff;

    // �u����??�ץ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AmmoTypeModifier;

    // ??�μv�T摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainModifier;

    // �Ѯ�v�T摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WeatherModifier;

    // �h��v�T摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoraleModifier;

    FMingDamageCalculation()
    {
        BaseDamage = 0.0f;
        DamageType = EMingDamageType::Kinetic;
        EquipmentType = EMingEquipmentType::Rifle;
        AttackDistance = 0.0f;
        HitLocation = FString("");
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

// �ɵ�摧毀
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingSupplyData
{
    GENERATED_BODY()

    // �ɵ���??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSupplyType SupplyType;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentAmount;

    // ??�j�eX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxCapacity;

    // ���ӳt??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConsumptionRate;

    // �ɵ�摧毀??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SupplyPriority;

    // �ɵ���??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SupplySourceID;

    // ??��??��?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastSupplyTime;

    // �ɵ�??X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SupplyStatus;

    // ���??���HX
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

// ��Ԩ�?X???
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingLogisticsConvoy
{
    GENERATED_BODY()

    // ��??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConvoyID;

    // ��?X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyName;

    // ��??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyType;

    // 摧毀��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentPosition;

    // 摧毀��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPosition;

    // �ɵ�摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingSupplyData> Cargo;

    // ��?X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    // �@??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProtectionLevel;

    // ??�o摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DepartureTime;

    // 故事選項?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EstimatedArrivalTime;

    // ��?X?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConvoyStatus;

    // �@?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EscortUnits;

    // ���I����
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

// ??�`��??
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingDamageResult
{
    GENERATED_BODY()

    // ???X?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackerID;

    // ??�m??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefenderID;

    // ��?X?�`
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ActualDamage;

    // ???X?�`
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage;

    // ??�`��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDamageType DamageType;

    // ??�_摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCriticalHit;

    // ??�_��z�@X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bArmorPenetrated;

    // ??�`摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DamageLocation;

    // ??�`�p??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DamageCalculationDetails;

    // �ɵ���??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingSupplyType, float> SupplyConsumption;

    // ??��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CombatTime;

    // ??����m
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

// ??��??�`�ƥ�e??






/**
 * ??��??�`��?X * �t�d�p?X?��??�԰��ˮ`??��??��Ըɵ��t��
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingCombatDamageManager : public UObject
{
    GENERATED_BODY()

public:
    UMingCombatDamageManager();

    // ???X?�ˮ`�t??
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool InitializeDamageSystem();

    // �p?X?��??�`
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    FMingDamageResult CalculateCombatDamage(const FMingDamageCalculation& DamageCalc);

    // ��?X???�ɵ�
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool RegisterUnitSupply(int32 UnitID, const FMingSupplyData& SupplyData);

    // ??�s摧毀�ɵ�
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool UpdateUnitSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount);

    // 目標數量�ɵ�
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    FMingSupplyData GetUnitSupply(int32 UnitID, EMingSupplyType SupplyType) const;

    // ����摧毀
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool ConsumeSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount);

    // ??�ث�Ԩ�??
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    int32 CreateLogisticsConvoy(const TArray<FMingSupplyData>& Cargo, const FVector& TargetLocation);

    // ������Ԩ�??
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool DispatchConvoy(int32 ConvoyID);

    // 摧毀��??�H��
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    FMingLogisticsConvoy GetConvoyInfo(int32 ConvoyID) const;

    // ??�s��??��m
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool UpdateConvoyPosition(int32 ConvoyID, const FVector& NewPosition);

    // 摧毀�ɵ�摧毀
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    TArray<FMingSupplyData> GetSupplyDemands() const;

    // �p??�ɵ�摧毀??
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    int32 CalculateSupplyPriority(int32 UnitID, EMingSupplyType SupplyType);

    // ???X?�`��??
    UFUNCTION(BlueprintPure, Category = "Combat Damage")
    TMap<EMingDamageType, int32> GetDamageStatistics() const;

    // �O?X?�`摧毀
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool SaveDamageData();

    // ���J??�`摧毀
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    bool LoadDamageData();

    // �M��摧毀�ˮ`��X
    UFUNCTION(BlueprintCallable, Category = "Combat Damage")
    void ClearAllDamageData();

    // �ƥ�e??
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

    // 摧毀�ɵ�摧毀??- �`??�G摧毀TMap ����XUPROPERTY
    TMap<int32, TMap<EMingSupplyType, FMingSupplyData>> UnitSupplyDatabase;

    // ��Ԩ�?X摧毀?
    UPROPERTY()
    TMap<int32, FMingLogisticsConvoy> ConvoyDatabase;

    // ??�`���v�O??
    UPROPERTY()
    TArray<FMingDamageResult> DamageHistory;

    // ��?X?�`??
    UPROPERTY()
    TMap<EMingEquipmentType, float> EquipmentDamageTable;

    // �u��??�`�ץ�??
    UPROPERTY()
    TMap<FString, float> AmmoDamageModifiers;

    // ??��??�`�ץ�??
    UPROPERTY()
    TMap<FString, float> TerrainDamageModifiers;

    // �Ѯ�??�`�ץ�??
    UPROPERTY()
    TMap<FString, float> WeatherDamageModifiers;

    // ??�_�w??�l??
    UPROPERTY()
    bool bInitialized;

private:
    // ���J??�]??�`??
    void LoadDefaultDamageTables();

    // �p?X??X?�`
    float CalculateBaseDamage(const FMingDamageCalculation& DamageCalc) const;

    // �p??�@�Ҵ��
    float CalculateArmorReduction(const FMingDamageCalculation& DamageCalc) const;

    // �p??�Z���I??
    float CalculateDistanceFalloff(const FMingDamageCalculation& DamageCalc) const;

    // �p?X??X?�`
    float CalculateCriticalDamage(float BaseDamage, const FMingDamageCalculation& DamageCalc) const;

    // �p?X?�μv�T
    float CalculateTerrainEffect(const FMingDamageCalculation& DamageCalc) const;

    // �p??�Ѯ�v�T
    float CalculateWeatherEffect(const FMingDamageCalculation& DamageCalc) const;

    // �p??�h��v�T
    float CalculateMoraleEffect(const FMingDamageCalculation& DamageCalc) const;

    // �p??�ɵ���??
    TMap<EMingSupplyType, float> CalculateSupplyConsumption(const FMingDamageCalculation& DamageCalc) const;

    // ??�s???X?��
    void UpdateUnitPerformance(int32 UnitID, const TMap<EMingSupplyType, FMingSupplyData>& Supplies);

    // 摧毀��Ԩ�??��??
    void ProcessConvoyMovement(float DeltaTime);

    // �ˬd�ɵ�摧毀
    void CheckSupplyDemands();

    // �p??��??���I
    float CalculateConvoyRisk(const FMingLogisticsConvoy& Convoy) const;

    // 摧毀�ɵ���??
    TArray<FVector> GenerateSupplyRoute(const FVector& Start, const FVector& End) const;

    // ��?X?�`�p??
    bool ValidateDamageCalculation(const FMingDamageCalculation& DamageCalc) const;

    // ��??�ɵ�摧毀
    bool ValidateSupplyData(const FMingSupplyData& SupplyData) const;

    // �p?X?�`摧毀
    FString CalculateHitLocation(const FVector& ImpactPoint) const;

    // 摧毀�ɵ����摧毀
    void HandleSupplyEmergency(int32 UnitID, EMingSupplyType SupplyType);

    // ??�s��?X?X
    void UpdateConvoyStatus(int32 ConvoyID, const FString& NewStatus);

    // �p??�ɵ�摧毀
    float CalculateSupplyEfficiency(int32 UnitID) const;
};

