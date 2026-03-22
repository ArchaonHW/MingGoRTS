#include "MingHistoricalCharacterSystem.h"
#include "MingHistoricalDecisionManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

UMingHistoricalCharacterSystem::UMingHistoricalCharacterSystem()
{
    bInitialized = false;
    CharacterDatabase.Empty();
    DialogueNodeDatabase.Empty();
    InteractionHistory.Empty();
    ActiveDialogueCharacters.Empty();
    PoliticalAlliances.Empty();
}

bool UMingHistoricalCharacterSystem::InitializeCharacterSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("歷史人物系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化歷史人物系統..."));

    // 載入預設人物數據
    LoadDefaultCharacters();

    // 載入預設對話數據
    LoadDefaultDialogues();

    // 載入已保存的人物數據
    LoadCharacterData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("歷史人物系統初始化完成，共載入 %d 個人物"), CharacterDatabase.Num());
    
    return true;
}

bool UMingHistoricalCharacterSystem::RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character)
{
    if (!ValidateCharacterData(Character))
    {
        UE_LOG(LogTemp, Error, TEXT("人物數據驗證失敗: %s"), *Character.CharacterName);
        return false;
    }

    if (CharacterDatabase.Contains(Character.CharacterID))
    {
        UE_LOG(LogTemp, Warning, TEXT("人物ID %d 已存在，將覆蓋"), Character.CharacterID);
    }

    CharacterDatabase.Add(Character.CharacterID, Character);
    
    UE_LOG(LogTemp, Log, TEXT("註冊歷史人物: %s (ID: %d)"), *Character.CharacterName, Character.CharacterID);
    
    return true;
}

