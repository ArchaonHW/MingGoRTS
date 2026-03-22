#include "MingTacticalCombatSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

UMingTacticalCombatSystem::UMingTacticalCombatSystem()
{
    PrimaryActorTick.bCanEverTick = true;
    bInitialized = false;
    UnitDatabase.Empty();
    FormationDatabase.Empty();
    SelectedUnits.Empty();
    CombatHistory.Empty();
    VisibilityMap.Empty();
    UnitFormations.Empty();
}

void UMingTacticalCombatSystem::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("戰術戰鬥系統開始運行"));
    
    // 初始化系統
    InitializeTacticalSystem();
}

void UMingTacticalCombatSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bInitialized)
    {
        return;
    }

    // 處理單位移動
    ProcessUnitMovement(DeltaTime);
    
    // 處理戰鬥邏輯
    ProcessCombatLogic(DeltaTime);
    
    // 更新戰爭迷霧
    UpdateFogOfWar();
}

bool UMingTacticalCombatSystem::InitializeTacticalSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("戰術戰鬥系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化戰術戰鬥系統..."));

    // 載入預設編隊
    LoadDefaultFormations();

    // 載入已保存的戰鬥數據
    LoadCombatData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("戰術戰鬥系統初始化完成"));
    
    return true;
}

bool UMingTacticalCombatSystem::RegisterCombatUnit(const FMingCombatUnit& Unit)
{
    if (!ValidateUnitData(Unit))
    {
        UE_LOG(LogTemp, Error, TEXT("單位數據驗證失敗: %s"), *Unit.UnitName);
        return false;
    }

    if (UnitDatabase.Contains(Unit.UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位ID %d 已存在，將覆蓋"), Unit.UnitID);
    }

    UnitDatabase.Add(Unit.UnitID, Unit);
    
    UE_LOG(LogTemp, Log, TEXT("註冊戰鬥單位: %s (ID: %d)"), *Unit.UnitName, Unit.UnitID);
    
    return true;
}

bool UMingTacticalCombatSystem::RemoveCombatUnit(int32 UnitID)
{
    if (!UnitDatabase.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在"), UnitID);
        return false;
    }

    // 從選擇列表中移除
    SelectedUnits.Remove(UnitID);
    
    // 從數據庫中移除
    UnitDatabase.Remove(UnitID);
    
    UE_LOG(LogTemp, Log, TEXT("移除戰鬥單位: ID %d"), UnitID);
    
    return true;
}

FMingCombatUnit UMingTacticalCombatSystem::GetUnitInfo(int32 UnitID) const
{
    if (UnitDatabase.Contains(UnitID))
    {
        return UnitDatabase[UnitID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在，返回空單位"), UnitID);
    return FMingCombatUnit();
}

bool UMingTacticalCombatSystem::SelectUnits(const TArray<int32>& UnitIDs)
{
    for (int32 UnitID : UnitIDs)
    {
        if (UnitDatabase.Contains(UnitID))
        {
            if (!SelectedUnits.Contains(UnitID))
            {
                SelectedUnits.Add(UnitID);
                
                // 更新單位選中狀態
                FMingCombatUnit& Unit = UnitDatabase[UnitID];
                Unit.bIsSelected = true;
                
                UE_LOG(LogTemp, Log, TEXT("選擇單位: %s (ID: %d)"), *Unit.UnitName, UnitID);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在，無法選擇"), UnitID);
        }
    }
    
    return true;
}

bool UMingTacticalCombatSystem::DeselectUnits(const TArray<int32>& UnitIDs)
{
    for (int32 UnitID : UnitIDs)
    {
        if (SelectedUnits.Contains(UnitID))
        {
            SelectedUnits.Remove(UnitID);
            
            // 更新單位選中狀態
            if (UnitDatabase.Contains(UnitID))
            {
                FMingCombatUnit& Unit = UnitDatabase[UnitID];
                Unit.bIsSelected = false;
                
                UE_LOG(LogTemp, Log, TEXT("取消選擇單位: %s (ID: %d)"), *Unit.UnitName, UnitID);
            }
        }
    }
    
    return true;
}

TArray<int32> UMingTacticalCombatSystem::GetSelectedUnits() const
{
    return SelectedUnits;
}

bool UMingTacticalCombatSystem::MoveUnits(const TArray<int32>& UnitIDs, const FVector& TargetLocation)
{
    for (int32 UnitID : UnitIDs)
    {
        if (UnitDatabase.Contains(UnitID))
        {
            FMingCombatUnit& Unit = UnitDatabase[UnitID];
            
            // 設置目標位置
            Unit.TargetPosition = TargetLocation;
            
            // 更新單位狀態
            UpdateUnitState(UnitID, EMingCombatState::Moving);
            
            UE_LOG(LogTemp, Log, TEXT("移動單位: %s 到位置 (%.1f, %.1f, %.1f)"), 
                *Unit.UnitName, TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在，無法移動"), UnitID);
        }
    }
    
    return true;
}

bool UMingTacticalCombatSystem::AttackTarget(int32 AttackerID, int32 TargetID)
{
    if (!UnitDatabase.Contains(AttackerID) || !UnitDatabase.Contains(TargetID))
    {
        UE_LOG(LogTemp, Error, TEXT("攻擊者或目標單位不存在"));
        return false;
    }

    FMingCombatUnit& Attacker = UnitDatabase[AttackerID];
    FMingCombatUnit& Target = UnitDatabase[TargetID];

    // 檢查攻擊範圍
    float Distance = FVector::Dist(Attacker.CurrentPosition, Target.CurrentPosition);
    if (Distance > Attacker.CombatStats.AttackRange)
    {
        UE_LOG(LogTemp, Warning, TEXT("目標超出攻擊範圍"));
        return false;
    }

    // 處理戰鬥
    FMingCombatResult CombatResult = ProcessCombat(AttackerID, TargetID);
    
    // 觸發戰鬥事件
    TArray<FMingCombatUnit> InvolvedUnits = {Attacker, Target};
    OnCombatOccurred.Broadcast(CombatResult, InvolvedUnits);
    
    UE_LOG(LogTemp, Log, TEXT("單位 %s 攻擊 %s，造成 %.1f 傷害"), 
        *Attacker.UnitName, *Target.UnitName, CombatResult.DamageDealt);
    
    return true;
}

bool UMingTacticalCombatSystem::SetFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType)
{
    if (!FormationDatabase.Contains(FormationType))
    {
        UE_LOG(LogTemp, Error, TEXT("編隊類型不存在"));
        return false;
    }

    const FMingTacticalFormation& Formation = FormationDatabase[FormationType];
    
    // 檢查單位數量
    if (UnitIDs.Num() < Formation.MinUnitCount || UnitIDs.Num() > Formation.MaxUnitCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("單位數量不符合編隊要求"));
        return false;
    }

    // 計算編隊中心點
    FVector FormationCenter = FVector::ZeroVector;
    for (int32 UnitID : UnitIDs)
    {
        if (UnitDatabase.Contains(UnitID))
        {
            FormationCenter += UnitDatabase[UnitID].CurrentPosition;
        }
    }
    FormationCenter /= UnitIDs.Num();

    // 生成編隊位置
    TArray<FVector> FormationPositions = GenerateFormationPositions(FormationType, FormationCenter, UnitIDs.Num());

    // 更新單位位置和編隊信息
    for (int32 i = 0; i < UnitIDs.Num(); i++)
    {
        int32 UnitID = UnitIDs[i];
        if (UnitDatabase.Contains(UnitID))
        {
            FMingCombatUnit& Unit = UnitDatabase[UnitID];
            Unit.TargetPosition = FormationPositions[i];
            Unit.FormationID = Formation.FormationID;
            Unit.FormationPosition = i;
            
            // 應用編隊加成
            for (const TPair<FString, float>& Bonus : Formation.FormationBonuses)
            {
                if (Bonus.Key == TEXT("morale"))
                {
                    Unit.CombatStats.Morale += Bonus.Value;
                }
                else if (Bonus.Key == TEXT("defense"))
                {
                    Unit.CombatStats.DefensePower += Bonus.Value;
                }
            }
        }
    }

    // 觸發編隊變化事件
    TArray<FMingCombatUnit> FormationUnits;
    for (int32 UnitID : UnitIDs)
    {
        if (UnitDatabase.Contains(UnitID))
        {
            FormationUnits.Add(UnitDatabase[UnitID]);
        }
    }
    
    OnFormationChanged.Broadcast(Formation.FormationID, FormationType, FormationUnits);

    UE_LOG(LogTemp, Log, TEXT("設置編隊: %s，涉及 %d 個單位"), 
        *Formation.FormationName, UnitIDs.Num());
    
    return true;
}

FMingTacticalFormation UMingTacticalCombatSystem::GetFormationInfo(EMingFormationType FormationType) const
{
    if (FormationDatabase.Contains(FormationType))
    {
        return FormationDatabase[FormationType];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("編隊類型不存在，返回默認編隊"));
    return FMingTacticalFormation();
}

bool UMingTacticalCombatSystem::IssueTacticalCommand(const FMingTacticalCommand& Command)
{
    // 根據命令類型處理不同邏輯
    if (Command.CommandType == TEXT("move"))
    {
        return MoveUnits({Command.TargetUnitID}, Command.TargetLocation);
    }
    else if (Command.CommandType == TEXT("attack"))
    {
        return AttackTarget(Command.TargetUnitID, Command.TargetUnitID);
    }
    else if (Command.CommandType == TEXT("formation"))
    {
        EMingFormationType FormationType = EMingFormationType::Line;
        // 從命令參數中解析編隊類型
        if (Command.CommandParameters.Contains(TEXT("formation_type")))
        {
            FString FormationTypeStr = Command.CommandParameters[TEXT("formation_type")];
            if (FormationTypeStr == TEXT("wedge"))
            {
                FormationType = EMingFormationType::Wedge;
            }
            else if (FormationTypeStr == TEXT("circle"))
            {
                FormationType = EMingFormationType::Circle;
            }
        }
        
        return SetFormation({Command.TargetUnitID}, FormationType);
    }

    UE_LOG(LogTemp, Warning, TEXT("未知的命令類型: %s"), *Command.CommandType);
    return false;
}

FMingCombatResult UMingTacticalCombatSystem::ProcessCombat(int32 AttackerID, int32 DefenderID)
{
    FMingCombatResult Result;
    Result.AttackerID = AttackerID;
    Result.DefenderID = DefenderID;
    Result.CombatTime = FDateTime::Now();

    if (!UnitDatabase.Contains(AttackerID) || !UnitDatabase.Contains(DefenderID))
    {
        UE_LOG(LogTemp, Error, TEXT("攻擊者或防禦者不存在"));
        return Result;
    }

    const FMingCombatUnit& Attacker = UnitDatabase[AttackerID];
    FMingCombatUnit& Defender = UnitDatabase[DefenderID];

    // 設置戰鬥位置
    Result.CombatLocation = Defender.CurrentPosition;

    // 計算命中概率
    float HitChance = CalculateHitChance(Attacker, Defender);
    Result.bIsHit = (FMath::FRand() < HitChance);

    if (Result.bIsHit)
    {
        // 計算暴擊概率
        float CriticalChance = CalculateCriticalChance(Attacker);
        Result.bIsCriticalHit = (FMath::FRand() < CriticalChance);

        // 計算傷害
        Result.DamageDealt = CalculateCombatDamage(Attacker, Defender);

        if (Result.bIsCriticalHit)
        {
            Result.DamageDealt *= 2.0f; // 暴擊雙倍傷害
            Result.CombatType = TEXT("critical_hit");
        }
        else
        {
            Result.CombatType = TEXT("normal_hit");
        }

        // 應用傷害到防禦者
        Defender.CombatStats.Health -= Result.DamageDealt;
        
        // 檢查是否被摧毀
        if (Defender.CombatStats.Health <= 0)
        {
            Defender.CombatStats.Health = 0;
            UpdateUnitState(DefenderID, EMingCombatState::Destroyed);
            Result.CombatType = TEXT("destroyed");
        }

        // 計算經驗值
        Result.ExperienceGained = 10.0f * (1.0f + Defender.CombatStats.Level * 0.1f);
        UpdateUnitExperience(AttackerID, Result.ExperienceGained);
    }
    else
    {
        Result.DamageDealt = 0.0f;
        Result.bIsCriticalHit = false;
        Result.CombatType = TEXT("miss");
    }

    // 記錄戰鬥歷史
    CombatHistory.Add(Result);

    // 更新攻擊者狀態
    UpdateUnitState(AttackerID, EMingCombatState::Attacking);

    return Result;
}

void UMingTacticalCombatSystem::UpdateFogOfWar()
{
    // 清空可見性地圖
    VisibilityMap.Empty();

    // 為每個單位計算可見性
    for (const TPair<int32, FMingCombatUnit>& Pair : UnitDatabase)
    {
        int32 ViewerID = Pair.Key;
        const FMingCombatUnit& Viewer = Pair.Value;
        
        TArray<int32> VisibleUnits;
        
        for (const TPair<int32, FMingCombatUnit>& TargetPair : UnitDatabase)
        {
            int32 TargetID = TargetPair.Key;
            const FMingCombatUnit& Target = TargetPair.Value;
            
            if (ViewerID != TargetID && IsUnitVisible(ViewerID, TargetID))
            {
                VisibleUnits.Add(TargetID);
                
                // 更新目標單位的可見性
                if (UnitDatabase.Contains(TargetID))
                {
                    FMingCombatUnit& TargetUnit = UnitDatabase[TargetID];
                    TargetUnit.bIsVisible = true;
                    TargetUnit.FogOfWarLevel = 0;
                }
            }
        }
        
        VisibilityMap.Add(ViewerID, VisibleUnits);
    }
}

TArray<int32> UMingTacticalCombatSystem::GetVisibleUnits(int32 ViewerUnitID) const
{
    if (VisibilityMap.Contains(ViewerUnitID))
    {
        return VisibilityMap[ViewerUnitID];
    }
    
    return TArray<int32>();
}

TMap<EMingUnitType, int32> UMingTacticalCombatSystem::GetCombatStatistics() const
{
    TMap<EMingUnitType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingUnitType::Infantry, 0);
    Statistics.Add(EMingUnitType::Cavalry, 0);
    Statistics.Add(EMingUnitType::Artillery, 0);
    Statistics.Add(EMingUnitType::Tank, 0);
    Statistics.Add(EMingUnitType::Aircraft, 0);
    Statistics.Add(EMingUnitType::Naval, 0);
    Statistics.Add(EMingUnitType::Support, 0);
    Statistics.Add(EMingUnitType::Commander, 0);
    
    // 統計單位類型
    for (const TPair<int32, FMingCombatUnit>& Pair : UnitDatabase)
    {
        const FMingCombatUnit& Unit = Pair.Value;
        int32& Count = Statistics[Unit.UnitType];
        Count++;
    }
    
    return Statistics;
}

bool UMingTacticalCombatSystem::SaveCombatData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("TacticalCombat.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("戰鬥數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個單位，%d 條戰鬥記錄"), UnitDatabase.Num(), CombatHistory.Num());
    
    return true;
}

bool UMingTacticalCombatSystem::LoadCombatData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("TacticalCombat.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入戰鬥數據"), *LoadPath);
    
    return true;
}

