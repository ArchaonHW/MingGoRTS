#include "MingCombatDamageManager.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Algo/RandomStream.h"

UMingCombatDamageManager::UMingCombatDamageManager()
{
    bInitialized = false;
    UnitSupplyDatabase.Empty();
    ConvoyDatabase.Empty();
    DamageHistory.Empty();
    EquipmentDamageTable.Empty();
    AmmoDamageModifiers.Empty();
    TerrainDamageModifiers.Empty();
    WeatherDamageModifiers.Empty();
}

void UMingCombatDamageManager::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("戰鬥傷害管理器開始運行"));
    
    // 初始化系統
    InitializeDamageSystem();
}

void UMingCombatDamageManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bInitialized)
    {
        return;
    }

    // 處理後勤車隊移動
    ProcessConvoyMovement(DeltaTime);

    // 檢查補給需求
    CheckSupplyDemands();
}

bool UMingCombatDamageManager::InitializeDamageSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("戰鬥傷害系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化戰鬥傷害系統..."));

    // 載入預設傷害表
    LoadDefaultDamageTables();

    // 載入已保存的傷害數據
    LoadDamageData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("戰鬥傷害系統初始化完成"));
    
    return true;
}

FMingDamageResult UMingCombatDamageManager::CalculateCombatDamage(const FMingDamageCalculation& DamageCalc)
{
    FMingDamageResult DamageResult;
    DamageResult.AttackerID = -1; // 需要從調用者設置
    DamageResult.DefenderID = -1; // 需要從調用者設置
    DamageResult.DamageType = DamageCalc.DamageType;
    DamageResult.CombatTime = FDateTime::Now();

    if (!ValidateDamageCalculation(DamageCalc))
    {
        UE_LOG(LogTemp, Error, TEXT("傷害計算參數驗證失敗"));
        return DamageResult;
    }

    // 計算基礎傷害
    float BaseDamage = CalculateBaseDamage(DamageCalc);
    DamageResult.BaseDamage = BaseDamage;

    // 計算護甲減傷
    float ArmorReduction = CalculateArmorReduction(DamageCalc);
    float DamageAfterArmor = BaseDamage - ArmorReduction;
    DamageResult.bArmorPenetrated = (DamageAfterArmor > 0);

    // 計算距離衰減
    float DistanceFalloff = CalculateDistanceFalloff(DamageCalc);
    float DamageAfterDistance = DamageAfterArmor * DistanceFalloff;

    // 計算地形影響
    float TerrainEffect = CalculateTerrainEffect(DamageCalc);
    float DamageAfterTerrain = DamageAfterDistance * TerrainEffect;

    // 計算天氣影響
    float WeatherEffect = CalculateWeatherEffect(DamageCalc);
    float DamageAfterWeather = DamageAfterTerrain * WeatherEffect;

    // 計算士氣影響
    float MoraleEffect = CalculateMoraleEffect(DamageCalc);
    float FinalDamage = DamageAfterWeather * MoraleEffect;

    // 計算暴擊傷害
    bool bIsCritical = (FMath::FRand() < DamageCalc.CriticalChance);
    if (bIsCritical)
    {
        FinalDamage = CalculateCriticalDamage(FinalDamage, DamageCalc);
    }

    DamageResult.bIsCriticalHit = bIsCritical;
    DamageResult.ActualDamage = FMath::Max(0.0f, FinalDamage);

    // 計算傷害部位
    DamageResult.DamageLocation = CalculateHitLocation(FVector::ZeroVector); // 需要實際位置

    // 生成傷害計算詳情
    DamageResult.DamageCalculationDetails = FString::Printf(
        TEXT("基礎傷害: %.1f, 護甲減傷: %.1f, 距離衰減: %.2f, 地形影響: %.2f, 天氣影響: %.2f, 士氣影響: %.2f, 暴擊: %s"),
        BaseDamage, ArmorReduction, DistanceFalloff, TerrainEffect, WeatherEffect, MoraleEffect,
        bIsCritical ? TEXT("是") : TEXT("否")
    );

    // 計算補給消耗
    DamageResult.SupplyConsumption = CalculateSupplyConsumption(DamageCalc);

    // 添加到傷害歷史
    DamageHistory.Add(DamageResult);

    // 觸發傷害計算事件
    OnDamageCalculated.Broadcast(DamageResult);

    UE_LOG(LogTemp, Log, TEXT("計算戰鬥傷害: 基礎 %.1f -> 實際 %.1f"), 
        BaseDamage, DamageResult.ActualDamage);
    
    return DamageResult;
}

