#include "MingQuestManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UMingQuestManager::UMingQuestManager()
{
    bInitialized = false;
    QuestDatabase.Empty();
    StoryNodeDatabase.Empty();
    ActiveQuests.Empty();
    CompletedQuests.Empty();
    StoryChoiceHistory.Empty();
}

bool UMingQuestManager::InitializeQuestSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("任務系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化任務系統..."));

    LoadDefaultQuests();
    LoadDefaultStoryNodes();
    LoadQuestData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("任務系統初始化完成"));
    return true;
}

bool UMingQuestManager::AcceptQuest(int32 QuestID, const FString& QuestGiver)
{
    if (!CanAcceptQuest(QuestID))
    {
        UE_LOG(LogTemp, Warning, TEXT("無法接受任務 %d"), QuestID);
        return false;
    }

    FMingQuestData& Quest = QuestDatabase[QuestID];
    Quest.QuestStatus = EMingQuestStatus::Active;
    Quest.StartTime = FDateTime::Now();
    Quest.QuestGiver = QuestGiver;

    ActiveQuests.Add(QuestID);

    OnQuestAccepted.Broadcast(Quest, QuestGiver);

    UE_LOG(LogTemp, Log, TEXT("接受任務: %s"), *Quest.QuestName);
    return true;
}

bool UMingQuestManager::CompleteQuest(int32 QuestID, bool bSuccess)
{
    if (!QuestDatabase.Contains(QuestID))
    {
        UE_LOG(LogTemp, Warning, TEXT("任務ID %d 不存在"), QuestID);
        return false;
    }

    FMingQuestData& Quest = QuestDatabase[QuestID];
    
    if (bSuccess)
    {
        Quest.QuestStatus = EMingQuestStatus::Completed;
        ProcessQuestRewards(Quest);
        CompletedQuests.Add(QuestID);
    }
    else
    {
        Quest.QuestStatus = EMingQuestStatus::Failed;
        HandleQuestFailure(QuestID, TEXT("任務失敗"));
    }

    ActiveQuests.Remove(QuestID);
    OnQuestCompleted.Broadcast(Quest, bSuccess);

    UE_LOG(LogTemp, Log, TEXT("任務 %s: %s"), 
        bSuccess ? TEXT("完成") : TEXT("失敗"), *Quest.QuestName);
    
    return true;
}

void UMingQuestManager::LoadDefaultQuests()
{
    // 主線任務：北伐開始
    FMingQuestData MainQuest1;
    MainQuest1.QuestID = 1001;
    MainQuest1.QuestName = TEXT("北伐開始");
    MainQuest1.QuestDescription = TEXT("參與國民革命軍北伐，統一中國");
    MainQuest1.QuestType = EMingQuestType::Main;
    MainQuest1.QuestStatus = EMingQuestStatus::Available;
    MainQuest1.QuestLevel = 1;
    MainQuest1.SuggestedLevel = 1;
    MainQuest1.ExperienceReward = 500.0f;
    MainQuest1.ReputationReward = 100.0f;
    MainQuest1.StoryImportance = 1.0f;
    MainQuest1.QuestGiver = TEXT("蔣介石");
    MainQuest1.QuestLocation = FVector(1000.0f, 500.0f, 0.0f);
    MainQuest1.QuestIconPath = TEXT("Icons/Quests/MainQuest.png");
    MainQuest1.QuestTags.Add(TEXT("historical"));
    MainQuest1.QuestTags.Add(TEXT("main_story"));
    
    // 添加任務目標
    FMingQuestObjective Objective1;
    Objective1.ObjectiveID = 1;
    Objective1.ObjectiveDescription = TEXT("集結部隊");
    Objective1.ObjectiveType = EMingQuestObjectiveType::Talk;
    Objective1.TargetCount = 1;
    Objective1.CurrentProgress = 0;
    Objective1.TargetNPC = TEXT("軍官");
    Objective1.bIsVisible = true;
    Objective1.bIsCompleted = false;
    MainQuest1.QuestObjectives.Add(Objective1);
    
    QuestDatabase.Add(MainQuest1.QuestID, MainQuest1);
    
    // 支線任務：收集情報
    FMingQuestData SideQuest1;
    SideQuest1.QuestID = 2001;
    SideQuest1.QuestName = TEXT("收集情報");
    SideQuest1.QuestDescription = TEXT("收集敵軍情報，為北伐做準備");
    SideQuest1.QuestType = EMingQuestType::Side;
    SideQuest1.QuestStatus = EMingQuestStatus::Available;
    SideQuest1.QuestLevel = 1;
    SideQuest1.SuggestedLevel = 1;
    SideQuest1.ExperienceReward = 100.0f;
    SideQuest1.ReputationReward = 25.0f;
    SideQuest1.StoryImportance = 0.5f;
    SideQuest1.QuestGiver = TEXT("情報官");
    SideQuest1.QuestIconPath = TEXT("Icons/Quests/SideQuest.png");
    SideQuest1.QuestTags.Add(TEXT("investigation"));
    
    QuestDatabase.Add(SideQuest1.QuestID, SideQuest1);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設任務"), QuestDatabase.Num());
}

