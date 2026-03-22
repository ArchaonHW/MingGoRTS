#pragma once

#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSBeiyangGovernment.generated.h"

/**
 * 北洋政府勢力
 * 中央正統、外交優勢、軍事強大但內部不穩定
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSBeiyangGovernment : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSBeiyangGovernment();

    // 初始化北洋政府
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
    // 北洋政府特色機制
    UPROPERTY()
    TArray<FFactionMechanic> BeiyangMechanics;

    // 北洋政府專屬單位
    UPROPERTY()
    TArray<FFactionUnit> BeiyangUnits;
};