bool UMingCombatDamageManager::RegisterUnitSupply(int32 UnitID, const FMingSupplyData& SupplyData)
{
    if (!ValidateSupplyData(SupplyData))
    {
        UE_LOG(LogTemp, Error, TEXT("補給數據驗證失敗"));
        return false;
    }

    if (!UnitSupplyDatabase.Contains(UnitID))
    {
        UnitSupplyDatabase.Add(UnitID, TMap<EMingSupplyType, FMingSupplyData>());
    }

    TMap<EMingSupplyType, FMingSupplyData>& UnitSupplies = UnitSupplyDatabase[UnitID];
    UnitSupplies.Add(SupplyData.SupplyType, SupplyData);

    UE_LOG(LogTemp, Log, TEXT("註冊單位補給: ID %d，類型 %d，數量 %.1f"), 
        UnitID, (int32)SupplyData.SupplyType, SupplyData.CurrentAmount);
    
    return true;
}

bool UMingCombatDamageManager::UpdateUnitSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount)
{
    if (!UnitSupplyDatabase.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在，無法更新補給"), UnitID);
        return false;
    }

    TMap<EMingSupplyType, FMingSupplyData>& UnitSupplies = UnitSupplyDatabase[UnitID];
    
    if (!UnitSupplies.Contains(SupplyType))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位 %d 沒有補給類型 %d"), UnitID, (int32)SupplyType);
        return false;
    }

    FMingSupplyData& Supply = UnitSupplies[SupplyType];
    float OldAmount = Supply.CurrentAmount;
    Supply.CurrentAmount = FMath::Clamp(Amount, 0.0f, Supply.MaxCapacity);
    Supply.LastSupplyTime = FDateTime::Now();

    // 更新補給狀態
    if (Supply.CurrentAmount <= Supply.EmergencyThreshold)
    {
        Supply.SupplyStatus = TEXT("emergency");
        HandleSupplyEmergency(UnitID, SupplyType);
    }
    else if (Supply.CurrentAmount <= Supply.MaxCapacity * 0.3f)
    {
        Supply.SupplyStatus = TEXT("low");
    }
    else if (Supply.CurrentAmount >= Supply.MaxCapacity * 0.8f)
    {
        Supply.SupplyStatus = TEXT("full");
    }
    else
    {
        Supply.SupplyStatus = TEXT("normal");
    }

    // 更新單位性能
    UpdateUnitPerformance(UnitID, UnitSupplies);

    // 觸發補給狀態變化事件
    OnSupplyStatusChanged.Broadcast(UnitID, SupplyType, Supply.CurrentAmount);

    UE_LOG(LogTemp, Log, TEXT("更新單位 %d 補給 %d: %.1f -> %.1f"), 
        UnitID, (int32)SupplyType, OldAmount, Supply.CurrentAmount);
    
    return true;
}

FMingSupplyData UMingCombatDamageManager::GetUnitSupply(int32 UnitID, EMingSupplyType SupplyType) const
{
    if (UnitSupplyDatabase.Contains(UnitID))
    {
        const TMap<EMingSupplyType, FMingSupplyData>& UnitSupplies = UnitSupplyDatabase[UnitID];
        if (UnitSupplies.Contains(SupplyType))
        {
            return UnitSupplies[SupplyType];
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("單位 %d 補給 %d 不存在，返回默認值"), UnitID, (int32)SupplyType);
    return FMingSupplyData();
}

bool UMingCombatDamageManager::ConsumeSupply(int32 UnitID, EMingSupplyType SupplyType, float Amount)
{
    if (!UnitSupplyDatabase.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在，無法消耗補給"), UnitID);
        return false;
    }

    TMap<EMingSupplyType, FMingSupplyData>& UnitSupplies = UnitSupplyDatabase[UnitID];
    
    if (!UnitSupplies.Contains(SupplyType))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位 %d 沒有補給類型 %d"), UnitID, (int32)SupplyType);
        return false;
    }

    FMingSupplyData& Supply = UnitSupplies[SupplyType];
    
    if (Supply.CurrentAmount < Amount)
    {
        UE_LOG(LogTemp, Warning, TEXT("單位 %d 補給不足，無法消耗 %.1f"), UnitID, Amount);
        return false;
    }

    float OldAmount = Supply.CurrentAmount;
    Supply.CurrentAmount -= Amount;
    Supply.LastSupplyTime = FDateTime::Now();

    // 觸發補給消耗事件
    TMap<EMingSupplyType, float> ConsumedSupplies;
    ConsumedSupplies.Add(SupplyType, Amount);
    OnSupplyConsumed.Broadcast(UnitID, ConsumedSupplies);

    // 更新單位性能
    UpdateUnitPerformance(UnitID, UnitSupplies);

    UE_LOG(LogTemp, Log, TEXT("單位 %d 消耗補給 %d: %.1f"), 
        UnitID, (int32)SupplyType, Amount);
    
    return true;
}

