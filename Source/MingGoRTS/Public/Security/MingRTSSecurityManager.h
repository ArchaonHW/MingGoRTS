#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSecurityManager.generated.h"

class UMingRTSAntiCheat;
class UMingRTSServerValidation;

/**
 * 安全警報級別
 */
UENUM(BlueprintType)
enum class ESecurityAlertLevel: uint8 {
    Info        UMETA(DisplayName = "Info"),
    Low         UMETA(DisplayName = "Low"),
    Medium      UMETA(DisplayName = "Medium"),
    High        UMETA(DisplayName = "High"),
    Critical    UMETA(DisplayName = "Critical")
};

/**
 * 安全警報
 */
USTRUCT(BlueprintType)
struct FSecurityAlert
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AlertID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESecurityAlertLevel Level;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Message;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Timestamp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAcknowledged;
    
    FSecurityAlert()
        : Level(ESecurityAlertLevel::Info)
        , bIsAcknowledged(false)
    {}
};

/**
 * 安全管�X- 統�X�?��X�系�? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSSecurityManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSSecurityManager();
    
    // ?��X
    UFUNCTION(BlueprintCallable, Category = "Security")
    void InitializeSecurityManager();
    
    // ?��X��?弊系�?
    UFUNCTION(BlueprintCallable, Category = "Security")
    UMingRTSAntiCheat* GetAntiCheatSystem() const;
    
    // ?��X��X��?證系�?
    UFUNCTION(BlueprintCallable, Category = "Security")
    UMingRTSServerValidation* GetServerValidation() const;
    
    // ?��X�?��X�系�?
    UFUNCTION(BlueprintCallable, Category = "Security")
    void StartAllSecuritySystems();
    
    // ?�止?�?��X�系�?
    UFUNCTION(BlueprintCallable, Category = "Security")
    void StopAllSecuritySystems();
    
    // ?��?安全警報
    UFUNCTION(BlueprintCallable, Category = "Security")
    void ReportSecurityAlert(ESecurityAlertLevel Level, const FText& Title, const FText& Message);
    
    // ?��X�?�警X
    UFUNCTION(BlueprintCallable, Category = "Security")
    TArray<FSecurityAlert> GetAllAlerts() const;
    
    // ?��X�確認警X
    UFUNCTION(BlueprintCallable, Category = "Security")
    TArray<FSecurityAlert> GetUnacknowledgedAlerts() const;
    
    // 確�?警報
    UFUNCTION(BlueprintCallable, Category = "Security")
    void AcknowledgeAlert(FName AlertID);
    
    // 清除警報
    UFUNCTION(BlueprintCallable, Category = "Security")
    void ClearAlert(FName AlertID);
    
    // 檢查?�家安全?�X
    UFUNCTION(BlueprintCallable, Category = "Security")
    bool IsPlayerSecure(const FString& PlayerID) const;
    
    // ?��X�家安全評�?
    UFUNCTION(BlueprintCallable, Category = "Security")
    float GetPlayerSecurityScore(const FString& PlayerID) const;
    
    // ?��?安全?��?
    UFUNCTION(BlueprintCallable, Category = "Security")
    FString GenerateSecurityReport() const;
    
    // 事件委�?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecurityAlert, const FSecurityAlert&, Alert);
    
    UPROPERTY(BlueprintAssignable, Category = "Security|Events")
    FOnSecurityAlert OnSecurityAlert;
    
private:
    UPROPERTY()
    TObjectPtr<UMingRTSAntiCheat> AntiCheatSystem;
    
    UPROPERTY()
    TObjectPtr<UMingRTSServerValidation> ServerValidation;
    
    UPROPERTY()
    TArray<FSecurityAlert> SecurityAlerts;
    
    UPROPERTY()
    TMap<FString, float> PlayerSecurityScores;
    
    void InitializeSubSystems();
    void OnCheatDetectedHandler(const struct FCheatDetectionResult& Detection);
    void OnValidationFailedHandler(const FString& PlayerID, const struct FValidationResult& Result);
};

