#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUniversityGuideManager.generated.h"

/**
 * È´òÊ†°ÂºïÂ??ßÂÆπÈ°ûÂ?
 */
UENUM(BlueprintType)
enum class EMingUniversityContentType : uint8
{
    CampusTour      UMETA(DisplayName = "?°Â?Â∞éË¶Ω"),
    AcademicIntro   UMETA(DisplayName = "Â≠∏Ë?‰ªãÁ¥π"),
    HistoricalGuide UMETA(DisplayName = "Ê≠∑Âè≤Â∞éË¶Ω"),
    CulturalIntro   UMETA(DisplayName = "?áÂ?‰ªãÁ¥π"),
    GameTutorial    UMETA(DisplayName = "?äÊà≤?ôÁ?"),
    ResearchShowcase UMETA(DisplayName = "?îÁ©∂?êÊ?Â±ïÁ§∫"),
    StudentLife     UMETA(DisplayName = "Â≠∏Á??üÊ¥ª"),
    FacultyIntro    UMETA(DisplayName = "Â∏´Ë?‰ªãÁ¥π"),
    Achievement     UMETA(DisplayName = "?êÂ∞±Â±ïÁ§∫"),
    InteractiveMap  UMETA(DisplayName = "‰∫íÂ??∞Â?")
};

/**
 * È´òÊ†°ÂºïÂ??çÁΩÆ
 */
USTRUCT(BlueprintType)
struct FMingUniversityGuideConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    EMingUniversityContentType ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    FString UniversityName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    FString Department;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    FString TargetAudience;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    FString Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    FString Language;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    FString CulturalContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    bool bInteractive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    bool bIncludeHistoricalContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    bool bIncludeStudentTestimonials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    TArray<FString> KeyTopics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "University Guide")
    FString GuideDescription;

    FUniversityGuideConfig()
        : ContentType(EUniversityContentType::CampusTour)
        , bInteractive(true)
        , bIncludeHistoricalContent(true)
        , bIncludeStudentTestimonials(false)
    {}
};

/**
 * È´òÊ†°ÂºïÂ??ßÂÆπ?∏Ê?
 */
USTRUCT(BlueprintType)
struct FUniversityGuideContent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "University Guide")
    FString ContentID;

    UPROPERTY(BlueprintReadOnly, Category = "University Guide")
    FString Title;

    UPROPERTY(BlueprintReadOnly, Category = "University Guide")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "University Guide")
    TArray<FString> MediaAssets;

    UPROPERTY(BlueprintReadOnly, Category = "University Guide")
    TArray<FString> AudioClips;

    UPROPERTY(BlueprintReadOnly, Category = "University Guide")
    bool bIsCompleted;

    UPROPERTY(BlueprintReadOnly, Category = "University Guide")
    float Progress;

    FUniversityGuideContent()
        : bIsCompleted(false)
        , Progress(0.0f)
    {}
};

/**
 * È´òÊ†°ÂºïÂ?ÁÆ°Á??? * ÁÆ°Á??äÊà≤?ßÁ?È´òÊ†°ÂºïÂ??åÊ??≤ÂÖßÂÆ? */
UCLASS()
class MINGPERSONAL_API UMingUniversityGuideManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUniversityGuideManager();

    virtual void Initialize();
    virtual void Shutdown();

    /**
     * ?üÂ?È´òÊ†°ÂºïÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StartUniversityGuide();

    /**
     * ?úÊ≠¢È´òÊ†°ÂºïÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopUniversityGuide();

    /**
     * Ë®≠ÁΩÆÂºïÂ??çÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetGuideConfig(const FUniversityGuideConfig& Config);

    /**
     * ?≤Â??∂Â??çÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FUniversityGuideConfig GetGuideConfig() const;

    /**
     * ?üÊ?ÂºïÂ??ßÂÆπ
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FString GenerateGuideContent(const FUniversityGuideConfig& Config);

    /**
     * È°ØÁ§∫‰∏ã‰??ãÂ?Â∞éÂÖßÂÆ?     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void ShowNextContent();

    /**
     * È°ØÁ§∫‰∏ä‰??ãÂ?Â∞éÂÖßÂÆ?     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void ShowPreviousContent();

    /**
     * Ë∑≥Ë??∞Ê?ÂÆöÂÖßÂÆ?     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void JumpToContent(const FString& ContentID);

    /**
     * Ê®ôË??ßÂÆπ?∫Â∑≤ÂÆåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void MarkContentAsCompleted(const FString& ContentID);

    /**
     * ?≤Â??Ä?âÂÖßÂÆ?     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    TArray<FUniversityGuideContent> GetAllContents() const;

    /**
     * ?≤Â??∂Â??ßÂÆπ
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FUniversityGuideContent GetCurrentContent() const;

    /**
     * ?≤Â?ÂÆåÊ??≤Â∫¶
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    float GetCompletionProgress() const;

    /**
     * ?≠ÊîæÂºïÂ??≥È†ª
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void PlayGuideAudio(const FString& AudioClipID);

    /**
     * ?úÊ≠¢ÂºïÂ??≥È†ª
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopGuideAudio();

    /**
     * Ë®≠ÁΩÆÂºïÂ??≥È?
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetGuideVolume(float Volume);

    /**
     * ?üÁî®/Á¶ÅÁî®‰∫íÂ?Ê®°Â?
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetInteractiveMode(bool bEnabled);

    /**
     * Ê™¢Êü•?ØÂê¶Ê≠?ú®?ãË?
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    bool IsGuideRunning() const;

    /**
     * ‰øùÂ?ÂºïÂ??≤Â∫¶
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SaveGuideProgress();

    /**
     * ËºâÂÖ•ÂºïÂ??≤Â∫¶
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void LoadGuideProgress();

    // ‰∫ã‰ª∂?≤Ê?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuideStarted, FString, GuideID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuideCompleted, FString, GuideID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContentChanged, FUniversityGuideContent, Content);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProgressUpdated, FString, ContentID, float, Progress);

    UPROPERTY(BlueprintAssignable, Category = "University Guide|Events")
    FOnGuideStarted OnGuideStarted;

    UPROPERTY(BlueprintAssignable, Category = "University Guide|Events")
    FOnGuideCompleted OnGuideCompleted;

    UPROPERTY(BlueprintAssignable, Category = "University Guide|Events")
    FOnContentChanged OnContentChanged;

    UPROPERTY(BlueprintAssignable, Category = "University Guide|Events")
    FOnProgressUpdated OnProgressUpdated;

private:
    UPROPERTY()
    FUniversityGuideConfig CurrentConfig;

    UPROPERTY()
    TArray<FUniversityGuideContent> GuideContents;

    UPROPERTY()
    int32 CurrentContentIndex;

    UPROPERTY()
    bool bIsRunning;

    UPROPERTY()
    float GuideVolume;

    UPROPERTY()
    bool bInteractiveMode;

    void GenerateDefaultContents();
    void UpdateProgress();
    FString GenerateContentID();
};