int32 UMingCombatDamageManager::CreateLogisticsConvoy(const TArray<FMingSupplyData>& Cargo, const FVector& TargetLocation)
{
    // 生成新的車隊ID
    int32 NewConvoyID = ConvoyDatabase.Num() + 1;

    // 創建後勤車隊
    FMingLogisticsConvoy NewConvoy;
    NewConvoy.ConvoyID = NewConvoyID;
    NewConvoy.ConvoyName = FString::Printf(TEXT("補給車隊 %d"), NewConvoyID);
    NewConvoy.ConvoyType = TEXT("supply");
    NewConvoy.CurrentPosition = FVector::ZeroVector; // 需要設置起始位置
    NewConvoy.TargetPosition = TargetLocation;
    NewConvoy.Cargo = Cargo;
    NewConvoy.MovementSpeed = 20.0f;
    NewConvoy.ProtectionLevel = 1.0f;
    NewConvoy.DepartureTime = FDateTime::Now();
    
    // 計算預計到達時間
    float Distance = FVector::Dist(NewConvoy.CurrentPosition, TargetLocation);
    float TravelTime = Distance / NewConvoy.MovementSpeed;
    NewConvoy.EstimatedArrivalTime = NewConvoy.DepartureTime + FTimespan::FromSeconds(TravelTime);
    
    NewConvoy.ConvoyStatus = TEXT("preparing");
    NewConvoy.RiskAssessment = CalculateConvoyRisk(NewConvoy);

    // 添加到車隊數據庫
    ConvoyDatabase.Add(NewConvoyID, NewConvoy);

    UE_LOG(LogTemp, Log, TEXT("創建後勤車隊: ID %d，目標位置 (%.1f, %.1f, %.1f)"), 
        NewConvoyID, TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
    
    return NewConvoyID;
}

bool UMingCombatDamageManager::DispatchConvoy(int32 ConvoyID)
{
    if (!ConvoyDatabase.Contains(ConvoyID))
    {
        UE_LOG(LogTemp, Warning, TEXT("車隊ID %d 不存在"), ConvoyID);
        return false;
    }

    FMingLogisticsConvoy& Convoy = ConvoyDatabase[ConvoyID];
    
    if (Convoy.ConvoyStatus != TEXT("preparing"))
    {
        UE_LOG(LogTemp, Warning, TEXT("車隊 %d 狀態不允許派遣: %s"), ConvoyID, *Convoy.ConvoyStatus);
        return false;
    }

    // 更新車隊狀態
    UpdateConvoyStatus(ConvoyID, TEXT("in_transit"));

    UE_LOG(LogTemp, Log, TEXT("派遣後勤車隊: ID %d"), ConvoyID);
    
    return true;
}

FMingLogisticsConvoy UMingCombatDamageManager::GetConvoyInfo(int32 ConvoyID) const
{
    if (ConvoyDatabase.Contains(ConvoyID))
    {
        return ConvoyDatabase[ConvoyID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("車隊ID %d 不存在，返回空車隊"), ConvoyID);
    return FMingLogisticsConvoy();
}

bool UMingCombatDamageManager::UpdateConvoyPosition(int32 ConvoyID, const FVector& NewPosition)
{
    if (!ConvoyDatabase.Contains(ConvoyID))
    {
        UE_LOG(LogTemp, Warning, TEXT("車隊ID %d 不存在"), ConvoyID);
        return false;
    }

    FMingLogisticsConvoy& Convoy = ConvoyDatabase[ConvoyID];
    FVector OldPosition = Convoy.CurrentPosition;
    Convoy.CurrentPosition = NewPosition;

    // 檢查是否到達目標
    float DistanceToTarget = FVector::Dist(NewPosition, Convoy.TargetPosition);
    if (DistanceToTarget <= 50.0f) // 50米內視為到達
    {
        UpdateConvoyStatus(ConvoyID, TEXT("delivered"));
        
        // 觸發補給送達事件
        OnSupplyDelivered.Broadcast(Convoy);
        
        UE_LOG(LogTemp, Log, TEXT("車隊 %d 已到達目標位置"), ConvoyID);
    }

    UE_LOG(LogTemp, Log, TEXT("更新車隊 %d 位置: (%.1f, %.1f, %.1f) -> (%.1f, %.1f, %.1f)"), 
        ConvoyID, OldPosition.X, OldPosition.Y, OldPosition.Z, NewPosition.X, NewPosition.Y, NewPosition.Z);
    
    return true;
}

TArray<FMingSupplyData> UMingCombatDamageManager::GetSupplyDemands() const
{
    TArray<FMingSupplyData> Demands;
    
    // 收集所有緊急補給需求
    for (const TPair<int32, TMap<EMingSupplyType, FMingSupplyData>>& UnitPair : UnitSupplyDatabase)
    {
        int32 UnitID = UnitPair.Key;
        const TMap<EMingSupplyType, FMingSupplyData>& UnitSupplies = UnitPair.Value;
        
        for (const TPair<EMingSupplyType, FMingSupplyData>& SupplyPair : UnitSupplies)
        {
            const FMingSupplyData& Supply = SupplyPair.Value;
            
            if (Supply.CurrentAmount <= Supply.EmergencyThreshold)
            {
                FMingSupplyData Demand = Supply;
                Demand.SupplySourceID = UnitID;
                Demands.Add(Demand);
            }
        }
    }

    return Demands;
}

int32 UMingCombatDamageManager::CalculateSupplyPriority(int32 UnitID, EMingSupplyType SupplyType)
{
    if (!UnitSupplyDatabase.Contains(UnitID))
    {
        return 0;
    }

    const TMap<EMingSupplyType, FMingSupplyData>& UnitSupplies = UnitSupplyDatabase[UnitID];
    
    if (!UnitSupplies.Contains(SupplyType))
    {
        return 0;
    }

    const FMingSupplyData& Supply = UnitSupplies[SupplyType];
    
    // 基礎優先級
    int32 BasePriority = Supply.SupplyPriority;
    
    // 緊急程度加成
    if (Supply.CurrentAmount <= Supply.EmergencyThreshold)
    {
        BasePriority += 100; // 緊急補給
    }
    else if (Supply.CurrentAmount <= Supply.MaxCapacity * 0.3f)
    {
        BasePriority += 50; // 低補給
    }
    
    // 單位重要性加成（這裡需要從戰鬥系統獲取單位類型）
    // BasePriority += GetUnitImportance(UnitID);
    
    return BasePriority;
}

TMap<EMingDamageType, int32> UMingCombatDamageManager::GetDamageStatistics() const
{
    TMap<EMingDamageType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingDamageType::Kinetic, 0);
    Statistics.Add(EMingDamageType::Explosive, 0);
    Statistics.Add(EMingDamageType::Fire, 0);
    Statistics.Add(EMingDamageType::Chemical, 0);
    Statistics.Add(EMingDamageType::Psychological, 0);
    Statistics.Add(EMingDamageType::Environmental, 0);
    
    // 統計傷害類型
    for (const FMingDamageResult& Damage : DamageHistory)
    {
        int32& Count = Statistics[Damage.DamageType];
        Count++;
    }
    
    return Statistics;
}

bool UMingCombatDamageManager::SaveDamageData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("CombatDamage.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("戰鬥傷害數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 條傷害記錄，%d 個補給單位，%d 個車隊"), 
        DamageHistory.Num(), UnitSupplyDatabase.Num(), ConvoyDatabase.Num());
    
    return true;
}

