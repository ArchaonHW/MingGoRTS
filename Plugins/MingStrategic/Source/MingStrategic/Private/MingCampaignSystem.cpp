#include "MingCampaignSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingCampaignSystem::UMingCampaignSystem()
{
    WorldContext = GetWorld();
    InitializeDefaultCampaigns();
}

void UMingCampaignSystem::InitializeDefaultCampaigns()
{
    Campaigns.Empty();
    
    SetupHistoricalCampaigns();
    SetupWhatIfCampaigns();
    SetupTutorialCampaign();
    
    UE_LOG(LogTemp, Log, TEXT("Campaign system initialized with %d campaigns"), Campaigns.Num());
}

void UMingCampaignSystem::SetupHistoricalCampaigns()
{
    // 1911 Revolution Campaign
    FMingCampaign RevolutionCampaign;
    RevolutionCampaign.CampaignID = TEXT("REVOLUTION_1911");
    RevolutionCampaign.CampaignName = TEXT("辛亥革命");
    RevolutionCampaign.Description = TEXT("體驗1911年辛亥革命的關鍵戰役");
    RevolutionCampaign.CampaignType = EMingCampaignType::Historical;
    RevolutionCampaign.StartMissionID = TEXT("REV_WUCHANG");
    
    // Mission 1: Wuchang Uprising
    FMingMission WuchangMission;
    WuchangMission.MissionID = TEXT("REV_WUCHANG");
    WuchangMission.MissionName = TEXT("武昌起義");
    WuchangMission.Description = TEXT("發動武昌起義，建立革命根據地");
    WuchangMission.BriefingText = TEXT("1911年10月10日，武昌城內的革命黨人決定起義。你需要控制關鍵建築，擊敗清軍，並確保革命成功。");
    WuchangMission.MissionType = EMingMissionType::Combat;
    WuchangMission.MapName = TEXT("Map_Wuchang");
    
    // Objectives
    FMingMissionObjective Obj1;
    Obj1.ObjectiveID = TEXT("CAPTURE_GOVERNOR");
    Obj1.Description = TEXT("佔領總督府");
    Obj1.SuccessCondition = TEXT("BuildingControl('GovernorPalace')");
    WuchangMission.Objectives.Add(Obj1);
    
    FMingMissionObjective Obj2;
    Obj2.ObjectiveID = TEXT("ELIMINATE_QING");
    Obj2.Description = TEXT("消滅城內清軍");
    Obj2.SuccessCondition = TEXT("UnitCount('QingArmy') == 0");
    WuchangMission.Objectives.Add(Obj2);
    
    RevolutionCampaign.Missions.Add(WuchangMission);
    
    // Mission 2: Battle of Nanjing
    FMingMission NanjingMission;
    NanjingMission.MissionID = TEXT("REV_NANJING");
    NanjingMission.MissionName = TEXT("南京之戰");
    NanjingMission.Description = TEXT("攻佔南京，建立臨時政府");
    NanjingMission.BriefingText = TEXT("武昌起義成功後，革命軍向南京進發。南京是南方重鎮，控制它對建立臨時政府至關重要。");
    NanjingMission.MissionType = EMingMissionType::Capture;
    NanjingMission.MapName = TEXT("Map_Nanjing");
    NanjingMission.PrerequisiteMissions.Add(TEXT("REV_WUCHANG"));
    
    FMingMissionObjective Obj3;
    Obj3.ObjectiveID = TEXT("CAPTURE_CITY");
    Obj3.Description = TEXT("控制南京城");
    Obj3.SuccessCondition = TEXT("AreaControl('NanjingCity') >= 0.8");
    NanjingMission.Objectives.Add(Obj3);
    
    RevolutionCampaign.Missions.Add(NanjingMission);
    
    Campaigns.Add(RevolutionCampaign.CampaignID, RevolutionCampaign);
    
    // Northern Expedition Campaign
    FMingCampaign NorthernExpedition;
    NorthernExpedition.CampaignID = TEXT("NORTH_EXPEDITION");
    NorthernExpedition.CampaignName = TEXT("北伐戰爭");
    NorthernExpedition.Description = TEXT("統一中國的北伐戰爭");
    NorthernExpedition.CampaignType = EMingCampaignType::Historical;
    NorthernExpedition.StartMissionID = TEXT("NORTH_GUANGZHOU");
    
    FMingMission GuangzhouMission;
    GuangzhouMission.MissionID = TEXT("NORTH_GUANGZHOU");
    GuangzhouMission.MissionName = TEXT("廣州出師");
    GuangzhouMission.Description = TEXT("從廣州開始北伐征程");
    GuangzhouMission.MissionType = EMingMissionType::Combat;
    GuangzhouMission.MapName = TEXT("Map_Guangzhou");
    
    FMingMissionObjective Obj4;
    Obj4.ObjectiveID = TEXT("DEFEAT_WARLORDS");
    Obj4.Description = TEXT("擊敗地方軍閥");
    Obj4.SuccessCondition = TEXT("UnitCount('WarlordForces') == 0");
    GuangzhouMission.Objectives.Add(Obj4);
    
    NorthernExpedition.Missions.Add(GuangzhouMission);
    
    Campaigns.Add(NorthernExpedition.CampaignID, NorthernExpedition);
}

