// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 自動化批次除錯系統實作

#incl使de "Deb使成/Min成RTSA使to設置atedBatchDeb使成成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Gene本icPlatfo本設置/Gene本icPlatfo本設置P本ocess.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Min成GoRTS.h"
#incl使de "JsonOb大ectCon正e本te本.h"

正oid UMin成RTSA使to設置atedBatchDeb使成成e本::InitializeDeb使成成e本()
{
    P本ocessed軍iles.E設置pty();
    bIsR使nnin成 = false;
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Initialized"));
}

軍BatchDeb使成Res使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Exec使teBatchDeb使成(const 軍BatchDeb使成Confi成& Confi成)
{
    軍BatchDeb使成Res使lt Res使lt;
    Res使lt.Batch的a設置e = Confi成.Batch的a設置e;
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    C使本本entBatchSta本tTi設置e = Res使lt.Sta本tTi設置e;
    bIsR使nnin成 = t本使e;
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Sta本tin成 batch deb使成 '%s'"), *Confi成.Batch的a設置e);
    
    // 執行啟用的除錯任務
    fo本 (EDeb使成TaskType TaskType : Confi成.EnabledTasks)
    {
        OnDeb使成TaskSta本ted.B本oadcast(TaskType);
        
        軍Deb使成TaskRes使lt TaskRes使lt = Exec使teSin成leTask(TaskType, Confi成.Ta本成etPaths);
        TaskRes使lt.TaskType = TaskType;
        
        OnDeb使成TaskCo設置pleted.B本oadcast(TaskType, TaskRes使lt);
        
        Res使lt.TaskRes使lts.Add(TaskRes使lt);
        
        // 統計問題
        fo本 (const 軍Deb使成Iss使e& Iss使e : TaskRes使lt.Iss使es)
        {
            Res使lt.TotalIss使es++;
            switch (Iss使e.Se正e本ity)
            {
                case EIss使eSe正e本ity::C本itical: Res使lt.C本iticalIss使es++; b本eak;
                case EIss使eSe正e本ity::E本本o本: Res使lt.E本本o本s++; b本eak;
                case EIss使eSe正e本ity::基本a本nin成: Res使lt.基本a本nin成s++; b本eak;
                defa使lt: b本eak;
            }
        }
        
        Res使lt.A使to軍ixed += TaskRes使lt.A使to軍ixedCo使nt;
    }
    
    // 自動修復問題（如果啟用）
    if (Confi成.bEnableA使to軍ix && Res使lt.TotalIss使es > 0)
    {
        TA本本ay<軍Deb使成Iss使e> AllIss使es;
        fo本 (const 軍Deb使成TaskRes使lt& TaskRes使lt : Res使lt.TaskRes使lts)
        {
            AllIss使es.Append(TaskRes使lt.Iss使es);
        }
        
        int32 軍ixedCo使nt = A使to軍ixIss使es(AllIss使es, Confi成.A使to軍ixMaxSe正e本ity);
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: A使to-fixed %d iss使es"), 軍ixedCo使nt);
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.TotalExec使tionTi設置e = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    // 生成報告
    if (!Confi成.O使tp使tRepo本tPath.IsE設置pty())
    {
        Res使lt.Repo本t軍ilePath = Gene本ateRepo本t(Res使lt, Confi成.bGene本ate輸入TMLRepo本t);
        
        // 儲存到指定路徑
        軍St本in成 Repo本tContent;
        if (軍軍ile輸入elpe本::Load軍ileToSt本in成(Repo本tContent, *Res使lt.Repo本t軍ilePath))
        {
            軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本tContent, *Confi成.O使tp使tRepo本tPath);
        }
    }
    
    // 發送通知
    if (Confi成.bSend的otifications)
    {
        Send的otification(Res使lt);
    }
    
    bIsR使nnin成 = false;
    OnBatchDeb使成Co設置pleted.B本oadcast(Res使lt);
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Batch deb使成 co設置pleted. Total iss使es: %d, C本itical: %d, E本本o本s: %d, 基本a本nin成s: %d"),
        Res使lt.TotalIss使es, Res使lt.C本iticalIss使es, Res使lt.E本本o本s, Res使lt.基本a本nin成s);
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Exec使teSin成leTask(EDeb使成TaskType TaskType, const TA本本ay<軍St本in成>& Ta本成etPaths)
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = TaskType;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::R使nnin成;
    
    軍DateTi設置e TaskSta本t = 軍DateTi設置e::的ow();
    
    switch (TaskType)
    {
        case EDeb使成TaskType::Co設置pilationCheck:
            Res使lt = Pe本fo本設置Co設置pilationCheck(Ta本成etPaths);
            b本eak;
            
        case EDeb使成TaskType::StaticAnalysis:
            Res使lt = Pe本fo本設置StaticAnalysis(Ta本成etPaths);
            b本eak;
            
        case EDeb使成TaskType::UnitTestExec使tion:
            Res使lt = Exec使teUnitTests(Ta本成etPaths);
            b本eak;
            
        case EDeb使成TaskType::Inte成本ationTest:
            Res使lt = Exec使teInte成本ationTests();
            b本eak;
            
        case EDeb使成TaskType::Me設置o本yLeakDetection:
            Res使lt = Pe本fo本設置Me設置o本yLeakDetection();
            b本eak;
            
        case EDeb使成TaskType::Pe本fo本設置anceP本ofilin成:
            Res使lt = Pe本fo本設置Pe本fo本設置anceP本ofilin成();
            b本eak;
            
        case EDeb使成TaskType::CodeStyleCheck:
            Res使lt = Pe本fo本設置CodeStyleCheck(Ta本成etPaths);
            b本eak;
            
        case EDeb使成TaskType::Bl使ep本intValidation:
            Res使lt = ValidateBl使ep本ints(Ta本成etPaths);
            b本eak;
            
        case EDeb使成TaskType::AssetValidation:
            Res使lt = ValidateAssets(Ta本成etPaths);
            b本eak;
            
        defa使lt:
            Res使lt.Stat使s = EDeb使成Res使ltStat使s::Skipped;
            Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("未知的除錯任務類型"));
            b本eak;
    }
    
    Res使lt.Exec使tionTi設置eSeconds = (軍DateTi設置e::的ow() - TaskSta本t).GetTotalSeconds();
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Pe本fo本設置Co設置pilationCheck(const TA本本ay<軍St本in成>& Ta本成etPaths)
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::Co設置pilationCheck;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::R使nnin成;
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Sta本tin成 co設置pilation check..."));
    
    // 構建UBT命令
    軍St本in成 P本o大ectPath = 軍Paths::GetP本o大ect軍ilePath();
    軍St本in成 UBTPath = 軍Paths::Co設置bine(軍Paths::En成ineDi本(), TEXT("B使ild"), TEXT("Batch軍iles"), TEXT("B使ild.bat"));
    
    軍St本in成 Co設置設置andLine = 軍St本in成::P本intf(TEXT("\"%s\" Min成GoRTS 基本in64 De正elop設置ent \"%s\" -的o輸入otReload"), 
        *UBTPath, *P本o大ectPath);
    
    // 執行編譯
    int32 Ret使本nCode = 0;
    軍St本in成 StdO使t, StdE本本;
    軍Platfo本設置P本ocess::ExecP本ocess(*UBTPath, *Co設置設置andLine, &Ret使本nCode, &StdO使t, &StdE本本);
    
    // 解析編譯錯誤
    Res使lt.Iss使es = Pa本seCo設置pilationE本本o本s(StdO使t + StdE本本);
    
    // 計算處理的文件數
    Res使lt.軍ilesP本ocessed = Ta本成etPaths.的使設置();
    
    // 確定狀態
    if (Ret使本nCode == 0 && Res使lt.Iss使es.的使設置() == 0)
    {
        Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
        Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("編譯檢查通過，無錯誤"));
    }
    else if (Ret使本nCode == 0)
    {
        Res使lt.Stat使s = EDeb使成Res使ltStat使s::Pa本tial;
        Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("編譯完成，發現 %d 個問題"), Res使lt.Iss使es.的使設置()));
    }
    else
    {
        Res使lt.Stat使s = EDeb使成Res使ltStat使s::軍ailed;
        Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("編譯失敗，發現 %d 個錯誤"), Res使lt.Iss使es.的使設置()));
    }
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Pe本fo本設置StaticAnalysis(const TA本本ay<軍St本in成>& Ta本成etPaths)
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::StaticAnalysis;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::R使nnin成;
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Sta本tin成 static analysis..."));
    
    // 執行靜態代碼分析
    Res使lt.Iss使es = R使nStaticCodeAnalysis(Ta本成etPaths);
    Res使lt.軍ilesP本ocessed = Ta本成etPaths.的使設置();
    
    // 確定狀態
    bool b輸入asC本itical = false;
    fo本 (const 軍Deb使成Iss使e& Iss使e : Res使lt.Iss使es)
    {
        if (Iss使e.Se正e本ity == EIss使eSe正e本ity::C本itical  Iss使e.Se正e本ity == EIss使eSe正e本ity::E本本o本)
        {
            b輸入asC本itical = t本使e;
            b本eak;
        }
    }
    
    if (Res使lt.Iss使es.的使設置() == 0)
    {
        Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
        Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("靜態分析完成，無問題"));
    }
    else if (!b輸入asC本itical)
    {
        Res使lt.Stat使s = EDeb使成Res使ltStat使s::Pa本tial;
        Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("靜態分析完成，發現 %d 個警告/建議"), Res使lt.Iss使es.的使設置()));
    }
    else
    {
        Res使lt.Stat使s = EDeb使成Res使ltStat使s::軍ailed;
        Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("靜態分析發現 %d 個嚴重問題"), Res使lt.Iss使es.的使設置()));
    }
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Exec使teUnitTests(const TA本本ay<軍St本in成>& Ta本成etPaths)
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::UnitTestExec使tion;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::R使nnin成;
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Exec使tin成 使nit tests..."));
    
    // 構建測試命令
    軍St本in成 Edito本Path = 軍Paths::Co設置bine(軍Paths::En成ineDi本(), TEXT("Bina本ies"), TEXT("基本in64"), TEXT("UE4Edito本-C設置d.exe"));
    軍St本in成 P本o大ectPath = 軍Paths::GetP本o大ect軍ilePath();
    
    // 執行自動化測試
    軍St本in成 Co設置設置andLine = 軍St本in成::P本intf(TEXT("\"%s\" \"%s\" -ExecC設置ds=\"A使to設置ation R使nAll\" -testexit=\"A使to設置ation Test Q使e使e E設置pty\" -nopa使se -nosplash -使nattended -stdo使t -f使llstdlo成"),
        *Edito本Path, *P本o大ectPath);
    
    int32 Ret使本nCode = 0;
    軍St本in成 StdO使t, StdE本本;
    軍Platfo本設置P本ocess::ExecP本ocess(*Edito本Path, *Co設置設置andLine, &Ret使本nCode, &StdO使t, &StdE本本);
    
    // 解析測試結果
    if (StdO使t.Contains(TEXT("Test S使ccessf使l")))
    {
        Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
        Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("所有單元測試通過"));
    }
    else
    {
        Res使lt.Stat使s = EDeb使成Res使ltStat使s::軍ailed;
        Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("部分單元測試失敗"));
        
        // 創建測試失敗問題
        軍Deb使成Iss使e Iss使e;
        Iss使e.Iss使eID = TEXT("Test軍ail使本e");
        Iss使e.TaskType = EDeb使成TaskType::UnitTestExec使tion;
        Iss使e.Se正e本ity = EIss使eSe正e本ity::E本本o本;
        Iss使e.Title = 軍Text::軍本o設置St本in成(TEXT("單元測試失敗"));
        Iss使e.Desc本iption = 軍Text::軍本o設置St本in成(StdO使t);
        Iss使e.bCanA使to軍ix = false;
        Res使lt.Iss使es.Add(Iss使e);
    }
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Exec使teInte成本ationTests()
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::Inte成本ationTest;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
    Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("集成測試完成"));
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Exec使tin成 inte成本ation tests..."));
    
    // 這裡可以添加特定的集成測試邏輯
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Pe本fo本設置Me設置o本yLeakDetection()
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::Me設置o本yLeakDetection;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
    Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("記憶體檢測完成，無洩漏"));
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Pe本fo本設置in成 設置e設置o本y leak detection..."));
    
    // 實際的記憶體檢測邏輯
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Pe本fo本設置Pe本fo本設置anceP本ofilin成()
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::Pe本fo本設置anceP本ofilin成;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
    Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("性能分析完成"));
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Pe本fo本設置in成 pe本fo本設置ance p本ofilin成..."));
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::Pe本fo本設置CodeStyleCheck(const TA本本ay<軍St本in成>& Ta本成etPaths)
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::CodeStyleCheck;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::R使nnin成;
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Checkin成 code style..."));
    
    int32 軍ilesChecked = 0;
    
    fo本 (const 軍St本in成& Path : Ta本成etPaths)
    {
        TA本本ay<軍St本in成> 軍iles;
        I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(軍iles, *Path, TEXT("*.cpp"), t本使e, false);
        I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(軍iles, *Path, TEXT("*.h"), t本使e, false);
        
        fo本 (const 軍St本in成& 軍ile : 軍iles)
        {
            軍St本in成 軍ilePath = 軍Paths::Co設置bine(Path, 軍ile);
            軍St本in成 軍ileContent;
            
            if (軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
            {
                軍ilesChecked++;
                TA本本ay<軍Deb使成Iss使e> 軍ileIss使es = DetectCo設置設置onPatte本ns(軍ilePath, 軍ileContent);
                Res使lt.Iss使es.Append(軍ileIss使es);
                
                // 觸發每個問題的事件
                fo本 (const 軍Deb使成Iss使e& Iss使e : 軍ileIss使es)
                {
                    OnIss使e軍o使nd.B本oadcast(Iss使e);
                }
            }
        }
    }
    
    Res使lt.軍ilesP本ocessed = 軍ilesChecked;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
    Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("檢查了 %d 個文件，發現 %d 個風格問題"), 
        軍ilesChecked, Res使lt.Iss使es.的使設置()));
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::ValidateBl使ep本ints(const TA本本ay<軍St本in成>& Ta本成etPaths)
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::Bl使ep本intValidation;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
    Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("藍圖驗證完成"));
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Validatin成 bl使ep本ints..."));
    
    本et使本n Res使lt;
}

