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

    // ?äÊà≤?ÉË©±?∏Ê?
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Session")
    FMingGameSessionData GameSessionData;

    // Ë®≠ÁΩÆ?∂Â?ËßíËâ≤?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetCurrentCharacterData(const FMingCharacterData& CharacterData);

    // ?≤Â??∂Â?ËßíËâ≤?∏Ê?
    UFUNCTION(BlueprintPure, Category = "Game Session")
    FMingCharacterData GetCurrentCharacterData() const;

    // Ë®≠ÁΩÆÂ≠òÊ?ÊßΩÂ?Á®?    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetSaveSlotName(const FString& SlotName);

    // ?≤Â?Â≠òÊ?ÊßΩÂ?Á®?    UFUNCTION(BlueprintPure, Category = "Game Session")
    FString GetSaveSlotName() const;

    // ?ãÂ??∞Á??äÊà≤?ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void StartNewGameSession(const FMingCharacterData& CharacterData, const FString& SaveSlot);

    // ÁµêÊ??∂Â??äÊà≤?ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void EndCurrentGameSession();

    // ‰øùÂ??äÊà≤?ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool SaveGameSession();

    // ËºâÂÖ•?äÊà≤?ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    bool LoadGameSession(const FString& SlotName);

    // Ê™¢Êü•?ØÂê¶?âÊ≠£?®ÈÄ≤Ë??ÑÊ?Ë©?    UFUNCTION(BlueprintPure, Category = "Game Session")
    bool HasActiveSession() const;

    // ?≤Â??ÉË©±?ÅÁ??ÇÈ?
    UFUNCTION(BlueprintPure, Category = "Game Session")
    float GetSessionDuration() const;

    // Ë®≠ÁΩÆ?äÊà≤?ÇÈ?
    UFUNCTION(BlueprintCallable, Category = "Game Session")
    void SetGameTime(ERepublicEra Era, int32 Year);

    // ?≤Â??äÊà≤?ÇÈ?
    UFUNCTION(BlueprintPure, Category = "Game Session")
    void GetGameTime(ERepublicEra& OutEra, int32& OutYear) const;

protected:
    // ?ÉË©±?ØÂê¶Ê¥ªË?
    UPROPERTY()
    bool bSessionActive;

    // ?üÊ??ÉË©± ID
    FString GenerateSessionID();

    // ?ÉË©±?∏Ê?Â∫èÂ???    FString SerializeSessionData() const;

    // ?ÉË©±?∏Ê??çÂ??óÂ?
    bool DeserializeSessionData(const FString& SerializedData);

    // Ê∏ÖÁ??ÉË©±?∏Ê?
    void ClearSessionData();
};
