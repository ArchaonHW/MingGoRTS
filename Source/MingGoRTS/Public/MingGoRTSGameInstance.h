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
        SaveSlotName = TEXT("");
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

    // 遊戲會話數據
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Session")
    FMingGameSessionData GameSessionData;

    // 設置當前角色數據
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetCurrentCharacterData(const FMingCharacterData& CharacterData);

    // 獲取當前角色數據
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FMingCharacterData GetCurrentCharacterData() const;

    // 設置存檔槽名稱
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetSaveSlotName(const FString& SlotName);

    // 獲取存檔槽名稱
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FString GetSaveSlotName() const;

    // 開始新的遊戲會話
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void StartNewGameSession(const FMingCharacterData& CharacterData, const FString& SaveSlot);

    // 結束當前遊戲會話
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void EndCurrentGameSession();

    // 保存遊戲會話
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool SaveGameSession();

    // 載入遊戲會話
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool LoadGameSession(const FString& SlotName);

    // 檢查是否有正在進行的會話
    UFUNCTION(BlueprintPure, Category = "Game Session")
    bool HasActiveSession() const;

    // 獲取會話持續時間
    UFUNCTION(BlueprintPure, Category = "Game Session")
    float GetSessionDuration() const;

    // 設置遊戲時間
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetGameTime(ERepublicEra Era, int32 Year);

    // 獲取遊戲時間
    UFUNCTION(BlueprintPure, Category = "Game Session")
    void GetGameTime(ERepublicEra& OutEra, int32& OutYear) const;

protected:
    // 會話是否活躍
    UPROPERTY()
    bool bSessionActive;

    // 生成會話 ID
    FString GenerateSessionID();

    // 會話數據序列化
    FString SerializeSessionData() const;

    // 會話數據反序列化
    bool DeserializeSessionData(const FString& SerializedData);

    // 清理會話數據
    void ClearSessionData();
};
