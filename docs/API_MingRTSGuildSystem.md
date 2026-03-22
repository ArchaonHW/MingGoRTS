# MingRTSGuildSystem 系统文档

## 概述

MingRTSGuildSystem 是一个完整的公会/联盟系统，支持公会创建、成员管理、资源管理、建筑升级、科技研究、联盟形成和外交关系管理。

## 核心功能

### 1. 公会等级
支持6种公会等级：
- **Leader (会长)**: 公会最高领导者，拥有所有权限
- **Officer (官员)**: 高级成员，可管理日常事务
- **Veteran (元老)**: 资深成员，有一定管理权限
- **Member (成员)**: 普通成员
- **Recruit (新兵)**: 新加入成员，权限受限
- **Guest (访客)**: 临时访问者，权限最小

### 2. 公会权限
- **InviteMembers**: 邀请新成员
- **KickMembers**: 踢出成员
- **PromoteMembers**: 晋升成员
- **DemoteMembers**: 降级成员
- **ManageResources**: 管理公会资源
- **DeclareWar**: 宣战
- **ManageDiplomacy**: 管理外交关系
- **EditGuildInfo**: 编辑公会信息
- **ManageBuildings**: 管理公会建筑
- **AccessBank**: 使用公会仓库

### 3. 联盟类型
- **Military**: 军事联盟 - 共同防御和进攻
- **Economic**: 经济联盟 - 贸易协定和资源共享
- **Research**: 研究联盟 - 科技合作
- **Defense**: 防御联盟 - 互助防御
- **Trade**: 贸易联盟 - 商业合作
- **Cultural**: 文化联盟 - 文化交流

### 4. 外交关系
- **Neutral**: 中立
- **Allied**: 同盟
- **Friendly**: 友好
- **Hostile**: 敌对
- **AtWar**: 战争状态
- **TradeAgreement**: 贸易协定
- **NonAggression**: 互不侵犯

## API 参考

### 系统初始化

```cpp
// 初始化公会系统
void InitializeGuildSystem();

// 关闭公会系统
void ShutdownGuildSystem();
```

### 公会管理

```cpp
// 创建公会
FString CreateGuild(const FString& GuildName, const FString& Tag, const FString& LeaderID);

// 解散公会
void DisbandGuild(const FString& GuildID);

// 设置公会信息
void SetGuildInfo(const FString& GuildID, const FGuildInfo& NewInfo);

// 获取公会信息
FGuildInfo GetGuildInfo(const FString& GuildID) const;

// 获取所有公会
TArray<FGuildInfo> GetAllGuilds() const;

// 检查公会是否存在
bool DoesGuildExist(const FString& GuildID) const;
```

### 成员管理

```cpp
// 邀请成员
bool InviteMember(const FString& GuildID, const FString& PlayerID, const FString& InviterID);

// 接受邀请
bool AcceptInvite(const FString& GuildID, const FString& PlayerID);

// 移除成员
void RemoveMember(const FString& GuildID, const FString& PlayerID, const FString& RemoverID);

// 晋升成员
void PromoteMember(const FString& GuildID, const FString& PlayerID, 
    EGuildRank NewRank, const FString& PromoterID);

// 降级成员
void DemoteMember(const FString& GuildID, const FString& PlayerID, 
    EGuildRank NewRank, const FString& DemoterID);

// 获取公会成员列表
TArray<FGuildMember> GetGuildMembers(const FString& GuildID) const;

// 获取成员信息
FGuildMember GetMemberInfo(const FString& GuildID, const FString& PlayerID) const;

// 更新成员贡献
void UpdateMemberContribution(const FString& GuildID, const FString& PlayerID, int32 Amount);

// 检查权限
bool HasPermission(const FString& GuildID, const FString& PlayerID, 
    EGuildPermission Permission) const;
```

### 资源管理

```cpp
// 添加公会资源
void AddGuildResources(const FString& GuildID, const FGuildResources& Resources);

// 移除公会资源
bool RemoveGuildResources(const FString& GuildID, const FGuildResources& Resources);

// 获取公会资源
FGuildResources GetGuildResources(const FString& GuildID) const;

// 存入资源
bool DepositResources(const FString& GuildID, const FString& PlayerID, 
    const FGuildResources& Resources);

// 取出资源
bool WithdrawResources(const FString& GuildID, const FString& PlayerID, 
    const FGuildResources& Resources);
```

### 建筑管理

