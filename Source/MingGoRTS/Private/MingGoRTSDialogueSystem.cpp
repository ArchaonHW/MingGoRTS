#include "MingGoRTSDialogueSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSDialogueSystem::UMingGoRTSDialogueSystem()
{
    bIsInitialized = false;
}

void UMingGoRTSDialogueSystem::InitializeDialogueSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("對話系統已經初始化"));
        return;
    }

    // 初始化對話庫
    InitializeDialogueLibrary();

    // 構建對話映射
    DialogueMap.Empty();
    for (const FMingDialogue& Dialogue : AllDialogues)
    {
        DialogueMap.Add(Dialogue.DialogueID, Dialogue);
        
        // 構建角色對話映射
        if (!CharacterDialogueMap.Contains(Dialogue.SpeakerID))
        {
            CharacterDialogueMap.Add(Dialogue.SpeakerID, TArray<FString>());
        }
        CharacterDialogueMap[Dialogue.SpeakerID].Add(Dialogue.DialogueID);
    }

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("對話系統初始化完成，共載入 %d 個對話"), AllDialogues.Num());
}

TArray<FMingDialogue> UMingGoRTSDialogueSystem::GetAllDialogues() const
{
    return AllDialogues;
}

FMingDialogue UMingGoRTSDialogueSystem::GetDialogue(const FString& DialogueID) const
{
    if (const FMingDialogue* Dialogue = DialogueMap.Find(DialogueID))
    {
        return *Dialogue;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到對話 ID：%s"), *DialogueID);
    return FMingDialogue();
}

bool UMingGoRTSDialogueSystem::StartDialogue(const FString& DialogueID, const FMingCharacterData& PlayerCharacter)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("對話系統未初始化"));
        return false;
    }

    if (!DialogueMap.Contains(DialogueID))
    {
        UE_LOG(LogTemp, Error, TEXT("對話不存在：%s"), *DialogueID);
        return false;
    }

    // 檢查對話是否可用
    if (!IsDialogueAvailable(DialogueID, PlayerCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("對話不可用：%s"), *DialogueID);
        return false;
    }

    // 設置當前對話和玩家
    CurrentDialogue = DialogueMap[DialogueID];
    CurrentPlayerCharacter = PlayerCharacter;

    // 添加到對話歷史
    AddDialogueToHistory(DialogueID);

    // 廣播對話開始事件
    OnDialogueStarted.Broadcast(DialogueID, CurrentDialogue.SpeakerID);

    // 執行自動結果
    if (CurrentDialogue.AutoOutcomes.Num() > 0)
    {
        ExecuteDialogueOutcomes(CurrentDialogue.AutoOutcomes);
    }

    UE_LOG(LogTemp, Log, TEXT("開始對話：%s，講者：%s"), *DialogueID, *CurrentDialogue.SpeakerID);
    return true;
}

bool UMingGoRTSDialogueSystem::SelectDialogueOption(int32 OptionIndex)
{
    if (CurrentDialogue.DialogueID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("沒有正在進行的對話"));
        return false;
    }

    if (OptionIndex < 0 || OptionIndex >= CurrentDialogue.DialogueOptions.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("無效的對話選項索引：%d"), OptionIndex);
        return false;
    }

    const FDialogueOption& SelectedOption = CurrentDialogue.DialogueOptions[OptionIndex];
    
    // 檢查選項是否可用
    if (!SelectedOption.bIsAvailable)
    {
        UE_LOG(LogTemp, Warning, TEXT("對話選項不可用：%d"), OptionIndex);
        return false;
    }

    // 廣播選項選擇事件
    OnDialogueOptionSelected.Broadcast(CurrentDialogue.DialogueID, OptionIndex, SelectedOption.OptionText);

    // 執行選項結果
    if (SelectedOption.Outcomes.Num() > 0)
    {
        ExecuteDialogueOutcomes(SelectedOption.Outcomes);
    }

    // 檢查是否有下一個對話
    if (!SelectedOption.NextDialogueID.IsEmpty())
    {
        return StartDialogue(SelectedOption.NextDialogueID, CurrentPlayerCharacter);
    }
    else
    {
        // 對話結束
        TArray<FDialogueOutcome> AllOutcomes = CurrentDialogue.AutoOutcomes;
        AllOutcomes.Append(SelectedOption.Outcomes);
        OnDialogueCompleted.Broadcast(CurrentDialogue.DialogueID, AllOutcomes);
        
        // 清空當前對話
        CurrentDialogue = FMingDialogue();
        
        UE_LOG(LogTemp, Log, TEXT("對話結束"));
        return true;
    }
}

