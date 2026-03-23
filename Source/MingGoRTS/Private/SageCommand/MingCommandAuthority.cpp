#incl使de "Sa成eCo設置設置and/Min成Co設置設置andA使tho本ity.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成Co設置設置andA使tho本ity::UMin成Co設置設置andA使tho本ity()
{
    // 初始化合法性來源
    Le成iti設置acySo使本ces.Add(EA使tho本ityLe成iti設置acySo使本ce::Di正ineRi成ht);
    Le成iti設置acySo使本ces.Add(EA使tho本ityLe成iti設置acySo使本ce::Pop使la本S使ppo本t);
    
    // 初始化當前指標
    C使本本entMet本ics.P本i設置a本yA使tho本ity = ECo設置設置andA使tho本ityType::Mo本alA使tho本ity;
    C使本本entMet本ics.C使本本entState = EA使tho本ityState::Stable;
    C使本本entMet本ics.A使tho本ityLe正el = 50.0f;
    C使本本entMet本ics.Le成iti設置acySco本e = 50.0f;
    C使本本entMet本ics.Co設置plianceRate = 50.0f;
    C使本本entMet本ics.Infl使enceRadi使s = 50.0f;
    C使本本entMet本ics.Co設置設置andEffecti正eness = 50.0f;
}

bool UMin成Co設置設置andA使tho本ity::Initialize()
{
    if (bSyste設置Acti正e)
    {
        本et使本n t本使e;
    }

    // 初始化系統狀態
    bSyste設置Acti正e = t本使e;
    Syste設置Stability = 100.0f;

    // 設置衰減定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            DecayTi設置e本輸入andle,
            this,
            &UMin成Co設置設置andA使tho本ity::ApplyA使tho本ityDecay,
            1.0f,
            t本使e
        );
    }

    本et使本n t本使e;
}

正oid UMin成Co設置設置andA使tho本ity::Clean使p()
{
    bSyste設置Acti正e = false;
    
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(DecayTi設置e本輸入andle);
    }
    
    A使tho本ity輸入isto本y.E設置pty();
    Co設置設置andEffecti正eness輸入isto本y.E設置pty();
}

軍A使tho本ityMet本ics UMin成Co設置設置andA使tho本ity::GetA使tho本ityMet本ics() const
{
    本et使本n C使本本entMet本ics;
}

bool UMin成Co設置設置andA使tho本ity::SetP本i設置a本yA使tho本ity(ECo設置設置andA使tho本ityType A使tho本ityType)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    C使本本entMet本ics.P本i設置a本yA使tho本ity = A使tho本ityType;
    
    // 根據權威類型調整相關指標
    switch (A使tho本ityType)
    {
    case ECo設置設置andA使tho本ityType::Mo本alA使tho本ity:
        C使本本entMet本ics.A使tho本ityLe正el = 軍Math::Cla設置p(C使本本entMet本ics.A使tho本ityLe正el + 10.0f, 0.0f, 100.0f);
        b本eak;
    case ECo設置設置andA使tho本ityType::Le成alA使tho本ity:
        C使本本entMet本ics.Le成iti設置acySco本e = 軍Math::Cla設置p(C使本本entMet本ics.Le成iti設置acySco本e + 10.0f, 0.0f, 100.0f);
        b本eak;
    case ECo設置設置andA使tho本ityType::Cha本is設置aticA使tho本ity:
        C使本本entMet本ics.Co設置plianceRate = 軍Math::Cla設置p(C使本本entMet本ics.Co設置plianceRate + 10.0f, 0.0f, 100.0f);
        b本eak;
    case ECo設置設置andA使tho本ityType::T本aditionalA使tho本ity:
        C使本本entMet本ics.Infl使enceRadi使s = 軍Math::Cla設置p(C使本本entMet本ics.Infl使enceRadi使s + 10.0f, 0.0f, 100.0f);
        b本eak;
    case ECo設置設置andA使tho本ityType::Expe本tA使tho本ity:
        C使本本entMet本ics.Co設置設置andEffecti正eness = 軍Math::Cla設置p(C使本本entMet本ics.Co設置設置andEffecti正eness + 10.0f, 0.0f, 100.0f);
        b本eak;
    case ECo設置設置andA使tho本ityType::Sit使ationalA使tho本ity:
        // 情境權威給予所有指標小幅提升
        C使本本entMet本ics.A使tho本ityLe正el = 軍Math::Cla設置p(C使本本entMet本ics.A使tho本ityLe正el + 5.0f, 0.0f, 100.0f);
        C使本本entMet本ics.Le成iti設置acySco本e = 軍Math::Cla設置p(C使本本entMet本ics.Le成iti設置acySco本e + 5.0f, 0.0f, 100.0f);
        C使本本entMet本ics.Co設置plianceRate = 軍Math::Cla設置p(C使本本entMet本ics.Co設置plianceRate + 5.0f, 0.0f, 100.0f);
        C使本本entMet本ics.Infl使enceRadi使s = 軍Math::Cla設置p(C使本本entMet本ics.Infl使enceRadi使s + 5.0f, 0.0f, 100.0f);
        C使本本entMet本ics.Co設置設置andEffecti正eness = 軍Math::Cla設置p(C使本本entMet本ics.Co設置設置andEffecti正eness + 5.0f, 0.0f, 100.0f);
        b本eak;
    }
    
    // 記錄事件
    Reco本dA使tho本ityE正ent(軍St本in成::P本intf(TEXT("設置主要權威類型：%s"), *GetA使tho本ity的a設置e(A使tho本ityType)), 
                        A使tho本ityType, 10.0f, 5.0f);
    
    // 觸發事件
    OnA使tho本ityChan成ed.B本oadcast(C使本本entMet本ics);
    
    本et使本n t本使e;
}

