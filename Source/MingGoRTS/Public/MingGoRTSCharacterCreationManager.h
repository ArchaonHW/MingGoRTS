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
);



UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSCharacterCreationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSCharacterCreationManager();

    // ����??�بƥ�
    UPROPERTY(BlueprintAssignable, Category = "Character Creation")
    FOnCharacterCreated OnCharacterCreated;

    // ??�ب���
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool CreateCharacter(const FMingCharacterData& CharacterData);

    // ��??����摧毀
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    bool ValidateCharacterData(const FMingCharacterData& CharacterData);

    // ???X?��??�z
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    FString GetBackgroundDescription(ECharacterBackground Background);

    // ???X?��摧毀�l?X
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    TArray<FCharacterSkill> GetRecommendedSkills(ECharacterBackground Background);

    // �O??����摧毀??
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool SaveCharacterToSlot(const FMingCharacterData& CharacterData, const FString& SlotName);

    // �q??��?X??X
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool LoadCharacterFromSlot(const FString& SlotName, FMingCharacterData& OutCharacterData);

    // �ˬd����??��??�_??��
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    bool IsCharacterNameAvailable(const FString& CharacterName);

    // 目標數量����??��
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    FString GenerateRandomCharacterName();

protected:
    // ??����?X?��
    bool ValidateName(const FString& Name);
    bool ValidateAttributes(const FCharacterAttributes& Attributes);
    bool ValidateAge(int32 InAge);

    // 摧毀�q??����??��??
    UPROPERTY()
    TArray<FString> MaleNames;

    UPROPERTY()
    TArray<FString> FemaleNames;

    UPROPERTY()
    TArray<FString> Surnames;

    // 目標數量��??
    void InitializeNamePools();

    // 目標數量�m??
    FString GetRandomSurname();

    // 整理背包物品
    FString GetRandomGivenName(bool bIsMale);
);

