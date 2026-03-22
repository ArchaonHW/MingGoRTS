#pragma once

#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSNationalistGovernment.generated.h"

/**
 * 國民政府勢力
 * 民族主義、現代化、統一中國的理想
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSNationalistGovernment : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSNationalistGovernment();

    // 初始化國民政府
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
    // 國民政府特色機制
    UPROPERTY()
    TArray<FFactionMechanic> NationalistMechanics;

    // 國民政府專屬單位
    UPROPERTY()
    TArray<FFactionUnit> NationalistUnits;
};