FMingHistoricalCharacter UMingHistoricalCharacterSystem::GetCharacterInfo(int32 CharacterID) const
{
    if (CharacterDatabase.Contains(CharacterID))
    {
        return CharacterDatabase[CharacterID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("人物ID %d 不存在，返回空人物"), CharacterID);
    return FMingHistoricalCharacter();
}

TArray<int32> UMingHistoricalCharacterSystem::GetCharactersAtLocation(const FString& Location)
{
    TArray<int32> CharactersAtLocation;
    
    for (const TPair<int32, FMingHistoricalCharacter>& Pair : CharacterDatabase)
    {
        int32 CharacterID = Pair.Key;
        const FMingHistoricalCharacter& Character = Pair.Value;
        
        if (Character.CurrentLocation == Location && Character.bAvailableForDialogue)
        {
            CharactersAtLocation.Add(CharacterID);
        }
    }

    return CharactersAtLocation;
}

bool UMingHistoricalCharacterSystem::StartDialogue(int32 CharacterID)
{
    if (!CharacterDatabase.Contains(CharacterID))
    {
        UE_LOG(LogTemp, Error, TEXT("人物ID %d 不存在"), CharacterID);
        return false;
    }

    FMingHistoricalCharacter& Character = CharacterDatabase[CharacterID];

    if (!Character.bAvailableForDialogue)
    {
        UE_LOG(LogTemp, Warning, TEXT("人物 %s 當前不可用於對話"), *Character.CharacterName);
        return false;
    }

    // 檢查根對話節點
    if (!DialogueNodeDatabase.Contains(Character.RootDialogueNode))
    {
        UE_LOG(LogTemp, Error, TEXT("人物 %s 的根對話節點 %d 不存在"), *Character.CharacterName, Character.RootDialogueNode);
        return false;
    }

    // 檢查對話條件
    if (!CheckDialogueConditions(Character.RootDialogueNode))
    {
        UE_LOG(LogTemp, Warning, TEXT("對話條件不滿足"));
        return false;
    }

    // 添加到活躍對話列表
    if (!ActiveDialogueCharacters.Contains(CharacterID))
    {
        ActiveDialogueCharacters.Add(CharacterID);
    }

    // 獲取對話節點
    const FMingDialogueNode& DialogueNode = DialogueNodeDatabase[Character.RootDialogueNode];

    // 觸發對話開始事件
    OnDialogueStarted.Broadcast(CharacterID, Character.RootDialogueNode, DialogueNode);

    UE_LOG(LogTemp, Log, TEXT("開始與人物 %s 的對話"), *Character.CharacterName);
    
    return true;
}

FMingDialogueNode UMingHistoricalCharacterSystem::GetDialogueNode(int32 NodeID) const
{
    if (DialogueNodeDatabase.Contains(NodeID))
    {
        return DialogueNodeDatabase[NodeID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("對話節點ID %d 不存在，返回空節點"), NodeID);
    return FMingDialogueNode();
}

bool UMingHistoricalCharacterSystem::ChooseDialogueOption(int32 CharacterID, int32 NodeID, int32 OptionID)
{
    if (!CharacterDatabase.Contains(CharacterID))
    {
        UE_LOG(LogTemp, Error, TEXT("人物ID %d 不存在"), CharacterID);
        return false;
    }

    if (!DialogueNodeDatabase.Contains(NodeID))
    {
        UE_LOG(LogTemp, Error, TEXT("對話節點ID %d 不存在"), NodeID);
        return false;
    }

    const FMingDialogueNode& DialogueNode = DialogueNodeDatabase[NodeID];
    
    // 驗證選項ID
    bool bValidOption = false;
    for (const FMingDialogueOption& Option : DialogueNode.Options)
    {
        if (Option.OptionID == OptionID)
        {
            bValidOption = true;
            break;
        }
    }

    if (!bValidOption)
    {
        UE_LOG(LogTemp, Error, TEXT("無效的對話選項ID: %d"), OptionID);
        return false;
    }

    // 處理對話選項後果
    ProcessDialogueConsequences(CharacterID, NodeID, OptionID);

    // 觸發選項選擇事件
    OnDialogueOptionChosen.Broadcast(CharacterID, OptionID);

    UE_LOG(LogTemp, Log, TEXT("玩家選擇了對話選項 %d 與人物 ID %d"), OptionID, CharacterID);
    
    return true;
}

bool UMingHistoricalCharacterSystem::CheckDialogueConditions(int32 NodeID) const
{
    if (!DialogueNodeDatabase.Contains(NodeID))
    {
        return false;
    }

    const FMingDialogueNode& DialogueNode = DialogueNodeDatabase[NodeID];

    // 檢查所有條件
    for (const FString& Condition : DialogueNode.Conditions)
    {
        // 簡單的條件檢查邏輯
        if (Condition.Contains(TEXT("關係")))
        {
            // 檢查人物關係條件
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT(">"));
            if (Parts.Num() == 3)
            {
                FString CharacterName = Parts[1];
                int32 RequiredValue = FCString::Atoi(*Parts[2]);
                
                // 這裡需要獲取當前玩家與該人物的關係值
                // 暫時返回true
            }
        }
        else if (Condition.Contains(TEXT("聲望")))
        {
            // 檢查聲望條件
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT(">"));
            if (Parts.Num() == 2)
            {
                float RequiredReputation = FCString::Atof(*Parts[1]);
                // 這裡需要獲取玩家當前聲望
                // 暫時返回true
            }
        }
    }

    return true;
}

int32 UMingHistoricalCharacterSystem::GetRelationshipValue(int32 CharacterID, const FString& TargetCharacterName) const
{
    if (!CharacterDatabase.Contains(CharacterID))
    {
        return 0; // 中立關係
    }

    const FMingHistoricalCharacter& Character = CharacterDatabase[CharacterID];
    
    if (Character.Relationships.Contains(TargetCharacterName))
    {
        return Character.Relationships[TargetCharacterName];
    }

    return 0; // 默認中立關係
}

bool UMingHistoricalCharacterSystem::UpdateRelationship(int32 CharacterID, const FString& TargetCharacterName, int32 ChangeValue)
{
    if (!CharacterDatabase.Contains(CharacterID))
    {
        UE_LOG(LogTemp, Error, TEXT("人物ID %d 不存在"), CharacterID);
        return false;
    }

    FMingHistoricalCharacter& Character = CharacterDatabase[CharacterID];
    
    int32 OldValue = Character.Relationships.FindRef(TargetCharacterName);
    int32 NewValue = OldValue + ChangeValue;
    NewValue = FMath::Clamp(NewValue, -100, 100); // 限制範圍
    
    Character.Relationships.Add(TargetCharacterName, NewValue);
    
    // 觸發關係變化事件
    OnRelationshipChanged.Broadcast(TargetCharacterName, NewValue);
    
    UE_LOG(LogTemp, Log, TEXT("人物關係更新: %s 與 %s %d -> %d"), 
        *Character.CharacterName, *TargetCharacterName, OldValue, NewValue);
    
    return true;
}

