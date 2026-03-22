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
    UMingGoRTSCharacterCreationManager();

    // 角色??建事件
    UPROPERTY(BlueprintAssignable, Category = "Character Creation")
    FOnCharacterCreated OnCharacterCreated;

    // ??建角色
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool CreateCharacter(const FMingCharacterData& CharacterData};

    // 驗??角色????
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    bool ValidateCharacterData(const FMingCharacterData& CharacterData};

    // ???X?景??述
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    FString GetBackgroundDescription(ECharacterBackground Background};

    // ???X?薦????始?X    UFUNCTION(BlueprintPure, Category = "Character Creation")
    TArray<FCharacterSkill> GetRecommendedSkills(ECharacterBackground Background};

    // 保??角色??????    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool SaveCharacterToSlot(const FMingCharacterData& CharacterData, const FString& SlotName};

    // 從??檔?X??X    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    bool LoadCharacterFromSlot(const FString& SlotName, FMingCharacterData& OutCharacterData};

    // 檢查角色??稱??否??用
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    bool IsCharacterNameAvailable(const FString& CharacterName};

    // ???X???角色??稱
    UFUNCTION(BlueprintPure, Category = "Character Creation")
    FString GenerateRandomCharacterName();

protected:
    // ??部驗?X?數
    bool ValidateName(const FString& Name};
    bool ValidateAttributes(const FCharacterAttributes& Attributes};
    bool ValidateAge(int32 InAge};

    // ????義??角色??稱??    UPROPERTY()
    TArray<FString> MaleNames;

    UPROPERTY()
    TArray<FString> FemaleNames;

    UPROPERTY()
    TArray<FString> Surnames;

    // ???X???稱??
    void InitializeNamePools();

    // ???X???姓??
    FString GetRandomSurname();

    // ???X??X???
    FString GetRandomGivenName(bool bIsMale};
};