bool UMingCombatDamageManager::LoadDamageData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("CombatDamage.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入戰鬥傷害數據"), *LoadPath);
    
    return true;
}

void UMingCombatDamageManager::ClearAllDamageData()
{
    UnitSupplyDatabase.Empty();
    ConvoyDatabase.Empty();
    DamageHistory.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("所有戰鬥傷害數據已清除"));
}

void UMingCombatDamageManager::LoadDefaultDamageTables()
{
    // 裝備傷害表
    EquipmentDamageTable.Add(EMingEquipmentType::Rifle, 25.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::MachineGun, 45.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::Artillery, 120.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::Mortar, 80.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::Grenade, 60.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::Bayonet, 15.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::Tank, 200.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::Aircraft, 150.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::NavalGun, 300.0f);
    EquipmentDamageTable.Add(EMingEquipmentType::Melee, 10.0f);

    // 彈藥傷害修正表
    AmmoDamageModifiers.Add(TEXT("standard"), 1.0f);
    AmmoDamageModifiers.Add(TEXT("armor_piercing"), 1.5f);
    AmmoDamageModifiers.Add(TEXT("high_explosive"), 1.3f);
    AmmoDamageModifiers.Add(TEXT("incendiary"), 1.2f);
    AmmoDamageModifiers.Add(TEXT("fragmentation"), 1.4f);

    // 地形傷害修正表
    TerrainDamageModifiers.Add(TEXT("open"), 1.0f);
    TerrainDamageModifiers.Add(TEXT("forest"), 0.8f);
    TerrainDamageModifiers.Add(TEXT("mountain"), 0.9f);
    TerrainDamageModifiers.Add(TEXT("urban"), 0.7f);
    TerrainDamageModifiers.Add(TEXT("water"), 0.6f);

    // 天氣傷害修正表
    WeatherDamageModifiers.Add(TEXT("clear"), 1.0f);
    WeatherDamageModifiers.Add(TEXT("rain"), 0.9f);
    WeatherDamageModifiers.Add(TEXT("fog"), 0.7f);
    WeatherDamageModifiers.Add(TEXT("snow"), 0.8f);
    WeatherDamageModifiers.Add(TEXT("storm"), 0.6f);

    UE_LOG(LogTemp, Log, TEXT("已載入預設傷害表"));
}

