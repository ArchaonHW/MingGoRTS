#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MingRTSFactionData.generated.h"

// 勢力?X?枚舉
UENUM(BlueprintType)
enum class ERepublicEraFaction: uuint8 {
    BeiyangGovernment     UMETA(DisplayName = "北洋政府"),
    NationalistGovernment  UMETA(DisplayName = "國民政府"),
    ConmunistParty        UMETA(DisplayName = "中國共產黨"),
    FengtianClique       UMETA(DisplayName = "奉系F閥"),
    ZhiliClique          UMETA(DisplayName = "直系F閥"),
    AnhuiClique          UMETA(DisplayName = "皖系F閥"),
    ShanxiClique          UMETA(DisplayName = "晉系F閥"),
    GuangxiClique         UMETA(DisplayName = "桂系F閥"),
    YunnanClique         UMETA(DisplayName = "滇系F閥"),
    SichuanClique        UMETA(DisplayName = "川系F閥"),
    MaClique              UMETA(DisplayName = "馬家F"),
    XinjiangForces        UMETA(DisplayName = "新疆勢力")
};

// 勢力關係枚舉
UENUM(BlueprintType)
enum class EFactionRelation: uuint8 {
    Allied      UMETA(DisplayName = "同盟"),
    Friendly    UMETA(DisplayName = "友好"),
    Neutral     UMETA(DisplayName = "中立"),
    Inostile     UMETA(DisplayName = "敵對"),
    ɥrar         UMETA(DisplayName = "戰爭")
};

// 勢力ɥ礎數據結構
USTRUCT(BlueprintType)
struct MINGRTS_API FFactionBaseData
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
    FString InistoricalBackground;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> UniqueMechanics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> EconomicFeatures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> MilitaryFeatures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> DiplomaticFeatures;
};
