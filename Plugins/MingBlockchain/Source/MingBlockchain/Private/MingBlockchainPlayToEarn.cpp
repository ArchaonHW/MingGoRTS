#include "MingBlockchainPlayToEarn.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

UMingBlockchainPlayToEarn::UMingBlockchainPlayToEarn()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    bPlayToEarnEnabled = true;
    GlobalRewardMultiplier = 1.0f;
    ExperiencePerLevel = 100.0f;
    DailyBonusAmount = 10.0f;
    ReferralBonusPercentage = 0.1f; // 10%
    MaxEventsPerDay = 100;
    LeaderboardUpdateInterval = 60.0f; // Update every minute
    TimeSinceLastLeaderboardUpdate = 0.0f;

    MiningSystem = nullptr;
    BlockchainWallet = nullptr;
}

void UMingBlockchainPlayToEarn::BeginPlay()
{
    Super::BeginPlay();
    InitializeRewardConfigs();
}

void UMingBlockchainPlayToEarn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Save all player progress
    for (auto& PlayerProgress : PlayerProgressData)
    {
        SavePlayerProgress(PlayerProgress.Key);
    }
    
    Super::EndPlay(EndPlayReason);
}

void UMingBlockchainPlayToEarn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bPlayToEarnEnabled)
    {
        TimeSinceLastLeaderboardUpdate += DeltaTime;
        
        if (TimeSinceLastLeaderboardUpdate >= LeaderboardUpdateInterval)
        {
            UpdateLeaderboard();
            TimeSinceLastLeaderboardUpdate = 0.0f;
        }

        // Process pending events
        for (auto& PlayerProgress : PlayerProgressData)
        {
            ProcessPendingEvents(PlayerProgress.Key);
        }

        // Clean up old events
        CleanupOldEvents();
    }
}

void UMingBlockchainPlayToEarn::InitializePlayToEarn(UMingBlockchainMiningSystem* InMiningSystem, UMingBlockchainWallet* InWallet)
{
    MiningSystem = InMiningSystem;
    BlockchainWallet = InWallet;

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Initialized with mining system and wallet"));
}

FString UMingBlockchainPlayToEarn::TriggerPlayToEarnEvent(const FString& PlayerID, EPlayToEarnEventType EventType, const TMap<FString, FString>& EventData)
{
    if (!bPlayToEarnEnabled)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainPlayToEarn: Play-to-earn is disabled"));
        return TEXT("");
    }

    if (!IsPlayerEligible(PlayerID, EventType))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainPlayToEarn: Player %s not eligible for event %d"), *PlayerID, (int32)EventType);
        return TEXT("");
    }

    FPlayToEarnEvent Event;
    Event.EventID = GenerateEventID();
    Event.PlayerID = PlayerID;
    Event.EventType = EventType;
    Event.EventName = GetEventName(EventType);
    Event.Description = GetEventDescription(EventType, EventData);
    Event.Timestamp = GetWorld()->GetTimeSeconds();
    Event.BaseRewardValue = CalculateBaseReward(EventType, GetPlayerTier(PlayerID));
    Event.Multiplier = GlobalRewardMultiplier;
    Event.IsProcessed = false;
    Event.EventData = EventData;

    // Set reward tier based on player progress
    FPlayerProgress& Progress = PlayerProgressData.FindOrAdd(PlayerID);
    Event.RewardTier = CalculateRewardTier(Progress);

    PendingEvents.Add(Event);

    // Update player statistics
    UpdatePlayerStatistics(PlayerID, Event);

    // Set cooldown for this event
    FPlayToEarnRewardConfig Config = GetRewardConfig(EventType);
    if (Config.CooldownSeconds > 0.0f)
    {
        SetEventCooldown(PlayerID, EventType, Config.CooldownSeconds);
    }

    OnPlayToEarnEventTriggered.Broadcast(Event);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Triggered event %s for player %s"), 
           *Event.EventName, *PlayerID);

    return Event.EventID;
}

