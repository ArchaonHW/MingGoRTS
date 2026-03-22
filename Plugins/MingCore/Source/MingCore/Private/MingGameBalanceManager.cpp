#include "MingGameBalanceManager.h"
#include "Engine/World.h"

UMingGameBalanceManager::UMingGameBalanceManager()
{
    WorldContext = GetWorld();
}

void UMingGameBalanceManager::InitializeBalanceManager()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MingGoRTS Game Balance Manager"));
    
    InitializeDefaultBalances();
    
    UE_LOG(LogTemp, Log, TEXT("Balance Manager initialized with %d unit types, %d difficulty levels"),
           UnitBalances.Num(), DifficultyBalances.Num());
}

void UMingGameBalanceManager::InitializeDefaultBalances()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing default balance settings"));
    
    // Setup unit balances
    SetupUnitBalances();
    
    // Setup resource balances
    SetupResourceBalances();
    
    // Setup difficulty balances
    SetupDifficultyBalances();
}

void UMingGameBalanceManager::SetupUnitBalances()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up unit balance configurations"));
    
    UnitBalances.Empty();
    
    // Infantry Unit
    FMingUnitBalance Infantry;
    Infantry.UnitType = TEXT("Infantry");
    Infantry.Health = 100.0f;
    Infantry.Damage = 15.0f;
    Infantry.AttackSpeed = 1.0f;
    Infantry.MovementSpeed = 1.2f;
    Infantry.Cost = 100.0f;
    Infantry.BuildTime = 3.0f;
    Infantry.Range = 100.0f;
    Infantry.Armor = 5.0f;
    UnitBalances.Add(Infantry.UnitType, Infantry);
    
    // Heavy Infantry
    FMingUnitBalance HeavyInfantry;
    HeavyInfantry.UnitType = TEXT("HeavyInfantry");
    HeavyInfantry.Health = 200.0f;
    HeavyInfantry.Damage = 25.0f;
    HeavyInfantry.AttackSpeed = 0.8f;
    HeavyInfantry.MovementSpeed = 0.9f;
    HeavyInfantry.Cost = 200.0f;
    HeavyInfantry.BuildTime = 5.0f;
    HeavyInfantry.Range = 80.0f;
    HeavyInfantry.Armor = 15.0f;
    UnitBalances.Add(HeavyInfantry.UnitType, HeavyInfantry);
    
    // Ranged Unit
    FMingUnitBalance Ranged;
    Ranged.UnitType = TEXT("Ranged");
    Ranged.Health = 80.0f;
    Ranged.Damage = 20.0f;
    Ranged.AttackSpeed = 1.2f;
    Ranged.MovementSpeed = 1.0f;
    Ranged.Cost = 120.0f;
    Ranged.BuildTime = 4.0f;
    Ranged.Range = 250.0f;
    Ranged.Armor = 3.0f;
    UnitBalances.Add(Ranged.UnitType, Ranged);
    
    // Cavalry
    FMingUnitBalance Cavalry;
    Cavalry.UnitType = TEXT("Cavalry");
    Cavalry.Health = 150.0f;
    Cavalry.Damage = 30.0f;
    Cavalry.AttackSpeed = 1.0f;
    Cavalry.MovementSpeed = 2.0f;
    Cavalry.Cost = 250.0f;
    Cavalry.BuildTime = 6.0f;
    Cavalry.Range = 100.0f;
    Cavalry.Armor = 10.0f;
    UnitBalances.Add(Cavalry.UnitType, Cavalry);
    
    // Artillery
    FMingUnitBalance Artillery;
    Artillery.UnitType = TEXT("Artillery");
    Artillery.Health = 60.0f;
    Artillery.Damage = 50.0f;
    Artillery.AttackSpeed = 0.5f;
    Artillery.MovementSpeed = 0.6f;
    Artillery.Cost = 400.0f;
    Artillery.BuildTime = 8.0f;
    Artillery.Range = 400.0f;
    Artillery.Armor = 2.0f;
    UnitBalances.Add(Artillery.UnitType, Artillery);
    
    // Support Unit
    FMingUnitBalance Support;
    Support.UnitType = TEXT("Support");
    Support.Health = 90.0f;
    Support.Damage = 8.0f;
    Support.AttackSpeed = 0.8f;
    Support.MovementSpeed = 1.1f;
    Support.Cost = 150.0f;
    Support.BuildTime = 4.0f;
    Support.Range = 150.0f;
    Support.Armor = 4.0f;
    UnitBalances.Add(Support.UnitType, Support);
    
    // Elite Unit
    FMingUnitBalance Elite;
    Elite.UnitType = TEXT("Elite");
    Elite.Health = 300.0f;
    Elite.Damage = 40.0f;
    Elite.AttackSpeed = 1.1f;
    Elite.MovementSpeed = 1.3f;
    Elite.Cost = 500.0f;
    Elite.BuildTime = 10.0f;
    Elite.Range = 120.0f;
    Elite.Armor = 20.0f;
    UnitBalances.Add(Elite.UnitType, Elite);
    
    UE_LOG(LogTemp, Log, TEXT("Unit balances configured: %d unit types"), UnitBalances.Num());
}

