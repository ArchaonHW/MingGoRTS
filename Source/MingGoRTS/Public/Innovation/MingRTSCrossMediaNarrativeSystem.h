#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCrossMediaNarrativeSystem.generated.h"

UENUM(BlueprintType)
enum class ENarrativeType : uint8
{
    MainStory,      // ‰∏ªÁ??Ö‰?
    SideStory,      // ?ØÁ??Ö‰?
    CharacterStory, // ËßíËâ≤?Ö‰?
    WorldStory,     // ‰∏ñÁ??Ö‰?
    PersonalStory   // ?ã‰∫∫?Ö‰?
};

UENUM(BlueprintType)
enum class EMediaType : uint8
{
    Text,           // ?áÊú¨
    Audio,          // ?≥È†ª
    Video,          // Ë¶ñÈ†ª
    Image,          // ?ñÂ?
    Interactive,    // ‰∫íÂ??ßÂÆπ
    Animation,      // ?ïÁï´
    VR_Experience,  // VRÈ´îÈ?
    AR_Overlay      // ARË¶ÜË?
};

UENUM(BlueprintType)
enum class ENarrativeStyle : uint8
{
    Linear,         // Á∑öÊÄ?    Branching,      // ?ÜÊîØ
    OpenWorld,      // ?ãÊîæ‰∏ñÁ?
    Episodic,       // ?áÈ?Âº?    Interactive,    // ‰∫íÂ?Âº?    Emergent        // Êº∏ÁèæÂº?};

USTRUCT(BlueprintType)
struct FNarrativeElement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ElementID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ElementName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMediaType MediaType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Content;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> Metadata;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInteractive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PlayerChoices;

    FNarrativeElement()
    {
        ElementID = TEXT("");
        ElementName = TEXT("");
        MediaType = EMediaType::Text;
        Content = TEXT("");
        AssetPath = TEXT("");
        Duration = 0.0f;
        bIsInteractive = false;
    }
};

USTRUCT(BlueprintType)
struct FNarrativeChapter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChapterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChapterTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChapterDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FNarrativeElement> Elements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ChapterChoices;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> NextChapters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChapterWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOptional;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ENarrativeStyle NarrativeStyle;

    FNarrativeChapter()
    {
        ChapterID = TEXT("");
        ChapterTitle = TEXT("");
        ChapterDescription = TEXT("");
        ChapterWeight = 1.0f;
        bIsOptional = false;
        NarrativeStyle = ENarrativeStyle::Linear;
    }
};

USTRUCT(BlueprintType)
struct FNarrativeThread
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreadID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreadName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ENarrativeType NarrativeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FNarrativeChapter> Chapters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StartingChapter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredChapters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> OptionalChapters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ChapterProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreadPriority;

    FNarrativeThread()
    {
        ThreadID = TEXT("");
        ThreadName = TEXT("");
        NarrativeType = ENarrativeType::MainStory;
        StartingChapter = TEXT("");
        bIsActive = true;
        ThreadPriority = 1.0f;
    }
};

USTRUCT(BlueprintType)
struct FCrossMediaExperience
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExperienceID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExperienceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FNarrativeThread> NarrativeThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMediaType, TArray<FNarrativeElement>> MediaElements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ActiveThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> PlayerChoices;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> GlobalVariables;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPersonalized;

    FCrossMediaExperience()
    {
        ExperienceID = TEXT("");
        ExperienceName = TEXT("");
        Description = TEXT("");
        OverallProgress = 0.0f;
        bIsPersonalized = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNarrativeEvent, const FString&, ThreadID, const FString&, EventData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMediaElementPlayed, const FString&, ElementID, EMediaType, MediaType, const FString&, Content);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerChoice, const FString&, ChoiceID, const FString&, ChoiceValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNarrativeProgress, const FString&, ExperienceID);

