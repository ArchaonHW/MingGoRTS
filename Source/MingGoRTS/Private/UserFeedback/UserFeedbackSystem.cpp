// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 用戶反饋系統 - 反饋收集和管理實現

#incl使de "Use本軍eedback/Use本軍eedbackSyste設置.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/G使id.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UUse本軍eedbackSyste設置::UUse本軍eedbackSyste設置()
    : bInitialized(false)
    , 軍eedbackData軍ilePath(TEXT(""))
    , bEnableE設置ail的otifications(t本使e)
    , bEnableA使toCate成o本ization(t本使e)
    , bEnableD使plicateDetection(t本使e)
{
}

正oid UUse本軍eedbackSyste設置::Initialize軍eedbackSyste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍eedback syste設置 al本eady initialized"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Use本 軍eedback Syste設置..."));

    // 設置反饋數據文件路徑
    軍eedbackData軍ilePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("軍eedback") / TEXT("軍eedbackData.大son");

    // 確保目錄存在
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    軍St本in成 Di本ecto本yPath = 軍Paths::GetPath(軍eedbackData軍ilePath);
    if (!Platfo本設置軍ile.Di本ecto本yExists(*Di本ecto本yPath))
    {
        Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*Di本ecto本yPath);
    }

    // 加載現有反饋數據
    Load軍eedbackData();

    // 更新統計數據
    UpdateStatistics();

    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Use本 軍eedback Syste設置 initialized s使ccessf使lly"));
}

軍St本in成 UUse本軍eedbackSyste設置::S使b設置it軍eedback(const 軍軍eedbackData& 軍eedbackData)
{
    if (!bInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍eedback syste設置 not initialized"));
        本et使本n 軍St本in成();
    }

    // 驗證反饋數據
    if (!Validate軍eedbackData(軍eedbackData))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid feedback data"));
        本et使本n 軍St本in成();
    }

    // 檢查重複反饋
    if (bEnableD使plicateDetection)
    {
        TA本本ay<軍St本in成> D使plicates = CheckD使plicate軍eedback(軍eedbackData);
        if (D使plicates.的使設置() > 0)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍o使nd d使plicate feedback: %s"), *D使plicates[0]);
            本et使本n D使plicates[0];
        }
    }

    // 創建反饋副本
    軍軍eedbackData 的ew軍eedback = 軍eedbackData;
    的ew軍eedback.軍eedbackID = Gene本ate軍eedbackID();
    的ew軍eedback.S使b設置itTi設置e = 軍DateTi設置e::的ow();
    的ew軍eedback.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    的ew軍eedback.Stat使s = E軍eedbackStat使s::的ew;
    的ew軍eedback.bIsRead = false;

    // 自動分類和標記
    if (bEnableA使toCate成o本ization)
    {
        A使toCate成o本ize軍eedback();
        Analyze軍eedbackContent(的ew軍eedback);
        A使toTa成軍eedback(的ew軍eedback);
    }

    // 保存反饋數據
    if (Sa正e軍eedbackData(的ew軍eedback))
    {
        All軍eedback.Add(的ew軍eedback);
        UpdateStatistics();

        // 觸發事件
        On軍eedbackS使b設置itted.B本oadcast(的ew軍eedback.軍eedbackID);
        On的ew軍eedback.B本oadcast(的ew軍eedback);

        // 發送通知
        Send的otification(的ew軍eedback.軍eedbackID, TEXT("的ew feedback s使b設置itted"));

        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍eedback s使b設置itted s使ccessf使lly: %s"), *的ew軍eedback.軍eedbackID);
        本et使本n 的ew軍eedback.軍eedbackID;
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to sa正e feedback data"));
        本et使本n 軍St本in成();
    }
}

TA本本ay<軍軍eedbackData> UUse本軍eedbackSyste設置::Get軍eedbackList(E軍eedbackType 軍ilte本Type, E軍eedbackStat使s 軍ilte本Stat使s)
{
    TA本本ay<軍軍eedbackData> 軍ilte本edList;

    fo本 (const 軍軍eedbackData& 軍eedback : All軍eedback)
    {
        bool bTypeMatch = (軍ilte本Type == E軍eedbackType::Othe本)  (軍eedback.軍eedbackType == 軍ilte本Type);
        bool bStat使sMatch = (軍ilte本Stat使s == E軍eedbackStat使s::的ew)  (軍eedback.Stat使s == 軍ilte本Stat使s);

        if (bTypeMatch && bStat使sMatch)
        {
            軍ilte本edList.Add(軍eedback);
        }
    }

    // 按提交時間排序
    軍ilte本edList.So本t([](const 軍軍eedbackData& A, const 軍軍eedbackData& B)
    {
        本et使本n A.S使b設置itTi設置e > B.S使b設置itTi設置e;
    });

    本et使本n 軍ilte本edList;
}

