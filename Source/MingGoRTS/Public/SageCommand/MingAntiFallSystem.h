#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAntiFallSystem.generated.h"

// 墮落徵象類型
UENUM(BlueprintType)
enum class ECorruptionSignType : uint8
{
    MoralDecay,      // 道德淪喪
    PowerAbuse,      // 權力濫用
    GreedGrowth      // 貪慾增長
};

// 防護等級
UENUM(BlueprintType)
enum class EPreventionLevel : uint8
{
    Low,     // 低級防護
    Medium,  // 中級防護
    High,    // 高級防護
    Supreme  // 至聖級防護
};

// 墮落數據
USTRUCT(BlueprintType)
struct MINGGORTS_API FCorruptionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECorruptionSignType SignType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CorruptionValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Threshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString WarningMessage;
};

// 防墮機制主類
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingAntiFallSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAntiFallSystem();

    UFUNCTION(BlueprintCallable, Category = "Anti-Fall")
    void InitializeSystem();

    // 檢測墮落徵象
    UFUNCTION(BlueprintCallable, Category = "Anti-Fall")
    bool DetectCorruptionSign(ECorruptionSignType SignType);

    // 應用防護
    UFUNCTION(BlueprintCallable, Category = "Anti-Fall")
    bool ApplyPrevention(ECorruptionSignType SignType, EPreventionLevel Level);

    // 淨化墮落值
    UFUNCTION(BlueprintCallable, Category = "Anti-Fall")
    float PurifyCorruption(float Amount);

    // 獲取墮落數據
    UFUNCTION(BlueprintCallable, Category = "Anti-Fall")
    FCorruptionData GetCorruptionData(ECorruptionSignType SignType) const;

    // 檢查是否瀕臨墮落
    UFUNCTION(BlueprintCallable, Category = "Anti-Fall")
    bool IsNearCorruption(ECorruptionSignType SignType) const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anti-Fall")
    TArray<FCorruptionData> CorruptionDataArray;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anti-Fall")
    bool bSystemActive;

private:
    void InitializeCorruptionData();
    void MonitorCorruptionLevels();
};
