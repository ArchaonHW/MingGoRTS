#include "MingResourceNode.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"

AMingResourceNode::AMingResourceNode()
{
    PrimaryActorTick.bCanEverTick = true;

    // 默認值
    ResourceType = EMingResourceType::Food;
    TotalAmount = 1000;
    CurrentAmount = 1000;
    GatherAmountPerTrip = 10;
    GatherTime = 2.0f;

    bCanRegenerate = false;
    RegenerationRate = 1.0f;
    RegenerationInterval = 5.0f;

    NodeState = EMingResourceNodeState::Available;
    CurrentGathererCount = 0;
    MaxGathererCount = 3;

    TimeSinceLastRegeneration = 0.0f;

    // 創建組件
    ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
    RootComponent = ResourceMesh;

    ResourceIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("ResourceIcon"));
    ResourceIcon->SetupAttachment(RootComponent);
    ResourceIcon->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

    AmountText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("AmountText"));
    AmountText->SetupAttachment(RootComponent);
    AmountText->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
    AmountText->SetHorizontalAlignment(EHTA_Center);
    AmountText->SetTextRenderColor(FColor::White);
    AmountText->SetWorldSize(30.0f);

    // 設置碰撞
    ResourceMesh->SetCollisionProfileName(TEXT("Resource"));
    ResourceMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMingResourceNode::BeginPlay()
{
    Super::BeginPlay();

    CurrentAmount = TotalAmount;
    UpdateAmountDisplay();
    UpdateVisualState();

    UE_LOG(LogTemp, Log, TEXT("Resource Node created: %s (%s) - Amount: %d"),
           *GetResourceName(), *GetName(), CurrentAmount);
}

void AMingResourceNode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 處理再生
    if (NodeState == EMingResourceNodeState::Regenerating && bCanRegenerate)
    {
        ProcessRegeneration(DeltaTime);
    }
}

bool AMingResourceNode::CanBeGathered() const
{
    return NodeState == EMingResourceNodeState::Available &&
           CurrentAmount > 0 &&
           CurrentGathererCount < MaxGathererCount;
}

int32 AMingResourceNode::GatherResource(int32 RequestedAmount)
{
    if (!CanBeGathered())
    {
        return 0;
    }

    // 計算實際可採集數量
    int32 ActualAmount = FMath::Min(RequestedAmount, GatherAmountPerTrip);
    ActualAmount = FMath::Min(ActualAmount, CurrentAmount);

    if (ActualAmount > 0)
    {
        CurrentAmount -= ActualAmount;
        UpdateAmountDisplay();

        // 廣播事件
        OnResourceGathered.Broadcast(ResourceType, ActualAmount);

        UE_LOG(LogTemp, Log, TEXT("Gathered %d %s from %s. Remaining: %d"),
               ActualAmount, *GetResourceName(), *GetName(), CurrentAmount);

        // 檢查是否枯竭
        if (CurrentAmount <= 0)
        {
            DepleteNode();
        }
    }

    return ActualAmount;
}

void AMingResourceNode::RegisterGatherer()
{
    CurrentGathererCount++;
    UE_LOG(LogTemp, Verbose, TEXT("Gatherer registered. Count: %d/%d"),
           CurrentGathererCount, MaxGathererCount);
}

void AMingResourceNode::UnregisterGatherer()
{
    CurrentGathererCount = FMath::Max(0, CurrentGathererCount - 1);
    UE_LOG(LogTemp, Verbose, TEXT("Gatherer unregistered. Count: %d/%d"),
           CurrentGathererCount, MaxGathererCount);
}

float AMingResourceNode::GetGatherProgress() const
{
    if (NodeState == EMingResourceNodeState::Depleted)
    {
        return 0.0f;
    }

    return (float)CurrentAmount / (float)TotalAmount;
}

void AMingResourceNode::StartRegeneration()
{
    if (bCanRegenerate && CurrentAmount < TotalAmount)
    {
        NodeState = EMingResourceNodeState::Regenerating;
        UE_LOG(LogTemp, Log, TEXT("Resource node %s started regeneration"), *GetName());
    }
}