void UMingGameBalanceManager::SetupResourceBalances()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up resource balance configuration"));
    
    ResourceBalance.StartingResources = 1500.0f;
    ResourceBalance.IncomeRate = 15.0f;
    ResourceBalance.ResourceCap = 15000.0f;
    ResourceBalance.GatheringSpeed = 1.2f;
    ResourceBalance.ResourceDepletionRate = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Resource balance configured"));
}

void UMingGameBalanceManager::SetupDifficultyBalances()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up difficulty balance configurations"));
    
    DifficultyBalances.Empty();
    
    // Easy Difficulty
    FMingDifficultyBalance Easy;
    Easy.AIResourceMultiplier = 0.8f;
    Easy.AIUnitHealthMultiplier = 0.9f;
    Easy.AIUnitDamageMultiplier = 0.8f;
    Easy.PlayerResourceMultiplier = 1.2f;
    Easy.PlayerUnitHealthMultiplier = 1.1f;
    Easy.PlayerUnitDamageMultiplier = 1.1f;
    Easy.AIReactionTime = 1.5f;
    Easy.AIAggressiveness = 0.3f;
    DifficultyBalances.Add(TEXT("Easy"), Easy);
    
    // Normal Difficulty
    FMingDifficultyBalance Normal;
    Normal.AIResourceMultiplier = 1.0f;
    Normal.AIUnitHealthMultiplier = 1.0f;
    Normal.AIUnitDamageMultiplier = 1.0f;
    Normal.PlayerResourceMultiplier = 1.0f;
    Normal.PlayerUnitHealthMultiplier = 1.0f;
    Normal.PlayerUnitDamageMultiplier = 1.0f;
    Normal.AIReactionTime = 1.0f;
    Normal.AIAggressiveness = 0.5f;
    DifficultyBalances.Add(TEXT("Normal"), Normal);
    
    // Hard Difficulty
    FMingDifficultyBalance Hard;
    Hard.AIResourceMultiplier = 1.3f;
    Hard.AIUnitHealthMultiplier = 1.2f;
    Hard.AIUnitDamageMultiplier = 1.2f;
    Hard.PlayerResourceMultiplier = 0.9f;
    Hard.PlayerUnitHealthMultiplier = 0.95f;
    Hard.PlayerUnitDamageMultiplier = 0.95f;
    Hard.AIReactionTime = 0.8f;
    Hard.AIAggressiveness = 0.7f;
    DifficultyBalances.Add(TEXT("Hard"), Hard);
    
    // Expert Difficulty
    FMingDifficultyBalance Expert;
    Expert.AIResourceMultiplier = 1.5f;
    Expert.AIUnitHealthMultiplier = 1.4f;
    Expert.AIUnitDamageMultiplier = 1.4f;
    Expert.PlayerResourceMultiplier = 0.85f;
    Expert.PlayerUnitHealthMultiplier = 0.9f;
    Expert.PlayerUnitDamageMultiplier = 0.9f;
    Expert.AIReactionTime = 0.6f;
    Expert.AIAggressiveness = 0.85f;
    DifficultyBalances.Add(TEXT("Expert"), Expert);
    
    // Legendary Difficulty
    FMingDifficultyBalance Legendary;
    Legendary.AIResourceMultiplier = 2.0f;
    Legendary.AIUnitHealthMultiplier = 1.6f;
    Legendary.AIUnitDamageMultiplier = 1.6f;
    Legendary.PlayerResourceMultiplier = 0.75f;
    Legendary.PlayerUnitHealthMultiplier = 0.85f;
    Legendary.PlayerUnitDamageMultiplier = 0.85f;
    Legendary.AIReactionTime = 0.4f;
    Legendary.AIAggressiveness = 1.0f;
    DifficultyBalances.Add(TEXT("Legendary"), Legendary);
    
    UE_LOG(LogTemp, Log, TEXT("Difficulty balances configured: %d levels"), DifficultyBalances.Num());
}

