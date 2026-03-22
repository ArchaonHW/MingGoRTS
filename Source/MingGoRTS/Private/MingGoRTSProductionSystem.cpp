#include "MingGoRTSProductionSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSProductionSystem::UMingGoRTSProductionSystem()
{
    bIsInitialized = false;
    
    // 初始化配置
    Config.GlobalProductionSpeed = 1.0f;
    Config.MaxOverclockEfficiency = 1.5f;
    Config.MaintenancePenalty = 0.3f;
    Config.bEnableAutoDistribution = true;
    Config.bEnableQualityControl = true;
    Config.MaxQueueSize = 10;
}

void UMingGoRTSProductionSystem::InitializeProductionSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("生產系統已經初始化"));
        return;
    }

    // 初始化民國時期生產配方
    InitializeRepublicanEraRecipes();

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("生產系統初始化完成，共載入 %d 個生產配方"), ProductionRecipes.Num());
}

TArray<EProductionCategory> UMingGoRTSProductionSystem::GetAllProductionCategories() const
{
    TArray<EProductionCategory> Categories;
    
    Categories.Add(EProductionCategory::Military);
    Categories.Add(EProductionCategory::Civilian);
    Categories.Add(EProductionCategory::Industrial);
    Categories.Add(EProductionCategory::Agricultural);
    Categories.Add(EProductionCategory::Medical);
    Categories.Add(EProductionCategory::Educational);
    Categories.Add(EProductionCategory::Infrastructure);
    Categories.Add(EProductionCategory::Luxury);
    Categories.Add(EProductionCategory::Research);
    Categories.Add(EProductionCategory::Special);
    
    return Categories;
}

TArray<FProductionRecipe> UMingGoRTSProductionSystem::GetRecipesByCategory(EProductionCategory Category) const
{
    TArray<FProductionRecipe> CategoryRecipes;
    
    for (const auto& RecipePair : ProductionRecipes)
    {
        const FProductionRecipe& Recipe = RecipePair.Value;
        if (Recipe.Category == Category)
        {
            CategoryRecipes.Add(Recipe);
        }
    }
    
    return CategoryRecipes;
}

FProductionRecipe UMingGoRTSProductionSystem::GetProductionRecipe(const FString& RecipeID) const
{
    if (const FProductionRecipe* Recipe = ProductionRecipes.Find(RecipeID))
    {
        return *Recipe;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到生產配方：%s"), *RecipeID);
    return FProductionRecipe();
}

FString UMingGoRTSProductionSystem::CreateProductionLine(const FString& ProductionLineName, const FString& BuildingID)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("生產系統未初始化"));
        return TEXT("");
    }

    FString ProductionLineID = GenerateProductionLineID(ProductionLineName);
    
    FProductionLine NewLine;
    NewLine.ProductionLineID = ProductionLineID;
    NewLine.ProductionLineName = ProductionLineName;
    NewLine.BuildingID = BuildingID;
    NewLine.Status = EProductionStatus::Idle;
    NewLine.CurrentEfficiency = 1.0f;
    NewLine.StartTime = FDateTime::Now();
    NewLine.LastUpdateTime = FDateTime::Now();
    
    ProductionLines.Add(ProductionLineID, NewLine);
    
    // 創建對應的生產隊列
    FProductionQueue NewQueue;
    NewQueue.QueueID = FString::Printf(TEXT("QUEUE_%s"), *ProductionLineID);
    NewQueue.ProductionLineID = ProductionLineID;
    ProductionQueues.Add(NewQueue.QueueID, NewQueue);
    
    RecordProductionHistory(FString::Printf(TEXT("創建生產線：%s"), *ProductionLineName));
    
    UE_LOG(LogTemp, Log, TEXT("創建生產線：%s，ID：%s"), *ProductionLineName, *ProductionLineID);
    return ProductionLineID;
}

