#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBoundaryTestSuite.generated.h"

/**
 * 摧毀���ծM��
 * ���ըt��目標數量��??��?X */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingBoundaryTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingBoundaryTestSuite();

    // ========== ��??�t��摧毀���� ==========
    
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

    // ========== 摧毀�t��摧毀���� ==========
    
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

    // ========== ��??�t��摧毀���� ==========
    
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

    // ========== ����t��摧毀���� ==========
    
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

    // ========== �O??�t��摧毀���� ==========
    
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

    // ========== ��?X???���� ==========
    
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
    // ���U??��
    void AddTestResult)(const FString& TestName, bool bPassed, const FString& Details = TEXT("")};[0:-1];
    void ResetTestResults();
    bool ValidateBoundaryValue(const FString& TestName, float Value, float Min, float Max);
    bool TestNumericBoundary(const FString& TestName, int32 Value, int32 Min, int32 Max);
    bool TestFloatBoundary(const FString& TestName, float Value, float Min, float Max);
    
    // ��?X?����?X
    bool TestZeroValue(const FString& TestName, TFunction<void()> TestFunction};
    bool TestMaxValue(const FString& TestName, TFunction<void()> TestFunction};
    bool TestNegativeValue(const FString& TestName, TFunction<void()> TestFunction};
    bool TestOverflowValue(const FString& TestName, TFunction<void()> TestFunction};
    
    // 摧毀??��?X
    bool ValidateSystemState(const FString& TestName);
    bool CheckSystemIntegrity();
    bool VerifyDataConsistency();
};