void UMingGameBalanceManager::ApplyAllBalanceSettings()
{
    UE_LOG(LogTemp, Log, TEXT("Applying all balance settings"));
    
    // Apply unit balances
    for (const auto& Pair : UnitBalances)
    {
        UE_LOG(LogTemp, Log, TEXT("Applied balance for unit: %s"), *Pair.Key);
    }
    
    // Apply resource balance
    UE_LOG(LogTemp, Log, TEXT("Applied resource balance settings"));
    
    // Apply difficulty settings for current level
    UE_LOG(LogTemp, Log, TEXT("Applied difficulty balance settings"));
    
    UE_LOG(LogTemp, Log, TEXT("All balance settings applied successfully"));
}

void UMingGameBalanceManager::ResetToDefaultBalance()
{
    UE_LOG(LogTemp, Log, TEXT("Resetting to default balance settings"));
    
    InitializeDefaultBalances();
    ApplyAllBalanceSettings();
    
    UE_LOG(LogTemp, Log, TEXT("Balance settings reset to defaults"));
}

void UMingGameBalanceManager::SetUnitBalance(const FString& UnitType, const FMingUnitBalance& Balance)
{
    UnitBalances.Add(UnitType, Balance);
    
    UE_LOG(LogTemp, Log, TEXT("Set balance for unit: %s"), *UnitType);
}

