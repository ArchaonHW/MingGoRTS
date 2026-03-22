#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManager.h"
#include "MingMetaSoundsSystem.h"
#include "MingAudioRelationshipManager.generated.h"

UENUM(BlueprintType)
enum class ERelationshipAudioType : uint8
{
    RelationshipIncrease,    // ???X??X???
    RelationshipDecrease,    // ????下?X???
    RelationshipBreak,       // ???X??X???
    NewFriendship,          // ????誼音X    Betrayal,               // ???X???
    Reconciliation,         // ??解????
    FirstMeeting,           // ??次見面????
    Farewell                // ??別????
};

UENUM(BlueprintType)
enum class EReputationAudioType : uint8
{
    ReputationIncrease,     // ???X??X???
    ReputationDecrease,     // ????下?X???
    NewReputationLevel,     // ??聲????級音X    LegendaryAchievement,   // ???X?就????
    RegionWelcome,          // ????歡?X???
    RegionHostility         // ???X??X???
};

UENUM(BlueprintType)
enum class EDialogueAudioType : uint8
{
    Friendly,               // ??好對話????
    Hostile,                // ????對話????
    Neutral,                // 中??對話????
    Romantic,               // 浪漫對話????
    Business,               // ??業對話????
    Intimidation,           // 威??對話????
    Persuasion,             // 說?X???
    Request                 // 請??對話????
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipAudioPlayed, ERelationshipAudioType, AudioType, USoundBase*, Sound};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReputationAudioPlayed, EReputationAudioType, AudioType, USoundBase*, Sound};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueAudioPlayed, EDialogueAudioType, AudioType, USoundBase*, Sound);

/**
 * ??頻????管?X * 負責???X?聲??系統?X?頻????
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAudioRelationshipManager();

    // ???X?音??系??    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void InitializeAudioRelationshipSystem(UObject* MetaSoundsSystem);

    // ???X?頻????
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void PlayRelationshipAudio(const FRelationshipAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void OnRelationshipTypeChanged(const FString& CharacterID, ERelationshipType OldType, ERelationshipType NewType);

    // ???X?頻????
    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void PlayReputationAudio(const FReputationAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void OnReputationLevelChanged(const FString& RegionID, EReputationLevel OldLevel, EReputationLevel NewLevel);

    // 對話??頻????
    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void PlayDialogueAudio(const FDialogueAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void OnDialogueEnded(const FString& CharacterID, bool bWasSuccessful);

    // 民?X???主?X?頻
    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayRepublicEraTheme(const FString& ThemeName, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayRegionalAmbient(const FString& RegionID, EReputationLevel ReputationLevel);

    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayCharacterTheme(const FString& CharacterID, ERelationshipType RelationshipType);

    // ??頻設置
    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetAudioIntensity(float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetRelationshipAudioVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetReputationAudioVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetDialogueAudioVolume(float Volume);

    // ??頻????查??    UFUNCTION(BlueprintPure, Category = "Audio Status")
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
    // ??頻系統引用
    UPROPERTY()
    TObjectPtr<UMingMetaSoundsSystem> MetaSoundsSystem;

    // ??頻設置
    UPROPERTY()
    float AudioIntensity = 1.0f;

    UPROPERTY()
    float RelationshipAudioVolume = 1.0f;

    UPROPERTY()
    float ReputationAudioVolume = 1.0f;

    UPROPERTY()
    float DialogueAudioVolume = 1.0f;

    // ??X    UPROPERTY()
    bool bIsInitialized = false;

    // ??部??頻???X?數
    USoundBase* GenerateRelationshipAudio(const FRelationshipAudioParams& Params};
    USoundBase* GenerateReputationAudio(const FReputationAudioParams& Params};
    USoundBase* GenerateDialogueAudio(const FDialogueAudioParams& Params};
    USoundBase* GenerateRepublicEraAudio(const FString& ThemeName, float Intensity);

    // ??頻??數計??
    float CalculateRelationshipIntensity(float RelationshipChange, bool bIsSignificant) const;
    float CalculateReputationIntensity(float ReputationChange, EReputationLevel Level) const;
    float CalculateDialogueIntensity(ERelationshipType RelationshipType, float EmotionalIntensity) const;

    // ??頻??放
    void PlayAudioAtLocation(USoundBase* Sound, FVector Location, float VolumeMultiplier = 1.0f};
    void PlayAudio2D(USoundBase* Sound, float VolumeMultiplier = 1.0f);

    // 民?X??X?頻主??
    UPROPERTY()
    TMap<FString, FString> RepublicEraThemes;

    UPROPERTY()
    TMap<ERelationshipAudioType, FString> RelationshipAudioThemes;

    UPROPERTY()
    TMap<EReputationAudioType, FString> ReputationAudioThemes;

    UPROPERTY()
    TMap<EDialogueAudioType, FString> DialogueAudioThemes;

private:
    // ???X?主??    void InitializeRepublicEraThemes();
    void InitializeRelationshipAudioThemes();
    void InitializeReputationAudioThemes();
    void InitializeDialogueAudioThemes();

    // ???X?頻主??路??
    FString GetRelationshipAudioPath(ERelationshipAudioType AudioType) const;
    FString GetReputationAudioPath(EReputationAudioType AudioType) const;
    FString GetDialogueAudioPath(EDialogueAudioType AudioType) const;
    FString GetRepublicEraThemePath(const FString& ThemeName) const;
};