void AMingResourceNode::StopRegeneration()
{
    if (NodeState == EMingResourceNodeState::Regenerating)
    {
        if (CurrentAmount > 0)
        {
            NodeState = EMingResourceNodeState::Available;
        }
        else
        {
            NodeState = EMingResourceNodeState::Depleted;
        }
    }
}

void AMingResourceNode::ProcessRegeneration(float DeltaTime)
{
    TimeSinceLastRegeneration += DeltaTime;

    if (TimeSinceLastRegeneration >= RegenerationInterval)
    {
        int32 RegenAmount = FMath::FloorToInt(RegenerationRate * TimeSinceLastRegeneration);

        if (RegenAmount > 0)
        {
            CurrentAmount = FMath::Min(CurrentAmount + RegenAmount, TotalAmount);
            TimeSinceLastRegeneration = 0.0f;

            UpdateAmountDisplay();
            OnResourceRegenerated.Broadcast(CurrentAmount);

            UE_LOG(LogTemp, Verbose, TEXT("Resource regenerated: %d. New amount: %d"),
                   RegenAmount, CurrentAmount);

            // 如果完全再生，恢復可用狀態
            if (CurrentAmount >= TotalAmount)
            {
                NodeState = EMingResourceNodeState::Available;
                UpdateVisualState();
                UE_LOG(LogTemp, Log, TEXT("Resource node %s fully regenerated"), *GetName());
            }
        }
    }
}

void AMingResourceNode::UpdateVisualState()
{
    // 根據資源量調整視覺效果
    float Percentage = GetResourcePercentage();

    // 調整網格大小表示資源量
    float Scale = FMath::Lerp(0.5f, 1.0f, Percentage);
    ResourceMesh->SetWorldScale3D(FVector(Scale));

    // 根據狀態調整顏色
    FLinearColor Color = GetResourceColor();
    if (NodeState == EMingResourceNodeState::Depleted)
    {
        Color = FLinearColor::Gray;
    }
    else if (NodeState == EMingResourceNodeState::Regenerating)
    {
        Color = FLinearColor::Green;
    }

    // 應用材質顏色（如果有動態材質實例）
    // TODO: 創建動態材質實例來改變顏色
}

void AMingResourceNode::SetHighlighted(bool bHighlighted)
{
    if (bHighlighted)
    {
        // 高亮效果：例如增加發光或改變顏色
        ResourceMesh->SetRenderCustomDepth(true);
        ResourceMesh->SetCustomDepthStencilValue(1);
    }
    else
    {
        ResourceMesh->SetRenderCustomDepth(false);
        ResourceMesh->SetCustomDepthStencilValue(0);
    }
}

void AMingResourceNode::UpdateAmountDisplay()
{
    // 更新數量文字顯示
    FString Text = FString::Printf(TEXT("%d / %d"), CurrentAmount, TotalAmount);
    AmountText->SetText(FText::FromString(Text));

    // 根據數量調整文字顏色
    float Percentage = GetResourcePercentage();
    if (Percentage > 0.5f)
    {
        AmountText->SetTextRenderColor(FColor::Green);
    }
    else if (Percentage > 0.25f)
    {
        AmountText->SetTextRenderColor(FColor::Yellow);
    }
    else
    {
        AmountText->SetTextRenderColor(FColor::Red);
    }
}

void AMingResourceNode::DepleteNode()
{
    NodeState = EMingResourceNodeState::Depleted;
    CurrentAmount = 0;

    OnResourceDepleted.Broadcast();
    UpdateVisualState();

    UE_LOG(LogTemp, Log, TEXT("Resource node %s depleted"), *GetName());

    // 如果可再生，開始再生
    if (bCanRegenerate)
    {
        StartRegeneration();
    }
}

void AMingResourceNode::OnGatheringComplete()
{
    // 採集完成時的回調
    UpdateVisualState();
}

FLinearColor AMingResourceNode::GetResourceColor() const
{
    return UMingResourceSystem::GetResourceColor(ResourceType);
}

FString AMingResourceNode::GetResourceName() const
{
    return UMingResourceSystem::GetResourceName(ResourceType);
}

float AMingResourceNode::GetResourcePercentage() const
{
    if (TotalAmount <= 0)
    {
        return 0.0f;
    }

    return FMath::Clamp((float)CurrentAmount / (float)TotalAmount, 0.0f, 1.0f);
}
