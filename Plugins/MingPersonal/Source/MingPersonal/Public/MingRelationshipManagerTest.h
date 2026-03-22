#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManagerTest.generated.h"

/**
 * 關係和聲望系統測試運行器
 * 提供 Blueprint 接口來運行系統測試
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipTestRunner : public UObject
{
    GENERATED_BODY()

public:
    /**
     * 運行所有關係和聲望系統測試
     * 在開發和調試時使用
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunRelationshipTests();

    /**
     * 運行特定測試
     * @param TestIndex 測試編號 (1-8)
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunSpecificTest(int32 TestIndex);

    /**
     * 驗證關係系統基本功能
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static bool VerifyRelationshipSystem();

    /**
     * 驗證聲望系統基本功能
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static bool VerifyReputationSystem();

    /**
     * 創建測試數據
     * 用於填充測試環境的關係和聲望數據
     */
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void CreateTestData();
};
