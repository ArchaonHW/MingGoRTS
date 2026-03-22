#include "MingDialogueSystem.h"
#include "Engine/World.h"

UMingDialogueSystem::UMingDialogueSystem()
{
    WorldContext = GetWorld();
    bIsConversationActive = false;
    bIsPaused = false;
    
    InitializeDefaultDialogues();
}

void UMingDialogueSystem::InitializeDefaultDialogues()
{
    Conversations.Empty();
    
    SetupHistoricalDialogues();
    SetupTutorialDialogues();
    
    UE_LOG(LogTemp, Log, TEXT("Dialogue system initialized with %d conversations"), Conversations.Num());
}

void UMingDialogueSystem::SetupHistoricalDialogues()
{
    // 1911 Revolution Historical Dialogue
    FMingDialogueConversation RevolutionDialogue;
    RevolutionDialogue.ConversationID = TEXT("REVOLUTION_INTRO");
    RevolutionDialogue.ConversationName = TEXT("革命起義");
    RevolutionDialogue.Description = TEXT("1911年辛亥革命的歷史背景");
    RevolutionDialogue.DialogueType = EMingDialogueType::Historical;
    RevolutionDialogue.StartDialogueID = TEXT("REV_INTRO_1");
    RevolutionDialogue.bRepeatable = false;
    
    // Opening historical context
    FMingDialogueLine IntroLine;
    IntroLine.DialogueID = TEXT("REV_INTRO_1");
    IntroLine.SpeakerName = TEXT("孫中山");
    IntroLine.SpeakerType = EMingDialogueSpeaker::Narrator;
    IntroLine.DialogueText = TEXT("1911年，清朝統治已近三百年。腐敗的政府和外國的侵略，讓中國處於危亡之際。我們必須推翻滿清，建立共和！");
    IntroLine.DisplayDuration = 5.0f;
    IntroLine.AudioPath = TEXT("Audio/SunYatSen_Revolution.wav");
    IntroLine.AutoNextDialogueID = TEXT("REV_INTRO_2");
    
    // Add historical note
    TArray<FString> Notes;
    Notes.Add(TEXT("孫中山 (1866-1925)，中國近代民主革命家"));
    Notes.Add(TEXT("1911年10月10日武昌起義，標誌著辛亥革命的開始"));
    HistoricalNotes.Add(IntroLine.DialogueID, Notes);
    
    RevolutionDialogue.DialogueLines.Add(IntroLine.DialogueID, IntroLine);
    
    // Player choice dialogue
    FMingDialogueLine ChoiceLine;
    ChoiceLine.DialogueID = TEXT("REV_INTRO_2");
    ChoiceLine.SpeakerName = TEXT("革命軍官");
    ChoiceLine.SpeakerType = EMingDialogueSpeaker::NPC;
    ChoiceLine.DialogueText = TEXT("作為革命軍的指揮官，你認為我們應該如何開始這場革命？");
    ChoiceLine.DisplayDuration = 4.0f;
    ChoiceLine.bIsChoice = true;
    
    FMingDialogueOption Option1;
    Option1.OptionText = TEXT("從武昌開始，奪取南方重鎮");
    Option1.NextDialogueID = TEXT("REV_CHOICE_WUCHANG");
    Option1.Consequences.Add(TEXT("SetCondition('Path_Wuchang', true)"));
    Option1.Priority = 1;
    
    FMingDialogueOption Option2;
    Option2.OptionText = TEXT("直接進攻北京，直搗黃龍");
    Option2.NextDialogueID = TEXT("REV_CHOICE_BEIJING");
    Option2.Consequences.Add(TEXT("SetCondition('Path_Beijing', true)"));
    Option2.Priority = 1;
    
    FMingDialogueOption Option3;
    Option3.OptionText = TEXT("先鞏固南方，再圖北伐");
    Option3.NextDialogueID = TEXT("REV_CHOICE_SOUTH");
    Option3.Consequences.Add(TEXT("SetCondition('Path_South', true)"));
    Option3.Priority = 1;
    
    ChoiceLine.Options.Add(Option1);
    ChoiceLine.Options.Add(Option2);
    ChoiceLine.Options.Add(Option3);
    
    RevolutionDialogue.DialogueLines.Add(ChoiceLine.DialogueID, ChoiceLine);
    
    // Wuchang path response
    FMingDialogueLine WuchangResponse;
    WuchangResponse.DialogueID = TEXT("REV_CHOICE_WUCHANG");
    WuchangResponse.SpeakerName = TEXT("孫中山");
    WuchangResponse.SpeakerType = EMingDialogueSpeaker::Narrator;
    WuchangResponse.DialogueText = TEXT("明智的選擇！武昌是長江重鎮，控制它就能切斷清軍的南北聯繫。從這裡開始，我們可以逐步向北推進。");
    WuchangResponse.DisplayDuration = 4.0f;
    WuchangResponse.TriggerEvents.Add(TEXT("StartMission('REV_WUCHANG')"));
    
    RevolutionDialogue.DialogueLines.Add(WuchangResponse.DialogueID, WuchangResponse);
    
    // Beijing path response
    FMingDialogueLine BeijingResponse;
    BeijingResponse.DialogueID = TEXT("REV_CHOICE_BEIJING");
    BeijingResponse.SpeakerName = TEXT("孫中山");
    BeijingResponse.SpeakerType = EMingDialogueSpeaker::Narrator;
    BeijingResponse.DialogueText = TEXT("大膽的計劃！直接攻擊北京雖然風險很大，但如果成功，將迅速結束清朝統治。這需要精銳部隊和周密計劃。");
    BeijingResponse.DisplayDuration = 4.0f;
    BeijingResponse.TriggerEvents.Add(TEXT("StartMission('REV_BEIJING')"));
    
    RevolutionDialogue.DialogueLines.Add(BeijingResponse.DialogueID, BeijingResponse);
    
    Conversations.Add(RevolutionDialogue.ConversationID, RevolutionDialogue);
    
    // Northern Expedition Dialogue
    FMingDialogueConversation NorthernExpeditionDialogue;
    NorthernExpeditionDialogue.ConversationID = TEXT("NORTH_EXPEDITION");
    NorthernExpeditionDialogue.ConversationName = TEXT("北伐戰爭");
    NorthernExpeditionDialogue.Description = TEXT("1926年北伐戰爭的戰略討論");
    NorthernExpeditionDialogue.DialogueType = EMingDialogueType::Briefing;
    NorthernExpeditionDialogue.StartDialogueID = TEXT("NORTH_BRIEF_1");
    
    FMingDialogueLine BriefingLine;
    BriefingLine.DialogueID = TEXT("NORTH_BRIEF_1");
    BriefingLine.SpeakerName = TEXT("蔣介石");
    BriefingLine.SpeakerType = EMingDialogueSpeaker::NPC;
    BriefingLine.DialogueText = TEXT("同志們！北伐戰爭即將開始。我們的目標是統一中國，消滅軍閥勢力。這是一場關乎國家命運的戰爭！");
    BriefingLine.DisplayDuration = 4.0f;
    BriefingLine.AutoNextDialogueID = TEXT("NORTH_BRIEF_2");
    
    NorthernExpeditionDialogue.DialogueLines.Add(BriefingLine.DialogueID, BriefingLine);
    
    FMingDialogueLine StrategyLine;
    StrategyLine.DialogueID = TEXT("NORTH_BRIEF_2");
    StrategyLine.SpeakerName = TEXT("蔣介石");
    StrategyLine.SpeakerType = EMingDialogueSpeaker::NPC;
    StrategyLine.DialogueText = TEXT("我們將分三路北伐：東路從廣州出發，中路從湖南北上，西路從四川東進。每一路都有其戰略意義。");
    StrategyLine.DisplayDuration = 4.0f;
    StrategyLine.bIsChoice = true;
    
    FMingDialogueOption EastOption;
    EastOption.OptionText = TEXT("我負責東路軍，從廣州開始");
    EastOption.NextDialogueID = TEXT("NORTH_EAST_PATH");
    EastOption.Consequences.Add(TEXT("SetCondition('NorthPath_East', true)"));
    
    FMingDialogueOption CentralOption;
    CentralOption.OptionText = TEXT("我負責中路軍，從湖南北上");
    CentralOption.NextDialogueID = TEXT("NORTH_CENTRAL_PATH");
    CentralOption.Consequences.Add(TEXT("SetCondition('NorthPath_Central', true)"));
    
    FMingDialogueOption WestOption;
    WestOption.OptionText = TEXT("我負責西路軍，從四川東進");
    WestOption.NextDialogueID = TEXT("NORTH_WEST_PATH");
    WestOption.Consequences.Add(TEXT("SetCondition('NorthPath_West', true)"));
    
    StrategyLine.Options.Add(EastOption);
    StrategyLine.Options.Add(CentralOption);
    StrategyLine.Options.Add(WestOption);
    
    NorthernExpeditionDialogue.DialogueLines.Add(StrategyLine.DialogueID, StrategyLine);
    
    Conversations.Add(NorthernExpeditionDialogue.ConversationID, NorthernExpeditionDialogue);
}