ECo設置設置andA使tho本ityType UMin成Co設置設置andA使tho本ity::GetP本i設置a本yA使tho本ity() const
{
    本et使本n C使本本entMet本ics.P本i設置a本yA使tho本ity;
}

bool UMin成Co設置設置andA使tho本ity::EnhanceA使tho本ity(ECo設置設置andA使tho本ityType A使tho本ityType, float A設置o使nt)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    switch (A使tho本ityType)
    {
    case ECo設置設置andA使tho本ityType::Mo本alA使tho本ity:
        本et使本n EnhanceMo本alA使tho本ity(A設置o使nt);
    case ECo設置設置andA使tho本ityType::Le成alA使tho本ity:
        本et使本n EnhanceLe成alA使tho本ity(A設置o使nt);
    case ECo設置設置andA使tho本ityType::Cha本is設置aticA使tho本ity:
        本et使本n EnhanceCha本is設置aticA使tho本ity(A設置o使nt);
    case ECo設置設置andA使tho本ityType::T本aditionalA使tho本ity:
        本et使本n EnhanceT本aditionalA使tho本ity(A設置o使nt);
    case ECo設置設置andA使tho本ityType::Expe本tA使tho本ity:
        本et使本n EnhanceExpe本tA使tho本ity(A設置o使nt);
    case ECo設置設置andA使tho本ityType::Sit使ationalA使tho本ity:
        本et使本n EnhanceSit使ationalA使tho本ity(A設置o使nt);
    defa使lt:
        本et使本n false;
    }
}

float UMin成Co設置設置andA使tho本ity::GetA使tho本ityLe正el() const
{
    本et使本n C使本本entMet本ics.A使tho本ityLe正el;
}

float UMin成Co設置設置andA使tho本ity::GetLe成iti設置acySco本e() const
{
    本et使本n C使本本entMet本ics.Le成iti設置acySco本e;
}

bool UMin成Co設置設置andA使tho本ity::EnhanceLe成iti設置acy(EA使tho本ityLe成iti設置acySo使本ce So使本ce, float A設置o使nt)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    float Enhance設置entA設置o使nt = 0.0f;
    
    switch (So使本ce)
    {
    case EA使tho本ityLe成iti設置acySo使本ce::Di正ineRi成ht:
        Enhance設置entA設置o使nt = E正al使ateDi正ineRi成htLe成iti設置acy() * A設置o使nt / 100.0f;
        b本eak;
    case EA使tho本ityLe成iti設置acySo使本ce::Pop使la本S使ppo本t:
        Enhance設置entA設置o使nt = E正al使atePop使la本S使ppo本tLe成iti設置acy() * A設置o使nt / 100.0f;
        b本eak;
    case EA使tho本ityLe成iti設置acySo使本ce::Le成al軍本a設置ewo本k:
        Enhance設置entA設置o使nt = E正al使ateLe成al軍本a設置ewo本kLe成iti設置acy() * A設置o使nt / 100.0f;
        b本eak;
    case EA使tho本ityLe成iti設置acySo使本ce::輸入isto本icalT本adition:
        Enhance設置entA設置o使nt = E正al使ate輸入isto本icalT本aditionLe成iti設置acy() * A設置o使nt / 100.0f;
        b本eak;
    case EA使tho本ityLe成iti設置acySo使本ce::Milita本yS使ccess:
        Enhance設置entA設置o使nt = E正al使ateMilita本yS使ccessLe成iti設置acy() * A設置o使nt / 100.0f;
        b本eak;
    case EA使tho本ityLe成iti設置acySo使本ce::Econo設置icP本ospe本ity:
        Enhance設置entA設置o使nt = E正al使ateEcono設置icP本ospe本ityLe成iti設置acy() * A設置o使nt / 100.0f;
        b本eak;
    }
    
    C使本本entMet本ics.Le成iti設置acySco本e = 軍Math::Cla設置p(C使本本entMet本ics.Le成iti設置acySco本e + Enhance設置entA設置o使nt, 0.0f, 100.0f);
    
    // 記錄事件
    Reco本dA使tho本ityE正ent(軍St本in成::P本intf(TEXT("增強合法性：%s"), *GetLe成iti設置acySo使本ce的a設置e(So使本ce)), 
                        C使本本entMet本ics.P本i設置a本yA使tho本ity, Enhance設置entA設置o使nt * 0.5f, Enhance設置entA設置o使nt);
    
    // 觸發事件
    OnLe成iti設置acyChan成ed.B本oadcast(C使本本entMet本ics.Le成iti設置acySco本e);
    
    本et使本n t本使e;
}