float UMingCombatDamageManager::CalculateBaseDamage(const FMingDamageCalculation& DamageCalc) const
{
    float BaseDamage = DamageCalc.BaseDamage;
    
    // 應用裝備傷害修正
    if (EquipmentDamageTable.Contains(DamageCalc.EquipmentType))
    {
        BaseDamage = EquipmentDamageTable[DamageCalc.EquipmentType];
    }
    
    // 應用彈藥類型修正
    if (DamageCalc.AmmoTypeModifier != 1.0f)
    {
        BaseDamage *= DamageCalc.AmmoTypeModifier;
    }
    
    return BaseDamage;
}

float UMingCombatDamageManager::CalculateArmorReduction(const FMingDamageCalculation& DamageCalc) const
{
    if (DamageCalc.ArmorValue <= 0.0f)
    {
        return 0.0f;
    }

    // 計算護甲穿透
    bool bPenetrated = DamageCalc.ArmorPenetration >= DamageCalc.ArmorValue;
    
    if (bPenetrated)
    {
        // 穿透護甲，減傷較少
        return DamageCalc.ArmorValue * 0.3f;
    }
    else
    {
        // 未穿透護甲，減傷較多
        return DamageCalc.ArmorValue * 0.8f;
    }
}

float UMingCombatDamageManager::CalculateDistanceFalloff(const FMingDamageCalculation& DamageCalc) const
{
    if (DamageCalc.DamageFalloff == 1.0f || DamageCalc.AttackDistance <= 0.0f)
    {
        return 1.0f;
    }

    // 計算距離衰減
    float MaxEffectiveRange = 500.0f; // 假設最大有效射程
    float FalloffFactor = 1.0f - (DamageCalc.AttackDistance / MaxEffectiveRange);
    FalloffFactor = FMath::Clamp(FalloffFactor, 0.1f, 1.0f);
    
    return FalloffFactor * DamageCalc.DamageFalloff;
}

float UMingCombatDamageManager::CalculateCriticalDamage(float BaseDamage, const FMingDamageCalculation& DamageCalc) const
{
    return BaseDamage * DamageCalc.CriticalMultiplier;
}

