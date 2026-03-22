#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManager.h"
#include "MingMetaSoundsSystem.h"
#include "MingAudioRelationshipManager.generated.h"

UENUM(BlueprintType)
enum class ERelationshipAudioType : uint8
{
    RelationshipIncrease,    // 關係提升音效
    RelationshipDecrease,    // 關係下降音效
    RelationshipBreak,       // 關係破裂音效
    NewFriendship,          // 新友誼音效
    Betrayal,               // 背叛音效
    Reconciliation,         // 和解音效
    FirstMeeting,           // 初次見面音效
    Farewell                // 告別音效
};

UENUM(BlueprintType)
enum class EReputationAudioType : uint8
{
    ReputationIncrease,     // 聲望提升音效
    ReputationDecrease,     // 聲望下降音效
    NewReputationLevel,     // 新聲望等級音效
    LegendaryAchievement,   // 傳奇成就音效
    RegionWelcome,          // 地區歡迎音效
    RegionHostility         // 地區敵意音效
};

UENUM(BlueprintType)
enum class EDialogueAudioType : uint8
{
    Friendly,               // 友好對話音效
    Hostile,                // 敵對對話音效
    Neutral,                // 中立對話音效
    Romantic,               // 浪漫對話音效
    Business,               // 商業對話音效
    Intimidation,           // 威脅對話音效
    Persuasion,             // 說服音效
    Request                 // 請求對話音效
};

USTRUCT(BlueprintType)
struct FRelationshipAudioParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Audio")
    ERelationshipAudioType AudioType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Audio")
    ERelationshipType RelationshipType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Audio")
    float RelationshipChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Audio")
    float Intensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Audio")
    bool bIsSignificantChange = false;

    FRelationshipAudioParams()
    {
        AudioType = ERelationshipAudioType::RelationshipIncrease;
        RelationshipType = ERelationshipType::Neutral;
        RelationshipChange = 0.0f;
        Intensity = 1.0f;
        bIsSignificantChange = false;
    }
};

USTRUCT(BlueprintType)
struct FReputationAudioParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation Audio")
    EReputationAudioType AudioType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation Audio")
    EReputationLevel CurrentLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation Audio")
    EReputationLevel PreviousLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation Audio")
    float ReputationChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation Audio")
    FString RegionID;

    FReputationAudioParams()
    {
        AudioType = EReputationAudioType::ReputationIncrease;
        CurrentLevel = EReputationLevel::Unknown;
        PreviousLevel = EReputationLevel::Unknown;
        ReputationChange = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FDialogueAudioParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Audio")
    EDialogueAudioType DialogueType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Audio")
    ERelationshipType RelationshipType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Audio")
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Audio")
    float EmotionalIntensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Audio")
    bool bIsImportant = false;

    FDialogueAudioParams()
    {
        DialogueType = EDialogueAudioType::Neutral;
        RelationshipType = ERelationshipType::Neutral;
        EmotionalIntensity = 1.0f;
        bIsImportant = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipAudioPlayed, ERelationshipAudioType, AudioType, USoundBase*, Sound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReputationAudioPlayed, EReputationAudioType, AudioType, USoundBase*, Sound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueAudioPlayed, EDialogueAudioType, AudioType, USoundBase*, Sound);

/**
 * 音頻關係管理器
 * 負責關係和聲望系統的音頻反饋
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAudioRelationshipManager();

    // 初始化音頻系統
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void InitializeAudioRelationshipSystem(UObject* MetaSoundsSystem);

    // 關係音頻處理
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void PlayRelationshipAudio(const FRelationshipAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void OnRelationshipTypeChanged(const FString& CharacterID, ERelationshipType OldType, ERelationshipType NewType);

    // 聲望音頻處理
    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void PlayReputationAudio(const FReputationAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void OnReputationLevelChanged(const FString& RegionID, EReputationLevel OldLevel, EReputationLevel NewLevel);

    // 對話音頻處理
    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void PlayDialogueAudio(const FDialogueAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void OnDialogueEnded(const FString& CharacterID, bool bWasSuccessful);

    // 民國時期主題音頻
    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayRepublicEraTheme(const FString& ThemeName, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayRegionalAmbient(const FString& RegionID, EReputationLevel ReputationLevel);

    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayCharacterTheme(const FString& CharacterID, ERelationshipType RelationshipType);

    // 音頻設置
    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetAudioIntensity(float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetRelationshipAudioVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetReputationAudioVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetDialogueAudioVolume(float Volume);

    // 音頻狀態查詢
    UFUNCTION(BlueprintPure, Category = "Audio Status")
    bool IsAudioSystemInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintPure, Category = "Audio Status")
    float GetAudioIntensity() const { return AudioIntensity; }

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnRelationshipAudioPlayed OnRelationshipAudioPlayed;

    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnReputationAudioPlayed OnReputationAudioPlayed;

    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnDialogueAudioPlayed OnDialogueAudioPlayed;

protected:
    // 音頻系統引用
    UPROPERTY()
    TObjectPtr<UMingMetaSoundsSystem> MetaSoundsSystem;

    // 音頻設置
    UPROPERTY()
    float AudioIntensity = 1.0f;

    UPROPERTY()
    float RelationshipAudioVolume = 1.0f;

    UPROPERTY()
    float ReputationAudioVolume = 1.0f;

    UPROPERTY()
    float DialogueAudioVolume = 1.0f;

    // 狀態
    UPROPERTY()
    bool bIsInitialized = false;

    // 內部音頻生成函數
    USoundBase* GenerateRelationshipAudio(const FRelationshipAudioParams& Params);
    USoundBase* GenerateReputationAudio(const FReputationAudioParams& Params);
    USoundBase* GenerateDialogueAudio(const FDialogueAudioParams& Params);
    USoundBase* GenerateRepublicEraAudio(const FString& ThemeName, float Intensity);

    // 音頻參數計算
    float CalculateRelationshipIntensity(float RelationshipChange, bool bIsSignificant) const;
    float CalculateReputationIntensity(float ReputationChange, EReputationLevel Level) const;
    float CalculateDialogueIntensity(ERelationshipType RelationshipType, float EmotionalIntensity) const;

    // 音頻播放
    void PlayAudioAtLocation(USoundBase* Sound, FVector Location, float VolumeMultiplier = 1.0f);
    void PlayAudio2D(USoundBase* Sound, float VolumeMultiplier = 1.0f);

    // 民國時期音頻主題
    UPROPERTY()
    TMap<FString, FString> RepublicEraThemes;

    UPROPERTY()
    TMap<ERelationshipAudioType, FString> RelationshipAudioThemes;

    UPROPERTY()
    TMap<EReputationAudioType, FString> ReputationAudioThemes;

    UPROPERTY()
    TMap<EDialogueAudioType, FString> DialogueAudioThemes;

private:
    // 初始化主題
    void InitializeRepublicEraThemes();
    void InitializeRelationshipAudioThemes();
    void InitializeReputationAudioThemes();
    void InitializeDialogueAudioThemes();

    // 獲取音頻主題路徑
    FString GetRelationshipAudioPath(ERelationshipAudioType AudioType) const;
    FString GetReputationAudioPath(EReputationAudioType AudioType) const;
    FString GetDialogueAudioPath(EDialogueAudioType AudioType) const;
    FString GetRepublicEraThemePath(const FString& ThemeName) const;
};
