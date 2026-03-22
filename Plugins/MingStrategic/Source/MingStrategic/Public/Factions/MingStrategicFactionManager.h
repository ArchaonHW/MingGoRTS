#pragma once

#include "CoreMinimal.h"
#include "Factions/MingRTSFactionData.h"
#include "MingStrategicFactionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFactionRelationChanged, EFactionType, FactionA, EFactionType, FactionB);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFactionDefeated, EFactionType, DefeatedFaction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFactionVictoryPointsChanged, EFactionType, Faction, int32, NewVictoryPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerFactionSelected, EFactionType, PlayerFaction);

/**
 * 勢力管理器 - 管理12個可玩勢力
 */
UCLASS(ClassGroup = (MingStrategic), BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingRTSFactionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSFactionManager();
    virtual void BeginDestroy() override;

    // 初始化/關閉
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|System")
    void InitializeFactionManager();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|System")
    void ShutdownFactionManager();

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|System")
    bool IsInitialized() const { return bInitialized; }

    // 勢力數據管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Data")
    void RegisterFaction(const FFactionData& FactionData);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Data")
    void UnregisterFaction(EFactionType FactionType);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Data")
    bool IsFactionRegistered(EFactionType FactionType) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Data")
    FFactionData GetFactionData(EFactionType FactionType) const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Data")
    void UpdateFactionData(EFactionType FactionType, const FFactionData& NewData);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Data")
    TArray<EFactionType> GetAllRegisteredFactions() const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Data")
    TArray<EFactionType> GetActiveFactions() const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Data")
    TArray<FFactionData> GetAllFactionData() const;

    // 勢力關係管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Relations")
    void SetFactionRelation(EFactionType FactionA, EFactionType FactionB, EFactionRelationType RelationType);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Relations")
    EFactionRelationType GetFactionRelation(EFactionType FactionA, EFactionType FactionB) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Relations")
    float GetFactionRelationValue(EFactionType FactionA, EFactionType FactionB) const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Relations")
    void ModifyFactionRelationValue(EFactionType FactionA, EFactionType FactionB, float Delta);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Relations")
    void DeclareWar(EFactionType Aggressor, EFactionType Target);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Relations")
    void MakePeace(EFactionType FactionA, EFactionType FactionB);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Relations")
    void FormAlliance(EFactionType FactionA, EFactionType FactionB);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Relations")
    void BreakAlliance(EFactionType FactionA, EFactionType FactionB);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Relations")
    void SignNonAggressionPact(EFactionType FactionA, EFactionType FactionB, int32 DurationTurns);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Relations")
    TArray<EFactionType> GetAllies(EFactionType Faction) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Relations")
    TArray<EFactionType> GetEnemies(EFactionType Faction) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Relations")
    TArray<EFactionType> GetNeighbors(EFactionType Faction) const;

    // 玩家勢力管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Player")
    void SetPlayerFaction(EFactionType FactionType);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Player")
    EFactionType GetPlayerFaction() const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Player")
    bool IsPlayerFaction(EFactionType FactionType) const;

    // 勢力狀態管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|State")
    void SetFactionState(EFactionType FactionType, EFactionState NewState);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|State")
    EFactionState GetFactionState(EFactionType FactionType) const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|State")
    void DefeatFaction(EFactionType FactionType);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|State")
    void ReviveFaction(EFactionType FactionType);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|State")
    bool IsFactionAlive(EFactionType FactionType) const;

    // 勝利點數管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Victory")
    void AddVictoryPoints(EFactionType FactionType, int32 Points);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Victory")
    void SetVictoryPoints(EFactionType FactionType, int32 Points);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Victory")
    int32 GetVictoryPoints(EFactionType FactionType) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Victory")
    EFactionType GetLeadingFaction() const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Victory")
    TArray<EFactionType> GetFactionRanking() const;

    // 領土管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Territory")
    void AddTerritory(EFactionType FactionType, int32 Count = 1);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Territory")
    void RemoveTerritory(EFactionType FactionType, int32 Count = 1);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Territory")
    int32 GetTerritoryCount(EFactionType FactionType) const;

    // 軍事/經濟力量
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Power")
    void UpdateMilitaryStrength(EFactionType FactionType, int32 NewStrength);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Power")
    void UpdateEconomicOutput(EFactionType FactionType, int32 NewOutput);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Power")
    int32 GetMilitaryStrength(EFactionType FactionType) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Power")
    int32 GetEconomicOutput(EFactionType FactionType) const;

    // 勢力AI配置
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|AI")
    void SetFactionAIStrategy(EFactionType FactionType, EFactionAIStrategy Strategy);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|AI")
    EFactionAIStrategy GetFactionAIStrategy(EFactionType FactionType) const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|AI")
    void SetFactionAIAdaptive(EFactionType FactionType, bool bAdaptive);

    // 回合更新
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Turn")
    void OnTurnStart(int32 TurnNumber);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Turn")
    void OnTurnEnd(int32 TurnNumber);

    // 獲取勢力配置數據
    UFUNCTION(BlueprintPure, Category = "MingRTS|Faction|Config")
    static FFactionData GetDefaultFactionData(EFactionType FactionType);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Config")
    void InitializeAllDefaultFactions();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Faction|Events")
    FOnFactionRelationChanged OnFactionRelationChanged;

    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Faction|Events")
    FOnFactionDefeated OnFactionDefeated;

    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Faction|Events")
    FOnFactionVictoryPointsChanged OnFactionVictoryPointsChanged;

    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Faction|Events")
    FOnPlayerFactionSelected OnPlayerFactionSelected;

private:
    UPROPERTY()
    TMap<EFactionType, FFactionData> FactionRegistry;

    UPROPERTY()
    EFactionType PlayerFaction;

    UPROPERTY()
    bool bInitialized;

    UPROPERTY()
    int32 CurrentTurn;

    // 初始化各勢力默認數據
    void InitializeBeiyangGovernment();
    void InitializeNationalistGovernment();
    void InitializeChineseCommunistParty();
    void InitializeFengtianClique();
    void InitializeZhiliClique();
    void InitializeAnhuiClique();
    void InitializeJinClique();
    void InitializeGuangxiClique();
    void InitializeYunnanClique();
    void InitializeSichuanClique();
    void InitializeMaFamily();
    void InitializeXinjiangFaction();

    // 初始化勢力關係
    void InitializeFactionRelations();

    // 驗證函數
    bool IsValidFaction(EFactionType FactionType) const;
    bool AreFactionsValid(EFactionType FactionA, EFactionType FactionB) const;

    // 輔助函數
    FFactionRelationData* GetRelationData(EFactionType FactionA, EFactionType FactionB);
    const FFactionRelationData* GetRelationData(EFactionType FactionA, EFactionType FactionB) const;
    void UpdateRelationForBoth(EFactionType FactionA, EFactionType FactionB, const FFactionRelationData& Data);
    EFactionRelationType DetermineRelationTypeFromValue(float Value) const;
};
