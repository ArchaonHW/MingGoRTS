#include "MingGoRTSTechTree.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSTechTree::UMingGoRTSTechTree()
{
    bIsInitialized = false;
    
    // 初始化配置
    Config.GlobalResearchSpeed = 1.0f;
    Config.MaxConcurrentResearch = 1;
    Config.bEnableTechSharing = false;
    Config.TechSharingEfficiency = 0.5f;
}

void UMingGoRTSTechTree::InitializeTechTree()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("科技樹系統已經初始化"));
        return;
    }

    // 初始化民國時期科技樹
    InitializeRepublicanEraTechTree();

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("科技樹系統初始化完成，共載入 %d 個科技節點"), TechNodes.Num());
}

TArray<ETechCategory> UMingGoRTSTechTree::GetAllTechCategories() const
{
    TArray<ETechCategory> Categories;
    
    Categories.Add(ETechCategory::Military);
    Categories.Add(ETechCategory::Economic);
    Categories.Add(ETechCategory::Diplomatic);
    Categories.Add(ETechCategory::Social);
    Categories.Add(ETechCategory::Industrial);
    Categories.Add(ETechCategory::Agricultural);
    Categories.Add(ETechCategory::Medical);
    Categories.Add(ETechCategory::Education);
    Categories.Add(ETechCategory::Infrastructure);
    Categories.Add(ETechCategory::Communication);
    
    return Categories;
}

TArray<FTechNode> UMingGoRTSTechTree::GetTechsByCategory(ETechCategory Category) const
{
    TArray<FTechNode> CategoryTechs;
    
    for (const auto& TechPair : TechNodes)
    {
        const FTechNode& Tech = TechPair.Value;
        if (Tech.Category == Category)
        {
            CategoryTechs.Add(Tech);
        }
    }
    
    return CategoryTechs;
}

FTechNode UMingGoRTSTechTree::GetTechNode(const FString& TechID) const
{
    if (const FTechNode* Tech = TechNodes.Find(TechID))
    {
        return *Tech;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到科技節點：%s"), *TechID);
    return FTechNode();
}

bool UMingGoRTSTechTree::StartResearch(const FString& TechID, const FString& BuildingID)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("科技樹系統未初始化"));
        return false;
    }

    if (!TechNodes.Contains(TechID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到科技節點：%s"), *TechID);
        return false;
    }

    FTechNode& Tech = TechNodes[TechID];
    
    // 檢查是否可以研發
    if (!CanResearchTech(TechID))
    {
        UE_LOG(LogTemp, Warning, TEXT("科技 %s 當前無法研發"), *TechID);
        return false;
    }

    // 檢查並行研發限制
    if (ActiveResearchProjects.Num() >= Config.MaxConcurrentResearch)
    {
        UE_LOG(LogTemp, Warning, TEXT("已達到最大並行研發數量限制"));
        return false;
    }

    // 創建研發進度
    FTechResearchProgress Progress;
    Progress.TechID = TechID;
    Progress.Progress = 0.0f;
    Progress.TimeRemaining = CalculateResearchTime(TechID);
    Progress.StartTime = FDateTime::Now();
    Progress.ResearchingBuildingID = BuildingID;
    Progress.bIsPaused = false;

    ResearchProgress.Add(TechID, Progress);
    ActiveResearchProjects.Add(TechID);
    
    // 更新科技狀態
    Tech.Status = ETechStatus::Researching;
    
    // 廣播研發開始事件
    OnTechResearchStarted.Broadcast(TechID, BuildingID);
    
    // 記錄歷史
    RecordTechHistory(FString::Printf(TEXT("開始研發科技：%s"), *Tech.TechName));
    
    UE_LOG(LogTemp, Log, TEXT("開始研發科技：%s，建築：%s"), *Tech.TechName, *BuildingID);
    return true;
}

