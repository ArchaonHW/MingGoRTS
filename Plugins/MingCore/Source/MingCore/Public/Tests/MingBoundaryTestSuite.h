#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBoundaryTestSuite.generated.h"

/**
 * ?äÁ?Ê∏¨Ë©¶Â•ó‰ª∂
 * Ê∏¨Ë©¶Á≥ªÁµ±?ÑÈ??åÊ?‰ª∂Â?Ê•µÈ??? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingBoundaryTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingBoundaryTestSuite();

    // ========== Ë≥áÊ?Á≥ªÁµ±?äÁ?Ê∏¨Ë©¶ ==========
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Resource")
    bool TestResourceBoundaryConditions();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Resource")
    bool TestZeroResources();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Resource")
    bool TestMaxResources();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Resource")
    bool TestNegativeResources();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Resource")
    bool TestResourceOverflow();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Resource")
    bool TestFloatingPointResources();

    // ========== ?Æ‰?Á≥ªÁµ±?äÁ?Ê∏¨Ë©¶ ==========
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Unit")
    bool TestUnitBoundaryConditions();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Unit")
    bool TestZeroUnits();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Unit")
    bool TestMaxUnits();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Unit")
    bool TestUnitHealthBoundaries();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Unit")
    bool TestUnitPositionBoundaries();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Unit")
    bool TestInvalidUnitStates();

    // ========== Âª∫Á?Á≥ªÁµ±?äÁ?Ê∏¨Ë©¶ ==========
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Building")
    bool TestBuildingBoundaryConditions();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Building")
    bool TestZeroBuildings();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Building")
    bool TestMaxBuildings();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Building")
    bool TestBuildingLevelBoundaries();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Building")
    bool TestInvalidBuildingStates();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Building")
    bool TestBuildingPositionBoundaries();

    // ========== Á∂≤Áµ°Á≥ªÁµ±?äÁ?Ê∏¨Ë©¶ ==========
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Network")
    bool TestNetworkBoundaryConditions();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Network")
    bool TestZeroConnections();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Network")
    bool TestMaxConnections();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Network")
    bool TestPacketSizeBoundaries();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Network")
    bool TestLatencyBoundaries();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Network")
    bool TestInvalidNetworkStates();

    // ========== ‰øùÂ?Á≥ªÁµ±?äÁ?Ê∏¨Ë©¶ ==========
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Save")
    bool TestSaveSystemBoundaryConditions();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Save")
    bool TestZeroSaveFiles();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Save")
    bool TestMaxSaveFiles();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Save")
    bool TestSaveFileSizeBoundaries();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Save")
    bool TestSaveFrequencyBoundaries();
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test | Save")
    bool TestInvalidSaveStates();

    // ========== Á∂úÂ??äÁ?Ê∏¨Ë©¶ ==========
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test")
    void RunAllBoundaryTests();
    
    UFUNCTION(BlueprintPure, Category = "Boundary Test")
    bool AreAllBoundaryTestsPassed() const;
    
    UFUNCTION(BlueprintPure, Category = "Boundary Test")
    int32 GetPassedTestCount() const;
    
    UFUNCTION(BlueprintPure, Category = "Boundary Test")
    int32 GetFailedTestCount() const;
    
    UFUNCTION(BlueprintPure, Category = "Boundary Test")
    float GetBoundaryTestCoverage() const;
    
    UFUNCTION(BlueprintCallable, Category = "Boundary Test")
    FString GenerateBoundaryTestReport();

protected:
    UPROPERTY()
    bool bAllTestsPassed;
    
    UPROPERTY()
    int32 PassedTests;
    
    UPROPERTY()
    int32 FailedTests;
    
    UPROPERTY()
    TArray<FString> TestResults;

private:
    // ËºîÂä©?ΩÊï∏
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT(""));
    void ResetTestResults();
    bool ValidateBoundaryValue(const FString& TestName, float Value, float Min, float Max);
    bool TestNumericBoundary(const FString& TestName, int32 Value, int32 Min, int32 Max);
    bool TestFloatBoundary(const FString& TestName, float Value, float Min, float Max);
    
    // Ê•µÈ??ºÊ∏¨Ë©¶Ë???    bool TestZeroValue(const FString& TestName, TFunction<void()> TestFunction);
    bool TestMaxValue(const FString& TestName, TFunction<void()> TestFunction);
    bool TestNegativeValue(const FString& TestName, TFunction<void()> TestFunction);
    bool TestOverflowValue(const FString& TestName, TFunction<void()> TestFunction);
    
    // ?Ä?ãÈ?Ë≠âË???    bool ValidateSystemState(const FString& TestName);
    bool CheckSystemIntegrity();
    bool VerifyDataConsistency();
};
