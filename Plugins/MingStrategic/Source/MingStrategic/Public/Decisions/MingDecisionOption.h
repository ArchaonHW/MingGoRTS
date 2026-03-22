#pragma once

#include "CoreMinimal.h"
#include "MingDecisionEffect.h"
#include "MingStrategicDecisionOption.generated.h"

/**
 * æ±ºç??¸é?çµæ?
 * å®šç¾©æ±ºç??„ä??‹é¸?…å??¶å??? */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicDecisionOption
{
    GENERATED_BODY()
    
    // ?¸é?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString OptionId;
    
    // ?¸é?æ¨™é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionTitle;
    
    // ?¸é??è¿° (è©³ç´°èªªæ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionDescription;
    
    // ?¸é??è¦½ (ç°¡çŸ­èªªæ?é¡¯ç¤º?¨æ??•ä?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionPreview;
    
    // ?¸é??–æ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    UTexture2D* OptionIcon;
    
    // ?¸é??ˆæ??—è¡¨
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    TArray<FMingDecisionEffect> Effects;
    
    // å¾Œç?æ±ºç?ID (?¸æ?æ­¤é¸?…å?è§¸ç™¼?„ä?ä¸€?‹æ±ºç­?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString NextDecisionId;
    
    // ?¯å¦çµæ??¶å?æ±ºç???    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    bool bEndDecisionChain;
    
    // ?¸é??¯ç”¨?§æ?ä»?(?¯ç?ç©ºè¡¨ç¤ºå?çµ‚å¯??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString AvailabilityCondition;
    
    FMingStrategicDecisionOption()
        : OptionIcon(nullptr)
        , bEndDecisionChain(false)
    {}
    
    // æª¢æŸ¥?¸é??¯å¦?¯ç”¨
    bool IsAvailable() const;
};