int32 UMingBlockchainPlayToEarn::ProcessPendingEvents(const FString& PlayerID)
{
    int32 ProcessedCount = 0;
    
    for (int32 i = PendingEvents.Num() - 1; i >= 0; --i)
    {
        FPlayToEarnEvent& Event = PendingEvents[i];
        
        if (Event.PlayerID == PlayerID && !Event.IsProcessed)
        {
            ProcessEventReward(Event);
            Event.IsProcessed = true;
            ProcessedCount++;
        }
    }

    if (ProcessedCount > 0)
    {
        SavePlayerProgress(PlayerID);
        UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Processed %d events for player %s"), 
               ProcessedCount, *PlayerID);
    }

    return ProcessedCount;
}

FPlayerProgress UMingBlockchainPlayToEarn::GetPlayerProgress(const FString& PlayerID) const
{
    if (const FPlayerProgress* Progress = PlayerProgressData.Find(PlayerID))
    {
        return *Progress;
    }
    
    // Load from storage if not in memory
    LoadPlayerProgress(PlayerID);
    
    if (const FPlayerProgress* Progress = PlayerProgressData.Find(PlayerID))
    {
        return *Progress;
    }
    
    return FPlayerProgress();
}

TArray<FLeaderboardEntry> UMingBlockchainPlayToEarn::GetLeaderboard(int32 Limit) const
{
    TArray<FLeaderboardEntry> Result;
    
    for (int32 i = 0; i < Leaderboard.Num() && i < Limit; ++i)
    {
        Result.Add(Leaderboard[i]);
    }
    
    return Result;
}

FMiningReward UMingBlockchainPlayToEarn::CalculateEventReward(const FPlayToEarnEvent& Event)
{
    FMiningReward Reward;
    
    // Calculate base reward with multipliers
    float FinalRewardValue = ApplyRewardMultipliers(Event.BaseRewardValue, Event);
    
    // Determine reward type based on tier and event type
    if (Event.RewardTier >= ERewardTier::Platinum)
    {
        Reward.RewardType = EMiningRewardType::NFT;
        Reward.AssetName = TEXT("Ming Dynasty Legendary Artifact");
        Reward.Amount = 1;
    }
    else if (Event.RewardTier >= ERewardTier::Gold)
    {
        Reward.RewardType = EMiningRewardType::Token;
        Reward.AssetName = TEXT("MingCoin Gold");
        Reward.Amount = FMath::RoundToInt(FinalRewardValue * 2.0f);
    }
    else
    {
        Reward.RewardType = EMiningRewardType::Token;
        Reward.AssetName = TEXT("MingCoin");
        Reward.Amount = FMath::RoundToInt(FinalRewardValue);
    }

    Reward.Value = FinalRewardValue;
    Reward.RewardID = Event.EventID;
    Reward.TokenContract = GetTokenContractForReward(Reward.RewardType);
    Reward.Metadata = GenerateRewardMetadata(Event);

    return Reward;
}

FPlayToEarnRewardConfig UMingBlockchainPlayToEarn::GetRewardConfig(EPlayToEarnEventType EventType) const
{
    if (const FPlayToEarnRewardConfig* Config = RewardConfigs.Find(EventType))
    {
        return *Config;
    }
    
    // Return default config if not found
    FPlayToEarnRewardConfig DefaultConfig;
    DefaultConfig.EventType = EventType;
    DefaultConfig.MinTier = ERewardTier::Bronze;
    DefaultConfig.MaxTier = ERewardTier::Gold;
    DefaultConfig.MinReward = 1.0f;
    DefaultConfig.MaxReward = 50.0f;
    DefaultConfig.CooldownSeconds = 0.0f;
    DefaultConfig.IsEnabled = true;
    
    return DefaultConfig;
}

void UMingBlockchainPlayToEarn::SetRewardConfig(EPlayToEarnEventType EventType, const FPlayToEarnRewardConfig& Config)
{
    RewardConfigs.Add(EventType, Config);
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Updated reward config for event type %d"), (int32)EventType);
}