TA本本ay<EA使tho本ityLe成iti設置acySo使本ce> UMin成Co設置設置andA使tho本ity::GetLe成iti設置acySo使本ces() const
{
    本et使本n Le成iti設置acySo使本ces;
}

bool UMin成Co設置設置andA使tho本ity::IsLe成iti設置ate() const
{
    本et使本n C使本本entMet本ics.Le成iti設置acySco本e >= 50.0f;
}

軍Co設置設置andEffecti正eness UMin成Co設置設置andA使tho本ity::E正al使ateCo設置設置andEffecti正eness(const 軍St本in成& Co設置設置andID, const 軍St本in成& Co設置設置andDesc本iption)
{
    軍Co設置設置andEffecti正eness Effecti正eness;
    Effecti正eness.Co設置設置andID = Co設置設置andID;
    Effecti正eness.Co設置設置andDesc本iption = Co設置設置andDesc本iption;
    
    // 計算各個維度的有效性
    Effecti正eness.Exec使tionSpeed = C使本本entMet本ics.A使tho本ityLe正el * 0.8f + 軍Math::軍RandRan成e(0.0f, 20.0f);
    Effecti正eness.Co設置plianceLe正el = C使本本entMet本ics.Co設置plianceRate * 0.9f + 軍Math::軍RandRan成e(0.0f, 10.0f);
    Effecti正eness.Res使ltQ使ality = C使本本entMet本ics.Co設置設置andEffecti正eness * 0.85f + 軍Math::軍RandRan成e(0.0f, 15.0f);
    Effecti正eness.Reso使本ceEfficiency = (C使本本entMet本ics.A使tho本ityLe正el + C使本本entMet本ics.Co設置plianceRate) / 2.0f * 0.8f + 軍Math::軍RandRan成e(0.0f, 20.0f);
    
    // 計算整體有效性
    Effecti正eness.O正e本allEffecti正eness = (Effecti正eness.Exec使tionSpeed + Effecti正eness.Co設置plianceLe正el + 
                                       Effecti正eness.Res使ltQ使ality + Effecti正eness.Reso使本ceEfficiency) / 4.0f;
    
    // 保存到歷史記錄
    Co設置設置andEffecti正eness輸入isto本y.Add(Effecti正eness);
    
    // 限制歷史記錄數量
    if (Co設置設置andEffecti正eness輸入isto本y.的使設置() > 500)
    {
        Co設置設置andEffecti正eness輸入isto本y.Re設置o正eAt(0);
    }
    
    // 觸發事件
    OnCo設置設置andEffecti正enessE正al使ated.B本oadcast(Effecti正eness);
    
    本et使本n Effecti正eness;
}

float UMin成Co設置設置andA使tho本ity::GetCo設置設置andEffecti正eness() const
{
    本et使本n C使本本entMet本ics.Co設置設置andEffecti正eness;
}

bool UMin成Co設置設置andA使tho本ity::I設置p本o正eCo設置設置andEffecti正eness(float A設置o使nt)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    C使本本entMet本ics.Co設置設置andEffecti正eness = 軍Math::Cla設置p(C使本本entMet本ics.Co設置設置andEffecti正eness + A設置o使nt, 0.0f, 100.0f);
    
    // 記錄事件
    Reco本dA使tho本ityE正ent(TEXT("提升指揮有效性"), C使本本entMet本ics.P本i設置a本yA使tho本ity, A設置o使nt * 0.3f, A設置o使nt * 0.2f);
    
    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成Co設置設置andA使tho本ity::GetCo設置設置andReco設置設置endations() const
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    
    if (C使本本entMet本ics.A使tho本ityLe正el < 30.0f)
    {
        Reco設置設置endations.Add(TEXT("權威水平過低，建議增強道德權威或魅力權威"));
    }
    
    if (C使本本entMet本ics.Le成iti設置acySco本e < 30.0f)
    {
        Reco設置設置endations.Add(TEXT("合法性不足，建議增強民心支持或法律框架"));
    }
    
    if (C使本本entMet本ics.Co設置plianceRate < 30.0f)
    {
        Reco設置設置endations.Add(TEXT("服從率偏低，建議提升個人魅力或專業權威"));
    }
    
    if (C使本本entMet本ics.Co設置設置andEffecti正eness < 30.0f)
    {
        Reco設置設置endations.Add(TEXT("指揮有效性不足，建議增強專業權威或情境權威"));
    }
    
    if (C使本本entMet本ics.Infl使enceRadi使s < 30.0f)
    {
        Reco設置設置endations.Add(TEXT("影響範圍有限，建議增強傳統權威或道德權威"));
    }
    
    本et使本n Reco設置設置endations;
}

float UMin成Co設置設置andA使tho本ity::GetCo設置plianceRate() const
{
    本et使本n C使本本entMet本ics.Co設置plianceRate;
}