bool UMingGoRTSDialogueSystem::IsDialogueAvailable(const FString& DialogueID, const FMingCharacterData& PlayerCharacter) const
{
    if (!DialogueMap.Contains(DialogueID))
    {
        return false;
    }

    const FMingDialogue& Dialogue = DialogueMap[DialogueID];

    // 檢查是否重複對話
    if (!Dialogue.bCanBeRepeated && DialogueHistory.Contains(DialogueID))
    {
        return false;
    }

    // 檢查觸發條件
    return ValidateDialogueConditions(Dialogue.TriggerConditions, PlayerCharacter);
}

bool UMingGoRTSDialogueSystem::IsDialogueOptionAvailable(const FString& DialogueID, int32 OptionIndex, const FMingCharacterData& PlayerCharacter) const
{
    if (!DialogueMap.Contains(DialogueID))
    {
        return false;
    }

    const FMingDialogue& Dialogue = DialogueMap[DialogueID];
    if (OptionIndex < 0 || OptionIndex >= Dialogue.DialogueOptions.Num())
    {
        return false;
    }

    const FDialogueOption& Option = Dialogue.DialogueOptions[OptionIndex];
    
    // 檢查選項可用性標記
    if (!Option.bIsAvailable)
    {
        return false;
    }

    // 檢查選項條件
    return ValidateDialogueConditions(Option.Conditions, PlayerCharacter);
}

TArray<FDialogueOption> UMingGoRTSDialogueSystem::GetAvailableDialogueOptions(const FString& DialogueID, const FMingCharacterData& PlayerCharacter) const
{
    TArray<FDialogueOption> AvailableOptions;
    
    if (DialogueMap.Contains(DialogueID))
    {
        const FMingDialogue& Dialogue = DialogueMap[DialogueID];
        for (int32 i = 0; i < Dialogue.DialogueOptions.Num(); ++i)
        {
            if (IsDialogueOptionAvailable(DialogueID, i, PlayerCharacter))
            {
                AvailableOptions.Add(Dialogue.DialogueOptions[i]);
            }
        }
    }
    
    return AvailableOptions;
}

TArray<FMingDialogue> UMingGoRTSDialogueSystem::GetCharacterDialogues(const FString& CharacterID) const
{
    TArray<FMingDialogue> CharacterDialogues;
    
    if (CharacterDialogueMap.Contains(CharacterID))
    {
        const TArray<FString>& DialogueIDs = CharacterDialogueMap[CharacterID];
        for (const FString& DialogueID : DialogueIDs)
        {
            if (DialogueMap.Contains(DialogueID))
            {
                CharacterDialogues.Add(DialogueMap[DialogueID]);
            }
        }
    }
    
    return CharacterDialogues;
}

TArray<FMingDialogue> UMingGoRTSDialogueSystem::GetHistoricalEventDialogues(const FString& EventID) const
{
    TArray<FMingDialogue> EventDialogues;
    
    for (const FMingDialogue& Dialogue : AllDialogues)
    {
        if (Dialogue.DialogueType == EDialogueType::HistoricalEvent && 
            Dialogue.HistoricalContext.Contains(EventID))
        {
            EventDialogues.Add(Dialogue);
        }
    }
    
    return EventDialogues;
}

TArray<FMingDialogue> UMingGoRTSDialogueSystem::GetImportantDialogues() const
{
    TArray<FMingDialogue> ImportantDialogues;
    
    for (const FMingDialogue& Dialogue : AllDialogues)
    {
        if (Dialogue.bIsImportant)
        {
            ImportantDialogues.Add(Dialogue);
        }
    }
    
    return ImportantDialogues;
}

TArray<FString> UMingGoRTSDialogueSystem::GetDialogueHistory() const
{
    return DialogueHistory;
}

