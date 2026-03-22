// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Historical Narrative Generator - Creates compelling historical narratives from events

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicHistorySystem.h"
#include "MingRTSHistoricalNarrativeGenerator.generated.h"

// Narrative Style Types
UENUM(BlueprintType)
enum class ENarrativeStyle: uint8 {
    Chronological UMETA(DisplayName = "編年體"),
    Thematic UMETA(DisplayName = "主題體"),
    Biographical UMETA(DisplayName = "傳記體"),
    Epic UMETA(DisplayName = "史詩體"),
    Academic UMETA(DisplayName = "學術體"),
    Personal UMETA(DisplayName = "個人體"),
    Cinematic UMETA(DisplayName = "電影體"),
    Interactive UMETA(DisplayName = "互動體")
};

// Narrative Tone Types
UENUM(BlueprintType)
enum class ENarrativeTone: uint8 {
    Neutral UMETA(DisplayName = "中立"),
    Triumphant UMETA(DisplayName = "勝利"),
    Tragic UMETA(DisplayName = "悲劇"),
    Mysterious UMETA(DisplayName = "神秘"),
    Heroic UMETA(DisplayName = "英雄"),
    Somber UMETA(DisplayName = "沉重"),
    Hopeful UMETA(DisplayName = "希望"),
    Dramatic UMETA(DisplayName = "戲劇性")
};

// Narrative Focus Types
UENUM(BlueprintType)
enum class ENarrativeFocus: uint8 {
    Events UMETA(DisplayName = "事件"),
    Characters UMETA(DisplayName = "人物"),
    Consequences UMETA(DisplayName = "後果"),
    Causality UMETA(DisplayName = "因果"),
    Themes UMETA(DisplayName = "主題"),
    Emotions UMETA(DisplayName = "情感"),
    Legacy UMETA(DisplayName = "遺產"),
    Lessons UMETA(DisplayName = "教訓")
};

// Narrative Structure
USTRUCT(BlueprintType)
struct MINGGORTS_API FNarrativeStructure
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    FString Introduction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    TArray<FString> Development;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    FString Climax;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    FString Resolution;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    FString Epilogue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    ENarrativeStyle Style;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    ENarrativeTone Tone;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Structure")
    ENarrativeFocus Focus;

    FNarrativeStructure()
    {
        Style = ENarrativeStyle::Chronological;
        Tone = ENarrativeTone::Neutral;
        Focus = ENarrativeFocus::Events;
    }
};

// Narrative Generation Request
USTRUCT(BlueprintType)
struct MINGGORTS_API FNarrativeGenerationRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    FString TimelineID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    FDateTime EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    ENarrativeStyle Style;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    ENarrativeTone Tone;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    ENarrativeFocus Focus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    TArray<FString> CharacterFocus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    TArray<EHistoricalEventType> EventFilter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    int32 MaxLength = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    bool bIncludePlayerActions = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Request")
    bool bGenerateAlternativeEndings = false;

    FNarrativeGenerationRequest()
    {
        Style = ENarrativeStyle::Chronological;
        Tone = ENarrativeTone::Neutral;
        Focus = ENarrativeFocus::Events;
        MaxLength = 1000;
        bIncludePlayerActions = true;
        bGenerateAlternativeEndings = false;
    }
};