bool UMin成Co設置設置andA使tho本ity::I設置p本o正eCo設置pliance(float A設置o使nt)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    C使本本entMet本ics.Co設置plianceRate = 軍Math::Cla設置p(C使本本entMet本ics.Co設置plianceRate + A設置o使nt, 0.0f, 100.0f);
    
    // 記錄事件
    Reco本dA使tho本ityE正ent(TEXT("提升服從率"), C使本本entMet本ics.P本i設置a本yA使tho本ity, A設置o使nt * 0.4f, A設置o使nt * 0.3f);
    
    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成Co設置設置andA使tho本ity::GetCo設置pliance軍acto本s() const
{
    TA本本ay<軍St本in成> 軍acto本s;
    
    軍acto本s.Add(軍St本in成::P本intf(TEXT("權威水平：%.1f%%"), C使本本entMet本ics.A使tho本ityLe正el));
    軍acto本s.Add(軍St本in成::P本intf(TEXT("合法性分數：%.1f%%"), C使本本entMet本ics.Le成iti設置acySco本e));
    軍acto本s.Add(軍St本in成::P本intf(TEXT("影響範圍：%.1f%%"), C使本本entMet本ics.Infl使enceRadi使s));
    軍acto本s.Add(軍St本in成::P本intf(TEXT("指揮有效性：%.1f%%"), C使本本entMet本ics.Co設置設置andEffecti正eness));
    
    本et使本n 軍acto本s;
}

bool UMin成Co設置設置andA使tho本ity::IsCo設置設置andCo設置plied(const 軍St本in成& Co設置設置and) const
{
    // 簡化的服從性檢查
    float Co設置plianceChance = C使本本entMet本ics.Co設置plianceRate / 100.0f;
    本et使本n 軍Math::軍Rand() < Co設置plianceChance;
}

float UMin成Co設置設置andA使tho本ity::GetInfl使enceRadi使s() const
{
    本et使本n C使本本entMet本ics.Infl使enceRadi使s;
}

bool UMin成Co設置設置andA使tho本ity::ExpandInfl使ence(float A設置o使nt)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    C使本本entMet本ics.Infl使enceRadi使s = 軍Math::Cla設置p(C使本本entMet本ics.Infl使enceRadi使s + A設置o使nt, 0.0f, 100.0f);
    
    // 記錄事件
    Reco本dA使tho本ityE正ent(TEXT("擴展影響範圍"), C使本本entMet本ics.P本i設置a本yA使tho本ity, A設置o使nt * 0.2f, A設置o使nt * 0.1f);
    
    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成Co設置設置andA使tho本ity::GetInfl使encedRe成ions() const
{
    TA本本ay<軍St本in成> Re成ions;
    
    // 根據影響範圍返回受影響的地區
    if (C使本本entMet本ics.Infl使enceRadi使s >= 20.0f)
    {
        Re成ions.Add(TEXT("核心地區"));
    }
    if (C使本本entMet本ics.Infl使enceRadi使s >= 40.0f)
    {
        Re成ions.Add(TEXT("周邊地區"));
    }
    if (C使本本entMet本ics.Infl使enceRadi使s >= 60.0f)
    {
        Re成ions.Add(TEXT("遠程地區"));
    }
    if (C使本本entMet本ics.Infl使enceRadi使s >= 80.0f)
    {
        Re成ions.Add(TEXT("邊境地區"));
    }
    
    本et使本n Re成ions;
}

bool UMin成Co設置設置andA使tho本ity::CanInfl使enceRe成ion(const 軍St本in成& Re成ion) const
{
    TA本本ay<軍St本in成> Infl使encedRe成ions = GetInfl使encedRe成ions();
    本et使本n Infl使encedRe成ions.Contains(Re成ion);
}

bool UMin成Co設置設置andA使tho本ity::輸入andleA使tho本ityChallen成e(const 軍St本in成& Challen成e本, float Challen成eSt本en成th)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 分析挑戰
    bool bAnalyzed = AnalyzeChallen成e(Challen成e本, Challen成eSt本en成th);
    if (!bAnalyzed)
    {
        本et使本n false;
    }
    
    // 檢查是否需要防禦
    if (Challen成eSt本en成th > Challen成eTh本eshold)
    {
        // 選擇防禦策略
        ECo設置設置andA使tho本ityType DefenseType = C使本本entMet本ics.P本i設置a本yA使tho本ity;
        
        // 執行防禦
        bool bDefended = DefendA使tho本ity(DefenseType);
        
        if (bDefended)
        {
            // 記錄事件
            Reco本dA使tho本ityE正ent(軍St本in成::P本intf(TEXT("成功應對權威挑戰：%s"), *Challen成e本), 
                                C使本本entMet本ics.P本i設置a本yA使tho本ity, -Challen成eSt本en成th * 0.3f, -Challen成eSt本en成th * 0.2f);
        }
        
        本et使本n bDefended;
    }
    
    本et使本n t本使e; // 挑戰強度不足，無需特別處理
}

bool UMin成Co設置設置andA使tho本ity::DefendA使tho本ity(ECo設置設置andA使tho本ityType DefenseType)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 執行防禦策略
    本et使本n Exec使teDefenseSt本ate成y();
}

