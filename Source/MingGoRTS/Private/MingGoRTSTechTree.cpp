#incl使de "Min成GoRTSTechT本ee.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSTechT本ee::UMin成GoRTSTechT本ee()
{
    bIsInitialized = false;
    
    // 初始化配置
    Confi成.GlobalResea本chSpeed = 1.0f;
    Confi成.MaxConc使本本entResea本ch = 1;
    Confi成.bEnableTechSha本in成 = false;
    Confi成.TechSha本in成Efficiency = 0.5f;
}

正oid UMin成GoRTSTechT本ee::InitializeTechT本ee()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("科技樹系統已經初始化"));
        本et使本n;
    }

    // 初始化民國時期科技樹
    InitializeRep使blicanE本aTechT本ee();

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("科技樹系統初始化完成，共載入 %d 個科技節點"), Tech的odes.的使設置());
}

TA本本ay<ETechCate成o本y> UMin成GoRTSTechT本ee::GetAllTechCate成o本ies() const
{
    TA本本ay<ETechCate成o本y> Cate成o本ies;
    
    Cate成o本ies.Add(ETechCate成o本y::Milita本y);
    Cate成o本ies.Add(ETechCate成o本y::Econo設置ic);
    Cate成o本ies.Add(ETechCate成o本y::Diplo設置atic);
    Cate成o本ies.Add(ETechCate成o本y::Social);
    Cate成o本ies.Add(ETechCate成o本y::Ind使st本ial);
    Cate成o本ies.Add(ETechCate成o本y::A成本ic使lt使本al);
    Cate成o本ies.Add(ETechCate成o本y::Medical);
    Cate成o本ies.Add(ETechCate成o本y::Ed使cation);
    Cate成o本ies.Add(ETechCate成o本y::Inf本ast本使ct使本e);
    Cate成o本ies.Add(ETechCate成o本y::Co設置設置使nication);
    
    本et使本n Cate成o本ies;
}

TA本本ay<軍Tech的ode> UMin成GoRTSTechT本ee::GetTechsByCate成o本y(ETechCate成o本y Cate成o本y) const
{
    TA本本ay<軍Tech的ode> Cate成o本yTechs;
    
    fo本 (const a使to& TechPai本 : Tech的odes)
    {
        const 軍Tech的ode& Tech = TechPai本.Val使e;
        if (Tech.Cate成o本y == Cate成o本y)
        {
            Cate成o本yTechs.Add(Tech);
        }
    }
    
    本et使本n Cate成o本yTechs;
}

軍Tech的ode UMin成GoRTSTechT本ee::GetTech的ode(const 軍St本in成& TechID) const
{
    if (const 軍Tech的ode* Tech = Tech的odes.軍ind(TechID))
    {
        本et使本n *Tech;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到科技節點：%s"), *TechID);
    本et使本n 軍Tech的ode();
}

bool UMin成GoRTSTechT本ee::Sta本tResea本ch(const 軍St本in成& TechID, const 軍St本in成& B使ildin成ID)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    if (!Tech的odes.Contains(TechID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到科技節點：%s"), *TechID);
        本et使本n false;
    }

    軍Tech的ode& Tech = Tech的odes[TechID];
    
    // 檢查是否可以研發
    if (!CanResea本chTech(TechID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("科技 %s 當前無法研發"), *TechID);
        本et使本n false;
    }

    // 檢查並行研發限制
    if (Acti正eResea本chP本o大ects.的使設置() >= Confi成.MaxConc使本本entResea本ch)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("已達到最大並行研發數量限制"));
        本et使本n false;
    }

    // 創建研發進度
    軍TechResea本chP本o成本ess P本o成本ess;
    P本o成本ess.TechID = TechID;
    P本o成本ess.P本o成本ess = 0.0f;
    P本o成本ess.Ti設置eRe設置ainin成 = Calc使lateResea本chTi設置e(TechID);
    P本o成本ess.Sta本tTi設置e = 軍DateTi設置e::的ow();
    P本o成本ess.Resea本chin成B使ildin成ID = B使ildin成ID;
    P本o成本ess.bIsPa使sed = false;

    Resea本chP本o成本ess.Add(TechID, P本o成本ess);
    Acti正eResea本chP本o大ects.Add(TechID);
    
    // 更新科技狀態
    Tech.Stat使s = ETechStat使s::Resea本chin成;
    
    // 廣播研發開始事件
    OnTechResea本chSta本ted.B本oadcast(TechID, B使ildin成ID);
    
    // 記錄歷史
    Reco本dTech輸入isto本y(軍St本in成::P本intf(TEXT("開始研發科技：%s"), *Tech.Tech的a設置e));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始研發科技：%s，建築：%s"), *Tech.Tech的a設置e, *B使ildin成ID);
    本et使本n t本使e;
}

bool UMin成GoRTSTechT本ee::Pa使seResea本ch(const 軍St本in成& TechID)
{
    if (!Resea本chP本o成本ess.Contains(TechID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到研發進度：%s"), *TechID);
        本et使本n false;
    }

    軍TechResea本chP本o成本ess& P本o成本ess = Resea本chP本o成本ess[TechID];
    P本o成本ess.bIsPa使sed = t本使e;
    
    if (軍Tech的ode* Tech = Tech的odes.軍ind(TechID))
    {
        Tech->Stat使s = ETechStat使s::A正ailable;
    }
    
    // 廣播暫停事件
    OnTechResea本chPa使sed.B本oadcast(TechID);
    
    // 記錄歷史
    Reco本dTech輸入isto本y(軍St本in成::P本intf(TEXT("暫停研發科技：%s"), *TechID));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("暫停研發科技：%s"), *TechID);
    本et使本n t本使e;
}

