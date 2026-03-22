# MingRTSCollaborativeAINetwork 系統文檔 / MingRTSCollaborativeAINetwork System Documentation

## 概述 / Overview

MingRTSCollaborativeAINetwork 是一個多AI智能體協作系統，允許多個AI智能體協同工作、共享知識和分配任務。

MingRTSCollaborativeAINetwork is a multi-agent AI collaboration system that allows multiple AI agents to work together, share knowledge, and distribute tasks.

## 核心功能 / Core Features

### 1. 智能體管理 / Agent Management
- **智能體註冊/註銷**: 動態添加和移除AI智能體
- **角色分配**: 支持11種智能體角色

- **Agent Registration/Unregister**: Dynamically add and remove AI agents
- **Role Assignment**: Support for 11 agent roles

角色類型 / Role Types:
- **Commander (指揮官)**: 指揮官 / Commander - 統籌全局 / Overall command
- **Scout (偵察兵)**: 偵察兵 / Scout - 情報收集 / Intelligence gathering
- **Attacker (攻擊者)**: 攻擊者 / Attacker - 進攻作戰 / Offensive operations
- **Defender (防禦者)**: 防禦者 / Defender - 防守據點 / Defensive positions
- **Support (支援)**: 支援 / Support - 後勤支援 / Logistics support
- **ResourceGatherer (資源收集者)**: 資源收集者 / Resource Gatherer - 資源採集 / Resource gathering
- **Builder (建造者)**: 建造者 / Builder - 建築建造 / Construction
- **Healer (治療者)**: 治療者 / Healer - 單位治療 / Unit healing
- **Specialist (專家)**: 專家 / Specialist - 特殊任務 / Special operations
- **Coordinator (協調者)**: 協調者 / Coordinator - 協調溝通 / Coordination
- **Learner (學習者)**: 學習者 / Learner - 學習適應 / Learning adaptation

### 2. 通信系統 / Communication System
- **消息傳遞**: 智能體間的私有消息 / Private messages between agents
- **廣播**: 向所有智能體廣播消息 / Broadcast messages to all agents
- **緊急通信**: 高優先級的緊急消息機制 / High-priority emergency messaging
- **確認機制**: 消息接收確認 / Message reception confirmation

### 3. 任務協作 / Task Collaboration
- **任務創建**: 創建需要協作完成的任務 / Create tasks requiring collaboration
- **自動分配**: 基於智能體技能和可用性自動分配任務 / Assign tasks based on agent skills
- **進度追踪**: 實時跟踪任務完成進度 / Real-time task completion tracking
- **獎勵分配**: 根據貢獻度分配獎勵 / Allocate rewards based on contribution

### 4. 協作策略 / Collaboration Strategies
- **Hierarchical**: 層級式指揮 / Hierarchical command
- **Democratic**: 民主式決策 / Democratic decision-making
- **Competitive**: 競爭性協作 / Competitive collaboration
- **Cooperative**: 合作式協作 / Cooperative collaboration
- **Swarm**: 蜂群式行為 / Swarm behavior
- **MarketBased**: 基於市場機制的協作 / Market-based collaboration

## API 參考 / API Reference

### 智能体管理

```cpp
// 注册智能体
FString RegisterAgent(const FString& AgentName, EAIAgentRole PrimaryRole);

// 注销智能体
void UnregisterAgent(const FString& AgentID);

// 更新智能体状态
void UpdateAgentStatus(const FString& AgentID, EAIAgentStatus NewStatus);

// 设置智能体角色
void SetAgentRole(const FString& AgentID, EAIAgentRole NewRole);

// 获取智能体数据
FAIAgentData GetAgentData(const FString& AgentID) const;

// 获取所有智能体ID
TArray<FString> GetAllAgentIDs() const;

// 按角色获取智能体
TArray<FString> GetAgentsByRole(EAIAgentRole Role) const;
```

### 通信

```cpp
// 发送消息
void SendMessage(const FString& SenderID, const TArray<FString>& RecipientIDs,
    EAICommunicationType Type, const FString& Content, float Priority);

// 广播消息
void BroadcastMessage(const FString& SenderID, EAICommunicationType Type,
    const FString& Content, float Priority);

// 发送紧急消息
void SendEmergencyMessage(const FString& SenderID, const FString& Content);

// 获取智能体消息
TArray<FAIAgentMessage> GetAgentMessages(const FString& AgentID) const;

// 确认消息
void AcknowledgeMessage(const FString& AgentID, const FString& MessageID);
```