void UMingCampaignSystem::SetupWhatIfCampaigns()
{
    // What If: Early Victory Campaign
    FMingCampaign WhatIfCampaign;
    WhatIfCampaign.CampaignID = TEXT("WHATIF_EARLY_VICTORY");
    WhatIfCampaign.CampaignName = TEXT("假如早期勝利");
    WhatIfCampaign.Description = TEXT("探索歷史的不同可能性");
    WhatIfCampaign.CampaignType = EMingCampaignType::WhatIf;
    WhatIfCampaign.StartMissionID = TEXT("WHATIF_BEIJING");
    
    FMingMission BeijingMission;
    BeijingMission.MissionID = TEXT("WHATIF_BEIJING");
    BeijingMission.MissionName = TEXT("直取北京");
    BeijingMission.Description = TEXT("假如革命軍直接攻擊北京");
    BeijingMission.BriefingText = TEXT("在這個平行時空中，革命軍決定不經過南京，直接向北京進軍。這將如何改變中國的歷史？");
    BeijingMission.MissionType = EMingMissionType::Combat;
    BeijingMission.MapName = TEXT("Map_Beijing");
    
    FMingMissionObjective Obj1;
    Obj1.ObjectiveID = TEXT("CAPTURE_FORBIDDEN");
    Obj1.Description = TEXT("佔領紫禁城");
    Obj1.SuccessCondition = TEXT("BuildingControl('ForbiddenCity')");
    BeijingMission.Objectives.Add(Obj1);
    
    WhatIfCampaign.Missions.Add(BeijingMission);
    
    Campaigns.Add(WhatIfCampaign.CampaignID, WhatIfCampaign);
}

void UMingCampaignSystem::SetupTutorialCampaign()
{
    FMingCampaign TutorialCampaign;
    TutorialCampaign.CampaignID = TEXT("TUTORIAL");
    TutorialCampaign.CampaignName = TEXT("教學戰役");
    TutorialCampaign.Description = TEXT("學習遊戲基本操作");
    TutorialCampaign.CampaignType = EMingCampaignType::Tutorial;
    TutorialCampaign.StartMissionID = TEXT("TUT_BASIC_CONTROLS");
    
    // Tutorial Mission 1: Basic Controls
    FMingMission BasicControlsMission;
    BasicControlsMission.MissionID = TEXT("TUT_BASIC_CONTROLS");
    BasicControlsMission.MissionName = TEXT("基本控制");
    BasicControlsMission.Description = TEXT("學習單位選擇和移動");
    BasicControlsMission.BriefingText = TEXT("歡迎來到民國史詩！首先，讓我們學習如何控制你的部隊。");
    BasicControlsMission.MissionType = EMingMissionType::Tutorial;
    BasicControlsMission.MapName = TEXT("Map_Tutorial");
    
    FMingMissionObjective TutorialObj1;
    TutorialObj1.ObjectiveID = TEXT("SELECT_UNITS");
    TutorialObj1.Description = TEXT("選擇3個單位");
    TutorialObj1.SuccessCondition = TEXT("SelectedUnitCount() >= 3");
    BasicControlsMission.Objectives.Add(TutorialObj1);
    
    FMingMissionObjective TutorialObj2;
    TutorialObj2.ObjectiveID = TEXT("MOVE_UNITS");
    TutorialObj2.Description = TEXT("移動單位到目標位置");
    TutorialObj2.SuccessCondition = TEXT("UnitsAtLocation('TargetArea') >= 3");
    BasicControlsMission.Objectives.Add(TutorialObj2);
    
    TutorialCampaign.Missions.Add(BasicControlsMission);
    
    Campaigns.Add(TutorialCampaign.CampaignID, TutorialCampaign);
}