void UMingDialogueSystem::SetupTutorialDialogues()
{
    FMingDialogueConversation TutorialDialogue;
    TutorialDialogue.ConversationID = TEXT("TUTORIAL_INTRO");
    TutorialDialogue.ConversationName = TEXT("歡迎教學");
    TutorialDialogue.Description = TEXT("遊戲基本操作教學");
    TutorialDialogue.DialogueType = EMingDialogueType::Tutorial;
    TutorialDialogue.StartDialogueID = TEXT("TUT_WELCOME");
    TutorialDialogue.bRepeatable = true;
    
    FMingDialogueLine WelcomeLine;
    WelcomeLine.DialogueID = TEXT("TUT_WELCOME");
    WelcomeLine.SpeakerName = TEXT("系統");
    WelcomeLine.SpeakerType = EMingDialogueSpeaker::System;
    WelcomeLine.DialogueText = TEXT("歡迎來到民國史詩！這是一款以民國時期為背景的即時戰略遊戲。讓我來教你基本操作。");
    WelcomeLine.DisplayDuration = 3.0f;
    WelcomeLine.AutoNextDialogueID = TEXT("TUT_CONTROLS");
    
    TutorialDialogue.DialogueLines.Add(WelcomeLine.DialogueID, WelcomeLine);
    
    FMingDialogueLine ControlsLine;
    ControlsLine.DialogueID = TEXT("TUT_CONTROLS");
    ControlsLine.SpeakerName = TEXT("系統");
    ControlsLine.SpeakerType = EMingDialogueSpeaker::System;
    ControlsLine.DialogueText = TEXT("使用滑鼠左鍵選擇單位，拖動可框選多個單位。右鍵點擊地面移動單位，右鍵點擊敵人進行攻擊。");
    ControlsLine.DisplayDuration = 4.0f;
    ControlsLine.AutoNextDialogueID = TEXT("TUT_RESOURCES");
    
    TutorialDialogue.DialogueLines.Add(ControlsLine.DialogueID, ControlsLine);
    
    FMingDialogueLine ResourcesLine;
    ResourcesLine.DialogueID = TEXT("TUT_RESOURCES");
    ResourcesLine.SpeakerName = TEXT("系統");
    ResourcesLine.SpeakerType = EMingDialogueSpeaker::System;
    ResourcesLine.DialogueText = TEXT("注意右上角的資源顯示。你需要管理資金、材料、食物等資源來建造建築和訓練單位。");
    ResourcesLine.DisplayDuration = 4.0f;
    ResourcesLine.AutoNextDialogueID = TEXT("TUT_BUILDINGS");
    
    TutorialDialogue.DialogueLines.Add(ResourcesLine.DialogueID, ResourcesLine);
    
    FMingDialogueLine BuildingsLine;
    BuildingsLine.DialogueID = TEXT("TUT_BUILDINGS");
    BuildingsLine.SpeakerName = TEXT("系統");
    BuildingsLine.SpeakerType = EMingDialogueSpeaker::System;
    BuildingsLine.DialogueText = TEXT("建造兵營可以訓練步兵，建造工廠可以生產裝甲單位。記住，不同的建築有不同的功能！");
    BuildingsLine.DisplayDuration = 4.0f;
    
    TutorialDialogue.DialogueLines.Add(BuildingsLine.DialogueID, BuildingsLine);
    
    Conversations.Add(TutorialDialogue.ConversationID, TutorialDialogue);
}