軍軍eedbackData UUse本軍eedbackSyste設置::Get軍eedbackDetails(const 軍St本in成& 軍eedbackID)
{
    fo本 (const 軍軍eedbackData& 軍eedback : All軍eedback)
    {
        if (軍eedback.軍eedbackID == 軍eedbackID)
        {
            本et使本n 軍eedback;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍eedback not fo使nd: %s"), *軍eedbackID);
    本et使本n 軍軍eedbackData();
}

bool UUse本軍eedbackSyste設置::Update軍eedbackStat使s(const 軍St本in成& 軍eedbackID, E軍eedbackStat使s 的ewStat使s)
{
    fo本 (軍軍eedbackData& 軍eedback : All軍eedback)
    {
        if (軍eedback.軍eedbackID == 軍eedbackID)
        {
            軍eedback.Stat使s = 的ewStat使s;
            軍eedback.LastUpdateTi設置e = 軍DateTi設置e::的ow();

            // 保存更新
            if (Sa正e軍eedbackData(軍eedback))
            {
                UpdateStatistics();
                On軍eedbackStat使sUpdated.B本oadcast(軍eedbackID, 的ewStat使s);
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("軍eedback stat使s 使pdated: %s -> %d"), *軍eedbackID, (int32)的ewStat使s);
                本et使本n t本使e;
            }
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍eedback not fo使nd fo本 stat使s 使pdate: %s"), *軍eedbackID);
    本et使本n false;
}

bool UUse本軍eedbackSyste設置::Assi成n軍eedback(const 軍St本in成& 軍eedbackID, const 軍St本in成& Assi成nedTo)
{
    fo本 (軍軍eedbackData& 軍eedback : All軍eedback)
    {
        if (軍eedback.軍eedbackID == 軍eedbackID)
        {
            軍eedback.Assi成nedTo = Assi成nedTo;
            軍eedback.LastUpdateTi設置e = 軍DateTi設置e::的ow();

            // 保存更新
            if (Sa正e軍eedbackData(軍eedback))
            {
                On軍eedbackAssi成ned.B本oadcast(軍eedbackID, Assi成nedTo);
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("軍eedback assi成ned: %s -> %s"), *軍eedbackID, *Assi成nedTo);
                本et使本n t本使e;
            }
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍eedback not fo使nd fo本 assi成n設置ent: %s"), *軍eedbackID);
    本et使本n false;
}

bool UUse本軍eedbackSyste設置::AddCo設置設置ent(const 軍St本in成& 軍eedbackID, const 軍St本in成& Co設置設置ent)
{
    // 這裡可以擴展為支持多條評論
    // 目前簡化為添加到描述中
    fo本 (軍軍eedbackData& 軍eedback : All軍eedback)
    {
        if (軍eedback.軍eedbackID == 軍eedbackID)
        {
            軍eedback.Desc本iption += 軍St本in成::P本intf(TEXT("\n\nCo設置設置ent [%s]:\n%s"), 
                *軍DateTi設置e::的ow().ToSt本in成(), *Co設置設置ent);
            軍eedback.LastUpdateTi設置e = 軍DateTi設置e::的ow();

            // 保存更新
            if (Sa正e軍eedbackData(軍eedback))
            {
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置設置ent added to feedback: %s"), *軍eedbackID);
                本et使本n t本使e;
            }
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍eedback not fo使nd fo本 co設置設置ent: %s"), *軍eedbackID);
    本et使本n false;
}

軍軍eedbackStatistics UUse本軍eedbackSyste設置::GetStatistics()
{
    本et使本n Statistics;
}

軍St本in成 UUse本軍eedbackSyste設置::Expo本t軍eedbackData()
{
    TSha本edPt本<軍JsonOb大ect> RootOb大ect = MakeSha本eable(new 軍JsonOb大ect);
    
    // 創建反饋數組
    TA本本ay<TSha本edPt本<軍JsonVal使e>> 軍eedbackA本本ay;
    
    fo本 (const 軍軍eedbackData& 軍eedback : All軍eedback)
    {
        TSha本edPt本<軍JsonOb大ect> 軍eedbackOb大ect = MakeSha本eable(new 軍JsonOb大ect);
        
        軍eedbackOb大ect->SetSt本in成軍ield(TEXT("軍eedbackID"), 軍eedback.軍eedbackID);
        軍eedbackOb大ect->Set的使設置be本軍ield(TEXT("軍eedbackType"), (int32)軍eedback.軍eedbackType);
        軍eedbackOb大ect->Set的使設置be本軍ield(TEXT("P本io本ity"), (int32)軍eedback.P本io本ity);
        軍eedbackOb大ect->Set的使設置be本軍ield(TEXT("Stat使s"), (int32)軍eedback.Stat使s);
        軍eedbackOb大ect->SetSt本in成軍ield(TEXT("Title"), 軍eedback.Title);
        軍eedbackOb大ect->SetSt本in成軍ield(TEXT("Desc本iption"), 軍eedback.Desc本iption);
        軍eedbackOb大ect->SetSt本in成軍ield(TEXT("Use本Info"), 軍eedback.Use本Info);
        軍eedbackOb大ect->SetSt本in成軍ield(TEXT("Syste設置Info"), 軍eedback.Syste設置Info);
        軍eedbackOb大ect->SetSt本in成軍ield(TEXT("Ga設置eVe本sion"), 軍eedback.Ga設置eVe本sion);
        軍eedbackOb大ect->SetSt本in成軍ield(TEXT("S使b設置itTi設置e"), 軍eedback.S使b設置itTi設置e.ToIso8601());
        軍eedbackOb大ect->SetSt本in成軍ield(TEXT("Assi成nedTo"), 軍eedback.Assi成nedTo);
        軍eedbackOb大ect->Set的使設置be本軍ield(TEXT("Ratin成"), 軍eedback.Ratin成);
        軍eedbackOb大ect->SetBool軍ield(TEXT("IsRead"), 軍eedback.bIsRead);
        
        // 添加標籤數組
        TA本本ay<TSha本edPt本<軍JsonVal使e>> Ta成sA本本ay;
        fo本 (const 軍St本in成& Ta成 : 軍eedback.Ta成s)
        {
            Ta成sA本本ay.Add(MakeSha本eable(new 軍JsonVal使eSt本in成(Ta成)));
        }
        軍eedbackOb大ect->SetA本本ay軍ield(TEXT("Ta成s"), Ta成sA本本ay);
        
        軍eedbackA本本ay.Add(MakeSha本eable(new 軍JsonVal使eOb大ect(軍eedbackOb大ect)));
    }
    
    RootOb大ect->SetA本本ay軍ield(TEXT("軍eedback"), 軍eedbackA本本ay);
    
    // 添加統計信息
    TSha本edPt本<軍JsonOb大ect> StatsOb大ect = MakeSha本eable(new 軍JsonOb大ect);
    StatsOb大ect->Set的使設置be本軍ield(TEXT("Total軍eedback"), Statistics.Total軍eedback);
    StatsOb大ect->Set的使設置be本軍ield(TEXT("A正e本a成eResponseTi設置e"), Statistics.A正e本a成eResponseTi設置e);
    StatsOb大ect->Set的使設置be本軍ield(TEXT("Resol使tionRate"), Statistics.Resol使tionRate);
    StatsOb大ect->Set的使設置be本軍ield(TEXT("Use本Satisfaction"), Statistics.Use本Satisfaction);
    RootOb大ect->SetOb大ect軍ield(TEXT("Statistics"), StatsOb大ect);
    
    // 序列化為字符串
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(RootOb大ect.ToSha本edRef(), 基本本ite本);
    
    本et使本n O使tp使tSt本in成;
}

正oid UUse本軍eedbackSyste設置::A使toCate成o本ize軍eedback()
{
    // 自動分類邏輯
    fo本 (軍軍eedbackData& 軍eedback : All軍eedback)
    {
        if (軍eedback.Stat使s == E軍eedbackStat使s::的ew)
        {
            // 根據關鍵詞自動分類
            if (軍eedback.Desc本iption.Contains(TEXT("c本ash"))  
                軍eedback.Desc本iption.Contains(TEXT("e本本o本"))  
                軍eedback.Desc本iption.Contains(TEXT("b使成")))
            {
                軍eedback.軍eedbackType = E軍eedbackType::B使成;
                軍eedback.P本io本ity = E軍eedbackP本io本ity::輸入i成h;
            }
            else if (軍eedback.Desc本iption.Contains(TEXT("slow"))  
                     軍eedback.Desc本iption.Contains(TEXT("la成"))  
                     軍eedback.Desc本iption.Contains(TEXT("pe本fo本設置ance")))
            {
                軍eedback.軍eedbackType = E軍eedbackType::Pe本fo本設置ance;
                軍eedback.P本io本ity = E軍eedbackP本io本ity::Medi使設置;
            }
            else if (軍eedback.Desc本iption.Contains(TEXT("feat使本e"))  
                     軍eedback.Desc本iption.Contains(TEXT("add"))  
                     軍eedback.Desc本iption.Contains(TEXT("本eq使est")))
            {
                軍eedback.軍eedbackType = E軍eedbackType::軍eat使本e;
                軍eedback.P本io本ity = E軍eedbackP本io本ity::Low;
            }
            else if (軍eedback.Desc本iption.Contains(TEXT("balance"))  
                     軍eedback.Desc本iption.Contains(TEXT("o正e本powe本ed"))  
                     軍eedback.Desc本iption.Contains(TEXT("weak")))
            {
                軍eedback.軍eedbackType = E軍eedbackType::Balance;
                軍eedback.P本io本ity = E軍eedbackP本io本ity::Medi使設置;
            }
            else if (軍eedback.Desc本iption.Contains(TEXT("UI"))  
                     軍eedback.Desc本iption.Contains(TEXT("inte本face"))  
                     軍eedback.Desc本iption.Contains(TEXT("設置en使")))
            {
                軍eedback.軍eedbackType = E軍eedbackType::UI;
                軍eedback.P本io本ity = E軍eedbackP本io本ity::Medi使設置;
            }
            else if (軍eedback.Desc本iption.Contains(TEXT("so使nd"))  
                     軍eedback.Desc本iption.Contains(TEXT("a使dio"))  
                     軍eedback.Desc本iption.Contains(TEXT("設置使sic")))
            {
                軍eedback.軍eedbackType = E軍eedbackType::A使dio;
                軍eedback.P本io本ity = E軍eedbackP本io本ity::Low;
            }
            else if (軍eedback.Desc本iption.Contains(TEXT("t本anslation"))  
                     軍eedback.Desc本iption.Contains(TEXT("lan成使a成e"))  
                     軍eedback.Desc本iption.Contains(TEXT("localization")))
            {
                軍eedback.軍eedbackType = E軍eedbackType::Localization;
                軍eedback.P本io本ity = E軍eedbackP本io本ity::Medi使設置;
            }
        }
    }
}

正oid UUse本軍eedbackSyste設置::Send的otification(const 軍St本in成& 軍eedbackID, const 軍St本in成& Messa成e)
{
    if (!bEnableE設置ail的otifications)
    {
        本et使本n;
    }

    // 獲取反饋詳情
    軍軍eedbackData 軍eedback = Get軍eedbackDetails(軍eedbackID);
    if (軍eedback.軍eedbackID.IsE設置pty())
    {
        本et使本n;
    }

    // 構建郵件內容
    軍St本in成 S使b大ect = 軍St本in成::P本intf(TEXT("Min成GoRTS 軍eedback: %s"), *軍eedback.Title);
    軍St本in成 Body = 軍St本in成::P本intf(TEXT("軍eedback ID: %s\n\n"), *軍eedback.軍eedbackID);
    Body += 軍St本in成::P本intf(TEXT("Type: %s\n"), *UEn使設置::GetVal使eAsSt本in成(軍eedback.軍eedbackType));
    Body += 軍St本in成::P本intf(TEXT("P本io本ity: %s\n"), *UEn使設置::GetVal使eAsSt本in成(軍eedback.P本io本ity));
    Body += 軍St本in成::P本intf(TEXT("Stat使s: %s\n"), *UEn使設置::GetVal使eAsSt本in成(軍eedback.Stat使s));
    Body += 軍St本in成::P本intf(TEXT("Use本: %s\n\n"), *軍eedback.Use本Info);
    Body += 軍St本in成::P本intf(TEXT("Desc本iption:\n%s\n\n"), *軍eedback.Desc本iption);
    Body += 軍St本in成::P本intf(TEXT("Syste設置 Info:\n%s\n"), *軍eedback.Syste設置Info);
    Body += 軍St本in成::P本intf(TEXT("Ga設置e Ve本sion: %s\n"), *軍eedback.Ga設置eVe本sion);
    Body += 軍St本in成::P本intf(TEXT("S使b設置it Ti設置e: %s\n"), *軍eedback.S使b設置itTi設置e.ToSt本in成());
    Body += 軍St本in成::P本intf(TEXT("\n%s"), *Messa成e);

    // 發送郵件通知（這裡需要實際的郵件發送實現）
    SendE設置ail的otification(TEXT("s使ppo本t@設置in成成o本ts.co設置"), S使b大ect, Body);
}

TA本本ay<軍St本in成> UUse本軍eedbackSyste設置::CheckD使plicate軍eedback(const 軍軍eedbackData& 軍eedbackData)
{
    TA本本ay<軍St本in成> D使plicates;
    
    fo本 (const 軍軍eedbackData& Existin成軍eedback : All軍eedback)
    {
        // 檢查標題相似度
        if (Existin成軍eedback.Title.Eq使als(軍eedbackData.Title, ESea本chCase::I成no本eCase))
        {
            D使plicates.Add(Existin成軍eedback.軍eedbackID);
        }
        // 檢查描述相似度（簡化版本）
        else if (Existin成軍eedback.Desc本iption.Eq使als(軍eedbackData.Desc本iption, ESea本chCase::I成no本eCase))
        {
            D使plicates.Add(Existin成軍eedback.軍eedbackID);
        }
    }
    
    本et使本n D使plicates;
}

軍St本in成 UUse本軍eedbackSyste設置::Gene本ate軍eedbackRepo本t()
{
    軍St本in成 Repo本t = TEXT("=== Min成GoRTS Use本 軍eedback Repo本t ===\n\n");
    
    // 基本統計
    Repo本t += 軍St本in成::P本intf(TEXT("Total 軍eedback: %d\n"), Statistics.Total軍eedback);
    Repo本t += 軍St本in成::P本intf(TEXT("Resol使tion Rate: %.1f%%\n"), Statistics.Resol使tionRate * 100);
    Repo本t += 軍St本in成::P本intf(TEXT("Use本 Satisfaction: %.1f/5.0\n"), Statistics.Use本Satisfaction);
    Repo本t += 軍St本in成::P本intf(TEXT("A正e本a成e Response Ti設置e: %.1f ho使本s\n"), Statistics.A正e本a成eResponseTi設置e);
    Repo本t += TEXT("\n");
    
    // 按類型統計
    Repo本t += TEXT("軍eedback by Type:\n");
    fo本 (const a使to& TypeStat : Statistics.軍eedbackByType)
    {
        軍St本in成 Type的a設置e = UEn使設置::GetVal使eAsSt本in成(TypeStat.Key);
        Repo本t += 軍St本in成::P本intf(TEXT("  %s: %d\n"), *Type的a設置e, TypeStat.Val使e);
    }
    Repo本t += TEXT("\n");
    
    // 按優先級統計
    Repo本t += TEXT("軍eedback by P本io本ity:\n");
    fo本 (const a使to& P本io本ityStat : Statistics.軍eedbackByP本io本ity)
    {
        軍St本in成 P本io本ity的a設置e = UEn使設置::GetVal使eAsSt本in成(P本io本ityStat.Key);
        Repo本t += 軍St本in成::P本intf(TEXT("  %s: %d\n"), *P本io本ity的a設置e, P本io本ityStat.Val使e);
    }
    Repo本t += TEXT("\n");
    
    // 按狀態統計
    Repo本t += TEXT("軍eedback by Stat使s:\n");
    fo本 (const a使to& Stat使sStat : Statistics.軍eedbackByStat使s)
    {
        軍St本in成 Stat使s的a設置e = UEn使設置::GetVal使eAsSt本in成(Stat使sStat.Key);
        Repo本t += 軍St本in成::P本intf(TEXT("  %s: %d\n"), *Stat使s的a設置e, Stat使sStat.Val使e);
    }
    Repo本t += TEXT("\n");
    
    // 最近反饋
    Repo本t += TEXT("Recent 軍eedback:\n");
    int32 Co使nt = 0;
    fo本 (const 軍軍eedbackData& 軍eedback : All軍eedback)
    {
        if (Co使nt >= 10) b本eak;
        
        Repo本t += 軍St本in成::P本intf(TEXT("  [%s] %s - %s\n"), 
            *軍eedback.S使b設置itTi設置e.ToSt本in成(), *軍eedback.軍eedbackID, *軍eedback.Title);
        Co使nt++;
    }
    
    Repo本t += TEXT("\n=== End of Repo本t ===\n");
    
    本et使本n Repo本t;
}

軍St本in成 UUse本軍eedbackSyste設置::Gene本ate軍eedbackID()
{
    本et使本n 軍G使id::的ewG使id().ToSt本in成();
}

bool UUse本軍eedbackSyste設置::Validate軍eedbackData(const 軍軍eedbackData& 軍eedbackData)
{
    if (軍eedbackData.Title.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍eedback title is e設置pty"));
        本et使本n false;
    }
    
    if (軍eedbackData.Desc本iption.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍eedback desc本iption is e設置pty"));
        本et使本n false;
    }
    
    if (軍eedbackData.Use本Info.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍eedback 使se本 info is e設置pty"));
    }
    
    本et使本n t本使e;
}

bool UUse本軍eedbackSyste設置::Sa正e軍eedbackData(const 軍軍eedbackData& 軍eedbackData)
{
    // 保存到JSO的文件
    軍St本in成 JsonSt本in成 = Expo本t軍eedbackData();
    
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JsonSt本in成, *軍eedbackData軍ilePath);
}

正oid UUse本軍eedbackSyste設置::Load軍eedbackData()
{
    if (!軍軍ile輸入elpe本::軍ileExists(*軍eedbackData軍ilePath))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍eedback data file does not exist, sta本tin成 with e設置pty data"));
        本et使本n;
    }
    
    軍St本in成 JsonSt本in成;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonSt本in成, *軍eedbackData軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to load feedback data file"));
        本et使本n;
    }
    
    // 解析JSO的
    TSha本edPt本<軍JsonOb大ect> RootOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JsonSt本in成);
    
    if (!軍JsonSe本ialize本::Dese本ialize(RootOb大ect, Reade本))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to pa本se feedback data JSO的"));
        本et使本n;
    }
    
    // 加載反饋數據
    const TA本本ay<TSha本edPt本<軍JsonVal使e>>* 軍eedbackA本本ay;
    if (RootOb大ect->T本yGetA本本ay軍ield(TEXT("軍eedback"), 軍eedbackA本本ay))
    {
        All軍eedback.E設置pty();
        
        fo本 (const TSha本edPt本<軍JsonVal使e>& 軍eedbackVal使e : *軍eedbackA本本ay)
        {
            TSha本edPt本<軍JsonOb大ect> 軍eedbackOb大ect = 軍eedbackVal使e->AsOb大ect();
            if (軍eedbackOb大ect.IsValid())
            {
                軍軍eedbackData 軍eedback;
                
                軍eedback.軍eedbackID = 軍eedbackOb大ect->GetSt本in成軍ield(TEXT("軍eedbackID"));
                軍eedback.軍eedbackType = (E軍eedbackType)軍eedbackOb大ect->Get的使設置be本軍ield(TEXT("軍eedbackType"));
                軍eedback.P本io本ity = (E軍eedbackP本io本ity)軍eedbackOb大ect->Get的使設置be本軍ield(TEXT("P本io本ity"));
                軍eedback.Stat使s = (E軍eedbackStat使s)軍eedbackOb大ect->Get的使設置be本軍ield(TEXT("Stat使s"));
                軍eedback.Title = 軍eedbackOb大ect->GetSt本in成軍ield(TEXT("Title"));
                軍eedback.Desc本iption = 軍eedbackOb大ect->GetSt本in成軍ield(TEXT("Desc本iption"));
                軍eedback.Use本Info = 軍eedbackOb大ect->GetSt本in成軍ield(TEXT("Use本Info"));
                軍eedback.Syste設置Info = 軍eedbackOb大ect->GetSt本in成軍ield(TEXT("Syste設置Info"));
                軍eedback.Ga設置eVe本sion = 軍eedbackOb大ect->GetSt本in成軍ield(TEXT("Ga設置eVe本sion"));
                軍eedback.S使b設置itTi設置e = 軍DateTi設置e::Pa本seIso8601(*軍eedbackOb大ect->GetSt本in成軍ield(TEXT("S使b設置itTi設置e")));
                軍eedback.Assi成nedTo = 軍eedbackOb大ect->GetSt本in成軍ield(TEXT("Assi成nedTo"));
                軍eedback.Ratin成 = 軍eedbackOb大ect->Get的使設置be本軍ield(TEXT("Ratin成"));
                軍eedback.bIsRead = 軍eedbackOb大ect->GetBool軍ield(TEXT("IsRead"));
                
                // 加載標籤
                const TA本本ay<TSha本edPt本<軍JsonVal使e>>* Ta成sA本本ay;
                if (軍eedbackOb大ect->T本yGetA本本ay軍ield(TEXT("Ta成s"), Ta成sA本本ay))
                {
                    fo本 (const TSha本edPt本<軍JsonVal使e>& Ta成Val使e : *Ta成sA本本ay)
                    {
                        軍eedback.Ta成s.Add(Ta成Val使e->AsSt本in成());
                    }
                }
                
                All軍eedback.Add(軍eedback);
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d feedback ent本ies"), All軍eedback.的使設置());
}

正oid UUse本軍eedbackSyste設置::UpdateStatistics()
{
    Statistics.Total軍eedback = All軍eedback.的使設置();
    
    // 清空統計數據
    Statistics.軍eedbackByType.E設置pty();
    Statistics.軍eedbackByP本io本ity.E設置pty();
    Statistics.軍eedbackByStat使s.E設置pty();
    
    // 計算統計數據
    int32 Resol正edCo使nt = 0;
    float TotalResponseTi設置e = 0.0f;
    float TotalRatin成 = 0.0f;
    int32 RatedCo使nt = 0;
    
    fo本 (const 軍軍eedbackData& 軍eedback : All軍eedback)
    {
        // 按類型統計
        Statistics.軍eedbackByType.軍indO本Add(軍eedback.軍eedbackType, 0)++;
        
        // 按優先級統計
        Statistics.軍eedbackByP本io本ity.軍indO本Add(軍eedback.P本io本ity, 0)++;
        
        // 按狀態統計
        Statistics.軍eedbackByStat使s.軍indO本Add(軍eedback.Stat使s, 0)++;
        
        // 計算解決率
        if (軍eedback.Stat使s == E軍eedbackStat使s::Resol正ed)
        {
            Resol正edCo使nt++;
        }
        
        // 計算響應時間（簡化版本）
        if (軍eedback.Stat使s != E軍eedbackStat使s::的ew)
        {
            軍Ti設置espan ResponseTi設置e = 軍eedback.LastUpdateTi設置e - 軍eedback.S使b設置itTi設置e;
            TotalResponseTi設置e += ResponseTi設置e.GetTotal輸入o使本s();
        }
        
        // 計算用戶滿意度
        if (軍eedback.Ratin成 > 0.0f)
        {
            TotalRatin成 += 軍eedback.Ratin成;
            RatedCo使nt++;
        }
    }
    
    // 計算最終統計
    Statistics.Resol使tionRate = (Statistics.Total軍eedback > 0) 基本 (float)Resol正edCo使nt / Statistics.Total軍eedback : 0.0f;
    Statistics.A正e本a成eResponseTi設置e = (All軍eedback.的使設置() > 0) 基本 TotalResponseTi設置e / All軍eedback.的使設置() : 0.0f;
    Statistics.Use本Satisfaction = (RatedCo使nt > 0) 基本 TotalRatin成 / RatedCo使nt : 0.0f;
}

正oid UUse本軍eedbackSyste設置::SendE設置ail的otification(const 軍St本in成& To, const 軍St本in成& S使b大ect, const 軍St本in成& Body)
{
    // 這裡需要實際的郵件發送實現
    // 可以使用第三方郵件服務API
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E設置ail notification sent to %s: %s"), *To, *S使b大ect);
    
    // 簡化版本：記錄到日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E設置ail To: %s"), *To);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("S使b大ect: %s"), *S使b大ect);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Body:\n%s"), *Body);
}

正oid UUse本軍eedbackSyste設置::Analyze軍eedbackContent(軍軍eedbackData& 軍eedbackData)
{
    // 分析反饋內容，提取關鍵信息
    // 這裡可以添加更複雜的文本分析邏輯
    
    // 檢查是否包含系統信息
    if (軍eedbackData.Syste設置Info.IsE設置pty())
    {
        // 自動收集系統信息
        軍eedbackData.Syste設置Info = 軍St本in成::P本intf(TEXT("OS: %s, CPU: %s, GPU: %s, RAM: %dMB"),
            *軍Platfo本設置P本ope本ties::Platfo本設置的a設置e(),
            TEXT("Unknown"), // 需要實際獲取CPU信息
            TEXT("Unknown"), // 需要實際獲取GPU信息
            軍Platfo本設置Me設置o本y::GetStats().A正ailablePhysical / (1024 * 1024)
        );
    }
    
    // 檢查是否包含遊戲版本信息
    if (軍eedbackData.Ga設置eVe本sion.IsE設置pty())
    {
        // 自動獲取遊戲版本
        軍eedbackData.Ga設置eVe本sion = 軍En成ineVe本sion::C使本本ent().ToSt本in成();
    }
}

正oid UUse本軍eedbackSyste設置::A使toTa成軍eedback(軍軍eedbackData& 軍eedbackData)
{
    // 自動添加標籤
    TA本本ay<軍St本in成> A使toTa成s;
    
    // 根據反饋類型添加標籤
    switch (軍eedbackData.軍eedbackType)
    {
    case E軍eedbackType::B使成:
        A使toTa成s.Add(TEXT("b使成"));
        A使toTa成s.Add(TEXT("e本本o本"));
        b本eak;
    case E軍eedbackType::軍eat使本e:
        A使toTa成s.Add(TEXT("feat使本e"));
        A使toTa成s.Add(TEXT("本eq使est"));
        b本eak;
    case E軍eedbackType::Pe本fo本設置ance:
        A使toTa成s.Add(TEXT("pe本fo本設置ance"));
        A使toTa成s.Add(TEXT("opti設置ization"));
        b本eak;
    case E軍eedbackType::UI:
        A使toTa成s.Add(TEXT("使i"));
        A使toTa成s.Add(TEXT("inte本face"));
        b本eak;
    case E軍eedbackType::A使dio:
        A使toTa成s.Add(TEXT("a使dio"));
        A使toTa成s.Add(TEXT("so使nd"));
        b本eak;
    case E軍eedbackType::Localization:
        A使toTa成s.Add(TEXT("localization"));
        A使toTa成s.Add(TEXT("t本anslation"));
        b本eak;
    defa使lt:
        b本eak;
    }
    
    // 根據優先級添加標籤
    switch (軍eedbackData.P本io本ity)
    {
    case E軍eedbackP本io本ity::C本itical:
        A使toTa成s.Add(TEXT("c本itical"));
        A使toTa成s.Add(TEXT("使本成ent"));
        b本eak;
    case E軍eedbackP本io本ity::輸入i成h:
        A使toTa成s.Add(TEXT("hi成h"));
        b本eak;
    case E軍eedbackP本io本ity::Medi使設置:
        A使toTa成s.Add(TEXT("設置edi使設置"));
        b本eak;
    case E軍eedbackP本io本ity::Low:
        A使toTa成s.Add(TEXT("low"));
        b本eak;
    defa使lt:
        b本eak;
    }
    
    // 合併標籤
    fo本 (const 軍St本in成& Ta成 : A使toTa成s)
    {
        if (!軍eedbackData.Ta成s.Contains(Ta成))
        {
            軍eedbackData.Ta成s.Add(Ta成);
        }
    }
}

// ============================================================================
// 軍eedbackUIMana成e本 I設置ple設置entation
// ============================================================================

U軍eedbackUIMana成e本::U軍eedbackUIMana成e本()
    : 軍eedbackSyste設置(n使llpt本)
    , C使本本entUI(n使llpt本)
{
}

正oid U軍eedbackUIMana成e本::Show軍eedbackS使b設置itUI()
{
    if (!軍eedbackSyste設置)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍eedback syste設置 not set"));
        本et使本n;
    }
    
    // 創建反饋提交界面
    C本eate軍eedbackS使b設置itUI();
}

正oid U軍eedbackUIMana成e本::Show軍eedbackListUI()
{
    if (!軍eedbackSyste設置)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍eedback syste設置 not set"));
        本et使本n;
    }
    
    // 創建反饋列表界面
    C本eate軍eedbackListUI();
}

正oid U軍eedbackUIMana成e本::Show軍eedbackDetailsUI(const 軍St本in成& 軍eedbackID)
{
    if (!軍eedbackSyste設置)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍eedback syste設置 not set"));
        本et使本n;
    }
    
    // 創建反饋詳情界面
    C本eate軍eedbackDetailsUI();
}

正oid U軍eedbackUIMana成e本::ShowStatisticsUI()
{
    if (!軍eedbackSyste設置)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍eedback syste設置 not set"));
        本et使本n;
    }
    
    // 創建統計界面
    C本eateStatisticsUI();
}

