#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStrategicManager.generated.h"

class IMingCoreEventBus;

/**
 * ?∞Áï•Â±§ÁÆ°?ÜÂô®
 * ÁÆ°Á??®Â??∞Áï•Ê±∫Á??åÂ?‰∫§È?‰ø? */
UCLASS()
class MINGSTRATEGIC_API UMingStrategicManager : public UObject
{
    GENERATED_BODY()

public:
    UMingStrategicManager();

    virtual void Initialize();
    virtual void Shutdown();

    // ?ùÂ??ñ‰?‰ª∂Ë???    void SetupEventSubscriptions();
    
    // Ê∏ÖÁ?‰∫ã‰ª∂Ë®ÇÈñ±
    void CleanupEventSubscriptions();

protected:
    // ?ïÁ??∞Áï•Ê±∫Á?‰∫ã‰ª∂
    UFUNCTION()
    void OnStrategicDecisionReceived(const FString& DecisionId, const FString& Choice);

    // ?ïÁ?Â§ñ‰∫§‰∫ã‰ª∂
    UFUNCTION()
    void OnDiplomaticEventReceived(const FString& EventType, const FString& TargetFaction);

private:
    bool bIsInitialized;
};