```cpp
// 添加公会建筑
void AddGuildBuilding(const FString& GuildID, const FGuildBuilding& Building);

// 升级建筑
bool UpgradeBuilding(const FString& GuildID, const FString& BuildingID);

// 获取公会建筑列表
TArray<FGuildBuilding> GetGuildBuildings(const FString& GuildID) const;
```

### 科技管理

```cpp
// 研究科技
bool ResearchTechnology(const FString& GuildID, const FString& TechID);

// 获取公会科技列表
TArray<FGuildTechnology> GetGuildTechnologies(const FString& GuildID) const;

// 获取激活的科技加成
TArray<FString> GetActiveTechBonuses(const FString& GuildID) const;
```

### 联盟管理

```cpp
// 创建联盟
FString CreateAlliance(const FString& AllianceName, EAllianceType Type, 
    const TArray<FString>& MemberGuilds);

// 解散联盟
void DisbandAlliance(const FString& AllianceID);

// 邀请加入联盟
bool InviteToAlliance(const FString& AllianceID, const FString& GuildID);

// 从联盟中移除
void RemoveFromAlliance(const FString& AllianceID, const FString& GuildID);

// 获取所有联盟
TArray<FGuildAlliance> GetAlliances() const;

// 获取联盟信息
FGuildAlliance GetAllianceInfo(const FString& AllianceID) const;
```

### 外交管理

```cpp
// 设置外交关系
void SetDiplomaticStatus(const FString& GuildID, const FString& TargetGuildID, 
    EGuildDiplomacyStatus Status);

// 获取外交关系
EGuildDiplomacyStatus GetDiplomaticStatus(const FString& GuildID, 
    const FString& TargetGuildID) const;

// 获取外交关系列表
TArray<FGuildDiplomacy> GetDiplomaticRelations(const FString& GuildID) const;

// 宣战
void DeclareWar(const FString& AttackerGuildID, const FString& DefenderGuildID);

// 提议和平
void OfferPeace(const FString& GuildID, const FString& TargetGuildID);
```

### 事件管理

```cpp
// 创建公会事件
void CreateGuildEvent(const FString& GuildID, const FGuildEvent& Event);

// 取消公会事件
void CancelGuildEvent(const FString& GuildID, const FString& EventID);

// 获取活跃事件
TArray<FGuildEvent> GetActiveEvents(const FString& GuildID) const;

// 参加事件
void JoinEvent(const FString& GuildID, const FString& EventID, const FString& PlayerID);
```

### 聊天与公告

```cpp
// 发送公会消息
void SendGuildMessage(const FString& GuildID, const FString& PlayerID, const FString& Message);

// 发布公告
void PostGuildAnnouncement(const FString& GuildID, const FString& PlayerID, 
    const FString& Title, const FString& Content);
```

### 公会升级

```cpp
// 添加公会经验
void AddGuildExperience(const FString& GuildID, int32 Amount);

// 公会升级
bool LevelUpGuild(const FString& GuildID);

// 获取下一级所需经验
int32 GetRequiredExperienceForNextLevel(int32 CurrentLevel) const;
```

### 搜索与过滤

```cpp
// 搜索公会
TArray<FGuildInfo> SearchGuilds(const FString& SearchTerm) const;

// 获取招募中的公会
TArray<FGuildInfo> GetRecruitingGuilds() const;

// 按地区获取公会
TArray<FGuildInfo> GetGuildsByRegion(const FString& Region) const;
```

### 统计信息

```cpp
// 获取公会总数
int32 GetTotalGuilds() const;

// 获取公会总成员数
int32 GetTotalMembers(const FString& GuildID) const;

// 获取在线成员数
int32 GetOnlineMembers(const FString& GuildID) const;
```

## 数据结构

### FGuildInfo
```cpp
struct FGuildInfo
{
    FString GuildID;                    // 公会ID
    FString GuildName;                  // 公会名称
    FString Tag;                        // 公会标签
    FString Description;                // 公会描述
    FString Emblem;                     // 公会徽章
    int32 GuildLevel;                   // 公会等级
    int32 Experience;                   // 当前经验
    int32 MaxMembers;                   // 最大成员数
    int32 CurrentMembers;               // 当前成员数
    FString LeaderID;                   // 会长ID
    FDateTime CreationDate;             // 创建日期
    FString Region;                     // 地区
    FString Language;                   // 语言
    bool bRecruiting;                   // 是否招募中
    int32 MinimumLevelRequirement;        // 最低等级要求
};
```