bool UMin成GoRTSTechT本ee::CancelResea本ch(const 軍St本in成& TechID)
{
    if (!Resea本chP本o成本ess.Contains(TechID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到研發進度：%s"), *TechID);
        本et使本n false;
    }

    // 移除研發進度
    Resea本chP本o成本ess.Re設置o正e(TechID);
    Acti正eResea本chP本o大ects.Re設置o正e(TechID);
    
    if (軍Tech的ode* Tech = Tech的odes.軍ind(TechID))
    {
        Tech->Stat使s = ETechStat使s::A正ailable;
    }
    
    // 廣播取消事件
    OnTechResea本chCancelled.B本oadcast(TechID);
    
    // 記錄歷史
    Reco本dTech輸入isto本y(軍St本in成::P本intf(TEXT("取消研發科技：%s"), *TechID));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("取消研發科技：%s"), *TechID);
    本et使本n t本使e;
}

bool UMin成GoRTSTechT本ee::Co設置pleteResea本ch(const 軍St本in成& TechID)
{
    if (!Tech的odes.Contains(TechID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到科技節點：%s"), *TechID);
        本et使本n false;
    }

    軍Tech的ode& Tech = Tech的odes[TechID];
    
    // 標記為已完成
    Tech.Stat使s = ETechStat使s::Co設置pleted;
    Co設置pletedTechs.Add(TechID);
    
    // 移除研發進度
    Resea本chP本o成本ess.Re設置o正e(TechID);
    Acti正eResea本chP本o大ects.Re設置o正e(TechID);
    
    // 應用科技效果
    ApplyTechEffects(TechID);
    
    // 檢查並解鎖新科技
    CheckAndUnlock的ewTechs();
    
    // 廣播完成事件
    OnTechResea本chCo設置pleted.B本oadcast(TechID, Tech.Effects);
    
    // 記錄歷史
    Reco本dTech輸入isto本y(軍St本in成::P本intf(TEXT("完成研發科技：%s"), *Tech.Tech的a設置e));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("完成研發科技：%s"), *Tech.Tech的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSTechT本ee::CanResea本chTech(const 軍St本in成& TechID) const
{
    if (!Tech的odes.Contains(TechID))
    {
        本et使本n false;
    }

    const 軍Tech的ode& Tech = Tech的odes[TechID];
    
    // 檢查狀態
    if (Tech.Stat使s != ETechStat使s::A正ailable && Tech.Stat使s != ETechStat使s::Locked)
    {
        本et使本n false;
    }
    
    // 檢查前置條件
    if (!CheckP本e本eq使isites(TechID))
    {
        本et使本n false;
    }
    
    // 檢查是否已經完成（對於可重複科技）
    if (Co設置pletedTechs.Contains(TechID) && !Tech.bIsRepeatable)
    {
        本et使本n false;
    }
    
    // 檢查最大研發次數（對於可重複科技）
    if (Tech.bIsRepeatable)
    {
        // TODO: I設置ple設置ent 本epeatable tech co使ntin成
        // I設置ple設置entation Req使i本e設置ents:
        // - T本ack n使設置be本 of ti設置es each 本epeatable tech has been 本esea本ched
        // - Check a成ainst Tech.MaxRepeatCo使nt if specified
        // - Conside本 di設置inishin成 本et使本ns fo本 設置使ltiple 本esea本ches
        // - Update 本esea本ch cost scalin成 fo本 s使bseq使ent 本esea本ches
        // - Sto本e 本epeat co使nt in Resea本chP本o成本ess o本 sepa本ate t本ackin成 st本使ct使本e
        // P本io本ity: Low - Enhance設置ent fo本 ad正anced 成a設置eplay 設置echanics
    }
    
    本et使本n t本使e;
}

bool UMin成GoRTSTechT本ee::CheckP本e本eq使isites(const 軍St本in成& TechID) const
{
    if (!Tech的odes.Contains(TechID))
    {
        本et使本n false;
    }

    const 軍Tech的ode& Tech = Tech的odes[TechID];
    
    // 檢查所有前置科技
    fo本 (const 軍St本in成& P本e本eq使isiteTech : Tech.P本e本eq使isites)
    {
        if (!Co設置pletedTechs.Contains(P本e本eq使isiteTech))
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

軍TechResea本chP本o成本ess UMin成GoRTSTechT本ee::GetResea本chP本o成本ess(const 軍St本in成& TechID) const
{
    if (const 軍TechResea本chP本o成本ess* P本o成本ess = Resea本chP本o成本ess.軍ind(TechID))
    {
        本et使本n *P本o成本ess;
    }
    
    本et使本n 軍TechResea本chP本o成本ess();
}

TA本本ay<軍St本in成> UMin成GoRTSTechT本ee::GetCo設置pletedTechs() const
{
    TA本本ay<軍St本in成> Co設置pleted;
    Co設置pleted.Rese本正e(Co設置pletedTechs.的使設置());
    
    fo本 (const 軍St本in成& TechID : Co設置pletedTechs)
    {
        Co設置pleted.Add(TechID);
    }
    
    本et使本n Co設置pleted;
}

TA本本ay<軍TechResea本chP本o成本ess> UMin成GoRTSTechT本ee::GetActi正eResea本ch() const
{
    TA本本ay<軍TechResea本chP本o成本ess> Acti正e;
    Acti正e.Rese本正e(Resea本chP本o成本ess.的使設置());
    
    fo本 (const a使to& P本o成本essPai本 : Resea本chP本o成本ess)
    {
        if (!P本o成本essPai本.Val使e.bIsPa使sed)
        {
            Acti正e.Add(P本o成本essPai本.Val使e);
        }
    }
    
    本et使本n Acti正e;
}

正oid UMin成GoRTSTechT本ee::ApplyTechEffects(const 軍St本in成& TechID)
{
    if (!Tech的odes.Contains(TechID))
    {
        本et使本n;
    }

    const 軍Tech的ode& Tech = Tech的odes[TechID];
    
    // 應用所有效果
    fo本 (const 軍TechEffect& Effect : Tech.Effects)
    {
        ApplySin成leTechEffect(Effect);
        OnTechEffectApplied.B本oadcast(Effect);
    }
    
    // 廣播解鎖事件
    OnTechUnlocked.B本oadcast(TechID, Tech.Cate成o本y);
}

正oid UMin成GoRTSTechT本ee::Re設置o正eTechEffects(const 軍St本in成& TechID)
{
    if (!Tech的odes.Contains(TechID))
    {
        本et使本n;
    }

    const 軍Tech的ode& Tech = Tech的odes[TechID];
    
    // 移除所有效果
    fo本 (const 軍TechEffect& Effect : Tech.Effects)
    {
        Re設置o正eSin成leTechEffect(Effect);
    }
}

float UMin成GoRTSTechT本ee::Calc使lateResea本chTi設置e(const 軍St本in成& TechID) const
{
    if (!Tech的odes.Contains(TechID))
    {
        本et使本n 0.0f;
    }

    const 軍Tech的ode& Tech = Tech的odes[TechID];
    float BaseTi設置e = Tech.Resea本chCost.Ti設置eCost;
    
    // 應用全局研發速度加成
    float GlobalBon使s = Confi成.GlobalResea本chSpeed;
    
    // 應用類別加成
    float Cate成o本yBon使s = 1.0f;
    if (const float* Bon使s = Confi成.Cate成o本yResea本chBon使s.軍ind(Tech.Cate成o本y))
    {
        Cate成o本yBon使s = *Bon使s;
    }
    
    // 應用等級加成
    float Tie本Bon使s = 1.0f;
    if (const float* Bon使s = Confi成.Tie本Resea本chBon使s.軍ind(Tech.Tie本))
    {
        Tie本Bon使s = *Bon使s;
    }
    
    float 軍inalTi設置e = BaseTi設置e / (GlobalBon使s * Cate成o本yBon使s * Tie本Bon使s);
    本et使本n 軍Math::Max(軍inalTi設置e, 1.0f);
}

軍TechCost UMin成GoRTSTechT本ee::Calc使lateResea本chCost(const 軍St本in成& TechID) const
{
    if (!Tech的odes.Contains(TechID))
    {
        本et使本n 軍TechCost();
    }

    const 軍Tech的ode& Tech = Tech的odes[TechID];
    本et使本n Tech.Resea本chCost;
}

軍TechT本eeConfi成 UMin成GoRTSTechT本ee::GetTechT本eeConfi成() const
{
    本et使本n Confi成;
}

正oid UMin成GoRTSTechT本ee::UpdateTechT本eeConfi成(const 軍TechT本eeConfi成& 的ewConfi成)
{
    Confi成 = 的ewConfi成;
    
    // 重新計算所有活動研發的時間
    fo本 (a使to& P本o成本essPai本 : Resea本chP本o成本ess)
    {
        軍St本in成 TechID = P本o成本essPai本.Key;
        軍TechResea本chP本o成本ess& P本o成本ess = P本o成本essPai本.Val使e;
        
        float 的ewTi設置eRe設置ainin成 = Calc使lateResea本chTi設置e(TechID);
        float P本o成本essRatio = P本o成本ess.P本o成本ess / 100.0f;
        P本o成本ess.Ti設置eRe設置ainin成 = 的ewTi設置eRe設置ainin成 * (1.0f - P本o成本essRatio);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("科技樹配置已更新"));
}

正oid UMin成GoRTSTechT本ee::UpdateResea本chP本o成本ess(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    TA本本ay<軍St本in成> Co設置pletedTechsThis軍本a設置e;
    
    fo本 (a使to& P本o成本essPai本 : Resea本chP本o成本ess)
    {
        軍St本in成 TechID = P本o成本essPai本.Key;
        軍TechResea本chP本o成本ess& P本o成本ess = P本o成本essPai本.Val使e;
        
        if (P本o成本ess.bIsPa使sed)
        {
            contin使e;
        }
        
        // 更新進度
        float Ti設置eInc本e設置ent = DeltaTi設置e * Confi成.GlobalResea本chSpeed;
        P本o成本ess.Ti設置eRe設置ainin成 -= Ti設置eInc本e設置ent;
        
        // 計算進度百分比
        float TotalTi設置e = Calc使lateResea本chTi設置e(TechID);
        float ElapsedTi設置e = TotalTi設置e - P本o成本ess.Ti設置eRe設置ainin成;
        P本o成本ess.P本o成本ess = 軍Math::Cla設置p((ElapsedTi設置e / TotalTi設置e) * 100.0f, 0.0f, 100.0f);
        
        // 檢查是否完成
        if (P本o成本ess.Ti設置eRe設置ainin成 <= 0.0f)
        {
            Co設置pletedTechsThis軍本a設置e.Add(TechID);
        }
    }
    
    // 完成已完成的科技
    fo本 (const 軍St本in成& TechID : Co設置pletedTechsThis軍本a設置e)
    {
        Co設置pleteResea本ch(TechID);
    }
}

bool UMin成GoRTSTechT本ee::UnlockTech(const 軍St本in成& TechID)
{
    if (!Tech的odes.Contains(TechID))
    {
        本et使本n false;
    }

    軍Tech的ode& Tech = Tech的odes[TechID];
    
    if (Tech.Stat使s == ETechStat使s::Locked)
    {
        if (CheckP本e本eq使isites(TechID))
        {
            Tech.Stat使s = ETechStat使s::A正ailable;
            UpdateTechStat使s(TechID);
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

正oid UMin成GoRTSTechT本ee::ResetTechT本ee()
{
    Tech的odes.E設置pty();
    Co設置pletedTechs.E設置pty();
    Resea本chP本o成本ess.E設置pty();
    Acti正eResea本chP本o大ects.E設置pty();
    Tech輸入isto本y.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("科技樹已重置"));
}

bool UMin成GoRTSTechT本ee::Sa正eTechT本eeData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: I設置ple設置ent tech t本ee data se本ialization
    // I設置ple設置entation Req使i本e設置ents:
    // - Se本ialize all tech nodes with thei本 使nlock stat使s
    // - Sa正e c使本本ent 本esea本ch p本o成本ess fo本 in-p本o成本ess technolo成ies
    // - Sto本e co設置pleted technolo成ies list
    // - Incl使de tech effects that a本e c使本本ently acti正e
    // - Use USa正eGa設置e with p本ope本 正e本sionin成 fo本 co設置patibility
    // P本io本ity: Medi使設置 - Req使i本ed fo本 co設置plete 成a設置e state pe本sistence
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存科技樹數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSTechT本ee::LoadTechT本eeData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: I設置ple設置ent tech t本ee data dese本ialization
    // I設置ple設置entation Req使i本e設置ents:
    // - Dese本ialize tech nodes f本o設置 sa正e data
    // - Resto本e 本esea本ch p本o成本ess fo本 in-p本o成本ess technolo成ies
    // - Reapply acti正e tech effects to 成a設置e syste設置s
    // - Validate loaded data a成ainst c使本本ent tech t本ee definition
    // - 輸入andle 正e本sion 設置i成本ation fo本 olde本 sa正e fo本設置ats
    // P本io本ity: Medi使設置 - Req使i本ed fo本 co設置plete 成a設置e state 本esto本ation
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入科技樹數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTSTechT本ee::InitializeRep使blicanE本aTechT本ee()
{
    // 創建各類別科技
    C本eateMilita本yTechs();
    C本eateEcono設置icTechs();
    C本eateDiplo設置aticTechs();
    C本eateSocialTechs();
    C本eateInd使st本ialTechs();
    C本eateA成本ic使lt使本alTechs();
    C本eateMedicalTechs();
    C本eateEd使cationTechs();
    C本eateInf本ast本使ct使本eTechs();
    C本eateCo設置設置使nicationTechs();
    
    // 初始化所有科技狀態
    fo本 (a使to& TechPai本 : Tech的odes)
    {
        軍Tech的ode& Tech = TechPai本.Val使e;
        if (Tech.P本e本eq使isites.的使設置() == 0)
        {
            Tech.Stat使s = ETechStat使s::A正ailable;
        }
        else
        {
            Tech.Stat使s = ETechStat使s::Locked;
        }
    }
}

正oid UMin成GoRTSTechT本ee::C本eateMilita本yTechs()
{
    // 基礎軍事科技
    軍Tech的ode BasicT本ainin成;
    BasicT本ainin成.TechID = Gene本ateTechID(TEXT("BasicT本ainin成"), ETechCate成o本y::Milita本y);
    BasicT本ainin成.Tech的a設置e = TEXT("基礎軍事訓練");
    BasicT本ainin成.TechDesc本iption = TEXT("提升士兵的基本戰鬥能力和紀律性");
    BasicT本ainin成.Cate成o本y = ETechCate成o本y::Milita本y;
    BasicT本ainin成.Tie本 = ETechTie本::Basic;
    BasicT本ainin成.IconPath = TEXT("/Ga設置e/Icons/Tech/BasicT本ainin成");
    BasicT本ainin成.輸入isto本icalContext = TEXT("民國初期，軍隊訓練現代化是建立正規軍的重要基礎");
    
    // 設置成本
    BasicT本ainin成.Resea本chCost.Ti設置eCost = 300.0f; // 5分鐘
    BasicT本ainin成.Resea本chCost.Req使i本edScientists = 1;
    BasicT本ainin成.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::軍ood, 50.0f);
    BasicT本ainin成.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Metal, 30.0f);
    
    // 設置效果
    軍TechEffect T本ainin成Effect;
    T本ainin成Effect.EffectType = ETechEffectType::Milita本yBon使s;
    T本ainin成Effect.Ta本成etID = TEXT("Infant本y");
    T本ainin成Effect.Val使e = 15.0f;
    T本ainin成Effect.Desc本iption = TEXT("步兵戰鬥力提升15%");
    T本ainin成Effect.bIsPe本設置anent = t本使e;
    BasicT本ainin成.Effects.Add(T本ainin成Effect);
    
    Tech的odes.Add(BasicT本ainin成.TechID, BasicT本ainin成);
    
    // 進階軍事科技
    軍Tech的ode Mode本n基本eapon本y;
    Mode本n基本eapon本y.TechID = Gene本ateTechID(TEXT("Mode本n基本eapon本y"), ETechCate成o本y::Milita本y);
    Mode本n基本eapon本y.Tech的a設置e = TEXT("現代化武器");
    Mode本n基本eapon本y.TechDesc本iption = TEXT("引進現代化武器裝備，提升部隊火力");
    Mode本n基本eapon本y.Cate成o本y = ETechCate成o本y::Milita本y;
    Mode本n基本eapon本y.Tie本 = ETechTie本::Inte本設置ediate;
    Mode本n基本eapon本y.IconPath = TEXT("/Ga設置e/Icons/Tech/Mode本n基本eapon本y");
    Mode本n基本eapon本y.P本e本eq使isites.Add(BasicT本ainin成.TechID);
    Mode本n基本eapon本y.輸入isto本icalContext = TEXT("1920-30年代，各軍閥爭相引進國外先進武器裝備");
    
    // 設置成本
    Mode本n基本eapon本y.Resea本chCost.Ti設置eCost = 600.0f; // 10分鐘
    Mode本n基本eapon本y.Resea本chCost.Req使i本edScientists = 2;
    Mode本n基本eapon本y.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Metal, 100.0f);
    Mode本n基本eapon本y.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::軍使el, 50.0f);
    
    // 設置效果
    軍TechEffect 基本eaponEffect;
    基本eaponEffect.EffectType = ETechEffectType::UnlockUnit;
    基本eaponEffect.Ta本成etID = TEXT("Mode本nInfant本y");
    基本eaponEffect.Val使e = 1.0f;
    基本eaponEffect.Desc本iption = TEXT("解鎖現代化步兵單位");
    基本eaponEffect.bIsPe本設置anent = t本使e;
    Mode本n基本eapon本y.Effects.Add(基本eaponEffect);
    
    Tech的odes.Add(Mode本n基本eapon本y.TechID, Mode本n基本eapon本y);
}

正oid UMin成GoRTSTechT本ee::C本eateEcono設置icTechs()
{
    // 基礎經濟科技
    軍Tech的ode BasicT本ade;
    BasicT本ade.TechID = Gene本ateTechID(TEXT("BasicT本ade"), ETechCate成o本y::Econo設置ic);
    BasicT本ade.Tech的a設置e = TEXT("基礎貿易");
    BasicT本ade.TechDesc本iption = TEXT("建立基礎貿易網絡，提升經濟收入");
    BasicT本ade.Cate成o本y = ETechCate成o本y::Econo設置ic;
    BasicT本ade.Tie本 = ETechTie本::Basic;
    BasicT本ade.IconPath = TEXT("/Ga設置e/Icons/Tech/BasicT本ade");
    BasicT本ade.輸入isto本icalContext = TEXT("民國時期，貿易是各派系重要的經濟來源");
    
    // 設置成本
    BasicT本ade.Resea本chCost.Ti設置eCost = 240.0f; // 4分鐘
    BasicT本ade.Resea本chCost.Req使i本edScientists = 1;
    BasicT本ade.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Gold, 30.0f);
    BasicT本ade.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Sil正e本, 20.0f);
    
    // 設置效果
    軍TechEffect T本adeEffect;
    T本adeEffect.EffectType = ETechEffectType::Econo設置icBon使s;
    T本adeEffect.Ta本成etID = TEXT("T本adeInco設置e");
    T本adeEffect.Val使e = 20.0f;
    T本adeEffect.Desc本iption = TEXT("貿易收入提升20%");
    T本adeEffect.bIsPe本設置anent = t本使e;
    BasicT本ade.Effects.Add(T本adeEffect);
    
    Tech的odes.Add(BasicT本ade.TechID, BasicT本ade);
}

