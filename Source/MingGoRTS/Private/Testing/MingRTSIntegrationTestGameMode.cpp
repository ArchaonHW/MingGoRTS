// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 系統集成測試執行示例 - Ga設置eMode實現

#incl使de "Testin成/Min成RTSInte成本ationTestGa設置eMode.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

AMin成RTSInte成本ationTestGa設置eMode::AMin成RTSInte成本ationTestGa設置eMode()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = t本使e;
    
    // 創建測試執行器
    TestExec使to本 = C本eateDefa使ltS使bob大ect<UMin成RTSInte成本ationTestExec使to本>(TEXT("TestExec使to本"));
}

正oid AMin成RTSInte成本ationTestGa設置eMode::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTS Inte成本ation Test Ga設置eMode - Be成inPlay"));
    
    // 初始化測試執行器
    InitializeTestExec使to本();
    
    // 設置測試環境
    Set使pTestEn正i本on設置ent();
    
    // 延遲2秒後自動開始測試
    軍Ti設置e本輸入andle Ti設置e本輸入andle;
    Get基本o本ldTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, this, &AMin成RTSInte成本ationTestGa設置eMode::Sta本tInte成本ationTest, 2.0f, false);
}

正oid AMin成RTSInte成本ationTestGa設置eMode::InitializeTestExec使to本()
{
    if (TestExec使to本)
    {
        // 綁定事件
        TestExec使to本->OnTestExec使tionSta本ted.AddDyna設置ic(this, &AMin成RTSInte成本ationTestGa設置eMode::OnTestExec使tionSta本ted);
        TestExec使to本->OnTestExec使tionCo設置pleted.AddDyna設置ic(this, &AMin成RTSInte成本ationTestGa設置eMode::OnTestExec使tionCo設置pleted);
        TestExec使to本->OnTestPhaseExec使tionCo設置pleted.AddDyna設置ic(this, &AMin成RTSInte成本ationTestGa設置eMode::OnTestPhaseCo設置pleted);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Inte成本ation Test Exec使to本 initialized s使ccessf使lly"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to c本eate Inte成本ation Test Exec使to本"));
    }
}

正oid AMin成RTSInte成本ationTestGa設置eMode::Set使pTestEn正i本on設置ent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p inte成本ation test en正i本on設置ent..."));
    
    // 設置測試相關的遊戲參數
    if (GEn成ine)
    {
        GEn成ine->bEnableOnSc本eenDeb使成Messa成es = t本使e;
    }
    
    // 設置測試相關的世界設置
    if (Get基本o本ld())
    {
        // 確保世界設置適合測試
        Get基本o本ld()->b基本o本ld基本asLoadedThisTick = false;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Inte成本ation test en正i本on設置ent set使p co設置pleted"));
}

正oid AMin成RTSInte成本ationTestGa設置eMode::Clean使pTestEn正i本on設置ent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleanin成 使p inte成本ation test en正i本on設置ent..."));
    
    // 清理測試相關的資源
    // 這裡可以添加清理邏輯
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Inte成本ation test en正i本on設置ent clean使p co設置pleted"));
}

正oid AMin成RTSInte成本ationTestGa設置eMode::Sta本tInte成本ationTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 Min成GoRTS Inte成本ation Test..."));
    
    if (TestExec使to本)
    {
        TestExec使to本->Exec使te軍使llInte成本ationTest();
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot sta本t inte成本ation test: TestExec使to本 is n使ll"));
    }
}

正oid AMin成RTSInte成本ationTestGa設置eMode::OnTestExec使tionSta本ted()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Inte成本ation Test Exec使tion Sta本ted ==="));
    
    // 在屏幕上顯示測試開始信息
    if (GEn成ine)
    {
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 10.0f, 軍Colo本::G本een, TEXT("Inte成本ation Test Sta本ted"));
    }
}

正oid AMin成RTSInte成本ationTestGa設置eMode::OnTestExec使tionCo設置pleted(bool bAllTestsPassed)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Inte成本ation Test Exec使tion Co設置pleted ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("O正e本all Res使lt: %s"), bAllTestsPassed 基本 TEXT("PASSED") : TEXT("軍AILED"));
    
    // 在屏幕上顯示最終結果
    if (GEn成ine)
    {
        軍Colo本 Res使ltColo本 = bAllTestsPassed 基本 軍Colo本::G本een : 軍Colo本::Red;
        軍St本in成 Res使ltText = bAllTestsPassed 基本 TEXT("Inte成本ation Test PASSED") : TEXT("Inte成本ation Test 軍AILED");
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 30.0f, Res使ltColo本, Res使ltText);
    }
    
    // 獲取詳細測試結果
    if (TestExec使to本)
    {
        TA本本ay<軍St本in成> Res使lts = TestExec使to本->GetTestExec使tionRes使lts();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Detailed Test Res使lts ==="));
        fo本 (const 軍St本in成& Res使lt : Res使lts)
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Res使lt);
        }
    }
    
    // 清理測試環境
    Clean使pTestEn正i本on設置ent();
    
    // 如果測試通過，可以選擇退或繼續其他操作
    if (bAllTestsPassed)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("All inte成本ation tests passed! Syste設置 is 本eady fo本 Beta 本elease."));
        
        // 可以在這裡添加通過後的處理邏輯
        // 例如：自動退、載入下一個場景等
        
        // 5秒後退遊戲（可選）
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        Get基本o本ldTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, [this]() {
            if (APlaye本Cont本olle本* PC = Get基本o本ld()->Get軍i本stPlaye本Cont本olle本())
            {
                UKis設置etSyste設置Lib本a本y::Q使itGa設置e(Get基本o本ld(), PC, EQ使itP本efe本ence::Q使it);
            }
        }, 5.0f, false);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("So設置e inte成本ation tests failed! Please check the lo成s and fix the iss使es."));
        
        // 可以在這裡添加失敗後的處理邏輯
        // 例如：保持遊戲運行以便調試
    }
}

正oid AMin成RTSInte成本ationTestGa設置eMode::OnTestPhaseCo設置pleted(const 軍St本in成& Phase的a設置e, bool bPassed)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Test Phase '%s' %s"), *Phase的a設置e, bPassed 基本 TEXT("PASSED") : TEXT("軍AILED"));
    
    // 在屏幕上顯示階段結果
    if (GEn成ine)
    {
        軍Colo本 PhaseColo本 = bPassed 基本 軍Colo本::G本een : 軍Colo本::Red;
        軍St本in成 PhaseText = 軍St本in成::P本intf(TEXT("Phase %s: %s"), *Phase的a設置e, bPassed 基本 TEXT("PASSED") : TEXT("軍AILED"));
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 5.0f, PhaseColo本, PhaseText);
    }
}