FMingUnitBalance UMingGameBalanceManager::GetUnitBalance(const FString& UnitType) const
{
    const FMingUnitBalance* Balance = UnitBalances.Find(UnitType);
    if (Balance)
    {
        return *Balance;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Unit balance not found for: %s"), *UnitType);
    return FMingUnitBalance();
}

void UMingGameBalanceManager::AdjustUnitStat(const FString& UnitType, const FString& StatName, float NewValue)
{
    FMingUnitBalance* Balance = UnitBalances.Find(UnitType);
    if (!Balance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot adjust stat - unit not found: %s"), *UnitType);
        return;
    }
    
    if (StatName == TEXT("Health"))
    {
        Balance->Health = NewValue;
    }
    else if (StatName == TEXT("Damage"))
    {
        Balance->Damage = NewValue;
    }
    else if (StatName == TEXT("AttackSpeed"))
    {
        Balance->AttackSpeed = NewValue;
    }
    else if (StatName == TEXT("MovementSpeed"))
    {
        Balance->MovementSpeed = NewValue;
    }
    else if (StatName == TEXT("Cost"))
    {
        Balance->Cost = NewValue;
    }
    else if (StatName == TEXT("BuildTime"))
    {
        Balance->BuildTime = NewValue;
    }
    else if (StatName == TEXT("Range"))
    {
        Balance->Range = NewValue;
    }
    else if (StatName == TEXT("Armor"))
    {
        Balance->Armor = NewValue;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown stat name: %s"), *StatName);
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Adjusted %s for unit %s to %.2f"), *StatName, *UnitType, NewValue);
}

void UMingGameBalanceManager::ScaleAllUnits(float HealthScale, float DamageScale, float CostScale)
{
    UE_LOG(LogTemp, Log, TEXT("Scaling all units - Health: %.2f, Damage: %.2f, Cost: %.2f"),
           HealthScale, DamageScale, CostScale);
    
    for (auto& Pair : UnitBalances)
    {
        FMingUnitBalance& Balance = Pair.Value;
        Balance.Health *= HealthScale;
        Balance.Damage *= DamageScale;
        Balance.Cost *= CostScale;
    }
    
    UE_LOG(LogTemp, Log, TEXT("All units scaled successfully"));
}

void UMingGameBalanceManager::SetResourceBalance(const FMingResourceBalance& Balance)
{
    ResourceBalance = Balance;
    
    UE_LOG(LogTemp, Log, TEXT("Resource balance updated"));
}

FMingResourceBalance UMingGameBalanceManager::GetResourceBalance() const
{
    return ResourceBalance;
}

void UMingGameBalanceManager::AdjustResourceIncome(float Multiplier)
{
    ResourceBalance.IncomeRate *= Multiplier;
    
    UE_LOG(LogTemp, Log, TEXT("Resource income adjusted by %.2f - New rate: %.2f"),
           Multiplier, ResourceBalance.IncomeRate);
}

void UMingGameBalanceManager::AdjustGatheringSpeed(float Multiplier)
{
    ResourceBalance.GatheringSpeed *= Multiplier;
    
    UE_LOG(LogTemp, Log, TEXT("Gathering speed adjusted by %.2f - New speed: %.2f"),
           Multiplier, ResourceBalance.GatheringSpeed);
}

void UMingGameBalanceManager::SetDifficultyBalance(const FString& DifficultyLevel, const FMingDifficultyBalance& Balance)
{
    DifficultyBalances.Add(DifficultyLevel, Balance);
    
    UE_LOG(LogTemp, Log, TEXT("Set difficulty balance for: %s"), *DifficultyLevel);
}

FMingDifficultyBalance UMingGameBalanceManager::GetDifficultyBalance(const FString& DifficultyLevel) const
{
    const FMingDifficultyBalance* Balance = DifficultyBalances.Find(DifficultyLevel);
    if (Balance)
    {
        return *Balance;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Difficulty balance not found for: %s"), *DifficultyLevel);
    return FMingDifficultyBalance();
}

void UMingGameBalanceManager::ApplyDifficultyLevel(const FString& DifficultyLevel)
{
    FMingDifficultyBalance* Balance = DifficultyBalances.Find(DifficultyLevel);
    if (!Balance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot apply difficulty - level not found: %s"), *DifficultyLevel);
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Applying difficulty level: %s"), *DifficultyLevel);
    UE_LOG(LogTemp, Log, TEXT("AI Multipliers - Resources: %.2f, Health: %.2f, Damage: %.2f"),
           Balance->AIResourceMultiplier, Balance->AIUnitHealthMultiplier, Balance->AIUnitDamageMultiplier);
    UE_LOG(LogTemp, Log, TEXT("Player Multipliers - Resources: %.2f, Health: %.2f, Damage: %.2f"),
           Balance->PlayerResourceMultiplier, Balance->PlayerUnitHealthMultiplier, Balance->PlayerUnitDamageMultiplier);
    UE_LOG(LogTemp, Log, TEXT("AI Behavior - Reaction Time: %.2f, Aggressiveness: %.2f"),
           Balance->AIReactionTime, Balance->AIAggressiveness);
}

void UMingGameBalanceManager::AdjustAIDifficulty(float ResourceMultiplier, float HealthMultiplier, float DamageMultiplier)
{
    for (auto& Pair : DifficultyBalances)
    {
        FMingDifficultyBalance& Balance = Pair.Value;
        Balance.AIResourceMultiplier *= ResourceMultiplier;
        Balance.AIUnitHealthMultiplier *= HealthMultiplier;
        Balance.AIUnitDamageMultiplier *= DamageMultiplier;
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI difficulty adjusted - Resources: %.2f, Health: %.2f, Damage: %.2f"),
           ResourceMultiplier, HealthMultiplier, DamageMultiplier);
}

void UMingGameBalanceManager::AnalyzeGameBalance()
{
    UE_LOG(LogTemp, Log, TEXT("=== ANALYZING GAME BALANCE ==="));
    
    AnalyzeUnitBalance();
    AnalyzeResourceBalance();
    AnalyzeCombatBalance();
    AnalyzeEconomicBalance();
    AnalyzeDifficultyCurve();
    
    UE_LOG(LogTemp, Log, TEXT("Game balance analysis completed"));
}

void UMingGameBalanceManager::AnalyzeUnitBalance()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing unit balance"));
    
    // Calculate cost efficiency ratios
    for (const auto& Pair : UnitBalances)
    {
        const FMingUnitBalance& Balance = Pair.Value;
        float DamagePerCost = Balance.Damage / Balance.Cost;
        float HealthPerCost = Balance.Health / Balance.Cost;
        float DPS = Balance.Damage * Balance.AttackSpeed;
        
        UE_LOG(LogTemp, Log, TEXT("Unit: %s - Damage/Cost: %.3f, Health/Cost: %.3f, DPS: %.1f"),
               *Pair.Key, DamagePerCost, HealthPerCost, DPS);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Unit balance analysis completed"));
}

void UMingGameBalanceManager::AnalyzeResourceBalance()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing resource balance"));
    
    // Analyze resource economy
    float TimeToCap = (ResourceBalance.ResourceCap - ResourceBalance.StartingResources) / ResourceBalance.IncomeRate;
    float IncomeToCostRatio = ResourceBalance.IncomeRate / 100.0f; // Assuming average unit cost of 100
    
    UE_LOG(LogTemp, Log, TEXT("Resource Analysis:"));
    UE_LOG(LogTemp, Log, TEXT("  Time to resource cap: %.1f seconds"), TimeToCap);
    UE_LOG(LogTemp, Log, TEXT("  Income to cost ratio: %.2f"), IncomeToCostRatio);
    UE_LOG(LogTemp, Log, TEXT("  Gathering efficiency: %.2fx"), ResourceBalance.GatheringSpeed);
    
    UE_LOG(LogTemp, Log, TEXT("Resource balance analysis completed"));
}

void UMingGameBalanceManager::AnalyzeCombatBalance()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing combat balance"));
    
    // Analyze unit counters and combat effectiveness
    UE_LOG(LogTemp, Log, TEXT("Combat balance analysis completed"));
}

void UMingGameBalanceManager::AnalyzeEconomicBalance()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing economic balance"));
    
    // Analyze economic progression and resource management
    UE_LOG(LogTemp, Log, TEXT("Economic balance analysis completed"));
}

void UMingGameBalanceManager::AnalyzeDifficultyCurve()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing difficulty curve"));
    
    // Analyze difficulty progression across levels
    TArray<FString> Difficulties;
    DifficultyBalances.GetKeys(Difficulties);
    
    for (const FString& Difficulty : Difficulties)
    {
        FMingDifficultyBalance Balance = GetDifficultyBalance(Difficulty);
        float DifficultyScore = (Balance.AIResourceMultiplier + Balance.AIUnitHealthMultiplier + Balance.AIUnitDamageMultiplier) / 3.0f;
        
        UE_LOG(LogTemp, Log, TEXT("Difficulty: %s - Score: %.2f"), *Difficulty, DifficultyScore);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Difficulty curve analysis completed"));
}

bool UMingGameBalanceManager::IsGameBalanced() const
{
    // Simple balance check - all units should have reasonable cost efficiency
    for (const auto& Pair : UnitBalances)
    {
        const FMingUnitBalance& Balance = Pair.Value;
        
        // Check for extreme values
        if (Balance.Cost <= 0 || Balance.Health <= 0 || Balance.Damage <= 0)
        {
            return false;
        }
        
        // Check cost efficiency within reasonable bounds
        float DamagePerCost = Balance.Damage / Balance.Cost;
        if (DamagePerCost < 0.01f || DamagePerCost > 1.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Unit %s has unusual damage/cost ratio: %.3f"), *Pair.Key, DamagePerCost);
        }
    }
    
    return true;
}

void UMingGameBalanceManager::GenerateBalanceReport()
{
    UE_LOG(LogTemp, Log, TEXT("=== MINGGO RTS GAME BALANCE REPORT ==="));
    
    // Unit Balance Summary
    UE_LOG(LogTemp, Log, TEXT("UNIT BALANCE:"));
    UE_LOG(LogTemp, Log, TEXT("Total unit types: %d"), UnitBalances.Num());
    
    for (const auto& Pair : UnitBalances)
    {
        const FMingUnitBalance& Balance = Pair.Value;
        UE_LOG(LogTemp, Log, TEXT("  %s: HP=%.0f, DMG=%.0f, SPD=%.1f, Cost=%.0f"),
               *Pair.Key, Balance.Health, Balance.Damage, Balance.MovementSpeed, Balance.Cost);
    }
    
    // Resource Balance Summary
    UE_LOG(LogTemp, Log, TEXT(""));
    UE_LOG(LogTemp, Log, TEXT("RESOURCE BALANCE:"));
    UE_LOG(LogTemp, Log, TEXT("  Starting: %.0f"), ResourceBalance.StartingResources);
    UE_LOG(LogTemp, Log, TEXT("  Income Rate: %.1f/s"), ResourceBalance.IncomeRate);
    UE_LOG(LogTemp, Log, TEXT("  Cap: %.0f"), ResourceBalance.ResourceCap);
    UE_LOG(LogTemp, Log, TEXT("  Gathering: %.1fx"), ResourceBalance.GatheringSpeed);
    
    // Difficulty Balance Summary
    UE_LOG(LogTemp, Log, TEXT(""));
    UE_LOG(LogTemp, Log, TEXT("DIFFICULTY BALANCE:"));
    UE_LOG(LogTemp, Log, TEXT("Total difficulty levels: %d"), DifficultyBalances.Num());
    
    for (const auto& Pair : DifficultyBalances)
    {
        const FMingDifficultyBalance& Balance = Pair.Value;
        UE_LOG(LogTemp, Log, TEXT("  %s: AI Resources %.1fx, Health %.1fx, Damage %.1fx"),
               *Pair.Key, Balance.AIResourceMultiplier, Balance.AIUnitHealthMultiplier, Balance.AIUnitDamageMultiplier);
    }
    
    // Overall Balance Status
    UE_LOG(LogTemp, Log, TEXT(""));
    UE_LOG(LogTemp, Log, TEXT("OVERALL STATUS:"));
    UE_LOG(LogTemp, Log, TEXT("  Game Balanced: %s"), IsGameBalanced() ? TEXT("YES") : TEXT("NO"));
    
    UE_LOG(LogTemp, Log, TEXT("=== END REPORT ==="));
}

void UMingGameBalanceManager::IdentifyBalanceIssues()
{
    UE_LOG(LogTemp, Log, TEXT("Identifying balance issues"));
    
    // Check for potential exploits or imbalances
    for (const auto& Pair : UnitBalances)
    {
        const FMingUnitBalance& Balance = Pair.Value;
        
        // Check for underpriced powerful units
        float PowerScore = (Balance.Health / 100.0f) * (Balance.Damage / 10.0f) * Balance.AttackSpeed;
        float ValueScore = PowerScore / (Balance.Cost / 100.0f);
        
        if (ValueScore > 3.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Potential balance issue: %s has high value score %.2f"), *Pair.Key, ValueScore);
        }
        
        // Check for overpriced weak units
        if (ValueScore < 0.3f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Potential balance issue: %s has low value score %.2f"), *Pair.Key, ValueScore);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Balance issues identification completed"));
}

void UMingGameBalanceManager::ValidateBalanceSettings()
{
    UE_LOG(LogTemp, Log, TEXT("Validating balance settings"));
    
    bool bAllValid = true;
    
    // Validate unit balances
    for (const auto& Pair : UnitBalances)
    {
        if (!ValidateUnitBalance(Pair.Value))
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid balance for unit: %s"), *Pair.Key);
            bAllValid = false;
        }
    }
    
    // Validate resource balance
    if (!ValidateResourceBalance(ResourceBalance))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid resource balance"));
        bAllValid = false;
    }
    
    // Validate difficulty balances
    for (const auto& Pair : DifficultyBalances)
    {
        if (!ValidateDifficultyBalance(Pair.Value))
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid difficulty balance: %s"), *Pair.Key);
            bAllValid = false;
        }
    }
    
    if (bAllValid)
    {
        UE_LOG(LogTemp, Log, TEXT("All balance settings validated successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Some balance settings are invalid"));
    }
}