float UMingCombatDamageManager::CalculateTerrainEffect(const FMingDamageCalculation& DamageCalc) const
{
    // 使用預設的地形修正值
    return DamageCalc.TerrainModifier;
}

float UMingCombatDamageManager::CalculateWeatherEffect(const FMingDamageCalculation& DamageCalc) const
{
    // 使用預設的天氣修正值
    return DamageCalc.WeatherModifier;
}

float UMingCombatDamageManager::CalculateMoraleEffect(const FMingDamageCalculation& DamageCalc) const
{
    // 使用預設的士氣修正值
    return DamageCalc.MoraleModifier;
}

TMap<EMingSupplyType, float> UMingCombatDamageManager::CalculateSupplyConsumption(const FMingDamageCalculation& DamageCalc) const
{
    TMap<EMingSupplyType, float> Consumption;
    
    // 根據裝備類型計算補給消耗
    switch (DamageCalc.EquipmentType)
    {
    case EMingEquipmentType::Rifle:
        Consumption.Add(EMingSupplyType::Ammunition, 1.0f);
        break;
    case EMingEquipmentType::MachineGun:
        Consumption.Add(EMingSupplyType::Ammunition, 5.0f);
        break;
    case EMingEquipmentType::Artillery:
        Consumption.Add(EMingSupplyType::Ammunition, 10.0f);
        break;
    case EMingEquipmentType::Tank:
        Consumption.Add(EMingSupplyType::Fuel, 2.0f);
        Consumption.Add(EMingSupplyType::Ammunition, 3.0f);
        break;
    case EMingEquipmentType::Aircraft:
        Consumption.Add(EMingSupplyType::Fuel, 5.0f);
        Consumption.Add(EMingSupplyType::Ammunition, 2.0f);
        break;
    default:
        Consumption.Add(EMingSupplyType::Ammunition, 1.0f);
        break;
    }
    
    return Consumption;
}

void UMingCombatDamageManager::UpdateUnitPerformance(int32 UnitID, const TMap<EMingSupplyType, FMingSupplyData>& Supplies)
{
    // 根據補給狀況更新單位性能
    float PerformanceModifier = 1.0f;
    
    for (const TPair<EMingSupplyType, FMingSupplyData>& SupplyPair : Supplies)
    {
        const FMingSupplyData& Supply = SupplyPair.Value;
        
        float SupplyRatio = Supply.CurrentAmount / Supply.MaxCapacity;
        
        if (SupplyRatio <= 0.2f)
        {
            PerformanceModifier *= 0.5f; // 嚴重影響
        }
        else if (SupplyRatio <= 0.5f)
        {
            PerformanceModifier *= 0.8f; // 中等影響
        }
        else if (SupplyRatio <= 0.8f)
        {
            PerformanceModifier *= 0.95f; // 輕微影響
        }
    }
    
    // 這裡需要調用戰鬥系統來更新單位性能
    // CombatSystem->UpdateUnitPerformance(UnitID, PerformanceModifier);
    
    UE_LOG(LogTemp, Log, TEXT("更新單位 %d 性能修正: %.2f"), UnitID, PerformanceModifier);
}

void UMingCombatDamageManager::ProcessConvoyMovement(float DeltaTime)
{
    for (TPair<int32, FMingLogisticsConvoy>& Pair : ConvoyDatabase)
    {
        int32 ConvoyID = Pair.Key;
        FMingLogisticsConvoy& Convoy = Pair.Value;
        
        if (Convoy.ConvoyStatus == TEXT("in_transit"))
        {
            // 計算移動方向
            FVector Direction = Convoy.TargetPosition - Convoy.CurrentPosition;
            float Distance = Direction.Size();
            
            if (Distance > 10.0f)
            {
                Direction.Normalize();
                FVector Movement = Direction * Convoy.MovementSpeed * DeltaTime;
                
                // 更新位置
                UpdateConvoyPosition(ConvoyID, Convoy.CurrentPosition + Movement);
            }
        }
    }
}

void UMingCombatDamageManager::CheckSupplyDemands()
{
    // 檢查所有單位的補給需求
    TArray<FMingSupplyData> Demands = GetSupplyDemands();
    
    if (Demands.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("檢測到 %d 個緊急補給需求"), Demands.Num());
        
        // 這裡可以實作自動補給邏輯
        // 例如：自動創建補給車隊
    }
}

