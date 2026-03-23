#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEconomicSystem.generated.h"

class UMingRTSResourceManager;

/**
 * 經濟系統管理器
 * 管理遊戲中的所有經濟活動
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSEconomicSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEconomicSystem();

    // 初始化經濟系統
    UFUNCTION(BlueprintCallable, Category = "Economic System")
    void InitializeEconomicSystem();

    // 獲取資源數量
    UFUNCTION(BlueprintPure, Category = "Economic System")
    int32 GetResourceAmount(const FString& ResourceType);

    // 添加資源
    UFUNCTION(BlueprintCallable, Category = "Economic System")
    void AddResource(const FString& ResourceType, int32 Amount);

    // 消耗資源
    UFUNCTION(BlueprintCallable, Category = "Economic System")
    bool ConsumeResource(const FString& ResourceType, int32 Amount);

    // 檢查資源是否足夠
    UFUNCTION(BlueprintPure, Category = "Economic System")
    bool HasEnoughResource(const FString& ResourceType, int32 Amount);

    // 獲取所有資源
    UFUNCTION(BlueprintPure, Category = "Economic System")
    TMap<FString, int32> GetAllResources();

    // 計算經濟增長率
    UFUNCTION(BlueprintCallable, Category = "Economic System")
    float CalculateEconomicGrowthRate();

    // 更新經濟系統
    UFUNCTION(BlueprintCallable, Category = "Economic System")
    void UpdateEconomicSystem(float DeltaTime);

protected:
    // 資源存儲
    UPROPERTY(BlueprintReadOnly, Category = "Economic System")
    TMap<FString, int32> Resources;

    // 資源管理器
    UPROPERTY(BlueprintReadOnly, Category = "Economic System")
    TObjectPtr<UMingRTSResourceManager> ResourceManager;

private:
    // 內部方法
    void CalculateResourceProduction();
    void CalculateResourceConsumption();
    void UpdateResourceValues();
};