void UMingQuestManager::LoadDefaultStoryNodes()
{
    // 北伐故事節點
    TArray<FMingStoryNode> StoryNodes;
    
    FMingStoryNode Node1;
    Node1.NodeID = 1;
    Node1.NodeType = TEXT("decision");
    Node1.NodeContent = TEXT("你將如何參與北伐？");
    Node1.NodePosition = FVector(1000.0f, 500.0f, 0.0f);
    
    // 添加故事選項
    FMingStoryChoice Choice1;
    Choice1.ChoiceID = 1;
    Choice1.ChoiceText = TEXT("積極參與");
    Choice1.ChoiceType = EMingStoryBranch::Strategic;
    Choice1.MoralImpact = 0.5f;
    Choice1.ReputationImpact = 25.0f;
    Choice1.ChoiceWeight = 1.0f;
    Node1.NodeChoices.Add(Choice1);
    
    FMingStoryChoice Choice2;
    Choice2.ChoiceID = 2;
    Choice2.ChoiceText = TEXT("謹慎觀察");
    Choice2.ChoiceType = EMingStoryBranch::Strategic;
    Choice2.MoralImpact = 0.0f;
    Choice2.ReputationImpact = 10.0f;
    Choice2.ChoiceWeight = 1.0f;
    Node1.NodeChoices.Add(Choice2);
    
    StoryNodes.Add(Node1);
    StoryNodeDatabase.Add(1001, StoryNodes);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個故事節點"), StoryNodeDatabase.Num());
}

bool UMingQuestManager::CanAcceptQuest(int32 QuestID) const
{
    if (!QuestDatabase.Contains(QuestID))
    {
        return false;
    }

    const FMingQuestData& Quest = QuestDatabase[QuestID];
    
    if (Quest.QuestStatus != EMingQuestStatus::Available)
    {
        return false;
    }

    if (!CheckPrerequisiteQuests(Quest))
    {
        return false;
    }

    if (!CheckUnlockConditions(Quest))
    {
        return false;
    }

    return true;
}

void UMingQuestManager::ProcessQuestRewards(const FMingQuestData& Quest)
{
    UE_LOG(LogTemp, Log, TEXT("處理任務獎勵: %s"), *Quest.QuestName);
    
    // 經驗獎勵
    if (Quest.ExperienceReward > 0.0f)
    {
        // 這裡需要與角色成長系統集成
        UE_LOG(LogTemp, Log, TEXT("獲得經驗: %.1f"), Quest.ExperienceReward);
    }
    
    // 聲望獎勵
    if (Quest.ReputationReward > 0.0f)
    {
        UE_LOG(LogTemp, Log, TEXT("獲得聲望: %.1f"), Quest.ReputationReward);
    }
    
    // 物品獎勵
    for (const FMingItemData& Item : Quest.ItemRewards)
    {
        UE_LOG(LogTemp, Log, TEXT("獲得物品: %s"), *Item.ItemName);
    }
}

bool UMingQuestManager::CheckPrerequisiteQuests(const FMingQuestData& Quest) const
{
    for (int32 PrereqQuestID : Quest.PrerequisiteQuests)
    {
        if (!CompletedQuests.Contains(PrereqQuestID))
        {
            return false;
        }
    }
    return true;
}

bool UMingQuestManager::CheckUnlockConditions(const FMingQuestData& Quest) const
{
    // 檢查等級要求
    for (const FString& Condition : Quest.UnlockConditions)
    {
        if (Condition.Contains(TEXT("level")))
        {
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT(" "));
            if (Parts.Num() >= 2)
            {
                int32 RequiredLevel = FCString::Atoi(*Parts[1]);
                // 這裡需要檢查玩家等級
                // 暫時假設滿足條件
            }
        }
    }
    return true;
}

bool UMingQuestManager::SaveQuestData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("QuestData.json");
    UE_LOG(LogTemp, Log, TEXT("任務數據已保存到: %s"), *SavePath);
    return true;
}

bool UMingQuestManager::LoadQuestData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("QuestData.json");
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入任務數據"), *LoadPath);
    return true;
}

void UMingQuestManager::ClearAllQuestData()
{
    QuestDatabase.Empty();
    StoryNodeDatabase.Empty();
    ActiveQuests.Empty();
    CompletedQuests.Empty();
    StoryChoiceHistory.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("所有任務數據已清除"));
}