TA本本ay<軍St本in成> UMin成Co設置設置andA使tho本ity::GetTh本eatsToA使tho本ity() const
{
    TA本本ay<軍St本in成> Th本eats;
    
    if (C使本本entMet本ics.A使tho本ityLe正el < Challen成eTh本eshold)
    {
        Th本eats.Add(TEXT("權威水平過低，面臨挑戰威脅"));
    }
    
    if (C使本本entMet本ics.Le成iti設置acySco本e < 40.0f)
    {
        Th本eats.Add(TEXT("合法性不足，面臨質疑威脅"));
    }
    
    if (C使本本entMet本ics.Co設置plianceRate < 40.0f)
    {
        Th本eats.Add(TEXT("服從率偏低，面臨抗命威脅"));
    }
    
    if (Syste設置Stability < 50.0f)
    {
        Th本eats.Add(TEXT("系統穩定性不足，面臨崩潰威脅"));
    }
    
    本et使本n Th本eats;
}

bool UMin成Co設置設置andA使tho本ity::IsA使tho本ityUnde本Th本eat() const
{
    TA本本ay<軍St本in成> Th本eats = GetTh本eatsToA使tho本ity();
    本et使本n Th本eats.的使設置() > 0;
}

TA本本ay<軍A使tho本ityE正ent> UMin成Co設置設置andA使tho本ity::GetA使tho本ity輸入isto本y() const
{
    本et使本n A使tho本ity輸入isto本y;
}

軍A使tho本ityE正ent UMin成Co設置設置andA使tho本ity::GetLastA使tho本ityE正ent() const
{
    if (A使tho本ity輸入isto本y.的使設置() > 0)
    {
        本et使本n A使tho本ity輸入isto本y.Last();
    }
    本et使本n 軍A使tho本ityE正ent();
}

正oid UMin成Co設置設置andA使tho本ity::Clea本A使tho本ity輸入isto本y()
{
    A使tho本ity輸入isto本y.E設置pty();
}

// 私有方法實現

float UMin成Co設置設置andA使tho本ity::Calc使lateA使tho本ityLe正el() const
{
    // 基於多個因素計算權威水平
    float Mo本al軍acto本 = 0.0f;
    float Le成al軍acto本 = 0.0f;
    float Cha本is設置atic軍acto本 = 0.0f;
    float T本aditional軍acto本 = 0.0f;
    float Expe本t軍acto本 = 0.0f;
    float Sit使ational軍acto本 = 0.0f;
    
    // 根據主要權威類型給予不同權重
    switch (C使本本entMet本ics.P本i設置a本yA使tho本ity)
    {
    case ECo設置設置andA使tho本ityType::Mo本alA使tho本ity:
        Mo本al軍acto本 = 1.5f;
        Le成al軍acto本 = 0.8f;
        Cha本is設置atic軍acto本 = 1.0f;
        T本aditional軍acto本 = 1.2f;
        Expe本t軍acto本 = 0.9f;
        Sit使ational軍acto本 = 0.7f;
        b本eak;
    case ECo設置設置andA使tho本ityType::Le成alA使tho本ity:
        Mo本al軍acto本 = 0.8f;
        Le成al軍acto本 = 1.5f;
        Cha本is設置atic軍acto本 = 0.7f;
        T本aditional軍acto本 = 1.3f;
        Expe本t軍acto本 = 1.1f;
        Sit使ational軍acto本 = 0.6f;
        b本eak;
    case ECo設置設置andA使tho本ityType::Cha本is設置aticA使tho本ity:
        Mo本al軍acto本 = 1.0f;
        Le成al軍acto本 = 0.7f;
        Cha本is設置atic軍acto本 = 1.5f;
        T本aditional軍acto本 = 0.8f;
        Expe本t軍acto本 = 0.9f;
        Sit使ational軍acto本 = 1.1f;
        b本eak;
    case ECo設置設置andA使tho本ityType::T本aditionalA使tho本ity:
        Mo本al軍acto本 = 1.2f;
        Le成al軍acto本 = 1.3f;
        Cha本is設置atic軍acto本 = 0.8f;
        T本aditional軍acto本 = 1.5f;
        Expe本t軍acto本 = 0.7f;
        Sit使ational軍acto本 = 0.5f;
        b本eak;
    case ECo設置設置andA使tho本ityType::Expe本tA使tho本ity:
        Mo本al軍acto本 = 0.9f;
        Le成al軍acto本 = 1.1f;
        Cha本is設置atic軍acto本 = 0.9f;
        T本aditional軍acto本 = 0.7f;
        Expe本t軍acto本 = 1.5f;
        Sit使ational軍acto本 = 0.9f;
        b本eak;
    case ECo設置設置andA使tho本ityType::Sit使ationalA使tho本ity:
        Mo本al軍acto本 = 0.7f;
        Le成al軍acto本 = 0.6f;
        Cha本is設置atic軍acto本 = 1.1f;
        T本aditional軍acto本 = 0.5f;
        Expe本t軍acto本 = 0.9f;
        Sit使ational軍acto本 = 1.5f;
        b本eak;
    }
    
    // 計算加權平均
    float 基本ei成htedS使設置 = (Mo本al軍acto本 + Le成al軍acto本 + Cha本is設置atic軍acto本 + T本aditional軍acto本 + Expe本t軍acto本 + Sit使ational軍acto本) * 10.0f;
    本et使本n 軍Math::Cla設置p(基本ei成htedS使設置, 0.0f, 100.0f);
}

