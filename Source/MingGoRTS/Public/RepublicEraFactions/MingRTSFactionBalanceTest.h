// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Republic Era Factions - Balance Test Suite

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSFactionBalanceTest.generated.h"

class UMingRTSFactionBase;
class UMingRTSFactionManager;

/**
 * Balance test suite for Republic Era factions
 * Tests faction balance, unit stats, and mechanics
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMingRTSFactionBalanceTest : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSFactionBalanceTest();
    
    /** Initialize the balance test suite */
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    void InitializeBalanceTest();
    
    /** Run all balance tests */
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    bool RunAllBalanceTests();
    
    /** Get test results summary */
    UFUNCTION(BlueprintPure, Category = "Faction|Testing")
    FString GetTestResultsSummary() const;
    
    // Individual tests
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    bool TestFactionAttributes();
    
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    bool TestUnitBalance();
    
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    bool TestMechanicsBalance();
    
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    bool TestDifficultyDistribution();
    
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    bool TestStartingConditions();
    
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    bool TestFactionDiversity();
    
    /** Validate all 12 factions are registered */
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    bool ValidateAllFactionsPresent() const;
    
    /** Generate balance report */
    UFUNCTION(BlueprintCallable, Category = "Faction|Testing")
    FString GenerateBalanceReport() const;

private:
    UPROPERTY()
    TObjectPtr<UMingRTSFactionManager> FactionManager;
    
    FTestResults TestResults;
    
    struct FTestResults
    {
        int32 TotalTests = 0;
        int32 PassedTests = 0;
        int32 FailedTests = 0;
        TArray<FString> ErrorMessages;
        
        void AddPass(const FString& TestName)
        {
            TotalTests++;
            PassedTests++;
        }
        
        void AddFail(const FString& TestName, const FString& Error)
        {
            TotalTests++;
            FailedTests++;
            ErrorMessages.Add(FString::Printf(TEXT("%s: %s"), *TestName, *Error));
        }
        
        float GetPassRate() const
        {
            return TotalTests > 0 ? (float)PassedTests / TotalTests : 0.0f;
        }
    };
    
    // Helper methods
    bool ValidateUnitStats(const FFactionUnit& Unit, const FString& FactionName);
    bool ValidateMechanicValues(const FFactionMechanic& Mechanic, const FString& FactionName);
    bool CheckStatRange(int32 Value, int32 Min, int32 Max, const FString& StatName, const FString& Context);
    bool CheckMultiplierRange(float Value, float Min, float Max, const FString& StatName, const FString& Context);
};
