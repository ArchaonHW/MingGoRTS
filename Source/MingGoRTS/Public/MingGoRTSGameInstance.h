#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FMingGameSessionData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Game Session")
    FMingCharacterData CurrentCharacterData;

    UPROPERTY(BlueprintReadWrite, Category = "Game Session")
    FString SaveSlotName;

    UPROPERTY(BlueprintReadWrite, Category = "Game Session")
    FString SessionID;

    UPROPERTY(BlueprintReadWrite, Category = "Game Session")
    FDateTime SessionStartTime;

    UPROPERTY(BlueprintReadWrite, Category = "Game Session")
    ERepublicEra CurrentEra;

    UPROPERTY(BlueprintReadWrite, Category = "Game Session")
    int32 CurrentYear;

    FMingGameSessionData()
    {
        SaveSlotName = TEXT(""};
        SessionID = TEXT("");
        SessionStartTime = FDateTime::Now();
        CurrentEra = ERepublicEra::EarlyRepublic;
        CurrentYear = 1912;
    }
};

UCLASS()
class MINGGORTS_API UMingGoRTSGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UMingGoRTSGameInstance();

    virtual void Init() override;
    virtual void Shutdown() override;

    // ??��??��????
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Session")
    FMingGameSessionData GameSessionData;

    // �]�m????����????
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetCurrentCharacterData(const FMingCharacterData& CharacterData);

    // ???X???����????
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FMingCharacterData GetCurrentCharacterData() const;

    // �]�m�s??��????
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetSaveSlotName(const FString& SlotName);

    // ????�s??��????
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FString GetSaveSlotName() const;

    // ???X??X?��??��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void StartNewGameSession(const FMingCharacterData& CharacterData, const FString& SaveSlot);

    // ��?X??X?��??��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void EndCurrentGameSession();

    // �O?X?��??��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool SaveGameSession();

    // ���J??��??��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool LoadGameSession(const FString& SlotName);

    // �ˬd??�_??��??�i?X?????
    UFUNCTION(BlueprintPure, Category = "Game Session")
    bool HasActiveSession() const;

    // ???X?��???X???
    UFUNCTION(BlueprintPure, Category = "Game Session")
    float GetSessionDuration() const;

    // �]�m??��????
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetGameTime(ERepublicEra Era, int32 Year);

    // ???X?��????
    UFUNCTION(BlueprintPure, Category = "Game Session")
    void GetGameTime(ERepublicEra& OutEra, int32& OutYear) const;

protected:
    // ??��??�_��??
    UPROPERTY()
    bool bSessionActive;

    // ???X?�� ID
    FString GenerateSessionID();

    // ??��????��?X
    FString SerializeSessionData() const;

    // ??��???X??X???
    bool DeserializeSessionData(const FString& SerializedData);

    // �M?X?��????
    void ClearSessionData();
};