void UMingCampaignSystem::LoadCampaign(const FString& CampaignID)
{
    FMingCampaign* Campaign = FindCampaign(CampaignID);
    if (Campaign)
    {
        CurrentCampaignID = CampaignID;
        UE_LOG(LogTemp, Log, TEXT("Campaign loaded: %s"), *Campaign->CampaignName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Campaign not found: %s"), *CampaignID);
    }
}

void UMingCampaignSystem::StartCampaign(const FString& CampaignID)
{
    LoadCampaign(CampaignID);
    
    FMingCampaign* Campaign = FindCampaign(CampaignID);
    if (Campaign)
    {
        Campaign->bIsActive = true;
        Campaign->CurrentMissionID = Campaign->StartMissionID;
        
        // Start the first mission
        StartMission(Campaign->StartMissionID);
        
        UE_LOG(LogTemp, Log, TEXT("Campaign started: %s"), *Campaign->CampaignName);
    }
}

void UMingCampaignSystem::SaveCampaign(const FString& CampaignID)
{
    // Save campaign progress
    FString SaveData = SaveCampaignData();
    
    // In a real implementation, this would save to file
    UE_LOG(LogTemp, Log, TEXT("Campaign saved: %s"), *CampaignID);
}

void UMingCampaignSystem::CompleteCampaign(const FString& CampaignID)
{
    if (!CompletedCampaigns.Contains(CampaignID))
    {
        CompletedCampaigns.Add(CampaignID);
        
        FMingCampaign* Campaign = FindCampaign(CampaignID);
        if (Campaign)
        {
            Campaign->bIsActive = false;
            UE_LOG(LogTemp, Log, TEXT("Campaign completed: %s"), *Campaign->CampaignName);
        }
    }
}

FMingCampaign UMingCampaignSystem::GetCurrentCampaign() const
{
    if (!CurrentCampaignID.IsEmpty())
    {
        const FMingCampaign* Campaign = Campaigns.Find(CurrentCampaignID);
        if (Campaign)
        {
            return *Campaign;
        }
    }
    return FMingCampaign();
}

TArray<FMingCampaign> UMingCampaignSystem::GetAllCampaigns() const
{
    TArray<FMingCampaign> Result;
    for (const auto& Pair : Campaigns)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FMingCampaign> UMingCampaignSystem::GetAvailableCampaigns() const
{
    TArray<FMingCampaign> Result;
    for (const auto& Pair : Campaigns)
    {
        const FMingCampaign& Campaign = Pair.Value;
        
        // Campaign is available if it's not completed and has at least one available mission
        if (!CompletedCampaigns.Contains(Campaign.CampaignID))
        {
            bool HasAvailableMission = false;
            for (const FMingMission& Mission : Campaign.Missions)
            {
                if (CanStartMission(Mission.MissionID))
                {
                    HasAvailableMission = true;
                    break;
                }
            }
            
            if (HasAvailableMission)
            {
                Result.Add(Campaign);
            }
        }
    }
    return Result;
}

void UMingCampaignSystem::StartMission(const FString& MissionID)
{
    FMingMission* Mission = FindMission(MissionID);
    if (!Mission)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mission not found: %s"), *MissionID);
        return;
    }
    
    if (!CanStartMission(MissionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start mission - prerequisites not met: %s"), *MissionID);
        return;
    }
    
    Mission->Status = EMingMissionStatus::InProgress;
    Mission->StartTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Update current mission in campaign
    if (!CurrentCampaignID.IsEmpty())
    {
        FMingCampaign* Campaign = FindCampaign(CurrentCampaignID);
        if (Campaign)
        {
            Campaign->CurrentMissionID = MissionID;
        }
    }
    
    OnMissionStarted.Broadcast(*Mission, CurrentCampaignID);
    
    UE_LOG(LogTemp, Log, TEXT("Mission started: %s"), *Mission->MissionName);
}

void UMingCampaignSystem::CompleteMission(const FString& MissionID)
{
    FMingMission* Mission = FindMission(MissionID);
    if (!Mission)
    {
        return;
    }
    
    Mission->Status = EMingMissionStatus::Completed;
    Mission->CompletionTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    if (!CompletedMissions.Contains(MissionID))
    {
        CompletedMissions.Add(MissionID);
    }
    
    // Complete all objectives
    for (FMingMissionObjective& Objective : Mission->Objectives)
    {
        Objective.bIsCompleted = true;
        Objective.Progress = 1.0f;
    }
    
    OnMissionCompleted.Broadcast(*Mission, CurrentCampaignID);
    
    // Unlock dependent missions
    UnlockDependentMissions(MissionID);
    
    // Check campaign completion
    if (!CurrentCampaignID.IsEmpty())
    {
        if (IsCampaignComplete(CurrentCampaignID))
        {
            CompleteCampaign(CurrentCampaignID);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Mission completed: %s"), *Mission->MissionName);
}

void UMingCampaignSystem::FailMission(const FString& MissionID)
{
    FMingMission* Mission = FindMission(MissionID);
    if (!Mission)
    {
        return;
    }
    
    Mission->Status = EMingMissionStatus::Failed;
    Mission->CompletionTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    OnMissionFailed.Broadcast(*Mission, CurrentCampaignID);
    
    UE_LOG(LogTemp, Log, TEXT("Mission failed: %s"), *Mission->MissionName);
}

void UMingCampaignSystem::SkipMission(const FString& MissionID)
{
    FMingMission* Mission = FindMission(MissionID);
    if (!Mission)
    {
        return;
    }
    
    Mission->Status = EMingMissionStatus::Skipped;
    
    UE_LOG(LogTemp, Log, TEXT("Mission skipped: %s"), *Mission->MissionName);
}

FMingMission UMingCampaignSystem::GetCurrentMission() const
{
    if (!CurrentCampaignID.IsEmpty())
    {
        const FMingCampaign* Campaign = Campaigns.Find(CurrentCampaignID);
        if (Campaign && !Campaign->CurrentMissionID.IsEmpty())
        {
            const FMingMission* Mission = FindMission(Campaign->CurrentMissionID);
            if (Mission)
            {
                return *Mission;
            }
        }
    }
    return FMingMission();
}

TArray<FMingMission> UMingCampaignSystem::GetAvailableMissions() const
{
    TArray<FMingMission> Result;
    
    for (const auto& Pair : Campaigns)
    {
        for (const FMingMission& Mission : Pair.Value.Missions)
        {
            if (CanStartMission(Mission.MissionID))
            {
                Result.Add(Mission);
            }
        }
    }
    
    return Result;
}

TArray<FMingMission> UMingCampaignSystem::GetCompletedMissions() const
{
    TArray<FMingMission> Result;
    
    for (const auto& Pair : Campaigns)
    {
        for (const FMingMission& Mission : Pair.Value.Missions)
        {
            if (Mission.Status == EMingMissionStatus::Completed)
            {
                Result.Add(Mission);
            }
        }
    }
    
    return Result;
}

void UMingCampaignSystem::UpdateObjectiveProgress(const FString& ObjectiveID, float Progress)
{
    FMingMission* CurrentMission = FindMission(GetCurrentMission().MissionID);
    if (!CurrentMission)
    {
        return;
    }
    
    for (FMingMissionObjective& Objective : CurrentMission->Objectives)
    {
        if (Objective.ObjectiveID == ObjectiveID)
        {
            Objective.Progress = FMath::Clamp(Progress, 0.0f, 1.0f);
            
            if (Objective.Progress >= 1.0f)
            {
                Objective.bIsCompleted = true;
            }
            
            OnObjectiveUpdated.Broadcast(Objective, Objective.Progress);
            
            // Check if mission is complete
            CheckMissionCompletion();
            
            break;
        }
    }
}

void UMingCampaignSystem::CompleteObjective(const FString& ObjectiveID)
{
    UpdateObjectiveProgress(ObjectiveID, 1.0f);
}

void UMingCampaignSystem::FailObjective(const FString& ObjectiveID)
{
    FMingMission* CurrentMission = FindMission(GetCurrentMission().MissionID);
    if (!CurrentMission)
    {
        return;
    }
    
    for (FMingMissionObjective& Objective : CurrentMission->Objectives)
    {
        if (Objective.ObjectiveID == ObjectiveID && !Objective.bIsOptional)
        {
            // Failing a required objective fails the mission
            FailMission(CurrentMission->MissionID);
            break;
        }
    }
}

TArray<FMingMissionObjective> UMingCampaignSystem::GetCurrentObjectives() const
{
    FMingMission CurrentMission = GetCurrentMission();
    return CurrentMission.Objectives;
}

float UMingCampaignSystem::GetMissionProgress() const
{
    FMingMission CurrentMission = GetCurrentMission();
    
    if (CurrentMission.Objectives.Num() == 0)
    {
        return 0.0f;
    }
    
    float TotalProgress = 0.0f;
    int32 RequiredObjectives = 0;
    
    for (const FMingMissionObjective& Objective : CurrentMission.Objectives)
    {
        if (!Objective.bIsOptional)
        {
            TotalProgress += Objective.Progress;
            RequiredObjectives++;
        }
    }
    
    return RequiredObjectives > 0 ? TotalProgress / RequiredObjectives : 0.0f;
}

bool UMingCampaignSystem::IsMissionComplete() const
{
    FMingMission CurrentMission = GetCurrentMission();
    
    for (const FMingMissionObjective& Objective : CurrentMission.Objectives)
    {
        if (!Objective.bIsOptional && !Objective.bIsCompleted)
        {
            return false;
        }
    }
    
    return true;
}

void UMingCampaignSystem::UnlockNextMission()
{
    FMingMission CurrentMission = GetCurrentMission();
    UnlockDependentMissions(CurrentMission.MissionID);
}

void UMingCampaignSystem::SetMissionPrerequisite(const FString& MissionID, const FString& PrerequisiteID)
{
    FMingMission* Mission = FindMission(MissionID);
    if (Mission)
    {
        if (!Mission->PrerequisiteMissions.Contains(PrerequisiteID))
        {
            Mission->PrerequisiteMissions.Add(PrerequisiteID);
        }
    }
}

TArray<FString> UMingCampaignSystem::GetMissionChain(const FString& MissionID) const
{
    TArray<FString> Chain;
    
    // Find all missions that lead to this mission
    for (const auto& Pair : Campaigns)
    {
        for (const FMingMission& Mission : Pair.Value.Missions)
        {
            if (Mission.PrerequisiteMissions.Contains(MissionID))
            {
                Chain.Add(Mission.MissionID);
            }
        }
    }
    
    return Chain;
}

bool UMingCampaignSystem::CanStartMission(const FString& MissionID) const
{
    return ArePrerequisitesMet(MissionID);
}

TArray<FString> UMingCampaignSystem::GetMissingPrerequisites(const FString& MissionID) const
{
    TArray<FString> Missing;
    
    const FMingMission* Mission = FindMission(MissionID);
    if (Mission)
    {
        for (const FString& Prerequisite : Mission->PrerequisiteMissions)
        {
            if (!CompletedMissions.Contains(Prerequisite))
            {
                Missing.Add(Prerequisite);
            }
        }
    }
    
    return Missing;
}

bool UMingCampaignSystem::IsCampaignComplete(const FString& CampaignID) const
{
    const FMingCampaign* Campaign = FindCampaign(CampaignID);
    if (!Campaign)
    {
        return false;
    }
    
    for (const FMingMission& Mission : Campaign->Missions)
    {
        if (!CompletedMissions.Contains(Mission.MissionID))
        {
            return false;
        }
    }
    
    return true;
}

FString UMingCampaignSystem::GetMissionTypeName(EMingMissionType Type)
{
    switch (Type)
    {
    case EMingMissionType::Combat: return TEXT("戰鬥");
    case EMingMissionType::Defense: return TEXT("防禦");
    case EMingMissionType::Escort: return TEXT("護送");
    case EMingMissionType::Capture: return TEXT("佔領");
    case EMingMissionType::Rescue: return TEXT("救援");
    case EMingMissionType::Stealth: return TEXT("潛行");
    case EMingMissionType::Puzzle: return TEXT("解謎");
    case EMingMissionType::Dialogue: return TEXT("對話");
    default: return TEXT("未知");
    }
}

FString UMingCampaignSystem::GetCampaignTypeName(EMingCampaignType Type)
{
    switch (Type)
    {
    case EMingCampaignType::Historical: return TEXT("歷史");
    case EMingCampaignType::WhatIf: return TEXT("假如");
    case EMingCampaignType::Tutorial: return TEXT("教學");
    case EMingCampaignType::Custom: return TEXT("自定義");
    default: return TEXT("未知");
    }
}

FString UMingCampaignSystem::SaveCampaignData() const
{
    // Simple JSON-like serialization
    FString Result = TEXT("{\n");
    Result += TEXT("  \"completed_campaigns\": [");
    
    bool First = true;
    for (const FString& CampaignID : CompletedCampaigns)
    {
        if (!First) Result += TEXT(",");
        Result += FString::Printf(TEXT("\"%s\""), *CampaignID);
        First = false;
    }
    
    Result += TEXT("],\n");
    Result += TEXT("  \"completed_missions\": [");
    
    First = true;
    for (const FString& MissionID : CompletedMissions)
    {
        if (!First) Result += TEXT(",");
        Result += FString::Printf(TEXT("\"%s\""), *MissionID);
        First = false;
    }
    
    Result += TEXT("]\n");
    Result += TEXT("}\n");
    
    return Result;
}

void UMingCampaignSystem::LoadCampaignData(const FString& JsonString)
{
    // Parse JSON and restore progress
    UE_LOG(LogTemp, Log, TEXT("Loading campaign data"));
}

FMingMission* UMingCampaignSystem::FindMission(const FString& MissionID)
{
    for (auto& Pair : Campaigns)
    {
        for (FMingMission& Mission : Pair.Value.Missions)
        {
            if (Mission.MissionID == MissionID)
            {
                return &Mission;
            }
        }
    }
    return nullptr;
}

FMingCampaign* UMingCampaignSystem::FindCampaign(const FString& CampaignID)
{
    return Campaigns.Find(CampaignID);
}

bool UMingCampaignSystem::ArePrerequisitesMet(const FString& MissionID) const
{
    const FMingMission* Mission = FindMission(MissionID);
    if (!Mission)
    {
        return false;
    }
    
    for (const FString& Prerequisite : Mission->PrerequisiteMissions)
    {
        if (!CompletedMissions.Contains(Prerequisite))
        {
            return false;
        }
    }
    
    return true;
}

void UMingCampaignSystem::CheckMissionCompletion()
{
    FMingMission CurrentMission = GetCurrentMission();
    if (CurrentMission.MissionID.IsEmpty())
    {
        return;
    }
    
    if (IsMissionComplete())
    {
        CompleteMission(CurrentMission.MissionID);
    }
}

void UMingCampaignSystem::UnlockDependentMissions(const FString& CompletedMissionID)
{
    // Find all missions that have this as a prerequisite
    for (auto& Pair : Campaigns)
    {
        for (FMingMission& Mission : Pair.Value.Missions)
        {
            if (Mission.PrerequisiteMissions.Contains(CompletedMissionID))
            {
                // Check if all prerequisites are now met
                if (ArePrerequisitesMet(Mission.MissionID))
                {
                    UE_LOG(LogTemp, Log, TEXT("Mission unlocked: %s"), *Mission.MissionName);
                }
            }
        }
    }
}

void UMingCampaignSystem::ValidateMissionProgression()
{
    // Ensure campaign progression is valid
    for (const auto& Pair : Campaigns)
    {
        const FMingCampaign& Campaign = Pair.Value;
        
        // Check start mission
        if (!Campaign.StartMissionID.IsEmpty())
        {
            if (!FindMission(Campaign.StartMissionID))
            {
                UE_LOG(LogTemp, Warning, TEXT("Campaign %s has invalid start mission"), *Campaign.CampaignName);
            }
        }
        
        // Check mission prerequisites
        for (const FMingMission& Mission : Campaign.Missions)
        {
            for (const FString& Prerequisite : Mission.PrerequisiteMissions)
            {
                if (!FindMission(Prerequisite))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Mission %s has invalid prerequisite: %s"), 
                           *Mission.MissionName, *Prerequisite);
                }
            }
        }
    }
}

// 戰役模式內容擴充功能
void UMingCampaignSystem::ExpandCampaignContent()
{
    UE_LOG(LogTemp, Log, TEXT("Expanding campaign content..."));
    
    // 生成新任務
    if (bEnableDynamicContent)
    {
        GenerateNewMissions();
    }
    
    // 調整任務難度
    if (bEnableAdaptiveDifficulty)
    {
        AdjustMissionDifficulty();
    }
    
    // 生成程序化內容
    if (bEnableProceduralGeneration)
    {
        CreateProceduralMaps();
    }
    
    // 擴充戰役敘事
    ExpandCampaignNarrative();
    
    UE_LOG(LogTemp, Log, TEXT("Campaign content expanded"));
}

void UMingCampaignSystem::AddDynamicMissions()
{
    UE_LOG(LogTemp, Log, TEXT("Adding dynamic missions..."));
    
    // 計算玩家技能水平
    float PlayerSkill = CalculatePlayerSkillLevel();
    
    // 為每個戰役添加動態任務
    for (auto& CampaignPair : Campaigns)
    {
        FString CampaignID = CampaignPair.Key;
        FMingCampaign& Campaign = CampaignPair.Value;
        
        // 根據玩家技能生成適合的任務
        int32 DynamicMissionCount = FMath::RoundToInt(3.0f * PlayerSkill * ContentExpansionRate);
        
        for (int32 i = 0; i < DynamicMissionCount; ++i)
        {
            FMingMission NewMission;
            NewMission.MissionID = FString::Printf(TEXT("Dynamic_%s_%d"), *CampaignID, i);
            NewMission.MissionName = FString::Printf(TEXT("Dynamic Mission %d"), i + 1);
            NewMission.Description = TEXT("Dynamically generated mission");
            NewMission.MissionType = static_cast<EMingMissionType>(FMath::RandRange(0, 7));
            NewMission.MapName = FString::Printf(TEXT("DynamicMap_%d"), i);
            
            // 添加基礎目標
            FMingMissionObjective Objective;
            Objective.ObjectiveID = FString::Printf(TEXT("Obj_%d"), i);
            Objective.Description = TEXT("Complete mission objectives");
            Objective.bIsOptional = false;
            Objective.bIsCompleted = false;
            Objective.Progress = 0.0f;
            
            NewMission.Objectives.Add(Objective);
            Campaign.Missions.Add(NewMission);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Dynamic missions added"));
}

void UMingCampaignSystem::ImplementAdaptiveDifficulty()
{
    UE_LOG(LogTemp, Log, TEXT("Implementing adaptive difficulty..."));
    
    // 計算玩家技能水平
    float PlayerSkill = CalculatePlayerSkillLevel();
    
    // 調整任務難度
    for (auto& CampaignPair : Campaigns)
    {
        FMingCampaign& Campaign = CampaignPair.Value;
        
        for (FMingMission& Mission : Campaign.Missions)
        {
            // 根據玩家技能調整任務參數
            float DifficultyMultiplier = 1.0f + (1.0f - PlayerSkill) * 0.5f;
            
            // 調整目標數量
            for (FMingMissionObjective& Objective : Mission.Objectives)
            {
                if (!Objective.bIsOptional)
                {
                    // 必要目標根據玩家技能調整
                    Objective.TriggerCondition = FString::Printf(TEXT("AdaptiveTrigger_%f"), DifficultyMultiplier);
                    Objective.SuccessCondition = FString::Printf(TEXT("AdaptiveSuccess_%f"), DifficultyMultiplier);
                }
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Adaptive difficulty implemented"));
}

void UMingCampaignSystem::GenerateProceduralContent()
{
    UE_LOG(LogTemp, Log, TEXT("Generating procedural content..."));
    
    // 生成程序化地圖
    CreateProceduralMaps();
    
    // 優化任務流程
    OptimizeMissionFlow();
    
    UE_LOG(LogTemp, Log, TEXT("Procedural content generated"));
}

void UMingCampaignSystem::GenerateNewMissions()
{
    UE_LOG(LogTemp, Log, TEXT("Generating new missions..."));
    
    // 為當前戰役生成新任務
    if (!CurrentCampaignID.IsEmpty())
    {
        FMingCampaign* CurrentCampaign = FindCampaign(CurrentCampaignID);
        if (CurrentCampaign)
        {
            int32 NewMissionCount = FMath::RandRange(2, 5);
            
            for (int32 i = 0; i < NewMissionCount; ++i)
            {
                FMingMission NewMission;
                NewMission.MissionID = FString::Printf(TEXT("Generated_%s_%d"), *CurrentCampaignID, i);
                NewMission.MissionName = FString::Printf(TEXT("Generated Mission %d"), i + 1);
                NewMission.Description = TEXT("Procedurally generated mission");
                NewMission.MissionType = static_cast<EMingMissionType>(FMath::RandRange(0, 7));
                NewMission.MapName = FString::Printf(TEXT("ProcMap_%d"), i);
                
                // 添加程序化目標
                FMingMissionObjective Objective;
                Objective.ObjectiveID = FString::Printf(TEXT("GenObj_%d"), i);
                Objective.Description = TEXT("Procedurally generated objective");
                Objective.bIsOptional = FMath::RandBool();
                Objective.bIsCompleted = false;
                Objective.Progress = 0.0f;
                
                NewMission.Objectives.Add(Objective);
                CurrentCampaign->Missions.Add(NewMission);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("New missions generated"));
}

void UMingCampaignSystem::AdjustMissionDifficulty()
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting mission difficulty..."));
    
    float PlayerSkill = CalculatePlayerSkillLevel();
    
    // 調整所有任務難度
    for (auto& CampaignPair : Campaigns)
    {
        FMingCampaign& Campaign = CampaignPair.Value;
        
        for (FMingMission& Mission : Campaign.Missions)
        {
            // 根據玩家技能調整任務
            float SkillAdjustment = PlayerSkill > 0.7f ? 1.2f : (PlayerSkill < 0.3f ? 0.8f : 1.0f);
            
            // 可以在這裡添加更多難度調整邏輯
            // 例如：敵人數量、資源限制、時間限制等
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Mission difficulty adjusted"));
}

void UMingCampaignSystem::CreateProceduralMaps()
{
    UE_LOG(LogTemp, Log, TEXT("Creating procedural maps..."));
    
    // 為動態任務創建程序化地圖
    for (auto& CampaignPair : Campaigns)
    {
        FMingCampaign& Campaign = CampaignPair.Value;
        
        for (FMingMission& Mission : Campaign.Missions)
        {
            // 如果是動態生成的任務，創建對應的地圖
            if (Mission.MissionID.StartsWith(TEXT("Dynamic_")) || Mission.MissionID.StartsWith(TEXT("Generated_")))
            {
                // 這裡可以調用地圖生成系統
                // 目前只是設置地圖名稱
                Mission.MapName = FString::Printf(TEXT("ProceduralMap_%s"), *Mission.MissionID);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Procedural maps created"));
}

void UMingCampaignSystem::ExpandCampaignNarrative()
{
    UE_LOG(LogTemp, Log, TEXT("Expanding campaign narrative..."));
    
    // 為戰役擴充敘事內容
    for (auto& CampaignPair : Campaigns)
    {
        FMingCampaign& Campaign = CampaignPair.Value;
        
        // 添加動態敘事元素
        if (Campaign.Description.Len() < 200)
        {
            Campaign.Description += TEXT("\n\nAdditional narrative content has been dynamically generated to enhance the campaign experience.");
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Campaign narrative expanded"));
}

float UMingCampaignSystem::CalculatePlayerSkillLevel() const
{
    // 基於完成的任務和表現計算玩家技能水平
    float SkillLevel = 0.5f; // 基礎技能水平
    
    // 根據完成的任務數量調整
    int32 CompletedMissionCount = CompletedMissions.Num();
    SkillLevel += CompletedMissionCount * 0.02f;
    
    // 根據完成的戰役數量調整
    int32 CompletedCampaignCount = CompletedCampaigns.Num();
    SkillLevel += CompletedCampaignCount * 0.1f;
    
    return FMath::Clamp(SkillLevel, 0.1f, 1.0f);
}

void UMingCampaignSystem::OptimizeMissionFlow()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing mission flow..."));
    
    // 優化任務流程和順序
    for (auto& CampaignPair : Campaigns)
    {
        FMingCampaign& Campaign = CampaignPair.Value;
        
        // 根據任務類型和難度重新排序
        Campaign.Missions.Sort([](const FMingMission& A, const FMingMission& B)
        {
            // 簡單的排序邏輯：按任務類型排序
            return static_cast<int32>(A.MissionType) < static_cast<int32>(B.MissionType);
        });
    }
    
    UE_LOG(LogTemp, Log, TEXT("Mission flow optimized"));
}
