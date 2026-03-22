#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSGuildSystem.generated.h"

// 公會等級
UENUM(BlueprintType)
enum class EGuildRank : uint8
{
    Leader              UMETA(DisplayName = "會長"),
    Officer             UMETA(DisplayName = "官員"),
    Veteran             UMETA(DisplayName = "元老"),
    Member              UMETA(DisplayName = "成員"),
    Recruit             UMETA(DisplayName = "新兵"),
    Guest               UMETA(DisplayName = "訪客")
};

// 聯盟類型
UENUM(BlueprintType)
enum class EAllianceType : uint8
{
    Military            UMETA(DisplayName = "軍事聯盟"),
    Economic            UMETA(DisplayName = "經濟聯盟"),
    Research            UMETA(DisplayName = "研究聯盟"),
    Defense             UMETA(DisplayName = "防禦聯盟"),
    Trade               UMETA(DisplayName = "貿易聯盟"),
    Cultural            UMETA(DisplayName = "文化聯盟")
};

// 公會權限
UENUM(BlueprintType)
enum class EGuildPermission : uint8
{
    InviteMembers       UMETA(DisplayName = "邀請成員"),
    KickMembers         UMETA(DisplayName = "踢出成員"),
    PromoteMembers      UMETA(DisplayName = "晉升成員"),
    DemoteMembers       UMETA(DisplayName = "降級成員"),
    ManageResources     UMETA(DisplayName = "管理資源"),
    DeclareWar          UMETA(DisplayName = "宣戰"),
    ManageDiplomacy     UMETA(DisplayName = "外交管理"),
    EditGuildInfo       UMETA(DisplayName = "編輯公會信息"),
    ManageBuildings     UMETA(DisplayName = "管理建築"),
    AccessBank          UMETA(DisplayName = "使用公會倉庫")
};

// 公會成員數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGuildMember
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;
    
    UPROPERTY(BlueprintReadOnly)
    FString PlayerName;
    
    UPROPERTY(BlueprintReadOnly)
    EGuildRank Rank;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ContributionPoints;
    
    UPROPERTY(BlueprintReadOnly)
    int32 WeeklyContribution;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime JoinDate;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime LastActive;
    
    UPROPERTY(BlueprintReadOnly)
    bool bOnline;
    
    UPROPERTY(BlueprintReadOnly)
    int32 GuildLevel;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<EGuildPermission> Permissions;
    
    UPROPERTY(BlueprintReadOnly)
    FString Title;
    
    UPROPERTY(BlueprintReadOnly)
    FString Notes;
    
    FGuildMember()
        : Rank(EGuildRank::Recruit)
        , ContributionPoints(0)
        , WeeklyContribution(0)
        , bOnline(false)
        , GuildLevel(1)
    {}
};

// 公會資源
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGuildResources
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 Gold;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Wood;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Stone;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Iron;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Food;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Reputation;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Influence;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> SpecialResources;
    
    FGuildResources()
        : Gold(0)
        , Wood(0)
        , Stone(0)
        , Iron(0)
        , Food(0)
        , Reputation(100)
        , Influence(0)
    {}
};

// 公會建築
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGuildBuilding
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString BuildingID;
    
    UPROPERTY(BlueprintReadOnly)
    FString BuildingName;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Level;
    
    UPROPERTY(BlueprintReadOnly)
    int32 MaxLevel;
    
    UPROPERTY(BlueprintReadOnly)
    FString Description;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> UpgradeRequirements;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Bonuses;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;
    
    FGuildBuilding()
        : Level(1)
        , MaxLevel(10)
        , bIsActive(true)
    {}
};

// 公會科技
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGuildTechnology
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString TechID;
    
    UPROPERTY(BlueprintReadOnly)
    FString TechName;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Level;
    
    UPROPERTY(BlueprintReadOnly)
    int32 MaxLevel;
    
    UPROPERTY(BlueprintReadOnly)
    FString Description;
    
    UPROPERTY(BlueprintReadOnly)
    FString Effect;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResearchRequirements;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsResearched;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Prerequisites;
    
    FGuildTechnology()
        : Level(0)
        , MaxLevel(5)
        , bIsResearched(false)
    {}
};

