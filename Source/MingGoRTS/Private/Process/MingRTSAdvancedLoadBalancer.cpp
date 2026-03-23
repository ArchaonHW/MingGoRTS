// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Advanced Load Balancer - High-Performance Load Distribution Implementation

#include "Process/MingRTSAdvancedLoadBalancer.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Guid.h"
#include "Containers/Queue.h"

UMingRTSAdvancedLoadBalancer::UMingRTSAdvancedLoadBalancer()
{
    InitializeLoadBalancer();
}

void UMingRTSAdvancedLoadBalancer::InitializeLoadBalancer()
{
    CurrentAlgorithm = ELoadBalancingAlgorithm::RoundRobin;
    bSessionAffinityEnabled = false;
    SessionTimeoutSeconds = 1800; // 30 minutes
    bCircuitBreakerEnabled = false;
    CircuitBreakerThreshold = 5;
    bRateLimitingEnabled = false;
    RateLimitRPS = 1000;
    RateLimitBurstSize = 100;
    RoundRobinIndex = 0;

    UE_LOG(LogTemp, Log, TEXT("Advanced Load Balancer initialized"));
}

FString UMingRTSAdvancedLoadBalancer::AddServerNode(const FString& IPAddress, int32 Port, int32 Weight)
{
    FServerNode NewNode;
    NewNode.NodeID = GenerateNodeID();
    NewNode.IPAddress = IPAddress;
    NewNode.Port = Port;
    NewNode.Weight = Weight;
    NewNode.Status = EServerStatus::Unknown;
    NewNode.LastHealthCheck = FDateTime::Now();
    NewNode.bEnabled = true;

    ServerNodes.Add(NewNode.NodeID, NewNode);
    NodeFailureCounts.Add(NewNode.NodeID, 0);

    OnNodeAdded.Broadcast(NewNode.NodeID, NewNode);
    
    UE_LOG(LogTemp, Log, TEXT("Added server node: %s (%s:%d)"), *NewNode.NodeID, *IPAddress, Port);
    return NewNode.NodeID;
}

bool UMingRTSAdvancedLoadBalancer::RemoveServerNode(const FString& NodeID)
{
    if (FServerNode* Node = ServerNodes.Find(NodeID))
    {
        ServerNodes.Remove(NodeID);
        NodeFailureCounts.Remove(NodeID);
        LastRequestTimes.Remove(NodeID);
        
        OnNodeRemoved.Broadcast(NodeID, TEXT("Manual removal"));
        UE_LOG(LogTemp, Log, TEXT("Removed server node: %s"), *NodeID);
        return true;
    }
    
    return false;
}

bool UMingRTSAdvancedLoadBalancer::UpdateServerNode(const FString& NodeID, const FServerNode& UpdatedNode)
{
    if (FServerNode* ExistingNode = ServerNodes.Find(NodeID))
    {
        *ExistingNode = UpdatedNode;
        ExistingNode->NodeID = NodeID; // Preserve original ID
        
        UE_LOG(LogTemp, Log, TEXT("Updated server node: %s"), *NodeID);
        return true;
    }
    
    return false;
}

FServerNode UMingRTSAdvancedLoadBalancer::GetServerNode(const FString& NodeID) const
{
    if (const FServerNode* Node = ServerNodes.Find(NodeID))
    {
        return *Node;
    }
    return FServerNode();
}

TArray<FServerNode> UMingRTSAdvancedLoadBalancer::GetAllServerNodes() const
{
    TArray<FServerNode> Result;
    ServerNodes.GenerateValueArray(Result);
    return Result;
}

TArray<FServerNode> UMingRTSAdvancedLoadBalancer::GetHealthyNodes() const
{
    TArray<FServerNode> HealthyNodes;
    
    for (const auto& NodePair : ServerNodes)
    {
        if (IsNodeHealthy(NodePair.Key) && NodePair.Value.bEnabled)
        {
            HealthyNodes.Add(NodePair.Value);
        }
    }
    
    return HealthyNodes;
}

bool UMingRTSAdvancedLoadBalancer::EnableServerNode(const FString& NodeID)
{
    if (FServerNode* Node = ServerNodes.Find(NodeID))
    {
        Node->bEnabled = true;
        UE_LOG(LogTemp, Log, TEXT("Enabled server node: %s"), *NodeID);
        return true;
    }
    return false;
}

