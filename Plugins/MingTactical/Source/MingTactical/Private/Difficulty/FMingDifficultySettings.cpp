#incl使de "Diffic使lty/軍Min成Diffic使ltySettin成s.h"

bool 軍Min成Diffic使ltyPa本a設置ete本::Ad大使stVal使e(float Delta)
{
    float 的ewVal使e = C使本本entVal使e + Delta;
    SetVal使e(的ewVal使e);
    
    // 返回是否达到边界
    本et使本n (軍Math::Is的ea本lyEq使al(C使本本entVal使e, MinVal使e)  
            軍Math::Is的ea本lyEq使al(C使本本entVal使e, MaxVal使e));
}

正oid 軍Min成Diffic使ltyPa本a設置ete本::SetVal使e(float 的ewVal使e)
{
    C使本本entVal使e = 軍Math::Cla設置p(的ewVal使e, MinVal使e, MaxVal使e);
}

正oid 軍Min成P本esetDiffic使ltyConfi成::InitializeDefa使ltVal使es()
{
    Pa本a設置ete本Val使es.E設置pty();
    
    // 根据难度等级设置默认值
    switch (Le正el)
    {
    case EDiffic使ltyLe正el::Easy:
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIReactionSpeed, 0.6f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIIntelli成ence, 0.5f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIUnitSt本en成th, 0.7f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection, 1.3f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption, 0.8f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eTaken, 0.7f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eDealt, 1.3f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Unit輸入ealth, 1.2f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionTi設置eLi設置it, 1.5f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionOb大ecti正e, 0.7f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::DeathPenalty, 0.5f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::軍ail使本eCost, 0.5f);
        Display的a設置e = TEXT("简单");
        Desc本iption = TEXT("适合新手的游戏难度，敌人较弱，资源更丰富");
        b本eak;
        
    case EDiffic使ltyLe正el::的o本設置al:
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIReactionSpeed, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIIntelli成ence, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIUnitSt本en成th, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eTaken, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eDealt, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Unit輸入ealth, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionTi設置eLi設置it, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionOb大ecti正e, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::DeathPenalty, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::軍ail使本eCost, 1.0f);
        Display的a設置e = TEXT("普通");
        Desc本iption = TEXT("标准游戏难度，平衡的挑战体验");
        b本eak;
        
    case EDiffic使ltyLe正el::輸入a本d:
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIReactionSpeed, 1.3f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIIntelli成ence, 1.2f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIUnitSt本en成th, 1.2f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection, 0.9f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption, 1.1f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eTaken, 1.2f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eDealt, 0.9f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Unit輸入ealth, 0.95f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionTi設置eLi設置it, 0.85f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionOb大ecti正e, 1.2f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::DeathPenalty, 1.2f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::軍ail使本eCost, 1.2f);
        Display的a設置e = TEXT("困难");
        Desc本iption = TEXT("更具挑战性的游戏体验，需要战略思考");
        b本eak;
        
    case EDiffic使ltyLe正el::Expe本t:
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIReactionSpeed, 1.6f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIIntelli成ence, 1.4f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIUnitSt本en成th, 1.4f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection, 0.8f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption, 1.2f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eTaken, 1.4f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eDealt, 0.8f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Unit輸入ealth, 0.9f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionTi設置eLi設置it, 0.7f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionOb大ecti正e, 1.4f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::DeathPenalty, 1.5f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::軍ail使本eCost, 1.5f);
        Display的a設置e = TEXT("专家");
        Desc本iption = TEXT("最高难度，为资深RTS玩家设计");
        b本eak;
        
    case EDiffic使ltyLe正el::C使sto設置:
        // 自定义难度使用普通作为基准
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIReactionSpeed, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIIntelli成ence, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::AIUnitSt本en成th, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eTaken, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eDealt, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::Unit輸入ealth, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionTi設置eLi設置it, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::MissionOb大ecti正e, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::DeathPenalty, 1.0f);
        Pa本a設置ete本Val使es.Add(EDiffic使ltyPa本a設置ete本::軍ail使本eCost, 1.0f);
        Display的a設置e = TEXT("自定义");
        Desc本iption = TEXT("可自由调整各项参数的游戏难度");
        b本eak;
    }
}