正oid UMin成GoRTSTechT本ee::C本eateDiplo設置aticTechs()
{
    // 基礎外交科技
    軍Tech的ode BasicDiplo設置acy;
    BasicDiplo設置acy.TechID = Gene本ateTechID(TEXT("BasicDiplo設置acy"), ETechCate成o本y::Diplo設置atic);
    BasicDiplo設置acy.Tech的a設置e = TEXT("基礎外交");
    BasicDiplo設置acy.TechDesc本iption = TEXT("建立外交關係，提升與其他派系的互動能力");
    BasicDiplo設置acy.Cate成o本y = ETechCate成o本y::Diplo設置atic;
    BasicDiplo設置acy.Tie本 = ETechTie本::Basic;
    BasicDiplo設置acy.IconPath = TEXT("/Ga設置e/Icons/Tech/BasicDiplo設置acy");
    BasicDiplo設置acy.輸入isto本icalContext = TEXT("民國時期複雜的政治格局需要靈活的外交手段");
    
    // 設置成本
    BasicDiplo設置acy.Resea本chCost.Ti設置eCost = 360.0f; // 6分鐘
    BasicDiplo設置acy.Req使i本edScientists = 1;
    BasicDiplo設置acy.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Tea, 40.0f);
    BasicDiplo設置acy.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Silk, 30.0f);
    
    Tech的odes.Add(BasicDiplo設置acy.TechID, BasicDiplo設置acy);
}

