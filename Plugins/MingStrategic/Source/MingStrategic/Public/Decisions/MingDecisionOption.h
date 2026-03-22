#pragma once

#include "CoreMinimal.h"
#include "MingDecisionEffect.h"
#include "MingStrategicDecisionOption.generated.h"

/**
 * 決�X��?結�?
 * 定義決�X��X�選?��X��X */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicDecisionOption
{
    GENERATED_BODY()
    
    // ?��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString OptionId;
    
    // ?��?標�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionTitle;
    
    // ?��X�述 (詳細說�?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionDescription;
    
    // ?��X�覽 (簡短說�?顯示?��X��?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FText OptionPreview;
    
    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    UTexture2D* OptionIcon;
    
    // ?��X��X�表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    TArray<FMingDecisionEffect> Effects;
    
    // 後�?決�?ID (?��?此選?��?觸發?��?一?�決�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString NextDecisionId;
    
    // ?�否結�X��?決�X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    bool bEndDecisionChain;
    
    // ?��X�用?��?�?(?��?空表示�?終可X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
    FString AvailabilityCondition;
    
    FMingStrategicDecisionOption()
        : OptionIcon(nullptr)
        , bEndDecisionChain(false)
    {}
    
    // 檢查?��X�否?�用
    bool IsAvailable() const;
};