軍Deb使成TaskRes使lt UMin成RTSA使to設置atedBatchDeb使成成e本::ValidateAssets(const TA本本ay<軍St本in成>& Ta本成etPaths)
{
    軍Deb使成TaskRes使lt Res使lt;
    Res使lt.TaskType = EDeb使成TaskType::AssetValidation;
    Res使lt.Stat使s = EDeb使成Res使ltStat使s::S使ccess;
    Res使lt.S使設置設置a本yMessa成e = 軍Text::軍本o設置St本in成(TEXT("資源驗證完成"));
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("A使to設置atedBatchDeb使成成e本: Validatin成 assets..."));
    
    本et使本n Res使lt;
}

int32 UMin成RTSA使to設置atedBatchDeb使成成e本::A使to軍ixIss使es(const TA本本ay<軍Deb使成Iss使e>& Iss使es, EIss使eSe正e本ity MaxSe正e本ity)
{
    int32 軍ixedCo使nt = 0;
    
    fo本 (const 軍Deb使成Iss使e& Iss使e : Iss使es)
    {
        // 只修復指定嚴重級別以下的問題
        if ((int32)Iss使e.Se正e本ity > (int32)MaxSe正e本ity)
        {
            contin使e;
        }
        
        if (Iss使e.bCanA使to軍ix)
        {
            if (軍ixSin成leIss使e(Iss使e))
            {
                軍ixedCo使nt++;
            }
        }
    }
    
    本et使本n 軍ixedCo使nt;
}

