#pragma once


#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSNationalistGovernment.generated.h"

/**
 * 國民政府勢力
 * 民族主義、現代化、統一中國N理想
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSNationalistGovernment : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSNationalistGovernment(};

    // 初始化國民政府
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
    // 國民政府特色機制
    UPROPERTY()
    TArray<FFactionMechanic> NationalistMechanics;

    // 國民政府專屬單位
    UPROPERTY()
    TArray<FFactionUnit> NationalistUnits;
};
