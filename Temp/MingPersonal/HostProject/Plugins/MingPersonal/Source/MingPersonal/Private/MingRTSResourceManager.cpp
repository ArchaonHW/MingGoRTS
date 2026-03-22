#include "MingRTSResourceManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "MingRTSEconomicSystem.h"
#include "MingRTSBuildingSystem.h"

UMingRTSResourceManager::UMingRTSResourceManager()
{
    CurrentWorld = nullptr;
    EconomicSystem = nullptr;
    BuildingSystem = nullptr;
    ResourceUpdateInterval = 1.0f; // 每秒更新
    LastResourceUpdateTime = 0.0f;
    ResourceExtractionInterval = 0.5f; // 每500ms提取
    LastResourceExtractionTime = 0.0f;
    ResourceRegenerationInterval = 2.0f; // 每2秒再生
    LastResourceRegenerationTime = 0.0f;
    ResourceDistributionInterval = 3.0f; // 每3秒分配
    LastResourceDistributionTime = 0.0f;
    
    InitializeResourceGenerationSettings();
}

void UMingRTSResourceManager::InitializeResourceManager(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSResourceManager: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    LastResourceUpdateTime = World->GetTimeSeconds();
    LastResourceExtractionTime = World->GetTimeSeconds();
    LastResourceRegenerationTime = World->GetTimeSeconds();
    LastResourceDistributionTime = World->GetTimeSeconds();
    
    // 清空現有資源節點
    ResourceNodeMap.Empty();
    ResourceExtractionData.Empty();
    
    // 自動生成初始資源節點
    GenerateResourceNodesAutomatically();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSResourceManager initialized successfully"));
}

void UMingRTSResourceManager::UpdateResourceManager(float DeltaTime)
{
    if (!CurrentWorld)
    {
        return;
    }

    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    // 定期更新資源節點
    if (CurrentTime - LastResourceUpdateTime >= ResourceUpdateInterval)
    {
        UpdateResourceNodeStates();
        UpdateResourceNodeInfluences();
        CheckResourceNodeIntegrity();
        LastResourceUpdateTime = CurrentTime;
    }
    
    // 處理資源提取
    ProcessResourceExtraction(DeltaTime);
    
    // 處理資源再生
    if (CurrentTime - LastResourceRegenerationTime >= ResourceRegenerationInterval)
    {
        ProcessResourceRegeneration(ResourceRegenerationInterval);
        LastResourceRegenerationTime = CurrentTime;
    }
    
    // 分配資源
    if (CurrentTime - LastResourceDistributionTime >= ResourceDistributionInterval)
    {
        DistributeResources();
        LastResourceDistributionTime = CurrentTime;
    }
    
    // 處理資源節點交互
    ProcessResourceNodeInteractions();
    
    // 清理無效資源節點
    CleanupInvalidResourceNodes();
}

bool UMingRTSResourceManager::GenerateResourceNode(EResourceNodeType NodeType, FVector Location)
{
    if (!CurrentWorld)
    {
        return false;
    }
    
    // 檢查位置是否有效
    if (!IsValidResourceNodeLocation(Location))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid resource node location: %s"), *Location.ToString());
        return false;
    }
    
    // 檢查是否超過最大節點數量
    if (ResourceNodeMap.Num() >= GenerationSettings.MaxResourceNodes)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum resource nodes reached"));
        return false;
    }
    
    // 檢查資源節點依賴
    if (!CheckResourceNodeDependencies(NodeType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Resource node dependencies not met"));
        return false;
    }
    
    // 創建資源節點Actor
    AActor* ResourceNodeActor = CreateResourceNodeActor(NodeType, Location);
    if (!ResourceNodeActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create resource node actor"));
        return false;
    }
    
    // 初始化資源節點數據
    FResourceNode ResourceNodeData;
    ResourceNodeData.NodeType = NodeType;
    ResourceNodeData.ResourceType = GenerationSettings.NodeTypeToResourceMapping.FindRef(NodeType);
    ResourceNodeData.CurrentState = EResourceNodeState::Untouched;
    ResourceNodeData.Location = Location;
    ResourceNodeData.ResourceAmount = GenerationSettings.DefaultResourceAmounts.FindRef(NodeType);
    ResourceNodeData.MaxResourceAmount = ResourceNodeData.ResourceAmount;
    ResourceNodeData.ExtractionRate = GenerationSettings.DefaultExtractionRates.FindRef(NodeType);
    ResourceNodeData.ExtractionMethod = EResourceExtractionMethod::Manual;
    ResourceNodeData.Quality = GenerationSettings.DefaultQuality;
    ResourceNodeData.RegenerationRate = GenerationSettings.DefaultRegenerationRates.FindRef(NodeType);
    ResourceNodeData.LastExtractionTime = CurrentWorld->GetTimeSeconds();
    ResourceNodeData.bIsAccessible = true;
    ResourceNodeData.InfluenceRadius = GenerationSettings.DefaultInfluenceRadius;
    
    // 添加到資源節點映射
    ResourceNodeMap.Add(ResourceNodeActor, ResourceNodeData);
    
    // 計算影響範圍
    CalculateResourceNodeInfluence(ResourceNodeActor);
    
    UE_LOG(LogTemp, Log, TEXT("Generated resource node: %s at %s"), 
        *GetResourceNodeName(NodeType), *Location.ToString());
    
    return true;
}