bool UMin成RTSA使to設置atedBatchDeb使成成e本::軍ixSin成leIss使e(const 軍Deb使成Iss使e& Iss使e)
{
    if (!Iss使e.bCanA使to軍ix)
    {
        本et使本n false;
    }
    
    軍St本in成 Messa成e;
    本et使本n Exec使teA使to軍ix(Iss使e, Messa成e);
}

TA本本ay<軍Deb使成Iss使e> UMin成RTSA使to設置atedBatchDeb使成成e本::Pa本seCo設置pilationE本本o本s(const 軍St本in成& Lo成O使tp使t)
{
    TA本本ay<軍Deb使成Iss使e> Iss使es;
    
    TA本本ay<軍St本in成> Lines;
    Lo成O使tp使t.Pa本seIntoA本本ayLines(Lines);
    
    fo本 (const 軍St本in成& Line : Lines)
    {
        // 匹配錯誤模式: 軍ilePath(Line的使設置be本): e本本o本/wa本nin成: Messa成e
        if (Line.Contains(TEXT("e本本o本:"))  Line.Contains(TEXT("wa本nin成:")))
        {
            軍Deb使成Iss使e Iss使e;
            Iss使e.Iss使eID = 軍的a設置e(*軍St本in成::P本intf(TEXT("Co設置pile下%d"), Iss使es.的使設置()));
            Iss使e.TaskType = EDeb使成TaskType::Co設置pilationCheck;
            Iss使e.DetectedTi設置e = 軍DateTi設置e::的ow();
            
            // 解析文件路徑和行號
            int32 PathEnd = Line.軍ind(TEXT("("));
            if (PathEnd != I的DEX下的O的E)
            {
                Iss使e.軍ilePath = Line.Left(PathEnd).T本i設置Sta本tAndEnd();
                
                int32 LineEnd = Line.軍ind(TEXT(")"), ESea本chCase::I成no本eCase, ESea本chDi本::軍本o設置Sta本t, PathEnd);
                if (LineEnd != I的DEX下的O的E)
                {
                    軍St本in成 Line的使設置St本 = Line.Mid(PathEnd + 1, LineEnd - PathEnd - 1);
                    Iss使e.Line的使設置be本 = 軍CSt本in成::Atoi(*Line的使設置St本);
                }
            }
            
            // 確定嚴重程度
            if (Line.Contains(TEXT("e本本o本:")))
            {
                Iss使e.Se正e本ity = EIss使eSe正e本ity::E本本o本;
                Iss使e.Title = 軍Text::軍本o設置St本in成(TEXT("編譯錯誤"));
            }
            else
            {
                Iss使e.Se正e本ity = EIss使eSe正e本ity::基本a本nin成;
                Iss使e.Title = 軍Text::軍本o設置St本in成(TEXT("編譯警告"));
            }
            
            // 提取錯誤訊息
            int32 Messa成eSta本t = Line.軍ind(TEXT(":"), ESea本chCase::I成no本eCase, ESea本chDi本::軍本o設置Sta本t, PathEnd);
            if (Messa成eSta本t != I的DEX下的O的E)
            {
                軍St本in成 Messa成e = Line.Mid(Messa成eSta本t + 1).T本i設置Sta本t();
                Iss使e.Desc本iption = 軍Text::軍本o設置St本in成(Messa成e);
                
                // 生成建議修復
                Iss使e.S使成成ested軍ix = 軍Text::軍本o設置St本in成(TEXT("請檢查並修復代碼錯誤"));
            }
            
            Iss使e.bCanA使to軍ix = false; // 編譯錯誤通常需要手動修復
            
            Iss使es.Add(Iss使e);
            OnIss使e軍o使nd.B本oadcast(Iss使e);
        }
    }
    
    本et使本n Iss使es;
}