void UMingTacticalCombatSystem::ClearAllUnits()
{
    UnitDatabase.Empty();
    SelectedUnits.Empty();
    CombatHistory.Empty();
    VisibilityMap.Empty();
    UnitFormations.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("所有戰鬥單位已清除"));
}

void UMingTacticalCombatSystem::LoadDefaultFormations()
{
    // 線形陣型
    FMingTacticalFormation LineFormation;
    LineFormation.FormationID = 1;
    LineFormation.FormationName = TEXT("線形陣型");
    LineFormation.FormationType = EMingFormationType::Line;
    LineFormation.FormationDescription = TEXT("單位排成一線，適合正面攻擊");
    LineFormation.FormationBonuses = {
        {TEXT("morale"), 5.0f},
        {TEXT("attack"), 10.0f}
    };
    LineFormation.FormationPenalties = {
        {TEXT("defense"), -5.0f},
        {TEXT("evasion"), -10.0f}
    };
    LineFormation.ApplicableUnitTypes = {EMingUnitType::Infantry, EMingUnitType::Cavalry};
    LineFormation.MinUnitCount = 3;
    LineFormation.MaxUnitCount = 50;
    
    // 生成線形陣型位置
    for (int32 i = 0; i < 50; i++)
    {
        FVector Position = FVector(i * 100.0f, 0.0f, 0.0f);
        LineFormation.FormationPositions.Add(Position);
    }
    
    FormationDatabase.Add(EMingFormationType::Line, LineFormation);
    
    // 楔形陣型
    FMingTacticalFormation WedgeFormation;
    WedgeFormation.FormationID = 2;
    WedgeFormation.FormationName = TEXT("楔形陣型");
    WedgeFormation.FormationType = EMingFormationType::Wedge;
    WedgeFormation.FormationDescription = TEXT("單位排成楔形，適合突破敵陣");
    WedgeFormation.FormationBonuses = {
        {TEXT("attack"), 15.0f},
        {TEXT("morale"), 10.0f}
    };
    WedgeFormation.FormationPenalties = {
        {TEXT("defense"), -10.0f},
        {TEXT("flank_vulnerability"), -20.0f}
    };
    WedgeFormation.ApplicableUnitTypes = {EMingUnitType::Cavalry, EMingUnitType::Tank};
    WedgeFormation.MinUnitCount = 5;
    WedgeFormation.MaxUnitCount = 30;
    
    // 生成楔形陣型位置
    for (int32 i = 0; i < 30; i++)
    {
        float Angle = (i % 3 - 1) * 30.0f; // -30, 0, 30 度
        float Distance = (i / 3) * 100.0f;
        FVector Position = FVector(
            Distance * FMath::Cos(FMath::DegreesToRadians(Angle)),
            Distance * FMath::Sin(FMath::DegreesToRadians(Angle)),
            0.0f
        );
        WedgeFormation.FormationPositions.Add(Position);
    }
    
    FormationDatabase.Add(EMingFormationType::Wedge, WedgeFormation);
    
    // 圓形陣型
    FMingTacticalFormation CircleFormation;
    CircleFormation.FormationID = 3;
    CircleFormation.FormationName = TEXT("圓形陣型");
    CircleFormation.FormationType = EMingFormationType::Circle;
    CircleFormation.FormationDescription = TEXT("單位排成圓形，適合防禦");
    CircleFormation.FormationBonuses = {
        {TEXT("defense"), 20.0f},
        {TEXT("cohesion"), 15.0f}
    };
    CircleFormation.FormationPenalties = {
        {TEXT("mobility"), -15.0f},
        {TEXT("attack"), -5.0f}
    };
    CircleFormation.ApplicableUnitTypes = {EMingUnitType::Infantry, EMingUnitType::Artillery};
    CircleFormation.MinUnitCount = 4;
    CircleFormation.MaxUnitCount = 40;
    
    // 生成圓形陣型位置
    for (int32 i = 0; i < 40; i++)
    {
        float Angle = (i / 40.0f) * 360.0f;
        float Radius = 100.0f;
        FVector Position = FVector(
            Radius * FMath::Cos(FMath::DegreesToRadians(Angle)),
            Radius * FMath::Sin(FMath::DegreesToRadians(Angle)),
            0.0f
        );
        CircleFormation.FormationPositions.Add(Position);
    }
    
    FormationDatabase.Add(EMingFormationType::Circle, CircleFormation);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設編隊"), FormationDatabase.Num());
}

