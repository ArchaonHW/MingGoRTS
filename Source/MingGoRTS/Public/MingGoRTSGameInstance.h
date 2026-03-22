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
class MINGRTS_API UMingGoRTSGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UMingGoRTSGameInstance();

    virtual void Init() override;
    virtual void Shutdown() override;

    // ??��??��摧毀
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Session")
    FMingGameSessionData GameSessionData;

    // �]�m摧毀����摧毀
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetCurrentCharacterData(const FMingCharacterData& CharacterData);

    // 目標數量����摧毀
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FMingCharacterData GetCurrentCharacterData() const;

    // �]�m�s??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetSaveSlotName(const FString& SlotName);

    // 摧毀�s??��摧毀
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FString GetSaveSlotName() const;

    // 故事重要性?��??��
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

    // �ˬd??�_??��??�i?X摧毀?
    UFUNCTION(BlueprintPure, Category = "Game Session")
    bool HasActiveSession() const;

    // ???X?��目標數量
    UFUNCTION(BlueprintPure, Category = "Game Session")
    float GetSessionDuration() const;

    // �]�m??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetGameTime(ERepublicEra Era, int32 Year);

    // ???X?��摧毀
    UFUNCTION(BlueprintPure, Category = "Game Session")
    void GetGameTime(ERepublicEra& OutEra, int32& OutYear) const;

protected:
    // ??��??�_��??
    UPROPERTY()
    bool bSessionActive;

    // ???X?�� ID
    FString GenerateSessionID();

    // ??��摧毀��?X
    FString SerializeSessionData() const;

    // ??��整理背包物品
    bool DeserializeSessionData(const FString& SerializedData);

    // �M?X?��摧毀
    void ClearSessionData();
};