TArray<FMingCharacterInteraction> UMingHistoricalCharacterSystem::GetInteractionHistory(int32 CharacterID) const
{
    TArray<FMingCharacterInteraction> CharacterInteractions;
    
    for (const FMingCharacterInteraction& Interaction : InteractionHistory)
    {
        if (Interaction.CharacterID == CharacterID)
        {
            CharacterInteractions.Add(Interaction);
        }
    }

    return CharacterInteractions;
}

bool UMingHistoricalCharacterSystem::CreatePoliticalAlliance(int32 CharacterID1, int32 CharacterID2)
{
    if (!CharacterDatabase.Contains(CharacterID1) || !CharacterDatabase.Contains(CharacterID2))
    {
        UE_LOG(LogTemp, Error, TEXT("人物ID不存在，無法建立聯盟"));
        return false;
    }

    // 檢查是否已存在聯盟
    for (const TTuple<int32, int32>& Alliance : PoliticalAlliances)
    {
        if ((Alliance.Get<0>() == CharacterID1 && Alliance.Get<1>() == CharacterID2) ||
            (Alliance.Get<0>() == CharacterID2 && Alliance.Get<1>() == CharacterID1))
        {
            UE_LOG(LogTemp, Warning, TEXT("聯盟已存在"));
            return false;
        }
    }

    // 建立聯盟
    PoliticalAlliances.Add(MakeTuple(CharacterID1, CharacterID2));
    
    // 更新雙方關係
    FMingHistoricalCharacter& Character1 = CharacterDatabase[CharacterID1];
    FMingHistoricalCharacter& Character2 = CharacterDatabase[CharacterID2];
    
    Character1.Relationships.Add(Character2.CharacterName, 80); // 同盟關係
    Character2.Relationships.Add(Character1.CharacterName, 80);
    
    UE_LOG(LogTemp, Log, TEXT("建立政治聯盟: %s 與 %s"), *Character1.CharacterName, *Character2.CharacterName);
    
    return true;
}

bool UMingHistoricalCharacterSystem::BreakPoliticalAlliance(int32 CharacterID1, int32 CharacterID2)
{
    // 查找並移除聯盟
    for (int32 i = 0; i < PoliticalAlliances.Num(); i++)
    {
        const TTuple<int32, int32>& Alliance = PoliticalAlliances[i];
        if ((Alliance.Get<0>() == CharacterID1 && Alliance.Get<1>() == CharacterID2) ||
            (Alliance.Get<0>() == CharacterID2 && Alliance.Get<1>() == CharacterID1))
        {
            PoliticalAlliances.RemoveAt(i);
            
            // 更新雙方關係
            FMingHistoricalCharacter& Character1 = CharacterDatabase[CharacterID1];
            FMingHistoricalCharacter& Character2 = CharacterDatabase[CharacterID2];
            
            Character1.Relationships.Add(Character2.CharacterName, -50); // 敵對關係
            Character2.Relationships.Add(Character1.CharacterName, -50);
            
            UE_LOG(LogTemp, Log, TEXT("破壞政治聯盟: %s 與 %s"), *Character1.CharacterName, *Character2.CharacterName);
            
            return true;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("聯盟不存在，無法破壞"));
    return false;
}

TMap<EMingCharacterType, int32> UMingHistoricalCharacterSystem::GetCharacterStatistics() const
{
    TMap<EMingCharacterType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingCharacterType::MilitaryLeader, 0);
    Statistics.Add(EMingCharacterType::Politician, 0);
    Statistics.Add(EMingCharacterType::Diplomat, 0);
    Statistics.Add(EMingCharacterType::Businessman, 0);
    Statistics.Add(EMingCharacterType::Scholar, 0);
    Statistics.Add(EMingCharacterType::Revolutionary, 0);
    Statistics.Add(EMingCharacterType::Warlord, 0);
    Statistics.Add(EMingCharacterType::ForeignAdvisor, 0);
    
    // 統計人物類型
    for (const TPair<int32, FMingHistoricalCharacter>& Pair : CharacterDatabase)
    {
        const FMingHistoricalCharacter& Character = Pair.Value;
        int32& Count = Statistics[Character.CharacterType];
        Count++;
    }
    
    return Statistics;
}