/**
 * Historical Narrative Generator - Creates compelling narratives from historical events
 * 
 * This system transforms raw historical event data into engaging narratives
 * using various styles, tones, and focuses to create immersive storytelling.
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSHistoricalNarrativeGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSHistoricalNarrativeGenerator();

    // Narrative Generation
    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    void InitializeNarrativeGenerator();

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FNarrativeStructure GenerateNarrative(const FNarrativeGenerationRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FString GenerateQuickNarrative(const TArray<FHistoricalEventData>& Events, ENarrativeStyle Style = ENarrativeStyle::Chronological);

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FString GenerateCharacterStory(const FString& CharacterName, const FString& TimelineID);

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FString GenerateThemeBasedNarrative(EHistoricalEventType Theme, const FString& TimelineID);

    // Narrative Analysis
    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    TArray<FString> ExtractKeyThemes(const FNarrativeStructure& Narrative) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    TArray<FString> ExtractCharacterNames(const FNarrativeStructure& Narrative) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    float CalculateNarrativeCoherence(const FNarrativeStructure& Narrative) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    ENarrativeTone DetectNarrativeTone(const FString& NarrativeText) const;

    // Narrative Customization
    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FString AdaptNarrativeForAudience(const FString& Narrative, const FString& AudienceType);

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FString LocalizeNarrative(const FString& Narrative, const FString& LanguageCode);

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FString AddNarrativeEmphasis(const FString& Narrative, const TArray<FString>& EmphasisPoints);

    // Narrative Templates
    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    void RegisterNarrativeTemplate(const FString& TemplateID, const FNarrativeStructure& Template);

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FNarrativeStructure GetNarrativeTemplate(const FString& TemplateID) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    TArray<FString> GetAvailableTemplates() const;

    // Narrative Export
    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    bool ExportNarrativeToFile(const FNarrativeStructure& Narrative, const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Narrative Generator")
    FString FormatNarrativeForDisplay(const FNarrativeStructure& Narrative, const FString& Format = TEXT("Standard"));

protected:
    // Narrative generation algorithms
    FString GenerateChronologicalNarrative(const TArray<FHistoricalEventData>& Events);
    FString GenerateThematicNarrative(const TArray<FHistoricalEventData>& Events, EHistoricalEventType Theme);
    FString GenerateBiographicalNarrative(const TArray<FHistoricalEventData>& Events, const FString& Character);
    FString GenerateEpicNarrative(const TArray<FHistoricalEventData>& Events);
    FString GenerateAcademicNarrative(const TArray<FHistoricalEventData>& Events);
    FString GeneratePersonalNarrative(const TArray<FHistoricalEventData>& Events);
    FString GenerateCinematicNarrative(const TArray<FHistoricalEventData>& Events);
    FString GenerateInteractiveNarrative(const TArray<FHistoricalEventData>& Events);

    // Narrative enhancement
    FString AddEmotionalDepth(const FString& BaseNarrative, ENarrativeTone Tone);
    FString AddDescriptiveLanguage(const FString& BaseNarrative, const TArray<FHistoricalEventData>& Events);
    FString AddTransitionalPhrases(const TArray<FString>& Sentences);
    FString AddConcludingRemarks(const FString& BaseNarrative, ENarrativeFocus Focus);

    // Content analysis
    TArray<FString> IdentifyKeyEvents(const TArray<FHistoricalEventData>& Events) const;
    TArray<FString> IdentifyCharacterArcs(const TArray<FHistoricalEventData>& Events) const;
    TArray<FString> IdentifyCausalChains(const TArray<FHistoricalEventData>& Events) const;
    TArray<FString> IdentifyThematicElements(const TArray<FHistoricalEventData>& Events) const;

    // Language processing
    FString ImproveNarrativeFlow(const FString& Narrative);
    FString OptimizeSentenceStructure(const FString& Narrative);
    FString EnhanceVocabulary(const FString& Narrative, ENarrativeStyle Style);
    FString AdjustNarrativePacing(const FString& Narrative, ENarrativeTone Tone);

private:
    UPROPERTY()
    TMap<FString, FNarrativeStructure> NarrativeTemplates;

    UPROPERTY()
    TMap<EHistoricalEventType, TArray<FString>> EventNarrativePatterns;

    UPROPERTY()
    TMap<ENarrativeStyle, TArray<FString>> StyleTemplates;

    UPROPERTY()
    TMap<ENarrativeTone, TArray<FString>> ToneVocabulary;

    UPROPERTY()
    bool bGeneratorInitialized;

    // Helper methods
    FString SelectAppropriateTemplate(const FNarrativeGenerationRequest& Request) const;
    TArray<FHistoricalEventData> FilterEventsByRequest(const TArray<FHistoricalEventData>& Events, const FNarrativeGenerationRequest& Request) const;
    FString GenerateNarrativeTitle(const TArray<FHistoricalEventData>& Events, ENarrativeStyle Style) const;
    void InitializeNarrativePatterns();
    void LoadNarrativeTemplates();
};
