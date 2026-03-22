#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSFactionData.h"
#include "MingRTSFactionManager.generated.h"

/**
 * 民國史詩勢力管理器
 * 管理12個歷史勢力的創建、平衡和交互
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSFactionManager : public UObject
{
    GENERATED_BODY()

public:
    // 建構函數
    UMingRTSFactionManager();

    // 初始化勢力管理器
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    bool InitializeFactionManager();

    // 創建指定勢力
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    class UMingRTSFactionBase* CreateFaction(ERepublicEraFaction FactionType);

    // 獲取所有勢力
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    TArray<class UMingRTSFactionBase*> GetAllFactions();

    // 獲取指定勢力
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    class UMingRTSFactionBase* GetFaction(ERepublicEraFaction FactionType);

    // 平衡所有勢力
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    bool BalanceAllFactions();

    // 計算勢力強度
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    float CalculateFactionPower(ERepublicEraFaction FactionType);

    // 獲取勢力關係
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    EFactionRelation GetFactionRelation(ERepublicEraFaction FactionA, ERepublicEraFaction FactionB);

    // 設置勢力關係
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    void SetFactionRelation(ERepublicEraFaction FactionA, ERepublicEraFaction FactionB, EFactionRelation Relation);

protected:
    // 已創建的勢力映射
    UPROPERTY()
    TMap<ERepublicEraFaction, class UMingRTSFactionBase*> Factions;

    // 勢力關係映射
    UPROPERTY()
    TMap<TPair<ERepublicEraFaction, ERepublicEraFaction>, EFactionRelation> FactionRelations;

    // 初始化所有勢力
    void InitializeAllFactions();

    // 創建北洋政府
    class UMingRTSFactionBase* CreateBeiyangGovernment();

    // 創建國民政府
    class UMingRTSFactionBase* CreateNationalistGovernment();

    // 創建中國共產黨
    class UMingRTSFactionBase* CreateCommunistParty();

    // 創建奉系軍閥
    class UMingRTSFactionBase* CreateFengtianClique();

    // 創建直系軍閥
    class UMingRTSFactionBase* CreateZhiliClique();

    // 創建皖系軍閥
    class UMingRTSFactionBase* CreateAnhuiClique();

    // 創建晉系軍閥
    class UMingRTSFactionBase* CreateShanxiClique();

    // 創建桂系軍閥
    class UMingRTSFactionBase* CreateGuangxiClique();

    // 創建滇系軍閥
    class UMingRTSFactionBase* CreateYunnanClique();

    // 創建川系軍閥
    class UMingRTSFactionBase* CreateSichuanClique();

    // 創建馬家軍
    class UMingRTSFactionBase* CreateMaClique();

    // 創建新疆勢力
    class UMingRTSFactionBase* CreateXinjiangForces();

private:
    // 勢力管理器是否已初始化
    bool bIsInitialized;
};
