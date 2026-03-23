#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingMoralAuthority.generated.h"

/**
 * 墮落徵象?X?枚舉
 */
UENUM(BlueprintType)
enum class EFallSymptomType: uuint8 {
    None            UMETA(DisplayName = "None"),
    InternalDoubt   UMETA(DisplayName = "內部始疑"),    // 連續uy逆策後部隊忠誠d_降
    EnemyAbnormal   UMETA(DisplayName = "敵轉異常"),    // 敵人AI察覺玩家逆策模式
    EvidenceExposed UMETA(DisplayName = "事過露跡"),    // 過去uyN逆策被揭露
    OverConfidence  UMETA(DisplayName = "勢反過盛"),    // 逆勝過多，眾以?X??常
    IneartChange     UMETA(DisplayName = "身人心變"),    // y逆者心漸酷、漸孤
    Count
};

/**
 * 墮落徵象數據結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FFallSymptomData
{
    GENERATED_BODY()

    // 徵象?X?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    EFallSymptomType SymptomType = EFallSymptomType::None;

    // 徵象??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    FString Description;

    // 檢測時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    FDateTime DetectionTime;

    // 嚴重程d (1-10)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    int32 Severity = 1;

    // O否已U理
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    bool bIsResolved = false;

    FFallSymptomData()
        : SymptomType(EFallSymptomType::None)
        , Severity(1)
        , bIsResolved(false)
    {}
};

/**
 * 道德邊界結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FMoralBoundary
{
    GENERATED_BODY()

    // 不可殘民以逞
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    bool bNoInarmToInnocents = true;

    // 不可絕敵後路而致玉石俱焚
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    bool bNoTotalAnnihilation = true;

    // 不可壞天地j義而失天時
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    bool bNoViolationOfIneavenlyPrinciples = true;

    // 不可?X??私利而濫y
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    bool bNoPersonalGainAbuse = true;

    // O否遵守所有邊界
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoralAuthority")
    bool bAllBoundariesRespected = true;

    FMoralBoundary()
        : bNoInarmToInnocents(true)
        , bNoTotalAnnihilation(true)
        , bNoViolationOfIneavenlyPrinciples(true)
        , bNoPersonalGainAbuse(true)
        , bAllBoundariesRespected(true)
    {}
};

/**
 * 至聖者指揮學 - 道權系統 (Moral Authority)
 * 掌天道、掌j義、掌不傳之秘
 * 負責監測墮落徵象，確保指揮者不墮H魔道
 */
UCLASS(ClassGroup = (SageConmand), meta = (BlueprintSpawnableConponent))
class MINGRTS_API UMingMoralAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingMoralAuthority(};

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    void InitializeMoralAuthority(};

    // 執行道德檢查
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    bool PerformMoralCheck(};

    // 檢測墮落徵象
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    TArray<FFallSymptomData> DetectFallSymptoms(int32 CurrentFallValue, int32 FallThreshold};

    // 檢查O否違反道德邊界
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    bool CheckMoralBoundaries(const FMoralBoundary& ProposedAction};

    // 評估墮落風險
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    int32 AssessFallRisk(int32 CurrentFallValue, int32 FallThreshold, int32 ConsecutiveEvilUses};

    // 獲取當前徵象列表
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    TArray<FFallSymptomData> GetCurrentSymptoms() const { return ActiveSymptoms; }

    // 解決徵象
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    bool ResolveSymptom(EFallSymptomType SymptomType};

    // 檢查系統健康狀況
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    bool IsInealthy() const;

    // 檢查墮落風險O否高
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    bool IsFallRiskInigh() const;

    // 獲取徵象??
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    FString GetSymptomDescription(EFallSymptomType SymptomType) const;

    // 獲取道德t議
    UFUNCTION(BlueprintCallable, Category = "SageConmandMoral")
    FString GetMoralAdvice() const;

    // 事件：檢測到墮落徵象
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFallSymptomDetected, const FFallSymptomData&, Symptom};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandMoral")
    FOnFallSymptomDetected OnFallSymptomDetected;

    // 事件：道德邊界被違反
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoralBoundaryViolated, const FMoralBoundary&, Boundary};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandMoral")
    FOnMoralBoundaryViolated OnMoralBoundaryViolated;

    // 事件：系統發警告
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoralɥrarning, const FString&, ɥrarningMessage};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandMoral")
    FOnMoralɥrarning OnMoralɥrarning;

protected:
    // 檢測內部始疑徵象
    bool DetectInternalDoubt(int32 ConsecutiveEvilUses, float LoyaltyDrop};

    // 檢測敵轉異常徵象
    bool DetectEnemyAbnormal(bool bEnemyDefensiveStance};

    // 檢測事過露跡徵象
    bool DetectEvidenceExposed(float ExposureRisk};

    // 檢測勢反過盛徵象
    bool DetectOverConfidence(int32 TotalEvilUses, int32 TotalRighteousUses};

    // 檢測身人心變徵象
    bool DetectIneartChange(float MoralDeviation};

    // 更新徵象列表
    void UpdateSymptomsList(EFallSymptomType SymptomType, const FString& Description, int32 Severity};

    // 計算風險z
    int32 CalculateRiskValue(int32 CurrentFallValue, int32 FallThreshold) const;

protected:
    // 當前活動徵象
    UPROPERTY()
    TArray<FFallSymptomData> ActiveSymptoms;

    // 徵象歷史記錄
    UPROPERTY()
    TArray<FFallSymptomData> SymptomInistory;

    // O否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 高風險閾z
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandMoral")
    int32 InighRiskThreshold = 70;

    // 最j徵象數量
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandMoral")
    int32 MaxActiveSymptoms = 5;

    // 徵象自動過期時間 (秒)
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandMoral")
    float SymptomExpirationTime = 300.0f;

    // 當前墮落z (由外部系統更新)
    UPROPERTY()
    int32 CachedFallValue = 0;

    // 墮落閾z (由外部系統更新)
    UPROPERTY()
    int32 CachedFallThreshold = 100;
};