float UMingCombatDamageManager::CalculateConvoyRisk(const FMingLogisticsConvoy& Convoy) const
{
    float BaseRisk = 0.5f;
    
    // 距離風險
    float Distance = FVector::Dist(Convoy.CurrentPosition, Convoy.TargetPosition);
    float DistanceRisk = FMath::Clamp(Distance / 1000.0f, 0.0f, 1.0f);
    
    // 護衛風險
    float EscortRisk = 1.0f - (Convoy.ProtectionLevel / 5.0f);
    
    // 貨物價值風險
    float CargoRisk = 0.0f;
    for (const FMingSupplyData& Cargo : Convoy.Cargo)
    {
        CargoRisk += Cargo.CurrentAmount * 0.01f;
    }
    CargoRisk = FMath::Clamp(CargoRisk, 0.0f, 1.0f);
    
    // 綜合風險評估
    float TotalRisk = (BaseRisk + DistanceRisk + EscortRisk + CargoRisk) / 4.0f;
    
    return FMath::Clamp(TotalRisk, 0.0f, 1.0f);
}

bool UMingCombatDamageManager::ValidateDamageCalculation(const FMingDamageCalculation& DamageCalc) const
{
    if (DamageCalc.BaseDamage < 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("基礎傷害不能為負數"));
        return false;
    }
    
    if (DamageCalc.AttackDistance < 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("攻擊距離不能為負數"));
        return false;
    }
    
    return true;
}

bool UMingCombatDamageManager::ValidateSupplyData(const FMingSupplyData& SupplyData) const
{
    if (SupplyData.CurrentAmount < 0.0f || SupplyData.MaxCapacity <= 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("補給數據無效"));
        return false;
    }
    
    if (SupplyData.CurrentAmount > SupplyData.MaxCapacity)
    {
        UE_LOG(LogTemp, Warning, TEXT("當前補給數量超過最大容量"));
    }
    
    return true;
}

FString UMingCombatDamageManager::CalculateHitLocation(const FVector& ImpactPoint) const
{
    // 簡單的傷害部位計算
    TArray<FString> HitLocations = {
        TEXT("頭部"), TEXT("胸部"), TEXT("腹部"), 
        TEXT("左臂"), TEXT("右臂"), TEXT("左腿"), TEXT("右腿")
    };
    
    int32 RandomIndex = FMath::RandRange(0, HitLocations.Num());
    return HitLocations[RandomIndex];
}

void UMingCombatDamageManager::HandleSupplyEmergency(int32 UnitID, EMingSupplyType SupplyType)
{
    UE_LOG(LogTemp, Warning, TEXT("單位 %d 補給 %d 緊急狀態"), UnitID, (int32)SupplyType);
    
    // 這裡可以實作緊急補給邏輯
    // 例如：發送緊急補給請求、降低單位性能等
}

void UMingCombatDamageManager::UpdateConvoyStatus(int32 ConvoyID, const FString& NewStatus)
{
    if (ConvoyDatabase.Contains(ConvoyID))
    {
        FMingLogisticsConvoy& Convoy = ConvoyDatabase[ConvoyID];
        FString OldStatus = Convoy.ConvoyStatus;
        Convoy.ConvoyStatus = NewStatus;
        
        // 觸發車隊狀態變化事件
        OnConvoyStatusChanged.Broadcast(ConvoyID, NewStatus);
        
        UE_LOG(LogTemp, Log, TEXT("車隊 %d 狀態更新: %s -> %s"), 
            ConvoyID, *OldStatus, *NewStatus);
    }
}

float UMingCombatDamageManager::CalculateSupplyEfficiency(int32 UnitID) const
{
    if (!UnitSupplyDatabase.Contains(UnitID))
    {
        return 1.0f;
    }

    const TMap<EMingSupplyType, FMingSupplyData>& UnitSupplies = UnitSupplyDatabase[UnitID];
    float TotalEfficiency = 1.0f;
    
    for (const TPair<EMingSupplyType, FMingSupplyData>& SupplyPair : UnitSupplies)
    {
        const FMingSupplyData& Supply = SupplyPair.Value;
        float SupplyRatio = Supply.CurrentAmount / Supply.MaxCapacity;
        TotalEfficiency *= SupplyRatio;
    }
    
    return FMath::Clamp(TotalEfficiency, 0.0f, 1.0f);
}
