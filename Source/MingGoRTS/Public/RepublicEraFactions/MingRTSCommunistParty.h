#pragma once

#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSCommunistParty.generated.h"

/**
 * 中國共產黨勢力
 * 人民戰爭、游擊戰、群眾動員
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSCommunistParty : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSCommunistParty();

    // 初始化中國共產黨
    virtual void InitializeFaction() override;

    // 獲取勢力屬性
    virtual FFactionAttributes GetFactionAttributes() const override;

    // 獲取專屬單位
    virtual TArray<FFactionUnit> GetUniqueUnits() const override;

    // 獲取特色機制
    virtual TArray<FFactionMechanic> GetUniqueMechanics() const override;

    // 獲取遊戲指南
    virtual FGameplayGuide GetGameplayGuide() const override;

protected:
    // 初始化屬性
    virtual void InitializeAttributes() override;

    // 初始化專屬單位
    virtual void InitializeUniqueUnits() override;

    // 初始化特色機制
    virtual void InitializeUniqueMechanics() override;

    // 初始化遊戲指南
    virtual void InitializeGameplayGuide() override;

private:
    // 中國共產黨特色機制
    UPROPERTY()
    TArray<FFactionMechanic> CommunistMechanics;

    // 中國共產黨專屬單位
    UPROPERTY()
    TArray<FFactionUnit> CommunistUnits;
};