/**
 * Ë∑®Â?È´îÊ?‰∫ãÁ≥ªÁµ?- ?äÊà≤?áÂ?Â™íÈ??ßÂÆπ?çÂ?Á≥ªÁµ±
 * ?ê‰?Ë∑®Â?È´îÁ??ò‰?È´îÈ??åÂÄã‰∫∫?ñÊ?‰∫ãÁ??? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCrossMediaNarrativeSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCrossMediaNarrativeSystem();

    // Á≥ªÁµ±?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    void InitializeCrossMediaSystem();

    // ?µÂª∫Ë∑®Â?È´îÈ?È©?    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    FCrossMediaExperience CreateCrossMediaExperience(const FString& ExperienceID, const FString& ExperienceName);

    // Ê∑ªÂ??ò‰?Á∑öÁ?
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    FNarrativeThread AddNarrativeThread(const FString& ExperienceID, ENarrativeType NarrativeType, const FString& ThreadName);

    // ?µÂª∫?ò‰?Á´†Á?
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    FNarrativeChapter CreateNarrativeChapter(const FString& ThreadID, const FString& ChapterTitle, ENarrativeStyle Style);

    // Ê∑ªÂ?Â™íÈ??ÉÁ?
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    FNarrativeElement AddMediaElement(const FString& ChapterID, EMediaType MediaType, const FString& Content);

    // ?üÊ??ã‰∫∫?ñÂÖßÂÆ?    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    FString GeneratePersonalizedContent(const FString& PlayerID, const FString& Context);

    // ?≠ÊîæÂ™íÈ??ÉÁ?
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    void PlayMediaElement(const FString& ElementID);

    // ?ïÁ??©ÂÆ∂?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    void ProcessPlayerChoice(const FString& ChoiceID, const FString& ChoiceValue);

    // ?≤Â??∂Â??ò‰??Ä??    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    FCrossMediaExperience GetCurrentExperience(const FString& ExperienceID) const;

    // Ë®àÁ??ò‰??≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    float CalculateNarrativeProgress(const FString& ExperienceID) const;

    // ?üÊ??ÜÊîØË∑ØÂ?
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    TArray<FString> GenerateBranchPaths(const FString& ChapterID, const FString& PlayerContext);

    // ?åÊ≠•Â§öÂ?È´îÂÖßÂÆ?    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    void SynchronizeMediaContent(const FString& ExperienceID);

    // ?≤Â??®Ëñ¶?ßÂÆπ
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    TArray<FNarrativeElement> GetRecommendedContent(const FString& PlayerID, const FString& CurrentChapter);

    // Ë®≠ÁΩÆ?ò‰??ÉÊï∏
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    void SetNarrativeParameters(float PersonalizationLevel, float BranchingComplexity, float MediaDiversity);

    // ?≤Â??ò‰?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    TMap<FString, float> GetNarrativeStatistics(const FString& ExperienceID) const;

    // ?çÁΩÆ?ò‰?È´îÈ?
    UFUNCTION(BlueprintCallable, Category = "Cross Media Narrative")
    void ResetNarrativeExperience(const FString& ExperienceID);

    // ‰∫ã‰ª∂ÂßîÊ?
    UPROPERTY(BlueprintAssignable)
    FOnNarrativeEvent OnNarrativeEvent;

    UPROPERTY(BlueprintAssignable)
    FOnMediaElementPlayed OnMediaElementPlayed;

    UPROPERTY(BlueprintAssignable)
    FOnPlayerChoice OnPlayerChoice;

    UPROPERTY(BlueprintAssignable)
    FOnNarrativeProgress OnNarrativeProgress;

protected:
    // ?ò‰??ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Parameters")
    float PersonalizationLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Parameters")
    float BranchingComplexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Parameters")
    float MediaDiversity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Parameters")
    int32 MaxConcurrentThreads;

    // Ë∑®Â?È´îÈ?È©óÂ???    UPROPERTY(BlueprintReadOnly, Category = "Narrative Data")
    TMap<FString, FCrossMediaExperience> CrossMediaExperiences;

    // Â™íÈ??ÉÁ?Â∫?    UPROPERTY(BlueprintReadOnly, Category = "Narrative Data")
    TMap<EMediaType, TArray<FNarrativeElement>> MediaElementLibrary;

    // ?©ÂÆ∂?èÂ•ΩÊ™îÊ?
    UPROPERTY(BlueprintReadOnly, Category = "Narrative Data")
    TMap<FString, TMap<EMediaType, float>> PlayerPreferences;

    // ?ò‰?Ê®°Êùø
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Templates")
    TArray<FNarrativeThread> NarrativeTemplates;

    // ?ßÂÆπ?üÊ???    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Data")
    TMap<EMediaType, FString> ContentGenerators;

private:
    // ?ßÈÉ®?ò‰??ïÁ??èËºØ
    void ProcessNarrativeProgression(const FString& ExperienceID);
    void UpdatePlayerPreferences(const FString& PlayerID, EMediaType MediaType, float Engagement);
    void GenerateDynamicContent(const FString& ExperienceID);
    
    // Â™íÈ??ßÂÆπ?üÊ?
    FString GenerateTextContent(const FString& Context, const FString& PlayerProfile);
    FString GenerateAudioContent(const FString& Context, const FString& PlayerProfile);
    FString GenerateVideoContent(const FString& Context, const FString& PlayerProfile);
    FString GenerateInteractiveContent(const FString& Context, const FString& PlayerProfile);
    
    // ?ò‰??ÜÊ?
    float CalculatePlayerEngagement(const FString& PlayerID, const FNarrativeElement& Element);
    TArray<FString> AnalyzePlayerChoices(const FString& PlayerID);
    FString PredictNextContent(const FString& PlayerID, const FString& CurrentChapter);
    
    // ËºîÂä©?ΩÊï∏
    FString GetMediaTypeName(EMediaType MediaType) const;
    float GetMediaTypeWeight(EMediaType MediaType) const;
    bool IsContentSuitable(const FNarrativeElement& Element, const FString& PlayerProfile);
    void RecordNarrativeEvent(const FString& ExperienceID, const FString& EventType, const FString& EventData);
    
    // Ë∑®Â?È´îÂ?Ê≠?    void SynchronizeTextWithAudio(const FString& ExperienceID);
    void SynchronizeVideoWithSubtitles(const FString& ExperienceID);
    void SynchronizeInteractiveWithNarrative(const FString& ExperienceID);
};