### 任务管理

```cpp
// 创建协作任务
FString CreateCollaborativeTask(const FString& TaskName, EAITaskPriority Priority,
    EAIAgentRole RequiredRole, int32 MinAgents, int32 MaxAgents, const FVector& TargetLocation);

// 分配任务给指定智能体
void AssignTaskToAgents(const FString& TaskID, const TArray<FString>& AgentIDs);

// 自动分配任务
void AssignTaskAutomatically(const FString& TaskID);

// 更新任务进度
void UpdateTaskProgress(const FString& TaskID, float Progress);

// 完成任务
void CompleteTask(const FString& TaskID);

// 取消任务
void CancelTask(const FString& TaskID);

// 获取任务信息
FAICollaborativeTask GetTaskInfo(const FString& TaskID) const;

// 获取活跃任务ID列表
TArray<FString> GetActiveTaskIDs() const;
```

### 集体知识

```cpp
// 分享知识
void ShareKnowledge(const FString& AgentID, const FString& Category,
    const FString& Content, float Confidence);

// 更新知识置信度
void UpdateKnowledgeConfidence(const FString& KnowledgeID, float NewConfidence);

// 获取指定类别的知识
TArray<FCollectiveIntelligenceData> GetCollectiveKnowledge(const FString& Category) const;

// 查询知识
FCollectiveIntelligenceData QueryKnowledge(const FString& Query) const;
```

### 协作策略

```cpp
// 执行最优角色分配
void OptimalRoleAssignment(const FString& TaskID);

// 形成任务组
void FormTaskGroup(EAIAgentRole GroupType, int32 GroupSize);

// 基于表现重新分配角色
void ReassignRolesBasedOnPerformance();

// 设置协作策略
void SetCollaborationStrategy(EAICollaborationStrategy NewStrategy);

// 获取当前策略
EAICollaborationStrategy GetCurrentStrategy() const;
```

### 网络指标

```cpp
// 获取网络指标
FAINetworkMetrics GetNetworkMetrics() const;

// 计算团队协同度
float CalculateTeamSynergy(const TArray<FString>& AgentIDs) const;

// 查找最优团队
TArray<FString> FindOptimalTeam(EAIAgentRole TaskType, int32 TeamSize) const;
```

## 数据结构

### FAIAgentData
```cpp
struct FAIAgentData
{
    FString AgentID;                    // 智能体ID
    FString AgentName;                  // 智能体名称
    EAIAgentRole PrimaryRole;           // 主要角色
    TArray<EAIAgentRole> SecondaryRoles; // 次要角色
    EAIAgentStatus Status;              // 当前状态
    TMap<FString, float> SkillLevels;   // 技能等级
    float ExperienceLevel;              // 经验等级
    float EfficiencyScore;              // 效率分数
    float TrustLevel;                   // 信任等级
    TArray<FString> ConnectedAgents;    // 连接的智能体
    FString CurrentTaskID;              // 当前任务ID
    float LastActivityTime;             // 最后活动时间
};
```

### FAICollaborativeTask
```cpp
struct FAICollaborativeTask
{
    FString TaskID;                     // 任务ID
    FString TaskName;                   // 任务名称
    EAITaskPriority Priority;           // 优先级
    EAIAgentRole RequiredRole;          // 所需角色
    int32 MinAgentsRequired;            // 最少需要智能体数
    int32 MaxAgentsAllowed;             // 最多允许智能体数
    TArray<FString> AssignedAgents;     // 已分配的智能体
    FString CoordinatorAgentID;         // 协调者ID
    float Progress;                     // 进度
    bool bIsComplete;                   // 是否完成
    FVector TargetLocation;             // 目标位置
    float TaskValue;                    // 任务价值
    float EstimatedDuration;            // 预计持续时间
    TMap<FString, float> AgentContributions; // 智能体贡献度
};
```

### FAIAgentMessage
```cpp
struct FAIAgentMessage
{
    FString MessageID;                  // 消息ID
    FString SenderID;                   // 发送者ID
    TArray<FString> RecipientIDs;       // 接收者ID列表
    EAICommunicationType CommunicationType; // 通信类型
    FString MessageContent;             // 消息内容
    float Timestamp;                    // 时间戳
    float Priority;                     // 优先级
    bool bRequiresAcknowledgment;       // 是否需要确认
    float ExpirationTime;               // 过期时间
};
```

