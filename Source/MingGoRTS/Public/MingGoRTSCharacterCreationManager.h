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
};



UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSCharacterCreationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSCharacterCreationManager(};

    // ����動�بɥ礎�
    UPROPERTY(BlueprintAssignable, Category = "Character Creation")
    FOnCharacterCreated OnCharacterCreated;

    // 動�ب���
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool CreateCharacter(const FMingCharacterData& CharacterData};

    // ��動����
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    bool ValidateCharacterData(const FMingCharacterData& CharacterData};

    // 動池��動�z
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    FString GetBackgroundDescription(ECharacterBackground Background};

    // 動池���l務
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    TArray<FCharacterSkill> GetRecommendedSkills(ECharacterBackground Background};

    // �O動����動
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool SaveCharacterToSlot(const FMingCharacterData& CharacterData, const FString& SlotName};

    // �q動��池務
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool LoadCharacterFromSlot(const FString& SlotName, FMingCharacterData& OutCharacterData};

    // �ˬd����動��動�_動��
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    bool IsCharacterNameAvailable(const FString& CharacterName};

    // 目標數量����動��
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    FString GenerateRandomCharacterName(};

protected:
    // 動����池��
    bool ValidateName(const FString& Name};
    bool ValidateAttributes(const FCharacterAttributes& Attributes};
    bool ValidateAge(int32 InAge};

    // �q動����動��動
    UPROPERTY()
    TArray<FString> MaleNames;

    UPROPERTY()
    TArray<FString> FemaleNames;

    UPROPERTY()
    TArray<FString> Surnames;

    // 目標數量��動
    void InitializeNamePools(};

    // 目標數量�m動
    FString GetRandomSurname(};

    // 整理背包物~?
    FString GetRandomGivenName(bool bIsMale};
};
};
