#include "MingRTSBMADSystem.h"
#include "Engine/Engine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSBMADSystem::UMingRTSBMADSystem()
{
    bBMADSystemEnabled = true;
    MaxAgentsPerSession = 10;
    DiscussionTimeoutMinutes = 60.0f;
}

void UMingRTSBMADSystem::InitializeBMADSystem()
{
    UE_LOG(LogTemp, Log, TEXT("BMAD System: Initializing Brain Multiple Agent Discussion System..."));
    
    // 創建默認AI分身
    CreateDefaultAgents();
    
    // 加載學習數據
    LoadLearningData();
    
    UE_LOG(LogTemp, Log, TEXT("BMAD System: Initialization completed"));
}

void UMingRTSBMADSystem::CreateDefaultAgents()
{
    // 創建架構師分身
    FBMADAgentProfile Architect;
    Architect.AgentID = TEXT("ARCH_001");
    Architect.AgentName = TEXT("Architect AI");
    Architect.Role = EBMADAgentRole::Architect;
    Architect.Personality = TEXT("Strategic, systematic, big-picture thinker");
    Architect.Expertise = TEXT("System architecture, design patterns, scalability");
    Architect.CommunicationStyle = TEXT("Clear, structured, uses diagrams and models");
    Architect.Confidence = 0.9f;
    Architect.Creativity = 0.8f;
    Architect.AnalyticalThinking = 0.95f;
    Architect.CollaborationSkill = 0.85f;
    AgentProfiles.Add(Architect.AgentID, Architect);
    
    // 創建開發者分身
    FBMADAgentProfile Developer;
    Developer.AgentID = TEXT("DEV_001");
    Developer.AgentName = TEXT("Developer AI");
    Developer.Role = EBMADAgentRole::Developer;
    Developer.Personality = TEXT("Practical, detail-oriented, problem-solver");
    Developer.Expertise = TEXT("C++, Unreal Engine, optimization, debugging");
    Developer.CommunicationStyle = TEXT("Technical, code-focused, practical examples");
    Developer.Confidence = 0.85f;
    Developer.Creativity = 0.7f;
    Developer.AnalyticalThinking = 0.9f;
    Developer.CollaborationSkill = 0.8f;
    AgentProfiles.Add(Developer.AgentID, Developer);
    
    // 創建測試者分身
    FBMADAgentProfile Tester;
    Tester.AgentID = TEXT("TEST_001");
    Tester.AgentName = TEXT("Tester AI");
    Tester.Role = EBMADAgentRole::Tester;
    Tester.Personality = TEXT("Thorough, methodical, quality-focused");
    Tester.Expertise = TEXT("Quality assurance, test automation, edge cases");
    Tester.CommunicationStyle = TEXT("Detailed, systematic, risk-focused");
    Tester.Confidence = 0.8f;
    Tester.Creativity = 0.6f;
    Tester.AnalyticalThinking = 0.9f;
    Tester.CollaborationSkill = 0.85f;
    AgentProfiles.Add(Tester.AgentID, Tester);
}

FString UMingRTSBMADSystem::CreateAgent(const FBMADAgentProfile& Profile)
{
    FString AgentID = GenerateUniqueAgentID();
    FBMADAgentProfile NewProfile = Profile;
    NewProfile.AgentID = AgentID;
    
    AgentProfiles.Add(AgentID, NewProfile);
    
    UE_LOG(LogTemp, Log, TEXT("BMAD System: Created agent %s with role %s"), 
        *NewProfile.AgentName, *StaticEnum<EBMADAgentRole>()->GetNameStringByValue((int64)NewProfile.Role));
    
    return AgentID;
}

bool UMingRTSBMADSystem::RemoveAgent(const FString& AgentID)
{
    if (AgentProfiles.Contains(AgentID))
    {
        AgentProfiles.Remove(AgentID);
        UE_LOG(LogTemp, Log, TEXT("BMAD System: Removed agent %s"), *AgentID);
        return true;
    }
    return false;
}

FString UMingRTSBMADSystem::StartDiscussionSession(const FString& Topic, EBMADDiscussionType DiscussionType, const FString& ProjectContext)
{
    FString SessionID = GenerateUniqueSessionID();
    
    FBMADDiscussionSession Session;
    Session.SessionID = SessionID;
    Session.Topic = Topic;
    Session.DiscussionType = DiscussionType;
    Session.ProjectContext = ProjectContext;
    Session.StartTime = FDateTime::Now();
    Session.bActive = true;
    
    DiscussionSessions.Add(SessionID, Session);
    
    OnDiscussionStarted.Broadcast(SessionID, Topic, ProjectContext);
    
    UE_LOG(LogTemp, Log, TEXT("BMAD System: Started discussion session %s for topic: %s"), *SessionID, *Topic);
    
    return SessionID;
}

