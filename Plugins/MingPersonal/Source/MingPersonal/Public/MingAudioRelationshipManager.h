#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManager.h"
#include "MingMetaSoundsSystem.h"
#include "MingAudioRelationshipManager.generated.h"

UENUM(BlueprintType)
enum class ERelationshipAudioType: uint8 {
    RelationshipIncrease,    // 故事選項?
    RelationshipDecrease,    // 摧毀�U?X???
    RelationshipBreak,       // 故事選項?
    NewFriendship,          // 摧毀�˭�X
    Betrayal,               // 目標數量
    Reconciliation,         // ??��摧毀
    FirstMeeting,           // ??������摧毀
    Farewell                // ??�O摧毀
};

UENUM(BlueprintType)
enum class EReputationAudioType: uint8 {
    ReputationIncrease,     // 故事選項?
    ReputationDecrease,     // 摧毀�U?X???
    NewReputationLevel,     // ??�n摧毀�ŭ�X
    LegendaryAchievement,   // ???X?�N摧毀
    RegionWelcome,          // 摧毀�w?X???
    RegionHostility         // 故事選項?
};

UENUM(BlueprintType)
enum class EDialogueAudioType: uint8 {
    Friendly,               // ??�n���摧毀
    Hostile,                // 摧毀���摧毀
    Neutral,                // ��??���摧毀
    Romantic,               // �������摧毀
    Business,               // ??�~���摧毀
    Intimidation,           // ��??���摧毀
    Persuasion,             // ��?X???
    Request                 // ��??���摧毀
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



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueAudioPlayed, EDialogueAudioType, AudioType, USoundBase*, Sound);

/**
 * ??�W摧毀��?X * �t�d???X?�n??�t��?X?�W摧毀
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAudioRelationshipManager();

    // ???X?��??�t??
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void InitializeAudioRelationshipSystem(UObject* MetaSoundsSystem);

    // ???X?�W摧毀
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void PlayRelationshipAudio(const FRelationshipAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void OnRelationshipTypeChanged(const FString& CharacterID, ERelationshipType OldType, ERelationshipType NewType);

    // ???X?�W摧毀
    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void PlayReputationAudio(const FReputationAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void OnReputationLevelChanged(const FString& RegionID, EReputationLevel OldLevel, EReputationLevel NewLevel);

    // ���??�W摧毀
    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void PlayDialogueAudio(const FDialogueAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void OnDialogueEnded(const FString& CharacterID, bool bWasSuccessful);

    // ��?X???�D?X?�W
    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayRepublicEraTheme(const FString& ThemeName, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayRegionalAmbient(const FString& RegionID, EReputationLevel ReputationLevel);

    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayCharacterTheme(const FString& CharacterID, ERelationshipType RelationshipType);

    // ??�W�]�m
    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetAudioIntensity(float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetRelationshipAudioVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetReputationAudioVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetDialogueAudioVolume(float Volume);

    // ??�W摧毀�d??
    UFUNCTION(BlueprintPure, Category = "Audio Status")
    bool IsAudioSystemInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintPure, Category = "Audio Status")
    float GetAudioIntensity() const { return AudioIntensity; }

    // �ƥ�
    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnRelationshipAudioPlayed OnRelationshipAudioPlayed;

    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnReputationAudioPlayed OnReputationAudioPlayed;

    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnDialogueAudioPlayed OnDialogueAudioPlayed;

protected:
    // ??�W�t�Τޥ�
    UPROPERTY()
    TObjectPtr<UMingMetaSoundsSystem> MetaSoundsSystem;

    // ??�W�]�m
    UPROPERTY()
    float AudioIntensity = 1.0f;

    UPROPERTY()
    float RelationshipAudioVolume = 1.0f;

    UPROPERTY()
    float ReputationAudioVolume = 1.0f;

    UPROPERTY()
    float DialogueAudioVolume = 1.0f;

    // ??X
    UPROPERTY()
    bool bIsInitialized = false;

    // ??��??�W???X?��
    USoundBase* GenerateRelationshipAudio(const FRelationshipAudioParams& Params);
    USoundBase* GenerateReputationAudio(const FReputationAudioParams& Params);
    USoundBase* GenerateDialogueAudio(const FDialogueAudioParams& Params);
    USoundBase* GenerateRepublicEraAudio(const FString& ThemeName, float Intensity);

    // ??�W??�ƭp??
    float CalculateRelationshipIntensity(float RelationshipChange, bool bIsSignificant) const;
    float CalculateReputationIntensity(float ReputationChange, EReputationLevel Level) const;
    float CalculateDialogueIntensity(ERelationshipType RelationshipType, float EmotionalIntensity) const;

    // ??�W??��
    void PlayAudioAtLocation(USoundBase* Sound, FVector Location, float VolumeMultiplier = 1.0f);
    void PlayAudio2D(USoundBase* Sound, float VolumeMultiplier = 1.0f);

    // ��?X??X?�W�D??
    UPROPERTY()
    TMap<FString, FString> RepublicEraThemes;

    UPROPERTY()
    TMap<ERelationshipAudioType, FString> RelationshipAudioThemes;

    UPROPERTY()
    TMap<EReputationAudioType, FString> ReputationAudioThemes;

    UPROPERTY()
    TMap<EDialogueAudioType, FString> DialogueAudioThemes;

private:
    // ???X?�D??
    void InitializeRepublicEraThemes();
    void InitializeRelationshipAudioThemes();
    void InitializeReputationAudioThemes();
    void InitializeDialogueAudioThemes();

    // ???X?�W�D??��??
    FString GetRelationshipAudioPath(ERelationshipAudioType AudioType) const;
    FString GetReputationAudioPath(EReputationAudioType AudioType) const;
    FString GetDialogueAudioPath(EDialogueAudioType AudioType) const;
    FString GetRepublicEraThemePath(const FString& ThemeName) const;
};