bool UMingBlockchainPlayToEarn::IsPlayerEligible(const FString& PlayerID, EPlayToEarnEventType EventType) const
{
    // Check if play-to-earn is enabled
    if (!bPlayToEarnEnabled)
    {
        return false;
    }

    // Check event cooldown
    if (IsEventOnCooldown(PlayerID, EventType))
    {
        return false;
    }

    // Check daily event limit
    FPlayerProgress Progress = GetPlayerProgress(PlayerID);
    if (Progress.EventCounts.Contains(EventType))
    {
        int32 EventCount = Progress.EventCounts[EventType];
        if (EventCount >= MaxEventsPerDay)
        {
            return false;
        }
    }

    // Check reward configuration
    FPlayToEarnRewardConfig Config = GetRewardConfig(EventType);
    if (!Config.IsEnabled)
    {
        return false;
    }

    return true;
}

ERewardTier UMingBlockchainPlayToEarn::GetPlayerTier(const FString& PlayerID) const
{
    FPlayerProgress Progress = GetPlayerProgress(PlayerID);
    return Progress.CurrentTier;
}

float UMingBlockchainPlayToEarn::GetExperienceToNextLevel(const FString& PlayerID) const
{
    FPlayerProgress Progress = GetPlayerProgress(PlayerID);
    float NextLevelExp = (Progress.PlayerLevel + 1) * ExperiencePerLevel;
    return NextLevelExp - Progress.ExperiencePoints;
}

float UMingBlockchainPlayToEarn::GetTotalEarnings(const FString& PlayerID) const
{
    FPlayerProgress Progress = GetPlayerProgress(PlayerID);
    return Progress.TotalEarnings;
}

float UMingBlockchainPlayToEarn::GetEventCooldown(const FString& PlayerID, EPlayToEarnEventType EventType) const
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float CooldownEnd = GetEventCooldownEnd(PlayerID, EventType);
    
    if (CooldownEnd > CurrentTime)
    {
        return CooldownEnd - CurrentTime;
    }
    
    return 0.0f;
}

bool UMingBlockchainPlayToEarn::ResetPlayerProgress(const FString& PlayerID)
{
    if (PlayerProgressData.Contains(PlayerID))
    {
        PlayerProgressData.Remove(PlayerID);
        SavePlayerProgress(PlayerID);
        
        UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Reset progress for player %s"), *PlayerID);
        return true;
    }
    
    return false;
}

TArray<EPlayToEarnEventType> UMingBlockchainPlayToEarn::GetSupportedEventTypes() const
{
    TArray<EPlayToEarnEventType> EventTypes;
    EventTypes.Add(EPlayToEarnEventType::BattleVictory);
    EventTypes.Add(EPlayToEarnEventType::ResourceGather);
    EventTypes.Add(EPlayToEarnEventType::BuildingComplete);
    EventTypes.Add(EPlayToEarnEventType::AchievementUnlock);
    EventTypes.Add(EPlayToEarnEventType::DailyLogin);
    EventTypes.Add(EPlayToEarnEventType::QuestComplete);
    EventTypes.Add(EPlayToEarnEventType::TournamentWin);
    EventTypes.Add(EPlayToEarnEventType::SocialActivity);
    EventTypes.Add(EPlayToEarnEventType::Exploration);
    EventTypes.Add(EPlayToEarnEventType::Crafting);
    
    return EventTypes;
}