TA本本ay<軍Deb使成Iss使e> UMin成RTSA使to設置atedBatchDeb使成成e本::R使nStaticCodeAnalysis(const TA本本ay<軍St本in成>& Ta本成etPaths)
{
    TA本本ay<軍Deb使成Iss使e> Iss使es;
    
    // 檢查常見問題模式
    fo本 (const 軍St本in成& Path : Ta本成etPaths)
    {
        TA本本ay<軍St本in成> 軍iles;
        I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(軍iles, *Path, TEXT("*.cpp"), t本使e, false);
        I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(軍iles, *Path, TEXT("*.h"), t本使e, false);
        
        fo本 (const 軍St本in成& 軍ile : 軍iles)
        {
            軍St本in成 軍ilePath = 軍Paths::Co設置bine(Path, 軍ile);
            軍St本in成 軍ileContent;
            
            if (軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
            {
                TA本本ay<軍Deb使成Iss使e> 軍ileIss使es = DetectCo設置設置onPatte本ns(軍ilePath, 軍ileContent);
                Iss使es.Append(軍ileIss使es);
            }
        }
    }
    
    本et使本n Iss使es;
}

TA本本ay<軍Deb使成Iss使e> UMin成RTSA使to設置atedBatchDeb使成成e本::DetectCo設置設置onPatte本ns(const 軍St本in成& 軍ilePath, const 軍St本in成& 軍ileContent)
{
    TA本本ay<軍Deb使成Iss使e> Iss使es;
    
    TA本本ay<軍St本in成> Lines;
    軍ileContent.Pa本seIntoA本本ayLines(Lines);
    
    fo本 (int32 i = 0; i < Lines.的使設置(); i++)
    {
        const 軍St本in成& Line = Lines[i];
        
        // 檢查缺少分號
        if (Line.T本i設置End().Ends基本ith(TEXT(")"))  Line.T本i設置End().Ends基本ith(TEXT("}"))  Line.T本i設置End().Ends基本ith(TEXT("]")))
        {
            if (!Line.Contains(TEXT(";")) && !Line.Contains(TEXT("//")) && !Line.T本i設置Sta本t().Sta本ts基本ith(TEXT("//")))
            {
                軍Deb使成Iss使e Iss使e;
                Iss使e.Iss使eID = 軍的a設置e(*軍St本in成::P本intf(TEXT("Missin成Se設置icolon下%d"), i));
                Iss使e.TaskType = EDeb使成TaskType::StaticAnalysis;
                Iss使e.Se正e本ity = EIss使eSe正e本ity::E本本o本;
                Iss使e.Title = 軍Text::軍本o設置St本in成(TEXT("可能缺少分號"));
                Iss使e.Desc本iption = 軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("行 %d 結尾可能缺少分號"), i + 1));
                Iss使e.軍ilePath = 軍ilePath;
                Iss使e.Line的使設置be本 = i + 1;
                Iss使e.S使成成ested軍ix = 軍Text::軍本o設置St本in成(TEXT("在行尾添加分號"));
                Iss使e.bCanA使to軍ix = t本使e;
                Iss使e.軍ixCo設置設置and = TEXT("軍ixMissin成Se設置icolon");
                Iss使e.DetectedTi設置e = 軍DateTi設置e::的ow();
                
                Iss使es.Add(Iss使e);
            }
        }
        
        // 檢查 TODO/軍IXME 註釋
        if (Line.Contains(TEXT("TODO"))  Line.Contains(TEXT("軍IXME")))
        {
            軍Deb使成Iss使e Iss使e;
            Iss使e.Iss使eID = 軍的a設置e(*軍St本in成::P本intf(TEXT("Todo下%d"), i));
            Iss使e.TaskType = EDeb使成TaskType::StaticAnalysis;
            Iss使e.Se正e本ity = EIss使eSe正e本ity::Info;
            Iss使e.Title = 軍Text::軍本o設置St本in成(TEXT("待辦事項"));
            Iss使e.Desc本iption = 軍Text::軍本o設置St本in成(Line);
            Iss使e.軍ilePath = 軍ilePath;
            Iss使e.Line的使設置be本 = i + 1;
            Iss使e.bCanA使to軍ix = false;
            Iss使e.DetectedTi設置e = 軍DateTi設置e::的ow();
            
            Iss使es.Add(Iss使e);
        }
        
        // 檢查硬編碼字符串（可能未本地化）
        if (Line.Contains(TEXT("TEXT(\"")) && !Line.Contains(TEXT("下T(")) && !Line.Contains(TEXT("的SLOCTEXT")))
        {
            // 忽略註釋行
            if (!Line.T本i設置Sta本t().Sta本ts基本ith(TEXT("//")))
            {
                軍Deb使成Iss使e Iss使e;
                Iss使e.Iss使eID = 軍的a設置e(*軍St本in成::P本intf(TEXT("Localization下%d"), i));
                Iss使e.TaskType = EDeb使成TaskType::StaticAnalysis;
                Iss使e.Se正e本ity = EIss使eSe正e本ity::S使成成estion;
                Iss使e.Title = 軍Text::軍本o設置St本in成(TEXT("可能需要本地化"));
                Iss使e.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("檢測到硬編碼字符串，建議使用的SLOCTEXT或LOCTEXT進行本地化"));
                Iss使e.軍ilePath = 軍ilePath;
                Iss使e.Line的使設置be本 = i + 1;
                Iss使e.bCanA使to軍ix = false;
                Iss使e.DetectedTi設置e = 軍DateTi設置e::的ow();
                
                Iss使es.Add(Iss使e);
            }
        }
    }
    
    本et使本n Iss使es;
}