// 公會事件
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGuildEvent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString EventID;
    
    UPROPERTY(BlueprintReadOnly)
    FString EventType;
    
    UPROPERTY(BlueprintReadOnly)
    FString Title;
    
    UPROPERTY(BlueprintReadOnly)
    FString Description;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Participants;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> Rewards;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;
    
    FGuildEvent()
        : bIsActive(false)
    {}
};

// 公會聯盟
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGuildAlliance
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString AllianceID;
    
    UPROPERTY(BlueprintReadOnly)
    FString AllianceName;
    
    UPROPERTY(BlueprintReadOnly)
    EAllianceType Type;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> MemberGuilds;
    
    UPROPERTY(BlueprintReadOnly)
    FString LeaderGuild;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime FormationDate;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SharedBenefits;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> SharedResources;
    
    UPROPERTY(BlueprintReadOnly)
    int32 AlliancePower;
    
    FGuildAlliance()
        : Type(EAllianceType::Military)
        , AlliancePower(0)
    {}
};

// 公會基本信息
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGuildInfo
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString GuildID;
    
    UPROPERTY(BlueprintReadOnly)
    FString GuildName;
    
    UPROPERTY(BlueprintReadOnly)
    FString Tag;
    
    UPROPERTY(BlueprintReadOnly)
    FString Description;
    
    UPROPERTY(BlueprintReadOnly)
    FString Emblem;
    
    UPROPERTY(BlueprintReadOnly)
    int32 GuildLevel;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Experience;
    
    UPROPERTY(BlueprintReadOnly)
    int32 MaxMembers;
    
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentMembers;
    
    UPROPERTY(BlueprintReadOnly)
    FString LeaderID;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime CreationDate;
    
    UPROPERTY(BlueprintReadOnly)
    FString Region;
    
    UPROPERTY(BlueprintReadOnly)
    FString Language;
    
    UPROPERTY(BlueprintReadOnly)
    bool bRecruiting;
    
    UPROPERTY(BlueprintReadOnly)
    int32 MinimumLevelRequirement;
    
    FGuildInfo()
        : GuildLevel(1)
        , Experience(0)
        , MaxMembers(50)
        , CurrentMembers(0)
        , bRecruiting(true)
        , MinimumLevelRequirement(1)
    {}
};

// 公會外交關係
UENUM(BlueprintType)
enum class EGuildDiplomacyStatus : uint8
{
    Neutral             UMETA(DisplayName = "中立"),
    Allied              UMETA(DisplayName = "同盟"),
    Friendly            UMETA(DisplayName = "友好"),
    Hostile             UMETA(DisplayName = "敵對"),
    AtWar               UMETA(DisplayName = "戰爭"),
    TradeAgreement      UMETA(DisplayName = "貿易協定"),
    NonAggression       UMETA(DisplayName = "互不侵犯")
};

// 外交關係
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FGuildDiplomacy
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString TargetGuildID;
    
    UPROPERTY(BlueprintReadOnly)
    FString TargetGuildName;
    
    UPROPERTY(BlueprintReadOnly)
    EGuildDiplomacyStatus Status;
    
    UPROPERTY(BlueprintReadOnly)
    int32 RelationshipValue;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime EstablishedDate;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime ExpirationDate;
    
    UPROPERTY(BlueprintReadOnly)
    FString Terms;
    
    FGuildDiplomacy()
        : Status(EGuildDiplomacyStatus::Neutral)
        , RelationshipValue(0)
    {}
};

// 委托聲明
declare_dynamic_multicast_delegate_three_params(FOnGuildMemberJoined, FString, GuildID, FString, PlayerID, FString, PlayerName);
declare_dynamic_multicast_delegate_two_params(FOnGuildMemberLeft, FString, GuildID, FString, PlayerID);
declare_dynamic_multicast_delegate_three_params(FOnGuildRankChanged, FString, PlayerID, EGuildRank, OldRank, EGuildRank, NewRank);
declare_dynamic_multicast_delegate_two_params(FOnAllianceFormed, FString, AllianceID, TArray<FString>, MemberGuilds);
declare_dynamic_multicast_delegate_two_params(FOnGuildWarDeclared, FString, AttackerGuild, FString, DefenderGuild);