void UMingBlockchainPlayToEarn::SetPlayToEarnEnabled(bool bEnabled)
{
    bPlayToEarnEnabled = bEnabled;
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Play-to-earn %s"), 
           bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingBlockchainPlayToEarn::IsPlayToEarnEnabled() const
{
    return bPlayToEarnEnabled;
}

void UMingBlockchainPlayToEarn::SetGlobalRewardMultiplier(float Multiplier)
{
    GlobalRewardMultiplier = FMath::Max(0.1f, Multiplier);
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Global reward multiplier set to %f"), GlobalRewardMultiplier);
}

float UMingBlockchainPlayToEarn::GetGlobalRewardMultiplier() const
{
    return GlobalRewardMultiplier;
}

void UMingBlockchainPlayToEarn::UpdatePlayerStatistics(const FString& PlayerID, const FPlayToEarnEvent& Event)
{
    FPlayerProgress& Progress = PlayerProgressData.FindOrAdd(PlayerID);
    
    // Update event count
    Progress.EventCounts.FindOrAdd(Event.EventType)++;
    
    // Add experience points
    float ExperienceGain = Event.BaseRewardValue * 10.0f; // 1 reward = 10 XP
    Progress.ExperiencePoints += ExperienceGain;
    
    // Check for level up
    int32 NewLevel = FMath::FloorToInt(Progress.ExperiencePoints / ExperiencePerLevel) + 1;
    if (NewLevel > Progress.PlayerLevel)
    {
        Progress.PlayerLevel = NewLevel;
        
        // Update tier based on new level
        ERewardTier NewTier = CalculateRewardTier(Progress);
        if (NewTier != Progress.CurrentTier)
        {
            Progress.CurrentTier = NewTier;
            OnPlayerTierChanged.Broadcast(PlayerID);
        }
    }
    
    // Update total events and earnings
    Progress.TotalEventsCompleted++;
    Progress.TotalEarnings += Event.BaseRewardValue * Event.Multiplier;
    
    // Calculate next tier progress
    float CurrentTierExp = GetExperienceForTier(Progress.CurrentTier);
    float NextTierExp = GetExperienceForTier((ERewardTier)((int32)Progress.CurrentTier + 1));
    Progress.NextTierProgress = (Progress.ExperiencePoints - CurrentTierExp) / (NextTierExp - CurrentTierExp);
}

bool UMingBlockchainPlayToEarn::GetDailyBonusStatus(const FString& PlayerID, float& OutBonusAmount) const
{
    FString CurrentDay = GetCurrentDayKey();
    FString LastBonusKey = PlayerID + TEXT("_daily_bonus_") + CurrentDay;
    
    // In a real implementation, this would check persistent storage
    // For now, assume bonus is available
    OutBonusAmount = DailyBonusAmount;
    return true;
}

float UMingBlockchainPlayToEarn::ClaimDailyBonus(const FString& PlayerID)
{
    float BonusAmount;
    if (GetDailyBonusStatus(PlayerID, BonusAmount))
    {
        // Create daily bonus event
        TMap<FString, FString> EventData;
        EventData.Add(TEXT("bonus_amount"), FString::SanitizeFloat(BonusAmount));
        
        TriggerPlayToEarnEvent(PlayerID, EPlayToEarnEventType::DailyLogin, EventData);
        
        UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Player %s claimed daily bonus of %f"), 
               *PlayerID, BonusAmount);
        
        return BonusAmount;
    }
    
    return 0.0f;
}

float UMingBlockchainPlayToEarn::GetReferralBonus(const FString& PlayerID) const
{
    FPlayerProgress Progress = GetPlayerProgress(PlayerID);
    
    // In a real implementation, this would calculate based on referred players
    // For now, return a simple calculation based on player's total earnings
    return Progress.TotalEarnings * ReferralBonusPercentage;
}

