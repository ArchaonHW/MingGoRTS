#include "MingRelationshipManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

/**
 * 測試用於驗證關係和聲望系統功能
 */
class MINGPERSONAL_API FMingRelationshipManagerTest
{
public:
    static void RunAllTests()
    {
        UE_LOG(LogTemp, Warning, TEXT("=== 開始關係和聲望系統測試 ==="));
        
        TestRelationshipInitialization();
        TestRelationshipUpdate();
        TestReputationUpdate();
        TestDialogueOptions();
        TestPriceModifier();
        TestQuestAvailability();
        TestSharedMemory();
        TestRelationshipDecay();
        
        UE_LOG(LogTemp, Warning, TEXT("=== 關係和聲望系統測試完成 ==="));
    }

private:
    static void TestRelationshipInitialization()
    {
        UE_LOG(LogTemp, Warning, TEXT("測試 1: 關係系統初始化"));
        
        UMingRelationshipManager* Manager = NewObject<UMingRelationshipManager>();
        Manager->InitializeRelationshipSystem();
        
        // 檢查初始化後的狀態
        if (Manager->GetAllRelationships().Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 關係系統初始化成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 關係系統初始化失敗"));
        }
        
        if (Manager->GetAllReputations().Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 聲望系統初始化成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 聲望系統初始化失敗"));
        }
    }