bool UMingRTSAdvancedLoadBalancer::DisableServerNode(const FString& NodeID)
{
    if (FServerNode* Node = ServerNodes.Find(NodeID))
    {
        Node->bEnabled = false;
        UE_LOG(LogTemp, Log, TEXT("Disabled server node: %s"), *NodeID);
        return true;
    }
    return false;
}

FString UMingRTSAdvancedLoadBalancer::RouteRequest(const FString& ClientIP, const FString& Path, const FString& RequestID)
{
    // Check rate limiting
    if (bRateLimitingEnabled && !CheckRateLimit(ClientIP))
    {
        UE_LOG(LogTemp, Warning, TEXT("Rate limit exceeded for client: %s"), *ClientIP);
        return FString();
    }

    // Get healthy nodes
    TArray<FString> HealthyNodeIDs;
    TArray<FServerNode> HealthyNodes = GetHealthyNodes();
    
    for (const FServerNode& Node : HealthyNodes)
    {
        HealthyNodeIDs.Add(Node.NodeID);
    }

    if (HealthyNodeIDs.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No healthy nodes available for request routing"));
        return FString();
    }

    // Check for matching load balancing rules
    FLoadBalancingRule* MatchingRule = FindMatchingRule(Path);
    ELoadBalancingAlgorithm Algorithm = MatchingRule ? MatchingRule->Algorithm : CurrentAlgorithm;

    // Check session affinity
    if (bSessionAffinityEnabled)
    {
        if (const FString* AffinityNodeID = SessionAffinityMap.Find(ClientIP))
        {
            if (HealthyNodeIDs.Contains(*AffinityNodeID))
            {
                UpdateMetrics(true, 0.0f, *AffinityNodeID);
                return *AffinityNodeID;
            }
        }
    }

    // Select node based on algorithm
    FString SelectedNodeID;
    float StartTime = FDateTime::Now().ToUnixTimestamp() * 1000.0f;

    switch (Algorithm)
    {
    case ELoadBalancingAlgorithm::RoundRobin:
        SelectedNodeID = SelectNodeRoundRobin(HealthyNodeIDs);
        break;
    case ELoadBalancingAlgorithm::WeightedRoundRobin:
        SelectedNodeID = SelectNodeWeightedRoundRobin(HealthyNodeIDs);
        break;
    case ELoadBalancingAlgorithm::LeastConnections:
        SelectedNodeID = SelectNodeLeastConnections(HealthyNodeIDs);
        break;
    case ELoadBalancingAlgorithm::WeightedLeastConnections:
        SelectedNodeID = SelectNodeWeightedLeastConnections(HealthyNodeIDs);
        break;
    case ELoadBalancingAlgorithm::IPHash:
        SelectedNodeID = SelectNodeIPHash(HealthyNodeIDs, ClientIP);
        break;
    case ELoadBalancingAlgorithm::URLHash:
        SelectedNodeID = SelectNodeURLHash(HealthyNodeIDs, Path);
        break;
    case ELoadBalancingAlgorithm::Random:
        SelectedNodeID = SelectNodeRandom(HealthyNodeIDs);
        break;
    case ELoadBalancingAlgorithm::ResponseTime:
        SelectedNodeID = SelectNodeResponseTime(HealthyNodeIDs);
        break;
    case ELoadBalancingAlgorithm::Custom:
        SelectedNodeID = SelectNodeCustom(HealthyNodeIDs, RequestID);
        break;
    default:
        SelectedNodeID = SelectNodeRoundRobin(HealthyNodeIDs);
        break;
    }

    // Update session affinity
    if (bSessionAffinityEnabled && !SelectedNodeID.IsEmpty())
    {
        SessionAffinityMap.Add(ClientIP, SelectedNodeID);
    }

    // Update node connection count
    if (FServerNode* Node = ServerNodes.Find(SelectedNodeID))
    {
        Node->CurrentConnections++;
    }

    float EndTime = FDateTime::Now().ToUnixTimestamp() * 1000.0f;
    float ResponseTime = EndTime - StartTime;

    UpdateMetrics(true, ResponseTime, SelectedNodeID);
    
    UE_LOG(LogTemp, VeryVerbose, TEXT("Routed request %s to node %s (algorithm: %s)"), 
        *RequestID, *SelectedNodeID, *StaticEnum<ELoadBalancingAlgorithm>()->GetValueAsString(Algorithm));
    
    return SelectedNodeID;
}