void UMingBlockchainPlayToEarn::InitializeRewardConfigs()
{
    // Battle Victory
    FPlayToEarnRewardConfig BattleConfig;
    BattleConfig.EventType = EPlayToEarnEventType::BattleVictory;
    BattleConfig.MinTier = ERewardTier::Bronze;
    BattleConfig.MaxTier = ERewardTier::Diamond;
    BattleConfig.MinReward = 5.0f;
    BattleConfig.MaxReward = 100.0f;
    BattleConfig.CooldownSeconds = 30.0f;
    BattleConfig.IsEnabled = true;
    RewardConfigs.Add(EPlayToEarnEventType::BattleVictory, BattleConfig);

    // Resource Gathering
    FPlayToEarnRewardConfig ResourceConfig;
    ResourceConfig.EventType = EPlayToEarnEventType::ResourceGather;
    ResourceConfig.MinTier = ERewardTier::Bronze;
    ResourceConfig.MaxTier = ERewardTier::Gold;
    ResourceConfig.MinReward = 1.0f;
    ResourceConfig.MaxReward = 20.0f;
    ResourceConfig.CooldownSeconds = 5.0f;
    ResourceConfig.IsEnabled = true;
    RewardConfigs.Add(EPlayToEarnEventType::ResourceGather, ResourceConfig);

    // Building Completion
    FPlayToEarnRewardConfig BuildingConfig;
    BuildingConfig.EventType = EPlayToEarnEventType::BuildingComplete;
    BuildingConfig.MinTier = ERewardTier::Silver;
    BuildingConfig.MaxTier = ERewardTier::Platinum;
    BuildingConfig.MinReward = 10.0f;
    BuildingConfig.MaxReward = 200.0f;
    BuildingConfig.CooldownSeconds = 0.0f;
    BuildingConfig.IsEnabled = true;
    RewardConfigs.Add(EPlayToEarnEventType::BuildingComplete, BuildingConfig);

    // Achievement Unlock
    FPlayToEarnRewardConfig AchievementConfig;
    AchievementConfig.EventType = EPlayToEarnEventType::AchievementUnlock;
    AchievementConfig.MinTier = ERewardTier::Silver;
    AchievementConfig.MaxTier = ERewardTier::Legendary;
    AchievementConfig.MinReward = 25.0f;
    AchievementConfig.MaxReward = 500.0f;
    AchievementConfig.CooldownSeconds = 0.0f;
    AchievementConfig.IsEnabled = true;
    RewardConfigs.Add(EPlayToEarnEventType::AchievementUnlock, AchievementConfig);

    // Daily Login
    FPlayToEarnRewardConfig DailyConfig;
    DailyConfig.EventType = EPlayToEarnEventType::DailyLogin;
    DailyConfig.MinTier = ERewardTier::Bronze;
    DailyConfig.MaxTier = ERewardTier::Gold;
    DailyConfig.MinReward = DailyBonusAmount;
    DailyConfig.MaxReward = DailyBonusAmount;
    DailyConfig.CooldownSeconds = 86400.0f; // 24 hours
    DailyConfig.IsEnabled = true;
    RewardConfigs.Add(EPlayToEarnEventType::DailyLogin, DailyConfig);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Reward configurations initialized"));
}

FString UMingBlockchainPlayToEarn::GenerateEventID() const
{
    return FGuid::NewGuid().ToString();
}

ERewardTier UMingBlockchainPlayToEarn::CalculateRewardTier(const FPlayerProgress& Progress) const
{
    if (Progress.ExperiencePoints >= 5000.0f) return ERewardTier::Legendary;
    if (Progress.ExperiencePoints >= 2500.0f) return ERewardTier::Diamond;
    if (Progress.ExperiencePoints >= 1000.0f) return ERewardTier::Platinum;
    if (Progress.ExperiencePoints >= 500.0f) return ERewardTier::Gold;
    if (Progress.ExperiencePoints >= 200.0f) return ERewardTier::Silver;
    return ERewardTier::Bronze;
}

float UMingBlockchainPlayToEarn::GetExperienceForTier(ERewardTier Tier) const
{
    switch (Tier)
    {
        case ERewardTier::Bronze:    return 0.0f;
        case ERewardTier::Silver:    return 200.0f;
        case ERewardTier::Gold:      return 500.0f;
        case ERewardTier::Platinum:  return 1000.0f;
        case ERewardTier::Diamond:   return 2500.0f;
        case ERewardTier::Legendary: return 5000.0f;
        default: return 0.0f;
    }
}