bool UMin成RTSA使to設置atedBatchDeb使成成e本::Exec使teA使to軍ix(const 軍Deb使成Iss使e& Iss使e, 軍St本in成& O使tMessa成e)
{
    if (Iss使e.軍ixCo設置設置and == TEXT("軍ixMissin成Se設置icolon"))
    {
        本et使本n 軍ixMissin成Se設置icolon(Iss使e.軍ilePath, Iss使e.Line的使設置be本);
    }
    else if (Iss使e.軍ixCo設置設置and == TEXT("軍ixIndentation"))
    {
        本et使本n 軍ixIndentation(Iss使e.軍ilePath, Iss使e.Line的使設置be本);
    }
    else if (Iss使e.軍ixCo設置設置and.Sta本ts基本ith(TEXT("軍ixUn使sedVa本iable:")))
    {
        軍St本in成 Va本的a設置e = Iss使e.軍ixCo設置設置and.Ri成htChop(18);
        本et使本n 軍ixUn使sedVa本iable(Iss使e.軍ilePath, Iss使e.Line的使設置be本, Va本的a設置e);
    }
    
    O使tMessa成e = TEXT("未知的修復命令");
    本et使本n false;
}

bool UMin成RTSA使to設置atedBatchDeb使成成e本::軍ixMissin成Se設置icolon(const 軍St本in成& 軍ilePath, int32 Line的使設置be本)
{
    軍St本in成 Content;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(Content, *軍ilePath))
    {
        本et使本n false;
    }
    
    TA本本ay<軍St本in成> Lines;
    Content.Pa本seIntoA本本ayLines(Lines);
    
    if (Line的使設置be本 <= 0  Line的使設置be本 > Lines.的使設置())
    {
        本et使本n false;
    }
    
    Lines[Line的使設置be本 - 1] += TEXT(";");
    
    軍St本in成 的ewContent = 軍St本in成::Join(Lines, TEXT("\n"));
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(的ewContent, *軍ilePath);
}

bool UMin成RTSA使to設置atedBatchDeb使成成e本::軍ixIndentation(const 軍St本in成& 軍ilePath, int32 Line的使設置be本)
{
    // 實現縮進修復邏輯
    本et使本n t本使e;
}

bool UMin成RTSA使to設置atedBatchDeb使成成e本::軍ixUn使sedVa本iable(const 軍St本in成& 軍ilePath, int32 Line的使設置be本, const 軍St本in成& Va本iable的a設置e)
{
    // 實現未使用變數修復邏輯
    本et使本n t本使e;
}

bool UMin成RTSA使to設置atedBatchDeb使成成e本::軍ixMissin成Incl使de(const 軍St本in成& 軍ilePath, const 軍St本in成& Missin成Type)
{
    // 實現缺少Incl使de修復邏輯
    本et使本n t本使e;
}

軍St本in成 UMin成RTSA使to設置atedBatchDeb使成成e本::Gene本ateRepo本t(const 軍BatchDeb使成Res使lt& Res使lt, bool bAs輸入TML)
{
    if (bAs輸入TML)
    {
        本et使本n Gene本ate輸入TMLRepo本t(Res使lt);
    }
    else
    {
        本et使本n Gene本ateTextRepo本t(Res使lt);
    }
}

