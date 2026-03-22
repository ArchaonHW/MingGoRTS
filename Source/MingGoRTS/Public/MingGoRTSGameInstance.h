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

    // ??戲??話????
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Session")
    FMingGameSessionData GameSessionData;

    // 設置????角色????
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetCurrentCharacterData(const FMingCharacterData& CharacterData};

    // ???X???角色????
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FMingCharacterData GetCurrentCharacterData() const;

    // 設置存??槽????    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetSaveSlotName(const FString& SlotName};

    // ????存??槽????    UFUNCTION(BlueprintPure, Category = "Game Session")
    FString GetSaveSlotName() const;

    // ???X??X?戲??話
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void StartNewGameSession(const FMingCharacterData& CharacterData, const FString& SaveSlot};

    // 結?X??X?戲??話
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void EndCurrentGameSession();

    // 保?X?戲??話
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool SaveGameSession();

    // 載入??戲??話
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool LoadGameSession(const FString& SlotName};

    // 檢查??否??正??進?X?????    UFUNCTION(BlueprintPure, Category = "Game Session")
    bool HasActiveSession() const;

    // ???X?話???X???
    UFUNCTION(BlueprintPure, Category = "Game Session")
    float GetSessionDuration() const;

    // 設置??戲????
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetGameTime(ERepublicEra Era, int32 Year};

    // ???X?戲????
    UFUNCTION(BlueprintPure, Category = "Game Session")
    void GetGameTime(ERepublicEra& OutEra, int32& OutYear) const;

protected:
    // ??話??否活??
    UPROPERTY()
    bool bSessionActive;

    // ???X?話 ID
    FString GenerateSessionID();

    // ??話????序?X    FString SerializeSessionData() const;

    // ??話???X??X???
    bool DeserializeSessionData(const FString& SerializedData};

    // 清?X?話????
    void ClearSessionData();
};