void UMingDialogueSystem::StartConversation(const FString& ConversationID)
{
    FMingDialogueConversation* Conversation = FindConversation(ConversationID);
    if (!Conversation)
    {
        UE_LOG(LogTemp, Warning, TEXT("Conversation not found: %s"), *ConversationID);
        return;
    }
    
    // Check if conversation can be started
    if (!CheckDialogueConditions(Conversation->RequiredConditions))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start conversation - conditions not met: %s"), *ConversationID);
        return;
    }
    
    CurrentConversationID = ConversationID;
    CurrentDialogueID = Conversation->StartDialogueID;
    bIsConversationActive = true;
    bIsPaused = false;
    
    // Start with the first dialogue line
    FMingDialogueLine* StartLine = FindDialogueLine(CurrentDialogueID);
    if (StartLine)
    {
        OnDialogueStarted.Broadcast(*StartLine, ConversationID);
        ProcessDialogueTriggers(*StartLine);
        
        UE_LOG(LogTemp, Log, TEXT("Conversation started: %s"), *Conversation->ConversationName);
    }
}

void UMingDialogueSystem::EndConversation()
{
    if (!bIsConversationActive)
    {
        return;
    }
    
    FMingDialogueConversation* Conversation = FindConversation(CurrentConversationID);
    bool bWasCompleted = false;
    
    if (Conversation)
    {
        Conversation->bHasBeenCompleted = true;
        bWasCompleted = true;
    }
    
    OnDialogueEnded.Broadcast(CurrentConversationID, bWasCompleted);
    
    bIsConversationActive = false;
    bIsPaused = false;
    CurrentConversationID.Empty();
    CurrentDialogueID.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Conversation ended"));
}