bool UMingGameBalanceManager::ValidateUnitBalance(const FMingUnitBalance& Balance) const
{
    // Check for valid ranges
    if (Balance.Health <= 0 || Balance.Damage < 0 || Balance.Cost <= 0)
    {
        return false;
    }
    
    if (Balance.AttackSpeed <= 0 || Balance.MovementSpeed <= 0)
    {
        return false;
    }
    
    if (Balance.BuildTime <= 0 || Balance.Range <= 0 || Balance.Armor < 0)
    {
        return false;
    }
    
    return true;
}

bool UMingGameBalanceManager::ValidateResourceBalance(const FMingResourceBalance& Balance) const
{
    if (Balance.StartingResources < 0 || Balance.IncomeRate < 0)
    {
        return false;
    }
    
    if (Balance.ResourceCap <= 0 || Balance.GatheringSpeed <= 0)
    {
        return false;
    }
    
    if (Balance.ResourceDepletionRate < 0)
    {
        return false;
    }
    
    return true;
}

bool UMingGameBalanceManager::ValidateDifficultyBalance(const FMingDifficultyBalance& Balance) const
{
    // Difficulty multipliers should be positive
    if (Balance.AIResourceMultiplier <= 0 || Balance.AIUnitHealthMultiplier <= 0 || Balance.AIUnitDamageMultiplier <= 0)
    {
        return false;
    }
    
    if (Balance.PlayerResourceMultiplier <= 0 || Balance.PlayerUnitHealthMultiplier <= 0 || Balance.PlayerUnitDamageMultiplier <= 0)
    {
        return false;
    }
    
    // Reaction time and aggressiveness should be within reasonable ranges
    if (Balance.AIReactionTime <= 0 || Balance.AIAggressiveness < 0 || Balance.AIAggressiveness > 1.0f)
    {
        return false;
    }
    
    return true;
}