bool UMingGoRTSTechTree::PauseResearch(const FString& TechID)
{
    if (!ResearchProgress.Contains(TechID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到研發進度：%s"), *TechID);
        return false;
    }

    FTechResearchProgress& Progress = ResearchProgress[TechID];
    Progress.bIsPaused = true;
    
    if (FTechNode* Tech = TechNodes.Find(TechID))
    {
        Tech->Status = ETechStatus::Available;
    }
    
    // 廣播暫停事件
    OnTechResearchPaused.Broadcast(TechID);
    
    // 記錄歷史
    RecordTechHistory(FString::Printf(TEXT("暫停研發科技：%s"), *TechID));
    
    UE_LOG(LogTemp, Log, TEXT("暫停研發科技：%s"), *TechID);
    return true;
}

bool UMingGoRTSTechTree::CancelResearch(const FString& TechID)
{
    if (!ResearchProgress.Contains(TechID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到研發進度：%s"), *TechID);
        return false;
    }

    // 移除研發進度
    ResearchProgress.Remove(TechID);
    ActiveResearchProjects.Remove(TechID);
    
    if (FTechNode* Tech = TechNodes.Find(TechID))
    {
        Tech->Status = ETechStatus::Available;
    }
    
    // 廣播取消事件
    OnTechResearchCancelled.Broadcast(TechID);
    
    // 記錄歷史
    RecordTechHistory(FString::Printf(TEXT("取消研發科技：%s"), *TechID));
    
    UE_LOG(LogTemp, Log, TEXT("取消研發科技：%s"), *TechID);
    return true;
}

bool UMingGoRTSTechTree::CompleteResearch(const FString& TechID)
{
    if (!TechNodes.Contains(TechID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到科技節點：%s"), *TechID);
        return false;
    }

    FTechNode& Tech = TechNodes[TechID];
    
    // 標記為已完成
    Tech.Status = ETechStatus::Completed;
    CompletedTechs.Add(TechID);
    
    // 移除研發進度
    ResearchProgress.Remove(TechID);
    ActiveResearchProjects.Remove(TechID);
    
    // 應用科技效果
    ApplyTechEffects(TechID);
    
    // 檢查並解鎖新科技
    CheckAndUnlockNewTechs();
    
    // 廣播完成事件
    OnTechResearchCompleted.Broadcast(TechID, Tech.Effects);
    
    // 記錄歷史
    RecordTechHistory(FString::Printf(TEXT("完成研發科技：%s"), *Tech.TechName));
    
    UE_LOG(LogTemp, Log, TEXT("完成研發科技：%s"), *Tech.TechName);
    return true;
}

bool UMingGoRTSTechTree::CanResearchTech(const FString& TechID) const
{
    if (!TechNodes.Contains(TechID))
    {
        return false;
    }

    const FTechNode& Tech = TechNodes[TechID];
    
    // 檢查狀態
    if (Tech.Status != ETechStatus::Available && Tech.Status != ETechStatus::Locked)
    {
        return false;
    }
    
    // 檢查前置條件
    if (!CheckPrerequisites(TechID))
    {
        return false;
    }
    
    // 檢查是否已經完成（對於可重複科技）
    if (CompletedTechs.Contains(TechID) && !Tech.bIsRepeatable)
    {
        return false;
    }
    
    // 檢查最大研發次數（對於可重複科技）
    if (Tech.bIsRepeatable)
    {
        // TODO: 實現重複科技計數
    }
    
    return true;
}

bool UMingGoRTSTechTree::CheckPrerequisites(const FString& TechID) const
{
    if (!TechNodes.Contains(TechID))
    {
        return false;
    }

    const FTechNode& Tech = TechNodes[TechID];
    
    // 檢查所有前置科技
    for (const FString& PrerequisiteTech : Tech.Prerequisites)
    {
        if (!CompletedTechs.Contains(PrerequisiteTech))
        {
            return false;
        }
    }
    
    return true;
}

FTechResearchProgress UMingGoRTSTechTree::GetResearchProgress(const FString& TechID) const
{
    if (const FTechResearchProgress* Progress = ResearchProgress.Find(TechID))
    {
        return *Progress;
    }
    
    return FTechResearchProgress();
}

TArray<FString> UMingGoRTSTechTree::GetCompletedTechs() const
{
    TArray<FString> Completed;
    Completed.Reserve(CompletedTechs.Num());
    
    for (const FString& TechID : CompletedTechs)
    {
        Completed.Add(TechID);
    }
    
    return Completed;
}

TArray<FTechResearchProgress> UMingGoRTSTechTree::GetActiveResearch() const
{
    TArray<FTechResearchProgress> Active;
    Active.Reserve(ResearchProgress.Num());
    
    for (const auto& ProgressPair : ResearchProgress)
    {
        if (!ProgressPair.Value.bIsPaused)
        {
            Active.Add(ProgressPair.Value);
        }
    }
    
    return Active;
}

void UMingGoRTSTechTree::ApplyTechEffects(const FString& TechID)
{
    if (!TechNodes.Contains(TechID))
    {
        return;
    }

    const FTechNode& Tech = TechNodes[TechID];
    
    // 應用所有效果
    for (const FTechEffect& Effect : Tech.Effects)
    {
        ApplySingleTechEffect(Effect);
        OnTechEffectApplied.Broadcast(Effect);
    }
    
    // 廣播解鎖事件
    OnTechUnlocked.Broadcast(TechID, Tech.Category);
}

void UMingGoRTSTechTree::RemoveTechEffects(const FString& TechID)
{
    if (!TechNodes.Contains(TechID))
    {
        return;
    }

    const FTechNode& Tech = TechNodes[TechID];
    
    // 移除所有效果
    for (const FTechEffect& Effect : Tech.Effects)
    {
        RemoveSingleTechEffect(Effect);
    }
}

float UMingGoRTSTechTree::CalculateResearchTime(const FString& TechID) const
{
    if (!TechNodes.Contains(TechID))
    {
        return 0.0f;
    }

    const FTechNode& Tech = TechNodes[TechID];
    float BaseTime = Tech.ResearchCost.TimeCost;
    
    // 應用全局研發速度加成
    float GlobalBonus = Config.GlobalResearchSpeed;
    
    // 應用類別加成
    float CategoryBonus = 1.0f;
    if (const float* Bonus = Config.CategoryResearchBonus.Find(Tech.Category))
    {
        CategoryBonus = *Bonus;
    }
    
    // 應用等級加成
    float TierBonus = 1.0f;
    if (const float* Bonus = Config.TierResearchBonus.Find(Tech.Tier))
    {
        TierBonus = *Bonus;
    }
    
    float FinalTime = BaseTime / (GlobalBonus * CategoryBonus * TierBonus);
    return FMath::Max(FinalTime, 1.0f);
}

FTechCost UMingGoRTSTechTree::CalculateResearchCost(const FString& TechID) const
{
    if (!TechNodes.Contains(TechID))
    {
        return FTechCost();
    }

    const FTechNode& Tech = TechNodes[TechID];
    return Tech.ResearchCost;
}

FTechTreeConfig UMingGoRTSTechTree::GetTechTreeConfig() const
{
    return Config;
}

void UMingGoRTSTechTree::UpdateTechTreeConfig(const FTechTreeConfig& NewConfig)
{
    Config = NewConfig;
    
    // 重新計算所有活動研發的時間
    for (auto& ProgressPair : ResearchProgress)
    {
        FString TechID = ProgressPair.Key;
        FTechResearchProgress& Progress = ProgressPair.Value;
        
        float NewTimeRemaining = CalculateResearchTime(TechID);
        float ProgressRatio = Progress.Progress / 100.0f;
        Progress.TimeRemaining = NewTimeRemaining * (1.0f - ProgressRatio);
    }
    
    UE_LOG(LogTemp, Log, TEXT("科技樹配置已更新"));
}

void UMingGoRTSTechTree::UpdateResearchProgress(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    TArray<FString> CompletedTechsThisFrame;
    
    for (auto& ProgressPair : ResearchProgress)
    {
        FString TechID = ProgressPair.Key;
        FTechResearchProgress& Progress = ProgressPair.Value;
        
        if (Progress.bIsPaused)
        {
            continue;
        }
        
        // 更新進度
        float TimeIncrement = DeltaTime * Config.GlobalResearchSpeed;
        Progress.TimeRemaining -= TimeIncrement;
        
        // 計算進度百分比
        float TotalTime = CalculateResearchTime(TechID);
        float ElapsedTime = TotalTime - Progress.TimeRemaining;
        Progress.Progress = FMath::Clamp((ElapsedTime / TotalTime) * 100.0f, 0.0f, 100.0f);
        
        // 檢查是否完成
        if (Progress.TimeRemaining <= 0.0f)
        {
            CompletedTechsThisFrame.Add(TechID);
        }
    }
    
    // 完成已完成的科技
    for (const FString& TechID : CompletedTechsThisFrame)
    {
        CompleteResearch(TechID);
    }
}

bool UMingGoRTSTechTree::UnlockTech(const FString& TechID)
{
    if (!TechNodes.Contains(TechID))
    {
        return false;
    }

    FTechNode& Tech = TechNodes[TechID];
    
    if (Tech.Status == ETechStatus::Locked)
    {
        if (CheckPrerequisites(TechID))
        {
            Tech.Status = ETechStatus::Available;
            UpdateTechStatus(TechID);
            return true;
        }
    }
    
    return false;
}

void UMingGoRTSTechTree::ResetTechTree()
{
    TechNodes.Empty();
    CompletedTechs.Empty();
    ResearchProgress.Empty();
    ActiveResearchProjects.Empty();
    TechHistory.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("科技樹已重置"));
}

bool UMingGoRTSTechTree::SaveTechTreeData(const FString& SaveSlotName)
{
    // TODO: 實現科技樹數據保存
    UE_LOG(LogTemp, Log, TEXT("保存科技樹數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSTechTree::LoadTechTreeData(const FString& SaveSlotName)
{
    // TODO: 實現科技樹數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入科技樹數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSTechTree::InitializeRepublicanEraTechTree()
{
    // 創建各類別科技
    CreateMilitaryTechs();
    CreateEconomicTechs();
    CreateDiplomaticTechs();
    CreateSocialTechs();
    CreateIndustrialTechs();
    CreateAgriculturalTechs();
    CreateMedicalTechs();
    CreateEducationTechs();
    CreateInfrastructureTechs();
    CreateCommunicationTechs();
    
    // 初始化所有科技狀態
    for (auto& TechPair : TechNodes)
    {
        FTechNode& Tech = TechPair.Value;
        if (Tech.Prerequisites.Num() == 0)
        {
            Tech.Status = ETechStatus::Available;
        }
        else
        {
            Tech.Status = ETechStatus::Locked;
        }
    }
}

void UMingGoRTSTechTree::CreateMilitaryTechs()
{
    // 基礎軍事科技
    FTechNode BasicTraining;
    BasicTraining.TechID = GenerateTechID(TEXT("BasicTraining"), ETechCategory::Military);
    BasicTraining.TechName = TEXT("基礎軍事訓練");
    BasicTraining.TechDescription = TEXT("提升士兵的基本戰鬥能力和紀律性");
    BasicTraining.Category = ETechCategory::Military;
    BasicTraining.Tier = ETechTier::Basic;
    BasicTraining.IconPath = TEXT("/Game/Icons/Tech/BasicTraining");
    BasicTraining.HistoricalContext = TEXT("民國初期，軍隊訓練現代化是建立正規軍的重要基礎");
    
    // 設置成本
    BasicTraining.ResearchCost.TimeCost = 300.0f; // 5分鐘
    BasicTraining.ResearchCost.RequiredScientists = 1;
    BasicTraining.ResearchCost.ResourceCosts.Add(EResourceType::Food, 50.0f);
    BasicTraining.ResearchCost.ResourceCosts.Add(EResourceType::Metal, 30.0f);
    
    // 設置效果
    FTechEffect TrainingEffect;
    TrainingEffect.EffectType = ETechEffectType::MilitaryBonus;
    TrainingEffect.TargetID = TEXT("Infantry");
    TrainingEffect.Value = 15.0f;
    TrainingEffect.Description = TEXT("步兵戰鬥力提升15%");
    TrainingEffect.bIsPermanent = true;
    BasicTraining.Effects.Add(TrainingEffect);
    
    TechNodes.Add(BasicTraining.TechID, BasicTraining);
    
    // 進階軍事科技
    FTechNode ModernWeaponry;
    ModernWeaponry.TechID = GenerateTechID(TEXT("ModernWeaponry"), ETechCategory::Military);
    ModernWeaponry.TechName = TEXT("現代化武器");
    ModernWeaponry.TechDescription = TEXT("引進現代化武器裝備，提升部隊火力");
    ModernWeaponry.Category = ETechCategory::Military;
    ModernWeaponry.Tier = ETechTier::Intermediate;
    ModernWeaponry.IconPath = TEXT("/Game/Icons/Tech/ModernWeaponry");
    ModernWeaponry.Prerequisites.Add(BasicTraining.TechID);
    ModernWeaponry.HistoricalContext = TEXT("1920-30年代，各軍閥爭相引進國外先進武器裝備");
    
    // 設置成本
    ModernWeaponry.ResearchCost.TimeCost = 600.0f; // 10分鐘
    ModernWeaponry.ResearchCost.RequiredScientists = 2;
    ModernWeaponry.ResearchCost.ResourceCosts.Add(EResourceType::Metal, 100.0f);
    ModernWeaponry.ResearchCost.ResourceCosts.Add(EResourceType::Fuel, 50.0f);
    
    // 設置效果
    FTechEffect WeaponEffect;
    WeaponEffect.EffectType = ETechEffectType::UnlockUnit;
    WeaponEffect.TargetID = TEXT("ModernInfantry");
    WeaponEffect.Value = 1.0f;
    WeaponEffect.Description = TEXT("解鎖現代化步兵單位");
    WeaponEffect.bIsPermanent = true;
    ModernWeaponry.Effects.Add(WeaponEffect);
    
    TechNodes.Add(ModernWeaponry.TechID, ModernWeaponry);
}

void UMingGoRTSTechTree::CreateEconomicTechs()
{
    // 基礎經濟科技
    FTechNode BasicTrade;
    BasicTrade.TechID = GenerateTechID(TEXT("BasicTrade"), ETechCategory::Economic);
    BasicTrade.TechName = TEXT("基礎貿易");
    BasicTrade.TechDescription = TEXT("建立基礎貿易網絡，提升經濟收入");
    BasicTrade.Category = ETechCategory::Economic;
    BasicTrade.Tier = ETechTier::Basic;
    BasicTrade.IconPath = TEXT("/Game/Icons/Tech/BasicTrade");
    BasicTrade.HistoricalContext = TEXT("民國時期，貿易是各派系重要的經濟來源");
    
    // 設置成本
    BasicTrade.ResearchCost.TimeCost = 240.0f; // 4分鐘
    BasicTrade.ResearchCost.RequiredScientists = 1;
    BasicTrade.ResearchCost.ResourceCosts.Add(EResourceType::Gold, 30.0f);
    BasicTrade.ResearchCost.ResourceCosts.Add(EResourceType::Silver, 20.0f);
    
    // 設置效果
    FTechEffect TradeEffect;
    TradeEffect.EffectType = ETechEffectType::EconomicBonus;
    TradeEffect.TargetID = TEXT("TradeIncome");
    TradeEffect.Value = 20.0f;
    TradeEffect.Description = TEXT("貿易收入提升20%");
    TradeEffect.bIsPermanent = true;
    BasicTrade.Effects.Add(TradeEffect);
    
    TechNodes.Add(BasicTrade.TechID, BasicTrade);
}

void UMingGoRTSTechTree::CreateDiplomaticTechs()
{
    // 基礎外交科技
    FTechNode BasicDiplomacy;
    BasicDiplomacy.TechID = GenerateTechID(TEXT("BasicDiplomacy"), ETechCategory::Diplomatic);
    BasicDiplomacy.TechName = TEXT("基礎外交");
    BasicDiplomacy.TechDescription = TEXT("建立外交關係，提升與其他派系的互動能力");
    BasicDiplomacy.Category = ETechCategory::Diplomatic;
    BasicDiplomacy.Tier = ETechTier::Basic;
    BasicDiplomacy.IconPath = TEXT("/Game/Icons/Tech/BasicDiplomacy");
    BasicDiplomacy.HistoricalContext = TEXT("民國時期複雜的政治格局需要靈活的外交手段");
    
    // 設置成本
    BasicDiplomacy.ResearchCost.TimeCost = 360.0f; // 6分鐘
    BasicDiplomacy.RequiredScientists = 1;
    BasicDiplomacy.ResearchCost.ResourceCosts.Add(EResourceType::Tea, 40.0f);
    BasicDiplomacy.ResearchCost.ResourceCosts.Add(EResourceType::Silk, 30.0f);
    
    TechNodes.Add(BasicDiplomacy.TechID, BasicDiplomacy);
}

void UMingGoRTSTechTree::CreateSocialTechs()
{
    // 基礎社會科技
    FTechNode BasicEducation;
    BasicEducation.TechID = GenerateTechID(TEXT("BasicEducation"), ETechCategory::Social);
    BasicEducation.TechName = TEXT("基礎教育");
    BasicEducation.TechDescription = TEXT("建立基礎教育體系，提升民眾素養");
    BasicEducation.Category = ETechCategory::Social;
    BasicEducation.Tier = ETechTier::Basic;
    BasicEducation.IconPath = TEXT("/Game/Icons/Tech/BasicEducation");
    BasicEducation.HistoricalContext = TEXT("民國時期，教育改革是國家現代化的重要組成部分");
    
    // 設置成本
    BasicEducation.ResearchCost.TimeCost = 480.0f; // 8分鐘
    BasicEducation.RequiredScientists = 2;
    BasicEducation.ResearchCost.ResourceCosts.Add(EResourceType::Paper, 60.0f);
    BasicEducation.ResearchCost.ResourceCosts.Add(EResourceType::Books, 40.0f);
    
    TechNodes.Add(BasicEducation.TechID, BasicEducation);
}

void UMingGoRTSTechTree::CreateIndustrialTechs()
{
    // 基礎工業科技
    FTechNode BasicManufacturing;
    BasicManufacturing.TechID = GenerateTechID(TEXT("BasicManufacturing"), ETechCategory::Industrial);
    BasicManufacturing.TechName = TEXT("基礎製造業");
    BasicManufacturing.TechDescription = TEXT("建立基礎製造業，提升生產效率");
    BasicManufacturing.Category = ETechCategory::Industrial;
    BasicManufacturing.Tier = ETechTier::Basic;
    BasicManufacturing.IconPath = TEXT("/Game/Icons/Tech/BasicManufacturing");
    BasicManufacturing.HistoricalContext = TEXT("民國時期，工業化是國家富強的必經之路");
    
    // 設置成本
    BasicManufacturing.ResearchCost.TimeCost = 420.0f; // 7分鐘
    BasicManufacturing.RequiredScientists = 2;
    BasicManufacturing.ResearchCost.ResourceCosts.Add(EResourceType::Metal, 80.0f);
    BasicManufacturing.ResearchCost.ResourceCosts.Add(EResourceType::Tools, 50.0f);
    
    TechNodes.Add(BasicManufacturing.TechID, BasicManufacturing);
}

void UMingGoRTSTechTree::CreateAgriculturalTechs()
{
    // 基礎農業科技
    FTechNode BasicFarming;
    BasicFarming.TechID = GenerateTechID(TEXT("BasicFarming"), ETechCategory::Agricultural);
    BasicFarming.TechName = TEXT("基礎農業");
    BasicFarming.TechDescription = TEXT("改進農業技術，提升糧食產量");
    BasicFarming.Category = ETechCategory::Agricultural;
    BasicFarming.Tier = ETechTier::Basic;
    BasicFarming.IconPath = TEXT("/Game/Icons/Tech/BasicFarming");
    BasicFarming.HistoricalContext = TEXT("民國時期，農業是國民經濟的基礎");
    
    // 設置成本
    BasicFarming.ResearchCost.TimeCost = 300.0f; // 5分鐘
    BasicFarming.RequiredScientists = 1;
    BasicFarming.ResearchCost.ResourceCosts.Add(EResourceType::Wood, 40.0f);
    BasicFarming.ResearchCost.ResourceCosts.Add(EResourceType::Food, 30.0f);
    
    TechNodes.Add(BasicFarming.TechID, BasicFarming);
}

void UMingGoRTSTechTree::CreateMedicalTechs()
{
    // 基礎醫療科技
    FTechNode BasicMedicine;
    BasicMedicine.TechID = GenerateTechID(TEXT("BasicMedicine"), ETechCategory::Medical);
    BasicMedicine.TechName = TEXT("基礎醫療");
    BasicMedicine.TechDescription = TEXT("建立基礎醫療體系，提升治療效果");
    BasicMedicine.Category = ETechCategory::Medical;
    BasicMedicine.Tier = ETechTier::Basic;
    BasicMedicine.IconPath = TEXT("/Game/Icons/Tech/BasicMedicine");
    BasicMedicine.HistoricalContext = TEXT("民國時期，西方醫學的引進改善了公共衛生狀況");
    
    // 設置成本
    BasicMedicine.ResearchCost.TimeCost = 360.0f; // 6分鐘
    BasicMedicine.RequiredScientists = 2;
    BasicMedicine.ResearchCost.ResourceCosts.Add(EResourceType::Medicine, 50.0f);
    BasicMedicine.ResearchCost.ResourceCosts.Add(EResourceType::Tools, 30.0f);
    
    TechNodes.Add(BasicMedicine.TechID, BasicMedicine);
}

void UMingGoRTSTechTree::CreateEducationTechs()
{
    // 高等教育科技
    FTechNode HigherEducation;
    HigherEducation.TechID = GenerateTechID(TEXT("HigherEducation"), ETechCategory::Education);
    HigherEducation.TechName = TEXT("高等教育");
    HigherEducation.TechDescription = TEXT("建立高等教育機構，培養專業人才");
    HigherEducation.Category = ETechCategory::Education;
    HigherEducation.Tier = ETechTier::Intermediate;
    HigherEducation.IconPath = TEXT("/Game/Icons/Tech/HigherEducation");
    HigherEducation.HistoricalContext = TEXT("民國時期，大學和專門學校的建立為國家培養了現代人才");
    
    // 設置成本
    HigherEducation.ResearchCost.TimeCost = 600.0f; // 10分鐘
    HigherEducation.RequiredScientists = 3;
    HigherEducation.ResearchCost.ResourceCosts.Add(EResourceType::Books, 80.0f);
    HigherEducation.ResearchCost.ResourceCosts.Add(EResourceType::Paper, 60.0f);
    
    TechNodes.Add(HigherEducation.TechID, HigherEducation);
}

void UMingGoRTSTechTree::CreateInfrastructureTechs()
{
    // 基礎設施科技
    FTechNode BasicInfrastructure;
    BasicInfrastructure.TechID = GenerateTechID(TEXT("BasicInfrastructure"), ETechCategory::Infrastructure);
    BasicInfrastructure.TechName = TEXT("基礎設施");
    BasicInfrastructure.TechDescription = TEXT("建設基礎設施，改善交通和通訊");
    BasicInfrastructure.Category = ETechCategory::Infrastructure;
    BasicInfrastructure.Tier = ETechTier::Basic;
    BasicInfrastructure.IconPath = TEXT("/Game/Icons/Tech/BasicInfrastructure");
    BasicInfrastructure.HistoricalContext = TEXT("民國時期，基礎設施建設是國家現代化的重要標誌");
    
    // 設置成本
    BasicInfrastructure.ResearchCost.TimeCost = 480.0f; // 8分鐘
    BasicInfrastructure.RequiredScientists = 2;
    BasicInfrastructure.ResearchCost.ResourceCosts.Add(EResourceType::Stone, 100.0f);
    BasicInfrastructure.ResearchCost.ResourceCosts.Add(EResourceType::Metal, 80.0f);
    
    TechNodes.Add(BasicInfrastructure.TechID, BasicInfrastructure);
}

void UMingGoRTSTechTree::CreateCommunicationTechs()
{
    // 基礎通訊科技
    FTechNode BasicCommunication;
    BasicCommunication.TechID = GenerateTechID(TEXT("BasicCommunication"), ETechCategory::Communication);
    BasicCommunication.TechName = TEXT("基礎通訊");
    BasicCommunication.TechDescription = TEXT("建立基礎通訊系統，提升信息傳遞效率");
    BasicCommunication.Category = ETechCategory::Communication;
    BasicCommunication.Tier = ETechTier::Basic;
    BasicCommunication.IconPath = TEXT("/Game/Icons/Tech/BasicCommunication");
    BasicCommunication.HistoricalContext = TEXT("民國時期，電報和電話的引進革命性地改變了信息傳遞方式");
    
    // 設置成本
    BasicCommunication.ResearchCost.TimeCost = 420.0f; // 7分鐘
    BasicCommunication.RequiredScientists = 2;
    BasicCommunication.ResearchCost.ResourceCosts.Add(EResourceType::Copper, 60.0f);
    BasicCommunication.ResearchCost.ResourceCosts.Add(EResourceType::Tools, 40.0f);
    
    TechNodes.Add(BasicCommunication.TechID, BasicCommunication);
}

void UMingGoRTSTechTree::UpdateTechStatus(const FString& TechID)
{
    // 更新相關科技的狀態
    for (auto& TechPair : TechNodes)
    {
        FString CurrentTechID = TechPair.Key;
        FTechNode& Tech = TechPair.Value;
        
        if (Tech.Status == ETechStatus::Locked)
        {
            if (CheckPrerequisites(CurrentTechID))
            {
                Tech.Status = ETechStatus::Available;
            }
        }
    }
}

void UMingGoRTSTechTree::CheckAndUnlockNewTechs()
{
    TArray<FString> NewlyUnlockedTechs;
    
    for (auto& TechPair : TechNodes)
    {
        FString TechID = TechPair.Key;
        FTechNode& Tech = TechPair.Value;
        
        if (Tech.Status == ETechStatus::Locked)
        {
            if (CheckPrerequisites(TechID))
            {
                Tech.Status = ETechStatus::Available;
                NewlyUnlockedTechs.Add(TechID);
            }
        }
    }
    
    // 廣播新解鎖的科技
    for (const FString& TechID : NewlyUnlockedTechs)
    {
        if (const FTechNode* Tech = TechNodes.Find(TechID))
        {
            OnTechUnlocked.Broadcast(TechID, Tech->Category);
        }
    }
}

void UMingGoRTSTechTree::ApplySingleTechEffect(const FTechEffect& Effect)
{
    // TODO: 實現具體的效果應用邏輯
    UE_LOG(LogTemp, Log, TEXT("應用科技效果：%s，目標：%s，數值：%.2f"), 
           *StaticEnum<ETechEffectType>()->GetValueAsString(Effect.EffectType), 
           *Effect.TargetID, Effect.Value);
}

void UMingGoRTSTechTree::RemoveSingleTechEffect(const FTechEffect& Effect)
{
    // TODO: 實現具體的效果移除邏輯
    UE_LOG(LogTemp, Log, TEXT("移除科技效果：%s，目標：%s，數值：%.2f"), 
           *StaticEnum<ETechEffectType>()->GetValueAsString(Effect.EffectType), 
           *Effect.TargetID, Effect.Value);
}

FString UMingGoRTSTechTree::GetTechCategoryName(ETechCategory Category) const
{
    const UEnum* EnumPtr = StaticEnum<ETechCategory>();
    if (EnumPtr)
    {
        return EnumPtr->GetNameStringByValue(static_cast<int64>(Category));
    }
    return TEXT("Unknown");
}

FString UMingGoRTSTechTree::GetTechTierName(ETechTier Tier) const
{
    const UEnum* EnumPtr = StaticEnum<ETechTier>();
    if (EnumPtr)
    {
        return EnumPtr->GetNameStringByValue(static_cast<int64>(Tier));
    }
    return TEXT("Unknown");
}

bool UMingGoRTSTechTree::ValidateTechData(const FTechNode& TechNode) const
{
    // 基本驗證
    if (TechNode.TechID.IsEmpty() || TechNode.TechName.IsEmpty())
    {
        return false;
    }
    
    if (TechNode.ResearchCost.TimeCost <= 0.0f)
    {
        return false;
    }
    
    return true;
}

FString UMingGoRTSTechTree::GenerateTechID(const FString& BaseName, ETechCategory Category) const
{
    FString CategoryName = GetTechCategoryName(Category);
    return FString::Printf(TEXT("TECH_%s_%s_%d"), *CategoryName, *BaseName, FMath::RandRange(1000, 9999));
}

void UMingGoRTSTechTree::RecordTechHistory(const FString& Event)
{
    TechHistory.Add(Event);
    
    // 限制歷史記錄數量
    if (TechHistory.Num() > 1000)
    {
        TechHistory.RemoveAt(0);
    }
}