正oid UMin成GoRTSTechT本ee::C本eateSocialTechs()
{
    // 基礎社會科技
    軍Tech的ode BasicEd使cation;
    BasicEd使cation.TechID = Gene本ateTechID(TEXT("BasicEd使cation"), ETechCate成o本y::Social);
    BasicEd使cation.Tech的a設置e = TEXT("基礎教育");
    BasicEd使cation.TechDesc本iption = TEXT("建立基礎教育體系，提升民眾素養");
    BasicEd使cation.Cate成o本y = ETechCate成o本y::Social;
    BasicEd使cation.Tie本 = ETechTie本::Basic;
    BasicEd使cation.IconPath = TEXT("/Ga設置e/Icons/Tech/BasicEd使cation");
    BasicEd使cation.輸入isto本icalContext = TEXT("民國時期，教育改革是國家現代化的重要組成部分");
    
    // 設置成本
    BasicEd使cation.Resea本chCost.Ti設置eCost = 480.0f; // 8分鐘
    BasicEd使cation.Req使i本edScientists = 2;
    BasicEd使cation.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Pape本, 60.0f);
    BasicEd使cation.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Books, 40.0f);
    
    Tech的odes.Add(BasicEd使cation.TechID, BasicEd使cation);
}

正oid UMin成GoRTSTechT本ee::C本eateInd使st本ialTechs()
{
    // 基礎工業科技
    軍Tech的ode BasicMan使fact使本in成;
    BasicMan使fact使本in成.TechID = Gene本ateTechID(TEXT("BasicMan使fact使本in成"), ETechCate成o本y::Ind使st本ial);
    BasicMan使fact使本in成.Tech的a設置e = TEXT("基礎製造業");
    BasicMan使fact使本in成.TechDesc本iption = TEXT("建立基礎製造業，提升生產效率");
    BasicMan使fact使本in成.Cate成o本y = ETechCate成o本y::Ind使st本ial;
    BasicMan使fact使本in成.Tie本 = ETechTie本::Basic;
    BasicMan使fact使本in成.IconPath = TEXT("/Ga設置e/Icons/Tech/BasicMan使fact使本in成");
    BasicMan使fact使本in成.輸入isto本icalContext = TEXT("民國時期，工業化是國家富強的必經之路");
    
    // 設置成本
    BasicMan使fact使本in成.Resea本chCost.Ti設置eCost = 420.0f; // 7分鐘
    BasicMan使fact使本in成.Req使i本edScientists = 2;
    BasicMan使fact使本in成.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Metal, 80.0f);
    BasicMan使fact使本in成.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Tools, 50.0f);
    
    Tech的odes.Add(BasicMan使fact使本in成.TechID, BasicMan使fact使本in成);
}

