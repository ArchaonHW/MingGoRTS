#pragma once


#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSBeiyangGovernment.generated.h"

/**
 * ?—æ??¿å??¢å?
 * ä¸­å¤®vçµ±ã€å?äº¤å„ª?¢ã€Fäº‹å¼·jä½†å…§?¨ä?ç©©å?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSBeiyangGovernment : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSBeiyangGovernment(};

    // ?å??–å?æ´‹æ”¿åº?
    virtual void InitializeFaction() override;

    // ?²å??¢å???
    virtual FFactionAttributes GetFactionAttributes() const override;

    // ?²å?å°ˆå±¬?®ä?
    virtual TArray<FFactionUnit> GetUniqueUnits() const override;

    // ?²å??¹è‰²æ©Ÿåˆ¶
    virtual TArray<FFactionMechanic> GetUniqueMechanics() const override;

    // ?²å??Šæˆ²?‡å?
    virtual FGameplayGuide GetGameplayGuide() const override;

protected:
    // ?å????
    virtual void InitializeAttributes() override;

    // ?å??–å?å±¬å–®ä½?
    virtual void InitializeUniqueUnits() override;

    // ?å??–ç‰¹?²æ???
    virtual void InitializeUniqueMechanics() override;

    // ?å??–é??²æ???
    virtual void InitializeGameplayGuide() override;

private:
    // ?—æ??¿å??¹è‰²æ©Ÿåˆ¶
    UPROPERTY()
    TArray<FFactionMechanic> BeiyangMechanics;

    // ?—æ??¿å?å°ˆå±¬?®ä?
    UPROPERTY()
    TArray<FFactionUnit> BeiyangUnits;
};

