#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingResourceSystem.h"
#include "MingResourceNode.generated.h"

UENUM(BlueprintType)
enum class EMingResourceNodeState : uint8
{
    Available     UMETA(DisplayName = "可採集"),
    Depleted      UMETA(DisplayName = "已枯竭"),
    Regenerating  UMETA(DisplayName = "再生中")
};

/**
 * 資源節點 - 地圖上的可採集資源
 * 如：糧食堆、礦場、燃料桶等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGBUILDING_API AMingResourceNode : public AActor
{
    GENERATED_BODY()

public:
    AMingResourceNode();

    // 資源屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    EMingResourceType ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    int32 TotalAmount;

    UPROPERTY(BlueprintReadOnly, Category = "Resource")
    int32 CurrentAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    int32 GatherAmountPerTrip; // 每次採集量

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    float GatherTime; // 採集所需時間（秒）

    // 再生設置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
    bool bCanRegenerate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
    float RegenerationRate; // 每秒再生量

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
    float RegenerationInterval; // 再生間隔

    // 狀態
    UPROPERTY(BlueprintReadOnly, Category = "State")
    EMingResourceNodeState NodeState;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    int32 CurrentGathererCount; // 當前採集者數量

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    int32 MaxGathererCount; // 最大同時採集者數量

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 採集接口
    UFUNCTION(BlueprintCallable, Category = "Gathering")
    bool CanBeGathered() const;

    UFUNCTION(BlueprintCallable, Category = "Gathering")
    int32 GatherResource(int32 RequestedAmount);

    UFUNCTION(BlueprintCallable, Category = "Gathering")
    void RegisterGatherer();

    UFUNCTION(BlueprintCallable, Category = "Gathering")
    void UnregisterGatherer();

    UFUNCTION(BlueprintCallable, Category = "Gathering")
    float GetGatherProgress() const;

    // 再生
    UFUNCTION(BlueprintCallable, Category = "Regeneration")
    void StartRegeneration();

    UFUNCTION(BlueprintCallable, Category = "Regeneration")
    void StopRegeneration();

    // 視覺反饋
    UFUNCTION(BlueprintCallable, Category = "Visual")
    void UpdateVisualState();

    UFUNCTION(BlueprintCallable, Category = "Visual")
    void SetHighlighted(bool bHighlighted);

    // 事件
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceGathered, EMingResourceType, ResourceType, int32, Amount);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceDepleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceRegenerated, int32, NewAmount);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceGathered OnResourceGathered;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceDepleted OnResourceDepleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceRegenerated OnResourceRegenerated;

    // 工具函數
    UFUNCTION(BlueprintPure, Category = "Utility")
    FLinearColor GetResourceColor() const;

    UFUNCTION(BlueprintPure, Category = "Utility")
    FString GetResourceName() const;

    UFUNCTION(BlueprintPure, Category = "Utility")
    float GetResourcePercentage() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UStaticMeshComponent> ResourceMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UBillboardComponent> ResourceIcon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UTextRenderComponent> AmountText;

    // 再生計時
    float TimeSinceLastRegeneration;

    void ProcessRegeneration(float DeltaTime);
    void UpdateAmountDisplay();
    void DepleteNode();

    UFUNCTION()
    void OnGatheringComplete();
};