void UMingBlockchainPlayToEarn::UpdateLeaderboard()
{
    Leaderboard.Empty();
    
    // Sort players by total earnings
    TArray<FString> PlayerIDs;
    PlayerProgressData.GetKeys(PlayerIDs);
    
    PlayerIDs.Sort([this](const FString& A, const FString& B) {
        const FPlayerProgress* ProgressA = PlayerProgressData.Find(A);
        const FPlayerProgress* ProgressB = PlayerProgressData.Find(B);
        return ProgressA && ProgressB && ProgressA->TotalEarnings > ProgressB->TotalEarnings;
    });
    
    // Create leaderboard entries
    for (int32 i = 0; i < PlayerIDs.Num(); ++i)
    {
        const FPlayerProgress* Progress = PlayerProgressData.Find(PlayerIDs[i]);
        if (Progress)
        {
            FLeaderboardEntry Entry;
            Entry.PlayerID = PlayerIDs[i];
            Entry.PlayerName = PlayerIDs[i]; // In real implementation, would use actual player name
            Entry.Rank = i + 1;
            Entry.TotalEarnings = Progress->TotalEarnings;
            Entry.EventsCompleted = Progress->TotalEventsCompleted;
            Entry.Tier = Progress->CurrentTier;
            Entry.AvatarURL = TEXT(""); // In real implementation, would use actual avatar
            
            Leaderboard.Add(Entry);
        }
    }
    
    OnLeaderboardUpdated.Broadcast(Leaderboard);
}

void UMingBlockchainPlayToEarn::ProcessEventReward(const FPlayToEarnEvent& Event)
{
    FMiningReward Reward = CalculateEventReward(Event);
    
    // Send reward to wallet
    if (BlockchainWallet)
    {
        BlockchainWallet->ReceiveReward(Reward);
    }
    
    // Send to mining system for processing
    if (MiningSystem)
    {
        MiningSystem->ProcessMiningReward(Reward);
    }
    
    OnRewardEarned.Broadcast(Event.PlayerID, Reward);
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Processed reward %s for player %s"), 
           *Reward.AssetName, *Event.PlayerID);
}

bool UMingBlockchainPlayToEarn::IsEventOnCooldown(const FString& PlayerID, EPlayToEarnEventType EventType) const
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float CooldownEnd = GetEventCooldownEnd(PlayerID, EventType);
    return CooldownEnd > CurrentTime;
}

float UMingBlockchainPlayToEarn::GetEventCooldownEnd(const FString& PlayerID, EPlayToEarnEventType EventType) const
{
    const FPlayerProgress* Progress = PlayerProgressData.Find(PlayerID);
    if (Progress && Progress->LastEventTime.Contains(EventType))
    {
        FPlayToEarnRewardConfig Config = GetRewardConfig(EventType);
        return Progress->LastEventTime[EventType] + Config.CooldownSeconds;
    }
    return 0.0f;
}

void UMingBlockchainPlayToEarn::SetEventCooldown(const FString& PlayerID, EPlayToEarnEventType EventType, float Duration)
{
    FPlayerProgress& Progress = PlayerProgressData.FindOrAdd(PlayerID);
    Progress.LastEventTime.Add(EventType, GetWorld()->GetTimeSeconds() + Duration);
}

bool UMingBlockchainPlayToEarn::ValidateEventData(const FPlayToEarnEvent& Event) const
{
    return !Event.EventID.IsEmpty() && 
           !Event.PlayerID.IsEmpty() && 
           Event.Timestamp > 0.0f &&
           Event.BaseRewardValue >= 0.0f;
}

