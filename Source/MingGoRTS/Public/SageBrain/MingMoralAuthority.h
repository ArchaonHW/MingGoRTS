#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingMoralAuthority.generated.h"

// 道德影響類型
UENUM(BlueprintType)
enum class EMoralInfluenceType : uint8
{
    None            UMETA(DisplayName = "無"),
    Inspiration     UMETA(DisplayName = "感召"),
    Judgment        UMETA(DisplayName = "審判"),
    Guidance        UMETA(DisplayName = "引導"),
    Purification    UMETA(DisplayName = "淨化"),
    Blessing        UMETA(DisplayName = "祝福")
};

// 道德狀態結構
USTRUCT(BlueprintType)
struct FMoralStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralAuthority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RighteousPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InfluenceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralAura;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FollowerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PublicTrust;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMoralInfluenceType> ActiveInfluences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> MoralModifiers;

    FMoralStatus()
    {
        MoralAuthority = 50.0f;
        RighteousPower = 50.0f;
        InfluenceRadius = 100.0f;
        MoralAura = 1.0f;
        FollowerCount = 0;
        PublicTrust = 50.0f;
    }
};

// 道德行動結構
USTRUCT(BlueprintType)
struct FMoralAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ActionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMoralInfluenceType InfluenceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InfluenceEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Effects;

    FMoralAction()
    {
        ActionName = TEXT("");
        InfluenceType = EMoralInfluenceType::None;
        MoralCost = 0.0f;
        InfluenceEffect = 0.0f;
        Duration = 0.0f;
    }
};

// 道德影響事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoralInfluenceApplied, EMoralInfluenceType, InfluenceType, float, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoralAuthorityChanged, float, NewAuthority);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFollowerGained, int32, NewCount, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPublicTrustChanged, float, NewTrust, const FString&, Reason);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingMoralAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingMoralAuthority();

    // 初始化道權系統
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    void InitializeMoralAuthority();

    // 獲取道德狀態
    UFUNCTION(BlueprintPure, Category = "Moral Authority")
    FMoralStatus GetMoralStatus() const { return CurrentMoralStatus; }

    // 設置道德權威
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    void SetMoralAuthority(float Authority);

    // 應用道德影響
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    bool ApplyMoralInfluence(EMoralInfluenceType InfluenceType, float Intensity, const FString& Target);

    // 執行道德行動
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    bool ExecuteMoralAction(const FString& ActionName);

    // 獲取可用道德行動
    UFUNCTION(BlueprintPure, Category = "Moral Authority")
    TArray<FMoralAction> GetAvailableMoralActions() const;

    // 感召追隨者
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    bool InspireFollowers(const FString& Message, float InfluenceRadius);

    // 進行道德審判
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    bool ExecuteMoralJudgment(const FString& Target, float JudgmentPower);

    // 引導民心
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    bool GuidePublicOpinion(const FString& Direction, float GuidanceStrength);

    // 淨化邪惡
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    bool PurifyEvil(const FString& Target, float PurificationPower);

    // 施加祝福
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    bool BestowBlessing(const FString& Target, float BlessingPower);

    // 計算道德影響範圍
    UFUNCTION(BlueprintPure, Category = "Moral Authority")
    float CalculateInfluenceRadius() const;

    // 檢查道德行動可行性
    UFUNCTION(BlueprintPure, Category = "Moral Authority")
    bool CanExecuteMoralAction(const FString& ActionName) const;

    // 獲取道德效率
    UFUNCTION(BlueprintPure, Category = "Moral Authority")
    float GetMoralEfficiency() const;

    // 應用道德修飾符
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    void ApplyMoralModifier(const FString& ModifierName, float ModifierValue);

    // 移除道德修飾符
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    void RemoveMoralModifier(const FString& ModifierName);

    // 更新公眾信任度
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    void UpdatePublicTrust(float TrustChange, const FString& Reason);

    // 獲取道德影響歷史
    UFUNCTION(BlueprintPure, Category = "Moral Authority")
    TArray<FMoralAction> GetMoralInfluenceHistory() const;

    // 檢查道德平衡
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    void CheckMoralBalance();

    // 恢復道德權威
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    void RestoreMoralAuthority(float Amount);

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Moral Authority")
    FOnMoralInfluenceApplied OnMoralInfluenceApplied;

    UPROPERTY(BlueprintAssignable, Category = "Moral Authority")
    FOnMoralAuthorityChanged OnMoralAuthorityChanged;

    UPROPERTY(BlueprintAssignable, Category = "Moral Authority")
    FOnFollowerGained OnFollowerGained;

    UPROPERTY(BlueprintAssignable, Category = "Moral Authority")
    FOnPublicTrustChanged OnPublicTrustChanged;

protected:
    // 藍圖實現事件
    UFUNCTION(BlueprintImplementableEvent, Category = "Moral Authority")
    void OnMoralAuthorityInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Moral Authority")
    void OnMoralInfluenceStarted(EMoralInfluenceType InfluenceType, const FString& Target);

    UFUNCTION(BlueprintImplementableEvent, Category = "Moral Authority")
    void OnMoralActionCompleted(const FString& ActionName, bool bSuccess);

    UFUNCTION(BlueprintImplementableEvent, Category = "Moral Authority")
    void OnMoralBalanceChanged(float BalanceLevel);

private:
    // 初始化道德行動
    void InitializeMoralActions();
    
    // 計算道德影響效果
    float CalculateInfluenceEffect(EMoralInfluenceType InfluenceType, float Intensity) const;
    
    // 更新道德光環
    void UpdateMoralAura();
    
    // 檢查道德行動條件
    bool CheckActionConditions(const FMoralAction& Action) const;
    
    // 應用行動效果
    void ApplyActionEffects(const FMoralAction& Action);
    
    // 記錄道德行動
    void RecordMoralAction(const FMoralAction& Action);

protected:
    // 當前道德狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    FMoralStatus CurrentMoralStatus;

    // 系統是否已初始化
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    bool bIsInitialized;

    // 道德權威最大值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    float MaxMoralAuthority;

    // 道德影響基礎範圍
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    float BaseInfluenceRadius;

    // 道德恢復速率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    float MoralRestoreRate;

    // 公眾信任影響係數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    float PublicTrustInfluenceFactor;

    // 道德行動映射
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    TMap<FString, FMoralAction> MoralActionsMap;

    // 道德影響歷史
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    TArray<FMoralAction> MoralInfluenceHistory;

    // 活躍道德影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    TMap<EMoralInfluenceType, float> ActiveInfluences;

    // 道德平衡等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    float MoralBalanceLevel;

    // 最大追隨者數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moral Authority")
    int32 MaxFollowers;
};