bool UMingGoRTSProductionSystem::StartProduction(const FString& ProductionLineID, const FString& RecipeID, EProductionPriority Priority)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("生產系統未初始化"));
        return false;
    }

    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    if (!ProductionRecipes.Contains(RecipeID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產配方：%s"), *RecipeID);
        return false;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    FProductionRecipe Recipe = ProductionRecipes[RecipeID];
    
    // 檢查生產條件
    if (!CanProduceRecipe(ProductionLineID, RecipeID))
    {
        UE_LOG(LogTemp, Warning, TEXT("生產線 %s 無法生產配方 %s"), *ProductionLineID, *RecipeID);
        return false;
    }

    // 檢查資源是否足夠
    if (ResourceSystem)
    {
        for (const auto& ResourcePair : Recipe.InputResources)
        {
            EResourceType ResourceType = ResourcePair.Key;
            float RequiredAmount = ResourcePair.Value;
            
            // TODO: 檢查資源存儲是否足夠
        }
    }

    // 設置生產狀態
    ProductionLine.Status = EProductionStatus::Producing;
    ProductionLine.CurrentRecipeID = RecipeID;
    ProductionLine.Progress = 0.0f;
    ProductionLine.TimeRemaining = CalculateProductionTime(ProductionLineID, RecipeID);
    ProductionLine.Priority = Priority;
    ProductionLine.StartTime = FDateTime::Now();
    ProductionLine.LastUpdateTime = FDateTime::Now();
    
    // 應用效率因子
    ApplyEfficiencyFactors(ProductionLineID);
    
    // 廣播生產開始事件
    OnProductionStarted.Broadcast(ProductionLineID, RecipeID);
    
    RecordProductionHistory(FString::Printf(TEXT("開始生產：%s - %s"), *ProductionLine.ProductionLineName, *Recipe.RecipeName));
    
    UE_LOG(LogTemp, Log, TEXT("開始生產：%s，配方：%s"), *ProductionLine.ProductionLineName, *Recipe.RecipeName);
    return true;
}

bool UMingGoRTSProductionSystem::PauseProduction(const FString& ProductionLineID, const FString& Reason)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    
    if (ProductionLine.Status != EProductionStatus::Producing)
    {
        UE_LOG(LogTemp, Warning, TEXT("生產線 %s 當前不在生產狀態"), *ProductionLineID);
        return false;
    }

    ProductionLine.Status = EProductionStatus::Paused;
    ProductionLine.bIsPaused = true;
    
    // 廣播暫停事件
    OnProductionPaused.Broadcast(ProductionLineID, Reason);
    
    RecordProductionHistory(FString::Printf(TEXT("暫停生產：%s - %s"), *ProductionLine.ProductionLineName, *Reason));
    
    UE_LOG(LogTemp, Log, TEXT("暫停生產：%s，原因：%s"), *ProductionLine.ProductionLineName, *Reason);
    return true;
}

bool UMingGoRTSProductionSystem::ResumeProduction(const FString& ProductionLineID, const FString& Reason)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    
    if (ProductionLine.Status != EProductionStatus::Paused)
    {
        UE_LOG(LogTemp, Warning, TEXT("生產線 %s 當前不是暫停狀態"), *ProductionLineID);
        return false;
    }

    ProductionLine.Status = EProductionStatus::Producing;
    ProductionLine.bIsPaused = false;
    
    // 廣播恢復事件
    OnProductionResumed.Broadcast(ProductionLineID, Reason);
    
    RecordProductionHistory(FString::Printf(TEXT("恢復生產：%s - %s"), *ProductionLine.ProductionLineName, *Reason));
    
    UE_LOG(LogTemp, Log, TEXT("恢復生產：%s，原因：%s"), *ProductionLine.ProductionLineName, *Reason);
    return true;
}

bool UMingGoRTSProductionSystem::CancelProduction(const FString& ProductionLineID)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    
    // 重置生產線狀態
    ProductionLine.Status = EProductionStatus::Idle;
    ProductionLine.CurrentRecipeID = TEXT("");
    ProductionLine.Progress = 0.0f;
    ProductionLine.TimeRemaining = 0.0f;
    ProductionLine.bIsPaused = false;
    
    // 廣播取消事件
    OnProductionFailed.Broadcast(ProductionLineID, TEXT("生產已取消"));
    
    RecordProductionHistory(FString::Printf(TEXT("取消生產：%s"), *ProductionLine.ProductionLineName));
    
    UE_LOG(LogTemp, Log, TEXT("取消生產：%s"), *ProductionLine.ProductionLineName);
    return true;
}

bool UMingGoRTSProductionSystem::SetProductionPriority(const FString& ProductionLineID, EProductionPriority Priority)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    ProductionLine.Priority = Priority;
    
    UE_LOG(LogTemp, Log, TEXT("設置生產線 %s 優先級為：%d"), *ProductionLine.ProductionLineName, static_cast<int32>(Priority));
    return true;
}

