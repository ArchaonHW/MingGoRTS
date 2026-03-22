#include "Social/MingRTSGuildSystem.h"

UMingRTSGuildSystem::UMingRTSGuildSystem()
    : bIsInitialized(false)
{
}

void UMingRTSGuildSystem::InitializeGuildSystem()
{
    if (bIsInitialized)
    {
        return;
    }
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSGuildSystem: Initialized"));
}

void UMingRTSGuildSystem::ShutdownGuildSystem()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    Guilds.Empty();
    GuildMembers.Empty();
    GuildResources.Empty();
    GuildBuildings.Empty();
    GuildTechnologies.Empty();
    GuildEvents.Empty();
    GuildDiplomacy.Empty();
    Alliances.Empty();
    PendingInvites.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSGuildSystem: Shutdown"));
}

FString UMingRTSGuildSystem::CreateGuild(const FString& GuildName, const FString& Tag, const FString& LeaderID)
{
    FString GuildID = FGuid::NewGuid().ToString();
    
    FGuildInfo Info;
    Info.GuildID = GuildID;
    Info.GuildName = GuildName;
    Info.Tag = Tag;
    Info.LeaderID = LeaderID;
    Info.CreationDate = FDateTime::Now();
    Info.CurrentMembers = 1;
    Info.GuildLevel = 1;
    
    Guilds.Add(GuildID, Info);
    
    // 添加會長
    FGuildMember Leader;
    Leader.PlayerID = LeaderID;
    Leader.Rank = EGuildRank::Leader;
    Leader.JoinDate = FDateTime::Now();
    Leader.LastActive = FDateTime::Now();
    Leader.bOnline = true;
    Leader.Permissions = GetPermissionsForRank(EGuildRank::Leader);
    Leader.Title = TEXT("會長");
    
    TArray<FGuildMember> Members;
    Members.Add(Leader);
    GuildMembers.Add(GuildID, Members);
    
    // 初始化資源
    GuildResources.Add(GuildID, FGuildResources());
    
    // 初始化建築
    TArray<FGuildBuilding> Buildings;
    GuildBuildings.Add(GuildID, Buildings);
    
    // 初始化科技
    TArray<FGuildTechnology> Techs;
    GuildTechnologies.Add(GuildID, Techs);
    
    // 初始化外交
    TArray<FGuildDiplomacy> Diplomacy;
    GuildDiplomacy.Add(GuildID, Diplomacy);
    
    // 初始化事件
    TArray<FGuildEvent> Events;
    GuildEvents.Add(GuildID, Events);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSGuildSystem: Created guild %s (%s)"), *GuildName, *GuildID);
    
    return GuildID;
}

void UMingRTSGuildSystem::DisbandGuild(const FString& GuildID)
{
    if (!Guilds.Contains(GuildID))
    {
        return;
    }
    
    // 通知所有成員
    if (GuildMembers.Contains(GuildID))
    {
        for (const FGuildMember& Member : GuildMembers[GuildID])
        {
            OnGuildMemberLeft.Broadcast(GuildID, Member.PlayerID);
        }
    }
    
    // 從所有聯盟中移除
    for (FGuildAlliance& Alliance : Alliances)
    {
        if (Alliance.MemberGuilds.Contains(GuildID))
        {
            Alliance.MemberGuilds.Remove(GuildID);
        }
    }
    
    // 清除外交關係
    GuildDiplomacy.Remove(GuildID);
    for (auto& Pair : GuildDiplomacy)
    {
        Pair.Value.RemoveAll([&](const FGuildDiplomacy& D) { return D.TargetGuildID == GuildID; });
    }
    
    // 移除公會數據
    Guilds.Remove(GuildID);
    GuildMembers.Remove(GuildID);
    GuildResources.Remove(GuildID);
    GuildBuildings.Remove(GuildID);
    GuildTechnologies.Remove(GuildID);
    GuildEvents.Remove(GuildID);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSGuildSystem: Disbanded guild %s"), *GuildID);
}

void UMingRTSGuildSystem::SetGuildInfo(const FString& GuildID, const FGuildInfo& NewInfo)
{
    if (Guilds.Contains(GuildID))
    {
        FGuildInfo& Info = Guilds[GuildID];
        Info.GuildName = NewInfo.GuildName;
        Info.Tag = NewInfo.Tag;
        Info.Description = NewInfo.Description;
        Info.Emblem = NewInfo.Emblem;
        Info.Region = NewInfo.Region;
        Info.Language = NewInfo.Language;
        Info.bRecruiting = NewInfo.bRecruiting;
        Info.MinimumLevelRequirement = NewInfo.MinimumLevelRequirement;
    }
}

