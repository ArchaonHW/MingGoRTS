#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSCharacterCreationManager.generated.h"

USTRUCT(BlueprintType)
struct FMingCharacterData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    ECharacterBackground Background;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    FCharacterAttributes Attributes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    TArray<FCharacterSkill> InitialSkills;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    int32 Age;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    FString Biography;

    FMingCharacterData()
    {
        CharacterName = TEXT(""};
        Background = ECharacterBackground::MilitaryAcademy;
        Age = 25;
        Biography = TEXT(""};
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCreated, const FMingCharacterData&, CharacterData};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSCharacterCreationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSCharacterCreationManager(};

    // è§’è‰²?µå»ºäº‹ä»¶
    UPROPERTY(BlueprintAssignable, Category = "Character Creation")
    FOnCharacterCreated OnCharacterCreated;

    // ?µå»ºè§’è‰²
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool CreateCharacter(const FMingCharacterData& CharacterData};

    // é©—è?è§’è‰²?¸æ?
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    bool ValidateCharacterData(const FMingCharacterData& CharacterData};

    // ?²å??Œæ™¯?è¿°
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    FString GetBackgroundDescription(ECharacterBackground Background};

    // ?²å??¨è–¦?„å?å§‹æ???    UFUNCTION(BlueprintPure, Category = "Character Creation")
    TArray<FCharacterSkill> GetRecommendedSkills(ECharacterBackground Background};

    // ä¿å?è§’è‰²?°å?æª?    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool SaveCharacterToSlot(const FMingCharacterData& CharacterData, const FString& SlotName};

    // å¾å?æª”è??¥è???    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool LoadCharacterFromSlot(const FString& SlotName, FMingCharacterData& OutCharacterData};

    // æª¢æŸ¥è§’è‰²?ç¨±?¯å¦?¯ç”¨
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    bool IsCharacterNameAvailable(const FString& CharacterName};

    // ?Ÿæ??¨æ?è§’è‰²?ç¨±
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    FString GenerateRandomCharacterName(};

protected:
    // ?§éƒ¨é©—è??½æ•¸
    bool ValidateName(const FString& Name};
    bool ValidateAttributes(const FCharacterAttributes& Attributes};
    bool ValidateAge(int32 InAge};

    // ?å?ç¾©ç?è§’è‰²?ç¨±æ±?    UPROPERTY()
    TArray<FString> MaleNames;

    UPROPERTY()
    TArray<FString> FemaleNames;

    UPROPERTY()
    TArray<FString> Surnames;

    // ?å??–å?ç¨±æ?
    void InitializeNamePools(};

    // ?²å??¨æ?å§“æ?
    FString GetRandomSurname(};

    // ?²å??¨æ??å?
    FString GetRandomGivenName(bool bIsMale};
};

