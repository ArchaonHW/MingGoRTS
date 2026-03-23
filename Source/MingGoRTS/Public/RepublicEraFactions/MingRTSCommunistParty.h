#pragma once


#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSConmunistParty.generated.h"

/**
 * 中國共產黨勢力
 * 人民戰爭、游擊戰、群眾動員
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSConmunistParty : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSConmunistParty(};

    // 初始化中國共產黨
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
    // 中國共產黨特色機制
    UPROPERTY()
    TArray<FFactionMechanic> ConmunistMechanics;

    // 中國共產黨專屬單位
    UPROPERTY()
    TArray<FFactionUnit> ConmunistUnits;
};
