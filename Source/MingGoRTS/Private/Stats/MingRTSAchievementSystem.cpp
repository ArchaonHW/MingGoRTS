#include "Stats/MingRTSAchievementSystem.h"
#include "Misc/DateTime.h"

UMingRTSAchievementSystem::UMingRTSAchievementSystem()
{
}

void UMingRTSAchievementSystem::InitializeAchievementSystem()
{
    LoadAchievementDefinitions();
    UE_LOG(LogTemp, Log, TEXT("Achievement System Initialized with %d achievements"), AchievementDefinitions.Num());
}

void UMingRTSAchievementSystem::LoadAchievementDefinitions()
{
    if (AchievementDefinitions.Num() > 0)
    {
        return; // 已經載入過
    }
    
    InitializeDefaultAchievements();
}

void UMingRTSAchievementSystem::InitializeDefaultAchievements()
{
    // 戰役成就
    AchievementDefinitions.Add(TEXT("FIRST_VICTORY"), 
        CreateAchievement(TEXT("FIRST_VICTORY"), 
            FText::FromString(TEXT("初戰告捷")),
            FText::FromString(TEXT("贏得第一場戰鬥")),
            EAchievementType::Campaign, EAchievementRarity::Common, 10));
    
    AchievementDefinitions.Add(TEXT("CONQUEROR"),
        CreateAchievement(TEXT("CONQUEROR"),
            FText::FromString(TEXT("征服者")),
            FText::FromString(TEXT("贏得50場戰鬥")),
            EAchievementType::Campaign, EAchievementRarity::Rare, 50));
    
    AchievementDefinitions.Add(TEXT("LEGENDARY_GENERAL"),
        CreateAchievement(TEXT("LEGENDARY_GENERAL"),
            FText::FromString(TEXT("傳奇將軍")),
            FText::FromString(TEXT("贏得100場戰鬥")),
            EAchievementType::Campaign, EAchievementRarity::Epic, 100));
    
    // 戰鬥成就
    AchievementDefinitions.Add(TEXT("FIRST_BLOOD"),
        CreateAchievement(TEXT("FIRST_BLOOD"),
            FText::FromString(TEXT("第一滴血")),
            FText::FromString(TEXT("消滅第一個敵方單位")),
            EAchievementType::Combat, EAchievementRarity::Common, 5));
    
    AchievementDefinitions.Add(TEXT("MASTER_TACTICIAN"),
        CreateAchievement(TEXT("MASTER_TACTICIAN"),
            FText::FromString(TEXT("戰術大師")),
            FText::FromString(TEXT("在一場戰鬥中使用超過10種不同的戰術")),
            EAchievementType::Combat, EAchievementRarity::Rare, 30));
    
    AchievementDefinitions.Add(TEXT("PERFECT_VICTORY"),
        CreateAchievement(TEXT("PERFECT_VICTORY"),
            FText::FromString(TEXT("完美勝利")),
            FText::FromString(TEXT"在不損失任何單位的情況下贏得戰鬥")),
            EAchievementType::Combat, EAchievementRarity::Epic, 50));
    
    // 經濟成就
    AchievementDefinitions.Add(TEXT("ENTREPRENEUR"),
        CreateAchievement(TEXT("ENTREPRENEUR"),
            FText::FromString(TEXT"企業家")),
            FText::FromString(TEXT"累積超過10000金錢")),
            EAchievementType::Economic, EAchievementRarity::Common, 15));
    
    AchievementDefinitions.Add(TEXT("MAGNATE"),
        CreateAchievement(TEXT("MAGNATE"),
            FText::FromString(TEXT"大亨")),
            FText::FromString(TEXT"累積超過100000金錢")),
            EAchievementType::Economic, EAchievementRarity::Rare, 30));
    
    // 策略成就
    AchievementDefinitions.Add(TEXT("DIPLOMAT"),
        CreateAchievement(TEXT("DIPLOMAT"),
            FText::FromString(TEXT"外交家")),
            FText::FromString(TEXT"與所有勢力建立友好關係")),
            EAchievementType::Strategic, EAchievementRarity::Rare, 40));
    
    AchievementDefinitions.Add(TEXT("MASTER_SPY"),
        CreateAchievement(TEXT("MASTER_SPY"),
            FText::FromString(TEXT"間諜大師")),
            FText::FromString(TEXT"成功執行100次間諜任務")),
            EAchievementType::Strategic, EAchievementRarity::Epic, 60));
    
    // 收集成就
    AchievementDefinitions.Add(TEXT("COLLECTOR"),
        CreateAchievement(TEXT("COLLECTOR"),
            FText::FromString(TEXT"收藏家")),
            FText::FromString(TEXT"解鎖所有類型的單位")),
            EAchievementType::Collection, EAchievementRarity::Rare, 30));
    
    // 挑戰成就
    AchievementDefinitions.Add(TEXT("SPEED_RUNNER"),
        CreateAchievement(TEXT("SPEED_RUNNER"),
            FText::FromString(TEXT"速通者")),
            FText::FromString(TEXT"在10分鐘內贏得戰鬥")),
            EAchievementType::Challenge, EAchievementRarity::Epic, 50));
    
    AchievementDefinitions.Add(TEXT("IRON_WILL"),
        CreateAchievement(TEXT("IRON_WILL"),
            FText::FromString(TEXT"鋼鐵意志")),
            FText::FromString(TEXT"在只剩1%資源的情況下逆轉勝利")),
            EAchievementType::Challenge, EAchievementRarity::Legendary, 100));
    
    // 隱藏成就
    AchievementDefinitions.Add(TEXT("EASTER_EGG"),
        CreateAchievement(TEXT("EASTER_EGG"),
            FText::FromString(TEXT"彩蛋")),
            FText::FromString(TEXT"???")),
            EAchievementType::Hidden, EAchievementRarity::Legendary, 50, true));
}

