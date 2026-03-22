#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingTacticalCombatSystem.generated.h"

// 單位類型枚舉
UENUM(BlueprintType)
enum class EMingUnitType : uint8
{
    Infantry,          // 步兵
    Cavalry,           // 騎兵
    Artillery,         // 砲兵
    Tank,              // 坦克
    Aircraft,          // 飛機
    Naval,             // 海軍
    Support,           // 支援單位
    Commander          // 指揮官
};

// 戰術編隊類型
UENUM(BlueprintType)
enum class EMingTacticalFormationType : uint8
{
    Line,              // 線形陣型
    Column,            // 縱隊陣型
    Wedge,             // 楔形陣型
    Diamond,           // 菱形陣型
    Circle,            // 圓形陣型
    Skirmish,          // 散兵陣型
    Square,            // 方形陣型
    Arrow,             // 箭頭陣型
    Crescent,          // 新月陣型
    Custom             // 自定義陣型
};

// 戰鬥狀態
UENUM(BlueprintType)
enum class EMingCombatState : uint8
{
    Idle,              // 空閒
    Moving,            // 移動中
    Attacking,          // 攻擊中
    Defending,         // 防禦中
    Retreating,         // 撤退中
    Routed,            // 潰敗
    Captured,          // 被俘虜
    Destroyed          // 被摧毀
};

// 單位戰鬥屬性
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingUnitCombatStats
{
    GENERATED_BODY()

    // 基礎屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefensePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionRange;

    // 戰術屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Morale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Discipline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Leadership;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Experience;

    // 特殊屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Accuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Evasion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Armor;

    FMingUnitCombatStats()
    {
        Health = 100.0f;
        MaxHealth = 100.0f;
        AttackPower = 20.0f;
        DefensePower = 15.0f;
        MovementSpeed = 5.0f;
        AttackRange = 2.0f;
        VisionRange = 8.0f;
        Morale = 75.0f;
        Discipline = 70.0f;
        Leadership = 50.0f;
        Experience = 0.0f;
        Accuracy = 0.7f;
        CriticalChance = 0.1f;
        Evasion = 0.05f;
        Armor = 0.2f;
    }
};

// 戰術陣型數據
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalFormation
{
    GENERATED_BODY()

    // 陣型ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // 陣型名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationName;

    // 陣型類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // 陣型描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationDescription;

    // 陣型位置數組
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> FormationPositions;

    // 陣型加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationBonuses;

    // 陣型減益
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationPenalties;

    // 適用單位類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingUnitType> ApplicableUnitTypes;

    // 最小單位數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinUnitCount;

    // 最大單位數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUnitCount;

    FMingTacticalFormation()
    {
        FormationID = -1;
        FormationName = TEXT("");
        FormationType = EMingFormationType::Line;
        FormationDescription = TEXT("");
        FormationPositions.Empty();
        FormationBonuses.Empty();
        FormationPenalties.Empty();
        ApplicableUnitTypes.Empty();
        MinUnitCount = 1;
        MaxUnitCount = 100;
    }
};

// 戰鬥單位數據
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingCombatUnit
{
    GENERATED_BODY()

    // 單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // 單位名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnitName;

    // 單位類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUnitType UnitType;

    // 當前位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentPosition;

    // 目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPosition;

    // 當前狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingCombatState CurrentState;

    // 戰鬥屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMingUnitCombatStats CombatStats;

    // 所屬編隊ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // 編隊內位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationPosition;

    // 指揮官ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommanderID;

    // 敵對單位列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EnemyUnits;

    // 友軍單位列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> FriendlyUnits;

    // 是否被選中
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSelected;

    // 是否可見
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisible;

    // 戰爭迷霧等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FogOfWarLevel;

    FMingCombatUnit()
    {
        UnitID = -1;
        UnitName = TEXT("");
        UnitType = EMingUnitType::Infantry;
        CurrentPosition = FVector::ZeroVector;
        TargetPosition = FVector::ZeroVector;
        CurrentState = EMingCombatState::Idle;
        FormationID = -1;
        FormationPosition = -1;
        CommanderID = -1;
        EnemyUnits.Empty();
        FriendlyUnits.Empty();
        bIsSelected = false;
        bIsVisible = true;
        FogOfWarLevel = 0;
    }
};

// 戰術命令
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalCommand
{
    GENERATED_BODY()

    // 命令ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandID;

    // 命令類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CommandType;

    // 目標單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;

    // 目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // 命令參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CommandParameters;

    // 命令優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority;

    // 命令執行時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // 是否為持續命令
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsContinuous;

    FMingTacticalCommand()
    {
        CommandID = -1;
        CommandType = TEXT("");
        TargetUnitID = -1;
        TargetLocation = FVector::ZeroVector;
        CommandParameters.Empty();
        Priority = 1;
        ExecutionTime = 0.0f;
        bIsContinuous = false;
    }
};