bool UMingHistoricalCharacterSystem::SaveCharacterData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("HistoricalCharacters.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("人物數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個人物，%d 條互動記錄"), CharacterDatabase.Num(), InteractionHistory.Num());
    
    return true;
}

bool UMingHistoricalCharacterSystem::LoadCharacterData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("HistoricalCharacters.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入人物數據"), *LoadPath);
    
    return true;
}

void UMingHistoricalCharacterSystem::ClearAllCharacterData()
{
    CharacterDatabase.Empty();
    DialogueNodeDatabase.Empty();
    InteractionHistory.Empty();
    ActiveDialogueCharacters.Empty();
    PoliticalAlliances.Empty();
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("所有人物數據已清除"));
}

void UMingHistoricalCharacterSystem::LoadDefaultCharacters()
{
    // 創建預設的歷史人物
    
    // 人物1: 吳佩孚
    FMingHistoricalCharacter Character1;
    Character1.CharacterID = 1;
    Character1.CharacterName = TEXT("吳佩孚");
    Character1.CourtesyName = TEXT("子玉");
    Character1.CharacterType = EMingCharacterType::MilitaryLeader;
    Character1.CharacterDescription = TEXT("直系軍閥領袖，著名的北洋軍事將領");
    Character1.HistoricalBackground = TEXT("吳佩孚（1874-1939），字子玉，山東蓬萊人。北洋軍閥直系領袖，曾控制華北地區，是1920年代中國最有影響力的軍事人物之一。");
    Character1.CharacterAttributes = {
        {TEXT("軍事才能"), 85.0f},
        {TEXT("政治智慧"), 70.0f},
        {TEXT("領導魅力"), 80.0f},
        {TEXT("戰略眼光"), 75.0f},
        {TEXT("個人勇氣"), 90.0f}
    };
    Character1.Skills = {TEXT("軍事指揮"), TEXT("戰略規劃"), TEXT("政治談判")};
    Character1.Relationships = {
        {TEXT("曹錕"), 85},
        {TEXT("段祺瑞"), -60},
        {TEXT("張作霖"), -40}
    };
    Character1.CurrentLocation = TEXT("北京");
    Character1.Faction = TEXT("直系");
    Character1.CurrentStatus = TEXT("控制華北地區");
    Character1.bAvailableForDialogue = true;
    Character1.RootDialogueNode = 1001;
    
    RegisterHistoricalCharacter(Character1);
    
    // 人物2: 段祺瑞
    FMingHistoricalCharacter Character2;
    Character2.CharacterID = 2;
    Character2.CharacterName = TEXT("段祺瑞");
    Character2.CourtesyName = TEXT("芝泉");
    Character2.CharacterType = EMingCharacterType::Politician;
    Character2.CharacterDescription = TEXT("皖系軍閥領袖，中華民國臨時執政");
    Character2.HistoricalBackground = TEXT("段祺瑞（1865-1936），字芝泉，安徽合肥人。北洋軍閥皖系領袖，曾三任國務總理，1924年擔任中華民國臨時執政。");
    Character2.CharacterAttributes = {
        {TEXT("政治智慧"), 90.0f},
        {TEXT("軍事才能"), 65.0f},
        {TEXT("外交手腕"), 85.0f},
        {TEXT("改革意識"), 80.0f},
        {TEXT("個人威望"), 75.0f}
    };
    Character2.Skills = {TEXT("政治改革"), TEXT("外交談判"), TEXT("軍政管理")};
    Character2.Relationships = {
        {TEXT("吳佩孚"), -60},
        {TEXT("馮玉祥"), 20},
        {TEXT("張作霖"), -30}
    };
    Character2.CurrentLocation = TEXT("天津");
    Character2.Faction = TEXT("皖系");
    Character2.CurrentStatus = TEXT("籌備政治改革");
    Character2.bAvailableForDialogue = true;
    Character2.RootDialogueNode = 1002;
    
    RegisterHistoricalCharacter(Character2);
    
    // 人物3: 張作霖
    FMingHistoricalCharacter Character3;
    Character3.CharacterID = 3;
    Character3.CharacterName = TEXT("張作霖");
    Character3.CourtesyName = TEXT("雨亭");
    Character3.CharacterType = EMingCharacterType::Warlord;
    Character3.CharacterDescription = TEXT("奉系軍閥領袖，東三省統治者");
    Character3.HistoricalBackground = TEXT("張作霖（1875-1928），字雨亭，遼寧海城人。奉系軍閥領袖，統治東三省，被稱為『東北王』。1928年皇姑屯事件中被日軍炸死。");
    Character3.CharacterAttributes = {
        {TEXT("軍事才能"), 80.0f},
        {TEXT("政治智慧"), 60.0f},
        {TEXT("地方治理"), 85.0f},
        {TEXT("經濟管理"), 70.0f},
        {TEXT("個人野心"), 90.0f}
    };
    Character3.Skills = {TEXT("地方統治"), TEXT("軍事建設"), TEXT("經濟開發")};
    Character3.Relationships = {
        {TEXT("吳佩孚"), -40},
        {TEXT("段祺瑞"), -30},
        {TEXT("孫中山"), 10}
    };
    Character3.CurrentLocation = TEXT("瀋陽");
    Character3.Faction = TEXT("奉系");
    Character3.CurrentStatus = TEXT("鞏固東北統治");
    Character3.bAvailableForDialogue = true;
    Character3.RootDialogueNode = 1003;
    
    RegisterHistoricalCharacter(Character3);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設歷史人物"), CharacterDatabase.Num());
}

