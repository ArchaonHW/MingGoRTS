#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSecurityManager.generated.h"

class UMingRTSAntiCheat;
class UMingRTSServerValidation;

/**
 * ÂÆâÂÖ®Ë≠¶Â†±Á¥öÂà•
 */
UENUM(BlueprintType)
enum class ESecurityAlertLevel : uint8
{
    Info        UMETA(DisplayName = "‰ø°ÊÅØ"),
    Low         UMETA(DisplayName = "‰Ω?),
    Medium      UMETA(DisplayName = "‰∏?),
    High        UMETA(DisplayName = "È´?),
    Critical    UMETA(DisplayName = "?¥È?")
};

/**
 * ÂÆâÂÖ®Ë≠¶Â†±
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
 * ÂÆâÂÖ®ÁÆ°Á???- Áµ±Á??Ä?âÂ??®Á≥ªÁµ? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSSecurityManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSSecurityManager(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Security")
    void InitializeSecurityManager(};
    
    // ?≤Â??ç‰?ÂºäÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Security")
    UMingRTSAntiCheat* GetAntiCheatSystem() const;
    
    // ?≤Â??çÂ??®È?Ë≠âÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Security")
    UMingRTSServerValidation* GetServerValidation() const;
    
    // ?üÂ??Ä?âÂ??®Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Security")
    void StartAllSecuritySystems(};
    
    // ?úÊ≠¢?Ä?âÂ??®Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Security")
    void StopAllSecuritySystems(};
    
    // ?±Â?ÂÆâÂÖ®Ë≠¶Â†±
    UFUNCTION(BlueprintCallable, Category = "Security")
    void ReportSecurityAlert(ESecurityAlertLevel Level, const FText& Title, const FText& Message};
    
    // ?≤Â??Ä?âË≠¶??    UFUNCTION(BlueprintCallable, Category = "Security")
    TArray<FSecurityAlert> GetAllAlerts() const;
    
    // ?≤Â??™Á¢∫Ë™çË≠¶??    UFUNCTION(BlueprintCallable, Category = "Security")
    TArray<FSecurityAlert> GetUnacknowledgedAlerts() const;
    
    // Á¢∫Ë?Ë≠¶Â†±
    UFUNCTION(BlueprintCallable, Category = "Security")
    void AcknowledgeAlert(FName AlertID};
    
    // Ê∏ÖÈô§Ë≠¶Â†±
    UFUNCTION(BlueprintCallable, Category = "Security")
    void ClearAlert(FName AlertID};
    
    // Ê™¢Êü•?©ÂÆ∂ÂÆâÂÖ®?Ä??    UFUNCTION(BlueprintCallable, Category = "Security")
    bool IsPlayerSecure(const FString& PlayerID) const;
    
    // ?≤Â??©ÂÆ∂ÂÆâÂÖ®Ë©ïÂ?
    UFUNCTION(BlueprintCallable, Category = "Security")
    float GetPlayerSecurityScore(const FString& PlayerID) const;
    
    // ?üÊ?ÂÆâÂÖ®?±Â?
    UFUNCTION(BlueprintCallable, Category = "Security")
    FString GenerateSecurityReport() const;
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecurityAlert, const FSecurityAlert&, Alert};
    
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
    
    void InitializeSubSystems(};
    void OnCheatDetectedHandler(const struct FCheatDetectionResult& Detection};
    void OnValidationFailedHandler(const FString& PlayerID, const struct FValidationResult& Result};
};