FAchievementData UMingRTSAchievementSystem::CreateAchievement(FName ID, const FText& Title, 
    const FText& Description, EAchievementType Type, EAchievementRarity Rarity, int32 Points, bool bIsSecret)
{
    FAchievementData Achievement;
    Achievement.AchievementID = ID;
    Achievement.Title = Title;
    Achievement.Description = bIsSecret ? FText::FromString(TEXT"???") : Description;
    Achievement.Type = Type;
    Achievement.Rarity = Rarity;
    Achievement.Points = Points;
    Achievement.bIsSecret = bIsSecret;
    Achievement.bIsHidden = bIsSecret;
    
    return Achievement;
}

TArray<FAchievementData> UMingRTSAchievementSystem::GetAllAchievements() const
{
    TArray<FAchievementData> Result;
    for (const auto& Pair : AchievementDefinitions)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FAchievementData> UMingRTSAchievementSystem::GetAchievementsByType(EAchievementType Type) const
{
    TArray<FAchievementData> Result;
    for (const auto& Pair : AchievementDefinitions)
    {
        if (Pair.Value.Type == Type)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FAchievementData> UMingRTSAchievementSystem::GetAchievementsByRarity(EAchievementRarity Rarity) const
{
    TArray<FAchievementData> Result;
    for (const auto& Pair : AchievementDefinitions)
    {
        if (Pair.Value.Rarity == Rarity)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

bool UMingRTSAchievementSystem::GetAchievementData(FName AchievementID, FAchievementData& OutData) const
{
    const FAchievementData* Data = AchievementDefinitions.Find(AchievementID);
    if (Data)
    {
        OutData = *Data;
        return true;
    }
    return false;
}

FPlayerAchievementProgress UMingRTSAchievementSystem::GetPlayerProgress(const FString& PlayerID, FName AchievementID) const
{
    const auto* PlayerData = PlayerAchievements.Find(PlayerID);
    if (PlayerData)
    {
        const FPlayerAchievementProgress* Progress = PlayerData->Find(AchievementID);
        if (Progress)
        {
            return *Progress;
        }
    }
    
    // 返回初始進度
    FPlayerAchievementProgress InitialProgress;
    InitialProgress.AchievementID = AchievementID;
    InitialProgress.CurrentProgress = 0;
    InitialProgress.TargetProgress = 1;
    InitialProgress.bIsCompleted = false;
    
    return InitialProgress;
}

void UMingRTSAchievementSystem::UpdateAchievementProgress(const FString& PlayerID, FName AchievementID, int32 Progress)
{
    auto& PlayerData = PlayerAchievements.FindOrAdd(PlayerID);
    auto& ProgressData = PlayerData.FindOrAdd(AchievementID);
    
    ProgressData.AchievementID = AchievementID;
    ProgressData.CurrentProgress = FMath::Max(0, Progress);
    
    // 獲取成就定義中的目標進度
    const FAchievementData* Definition = AchievementDefinitions.Find(AchievementID);
    if (Definition)
    {
        // 這裡可以從定義中獲取具體的目標值
        // 簡化處理，默認為1
        ProgressData.TargetProgress = 1;
    }
    
    // 檢查是否完成
    if (!ProgressData.bIsCompleted && ProgressData.CurrentProgress >= ProgressData.TargetProgress)
    {
        UnlockAchievement(PlayerID, AchievementID);
    }
    else
    {
        OnAchievementProgressUpdated.Broadcast(PlayerID, AchievementID, ProgressData.CurrentProgress);
    }
    
    // 保存數據
    SaveAchievementData(PlayerID);
}

void UMingRTSAchievementSystem::IncrementAchievementProgress(const FString& PlayerID, FName AchievementID, int32 Amount)
{
    FPlayerAchievementProgress CurrentProgress = GetPlayerProgress(PlayerID, AchievementID);
    UpdateAchievementProgress(PlayerID, AchievementID, CurrentProgress.CurrentProgress + Amount);
}

void UMingRTSAchievementSystem::UnlockAchievement(const FString& PlayerID, FName AchievementID)
{
    auto& PlayerData = PlayerAchievements.FindOrAdd(PlayerID);
    auto& ProgressData = PlayerData.FindOrAdd(AchievementID);
    
    if (ProgressData.bIsCompleted)
    {
        return; // 已經解鎖過
    }
    
    // 檢查前置條件
    CheckPrerequisites(PlayerID, AchievementID);
    
    ProgressData.bIsCompleted = true;
    ProgressData.CurrentProgress = ProgressData.TargetProgress;
    ProgressData.UnlockDate = FDateTime::Now().ToString();
    
    FAchievementData AchievementData;
    if (GetAchievementData(AchievementID, AchievementData))
    {
        OnAchievementUnlocked.Broadcast(PlayerID, AchievementID);
        OnAchievementCompleted.Broadcast(PlayerID, AchievementData);
        
        UE_LOG(LogTemp, Log, TEXT("Achievement unlocked: %s for player %s"), *AchievementID.ToString(), *PlayerID);
        
        // 檢查相關成就
        CheckRelatedAchievements(PlayerID, AchievementID);
    }
    
    // 保存數據
    SaveAchievementData(PlayerID);
}

bool UMingRTSAchievementSystem::IsAchievementUnlocked(const FString& PlayerID, FName AchievementID) const
{
    FPlayerAchievementProgress Progress = GetPlayerProgress(PlayerID, AchievementID);
    return Progress.bIsCompleted;
}

TArray<FPlayerAchievementProgress> UMingRTSAchievementSystem::GetUnlockedAchievements(const FString& PlayerID) const
{
    TArray<FPlayerAchievementProgress> Result;
    
    const auto* PlayerData = PlayerAchievements.Find(PlayerID);
    if (PlayerData)
    {
        for (const auto& Pair : *PlayerData)
        {
            if (Pair.Value.bIsCompleted)
            {
                Result.Add(Pair.Value);
            }
        }
    }
    
    return Result;
}

TArray<FPlayerAchievementProgress> UMingRTSAchievementSystem::GetInProgressAchievements(const FString& PlayerID) const
{
    TArray<FPlayerAchievementProgress> Result;
    
    for (const auto& Pair : AchievementDefinitions)
    {
        FName AchievementID = Pair.Key;
        FPlayerAchievementProgress Progress = GetPlayerProgress(PlayerID, AchievementID);
        
        if (!Progress.bIsCompleted && Progress.CurrentProgress > 0)
        {
            Result.Add(Progress);
        }
    }
    
    return Result;
}

FAchievementStats UMingRTSAchievementSystem::GetAchievementStats(const FString& PlayerID) const
{
    FAchievementStats Stats;
    Stats.TotalAchievements = AchievementDefinitions.Num();
    
    int32 TotalPoints = 0;
    for (const auto& Pair : AchievementDefinitions)
    {
        TotalPoints += Pair.Value.Points;
    }
    Stats.TotalPoints = TotalPoints;
    
    TArray<FPlayerAchievementProgress> Unlocked = GetUnlockedAchievements(PlayerID);
    Stats.UnlockedAchievements = Unlocked.Num();
    
    int32 EarnedPoints = 0;
    for (const auto& Progress : Unlocked)
    {
        const FAchievementData* Definition = AchievementDefinitions.Find(Progress.AchievementID);
        if (Definition)
        {
            EarnedPoints += Definition->Points;
        }
    }
    Stats.EarnedPoints = EarnedPoints;
    
    if (Stats.TotalAchievements > 0)
    {
        Stats.CompletionPercentage = (float)Stats.UnlockedAchievements / (float)Stats.TotalAchievements * 100.0f;
    }
    
    return Stats;
}

TArray<FPlayerAchievementProgress> UMingRTSAchievementSystem::GetRecentlyUnlockedAchievements(const FString& PlayerID, int32 Count) const
{
    TArray<FPlayerAchievementProgress> Unlocked = GetUnlockedAchievements(PlayerID);
    
    // 按解鎖日期排序（最新的在前）
    Unlocked.Sort([](const FPlayerAchievementProgress& A, const FPlayerAchievementProgress& B) {
        return A.UnlockDate > B.UnlockDate;
    });
    
    // 返回前N個
    if (Unlocked.Num() > Count)
    {
        Unlocked.SetNum(Count);
    }
    
    return Unlocked;
}

void UMingRTSAchievementSystem::ResetPlayerAchievements(const FString& PlayerID)
{
    PlayerAchievements.Remove(PlayerID);
    UE_LOG(LogTemp, Log, TEXT("Reset achievements for player %s"), *PlayerID);
}

void UMingRTSAchievementSystem::SaveAchievementData(const FString& PlayerID)
{
    // 這裡應該實現保存到文件的邏輯
    UE_LOG(LogTemp, Log, TEXT("Saved achievement data for player %s"), *PlayerID);
}

void UMingRTSAchievementSystem::LoadAchievementData(const FString& PlayerID)
{
    // 這裡應該實現從文件載入的邏輯
    UE_LOG(LogTemp, Log, TEXT("Loaded achievement data for player %s"), *PlayerID);
}

void UMingRTSAchievementSystem::CheckPrerequisites(const FString& PlayerID, FName AchievementID)
{
    const FAchievementData* Definition = AchievementDefinitions.Find(AchievementID);
    if (!Definition)
    {
        return;
    }
    
    // 檢查前置成就
    for (FName Prereq : Definition->Prerequisites)
    {
        if (!IsAchievementUnlocked(PlayerID, Prereq))
        {
            // 前置成就未解鎖，不能解鎖此成就
            return;
        }
    }
}

void UMingRTSAchievementSystem::CheckRelatedAchievements(const FString& PlayerID, FName AchievementID)
{
    // 檢查是否觸發相關成就的進度更新
    // 例如：解鎖"初戰告捷"後，更新"征服者"的進度
}

void UMingRTSAchievementSystem::BroadcastAchievementUnlock(const FString& PlayerID, FName AchievementID)
{
    OnAchievementUnlocked.Broadcast(PlayerID, AchievementID);
}