float UMingTacticalCombatSystem::CalculateCombatDamage(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender)
{
    // 基礎傷害計算
    float BaseDamage = Attacker.CombatStats.AttackPower;
    
    // 考慮防禦者的防禦力
    float DefenseReduction = Defender.CombatStats.DefensePower * 0.5f;
    
    // 考慮防禦者的護甲
    float ArmorReduction = Defender.CombatStats.Armor * BaseDamage * 0.3f;
    
    // 考慮攻擊者的經驗加成
    float ExperienceBonus = Attacker.CombatStats.Experience * 0.1f;
    
    // 考慮士氣影響
    float MoraleFactor = (Attacker.CombatStats.Morale + Defender.CombatStats.Morale) / 200.0f;
    
    // 最終傷害計算
    float FinalDamage = (BaseDamage - DefenseReduction - ArmorReduction + ExperienceBonus) * MoraleFactor;
    
    // 確保傷害不為負值
    return FMath::Max(0.0f, FinalDamage);
}

float UMingTacticalCombatSystem::CalculateHitChance(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender)
{
    // 基礎命中概率
    float BaseHitChance = Attacker.CombatStats.Accuracy;
    
    // 距離影響
    float Distance = FVector::Dist(Attacker.CurrentPosition, Defender.CurrentPosition);
    float MaxRange = Attacker.CombatStats.AttackRange;
    float DistanceFactor = 1.0f - (Distance / MaxRange) * 0.5f;
    
    // 目標閃避影響
    float EvasionFactor = 1.0f - Defender.CombatStats.Evasion;
    
    // 士氣影響
    float MoraleFactor = (Attacker.CombatStats.Morale / 100.0f);
    
    // 最終命中概率
    float FinalHitChance = BaseHitChance * DistanceFactor * EvasionFactor * MoraleFactor;
    
    return FMath::Clamp(FinalHitChance, 0.0f, 1.0f);
}

