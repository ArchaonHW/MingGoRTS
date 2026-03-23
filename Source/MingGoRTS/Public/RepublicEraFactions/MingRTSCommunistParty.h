#pragma once


#include "CoreMinimal.h"
#include "MingRTSFactionBase.h"
#include "MingRTSConmunistParty.generated.h"

/**
 * ä¸­å??±ç”¢é»¨å‹¢??
 * äººæ??°çˆ­?æ¸¸?Šæˆ°?ç¾¤?¾å???
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSConmunistParty : public UMingRTSFactionBase
{
    GENERATED_BODY()

public:
    UMingRTSConmunistParty(};

    // ?å??–ä¸­?‹å…±?¢é»¨
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
    // ä¸­å??±ç”¢é»¨ç‰¹?²æ???
    UPROPERTY()
    TArray<FFactionMechanic> ConmunistMechanics;

    // ä¸­å??±ç”¢é»¨å?å±¬å–®ä½?
    UPROPERTY()
    TArray<FFactionUnit> ConmunistUnits;
};

