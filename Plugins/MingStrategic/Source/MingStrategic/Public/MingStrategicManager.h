#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStrategicManager.generated.h"

class IMingCoreEventBus;

/**
 * 戰略層管理器
 * 管理全局戰略決策和外交關係
 */
UCLASS()
class MINGSTRATEGIC_API UMingStrategicManager : public UObject
{
    GENERATED_BODY()

public:
    UMingStrategicManager();

    virtual void Initialize();
    virtual void Shutdown();

    // 初始化事件訂閱
    void SetupEventSubscriptions();
    
    // 清理事件訂閱
    void CleanupEventSubscriptions();

protected:
    // 處理戰略決策事件
    UFUNCTION()
    void OnStrategicDecisionReceived(const FString& DecisionId, const FString& Choice);

    // 處理外交事件
    UFUNCTION()
    void OnDiplomaticEventReceived(const FString& EventType, const FString& TargetFaction);

private:
    bool bIsInitialized;
};