void UMingGoRTSDialogueSystem::AddDialogueToHistory(const FString& DialogueID)
{
    if (!DialogueHistory.Contains(DialogueID))
    {
        DialogueHistory.Add(DialogueID);
        
        // 限制歷史記錄數量
        if (DialogueHistory.Num() > 100)
        {
            DialogueHistory.RemoveAt(0);
        }
        
        UE_LOG(LogTemp, Log, TEXT("對話已添加到歷史：%s"), *DialogueID);
    }
}

void UMingGoRTSDialogueSystem::ClearDialogueHistory()
{
    DialogueHistory.Empty();
    UE_LOG(LogTemp, Log, TEXT("對話歷史已清空"));
}

TArray<FDialogueOutcome> UMingGoRTSDialogueSystem::CalculateDialogueOutcomes(const FString& DialogueID, int32 OptionIndex) const
{
    TArray<FDialogueOutcome> Outcomes;
    
    if (DialogueMap.Contains(DialogueID))
    {
        const FMingDialogue& Dialogue = DialogueMap[DialogueID];
        
        // 添加自動結果
        Outcomes.Append(Dialogue.AutoOutcomes);
        
        // 添加選項結果
        if (OptionIndex >= 0 && OptionIndex < Dialogue.DialogueOptions.Num())
        {
            Outcomes.Append(Dialogue.DialogueOptions[OptionIndex].Outcomes);
        }
    }
    
    return Outcomes;
}

void UMingGoRTSDialogueSystem::ApplyDialogueOutcomes(const TArray<FDialogueOutcome>& Outcomes)
{
    for (const FDialogueOutcome& Outcome : Outcomes)
    {
        switch (Outcome.OutcomeType)
        {
        case EDialogueOutcome::RelationshipChange:
            UpdateCharacterRelationship(Outcome.TargetID, Outcome.Value);
            break;
        case EDialogueOutcome::QuestTrigger:
            TriggerQuest(Outcome.TargetID);
            break;
        case EDialogueOutcome::InformationGain:
            GrantInformation(Outcome.TargetID);
            break;
        case EDialogueOutcome::SkillGain:
            ImproveSkill(FName(*Outcome.TargetID), Outcome.Value);
            break;
        case EDialogueOutcome::EventTrigger:
            TriggerEvent(Outcome.TargetID);
            break;
        default:
            break;
        }
    }
}