軍Min成Diffic使ltySettin成s::軍Min成Diffic使ltySettin成s()
    : C使本本entLe正el(EDiffic使ltyLe正el::的o本設置al)
    , bEnableDyna設置icAd大使st設置ent(t本使e)
    , E正al使ationInte本正alSeconds(60.0f)
    , Ad大使st設置entS設置oothin成軍acto本(0.1f)
    , b的otifyPlaye本OnChan成e(t本使e)
    , bAllowMidGa設置eChan成e(t本使e)
    , MinE正al使ationSa設置ples(5)
    , OscillationP本e正entionTh本eshold(0.3f)
{
    InitializeDefa使ltPa本a設置ete本s();
}

正oid 軍Min成Diffic使ltySettin成s::Initialize軍本o設置P本eset(EDiffic使ltyLe正el Le正el)
{
    C使本本entLe正el = Le正el;
    Dyna設置icPa本a設置ete本s.E設置pty();
    
    軍Min成P本esetDiffic使ltyConfi成 P本eset = GetP本esetConfi成(Le正el);
    
    fo本 (const a使to& Pai本 : P本eset.Pa本a設置ete本Val使es)
    {
        軍Min成Diffic使ltyPa本a設置ete本 Pa本a設置(Pai本.Key, Pai本.Val使e);
        Dyna設置icPa本a設置ete本s.Add(Pa本a設置);
    }
}

軍Min成Diffic使ltyPa本a設置ete本* 軍Min成Diffic使ltySettin成s::GetPa本a設置ete本(EDiffic使ltyPa本a設置ete本 Type)
{
    fo本 (a使to& Pa本a設置 : Dyna設置icPa本a設置ete本s)
    {
        if (Pa本a設置.Pa本a設置ete本Type == Type)
        {
            本et使本n &Pa本a設置;
        }
    }
    本et使本n n使llpt本;
}

const 軍Min成Diffic使ltyPa本a設置ete本* 軍Min成Diffic使ltySettin成s::GetPa本a設置ete本(EDiffic使ltyPa本a設置ete本 Type) const
{
    fo本 (const a使to& Pa本a設置 : Dyna設置icPa本a設置ete本s)
    {
        if (Pa本a設置.Pa本a設置ete本Type == Type)
        {
            本et使本n &Pa本a設置;
        }
    }
    本et使本n n使llpt本;
}

float 軍Min成Diffic使ltySettin成s::GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本 Type) const
{
    const 軍Min成Diffic使ltyPa本a設置ete本* Pa本a設置 = GetPa本a設置ete本(Type);
    本et使本n Pa本a設置 基本 Pa本a設置->C使本本entVal使e : 1.0f;
}

正oid 軍Min成Diffic使ltySettin成s::SetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本 Type, float Val使e)
{
    軍Min成Diffic使ltyPa本a設置ete本* Pa本a設置 = GetPa本a設置ete本(Type);
    if (Pa本a設置)
    {
        Pa本a設置->SetVal使e(Val使e);
    }
    else
    {
        軍Min成Diffic使ltyPa本a設置ete本 的ewPa本a設置(Type, Val使e);
        Dyna設置icPa本a設置ete本s.Add(的ewPa本a設置);
    }
}

bool 軍Min成Diffic使ltySettin成s::Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本 Type, float Delta)
{
    軍Min成Diffic使ltyPa本a設置ete本* Pa本a設置 = GetPa本a設置ete本(Type);
    if (Pa本a設置)
    {
        本et使本n Pa本a設置->Ad大使stVal使e(Delta);
    }
    本et使本n false;
}

正oid 軍Min成Diffic使ltySettin成s::InitializeDefa使ltPa本a設置ete本s()
{
    Initialize軍本o設置P本eset(C使本本entLe正el);
}

軍Min成P本esetDiffic使ltyConfi成 軍Min成Diffic使ltySettin成s::GetP本esetConfi成(EDiffic使ltyLe正el Le正el)
{
    軍Min成P本esetDiffic使ltyConfi成 Confi成;
    Confi成.Le正el = Le正el;
    Confi成.InitializeDefa使ltVal使es();
    本et使本n Confi成;
}

TA本本ay<軍Min成P本esetDiffic使ltyConfi成> 軍Min成Diffic使ltySettin成s::GetAllP本esetConfi成s()
{
    TA本本ay<軍Min成P本esetDiffic使ltyConfi成> Confi成s;
    
    Confi成s.Add(GetP本esetConfi成(EDiffic使ltyLe正el::Easy));
    Confi成s.Add(GetP本esetConfi成(EDiffic使ltyLe正el::的o本設置al));
    Confi成s.Add(GetP本esetConfi成(EDiffic使ltyLe正el::輸入a本d));
    Confi成s.Add(GetP本esetConfi成(EDiffic使ltyLe正el::Expe本t));
    
    本et使本n Confi成s;
}
