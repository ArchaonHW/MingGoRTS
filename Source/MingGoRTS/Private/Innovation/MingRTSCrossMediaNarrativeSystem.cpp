#include "MingRTSCrossMediaNarrativeSystem.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSCrossMediaNarrativeSystem::UMingRTSCrossMediaNarrativeSystem()
{
    PersonalizationLevel = 0.7f;
    BranchingComplexity = 0.6f;
    MediaDiversity = 0.8f;
    MaxConcurrentThreads = 5;
}

void UMingRTSCrossMediaNarrativeSystem::InitializeCrossMediaSystem()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Initializing cross media narrative system..."));

    // 初始化媒體元素庫
    InitializeMediaElementLibrary();

    // 初始化內容生成器
    InitializeContentGenerators();

    // 初始化敘事模板
    InitializeNarrativeTemplates();

    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Cross media narrative system initialized successfully"));
}

FCrossMediaExperience UMingRTSCrossMediaNarrativeSystem::CreateCrossMediaExperience(const FString& ExperienceID, const FString& ExperienceName)
{
    FCrossMediaExperience NewExperience;
    NewExperience.ExperienceID = ExperienceID;
    NewExperience.ExperienceName = ExperienceName;
    NewExperience.Description = FString::Printf(TEXT("Cross media experience: %s"), *ExperienceName);
    NewExperience.bIsPersonalized = true;

    // 初始化媒體元素映射
    for (int32 i = 0; i < 8; i++)
    {
        EMediaType MediaType = (EMediaType)i;
        TArray<FNarrativeElement> EmptyElements;
        NewExperience.MediaElements.Add(MediaType, EmptyElements);
    }

    CrossMediaExperiences.Add(ExperienceID, NewExperience);

    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Created cross media experience %s"), *ExperienceName);
    return NewExperience;
}