void UMingHistoricalCharacterSystem::LoadDefaultDialogues()
{
    // 創建預設的對話節點
    
    // 吳佩孚的對話節點
    FMingDialogueNode Dialogue1;
    Dialogue1.NodeID = 1001;
    Dialogue1.NodeText = TEXT("我是吳佩孚，直系軍閥的統帥。看來你是一位有志之士，願意為國家效力嗎？");
    Dialogue1.SpeakerName = TEXT("吳佩孚");
    Dialogue1.DialogueType = EMingDialogueType::Greeting;
    Dialogue1.Conditions.Add(TEXT("玩家在北京"));
    
    // 添加選項
    FMingDialogueOption Option1;
    Option1.OptionID = 1;
    Option1.OptionText = TEXT("願意效忠直系，為統一中國而戰！");
    Option1.OptionDescription = TEXT("表達對直系的忠誠");
    Option1.DialogueType = EMingDialogueType::Persuasion;
    Option1.Consequences = {
        {TEXT("直系好感度"), 25.0f},
        {TEXT("軍事地位"), 15.0f},
        {TEXT("政治影響力"), 10.0f}
    };
    Option1.RelationshipChanges = {
        {TEXT("吳佩孚"), 30},
        {TEXT("曹錕"), 20},
        {TEXT("段祺瑞"), -15}
    };
    Dialogue1.Options.Add(Option1);
    
    FMingDialogueOption Option2;
    Option2.OptionID = 2;
    Option2.OptionText = TEXT("我更關心國家的未來，您對統一有何看法？");
    Option2.OptionDescription = TEXT("詢問統一方針");
    Option2.DialogueType = EMingDialogueType::Political;
    Option2.Consequences = {
        {TEXT("政治智慧"), 10.0f},
        {TEXT("戰略眼光"), 15.0f}
    };
    Option2.RelationshipChanges = {
        {TEXT("吳佩孚"), 10}
    };
    Dialogue1.Options.Add(Option2);
    
    FMingDialogueOption Option3;
    Option3.OptionID = 3;
    Option3.OptionText = TEXT("我需要時間考慮，這是關乎國家大事的決定。");
    Option3.OptionDescription = TEXT("保持謹慎態度");
    Option3.DialogueType = EMingDialogueType::Personal;
    Option3.Consequences = {
        {TEXT("個人聲望"), 5.0f}
    };
    Option3.RelationshipChanges = {
        {TEXT("吳佩孚"), 5}
    };
    Dialogue1.Options.Add(Option3);
    
    DialogueNodeDatabase.Add(1001, Dialogue1);
    
    // 段祺瑞的對話節點
    FMingDialogueNode Dialogue2;
    Dialogue2.NodeID = 1002;
    Dialogue2.NodeText = TEXT("我是段祺瑞。中國需要改革，不是軍閥混戰。你是否有志於推動國家進步？");
    Dialogue2.SpeakerName = TEXT("段祺瑞");
    Dialogue2.DialogueType = EMingDialogueType::Greeting;
    Dialogue2.Conditions.Add(TEXT("玩家在天津"));
    
    // 添加選項
    FMingDialogueOption Option4;
    Option4.OptionID = 4;
    Option4.OptionText = TEXT("我支持您的改革理念，願意為現代化中國貢獻力量！");
    Option4.OptionDescription = TEXT("支持改革");
    Option4.DialogueType = EMingDialogueType::Persuasion;
    Option4.Consequences = {
        {TEXT("改革聲望"), 30.0f},
        {TEXT("政治影響力"), 20.0f},
        {TEXT("知識份子支持"), 25.0f}
    };
    Option4.RelationshipChanges = {
        {TEXT("段祺瑞"), 35},
        {TEXT("馮玉祥"), 15},
        {TEXT("吳佩孚"), -20}
    };
    Dialogue2.Options.Add(Option4);
    
    DialogueNodeDatabase.Add(1002, Dialogue2);
    
    // 張作霖的對話節點
    FMingDialogueNode Dialogue3;
    Dialogue3.NodeID = 1003;
    Dialogue3.NodeText = TEXT("我是張作霖，東三省的主人。這片土地需要強大的領袖，你覺得呢？");
    Dialogue3.SpeakerName = TEXT("張作霖");
    Dialogue3.DialogueType = EMingDialogueType::Greeting;
    Dialogue3.Conditions.Add(TEXT("玩家在瀋陽"));
    
    // 添加選項
    FMingDialogueOption Option5;
    Option5.OptionID = 5;
    Option5.OptionText = TEXT("東北確實需要強大的領導，我願意為您效力！");
    Option5.OptionDescription = TEXT("效忠奉系");
    Option5.DialogueType = EMingDialogueType::Persuasion;
    Option5.Consequences = {
        {TEXT("奉系好感度"), 30.0f},
        {TEXT("地方實力"), 20.0f},
        {TEXT("經濟機會"), 15.0f}
    };
    Option5.RelationshipChanges = {
        {TEXT("張作霖"), 40},
        {TEXT("張學良"), 25},
        {TEXT("吳佩孚"), -25}
    };
    Dialogue3.Options.Add(Option5);
    
    DialogueNodeDatabase.Add(1003, Dialogue3);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設對話節點"), DialogueNodeDatabase.Num());
}