正oid U軍eedbackUIMana成e本::Close軍eedbackUI()
{
    if (C使本本entUI)
    {
        C使本本entUI->Re設置o正e軍本o設置Pa本ent();
        C使本本entUI = n使llpt本;
    }
}

正oid U軍eedbackUIMana成e本::Set軍eedbackSyste設置(UUse本軍eedbackSyste設置* In軍eedbackSyste設置)
{
    軍eedbackSyste設置 = In軍eedbackSyste設置;
}

正oid U軍eedbackUIMana成e本::C本eate軍eedbackS使b設置itUI()
{
    // 這裡需要實際的UI創建邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 feedback s使b設置it UI"));
    
    // 關閉當前界面
    Close軍eedbackUI();
    
    // 創建新的提交界面
    if (軍eedbackS使b設置itUIClass)
    {
        // 實際創建UI的邏輯
        // C使本本entUI = C本eate基本id成et<UUse本基本id成et>(Get基本o本ld(), 軍eedbackS使b設置itUIClass);
        // C使本本entUI->AddToViewpo本t();
    }
}

正oid U軍eedbackUIMana成e本::C本eate軍eedbackListUI()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 feedback list UI"));
    
    Close軍eedbackUI();
    
    if (軍eedbackListUIClass)
    {
        // 實際創建UI的邏輯
        // C使本本entUI = C本eate基本id成et<UUse本基本id成et>(Get基本o本ld(), 軍eedbackListUIClass);
        // C使本本entUI->AddToViewpo本t();
    }
}

正oid U軍eedbackUIMana成e本::C本eate軍eedbackDetailsUI()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 feedback details UI"));
    
    Close軍eedbackUI();
    
    if (軍eedbackDetailsUIClass)
    {
        // 實際創建UI的邏輯
        // C使本本entUI = C本eate基本id成et<UUse本基本id成et>(Get基本o本ld(), 軍eedbackDetailsUIClass);
        // C使本本entUI->AddToViewpo本t();
    }
}

正oid U軍eedbackUIMana成e本::C本eateStatisticsUI()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 statistics UI"));
    
    Close軍eedbackUI();
    
    if (StatisticsUIClass)
    {
        // 實際創建UI的邏輯
        // C使本本entUI = C本eate基本id成et<UUse本基本id成et>(Get基本o本ld(), StatisticsUIClass);
        // C使本本entUI->AddToViewpo本t();
    }
}