void UMingRTSResourceManager::GenerateResourceNodesAutomatically()
{
    if (!CurrentWorld)
    {
        return;
    }
    
    // 獲取世界邊界（簡化實現）
    FVector WorldCenter = FVector(0.0f, 0.0f, 0.0f);
    float WorldRadius = 2000.0f;
    
    // 生成各種類型的資源節點
    for (int32 i = 0; i < (int32)EResourceNodeType::TeaPlantation + 1; i++)
    {
        EResourceNodeType NodeType = (EResourceNodeType)i;
        
        // 根據節點類型決定生成數量
        int32 NodeCount = 0;
        switch (NodeType)
        {
            case EResourceNodeType::Forest:
                NodeCount = 8;
                break;
            case EResourceNodeType::Mine:
                NodeCount = 5;
                break;
            case EResourceNodeType::Quarry:
                NodeCount = 6;
                break;
            case EResourceNodeType::Farm:
                NodeCount = 10;
                break;
            case EResourceNodeType::OilWell:
                NodeCount = 3;
                break;
            case EResourceNodeType::GasField:
                NodeCount = 2;
                break;
            case EResourceNodeType::FishingGround:
                NodeCount = 7;
                break;
            case EResourceNodeType::HerbGarden:
                NodeCount = 4;
                break;
            case EResourceNodeType::SpiceField:
                NodeCount = 3;
                break;
            case EResourceNodeType::TeaPlantation:
                NodeCount = 5;
                break;
            default:
                NodeCount = 2;
                break;
        }
        
        // 生成節點
        for (int32 j = 0; j < NodeCount; j++)
        {
            FVector Location = GetBestResourceNodeLocation(NodeType, WorldCenter);
            if (Location != FVector::ZeroVector)
            {
                GenerateResourceNode(NodeType, Location);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d resource nodes automatically"), ResourceNodeMap.Num());
}

bool UMingRTSResourceManager::DiscoverResourceNode(AActor* ResourceNodeActor)
{
    FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (!ResourceNodeData)
    {
        return false;
    }
    
    if (ResourceNodeData->CurrentState == EResourceNodeState::Untouched)
    {
        ResourceNodeData->CurrentState = EResourceNodeState::Discovered;
        
        // 觸發發現事件
        OnResourceNodeDiscovered.Broadcast(*ResourceNodeData);
        
        UE_LOG(LogTemp, Log, TEXT("Discovered resource node: %s"), *GetResourceNodeName(ResourceNodeData->NodeType));
        
        return true;
    }
    
    return false;
}

float UMingRTSResourceManager::ExtractResource(AActor* ResourceNodeActor, AActor* ExtractorBuilding, float Amount)
{
    FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (!ResourceNodeData)
    {
        return 0.0f;
    }
    
    if (ResourceNodeData->CurrentState != EResourceNodeState::BeingExtracted && 
        ResourceNodeData->CurrentState != EResourceNodeState::Discovered)
    {
        return 0.0f;
    }
    
    if (!ResourceNodeData->bIsAccessible)
    {
        return 0.0f;
    }
    
    // 計算實際提取量（考慮質量和效率）
    float Efficiency = CalculateExtractionEfficiency(ResourceNodeActor, ExtractorBuilding);
    float ActualAmount = Amount * ResourceNodeData->Quality * Efficiency;
    
    // 限制提取量
    ActualAmount = FMath::Min(ActualAmount, ResourceNodeData->ResourceAmount);
    
    if (ActualAmount <= 0.0f)
    {
        return 0.0f;
    }
    
    // 提取資源
    ResourceNodeData->ResourceAmount -= ActualAmount;
    ResourceNodeData->LastExtractionTime = CurrentWorld->GetTimeSeconds();
    
    // 更新狀態
    if (ResourceNodeData->ResourceAmount <= 0.0f)
    {
        ResourceNodeData->CurrentState = EResourceNodeState::Depleted;
        OnResourceNodeDepleted.Broadcast(*ResourceNodeData);
    }
    else
    {
        ResourceNodeData->CurrentState = EResourceNodeState::BeingExtracted;
    }
    
    // 添加到經濟系統
    if (EconomicSystem)
    {
        EconomicSystem->AddResource(ResourceNodeData->ResourceType, ActualAmount);
    }
    
    // 觸發提取事件
    OnResourceExtracted.Broadcast(ResourceNodeData->ResourceType, ActualAmount);
    
    UE_LOG(LogTemp, Log, TEXT("Extracted %.2f %s from %s"), 
        ActualAmount, *LexToString(ResourceNodeData->ResourceType), *GetResourceNodeName(ResourceNodeData->NodeType));
    
    return ActualAmount;
}

bool UMingRTSResourceManager::SetupResourceExtraction(AActor* ResourceNodeActor, AActor* ExtractorBuilding)
{
    FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (!ResourceNodeData)
    {
        return false;
    }
    
    // 檢查是否已經有提取設置
    for (FResourceExtractionData& ExtractionData : ResourceExtractionData)
    {
        if (ExtractionData.ResourceNode == ResourceNodeActor && 
            ExtractionData.ExtractorBuilding == ExtractorBuilding)
        {
            ExtractionData.bIsActive = true;
            return true;
        }
    }
    
    // 創建新的提取數據
    FResourceExtractionData NewExtractionData;
    NewExtractionData.ResourceNode = ResourceNodeActor;
    NewExtractionData.ExtractorBuilding = ExtractorBuilding;
    NewExtractionData.ResourceType = ResourceNodeData->ResourceType;
    NewExtractionData.ExtractionRate = ResourceNodeData->ExtractionRate;
    NewExtractionData.Efficiency = CalculateExtractionEfficiency(ResourceNodeActor, ExtractorBuilding);
    NewExtractionData.LastExtractionTime = CurrentWorld->GetTimeSeconds();
    NewExtractionData.TotalExtracted = 0.0f;
    NewExtractionData.bIsActive = true;
    
    ResourceExtractionData.Add(NewExtractionData);
    
    // 發現資源節點
    DiscoverResourceNode(ResourceNodeActor);
    
    UE_LOG(LogTemp, Log, TEXT("Setup resource extraction for %s"), *GetResourceNodeName(ResourceNodeData->NodeType));
    
    return true;
}

void UMingRTSResourceManager::StopResourceExtraction(AActor* ResourceNodeActor, AActor* ExtractorBuilding)
{
    for (FResourceExtractionData& ExtractionData : ResourceExtractionData)
    {
        if (ExtractionData.ResourceNode == ResourceNodeActor && 
            ExtractionData.ExtractorBuilding == ExtractorBuilding)
        {
            ExtractionData.bIsActive = false;
            
            FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
            if (ResourceNodeData)
            {
                UE_LOG(LogTemp, Log, TEXT("Stopped resource extraction for %s"), *GetResourceNodeName(ResourceNodeData->NodeType));
            }
            
            return;
        }
    }
}

FResourceNode UMingRTSResourceManager::GetResourceNode(AActor* ResourceNodeActor) const
{
    const FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (ResourceNodeData)
    {
        return *ResourceNodeData;
    }
    return FResourceNode();
}

TArray<AActor*> UMingRTSResourceManager::GetAllResourceNodes() const
{
    TArray<AActor*> AllResourceNodes;
    for (const TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        if (ResourceNodePair.Key && !ResourceNodePair.Key->IsPendingKill())
        {
            AllResourceNodes.Add(ResourceNodePair.Key);
        }
    }
    return AllResourceNodes;
}

TArray<AActor*> UMingRTSResourceManager::GetResourceNodesByType(EResourceNodeType NodeType) const
{
    TArray<AActor*> NodesByType;
    for (const TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        if (ResourceNodePair.Key && !ResourceNodePair.Key->IsPendingKill() && 
            ResourceNodePair.Value.NodeType == NodeType)
        {
            NodesByType.Add(ResourceNodePair.Key);
        }
    }
    return NodesByType;
}

TArray<AActor*> UMingRTSResourceManager::GetResourceNodesByResourceType(EResourceType ResourceType) const
{
    TArray<AActor*> NodesByResourceType;
    for (const TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        if (ResourceNodePair.Key && !ResourceNodePair.Key->IsPendingKill() && 
            ResourceNodePair.Value.ResourceType == ResourceType)
        {
            NodesByResourceType.Add(ResourceNodePair.Key);
        }
    }
    return NodesByResourceType;
}

TArray<AActor*> UMingRTSResourceManager::GetNearbyResourceNodes(FVector Location, float Radius) const
{
    TArray<AActor*> NearbyNodes;
    for (const TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        if (ResourceNodePair.Key && !ResourceNodePair.Key->IsPendingKill())
        {
            float Distance = FVector::Dist(Location, ResourceNodePair.Value.Location);
            if (Distance <= Radius)
            {
                NearbyNodes.Add(ResourceNodePair.Key);
            }
        }
    }
    return NearbyNodes;
}

bool UMingRTSResourceManager::IsResourceNodeAccessible(AActor* ResourceNodeActor) const
{
    const FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (ResourceNodeData)
    {
        return ResourceNodeData->bIsAccessible;
    }
    return false;
}

void UMingRTSResourceManager::SetResourceNodeAccessibility(AActor* ResourceNodeActor, bool bIsAccessible)
{
    FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (ResourceNodeData)
    {
        ResourceNodeData->bIsAccessible = bIsAccessible;
        UE_LOG(LogTemp, Log, TEXT("Set resource node accessibility: %s"), bIsAccessible ? TEXT("true") : TEXT("false"));
    }
}

float UMingRTSResourceManager::GetResourceNodeQuality(AActor* ResourceNodeActor) const
{
    const FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (ResourceNodeData)
    {
        return ResourceNodeData->Quality;
    }
    return 1.0f;
}

void UMingRTSResourceManager::SetResourceNodeQuality(AActor* ResourceNodeActor, float Quality)
{
    FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (ResourceNodeData)
    {
        ResourceNodeData->Quality = FMath::Clamp(Quality, 0.1f, 2.0f);
        UE_LOG(LogTemp, Log, TEXT("Set resource node quality to %.2f"), ResourceNodeData->Quality);
    }
}

TArray<FResourceExtractionData> UMingRTSResourceManager::GetResourceExtractionData() const
{
    return ResourceExtractionData;
}

float UMingRTSResourceManager::CalculateExtractionEfficiency(AActor* ResourceNodeActor, AActor* ExtractorBuilding) const
{
    const FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (!ResourceNodeData)
    {
        return 0.0f;
    }
    
    float BaseEfficiency = 1.0f;
    
    // 根據提取方法調整效率
    switch (ResourceNodeData->ExtractionMethod)
    {
        case EResourceExtractionMethod::Manual:
            BaseEfficiency = 0.5f;
            break;
        case EResourceExtractionMethod::Tool:
            BaseEfficiency = 0.8f;
            break;
        case EResourceExtractionMethod::Machine:
            BaseEfficiency = 1.2f;
            break;
        case EResourceExtractionMethod::Advanced:
            BaseEfficiency = 1.5f;
            break;
        case EResourceExtractionMethod::Automated:
            BaseEfficiency = 2.0f;
            break;
    }
    
    // 根據資源節點質量調整
    BaseEfficiency *= ResourceNodeData->Quality;
    
    // 根據建築類型調整（如果有建築系統）
    if (BuildingSystem && ExtractorBuilding)
    {
        FBuildingData BuildingData = BuildingSystem->GetBuildingData(ExtractorBuilding);
        BaseEfficiency *= BuildingSystem->CalculateBuildingEfficiency(ExtractorBuilding);
    }
    
    return FMath::Clamp(BaseEfficiency, 0.1f, 3.0f);
}

void UMingRTSResourceManager::ProcessResourceRegeneration(float DeltaTime)
{
    for (TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        FResourceNode& ResourceNodeData = ResourceNodePair.Value;
        
        // 只有可再生資源才能再生
        if (ResourceNodeData.RegenerationRate > 0.0f)
        {
            // 檢查是否在再生狀態
            if (ResourceNodeData.CurrentState == EResourceNodeState::Depleted ||
                ResourceNodeData.CurrentState == EResourceNodeState::Untouched)
            {
                ResourceNodeData.CurrentState = EResourceNodeState::Regenerating;
            }
            
            // 再生資源
            if (ResourceNodeData.CurrentState == EResourceNodeState::Regenerating)
            {
                float RegenerationAmount = ResourceNodeData.RegenerationRate * DeltaTime;
                ResourceNodeData.ResourceAmount = FMath::Min(
                    ResourceNodeData.ResourceAmount + RegenerationAmount,
                    ResourceNodeData.MaxResourceAmount
                );
                
                // 如果再生到一定數量，變為可發現狀態
                if (ResourceNodeData.ResourceAmount >= ResourceNodeData.MaxResourceAmount * 0.3f)
                {
                    ResourceNodeData.CurrentState = EResourceNodeState::Discovered;
                    OnResourceNodeRegenerated.Broadcast(ResourceNodeData);
                }
            }
        }
    }
}

void UMingRTSResourceManager::UpdateResourceNodeStates()
{
    for (TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        FResourceNode& ResourceNodeData = ResourceNodePair.Value;
        
        // 檢查狀態轉換
        switch (ResourceNodeData.CurrentState)
        {
            case EResourceNodeState::BeingExtracted:
                // 如果長時間沒有提取，變為發現狀態
                if (CurrentWorld->GetTimeSeconds() - ResourceNodeData.LastExtractionTime > 30.0f)
                {
                    ResourceNodeData.CurrentState = EResourceNodeState::Discovered;
                }
                break;
                
            case EResourceNodeState::Depleted:
                // 如果有再生率，開始再生
                if (ResourceNodeData.RegenerationRate > 0.0f)
                {
                    ResourceNodeData.CurrentState = EResourceNodeState::Regenerating;
                }
                break;
                
            default:
                break;
        }
    }
}

void UMingRTSResourceManager::DistributeResources()
{
    // 簡化實現：將資源分配給附近的建築
    if (!BuildingSystem || !EconomicSystem)
    {
        return;
    }
    
    TArray<AActor*> AllBuildings = BuildingSystem->GetAllBuildings();
    
    for (AActor* Building : AllBuildings)
    {
        FBuildingData BuildingData = BuildingSystem->GetBuildingData(Building);
        
        // 檢查建築是否需要資源
        if (BuildingData.ConsumedResources.Num() > 0)
        {
            // 獲取附近的資源節點
            TArray<AActor*> NearbyNodes = GetNearbyResourceNodes(BuildingData.Location, BuildingData.InfluenceRadius);
            
            for (EResourceType ConsumedResource : BuildingData.ConsumedResources)
            {
                // 尋找對應的資源節點
                for (AActor* ResourceNode : NearbyNodes)
                {
                    FResourceNode ResourceNodeData = GetResourceNode(ResourceNode);
                    if (ResourceNodeData.ResourceType == ConsumedResource && 
                        ResourceNodeData.CurrentState == EResourceNodeState::Discovered)
                    {
                        // 設置資源提取
                        SetupResourceExtraction(ResourceNode, Building);
                        break;
                    }
                }
            }
        }
    }
}

void UMingRTSResourceManager::OptimizeResourceDistribution()
{
    // 優化資源分配算法
    // 這裡可以添加更複雜的優化邏輯
    
    UE_LOG(LogTemp, Log, TEXT("Optimizing resource distribution..."));
    
    // 重新平衡資源分配
    BalanceResourceDistribution();
}

TMap<EResourceType, float> UMingRTSResourceManager::GetResourceStatistics() const
{
    TMap<EResourceType, float> Statistics;
    
    for (const TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        const FResourceNode& ResourceNodeData = ResourceNodePair.Value;
        
        float CurrentAmount = Statistics.FindRef(ResourceNodeData.ResourceType);
        Statistics.Add(ResourceNodeData.ResourceType, CurrentAmount + ResourceNodeData.ResourceAmount);
    }
    
    return Statistics;
}

TMap<EResourceNodeType, int32> UMingRTSResourceManager::GetResourceNodeStatistics() const
{
    TMap<EResourceNodeType, int32> Statistics;
    
    for (const TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        const FResourceNode& ResourceNodeData = ResourceNodePair.Value;
        
        int32 CurrentCount = Statistics.FindRef(ResourceNodeData.NodeType);
        Statistics.Add(ResourceNodeData.NodeType, CurrentCount + 1);
    }
    
    return Statistics;
}

void UMingRTSResourceManager::InitializeResourceGenerationSettings()
{
    // 使用默認設置初始化
    GenerationSettings = FResourceGenerationSettings();
}

AActor* UMingRTSResourceManager::CreateResourceNodeActor(EResourceNodeType NodeType, FVector Location)
{
    if (!CurrentWorld)
    {
        return nullptr;
    }
    
    // 這裡應該根據節點類型創建對應的Actor類
    // 簡化實現，創建一個基礎Actor
    AActor* ResourceNodeActor = CurrentWorld->SpawnActor<AActor>(AActor::StaticClass(), Location);
    
    if (ResourceNodeActor)
    {
        ResourceNodeActor->SetActorLabel(GetResourceNodeName(NodeType));
    }
    
    return ResourceNodeActor;
}

void UMingRTSResourceManager::ProcessResourceExtraction(float DeltaTime)
{
    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    if (CurrentTime - LastResourceExtractionTime >= ResourceExtractionInterval)
    {
        for (FResourceExtractionData& ExtractionData : ResourceExtractionData)
        {
            if (ExtractionData.bIsActive && ExtractionData.ResourceNode && ExtractionData.ExtractorBuilding)
            {
                // 提取資源
                float ExtractedAmount = ExtractResource(
                    ExtractionData.ResourceNode,
                    ExtractionData.ExtractorBuilding,
                    ExtractionData.ExtractionRate * ResourceExtractionInterval
                );
                
                ExtractionData.TotalExtracted += ExtractedAmount;
                ExtractionData.LastExtractionTime = CurrentTime;
            }
        }
        
        LastResourceExtractionTime = CurrentTime;
    }
}

void UMingRTSResourceManager::UpdateResourceNodeInfluences()
{
    for (TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        CalculateResourceNodeInfluence(ResourceNodePair.Key);
    }
}

void UMingRTSResourceManager::CheckResourceNodeIntegrity()
{
    for (TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        if (!ValidateResourceNodeData(ResourceNodePair.Value))
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid resource node data detected"));
        }
    }
}

void UMingRTSResourceManager::CleanupInvalidResourceNodes()
{
    // 清理無效資源節點
    for (auto It = ResourceNodeMap.CreateIterator(); It; ++It)
    {
        if (!It->Key || It->Key->IsPendingKill())
        {
            It.RemoveCurrent();
        }
    }
    
    // 清理無效提取數據
    for (int32 i = ResourceExtractionData.Num() - 1; i >= 0; --i)
    {
        const FResourceExtractionData& ExtractionData = ResourceExtractionData[i];
        if (!ExtractionData.ResourceNode || !ExtractionData.ExtractorBuilding ||
            ExtractionData.ResourceNode->IsPendingKill() || ExtractionData.ExtractorBuilding->IsPendingKill())
        {
            ResourceExtractionData.RemoveAt(i);
        }
    }
}

void UMingRTSResourceManager::CalculateResourceNodeInfluence(AActor* ResourceNodeActor)
{
    FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (!ResourceNodeData)
    {
        return;
    }
    
    // 清空附近建築列表
    ResourceNodeData->NearbyBuildings.Empty();
    
    // 獲取附近的建築
    if (BuildingSystem)
    {
        TArray<AActor*> NearbyBuildings = BuildingSystem->GetBuildingsInInfluenceRange(ResourceNodeActor);
        ResourceNodeData->NearbyBuildings = NearbyBuildings;
    }
}

void UMingRTSResourceManager::ProcessResourceNodeInteractions()
{
    // 處理資源節點之間的交互
    for (TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        AActor* ResourceNodeActor = ResourceNodePair.Key;
        FResourceNode& ResourceNodeData = ResourceNodePair.Value;
        
        if (!ResourceNodeActor)
        {
            continue;
        }
        
        // 應用資源節點效果
        ApplyResourceNodeEffects(ResourceNodeActor);
        
        // 更新附近建築
        UpdateNearbyBuildings(ResourceNodeActor);
    }
}

FString UMingRTSResourceManager::GetResourceNodeName(EResourceNodeType NodeType) const
{
    switch (NodeType)
    {
        case EResourceNodeType::Forest: return TEXT("Forest");
        case EResourceNodeType::Mine: return TEXT("Mine");
        case EResourceNodeType::Quarry: return TEXT("Quarry");
        case EResourceNodeType::Farm: return TEXT("Farm");
        case EResourceNodeType::OilWell: return TEXT("Oil Well");
        case EResourceNodeType::GasField: return TEXT("Gas Field");
        case EResourceNodeType::FishingGround: return TEXT("Fishing Ground");
        case EResourceNodeType::HerbGarden: return TEXT("Herb Garden");
        case EResourceNodeType::SpiceField: return TEXT("Spice Field");
        case EResourceNodeType::TeaPlantation: return TEXT("Tea Plantation");
        default: return TEXT("Unknown Resource Node");
    }
}

FString UMingRTSResourceManager::GetResourceNodeDescription(EResourceNodeType NodeType) const
{
    switch (NodeType)
    {
        case EResourceNodeType::Forest: return TEXT("Natural forest providing wood resources");
        case EResourceNodeType::Mine: return TEXT("Underground mine containing iron and other minerals");
        case EResourceNodeType::Quarry: return TEXT("Stone quarry for construction materials");
        case EResourceNodeType::Farm: return TEXT("Agricultural farm producing food");
        case EResourceNodeType::OilWell: return TEXT("Oil well extracting fossil fuels");
        case EResourceNodeType::GasField: return TEXT("Natural gas field");
        case EResourceNodeType::FishingGround: return TEXT("Rich fishing grounds");
        case EResourceNodeType::HerbGarden: return TEXT("Herb garden for medicinal plants");
        case EResourceNodeType::SpiceField: return TEXT("Spice field for valuable spices");
        case EResourceNodeType::TeaPlantation: return TEXT("Tea plantation for tea leaves");
        default: return TEXT("Unknown resource node type");
    }
}

bool UMingRTSResourceManager::IsValidResourceNodeLocation(FVector Location) const
{
    // 檢查位置是否與現有節點太近
    for (const TPair<TObjectPtr<AActor>, FResourceNode>& ResourceNodePair : ResourceNodeMap)
    {
        float Distance = FVector::Dist(Location, ResourceNodePair.Value.Location);
        if (Distance < GenerationSettings.ResourceNodeSpacing)
        {
            return false;
        }
    }
    
    return true;
}

float UMingRTSResourceManager::CalculateResourceNodeValue(const FResourceNode& ResourceNode) const
{
    float BaseValue = 0.0f;
    
    // 根據資源類型確定基礎價值
    switch (ResourceNode.ResourceType)
    {
        case EResourceType::Gold: BaseValue = 100.0f; break;
        case EResourceType::Iron: BaseValue = 50.0f; break;
        case EResourceType::Stone: BaseValue = 20.0f; break;
        case EResourceType::Wood: BaseValue = 15.0f; break;
        case EResourceType::Food: BaseValue = 10.0f; break;
        case EResourceType::Coal: BaseValue = 25.0f; break;
        case EResourceType::Silk: BaseValue = 80.0f; break;
        case EResourceType::Tea: BaseValue = 30.0f; break;
        case EResourceType::Porcelain: BaseValue = 60.0f; break;
        case EResourceType::Spice: BaseValue = 40.0f; break;
        default: BaseValue = 10.0f; break;
    }
    
    // 根據質量調整
    BaseValue *= ResourceNode.Quality;
    
    // 根據資源量調整
    BaseValue *= (ResourceNode.ResourceAmount / ResourceNode.MaxResourceAmount);
    
    return BaseValue;
}

void UMingRTSResourceManager::ApplyResourceNodeEffects(AActor* ResourceNodeActor)
{
    FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (!ResourceNodeData)
    {
        return;
    }
    
    // 根據節點類型應用效果
    switch (ResourceNodeData->NodeType)
    {
        case EResourceNodeType::Forest:
            // 森林提供環境加成
            break;
        case EResourceNodeType::Mine:
            // 礦場可能降低附近環境質量
            break;
        default:
            break;
    }
}

void UMingRTSResourceManager::UpdateNearbyBuildings(AActor* ResourceNodeActor)
{
    FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (!ResourceNodeData || !BuildingSystem)
    {
        return;
    }
    
    // 更新附近建築的生產效率
    for (AActor* Building : ResourceNodeData->NearbyBuildings)
    {
        FBuildingData BuildingData = BuildingSystem->GetBuildingData(Building);
        
        // 根據資源節點類型給予加成
        float Bonus = 1.0f;
        switch (ResourceNodeData->NodeType)
        {
            case EResourceNodeType::Farm:
                if (BuildingData.BuildingType == EBuildingType::Market)
                {
                    Bonus = 1.2f; // 農場附近市場效率提升
                }
                break;
            case EResourceNodeType::Mine:
                if (BuildingData.BuildingType == EBuildingType::Workshop)
                {
                    Bonus = 1.3f; // 礦場附近工坊效率提升
                }
                break;
            default:
                break;
        }
        
        // 這裡可以應用加成到建築系統
    }
}

bool UMingRTSResourceManager::CheckResourceNodeDependencies(EResourceNodeType NodeType) const
{
    // 檢查資源節點的依賴條件
    // 例如某些資源節點需要特定的建築才能生成
    
    switch (NodeType)
    {
        case EResourceNodeType::OilWell:
            // 需要先有礦場
            return GetResourceNodesByType(EResourceNodeType::Mine).Num() > 0;
        case EResourceNodeType::GasField:
            // 需要先有油井
            return GetResourceNodesByType(EResourceNodeType::OilWell).Num() > 0;
        default:
            return true;
    }
}

FVector UMingRTSResourceManager::GetBestResourceNodeLocation(EResourceNodeType NodeType, FVector CenterLocation) const
{
    // 在指定範圍內尋找最佳位置
    float SearchRadius = 1500.0f;
    int32 MaxAttempts = 50;
    
    for (int32 i = 0; i < MaxAttempts; i++)
    {
        // 隨機生成位置
        float Angle = FMath::RandRange(0.0f, 2.0f * PI);
        float Distance = FMath::RandRange(200.0f, SearchRadius);
        
        FVector Location = CenterLocation + FVector(
            FMath::Cos(Angle) * Distance,
            FMath::Sin(Angle) * Distance,
            0.0f
        );
        
        // 檢查位置是否有效
        if (IsValidResourceNodeLocation(Location))
        {
            return Location;
        }
    }
    
    return FVector::ZeroVector;
}

bool UMingRTSResourceManager::ValidateResourceNodeData(const FResourceNode& ResourceNode) const
{
    // 驗證資源節點數據的有效性
    if (ResourceNode.MaxResourceAmount <= 0.0f)
    {
        return false;
    }
    
    if (ResourceNode.ResourceAmount < 0.0f || ResourceNode.ResourceAmount > ResourceNode.MaxResourceAmount)
    {
        return false;
    }
    
    if (ResourceNode.Quality <= 0.0f)
    {
        return false;
    }
    
    if (ResourceNode.ExtractionRate < 0.0f)
    {
        return false;
    }
    
    return true;
}

void UMingRTSResourceManager::RecordResourceStatistics()
{
    // 記錄資源統計數據用於分析
    TMap<EResourceType, float> CurrentStats = GetResourceStatistics();
    
    for (const TPair<EResourceType, float>& Stat : CurrentStats)
    {
        UE_LOG(LogTemp, Log, TEXT("Resource %s: %.2f"), *LexToString(Stat.Key), Stat.Value);
    }
}

float UMingRTSResourceManager::PredictResourceDepletionTime(AActor* ResourceNodeActor) const
{
    const FResourceNode* ResourceNodeData = ResourceNodeMap.Find(ResourceNodeActor);
    if (!ResourceNodeData)
    {
        return -1.0f;
    }
    
    // 計算總提取率
    float TotalExtractionRate = 0.0f;
    for (const FResourceExtractionData& ExtractionData : ResourceExtractionData)
    {
        if (ExtractionData.ResourceNode == ResourceNodeActor && ExtractionData.bIsActive)
        {
            TotalExtractionRate += ExtractionData.ExtractionRate * ExtractionData.Efficiency;
        }
    }
    
    if (TotalExtractionRate <= 0.0f)
    {
        return -1.0f; // 不會耗盡
    }
    
    // 預測耗盡時間
    return ResourceNodeData->ResourceAmount / TotalExtractionRate;
}

void UMingRTSResourceManager::OptimizeResourceExtractionPaths()
{
    // 優化資源提取路徑
    UE_LOG(LogTemp, Log, TEXT("Optimizing resource extraction paths..."));
    
    // 這裡可以添加路徑優化算法
}

void UMingRTSResourceManager::HandleResourceConflicts()
{
    // 處理資源衝突
    UE_LOG(LogTemp, Log, TEXT("Handling resource conflicts..."));
    
    // 檢查是否有多個建築爭奪同一資源節點
    TMap<AActor*, int32> ResourceNodeUsage;
    
    for (const FResourceExtractionData& ExtractionData : ResourceExtractionData)
    {
        if (ExtractionData.bIsActive && ExtractionData.ResourceNode)
        {
            int32 UsageCount = ResourceNodeUsage.FindRef(ExtractionData.ResourceNode);
            ResourceNodeUsage.Add(ExtractionData.ResourceNode, UsageCount + 1);
        }
    }
    
    // 處理衝突
    for (const TPair<AActor*, int32>& Usage : ResourceNodeUsage)
    {
        if (Usage.Value > 3) // 超過3個建築使用同一節點
        {
            // 降低效率或重新分配
            UE_LOG(LogTemp, Warning, TEXT("Resource node conflict detected: %d users"), Usage.Value);
        }
    }
}

void UMingRTSResourceManager::BalanceResourceDistribution()
{
    // 平衡資源分布
    UE_LOG(LogTemp, Log, TEXT("Balancing resource distribution..."));
    
    // 分析資源分布並進行平衡
    TMap<EResourceType, float> CurrentDistribution = GetResourceStatistics();
    
    // 這裡可以添加更複雜的平衡算法
}