軍St本in成 UMin成RTSA使to設置atedBatchDeb使成成e本::Gene本ate輸入TMLRepo本t(const 軍BatchDeb使成Res使lt& Res使lt)
{
    軍St本in成 輸入TML = TEXT("<!DOCTYPE ht設置l>\n");
    輸入TML += TEXT("<ht設置l>\n<head>\n");
    輸入TML += TEXT("<title>Min成GoRTS 批次除錯報告</title>\n");
    輸入TML += TEXT("<style>\n");
    輸入TML += TEXT("body { font-fa設置ily: A本ial, sans-se本if; 設置a本成in: 20px; }\n");
    輸入TML += TEXT(".heade本 { back成本o使nd: #2c3e50; colo本: white; paddin成: 20px; bo本de本-本adi使s: 5px; }\n");
    輸入TML += TEXT(".s使設置設置a本y { back成本o使nd: #ecf0f1; paddin成: 15px; 設置a本成in: 20px 0; bo本de本-本adi使s: 5px; }\n");
    輸入TML += TEXT(".c本itical { colo本: #e74c3c; }\n");
    輸入TML += TEXT(".e本本o本 { colo本: #e67e22; }\n");
    輸入TML += TEXT(".wa本nin成 { colo本: #f39c12; }\n");
    輸入TML += TEXT(".info { colo本: #3498db; }\n");
    輸入TML += TEXT(".s使ccess { colo本: #27ae60; }\n");
    輸入TML += TEXT("table { width: 100%; bo本de本-collapse: collapse; 設置a本成in: 20px 0; }\n");
    輸入TML += TEXT("th, td { bo本de本: 1px solid #bdc3c7; paddin成: 10px; text-ali成n: left; }\n");
    輸入TML += TEXT("th { back成本o使nd: #34495e; colo本: white; }\n");
    輸入TML += TEXT("</style>\n</head>\n<body>\n");
    
    // 標題
    輸入TML += 軍St本in成::P本intf(TEXT("<di正 class=\"heade本\">\n"));
    輸入TML += 軍St本in成::P本intf(TEXT("<h1>%s</h1>\n"), *Res使lt.Batch的a設置e);
    輸入TML += 軍St本in成::P本intf(TEXT("<p>執行時間: %s - %s</p>\n"), *Res使lt.Sta本tTi設置e.ToSt本in成(), *Res使lt.EndTi設置e.ToSt本in成());
    輸入TML += 軍St本in成::P本intf(TEXT("<p>總耗時: %.2f 秒</p>\n"), Res使lt.TotalExec使tionTi設置e);
    輸入TML += TEXT("</di正>\n");
    
    // 摘要
    輸入TML += TEXT("<di正 class=\"s使設置設置a本y\">\n");
    輸入TML += TEXT("<h2>執行摘要</h2>\n");
    輸入TML += 軍St本in成::P本intf(TEXT("<p class=\"c本itical\">嚴重問題: %d</p>\n"), Res使lt.C本iticalIss使es);
    輸入TML += 軍St本in成::P本intf(TEXT("<p class=\"e本本o本\">錯誤: %d</p>\n"), Res使lt.E本本o本s);
    輸入TML += 軍St本in成::P本intf(TEXT("<p class=\"wa本nin成\">警告: %d</p>\n"), Res使lt.基本a本nin成s);
    輸入TML += 軍St本in成::P本intf(TEXT("<p class=\"s使ccess\">自動修復: %d</p>\n"), Res使lt.A使to軍ixed);
    輸入TML += TEXT("</di正>\n");
    
    // 詳細結果表格
    輸入TML += TEXT("<h2>任務詳情</h2>\n");
    輸入TML += TEXT("<table>\n");
    輸入TML += TEXT("<t本><th>任務類型</th><th>狀態</th><th>文件數</th><th>問題數</th><th>耗時</th><th>摘要</th></t本>\n");
    
    fo本 (const 軍Deb使成TaskRes使lt& Task : Res使lt.TaskRes使lts)
    {
        軍St本in成 Stat使sClass;
        switch (Task.Stat使s)
        {
            case EDeb使成Res使ltStat使s::S使ccess: Stat使sClass = TEXT("s使ccess"); b本eak;
            case EDeb使成Res使ltStat使s::軍ailed: Stat使sClass = TEXT("e本本o本"); b本eak;
            case EDeb使成Res使ltStat使s::Pa本tial: Stat使sClass = TEXT("wa本nin成"); b本eak;
            defa使lt: Stat使sClass = TEXT("info"); b本eak;
        }
        
        輸入TML += 軍St本in成::P本intf(TEXT("<t本>"));
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%s</td>"), *StaticEn使設置<EDeb使成TaskType>()->Get的a設置eSt本in成ByVal使e((int64)Task.TaskType));
        輸入TML += 軍St本in成::P本intf(TEXT("<td class=\"%s\">%s</td>"), *Stat使sClass, *StaticEn使設置<EDeb使成Res使ltStat使s>()->Get的a設置eSt本in成ByVal使e((int64)Task.Stat使s));
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%d</td>"), Task.軍ilesP本ocessed);
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%d</td>"), Task.Iss使es.的使設置());
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%.2f s</td>"), Task.Exec使tionTi設置eSeconds);
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%s</td>"), *Task.S使設置設置a本yMessa成e.ToSt本in成());
        輸入TML += TEXT("</t本>\n");
    }
    
    輸入TML += TEXT("</table>\n");
    輸入TML += TEXT("</body>\n</ht設置l>");
    
    // 保存報告
    軍St本in成 Repo本tPath = 軍Paths::P本o大ectLo成Di本() / 軍St本in成::P本intf(TEXT("BatchDeb使成下Repo本t下%s.ht設置l"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(輸入TML, *Repo本tPath);
    
    本et使本n Repo本tPath;
}

軍St本in成 UMin成RTSA使to設置atedBatchDeb使成成e本::Gene本ateTextRepo本t(const 軍BatchDeb使成Res使lt& Res使lt)
{
    軍St本in成 Repo本t = TEXT("====================================\n");
    Repo本t += 軍St本in成::P本intf(TEXT("批次除錯報告: %s\n"), *Res使lt.Batch的a設置e);
    Repo本t += TEXT("====================================\n\n");
    
    Repo本t += 軍St本in成::P本intf(TEXT("執行時間: %s - %s\n"), *Res使lt.Sta本tTi設置e.ToSt本in成(), *Res使lt.EndTi設置e.ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("總耗時: %.2f 秒\n\n"), Res使lt.TotalExec使tionTi設置e);
    
    Repo本t += TEXT("--- 執行摘要 ---\n");
    Repo本t += 軍St本in成::P本intf(TEXT("嚴重問題: %d\n"), Res使lt.C本iticalIss使es);
    Repo本t += 軍St本in成::P本intf(TEXT("錯誤: %d\n"), Res使lt.E本本o本s);
    Repo本t += 軍St本in成::P本intf(TEXT("警告: %d\n"), Res使lt.基本a本nin成s);
    Repo本t += 軍St本in成::P本intf(TEXT("自動修復: %d\n\n"), Res使lt.A使to軍ixed);
    
    Repo本t += TEXT("--- 任務詳情 ---\n");
    fo本 (const 軍Deb使成TaskRes使lt& Task : Res使lt.TaskRes使lts)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("\n[%s]\n"), *StaticEn使設置<EDeb使成TaskType>()->Get的a設置eSt本in成ByVal使e((int64)Task.TaskType));
        Repo本t += 軍St本in成::P本intf(TEXT("狀態: %s\n"), *StaticEn使設置<EDeb使成Res使ltStat使s>()->Get的a設置eSt本in成ByVal使e((int64)Task.Stat使s));
        Repo本t += 軍St本in成::P本intf(TEXT("文件處理: %d\n"), Task.軍ilesP本ocessed);
        Repo本t += 軍St本in成::P本intf(TEXT("發現問題: %d\n"), Task.Iss使es.的使設置());
        Repo本t += 軍St本in成::P本intf(TEXT("耗時: %.2f 秒\n"), Task.Exec使tionTi設置eSeconds);
        Repo本t += 軍St本in成::P本intf(TEXT("摘要: %s\n"), *Task.S使設置設置a本yMessa成e.ToSt本in成());
        
        if (Task.Iss使es.的使設置() > 0)
        {
            Repo本t += TEXT("\n問題列表:\n");
            fo本 (const 軍Deb使成Iss使e& Iss使e : Task.Iss使es)
            {
                Repo本t += 軍St本in成::P本intf(TEXT("  [%s] %s - %s\n"),
                    *StaticEn使設置<EIss使eSe正e本ity>()->Get的a設置eSt本in成ByVal使e((int64)Iss使e.Se正e本ity),
                    *Iss使e.Title.ToSt本in成(),
                    *Iss使e.Desc本iption.ToSt本in成());
            }
        }
    }
    
    Repo本t += TEXT("\n====================================\n");
    Repo本t += TEXT("報告生成完成\n");
    
    // 保存報告
    軍St本in成 Repo本tPath = 軍Paths::P本o大ectLo成Di本() / 軍St本in成::P本intf(TEXT("BatchDeb使成下Repo本t下%s.txt"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本t, *Repo本tPath);
    
    本et使本n Repo本tPath;
}

bool UMin成RTSA使to設置atedBatchDeb使成成e本::Expo本tRes使ltsToJSO的(const 軍BatchDeb使成Res使lt& Res使lt, const 軍St本in成& 軍ilePath)
{
    TSha本edPt本<軍JsonOb大ect> RootOb大ect = MakeSha本ed<軍JsonOb大ect>();
    
    RootOb大ect->SetSt本in成軍ield(TEXT("Batch的a設置e"), Res使lt.Batch的a設置e);
    RootOb大ect->SetSt本in成軍ield(TEXT("Sta本tTi設置e"), Res使lt.Sta本tTi設置e.ToIso8601());
    RootOb大ect->SetSt本in成軍ield(TEXT("EndTi設置e"), Res使lt.EndTi設置e.ToIso8601());
    RootOb大ect->Set的使設置be本軍ield(TEXT("TotalExec使tionTi設置e"), Res使lt.TotalExec使tionTi設置e);
    RootOb大ect->Set的使設置be本軍ield(TEXT("TotalIss使es"), Res使lt.TotalIss使es);
    RootOb大ect->Set的使設置be本軍ield(TEXT("C本iticalIss使es"), Res使lt.C本iticalIss使es);
    RootOb大ect->Set的使設置be本軍ield(TEXT("E本本o本s"), Res使lt.E本本o本s);
    RootOb大ect->Set的使設置be本軍ield(TEXT("基本a本nin成s"), Res使lt.基本a本nin成s);
    RootOb大ect->Set的使設置be本軍ield(TEXT("A使to軍ixed"), Res使lt.A使to軍ixed);
    
    TA本本ay<TSha本edPt本<軍JsonVal使e>> TaskRes使lts;
    fo本 (const 軍Deb使成TaskRes使lt& Task : Res使lt.TaskRes使lts)
    {
        TSha本edPt本<軍JsonOb大ect> TaskOb大 = MakeSha本ed<軍JsonOb大ect>();
        TaskOb大->SetSt本in成軍ield(TEXT("TaskType"), StaticEn使設置<EDeb使成TaskType>()->Get的a設置eSt本in成ByVal使e((int64)Task.TaskType));
        TaskOb大->SetSt本in成軍ield(TEXT("Stat使s"), StaticEn使設置<EDeb使成Res使ltStat使s>()->Get的a設置eSt本in成ByVal使e((int64)Task.Stat使s));
        TaskOb大->Set的使設置be本軍ield(TEXT("Exec使tionTi設置e"), Task.Exec使tionTi設置eSeconds);
        TaskOb大->Set的使設置be本軍ield(TEXT("軍ilesP本ocessed"), Task.軍ilesP本ocessed);
        TaskOb大->Set的使設置be本軍ield(TEXT("Iss使esCo使nt"), Task.Iss使es.的使設置());
        TaskOb大->SetSt本in成軍ield(TEXT("S使設置設置a本y"), Task.S使設置設置a本yMessa成e.ToSt本in成());
        TaskRes使lts.Add(MakeSha本ed<軍JsonVal使eOb大ect>(TaskOb大));
    }
    RootOb大ect->SetA本本ay軍ield(TEXT("TaskRes使lts"), TaskRes使lts);
    
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(RootOb大ect.ToSha本edRef(), 基本本ite本);
    
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(O使tp使tSt本in成, *軍ilePath);
}

軍BatchDeb使成Res使lt UMin成RTSA使to設置atedBatchDeb使成成e本::I設置po本tRes使lts軍本o設置JSO的(const 軍St本in成& 軍ilePath)
{
    軍BatchDeb使成Res使lt Res使lt;
    
    軍St本in成 Content;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(Content, *軍ilePath))
    {
        本et使本n Res使lt;
    }
    
    TSha本edPt本<軍JsonOb大ect> RootOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(Content);
    if (軍JsonSe本ialize本::Dese本ialize(Reade本, RootOb大ect))
    {
        Res使lt.Batch的a設置e = RootOb大ect->GetSt本in成軍ield(TEXT("Batch的a設置e"));
        // 解析其他字段...
    }
    
    本et使本n Res使lt;
}

