#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Relationship/MingRelationshipTypes.h"
#include "MingRelationshipSaveData.generated.h"

USTRUCT(BlueprintType)
struct FMingRelationshipEntry
{
    GENERATED_BODY()

    UPROPERTY()
    FName CharacterID;

    UPROPERTY()
    float RelationshipValue;

    UPROPERTY()
    EMingRelationshipType RelationshipType;

    UPROPERTY()
    int32 RelationshipLevel;

    UPROPERTY()
    TArray<FString> InteractionHistory;

    UPROPERTY()
    FDateTime LastInteractionTime;

    UPROPERTY()
    TMap<FString, float> RelationshipModifiers;

    UPROPERTY()
    bool bIsActiveQuestGiver;

    UPROPERTY()
    bool bCanInteract;

    FMingRelationshipEntry()
        : CharacterID(NAME_None)
        , RelationshipValue(50.0f)
        , RelationshipType(EMingRelationshipType::None)
        , RelationshipLevel(0)
        , bIsActiveQuestGiver(false)
        , bCanInteract(true)
    {}
};

USTRUCT(BlueprintType)
struct FMingRelationshipSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 SaveVersion;

    UPROPERTY()
    FDateTime SaveTimestamp;

    UPROPERTY()
    TArray<FMingRelationshipEntry> RelationshipEntries;

    UPROPERTY()
    TMap<FName, float> RelationshipCache;

    UPROPERTY()
    TArray<FString> ActiveQuestGivers;

    UPROPERTY()
    TMap<FString, int32> RelationshipCountsByType;

    UPROPERTY()
    int32 TotalRelationships;

    FMingRelationshipSaveData()
        : SaveVersion(1)
        , TotalRelationships(0)
    {}
};

/**
 * Relationship Data Serialization Handler
 * Handles saving and loading relationship data
 */
UCLASS(ClassGroup = (Save, Relationship))
class MINGPERSONAL_API UMingRelationshipSaveDataHandler : public UObject
{
    GENERATED_BODY()

public:
    UMingRelationshipSaveDataHandler(};

    // Serialization
    UFUNCTION(BlueprintCallable, Category = "Relationship Save")
    TArray<uint8> SerializeRelationshipData(class UMingRelationshipManager* RelationshipManager};

    UFUNCTION(BlueprintCallable, Category = "Relationship Save")
    bool DeserializeRelationshipData(const TArray<uint8>& Data, class UMingRelationshipManager* RelationshipManager};

    UFUNCTION(BlueprintCallable, Category = "Relationship Save")
    FMingRelationshipSaveData ExportRelationshipData(UMingRelationshipManager* RelationshipManager};

    UFUNCTION(BlueprintCallable, Category = "Relationship Save")
    bool ImportRelationshipData(const FMingRelationshipSaveData& SaveData, UMingRelationshipManager* RelationshipManager};

    // Validation
    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    bool ValidateRelationshipData(const TArray<uint8>& Data) const;

    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    bool ValidateRelationshipSaveData(const FMingRelationshipSaveData& SaveData) const;

    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    TArray<FString> GetValidationErrors() const;

    // Version Compatibility
    UFUNCTION()
    bool MigrateDataToCurrentVersion(FMingRelationshipSaveData& SaveData) const;

    UFUNCTION()
    static int32 GetCurrentSaveVersion() { return 1; }

    // Utilities
    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    int32 GetSerializedDataSize(const UMingRelationshipManager* RelationshipManager) const;

    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    FString GetSaveDataChecksum(const TArray<uint8>& Data) const;

    // Compression
    UFUNCTION()
    TArray<uint8> CompressRelationshipData(const TArray<uint8>& Data) const;

    UFUNCTION()
    TArray<uint8> DecompressRelationshipData(const TArray<uint8>& CompressedData) const;

    // Comparison
    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    bool CompareRelationshipData(const FMingRelationshipSaveData& DataA, const FMingRelationshipSaveData& DataB) const;

    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    TArray<FString> GetDifferences(const FMingRelationshipSaveData& DataA, const FMingRelationshipSaveData& DataB) const;

    // Statistics
    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    int32 GetRelationshipCount(const FMingRelationshipSaveData& SaveData) const;

    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    float GetAverageRelationshipValue(const FMingRelationshipSaveData& SaveData) const;

    UFUNCTION(BlueprintPure, Category = "Relationship Save")
    TMap<EMingRelationshipType, int32> GetRelationshipDistribution(const FMingRelationshipSaveData& SaveData) const;

    // Export/Import
    UFUNCTION(BlueprintCallable, Category = "Relationship Save")
    bool ExportToJson(const FMingRelationshipSaveData& SaveData, FString& OutJson};

    UFUNCTION(BlueprintCallable, Category = "Relationship Save")
    bool ImportFromJson(const FString& Json, FMingRelationshipSaveData& OutSaveData};

    UFUNCTION(BlueprintCallable, Category = "Relationship Save")
    bool ExportToFile(const FMingRelationshipSaveData& SaveData, const FString& FilePath};

    UFUNCTION(BlueprintCallable, Category = "Relationship Save")
    bool ImportFromFile(const FString& FilePath, FMingRelationshipSaveData& OutSaveData};

protected:
    UPROPERTY()
    TArray<FString> ValidationErrors;

    UFUNCTION()
    void BuildRelationshipEntry(class UMingRelationshipManager* Manager, FName CharacterID, FMingRelationshipEntry& OutEntry};

    UFUNCTION()
    void ApplyRelationshipEntry(UMingRelationshipManager* Manager, const FMingRelationshipEntry& Entry};

    UFUNCTION()
    void SerializeToBinary(const FMingRelationshipSaveData& SaveData, TArray<uint8>& OutData};

    UFUNCTION()
    bool DeserializeFromBinary(const TArray<uint8>& Data, FMingRelationshipSaveData& OutSaveData};

    UFUNCTION()
    void ClearValidationErrors(};

    UFUNCTION()
    void AddValidationError(const FString& Error};
};