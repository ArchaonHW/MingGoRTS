#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManager.h"
#include "MingMetaSoundsSystem.h"
#include "MingAudioRelationshipManager.generated.h"

UENUM(BlueprintType)
enum class ERelationshipAudioType : uint8
{
    RelationshipIncrease,    // ?ú‰??êÂ??≥Ê?
    RelationshipDecrease,    // ?ú‰?‰∏ãÈ??≥Ê?
    RelationshipBreak,       // ?ú‰??¥Ë??≥Ê?
    NewFriendship,          // ?∞Â?Ë™ºÈü≥??    Betrayal,               // ?åÂ??≥Ê?
    Reconciliation,         // ?åËß£?≥Ê?
    FirstMeeting,           // ?ùÊ¨°Ë¶ãÈù¢?≥Ê?
    Farewell                // ?äÂà•?≥Ê?
};

UENUM(BlueprintType)
enum class EReputationAudioType : uint8
{
    ReputationIncrease,     // ?≤Ê??êÂ??≥Ê?
    ReputationDecrease,     // ?≤Ê?‰∏ãÈ??≥Ê?
    NewReputationLevel,     // ?∞ËÅ≤?õÁ?Á¥öÈü≥??    LegendaryAchievement,   // ?≥Â??êÂ∞±?≥Ê?
    RegionWelcome,          // ?∞Â?Ê≠°Ë??≥Ê?
    RegionHostility         // ?∞Â??µÊ??≥Ê?
};

UENUM(BlueprintType)
enum class EDialogueAudioType : uint8
{
    Friendly,               // ?ãÂ•ΩÂ∞çË©±?≥Ê?
    Hostile,                // ?µÂ?Â∞çË©±?≥Ê?
    Neutral,                // ‰∏≠Á?Â∞çË©±?≥Ê?
    Romantic,               // Êµ™Êº´Â∞çË©±?≥Ê?
    Business,               // ?ÜÊ•≠Â∞çË©±?≥Ê?
    Intimidation,           // Â®ÅË?Â∞çË©±?≥Ê?
    Persuasion,             // Ë™™Ê??≥Ê?
    Request                 // Ë´ãÊ?Â∞çË©±?≥Ê?
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
 * ?≥È†ª?ú‰?ÁÆ°Á??? * Ë≤†Ë≤¨?ú‰??åËÅ≤?õÁ≥ªÁµ±Á??≥È†ª?çÈ?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAudioRelationshipManager();

    // ?ùÂ??ñÈü≥?ªÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void InitializeAudioRelationshipSystem(UObject* MetaSoundsSystem);

    // ?ú‰??≥È†ª?ïÁ?
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void PlayRelationshipAudio(const FRelationshipAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void OnRelationshipTypeChanged(const FString& CharacterID, ERelationshipType OldType, ERelationshipType NewType);

    // ?≤Ê??≥È†ª?ïÁ?
    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void PlayReputationAudio(const FReputationAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Audio Reputation")
    void OnReputationLevelChanged(const FString& RegionID, EReputationLevel OldLevel, EReputationLevel NewLevel);

    // Â∞çË©±?≥È†ª?ïÁ?
    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void PlayDialogueAudio(const FDialogueAudioParams& Params);

    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Audio Dialogue")
    void OnDialogueEnded(const FString& CharacterID, bool bWasSuccessful);

    // Ê∞ëÂ??ÇÊ?‰∏ªÈ??≥È†ª
    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayRepublicEraTheme(const FString& ThemeName, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayRegionalAmbient(const FString& RegionID, EReputationLevel ReputationLevel);

    UFUNCTION(BlueprintCallable, Category = "Audio Theme")
    void PlayCharacterTheme(const FString& CharacterID, ERelationshipType RelationshipType);

    // ?≥È†ªË®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetAudioIntensity(float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetRelationshipAudioVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetReputationAudioVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetDialogueAudioVolume(float Volume);

    // ?≥È†ª?Ä?ãÊü•Ë©?    UFUNCTION(BlueprintPure, Category = "Audio Status")
    bool IsAudioSystemInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintPure, Category = "Audio Status")
    float GetAudioIntensity() const { return AudioIntensity; }

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnRelationshipAudioPlayed OnRelationshipAudioPlayed;

    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnReputationAudioPlayed OnReputationAudioPlayed;

    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnDialogueAudioPlayed OnDialogueAudioPlayed;

protected:
    // ?≥È†ªÁ≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UMingMetaSoundsSystem> MetaSoundsSystem;

    // ?≥È†ªË®≠ÁΩÆ
    UPROPERTY()
    float AudioIntensity = 1.0f;

    UPROPERTY()
    float RelationshipAudioVolume = 1.0f;

    UPROPERTY()
    float ReputationAudioVolume = 1.0f;

    UPROPERTY()
    float DialogueAudioVolume = 1.0f;

    // ?Ä??    UPROPERTY()
    bool bIsInitialized = false;

    // ?ßÈÉ®?≥È†ª?üÊ??ΩÊï∏
    USoundBase* GenerateRelationshipAudio(const FRelationshipAudioParams& Params);
    USoundBase* GenerateReputationAudio(const FReputationAudioParams& Params);
    USoundBase* GenerateDialogueAudio(const FDialogueAudioParams& Params);
    USoundBase* GenerateRepublicEraAudio(const FString& ThemeName, float Intensity);

    // ?≥È†ª?ÉÊï∏Ë®àÁ?
    float CalculateRelationshipIntensity(float RelationshipChange, bool bIsSignificant) const;
    float CalculateReputationIntensity(float ReputationChange, EReputationLevel Level) const;
    float CalculateDialogueIntensity(ERelationshipType RelationshipType, float EmotionalIntensity) const;

    // ?≥È†ª?≠Êîæ
    void PlayAudioAtLocation(USoundBase* Sound, FVector Location, float VolumeMultiplier = 1.0f);
    void PlayAudio2D(USoundBase* Sound, float VolumeMultiplier = 1.0f);

    // Ê∞ëÂ??ÇÊ??≥È†ª‰∏ªÈ?
    UPROPERTY()
    TMap<FString, FString> RepublicEraThemes;

    UPROPERTY()
    TMap<ERelationshipAudioType, FString> RelationshipAudioThemes;

    UPROPERTY()
    TMap<EReputationAudioType, FString> ReputationAudioThemes;

    UPROPERTY()
    TMap<EDialogueAudioType, FString> DialogueAudioThemes;

private:
    // ?ùÂ??ñ‰∏ªÈ°?    void InitializeRepublicEraThemes();
    void InitializeRelationshipAudioThemes();
    void InitializeReputationAudioThemes();
    void InitializeDialogueAudioThemes();

    // ?≤Â??≥È†ª‰∏ªÈ?Ë∑ØÂ?
    FString GetRelationshipAudioPath(ERelationshipAudioType AudioType) const;
    FString GetReputationAudioPath(EReputationAudioType AudioType) const;
    FString GetDialogueAudioPath(EDialogueAudioType AudioType) const;
    FString GetRepublicEraThemePath(const FString& ThemeName) const;
};
