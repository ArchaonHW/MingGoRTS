#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSDialogueSystem.h"
#include "MingGoRTSHistoricalCharacters.generated.h"

UENUM(BlueprintType)
enum class EHistoricalFaction : uint8
{
    Nationalist, UMETA(DisplayName = "Nationalist"),
    Communist, UMETA(DisplayName = "Communist"),
    Warlord, UMETA(DisplayName = "Warlord"),
    Independent, UMETA(DisplayName = "Independent"),
    Foreign, UMETA(DisplayName = "Foreign")
};

UENUM(BlueprintType)
enum class ECharacterRole : uint8
{
    Leader, UMETA(DisplayName = "Leader"),
    Military, UMETA(DisplayName = "Military"),
    Political, UMETA(DisplayName = "Political"),
    Economic, UMETA(DisplayName = "Economic"),
    Cultural, UMETA(DisplayName = "Cultural"),
    Diplomatic, UMETA(DisplayName = "Diplomatic")
};

/**
 * Historical character data structure
 */
USTRUCT(BlueprintType)
struct FHistoricalCharacter
{
    GENERATED_BODY()

    // Character name
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    FString CharacterName;

    // Faction affiliation
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    EHistoricalFaction Faction;

    // Character role
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    ECharacterRole Role;

    // Birth year
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    int32 BirthYear;

    // Death year
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    int32 DeathYear;

    // Character description
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    FText Description;

    // Character portrait
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    UTexture2D* Portrait;

    // Historical significance
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    FString HistoricalSignificance;

    FHistoricalCharacter()
        : CharacterName(TEXT(""))
        , Faction(EHistoricalFaction::Independent)
        , Role(ECharacterRole::Leader)
        , BirthYear(1900)
        , DeathYear(2000)
        , Description(FText::GetEmpty())
        , Portrait(nullptr)
        , HistoricalSignificance(TEXT(""))
    {}
};

/**
 * MingGoRTS Historical Characters System
 * Manages historical characters and their interactions
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalCharacters : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalCharacters();

    /**
     * Initialize historical characters system
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Characters")
    void InitializeHistoricalCharacters();

    /**
     * Get character by name
     */
    UFUNCTION(BlueprintPure, Category = "Historical Characters")
    FHistoricalCharacter GetCharacterByName(const FString& CharacterName);

    /**
     * Get characters by faction
     */
    UFUNCTION(BlueprintPure, Category = "Historical Characters")
    TArray<FHistoricalCharacter> GetCharactersByFaction(EHistoricalFaction Faction);

    /**
     * Get characters by role
     */
    UFUNCTION(BlueprintPure, Category = "Historical Characters")
    TArray<FHistoricalCharacter> GetCharactersByRole(ECharacterRole Role);

    /**
     * Get all characters
     */
    UFUNCTION(BlueprintPure, Category = "Historical Characters")
    TArray<FHistoricalCharacter> GetAllCharacters();

    /**
     * Add new character
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Characters")
    bool AddCharacter(const FHistoricalCharacter& Character);

    /**
     * Update character data
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Characters")
    bool UpdateCharacter(const FString& CharacterName, const FHistoricalCharacter& UpdatedCharacter);

    /**
     * Remove character
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Characters")
    bool RemoveCharacter(const FString& CharacterName);

    /**
     * Get character dialogue options
     */
    UFUNCTION(BlueprintPure, Category = "Historical Characters")
    TArray<FDialogueOption> GetCharacterDialogueOptions(const FString& CharacterName);

    /**
     * Check if character exists
     */
    UFUNCTION(BlueprintPure, Category = "Historical Characters")
    bool CharacterExists(const FString& CharacterName) const;

protected:
    // Character database
    UPROPERTY()
    TMap<FString, FHistoricalCharacter> CharacterDatabase;

    // Initialize default characters
    void InitializeDefaultCharacters();

    // Validate character data
    bool ValidateCharacter(const FHistoricalCharacter& Character) const;

    // Log character events
    void LogCharacterEvent(const FString& Event, const FString& CharacterName);
};