bool UMingGoRTSProductionSystem::AddToProductionQueue(const FString& ProductionLineID, const FString& RecipeID)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    if (!ProductionRecipes.Contains(RecipeID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產配方：%s"), *RecipeID);
        return false;
    }

    // 找到對應的生產隊列
    FString QueueID = FString::Printf(TEXT("QUEUE_%s"), *ProductionLineID);
    if (!ProductionQueues.Contains(QueueID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產隊列：%s"), *QueueID);
        return false;
    }

    FProductionQueue& Queue = ProductionQueues[QueueID];
    
    // 檢查隊列大小限制
    if (Queue.RecipeQueue.Num() >= Config.MaxQueueSize)
    {
        UE_LOG(LogTemp, Warning, TEXT("生產隊列已滿"));
        return false;
    }

    // 添加到隊列
    Queue.RecipeQueue.Add(RecipeID);
    
    // 如果生產線空閒且設置為自動開始，則開始生產
    if (Queue.bAutoStart && ProductionLines[ProductionLineID].Status == EProductionStatus::Idle)
    {
        StartProduction(ProductionLineID, RecipeID);
    }
    
    // 廣播隊列變更事件
    OnProductionQueueChanged.Broadcast(QueueID, Queue.RecipeQueue);
    
    UE_LOG(LogTemp, Log, TEXT("添加配方到生產隊列：%s"), *RecipeID);
    return true;
}

bool UMingGoRTSProductionSystem::RemoveFromProductionQueue(const FString& ProductionLineID, int32 QueueIndex)
{
    FString QueueID = FString::Printf(TEXT("QUEUE_%s"), *ProductionLineID);
    if (!ProductionQueues.Contains(QueueID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產隊列：%s"), *QueueID);
        return false;
    }

    FProductionQueue& Queue = ProductionQueues[QueueID];
    
    if (!Queue.RecipeQueue.IsValidIndex(QueueIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("無效的隊列索引：%d"), QueueIndex);
        return false;
    }

    Queue.RecipeQueue.RemoveAt(QueueIndex);
    
    // 廣播隊列變更事件
    OnProductionQueueChanged.Broadcast(QueueID, Queue.RecipeQueue);
    
    UE_LOG(LogTemp, Log, TEXT("從生產隊列移除索引 %d"), QueueIndex);
    return true;
}

bool UMingGoRTSProductionSystem::ClearProductionQueue(const FString& ProductionLineID)
{
    FString QueueID = FString::Printf(TEXT("QUEUE_%s"), *ProductionLineID);
    if (!ProductionQueues.Contains(QueueID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產隊列：%s"), *QueueID);
        return false;
    }

    FProductionQueue& Queue = ProductionQueues[QueueID];
    Queue.RecipeQueue.Empty();
    Queue.CurrentIndex = 0;
    
    // 廣播隊列變更事件
    OnProductionQueueChanged.Broadcast(QueueID, Queue.RecipeQueue);
    
    UE_LOG(LogTemp, Log, TEXT("清空生產隊列：%s"), *ProductionLineID);
    return true;
}

FProductionLine UMingGoRTSProductionSystem::GetProductionLine(const FString& ProductionLineID) const
{
    if (const FProductionLine* ProductionLine = ProductionLines.Find(ProductionLineID))
    {
        return *ProductionLine;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到生產線：%s"), *ProductionLineID);
    return FProductionLine();
}

FProductionQueue UMingGoRTSProductionSystem::GetProductionQueue(const FString& ProductionLineID) const
{
    FString QueueID = FString::Printf(TEXT("QUEUE_%s"), *ProductionLineID);
    if (const FProductionQueue* Queue = ProductionQueues.Find(QueueID))
    {
        return *Queue;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到生產隊列：%s"), *QueueID);
    return FProductionQueue();
}

TArray<FProductionLine> UMingGoRTSProductionSystem::GetAllProductionLines() const
{
    TArray<FProductionLine> AllLines;
    AllLines.Reserve(ProductionLines.Num());
    
    for (const auto& LinePair : ProductionLines)
    {
        AllLines.Add(LinePair.Value);
    }
    
    return AllLines;
}

float UMingGoRTSProductionSystem::CalculateProductionEfficiency(const FString& ProductionLineID) const
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        return 1.0f;
    }

    const FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    return CalculateFinalEfficiency(ProductionLine);
}

float UMingGoRTSProductionSystem::CalculateProductionTime(const FString& ProductionLineID, const FString& RecipeID) const
{
    if (!ProductionLines.Contains(ProductionLineID) || !ProductionRecipes.Contains(RecipeID))
    {
        return 0.0f;
    }

    const FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    const FProductionRecipe& Recipe = ProductionRecipes[RecipeID];
    
    float BaseTime = Recipe.ProductionTime;
    float Efficiency = CalculateFinalEfficiency(ProductionLine);
    float GlobalSpeed = Config.GlobalProductionSpeed;
    
    // 應用類別加成
    float CategoryBonus = 1.0f;
    if (const float* Bonus = Config.CategoryBonus.Find(Recipe.Category))
    {
        CategoryBonus = *Bonus;
    }
    
    float FinalTime = BaseTime / (Efficiency * GlobalSpeed * CategoryBonus);
    return FMath::Max(FinalTime, 0.1f);
}

bool UMingGoRTSProductionSystem::CanProduceRecipe(const FString& ProductionLineID, const FString& RecipeID) const
{
    if (!ProductionLines.Contains(ProductionLineID) || !ProductionRecipes.Contains(RecipeID))
    {
        return false;
    }

    const FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    const FProductionRecipe& Recipe = ProductionRecipes[RecipeID];
    
    // 檢查生產線狀態
    if (ProductionLine.Status != EProductionStatus::Idle && ProductionLine.Status != EProductionStatus::Completed)
    {
        return false;
    }
    
    // 檢查建築要求
    for (EResourceType RequiredBuilding : Recipe.RequiredBuildings)
    {
        // TODO: 檢查生產線建築是否符合要求
    }
    
    // 檢查技能要求
    // TODO: 檢查操作員技能等級
    
    // 檢查科技要求
    for (const FString& RequiredTech : Recipe.RequiredTechnologies)
    {
        // TODO: 檢查科技是否已解鎖
    }
    
    return true;
}

FProductionStatistics UMingGoRTSProductionSystem::GetProductionStatistics() const
{
    return Statistics;
}

void UMingGoRTSProductionSystem::UpdateProductionProgress(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    TArray<FString> CompletedProductionLines;
    
    for (auto& LinePair : ProductionLines)
    {
        FString ProductionLineID = LinePair.Key;
        FProductionLine& ProductionLine = LinePair.Value;
        
        if (ProductionLine.Status != EProductionStatus::Producing || ProductionLine.bIsPaused)
        {
            continue;
        }
        
        // 更新進度
        float TimeIncrement = DeltaTime * Config.GlobalProductionSpeed;
        ProductionLine.TimeRemaining -= TimeIncrement;
        ProductionLine.LastUpdateTime = FDateTime::Now();
        
        // 計算進度百分比
        float TotalTime = CalculateProductionTime(ProductionLineID, ProductionLine.CurrentRecipeID);
        float ElapsedTime = TotalTime - ProductionLine.TimeRemaining;
        ProductionLine.Progress = FMath::Clamp((ElapsedTime / TotalTime) * 100.0f, 0.0f, 100.0f);
        
        // 廣播進度更新
        OnProductionProgress.Broadcast(ProductionLineID, ProductionLine.Progress, ProductionLine.TimeRemaining);
        
        // 檢查是否完成
        if (ProductionLine.TimeRemaining <= 0.0f)
        {
            CompletedProductionLines.Add(ProductionLineID);
        }
    }
    
    // 處理已完成的生產
    for (const FString& ProductionLineID : CompletedProductionLines)
    {
        HandleProductionCompleted(ProductionLineID);
    }
}

bool UMingGoRTSProductionSystem::UpgradeProductionLine(const FString& ProductionLineID)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    
    if (ProductionLine.Status != EProductionStatus::Idle)
    {
        UE_LOG(LogTemp, Warning, TEXT("生產線 %s 必須空閒才能升級"), *ProductionLineID);
        return false;
    }

    ProductionLine.Status = EProductionStatus::Upgrading;
    
    // TODO: 實現升級邏輯（消耗資源、時間等）
    
    ProductionLine.Status = EProductionStatus::Idle;
    
    RecordProductionHistory(FString::Printf(TEXT("升級生產線：%s"), *ProductionLine.ProductionLineName));
    
    UE_LOG(LogTemp, Log, TEXT("升級生產線：%s"), *ProductionLine.ProductionLineName);
    return true;
}

bool UMingGoRTSProductionSystem::MaintainProductionLine(const FString& ProductionLineID)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    
    if (ProductionLine.Status == EProductionStatus::Maintenance)
    {
        UE_LOG(LogTemp, Warning, TEXT("生產線 %s 已在維護中"), *ProductionLineID);
        return false;
    }

    ProductionLine.Status = EProductionStatus::Maintenance;
    
    // 應用維護懲罰
    ProductionLine.EfficiencyFactors.Add(EProductionEfficiencyFactor::Maintenance, Config.MaintenancePenalty);
    
    RecordProductionHistory(FString::Printf(TEXT("維護生產線：%s"), *ProductionLine.ProductionLineName));
    
    UE_LOG(LogTemp, Log, TEXT("維護生產線：%s"), *ProductionLine.ProductionLineName);
    return true;
}

bool UMingGoRTSProductionSystem::SetAutomation(const FString& ProductionLineID, bool bIsAutomated)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到生產線：%s"), *ProductionLineID);
        return false;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    ProductionLine.bIsAutomated = bIsAutomated;
    
    // 自動化影響效率
    if (bIsAutomated)
    {
        ProductionLine.EfficiencyFactors.Add(EProductionEfficiencyFactor::BuildingBonus, 0.2f);
    }
    else
    {
        ProductionLine.EfficiencyFactors.Remove(EProductionEfficiencyFactor::BuildingBonus);
    }
    
    // 重新計算效率
    ProductionLine.CurrentEfficiency = CalculateFinalEfficiency(ProductionLine);
    
    RecordProductionHistory(FString::Printf(TEXT("設置生產線 %s 自動化：%s"), *ProductionLine.ProductionLineName, bIsAutomated ? TEXT("開啟") : TEXT("關閉")));
    
    UE_LOG(LogTemp, Log, TEXT("設置生產線 %s 自動化：%s"), *ProductionLine.ProductionLineName, bIsAutomated ? TEXT("開啟") : TEXT("關閉"));
    return true;
}