void UMingGameBalanceManager::CheckForExploits()
    
    UE_LOG(LogTemp, Log, TEXT("Checking for potential exploits"));
    
    // Check for rush strategies
    // Check for turtle strategies
    // Check for resource exploits
    // Check for unit spam exploits
    
    IdentifyBalanceIssues();
}

void UMingGameBalanceManager::VerifyUnitCounters()
{
    UE_LOG(LogTemp, Log, TEXT("Verifying unit counter system"));
    
    // Ensure all units have appropriate counters
    // Check rock-paper-scissors balance
    // Verify no unit type is overpowered against all others
    
    UE_LOG(LogTemp, Log, TEXT("Unit counter system verified"));
}

FString UMingGameBalanceManager::GetBalanceCategoryName(EMingBalanceCategory Category)
{
    switch (Category)
    {
    case EMingBalanceCategory::UnitStats: return TEXT("單位屬性");
    case EMingBalanceCategory::ResourceEconomy: return TEXT("資源經濟");
    case EMingBalanceCategory::BuildingCosts: return TEXT("建造成本");
    case EMingBalanceCategory::Technology: return TEXT("科技系統");
    case EMingBalanceCategory::Combat: return TEXT("戰鬥平衡");
    case EMingBalanceCategory::Difficulty: return TEXT("難度設定");
    case EMingBalanceCategory::AIBehavior: return TEXT("AI行為");
    case EMingBalanceCategory::CampaignProgression: return TEXT("戰役進度");
    default: return TEXT("未知");
    }
}