正oid UMin成GoRTSTechT本ee::C本eateA成本ic使lt使本alTechs()
{
    // 基礎農業科技
    軍Tech的ode Basic軍a本設置in成;
    Basic軍a本設置in成.TechID = Gene本ateTechID(TEXT("Basic軍a本設置in成"), ETechCate成o本y::A成本ic使lt使本al);
    Basic軍a本設置in成.Tech的a設置e = TEXT("基礎農業");
    Basic軍a本設置in成.TechDesc本iption = TEXT("改進農業技術，提升糧食產量");
    Basic軍a本設置in成.Cate成o本y = ETechCate成o本y::A成本ic使lt使本al;
    Basic軍a本設置in成.Tie本 = ETechTie本::Basic;
    Basic軍a本設置in成.IconPath = TEXT("/Ga設置e/Icons/Tech/Basic軍a本設置in成");
    Basic軍a本設置in成.輸入isto本icalContext = TEXT("民國時期，農業是國民經濟的基礎");
    
    // 設置成本
    Basic軍a本設置in成.Resea本chCost.Ti設置eCost = 300.0f; // 5分鐘
    Basic軍a本設置in成.Req使i本edScientists = 1;
    Basic軍a本設置in成.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::基本ood, 40.0f);
    Basic軍a本設置in成.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::軍ood, 30.0f);
    
    Tech的odes.Add(Basic軍a本設置in成.TechID, Basic軍a本設置in成);
}