bool UMingGoRTSDialogueSystem::SaveDialogueData(const FString& SaveSlotName)
{
    // TODO: 實現對話數據保存
    UE_LOG(LogTemp, Log, TEXT("保存對話數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSDialogueSystem::LoadDialogueData(const FString& SaveSlotName)
{
    // TODO: 實現對話數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入對話數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSDialogueSystem::InitializeDialogueLibrary()
{
    CreateHistoricalCharacterDialogues();
    CreateMilitaryDialogues();
    CreateDiplomaticDialogues();
    CreatePersonalStoryDialogues();
    CreateQuestDialogues();
}

void UMingGoRTSDialogueSystem::CreateHistoricalCharacterDialogues()
{
    // 蔣介石對話
    FMingDialogue ChiangDialogue;
    ChiangDialogue.DialogueID = GenerateDialogueID(TEXT("ChiangKaiShek_FirstMeeting"), TEXT("ChiangKaiShek"));
    ChiangDialogue.SpeakerID = TEXT("ChiangKaiShek");
    ChiangDialogue.DialogueText = TEXT("年輕人，我看你氣宇不凡，想必是黃埔軍校的精英。當前國家多難，正需要你這樣的人才為國效力。");
    ChiangDialogue.DialogueType = EDialogueType::HistoricalEvent;
    ChiangDialogue.SpeakerPortrait = TEXT("/Game/Portraits/ChiangKaiShek.ChiangKaiShek");
    ChiangDialogue.HistoricalContext = TEXT("NorthernExpedition");
    ChiangDialogue.bIsImportant = true;
    ChiangDialogue.bCanBeRepeated = false;

    // 添加對話選項
    FDialogueOption Option1;
    Option1.OptionText = TEXT("願為國家統一奉獻力量！");
    Option1.Attitude = EDialogueAttitude::Respectful;
    Option1.NextDialogueID = GenerateDialogueID(TEXT("ChiangKaiShek_LoyalResponse"), TEXT("ChiangKaiShek"));
    
    FDialogueOutcome Outcome1;
    Outcome1.OutcomeType = EDialogueOutcome::RelationshipChange;
    Outcome1.TargetID = TEXT("ChiangKaiShek");
    Outcome1.Value = 20.0f;
    Outcome1.Description = TEXT("獲得蔣介石信任");
    Option1.Outcomes.Add(Outcome1);
    
    ChiangDialogue.DialogueOptions.Add(Option1);

    FDialogueOption Option2;
    Option2.OptionText = TEXT("我需要考慮一下您的提議。");
    Option2.Attitude = EDialogueAttitude::Neutral;
    Option2.NextDialogueID = GenerateDialogueID(TEXT("ChiangKaiShek_NeutralResponse"), TEXT("ChiangKaiShek"));
    
    FDialogueOutcome Outcome2;
    Outcome2.OutcomeType = EDialogueOutcome::RelationshipChange;
    Outcome2.TargetID = TEXT("ChiangKaiShek");
    Outcome2.Value = 5.0f;
    Outcome2.Description = TEXT("保持中立關係");
    Option2.Outcomes.Add(Outcome2);
    
    ChiangDialogue.DialogueOptions.Add(Option2);

    AllDialogues.Add(ChiangDialogue);

    // 毛澤東對話
    FMingDialogue MaoDialogue;
    MaoDialogue.DialogueID = GenerateDialogueID(TEXT("MaoZedong_FirstMeeting"), TEXT("MaoZedong"));
    MaoDialogue.SpeakerID = TEXT("MaoZedong");
    MaoDialogue.DialogueText = TEXT("同志，我看你心懷天下，關心勞苦大眾。在這個動盪的年代，我們應該為人民的解放事業而奮鬥。");
    MaoDialogue.DialogueType = EDialogueType::HistoricalEvent;
    MaoDialogue.SpeakerPortrait = TEXT("/Game/Portraits/MaoZedong.MaoZedong");
    MaoDialogue.HistoricalContext = TEXT("RevolutionaryMovement");
    MaoDialogue.bIsImportant = true;
    MaoDialogue.bCanBeRepeated = false;

    FDialogueOption MaoOption1;
    MaoOption1.OptionText = TEXT("我願意加入革命事業！");
    MaoOption1.Attitude = EDialogueAttitude::Friendly;
    MaoOption1.NextDialogueID = GenerateDialogueID(TEXT("MaoZedong_RevolutionaryResponse"), TEXT("MaoZedong"));
    
    FDialogueOutcome MaoOutcome1;
    MaoOutcome1.OutcomeType = EDialogueOutcome::QuestTrigger;
    MaoOutcome1.TargetID = TEXT("RevolutionaryQuest");
    MaoOutcome1.Value = 1.0f;
    MaoOutcome1.Description = TEXT("觸發革命任務線");
    MaoOption1.Outcomes.Add(MaoOutcome1);
    
    MaoDialogue.DialogueOptions.Add(MaoOption1);

    AllDialogues.Add(MaoDialogue);
}

void UMingGoRTSDialogueSystem::CreateMilitaryDialogues()
{
    // 軍事簡報對話
    FMingDialogue MilitaryBriefing;
    MilitaryBriefing.DialogueID = GenerateDialogueID(TEXT("MilitaryBriefing_BattlePlan"), TEXT("Commander"));
    MilitaryBriefing.SpeakerID = TEXT("Commander");
    MilitaryBriefing.DialogueText = TEXT("將軍，根據最新情報，敵軍在前方10公里處集結。我們必須立即制定作戰計劃。你認為我們應該如何應對？");
    MilitaryBriefing.DialogueType = EDialogueType::MilitaryBriefing;
    MilitaryBriefing.SpeakerPortrait = TEXT("/Game/Portraits/MilitaryCommander.MilitaryCommander");

    FDialogueOption MilitaryOption1;
    MilitaryOption1.OptionText = TEXT("主動出擊，先發制人！");
    MilitaryOption1.Attitude = EDialogueAttitude::Hostile;
    MilitaryOption1.NextDialogueID = GenerateDialogueID(TEXT("MilitaryBriefing_AggressivePlan"), TEXT("Commander"));
    
    FDialogueOutcome MilitaryOutcome1;
    MilitaryOutcome1.OutcomeType = EDialogueOutcome::SkillGain;
    MilitaryOutcome1.TargetID = TEXT("TacticalCommand");
    MilitaryOutcome1.Value = 10.0f;
    MilitaryOutcome1.Description = TEXT("戰術指揮技能提升");
    MilitaryOption1.Outcomes.Add(MilitaryOutcome1);
    
    MilitaryBriefing.DialogueOptions.Add(MilitaryOption1);

    FDialogueOption MilitaryOption2;
    MilitaryOption2.OptionText = TEXT("固守防線，等待時機。");
    MilitaryOption2.Attitude = EDialogueAttitude::Neutral;
    MilitaryOption2.NextDialogueID = GenerateDialogueID(TEXT("MilitaryBriefing_DefensivePlan"), TEXT("Commander"));
    
    FDialogueOutcome MilitaryOutcome2;
    MilitaryOutcome2.OutcomeType = EDialogueOutcome::SkillGain;
    MilitaryOutcome2.TargetID = TEXT("Leadership");
    MilitaryOutcome2.Value = 8.0f;
    MilitaryOutcome2.Description = TEXT("領導力技能提升");
    MilitaryOption2.Outcomes.Add(MilitaryOutcome2);
    
    MilitaryBriefing.DialogueOptions.Add(MilitaryOption2);

    AllDialogues.Add(MilitaryBriefing);
}

void UMingGoRTSDialogueSystem::CreateDiplomaticDialogues()
{
    // 外交談話
    FMingDialogue DiplomaticTalk;
    DiplomaticTalk.DialogueID = GenerateDialogueID(TEXT("DiplomaticTalk_Alliance"), TEXT("Diplomat"));
    DiplomaticTalk.SpeakerID = TEXT("Diplomat");
    DiplomaticTalk.DialogueText = TEXT("將軍，我們的代表團前來商討結盟事宜。面對共同的威脅，我們應該團一致，共同對敵。");
    DiplomaticTalk.DialogueType = EDialogueType::DiplomaticTalk;
    DiplomaticTalk.SpeakerPortrait = TEXT("/Game/Portraits/Diplomat.Diplomat");

    FDialogueOption DiplomaticOption1;
    DiplomaticOption1.OptionText = TEXT("我們很樂意與貴方結盟。");
    DiplomaticOption1.Attitude = EDialogueAttitude::Friendly;
    DiplomaticOption1.NextDialogueID = GenerateDialogueID(TEXT("DiplomaticTalk_AllianceAccepted"), TEXT("Diplomat"));
    
    FDialogueOutcome DiplomaticOutcome1;
    DiplomaticOutcome1.OutcomeType = EDialogueOutcome::EventTrigger;
    DiplomaticOutcome1.TargetID = TEXT("AllianceFormed");
    DiplomaticOutcome1.Value = 1.0f;
    DiplomaticOutcome1.Description = TEXT("形成聯盟");
    DiplomaticOption1.Outcomes.Add(DiplomaticOutcome1);
    
    DiplomaticTalk.DialogueOptions.Add(DiplomaticOption1);

    AllDialogues.Add(DiplomaticTalk);
}

void UMingGoRTSDialogueSystem::CreatePersonalStoryDialogues()
{
    // 個人故事對話
    FMingDialogue PersonalStory;
    PersonalStory.DialogueID = GenerateDialogueID(TEXT("PersonalStory_Family"), TEXT("FamilyMember"));
    PersonalStory.SpeakerID = TEXT("FamilyMember");
    PersonalStory.DialogueText = TEXT("孩子，你在軍中的生活還好嗎？家鄉的親人們都很想念你。希望你平安歸來。");
    PersonalStory.DialogueType = EDialogueType::PersonalStory;
    PersonalStory.SpeakerPortrait = TEXT("/Game/Portraits/FamilyMember.FamilyMember");

    FDialogueOption PersonalOption1;
    PersonalOption1.OptionText = TEXT("我很好，請家人放心。");
    PersonalOption1.Attitude = EDialogueAttitude::Friendly;
    PersonalOption1.NextDialogueID = GenerateDialogueID(TEXT("PersonalStory_Reassuring"), TEXT("FamilyMember"));
    
    FDialogueOutcome PersonalOutcome1;
    PersonalOutcome1.OutcomeType = EDialogueOutcome::RelationshipChange;
    PersonalOutcome1.TargetID = TEXT("Family");
    PersonalOutcome1.Value = 15.0f;
    PersonalOutcome1.Description = TEXT("家庭關係改善");
    PersonalOption1.Outcomes.Add(PersonalOutcome1);
    
    PersonalStory.DialogueOptions.Add(PersonalOption1);

    AllDialogues.Add(PersonalStory);
}

void UMingGoRTSDialogueSystem::CreateQuestDialogues()
{
    // 任務對話
    FMingDialogue QuestDialogue;
    QuestDialogue.DialogueID = GenerateDialogueID(TEXT("Quest_RescueMission"), TEXT("QuestGiver"));
    QuestDialogue.SpeakerID = TEXT("QuestGiver");
    QuestDialogue.DialogueText = TEXT("將軍，我們的村莊遭到了土匪的襲擊，許多村民被挾為人質。懇請您出手相救！");
    QuestDialogue.DialogueType = EDialogueType::QuestDialogue;
    QuestDialogue.SpeakerPortrait = TEXT("/Game/Portraits/Villager.Villager");

    FDialogueOption QuestOption1;
    QuestOption1.OptionText = TEXT("我會立即派人營救！");
    QuestOption1.Attitude = EDialogueAttitude::Friendly;
    QuestOption1.NextDialogueID = GenerateDialogueID(TEXT("Quest_RescueMission_Accepted"), TEXT("QuestGiver"));
    
    FDialogueOutcome QuestOutcome1;
    QuestOutcome1.OutcomeType = EDialogueOutcome::QuestTrigger;
    QuestOutcome1.TargetID = TEXT("RescueMission");
    QuestOutcome1.Value = 1.0f;
    QuestOutcome1.Description = TEXT("觸發營救任務");
    QuestOption1.Outcomes.Add(QuestOutcome1);
    
    QuestDialogue.DialogueOptions.Add(QuestOption1);

    AllDialogues.Add(QuestDialogue);
}

bool UMingGoRTSDialogueSystem::ValidateDialogueConditions(const TArray<FDialogueCondition>& Conditions, const FMingCharacterData& PlayerCharacter) const
{
    for (const FDialogueCondition& Condition : Conditions)
    {
        if (!ValidateSingleCondition(Condition, PlayerCharacter))
        {
            return false;
        }
    }
    return true;
}

bool UMingGoRTSDialogueSystem::ValidateSingleCondition(const FDialogueCondition& Condition, const FMingCharacterData& PlayerCharacter) const
{
    if (Condition.ConditionType == TEXT("Attribute"))
    {
        float AttributeValue = GetPlayerAttribute(Condition.Parameter);
        float RequiredValue = FCString::Atof(*Condition.Value);
        
        if (Condition.Operator == TEXT(">="))
        {
            return AttributeValue >= RequiredValue;
        }
        else if (Condition.Operator == TEXT("<="))
        {
            return AttributeValue <= RequiredValue;
        }
        else if (Condition.Operator == TEXT("=="))
        {
            return AttributeValue == RequiredValue;
        }
        else if (Condition.Operator == TEXT(">"))
        {
            return AttributeValue > RequiredValue;
        }
        else if (Condition.Operator == TEXT("<"))
        {
            return AttributeValue < RequiredValue;
        }
    }
    else if (Condition.ConditionType == TEXT("Relationship"))
    {
        float RelationshipValue = GetCharacterRelationship(Condition.Parameter);
        float RequiredValue = FCString::Atof(*Condition.Value);
        
        if (Condition.Operator == TEXT(">="))
        {
            return RelationshipValue >= RequiredValue;
        }
    }
    else if (Condition.ConditionType == TEXT("SkillLevel"))
    {
        int32 SkillLevel = GetSkillLevel(FName(*Condition.Parameter));
        int32 RequiredLevel = FCString::Atoi(*Condition.Value);
        
        if (Condition.Operator == TEXT(">="))
        {
            return SkillLevel >= RequiredLevel;
        }
    }
    else if (Condition.ConditionType == TEXT("QuestCompleted"))
    {
        bool bQuestCompleted = IsQuestCompleted(Condition.Parameter);
        bool bRequired = Condition.Value.ToBool();
        
        return bQuestCompleted == bRequired;
    }
    else if (Condition.ConditionType == TEXT("CharacterKnown"))
    {
        bool bCharacterKnown = IsCharacterKnown(Condition.Parameter);
        bool bRequired = Condition.Value.ToBool();
        
        return bCharacterKnown == bRequired;
    }

    return true; // 默認通過未知條件
}

void UMingGoRTSDialogueSystem::ExecuteDialogueOutcomes(const TArray<FDialogueOutcome>& Outcomes)
{
    ApplyDialogueOutcomes(Outcomes);
}

void UMingGoRTSDialogueSystem::UpdateCharacterRelationship(const FString& CharacterID, float RelationshipChange)
{
    // TODO: 實現角色關係更新
    UE_LOG(LogTemp, Log, TEXT("更新角色 %s 關係：%f"), *CharacterID, RelationshipChange);
}

void UMingGoRTSDialogueSystem::TriggerQuest(const FString& QuestID)
{
    // TODO: 實現任務觸發
    UE_LOG(LogTemp, Log, TEXT("觸發任務：%s"), *QuestID);
}

void UMingGoRTSDialogueSystem::GrantInformation(const FString& InformationID)
{
    // TODO: 實現情報給予
    UE_LOG(LogTemp, Log, TEXT("給予情報：%s"), *InformationID);
}

void UMingGoRTSDialogueSystem::ImproveSkill(const FName& SkillID, float ImprovementAmount)
{
    // TODO: 實現技能提升
    UE_LOG(LogTemp, Log, TEXT("提升技能 %s：%f"), *SkillID.ToString(), ImprovementAmount);
}

void UMingGoRTSDialogueSystem::TriggerEvent(const FString& EventID)
{
    // TODO: 實現事件觸發
    UE_LOG(LogTemp, Log, TEXT("觸發事件：%s"), *EventID);
}

FString UMingGoRTSDialogueSystem::GenerateDialogueID(const FString& BaseName, const FString& CharacterID) const
{
    return FString::Printf(TEXT("%s_%s"), *CharacterID, *BaseName);
}

float UMingGoRTSDialogueSystem::GetCharacterRelationship(const FString& CharacterID) const
{
    // TODO: 實現角色關係獲取
    return 50.0f; // 默認中立關係
}

bool UMingGoRTSDialogueSystem::IsCharacterKnown(const FString& CharacterID) const
{
    // TODO: 實現角色已知狀態檢查
    return DialogueHistory.Num() > 0; // 簡化實現
}

float UMingGoRTSDialogueSystem::GetPlayerAttribute(const FString& AttributeName) const
{
    if (AttributeName == TEXT("Leadership"))
    {
        return CurrentPlayerCharacter.Attributes.Leadership;
    }
    else if (AttributeName == TEXT("Intelligence"))
    {
        return CurrentPlayerCharacter.Attributes.Intelligence;
    }
    else if (AttributeName == TEXT("Courage"))
    {
        return CurrentPlayerCharacter.Attributes.Courage;
    }
    else if (AttributeName == TEXT("Charisma"))
    {
        return CurrentPlayerCharacter.Attributes.Charisma;
    }
    else if (AttributeName == TEXT("Constitution"))
    {
        return CurrentPlayerCharacter.Attributes.Constitution;
    }
    
    return 50.0f; // 默認值
}

bool UMingGoRTSDialogueSystem::IsQuestCompleted(const FString& QuestID) const
{
    // TODO: 實現任務狀態檢查
    return false; // 默認未完成
}

int32 UMingGoRTSDialogueSystem::GetSkillLevel(const FName& SkillID) const
{
    // TODO: 實現技能等級獲取
    return 1; // 默認等級
}
