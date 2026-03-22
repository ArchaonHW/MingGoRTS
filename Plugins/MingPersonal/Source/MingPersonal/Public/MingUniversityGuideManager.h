#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUniversityGuideManager.generated.h"

/**
 * 高校引導內容類型
 */
UENUM(BlueprintType)
enum class EMingUniversityContentType : uint8
{
    CampusTour      UMETA(DisplayName = "校園導覽"),
    AcademicIntro   UMETA(DisplayName = "學術介紹"),
    HistoricalGuide UMETA(DisplayName = "歷史導覽"),
    CulturalIntro   UMETA(DisplayName = "文化介紹"),
    GameTutorial    UMETA(DisplayName = "遊戲教程"),
    ResearchShowcase UMETA(DisplayName = "研究成果展示"),
    StudentLife     UMETA(DisplayName = "學生生活"),
    FacultyIntro    UMETA(DisplayName = "師資介紹"),
    Achievement     UMETA(DisplayName = "成就展示"),
    InteractiveMap  UMETA(DisplayName = "互動地圖")
};

/**
 * 高校引導配置
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
 * 高校引導內容數據
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
 * 高校引導管理器
 * 管理遊戲內的高校引導和教育內容
 */
UCLASS()
class MINGPERSONAL_API UMingUniversityGuideManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUniversityGuideManager();

    virtual void Initialize();
    virtual void Shutdown();

    /**
     * 啟動高校引導
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StartUniversityGuide();

    /**
     * 停止高校引導
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopUniversityGuide();

    /**
     * 設置引導配置
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetGuideConfig(const FUniversityGuideConfig& Config);

    /**
     * 獲取當前配置
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FUniversityGuideConfig GetGuideConfig() const;

    /**
     * 生成引導內容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FString GenerateGuideContent(const FUniversityGuideConfig& Config);

    /**
     * 顯示下一個引導內容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void ShowNextContent();

    /**
     * 顯示上一個引導內容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void ShowPreviousContent();

    /**
     * 跳轉到指定內容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void JumpToContent(const FString& ContentID);

    /**
     * 標記內容為已完成
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void MarkContentAsCompleted(const FString& ContentID);

    /**
     * 獲取所有內容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    TArray<FUniversityGuideContent> GetAllContents() const;

    /**
     * 獲取當前內容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    FUniversityGuideContent GetCurrentContent() const;

    /**
     * 獲取完成進度
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    float GetCompletionProgress() const;

    /**
     * 播放引導音頻
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void PlayGuideAudio(const FString& AudioClipID);

    /**
     * 停止引導音頻
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopGuideAudio();

    /**
     * 設置引導音量
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetGuideVolume(float Volume);

    /**
     * 啟用/禁用互動模式
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SetInteractiveMode(bool bEnabled);

    /**
     * 檢查是否正在運行
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    bool IsGuideRunning() const;

    /**
     * 保存引導進度
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void SaveGuideProgress();

    /**
     * 載入引導進度
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void LoadGuideProgress();

    // 事件聲明
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