float UMingTacticalCombatSystem::CalculateCriticalChance(const FMingCombatUnit& Attacker)
{
    // 基礎暴擊概率
    float BaseCriticalChance = Attacker.CombatStats.CriticalChance;
    
    // 經驗加成
    float ExperienceBonus = Attacker.CombatStats.Experience * 0.001f;
    
    // 士氣影響
    float MoraleFactor = (Attacker.CombatStats.Morale / 100.0f);
    
    // 最終暴擊概率
    float FinalCriticalChance = (BaseCriticalChance + ExperienceBonus) * MoraleFactor;
    
    return FMath::Clamp(FinalCriticalChance, 0.0f, 1.0f);
}

void UMingTacticalCombatSystem::UpdateUnitState(int32 UnitID, EMingCombatState NewState)
{
    if (UnitDatabase.Contains(UnitID))
    {
        FMingCombatUnit& Unit = UnitDatabase[UnitID];
        EMingCombatState OldState = Unit.CurrentState;
        Unit.CurrentState = NewState;
        
        // 觸發狀態變化事件
        OnUnitStateChanged.Broadcast(Unit);
        
        UE_LOG(LogTemp, Log, TEXT("單位 %s 狀態更新: %d -> %d"), 
            *Unit.UnitName, (int32)OldState, (int32)NewState);
    }
}

