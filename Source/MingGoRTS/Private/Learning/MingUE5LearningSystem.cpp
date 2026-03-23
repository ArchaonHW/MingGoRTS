// MingGoRTS UE5.7 API Learning System
// Generated: 2026-03-23 14:01:33

#include 'MingUE5LearningSystem.h'
#include 'HAL/PlatformFilemanager.h'
#include 'Misc/FileHelper.h'
#include 'Misc/Paths.h'
#include 'Misc/DateTime.h'

UMingUE5LearningSystem::UMingUE5LearningSystem()
{
    LearnedAPIs = 0;
    TotalAPIs = 0;
}

bool UMingUE5LearningSystem::InitializeLearningSystem()
{
    // Initialize API knowledge base
    InitializeAPIKnowledgeBase();

    // Update total APIs count
    TotalAPIs = APIKnowledgeBase.Num();

    return true;
}

bool UMingUE5LearningSystem::LearnAPI(const FString& APIName)
{
    if (!IsValidAPIName(APIName))
    {
        return false;
    }

    // Check if already learned
    if (APIKnowledgeBase.Contains(APIName))
    {
        UpdateLearningProgress();
        return true;
    }

    return false;
}

float UMingUE5LearningSystem::GetLearningProgress() const
{
    if (TotalAPIs == 0)
    {
        return 0.0f;
    }

    return (float)LearnedAPIs / TotalAPIs * 100.0f;
}

bool UMingUE5LearningSystem::GenerateLearningReport(const FString& ReportPath)
{
    FString ReportContent = TEXT('UE5.7 API Learning Report\\n');
    ReportContent += FString::Printf(TEXT('Generated: %s\\n'), *FDateTime::Now().ToString());
    ReportContent += FString::Printf(TEXT('Progress: %.1f%%\\n'), GetLearningProgress());
    ReportContent += FString::Printf(TEXT('Learned: %d/%d APIs\\n'), LearnedAPIs, TotalAPIs);

    return FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
}

void UMingUE5LearningSystem::InitializeAPIKnowledgeBase()
{
    // Core APIs
    APIKnowledgeBase.Add(TEXT('UEngine'), TEXT('The main engine class that manages core engine systems and provides access to global engine functionality.'));
    APIKnowledgeBase.Add(TEXT('UGameInstance'), TEXT('A game instance that persists across level transitions and manages game-wide state.'));
    APIKnowledgeBase.Add(TEXT('UWorld'), TEXT('Represents a game world containing actors, levels, and manages the simulation of that world.'));
    APIKnowledgeBase.Add(TEXT('AActor'), TEXT('Base class for all objects that can be placed or spawned in a level.'));
    APIKnowledgeBase.Add(TEXT('UObject'), TEXT('Base class for most objects in Unreal Engine, providing reflection, garbage collection, and serialization.'));

    // Gameplay APIs
    APIKnowledgeBase.Add(TEXT('AActorComponent'), TEXT('Base class for components that can be attached to actors to add functionality.'));
    APIKnowledgeBase.Add(TEXT('UGameModeBase'), TEXT('Base class for game modes that define the rules and behavior of the game.'));
    APIKnowledgeBase.Add(TEXT('APlayerController'), TEXT('Controls a player's pawn and handles player input and camera.'));
    APIKnowledgeBase.Add(TEXT('APawn'), TEXT('Base class for actors that can be controlled by players or AI.'));

    // AI APIs
    APIKnowledgeBase.Add(TEXT('UBehaviorTree'), TEXT('Asset that defines AI behavior using a tree of nodes.'));
    APIKnowledgeBase.Add(TEXT('UBlackboardComponent'), TEXT('Component that stores data used by behavior trees for AI decision making.'));
    APIKnowledgeBase.Add(TEXT('UEnvQuerySystem'), TEXT('System that handles environment queries for AI to find locations or objects.'));
    APIKnowledgeBase.Add(TEXT('ANavigationData'), TEXT('Base class for navigation data used by AI for pathfinding.'));

    // Rendering APIs
    APIKnowledgeBase.Add(TEXT('UMaterial'), TEXT('Asset that defines the appearance of surfaces through properties and expressions.'));
    APIKnowledgeBase.Add(TEXT('UMaterialInstanceDynamic'), TEXT('Runtime material instance that can modify material parameters dynamically.'));
    APIKnowledgeBase.Add(TEXT('FPrimitiveSceneProxy'), TEXT('Proxy class for rendering primitives in the scene rendering system.'));
    APIKnowledgeBase.Add(TEXT('FSceneView'), TEXT('Represents a view of the scene for rendering.'));

    // Performance APIs
    APIKnowledgeBase.Add(TEXT('FStatGroup'), TEXT('Declaration for grouping performance statistics.'));
    APIKnowledgeBase.Add(TEXT('IStatsThread'), TEXT('Interface for accessing the stats thread system.'));
    APIKnowledgeBase.Add(TEXT('FStreamingManager'), TEXT('Manager for handling asset streaming and level streaming.'));
}

void UMingUE5LearningSystem::UpdateLearningProgress()
{
    LearnedAPIs = APIKnowledgeBase.Num();
}

bool UMingUE5LearningSystem::IsValidAPIName(const FString& APIName) const
{
    return APIKnowledgeBase.Contains(APIName);
}

