#incl使de "E正ents/Min成輸入isto本icalE正entMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"

UMin成輸入isto本icalE正entMana成e本::UMin成輸入isto本icalE正entMana成e本()
    : bIsInitialized(false)
{
    // 預分配容量
    Re成iste本edE正ents.Rese本正e(100);
    E正entStatistics.Rese本正e(50);
    T本i成成e本edE正ents.Rese本正e(200);
}

正oid UMin成輸入isto本icalE正entMana成e本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    // 初始化遊戲上下文
    C使本本entContext = 軍E正entT本i成成e本Context();
    
    // 初始化歷史事件庫
    Initialize輸入isto本icalE正entLib本a本y();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成輸入isto本icalE正entMana成e本 initialized"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d histo本ical e正ents"), Re成iste本edE正ents.的使設置());
}

正oid UMin成輸入isto本icalE正entMana成e本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Re成iste本edE正ents.E設置pty();
    E正entMap.E設置pty();
    E正entStatistics.E設置pty();
    T本i成成e本edE正ents.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成輸入isto本icalE正entMana成e本 sh使tdown"));
}

bool UMin成輸入isto本icalE正entMana成e本::Re成iste本輸入isto本icalE正ent(const 軍Min成輸入isto本icalE正ent& E正ent)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("E正entMana成e本 not initialized"));
        本et使本n false;
    }

    if (E正ent.E正entID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("E正ent ID cannot be e設置pty"));
        本et使本n false;
    }

    // 檢查是否已存在
    if (E正entMap.Contains(E正ent.E正entID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("E正ent %s al本eady 本e成iste本ed"), *E正ent.E正entID);
        本et使本n false;
    }

    // 添加到註冊列表
    Re成iste本edE正ents.Add(E正ent);
    E正entMap.Add(E正ent.E正entID, E正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical e正ent 本e成iste本ed: %s - %s"), 
        *E正ent.E正entID, *E正ent.E正ent的a設置e);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalE正entMana成e本::Un本e成iste本輸入isto本icalE正ent(const 軍St本in成& E正entID)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    if (!E正entMap.Contains(E正entID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("E正ent %s not fo使nd"), *E正entID);
        本et使本n false;
    }

    // 從註冊列表移除
    fo本 (int32 i = 0; i < Re成iste本edE正ents.的使設置(); ++i)
    {
        if (Re成iste本edE正ents[i].E正entID == E正entID)
        {
            Re成iste本edE正ents.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    E正entMap.Re設置o正e(E正entID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical e正ent 使n本e成iste本ed: %s"), *E正entID);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalE正entMana成e本::CheckE正entT本i成成e本Conditions(const 軍St本in成& E正entID, const 軍E正entT本i成成e本Context& Context)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    const 軍Min成輸入isto本icalE正ent* E正ent = E正entMap.軍ind(E正entID);
    if (!E正ent)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("E正ent %s not fo使nd"), *E正entID);
        本et使本n false;
    }

    // 檢查是否已經觸發過
    if (T本i成成e本edE正ents.Contains(E正entID) && E正ent->bIsC本iticalE正ent)
    {
        本et使本n false;
    }

    // 檢查前置事件
    fo本 (const 軍St本in成& P本e本eq使isiteE正ent : E正ent->P本e本eq使isiteE正ents)
    {
        if (!T本i成成e本edE正ents.Contains(P本e本eq使isiteE正ent))
        {
            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("E正ent %s p本e本eq使isite %s not t本i成成e本ed"), 
                *E正entID, *P本e本eq使isiteE正ent);
            本et使本n false;
        }
    }

    // 檢查所有觸發條件
    fo本 (const 軍St本in成& Condition : E正ent->T本i成成e本Conditions)
    {
        if (!E正al使ateCondition(Condition, Context))
        {
            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("E正ent %s condition not 設置et: %s"), *E正entID, *Condition);
            本et使本n false;
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正ent %s conditions satisfied"), *E正entID);
    本et使本n t本使e;
}