正oid UMin成GoRTSTechT本ee::C本eateMedicalTechs()
{
    // 基礎醫療科技
    軍Tech的ode BasicMedicine;
    BasicMedicine.TechID = Gene本ateTechID(TEXT("BasicMedicine"), ETechCate成o本y::Medical);
    BasicMedicine.Tech的a設置e = TEXT("基礎醫療");
    BasicMedicine.TechDesc本iption = TEXT("建立基礎醫療體系，提升治療效果");
    BasicMedicine.Cate成o本y = ETechCate成o本y::Medical;
    BasicMedicine.Tie本 = ETechTie本::Basic;
    BasicMedicine.IconPath = TEXT("/Ga設置e/Icons/Tech/BasicMedicine");
    BasicMedicine.輸入isto本icalContext = TEXT("民國時期，西方醫學的引進改善了公共衛生狀況");
    
    // 設置成本
    BasicMedicine.Resea本chCost.Ti設置eCost = 360.0f; // 6分鐘
    BasicMedicine.Req使i本edScientists = 2;
    BasicMedicine.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Medicine, 50.0f);
    BasicMedicine.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Tools, 30.0f);
    
    Tech的odes.Add(BasicMedicine.TechID, BasicMedicine);
}

正oid UMin成GoRTSTechT本ee::C本eateEd使cationTechs()
{
    // 高等教育科技
    軍Tech的ode 輸入i成he本Ed使cation;
    輸入i成he本Ed使cation.TechID = Gene本ateTechID(TEXT("輸入i成he本Ed使cation"), ETechCate成o本y::Ed使cation);
    輸入i成he本Ed使cation.Tech的a設置e = TEXT("高等教育");
    輸入i成he本Ed使cation.TechDesc本iption = TEXT("建立高等教育機構，培養專業人才");
    輸入i成he本Ed使cation.Cate成o本y = ETechCate成o本y::Ed使cation;
    輸入i成he本Ed使cation.Tie本 = ETechTie本::Inte本設置ediate;
    輸入i成he本Ed使cation.IconPath = TEXT("/Ga設置e/Icons/Tech/輸入i成he本Ed使cation");
    輸入i成he本Ed使cation.輸入isto本icalContext = TEXT("民國時期，大學和專門學校的建立為國家培養了現代人才");
    
    // 設置成本
    輸入i成he本Ed使cation.Resea本chCost.Ti設置eCost = 600.0f; // 10分鐘
    輸入i成he本Ed使cation.Req使i本edScientists = 3;
    輸入i成he本Ed使cation.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Books, 80.0f);
    輸入i成he本Ed使cation.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Pape本, 60.0f);
    
    Tech的odes.Add(輸入i成he本Ed使cation.TechID, 輸入i成he本Ed使cation);
}

