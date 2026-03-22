#pragma once

#include "CoreMinimal.h"
#include "MingDecisionOption.h"
#include "MingDecision.generated.h"

/**
 * Ê±∫Á?Ëß∏ÁôºÈ°ûÂ?
 */
UENUM(BlueprintType)
enum class EDecisionTriggerType : uint8
{
    Manual              UMETA(DisplayName = "Manual"),
    TimeBased           UMETA(DisplayName = "Time Based"),
    EventBased          UMETA(DisplayName = "Event Based"),
    ConditionBased      UMETA(DisplayName = "Condition Based"),
    LocationBased       UMETA(DisplayName = "Location Based")
};

/**
 * Ê±∫Á??™Â?Á¥? */
UENUM(BlueprintType)
enum class EDecisionPriority : uint8
{
    Low                 UMETA(DisplayName = "Low"),
    Normal              UMETA(DisplayName = "Normal"),
    High                UMETA(DisplayName = "High"),
    Critical            UMETA(DisplayName = "Critical")
};

/**
 * Ê±∫Á??∏Ê?ÁµêÊ?
 * ÂÆöÁæ©‰∏Ä?ãÂ??¥Á?Ê≠∑Âè≤Ê±∫Á?
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingDecision
{
    GENERATED_BODY()
    
    // Ê±∫Á?ID (?Ø‰?Ê®ôË?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString DecisionId;
    
    // Ê±∫Á?Ê®ôÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FText DecisionTitle;
    
    // Ê±∫Á??åÊôØ?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FText BackgroundDescription;
    
    // Ê±∫Á??¥ÊôØ?ñÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    UTexture2D* BackgroundImage;
    
    // Ê±∫Á??∏È??óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    TArray<FMingDecisionOption> Options;
    
    // Ëß∏ÁôºÈ°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    EDecisionTriggerType TriggerType;
    
    // ?™Â?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    EDecisionPriority Priority;
    
    // Ëß∏ÁôºÊ¢ù‰ª∂ (JSON?ºÂ??ñËá™ÂÆöÁæ©Â≠óÁ¨¶‰∏?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString TriggerCondition;
    
    // ?éÊ??ÇÈ? (?äÊà≤?ßÊ??ìÔ?-1Ë°®Á§∫‰∏çÈ???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    float ExpirationTime;
    
    // ?ØÂê¶?Ø‰ª•Âª∂Â??ïÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    bool bCanPostpone;
    
    // Ê±∫Á?È°ûÂà• (?®Êñº?ÜÈ?È°ØÁ§∫)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString DecisionCategory;
    
    // Ê≠∑Âè≤?ÇÊ? (Â¶ÇÔ?1924-1927 ?ó‰??ÇÊ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString HistoricalPeriod;
    
    // ?∏È?ËßíËâ≤ID?óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    TArray<FString> RelatedCharacters;
    
    // ?∏È??∞È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    FString RelatedLocation;
    
    FMingDecision()
        : BackgroundImage(nullptr)
        , TriggerType(EDecisionTriggerType::Manual)
        , Priority(EDecisionPriority::Normal)
        , ExpirationTime(-1.0f)
        , bCanPostpone(true)
    {}
    
    // Ê™¢Êü•Ê±∫Á??ØÂê¶?éÊ?
    bool IsExpired(float CurrentGameTime) const;
    
    // ?≤Â??ØÁî®?∏È??óË°®
    TArray<FMingDecisionOption> GetAvailableOptions() const;
};

