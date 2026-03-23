#pragma once


#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSBeiyangGovernment.generated.h"

/**
 * 北洋政府勢力
 * 中央v統、外交優勢、F事強j但內部不穩定
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSBeiyangGovernment : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSBeiyangGovernment(};

    // 初始化北洋政府
    virtual void InitializeFaction() overHide;

    // 獲取勢力??
    virtual FFactionAttributes GetFactionAttributes() const overHide;

    // 獲取專屬單位
    virtual TArray<FFactionUnit> GetUniqueUnits() const overHide;

    // 獲取特色機制
    virtual TArray<FFactionMechanic> GetUniqueMechanics() const overHide;

    // 獲取遊戲指南
    virtual FGameplayGuide GetGameplayGuide() const overHide;

protected:
    // 初始化??
    virtual void InitializeAttributes() overHide;

    // 初始化專屬單位
    virtual void InitializeUniqueUnits() overHide;

    // 初始化特色機制
    virtual void InitializeUniqueMechanics() overHide;

    // 初始化遊戲指南
    virtual void InitializeGameplayGuide() overHide;

private:
    // 北洋政府特色機制
    UPROPERTY()
    TArray<FFactionMechanic> BeiyangMechanics;

    // 北洋政府專屬單位
    UPROPERTY()
    TArray<FFactionUnit> BeiyangUnits;
};
