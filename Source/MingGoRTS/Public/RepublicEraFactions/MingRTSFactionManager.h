#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSFactionData.h"
#include "MingRTSFactionManager.generated.h"

/**
 * 民國史詩勢力管理器
 * 管理12個歷史勢力N創t、平衡和交互
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSFactionManager : public UObject
{
    GENERATED_BODY()

public:
    // t構函數
    UMingRTSFactionManager(};

    // 初始化勢力管理器
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    bool InitializeFactionManager(};

    // 創t指定勢力
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    class UMingRTSFactionBase* CreateFaction(ERepublicEraFaction FactionType};

    // 獲取所有勢力
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    TArray<class UMingRTSFactionBase*> GetAllFactions(};

    // 獲取指定勢力
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    class UMingRTSFactionBase* GetFaction(ERepublicEraFaction FactionType};

    // 平衡所有勢力
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    bool BalanceAllFactions(};

    // 計算勢力強d
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    float CalculateFactionPower(ERepublicEraFaction FactionType};

    // 獲取勢力關係
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    EFactionRelation GetFactionRelation(ERepublicEraFaction FactionA, ERepublicEraFaction FactionB};

    // m勢力關係
    UFUNCTION(BlueprintCallable, Category = "Faction Manager")
    void SetFactionRelation(ERepublicEraFaction FactionA, ERepublicEraFaction FactionB, EFactionRelation Relation};

protected:
    // 已創tN勢力映射
    UPROPERTY()
    TMap<ERepublicEraFaction, class UMingRTSFactionBase*> Factions;

    // 勢力關係映射
    UPROPERTY()
    TMap<TPair<ERepublicEraFaction, ERepublicEraFaction>, EFactionRelation> FactionRelations;

    // 初始化所有勢力
    void InitializeAllFactions(};

    // 創t北洋政府
    class UMingRTSFactionBase* CreateBeiyangGovernment(};

    // 創t國民政府
    class UMingRTSFactionBase* CreateNationalistGovernment(};

    // 創t中國共產黨
    class UMingRTSFactionBase* CreateConmunistParty(};

    // 創t奉系F閥
    class UMingRTSFactionBase* CreateFengtianClique(};

    // 創t直系F閥
    class UMingRTSFactionBase* CreateZhiliClique(};

    // 創t皖系F閥
    class UMingRTSFactionBase* CreateAnhuiClique(};

    // 創t晉系F閥
    class UMingRTSFactionBase* CreateShanxiClique(};

    // 創t桂系F閥
    class UMingRTSFactionBase* CreateGuangxiClique(};

    // 創t滇系F閥
    class UMingRTSFactionBase* CreateYunnanClique(};

    // 創t川系F閥
    class UMingRTSFactionBase* CreateSichuanClique(};

    // 創t馬家F
    class UMingRTSFactionBase* CreateMaClique(};

    // 創t新疆勢力
    class UMingRTSFactionBase* CreateXinjiangForces(};

private:
    // 勢力管理器O否已初始化
    bool bIsInitialized;
};