// 戰鬥結果
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingCombatResult
{
    GENERATED_BODY()

    // 攻擊單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackerID;

    // 防禦單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefenderID;

    // 傷害數值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageDealt;

    // 是否暴擊
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCriticalHit;

    // 是否命中
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHit;

    // 戰鬥時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CombatTime;

    // 戰鬥位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CombatLocation;

    // 戰鬥類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CombatType;

    // 經驗值獲得
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceGained;

    FMingCombatResult()
    {
        AttackerID = -1;
        DefenderID = -1;
        DamageDealt = 0.0f;
        bIsCriticalHit = false;
        bIsHit = false;
        CombatTime = FDateTime::Now();
        CombatLocation = FVector::ZeroVector;
        CombatType = TEXT("");
        ExperienceGained = 0.0f;
    }
};

// 戰術戰鬥事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitStateChanged, const FMingCombatUnit&, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatOccurred, const FMingCombatResult&, Result, const TArray<FMingCombatUnit>&, InvolvedUnits);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFormationChanged, int32, FormationID, EMingFormationType, NewFormationType, const TArray<FMingCombatUnit>&, Units);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTacticalCommandIssued, const FMingTacticalCommand&, Command);

/**
 * 戰術戰鬥系統
 * 負責管理大規模單位控制和戰術戰鬥
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingTacticalCombatSystem : public AActor
{
    GENERATED_BODY()

public:
    UMingTacticalCombatSystem();

    // 初始化戰術戰鬥系統
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool InitializeTacticalSystem();

    // 註冊戰鬥單位
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool RegisterCombatUnit(const FMingCombatUnit& Unit);

    // 移除戰鬥單位
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool RemoveCombatUnit(int32 UnitID);

    // 獲取單位信息
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    FMingCombatUnit GetUnitInfo(int32 UnitID) const;

    // 選擇單位
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SelectUnits(const TArray<int32>& UnitIDs);

    // 取消選擇單位
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool DeselectUnits(const TArray<int32>& UnitIDs);

    // 獲取已選擇單位
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TArray<int32> GetSelectedUnits() const;

    // 移動單位
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool MoveUnits(const TArray<int32>& UnitIDs, const FVector& TargetLocation);

    // 攻擊目標
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool AttackTarget(int32 AttackerID, int32 TargetID);

    // 設置編隊
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SetFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType);

    // 獲取編隊信息
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    FMingTacticalFormation GetFormationInfo(EMingFormationType FormationType) const;

    // 發布戰術命令
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool IssueTacticalCommand(const FMingTacticalCommand& Command);

    // 處理戰鬥
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    FMingCombatResult ProcessCombat(int32 AttackerID, int32 DefenderID);

    // 更新戰爭迷霧
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    void UpdateFogOfWar();

    // 獲取可見單位
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TArray<int32> GetVisibleUnits(int32 ViewerUnitID) const;

    // 獲取戰鬥統計
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TMap<EMingUnitType, int32> GetCombatStatistics() const;

    // 保存戰鬥數據
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SaveCombatData();

    // 載入戰鬥數據
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool LoadCombatData();

    // 清除所有單位
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    void ClearAllUnits();

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnUnitStateChanged OnUnitStateChanged;

    UPROPERTY(BlueprintAssignable)
    FOnCombatOccurred OnCombatOccurred;

    UPROPERTY(BlueprintAssignable)
    FOnFormationChanged OnFormationChanged;

    UPROPERTY(BlueprintAssignable)
    FOnTacticalCommandIssued OnTacticalCommandIssued;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // 單位數據庫
    UPROPERTY()
    TMap<int32, FMingCombatUnit> UnitDatabase;

    // 編隊數據庫
    UPROPERTY()
    TMap<EMingFormationType, FMingTacticalFormation> FormationDatabase;

    // 已選擇單位列表
    UPROPERTY()
    TArray<int32> SelectedUnits;

    // 戰鬥歷史記錄
    UPROPERTY()
    TArray<FMingCombatResult> CombatHistory;

    // 戰爭迷霧數據
    UPROPERTY()
    TMap<int32, TArray<int32>> VisibilityMap;

    // 當前編隊
    UPROPERTY()
    TMap<int32, EMingFormationType> UnitFormations;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設編隊
    void LoadDefaultFormations();

    // 計算戰鬥傷害
    float CalculateCombatDamage(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender);

    // 計算命中概率
    float CalculateHitChance(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender);

    // 計算暴擊概率
    float CalculateCriticalChance(const FMingCombatUnit& Attacker);

    // 更新單位狀態
    void UpdateUnitState(int32 UnitID, EMingCombatState NewState);

    // 處理單位移動
    void ProcessUnitMovement(float DeltaTime);

    // 處理戰鬥邏輯
    void ProcessCombatLogic(float DeltaTime);

    // 更新單位經驗
    void UpdateUnitExperience(int32 UnitID, float ExperienceAmount);

    // 檢查單位視野
    bool IsUnitVisible(int32 ViewerID, int32 TargetID) const;

    // 生成編隊位置
    TArray<FVector> GenerateFormationPositions(EMingFormationType FormationType, const FVector& Center, int32 UnitCount) const;

    // 驗證單位數據
    bool ValidateUnitData(const FMingCombatUnit& Unit) const;
};