void UMingRTSAdvancedLoadBalancer::SetLoadBalancingAlgorithm(ELoadBalancingAlgorithm Algorithm)
{
    CurrentAlgorithm = Algorithm;
    UE_LOG(LogTemp, Log, TEXT("Set load balancing algorithm to: %s"), *StaticEnum<ELoadBalancingAlgorithm>()->GetValueAsString(Algorithm));
}

ELoadBalancingAlgorithm UMingRTSAdvancedLoadBalancer::GetLoadBalancingAlgorithm() const
{
    return CurrentAlgorithm;
}

void UMingRTSAdvancedLoadBalancer::ConfigureHealthChecks(const FHealthCheckConfig& Config)
{
    HealthCheckConfig = Config;
    UE_LOG(LogTemp, Log, TEXT("Configured health checks: %s every %d seconds"), 
        *StaticEnum<EHealthCheckType>()->GetValueAsString(Config.CheckType), Config.IntervalSeconds);
}

void UMingRTSAdvancedLoadBalancer::StartHealthMonitoring()
{
    UE_LOG(LogTemp, Log, TEXT("Started health monitoring"));
    // In a real implementation, this would start a timer or background thread
}

void UMingRTSAdvancedLoadBalancer::StopHealthMonitoring()
{
    UE_LOG(LogTemp, Log, TEXT("Stopped health monitoring"));
    // In a real implementation, this would stop the timer or background thread
}

void UMingRTSAdvancedLoadBalancer::PerformHealthCheck(const FString& NodeID)
{
    if (FServerNode* Node = ServerNodes.Find(NodeID))
    {
        bool bHealthy = CheckNodeHealth(NodeID);
        EServerStatus NewStatus = bHealthy ? EServerStatus::Healthy : EServerStatus::Unhealthy;
        
        if (Node->Status != NewStatus)
        {
            UpdateNodeStatus(NodeID, NewStatus);
        }
        
        Node->LastHealthCheck = FDateTime::Now();
    }
}

void UMingRTSAdvancedLoadBalancer::PerformHealthCheckAll()
{
    for (const auto& NodePair : ServerNodes)
    {
        PerformHealthCheck(NodePair.Key);
    }
}

FString UMingRTSAdvancedLoadBalancer::AddLoadBalancingRule(const FString& Name, const FString& Pattern, ELoadBalancingAlgorithm Algorithm)
{
    FLoadBalancingRule NewRule;
    NewRule.RuleID = GenerateRuleID();
    NewRule.Name = Name;
    NewRule.Pattern = Pattern;
    NewRule.Algorithm = Algorithm;
    NewRule.bEnabled = true;

    LoadBalancingRules.Add(NewRule.RuleID, NewRule);
    
    UE_LOG(LogTemp, Log, TEXT("Added load balancing rule: %s (%s)"), *Name, *Pattern);
    return NewRule.RuleID;
}

bool UMingRTSAdvancedLoadBalancer::RemoveLoadBalancingRule(const FString& RuleID)
{
    if (LoadBalancingRules.Remove(RuleID) > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Removed load balancing rule: %s"), *RuleID);
        return true;
    }
    return false;
}

bool UMingRTSAdvancedLoadBalancer::EnableLoadBalancingRule(const FString& RuleID)
{
    if (FLoadBalancingRule* Rule = LoadBalancingRules.Find(RuleID))
    {
        Rule->bEnabled = true;
        return true;
    }
    return false;
}

bool UMingRTSAdvancedLoadBalancer::DisableLoadBalancingRule(const FString& RuleID)
{
    if (FLoadBalancingRule* Rule = LoadBalancingRules.Find(RuleID))
    {
        Rule->bEnabled = false;
        return true;
    }
    return false;
}