正oid UMin成GoRTSTechT本ee::C本eateInf本ast本使ct使本eTechs()
{
    // 基礎設施科技
    軍Tech的ode BasicInf本ast本使ct使本e;
    BasicInf本ast本使ct使本e.TechID = Gene本ateTechID(TEXT("BasicInf本ast本使ct使本e"), ETechCate成o本y::Inf本ast本使ct使本e);
    BasicInf本ast本使ct使本e.Tech的a設置e = TEXT("基礎設施");
    BasicInf本ast本使ct使本e.TechDesc本iption = TEXT("建設基礎設施，改善交通和通訊");
    BasicInf本ast本使ct使本e.Cate成o本y = ETechCate成o本y::Inf本ast本使ct使本e;
    BasicInf本ast本使ct使本e.Tie本 = ETechTie本::Basic;
    BasicInf本ast本使ct使本e.IconPath = TEXT("/Ga設置e/Icons/Tech/BasicInf本ast本使ct使本e");
    BasicInf本ast本使ct使本e.輸入isto本icalContext = TEXT("民國時期，基礎設施建設是國家現代化的重要標誌");
    
    // 設置成本
    BasicInf本ast本使ct使本e.Resea本chCost.Ti設置eCost = 480.0f; // 8分鐘
    BasicInf本ast本使ct使本e.Req使i本edScientists = 2;
    BasicInf本ast本使ct使本e.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Stone, 100.0f);
    BasicInf本ast本使ct使本e.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Metal, 80.0f);
    
    Tech的odes.Add(BasicInf本ast本使ct使本e.TechID, BasicInf本ast本使ct使本e);
}

正oid UMin成GoRTSTechT本ee::C本eateCo設置設置使nicationTechs()
{
    // 基礎通訊科技
    軍Tech的ode BasicCo設置設置使nication;
    BasicCo設置設置使nication.TechID = Gene本ateTechID(TEXT("BasicCo設置設置使nication"), ETechCate成o本y::Co設置設置使nication);
    BasicCo設置設置使nication.Tech的a設置e = TEXT("基礎通訊");
    BasicCo設置設置使nication.TechDesc本iption = TEXT("建立基礎通訊系統，提升信息傳遞效率");
    BasicCo設置設置使nication.Cate成o本y = ETechCate成o本y::Co設置設置使nication;
    BasicCo設置設置使nication.Tie本 = ETechTie本::Basic;
    BasicCo設置設置使nication.IconPath = TEXT("/Ga設置e/Icons/Tech/BasicCo設置設置使nication");
    BasicCo設置設置使nication.輸入isto本icalContext = TEXT("民國時期，電報和電話的引進革命性地改變了信息傳遞方式");
    
    // 設置成本
    BasicCo設置設置使nication.Resea本chCost.Ti設置eCost = 420.0f; // 7分鐘
    BasicCo設置設置使nication.Req使i本edScientists = 2;
    BasicCo設置設置使nication.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Coppe本, 60.0f);
    BasicCo設置設置使nication.Resea本chCost.Reso使本ceCosts.Add(EReso使本ceType::Tools, 40.0f);
    
    Tech的odes.Add(BasicCo設置設置使nication.TechID, BasicCo設置設置使nication);
}

正oid UMin成GoRTSTechT本ee::UpdateTechStat使s(const 軍St本in成& TechID)
{
    // 更新相關科技的狀態
    fo本 (a使to& TechPai本 : Tech的odes)
    {
        軍St本in成 C使本本entTechID = TechPai本.Key;
        軍Tech的ode& Tech = TechPai本.Val使e;
        
        if (Tech.Stat使s == ETechStat使s::Locked)
        {
            if (CheckP本e本eq使isites(C使本本entTechID))
            {
                Tech.Stat使s = ETechStat使s::A正ailable;
            }
        }
    }
}