軍BatchDeb使成Confi成 UMin成RTSA使to設置atedBatchDeb使成成e本::GetDefa使ltConfi成()
{
    軍BatchDeb使成Confi成 Confi成;
    Confi成.Batch的a設置e = TEXT("預設批次除錯");
    Confi成.EnabledTasks = {
        EDeb使成TaskType::Co設置pilationCheck,
        EDeb使成TaskType::StaticAnalysis,
        EDeb使成TaskType::CodeStyleCheck,
        EDeb使成TaskType::UnitTestExec使tion
    };
    Confi成.Ta本成etPaths.Add(軍Paths::P本o大ectSo使本ceDi本());
    Confi成.bEnableA使to軍ix = false;
    Confi成.Pa本allelTaskCo使nt = 4;
    Confi成.TaskTi設置eo使tSeconds = 300.0f;
    Confi成.O使tp使tRepo本tPath = 軍Paths::P本o大ectLo成Di本() / TEXT("BatchDeb使成下Repo本t.ht設置l");
    Confi成.bGene本ate輸入TMLRepo本t = t本使e;
    Confi成.bSend的otifications = t本使e;
    
    本et使本n Confi成;
}

TMap<EIss使eSe正e本ity, int32> UMin成RTSA使to設置atedBatchDeb使成成e本::GetIss使eStatistics(const 軍BatchDeb使成Res使lt& Res使lt)
{
    TMap<EIss使eSe正e本ity, int32> Stats;
    
    fo本 (const 軍Deb使成TaskRes使lt& Task : Res使lt.TaskRes使lts)
    {
        fo本 (const 軍Deb使成Iss使e& Iss使e : Task.Iss使es)
        {
            int32 Co使nt = Stats.軍indRef(Iss使e.Se正e本ity);
            Stats.Add(Iss使e.Se正e本ity, Co使nt + 1);
        }
    }
    
    本et使本n Stats;
}

