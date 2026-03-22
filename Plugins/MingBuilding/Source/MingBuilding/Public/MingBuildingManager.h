#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingManager.generated.h"

/**
 * 建築層管理器
 * 管理基地建設和資源生產
 */
UCLASS()
class MINGBUILDING_API UMingBuildingManager : public UObject
{
    GENERATED_BODY()

public:
    UMingBuildingManager();

    virtual void Initialize();
    virtual void Shutdown();

    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

    // 建築接口
    UFUNCTION(BlueprintCallable, Category = "Building")
    void ConstructBuilding(const FString& BuildingType, const FVector& Position);

    UFUNCTION(BlueprintCallable, Category = "Building")
    void UpgradeBuilding(int32 BuildingId);

    // 資源管理
    UFUNCTION(BlueprintCallable, Category = "Resource")
    void AddResource(const FString& ResourceType, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Resource")
    int32 GetResourceAmount(const FString& ResourceType) const;

protected:
    void OnBuildingConstructed(const FString& BuildingType);
    void OnResourceUpdated(const FString& ResourceType, int32 NewAmount);

private:
    bool bIsInitialized;
    
    // 資源存儲
    TMap<FString, int32> ResourceInventory;
};