FString UMingBlockchainPlayToEarn::GetEventName(EPlayToEarnEventType EventType) const
{
    switch (EventType)
    {
        case EPlayToEarnEventType::BattleVictory:   return TEXT("Battle Victory");
        case EPlayToEarnEventType::ResourceGather:  return TEXT("Resource Gathering");
        case EPlayToEarnEventType::BuildingComplete: return TEXT("Building Completed");
        case EPlayToEarnEventType::AchievementUnlock: return TEXT("Achievement Unlocked");
        case EPlayToEarnEventType::DailyLogin:       return TEXT("Daily Login");
        case EPlayToEarnEventType::QuestComplete:     return TEXT("Quest Completed");
        case EPlayToEarnEventType::TournamentWin:    return TEXT("Tournament Victory");
        case EPlayToEarnEventType::SocialActivity:    return TEXT("Social Activity");
        case EPlayToEarnEventType::Exploration:       return TEXT("Exploration");
        case EPlayToEarnEventType::Crafting:          return TEXT("Crafting");
        default: return TEXT("Unknown Event");
    }
}

FString UMingBlockchainPlayToEarn::GetEventDescription(EPlayToEarnEventType EventType, const TMap<FString, FString>& EventData) const
{
    FString BaseDescription;
    
    switch (EventType)
    {
        case EPlayToEarnEventType::BattleVictory:
            BaseDescription = TEXT("Victorious in battle against enemy forces");
            break;
        case EPlayToEarnEventType::ResourceGather:
            BaseDescription = TEXT("Successfully gathered valuable resources");
            break;
        case EPlayToEarnEventType::BuildingComplete:
            BaseDescription = TEXT("Completed construction of an important building");
            break;
        case EPlayToEarnEventType::AchievementUnlock:
            BaseDescription = TEXT("Unlocked a significant achievement");
            break;
        case EPlayToEarnEventType::DailyLogin:
            BaseDescription = TEXT("Daily login bonus claimed");
            break;
        case EPlayToEarnEventType::QuestComplete:
            BaseDescription = TEXT("Successfully completed a quest");
            break;
        case EPlayToEarnEventType::TournamentWin:
            BaseDescription = TEXT("Achieved victory in a tournament");
            break;
        case EPlayToEarnEventType::SocialActivity:
            BaseDescription = TEXT("Participated in social activities");
            break;
        case EPlayToEarnEventType::Exploration:
            BaseDescription = TEXT("Discovered new territories");
            break;
        case EPlayToEarnEventType::Crafting:
            BaseDescription = TEXT("Crafted valuable items");
            break;
        default:
            BaseDescription = TEXT("Completed a game activity");
            break;
    }
    
    // Add specific data from EventData
    if (EventData.Contains(TEXT("details")))
    {
        BaseDescription += TEXT(": ") + EventData[TEXT("details")];
    }
    
    return BaseDescription;
}

float UMingBlockchainPlayToEarn::CalculateBaseReward(EPlayToEarnEventType EventType, ERewardTier Tier) const
{
    FPlayToEarnRewardConfig Config = GetRewardConfig(EventType);
    
    // Calculate reward based on tier
    float TierMultiplier = 1.0f + ((int32)Tier * 0.5f); // Each tier adds 50% bonus
    float BaseReward = FMath::Lerp(Config.MinReward, Config.MaxReward, TierMultiplier / 3.0f);
    
    return BaseReward;
}

float UMingBlockchainPlayToEarn::ApplyRewardMultipliers(float BaseReward, const FPlayToEarnEvent& Event) const
{
    float FinalReward = BaseReward;
    
    // Apply global multiplier
    FinalReward *= GlobalRewardMultiplier;
    
    // Apply tier multiplier
    float TierMultiplier = 1.0f + ((int32)Event.RewardTier * 0.25f);
    FinalReward *= TierMultiplier;
    
    // Apply event-specific multiplier
    FinalReward *= Event.Multiplier;
    
    return FinalReward;
}

FString UMingBlockchainPlayToEarn::GetTierName(ERewardTier Tier) const
{
    switch (Tier)
    {
        case ERewardTier::Bronze:    return TEXT("Bronze");
        case ERewardTier::Silver:    return TEXT("Silver");
        case ERewardTier::Gold:      return TEXT("Gold");
        case ERewardTier::Platinum:  return TEXT("Platinum");
        case ERewardTier::Diamond:   return TEXT("Diamond");
        case ERewardTier::Legendary: return TEXT("Legendary");
        default: return TEXT("Unknown");
    }
}

