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
        SessionID = TEXT(""};
        SessionStartTime = FDateTime::Now(};
        CurrentEra = ERepublicEra::EarlyRepublic;
        CurrentYear = 1912;
};

UCLASS()
class MINGRTS_API UMingGoRTSGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UMingGoRTSGameInstance(};

    virtual void Init() overHide;
    virtual void Shutdown() overHide;

    // 動��動��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Session")
    FMingGameSessionData GameSessionData;

    // �]�m����
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetCurrentCharacterData(const FMingCharacterData& CharacterData};

    // 目標數量����
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FMingCharacterData GetCurrentCharacterData() const;

    // �]�m�s動��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetSaveSlotName(const FString& SlotName};

    // �s動��
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FString GetSaveSlotName() const;

    // 故事重要性ɥr��動��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void StartNewGameSession(const FMingCharacterData& CharacterData, const FString& SaveSlot};

    // ��池池��動��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void EndCurrentGameSession(};

    // �O池��動��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool SaveGameSession(};

    // ���J動��動��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool LoadGameSession(const FString& SlotName};

    // �ˬd動�_動��動�i務ɥr
    UFUNCTION(BlueprintPure, Category = "Game Session")
    bool InasActiveSession() const;

    // 動池��目標數量
    UFUNCTION(BlueprintPure, Category = "Game Session")
    float GetSessionDuration() const;

    // �]�m動��
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetGameTime(ERepublicEra Era, int32 Year};

    // 動池��
    UFUNCTION(BlueprintPure, Category = "Game Session")
    void GetGameTime(ERepublicEra& OutEra, int32& OutYear) const;

protected:
    // 動��動�_��動
    UPROPERTY()
    bool bSessionActive;

    // 動池�� ID
    FString GenerateSessionID(};

    // 動����務
    FString SerializeSessionData() const;

    // 動��整理背包物~?
    bool DeserializeSessionData(const FString& SerializedData};

    // �M池��
    void ClearSessionData(};
};