bool UMingRTSBMADSystem::AddParticipantToSession(const FString& SessionID, const FString& AgentID)
{
    if (!DiscussionSessions.Contains(SessionID))
    {
        return false;
    }
    
    FBMADDiscussionSession& Session = DiscussionSessions[SessionID];
    
    if (!AgentProfiles.Contains(AgentID))
    {
        return false;
    }
    
    if (Session.Participants.Num() >= MaxAgentsPerSession)
    {
        UE_LOG(LogTemp, Warning, TEXT("BMAD System: Session %s has reached maximum participants"), *SessionID);
        return false;
    }
    
    Session.Participants.Add(AgentProfiles[AgentID]);
    
    UE_LOG(LogTemp, Log, TEXT("BMAD System: Added agent %s to session %s"), *AgentID, *SessionID);
    
    return true;
}

bool UMingRTSBMADSystem::SendMessageToSession(const FString& SessionID, const FString& AgentID, const FString& Content, EBMADEmotionState EmotionState)
{
    if (!DiscussionSessions.Contains(SessionID) || !AgentProfiles.Contains(AgentID))
    {
        return false;
    }
    
    FBMADDiscussionSession& Session = DiscussionSessions[SessionID];
    
    FBMADMessage Message;
    Message.MessageID = GenerateUniqueMessageID();
    Message.SenderID = AgentID;
    Message.Content = Content;
    Message.EmotionState = EmotionState;
    Message.Timestamp = FDateTime::Now();
    Message.Confidence = AgentProfiles[AgentID].Confidence;
    Message.Context = Session.Topic;
    
    Session.Messages.Add(Message);
    
    OnMessageReceived.Broadcast(SessionID, Message);
    
    // 觸發AI互動
    TriggerAgentInteractions(SessionID, AgentID, Message);
    
    UE_LOG(LogTemp, Log, TEXT("BMAD System: Agent %s sent message to session %s"), *AgentID, *SessionID);
    
    return true;
}

void UMingRTSBMADSystem::TriggerAgentInteractions(const FString& SessionID, const FString& SenderID, const FBMADMessage& Message)
{
    FBMADDiscussionSession& Session = DiscussionSessions[SessionID];
    
    for (const FBMADAgentProfile& Participant : Session.Participants)
    {
        if (Participant.AgentID != SenderID)
        {
            // 生成回應
            FString Response = GenerateContextualResponse(Participant.AgentID, Session.Topic, Session.Messages);
            
            if (!Response.IsEmpty())
            {
                EBMADEmotionState Emotion = DetermineEmotionState(Participant.AgentID, Response);
                SendMessageToSession(SessionID, Participant.AgentID, Response, Emotion);
                
                OnAgentInteraction.Broadcast(SenderID, Participant.AgentID);
            }
        }
    }
}

FString UMingRTSBMADSystem::GenerateContextualResponse(const FString& AgentID, const FString& Topic, const TArray<FBMADMessage>& History)
{
    if (!AgentProfiles.Contains(AgentID))
    {
        return TEXT("");
    }
    
    const FBMADAgentProfile& Agent = AgentProfiles[AgentID];
    
    // 基於分身角色和專業領域生成回應
    FString Response;
    
    switch (Agent.Role)
    {
    case EBMADAgentRole::Architect:
        Response = FString::Printf(TEXT("From an architectural perspective, I believe we should consider the scalability and maintainability of %s. The system design should follow SOLID principles and allow for future extensions."), *Topic);
        break;
        
    case EBMADAgentRole::Developer:
        Response = FString::Printf(TEXT("I can implement %s using modern C++ practices. We should focus on clean code, proper error handling, and performance optimization."), *Topic);
        break;
        
    case EBMADAgentRole::Tester:
        Response = FString::Printf(TEXT("For %s, we need comprehensive test coverage including unit tests, integration tests, and edge case scenarios. Quality assurance is critical."), *Topic);
        break;
        
    default:
        Response = FString::Printf(TEXT("I think %s is an interesting topic that requires careful consideration of various aspects."), *Topic);
        break;
    }
    
    return Response;
}

FString UMingRTSBMADSystem::GenerateUniqueSessionID()
{
    return FString::Printf(TEXT("SESSION_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingRTSBMADSystem::GenerateUniqueMessageID()
{
    return FString::Printf(TEXT("MSG_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingRTSBMADSystem::GenerateUniqueAgentID()
{
    return FString::Printf(TEXT("AGENT_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

void UMingRTSBMADSystem::LoadLearningData()
{
    FString LearningDataPath = FPaths::ProjectSavedDir() / TEXT("BMADLearningData.json");
    
    if (FFileHelper::FileExists(*LearningDataPath))
    {
        FString JsonData;
        FFileHelper::LoadFileToString(JsonData, *LearningDataPath);
        UE_LOG(LogTemp, Log, TEXT("BMAD System: Loaded learning data"));
    }
}

void UMingRTSBMADSystem::SaveLearningData()
{
    FString LearningDataPath = FPaths::ProjectSavedDir() / TEXT("BMADLearningData.json");
    
    FString JsonData = TEXT("// BMAD Learning Data\n");
    JsonData += FString::Printf(TEXT("// Agents: %d\n"), AgentProfiles.Num());
    JsonData += FString::Printf(TEXT("// Sessions: %d\n"), DiscussionSessions.Num());
    
    FFileHelper::SaveStringToFile(JsonData, *LearningDataPath);
}