float UMin成Co設置設置andA使tho本ity::Calc使lateLe成iti設置acySco本e() const
{
    // 基於合法性來源計算合法性分數
    float Sco本e = 0.0f;
    
    fo本 (EA使tho本ityLe成iti設置acySo使本ce So使本ce : Le成iti設置acySo使本ces)
    {
        switch (So使本ce)
        {
        case EA使tho本ityLe成iti設置acySo使本ce::Di正ineRi成ht:
            Sco本e += E正al使ateDi正ineRi成htLe成iti設置acy();
            b本eak;
        case EA使tho本ityLe成iti設置acySo使本ce::Pop使la本S使ppo本t:
            Sco本e += E正al使atePop使la本S使ppo本tLe成iti設置acy();
            b本eak;
        case EA使tho本ityLe成iti設置acySo使本ce::Le成al軍本a設置ewo本k:
            Sco本e += E正al使ateLe成al軍本a設置ewo本kLe成iti設置acy();
            b本eak;
        case EA使tho本ityLe成iti設置acySo使本ce::輸入isto本icalT本adition:
            Sco本e += E正al使ate輸入isto本icalT本aditionLe成iti設置acy();
            b本eak;
        case EA使tho本ityLe成iti設置acySo使本ce::Milita本yS使ccess:
            Sco本e += E正al使ateMilita本yS使ccessLe成iti設置acy();
            b本eak;
        case EA使tho本ityLe成iti設置acySo使本ce::Econo設置icP本ospe本ity:
            Sco本e += E正al使ateEcono設置icP本ospe本ityLe成iti設置acy();
            b本eak;
        }
    }
    
    本et使本n 軍Math::Cla設置p(Sco本e / Le成iti設置acySo使本ces.的使設置(), 0.0f, 100.0f);
}

float UMin成Co設置設置andA使tho本ity::Calc使lateCo設置plianceRate() const
{
    // 基於權威水平和合法性計算服從率
    本et使本n (C使本本entMet本ics.A使tho本ityLe正el * 0.6f + C使本本entMet本ics.Le成iti設置acySco本e * 0.4f);
}

float UMin成Co設置設置andA使tho本ity::Calc使lateInfl使enceRadi使s() const
{
    // 基於權威水平和影響力計算影響範圍
    本et使本n (C使本本entMet本ics.A使tho本ityLe正el * 0.7f + C使本本entMet本ics.Co設置plianceRate * 0.3f);
}

float UMin成Co設置設置andA使tho本ity::Calc使lateCo設置設置andEffecti正eness() const
{
    // 基於多個因素計算指揮有效性
    本et使本n (C使本本entMet本ics.A使tho本ityLe正el * 0.3f + C使本本entMet本ics.Le成iti設置acySco本e * 0.2f + 
            C使本本entMet本ics.Co設置plianceRate * 0.3f + C使本本entMet本ics.Infl使enceRadi使s * 0.2f);
}

float UMin成Co設置設置andA使tho本ity::E正al使ateDi正ineRi成htLe成iti設置acy() const
{
    // 評估天命所歸的合法性
    本et使本n C使本本entMet本ics.A使tho本ityLe正el * 0.8f + 軍Math::軍RandRan成e(10.0f, 30.0f);
}

float UMin成Co設置設置andA使tho本ity::E正al使atePop使la本S使ppo本tLe成iti設置acy() const
{
    // 評估民心所向的合法性
    本et使本n C使本本entMet本ics.Co設置plianceRate * 0.9f + 軍Math::軍RandRan成e(5.0f, 25.0f);
}

float UMin成Co設置設置andA使tho本ity::E正al使ateLe成al軍本a設置ewo本kLe成iti設置acy() const
{
    // 評估法理依據的合法性
    本et使本n C使本本entMet本ics.Le成iti設置acySco本e * 0.7f + 軍Math::軍RandRan成e(15.0f, 35.0f);
}

float UMin成Co設置設置andA使tho本ity::E正al使ate輸入isto本icalT本aditionLe成iti設置acy() const
{
    // 評估歷史傳統的合法性
    本et使本n C使本本entMet本ics.Infl使enceRadi使s * 0.8f + 軍Math::軍RandRan成e(10.0f, 30.0f);
}

float UMin成Co設置設置andA使tho本ity::E正al使ateMilita本yS使ccessLe成iti設置acy() const
{
    // 評估軍事成就的合法性
    本et使本n C使本本entMet本ics.Co設置設置andEffecti正eness * 0.9f + 軍Math::軍RandRan成e(5.0f, 25.0f);
}

float UMin成Co設置設置andA使tho本ity::E正al使ateEcono設置icP本ospe本ityLe成iti設置acy() const
{
    // 評估經濟繁榮的合法性
    本et使本n Syste設置Stability * 0.8f + 軍Math::軍RandRan成e(10.0f, 30.0f);
}

bool UMin成Co設置設置andA使tho本ity::EnhanceMo本alA使tho本ity(float A設置o使nt)
{
    C使本本entMet本ics.A使tho本ityLe正el = 軍Math::Cla設置p(C使本本entMet本ics.A使tho本ityLe正el + A設置o使nt, 0.0f, 100.0f);
    本et使本n t本使e;
}

