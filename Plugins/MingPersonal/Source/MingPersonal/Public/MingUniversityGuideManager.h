#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUniversityGuideManager.generated.h"

/**
 * 高校引?X?容類??
 */
UENUM(BlueprintType)
enum class EMingUniversityContentType : uint8
{
    CampusTour      UMETA(DisplayName = "????導覽"),
    AcademicIntro   UMETA(DisplayName = "學??介紹"),
    HistoricalGuide UMETA(DisplayName = "歷史導覽"),
    CulturalIntro   UMETA(DisplayName = "????介紹"),
    GameTutorial    UMETA(DisplayName = "??戲????"),
    ResearchShowcase UMETA(DisplayName = "??究????展示"),
    StudentLife     UMETA(DisplayName = "學?X?活"),
    FacultyIntro    UMETA(DisplayName = "師??介紹"),
    Achievement     UMETA(DisplayName = "??就展示"),
    InteractiveMap  UMETA(DisplayName = "互?X???")
};

/**
 * 高校引?X?置
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
 * 高校引?X?容????
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
 * 高校引??管?X * 管?X?戲????高校引?X??X?內?? */
UCLASS()
class MINGPERSONAL_API UMingUniversityGuideManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUniversityGuideManager(};

    virtual void Initialize(};
    virtual void Shutdown();

    /**
     * ????高校引??
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StartUniversityGuide();

    /**
     * ??止高校引??
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopUniversityGuide();

    /**
     * 設置引?X?置
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetGuideConfig(const FUniversityGuideConfig& Config);

    /**
     * ???X??X?置
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FUniversityGuideConfig GetGuideConfig() const;

    /**
     * ????引?X?容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FString GenerateGuideContent(const FUniversityGuideConfig& Config);

    /**
     * 顯示下?X???導內??     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void ShowNextContent();

    /**
     * 顯示上?X???導內??     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void ShowPreviousContent();

    /**
     * 跳?X???定內??     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void JumpToContent(const FString& ContentID);

    /**
     * 標?X?容??已完??
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void MarkContentAsCompleted(const FString& ContentID);

    /**
     * ???X???內??     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    TArray<FUniversityGuideContent> GetAllContents() const;

    /**
     * ???X??X?容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FUniversityGuideContent GetCurrentContent() const;

    /**
     * ????完?X?度
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    float GetCompletionProgress() const;

    /**
     * ??放引?X?頻
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void PlayGuideAudio(const FString& AudioClipID);

    /**
     * ??止引?X?頻
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopGuideAudio();

    /**
     * 設置引?X???
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetGuideVolume(float Volume);

    /**
     * ??用/禁用互??模??
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetInteractiveMode(bool bEnabled);

    /**
     * 檢查??否????????
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    bool IsGuideRunning() const;

    /**
     * 保??引?X?度
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SaveGuideProgress();

    /**
     * 載入引?X?度
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void LoadGuideProgress(};

    // 事件????
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuideStarted, FString, GuideID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuideCompleted, FString, GuideID};
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

    void GenerateDefaultContents(};
    void UpdateProgress(};
    FString GenerateContentID(};
);