FGuildInfo UMingRTSGuildSystem::GetGuildInfo(const FString& GuildID) const
{
    if (Guilds.Contains(GuildID))
    {
        return Guilds[GuildID];
    }
    return FGuildInfo();
}

TArray<FGuildInfo> UMingRTSGuildSystem::GetAllGuilds() const
{
    TArray<FGuildInfo> Result;
    for (const auto& Pair : Guilds)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

bool UMingRTSGuildSystem::DoesGuildExist(const FString& GuildID) const
{
    return Guilds.Contains(GuildID);
}

bool UMingRTSGuildSystem::InviteMember(const FString& GuildID, const FString& PlayerID, const FString& InviterID)
{
    if (!Guilds.Contains(GuildID))
    {
        return false;
    }
    
    if (!CanPerformAction(GuildID, InviterID, EGuildPermission::InviteMembers))
    {
        return false;
    }
    
    FGuildInfo& Info = Guilds[GuildID];
    if (Info.CurrentMembers >= Info.MaxMembers)
    {
        return false;
    }
    
    // 記錄邀請
    FString InviteKey = GuildID + TEXT("_") + PlayerID;
    PendingInvites.Add(InviteKey, FDateTime::Now());
    
    LogGuildAction(GuildID, InviterID, FString::Printf(TEXT("Invited player %s"), *PlayerID));
    
    return true;
}

bool UMingRTSGuildSystem::AcceptInvite(const FString& GuildID, const FString& PlayerID)
{
    FString InviteKey = GuildID + TEXT("_") + PlayerID;
    
    if (!PendingInvites.Contains(InviteKey))
    {
        return false;
    }
    
    // 檢查邀請是否過期 (7天)
    FDateTime InviteTime = PendingInvites[InviteKey];
    if (FDateTime::Now() - InviteTime > FTimespan::FromDays(7))
    {
        PendingInvites.Remove(InviteKey);
        return false;
    }
    
    // 添加到公會
    FGuildMember NewMember;
    NewMember.PlayerID = PlayerID;
    NewMember.Rank = EGuildRank::Recruit;
    NewMember.JoinDate = FDateTime::Now();
    NewMember.LastActive = FDateTime::Now();
    NewMember.bOnline = true;
    NewMember.Permissions = GetPermissionsForRank(EGuildRank::Recruit);
    
    if (GuildMembers.Contains(GuildID))
    {
        GuildMembers[GuildID].Add(NewMember);
    }
    
    // 更新人數
    if (Guilds.Contains(GuildID))
    {
        Guilds[GuildID].CurrentMembers++;
    }
    
    PendingInvites.Remove(InviteKey);
    
    OnGuildMemberJoined.Broadcast(GuildID, PlayerID, PlayerID);
    LogGuildAction(GuildID, PlayerID, TEXT("Joined the guild"));
    
    return true;
}

void UMingRTSGuildSystem::RemoveMember(const FString& GuildID, const FString& PlayerID, const FString& RemoverID)
{
    if (!Guilds.Contains(GuildID) || !GuildMembers.Contains(GuildID))
    {
        return;
    }
    
    // 檢查權限
    FGuildMember RemoverMember = GetMemberInfo(GuildID, RemoverID);
    FGuildMember TargetMember = GetMemberInfo(GuildID, PlayerID);
    
    // 不能踢出比自己等級高或相同的人（會長除外）
    if (RemoverID != Guilds[GuildID].LeaderID)
    {
        if (!IsRankHigher(RemoverMember.Rank, TargetMember.Rank))
        {
            return;
        }
        
        if (!CanPerformAction(GuildID, RemoverID, EGuildPermission::KickMembers))
        {
            return;
        }
    }
    
    // 移除成員
    TArray<FGuildMember>& Members = GuildMembers[GuildID];
    for (int32 i = 0; i < Members.Num(); ++i)
    {
        if (Members[i].PlayerID == PlayerID)
        {
            Members.RemoveAt(i);
            break;
        }
    }
    
    // 更新人數
    Guilds[GuildID].CurrentMembers--;
    
    OnGuildMemberLeft.Broadcast(GuildID, PlayerID);
    LogGuildAction(GuildID, RemoverID, FString::Printf(TEXT("Removed member %s"), *PlayerID));
}

void UMingRTSGuildSystem::PromoteMember(const FString& GuildID, const FString& PlayerID, EGuildRank NewRank, const FString& PromoterID)
{
    if (!CanPerformAction(GuildID, PromoterID, EGuildPermission::PromoteMembers))
    {
        return;
    }
    
    if (!GuildMembers.Contains(GuildID))
    {
        return;
    }
    
    TArray<FGuildMember>& Members = GuildMembers[GuildID];
    for (FGuildMember& Member : Members)
    {
        if (Member.PlayerID == PlayerID)
        {
            EGuildRank OldRank = Member.Rank;
            
            // 不能晉升到會長或比自己高的等級
            if (NewRank == EGuildRank::Leader || !IsRankHigher(GetMemberInfo(GuildID, PromoterID).Rank, NewRank))
            {
                return;
            }
            
            Member.Rank = NewRank;
            Member.Permissions = GetPermissionsForRank(NewRank);
            
            if (NewRank == EGuildRank::Officer)
            {
                Member.Title = TEXT("官員");
            }
            else if (NewRank == EGuildRank::Veteran)
            {
                Member.Title = TEXT("元老");
            }
            else if (NewRank == EGuildRank::Member)
            {
                Member.Title = TEXT("成員");
            }
            
            OnGuildRankChanged.Broadcast(PlayerID, OldRank, NewRank);
            LogGuildAction(GuildID, PromoterID, FString::Printf(TEXT("Promoted %s to %d"), *PlayerID, static_cast<int32>(NewRank)));
            break;
        }
    }
}

void UMingRTSGuildSystem::DemoteMember(const FString& GuildID, const FString& PlayerID, EGuildRank NewRank, const FString& DemoterID)
{
    if (!CanPerformAction(GuildID, DemoterID, EGuildPermission::DemoteMembers))
    {
        return;
    }
    
    if (!GuildMembers.Contains(GuildID))
    {
        return;
    }
    
    FGuildMember DemoterMember = GetMemberInfo(GuildID, DemoterID);
    FGuildMember TargetMember = GetMemberInfo(GuildID, PlayerID);
    
    // 只能降級比自己等級低的成員
    if (!IsRankHigher(DemoterMember.Rank, TargetMember.Rank))
    {
        return;
    }
    
    TArray<FGuildMember>& Members = GuildMembers[GuildID];
    for (FGuildMember& Member : Members)
    {
        if (Member.PlayerID == PlayerID)
        {
            EGuildRank OldRank = Member.Rank;
            Member.Rank = NewRank;
            Member.Permissions = GetPermissionsForRank(NewRank);
            
            OnGuildRankChanged.Broadcast(PlayerID, OldRank, NewRank);
            LogGuildAction(GuildID, DemoterID, FString::Printf(TEXT("Demoted %s to %d"), *PlayerID, static_cast<int32>(NewRank)));
            break;
        }
    }
}

TArray<FGuildMember> UMingRTSGuildSystem::GetGuildMembers(const FString& GuildID) const
{
    if (GuildMembers.Contains(GuildID))
    {
        return GuildMembers[GuildID];
    }
    return TArray<FGuildMember>();
}

FGuildMember UMingRTSGuildSystem::GetMemberInfo(const FString& GuildID, const FString& PlayerID) const
{
    if (GuildMembers.Contains(GuildID))
    {
        for (const FGuildMember& Member : GuildMembers[GuildID])
        {
            if (Member.PlayerID == PlayerID)
            {
                return Member;
            }
        }
    }
    return FGuildMember();
}

void UMingRTSGuildSystem::UpdateMemberContribution(const FString& GuildID, const FString& PlayerID, int32 Amount)
{
    if (!GuildMembers.Contains(GuildID))
    {
        return;
    }
    
    TArray<FGuildMember>& Members = GuildMembers[GuildID];
    for (FGuildMember& Member : Members)
    {
        if (Member.PlayerID == PlayerID)
        {
            Member.ContributionPoints += Amount;
            Member.WeeklyContribution += Amount;
            Member.LastActive = FDateTime::Now();
            break;
        }
    }
}

bool UMingRTSGuildSystem::HasPermission(const FString& GuildID, const FString& PlayerID, EGuildPermission Permission) const
{
    FGuildMember Member = GetMemberInfo(GuildID, PlayerID);
    return Member.Permissions.Contains(Permission);
}

void UMingRTSGuildSystem::AddGuildResources(const FString& GuildID, const FGuildResources& Resources)
{
    if (!GuildResources.Contains(GuildID))
    {
        return;
    }
    
    FGuildResources& Res = GuildResources[GuildID];
    Res.Gold += Resources.Gold;
    Res.Wood += Resources.Wood;
    Res.Stone += Resources.Stone;
    Res.Iron += Resources.Iron;
    Res.Food += Resources.Food;
    Res.Reputation += Resources.Reputation;
    Res.Influence += Resources.Influence;
    
    for (const auto& Pair : Resources.SpecialResources)
    {
        Res.SpecialResources.FindOrAdd(Pair.Key) += Pair.Value;
    }
    
    UpdateGuildPower(GuildID);
}

bool UMingRTSGuildSystem::RemoveGuildResources(const FString& GuildID, const FGuildResources& Resources)
{
    if (!GuildResources.Contains(GuildID))
    {
        return false;
    }
    
    FGuildResources& Res = GuildResources[GuildID];
    
    // 檢查資源是否足夠
    if (Res.Gold < Resources.Gold || Res.Wood < Resources.Wood ||
        Res.Stone < Resources.Stone || Res.Iron < Resources.Iron ||
        Res.Food < Resources.Food)
    {
        return false;
    }
    
    Res.Gold -= Resources.Gold;
    Res.Wood -= Resources.Wood;
    Res.Stone -= Resources.Stone;
    Res.Iron -= Resources.Iron;
    Res.Food -= Resources.Food;
    Res.Reputation -= Resources.Reputation;
    Res.Influence -= Resources.Influence;
    
    for (const auto& Pair : Resources.SpecialResources)
    {
        int32& Amount = Res.SpecialResources.FindOrAdd(Pair.Key);
        if (Amount >= Pair.Value)
        {
            Amount -= Pair.Value;
        }
    }
    
    UpdateGuildPower(GuildID);
    return true;
}

FGuildResources UMingRTSGuildSystem::GetGuildResources(const FString& GuildID) const
{
    if (GuildResources.Contains(GuildID))
    {
        return GuildResources[GuildID];
    }
    return FGuildResources();
}

bool UMingRTSGuildSystem::DepositResources(const FString& GuildID, const FString& PlayerID, const FGuildResources& Resources)
{
    AddGuildResources(GuildID, Resources);
    UpdateMemberContribution(GuildID, PlayerID, 
        Resources.Gold / 100 + Resources.Wood / 10 + Resources.Stone / 10 + Resources.Iron / 5);
    
    LogGuildAction(GuildID, PlayerID, TEXT("Deposited resources"));
    return true;
}

bool UMingRTSGuildSystem::WithdrawResources(const FString& GuildID, const FString& PlayerID, const FGuildResources& Resources)
{
    if (!CanPerformAction(GuildID, PlayerID, EGuildPermission::ManageResources))
    {
        return false;
    }
    
    return RemoveGuildResources(GuildID, Resources);
}

void UMingRTSGuildSystem::AddGuildBuilding(const FString& GuildID, const FGuildBuilding& Building)
{
    if (!GuildBuildings.Contains(GuildID))
    {
        return;
    }
    
    FGuildBuilding NewBuilding = Building;
    NewBuilding.BuildingID = FGuid::NewGuid().ToString();
    GuildBuildings[GuildID].Add(NewBuilding);
}

bool UMingRTSGuildSystem::UpgradeBuilding(const FString& GuildID, const FString& BuildingID)
{
    if (!GuildBuildings.Contains(GuildID) || !GuildResources.Contains(GuildID))
    {
        return false;
    }
    
    TArray<FGuildBuilding>& Buildings = GuildBuildings[GuildID];
    for (FGuildBuilding& Building : Buildings)
    {
        if (Building.BuildingID == BuildingID)
        {
            if (Building.Level >= Building.MaxLevel)
            {
                return false;
            }
            
            // 檢查升級資源
            FGuildResources Required;
            for (const auto& Pair : Building.UpgradeRequirements)
            {
                if (Pair.Key == TEXT("Gold")) Required.Gold = Pair.Value * Building.Level;
                else if (Pair.Key == TEXT("Wood")) Required.Wood = Pair.Value * Building.Level;
                else if (Pair.Key == TEXT("Stone")) Required.Stone = Pair.Value * Building.Level;
            }
            
            if (!RemoveGuildResources(GuildID, Required))
            {
                return false;
            }
            
            Building.Level++;
            return true;
        }
    }
    
    return false;
}

TArray<FGuildBuilding> UMingRTSGuildSystem::GetGuildBuildings(const FString& GuildID) const
{
    if (GuildBuildings.Contains(GuildID))
    {
        return GuildBuildings[GuildID];
    }
    return TArray<FGuildBuilding>();
}

bool UMingRTSGuildSystem::ResearchTechnology(const FString& GuildID, const FString& TechID)
{
    if (!GuildTechnologies.Contains(GuildID) || !GuildResources.Contains(GuildID))
    {
        return false;
    }
    
    TArray<FGuildTechnology>& Techs = GuildTechnologies[GuildID];
    for (FGuildTechnology& Tech : Techs)
    {
        if (Tech.TechID == TechID)
        {
            if (Tech.bIsResearched)
            {
                return false;
            }
            
            // 檢查前置科技
            for (const FString& Prereq : Tech.Prerequisites)
            {
                bool bFound = false;
                for (const FGuildTechnology& T : Techs)
                {
                    if (T.TechID == Prereq && T.bIsResearched)
                    {
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    return false;
                }
            }
            
            // 檢查資源
            FGuildResources Required;
            for (const auto& Pair : Tech.ResearchRequirements)
            {
                if (Pair.Key == TEXT("Gold")) Required.Gold = Pair.Value;
                else if (Pair.Key == TEXT("Reputation")) Required.Reputation = Pair.Value;
            }
            
            if (!RemoveGuildResources(GuildID, Required))
            {
                return false;
            }
            
            Tech.bIsResearched = true;
            Tech.Level = 1;
            return true;
        }
    }
    
    return false;
}

TArray<FGuildTechnology> UMingRTSGuildSystem::GetGuildTechnologies(const FString& GuildID) const
{
    if (GuildTechnologies.Contains(GuildID))
    {
        return GuildTechnologies[GuildID];
    }
    return TArray<FGuildTechnology>();
}

TArray<FString> UMingRTSGuildSystem::GetActiveTechBonuses(const FString& GuildID) const
{
    TArray<FString> Bonuses;
    
    if (GuildTechnologies.Contains(GuildID))
    {
        for (const FGuildTechnology& Tech : GuildTechnologies[GuildID])
        {
            if (Tech.bIsResearched)
            {
                Bonuses.Add(Tech.Effect);
            }
        }
    }
    
    return Bonuses;
}

FString UMingRTSGuildSystem::CreateAlliance(const FString& AllianceName, EAllianceType Type, const TArray<FString>& MemberGuilds)
{
    if (MemberGuilds.Num() < 2)
    {
        return FString();
    }
    
    FGuildAlliance Alliance;
    Alliance.AllianceID = FGuid::NewGuid().ToString();
    Alliance.AllianceName = AllianceName;
    Alliance.Type = Type;
    Alliance.MemberGuilds = MemberGuilds;
    Alliance.LeaderGuild = MemberGuilds[0];
    Alliance.FormationDate = FDateTime::Now();
    
    // 計算聯盟實力
    int32 TotalPower = 0;
    for (const FString& GuildID : MemberGuilds)
    {
        if (Guilds.Contains(GuildID))
        {
            TotalPower += Guilds[GuildID].GuildLevel * 100 + Guilds[GuildID].CurrentMembers * 10;
        }
    }
    Alliance.AlliancePower = TotalPower;
    
    Alliances.Add(Alliance);
    
    OnAllianceFormed.Broadcast(Alliance.AllianceID, MemberGuilds);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSGuildSystem: Created alliance %s"), *AllianceName);
    
    return Alliance.AllianceID;
}

void UMingRTSGuildSystem::DisbandAlliance(const FString& AllianceID)
{
    for (int32 i = 0; i < Alliances.Num(); ++i)
    {
        if (Alliances[i].AllianceID == AllianceID)
        {
            Alliances.RemoveAt(i);
            break;
        }
    }
}

bool UMingRTSGuildSystem::InviteToAlliance(const FString& AllianceID, const FString& GuildID)
{
    for (FGuildAlliance& Alliance : Alliances)
    {
        if (Alliance.AllianceID == AllianceID)
        {
            if (!Alliance.MemberGuilds.Contains(GuildID))
            {
                Alliance.MemberGuilds.Add(GuildID);
                UpdateGuildPower(GuildID);
                return true;
            }
        }
    }
    return false;
}

void UMingRTSGuildSystem::RemoveFromAlliance(const FString& AllianceID, const FString& GuildID)
{
    for (FGuildAlliance& Alliance : Alliances)
    {
        if (Alliance.AllianceID == AllianceID)
        {
            Alliance.MemberGuilds.Remove(GuildID);
            break;
        }
    }
}

TArray<FGuildAlliance> UMingRTSGuildSystem::GetAlliances() const
{
    return Alliances;
}

FGuildAlliance UMingRTSGuildSystem::GetAllianceInfo(const FString& AllianceID) const
{
    for (const FGuildAlliance& Alliance : Alliances)
    {
        if (Alliance.AllianceID == AllianceID)
        {
            return Alliance;
        }
    }
    return FGuildAlliance();
}

void UMingRTSGuildSystem::SetDiplomaticStatus(const FString& GuildID, const FString& TargetGuildID, EGuildDiplomacyStatus Status)
{
    if (!GuildDiplomacy.Contains(GuildID))
    {
        return;
    }
    
    TArray<FGuildDiplomacy>& Diplomacy = GuildDiplomacy[GuildID];
    
    // 查找現有關係
    for (FGuildDiplomacy& D : Diplomacy)
    {
        if (D.TargetGuildID == TargetGuildID)
        {
            D.Status = Status;
            D.EstablishedDate = FDateTime::Now();
            return;
        }
    }
    
    // 創建新關係
    FGuildDiplomacy NewDiplomacy;
    NewDiplomacy.TargetGuildID = TargetGuildID;
    NewDiplomacy.TargetGuildName = Guilds.Contains(TargetGuildID) ? Guilds[TargetGuildID].GuildName : TargetGuildID;
    NewDiplomacy.Status = Status;
    NewDiplomacy.EstablishedDate = FDateTime::Now();
    Diplomacy.Add(NewDiplomacy);
}

EGuildDiplomacyStatus UMingRTSGuildSystem::GetDiplomaticStatus(const FString& GuildID, const FString& TargetGuildID) const
{
    if (GuildDiplomacy.Contains(GuildID))
    {
        for (const FGuildDiplomacy& D : GuildDiplomacy[GuildID])
        {
            if (D.TargetGuildID == TargetGuildID)
            {
                return D.Status;
            }
        }
    }
    return EGuildDiplomacyStatus::Neutral;
}

TArray<FGuildDiplomacy> UMingRTSGuildSystem::GetDiplomaticRelations(const FString& GuildID) const
{
    if (GuildDiplomacy.Contains(GuildID))
    {
        return GuildDiplomacy[GuildID];
    }
    return TArray<FGuildDiplomacy>();
}

void UMingRTSGuildSystem::DeclareWar(const FString& AttackerGuildID, const FString& DefenderGuildID)
{
    SetDiplomaticStatus(AttackerGuildID, DefenderGuildID, EGuildDiplomacyStatus::AtWar);
    SetDiplomaticStatus(DefenderGuildID, AttackerGuildID, EGuildDiplomacyStatus::AtWar);
    
    OnGuildWarDeclared.Broadcast(AttackerGuildID, DefenderGuildID);
    
    LogGuildAction(AttackerGuildID, TEXT("System"), FString::Printf(TEXT("Declared war on %s"), *DefenderGuildID));
}

void UMingRTSGuildSystem::OfferPeace(const FString& GuildID, const FString& TargetGuildID)
{
    SetDiplomaticStatus(GuildID, TargetGuildID, EGuildDiplomacyStatus::Neutral);
    SetDiplomaticStatus(TargetGuildID, GuildID, EGuildDiplomacyStatus::Neutral);
    
    LogGuildAction(GuildID, TEXT("System"), FString::Printf(TEXT("Offered peace to %s"), *TargetGuildID));
}

void UMingRTSGuildSystem::CreateGuildEvent(const FString& GuildID, const FGuildEvent& Event)
{
    if (!GuildEvents.Contains(GuildID))
    {
        return;
    }
    
    FGuildEvent NewEvent = Event;
    NewEvent.EventID = FGuid::NewGuid().ToString();
    NewEvent.bIsActive = true;
    
    GuildEvents[GuildID].Add(NewEvent);
}

void UMingRTSGuildSystem::CancelGuildEvent(const FString& GuildID, const FString& EventID)
{
    if (!GuildEvents.Contains(GuildID))
    {
        return;
    }
    
    TArray<FGuildEvent>& Events = GuildEvents[GuildID];
    for (FGuildEvent& Event : Events)
    {
        if (Event.EventID == EventID)
        {
            Event.bIsActive = false;
            break;
        }
    }
}

TArray<FGuildEvent> UMingRTSGuildSystem::GetActiveEvents(const FString& GuildID) const
{
    TArray<FGuildEvent> ActiveEvents;
    
    if (GuildEvents.Contains(GuildID))
    {
        for (const FGuildEvent& Event : GuildEvents[GuildID])
        {
            if (Event.bIsActive && Event.EndTime > FDateTime::Now())
            {
                ActiveEvents.Add(Event);
            }
        }
    }
    
    return ActiveEvents;
}

void UMingRTSGuildSystem::JoinEvent(const FString& GuildID, const FString& EventID, const FString& PlayerID)
{
    if (!GuildEvents.Contains(GuildID))
    {
        return;
    }
    
    TArray<FGuildEvent>& Events = GuildEvents[GuildID];
    for (FGuildEvent& Event : Events)
    {
        if (Event.EventID == EventID && Event.bIsActive)
        {
            if (!Event.Participants.Contains(PlayerID))
            {
                Event.Participants.Add(PlayerID);
            }
            break;
        }
    }
}

void UMingRTSGuildSystem::SendGuildMessage(const FString& GuildID, const FString& PlayerID, const FString& Message)
{
    BroadcastToGuildMembers(GuildID, FString::Printf(TEXT("[%s]: %s"), *PlayerID, *Message));
}

void UMingRTSGuildSystem::PostGuildAnnouncement(const FString& GuildID, const FString& PlayerID, const FString& Title, const FString& Content)
{
    if (!CanPerformAction(GuildID, PlayerID, EGuildPermission::EditGuildInfo))
    {
        return;
    }
    
    BroadcastToGuildMembers(GuildID, FString::Printf(TEXT("[公告] %s: %s"), *Title, *Content));
    LogGuildAction(GuildID, PlayerID, FString::Printf(TEXT("Posted announcement: %s"), *Title));
}

void UMingRTSGuildSystem::AddGuildExperience(const FString& GuildID, int32 Amount)
{
    if (!Guilds.Contains(GuildID))
    {
        return;
    }
    
    FGuildInfo& Info = Guilds[GuildID];
    Info.Experience += Amount;
    
    // 檢查升級
    while (Info.Experience >= GetRequiredExperienceForNextLevel(Info.GuildLevel))
    {
        LevelUpGuild(GuildID);
    }
}

bool UMingRTSGuildSystem::LevelUpGuild(const FString& GuildID)
{
    if (!Guilds.Contains(GuildID))
    {
        return false;
    }
    
    FGuildInfo& Info = Guilds[GuildID];
    int32 Required = GetRequiredExperienceForNextLevel(Info.GuildLevel);
    
    if (Info.Experience < Required)
    {
        return false;
    }
    
    Info.Experience -= Required;
    Info.GuildLevel++;
    Info.MaxMembers += 10; // 每級增加10人上限
    
    BroadcastToGuildMembers(GuildID, FString::Printf(TEXT("公會升級至 %d 級！"), Info.GuildLevel));
    LogGuildAction(GuildID, TEXT("System"), FString::Printf(TEXT("Guild leveled up to %d"), Info.GuildLevel));
    
    return true;
}

int32 UMingRTSGuildSystem::GetRequiredExperienceForNextLevel(int32 CurrentLevel) const
{
    // 經驗需求遞增
    return CurrentLevel * CurrentLevel * 1000;
}

TArray<FGuildInfo> UMingRTSGuildSystem::SearchGuilds(const FString& SearchTerm) const
{
    TArray<FGuildInfo> Results;
    
    for (const auto& Pair : Guilds)
    {
        if (Pair.Value.GuildName.Contains(SearchTerm) || 
            Pair.Value.Tag.Contains(SearchTerm) ||
            Pair.Value.Description.Contains(SearchTerm))
        {
            Results.Add(Pair.Value);
        }
    }
    
    return Results;
}

TArray<FGuildInfo> UMingRTSGuildSystem::GetRecruitingGuilds() const
{
    TArray<FGuildInfo> Results;
    
    for (const auto& Pair : Guilds)
    {
        if (Pair.Value.bRecruiting)
        {
            Results.Add(Pair.Value);
        }
    }
    
    return Results;
}

TArray<FGuildInfo> UMingRTSGuildSystem::GetGuildsByRegion(const FString& Region) const
{
    TArray<FGuildInfo> Results;
    
    for (const auto& Pair : Guilds)
    {
        if (Pair.Value.Region == Region)
        {
            Results.Add(Pair.Value);
        }
    }
    
    return Results;
}

int32 UMingRTSGuildSystem::GetTotalGuilds() const
{
    return Guilds.Num();
}

int32 UMingRTSGuildSystem::GetTotalMembers(const FString& GuildID) const
{
    if (Guilds.Contains(GuildID))
    {
        return Guilds[GuildID].CurrentMembers;
    }
    return 0;
}

int32 UMingRTSGuildSystem::GetOnlineMembers(const FString& GuildID) const
{
    if (!GuildMembers.Contains(GuildID))
    {
        return 0;
    }
    
    int32 OnlineCount = 0;
    for (const FGuildMember& Member : GuildMembers[GuildID])
    {
        if (Member.bOnline)
        {
            OnlineCount++;
        }
    }
    
    return OnlineCount;
}

// 輔助函數
bool UMingRTSGuildSystem::CanPerformAction(const FString& GuildID, const FString& PlayerID, EGuildPermission RequiredPermission) const
{
    return HasPermission(GuildID, PlayerID, RequiredPermission);
}

void UMingRTSGuildSystem::BroadcastToGuildMembers(const FString& GuildID, const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[Guild %s] %s"), *GuildID, *Message);
}

void UMingRTSGuildSystem::UpdateGuildPower(const FString& GuildID)
{
    if (!Guilds.Contains(GuildID))
    {
        return;
    }
    
    // 計算公會實力
    int32 Power = 0;
    
    // 基礎：等級和人數
    Power += Guilds[GuildID].GuildLevel * 100;
    Power += Guilds[GuildID].CurrentMembers * 10;
    
    // 資源加成
    if (GuildResources.Contains(GuildID))
    {
        const FGuildResources& Res = GuildResources[GuildID];
        Power += Res.Gold / 1000;
        Power += Res.Reputation;
        Power += Res.Influence * 2;
    }
    
    // 建築加成
    if (GuildBuildings.Contains(GuildID))
    {
        for (const FGuildBuilding& Building : GuildBuildings[GuildID])
        {
            Power += Building.Level * 5;
        }
    }
    
    // 科技加成
    if (GuildTechnologies.Contains(GuildID))
    {
        for (const FGuildTechnology& Tech : GuildTechnologies[GuildID])
        {
            if (Tech.bIsResearched)
            {
                Power += 10;
            }
        }
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Guild %s power updated to %d"), *GuildID, Power);
}

void UMingRTSGuildSystem::CheckAllianceBenefits(const FString& AllianceID)
{
    // 檢查聯盟加成
    for (const FGuildAlliance& Alliance : Alliances)
    {
        if (Alliance.AllianceID == AllianceID)
        {
            UE_LOG(LogTemp, Verbose, TEXT("Alliance %s benefits checked"), *Alliance.AllianceName);
            break;
        }
    }
}

TArray<EGuildPermission> UMingRTSGuildSystem::GetPermissionsForRank(EGuildRank Rank) const
{
    TArray<EGuildPermission> Permissions;
    
    switch (Rank)
    {
    case EGuildRank::Leader:
        Permissions.Add(EGuildPermission::InviteMembers);
        Permissions.Add(EGuildPermission::KickMembers);
        Permissions.Add(EGuildPermission::PromoteMembers);
        Permissions.Add(EGuildPermission::DemoteMembers);
        Permissions.Add(EGuildPermission::ManageResources);
        Permissions.Add(EGuildPermission::DeclareWar);
        Permissions.Add(EGuildPermission::ManageDiplomacy);
        Permissions.Add(EGuildPermission::EditGuildInfo);
        Permissions.Add(EGuildPermission::ManageBuildings);
        Permissions.Add(EGuildPermission::AccessBank);
        break;
        
    case EGuildRank::Officer:
        Permissions.Add(EGuildPermission::InviteMembers);
        Permissions.Add(EGuildPermission::KickMembers);
        Permissions.Add(EGuildPermission::ManageResources);
        Permissions.Add(EGuildPermission::AccessBank);
        break;
        
    case EGuildRank::Veteran:
        Permissions.Add(EGuildPermission::InviteMembers);
        Permissions.Add(EGuildPermission::AccessBank);
        break;
        
    case EGuildRank::Member:
        Permissions.Add(EGuildPermission::AccessBank);
        break;
        
    default:
        break;
    }
    
    return Permissions;
}

bool UMingRTSGuildSystem::IsRankHigher(EGuildRank A, EGuildRank B) const
{
    int32 ValueA = static_cast<int32>(A);
    int32 ValueB = static_cast<int32>(B);
    
    // 注意：列舉值越小等級越高
    return ValueA < ValueB;
}

void UMingRTSGuildSystem::LogGuildAction(const FString& GuildID, const FString& PlayerID, const FString& Action)
{
    UE_LOG(LogTemp, Log, TEXT("[Guild %s][Player %s] %s"), *GuildID, *PlayerID, *Action);
}