void UMingTacticalCombatSystem::ProcessUnitMovement(float DeltaTime)
{
    for (TPair<int32, FMingCombatUnit>& Pair : UnitDatabase)
    {
        int32 UnitID = Pair.Key;
        FMingCombatUnit& Unit = Pair.Value;
        
        if (Unit.CurrentState == EMingCombatState::Moving)
        {
            // 計算移動方向
            FVector Direction = Unit.TargetPosition - Unit.CurrentPosition;
            float Distance = Direction.Size();
            
            if (Distance > 10.0f) // 還未到達目標
            {
                Direction.Normalize();
                FVector Movement = Direction * Unit.CombatStats.MovementSpeed * DeltaTime;
                
                // 更新位置
                Unit.CurrentPosition += Movement;
                
                UE_LOG(LogTemp, VeryVerbose, TEXT("單位 %s 移動到 (%.1f, %.1f, %.1f)"), 
                    *Unit.UnitName, Unit.CurrentPosition.X, Unit.CurrentPosition.Y, Unit.CurrentPosition.Z);
            }
            else
            {
                // 到達目標位置
                Unit.CurrentPosition = Unit.TargetPosition;
                UpdateUnitState(UnitID, EMingCombatState::Idle);
                
                UE_LOG(LogTemp, Log, TEXT("單位 %s 到達目標位置"), *Unit.UnitName);
            }
        }
    }
}