### FGuildMember
```cpp
struct FGuildMember
{
    FString PlayerID;                   // 玩家ID
    FString PlayerName;                 // 玩家名称
    EGuildRank Rank;                    // 公会等级
    int32 ContributionPoints;           // 贡献点数
    int32 WeeklyContribution;           // 本周贡献
    FDateTime JoinDate;                 // 加入日期
    FDateTime LastActive;               // 最后活跃时间
    bool bOnline;                       // 是否在线
    int32 GuildLevel;                   // 公会内等级
    TArray<EGuildPermission> Permissions; // 权限列表
    FString Title;                      // 称号
    FString Notes;                      // 备注
};
```

### FGuildResources
```cpp
struct FGuildResources
{
    int32 Gold;                         // 金币
    int32 Wood;                         // 木材
    int32 Stone;                        // 石材
    int32 Iron;                         // 铁矿
    int32 Food;                         // 食物
    int32 Reputation;                   // 声望
    int32 Influence;                    // 影响力
    TMap<FString, int32> SpecialResources; // 特殊资源
};
```

### FGuildBuilding
```cpp
struct FGuildBuilding
{
    FString BuildingID;                 // 建筑ID
    FString BuildingName;               // 建筑名称
    int32 Level;                        // 当前等级
    int32 MaxLevel;                     // 最大等级
    FString Description;                // 描述
    TMap<FString, int32> UpgradeRequirements; // 升级需求
    TArray<FString> Bonuses;            // 加成效果
    bool bIsActive;                     // 是否激活
};
```

### FGuildTechnology
```cpp
struct FGuildTechnology
{
    FString TechID;                     // 科技ID
    FString TechName;                   // 科技名称
    int32 Level;                        // 当前等级
    int32 MaxLevel;                     // 最大等级
    FString Description;                // 描述
    FString Effect;                     // 效果描述
    TMap<FString, int32> ResearchRequirements; // 研究需求
    bool bIsResearched;                 // 是否已研究
    TArray<FString> Prerequisites;    // 前置科技
};
```

### FGuildEvent
```cpp
struct FGuildEvent
{
    FString EventID;                    // 事件ID
    FString EventType;                  // 事件类型
    FString Title;                      // 标题
    FString Description;                // 描述
    FDateTime StartTime;                // 开始时间
    FDateTime EndTime;                  // 结束时间
    TArray<FString> Participants;       // 参与者
    TMap<FString, int32> Rewards;       // 奖励
    bool bIsActive;                     // 是否活跃
};
```

### FGuildAlliance
```cpp
struct FGuildAlliance
{
    FString AllianceID;                 // 联盟ID
    FString AllianceName;               // 联盟名称
    EAllianceType Type;                 // 联盟类型
    TArray<FString> MemberGuilds;       // 成员公会
    FString LeaderGuild;                // 领导公会
    FDateTime FormationDate;            // 成立日期
    TArray<FString> SharedBenefits;     // 共享福利
    TMap<FString, int32> SharedResources; // 共享资源
    int32 AlliancePower;                // 联盟实力
};
```

### FGuildDiplomacy
```cpp
struct FGuildDiplomacy
{
    FString TargetGuildID;              // 目标公会ID
    FString TargetGuildName;            // 目标公会名称
    EGuildDiplomacyStatus Status;       // 外交状态
    int32 RelationshipValue;            // 关系值
    FDateTime EstablishedDate;          // 建立日期
    FDateTime ExpirationDate;           // 过期日期
    FString Terms;                      // 条款
};
```

## 权限等级

### 会长权限
- InviteMembers, KickMembers, PromoteMembers, DemoteMembers
- ManageResources, DeclareWar, ManageDiplomacy
- EditGuildInfo, ManageBuildings, AccessBank

### 官员权限
- InviteMembers, KickMembers
- ManageResources, AccessBank

### 元老权限
- InviteMembers, AccessBank

### 成员权限
- AccessBank

## 事件委托

```cpp
// 成员加入公会
FOnGuildMemberJoined OnGuildMemberJoined;

// 成员离开公会
FOnGuildMemberLeft OnGuildMemberLeft;

// 成员等级变更
FOnGuildRankChanged OnGuildRankChanged;

// 联盟成立
FOnAllianceFormed OnAllianceFormed;

// 公会宣战
FOnGuildWarDeclared OnGuildWarDeclared;
```

## 使用示例