FNarrativeThread UMingRTSCrossMediaNarrativeSystem::AddNarrativeThread(const FString& ExperienceID, ENarrativeType NarrativeType, const FString& ThreadName)
{
    FNarrativeThread NewThread;
    NewThread.ThreadID = FString::Printf(TEXT("Thread_%s_%d"), *ExperienceID, FMath::RandRange(1000, 9999));
    NewThread.ThreadName = ThreadName;
    NewThread.NarrativeType = NarrativeType;
    NewThread.ThreadPriority = CalculateThreadPriority(NarrativeType);

    // 根據敘事類型設置默認參數
    switch (NarrativeType)
    {
        case ENarrativeType::MainStory:
            NewThread.ThreadPriority = 1.0f;
            break;
        case ENarrativeType::SideStory:
            NewThread.ThreadPriority = 0.7f;
            break;
        case ENarrativeType::CharacterStory:
            NewThread.ThreadPriority = 0.8f;
            break;
        case ENarrativeType::WorldStory:
            NewThread.ThreadPriority = 0.6f;
            break;
        case ENarrativeType::PersonalStory:
            NewThread.ThreadPriority = 0.9f;
            NewThread.bIsPersonalized = true;
            break;
    }

    if (CrossMediaExperiences.Contains(ExperienceID))
    {
        CrossMediaExperiences[ExperienceID].NarrativeThreads.Add(NewThread);
        
        // 如果是主線故事，設置為活躍線程
        if (NarrativeType == ENarrativeType::MainStory)
        {
            CrossMediaExperiences[ExperienceID].ActiveThreads.Add(NewThread.ThreadID);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Added narrative thread %s to experience %s"), *ThreadName, *ExperienceID);
    return NewThread;
}

FNarrativeChapter UMingRTSCrossMediaNarrativeSystem::CreateNarrativeChapter(const FString& ThreadID, const FString& ChapterTitle, ENarrativeStyle Style)
{
    FNarrativeChapter NewChapter;
    NewChapter.ChapterID = FString::Printf(TEXT("Chapter_%s_%d"), *ThreadID, FMath::RandRange(1000, 9999));
    NewChapter.ChapterTitle = ChapterTitle;
    NewChapter.ChapterDescription = FString::Printf(TEXT("Chapter: %s"), *ChapterTitle);
    NewChapter.NarrativeStyle = Style;
    NewChapter.ChapterWeight = CalculateChapterWeight(Style);

    // 根據敘事風格設置章節特性
    switch (Style)
    {
        case ENarrativeStyle::Linear:
            NewChapter.bIsOptional = false;
            break;
        case ENarrativeStyle::Branching:
            NewChapter.bIsOptional = true;
            break;
        case ENarrativeStyle::OpenWorld:
            NewChapter.bIsOptional = true;
            NewChapter.ChapterWeight = 0.5f;
            break;
        case ENarrativeStyle::Interactive:
            NewChapter.bIsOptional = false;
            NewChapter.ChapterWeight = 1.2f;
            break;
        default:
            break;
    }

    // 添加到相應的線程
    for (auto& ExperiencePair : CrossMediaExperiences)
    {
        FCrossMediaExperience& Experience = ExperiencePair.Value;
        for (FNarrativeThread& Thread : Experience.NarrativeThreads)
        {
            if (Thread.ThreadID == ThreadID)
            {
                Thread.Chapters.Add(NewChapter);
                
                // 如果是第一章節，設置為起始章節
                if (Thread.StartingChapter.IsEmpty())
                {
                    Thread.StartingChapter = NewChapter.ChapterID;
                }
                break;
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Created narrative chapter %s"), *ChapterTitle);
    return NewChapter;
}

FNarrativeElement UMingRTSCrossMediaNarrativeSystem::AddMediaElement(const FString& ChapterID, EMediaType MediaType, const FString& Content)
{
    FNarrativeElement NewElement;
    NewElement.ElementID = FString::Printf(TEXT("Element_%s_%d"), *ChapterID, FMath::RandRange(1000, 9999));
    NewElement.MediaType = MediaType;
    NewElement.Content = Content;
    NewElement.Duration = CalculateElementDuration(MediaType, Content);
    NewElement.bIsInteractive = IsMediaTypeInteractive(MediaType);

    // 根據媒體類型設置資產路徑
    NewElement.AssetPath = GenerateAssetPath(MediaType, Content);

    // 添加元數據
    NewElement.Metadata.Add(TEXT("MediaType"), GetMediaTypeName(MediaType));
    NewElement.Metadata.Add(TEXT("Created"), FDateTime::Now().ToString());
    NewElement.Metadata.Add(TEXT("Length"), FString::SanitizeFloat(NewElement.Duration));

    // 添加到章節
    for (auto& ExperiencePair : CrossMediaExperiences)
    {
        FCrossMediaExperience& Experience = ExperiencePair.Value;
        for (FNarrativeThread& Thread : Experience.NarrativeThreads)
        {
            for (FNarrativeChapter& Chapter : Thread.Chapters)
            {
                if (Chapter.ChapterID == ChapterID)
                {
                    Chapter.Elements.Add(NewElement);
                    
                    // 同時添加到體驗的媒體元素庫
                    Experience.MediaElements[MediaType].Add(NewElement);
                    break;
                }
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Added media element %s to chapter %s"), *NewElement.ElementID, *ChapterID);
    return NewElement;
}

FString UMingRTSCrossMediaNarrativeSystem::GeneratePersonalizedContent(const FString& PlayerID, const FString& Context)
{
    // 獲取玩家偏好
    TMap<EMediaType, float> PlayerPref = GetPlayerPreferences(PlayerID);
    
    // 分析上下文
    FString AnalyzedContext = AnalyzeContext(Context);
    
    // 生成個人化內容
    FString PersonalizedContent;
    
    // 根據玩家偏好選擇主要媒體類型
    EMediaType PrimaryMediaType = GetPreferredMediaType(PlayerPref);
    
    switch (PrimaryMediaType)
    {
        case EMediaType::Text:
            PersonalizedContent = GenerateTextContent(AnalyzedContext, PlayerID);
            break;
        case EMediaType::Audio:
            PersonalizedContent = GenerateAudioContent(AnalyzedContext, PlayerID);
            break;
        case EMediaType::Video:
            PersonalizedContent = GenerateVideoContent(AnalyzedContext, PlayerID);
            break;
        case EMediaType::Interactive:
            PersonalizedContent = GenerateInteractiveContent(AnalyzedContext, PlayerID);
            break;
        default:
            PersonalizedContent = GenerateTextContent(AnalyzedContext, PlayerID);
            break;
    }
    
    // 添加個人化標記
    PersonalizedContent = FString::Printf(TEXT("[Personalized for %s] %s"), *PlayerID, *PersonalizedContent);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Generated personalized content for player %s"), *PlayerID);
    return PersonalizedContent;
}

void UMingRTSCrossMediaNarrativeSystem::PlayMediaElement(const FString& ElementID)
{
    // 查找媒體元素
    FNarrativeElement* FoundElement = nullptr;
    FString FoundExperienceID;
    
    for (auto& ExperiencePair : CrossMediaExperiences)
    {
        FCrossMediaExperience& Experience = ExperiencePair.Value;
        for (auto& MediaPair : Experience.MediaElements)
        {
            for (FNarrativeElement& Element : MediaPair.Value)
            {
                if (Element.ElementID == ElementID)
                {
                    FoundElement = &Element;
                    FoundExperienceID = ExperiencePair.Key;
                    break;
                }
            }
        }
        
        if (FoundElement != nullptr)
        {
            break;
        }
    }
    
    if (FoundElement != nullptr)
    {
        // 播放媒體元素
        PlayMediaElementInternal(*FoundElement);
        
        // 觸發事件
        OnMediaElementPlayed.Broadcast(ElementID, FoundElement->MediaType, FoundElement->Content);
        
        // 記錄敘事事件
        RecordNarrativeEvent(FoundExperienceID, TEXT("MediaElementPlayed"), ElementID);
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Played media element %s"), *ElementID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSCrossMediaNarrativeSystem: Media element %s not found"), *ElementID);
    }
}

void UMingRTSCrossMediaNarrativeSystem::ProcessPlayerChoice(const FString& ChoiceID, const FString& ChoiceValue)
{
    // 記錄玩家選擇
    for (auto& ExperiencePair : CrossMediaExperiences)
    {
        FCrossMediaExperience& Experience = ExperiencePair.Value;
        Experience.PlayerChoices.Add(ChoiceID, ChoiceValue);
    }
    
    // 觸發選擇事件
    OnPlayerChoice.Broadcast(ChoiceID, ChoiceValue);
    
    // 處理選擇後果
    ProcessChoiceConsequences(ChoiceID, ChoiceValue);
    
    // 更新敘事進度
    UpdateNarrativeProgression();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Processed player choice %s = %s"), *ChoiceID, *ChoiceValue);
}

FCrossMediaExperience UMingRTSCrossMediaNarrativeSystem::GetCurrentExperience(const FString& ExperienceID) const
{
    return CrossMediaExperiences.FindRef(ExperienceID);
}

float UMingRTSCrossMediaNarrativeSystem::CalculateNarrativeProgress(const FString& ExperienceID) const
{
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return 0.0f;
    }
    
    const FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    float TotalProgress = 0.0f;
    int32 ThreadCount = 0;
    
    // 計算所有線程的進度
    for (const FNarrativeThread& Thread : Experience.NarrativeThreads)
    {
        float ThreadProgress = CalculateThreadProgress(Thread);
        TotalProgress += ThreadProgress * Thread.ThreadPriority;
        ThreadCount++;
    }
    
    if (ThreadCount > 0)
    {
        TotalProgress /= ThreadCount;
    }
    
    return FMath::Clamp(TotalProgress, 0.0f, 1.0f);
}

TArray<FString> UMingRTSCrossMediaNarrativeSystem::GenerateBranchPaths(const FString& ChapterID, const FString& PlayerContext)
{
    TArray<FString> BranchPaths;
    
    // 分析玩家上下文
    FString AnalyzedContext = AnalyzeContext(PlayerContext);
    
    // 生成分支路徑
    int32 BranchCount = FMath::RoundToInt(BranchingComplexity * 5.0f); // 最多5個分支
    
    for (int32 i = 0; i < BranchCount; i++)
    {
        FString BranchPath = GenerateBranchPath(AnalyzedContext, i);
        BranchPaths.Add(BranchPath);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Generated %d branch paths for chapter %s"), BranchCount, *ChapterID);
    return BranchPaths;
}

void UMingRTSCrossMediaNarrativeSystem::SynchronizeMediaContent(const FString& ExperienceID)
{
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return;
    }
    
    FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    
    // 同步文本與音頻
    SynchronizeTextWithAudio(ExperienceID);
    
    // 同步視頻與字幕
    SynchronizeVideoWithSubtitles(ExperienceID);
    
    // 同步互動內容與敘事
    SynchronizeInteractiveWithNarrative(ExperienceID);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Synchronized media content for experience %s"), *ExperienceID);
}

TArray<FNarrativeElement> UMingRTSCrossMediaNarrativeSystem::GetRecommendedContent(const FString& PlayerID, const FString& CurrentChapter)
{
    TArray<FNarrativeElement> RecommendedContent;
    
    // 獲取玩家偏好
    TMap<EMediaType, float> PlayerPref = GetPlayerPreferences(PlayerID);
    
    // 分析當前章節
    FString ChapterContext = AnalyzeChapterContext(CurrentChapter);
    
    // 推薦內容
    for (auto& ExperiencePair : CrossMediaExperiences)
    {
        FCrossMediaExperience& Experience = ExperiencePair.Value;
        for (auto& MediaPair : Experience.MediaElements)
        {
            for (const FNarrativeElement& Element : MediaPair.Value)
            {
                if (IsContentSuitable(Element, PlayerID))
                {
                    float SuitabilityScore = CalculateContentSuitability(Element, PlayerPref, ChapterContext);
                    
                    if (SuitabilityScore > 0.7f)
                    {
                        RecommendedContent.Add(Element);
                    }
                }
            }
        }
    }
    
    // 按適合性排序
    RecommendedContent.Sort([this, &PlayerPref, &ChapterContext](const FNarrativeElement& A, const FNarrativeElement& B)
    {
        float ScoreA = CalculateContentSuitability(A, PlayerPref, ChapterContext);
        float ScoreB = CalculateContentSuitability(B, PlayerPref, ChapterContext);
        return ScoreA > ScoreB;
    });
    
    // 限制推薦數量
    if (RecommendedContent.Num() > 10)
    {
        RecommendedContent.SetNum(10);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Generated %d recommended content items for player %s"), RecommendedContent.Num(), *PlayerID);
    return RecommendedContent;
}

void UMingRTSCrossMediaNarrativeSystem::SetNarrativeParameters(float PersonalizationLevel, float BranchingComplexity, float MediaDiversity)
{
    this->PersonalizationLevel = FMath::Clamp(PersonalizationLevel, 0.0f, 1.0f);
    this->BranchingComplexity = FMath::Clamp(BranchingComplexity, 0.0f, 1.0f);
    this->MediaDiversity = FMath::Clamp(MediaDiversity, 0.0f, 1.0f);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Updated narrative parameters - Personalization: %.2f, Branching: %.2f, Media: %.2f"), 
           this->PersonalizationLevel, this->BranchingComplexity, this->MediaDiversity);
}

TMap<FString, float> UMingRTSCrossMediaNarrativeSystem::GetNarrativeStatistics(const FString& ExperienceID) const
{
    TMap<FString, float> Statistics;
    
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return Statistics;
    }
    
    const FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    
    // 基本統計
    Statistics.Add(TEXT("OverallProgress"), Experience.OverallProgress);
    Statistics.Add(TEXT("ThreadCount"), Experience.NarrativeThreads.Num());
    Statistics.Add(TEXT("ActiveThreadCount"), Experience.ActiveThreads.Num());
    Statistics.Add(TEXT("PlayerChoiceCount"), Experience.PlayerChoices.Num());
    
    // 媒體統計
    int32 TotalMediaElements = 0;
    for (const auto& MediaPair : Experience.MediaElements)
    {
        TotalMediaElements += MediaPair.Value.Num();
    }
    Statistics.Add(TEXT("TotalMediaElements"), TotalMediaElements);
    
    // 媒體類型分佈
    for (int32 i = 0; i < 8; i++)
    {
        EMediaType MediaType = (EMediaType)i;
        int32 Count = Experience.MediaElements.FindRef(MediaType).Num();
        Statistics.Add(GetMediaTypeName(MediaType), Count);
    }
    
    // 敘事類型統計
    TMap<ENarrativeType, int32> NarrativeTypeCounts;
    for (const FNarrativeThread& Thread : Experience.NarrativeThreads)
    {
        NarrativeTypeCounts.FindOrAdd(Thread.NarrativeType)++;
    }
    
    Statistics.Add(TEXT("MainStoryCount"), NarrativeTypeCounts.FindRef(ENarrativeType::MainStory));
    Statistics.Add(TEXT("SideStoryCount"), NarrativeTypeCounts.FindRef(ENarrativeType::SideStory));
    Statistics.Add(TEXT("CharacterStoryCount"), NarrativeTypeCounts.FindRef(ENarrativeType::CharacterStory));
    Statistics.Add(TEXT("WorldStoryCount"), NarrativeTypeCounts.FindRef(ENarrativeType::WorldStory));
    Statistics.Add(TEXT("PersonalStoryCount"), NarrativeTypeCounts.FindRef(ENarrativeType::PersonalStory));
    
    return Statistics;
}

void UMingRTSCrossMediaNarrativeSystem::ResetNarrativeExperience(const FString& ExperienceID)
{
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return;
    }
    
    FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    
    // 重置進度
    Experience.OverallProgress = 0.0f;
    Experience.PlayerChoices.Empty();
    Experience.GlobalVariables.Empty();
    
    // 重置線程進度
    for (FNarrativeThread& Thread : Experience.NarrativeThreads)
    {
        Thread.ChapterProgress.Empty();
        
        // 重置到起始章節
        if (!Thread.StartingChapter.IsEmpty())
        {
            Thread.ActiveThreads.Add(Thread.ThreadID);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Reset narrative experience %s"), *ExperienceID);
}

// 私有方法實現

void UMingRTSCrossMediaNarrativeSystem::InitializeMediaElementLibrary()
{
    MediaElementLibrary.Empty();
    
    // 初始化各種媒體類型的庫
    for (int32 i = 0; i < 8; i++)
    {
        EMediaType MediaType = (EMediaType)i;
        TArray<FNarrativeElement> EmptyElements;
        MediaElementLibrary.Add(MediaType, EmptyElements);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Initialized media element library"));
}

void UMingRTSCrossMediaNarrativeSystem::InitializeContentGenerators()
{
    ContentGenerators.Empty();
    
    ContentGenerators.Add(EMediaType::Text, TEXT("TextGenerator"));
    ContentGenerators.Add(EMediaType::Audio, TEXT("AudioGenerator"));
    ContentGenerators.Add(EMediaType::Video, TEXT("VideoGenerator"));
    ContentGenerators.Add(EMediaType::Image, TEXT("ImageGenerator"));
    ContentGenerators.Add(EMediaType::Interactive, TEXT("InteractiveGenerator"));
    ContentGenerators.Add(EMediaType::Animation, TEXT("AnimationGenerator"));
    ContentGenerators.Add(EMediaType::VR_Experience, TEXT("VRGenerator"));
    ContentGenerators.Add(EMediaType::AR_Overlay, TEXT("ARGenerator"));
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Initialized content generators"));
}

void UMingRTSCrossMediaNarrativeSystem::InitializeNarrativeTemplates()
{
    NarrativeTemplates.Empty();
    
    // 主線故事模板
    FNarrativeThread MainStoryTemplate;
    MainStoryTemplate.ThreadName = TEXT("Main Story Template");
    MainStoryTemplate.NarrativeType = ENarrativeType::MainStory;
    MainStoryTemplate.ThreadPriority = 1.0f;
    NarrativeTemplates.Add(MainStoryTemplate);
    
    // 支線故事模板
    FNarrativeThread SideStoryTemplate;
    SideStoryTemplate.ThreadName = TEXT("Side Story Template");
    SideStoryTemplate.NarrativeType = ENarrativeType::SideStory;
    SideStoryTemplate.ThreadPriority = 0.7f;
    NarrativeTemplates.Add(SideStoryTemplate);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Initialized narrative templates"));
}

void UMingRTSCrossMediaNarrativeSystem::ProcessNarrativeProgression(const FString& ExperienceID)
{
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return;
    }
    
    FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    
    // 計算新的進度
    float NewProgress = CalculateNarrativeProgress(ExperienceID);
    
    // 如果進度有變化，觸發事件
    if (FMath::Abs(NewProgress - Experience.OverallProgress) > 0.01f)
    {
        Experience.OverallProgress = NewProgress;
        OnNarrativeProgress.Broadcast(ExperienceID);
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Narrative progress updated to %.2f for experience %s"), NewProgress, *ExperienceID);
    }
}

void UMingRTSCrossMediaNarrativeSystem::UpdatePlayerPreferences(const FString& PlayerID, EMediaType MediaType, float Engagement)
{
    if (!PlayerPreferences.Contains(PlayerID))
    {
        TMap<EMediaType, float> NewPreferences;
        PlayerPreferences.Add(PlayerID, NewPreferences);
    }
    
    TMap<EMediaType, float>& Preferences = PlayerPreferences[PlayerID];
    float CurrentPreference = Preferences.FindRef(MediaType);
    float NewPreference = FMath::Clamp(CurrentPreference + Engagement * 0.1f, 0.0f, 1.0f);
    
    Preferences.Add(MediaType, NewPreference);
}

void UMingRTSCrossMediaNarrativeSystem::GenerateDynamicContent(const FString& ExperienceID)
{
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return;
    }
    
    FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    
    // 根據玩家選擇和進度生成動態內容
    for (const auto& ChoicePair : Experience.PlayerChoices)
    {
        FString DynamicContent = GenerateContentForChoice(ChoicePair.Key, ChoicePair.Value);
        
        // 創建新的媒體元素
        FNarrativeElement DynamicElement;
        DynamicElement.ElementID = FString::Printf(TEXT("Dynamic_%s_%d"), *ChoicePair.Key, FMath::RandRange(1000, 9999));
        DynamicElement.MediaType = EMediaType::Text;
        DynamicElement.Content = DynamicContent;
        DynamicElement.bIsInteractive = true;
        
        // 添加到適當的媒體類型
        Experience.MediaElements[EMediaType::Text].Add(DynamicElement);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Generated dynamic content for experience %s"), *ExperienceID);
}

FString UMingRTSCrossMediaNarrativeSystem::GenerateTextContent(const FString& Context, const FString& PlayerProfile)
{
    // 基於上下文和玩家檔案生成文本內容
    FString GeneratedText;
    
    if (Context.Contains(TEXT("戰鬥")))
    {
        GeneratedText = TEXT("在激烈的戰鬥中，你展現了無與倫比的勇氣和智慧。");
    }
    else if (Context.Contains(TEXT("探索")))
    {
        GeneratedText = TEXT("探索未知的世界，你發現了隱藏的秘密和寶藏。");
    }
    else if (Context.Contains(TEXT("對話")))
    {
        GeneratedText = TEXT("與角色的深入對話，揭示了更多關於這個世界的故事。");
    }
    else
    {
        GeneratedText = TEXT("在這個充滿挑戰的世界裡，每一步都充滿了新的可能性。");
    }
    
    // 添加個人化元素
    GeneratedText += FString::Printf(TEXT("\n\n[為 %s 個人化]"), *PlayerProfile);
    
    return GeneratedText;
}

FString UMingRTSCrossMediaNarrativeSystem::GenerateAudioContent(const FString& Context, const FString& PlayerProfile)
{
    // 生成音頻內容描述
    FString AudioDescription;
    
    if (Context.Contains(TEXT("戰鬥")))
    {
        AudioDescription = TEXT("史詩般的戰鬥音樂，融合了傳統中國樂器和現代交響樂。");
    }
    else if (Context.Contains(TEXT("和平")))
    {
        AudioDescription = TEXT("寧靜的背景音樂，營造出和諧的氛圍。");
    }
    else if (Context.Contains(TEXT("神秘")))
    {
        AudioDescription = TEXT("神秘的音效，帶有懸疑和探索的感覺。");
    }
    else
    {
        AudioDescription = TEXT("適應性音樂，根據玩家行為動態變化。");
    }
    
    return AudioDescription;
}

FString UMingRTSCrossMediaNarrativeSystem::GenerateVideoContent(const FString& Context, const FString& PlayerProfile)
{
    // 生成視頻內容描述
    FString VideoDescription;
    
    if (Context.Contains(TEXT("過場")))
    {
        VideoDescription = TEXT("精華過場動畫，展示關鍵劇情時刻。");
    }
    else if (Context.Contains(TEXT("教學")))
    {
        VideoDescription = TEXT("互動教學視頻，指導玩家掌握遊戲機制。");
    }
    else if (Context.Contains(TEXT("回顧")))
    {
        VideoDescription = TEXT("劇情回顧視頻，總結玩家的冒險旅程。");
    }
    else
    {
        VideoDescription = TEXT("動態生成的視頻內容，適應玩家進度。");
    }
    
    return VideoDescription;
}

FString UMingRTSCrossMediaNarrativeSystem::GenerateInteractiveContent(const FString& Context, const FString& PlayerProfile)
{
    // 生成互動內容描述
    FString InteractiveDescription;
    
    if (Context.Contains(TEXT("選擇")))
    {
        InteractiveDescription = TEXT("分支對話系統，玩家的選擇影響劇情走向。");
    }
    else if (Context.Contains(TEXT("解謎")))
    {
        InteractiveDescription = TEXT("互動解謎環節，考驗玩家的智慧和觀察力。");
    }
    else if (Context.Contains(TEXT("探索")))
    {
        InteractiveDescription = TEXT("開放世界探索，發現隱藏的內容和秘密。");
    }
    else
    {
        InteractiveDescription = TEXT("適應性互動內容，根據玩家偏好調整難度。");
    }
    
    return InteractiveDescription;
}

float UMingRTSCrossMediaNarrativeSystem::CalculatePlayerEngagement(const FString& PlayerID, const FNarrativeElement& Element)
{
    // 計算玩家參與度
    float Engagement = 0.5f; // 基礎參與度
    
    // 根據媒體類型調整
    switch (Element.MediaType)
    {
        case EMediaType::Interactive:
            Engagement += 0.3f;
            break;
        case EMediaType::Video:
            Engagement += 0.2f;
            break;
        case EMediaType::Audio:
            Engagement += 0.1f;
            break;
        case EMediaType::Text:
            Engagement += 0.15f;
            break;
        default:
            break;
    }
    
    // 根據內容長度調整
    if (Element.Duration > 0.0f)
    {
        if (Element.Duration < 30.0f)
        {
            Engagement += 0.1f; // 短內容更容易保持參與
        }
        else if (Element.Duration > 300.0f)
        {
            Engagement -= 0.2f; // 長內容可能降低參與
        }
    }
    
    return FMath::Clamp(Engagement, 0.0f, 1.0f);
}

TArray<FString> UMingRTSCrossMediaNarrativeSystem::AnalyzePlayerChoices(const FString& PlayerID)
{
    TArray<FString> ChoicePatterns;
    
    // 分析玩家選擇模式
    for (auto& ExperiencePair : CrossMediaExperiences)
    {
        const FCrossMediaExperience& Experience = ExperiencePair.Value;
        
        for (const auto& ChoicePair : Experience.PlayerChoices)
        {
            FString ChoicePattern = FString::Printf(TEXT("%s:%s"), *ChoicePair.Key, *ChoicePair.Value);
            ChoicePatterns.Add(ChoicePattern);
        }
    }
    
    return ChoicePatterns;
}

FString UMingRTSCrossMediaNarrativeSystem::PredictNextContent(const FString& PlayerID, const FString& CurrentChapter)
{
    // 預測下一個內容
    FString Prediction;
    
    // 基於玩家歷史選擇預測
    TArray<FString> ChoiceHistory = AnalyzePlayerChoices(PlayerID);
    
    if (ChoiceHistory.Num() > 0)
    {
        // 簡單的模式識別
        FString LastChoice = ChoiceHistory[ChoiceHistory.Num() - 1];
        
        if (LastChoice.Contains(TEXT("戰鬥")))
        {
            Prediction = TEXT("戰鬥相關內容");
        }
        else if (LastChoice.Contains(TEXT("探索")))
        {
            Prediction = TEXT("探索相關內容");
        }
        else if (LastChoice.Contains(TEXT("對話")))
        {
            Prediction = TEXT("對話相關內容");
        }
        else
        {
            Prediction = TEXT("平衡的內容");
        }
    }
    else
    {
        Prediction = TEXT("基礎介紹內容");
    }
    
    return Prediction;
}

FString UMingRTSCrossMediaNarrativeSystem::GetMediaTypeName(EMediaType MediaType) const
{
    switch (MediaType)
    {
        case EMediaType::Text: return TEXT("Text");
        case EMediaType::Audio: return TEXT("Audio");
        case EMediaType::Video: return TEXT("Video");
        case EMediaType::Image: return TEXT("Image");
        case EMediaType::Interactive: return TEXT("Interactive");
        case EMediaType::Animation: return TEXT("Animation");
        case EMediaType::VR_Experience: return TEXT("VR_Experience");
        case EMediaType::AR_Overlay: return TEXT("AR_Overlay");
        default: return TEXT("Unknown");
    }
}

float UMingRTSCrossMediaNarrativeSystem::GetMediaTypeWeight(EMediaType MediaType) const
{
    // 不同媒體類型的權重
    switch (MediaType)
    {
        case EMediaType::Interactive: return 1.0f;
        case EMediaType::Video: return 0.9f;
        case EMediaType::Audio: return 0.7f;
        case EMediaType::Text: return 0.8f;
        case EMediaType::Animation: return 0.85f;
        case EMediaType::Image: return 0.6f;
        case EMediaType::VR_Experience: return 1.1f;
        case EMediaType::AR_Overlay: return 0.95f;
        default: return 0.5f;
    }
}

bool UMingRTSCrossMediaNarrativeSystem::IsContentSuitable(const FNarrativeElement& Element, const FString& PlayerProfile)
{
    // 檢查內容是否適合玩家
    // 這裡可以實現更複雜的適合性檢查邏輯
    
    // 基於媒體類型和玩家偏好的簡單檢查
    if (PlayerPreferences.Contains(PlayerProfile))
    {
        const TMap<EMediaType, float>& Preferences = PlayerPreferences[PlayerProfile];
        float Preference = Preferences.FindRef(Element.MediaType);
        
        return Preference > 0.3f; // 偏好度超過30%認為適合
    }
    
    return true; // 沒有偏好信息時默認適合
}

void UMingRTSCrossMediaNarrativeSystem::RecordNarrativeEvent(const FString& ExperienceID, const FString& EventType, const FString& EventData)
{
    // 記錄敘事事件
    FString EventRecord = FString::Printf(TEXT("[%s] %s: %s"), *FDateTime::Now().ToString(), *EventType, *EventData);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: %s"), *EventRecord);
    
    // 這裡可以添加更複雜的事件記錄邏輯，比如保存到數據庫
}

void UMingRTSCrossMediaNarrativeSystem::SynchronizeTextWithAudio(const FString& ExperienceID)
{
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return;
    }
    
    FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    
    // 同步文本和音頻內容
    TArray<FNarrativeElement>& TextElements = Experience.MediaElements[EMediaType::Text];
    TArray<FNarrativeElement>& AudioElements = Experience.MediaElements[EMediaType::Audio];
    
    // 簡單的同步邏輯：為每個文本元素匹配對應的音頻
    for (FNarrativeElement& TextElement : TextElements)
    {
        for (FNarrativeElement& AudioElement : AudioElements)
        {
            if (TextElement.ElementID.Contains(AudioElement.ElementID) || 
                AudioElement.ElementID.Contains(TextElement.ElementID))
            {
                // 建立同步關係
                TextElement.Dependencies.Add(AudioElement.ElementID);
                AudioElement.Dependencies.Add(TextElement.ElementID);
                
                // 調整時長以匹配
                if (TextElement.Duration > 0.0f && AudioElement.Duration > 0.0f)
                {
                    float AverageDuration = (TextElement.Duration + AudioElement.Duration) / 2.0f;
                    TextElement.Duration = AverageDuration;
                    AudioElement.Duration = AverageDuration;
                }
                
                break;
            }
        }
    }
}

void UMingRTSCrossMediaNarrativeSystem::SynchronizeVideoWithSubtitles(const FString& ExperienceID)
{
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return;
    }
    
    FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    
    // 同步視頻和字幕
    TArray<FNarrativeElement>& VideoElements = Experience.MediaElements[EMediaType::Video];
    TArray<FNarrativeElement>& TextElements = Experience.MediaElements[EMediaType::Text];
    
    for (FNarrativeElement& VideoElement : VideoElements)
    {
        for (FNarrativeElement& TextElement : TextElements)
        {
            if (TextElement.Content.Contains(TEXT("字幕")) || 
                TextElement.ElementID.Contains(VideoElement.ElementID))
            {
                // 建立字幕關係
                VideoElement.Dependencies.Add(TextElement.ElementID);
                TextElement.Metadata.Add(TEXT("VideoSync"), VideoElement.ElementID);
                
                break;
            }
        }
    }
}

void UMingRTSCrossMediaNarrativeSystem::SynchronizeInteractiveWithNarrative(const FString& ExperienceID)
{
    if (!CrossMediaExperiences.Contains(ExperienceID))
    {
        return;
    }
    
    FCrossMediaExperience& Experience = CrossMediaExperiences[ExperienceID];
    
    // 同步互動內容與敘事
    TArray<FNarrativeElement>& InteractiveElements = Experience.MediaElements[EMediaType::Interactive];
    
    for (FNarrativeElement& InteractiveElement : InteractiveElements)
    {
        // 為互動元素添加敘事上下文
        InteractiveElement.Metadata.Add(TEXT("NarrativeContext"), Experience.ExperienceName);
        InteractiveElement.Metadata.Add(TEXT("PersonalizationLevel"), FString::SanitizeFloat(PersonalizationLevel));
    }
}

// 輔助方法實現

float UMingRTSCrossMediaNarrativeSystem::CalculateThreadPriority(ENarrativeType NarrativeType) const
{
    switch (NarrativeType)
    {
        case ENarrativeType::MainStory: return 1.0f;
        case ENarrativeType::SideStory: return 0.7f;
        case ENarrativeType::CharacterStory: return 0.8f;
        case ENarrativeType::WorldStory: return 0.6f;
        case ENarrativeType::PersonalStory: return 0.9f;
        default: return 0.5f;
    }
}

float UMingRTSCrossMediaNarrativeSystem::CalculateChapterWeight(ENarrativeStyle Style) const
{
    switch (Style)
    {
        case ENarrativeStyle::Linear: return 1.0f;
        case ENarrativeStyle::Branching: return 0.8f;
        case ENarrativeStyle::OpenWorld: return 0.6f;
        case ENarrativeStyle::Episodic: return 0.9f;
        case ENarrativeStyle::Interactive: return 1.2f;
        case ENarrativeStyle::Emergent: return 0.7f;
        default: return 1.0f;
    }
}

float UMingRTSCrossMediaNarrativeSystem::CalculateElementDuration(EMediaType MediaType, const FString& Content) const
{
    // 基於媒體類型和內容長度計算時長
    switch (MediaType)
    {
        case EMediaType::Text:
            return Content.Len() * 0.1f; // 每個字符0.1秒
        case EMediaType::Audio:
            return 60.0f; // 默認1分鐘
        case EMediaType::Video:
            return 120.0f; // 默認2分鐘
        case EMediaType::Interactive:
            return 180.0f; // 默認3分鐘
        case EMediaType::Animation:
            return 30.0f; // 默認30秒
        default:
            return 60.0f;
    }
}

bool UMingRTSCrossMediaNarrativeSystem::IsMediaTypeInteractive(EMediaType MediaType) const
{
    return MediaType == EMediaType::Interactive || 
           MediaType == EMediaType::VR_Experience || 
           MediaType == EMediaType::AR_Overlay;
}

FString UMingRTSCrossMediaNarrativeSystem::GenerateAssetPath(EMediaType MediaType, const FString& Content) const
{
    FString AssetPath;
    
    switch (MediaType)
    {
        case EMediaType::Text:
            AssetPath = TEXT("/Game/Content/Text/");
            break;
        case EMediaType::Audio:
            AssetPath = TEXT("/Game/Content/Audio/");
            break;
        case EMediaType::Video:
            AssetPath = TEXT("/Game/Content/Video/");
            break;
        case EMediaType::Image:
            AssetPath = TEXT("/Game/Content/Images/");
            break;
        case EMediaType::Interactive:
            AssetPath = TEXT("/Game/Content/Interactive/");
            break;
        default:
            AssetPath = TEXT("/Game/Content/Media/");
            break;
    }
    
    AssetPath += FString::Printf(TEXT("Asset_%d"), FMath::RandRange(1000, 9999));
    return AssetPath;
}

TMap<EMediaType, float> UMingRTSCrossMediaNarrativeSystem::GetPlayerPreferences(const FString& PlayerID) const
{
    return PlayerPreferences.FindRef(PlayerID);
}

EMediaType UMingRTSCrossMediaNarrativeSystem::GetPreferredMediaType(const TMap<EMediaType, float>& Preferences) const
{
    EMediaType PreferredType = EMediaType::Text;
    float MaxPreference = 0.0f;
    
    for (const auto& PrefPair : Preferences)
    {
        if (PrefPair.Value > MaxPreference)
        {
            MaxPreference = PrefPair.Value;
            PreferredType = PrefPair.Key;
        }
    }
    
    return PreferredType;
}

FString UMingRTSCrossMediaNarrativeSystem::AnalyzeContext(const FString& Context) const
{
    // 分析上下文，提取關鍵信息
    FString AnalyzedContext = Context;
    
    // 這裡可以添加更複雜的上下文分析邏輯
    // 比如自然語言處理、關鍵詞提取等
    
    return AnalyzedContext;
}

FString UMingRTSCrossMediaNarrativeSystem::AnalyzeChapterContext(const FString& ChapterID) const
{
    // 分析章節上下文
    FString Context = TEXT("ChapterContext:");
    
    // 查找章節信息
    for (const auto& ExperiencePair : CrossMediaExperiences)
    {
        const FCrossMediaExperience& Experience = ExperiencePair.Value;
        for (const FNarrativeThread& Thread : Experience.NarrativeThreads)
        {
            for (const FNarrativeChapter& Chapter : Thread.Chapters)
            {
                if (Chapter.ChapterID == ChapterID)
                {
                    Context += Chapter.ChapterTitle;
                    Context += ":";
                    Context += Chapter.ChapterDescription;
                    return Context;
                }
            }
        }
    }
    
    return Context + TEXT("Unknown");
}

float UMingRTSCrossMediaNarrativeSystem::CalculateThreadProgress(const FNarrativeThread& Thread) const
{
    if (Thread.Chapters.Num() == 0)
    {
        return 0.0f;
    }
    
    float CompletedChapters = 0.0f;
    float TotalWeight = 0.0f;
    
    for (const FNarrativeChapter& Chapter : Thread.Chapters)
    {
        float ChapterProgress = Thread.ChapterProgress.FindRef(Chapter.ChapterID);
        CompletedChapters += ChapterProgress * Chapter.ChapterWeight;
        TotalWeight += Chapter.ChapterWeight;
    }
    
    return TotalWeight > 0.0f ? CompletedChapters / TotalWeight : 0.0f;
}

void UMingRTSCrossMediaNarrativeSystem::PlayMediaElementInternal(const FNarrativeElement& Element)
{
    // 內部媒體元素播放邏輯
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Playing media element %s (%s)"), 
           *Element.ElementID, *GetMediaTypeName(Element.MediaType));
    
    // 這裡需要與實際的媒體播放系統集成
    // 暫時只記錄日誌
}

void UMingRTSCrossMediaNarrativeSystem::ProcessChoiceConsequences(const FString& ChoiceID, const FString& ChoiceValue)
{
    // 處理選擇後果
    UE_LOG(LogTemp, Log, TEXT("MingRTSCrossMediaNarrativeSystem: Processing consequences for choice %s = %s"), *ChoiceID, *ChoiceValue);
    
    // 這裡可以實現複雜的選擇後果處理邏輯
    // 比如影響敘事走向、解鎖新內容等
}

void UMingRTSCrossMediaNarrativeSystem::UpdateNarrativeProgression()
{
    // 更新所有體驗的敘事進度
    for (auto& ExperiencePair : CrossMediaExperiences)
    {
        ProcessNarrativeProgression(ExperiencePair.Key);
    }
}

FString UMingRTSCrossMediaNarrativeSystem::GenerateBranchPath(const FString& Context, int32 BranchIndex)
{
    // 生成分支路徑
    FString BranchPath = FString::Printf(TEXT("Branch_%d_"), BranchIndex);
    
    if (Context.Contains(TEXT("戰鬥")))
    {
        BranchPath += TEXT("Combat_Path");
    }
    else if (Context.Contains(TEXT("外交")))
    {
        BranchPath += TEXT("Diplomacy_Path");
    }
    else if (Context.Contains(TEXT("探索")))
    {
        BranchPath += TEXT("Exploration_Path");
    }
    else
    {
        BranchPath += TEXT("Default_Path");
    }
    
    return BranchPath;
}

float UMingRTSCrossMediaNarrativeSystem::CalculateContentSuitability(const FNarrativeElement& Element, const TMap<EMediaType, float>& PlayerPreferences, const FString& ChapterContext) const
{
    float Suitability = 0.5f; // 基礎適合性
    
    // 基於玩家偏好
    float PlayerPreference = PlayerPreferences.FindRef(Element.MediaType);
    Suitability += PlayerPreference * 0.4f;
    
    // 基於內容匹配
    if (Element.Content.Contains(ChapterContext) || ChapterContext.Contains(Element.Content))
    {
        Suitability += 0.3f;
    }
    
    // 基於媒體類型權重
    float MediaWeight = GetMediaTypeWeight(Element.MediaType);
    Suitability += MediaWeight * 0.2f;
    
    // 基於互動性
    if (Element.bIsInteractive)
    {
        Suitability += 0.1f;
    }
    
    return FMath::Clamp(Suitability, 0.0f, 1.0f);
}

FString UMingRTSCrossMediaNarrativeSystem::GenerateContentForChoice(const FString& ChoiceID, const FString& ChoiceValue)
{
    // 為選擇生成對應內容
    FString GeneratedContent = FString::Printf(TEXT("基於選擇 %s 的動態內容："), *ChoiceID);
    
    if (ChoiceValue.Contains(TEXT("戰鬥")))
    {
        GeneratedContent += TEXT("你選擇了戰鬥路線，接下來將面臨更激烈的挑戰。");
    }
    else if (ChoiceValue.Contains(TEXT("和平")))
    {
        GeneratedContent += TEXT("你選擇了和平路線，將有機會通過外交解決問題。");
    }
    else if (ChoiceValue.Contains(TEXT("探索")))
    {
        GeneratedContent += TEXT("你選擇了探索路線，將發現更多隱藏的秘密。");
    }
    else
    {
        GeneratedContent += TEXT("你的選擇將影響故事的發展方向。");
    }
    
    return GeneratedContent;
}