TArray<FLoadBalancingRule> UMingRTSAdvancedLoadBalancer::GetAllLoadBalancingRules() const
{
    TArray<FLoadBalancingRule> Result;
    LoadBalancingRules.GenerateValueArray(Result);
    return Result;
}

FLoadBalancingMetrics UMingRTSAdvancedLoadBalancer::GetMetrics() const
{
    return Metrics;
}

void UMingRTSAdvancedLoadBalancer::ResetMetrics()
{
    Metrics = FLoadBalancingMetrics();
    UE_LOG(LogTemp, Log, TEXT("Reset load balancing metrics"));
}

TMap<FString, float> UMingRTSAdvancedLoadBalancer::GetNodePerformanceScores() const
{
    TMap<FString, float> PerformanceScores;
    
    for (const auto& NodePair : ServerNodes)
    {
        float Score = 0.0f;
        
        // Calculate performance score based on response time and success rate
        if (const float* ResponseTime = Metrics.NodeResponseTimes.Find(NodePair.Key))
        {
            Score += 1.0f / (1.0f + *ResponseTime); // Lower response time = higher score
        }
        
        if (const int32* RequestCount = Metrics.NodeRequestCounts.Find(NodePair.Key))
        {
            Score += FMath::Clamp(float(*RequestCount) / Metrics.TotalRequests, 0.0f, 1.0f);
        }
        
        PerformanceScores.Add(NodePair.Key, Score);
    }
    
    return PerformanceScores;
}

void UMingRTSAdvancedLoadBalancer::UpdateNodePerformance(const FString& NodeID, float ResponseTime, bool bSuccess)
{
    UpdateMetrics(bSuccess, ResponseTime, NodeID);
    
    if (FServerNode* Node = ServerNodes.Find(NodeID))
    {
        Node->ResponseTime = ResponseTime;
        
        if (!bSuccess)
        {
            Node->ConsecutiveFailures++;
            if (bCircuitBreakerEnabled && Node->ConsecutiveFailures >= CircuitBreakerThreshold)
            {
                UpdateNodeStatus(NodeID, EServerStatus::Failed);
            }
        }
        else
        {
            Node->ConsecutiveFailures = 0;
            if (Node->Status == EServerStatus::Failed)
            {
                UpdateNodeStatus(NodeID, EServerStatus::Healthy);
            }
        }
    }
}