void UMingDialogueSystem::PauseConversation()
{
    bIsPaused = true;
    UE_LOG(LogTemp, Log, TEXT("Conversation paused"));
}

void UMingDialogueSystem::ResumeConversation()
{
    bIsPaused = false;
    UE_LOG(LogTemp, Log, TEXT("Conversation resumed"));
}

bool UMingDialogueSystem::IsConversationActive() const
{
    return bIsConversationActive;
}

FString UMingDialogueSystem::GetCurrentConversationID() const
{
    return CurrentConversationID;
}

void UMingDialogueSystem::SelectDialogueOption(int32 OptionIndex)
{
    if (!bIsConversationActive || bIsPaused)
    {
        return;
    }
    
    FMingDialogueLine* CurrentLine = FindDialogueLine(CurrentDialogueID);
    if (!CurrentLine || !CurrentLine->bIsChoice)
    {
        return;
    }
    
    if (OptionIndex < 0 || OptionIndex >= CurrentLine->Options.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid option index: %d"), OptionIndex);
        return;
    }
    
    FMingDialogueOption& SelectedOption = CurrentLine->Options[OptionIndex];
    
    // Record player choice
    RecordPlayerChoice(CurrentDialogueID, OptionIndex);
    
    // Process consequences
    ProcessChoiceConsequences(SelectedOption);
    
    // Trigger event
    OnDialogueOptionSelected.Broadcast(OptionIndex, CurrentDialogueID);
    
    // Move to next dialogue
    if (!SelectedOption.NextDialogueID.IsEmpty())
    {
        GoToDialogue(SelectedOption.NextDialogueID);
    }
    else
    {
        EndConversation();
    }
}