void UMingTacticalCombatSystem::ProcessCombatLogic(float DeltaTime)
{
    // 處理戰鬥邏輯
    // 這裡可以添加更複雜的戰的戰鬥AI邏輯
    // 例如：自動攻擊範圍內的敵人、尋找掩護、協同攻擊等
}

void UMingTacticalCombatSystem::UpdateUnitExperience(int32 UnitID, float ExperienceAmount)
{
    if (UnitDatabase.Contains(UnitID))
    {
        FMingCombatUnit& Unit = UnitDatabase[UnitID];
        Unit.CombatStats.Experience += ExperienceAmount;
        
        // 檢查是否升級
        if (Unit.CombatStats.Experience >= 100.0f)
        {
            Unit.CombatStats.Experience = 0.0f;
            Unit.CombatStats.Level += 1;
            
            // 升級加成
            Unit.CombatStats.MaxHealth += 10.0f;
            Unit.CombatStats.AttackPower += 2.0f;
            Unit.CombatStats.DefensePower += 1.5f;
            
            UE_LOG(LogTemp, Log, TEXT("單位 %s 升級到等級 %d"), *Unit.UnitName, Unit.CombatStats.Level);
        }
    }
}

bool UMingTacticalCombatSystem::IsUnitVisible(int32 ViewerID, int32 TargetID) const
{
    if (!UnitDatabase.Contains(ViewerID) || !UnitDatabase.Contains(TargetID))
    {
        return false;
    }

    const FMingCombatUnit& Viewer = UnitDatabase[ViewerID];
    const FMingCombatUnit& Target = UnitDatabase[TargetID];

    // 計算距離
    float Distance = FVector::Dist(Viewer.CurrentPosition, Target.CurrentPosition);
    
    // 檢查是否在視野範圍內
    if (Distance <= Viewer.CombatStats.VisionRange)
    {
        // 簡單的可見性檢查（可以添加地形、遮蔽等複雜因素）
        return true;
    }

    return false;
}

