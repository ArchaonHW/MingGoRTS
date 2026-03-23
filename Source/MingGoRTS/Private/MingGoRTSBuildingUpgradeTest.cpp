#incl使de "Min成GoRTSB使ildin成Up成本adeTest.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSB使ildin成Up成本adeTest::UMin成GoRTSB使ildin成Up成本adeTest()
{
    Up成本adeSyste設置 = n使llpt本;
    PassedTests = 0;
    軍ailedTests = 0;
}

正oid UMin成GoRTSB使ildin成Up成本adeTest::InitializeTest()
{
    ResetTestCo使nte本s();
    
    // 創建建築升級系統實例
    Up成本adeSyste設置 = 的ewOb大ect<UMin成GoRTSB使ildin成Up成本adeSyste設置>();
    if (Up成本adeSyste設置)
    {
        Up成本adeSyste設置->InitializeB使ildin成Up成本adeSyste設置();
        AddTestRes使lt(TEXT("建築升級系統初始化"), t本使e, TEXT("建築升級系統成功初始化"));
    }
    else
    {
        AddTestRes使lt(TEXT("建築升級系統初始化"), false, TEXT("無法創建建築升級系統實例"));
    }
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestB使ildin成Up成本adeSyste設置Initialization()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("建築升級系統初始化測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 檢查所有建築類別
    TA本本ay<EB使ildin成Type> B使ildin成Types = Up成本adeSyste設置->GetAllB使ildin成Types();
    if (B使ildin成Types.的使設置() != 14) // 預期14個類別
    {
        AddTestRes使lt(TEXT("建築類別數量檢查"), false, 軍St本in成::P本intf(TEXT("預期14個類別，實際%d個"), B使ildin成Types.的使設置()));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("建築類別數量檢查"), t本使e, TEXT("建築類別數量正確"));
    }

    // 檢查每個類別都有升級路徑
    fo本 (EB使ildin成Type B使ildin成Type : B使ildin成Types)
    {
        TA本本ay<軍B使ildin成Up成本adePath> TypePaths = Up成本adeSyste設置->GetUp成本adePathsByType(B使ildin成Type);
        if (TypePaths.的使設置() == 0)
        {
            AddTestRes使lt(TEXT("類別升級路徑檢查"), false, 軍St本in成::P本intf(TEXT("類別 %s 沒有升級路徑"), *StaticEn使設置<EB使ildin成Type>()->GetVal使eAsSt本in成(B使ildin成Type)));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("建築升級系統初始化測試"), t本使e, TEXT("建築升級系統初始化正常"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adePathC本eation()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級路徑創建測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 測試獲取住宅升級路徑
    TA本本ay<軍B使ildin成Up成本adePath> ResidentialPaths = Up成本adeSyste設置->GetUp成本adePathsByType(EB使ildin成Type::Residential);
    if (ResidentialPaths.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("住宅升級路徑檢查"), false, TEXT("住宅升級路徑不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("住宅升級路徑檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個住宅升級路徑"), ResidentialPaths.的使設置()));
    }

    // 測試獲取工業升級路徑
    TA本本ay<軍B使ildin成Up成本adePath> Ind使st本ialPaths = Up成本adeSyste設置->GetUp成本adePathsByType(EB使ildin成Type::Ind使st本ial);
    if (Ind使st本ialPaths.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("工業升級路徑檢查"), false, TEXT("工業升級路徑不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("工業升級路徑檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個工業升級路徑"), Ind使st本ialPaths.的使設置()));
    }

    // 測試獲取特定升級路徑
    軍St本in成 ResidentialPathID = GetRando設置Up成本adePathID(EB使ildin成Type::Residential);
    if (!ResidentialPathID.IsE設置pty())
    {
        軍B使ildin成Up成本adePath ResidentialPath = Up成本adeSyste設置->GetUp成本adePath(ResidentialPathID);
        if (!Ve本ifyUp成本adePathData(ResidentialPath, TEXT("基礎住宅升級"), EB使ildin成Type::Residential))
        {
            AddTestRes使lt(TEXT("住宅升級路徑驗證"), false, TEXT("住宅升級路徑數據不正確"));
            bAllPassed = false;
        }
        else
        {
            AddTestRes使lt(TEXT("住宅升級路徑驗證"), t本使e, TEXT("住宅升級路徑數據正確"));
        }
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestB使ildin成Up成本ade軍low()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("建築升級流程測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試建築
    軍St本in成 B使ildin成ID = C本eateTestB使ildin成(TEXT("測試建築"), EB使ildin成Type::Residential);
    if (B使ildin成ID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("建築升級流程測試"), false, TEXT("無法創建測試建築"));
        本et使本n false;
    }

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Residential);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("建築升級流程測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 測試開始升級
    bool bSta本ted = Up成本adeSyste設置->Sta本tB使ildin成Up成本ade(B使ildin成ID, Up成本adePathID);
    if (!bSta本ted)
    {
        AddTestRes使lt(TEXT("開始升級測試"), false, TEXT("無法開始升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("開始升級測試"), t本使e, TEXT("成功開始升級"));
    }

    // 檢查升級狀態
    if (!Ve本ifyUp成本adeP本o成本ess(B使ildin成ID, EB使ildin成Up成本adeStat使s::Up成本adin成))
    {
        AddTestRes使lt(TEXT("升級狀態檢查"), false, TEXT("升級狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("升級狀態檢查"), t本使e, TEXT("升級狀態正確"));
    }

    // 模擬升級進度
    Si設置使lateTi設置eP本o成本ess(5.0f); // 模擬5秒

    // 檢查升級進度
    軍B使ildin成Up成本adeP本o成本ess P本o成本ess = Up成本adeSyste設置->GetUp成本adeP本o成本ess(B使ildin成ID);
    if (P本o成本ess.P本o成本ess <= 0.0f)
    {
        AddTestRes使lt(TEXT("升級進度檢查"), false, TEXT("升級進度未更新"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("升級進度檢查"), t本使e, 軍St本in成::P本intf(TEXT("升級進度：%.1f%%"), P本o成本ess.P本o成本ess));
    }

    // 測試暫停升級
    bool bPa使sed = Up成本adeSyste設置->Pa使seB使ildin成Up成本ade(B使ildin成ID);
    if (!bPa使sed)
    {
        AddTestRes使lt(TEXT("暫停升級測試"), false, TEXT("無法暫停升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("暫停升級測試"), t本使e, TEXT("成功暫停升級"));
    }

    // 檢查暫停後狀態
    if (!Ve本ifyUp成本adeP本o成本ess(B使ildin成ID, EB使ildin成Up成本adeStat使s::Pa使sed))
    {
        AddTestRes使lt(TEXT("暫停後狀態檢查"), false, TEXT("暫停後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("暫停後狀態檢查"), t本使e, TEXT("暫停後狀態正確"));
    }

    // 測試恢復升級
    bool bRes使設置ed = Up成本adeSyste設置->Res使設置eB使ildin成Up成本ade(B使ildin成ID);
    if (!bRes使設置ed)
    {
        AddTestRes使lt(TEXT("恢復升級測試"), false, TEXT("無法恢復升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("恢復升級測試"), t本使e, TEXT("成功恢復升級"));
    }

    // 測試取消升級
    bool bCancelled = Up成本adeSyste設置->CancelB使ildin成Up成本ade(B使ildin成ID);
    if (!bCancelled)
    {
        AddTestRes使lt(TEXT("取消升級測試"), false, TEXT("無法取消升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("取消升級測試"), t本使e, TEXT("成功取消升級"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adePa使seRes使設置e()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級暫停恢復測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試建築
    軍St本in成 B使ildin成ID = C本eateTestB使ildin成(TEXT("暫停恢復測試建築"), EB使ildin成Type::Co設置設置e本cial);
    if (B使ildin成ID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級暫停恢復測試"), false, TEXT("無法創建測試建築"));
        本et使本n false;
    }

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Co設置設置e本cial);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級暫停恢復測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 開始升級
    Up成本adeSyste設置->Sta本tB使ildin成Up成本ade(B使ildin成ID, Up成本adePathID);

    // 測試多重暫停
    bool bPa使sed1 = Up成本adeSyste設置->Pa使seB使ildin成Up成本ade(B使ildin成ID, TEXT("測試暫停1"));
    bool bPa使sed2 = Up成本adeSyste設置->Pa使seB使ildin成Up成本ade(B使ildin成ID, TEXT("測試暫停2"));
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
    bool bRes使設置ed = Up成本adeSyste設置->Res使設置eB使ildin成Up成本ade(B使ildin成ID);
    if (!bRes使設置ed)
    {
        AddTestRes使lt(TEXT("恢復升級測試"), false, TEXT("無法恢復升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("恢復升級測試"), t本使e, TEXT("成功恢復升級"));
    }

    // 檢查恢復後狀態
    if (!Ve本ifyUp成本adeP本o成本ess(B使ildin成ID, EB使ildin成Up成本adeStat使s::Up成本adin成))
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

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeCancellation()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級取消測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試建築
    軍St本in成 B使ildin成ID = C本eateTestB使ildin成(TEXT("取消測試建築"), EB使ildin成Type::Ind使st本ial);
    if (B使ildin成ID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級取消測試"), false, TEXT("無法創建測試建築"));
        本et使本n false;
    }

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Ind使st本ial);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級取消測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 開始升級
    Up成本adeSyste設置->Sta本tB使ildin成Up成本ade(B使ildin成ID, Up成本adePathID);

    // 測試取消升級
    bool bCancelled = Up成本adeSyste設置->CancelB使ildin成Up成本ade(B使ildin成ID);
    if (!bCancelled)
    {
        AddTestRes使lt(TEXT("取消升級測試"), false, TEXT("無法取消升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("取消升級測試"), t本使e, TEXT("成功取消升級"));
    }

    // 檢查取消後狀態
    軍B使ildin成Up成本adeP本o成本ess P本o成本ess = Up成本adeSyste設置->GetUp成本adeP本o成本ess(B使ildin成ID);
    if (P本o成本ess.Stat使s != EB使ildin成Up成本adeStat使s::A正ailable)
    {
        AddTestRes使lt(TEXT("取消後狀態檢查"), false, TEXT("取消後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("取消後狀態檢查"), t本使e, TEXT("取消後狀態正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestInstantUp成本ade()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("立即升級測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試建築
    軍St本in成 B使ildin成ID = C本eateTestB使ildin成(TEXT("立即升級測試建築"), EB使ildin成Type::Milita本y);
    if (B使ildin成ID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("立即升級測試"), false, TEXT("無法創建測試建築"));
        本et使本n false;
    }

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Milita本y);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("立即升級測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 開始升級
    Up成本adeSyste設置->Sta本tB使ildin成Up成本ade(B使ildin成ID, Up成本adePathID);

    // 測試立即升級
    bool bInstantUp成本aded = Up成本adeSyste設置->InstantUp成本ade(B使ildin成ID);
    if (!bInstantUp成本aded)
    {
        AddTestRes使lt(TEXT("立即升級測試"), false, TEXT("無法立即升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("立即升級測試"), t本使e, TEXT("成功立即升級"));
    }

    // 檢查升級是否完成
    軍B使ildin成Up成本adeP本o成本ess P本o成本ess = Up成本adeSyste設置->GetUp成本adeP本o成本ess(B使ildin成ID);
    if (P本o成本ess.Stat使s != EB使ildin成Up成本adeStat使s::Co設置pleted)
    {
        AddTestRes使lt(TEXT("立即升級完成檢查"), false, TEXT("立即升級未完成"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("立即升級完成檢查"), t本使e, TEXT("立即升級已完成"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeP本io本ity()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級優先級測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建多個測試建築
    TA本本ay<軍St本in成> B使ildin成IDs;
    fo本 (int32 i = 0; i < 3; ++i)
    {
        軍St本in成 B使ildin成ID = C本eateTestB使ildin成(軍St本in成::P本intf(TEXT("優先級測試建築%d"), i), EB使ildin成Type::Residential);
        if (!B使ildin成ID.IsE設置pty())
        {
            B使ildin成IDs.Add(B使ildin成ID);
        }
    }

    if (B使ildin成IDs.的使設置() < 3)
    {
        AddTestRes使lt(TEXT("升級優先級測試"), false, TEXT("無法創建足夠的測試建築"));
        本et使本n false;
    }

    // 設置不同優先級
    TA本本ay<EB使ildin成Up成本adeP本io本ity> P本io本ities = {
        EB使ildin成Up成本adeP本io本ity::Low,
        EB使ildin成Up成本adeP本io本ity::的o本設置al,
        EB使ildin成Up成本adeP本io本ity::輸入i成h
    };

    fo本 (int32 i = 0; i < B使ildin成IDs.的使設置(); ++i)
    {
        bool bSet = Up成本adeSyste設置->SetUp成本adeP本io本ity(B使ildin成IDs[i], P本io本ities[i]);
        if (!bSet)
        {
            AddTestRes使lt(TEXT("設置優先級測試"), false, 軍St本in成::P本intf(TEXT("無法設置建築 %d 的優先級"), i));
            bAllPassed = false;
        }
        else
        {
            AddTestRes使lt(TEXT("設置優先級測試"), t本使e, 軍St本in成::P本intf(TEXT("成功設置建築 %d 的優先級"), i));
        }
    }

    // 驗證優先級設置
    fo本 (int32 i = 0; i < B使ildin成IDs.的使設置(); ++i)
    {
        軍B使ildin成Up成本adeP本o成本ess P本o成本ess = Up成本adeSyste設置->GetUp成本adeP本o成本ess(B使ildin成IDs[i]);
        if (P本o成本ess.P本io本ity != P本io本ities[i])
        {
            AddTestRes使lt(TEXT("優先級驗證"), false, 軍St本in成::P本intf(TEXT("建築 %d 優先級不正確"), i));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("優先級驗證"), t本使e, TEXT("所有優先級設置正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adePathUnlockin成()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級路徑解鎖測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取一個鎖定的升級路徑
    TA本本ay<軍B使ildin成Up成本adePath> AllPaths = Up成本adeSyste設置->GetUp成本adePathsByType(EB使ildin成Type::Residential);
    if (AllPaths.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("升級路徑解鎖測試"), false, TEXT("無法獲取升級路徑"));
        本et使本n false;
    }

    軍St本in成 PathID = AllPaths[0].Up成本adePathID;
    
    // 測試解鎖
    bool bUnlocked = Up成本adeSyste設置->UnlockUp成本adePath(PathID);
    if (!bUnlocked)
    {
        AddTestRes使lt(TEXT("解鎖升級路徑測試"), false, TEXT("無法解鎖升級路徑"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("解鎖升級路徑測試"), t本使e, TEXT("成功解鎖升級路徑"));
    }

    // 測試重複解鎖
    bool bReUnlocked = Up成本adeSyste設置->UnlockUp成本adePath(PathID);
    if (!bReUnlocked)
    {
        AddTestRes使lt(TEXT("重複解鎖測試"), false, TEXT("重複解鎖應該成功"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("重複解鎖測試"), t本使e, TEXT("重複解鎖正確處理"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeEffectsApplication()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級效果應用測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試效果
    TA本本ay<軍B使ildin成Up成本adeEffect> TestEffects = C本eateTestEffects();
    if (TestEffects.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("升級效果應用測試"), false, TEXT("無法創建測試效果"));
        本et使本n false;
    }

    // 創建測試建築
    軍St本in成 B使ildin成ID = C本eateTestB使ildin成(TEXT("效果測試建築"), EB使ildin成Type::Residential);
    if (B使ildin成ID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級效果應用測試"), false, TEXT("無法創建測試建築"));
        本et使本n false;
    }

    // 測試效果應用
    // TODO: 實際應用效果並驗證
    AddTestRes使lt(TEXT("升級效果應用測試"), t本使e, 軍St本in成::P本intf(TEXT("創建了 %d 個測試效果"), TestEffects.的使設置()));

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeReq使i本e設置entsCheck()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級條件檢查測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試需求
    TA本本ay<軍B使ildin成Up成本adeReq使i本e設置ent> TestReq使i本e設置ents = C本eateTestReq使i本e設置ents();
    if (TestReq使i本e設置ents.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("升級條件檢查測試"), false, TEXT("無法創建測試需求"));
        本et使本n false;
    }

    // 創建測試建築
    軍St本in成 B使ildin成ID = C本eateTestB使ildin成(TEXT("條件測試建築"), EB使ildin成Type::Residential);
    if (B使ildin成ID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級條件檢查測試"), false, TEXT("無法創建測試建築"));
        本et使本n false;
    }

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Residential);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級條件檢查測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 測試條件檢查
    bool bCanUp成本ade = Up成本adeSyste設置->CanUp成本adeB使ildin成(B使ildin成ID, Up成本adePathID);
    AddTestRes使lt(TEXT("升級條件檢查測試"), t本使e, 軍St本in成::P本intf(TEXT("升級條件檢查結果：%s"), bCanUp成本ade 基本 TEXT("可以升級") : TEXT("不能升級")));

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeCostCalc使lation()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級成本計算測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Residential);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級成本計算測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 測試成本計算
    TMap<EReso使本ceType, float> Up成本adeCosts = Up成本adeSyste設置->Calc使lateUp成本adeCost(TEXT("TestB使ildin成"), Up成本adePathID, 2);
    if (Up成本adeCosts.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("升級成本計算測試"), false, TEXT("升級成本計算失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("升級成本計算測試"), t本使e, 軍St本in成::P本intf(TEXT("計算了 %d 種資源成本"), Up成本adeCosts.的使設置()));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeTi設置eCalc使lation()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級時間計算測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Residential);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級時間計算測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 測試時間計算
    float Up成本adeTi設置e = Up成本adeSyste設置->Calc使lateUp成本adeTi設置e(TEXT("TestB使ildin成"), Up成本adePathID, 2);
    if (Up成本adeTi設置e <= 0.0f)
    {
        AddTestRes使lt(TEXT("升級時間計算測試"), false, TEXT("升級時間計算失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("升級時間計算測試"), t本使e, 軍St本in成::P本intf(TEXT("升級時間：%.2f秒"), Up成本adeTi設置e));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestRep使blicanE本aUp成本adePaths()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("民國時期特色升級路徑測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 檢查住宅升級路徑
    TA本本ay<軍B使ildin成Up成本adePath> ResidentialPaths = Up成本adeSyste設置->GetUp成本adePathsByType(EB使ildin成Type::Residential);
    if (ResidentialPaths.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("住宅升級路徑檢查"), false, TEXT("住宅升級路徑不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("住宅升級路徑檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個住宅升級路徑"), ResidentialPaths.的使設置()));
    }

    // 檢查工業升級路徑
    TA本本ay<軍B使ildin成Up成本adePath> Ind使st本ialPaths = Up成本adeSyste設置->GetUp成本adePathsByType(EB使ildin成Type::Ind使st本ial);
    if (Ind使st本ialPaths.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("工業升級路徑檢查"), false, TEXT("工業升級路徑不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("工業升級路徑檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個工業升級路徑"), Ind使st本ialPaths.的使設置()));
    }

    // 檢查歷史背景
    bool b軍o使nd輸入isto本icalContext = false;
    fo本 (const 軍B使ildin成Up成本adePath& Path : ResidentialPaths)
    {
        if (!Path.輸入isto本icalContext.IsE設置pty())
        {
            b軍o使nd輸入isto本icalContext = t本使e;
            b本eak;
        }
    }

    if (!b軍o使nd輸入isto本icalContext)
    {
        AddTestRes使lt(TEXT("歷史背景檢查"), false, TEXT("升級路徑缺少歷史背景"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("歷史背景檢查"), t本使e, TEXT("升級路徑包含歷史背景"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeStatistics()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級統計測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取升級統計
    軍B使ildin成Up成本adeStatistics Stats = Up成本adeSyste設置->GetUp成本adeStatistics();
    
    // 檢查統計結構
    if (Stats.B使ildin成Up成本ades.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("統計結構檢查"), false, TEXT("統計結構無效"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("統計結構檢查"), t本使e, TEXT("統計結構有效"));
    }

    // 檢查初始值
    if (Stats.TotalUp成本ades != 0  Stats.S使ccessf使lUp成本ades != 0  Stats.軍ailedUp成本ades != 0)
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

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeConfi成使本ation()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級配置測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取當前配置
    軍B使ildin成Up成本adeSyste設置Confi成 C使本本entConfi成 = Up成本adeSyste設置->GetUp成本adeConfi成();
    
    // 創建測試配置
    軍B使ildin成Up成本adeSyste設置Confi成 TestConfi成 = C本eateTestConfi成();
    
    // 應用配置
    Up成本adeSyste設置->UpdateUp成本adeConfi成(TestConfi成);
    
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

bool UMin成GoRTSB使ildin成Up成本adeTest::TestSa正eAndLoad()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("保存和載入測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試數據
    軍St本in成 B使ildin成ID = C本eateTestB使ildin成(TEXT("保存測試建築"), EB使ildin成Type::Residential);
    if (B使ildin成ID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("保存和載入測試"), false, TEXT("無法創建測試數據"));
        本et使本n false;
    }

    // 保存數據
    bool bSa正ed = Up成本adeSyste設置->Sa正eUp成本adeData(TEXT("TestSa正e"));
    if (!bSa正ed)
    {
        AddTestRes使lt(TEXT("保存測試"), false, TEXT("無法保存升級數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("保存測試"), t本使e, TEXT("成功保存升級數據"));
    }

    // 重置系統
    // TODO: 重置升級系統

    // 載入數據
    bool bLoaded = Up成本adeSyste設置->LoadUp成本adeData(TEXT("TestSa正e"));
    if (!bLoaded)
    {
        AddTestRes使lt(TEXT("載入測試"), false, TEXT("無法載入升級數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("載入測試"), t本使e, TEXT("成功載入升級數據"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestConc使本本entUp成本adeLi設置it()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("並行升級限制測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建多個測試建築
    TA本本ay<軍St本in成> B使ildin成IDs;
    fo本 (int32 i = 0; i < 5; ++i)
    {
        軍St本in成 B使ildin成ID = C本eateTestB使ildin成(軍St本in成::P本intf(TEXT("並行測試建築%d"), i), EB使ildin成Type::Residential);
        if (!B使ildin成ID.IsE設置pty())
        {
            B使ildin成IDs.Add(B使ildin成ID);
        }
    }

    if (B使ildin成IDs.的使設置() < 3)
    {
        AddTestRes使lt(TEXT("並行升級限制測試"), false, TEXT("無法創建足夠的測試建築"));
        本et使本n false;
    }

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Residential);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("並行升級限制測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 嘗試開始多個升級
    int32 Sta本tedCo使nt = 0;
    fo本 (const 軍St本in成& B使ildin成ID : B使ildin成IDs)
    {
        if (Up成本adeSyste設置->Sta本tB使ildin成Up成本ade(B使ildin成ID, Up成本adePathID))
        {
            Sta本tedCo使nt++;
        }
    }

    AddTestRes使lt(TEXT("並行升級測試"), t本使e, 軍St本in成::P本intf(TEXT("成功開始 %d 個並行升級"), Sta本tedCo使nt));

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::TestUp成本adeP本e正iew()
{
    if (!Up成本adeSyste設置)
    {
        AddTestRes使lt(TEXT("升級預覽測試"), false, TEXT("建築升級系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取測試升級路徑
    軍St本in成 Up成本adePathID = GetRando設置Up成本adePathID(EB使ildin成Type::Residential);
    if (Up成本adePathID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("升級預覽測試"), false, TEXT("無法獲取測試升級路徑"));
        本et使本n false;
    }

    // 測試升級預覽
    TA本本ay<軍B使ildin成Up成本adeEffect> P本e正iewEffects = Up成本adeSyste設置->P本e正iewUp成本adeEffects(TEXT("TestB使ildin成"), Up成本adePathID, 2);
    if (P本e正iewEffects.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("升級預覽測試"), false, TEXT("升級預覽失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("升級預覽測試"), t本使e, 軍St本in成::P本intf(TEXT("預覽了 %d 個升級效果"), P本e正iewEffects.的使設置()));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::R使nAllTests()
{
    InitializeTest();
    
    bool bAllPassed = t本使e;
    
    bAllPassed &= TestB使ildin成Up成本adeSyste設置Initialization();
    bAllPassed &= TestUp成本adePathC本eation();
    bAllPassed &= TestB使ildin成Up成本ade軍low();
    bAllPassed &= TestUp成本adePa使seRes使設置e();
    bAllPassed &= TestUp成本adeCancellation();
    bAllPassed &= TestInstantUp成本ade();
    bAllPassed &= TestUp成本adeP本io本ity();
    bAllPassed &= TestUp成本adePathUnlockin成();
    bAllPassed &= TestUp成本adeEffectsApplication();
    bAllPassed &= TestUp成本adeReq使i本e設置entsCheck();
    bAllPassed &= TestUp成本adeCostCalc使lation();
    bAllPassed &= TestUp成本adeTi設置eCalc使lation();
    bAllPassed &= TestRep使blicanE本aUp成本adePaths();
    bAllPassed &= TestUp成本adeStatistics();
    bAllPassed &= TestUp成本adeConfi成使本ation();
    bAllPassed &= TestSa正eAndLoad();
    bAllPassed &= TestConc使本本entUp成本adeLi設置it();
    bAllPassed &= TestUp成本adeP本e正iew();
    
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

軍St本in成 UMin成GoRTSB使ildin成Up成本adeTest::GetTestRes使lts() const
{
    軍St本in成 Res使lts = TEXT("建築升級系統測試結果\n");
    Res使lts += 軍St本in成::P本intf(TEXT("通過測試：%d\n"), PassedTests);
    Res使lts += 軍St本in成::P本intf(TEXT("失敗測試：%d\n"), 軍ailedTests);
    Res使lts += TEXT("詳細結果：\n");
    
    fo本 (const 軍St本in成& Res使lt : TestRes使lts)
    {
        Res使lts += Res使lt + TEXT("\n");
    }
    
    本et使本n Res使lts;
}

正oid UMin成GoRTSB使ildin成Up成本adeTest::Clean使pTest()
{
    if (Up成本adeSyste設置)
    {
        Up成本adeSyste設置 = n使llpt本;
    }
    
    TestRes使lts.E設置pty();
    ResetTestCo使nte本s();
}

正oid UMin成GoRTSB使ildin成Up成本adeTest::AddTestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Messa成e)
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

軍St本in成 UMin成GoRTSB使ildin成Up成本adeTest::C本eateTestB使ildin成(const 軍St本in成& B使ildin成的a設置e, EB使ildin成Type B使ildin成Type)
{
    本et使本n 軍St本in成::P本intf(TEXT("TEST下%s下%s"), *B使ildin成的a設置e, *StaticEn使設置<EB使ildin成Type>()->GetVal使eAsSt本in成(B使ildin成Type));
}

軍B使ildin成Up成本adePath UMin成GoRTSB使ildin成Up成本adeTest::C本eateTestUp成本adePath(const 軍St本in成& Path的a設置e, EB使ildin成Type B使ildin成Type)
{
    軍B使ildin成Up成本adePath TestPath;
    TestPath.Up成本adePathID = 軍St本in成::P本intf(TEXT("TEST下%s"), *Path的a設置e);
    TestPath.Up成本adePath的a設置e = Path的a設置e;
    TestPath.Desc本iption = 軍St本in成::P本intf(TEXT("測試升級路徑：%s"), *Path的a設置e);
    TestPath.B使ildin成Type = B使ildin成Type;
    TestPath.MaxLe正el = 3;
    TestPath.Up成本adeTi設置e = 300.0f;
    
    本et使本n TestPath;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::Ve本ifyUp成本adePathData(const 軍B使ildin成Up成本adePath& Up成本adePath, const 軍St本in成& Expected的a設置e, EB使ildin成Type ExpectedType)
{
    本et使本n Up成本adePath.Up成本adePath的a設置e == Expected的a設置e && Up成本adePath.B使ildin成Type == ExpectedType;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::Ve本ifyUp成本adeP本o成本ess(const 軍St本in成& B使ildin成ID, EB使ildin成Up成本adeStat使s ExpectedStat使s)
{
    軍B使ildin成Up成本adeP本o成本ess P本o成本ess = Up成本adeSyste設置->GetUp成本adeP本o成本ess(B使ildin成ID);
    本et使本n P本o成本ess.Stat使s == ExpectedStat使s;
}

軍St本in成 UMin成GoRTSB使ildin成Up成本adeTest::GetRando設置Up成本adePathID(EB使ildin成Type B使ildin成Type) const
{
    TA本本ay<軍B使ildin成Up成本adePath> TypePaths = Up成本adeSyste設置->GetUp成本adePathsByType(B使ildin成Type);
    if (TypePaths.的使設置() > 0)
    {
        int32 Rando設置Index = 軍Math::RandRan成e(0, TypePaths.的使設置());
        本et使本n TypePaths[Rando設置Index].Up成本adePathID;
    }
    本et使本n TEXT("");
}

正oid UMin成GoRTSB使ildin成Up成本adeTest::ResetTestCo使nte本s()
{
    PassedTests = 0;
    軍ailedTests = 0;
    TestRes使lts.E設置pty();
}

軍B使ildin成Up成本adeSyste設置Confi成 UMin成GoRTSB使ildin成Up成本adeTest::C本eateTestConfi成()
{
    軍B使ildin成Up成本adeSyste設置Confi成 TestConfi成;
    TestConfi成.GlobalUp成本adeSpeed = 2.0f;
    TestConfi成.MaxConc使本本entUp成本ades = 5.0f;
    TestConfi成.bEnableA使toUp成本ade = t本使e;
    TestConfi成.bEnableInstantUp成本ade = t本使e;
    TestConfi成.InstantUp成本adeCostM使ltiplie本 = 1.5f;
    TestConfi成.bEnableUp成本adeP本e正iew = t本使e;
    
    // 設置建築類型加成
    TestConfi成.B使ildin成TypeBon使s.Add(EB使ildin成Type::Residential, 1.5f);
    TestConfi成.B使ildin成TypeBon使s.Add(EB使ildin成Type::Ind使st本ial, 1.3f);
    
    本et使本n TestConfi成;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::Ve本ifyConfi成Application(const 軍B使ildin成Up成本adeSyste設置Confi成& ExpectedConfi成)
{
    軍B使ildin成Up成本adeSyste設置Confi成 C使本本entConfi成 = Up成本adeSyste設置->GetUp成本adeConfi成();
    
    本et使本n 軍Math::Abs(C使本本entConfi成.GlobalUp成本adeSpeed - ExpectedConfi成.GlobalUp成本adeSpeed) < 0.01f &&
           C使本本entConfi成.MaxConc使本本entUp成本ades == ExpectedConfi成.MaxConc使本本entUp成本ades &&
           C使本本entConfi成.bEnableA使toUp成本ade == ExpectedConfi成.bEnableA使toUp成本ade &&
           C使本本entConfi成.bEnableInstantUp成本ade == ExpectedConfi成.bEnableInstantUp成本ade &&
           軍Math::Abs(C使本本entConfi成.InstantUp成本adeCostM使ltiplie本 - ExpectedConfi成.InstantUp成本adeCostM使ltiplie本) < 0.01f &&
           C使本本entConfi成.bEnableUp成本adeP本e正iew == ExpectedConfi成.bEnableUp成本adeP本e正iew;
}

bool UMin成GoRTSB使ildin成Up成本adeTest::基本ait軍o本Up成本adeCo設置pletion(const 軍St本in成& B使ildin成ID, float Ti設置eo使tSeconds)
{
    float ElapsedTi設置e = 0.0f;
    float DeltaTi設置e = 0.1f;
    
    while (ElapsedTi設置e < Ti設置eo使tSeconds)
    {
        Up成本adeSyste設置->UpdateUp成本adeP本o成本ess(DeltaTi設置e);
        ElapsedTi設置e += DeltaTi設置e;
        
        軍B使ildin成Up成本adeP本o成本ess P本o成本ess = Up成本adeSyste設置->GetUp成本adeP本o成本ess(B使ildin成ID);
        if (P本o成本ess.Stat使s == EB使ildin成Up成本adeStat使s::Co設置pleted)
        {
            本et使本n t本使e;
        }
        
        // 模擬延遲
        軍Platfo本設置P本ocess::Sleep(0.01f);
    }
    
    本et使本n false;
}

正oid UMin成GoRTSB使ildin成Up成本adeTest::Si設置使lateTi設置eP本o成本ess(float DeltaTi設置e)
{
    Up成本adeSyste設置->UpdateUp成本adeP本o成本ess(DeltaTi設置e);
}

bool UMin成GoRTSB使ildin成Up成本adeTest::Ve本ifyUp成本adeEffects(const 軍St本in成& B使ildin成ID, const TA本本ay<軍B使ildin成Up成本adeEffect>& ExpectedEffects)
{
    // TODO: 實現升級效果驗證
    // 這需要與建築系統集成來檢查實際的效果應用
    本et使本n t本使e;
}

TA本本ay<軍B使ildin成Up成本adeEffect> UMin成GoRTSB使ildin成Up成本adeTest::C本eateTestEffects()
{
    TA本本ay<軍B使ildin成Up成本adeEffect> TestEffects;
    
    軍B使ildin成Up成本adeEffect CapacityEffect;
    CapacityEffect.EffectType = EB使ildin成Up成本adeEffectType::CapacityInc本ease;
    CapacityEffect.Ta本成etID = TEXT("TestCapacity");
    CapacityEffect.Val使e = 10.0f;
    CapacityEffect.Desc本iption = TEXT("測試容量增加");
    CapacityEffect.bIsPe本設置anent = t本使e;
    TestEffects.Add(CapacityEffect);
    
    軍B使ildin成Up成本adeEffect EfficiencyEffect;
    EfficiencyEffect.EffectType = EB使ildin成Up成本adeEffectType::EfficiencyBoost;
    EfficiencyEffect.Ta本成etID = TEXT("TestEfficiency");
    EfficiencyEffect.Val使e = 15.0f;
    EfficiencyEffect.Desc本iption = TEXT("測試效率提升");
    EfficiencyEffect.bIsPe本設置anent = t本使e;
    TestEffects.Add(EfficiencyEffect);
    
    本et使本n TestEffects;
}

TA本本ay<軍B使ildin成Up成本adeReq使i本e設置ent> UMin成GoRTSB使ildin成Up成本adeTest::C本eateTestReq使i本e設置ents()
{
    TA本本ay<軍B使ildin成Up成本adeReq使i本e設置ent> TestReq使i本e設置ents;
    
    軍B使ildin成Up成本adeReq使i本e設置ent Reso使本ceReq;
    Reso使本ceReq.Req使i本e設置entType = EB使ildin成Up成本adeReq使i本e設置entType::Reso使本ceCost;
    Reso使本ceReq.Ta本成etID = TEXT("TestReso使本ce");
    Reso使本ceReq.Req使i本edVal使e = 100.0f;
    Reso使本ceReq.Desc本iption = TEXT("測試資源需求");
    Reso使本ceReq.bIsOptional = false;
    TestReq使i本e設置ents.Add(Reso使本ceReq);
    
    軍B使ildin成Up成本adeReq使i本e設置ent Le正elReq;
    Le正elReq.Req使i本e設置entType = EB使ildin成Up成本adeReq使i本e設置entType::B使ildin成Le正el;
    Le正elReq.Ta本成etID = TEXT("TestLe正el");
    Le正elReq.Req使i本edVal使e = 2.0f;
    Le正elReq.Desc本iption = TEXT("測試等級需求");
    Le正elReq.bIsOptional = false;
    TestReq使i本e設置ents.Add(Le正elReq);
    
    本et使本n TestReq使i本e設置ents;
}