void UMingRTSAdvancedLoadBalancer::EnableSessionAffinity(bool bEnabled)
{
    bSessionAffinityEnabled = bEnabled;
    if (!bEnabled)
    {
        SessionAffinityMap.Empty();
    }
    UE_LOG(LogTemp, Log, TEXT("Session affinity %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSAdvancedLoadBalancer::SetSessionTimeout(int32 TimeoutSeconds)
{
    SessionTimeoutSeconds = TimeoutSeconds;
    UE_LOG(LogTemp, Log, TEXT("Set session timeout to %d seconds"), TimeoutSeconds);
}

void UMingRTSAdvancedLoadBalancer::EnableCircuitBreaker(bool bEnabled, int32 FailureThreshold)
{
    bCircuitBreakerEnabled = bEnabled;
    CircuitBreakerThreshold = FailureThreshold;
    UE_LOG(LogTemp, Log, TEXT("Circuit breaker %s (threshold: %d)"), 
        bEnabled ? TEXT("enabled") : TEXT("disabled"), FailureThreshold);
}

void UMingRTSAdvancedLoadBalancer::EnableRateLimiting(int32 RequestsPerSecond, int32 BurstSize)
{
    bRateLimitingEnabled = true;
    RateLimitRPS = RequestsPerSecond;
    RateLimitBurstSize = BurstSize;
    UE_LOG(LogTemp, Log, TEXT("Rate limiting enabled (%d RPS, burst: %d)"), RequestsPerSecond, BurstSize);
}

// Internal Methods
FString UMingRTSAdvancedLoadBalancer::GenerateNodeID() const
{
    return FString::Printf(TEXT("node_%s"), *FGuid::NewGuid().ToString());
}

FString UMingRTSAdvancedLoadBalancer::GenerateRuleID() const
{
    return FString::Printf(TEXT("rule_%s"), *FGuid::NewGuid().ToString());
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeRoundRobin(const TArray<FString>& HealthyNodes)
{
    if (HealthyNodes.Num() == 0) return FString();
    
    FString SelectedNode = HealthyNodes[RoundRobinIndex % HealthyNodes.Num()];
    RoundRobinIndex = (RoundRobinIndex + 1) % HealthyNodes.Num();
    return SelectedNode;
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeWeightedRoundRobin(const TArray<FString>& HealthyNodes)
{
    if (HealthyNodes.Num() == 0) return FString();
    
    // Calculate total weight
    int32 TotalWeight = 0;
    TArray<FString> WeightedNodes;
    
    for (const FString& NodeID : HealthyNodes)
    {
        if (const FServerNode* Node = ServerNodes.Find(NodeID))
        {
            TotalWeight += Node->Weight;
            for (int32 i = 0; i < Node->Weight; i++)
            {
                WeightedNodes.Add(NodeID);
            }
        }
    }
    
    if (WeightedNodes.Num() == 0) return FString();
    
    int32 RandomIndex = FMath::RandRange(0, WeightedNodes.Num() - 1);
    return WeightedNodes[RandomIndex];
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeLeastConnections(const TArray<FString>& HealthyNodes)
{
    if (HealthyNodes.Num() == 0) return FString();
    
    FString SelectedNode;
    int32 MinConnections = INT_MAX;
    
    for (const FString& NodeID : HealthyNodes)
    {
        if (const FServerNode* Node = ServerNodes.Find(NodeID))
        {
            if (Node->CurrentConnections < MinConnections)
            {
                MinConnections = Node->CurrentConnections;
                SelectedNode = NodeID;
            }
        }
    }
    
    return SelectedNode;
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeWeightedLeastConnections(const TArray<FString>& HealthyNodes)
{
    if (HealthyNodes.Num() == 0) return FString();
    
    FString SelectedNode;
    float MinScore = FLT_MAX;
    
    for (const FString& NodeID : HealthyNodes)
    {
        if (const FServerNode* Node = ServerNodes.Find(NodeID))
        {
            float Score = float(Node->CurrentConnections) / Node->Weight;
            if (Score < MinScore)
            {
                MinScore = Score;
                SelectedNode = NodeID;
            }
        }
    }
    
    return SelectedNode;
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeIPHash(const TArray<FString>& HealthyNodes, const FString& ClientIP)
{
    if (HealthyNodes.Num() == 0) return FString();
    
    uint32 Hash = FCrc::StrCrc32(*ClientIP, nullptr);
    int32 Index = Hash % HealthyNodes.Num();
    return HealthyNodes[Index];
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeURLHash(const TArray<FString>& HealthyNodes, const FString& Path)
{
    if (HealthyNodes.Num() == 0) return FString();
    
    uint32 Hash = FCrc::StrCrc32(*Path, nullptr);
    int32 Index = Hash % HealthyNodes.Num();
    return HealthyNodes[Index];
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeRandom(const TArray<FString>& HealthyNodes)
{
    if (HealthyNodes.Num() == 0) return FString();
    
    int32 RandomIndex = FMath::RandRange(0, HealthyNodes.Num() - 1);
    return HealthyNodes[RandomIndex];
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeResponseTime(const TArray<FString>& HealthyNodes)
{
    if (HealthyNodes.Num() == 0) return FString();
    
    FString SelectedNode;
    float MinResponseTime = FLT_MAX;
    
    for (const FString& NodeID : HealthyNodes)
    {
        if (const FServerNode* Node = ServerNodes.Find(NodeID))
        {
            if (Node->ResponseTime < MinResponseTime)
            {
                MinResponseTime = Node->ResponseTime;
                SelectedNode = NodeID;
            }
        }
    }
    
    return SelectedNode;
}

FString UMingRTSAdvancedLoadBalancer::SelectNodeCustom(const TArray<FString>& HealthyNodes, const FString& Context)
{
    // Custom selection logic - for now, use round robin
    return SelectNodeRoundRobin(HealthyNodes);
}

bool UMingRTSAdvancedLoadBalancer::IsNodeHealthy(const FString& NodeID) const
{
    if (const FServerNode* Node = ServerNodes.Find(NodeID))
    {
        return Node->Status == EServerStatus::Healthy || 
               Node->Status == EServerStatus::Unknown ||
               Node->Status == EServerStatus::Draining;
    }
    return false;
}

void UMingRTSAdvancedLoadBalancer::UpdateNodeStatus(const FString& NodeID, EServerStatus NewStatus)
{
    if (FServerNode* Node = ServerNodes.Find(NodeID))
    {
        EServerStatus OldStatus = Node->Status;
        Node->Status = NewStatus;
        
        OnServerStatusChanged.Broadcast(NodeID, NewStatus);
        
        UE_LOG(LogTemp, Log, TEXT("Node status changed: %s %s -> %s"), 
            *NodeID, *StaticEnum<EServerStatus>()->GetValueAsString(OldStatus), *StaticEnum<EServerStatus>()->GetValueAsString(NewStatus));
    }
}

void UMingRTSAdvancedLoadBalancer::UpdateMetrics(bool bSuccess, float ResponseTime, const FString& NodeID)
{
    Metrics.TotalRequests++;
    
    if (bSuccess)
    {
        Metrics.SuccessfulRequests++;
    }
    else
    {
        Metrics.FailedRequests++;
    }
    
    // Update average response time
    if (Metrics.TotalRequests > 0)
    {
        Metrics.AverageResponseTime = (Metrics.AverageResponseTime * (Metrics.TotalRequests - 1) + ResponseTime) / Metrics.TotalRequests;
    }
    
    // Update node-specific metrics
    int32& NodeRequestCount = Metrics.NodeRequestCounts.FindOrAdd(NodeID, 0);
    NodeRequestCount++;
    
    float& NodeResponseTime = Metrics.NodeResponseTimes.FindOrAdd(NodeID, 0.0f);
    NodeResponseTime = (NodeResponseTime * (NodeRequestCount - 1) + ResponseTime) / NodeRequestCount;
    
    // Calculate requests per second
    FDateTime Now = FDateTime::Now();
    static FDateTime LastCalculation = Now;
    static int32 LastTotalRequests = 0;
    
    if (Now - LastCalculation > FTimespan::FromSeconds(1.0))
    {
        float TimeDiff = (Now - LastCalcuation).GetTotalSeconds();
        int32 RequestDiff = Metrics.TotalRequests - LastTotalRequests;
        Metrics.RequestsPerSecond = RequestDiff / TimeDiff;
        
        LastCalculation = Now;
        LastTotalRequests = Metrics.TotalRequests;
        
        OnLoadBalancingMetricsUpdated.Broadcast(Metrics);
    }
}

FLoadBalancingRule* UMingRTSAdvancedLoadBalancer::FindMatchingRule(const FString& Path)
{
    for (auto& RulePair : LoadBalancingRules)
    {
        FLoadBalancingRule& Rule = RulePair.Value;
        if (Rule.bEnabled && Path.Contains(Rule.Pattern))
        {
            return &Rule;
        }
    }
    return nullptr;
}

bool UMingRTSAdvancedLoadBalancer::CheckRateLimit(const FString& ClientIP)
{
    FDateTime Now = FDateTime::Now();
    FDateTime& LastRequestTime = LastRequestTimes.FindOrAdd(ClientIP, Now);
    
    FTimespan TimeSinceLastRequest = Now - LastRequestTime;
    
    // Simple rate limiting - allow one request per minimum interval
    float MinInterval = 1.0f / RateLimitRPS;
    
    if (TimeSinceLastRequest.GetTotalSeconds() >= MinInterval)
    {
        LastRequestTime = Now;
        return true;
    }
    
    return false;
}

void UMingRTSAdvancedLoadBalancer::CleanupExpiredSessions()
{
    FDateTime Now = FDateTime::Now();
    FTimespan Timeout = FTimespan::FromSeconds(SessionTimeoutSeconds);
    
    TArray<FString> ExpiredSessions;
    
    for (const auto& SessionPair : SessionAffinityMap)
    {
        // In a real implementation, we'd need to track session creation time
        // For now, this is a placeholder
    }
    
    for (const FString& ExpiredSession : ExpiredSessions)
    {
        SessionAffinityMap.Remove(ExpiredSession);
    }
}
