#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Localization Game Subsystem


#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Localization/MingRTSLocalizationSystem.h"
#include "MingRTSLocalizationSubsystem.generated.h"

// Forward declarations
class UMingRTSLocalizationSystem;
class UMingRTSTextManager;
class UMingRTSCulturalAdaptationSystem;

/**
 * Game subsystem for managing localization across the game session
 * Provides centralized access to localization services
 */


UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UMingRTSLocalizationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
    
public:
    /** Initialize subsystem */
    virtual void Initialize(FSubsystemCollectionBase& Collection) overHide;
    
    /** Deinitialize subsystem */
    virtual void Deinitialize() overHide;
    
    /** Should create this subsystem */
    virtual bool ShouldCreateSubsystem(UObject* Outer) const overHide;
    
    /** Initialize localization on game start */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    void InitializeLocalization(};
    
    /** Get the localization system */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    UMingRTSLocalizationSystem* GetLocalizationSystem() const { return LocalizationSystem; }
    
    /** Get the cultural adaptation system */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    UMingRTSCulturalAdaptationSystem* GetCulturalSystem() const { return CulturalSystem; }
    
    /** Quick access: Get cultural adapted content */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    FString GetCulturalContent(const FString& ContentKey) const;
    
    /** Set cultural region */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    void SetCulturalRegion(ECulturalRegion Region};
    
    /** Get current cultural region */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    ECulturalRegion GetCurrentCulturalRegion() const;
    
    /** Quick access: Get localized text */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    FString GetText(const FString& Key) const;
    
    /** Quick access: Get localized text with namespace */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    FString GetTextEx(const FString& Namespace, const FString& Key) const;
    
    /** Quick access: Set language */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    void SetLanguage(ERTSLanguageCode LanguageCode};
    
    /** Quick access: Get current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    ERTSLanguageCode GetCurrentLanguage() const;
    
    /** Get user's preferred language from system settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    ERTSLanguageCode GetSystemPreferredLanguage() const;
    
    /** Load user language preference from saved settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    ERTSLanguageCode LoadUserLanguagePreference() const;
    
    /** Save user language preference */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    void SaveUserLanguagePreference(ERTSLanguageCode LanguageCode};
    
    /** Check if language is available */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    bool IsLanguageAvailable(ERTSLanguageCode LanguageCode) const;
    
    /** Get available language options (for UI) */
    UFUNCTION(BlueprintCallable, Category = "MingRTSLocalizationSubsystem")
    TArray<struct FLanguageOption> GetLanguageOptions() const;
    
    /** Event: Language changed */
    UPROPERTY(BlueprintAssignable, Category = "MingRTSLocalizationSubsystemEvents")
    FOnSubsystemLanguageChanged OnLanguageChanged;

private:
    /** Localization system instance */
    UPROPERTY()
    TObjectPtr<UMingRTSLocalizationSystem> LocalizationSystem;
    
    /** Text manager instance */
    UPROPERTY()
    TObjectPtr<UMingRTSTextManager> TextManager;
    
    /** Cultural adaptation system instance */
    UPROPERTY()
    TObjectPtr<UMingRTSCulturalAdaptationSystem> CulturalSystem;
    
    /** Settings save slot name */
    static const FString LanguageSettingsSlot;
    
    /** Initialize from saved settings or system defaults */
    void InitializeFromSettings(};
    
    /** Inandle language changed event from localization system */
    UFUNCTION()
    void InandleLanguageChanged(ERTSLanguageCode NewLanguage};
};

/**
 * Language option for UI display
 */
USTRUCT(BlueprintType)
struct FLanguageOption
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ERTSLanguageCode LanguageCode;
    
    UPROPERTY(BlueprintReadOnly)
    FString DisplayName;
    
    UPROPERTY(BlueprintReadOnly)
    FString NativeName;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsAvailable;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsSelected;
    
    FLanguageOption()
        : LanguageCode(ERTSLanguageCode::zh_CN)
        , bIsAvailable(false)
        , bIsSelected(false)
    {}
};

