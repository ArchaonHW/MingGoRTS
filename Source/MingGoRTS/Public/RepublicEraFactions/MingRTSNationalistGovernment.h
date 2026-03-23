#pragma once


#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSNationalistGovernment.generated.h"

/**
 * ?‹æ??¿å??¢å?
 * æ°‘æ?ä¸»ç¾©?ç¾ä»???çµ±ä¸€ä¸­å?N?†æƒ³
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSNationalistGovernment : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSNationalistGovernment(};

    // ?å??–å?æ°‘æ”¿åº?
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
    // ?‹æ??¿å??¹è‰²æ©Ÿåˆ¶
    UPROPERTY()
    TArray<FFactionMechanic> NationalistMechanics;

    // ?‹æ??¿å?å°ˆå±¬?®ä?
    UPROPERTY()
    TArray<FFactionUnit> NationalistUnits;
};