UCLASS(ClassGroup = (MingPersonal), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingRTSGuildSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSGuildSystem();
    
    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "Guild|System")
    void InitializeGuildSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Guild|System")
    void ShutdownGuildSystem();
    
    // 公會管理
    UFUNCTION(BlueprintCallable, Category = "Guild|Management")
    FString CreateGuild(const FString& GuildName, const FString& Tag, const FString& LeaderID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Management")
    void DisbandGuild(const FString& GuildID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Management")
    void SetGuildInfo(const FString& GuildID, const FGuildInfo& NewInfo);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Management")
    FGuildInfo GetGuildInfo(const FString& GuildID) const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Management")
    TArray<FGuildInfo> GetAllGuilds() const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Management")
    bool DoesGuildExist(const FString& GuildID) const;
    
    // 成員管理
    UFUNCTION(BlueprintCallable, Category = "Guild|Members")
    bool InviteMember(const FString& GuildID, const FString& PlayerID, const FString& InviterID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Members")
    bool AcceptInvite(const FString& GuildID, const FString& PlayerID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Members")
    void RemoveMember(const FString& GuildID, const FString& PlayerID, const FString& RemoverID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Members")
    void PromoteMember(const FString& GuildID, const FString& PlayerID, EGuildRank NewRank, const FString& PromoterID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Members")
    void DemoteMember(const FString& GuildID, const FString& PlayerID, EGuildRank NewRank, const FString& DemoterID);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Members")
    TArray<FGuildMember> GetGuildMembers(const FString& GuildID) const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Members")
    FGuildMember GetMemberInfo(const FString& GuildID, const FString& PlayerID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Members")
    void UpdateMemberContribution(const FString& GuildID, const FString& PlayerID, int32 Amount);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Members")
    bool HasPermission(const FString& GuildID, const FString& PlayerID, EGuildPermission Permission) const;
    
    // 資源管理
    UFUNCTION(BlueprintCallable, Category = "Guild|Resources")
    void AddGuildResources(const FString& GuildID, const FGuildResources& Resources);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Resources")
    bool RemoveGuildResources(const FString& GuildID, const FGuildResources& Resources);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Resources")
    FGuildResources GetGuildResources(const FString& GuildID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Resources")
    bool DepositResources(const FString& GuildID, const FString& PlayerID, const FGuildResources& Resources);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Resources")
    bool WithdrawResources(const FString& GuildID, const FString& PlayerID, const FGuildResources& Resources);
    
    // 建築管理
    UFUNCTION(BlueprintCallable, Category = "Guild|Buildings")
    void AddGuildBuilding(const FString& GuildID, const FGuildBuilding& Building);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Buildings")
    bool UpgradeBuilding(const FString& GuildID, const FString& BuildingID);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Buildings")
    TArray<FGuildBuilding> GetGuildBuildings(const FString& GuildID) const;
    
    // 科技管理
    UFUNCTION(BlueprintCallable, Category = "Guild|Technology")
    bool ResearchTechnology(const FString& GuildID, const FString& TechID);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Technology")
    TArray<FGuildTechnology> GetGuildTechnologies(const FString& GuildID) const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Technology")
    TArray<FString> GetActiveTechBonuses(const FString& GuildID) const;
    
    // 聯盟管理
    UFUNCTION(BlueprintCallable, Category = "Guild|Alliance")
    FString CreateAlliance(const FString& AllianceName, EAllianceType Type, const TArray<FString>& MemberGuilds);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Alliance")
    void DisbandAlliance(const FString& AllianceID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Alliance")
    bool InviteToAlliance(const FString& AllianceID, const FString& GuildID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Alliance")
    void RemoveFromAlliance(const FString& AllianceID, const FString& GuildID);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Alliance")
    TArray<FGuildAlliance> GetAlliances() const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Alliance")
    FGuildAlliance GetAllianceInfo(const FString& AllianceID) const;
    
    // 外交管理
    UFUNCTION(BlueprintCallable, Category = "Guild|Diplomacy")
    void SetDiplomaticStatus(const FString& GuildID, const FString& TargetGuildID, EGuildDiplomacyStatus Status);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Diplomacy")
    EGuildDiplomacyStatus GetDiplomaticStatus(const FString& GuildID, const FString& TargetGuildID) const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Diplomacy")
    TArray<FGuildDiplomacy> GetDiplomaticRelations(const FString& GuildID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Diplomacy")
    void DeclareWar(const FString& AttackerGuildID, const FString& DefenderGuildID);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Diplomacy")
    void OfferPeace(const FString& GuildID, const FString& TargetGuildID);
    
    // 事件管理
    UFUNCTION(BlueprintCallable, Category = "Guild|Events")
    void CreateGuildEvent(const FString& GuildID, const FGuildEvent& Event);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Events")
    void CancelGuildEvent(const FString& GuildID, const FString& EventID);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Events")
    TArray<FGuildEvent> GetActiveEvents(const FString& GuildID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Events")
    void JoinEvent(const FString& GuildID, const FString& EventID, const FString& PlayerID);
    
    // 聊天與公告
    UFUNCTION(BlueprintCallable, Category = "Guild|Communication")
    void SendGuildMessage(const FString& GuildID, const FString& PlayerID, const FString& Message);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Communication")
    void PostGuildAnnouncement(const FString& GuildID, const FString& PlayerID, const FString& Title, const FString& Content);
    
    // 公會升級
    UFUNCTION(BlueprintCallable, Category = "Guild|Progression")
    void AddGuildExperience(const FString& GuildID, int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Guild|Progression")
    bool LevelUpGuild(const FString& GuildID);
    
    UFUNCTION(BlueprintPure, Category = "Guild|Progression")
    int32 GetRequiredExperienceForNextLevel(int32 CurrentLevel) const;
    
    // 搜索與過濾
    UFUNCTION(BlueprintPure, Category = "Guild|Search")
    TArray<FGuildInfo> SearchGuilds(const FString& SearchTerm) const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Search")
    TArray<FGuildInfo> GetRecruitingGuilds() const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Search")
    TArray<FGuildInfo> GetGuildsByRegion(const FString& Region) const;
    
    // 統計數據
    UFUNCTION(BlueprintPure, Category = "Guild|Stats")
    int32 GetTotalGuilds() const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Stats")
    int32 GetTotalMembers(const FString& GuildID) const;
    
    UFUNCTION(BlueprintPure, Category = "Guild|Stats")
    int32 GetOnlineMembers(const FString& GuildID) const;
    
    // 委託
    UPROPERTY(BlueprintAssignable, Category = "Guild|Events")
    FOnGuildMemberJoined OnGuildMemberJoined;
    
    UPROPERTY(BlueprintAssignable, Category = "Guild|Events")
    FOnGuildMemberLeft OnGuildMemberLeft;
    
    UPROPERTY(BlueprintAssignable, Category = "Guild|Events")
    FOnGuildRankChanged OnGuildRankChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Guild|Events")
    FOnAllianceFormed OnAllianceFormed;
    
    UPROPERTY(BlueprintAssignable, Category = "Guild|Events")
    FOnGuildWarDeclared OnGuildWarDeclared;
    
protected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    TMap<FString, FGuildInfo> Guilds;
    
    UPROPERTY()
    TMap<FString, TArray<FGuildMember>> GuildMembers;
    
    UPROPERTY()
    TMap<FString, FGuildResources> GuildResources;
    
    UPROPERTY()
    TMap<FString, TArray<FGuildBuilding>> GuildBuildings;
    
    UPROPERTY()
    TMap<FString, TArray<FGuildTechnology>> GuildTechnologies;
    
    UPROPERTY()
    TMap<FString, TArray<FGuildEvent>> GuildEvents;
    
    UPROPERTY()
    TMap<FString, TArray<FGuildDiplomacy>> GuildDiplomacy;
    
    UPROPERTY()
    TArray<FGuildAlliance> Alliances;
    
    UPROPERTY()
    TMap<FString, FDateTime> PendingInvites;
    
    // 輔助函數
    bool CanPerformAction(const FString& GuildID, const FString& PlayerID, EGuildPermission RequiredPermission) const;
    void BroadcastToGuildMembers(const FString& GuildID, const FString& Message);
    void UpdateGuildPower(const FString& GuildID);
    void CheckAllianceBenefits(const FString& AllianceID);
    TArray<EGuildPermission> GetPermissionsForRank(EGuildRank Rank) const;
    bool IsRankHigher(EGuildRank A, EGuildRank B) const;
    void LogGuildAction(const FString& GuildID, const FString& PlayerID, const FString& Action);
};