FProductionSystemConfig UMingGoRTSProductionSystem::GetProductionConfig() const
{
    return Config;
}

void UMingGoRTSProductionSystem::UpdateProductionConfig(const FProductionSystemConfig& NewConfig)
{
    Config = NewConfig;
    
    // 重新計算所有生產線的效率
    for (auto& LinePair : ProductionLines)
    {
        FString ProductionLineID = LinePair.Key;
        ApplyEfficiencyFactors(ProductionLineID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("生產系統配置已更新"));
}

bool UMingGoRTSProductionSystem::SaveProductionData(const FString& SaveSlotName)
{
    // TODO: 實現生產數據保存
    UE_LOG(LogTemp, Log, TEXT("保存生產數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSProductionSystem::LoadProductionData(const FString& SaveSlotName)
{
    // TODO: 實現生產數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入生產數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSProductionSystem::InitializeRepublicanEraRecipes()
{
    // 創建各類別生產配方
    CreateMilitaryProductionRecipes();
    CreateCivilianProductionRecipes();
    CreateIndustrialProductionRecipes();
    CreateAgriculturalProductionRecipes();
    CreateMedicalProductionRecipes();
    CreateEducationalProductionRecipes();
    CreateInfrastructureProductionRecipes();
    CreateLuxuryProductionRecipes();
}

void UMingGoRTSProductionSystem::CreateMilitaryProductionRecipes()
{
    // 步槍生產配方
    FProductionRecipe RifleProduction;
    RifleProduction.RecipeID = GenerateRecipeID(TEXT("RifleProduction"), EProductionCategory::Military);
    RifleProduction.RecipeName = TEXT("步槍生產");
    RifleProduction.Description = TEXT("生產標準制式步槍，裝備步兵部隊");
    RifleProduction.Category = EProductionCategory::Military;
    RifleProduction.ProductionTime = 180.0f; // 3分鐘
    RifleProduction.BaseEfficiency = 1.0f;
    RifleProduction.MinSkillLevel = 1;
    RifleProduction.DefaultQuality = EProductionQuality::Normal;
    RifleProduction.bIsRepeatable = true;
    RifleProduction.HistoricalContext = TEXT("民國時期，步槍是軍隊的標準裝備");
    
    // 輸入資源
    RifleProduction.InputResources.Add(EResourceType::Metal, 5.0f);
    RifleProduction.InputResources.Add(EResourceType::Wood, 2.0f);
    RifleProduction.InputResources.Add(EResourceType::Tools, 1.0f);
    
    // 輸出資源
    RifleProduction.OutputResources.Add(EResourceType::Weapons, 10.0f);
    
    // 需要建築
    RifleProduction.RequiredBuildings.Add(EResourceType::Factory);
    
    ProductionRecipes.Add(RifleProduction.RecipeID, RifleProduction);
    
    // 子彈生產配方
    FProductionRecipe AmmunitionProduction;
    AmmunitionProduction.RecipeID = GenerateRecipeID(TEXT("AmmunitionProduction"), EProductionCategory::Military);
    AmmunitionProduction.RecipeName = TEXT("子彈生產");
    AmmunitionProduction.Description = TEXT("生產各類子彈，供應軍隊作戰需求");
    AmmunitionProduction.Category = EProductionCategory::Military;
    AmmunitionProduction.ProductionTime = 120.0f; // 2分鐘
    AmmunitionProduction.BaseEfficiency = 1.2f;
    AmmunitionProduction.MinSkillLevel = 1;
    AmmunitionProduction.DefaultQuality = EProductionQuality::Normal;
    AmmunitionProduction.bIsRepeatable = true;
    AmmunitionProduction.HistoricalContext = TEXT("彈藥補給是戰爭持續的關鍵");
    
    // 輸入資源
    AmmunitionProduction.InputResources.Add(EResourceType::Metal, 3.0f);
    AmmunitionProduction.InputResources.Add(EResourceType::Gunpowder, 2.0f);
    
    // 輸出資源
    AmmunitionProduction.OutputResources.Add(EResourceType::Ammunition, 50.0f);
    
    ProductionRecipes.Add(AmmunitionProduction.RecipeID, AmmunitionProduction);
}

void UMingGoRTSProductionSystem::CreateCivilianProductionRecipes()
{
    // 服裝生產配方
    FProductionRecipe ClothingProduction;
    ClothingProduction.RecipeID = GenerateRecipeID(TEXT("ClothingProduction"), EProductionCategory::Civilian);
    ClothingProduction.RecipeName = TEXT("服裝生產");
    ClothingProduction.Description = TEXT("生產民用服裝，滿足民眾日常需求");
    ClothingProduction.Category = EProductionCategory::Civilian;
    ClothingProduction.ProductionTime = 150.0f; // 2.5分鐘
    ClothingProduction.BaseEfficiency = 1.0f;
    ClothingProduction.MinSkillLevel = 0;
    ClothingProduction.DefaultQuality = EProductionQuality::Normal;
    ClothingProduction.bIsRepeatable = true;
    ClothingProduction.HistoricalContext = TEXT("民國時期，紡織業是重要的民生工業");
    
    // 輸入資源
    ClothingProduction.InputResources.Add(EResourceType::Cotton, 3.0f);
    ClothingProduction.InputResources.Add(EResourceType::Silk, 1.0f);
    
    // 輸出資源
    ClothingProduction.OutputResources.Add(EResourceType::Clothing, 20.0f);
    
    ProductionRecipes.Add(ClothingProduction.RecipeID, ClothingProduction);
}

void UMingGoRTSProductionSystem::CreateIndustrialProductionRecipes()
{
    // 工具生產配方
    FProductionRecipe ToolsProduction;
    ToolsProduction.RecipeID = GenerateRecipeID(TEXT("ToolsProduction"), EProductionCategory::Industrial);
    ToolsProduction.RecipeName = TEXT("工具生產");
    ToolsProduction.Description = TEXT("生產各種工具，支持工業和農業發展");
    ToolsProduction.Category = EProductionCategory::Industrial;
    ToolsProduction.ProductionTime = 200.0f; // 3.33分鐘
    ToolsProduction.BaseEfficiency = 0.9f;
    ToolsProduction.MinSkillLevel = 1;
    ToolsProduction.DefaultQuality = EProductionQuality::Normal;
    ToolsProduction.bIsRepeatable = true;
    ToolsProduction.HistoricalContext = TEXT("工具製造是工業化的基礎");
    
    // 輸入資源
    ToolsProduction.InputResources.Add(EResourceType::Metal, 4.0f);
    ToolsProduction.InputResources.Add(EResourceType::Wood, 3.0f);
    
    // 輸出資源
    ToolsProduction.OutputResources.Add(EResourceType::Tools, 15.0f);
    
    ProductionRecipes.Add(ToolsProduction.RecipeID, ToolsProduction);
}

void UMingGoRTSProductionSystem::CreateAgriculturalProductionRecipes()
{
    // 食品加工配方
    FProductionRecipe FoodProcessing;
    FoodProcessing.RecipeID = GenerateRecipeID(TEXT("FoodProcessing"), EProductionCategory::Agricultural);
    FoodProcessing.RecipeName = TEXT("食品加工");
    FoodProcessing.Description = TEXT("加工農產品，延長保存時間並提高價值");
    FoodProcessing.Category = EProductionCategory::Agricultural;
    FoodProcessing.ProductionTime = 100.0f; // 1.67分鐘
    FoodProcessing.BaseEfficiency = 1.1f;
    FoodProcessing.MinSkillLevel = 0;
    FoodProcessing.DefaultQuality = EProductionQuality::Normal;
    FoodProcessing.bIsRepeatable = true;
    FoodProcessing.HistoricalContext = TEXT("食品加工是農業經濟的重要環節");
    
    // 輸入資源
    FoodProcessing.InputResources.Add(EResourceType::Food, 5.0f);
    FoodProcessing.InputResources.Add(EResourceType::Salt, 1.0f);
    
    // 輸出資源
    FoodProcessing.OutputResources.Add(EResourceType::ProcessedFood, 8.0f);
    
    ProductionRecipes.Add(FoodProcessing.RecipeID, FoodProcessing);
}

void UMingGoRTSProductionSystem::CreateMedicalProductionRecipes()
{
    // 藥品生產配方
    FProductionRecipe MedicineProduction;
    MedicineProduction.RecipeID = GenerateRecipeID(TEXT("MedicineProduction"), EProductionCategory::Medical);
    MedicineProduction.RecipeName = TEXT("藥品生產");
    MedicineProduction.Description = TEXT("生產基礎藥品，改善公共衛生");
    MedicineProduction.Category = EProductionCategory::Medical;
    MedicineProduction.ProductionTime = 240.0f; // 4分鐘
    MedicineProduction.BaseEfficiency = 0.8f;
    MedicineProduction.MinSkillLevel = 2;
    MedicineProduction.DefaultQuality = EProductionQuality::Normal;
    MedicineProduction.bIsRepeatable = true;
    MedicineProduction.HistoricalContext = TEXT("民國時期，西方醫學的引進改善了公共衛生");
    
    // 輸入資源
    MedicineProduction.InputResources.Add(EResourceType::Herbs, 4.0f);
    MedicineProduction.InputResources.Add(EResourceType::Alcohol, 2.0f);
    
    // 輸出資源
    MedicineProduction.OutputResources.Add(EResourceType::Medicine, 10.0f);
    
    ProductionRecipes.Add(MedicineProduction.RecipeID, MedicineProduction);
}

void UMingGoRTSProductionSystem::CreateEducationalProductionRecipes()
{
    // 書籍印刷配方
    FProductionRecipe BookPrinting;
    BookPrinting.RecipeID = GenerateRecipeID(TEXT("BookPrinting"), EProductionCategory::Educational);
    BookPrinting.RecipeName = TEXT("書籍印刷");
    BookPrinting.Description = TEXT("印刷書籍，傳播知識和教育");
    BookPrinting.Category = EProductionCategory::Educational;
    BookPrinting.ProductionTime = 300.0f; // 5分鐘
    BookPrinting.BaseEfficiency = 0.7f;
    BookPrinting.MinSkillLevel = 2;
    BookPrinting.DefaultQuality = EProductionQuality::Good;
    BookPrinting.bIsRepeatable = true;
    BookPrinting.HistoricalContext = TEXT("印刷業的發展促進了知識傳播和教育普及");
    
    // 輸入資源
    BookPrinting.InputResources.Add(EResourceType::Paper, 10.0f);
    BookPrinting.InputResources.Add(EResourceType::Ink, 2.0f);
    
    // 輸出資源
    BookPrinting.OutputResources.Add(EResourceType::Books, 25.0f);
    
    ProductionRecipes.Add(BookPrinting.RecipeID, BookPrinting);
}

void UMingGoRTSProductionSystem::CreateInfrastructureProductionRecipes()
{
    // 建材生產配方
    FProductionRecipe BuildingMaterials;
    BuildingMaterials.RecipeID = GenerateRecipeID(TEXT("BuildingMaterials"), EProductionCategory::Infrastructure);
    BuildingMaterials.RecipeName = TEXT("建材生產");
    BuildingMaterials.Description = TEXT("生產建築材料，支持基礎設施建設");
    BuildingMaterials.Category = EProductionCategory::Infrastructure;
    BuildingMaterials.ProductionTime = 250.0f; // 4.17分鐘
    BuildingMaterials.BaseEfficiency = 1.0f;
    BuildingMaterials.MinSkillLevel = 1;
    BuildingMaterials.DefaultQuality = EProductionQuality::Normal;
    BuildingMaterials.bIsRepeatable = true;
    BuildingMaterials.HistoricalContext = TEXT("建築材料是城市化和基礎設施建設的基礎");
    
    // 輸入資源
    BuildingMaterials.InputResources.Add(EResourceType::Stone, 8.0f);
    BuildingMaterials.InputResources.Add(EResourceType::Cement, 4.0f);
    
    // 輸出資源
    BuildingMaterials.OutputResources.Add(EResourceType::BuildingMaterials, 30.0f);
    
    ProductionRecipes.Add(BuildingMaterials.RecipeID, BuildingMaterials);
}

void UMingGoRTSProductionSystem::CreateLuxuryProductionRecipes()
{
    // 茶葉加工配方
    FProductionRecipe TeaProcessing;
    TeaProcessing.RecipeID = GenerateRecipeID(TEXT("TeaProcessing"), EProductionCategory::Luxury);
    TeaProcessing.RecipeName = TEXT("茶葉加工");
    TeaProcessing.Description = TEXT("加工高品質茶葉，生產奢侈品茶");
    TeaProcessing.Category = EProductionCategory::Luxury;
    TeaProcessing.ProductionTime = 180.0f; // 3分鐘
    TeaProcessing.BaseEfficiency = 0.9f;
    TeaProcessing.MinSkillLevel = 2;
    TeaProcessing.DefaultQuality = EProductionQuality::Good;
    TeaProcessing.bIsRepeatable = true;
    TeaProcessing.HistoricalContext = TEXT("茶葉是中國重要的傳統出口商品");
    
    // 輸入資源
    TeaProcessing.InputResources.Add(EResourceType::TeaLeaves, 6.0f);
    
    // 輸出資源
    TeaProcessing.OutputResources.Add(EResourceType::Tea, 20.0f);
    
    ProductionRecipes.Add(TeaProcessing.RecipeID, TeaProcessing);
}

void UMingGoRTSProductionSystem::HandleProductionCompleted(const FString& ProductionLineID)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        return;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    FProductionRecipe Recipe = ProductionRecipes[ProductionLine.CurrentRecipeID];
    
    // 消耗輸入資源
    if (ResourceSystem)
    {
        for (const auto& ResourcePair : Recipe.InputResources)
        {
            // TODO: 從資源存儲中扣除輸入資源
        }
    }
    
    // 生產輸出資源
    if (ResourceSystem)
    {
        for (const auto& ResourcePair : Recipe.OutputResources)
        {
            // TODO: 將輸出資源添加到資源存儲
        }
    }
    
    // 更新統計
    ProductionLine.ProductionCount++;
    Statistics.TotalProductionRuns++;
    Statistics.SuccessfulRuns++;
    Statistics.LastProductionTime = FDateTime::Now();
    
    for (const auto& ResourcePair : Recipe.OutputResources)
    {
        Statistics.TotalProduced.FindOrAdd(ResourcePair.Key) += ResourcePair.Value;
        Statistics.CategoryProduction.FindOrAdd(Recipe.Category) += ResourcePair.Value;
    }
    
    for (const auto& ResourcePair : Recipe.InputResources)
    {
        Statistics.TotalConsumed.FindOrAdd(ResourcePair.Key) += ResourcePair.Value;
    }
    
    // 廣播完成事件
    OnProductionCompleted.Broadcast(ProductionLineID, Recipe.OutputResources);
    
    // 處理生產隊列
    ProcessProductionQueue(ProductionLineID);
    
    // 重置生產線狀態
    ProductionLine.Status = EProductionStatus::Completed;
    ProductionLine.CurrentRecipeID = TEXT("");
    ProductionLine.Progress = 0.0f;
    ProductionLine.TimeRemaining = 0.0f;
    
    RecordProductionHistory(FString::Printf(TEXT("完成生產：%s - %s"), *ProductionLine.ProductionLineName, *Recipe.RecipeName));
}

void UMingGoRTSProductionSystem::HandleProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        return;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    
    // 更新統計
    Statistics.TotalProductionRuns++;
    Statistics.FailedRuns++;
    
    // 重置生產線狀態
    ProductionLine.Status = EProductionStatus::Failed;
    ProductionLine.CurrentRecipeID = TEXT("");
    ProductionLine.Progress = 0.0f;
    ProductionLine.TimeRemaining = 0.0f;
    
    // 廣播失敗事件
    OnProductionFailed.Broadcast(ProductionLineID, ErrorMessage);
    
    RecordProductionHistory(FString::Printf(TEXT("生產失敗：%s - %s"), *ProductionLine.ProductionLineName, *ErrorMessage));
}

void UMingGoRTSProductionSystem::UpdateProductionLineStatus(const FString& ProductionLineID)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        return;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    
    // 根據條件更新狀態
    if (ProductionLine.Status == EProductionStatus::Producing && ProductionLine.TimeRemaining <= 0.0f)
    {
        ProductionLine.Status = EProductionStatus::Completed;
    }
}

void UMingGoRTSProductionSystem::ProcessProductionQueue(const FString& ProductionLineID)
{
    FString QueueID = FString::Printf(TEXT("QUEUE_%s"), *ProductionLineID);
    if (!ProductionQueues.Contains(QueueID))
    {
        return;
    }

    FProductionQueue& Queue = ProductionQueues[QueueID];
    
    // 如果隊列為空，設置為空閒
    if (Queue.RecipeQueue.Num() == 0)
    {
        if (FProductionLine* ProductionLine = ProductionLines.Find(ProductionLineID))
        {
            ProductionLine->Status = EProductionStatus::Idle;
        }
        return;
    }
    
    // 如果是循環隊列，移動到下一個
    if (Queue.bIsLooping)
    {
        Queue.CurrentIndex = (Queue.CurrentIndex + 1) % Queue.RecipeQueue.Num();
    }
    else
    {
        // 移除已完成的配方
        if (Queue.CurrentIndex < Queue.RecipeQueue.Num())
        {
            Queue.RecipeQueue.RemoveAt(Queue.CurrentIndex);
            if (Queue.CurrentIndex >= Queue.RecipeQueue.Num())
            {
                Queue.CurrentIndex = 0;
            }
        }
    }
    
    // 開始下一個生產
    if (Queue.RecipeQueue.Num() > 0 && Queue.bAutoStart)
    {
        FString NextRecipeID = Queue.RecipeQueue[Queue.CurrentIndex];
        StartProduction(ProductionLineID, NextRecipeID);
    }
}

void UMingGoRTSProductionSystem::ApplyEfficiencyFactors(const FString& ProductionLineID)
{
    if (!ProductionLines.Contains(ProductionLineID))
    {
        return;
    }

    FProductionLine& ProductionLine = ProductionLines[ProductionLineID];
    ProductionLine.CurrentEfficiency = CalculateFinalEfficiency(ProductionLine);
}

float UMingGoRTSProductionSystem::CalculateFinalEfficiency(const FProductionLine& ProductionLine) const
{
    float FinalEfficiency = 1.0f;
    
    // 應用所有效率因子
    for (const auto& FactorPair : ProductionLine.EfficiencyFactors)
    {
        FinalEfficiency *= FactorPair.Value;
    }
    
    // 限制最大效率
    float MaxEfficiency = Config.MaxOverclockEfficiency;
    FinalEfficiency = FMath::Clamp(FinalEfficiency, 0.1f, MaxEfficiency);
    
    return FinalEfficiency;
}

FString UMingGoRTSProductionSystem::GetProductionCategoryName(EProductionCategory Category) const
{
    const UEnum* EnumPtr = StaticEnum<EProductionCategory>();
    if (EnumPtr)
    {
        return EnumPtr->GetNameStringByValue(static_cast<int64>(Category));
    }
    return TEXT("Unknown");
}

FString UMingGoRTSProductionSystem::GetProductionStatusName(EProductionStatus Status) const
{
    const UEnum* EnumPtr = StaticEnum<EProductionStatus>();
    if (EnumPtr)
    {
        return EnumPtr->GetNameStringByValue(static_cast<int64>(Status));
    }
    return TEXT("Unknown");
}

bool UMingGoRTSProductionSystem::ValidateRecipeData(const FProductionRecipe& Recipe) const
{
    // 基本驗證
    if (Recipe.RecipeID.IsEmpty() || Recipe.RecipeName.IsEmpty())
    {
        return false;
    }
    
    if (Recipe.ProductionTime <= 0.0f)
    {
        return false;
    }
    
    if (Recipe.InputResources.Num() == 0 || Recipe.OutputResources.Num() == 0)
    {
        return false;
    }
    
    return true;
}

FString UMingGoRTSProductionSystem::GenerateProductionLineID(const FString& BaseName) const
{
    return FString::Printf(TEXT("PROD_LINE_%s_%d"), *BaseName, FMath::RandRange(1000, 9999));
}

FString UMingGoRTSProductionSystem::GenerateRecipeID(const FString& BaseName, EProductionCategory Category) const
{
    FString CategoryName = GetProductionCategoryName(Category);
    return FString::Printf(TEXT("RECIPE_%s_%s_%d"), *CategoryName, *BaseName, FMath::RandRange(1000, 9999));
}

void UMingGoRTSProductionSystem::RecordProductionHistory(const FString& Event)
{
    ProductionHistory.Add(Event);
    
    // 限制歷史記錄數量
    if (ProductionHistory.Num() > 1000)
    {
        ProductionHistory.RemoveAt(0);
    }
}
