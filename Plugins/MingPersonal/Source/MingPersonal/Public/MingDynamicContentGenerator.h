#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingAIUIManager.h"
#include "MingDynamicContentGenerator.generated.h"

USTRUCT(BlueprintType)
struct FDynamicContentConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Content")
    FString ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Content")
    FString Template;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Content")
    TArray<FString> Variables;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Content")
    bool bIsContextual = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Content")
    float RelevanceScore = 0.0f;

    FDynamicContentConfig()
    {
        ContentType = TEXT(""};
        Template = TEXT(""};
        bIsContextual = true;
        RelevanceScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FGeneratedContent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Generated Content")
    FString ContentID;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Content")
    FString ContentType;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Content")
    FString Title;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Content")
    FString Body;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Content")
    TArray<FString> ActionButtons;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Content")
    float RelevanceScore;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Content")
    FDateTime GeneratedTime;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Content")
    TMap<FString, FString> Metadata;

    FGeneratedContent()
    {
        ContentID = TEXT(""};
        ContentType = TEXT(""};
        Title = TEXT(""};
        Body = TEXT(""};
        RelevanceScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FContextualData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Contextual Data")
    FString CurrentLocation;

    UPROPERTY(BlueprintReadOnly, Category = "Contextual Data")
    FString ActiveQuest;

    UPROPERTY(BlueprintReadOnly, Category = "Contextual Data")
    FString CurrentRelationship;

    UPROPERTY(BlueprintReadOnly, Category = "Contextual Data")
    float PlayerLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Contextual Data")
    EAIUserProfile UserProfile;

    UPROPERTY(BlueprintReadOnly, Category = "Contextual Data")
    TArray<FString> RecentActions;

    UPROPERTY(BlueprintReadOnly, Category = "Contextual Data")
    TMap<FString, float> SkillLevels;

    FContextualData()
    {
        CurrentLocation = TEXT(""};
        ActiveQuest = TEXT(""};
        CurrentRelationship = TEXT(""};
        PlayerLevel = 1.0f;
        UserProfile = EAIUserProfile::NewPlayer;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContentGenerated, const FGeneratedContent&, Content};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnContentRequested, const FString&, ContentType, const FContextualData&, Context);

/**
 * ???X?甧???X * ??AI?????X??X???UI??甧
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingDynamicContentGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingDynamicContentGenerator(};

    // ???X?ず甧?X?竟
    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void InitializeContentGenerator(UMingAIUIManager* InAIManager);

    // ??甧????
    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent GenerateContent(const FString& ContentType, const FContextualData& Context);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent GenerateNotification(const FString& Trigger, const FContextualData& Context);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent GenerateTooltip(const FString& Target, const FContextualData& Context);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent GenerateHelpText(const FString& Topic, const FContextualData& Context);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent GenerateQuestHint(const FString& QuestID, const FContextualData& Context);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent GenerateRelationshipAdvice(const FString& CharacterID, const FContextualData& Context);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent GenerateStrategicAdvice(const FContextualData& Context);

    // 家狾恨??
    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void AddContentTemplate(const FDynamicContentConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void RemoveContentTemplate(const FString& ContentType);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FDynamicContentConfig GetContentTemplate(const FString& ContentType) const;

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    TArray<FDynamicContentConfig> GetAllTemplates() const;

    // ?X??X    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FContextualData AnalyzeCurrentContext();

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void UpdateContextualData(const FContextualData& NewContext);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    float CalculateContentRelevance(const FString& ContentType, const FContextualData& Context) const;

    // ??甧????
    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void OptimizeContentForUser();

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void PersonalizeContent(FGeneratedContent& Content, const FContextualData& Context);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void AdjustContentTone(FGeneratedContent& Content, EAIUserProfile UserProfile);

    // ???X???
    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    TArray<FGeneratedContent> GenerateContentBatch(const TArray<FString>& ContentTypes, const FContextualData& Context};

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void ScheduleContentGeneration(const FString& ContentType, float Delay);

    // ??甧絯??
    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void CacheContent(const FGeneratedContent& Content);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent GetCachedContent(const FString& ContentID) const;

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void ClearContentCache();

    // ????やX    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    FGeneratedContent LocalizeContent(const FGeneratedContent& Content, const FString& LanguageCode);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void AddLocalizationTemplate(const FString& ContentType, const FString& LanguageCode, const FString& Template);

    // ??甧蝶︳
    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    float EvaluateContentQuality(const FGeneratedContent& Content) const;

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    bool IsContentAppropriate(const FGeneratedContent& Content, const FContextualData& Context) const;

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void RateContent(const FString& ContentID, float Rating);

    // ????琩??    UFUNCTION(BlueprintPure, Category = "Dynamic Content")
    bool IsGeneratorReady() const { return bIsInitialized; }

    UFUNCTION(BlueprintPure, Category = "Dynamic Content")
    int32 GetTemplateCount() const { return ContentTemplates.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Dynamic Content")
    int32 GetCachedContentCount() const { return ContentCache.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Dynamic Content")
    FContextualData GetCurrentContext() const { return CurrentContext; }

    // 砞竚
    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void SetGenerationEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void SetCacheEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void SetMaxCacheSize(int32 MaxSize);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Content")
    void SetRelevanceThreshold(float Threshold);

    // ㄆン
    UPROPERTY(BlueprintAssignable, Category = "Content Events")
    FOnContentGenerated OnContentGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Content Events")
    FOnContentRequested OnContentRequested;

protected:
    // ╰参まノ
    UPROPERTY()
    TObjectPtr<UMingAIUIManager> AIManager;

    // ??竚
    UPROPERTY()
    bool bIsInitialized = false;

    UPROPERTY()
    bool bGenerationEnabled = true;

    UPROPERTY()
    bool bCacheEnabled = true;

    UPROPERTY()
    float RelevanceThreshold = 0.5f;

    UPROPERTY()
    int32 MaxCacheSize = 1000;

    // ????纗
    UPROPERTY()
    TMap<FString, FDynamicContentConfig> ContentTemplates;

    UPROPERTY()
    TMap<FString, FGeneratedContent> ContentCache;

    UPROPERTY()
    FContextualData CurrentContext;

    // 猔??TMap<TArray> ぃやXUPROPERTY
    TMap<FString, TArray<FString>> LocalizationTemplates;

    UPROPERTY()
    TMap<FString, float> ContentRatings;

    // ??场??计
    void InitializeDefaultTemplates();
    FString ProcessTemplate(const FString& Template, const TMap<FString, FString>& Variables) const;
    TMap<FString, FString> ExtractContextVariables(const FContextualData& Context) const;
    FString GenerateContentID(const FString& ContentType) const;
    void UpdateContentStatistics(const FGeneratedContent& Content);
    bool ShouldGenerateContent(const FString& ContentType, const FContextualData& Context) const;

    // AI????徊
    FString GenerateTextWithAI(const FString& Prompt, const TMap<FString, FString>& Context) const;
    TArray<FString> GenerateActionSuggestions(const FContextualData& Context) const;
    FString AdjustToneForProfile(const FString& Text, EAIUserProfile Profile) const;
    float CalculateSemanticRelevance(const FString& Content, const FString& Context) const;

private:
    // 徊??计
    void SaveContentData(};
    void LoadContentData(};
    void CleanupOldCache();
    FString GetCurrentLanguageCode() const;
};