### FCollectiveIntelligenceData
```cpp
struct FCollectiveIntelligenceData
{
    FString KnowledgeID;                // 知识ID
    FString KnowledgeCategory;          // 知识类别
    FString KnowledgeContent;           // 知识内容
    float ConfidenceLevel;              // 置信度
    int32 ContributorCount;             // 贡献者数量
    int32 UsageCount;                   // 使用次数
    float CreationTime;                 // 创建时间
    float LastUpdateTime;               // 最后更新时间
};
```

### FAINetworkMetrics
```cpp
struct FAINetworkMetrics
{
    int32 TotalAgents;                  // 总智能体数
    int32 ActiveAgents;                 // 活跃智能体数
    TMap<EAIAgentRole, int32> RoleDistribution; // 角色分布
    int32 TotalTasks;                   // 总任务数
    int32 CompletedTasks;               // 已完成任务数
    float AverageTaskCompletionTime;    // 平均任务完成时间
    float NetworkEfficiency;            // 网络效率
    float CollaborationSuccessRate;     // 协作成功率
    float CollectiveIntelligenceScore;  // 集体智能分数
};
```

## 事件委托

```cpp
// 智能体加入网络
FOnAgentJoined OnAgentJoined;

// 智能体离开网络
FOnAgentLeft OnAgentLeft;

// 收到消息
FOnMessageReceived OnMessageReceived;

// 任务已分配
FOnTaskAssigned OnTaskAssigned;

// 任务完成
FOnTaskCompleted OnTaskCompleted;

// 集体知识更新
FOnCollectiveIntelligenceUpdate OnCollectiveIntelligenceUpdate;
```

## 使用示例

### 初始化网络
```cpp
UMingRTSCollaborativeAINetwork* Network = NewObject<UMingRTSCollaborativeAINetwork>();
Network->InitializeNetwork(EAICollaborationStrategy::Cooperative);
```

### 注册智能体
```cpp
FString CommanderID = Network->RegisterAgent(TEXT("Commander1"), EAIAgentRole::Commander);
FString ScoutID = Network->RegisterAgent(TEXT("Scout1"), EAIAgentRole::Scout);
FString AttackerID = Network->RegisterAgent(TEXT("Attacker1"), EAIAgentRole::Attacker);
```

### 创建协作任务
```cpp
FString TaskID = Network->CreateCollaborativeTask(
    TEXT("Attack Enemy Base"),
    EAITaskPriority::High,
    EAIAgentRole::Attacker,
    2,  // 最少需要2个智能体
    5,  // 最多5个智能体
    FVector(1000.0f, 2000.0f, 0.0f)
);
```

### 智能体通信
```cpp
// 指挥官发送战术指令
TArray<FString> Recipients = {ScoutID, AttackerID};
Network->SendMessage(
    CommanderID,
    Recipients,
    EAICommunicationType::Strategy,
    TEXT("Enemy base detected at coordinates (1000, 2000). Prepare for attack."),
    8.0f
);
```

### 分享知识
```cpp
Network->ShareKnowledge(
    ScoutID,
    TEXT("EnemyPositions"),
    TEXT("Enemy base located at (1000, 2000) with 5 units defending."),
    0.95f  // 高置信度
);
```

### 监听事件
```cpp
Network->OnTaskCompleted.AddDynamic(this, &AMyGameMode::OnTaskCompleted);

void AMyGameMode::OnTaskCompleted(const FString& TaskID)
{
    UE_LOG(LogTemp, Log, TEXT("Task %s completed!"), *TaskID);
}
```

## 性能考虑

- **智能体数量**: 建议单个网络不超过100个智能体
- **消息频率**: 避免每秒超过1000条消息
- **知识库大小**: 定期清理低置信度知识
- **任务数量**: 同时活跃任务建议不超过50个

## 最佳实践

1. **角色分配**: 根据任务需求合理分配智能体角色
2. **通信管理**: 使用适当的通信类型和优先级
3. **知识共享**: 鼓励智能体分享关键情报
4. **任务分解**: 将复杂任务分解为子任务分配给不同智能体
5. **监控指标**: 定期检查网络指标以优化协作效率

## 扩展性

系统支持通过以下方式扩展:
- 添加新的智能体角色
- 自定义协作策略
- 实现新的任务分配算法
- 扩展知识表示方式