void UMingDialogueSystem::NextDialogue()
{
    if (!bIsConversationActive || bIsPaused)
    {
        return;
    }
    
    FMingDialogueLine* CurrentLine = FindDialogueLine(CurrentDialogueID);
    if (!CurrentLine)
    {
        return;
    }
    
    // If it's a choice dialogue, don't auto-advance
    if (CurrentLine->bIsChoice)
    {
        return;
    }
    
    // Move to auto-next dialogue
    if (!CurrentLine->AutoNextDialogueID.IsEmpty())
    {
        GoToDialogue(CurrentLine->AutoNextDialogueID);
    }
    else
    {
        EndConversation();
    }
}

void UMingDialogueSystem::SkipDialogue()
{
    FMingDialogueLine* CurrentLine = FindDialogueLine(CurrentDialogueID);
    if (CurrentLine && CurrentLine->bSkippable)
    {
        NextDialogue();
    }
}

void UMingDialogueSystem::GoToDialogue(const FString& DialogueID)
{
    FMingDialogueLine* NextLine = FindDialogueLine(DialogueID);
    if (!NextLine)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue not found: %s"), *DialogueID);
        return;
    }
    
    CurrentDialogueID = DialogueID;
    
    // Update options based on conditions
    UpdateDialogueOptions();
    
    // Process triggers
    ProcessDialogueTriggers(*NextLine);
    
    // Auto-advance if not a choice
    if (!NextLine->bIsChoice && !NextLine->AutoNextDialogueID.IsEmpty())
    {
        AutoAdvanceDialogue();
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Advanced to dialogue: %s"), *DialogueID);
}

FMingDialogueLine UMingDialogueSystem::GetCurrentDialogueLine() const
{
    const FMingDialogueLine* Line = FindDialogueLine(CurrentDialogueID);
    return Line ? *Line : FMingDialogueLine();
}

TArray<FMingDialogueOption> UMingDialogueSystem::GetCurrentOptions() const
{
    FMingDialogueLine* CurrentLine = FindDialogueLine(CurrentDialogueID);
    return CurrentLine ? CurrentLine->Options : TArray<FMingDialogueOption>();
}

bool UMingDialogueSystem::HasCurrentDialogue() const
{
    return !CurrentDialogueID.IsEmpty() && FindDialogueLine(CurrentDialogueID) != nullptr;
}

void UMingDialogueSystem::LoadConversation(const FMingDialogueConversation& Conversation)
{
    Conversations.Add(Conversation.ConversationID, Conversation);
    UE_LOG(LogTemp, Log, TEXT("Loaded conversation: %s"), *Conversation.ConversationName);
}

void UMingDialogueSystem::LoadDialogueFromFile(const FString& FilePath)
{
    // This would load from JSON file
    UE_LOG(LogTemp, Log, TEXT("Loading dialogue from file: %s"), *FilePath);
}

void UMingDialogueSystem::AddDialogueLine(const FString& ConversationID, const FMingDialogueLine& DialogueLine)
{
    FMingDialogueConversation* Conversation = FindConversation(ConversationID);
    if (Conversation)
    {
        Conversation->DialogueLines.Add(DialogueLine.DialogueID, DialogueLine);
    }
}

