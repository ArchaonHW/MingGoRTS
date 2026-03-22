#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUniversityGuideManager.generated.h"

/**
 * ���դ�?X?�e��??
 */
UENUM(BlueprintType)
enum class EMingUniversityContentType: uint8 {
    CampusTour      UMETA(DisplayName = "摧毀����"),
    AcademicIntro   UMETA(DisplayName = "��??����"),
    HistoricalGuide UMETA(DisplayName = "���v����"),
    CulturalIntro   UMETA(DisplayName = "摧毀����"),
    GameTutorial    UMETA(DisplayName = "??��摧毀"),
    ResearchShowcase UMETA(DisplayName = "??�s摧毀�i��"),
    StudentLife     UMETA(DisplayName = "��?X?��"),
    FacultyIntro    UMETA(DisplayName = "�v??����"),
    Achievement     UMETA(DisplayName = "??�N�i��"),
    InteractiveMap  UMETA(DisplayName = "��?X???")
};

/**
 * ���դ�?X?�m
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
 * ���դ�?X?�e摧毀
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
 * ���դ�??��?X * ��?X?��摧毀���դ�?X??X?��?? */
UCLASS()
class MINGPERSONAL_API UMingUniversityGuideManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUniversityGuideManager();

    virtual void Initialize();
    virtual void Shutdown();

    /**
     * 摧毀���դ�??
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StartUniversityGuide();

    /**
     * ??��դ�??
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopUniversityGuide();

    /**
     * �]�m��?X?�m
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetGuideConfig(const FUniversityGuideConfig& Config);

    /**
     * 故事重要性?�m
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FUniversityGuideConfig GetGuideConfig() const;

    /**
     * 摧毀��?X?�e
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FString GenerateGuideContent(const FUniversityGuideConfig& Config);

    /**
     * ��ܤU?X???�ɤ�??     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void ShowNextContent();

    /**
     * ��ܤW?X???�ɤ�??     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void ShowPreviousContent();

    /**
     * ��?X???�w��??     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void JumpToContent(const FString& ContentID);

    /**
     * ��?X?�e??�w��??
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void MarkContentAsCompleted(const FString& ContentID);

    /**
     * 目標數量��??     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    TArray<FUniversityGuideContent> GetAllContents() const;

    /**
     * 故事重要性?�e
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FUniversityGuideContent GetCurrentContent() const;

    /**
     * 摧毀��?X?��
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    float GetCompletionProgress() const;

    /**
     * ??���?X?�W
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void PlayGuideAudio(const FString& AudioClipID);

    /**
     * ??���?X?�W
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopGuideAudio();

    /**
     * �]�m��?X???
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetGuideVolume(float Volume);

    /**
     * ??��/�T�Τ�??��??
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetInteractiveMode(bool bEnabled);

    /**
     * �ˬd??�_摧毀摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    bool IsGuideRunning() const;

    /**
     * �O??��?X?��
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SaveGuideProgress();

    /**
     * ���J��?X?��
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void LoadGuideProgress();

    // �ƥ�摧毀
    
    
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
);

