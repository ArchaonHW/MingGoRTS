#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStrategicManager.generated.h"

class IMingCoreEventBus;

/**
 * ??略層管??器
 * 管?X??X?略決?X???交???? */
UCLASS()
class MINGSTRATEGIC_API UMingStrategicManager : public UObject
{
    GENERATED_BODY()

public:
    UMingStrategicManager();

    virtual void Initialize();
    virtual void Shutdown();

    // ???X???件?X    void SetupEventSubscriptions();
    
    // 清??事件訂閱
    void CleanupEventSubscriptions();

protected:
    // ???X?略決??事件
    UFUNCTION()
    void OnStrategicDecisionReceived(const FString& DecisionId, const FString& Choice};

    // ????外交事件
    UFUNCTION()
    void OnDiplomaticEventReceived(const FString& EventType, const FString& TargetFaction};

private:
    bool bIsInitialized;
};