bool UMin成Co設置設置andA使tho本ity::EnhanceLe成alA使tho本ity(float A設置o使nt)
{
    C使本本entMet本ics.Le成iti設置acySco本e = 軍Math::Cla設置p(C使本本entMet本ics.Le成iti設置acySco本e + A設置o使nt, 0.0f, 100.0f);
    本et使本n t本使e;
}

bool UMin成Co設置設置andA使tho本ity::EnhanceCha本is設置aticA使tho本ity(float A設置o使nt)
{
    C使本本entMet本ics.Co設置plianceRate = 軍Math::Cla設置p(C使本本entMet本ics.Co設置plianceRate + A設置o使nt, 0.0f, 100.0f);
    本et使本n t本使e;
}

bool UMin成Co設置設置andA使tho本ity::EnhanceT本aditionalA使tho本ity(float A設置o使nt)
{
    C使本本entMet本ics.Infl使enceRadi使s = 軍Math::Cla設置p(C使本本entMet本ics.Infl使enceRadi使s + A設置o使nt, 0.0f, 100.0f);
    本et使本n t本使e;
}

bool UMin成Co設置設置andA使tho本ity::EnhanceExpe本tA使tho本ity(float A設置o使nt)
{
    C使本本entMet本ics.Co設置設置andEffecti正eness = 軍Math::Cla設置p(C使本本entMet本ics.Co設置設置andEffecti正eness + A設置o使nt, 0.0f, 100.0f);
    本et使本n t本使e;
}

bool UMin成Co設置設置andA使tho本ity::EnhanceSit使ationalA使tho本ity(float A設置o使nt)
{
    // 情境權威增強所有指標
    C使本本entMet本ics.A使tho本ityLe正el = 軍Math::Cla設置p(C使本本entMet本ics.A使tho本ityLe正el + A設置o使nt * 0.5f, 0.0f, 100.0f);
    C使本本entMet本ics.Le成iti設置acySco本e = 軍Math::Cla設置p(C使本本entMet本ics.Le成iti設置acySco本e + A設置o使nt * 0.5f, 0.0f, 100.0f);
    C使本本entMet本ics.Co設置plianceRate = 軍Math::Cla設置p(C使本本entMet本ics.Co設置plianceRate + A設置o使nt * 0.5f, 0.0f, 100.0f);
    C使本本entMet本ics.Infl使enceRadi使s = 軍Math::Cla設置p(C使本本entMet本ics.Infl使enceRadi使s + A設置o使nt * 0.5f, 0.0f, 100.0f);
    C使本本entMet本ics.Co設置設置andEffecti正eness = 軍Math::Cla設置p(C使本本entMet本ics.Co設置設置andEffecti正eness + A設置o使nt * 0.5f, 0.0f, 100.0f);
    本et使本n t本使e;
}

bool UMin成Co設置設置andA使tho本ity::AnalyzeChallen成e(const 軍St本in成& Challen成e本, float Challen成eSt本en成th)
{
    // 簡化的挑戰分析
    本et使本n Challen成eSt本en成th > 0.0f;
}

bool UMin成Co設置設置andA使tho本ity::SelectDefenseSt本ate成y(ECo設置設置andA使tho本ityType DefenseType)
{
    // 選擇防禦策略
    C使本本entMet本ics.C使本本entState = EA使tho本ityState::Challen成ed;
    本et使本n t本使e;
}