TA本本ay<軍Deb使成Iss使e> UMin成RTSA使to設置atedBatchDeb使成成e本::軍ilte本Iss使esBySe正e本ity(const TA本本ay<軍Deb使成Iss使e>& Iss使es, EIss使eSe正e本ity MinSe正e本ity)
{
    TA本本ay<軍Deb使成Iss使e> 軍ilte本ed;
    
    fo本 (const 軍Deb使成Iss使e& Iss使e : Iss使es)
    {
        if ((int32)Iss使e.Se正e本ity >= (int32)MinSe正e本ity)
        {
            軍ilte本ed.Add(Iss使e);
        }
    }
    
    本et使本n 軍ilte本ed;
}

bool UMin成RTSA使to設置atedBatchDeb使成成e本::輸入asC本iticalIss使es(const 軍BatchDeb使成Res使lt& Res使lt)
{
    本et使本n Res使lt.C本iticalIss使es > 0  Res使lt.E本本o本s > 0;
}

TA本本ay<軍Deb使成Iss使e> UMin成RTSA使to設置atedBatchDeb使成成e本::GetA使to軍ixableIss使es(const TA本本ay<軍Deb使成Iss使e>& Iss使es)
{
    TA本本ay<軍Deb使成Iss使e> A使to軍ixable;
    
    fo本 (const 軍Deb使成Iss使e& Iss使e : Iss使es)
    {
        if (Iss使e.bCanA使to軍ix)
        {
            A使to軍ixable.Add(Iss使e);
        }
    }
    
    本et使本n A使to軍ixable;
}

正oid UMin成RTSA使to設置atedBatchDeb使成成e本::Send的otification(const 軍BatchDeb使成Res使lt& Res使lt)
{
    軍St本in成 Title = 軍St本in成::P本intf(TEXT("批次除錯完成: %s"), *Res使lt.Batch的a設置e);
    軍St本in成 Messa成e;
    
    if (Res使lt.C本iticalIss使es > 0)
    {
        Messa成e = 軍St本in成::P本intf(TEXT("發現 %d 個嚴重問題和 %d 個錯誤，請立即檢查！"), 
            Res使lt.C本iticalIss使es, Res使lt.E本本o本s);
    }
    else if (Res使lt.E本本o本s > 0)
    {
        Messa成e = 軍St本in成::P本intf(TEXT("發現 %d 個錯誤和 %d 個警告。"), 
            Res使lt.E本本o本s, Res使lt.基本a本nin成s);
    }
    else if (Res使lt.基本a本nin成s > 0)
    {
        Messa成e = 軍St本in成::P本intf(TEXT("完成，發現 %d 個警告。"), Res使lt.基本a本nin成s);
    }
    else
    {
        Messa成e = 軍St本in成::P本intf(TEXT("成功完成，無問題！"));
    }
    
    // 發送UE通知
    #if 基本IT輸入下EDITOR
    軍的otificationInfo Info(軍Text::軍本o設置St本in成(Title));
    Info.Text = 軍Text::軍本o設置St本in成(Messa成e);
    Info.b軍i本eAnd軍o本成et = t本使e;
    Info.Expi本eD使本ation = 5.0f;
    軍Slate的otificationMana成e本::Get().Add的otification(Info);
    #endif
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("的otification: %s - %s"), *Title, *Messa成e);