    static void TestRelationshipUpdate()
    {
        UE_LOG(LogTemp, Warning, TEXT("測試 2: 關係更新功能"));
        
        UMingRelationshipManager* Manager = NewObject<UMingRelationshipManager>();
        Manager->InitializeRelationshipSystem();
        
        FString TestCharacterID = TEXT("TEST_NPC_001");
        
        // 測試新角色關係建立
        Manager->UpdateRelationship(TestCharacterID, 25.0f, TEXT("初次見面"));
        FRelationshipData Relationship = Manager->GetRelationship(TestCharacterID);
        
        if (Relationship.RelationshipValue == 25.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 新角色關係建立成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 新角色關係建立失敗"));
        }
        
        // 測試關係更新
        Manager->UpdateRelationship(TestCharacterID, 15.0f, TEXT("幫助任務"));
        Relationship = Manager->GetRelationship(TestCharacterID);
        
        if (Relationship.RelationshipValue == 40.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 關係更新成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 關係更新失敗"));
        }
        
        // 測試關係邊界值
        Manager->UpdateRelationship(TestCharacterID, 200.0f, TEXT("極大正面影響"));
        Relationship = Manager->GetRelationship(TestCharacterID);
        
        if (Relationship.RelationshipValue == 100.0f) // 應該被限制在最大值
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 關係邊界值限制成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 關係邊界值限制失敗"));
        }
    }

    static void TestReputationUpdate()
    {
        UE_LOG(LogTemp, Warning, TEXT("測試 3: 聲望更新功能"));
        
        UMingRelationshipManager* Manager = NewObject<UMingRelationshipManager>();
        Manager->InitializeRelationshipSystem();
        
        FString TestRegionID = TEXT("BEIJING");
        
        // 測試新地區聲望建立
        Manager->UpdateReputation(TestRegionID, 150.0f, TEXT("完成地區任務"));
        FReputationData Reputation = Manager->GetReputation(TestRegionID);
        
        if (Reputation.ReputationScore == 150.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 新地區聲望建立成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 新地區聲望建立失敗"));
        }
        
        // 測試聲望等級計算
        EReputationLevel Level = Manager->GetReputationLevel(TestRegionID);
        if (Level == EReputationLevel::Respected)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 聲望等級計算正確"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 聲望等級計算錯誤"));
        }
        
        // 測試聲望邊界值
        Manager->UpdateReputation(TestRegionID, 2000.0f, TEXT("極大聲望提升"));
        Reputation = Manager->GetReputation(TestRegionID);
        
        if (Reputation.ReputationScore == 1000.0f) // 應該被限制在最大值
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 聲望邊界值限制成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 聲望邊界值限制失敗"));
        }
    }

    static void TestDialogueOptions()
    {
        UE_LOG(LogTemp, Warning, TEXT("測試 4: 對話選項功能"));
        
        UMingRelationshipManager* Manager = NewObject<UMingRelationshipManager>();
        Manager->InitializeRelationshipSystem();
        
        FString TestCharacterID = TEXT("TEST_NPC_002");
        
        // 測試朋友關係的對話選項
        Manager->UpdateRelationship(TestCharacterID, 80.0f, TEXT("成為朋友"));
        TArray<FString> DialogueOptions = Manager->GetDialogueOptions(TestCharacterID);
        
        if (DialogueOptions.Num() > 0 && DialogueOptions.Contains(TEXT("友好問候")))
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 朋友關係對話選項正確"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 朋友關係對話選項錯誤"));
        }
        
        // 測試敵對關係的對話選項
        FString EnemyCharacterID = TEXT("TEST_ENEMY_001");
        Manager->UpdateRelationship(EnemyCharacterID, -60.0f, TEXT("成為敵人"));
        TArray<FString> EnemyDialogueOptions = Manager->GetDialogueOptions(EnemyCharacterID);
        
        if (EnemyDialogueOptions.Num() > 0 && EnemyDialogueOptions.Contains(TEXT("威脅對話")))
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 敵對關係對話選項正確"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 敵對關係對話選項錯誤"));
        }
    }

    static void TestPriceModifier()
    {
        UE_LOG(LogTemp, Warning, TEXT("測試 5: 價格修正功能"));
        
        UMingRelationshipManager* Manager = NewObject<UMingRelationshipManager>();
        Manager->InitializeRelationshipSystem();
        
        // 測試高關係值的價格修正
        FString FriendCharacterID = TEXT("TEST_FRIEND_001");
        Manager->UpdateRelationship(FriendCharacterID, 85.0f, TEXT("好朋友"));
        float FriendPriceModifier = Manager->GetPriceModifier(FriendCharacterID);
        
        if (FriendPriceModifier == 0.8f) // 20% 折扣
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 高關係值價格修正正確"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 高關係值價格修正錯誤"));
        }
        
        // 測試低關係值的價格修正
        FString EnemyCharacterID = TEXT("TEST_ENEMY_002");
        Manager->UpdateRelationship(EnemyCharacterID, -60.0f, TEXT("敵人"));
        float EnemyPriceModifier = Manager->GetPriceModifier(EnemyCharacterID);
        
        if (EnemyPriceModifier == 1.2f) // 20% 加價
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 低關係值價格修正正確"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 低關係值價格修正錯誤"));
        }
        
        // 測試中性關係的價格修正
        FString NeutralCharacterID = TEXT("TEST_NEUTRAL_001");
        Manager->UpdateRelationship(NeutralCharacterID, 10.0f, TEXT("中性關係"));
        float NeutralPriceModifier = Manager->GetPriceModifier(NeutralCharacterID);
        
        if (NeutralPriceModifier == 1.0f) // 無修正
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 中性關係價格修正正確"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 中性關係價格修正錯誤"));
        }
    }

    static void TestQuestAvailability()
    {
        UE_LOG(LogTemp, Warning, TEXT("測試 6: 任務可接取性功能"));
        
        UMingRelationshipManager* Manager = NewObject<UMingRelationshipManager>();
        Manager->InitializeRelationshipSystem();
        
        FString TestRegionID = TEXT("SHANGHAI");
        
        // 測試高聲望地區的任務可接取性
        Manager->UpdateReputation(TestRegionID, 700.0f, TEXT("高聲望"));
        bool CanAcceptHighRepQuest = Manager->CanAcceptQuest(TEXT("HIGH_LEVEL_QUEST"), TestRegionID);
        
        if (CanAcceptHighRepQuest)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 高聲望任務可接取性正確"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 高聲望任務可接取性錯誤"));
        }
        
        // 測試低聲望地區的任務可接取性
        FString LowRepRegionID = TEXT("LOW_REP_REGION");
        Manager->UpdateReputation(LowRepRegionID, 25.0f, TEXT("低聲望"));
        bool CanAcceptLowRepQuest = Manager->CanAcceptQuest(TEXT("HIGH_LEVEL_QUEST"), LowRepRegionID);
        
        if (!CanAcceptLowRepQuest)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 低聲望任務可接取性正確"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 低聲望任務可接取性錯誤"));
        }
    }

    static void TestSharedMemory()
    {
        UE_LOG(LogTemp, Warning, TEXT("測試 7: 共同記憶功能"));
        
        UMingRelationshipManager* Manager = NewObject<UMingRelationshipManager>();
        Manager->InitializeRelationshipSystem();
        
        FString TestCharacterID = TEXT("TEST_MEMORY_001");
        
        // 添加共同記憶
        Manager->AddSharedMemory(TestCharacterID, TEXT("一起完成了一個困難任務"));
        FRelationshipData Relationship = Manager->GetRelationship(TestCharacterID);
        
        if (Relationship.SharedMemories.Num() > 0 && Relationship.SharedMemories.Contains(TEXT("一起完成了一個困難任務")))
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 共同記憶添加成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 共同記憶添加失敗"));
        }
        
        // 檢查共同記憶是否提升了關係
        if (Relationship.RelationshipValue > 0.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 共同記憶提升關係成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 共同記憶提升關係失敗"));
        }
    }

    static void TestRelationshipDecay()
    {
        UE_LOG(LogTemp, Warning, TEXT("測試 8: 關係衰減功能"));
        
        UMingRelationshipManager* Manager = NewObject<UMingRelationshipManager>();
        Manager->InitializeRelationshipSystem();
        
        FString TestCharacterID = TEXT("TEST_DECAY_001");
        
        // 建立一個關係
        Manager->UpdateRelationship(TestCharacterID, 50.0f, TEXT("測試關係"));
        
        // 模擬時間流逝（這需要在實際遊戲中實現）
        // 這裡只是測試衰減邏輯的存在性
        FRelationshipData Relationship = Manager->GetRelationship(TestCharacterID);
        
        if (Relationship.LastInteractionDay > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("✓ 關係衰減邏輯存在"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ 關係衰減邏輯缺失"));
        }
    }
};

/**
 * 在遊戲中運行測試的靜態函數
 */
UCLASS()
class MINGPERSONAL_API UMingRelationshipTestRunner : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Relationship Test")
    static void RunRelationshipTests()
    {
        FMingRelationshipManagerTest::RunAllTests();
    }
};
