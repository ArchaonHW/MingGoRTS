#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCodexSystem.generated.h"

/**
 * Codex entry types
 */
UENUM(BlueprintType)
enum class ECodexEntryType: uint8 {
    GameBasics UMETA(DisplayName = "Game Basics"),
    FactionInfo UMETA(DisplayName = "Faction Info"),
    UnitInfo UMETA(DisplayName = "Unit Info"),
    BuildingInfo UMETA(DisplayName = "Building Info"),
    Technology UMETA(DisplayName = "Technology"),
    Historical UMETA(DisplayName = "Historical"),
    Tutorial UMETA(DisplayName = "Tutorial"),
    Strategy UMETA(DisplayName = "Strategy"),
    Tips UMETA(DisplayName = "Tips")
};

/**
 * Codex entry data structure
 */
USTRUCT(BlueprintType)
struct FCodexEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Codex")
    FString EntryId;

    UPROPERTY(BlueprintReadOnly, Category = "Codex")
    FString Title;

    UPROPERTY(BlueprintReadOnly, Category = "Codex")
    ECodexEntryType EntryType;

    UPROPERTY(BlueprintReadOnly, Category = "Codex")
    FText Content;

    UPROPERTY(BlueprintReadOnly, Category = "Codex")
    TArray<FString> RelatedEntries;

    UPROPERTY(BlueprintReadOnly, Category = "Codex")
    bool IsUnlocked;

    FCodexEntry()
        : EntryId(TEXT(""))
        , Title(TEXT(""))
        , EntryType(ECodexEntryType::GameBasics)
        , Content(FText::GetEmpty())
        , IsUnlocked(false)
    {}
};

/**
 * MingGoRTS Codex System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSCodexSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCodexSystem();

    UFUNCTION(BlueprintCallable, Category = "Codex System")
    void InitializeCodexSystem();

    UFUNCTION(BlueprintPure, Category = "Codex System")
    TArray<FCodexEntry> GetAllEntries();

    UFUNCTION(BlueprintPure, Category = "Codex System")
    TArray<FCodexEntry> GetEntriesByType(ECodexEntryType EntryType);

    UFUNCTION(BlueprintCallable, Category = "Codex System")
    bool UnlockEntry(const FString& EntryId);

    UFUNCTION(BlueprintPure, Category = "Codex System")
    FCodexEntry GetEntry(const FString& EntryId);

    UFUNCTION(BlueprintPure, Category = "Codex System")
    bool IsEntryUnlocked(const FString& EntryId) const;

protected:
    UPROPERTY()
    TMap<FString, FCodexEntry> CodexEntries;

    void InitializeDefaultEntries();
};