軍E正entExec使tionRes使lt UMin成輸入isto本icalE正entMana成e本::T本i成成e本輸入isto本icalE正ent(const 軍St本in成& E正entID, const 軍E正entT本i成成e本Context& Context)
{
    軍E正entExec使tionRes使lt Res使lt;
    Res使lt.E正entID = E正entID;
    Res使lt.Exec使tionTi設置e = Context.C使本本entGa設置eTi設置e;

    if (!bIsInitialized)
    {
        Res使lt.E本本o本Messa成e = TEXT("E正entMana成e本 not initialized");
        本et使本n Res使lt;
    }

    const 軍Min成輸入isto本icalE正ent* E正ent = E正entMap.軍ind(E正entID);
    if (!E正ent)
    {
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("E正ent %s not fo使nd"), *E正entID);
        本et使本n Res使lt;
    }

    // 檢查觸發條件
    if (!CheckE正entT本i成成e本Conditions(E正entID, Context))
    {
        Res使lt.E本本o本Messa成e = TEXT("T本i成成e本 conditions not satisfied");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本i成成e本in成 histo本ical e正ent: %s - %s"), 
        *E正entID, *E正ent->E正ent的a設置e);

    // 執行事件後果
    fo本 (const 軍St本in成& Conseq使ence : E正ent->E正entConseq使ences)
    {
        if (Exec使teE正entConseq使ence(Conseq使ence, E正entID))
        {
            Res使lt.Exec使tedConseq使ences.Add(Conseq使ence);
        }
        else
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ailed to exec使te conseq使ence: %s"), *Conseq使ence);
        }
    }

    // 應用事件影響
    ApplyE正entI設置pact(*E正ent);

    // 添加到已觸發列表
    T本i成成e本edE正ents.Add(E正entID);

    // 更新統計
    UpdateE正entStatistics(E正entID);

    Res使lt.bS使ccess = t本使e;

    // 記錄事件日誌
    Lo成E正ent(E正entID, 軍St本in成::P本intf(TEXT("E正ent t本i成成e本ed s使ccessf使lly: %s"), *E正ent->E正ent的a設置e));

    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical e正ent t本i成成e本ed s使ccessf使lly: %s"), *E正entID);
    
    本et使本n Res使lt;
}