TArray<FMingDialogueConversation> UMingDialogueSystem::GetAllConversations() const
{
    TArray<FMingDialogueConversation> Result;
    for (const auto& Pair : Conversations)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FMingDialogueConversation> UMingDialogueSystem::GetAvailableConversations() const
{
    TArray<FMingDialogueConversation> Result;
    for (const auto& Pair : Conversations)
    {
        const FMingDialogueConversation& Conversation = Pair.Value;
        
        if (CheckDialogueConditions(Conversation.RequiredConditions))
        {
            if (Conversation.bRepeatable || !Conversation.bHasBeenCompleted)
            {
                Result.Add(Conversation);
            }
        }
    }
    return Result;
}

void UMingDialogueSystem::ShowHistoricalContext(const FString& EventID)
{
    // Show historical context for an event
    OnHistoricalContextShown.Broadcast(EventID);
    
    UE_LOG(LogTemp, Log, TEXT("Showing historical context: %s"), *EventID);
}

void UMingDialogueSystem::AddHistoricalNote(const FString& DialogueID, const FString& Note)
{
    if (!HistoricalNotes.Contains(DialogueID))
    {
        HistoricalNotes.Add(DialogueID, TArray<FString>());
    }
    
    HistoricalNotes[DialogueID].Add(Note);
}

TArray<FString> UMingDialogueSystem::GetHistoricalNotes(const FString& DialogueID) const
{
    const TArray<FString>* Notes = HistoricalNotes.Find(DialogueID);
    return Notes ? *Notes : TArray<FString>();
}

void UMingDialogueSystem::ProcessChoice(const FString& ChoiceID)
{
    // Process a specific choice
    UE_LOG(LogTemp, Log, TEXT("Processing choice: %s"), *ChoiceID);
}

void UMingDialogueSystem::RecordPlayerChoice(const FString& DialogueID, int32 OptionIndex)
{
    PlayerChoices.Add(DialogueID, OptionIndex);
    UE_LOG(LogTemp, Verbose, TEXT("Recorded choice: %s - Option %d"), *DialogueID, OptionIndex);
}

TMap<FString, int32> UMingDialogueSystem::GetPlayerChoices() const
{
    return PlayerChoices;
}

TArray<FString> UMingDialogueSystem::GetChoiceConsequences(const FString& DialogueID, int32 OptionIndex) const
{
    const FMingDialogueLine* Line = FindDialogueLine(DialogueID);
    if (Line && OptionIndex >= 0 && OptionIndex < Line->Options.Num())
    {
        return Line->Options[OptionIndex].Consequences;
    }
    return TArray<FString>();
}

void UMingDialogueSystem::SetDialogueCondition(const FString& ConditionName, bool bValue)
{
    DialogueConditions.Add(ConditionName, bValue);
    UE_LOG(LogTemp, Verbose, TEXT("Set condition %s to %s"), *ConditionName, bValue ? TEXT("true") : TEXT("false"));
}

bool UMingDialogueSystem::GetDialogueCondition(const FString& ConditionName) const
{
    const bool* Value = DialogueConditions.Find(ConditionName);
    return Value ? *Value : false;
}

void UMingDialogueSystem::TriggerDialogueEvent(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Dialogue event triggered: %s"), *EventName);
    
    // Process event consequences
    if (EventName.StartsWith(TEXT("StartMission(")))
    {
        // Extract mission ID
        FString MissionID = EventName;
        MissionID.RemoveFromStart(TEXT("StartMission("));
        MissionID.RemoveFromEnd(TEXT(")"));
        
        // This would trigger mission start
        UE_LOG(LogTemp, Log, TEXT("Would start mission: %s"), *MissionID);
    }
}

FString UMingDialogueSystem::GetSpeakerTypeName(EMingDialogueSpeaker SpeakerType)
{
    switch (SpeakerType)
    {
    case EMingDialogueSpeaker::Player: return TEXT("玩家");
    case EMingDialogueSpeaker::NPC: return TEXT("NPC");
    case EMingDialogueSpeaker::System: return TEXT("系統");
    case EMingDialogueSpeaker::Narrator: return TEXT("旁白");
    default: return TEXT("未知");
    }
}

FString UMingDialogueSystem::GetDialogueTypeName(EMingDialogueType DialogueType)
{
    switch (DialogueType)
    {
    case EMingDialogueType::Conversation: return TEXT("對話");
    case EMingDialogueType::Briefing: return TEXT("簡報");
    case EMingDialogueType::Debriefing: return TEXT("任務報告");
    case EMingDialogueType::Historical: return TEXT("歷史");
    case EMingDialogueType::Choice: return TEXT("選擇");
    case EMingDialogueType::Monologue: return TEXT("獨白");
    default: return TEXT("未知");
    }
}

FString UMingDialogueSystem::SaveDialogueData() const
{
    FString Result = TEXT("{\n");
    Result += TEXT("  \"choices\": {\n");
    
    bool First = true;
    for (const auto& Pair : PlayerChoices)
    {
        if (!First) Result += TEXT(",\n");
        Result += FString::Printf(TEXT("    \"%s\": %d"), *Pair.Key, Pair.Value);
        First = false;
    }
    
    Result += TEXT("\n  },\n");
    Result += TEXT("  \"conditions\": {\n");
    
    First = true;
    for (const auto& Pair : DialogueConditions)
    {
        if (!First) Result += TEXT(",\n");
        Result += FString::Printf(TEXT("    \"%s\": %s"), *Pair.Key, Pair.Value ? TEXT("true") : TEXT("false"));
        First = false;
    }
    
    Result += TEXT("\n  }\n");
    Result += TEXT("}\n");
    
    return Result;
}

void UMingDialogueSystem::LoadDialogueData(const FString& JsonString)
{
    // Parse JSON and restore dialogue data
    UE_LOG(LogTemp, Log, TEXT("Loading dialogue data"));
}

FMingDialogueLine* UMingDialogueSystem::FindDialogueLine(const FString& DialogueID)
{
    if (CurrentConversationID.IsEmpty())
    {
        return nullptr;
    }
    
    FMingDialogueConversation* Conversation = FindConversation(CurrentConversationID);
    if (Conversation)
    {
        return Conversation->DialogueLines.Find(DialogueID);
    }
    
    return nullptr;
}

FMingDialogueConversation* UMingDialogueSystem::FindConversation(const FString& ConversationID)
{
    return Conversations.Find(ConversationID);
}

void UMingDialogueSystem::ProcessDialogueTriggers(const FMingDialogueLine& DialogueLine)
{
    for (const FString& Event : DialogueLine.TriggerEvents)
    {
        TriggerDialogueEvent(Event);
    }
}

void UMingDialogueSystem::ProcessChoiceConsequences(const FMingDialogueOption& Option)
{
    for (const FString& Consequence : Option.Consequences)
    {
        if (Consequence.StartsWith(TEXT("SetCondition(")))
        {
            // Parse condition setting
            FString ConditionStr = Consequence;
            ConditionStr.RemoveFromStart(TEXT("SetCondition("));
            ConditionStr.RemoveFromEnd(TEXT(")"));
            
            TArray<FString> Parts;
            ConditionStr.ParseIntoArray(Parts, TEXT(","));
            
            if (Parts.Num() == 2)
            {
                FString ConditionName = Parts[0].TrimQuotes();
                FString ValueStr = Parts[1].TrimQuotes();
                bool bValue = ValueStr.ToBool();
                
                SetDialogueCondition(ConditionName, bValue);
            }
        }
    }
}

bool UMingDialogueSystem::CheckDialogueConditions(const TArray<FString>& Conditions) const
{
    for (const FString& Condition : Conditions)
    {
        if (!GetDialogueCondition(Condition))
        {
            return false;
        }
    }
    return true;
}

void UMingDialogueSystem::UpdateDialogueOptions()
{
    FMingDialogueLine* CurrentLine = FindDialogueLine(CurrentDialogueID);
    if (!CurrentLine)
    {
        return;
    }
    
    // Update option availability based on conditions
    for (FMingDialogueOption& Option : CurrentLine->Options)
    {
        Option.bIsEnabled = CheckDialogueConditions(Option.RequiredConditions);
    }
}

void UMingDialogueSystem::AutoAdvanceDialogue()
{
    // This would be called by a timer to auto-advance dialogue
    // For now, just log
    UE_LOG(LogTemp, Verbose, TEXT("Auto-advance dialogue scheduled"));
}