TArray<FVector> UMingTacticalCombatSystem::GenerateFormationPositions(EMingFormationType FormationType, const FVector& Center, int32 UnitCount) const
{
    TArray<FVector> Positions;
    
    if (!FormationDatabase.Contains(FormationType))
    {
        // 默認線形排列
        for (int32 i = 0; i < UnitCount; i++)
        {
            FVector Position = Center + FVector(i * 100.0f - (UnitCount - 1) * 50.0f, 0.0f, 0.0f);
            Positions.Add(Position);
        }
        return Positions;
    }

    const FMingTacticalFormation& Formation = FormationDatabase[FormationType];
    
    // 使用預定義的編隊位置
    for (int32 i = 0; i < UnitCount && i < Formation.FormationPositions.Num(); i++)
    {
        FVector Position = Center + Formation.FormationPositions[i];
        Positions.Add(Position);
    }
    
    return Positions;
}

bool UMingTacticalCombatSystem::ValidateUnitData(const FMingCombatUnit& Unit) const
{
    if (Unit.UnitID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("單位ID必須大於0"));
        return false;
    }
    
    if (Unit.UnitName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("單位名稱不能為空"));
        return false;
    }
    
    if (Unit.CombatStats.MaxHealth <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("單位最大生命值必須大於0"));
        return false;
    }
    
    return true;
}