軍Min成輸入isto本icalE正ent UMin成輸入isto本icalE正entMana成e本::Get輸入isto本icalE正ent(const 軍St本in成& E正entID) const
{
    const 軍Min成輸入isto本icalE正ent* E正ent = E正entMap.軍ind(E正entID);
    本et使本n E正ent 基本 *E正ent : 軍Min成輸入isto本icalE正ent();
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正entMana成e本::GetAll輸入isto本icalE正ents() const
{
    本et使本n Re成iste本edE正ents;
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正entMana成e本::GetE正entsByE本a(ERep使blicE本a E本a) const
{
    TA本本ay<軍Min成輸入isto本icalE正ent> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalE正ent& E正ent : Re成iste本edE正ents)
    {
        if (E正ent.E本a == E本a)
        {
            Res使lt.Add(E正ent);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正entMana成e本::GetE正entsByType(E輸入isto本icalE正entType E正entType) const
{
    TA本本ay<軍Min成輸入isto本icalE正ent> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalE正ent& E正ent : Re成iste本edE正ents)
    {
        if (E正ent.E正entType == E正entType)
        {
            Res使lt.Add(E正ent);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正entMana成e本::GetC本iticalE正ents() const
{
    TA本本ay<軍Min成輸入isto本icalE正ent> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalE正ent& E正ent : Re成iste本edE正ents)
    {
        if (E正ent.bIsC本iticalE正ent)
        {
            Res使lt.Add(E正ent);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍St本in成> UMin成輸入isto本icalE正entMana成e本::GetT本i成成e本ableE正ents(const 軍E正entT本i成成e本Context& Context) const
{
    TA本本ay<軍St本in成> T本i成成e本ableE正ents;
    
    fo本 (const 軍Min成輸入isto本icalE正ent& E正ent : Re成iste本edE正ents)
    {
        if (CheckE正entT本i成成e本Conditions(E正ent.E正entID, Context))
        {
            T本i成成e本ableE正ents.Add(E正ent.E正entID);
        }
    }
    
    本et使本n T本i成成e本ableE正ents;
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正entMana成e本::GetE正entChain(const 軍St本in成& E正entID) const
{
    TA本本ay<軍Min成輸入isto本icalE正ent> E正entChain;
    
    const 軍Min成輸入isto本icalE正ent* E正ent = E正entMap.軍ind(E正entID);
    if (!E正ent)
    {
        本et使本n E正entChain;
    }

    // 添加當前事件
    E正entChain.Add(*E正ent);
    
    // 遞歸添加後續事件
    TA本本ay<軍St本in成> P本ocessedE正ents;
    TA本本ay<軍St本in成> E正entsToP本ocess = E正ent->軍ollowUpE正ents;
    
    while (E正entsToP本ocess.的使設置() > 0)
    {
        軍St本in成 的extE正entID = E正entsToP本ocess[0];
        E正entsToP本ocess.Re設置o正eAt(0);
        
        if (P本ocessedE正ents.Contains(的extE正entID))
        {
            contin使e;
        }
        
        const 軍Min成輸入isto本icalE正ent* 的extE正ent = E正entMap.軍ind(的extE正entID);
        if (的extE正ent)
        {
            E正entChain.Add(*的extE正ent);
            P本ocessedE正ents.Add(的extE正entID);
            
            // 添加後續事件到處理隊列
            fo本 (const 軍St本in成& 軍ollowUpE正ent : 的extE正ent->軍ollowUpE正ents)
            {
                if (!P本ocessedE正ents.Contains(軍ollowUpE正ent))
                {
                    E正entsToP本ocess.Add(軍ollowUpE正ent);
                }
            }
        }
    }
    
    本et使本n E正entChain;
}

正oid UMin成輸入isto本icalE正entMana成e本::UpdateGa設置eContext(const 軍E正entT本i成成e本Context& Context)
{
    C使本本entContext = Context;
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Ga設置e context 使pdated - E本a: %d, Yea本: %d"), 
        static下cast<int32>(Context.C使本本entE本a), Context.C使本本entYea本);
}

軍E正entT本i成成e本Context UMin成輸入isto本icalE正entMana成e本::GetC使本本entGa設置eContext() const
{
    本et使本n C使本本entContext;
}

正oid UMin成輸入isto本icalE正entMana成e本::SetGa設置eTi設置e(float Ga設置eTi設置e)
{
    C使本本entContext.C使本本entGa設置eTi設置e = Ga設置eTi設置e;
    
    // 根據遊戲時間更新年份和時期
    int32 Ga設置eYea本s = 軍Math::軍loo本ToInt(Ga設置eTi設置e / (365.0f * 24.0f * 60.0f * 60.0f)); // 假設1年 = 365天
    
    C使本本entContext.C使本本entYea本 = 1912 + Ga設置eYea本s;
    
    if (C使本本entContext.C使本本entYea本 <= 1928)
    {
        C使本本entContext.C使本本entE本a = ERep使blicE本a::Ea本lyRep使blic;
    }
    else if (C使本本entContext.C使本本entYea本 <= 1937)
    {
        C使本本entContext.C使本本entE本a = ERep使blicE本a::的an大in成Decade;
    }
    else if (C使本本entContext.C使本本entYea本 <= 1945)
    {
        C使本本entContext.C使本本entE本a = ERep使blicE本a::基本a本OfResistance;
    }
    else
    {
        C使本本entContext.C使本本entE本a = ERep使blicE本a::Ci正il基本a本;
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Ga設置e ti設置e 使pdated: %.1f, Yea本: %d, E本a: %d"), 
        Ga設置eTi設置e, C使本本entContext.C使本本entYea本, static下cast<int32>(C使本本entContext.C使本本entE本a));
}

正oid UMin成輸入isto本icalE正entMana成e本::SetRep使blicE本a(ERep使blicE本a E本a)
{
    C使本本entContext.C使本本entE本a = E本a;
    
    // 根據時期設置對應年份
    switch (E本a)
    {
    case ERep使blicE本a::Ea本lyRep使blic:
        C使本本entContext.C使本本entYea本 = 1912;
        b本eak;
    case ERep使blicE本a::的an大in成Decade:
        C使本本entContext.C使本本entYea本 = 1928;
        b本eak;
    case ERep使blicE本a::基本a本OfResistance:
        C使本本entContext.C使本本entYea本 = 1937;
        b本eak;
    case ERep使blicE本a::Ci正il基本a本:
        C使本本entContext.C使本本entYea本 = 1945;
        b本eak;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rep使blic e本a set to: %d, Yea本: %d"), 
        static下cast<int32>(E本a), C使本本entContext.C使本本entYea本);
}

正oid UMin成輸入isto本icalE正entMana成e本::AddPlaye本Decision(const 軍St本in成& DecisionID, const 軍St本in成& DecisionVal使e)
{
    C使本本entContext.Playe本Decisions.Add(DecisionID, DecisionVal使e);
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Playe本 decision added: %s = %s"), *DecisionID, *DecisionVal使e);
}

TMap<軍St本in成, int32> UMin成輸入isto本icalE正entMana成e本::GetE正entStatistics() const
{
    本et使本n E正entStatistics;
}

正oid UMin成輸入isto本icalE正entMana成e本::ResetAllE正ents()
{
    T本i成成e本edE正ents.E設置pty();
    E正entStatistics.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All e正ents 本eset"));
}

軍E正entExec使tionRes使lt UMin成輸入isto本icalE正entMana成e本::軍o本ceT本i成成e本E正ent(const 軍St本in成& E正entID)
{
    本et使本n T本i成成e本輸入isto本icalE正ent(E正entID, C使本本entContext);
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正entMana成e本::GetReco設置設置endedE正ents(const 軍E正entT本i成成e本Context& Context) const
{
    TA本本ay<軍Min成輸入isto本icalE正ent> Reco設置設置endedE正ents;
    
    fo本 (const 軍Min成輸入isto本icalE正ent& E正ent : Re成iste本edE正ents)
    {
        // 跳過已觸發的關鍵事件
        if (E正ent.bIsC本iticalE正ent && T本i成成e本edE正ents.Contains(E正ent.E正entID))
        {
            contin使e;
        }
        
        // 計算事件影響分數
        float I設置pactSco本e = Calc使lateE正entI設置pact(E正ent);
        
        // 檢查基本條件
        bool bBasicConditionsMet = t本使e;
        fo本 (const 軍St本in成& Condition : E正ent.T本i成成e本Conditions)
        {
            if (!E正al使ateBasicCondition(Condition, Context))
            {
                bBasicConditionsMet = false;
                b本eak;
            }
        }
        
        if (bBasicConditionsMet && I設置pactSco本e > 0.5f)
        {
            Reco設置設置endedE正ents.Add(E正ent);
        }
    }
    
    // 按影響分數排序
    Reco設置設置endedE正ents.So本t([this](const 軍Min成輸入isto本icalE正ent& A, const 軍Min成輸入isto本icalE正ent& B)
    {
        本et使本n Calc使lateE正entI設置pact(A) > Calc使lateE正entI設置pact(B);
    });
    
    本et使本n Reco設置設置endedE正ents;
}

// === 內部方法實現 ===

正oid UMin成輸入isto本icalE正entMana成e本::Initialize輸入isto本icalE正entLib本a本y()
{
    C本eateEa本lyRep使blicE正ents();
    C本eate的an大in成DecadeE正ents();
    C本eate基本a本OfResistanceE正ents();
    C本eateCi正il基本a本E正ents();
}

正oid UMin成輸入isto本icalE正entMana成e本::C本eateEa本lyRep使blicE正ents()
{
    // 辛亥革命成功事件
    軍Min成輸入isto本icalE正ent XinhaiRe正ol使tion;
    XinhaiRe正ol使tion.E正entID = TEXT("xinhai下本e正ol使tion下1912");
    XinhaiRe正ol使tion.E正ent的a設置e = TEXT("辛亥革命成功");
    XinhaiRe正ol使tion.E正entDesc本iption = TEXT("1912年，孫中山領導的辛亥革命成功，中華民國成立");
    XinhaiRe正ol使tion.E本a = ERep使blicE本a::Ea本lyRep使blic;
    XinhaiRe正ol使tion.輸入isto本icalYea本 = 1912;
    XinhaiRe正ol使tion.E正entType = E輸入isto本icalE正entType::Re正ol使tiona本y;
    XinhaiRe正ol使tion.I設置pactScope = EE正entI設置pactScope::的ational;
    XinhaiRe正ol使tion.P本io本ity = 100;
    XinhaiRe正ol使tion.bIsC本iticalE正ent = t本使e;
    XinhaiRe正ol使tion.T本i成成e本Conditions.Add(TEXT("成a設置e下ti設置e >= 0"));
    XinhaiRe正ol使tion.E正entConseq使ences.Add(TEXT("set下本ep使blic下e本a:ea本ly下本ep使blic"));
    XinhaiRe正ol使tion.E正entConseq使ences.Add(TEXT("add下national下p本esti成e:+50"));
    XinhaiRe正ol使tion.Related軍i成使本es.Add(TEXT("s使n下yat下sen"));
    XinhaiRe正ol使tion.RelatedLocations.Add(TEXT("nan大in成"));
    XinhaiRe正ol使tion.E正entTa成s.Add(TEXT("fo使ndin成"));
    XinhaiRe正ol使tion.E正entTa成s.Add(TEXT("本e正ol使tion"));
    
    Re成iste本輸入isto本icalE正ent(XinhaiRe正ol使tion);

    // 二次革命事件
    軍Min成輸入isto本icalE正ent SecondRe正ol使tion;
    SecondRe正ol使tion.E正entID = TEXT("second下本e正ol使tion下1913");
    SecondRe正ol使tion.E正ent的a設置e = TEXT("二次革命");
    SecondRe正ol使tion.E正entDesc本iption = TEXT("1913年，孫中山發動二次革命，反對袁世凱獨裁");
    SecondRe正ol使tion.E本a = ERep使blicE本a::Ea本lyRep使blic;
    SecondRe正ol使tion.輸入isto本icalYea本 = 1913;
    SecondRe正ol使tion.E正entType = E輸入isto本icalE正entType::Political;
    SecondRe正ol使tion.I設置pactScope = EE正entI設置pactScope::的ational;
    SecondRe正ol使tion.P本io本ity = 80;
    SecondRe正ol使tion.bIsC本iticalE正ent = t本使e;
    SecondRe正ol使tion.P本e本eq使isiteE正ents.Add(TEXT("xinhai下本e正ol使tion下1912"));
    SecondRe正ol使tion.T本i成成e本Conditions.Add(TEXT("成a設置e下ti設置e >= 8760")); // 1年後
    SecondRe正ol使tion.T本i成成e本Conditions.Add(TEXT("playe本下decision:oppose下y使an下shikai"));
    SecondRe正ol使tion.E正entConseq使ences.Add(TEXT("t本i成成e本下ci正il下使n本est"));
    SecondRe正ol使tion.E正entConseq使ences.Add(TEXT("add下設置ilita本y下tension:+30"));
    SecondRe正ol使tion.Related軍i成使本es.Add(TEXT("s使n下yat下sen"));
    SecondRe正ol使tion.Related軍i成使本es.Add(TEXT("y使an下shikai"));
    SecondRe正ol使tion.E正entTa成s.Add(TEXT("conflict"));
    SecondRe正ol使tion.E正entTa成s.Add(TEXT("political"));
    
    Re成iste本輸入isto本icalE正ent(SecondRe正ol使tion);
}

正oid UMin成輸入isto本icalE正entMana成e本::C本eate的an大in成DecadeE正ents()
{
    // 北伐統一事件
    軍Min成輸入isto本icalE正ent 的o本the本nExpedition;
    的o本the本nExpedition.E正entID = TEXT("no本the本n下expedition下1926");
    的o本the本nExpedition.E正ent的a設置e = TEXT("北伐統一");
    的o本the本nExpedition.E正entDesc本iption = TEXT("1926年，國民革命軍開始北伐，統一中國");
    的o本the本nExpedition.E本a = ERep使blicE本a::的an大in成Decade;
    的o本the本nExpedition.輸入isto本icalYea本 = 1926;
    的o本the本nExpedition.E正entType = E輸入isto本icalE正entType::Milita本y;
    的o本the本nExpedition.I設置pactScope = EE正entI設置pactScope::的ational;
    的o本the本nExpedition.P本io本ity = 90;
    的o本the本nExpedition.bIsC本iticalE正ent = t本使e;
    的o本the本nExpedition.T本i成成e本Conditions.Add(TEXT("成a設置e下ti設置e >= 499320")); // 1926年
    的o本the本nExpedition.T本i成成e本Conditions.Add(TEXT("設置ilita本y下st本en成th >= 50"));
    的o本the本nExpedition.E正entConseq使ences.Add(TEXT("set下本ep使blic下e本a:nan大in成下decade"));
    的o本the本nExpedition.E正entConseq使ences.Add(TEXT("使nify下china"));
    的o本the本nExpedition.E正entConseq使ences.Add(TEXT("add下national下p本esti成e:+100"));
    的o本the本nExpedition.Related軍i成使本es.Add(TEXT("chian成下kai下shek"));
    的o本the本nExpedition.E正entTa成s.Add(TEXT("使nification"));
    的o本the本nExpedition.E正entTa成s.Add(TEXT("設置ilita本y"));
    
    Re成iste本輸入isto本icalE正ent(的o本the本nExpedition);

    // 九一八事變事件
    軍Min成輸入isto本icalE正ent M使kdenIncident;
    M使kdenIncident.E正entID = TEXT("設置使kden下incident下1931");
    M使kdenIncident.E正ent的a設置e = TEXT("九一八事變");
    M使kdenIncident.E正entDesc本iption = TEXT("1931年，日本發動九一八事變，侵占中國東北");
    M使kdenIncident.E本a = ERep使blicE本a::的an大in成Decade;
    M使kdenIncident.輸入isto本icalYea本 = 1931;
    M使kdenIncident.E正entType = E輸入isto本icalE正entType::Milita本y;
    M使kdenIncident.I設置pactScope = EE正entI設置pactScope::Inte本national;
    M使kdenIncident.P本io本ity = 95;
    M使kdenIncident.bIsC本iticalE正ent = t本使e;
    M使kdenIncident.T本i成成e本Conditions.Add(TEXT("成a設置e下ti設置e >= 622080")); // 1931年
    M使kdenIncident.T本i成成e本Conditions.Add(TEXT("大apan下a成成本ession >= 30"));
    M使kdenIncident.E正entConseq使ences.Add(TEXT("lose下te本本ito本y:設置anch使本ia"));
    M使kdenIncident.E正entConseq使ences.Add(TEXT("add下大apanese下th本eat:+50"));
    M使kdenIncident.E正entConseq使ences.Add(TEXT("t本i成成e本下anti下大apanese下senti設置ent"));
    M使kdenIncident.RelatedLocations.Add(TEXT("設置anch使本ia"));
    M使kdenIncident.E正entTa成s.Add(TEXT("in正asion"));
    M使kdenIncident.E正entTa成s.Add(TEXT("conflict"));
    
    Re成iste本輸入isto本icalE正ent(M使kdenIncident);
}

正oid UMin成輸入isto本icalE正entMana成e本::C本eate基本a本OfResistanceE正ents()
{
    // 七七事變事件
    軍Min成輸入isto本icalE正ent Ma本coPoloB本id成e;
    Ma本coPoloB本id成e.E正entID = TEXT("設置a本co下polo下b本id成e下1937");
    Ma本coPoloB本id成e.E正ent的a設置e = TEXT("七七事變");
    Ma本coPoloB本id成e.E正entDesc本iption = TEXT("1937年7月7日，盧溝橋事變爆發，全面抗戰開始");
    Ma本coPoloB本id成e.E本a = ERep使blicE本a::基本a本OfResistance;
    Ma本coPoloB本id成e.輸入isto本icalYea本 = 1937;
    Ma本coPoloB本id成e.E正entType = E輸入isto本icalE正entType::Milita本y;
    Ma本coPoloB本id成e.I設置pactScope = EE正entI設置pactScope::Inte本national;
    Ma本coPoloB本id成e.P本io本ity = 100;
    Ma本coPoloB本id成e.bIsC本iticalE正ent = t本使e;
    Ma本coPoloB本id成e.P本e本eq使isiteE正ents.Add(TEXT("設置使kden下incident下1931"));
    Ma本coPoloB本id成e.T本i成成e本Conditions.Add(TEXT("成a設置e下ti設置e >= 788928")); // 1937年
    Ma本coPoloB本id成e.T本i成成e本Conditions.Add(TEXT("大apan下a成成本ession >= 70"));
    Ma本coPoloB本id成e.E正entConseq使ences.Add(TEXT("set下本ep使blic下e本a:wa本下of下本esistance"));
    Ma本coPoloB本id成e.E正entConseq使ences.Add(TEXT("sta本t下total下wa本"));
    Ma本coPoloB本id成e.E正entConseq使ences.Add(TEXT("使nite下all下factions"));
    Ma本coPoloB本id成e.E正entConseq使ences.Add(TEXT("add下national下使nity:+80"));
    Ma本coPoloB本id成e.RelatedLocations.Add(TEXT("bei大in成"));
    Ma本coPoloB本id成e.RelatedLocations.Add(TEXT("設置a本co下polo下b本id成e"));
    Ma本coPoloB本id成e.E正entTa成s.Add(TEXT("wa本"));
    Ma本coPoloB本id成e.E正entTa成s.Add(TEXT("本esistance"));
    
    Re成iste本輸入isto本icalE正ent(Ma本coPoloB本id成e);

    // 南京大屠殺事件
    軍Min成輸入isto本icalE正ent 的an大in成Massac本e;
    的an大in成Massac本e.E正entID = TEXT("nan大in成下設置assac本e下1937");
    的an大in成Massac本e.E正ent的a設置e = TEXT("南京大屠殺");
    的an大in成Massac本e.E正entDesc本iption = TEXT("1937年12月，日軍在南京進行大屠殺");
    的an大in成Massac本e.E本a = ERep使blicE本a::基本a本OfResistance;
    的an大in成Massac本e.輸入isto本icalYea本 = 1937;
    的an大in成Massac本e.E正entType = E輸入isto本icalE正entType::Social;
    的an大in成Massac本e.I設置pactScope = EE正entI設置pactScope::的ational;
    的an大in成Massac本e.P本io本ity = 85;
    的an大in成Massac本e.bIsC本iticalE正ent = t本使e;
    的an大in成Massac本e.P本e本eq使isiteE正ents.Add(TEXT("設置a本co下polo下b本id成e下1937"));
    的an大in成Massac本e.T本i成成e本Conditions.Add(TEXT("成a設置e下ti設置e >= 795168")); // 1937年12月
    的an大in成Massac本e.T本i成成e本Conditions.Add(TEXT("nan大in成下occ使pied"));
    的an大in成Massac本e.E正entConseq使ences.Add(TEXT("add下national下t本a使設置a:+100"));
    的an大in成Massac本e.E正entConseq使ences.Add(TEXT("inc本ease下本esistance下dete本設置ination:+50"));
    的an大in成Massac本e.E正entConseq使ences.Add(TEXT("inte本national下conde設置nation"));
    的an大in成Massac本e.RelatedLocations.Add(TEXT("nan大in成"));
    的an大in成Massac本e.E正entTa成s.Add(TEXT("t本a成edy"));
    的an大in成Massac本e.E正entTa成s.Add(TEXT("at本ocity"));
    
    Re成iste本輸入isto本icalE正ent(的an大in成Massac本e);
}

正oid UMin成輸入isto本icalE正entMana成e本::C本eateCi正il基本a本E正ents()
{
    // 重慶談判事件
    軍Min成輸入isto本icalE正ent Chon成qin成的e成otiations;
    Chon成qin成的e成otiations.E正entID = TEXT("chon成qin成下ne成otiations下1945");
    Chon成qin成的e成otiations.E正ent的a設置e = TEXT("重慶談判");
    Chon成qin成的e成otiations.E正entDesc本iption = TEXT("1945年，國共兩黨在重慶進行和平談判");
    Chon成qin成的e成otiations.E本a = ERep使blicE本a::Ci正il基本a本;
    Chon成qin成的e成otiations.輸入isto本icalYea本 = 1945;
    Chon成qin成的e成otiations.E正entType = E輸入isto本icalE正entType::Political;
    Chon成qin成的e成otiations.I設置pactScope = EE正entI設置pactScope::的ational;
    Chon成qin成的e成otiations.P本io本ity = 80;
    Chon成qin成的e成otiations.bIsC本iticalE正ent = t本使e;
    Chon成qin成的e成otiations.P本e本eq使isiteE正ents.Add(TEXT("設置a本co下polo下b本id成e下1937"));
    Chon成qin成的e成otiations.T本i成成e本Conditions.Add(TEXT("成a設置e下ti設置e >= 1054080")); // 1945年
    Chon成qin成的e成otiations.T本i成成e本Conditions.Add(TEXT("大apan下defeated"));
    Chon成qin成的e成otiations.E正entConseq使ences.Add(TEXT("set下本ep使blic下e本a:ci正il下wa本"));
    Chon成qin成的e成otiations.E正entConseq使ences.Add(TEXT("sta本t下peace下ne成otiations"));
    Chon成qin成的e成otiations.E正entConseq使ences.Add(TEXT("inc本ease下political下tension:+40"));
    Chon成qin成的e成otiations.Related軍i成使本es.Add(TEXT("chian成下kai下shek"));
    Chon成qin成的e成otiations.Related軍i成使本es.Add(TEXT("設置ao下zedon成"));
    Chon成qin成的e成otiations.RelatedLocations.Add(TEXT("chon成qin成"));
    Chon成qin成的e成otiations.E正entTa成s.Add(TEXT("ne成otiation"));
    Chon成qin成的e成otiations.E正entTa成s.Add(TEXT("political"));
    
    Re成iste本輸入isto本icalE正ent(Chon成qin成的e成otiations);
}

// === 條件評估方法 ===

bool UMin成輸入isto本icalE正entMana成e本::E正al使ateCondition(const 軍St本in成& Condition, const 軍E正entT本i成成e本Context& Context) const
{
    TA本本ay<軍St本in成> Pa本ts = Pa本seConditionSt本in成(Condition);
    
    if (Pa本ts.的使設置() < 3)
    {
        本et使本n false;
    }
    
    軍St本in成 Key = Pa本ts[0];
    軍St本in成 Ope本ato本 = Pa本ts[1];
    軍St本in成 Val使e = Pa本ts[2];
    
    if (Key == TEXT("成a設置e下ti設置e"))
    {
        本et使本n Co設置pa本eVal使es(Context.C使本本entGa設置eTi設置e, Ope本ato本, 軍CSt本in成::Atof(*Val使e));
    }
    else if (Key == TEXT("playe本下decision"))
    {
        軍St本in成* DecisionVal使e = Context.Playe本Decisions.軍ind(Val使e);
        本et使本n DecisionVal使e != n使llpt本;
    }
    else if (Key == TEXT("設置ilita本y下st本en成th"))
    {
        float Milita本ySt本en成th = GetContextVal使e(TEXT("設置ilita本y下st本en成th"), Context);
        本et使本n Co設置pa本eVal使es(Milita本ySt本en成th, Ope本ato本, 軍CSt本in成::Atof(*Val使e));
    }
    
    本et使本n false;
}

bool UMin成輸入isto本icalE正entMana成e本::Exec使teE正entConseq使ence(const 軍St本in成& Conseq使ence, const 軍St本in成& E正entID)
{
    TA本本ay<軍St本in成> Pa本ts = Pa本seConditionSt本in成(Conseq使ence);
    
    if (Pa本ts.的使設置() < 2)
    {
        本et使本n false;
    }
    
    軍St本in成 Action = Pa本ts[0];
    軍St本in成 Pa本a設置ete本 = Pa本ts[1];
    
    if (Action == TEXT("set下本ep使blic下e本a"))
    {
        if (Pa本a設置ete本 == TEXT("ea本ly下本ep使blic"))
        {
            SetRep使blicE本a(ERep使blicE本a::Ea本lyRep使blic);
        }
        else if (Pa本a設置ete本 == TEXT("nan大in成下decade"))
        {
            SetRep使blicE本a(ERep使blicE本a::的an大in成Decade);
        }
        else if (Pa本a設置ete本 == TEXT("wa本下of下本esistance"))
        {
            SetRep使blicE本a(ERep使blicE本a::基本a本OfResistance);
        }
        else if (Pa本a設置ete本 == TEXT("ci正il下wa本"))
        {
            SetRep使blicE本a(ERep使blicE本a::Ci正il基本a本);
        }
        本et使本n t本使e;
    }
    else if (Action == TEXT("add下national下p本esti成e"))
    {
        // 這裡應該調用遊戲系統來添加聲望值
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added national p本esti成e: %s"), *Pa本a設置ete本);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成輸入isto本icalE正entMana成e本::ApplyE正entI設置pact(const 軍Min成輸入isto本icalE正ent& E正ent)
{
    // 根據事件影響範圍應用影響
    switch (E正ent.I設置pactScope)
    {
    case EE正entI設置pactScope::Local:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied local i設置pact fo本 e正ent: %s"), *E正ent.E正entID);
        b本eak;
    case EE正entI設置pactScope::Re成ional:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied 本e成ional i設置pact fo本 e正ent: %s"), *E正ent.E正entID);
        b本eak;
    case EE正entI設置pactScope::的ational:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied national i設置pact fo本 e正ent: %s"), *E正ent.E正entID);
        b本eak;
    case EE正entI設置pactScope::Inte本national:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied inte本national i設置pact fo本 e正ent: %s"), *E正ent.E正entID);
        b本eak;
    }
}

正oid UMin成輸入isto本icalE正entMana成e本::UpdateE正entStatistics(const 軍St本in成& E正entID)
{
    int32* Co使nt = E正entStatistics.軍ind(E正entID);
    if (Co使nt)
    {
        (*Co使nt)++;
    }
    else
    {
        E正entStatistics.Add(E正entID, 1);
    }
}

float UMin成輸入isto本icalE正entMana成e本::Calc使lateE正entI設置pact(const 軍Min成輸入isto本icalE正ent& E正ent) const
{
    float I設置pact = 0.0f;
    
    // 基於優先級
    I設置pact += E正ent.P本io本ity * 0.01f;
    
    // 基於影響範圍
    switch (E正ent.I設置pactScope)
    {
    case EE正entI設置pactScope::Local:
        I設置pact += 0.2f;
        b本eak;
    case EE正entI設置pactScope::Re成ional:
        I設置pact += 0.4f;
        b本eak;
    case EE正entI設置pactScope::的ational:
        I設置pact += 0.6f;
        b本eak;
    case EE正entI設置pactScope::Inte本national:
        I設置pact += 0.8f;
        b本eak;
    }
    
    // 關鍵事件額外加分
    if (E正ent.bIsC本iticalE正ent)
    {
        I設置pact += 0.3f;
    }
    
    本et使本n 軍Math::Cla設置p(I設置pact, 0.0f, 1.0f);
}

正oid UMin成輸入isto本icalE正entMana成e本::Lo成E正ent(const 軍St本in成& E正entID, const 軍St本in成& Messa成e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[E正ent %s] %s"), *E正entID, *Messa成e);
}

bool UMin成輸入isto本icalE正entMana成e本::Co設置pa本eVal使es(float Val使e1, const 軍St本in成& Ope本ato本, float Val使e2) const
{
    if (Ope本ato本 == TEXT(">"))
        本et使本n Val使e1 > Val使e2;
    else if (Ope本ato本 == TEXT("<"))
        本et使本n Val使e1 < Val使e2;
    else if (Ope本ato本 == TEXT("=="))
        本et使本n 軍Math::Is的ea本lyEq使al(Val使e1, Val使e2);
    else if (Ope本ato本 == TEXT(">="))
        本et使本n Val使e1 >= Val使e2;
    else if (Ope本ato本 == TEXT("<="))
        本et使本n Val使e1 <= Val使e2;
    
    本et使本n false;
}

TA本本ay<軍St本in成> UMin成輸入isto本icalE正entMana成e本::Pa本seConditionSt本in成(const 軍St本in成& Condition) const
{
    TA本本ay<軍St本in成> Pa本ts;
    Condition.Pa本seIntoA本本ay(Pa本ts, TEXT(" "));
    本et使本n Pa本ts;
}

float UMin成輸入isto本icalE正entMana成e本::GetContextVal使e(const 軍St本in成& Key, const 軍E正entT本i成成e本Context& Context) const
{
    // 簡化實作，實際應該從遊戲系統獲取
    if (Key == TEXT("設置ilita本y下st本en成th"))
    {
        本et使本n 50.0f; // 示例值
    }
    else if (Key == TEXT("大apan下a成成本ession"))
    {
        本et使本n 30.0f; // 示例值
    }
    
    本et使本n 0.0f;
}

bool UMin成輸入isto本icalE正entMana成e本::E正al使ateBasicCondition(const 軍St本in成& Condition, const 軍E正entT本i成成e本Context& Context) const
{
    本et使本n E正al使ateCondition(Condition, Context);
}
