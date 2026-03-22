// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Localization Game Subsystem

#pragma once

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
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    /** Deinitialize subsystem */
    virtual void Deinitialize() override;
    
    /** Should create this subsystem */
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    
    /** Initialize localization on game start */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    void InitializeLocalization();
    
    /** Get the localization system */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    UMingRTSLocalizationSystem* GetLocalizationSystem() const { return LocalizationSystem; }
    
    /** Get the cultural adaptation system */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    UMingRTSCulturalAdaptationSystem* GetCulturalSystem() const { return CulturalSystem; }
    
    /** Quick access: Get cultural adapted content */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    FString GetCulturalContent(const FString& ContentKey) const;
    
    /** Set cultural region */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    void SetCulturalRegion(ECulturalRegion Region);
    
    /** Get current cultural region */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    ECulturalRegion GetCurrentCulturalRegion() const;
    
    /** Quick access: Get localized text */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    FString GetText(const FString& Key) const;
    
    /** Quick access: Get localized text with namespace */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    FString GetTextEx(const FString& Namespace, const FString& Key) const;
    
    /** Quick access: Set language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    void SetLanguage(ERTSLanguageCode LanguageCode);
    
    /** Quick access: Get current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    ERTSLanguageCode GetCurrentLanguage() const;
    
    /** Get user's preferred language from system settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    ERTSLanguageCode GetSystemPreferredLanguage() const;
    
    /** Load user language preference from saved settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    ERTSLanguageCode LoadUserLanguagePreference() const;
    
    /** Save user language preference */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    void SaveUserLanguagePreference(ERTSLanguageCode LanguageCode);
    
    /** Check if language is available */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    bool IsLanguageAvailable(ERTSLanguageCode LanguageCode) const;
    
    /** Get available language options (for UI) */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Subsystem")
    TArray<struct FLanguageOption> GetLanguageOptions() const;
    
    /** Event: Language changed */
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Localization|Subsystem|Events")
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
    void InitializeFromSettings();
    
    /** Handle language changed event from localization system */
    UFUNCTION()
    void HandleLanguageChanged(ERTSLanguageCode NewLanguage);
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