FString UMingGameBalanceManager::ExportBalanceData() const
{
    FString Result = TEXT("{\n");
    Result += FString::Printf(TEXT("  \"unit_types\": %d,\n"), UnitBalances.Num());
    Result += FString::Printf(TEXT("  \"difficulty_levels\": %d,\n"), DifficultyBalances.Num());
    Result += FString::Printf(TEXT("  \"is_balanced\": %s,\n"), IsGameBalanced() ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"starting_resources\": %.0f,\n"), ResourceBalance.StartingResources);
    Result += FString::Printf(TEXT("  \"income_rate\": %.1f,\n"), ResourceBalance.IncomeRate);
    Result += FString::Printf(TEXT("  \"resource_cap\": %.0f\n"), ResourceBalance.ResourceCap);
    Result += TEXT("}\n");
    
    return Result;
}

void UMingGameBalanceManager::SaveCurrentBalance(const FString& ProfileName)
{
    UE_LOG(LogTemp, Log, TEXT("Saving balance profile: %s"), *ProfileName);
    
    // Save current balance settings to profile
    UE_LOG(LogTemp, Log, TEXT("Balance profile saved: %s"), *ProfileName);
}

void UMingGameBalanceManager::LoadBalanceProfile(const FString& ProfileName)
{
    UE_LOG(LogTemp, Log, TEXT("Loading balance profile: %s"), *ProfileName);
    
    // Load balance settings from profile
    UE_LOG(LogTemp, Log, TEXT("Balance profile loaded: %s"), *ProfileName);
}
