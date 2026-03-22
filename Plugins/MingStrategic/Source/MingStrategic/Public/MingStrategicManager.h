#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStrategicManager.generated.h"

class IMingCoreEventBus;

/**
 * ??���h��??��
 * ��?X??X?���M?X???��摧毀 */
UCLASS()
class MINGSTRATEGIC_API UMingStrategicManager : public UObject
{
    GENERATED_BODY()

public:
    UMingStrategicManager();

    virtual void Initialize();
    virtual void Shutdown();

    // 目標數量��?X
    void SetupEventSubscriptions();
    
    // �M??�ƥ�q�\
    void CleanupEventSubscriptions();

protected:
    // ???X?���M??�ƥ�
    UFUNCTION()
    void OnStrategicDecisionReceived(const FString& DecisionId, const FString& Choice);

    // 摧毀�~��ƥ�
    UFUNCTION()
    void OnDiplomaticEventReceived(const FString& EventType, const FString& TargetFaction);

private:
    bool bIsInitialized;
);