void UMingBlockchainPlayToEarn::SavePlayerProgress(const FString& PlayerID)
{
    // In a real implementation, this would save to persistent storage
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("PlayToEarn/") + PlayerID + TEXT(".json");
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Saved progress for player %s to %s"), 
           *PlayerID, *SavePath);
}

void UMingBlockchainPlayToEarn::LoadPlayerProgress(const FString& PlayerID)
{
    // In a real implementation, this would load from persistent storage
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("PlayToEarn/") + PlayerID + TEXT(".json");
    
    // Create default progress if not found
    if (!PlayerProgressData.Contains(PlayerID))
    {
        FPlayerProgress DefaultProgress;
        DefaultProgress.PlayerID = PlayerID;
        DefaultProgress.PlayerLevel = 1;
        DefaultProgress.ExperiencePoints = 0.0f;
        DefaultProgress.TotalEventsCompleted = 0;
        DefaultProgress.TotalEarnings = 0.0f;
        DefaultProgress.CurrentTier = ERewardTier::Bronze;
        DefaultProgress.NextTierProgress = 0.0f;
        
        PlayerProgressData.Add(PlayerID, DefaultProgress);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainPlayToEarn: Loaded progress for player %s from %s"), 
           *PlayerID, *LoadPath);
}

void UMingBlockchainPlayToEarn::CleanupOldEvents()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    const float MaxEventAge = 3600.0f; // 1 hour
    
    PendingEvents.RemoveAll([CurrentTime, MaxEventAge](const FPlayToEarnEvent& Event) {
        return (CurrentTime - Event.Timestamp) > MaxEventAge;
    });
}

void UMingBlockchainPlayToEarn::CheckDailyReset(const FString& PlayerID)
{
    FString CurrentDay = GetCurrentDayKey();
    FString LastResetKey = PlayerID + TEXT("_last_reset_");
    
    // In a real implementation, this would check persistent storage
    // For now, assume daily reset happens at midnight
}

FString UMingBlockchainPlayToEarn::GetCurrentDayKey() const
{
    FDateTime Now = FDateTime::Now();
    return FString::Printf(TEXT("%04d%02d%02d"), Now.GetYear(), Now.GetMonth(), Now.GetDay());
}

FString UMingBlockchainPlayToEarn::GetTokenContractForReward(EMiningRewardType RewardType) const
{
    switch (RewardType)
    {
        case EMiningRewardType::Token:    return TEXT("0x1234567890abcdef1234567890abcdef12345678");
        case EMiningRewardType::NFT:      return TEXT("0xabcdef1234567890abcdef1234567890abcdef12");
        default: return TEXT("");
    }
}

FString UMingBlockchainPlayToEarn::GenerateRewardMetadata(const FPlayToEarnEvent& Event) const
{
    FString Metadata = TEXT("{");
    Metadata += TEXT("\"event_id\":\"") + Event.EventID + TEXT("\",");
    Metadata += TEXT("\"player_id\":\"") + Event.PlayerID + TEXT("\",");
    Metadata += TEXT("\"event_type\":") + FString::FromInt((int32)Event.EventType) + TEXT(",");
    Metadata += TEXT("\"event_name\":\"") + Event.EventName + TEXT("\",");
    Metadata += TEXT("\"tier\":\"") + GetTierName(Event.RewardTier) + TEXT("\",");
    Metadata += TEXT("\"base_reward\":") + FString::SanitizeFloat(Event.BaseRewardValue) + TEXT(",");
    Metadata += TEXT("\"multiplier\":") + FString::SanitizeFloat(Event.Multiplier) + TEXT(",");
    Metadata += TEXT("\"timestamp\":") + FString::SanitizeFloat(Event.Timestamp);
    Metadata += TEXT("}");
    
    return Metadata;
}
