#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MingRTSFactionData.generated.h"

// 勢力類型枚舉
UENUM(BlueprintType)
enum class ERepublicEraFaction: uint8 {
    BeiyangGovernment     UMETA(DisplayName = "北洋政府"),
    NationalistGovernment  UMETA(DisplayName = "國民政府"),
    CommunistParty        UMETA(DisplayName = "中國共產黨"),
    FengtianClique       UMETA(DisplayName = "奉系軍閥"),
    ZhiliClique          UMETA(DisplayName = "直系軍閥"),
    AnhuiClique          UMETA(DisplayName = "皖系軍閥"),
    ShanxiClique          UMETA(DisplayName = "晉系軍閥"),
    GuangxiClique         UMETA(DisplayName = "桂系軍閥"),
    YunnanClique         UMETA(DisplayName = "滇系軍閥"),
    SichuanClique        UMETA(DisplayName = "川系軍閥"),
    MaClique              UMETA(DisplayName = "馬家軍"),
    XinjiangForces        UMETA(DisplayName = "新疆勢力")
};

// 勢力關係枚舉
UENUM(BlueprintType)
enum class EFactionRelation: uint8 {
    Allied      UMETA(DisplayName = "同盟"),
    Friendly    UMETA(DisplayName = "友好"),
    Neutral     UMETA(DisplayName = "中立"),
    Hostile     UMETA(DisplayName = "敵對"),
    War         UMETA(DisplayName = "戰爭")
};

// 勢力基礎數據結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FFactionBaseData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    ERepublicEraFaction FactionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString FactionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString FactionNameEN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 DifficultyLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FVector2D StartingLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 StartingYear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString HistoricalBackground;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> UniqueMechanics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> EconomicFeatures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> MilitaryFeatures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> DiplomaticFeatures;
};
