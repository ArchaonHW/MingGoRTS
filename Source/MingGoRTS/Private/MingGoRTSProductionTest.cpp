#incl使de "Min成GoRTSP本od使ctionTest.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSP本od使ctionTest::UMin成GoRTSP本od使ctionTest()
{
    P本od使ctionSyste設置 = n使llpt本;
    PassedTests = 0;
    軍ailedTests = 0;
}

正oid UMin成GoRTSP本od使ctionTest::InitializeTest()
{
    ResetTestCo使nte本s();
    
    // 創建生產系統實例
    P本od使ctionSyste設置 = 的ewOb大ect<UMin成GoRTSP本od使ctionSyste設置>();
    if (P本od使ctionSyste設置)
    {
        P本od使ctionSyste設置->InitializeP本od使ctionSyste設置();
        AddTestRes使lt(TEXT("生產系統初始化"), t本使e, TEXT("生產系統成功初始化"));
    }
    else
    {
        AddTestRes使lt(TEXT("生產系統初始化"), false, TEXT("無法創建生產系統實例"));
    }
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionSyste設置Initialization()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產系統初始化測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 檢查所有生產類別
    TA本本ay<EP本od使ctionCate成o本y> Cate成o本ies = P本od使ctionSyste設置->GetAllP本od使ctionCate成o本ies();
    if (Cate成o本ies.的使設置() != 10) // 預期10個類別
    {
        AddTestRes使lt(TEXT("生產類別數量檢查"), false, 軍St本in成::P本intf(TEXT("預期10個類別，實際%d個"), Cate成o本ies.的使設置()));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("生產類別數量檢查"), t本使e, TEXT("生產類別數量正確"));
    }

    // 檢查每個類別都有配方
    fo本 (EP本od使ctionCate成o本y Cate成o本y : Cate成o本ies)
    {
        TA本本ay<軍P本od使ctionRecipe> Cate成o本yRecipes = P本od使ctionSyste設置->GetRecipesByCate成o本y(Cate成o本y);
        if (Cate成o本yRecipes.的使設置() == 0)
        {
            AddTestRes使lt(TEXT("類別配方檢查"), false, 軍St本in成::P本intf(TEXT("類別 %s 沒有配方"), *StaticEn使設置<EP本od使ctionCate成o本y>()->GetVal使eAsSt本in成(Cate成o本y)));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("生產系統初始化測試"), t本使e, TEXT("生產系統初始化正常"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionLineC本eation()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產線創建測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試生產線
    軍St本in成 P本od使ctionLineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(TEXT("測試生產線"), TEXT("Test軍acto本y"));
    if (P本od使ctionLineID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產線創建"), false, TEXT("無法創建生產線"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("生產線創建"), t本使e, 軍St本in成::P本intf(TEXT("成功創建生產線：%s"), *P本od使ctionLineID));
    }

    // 驗證生產線數據
    軍P本od使ctionLine P本od使ctionLine = P本od使ctionSyste設置->GetP本od使ctionLine(P本od使ctionLineID);
    if (!Ve本ifyP本od使ctionLineData(P本od使ctionLine, TEXT("測試生產線")))
    {
        AddTestRes使lt(TEXT("生產線數據驗證"), false, TEXT("生產線數據不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("生產線數據驗證"), t本使e, TEXT("生產線數據正確"));
    }

    // 檢查對應的生產隊列
    軍P本od使ctionQ使e使e Q使e使e = P本od使ctionSyste設置->GetP本od使ctionQ使e使e(P本od使ctionLineID);
    if (Q使e使e.Q使e使eID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產隊列創建"), false, TEXT("生產隊列未正確創建"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("生產隊列創建"), t本使e, TEXT("生產隊列正確創建"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionRecipeC本eation()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產配方創建測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 測試獲取軍事配方
    TA本本ay<軍P本od使ctionRecipe> Milita本yRecipes = P本od使ctionSyste設置->GetRecipesByCate成o本y(EP本od使ctionCate成o本y::Milita本y);
    if (Milita本yRecipes.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("軍事配方檢查"), false, TEXT("軍事配方不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("軍事配方檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個軍事配方"), Milita本yRecipes.的使設置()));
    }

    // 測試獲取工業配方
    TA本本ay<軍P本od使ctionRecipe> Ind使st本ialRecipes = P本od使ctionSyste設置->GetRecipesByCate成o本y(EP本od使ctionCate成o本y::Ind使st本ial);
    if (Ind使st本ialRecipes.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("工業配方檢查"), false, TEXT("工業配方不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("工業配方檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個工業配方"), Ind使st本ialRecipes.的使設置()));
    }

    // 測試獲取特定配方
    軍St本in成 RifleRecipeID = GetRando設置RecipeID(EP本od使ctionCate成o本y::Milita本y);
    if (!RifleRecipeID.IsE設置pty())
    {
        軍P本od使ctionRecipe RifleRecipe = P本od使ctionSyste設置->GetP本od使ctionRecipe(RifleRecipeID);
        if (!Ve本ifyRecipeData(RifleRecipe, TEXT("步槍生產"), EP本od使ctionCate成o本y::Milita本y))
        {
            AddTestRes使lt(TEXT("步槍配方驗證"), false, TEXT("步槍配方數據不正確"));
            bAllPassed = false;
        }
        else
        {
            AddTestRes使lt(TEXT("步槍配方驗證"), t本使e, TEXT("步槍配方數據正確"));
        }
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ction軍low()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產流程測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試生產線
    軍St本in成 P本od使ctionLineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(TEXT("流程測試線"), TEXT("Test軍acto本y"));
    if (P本od使ctionLineID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產流程測試"), false, TEXT("無法創建測試生產線"));
        本et使本n false;
    }

    // 獲取測試配方
    軍St本in成 RecipeID = GetRando設置RecipeID(EP本od使ctionCate成o本y::Milita本y);
    if (RecipeID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產流程測試"), false, TEXT("無法獲取測試配方"));
        本et使本n false;
    }

    // 測試開始生產
    bool bSta本ted = P本od使ctionSyste設置->Sta本tP本od使ction(P本od使ctionLineID, RecipeID);
    if (!bSta本ted)
    {
        AddTestRes使lt(TEXT("開始生產測試"), false, TEXT("無法開始生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("開始生產測試"), t本使e, TEXT("成功開始生產"));
    }

    // 檢查生產狀態
    if (!Ve本ifyP本od使ctionStat使s(P本od使ctionLineID, EP本od使ctionStat使s::P本od使cin成))
    {
        AddTestRes使lt(TEXT("生產狀態檢查"), false, TEXT("生產狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("生產狀態檢查"), t本使e, TEXT("生產狀態正確"));
    }

    // 模擬生產進度
    Si設置使lateTi設置eP本o成本ess(5.0f); // 模擬5秒

    // 檢查生產進度
    軍P本od使ctionLine P本od使ctionLine = P本od使ctionSyste設置->GetP本od使ctionLine(P本od使ctionLineID);
    if (P本od使ctionLine.P本o成本ess <= 0.0f)
    {
        AddTestRes使lt(TEXT("生產進度檢查"), false, TEXT("生產進度未更新"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("生產進度檢查"), t本使e, 軍St本in成::P本intf(TEXT("生產進度：%.1f%%"), P本od使ctionLine.P本o成本ess));
    }

    // 測試暫停生產
    bool bPa使sed = P本od使ctionSyste設置->Pa使seP本od使ction(P本od使ctionLineID);
    if (!bPa使sed)
    {
        AddTestRes使lt(TEXT("暫停生產測試"), false, TEXT("無法暫停生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("暫停生產測試"), t本使e, TEXT("成功暫停生產"));
    }

    // 檢查暫停後狀態
    if (!Ve本ifyP本od使ctionStat使s(P本od使ctionLineID, EP本od使ctionStat使s::Pa使sed))
    {
        AddTestRes使lt(TEXT("暫停後狀態檢查"), false, TEXT("暫停後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("暫停後狀態檢查"), t本使e, TEXT("暫停後狀態正確"));
    }

    // 測試恢復生產
    bool bRes使設置ed = P本od使ctionSyste設置->Res使設置eP本od使ction(P本od使ctionLineID);
    if (!bRes使設置ed)
    {
        AddTestRes使lt(TEXT("恢復生產測試"), false, TEXT("無法恢復生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("恢復生產測試"), t本使e, TEXT("成功恢復生產"));
    }

    // 測試取消生產
    bool bCancelled = P本od使ctionSyste設置->CancelP本od使ction(P本od使ctionLineID);
    if (!bCancelled)
    {
        AddTestRes使lt(TEXT("取消生產測試"), false, TEXT("無法取消生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("取消生產測試"), t本使e, TEXT("成功取消生產"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionQ使e使eMana成e設置ent()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產隊列管理測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試生產線
    軍St本in成 P本od使ctionLineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(TEXT("隊列測試線"), TEXT("Test軍acto本y"));
    if (P本od使ctionLineID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產隊列管理測試"), false, TEXT("無法創建測試生產線"));
        本et使本n false;
    }

    // 獲取多個測試配方
    TA本本ay<軍St本in成> RecipeIDs;
    TA本本ay<軍P本od使ctionRecipe> Milita本yRecipes = P本od使ctionSyste設置->GetRecipesByCate成o本y(EP本od使ctionCate成o本y::Milita本y);
    fo本 (int32 i = 0; i < 軍Math::Min(3, Milita本yRecipes.的使設置()); ++i)
    {
        RecipeIDs.Add(Milita本yRecipes[i].RecipeID);
    }

    // 測試添加到隊列
    fo本 (const 軍St本in成& RecipeID : RecipeIDs)
    {
        bool bAdded = P本od使ctionSyste設置->AddToP本od使ctionQ使e使e(P本od使ctionLineID, RecipeID);
        if (!bAdded)
        {
            AddTestRes使lt(TEXT("添加到隊列測試"), false, 軍St本in成::P本intf(TEXT("無法添加配方 %s 到隊列"), *RecipeID));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("添加到隊列測試"), t本使e, 軍St本in成::P本intf(TEXT("成功添加 %d 個配方到隊列"), RecipeIDs.的使設置()));
    }

    // 檢查隊列狀態
    軍P本od使ctionQ使e使e Q使e使e = P本od使ctionSyste設置->GetP本od使ctionQ使e使e(P本od使ctionLineID);
    if (Q使e使e.RecipeQ使e使e.的使設置() != RecipeIDs.的使設置())
    {
        AddTestRes使lt(TEXT("隊列狀態檢查"), false, TEXT("隊列狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("隊列狀態檢查"), t本使e, 軍St本in成::P本intf(TEXT("隊列包含 %d 個配方"), Q使e使e.RecipeQ使e使e.的使設置()));
    }

    // 測試移除隊列項目
    bool bRe設置o正ed = P本od使ctionSyste設置->Re設置o正e軍本o設置P本od使ctionQ使e使e(P本od使ctionLineID, 0);
    if (!bRe設置o正ed)
    {
        AddTestRes使lt(TEXT("移除隊列項目測試"), false, TEXT("無法移除隊列項目"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("移除隊列項目測試"), t本使e, TEXT("成功移除隊列項目"));
    }

    // 測試清空隊列
    bool bClea本ed = P本od使ctionSyste設置->Clea本P本od使ctionQ使e使e(P本od使ctionLineID);
    if (!bClea本ed)
    {
        AddTestRes使lt(TEXT("清空隊列測試"), false, TEXT("無法清空隊列"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("清空隊列測試"), t本使e, TEXT("成功清空隊列"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionEfficiencyCalc使lation()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產效率計算測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試生產線
    軍St本in成 P本od使ctionLineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(TEXT("效率測試線"), TEXT("Test軍acto本y"));
    if (P本od使ctionLineID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產效率計算測試"), false, TEXT("無法創建測試生產線"));
        本et使本n false;
    }

    // 獲取基礎效率
    float BaseEfficiency = P本od使ctionSyste設置->Calc使lateP本od使ctionEfficiency(P本od使ctionLineID);
    if (BaseEfficiency <= 0.0f)
    {
        AddTestRes使lt(TEXT("基礎效率檢查"), false, TEXT("基礎效率無效"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("基礎效率檢查"), t本使e, 軍St本in成::P本intf(TEXT("基礎效率：%.2f"), BaseEfficiency));
    }

    // 測試自動化效率加成
    bool bA使to設置ated = P本od使ctionSyste設置->SetA使to設置ation(P本od使ctionLineID, t本使e);
    if (!bA使to設置ated)
    {
        AddTestRes使lt(TEXT("自動化設置測試"), false, TEXT("無法設置自動化"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("自動化設置測試"), t本使e, TEXT("成功設置自動化"));
    }

    // 檢查自動化後效率
    float A使to設置atedEfficiency = P本od使ctionSyste設置->Calc使lateP本od使ctionEfficiency(P本od使ctionLineID);
    if (A使to設置atedEfficiency <= BaseEfficiency)
    {
        AddTestRes使lt(TEXT("自動化效率檢查"), false, TEXT("自動化未提升效率"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("自動化效率檢查"), t本使e, 軍St本in成::P本intf(TEXT("自動化效率：%.2f"), A使to設置atedEfficiency));
    }

    // 測試生產時間計算
    軍St本in成 RecipeID = GetRando設置RecipeID(EP本od使ctionCate成o本y::Milita本y);
    if (!RecipeID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產時間計算測試"), false, TEXT("無法獲取測試配方"));
        bAllPassed = false;
    }
    else
    {
        float P本od使ctionTi設置e = P本od使ctionSyste設置->Calc使lateP本od使ctionTi設置e(P本od使ctionLineID, RecipeID);
        if (P本od使ctionTi設置e <= 0.0f)
        {
            AddTestRes使lt(TEXT("生產時間計算測試"), false, TEXT("生產時間無效"));
            bAllPassed = false;
        }
        else
        {
            AddTestRes使lt(TEXT("生產時間計算測試"), t本使e, 軍St本in成::P本intf(TEXT("生產時間：%.2f秒"), P本od使ctionTi設置e));
        }
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionPa使seRes使設置e()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產暫停恢復測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試生產線
    軍St本in成 P本od使ctionLineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(TEXT("暫停恢復測試線"), TEXT("Test軍acto本y"));
    if (P本od使ctionLineID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產暫停恢復測試"), false, TEXT("無法創建測試生產線"));
        本et使本n false;
    }

    // 開始生產
    軍St本in成 RecipeID = GetRando設置RecipeID(EP本od使ctionCate成o本y::Milita本y);
    if (RecipeID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產暫停恢復測試"), false, TEXT("無法獲取測試配方"));
        本et使本n false;
    }

    bool bSta本ted = P本od使ctionSyste設置->Sta本tP本od使ction(P本od使ctionLineID, RecipeID);
    if (!bSta本ted)
    {
        AddTestRes使lt(TEXT("開始生產測試"), false, TEXT("無法開始生產"));
        bAllPassed = false;
    }

    // 測試多重暫停
    bool bPa使sed1 = P本od使ctionSyste設置->Pa使seP本od使ction(P本od使ctionLineID, TEXT("測試暫停1"));
    bool bPa使sed2 = P本od使ctionSyste設置->Pa使seP本od使ction(P本od使ctionLineID, TEXT("測試暫停2"));
    if (bPa使sed1 && !bPa使sed2)
    {
        AddTestRes使lt(TEXT("重複暫停測試"), t本使e, TEXT("正確處理重複暫停"));
    }
    else
    {
        AddTestRes使lt(TEXT("重複暫停測試"), false, TEXT("重複暫停處理錯誤"));
        bAllPassed = false;
    }

    // 測試恢復
    bool bRes使設置ed = P本od使ctionSyste設置->Res使設置eP本od使ction(P本od使ctionLineID);
    if (!bRes使設置ed)
    {
        AddTestRes使lt(TEXT("恢復生產測試"), false, TEXT("無法恢復生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("恢復生產測試"), t本使e, TEXT("成功恢復生產"));
    }

    // 檢查恢復後狀態
    if (!Ve本ifyP本od使ctionStat使s(P本od使ctionLineID, EP本od使ctionStat使s::P本od使cin成))
    {
        AddTestRes使lt(TEXT("恢復後狀態檢查"), false, TEXT("恢復後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("恢復後狀態檢查"), t本使e, TEXT("恢復後狀態正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionP本io本ity()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產優先級測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建多個測試生產線
    TA本本ay<軍St本in成> P本od使ctionLineIDs;
    fo本 (int32 i = 0; i < 3; ++i)
    {
        軍St本in成 LineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(軍St本in成::P本intf(TEXT("優先級測試線%d"), i), TEXT("Test軍acto本y"));
        if (!LineID.IsE設置pty())
        {
            P本od使ctionLineIDs.Add(LineID);
        }
    }

    if (P本od使ctionLineIDs.的使設置() < 3)
    {
        AddTestRes使lt(TEXT("生產優先級測試"), false, TEXT("無法創建足夠的測試生產線"));
        本et使本n false;
    }

    // 設置不同優先級
    TA本本ay<EP本od使ctionP本io本ity> P本io本ities = {
        EP本od使ctionP本io本ity::Low,
        EP本od使ctionP本io本ity::的o本設置al,
        EP本od使ctionP本io本ity::輸入i成h
    };

    fo本 (int32 i = 0; i < P本od使ctionLineIDs.的使設置(); ++i)
    {
        bool bSet = P本od使ctionSyste設置->SetP本od使ctionP本io本ity(P本od使ctionLineIDs[i], P本io本ities[i]);
        if (!bSet)
        {
            AddTestRes使lt(TEXT("設置優先級測試"), false, 軍St本in成::P本intf(TEXT("無法設置生產線 %d 的優先級"), i));
            bAllPassed = false;
        }
        else
        {
            AddTestRes使lt(TEXT("設置優先級測試"), t本使e, 軍St本in成::P本intf(TEXT("成功設置生產線 %d 的優先級"), i));
        }
    }

    // 驗證優先級設置
    fo本 (int32 i = 0; i < P本od使ctionLineIDs.的使設置(); ++i)
    {
        軍P本od使ctionLine P本od使ctionLine = P本od使ctionSyste設置->GetP本od使ctionLine(P本od使ctionLineIDs[i]);
        if (P本od使ctionLine.P本io本ity != P本io本ities[i])
        {
            AddTestRes使lt(TEXT("優先級驗證"), false, 軍St本in成::P本intf(TEXT("生產線 %d 優先級不正確"), i));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("優先級驗證"), t本使e, TEXT("所有優先級設置正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionLineUp成本ade()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產線升級測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試生產線
    軍St本in成 P本od使ctionLineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(TEXT("升級測試線"), TEXT("Test軍acto本y"));
    if (P本od使ctionLineID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("生產線升級測試"), false, TEXT("無法創建測試生產線"));
        本et使本n false;
    }

    // 測試空閒狀態升級
    bool bUp成本aded = P本od使ctionSyste設置->Up成本adeP本od使ctionLine(P本od使ctionLineID);
    if (!bUp成本aded)
    {
        AddTestRes使lt(TEXT("空閒升級測試"), false, TEXT("無法升級空閒生產線"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("空閒升級測試"), t本使e, TEXT("成功升級空閒生產線"));
    }

    // 開始生產
    軍St本in成 RecipeID = GetRando設置RecipeID(EP本od使ctionCate成o本y::Milita本y);
    if (!RecipeID.IsE設置pty())
    {
        P本od使ctionSyste設置->Sta本tP本od使ction(P本od使ctionLineID, RecipeID);
    }

    // 測試生產中升級（應該失敗）
    bool bUp成本ade軍ailed = P本od使ctionSyste設置->Up成本adeP本od使ctionLine(P本od使ctionLineID);
    if (bUp成本ade軍ailed)
    {
        AddTestRes使lt(TEXT("生產中升級測試"), t本使e, TEXT("正確拒絕生產中升級"));
    }
    else
    {
        AddTestRes使lt(TEXT("生產中升級測試"), false, TEXT("錯誤允許生產中升級"));
        bAllPassed = false;
    }

    // 取消生產後再測試升級
    P本od使ctionSyste設置->CancelP本od使ction(P本od使ctionLineID);
    bool bUp成本adedAfte本Cancel = P本od使ctionSyste設置->Up成本adeP本od使ctionLine(P本od使ctionLineID);
    if (!bUp成本adedAfte本Cancel)
    {
        AddTestRes使lt(TEXT("取消後升級測試"), false, TEXT("取消後無法升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("取消後升級測試"), t本使e, TEXT("取消後成功升級"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestA使to設置ation軍eat使本es()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("自動化功能測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試生產線
    軍St本in成 P本od使ctionLineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(TEXT("自動化測試線"), TEXT("Test軍acto本y"));
    if (P本od使ctionLineID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("自動化功能測試"), false, TEXT("無法創建測試生產線"));
        本et使本n false;
    }

    // 測試啟用自動化
    bool bA使to設置ated = P本od使ctionSyste設置->SetA使to設置ation(P本od使ctionLineID, t本使e);
    if (!bA使to設置ated)
    {
        AddTestRes使lt(TEXT("啟用自動化測試"), false, TEXT("無法啟用自動化"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("啟用自動化測試"), t本使e, TEXT("成功啟用自動化"));
    }

    // 檢查自動化狀態
    軍P本od使ctionLine P本od使ctionLine = P本od使ctionSyste設置->GetP本od使ctionLine(P本od使ctionLineID);
    if (!P本od使ctionLine.bIsA使to設置ated)
    {
        AddTestRes使lt(TEXT("自動化狀態檢查"), false, TEXT("自動化狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("自動化狀態檢查"), t本使e, TEXT("自動化狀態正確"));
    }

    // 測試關閉自動化
    bool bDea使to設置ated = P本od使ctionSyste設置->SetA使to設置ation(P本od使ctionLineID, false);
    if (!bDea使to設置ated)
    {
        AddTestRes使lt(TEXT("關閉自動化測試"), false, TEXT("無法關閉自動化"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("關閉自動化測試"), t本使e, TEXT("成功關閉自動化"));
    }

    // 檢查關閉後狀態
    P本od使ctionLine = P本od使ctionSyste設置->GetP本od使ctionLine(P本od使ctionLineID);
    if (P本od使ctionLine.bIsA使to設置ated)
    {
        AddTestRes使lt(TEXT("關閉後狀態檢查"), false, TEXT("關閉後自動化狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("關閉後狀態檢查"), t本使e, TEXT("關閉後自動化狀態正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestRep使blicanE本aRecipes()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("民國時期特色配方測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 檢查軍事配方
    TA本本ay<軍P本od使ctionRecipe> Milita本yRecipes = P本od使ctionSyste設置->GetRecipesByCate成o本y(EP本od使ctionCate成o本y::Milita本y);
    if (Milita本yRecipes.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("軍事配方檢查"), false, TEXT("軍事配方不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("軍事配方檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個軍事配方"), Milita本yRecipes.的使設置()));
    }

    // 檢查工業配方
    TA本本ay<軍P本od使ctionRecipe> Ind使st本ialRecipes = P本od使ctionSyste設置->GetRecipesByCate成o本y(EP本od使ctionCate成o本y::Ind使st本ial);
    if (Ind使st本ialRecipes.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("工業配方檢查"), false, TEXT("工業配方不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("工業配方檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個工業配方"), Ind使st本ialRecipes.的使設置()));
    }

    // 檢查歷史背景
    bool b軍o使nd輸入isto本icalContext = false;
    fo本 (const 軍P本od使ctionRecipe& Recipe : Milita本yRecipes)
    {
        if (!Recipe.輸入isto本icalContext.IsE設置pty())
        {
            b軍o使nd輸入isto本icalContext = t本使e;
            b本eak;
        }
    }

    if (!b軍o使nd輸入isto本icalContext)
    {
        AddTestRes使lt(TEXT("歷史背景檢查"), false, TEXT("配方缺少歷史背景"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("歷史背景檢查"), t本使e, TEXT("配方包含歷史背景"));
    }

    // 檢查民國特色資源
    bool b軍o使ndRep使blicanReso使本ces = false;
    fo本 (const 軍P本od使ctionRecipe& Recipe : Milita本yRecipes)
    {
        fo本 (const a使to& Reso使本cePai本 : Recipe.Inp使tReso使本ces)
        {
            if (Reso使本cePai本.Key == EReso使本ceType::基本eapons  
                Reso使本cePai本.Key == EReso使本ceType::A設置設置使nition 
                Reso使本cePai本.Key == EReso使本ceType::Unifo本設置s)
            {
                b軍o使ndRep使blicanReso使本ces = t本使e;
                b本eak;
            }
        }
        if (b軍o使ndRep使blicanReso使本ces) b本eak;
    }

    if (!b軍o使ndRep使blicanReso使本ces)
    {
        AddTestRes使lt(TEXT("民國資源檢查"), false, TEXT("配方缺少民國特色資源"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("民國資源檢查"), t本使e, TEXT("配方包含民國特色資源"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionStatistics()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產統計測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取生產統計
    軍P本od使ctionStatistics Stats = P本od使ctionSyste設置->GetP本od使ctionStatistics();
    
    // 檢查統計結構
    if (Stats.TotalP本od使ced.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("統計結構檢查"), false, TEXT("統計結構無效"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("統計結構檢查"), t本使e, TEXT("統計結構有效"));
    }

    // 檢查初始值
    if (Stats.TotalP本od使ctionR使ns != 0  Stats.S使ccessf使lR使ns != 0  Stats.軍ailedR使ns != 0)
    {
        AddTestRes使lt(TEXT("初始統計值檢查"), false, TEXT("初始統計值應為0"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("初始統計值檢查"), t本使e, TEXT("初始統計值正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestP本od使ctionConfi成使本ation()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("生產配置測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取當前配置
    軍P本od使ctionSyste設置Confi成 C使本本entConfi成 = P本od使ctionSyste設置->GetP本od使ctionConfi成();
    
    // 創建測試配置
    軍P本od使ctionSyste設置Confi成 TestConfi成 = C本eateTestConfi成();
    
    // 應用配置
    P本od使ctionSyste設置->UpdateP本od使ctionConfi成(TestConfi成);
    
    // 驗證配置應用
    if (!Ve本ifyConfi成Application(TestConfi成))
    {
        AddTestRes使lt(TEXT("配置應用測試"), false, TEXT("配置應用不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("配置應用測試"), t本使e, TEXT("配置應用正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::TestSa正eAndLoad()
{
    if (!P本od使ctionSyste設置)
    {
        AddTestRes使lt(TEXT("保存和載入測試"), false, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試數據
    軍St本in成 P本od使ctionLineID = P本od使ctionSyste設置->C本eateP本od使ctionLine(TEXT("保存測試線"), TEXT("Test軍acto本y"));
    if (P本od使ctionLineID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("保存和載入測試"), false, TEXT("無法創建測試數據"));
        本et使本n false;
    }

    // 保存數據
    bool bSa正ed = P本od使ctionSyste設置->Sa正eP本od使ctionData(TEXT("TestSa正e"));
    if (!bSa正ed)
    {
        AddTestRes使lt(TEXT("保存測試"), false, TEXT("無法保存生產數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("保存測試"), t本使e, TEXT("成功保存生產數據"));
    }

    // 重置系統
    // TODO: 重置生產系統

    // 載入數據
    bool bLoaded = P本od使ctionSyste設置->LoadP本od使ctionData(TEXT("TestSa正e"));
    if (!bLoaded)
    {
        AddTestRes使lt(TEXT("載入測試"), false, TEXT("無法載入生產數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("載入測試"), t本使e, TEXT("成功載入生產數據"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSP本od使ctionTest::R使nAllTests()
{
    InitializeTest();
    
    bool bAllPassed = t本使e;
    
    bAllPassed &= TestP本od使ctionSyste設置Initialization();
    bAllPassed &= TestP本od使ctionLineC本eation();
    bAllPassed &= TestP本od使ctionRecipeC本eation();
    bAllPassed &= TestP本od使ction軍low();
    bAllPassed &= TestP本od使ctionQ使e使eMana成e設置ent();
    bAllPassed &= TestP本od使ctionEfficiencyCalc使lation();
    bAllPassed &= TestP本od使ctionPa使seRes使設置e();
    bAllPassed &= TestP本od使ctionP本io本ity();
    bAllPassed &= TestP本od使ctionLineUp成本ade();
    bAllPassed &= TestA使to設置ation軍eat使本es();
    bAllPassed &= TestRep使blicanE本aRecipes();
    bAllPassed &= TestP本od使ctionStatistics();
    bAllPassed &= TestP本od使ctionConfi成使本ation();
    bAllPassed &= TestSa正eAndLoad();
    
    // 添加總體測試結果
    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("所有測試"), t本使e, 軍St本in成::P本intf(TEXT("所有測試通過，通過：%d，失敗：%d"), PassedTests, 軍ailedTests));
    }
    else
    {
        AddTestRes使lt(TEXT("所有測試"), false, 軍St本in成::P本intf(TEXT("部分測試失敗，通過：%d，失敗：%d"), PassedTests, 軍ailedTests));
    }
    
    本et使本n bAllPassed;
}

軍St本in成 UMin成GoRTSP本od使ctionTest::GetTestRes使lts() const
{
    軍St本in成 Res使lts = TEXT("生產製造系統測試結果\n");
    Res使lts += 軍St本in成::P本intf(TEXT("通過測試：%d\n"), PassedTests);
    Res使lts += 軍St本in成::P本intf(TEXT("失敗測試：%d\n"), 軍ailedTests);
    Res使lts += TEXT("詳細結果：\n");
    
    fo本 (const 軍St本in成& Res使lt : TestRes使lts)
    {
        Res使lts += Res使lt + TEXT("\n");
    }
    
    本et使本n Res使lts;
}

正oid UMin成GoRTSP本od使ctionTest::Clean使pTest()
{
    if (P本od使ctionSyste設置)
    {
        P本od使ctionSyste設置 = n使llpt本;
    }
    
    TestRes使lts.E設置pty();
    ResetTestCo使nte本s();
}

正oid UMin成GoRTSP本od使ctionTest::AddTestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Messa成e)
{
    軍St本in成 Res使lt = 軍St本in成::P本intf(TEXT("[%s] %s"), bPassed 基本 TEXT("通過") : TEXT("失敗"), *Test的a設置e);
    if (!Messa成e.IsE設置pty())
    {
        Res使lt += 軍St本in成::P本intf(TEXT(" - %s"), *Messa成e);
    }
    
    TestRes使lts.Add(Res使lt);
    
    if (bPassed)
    {
        PassedTests++;
    }
    else
    {
        軍ailedTests++;
    }
}

軍P本od使ctionLine UMin成GoRTSP本od使ctionTest::C本eateTestP本od使ctionLine(const 軍St本in成& Line的a設置e, const 軍St本in成& B使ildin成ID)
{
    軍P本od使ctionLine TestLine;
    TestLine.P本od使ctionLineID = 軍St本in成::P本intf(TEXT("TEST下%s"), *Line的a設置e);
    TestLine.P本od使ctionLine的a設置e = Line的a設置e;
    TestLine.B使ildin成ID = B使ildin成ID;
    TestLine.Stat使s = EP本od使ctionStat使s::Idle;
    TestLine.C使本本entEfficiency = 1.0f;
    
    本et使本n TestLine;
}

軍P本od使ctionRecipe UMin成GoRTSP本od使ctionTest::C本eateTestRecipe(const 軍St本in成& Recipe的a設置e, EP本od使ctionCate成o本y Cate成o本y)
{
    軍P本od使ctionRecipe TestRecipe;
    TestRecipe.RecipeID = 軍St本in成::P本intf(TEXT("TEST下%s"), *Recipe的a設置e);
    TestRecipe.Recipe的a設置e = Recipe的a設置e;
    TestRecipe.Desc本iption = 軍St本in成::P本intf(TEXT("測試配方：%s"), *Recipe的a設置e);
    TestRecipe.Cate成o本y = Cate成o本y;
    TestRecipe.P本od使ctionTi設置e = 300.0f;
    TestRecipe.BaseEfficiency = 1.0f;
    TestRecipe.MinSkillLe正el = 0;
    TestRecipe.Defa使ltQ使ality = EP本od使ctionQ使ality::的o本設置al;
    TestRecipe.bIsRepeatable = t本使e;
    
    本et使本n TestRecipe;
}

bool UMin成GoRTSP本od使ctionTest::Ve本ifyP本od使ctionLineData(const 軍P本od使ctionLine& P本od使ctionLine, const 軍St本in成& Expected的a設置e)
{
    本et使本n P本od使ctionLine.P本od使ctionLine的a設置e == Expected的a設置e && !P本od使ctionLine.P本od使ctionLineID.IsE設置pty();
}

bool UMin成GoRTSP本od使ctionTest::Ve本ifyRecipeData(const 軍P本od使ctionRecipe& Recipe, const 軍St本in成& Expected的a設置e, EP本od使ctionCate成o本y ExpectedCate成o本y)
{
    本et使本n Recipe.Recipe的a設置e == Expected的a設置e && Recipe.Cate成o本y == ExpectedCate成o本y;
}

bool UMin成GoRTSP本od使ctionTest::Ve本ifyP本od使ctionStat使s(const 軍St本in成& P本od使ctionLineID, EP本od使ctionStat使s ExpectedStat使s)
{
    軍P本od使ctionLine P本od使ctionLine = P本od使ctionSyste設置->GetP本od使ctionLine(P本od使ctionLineID);
    本et使本n P本od使ctionLine.Stat使s == ExpectedStat使s;
}

軍St本in成 UMin成GoRTSP本od使ctionTest::GetRando設置RecipeID(EP本od使ctionCate成o本y Cate成o本y) const
{
    TA本本ay<軍P本od使ctionRecipe> Cate成o本yRecipes = P本od使ctionSyste設置->GetRecipesByCate成o本y(Cate成o本y);
    if (Cate成o本yRecipes.的使設置() > 0)
    {
        int32 Rando設置Index = 軍Math::RandRan成e(0, Cate成o本yRecipes.的使設置());
        本et使本n Cate成o本yRecipes[Rando設置Index].RecipeID;
    }
    本et使本n TEXT("");
}

正oid UMin成GoRTSP本od使ctionTest::ResetTestCo使nte本s()
{
    PassedTests = 0;
    軍ailedTests = 0;
    TestRes使lts.E設置pty();
}

軍P本od使ctionSyste設置Confi成 UMin成GoRTSP本od使ctionTest::C本eateTestConfi成()
{
    軍P本od使ctionSyste設置Confi成 TestConfi成;
    TestConfi成.GlobalP本od使ctionSpeed = 2.0f;
    TestConfi成.MaxO正e本clockEfficiency = 2.0f;
    TestConfi成.MaintenancePenalty = 0.5f;
    TestConfi成.bEnableA使toDist本ib使tion = t本使e;
    TestConfi成.bEnableQ使alityCont本ol = t本使e;
    TestConfi成.MaxQ使e使eSize = 15;
    
    // 設置類別加成
    TestConfi成.Cate成o本yBon使s.Add(EP本od使ctionCate成o本y::Milita本y, 1.5f);
    TestConfi成.Cate成o本yBon使s.Add(EP本od使ctionCate成o本y::Ind使st本ial, 1.3f);
    
    本et使本n TestConfi成;
}

bool UMin成GoRTSP本od使ctionTest::Ve本ifyConfi成Application(const 軍P本od使ctionSyste設置Confi成& ExpectedConfi成)
{
    軍P本od使ctionSyste設置Confi成 C使本本entConfi成 = P本od使ctionSyste設置->GetP本od使ctionConfi成();
    
    本et使本n 軍Math::Abs(C使本本entConfi成.GlobalP本od使ctionSpeed - ExpectedConfi成.GlobalP本od使ctionSpeed) < 0.01f &&
           C使本本entConfi成.MaxO正e本clockEfficiency == ExpectedConfi成.MaxO正e本clockEfficiency &&
           C使本本entConfi成.MaintenancePenalty == ExpectedConfi成.MaintenancePenalty &&
           C使本本entConfi成.bEnableA使toDist本ib使tion == ExpectedConfi成.bEnableA使toDist本ib使tion &&
           C使本本entConfi成.bEnableQ使alityCont本ol == ExpectedConfi成.bEnableQ使alityCont本ol &&
           C使本本entConfi成.MaxQ使e使eSize == ExpectedConfi成.MaxQ使e使eSize;
}

bool UMin成GoRTSP本od使ctionTest::基本ait軍o本P本od使ctionCo設置pletion(const 軍St本in成& P本od使ctionLineID, float Ti設置eo使tSeconds)
{
    float ElapsedTi設置e = 0.0f;
    float DeltaTi設置e = 0.1f;
    
    while (ElapsedTi設置e < Ti設置eo使tSeconds)
    {
        P本od使ctionSyste設置->UpdateP本od使ctionP本o成本ess(DeltaTi設置e);
        ElapsedTi設置e += DeltaTi設置e;
        
        軍P本od使ctionLine P本od使ctionLine = P本od使ctionSyste設置->GetP本od使ctionLine(P本od使ctionLineID);
        if (P本od使ctionLine.Stat使s == EP本od使ctionStat使s::Co設置pleted)
        {
            本et使本n t本使e;
        }
        
        // 模擬延遲
        軍Platfo本設置P本ocess::Sleep(0.01f);
    }
    
    本et使本n false;
}

正oid UMin成GoRTSP本od使ctionTest::Si設置使lateTi設置eP本o成本ess(float DeltaTi設置e)
{
    P本od使ctionSyste設置->UpdateP本od使ctionP本o成本ess(DeltaTi設置e);
}

bool UMin成GoRTSP本od使ctionTest::Ve本ifyReso使本ceChan成es(const TMap<EReso使本ceType, float>& ExpectedChan成es)
{
    // TODO: 實現資源變化驗證
    // 這需要與資源系統集成來檢查實際的資源變化
    本et使本n t本使e;
}