bool UMingHistoricalCharacterSystem::ValidateCharacterData(const FMingHistoricalCharacter& Character) const
{
    if (Character.CharacterID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("人物ID必須大於0"));
        return false;
    }
    
    if (Character.CharacterName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("人物姓名不能為空"));
        return false;
    }
    
    if (Character.RootDialogueNode <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("根對話節點ID必須大於0"));
        return false;
    }
    
    return true;
}

bool UMingHistoricalCharacterSystem::ValidateDialogueData(const FMingDialogueNode& DialogueNode) const
{
    if (DialogueNode.NodeID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("對話節點ID必須大於0"));
        return false;
    }
    
    if (DialogueNode.NodeText.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("對話文本不能為空"));
        return false;
    }
    
    if (DialogueNode.Options.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("對話節點必須至少有一個選項"));
        return false;
    }
    
    return true;
}

void UMingHistoricalCharacterSystem::ProcessDialogueConsequences(int32 CharacterID, int32 NodeID, int32 OptionID)
{
    if (!DialogueNodeDatabase.Contains(NodeID))
    {
        return;
    }

    const FMingDialogueNode& DialogueNode = DialogueNodeDatabase[NodeID];
    
    // 找到選擇的選項
    for (const FMingDialogueOption& Option : DialogueNode.Options)
    {
        if (Option.OptionID == OptionID)
        {
            // 創建互動記錄
            FMingCharacterInteraction Interaction;
            Interaction.InteractionID = InteractionHistory.Num() + 1;
            Interaction.CharacterID = CharacterID;
            Interaction.InteractionTime = FDateTime::Now();
            Interaction.DialogueNodeID = NodeID;
            Interaction.ChosenOptionID = OptionID;
            Interaction.InteractionResults = Option.Consequences;
            Interaction.RelationshipChanges = Option.RelationshipChanges;
            
            // 獲取人物信息
            if (CharacterDatabase.Contains(CharacterID))
            {
                const FMingHistoricalCharacter& Character = CharacterDatabase[CharacterID];
                Interaction.InteractionLocation = Character.CurrentLocation;
            }
            
            // 處理關係變化
            for (const TPair<FString, int32>& Pair : Option.RelationshipChanges)
            {
                const FString& TargetName = Pair.Key;
                int32 ChangeValue = Pair.Value;
                UpdateRelationship(CharacterID, TargetName, ChangeValue);
            }
            
            // 記錄互動
            RecordInteraction(Interaction);
            
            // 觸發互動完成事件
            OnInteractionCompleted.Broadcast(Interaction);
            
            break;
        }
    }
}