### 初始化系统
```cpp
UMingRTSGuildSystem* GuildSystem = NewObject<UMingRTSGuildSystem>();
GuildSystem->InitializeGuildSystem();

// 绑定事件
GuildSystem->OnGuildMemberJoined.AddDynamic(this, &AMyGameMode::OnMemberJoined);
GuildSystem->OnGuildWarDeclared.AddDynamic(this, &AMyGameMode::OnWarDeclared);
```

### 创建公会
```cpp
FString GuildID = GuildSystem->CreateGuild(
    TEXT("Dragon Slayers"),
    TEXT("DS"),
    TEXT("Player123")  // 会长ID
);

// 设置公会信息
FGuildInfo Info = GuildSystem->GetGuildInfo(GuildID);
Info.Description = TEXT("We are the elite warriors of the realm.");
Info.Region = TEXT("North America");
Info.bRecruiting = true;
GuildSystem->SetGuildInfo(GuildID, Info);
```

### 邀请和接受成员
```cpp
// 会长邀请新成员
bool bInvited = GuildSystem->InviteMember(GuildID, TEXT("Player456"), TEXT("Player123"));

// 玩家接受邀请
bool bAccepted = GuildSystem->AcceptInvite(GuildID, TEXT("Player456"));
```

### 成员管理
```cpp
// 晋升成员为官员
GuildSystem->PromoteMember(GuildID, TEXT("Player456"), EGuildRank::Officer, TEXT("Player123"));

// 查看公会成员
TArray<FGuildMember> Members = GuildSystem->GetGuildMembers(GuildID);
for (const FGuildMember& Member : Members)
{
    UE_LOG(LogTemp, Log, TEXT("%s - Rank: %d"), 
        *Member.PlayerName, static_cast<int32>(Member.Rank));
}
```

### 资源管理
```cpp
// 存入资源
FGuildResources Deposit;
Deposit.Gold = 1000;
Deposit.Wood = 500;
GuildSystem->DepositResources(GuildID, TEXT("Player456"), Deposit);

// 查看公会资源
FGuildResources Resources = GuildSystem->GetGuildResources(GuildID);
UE_LOG(LogTemp, Log, TEXT("Guild Gold: %d"), Resources.Gold);
```

### 创建联盟
```cpp
// 创建军事联盟
TArray<FString> MemberGuilds = {GuildID, TEXT("Guild2"), TEXT("Guild3")};
FString AllianceID = GuildSystem->CreateAlliance(
    TEXT("Northern Defense Pact"),
    EAllianceType::Military,
    MemberGuilds
);
```

### 外交关系
```cpp
// 建立同盟
GuildSystem->SetDiplomaticStatus(GuildID, TEXT("FriendlyGuild"), 
    EGuildDiplomacyStatus::Allied);

// 宣战
GuildSystem->DeclareWar(GuildID, TEXT("EnemyGuild"));

// 检查外交关系
EGuildDiplomacyStatus Status = GuildSystem->GetDiplomaticStatus(
    GuildID, TEXT("EnemyGuild"));
// Status == EGuildDiplomacyStatus::AtWar
```

### 公会升级
```cpp
// 添加经验
GuildSystem->AddGuildExperience(GuildID, 5000);

// 检查是否可以升级
int32 Required = GuildSystem->GetRequiredExperienceForNextLevel(
    GuildSystem->GetGuildInfo(GuildID).GuildLevel);

// 尝试升级
bool bLeveledUp = GuildSystem->LevelUpGuild(GuildID);
```

## 最佳实践

1. **权限管理**: 谨慎分配高级权限，定期审查官员列表
2. **资源透明**: 保持公会资源流动透明，建立审计机制
3. **外交谨慎**: 宣战前应充分考虑后果，维护良好的外交关系
4. **成员参与**: 通过事件和活动提高成员参与度
5. **建筑规划**: 根据公会发展方向优先升级相关建筑
6. **科技研究**: 根据公会战略优先研究相关科技

## 性能考虑

- **成员数量**: 单个公会建议不超过500名成员
- **建筑数量**: 单个公会建议不超过100栋建筑
- **科技数量**: 单个公会建议不超过200项科技
- **联盟规模**: 单个联盟建议不超过20个公会
- **查询优化**: 频繁查询的数据应进行缓存

## 扩展性

系统支持以下扩展：
- 自定义公会等级
- 新的权限类型
- 特殊建筑类型
- 自定义科技树
- 新的联盟类型
- 复杂的外交条约系统