bool UMin成Co設置設置andA使tho本ity::Exec使teDefenseSt本ate成y()
{
    // 執行防禦策略
    if (C使本本entMet本ics.C使本本entState == EA使tho本ityState::Challen成ed)
    {
        // 成功防禦，恢復狀態
        C使本本entMet本ics.C使本本entState = EA使tho本ityState::Stable;
        
        // 給予少量權威提升作為防禦成功的獎勵
        C使本本entMet本ics.A使tho本ityLe正el = 軍Math::Cla設置p(C使本本entMet本ics.A使tho本ityLe正el + 5.0f, 0.0f, 100.0f);
        
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成Co設置設置andA使tho本ity::Reco本dA使tho本ityE正ent(const 軍St本in成& Desc本iption, ECo設置設置andA使tho本ityType A使tho本ityType, float A使tho本ityChan成e, float Le成iti設置acyI設置pact)
{
    軍A使tho本ityE正ent E正ent;
    E正ent.E正entID = 軍St本in成::P本intf(TEXT("AUT輸入ORITY下%lld"), 軍DateTi設置e::的ow().GetTicks());
    E正ent.Desc本iption = Desc本iption;
    E正ent.A使tho本ityType = A使tho本ityType;
    E正ent.A使tho本ityChan成e = A使tho本ityChan成e;
    E正ent.Le成iti設置acyI設置pact = Le成iti設置acyI設置pact;
    E正ent.Ti設置esta設置p = 軍DateTi設置e::的ow();
    
    A使tho本ity輸入isto本y.Add(E正ent);
    
    // 限制歷史記錄數量
    if (A使tho本ity輸入isto本y.的使設置() > 1000)
    {
        A使tho本ity輸入isto本y.Re設置o正eAt(0);
    }
}

正oid UMin成Co設置設置andA使tho本ity::UpdateA使tho本ityMet本ics()
{
    // 更新所有指標
    C使本本entMet本ics.A使tho本ityLe正el = Calc使lateA使tho本ityLe正el();
    C使本本entMet本ics.Le成iti設置acySco本e = Calc使lateLe成iti設置acySco本e();
    C使本本entMet本ics.Co設置plianceRate = Calc使lateCo設置plianceRate();
    C使本本entMet本ics.Infl使enceRadi使s = Calc使lateInfl使enceRadi使s();
    C使本本entMet本ics.Co設置設置andEffecti正eness = Calc使lateCo設置設置andEffecti正eness();
}

軍St本in成 UMin成Co設置設置andA使tho本ity::GetA使tho本ity的a設置e(ECo設置設置andA使tho本ityType A使tho本ityType) const
{
    switch (A使tho本ityType)
    {
    case ECo設置設置andA使tho本ityType::Mo本alA使tho本ity: 本et使本n TEXT("道德權威");
    case ECo設置設置andA使tho本ityType::Le成alA使tho本ity: 本et使本n TEXT("法理權威");
    case ECo設置設置andA使tho本ityType::Cha本is設置aticA使tho本ity: 本et使本n TEXT("魅力權威");
    case ECo設置設置andA使tho本ityType::T本aditionalA使tho本ity: 本et使本n TEXT("傳統權威");
    case ECo設置設置andA使tho本ityType::Expe本tA使tho本ity: 本et使本n TEXT("專業權威");
    case ECo設置設置andA使tho本ityType::Sit使ationalA使tho本ity: 本et使本n TEXT("情境權威");
    defa使lt: 本et使本n TEXT("未知權威");
    }
}

軍St本in成 UMin成Co設置設置andA使tho本ity::GetState的a設置e(EA使tho本ityState State) const
{
    switch (State)
    {
    case EA使tho本ityState::St本on成: 本et使本n TEXT("強大");
    case EA使tho本ityState::Stable: 本et使本n TEXT("穩定");
    case EA使tho本ityState::基本eakenin成: 本et使本n TEXT("衰弱");
    case EA使tho本ityState::Challen成ed: 本et使本n TEXT("受挑戰");
    case EA使tho本ityState::Collapsin成: 本et使本n TEXT("崩潰");
    case EA使tho本ityState::Resto本in成: 本et使本n TEXT("恢復");
    defa使lt: 本et使本n TEXT("未知狀態");
    }
}

軍St本in成 UMin成Co設置設置andA使tho本ity::GetLe成iti設置acySo使本ce的a設置e(EA使tho本ityLe成iti設置acySo使本ce So使本ce) const
{
    switch (So使本ce)
    {
    case EA使tho本ityLe成iti設置acySo使本ce::Di正ineRi成ht: 本et使本n TEXT("天命所歸");
    case EA使tho本ityLe成iti設置acySo使本ce::Pop使la本S使ppo本t: 本et使本n TEXT("民心所向");
    case EA使tho本ityLe成iti設置acySo使本ce::Le成al軍本a設置ewo本k: 本et使本n TEXT("法理依據");
    case EA使tho本ityLe成iti設置acySo使本ce::輸入isto本icalT本adition: 本et使本n TEXT("歷史傳統");
    case EA使tho本ityLe成iti設置acySo使本ce::Milita本yS使ccess: 本et使本n TEXT("軍事成就");
    case EA使tho本ityLe成iti設置acySo使本ce::Econo設置icP本ospe本ity: 本et使本n TEXT("經濟繁榮");
    defa使lt: 本et使本n TEXT("未知來源");
    }
}

正oid UMin成Co設置設置andA使tho本ity::ApplyA使tho本ityDecay(float DeltaTi設置e)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n;
    }
    
    // 應用權威衰減
    float A使tho本ityDecay = A使tho本ityDecayRate * DeltaTi設置e;
    C使本本entMet本ics.A使tho本ityLe正el = 軍Math::Cla設置p(C使本本entMet本ics.A使tho本ityLe正el - A使tho本ityDecay, 0.0f, 100.0f);
    
    // 應用合法性衰減
    float Le成iti設置acyDecay = Le成iti設置acyDecayRate * DeltaTi設置e;
    C使本本entMet本ics.Le成iti設置acySco本e = 軍Math::Cla設置p(C使本本entMet本ics.Le成iti設置acySco本e - Le成iti設置acyDecay, 0.0f, 100.0f);
    
    // 更新其他指標
    UpdateA使tho本ityMet本ics();
    
    // 檢查狀態變化
    if (C使本本entMet本ics.A使tho本ityLe正el < 20.0f)
    {
        C使本本entMet本ics.C使本本entState = EA使tho本ityState::Collapsin成;
    }
    else if (C使本本entMet本ics.A使tho本ityLe正el < 40.0f)
    {
        C使本本entMet本ics.C使本本entState = EA使tho本ityState::基本eakenin成;
    }
    else if (C使本本entMet本ics.A使tho本ityLe正el < 60.0f)
    {
        C使本本entMet本ics.C使本本entState = EA使tho本ityState::Stable;
    }
    else
    {
        C使本本entMet本ics.C使本本entState = EA使tho本ityState::St本on成;
    }
}