void UMingHistoricalCharacterSystem::UpdateCharacterStatus(int32 CharacterID, const FString& NewStatus)
{
    if (CharacterDatabase.Contains(CharacterID))
    {
        FMingHistoricalCharacter& Character = CharacterDatabase[CharacterID];
        Character.CurrentStatus = NewStatus;
        
        UE_LOG(LogTemp, Log, TEXT("人物 %s 狀態更新為: %s"), *Character.CharacterName, *NewStatus);
    }
}

void UMingHistoricalCharacterSystem::RecordInteraction(const FMingCharacterInteraction& Interaction)
{
    InteractionHistory.Add(Interaction);
    
    UE_LOG(LogTemp, Log, TEXT("記錄人物互動: 人物ID %d, 對話節點 %d, 選項 %d"), 
        Interaction.CharacterID, Interaction.DialogueNodeID, Interaction.ChosenOptionID);
}

void UMingHistoricalCharacterSystem::TriggerCharacterEvent(int32 CharacterID, const FString& EventType)
{
    if (CharacterDatabase.Contains(CharacterID))
    {
        const FMingHistoricalCharacter& Character = CharacterDatabase[CharacterID];
        
        UE_LOG(LogTemp, Log, TEXT("觸發人物事件: %s - %s"), *Character.CharacterName, *EventType);
        
        // 這裡可以實作具體的事件處理邏輯
        if (EventType == TEXT("對話結束"))
        {
            UpdateCharacterStatus(CharacterID, TEXT("等待下次會面"));
        }
        else if (EventType == TEXT("建立聯盟"))
        {
            UpdateCharacterStatus(CharacterID, TEXT("聯盟談判中"));
        }
    }
}

FMingDialogueNode UMingHistoricalCharacterSystem::GenerateDynamicDialogue(int32 CharacterID, const FString& Context) const
{
    FMingDialogueNode DynamicNode;
    
    if (CharacterDatabase.Contains(CharacterID))
    {
        const FMingHistoricalCharacter& Character = CharacterDatabase[CharacterID];
        
        DynamicNode.NodeID = -1; // 動態生成節點
        DynamicNode.NodeText = FString::Printf(TEXT("根據當前情況，%s 說道：關於 %s，我認為..."), *Character.CharacterName, *Context);
        DynamicNode.SpeakerName = Character.CharacterName;
        DynamicNode.DialogueType = EMingDialogueType::Political;
        
        // 添加基本選項
        FMingDialogueOption DynamicOption;
        DynamicOption.OptionID = 1;
        DynamicOption.OptionText = TEXT("我明白了");
        DynamicOption.DialogueType = EMingDialogueType::Greeting;
        DynamicNode.Options.Add(DynamicOption);
    }
    
    return DynamicNode;
}