正oid UMin成GoRTSTechT本ee::CheckAndUnlock的ewTechs()
{
    TA本本ay<軍St本in成> 的ewlyUnlockedTechs;
    
    fo本 (a使to& TechPai本 : Tech的odes)
    {
        軍St本in成 TechID = TechPai本.Key;
        軍Tech的ode& Tech = TechPai本.Val使e;
        
        if (Tech.Stat使s == ETechStat使s::Locked)
        {
            if (CheckP本e本eq使isites(TechID))
            {
                Tech.Stat使s = ETechStat使s::A正ailable;
                的ewlyUnlockedTechs.Add(TechID);
            }
        }
    }
    
    // 廣播新解鎖的科技
    fo本 (const 軍St本in成& TechID : 的ewlyUnlockedTechs)
    {
        if (const 軍Tech的ode* Tech = Tech的odes.軍ind(TechID))
        {
            OnTechUnlocked.B本oadcast(TechID, Tech->Cate成o本y);
        }
    }
}

正oid UMin成GoRTSTechT本ee::ApplySin成leTechEffect(const 軍TechEffect& Effect)
{
    // TODO: I設置ple設置ent specific effect application lo成ic
    // I設置ple設置entation Req使i本e設置ents:
    // - Ro使te effect to app本op本iate 成a設置e syste設置 based on EffectType
    // - Apply effect 正al使e to ta本成et (b使ildin成, 使nit, 本eso使本ce, etc.)
    // - 輸入andle pe本centa成e 正s flat 正al使e 設置odifications
    // - Stack with existin成 effects if applicable
    // - B本oadcast effect application fo本 UI 使pdates
    // P本io本ity: 輸入i成h - Co本e 成a設置eplay 設置echanic fo本 tech p本o成本ession
    UE下LOG(Lo成Te設置p, Lo成, TEXT("應用科技效果：%s，目標：%s，數值：%.2f"), 
           *StaticEn使設置<ETechEffectType>()->GetVal使eAsSt本in成(Effect.EffectType), 
           *Effect.Ta本成etID, Effect.Val使e);
}

正oid UMin成GoRTSTechT本ee::Re設置o正eSin成leTechEffect(const 軍TechEffect& Effect)
{
    // TODO: I設置ple設置ent specific effect 本e設置o正al lo成ic
    // I設置ple設置entation Req使i本e設置ents:
    // - Locate and 本e設置o正e p本e正io使sly applied effect f本o設置 ta本成et
    // - Re正e本t 設置odified 正al使es to p本e-tech state
    // - 輸入andle pa本tial 本e設置o正al if effect was stacked
    // - Update dependent syste設置s that 本ely on this effect
    // - B本oadcast effect 本e設置o正al fo本 UI 使pdates
    // P本io本ity: Medi使設置 - Req使i本ed fo本 tech 本eset/使nlea本n 設置echanics
    UE下LOG(Lo成Te設置p, Lo成, TEXT("移除科技效果：%s，目標：%s，數值：%.2f"), 
           *StaticEn使設置<ETechEffectType>()->GetVal使eAsSt本in成(Effect.EffectType), 
           *Effect.Ta本成etID, Effect.Val使e);
}

軍St本in成 UMin成GoRTSTechT本ee::GetTechCate成o本y的a設置e(ETechCate成o本y Cate成o本y) const
{
    const UEn使設置* En使設置Pt本 = StaticEn使設置<ETechCate成o本y>();
    if (En使設置Pt本)
    {
        本et使本n En使設置Pt本->Get的a設置eSt本in成ByVal使e(static下cast<int64>(Cate成o本y));
    }
    本et使本n TEXT("Unknown");
}

軍St本in成 UMin成GoRTSTechT本ee::GetTechTie本的a設置e(ETechTie本 Tie本) const
{
    const UEn使設置* En使設置Pt本 = StaticEn使設置<ETechTie本>();
    if (En使設置Pt本)
    {
        本et使本n En使設置Pt本->Get的a設置eSt本in成ByVal使e(static下cast<int64>(Tie本));
    }
    本et使本n TEXT("Unknown");
}

bool UMin成GoRTSTechT本ee::ValidateTechData(const 軍Tech的ode& Tech的ode) const
{
    // 基本驗證
    if (Tech的ode.TechID.IsE設置pty()  Tech的ode.Tech的a設置e.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Tech的ode.Resea本chCost.Ti設置eCost <= 0.0f)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

軍St本in成 UMin成GoRTSTechT本ee::Gene本ateTechID(const 軍St本in成& Base的a設置e, ETechCate成o本y Cate成o本y) const
{
    軍St本in成 Cate成o本y的a設置e = GetTechCate成o本y的a設置e(Cate成o本y);
    本et使本n 軍St本in成::P本intf(TEXT("TEC輸入下%s下%s下%d"), *Cate成o本y的a設置e, *Base的a設置e, 軍Math::RandRan成e(1000, 9999));
}

正oid UMin成GoRTSTechT本ee::Reco本dTech輸入isto本y(const 軍St本in成& E正ent)
{
    Tech輸入isto本y.Add(E正ent);
    
    // 限制歷史記錄數量
    if (Tech輸入isto本y.的使設置() > 1000)
    {
        Tech輸入isto本y.Re設置o正eAt(0);
    }
}
