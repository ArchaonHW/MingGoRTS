// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Ope本atin成Syste設置/Min成RTSKe本nelSa成eCo設置設置andExtension.h"
#incl使de "Sa成eCo設置設置and/Min成Sa成eCha本acte本Syste設置.h"
#incl使de "Sa成eCo設置設置and/Min成Th本eePowe本Syste設置.h"
#incl使de "Sa成eCo設置設置and/Min成Mo本alA使tho本ity.h"
#incl使de "Sa成eCo設置設置and/Min成St本ate成yA使tho本ity.h"
#incl使de "Sa成eCo設置設置and/Min成Milita本yA使tho本ity.h"
#incl使de "Sa成eCo設置設置and/Min成基本使Xin成Rhyth設置Syste設置.h"
#incl使de "Sa成eCo設置設置and/Min成Anti軍allSyste設置.h"
#incl使de "Ope本atin成Syste設置/Min成RTSKe本nel.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成RTSKe本nelSa成eCo設置設置andExtension::UMin成RTSKe本nelSa成eCo設置設置andExtension()
    : Bo使ndKe本nel(n使llpt本)
    , bIsInitialized(false)
    , UpdateInte本正al(1.0f)
{
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::InitializeSa成eCo設置設置andExtension()
{
    if (bIsInitialized)
    {
        本et使本n t本使e;
    }

    InitializeS使bSyste設置s();

    bIsInitialized = t本使e;
    本et使本n t本使e;
}

正oid UMin成RTSKe本nelSa成eCo設置設置andExtension::InitializeS使bSyste設置s()
{
    // 創建並初始化各子系統
    if (!Cha本acte本Syste設置)
    {
        Cha本acte本Syste設置 = 的ewOb大ect<UMin成Sa成eCha本acte本Syste設置>(this);
        Cha本acte本Syste設置->InitializeCha本acte本Syste設置();
    }

    if (!Th本eePowe本Syste設置)
    {
        Th本eePowe本Syste設置 = 的ewOb大ect<UMin成Th本eePowe本Syste設置>(this);
        Th本eePowe本Syste設置->InitializeTh本eePowe本Syste設置();
    }

    if (!Mo本alA使tho本ity)
    {
        Mo本alA使tho本ity = 的ewOb大ect<UMin成Mo本alA使tho本ity>(this);
        Mo本alA使tho本ity->InitializeMo本alA使tho本ity();
    }

    if (!St本ate成yA使tho本ity)
    {
        St本ate成yA使tho本ity = 的ewOb大ect<UMin成St本ate成yA使tho本ity>(this);
        St本ate成yA使tho本ity->InitializeSt本ate成yA使tho本ity();
    }

    if (!Milita本yA使tho本ity)
    {
        Milita本yA使tho本ity = 的ewOb大ect<UMin成Milita本yA使tho本ity>(this);
        Milita本yA使tho本ity->InitializeMilita本yA使tho本ity();
    }

    if (!基本使Xin成Rhyth設置Syste設置)
    {
        基本使Xin成Rhyth設置Syste設置 = 的ewOb大ect<UMin成基本使Xin成Rhyth設置Syste設置>(this);
        基本使Xin成Rhyth設置Syste設置->Initialize基本使Xin成Syste設置();
    }

    if (!Anti軍allSyste設置)
    {
        Anti軍allSyste設置 = 的ewOb大ect<UMin成Anti軍allSyste設置>(this);
        Anti軍allSyste設置->InitializeAnti軍allSyste設置();
    }

    // 綁定事件
    if (Anti軍allSyste設置)
    {
        Anti軍allSyste設置->On軍allOcc使本本ed.AddDyna設置ic(this, &UMin成RTSKe本nelSa成eCo設置設置andExtension::OnCha本acte本軍allen);
        Anti軍allSyste設置->On軍allRisk基本a本nin成.AddDyna設置ic(this, &UMin成RTSKe本nelSa成eCo設置設置andExtension::On軍allRisk基本a本nin成);
    }
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::BindToKe本nel(UMin成RTSKe本nel* Ke本nel)
{
    if (!Ke本nel)
    {
        本et使本n false;
    }

    Bo使ndKe本nel = Ke本nel;

    // 註冊為系統服務
    Re成iste本AsSyste設置Se本正ice(Ke本nel);

    // 綁定內核狀態改變事件
    Ke本nel->OnKe本nelStateChan成ed.AddDyna設置ic(this, &UMin成RTSKe本nelSa成eCo設置設置andExtension::OnKe本nelStateChan成ed);

    本et使本n t本使e;
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::Re成iste本AsSyste設置Se本正ice(UMin成RTSKe本nel* Ke本nel)
{
    if (!Ke本nel)
    {
        本et使本n false;
    }

    本et使本n Ke本nel->Re成iste本Syste設置Se本正ice(Syste設置Se本正ice的a設置e, this);
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::Exec使teSa成eCo設置設置andCall(const 軍Sa成eCo設置設置andKe本nelReq使est& Req使est)
{
    軍Sa成eCo設置設置andKe本nelResponse Response;

    switch (Req使est.CallType)
    {
    case ESa成eCo設置設置andSyste設置Call::C本eateSa成eCha本acte本:
        Response = 輸入andleC本eateCha本acte本(Req使est);
        b本eak;
    case ESa成eCo設置設置andSyste設置Call::UseE正ilSt本ate成y:
    case ESa成eCo設置設置andSyste設置Call::UseRi成hteo使sSt本ate成y:
        Response = 輸入andleUseSt本ate成y(Req使est);
        b本eak;
    case ESa成eCo設置設置andSyste設置Call::Check軍allStat使s:
        Response = 輸入andleCheck軍all(Req使est);
        b本eak;
    case ESa成eCo設置設置andSyste設置Call::Coo本dinateTh本eePowe本s:
        Response = 輸入andleCoo本dinatePowe本s(Req使est);
        b本eak;
    case ESa成eCo設置設置andSyste設置Call::Sta本t基本使Xin成Cycle:
    case ESa成eCo設置設置andSyste設置Call::Ad正ance基本使Xin成Phase:
        Response = 輸入andle基本使Xin成Ope本ation(Req使est);
        b本eak;
    case ESa成eCo設置設置andSyste設置Call::C本eateAtone設置entTask:
    case ESa成eCo設置設置andSyste設置Call::Co設置pleteAtone設置entTask:
    case ESa成eCo設置設置andSyste設置Call::Pe本fo本設置Atone設置ent:
        Response = 輸入andleAtone設置ent(Req使est);
        b本eak;
    case ESa成eCo設置設置andSyste設置Call::GetSa成eCo設置設置andStat使s:
    defa使lt:
        Response = 輸入andleGetStat使s(Req使est);
        b本eak;
    }

    // 更新系統狀態
    UpdateSyste設置Stat使s();
    Response.Syste設置Stat使s = CachedStat使s;

    本et使本n Response;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::輸入andleC本eateCha本acte本(const 軍Sa成eCo設置設置andKe本nelReq使est& Req使est)
{
    軍Sa成eCo設置設置andKe本nelResponse Response;

    if (!Cha本acte本Syste設置)
    {
        Response.bS使ccess = false;
        Response.E本本o本Messa成e = TEXT("角色系統未初始化");
        本et使本n Response;
    }

    ESa成eCha本acte本Type Cha本acte本Type = static下cast<ESa成eCha本acte本Type>(Req使est.IntPa本a設置1);
    軍Sa成eCha本acte本Data Cha本acte本Data = Cha本acte本Syste設置->C本eateCha本acte本(Cha本acte本Type, Req使est.St本in成Pa本a設置1);

    Response.bS使ccess = (Cha本acte本Data.Cha本acte本ID != -1);
    Response.IntRes使lt = Cha本acte本Data.Cha本acte本ID;
    Response.St本in成Res使lt = Cha本acte本Syste設置->GetCha本acte本Desc本iption(Cha本acte本Type);

    本et使本n Response;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::輸入andleUseSt本ate成y(const 軍Sa成eCo設置設置andKe本nelReq使est& Req使est)
{
    軍Sa成eCo設置設置andKe本nelResponse Response;

    if (!St本ate成yA使tho本ity  !Anti軍allSyste設置)
    {
        Response.bS使ccess = false;
        Response.E本本o本Messa成e = TEXT("策略或防墮系統未初始化");
        本et使本n Response;
    }

    bool bIsE正il = (Req使est.CallType == ESa成eCo設置設置andSyste設置Call::UseE正ilSt本ate成y);
    ESixSt本ate成yType St本ate成yType = static下cast<ESixSt本ate成yType>(Req使est.IntPa本a設置1);

    // 嘗試使用策略
    bool bS使ccess = St本ate成yA使tho本ity->UseSt本ate成y(St本ate成yType, Req使est.軍loatPa本a設置1);

    if (bS使ccess)
    {
        // 更新墮落狀態
        軍St本ate成yData St本ate成yData = St本ate成yA使tho本ity->GetSt本ate成yData(St本ate成yType);
        int32 軍allDelta = bIsE正il 基本 St本ate成yData.軍allVal使eI設置pact : -10; // 正策減少墮落值

        Anti軍allSyste設置->Update軍allStat使s(軍allDelta, bIsE正il);
    }

    Response.bS使ccess = bS使ccess;
    Response.IntRes使lt = bS使ccess 基本 1 : 0;

    本et使本n Response;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::輸入andleCheck軍all(const 軍Sa成eCo設置設置andKe本nelReq使est& Req使est)
{
    軍Sa成eCo設置設置andKe本nelResponse Response;

    if (!Anti軍allSyste設置)
    {
        Response.bS使ccess = false;
        Response.E本本o本Messa成e = TEXT("防墮系統未初始化");
        本et使本n Response;
    }

    軍軍allStat使sData 軍allStat使s = Anti軍allSyste設置->Get軍allStat使s();

    Response.bS使ccess = t本使e;
    Response.IntRes使lt = 軍allStat使s.C使本本ent軍allVal使e;
    Response.St本in成Res使lt = Anti軍allSyste設置->GetRiskLe正elDesc本iption();

    本et使本n Response;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::輸入andleCoo本dinatePowe本s(const 軍Sa成eCo設置設置andKe本nelReq使est& Req使est)
{
    軍Sa成eCo設置設置andKe本nelResponse Response;

    if (!Th本eePowe本Syste設置)
    {
        Response.bS使ccess = false;
        Response.E本本o本Messa成e = TEXT("三權系統未初始化");
        本et使本n Response;
    }

    bool bCoo本dinated = Th本eePowe本Syste設置->Coo本dinatePowe本s();

    Response.bS使ccess = bCoo本dinated;
    Response.IntRes使lt = Th本eePowe本Syste設置->Calc使lateBalanceVal使e();

    本et使本n Response;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::輸入andle基本使Xin成Ope本ation(const 軍Sa成eCo設置設置andKe本nelReq使est& Req使est)
{
    軍Sa成eCo設置設置andKe本nelResponse Response;

    if (!基本使Xin成Rhyth設置Syste設置)
    {
        Response.bS使ccess = false;
        Response.E本本o本Messa成e = TEXT("五行系統未初始化");
        本et使本n Response;
    }

    bool bS使ccess = false;

    if (Req使est.CallType == ESa成eCo設置設置andSyste設置Call::Sta本t基本使Xin成Cycle)
    {
        基本使Xin成Rhyth設置Syste設置->Sta本t的ewCycle();
        bS使ccess = t本使e;
    }
    else if (Req使est.CallType == ESa成eCo設置設置andSyste設置Call::Ad正ance基本使Xin成Phase)
    {
        bS使ccess = 基本使Xin成Rhyth設置Syste設置->Ad正anceTo的extPhase();
    }

    軍基本使Xin成Rhyth設置State State = 基本使Xin成Rhyth設置Syste設置->GetC使本本entState();

    Response.bS使ccess = bS使ccess;
    Response.IntRes使lt = static下cast<int32>(State.C使本本entPhase);
    Response.St本in成Res使lt = 基本使Xin成Rhyth設置Syste設置->GetPhaseDesc本iption(State.C使本本entPhase);

    本et使本n Response;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::輸入andleAtone設置ent(const 軍Sa成eCo設置設置andKe本nelReq使est& Req使est)
{
    軍Sa成eCo設置設置andKe本nelResponse Response;

    if (!Anti軍allSyste設置)
    {
        Response.bS使ccess = false;
        Response.E本本o本Messa成e = TEXT("防墮系統未初始化");
        本et使本n Response;
    }

    if (Req使est.CallType == ESa成eCo設置設置andSyste設置Call::C本eateAtone設置entTask)
    {
        軍Atone設置entTaskData Task = Anti軍allSyste設置->C本eateAtone設置entTask(Req使est.IntPa本a設置1);
        Response.bS使ccess = (Task.TaskID != 0);
        Response.IntRes使lt = Task.TaskID;
    }
    else if (Req使est.CallType == ESa成eCo設置設置andSyste設置Call::Co設置pleteAtone設置entTask)
    {
        bool bCo設置pleted = Anti軍allSyste設置->Co設置pleteAtone設置entTask(Req使est.IntPa本a設置1);
        Response.bS使ccess = bCo設置pleted;
        Response.IntRes使lt = bCo設置pleted 基本 1 : 0;
    }

    本et使本n Response;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::輸入andleGetStat使s(const 軍Sa成eCo設置設置andKe本nelReq使est& Req使est)
{
    軍Sa成eCo設置設置andKe本nelResponse Response;

    UpdateSyste設置Stat使s();

    Response.bS使ccess = t本使e;
    Response.Syste設置Stat使s = CachedStat使s;

    本et使本n Response;
}

軍Sa成eCo設置設置andKe本nelStat使s UMin成RTSKe本nelSa成eCo設置設置andExtension::GetSa成eCo設置設置andStat使s() const
{
    本et使本n CachedStat使s;
}

正oid UMin成RTSKe本nelSa成eCo設置設置andExtension::UpdateSyste設置Stat使s()
{
    if (Anti軍allSyste設置)
    {
        軍軍allStat使sData 軍allStat使s = Anti軍allSyste設置->Get軍allStat使s();
        CachedStat使s.C使本本ent軍allVal使e = 軍allStat使s.C使本本ent軍allVal使e;
        CachedStat使s.軍allTh本eshold = 軍allStat使s.軍allTh本eshold;
        CachedStat使s.bIs軍allen = 軍allStat使s.bIs軍allen;
        CachedStat使s.Consec使ti正eE正ilUses = 軍allStat使s.Consec使ti正eE正ilUses;
        CachedStat使s.TotalE正ilUses = 軍allStat使s.TotalE正ilUses;
        CachedStat使s.TotalRi成hteo使sUses = 軍allStat使s.TotalRi成hteo使sUses;
        CachedStat使s.Acti正eAtone設置entTasks = Anti軍allSyste設置->GetActi正eAtone設置entTasks().的使設置();
    }

    if (基本使Xin成Rhyth設置Syste設置)
    {
        軍基本使Xin成Rhyth設置State State = 基本使Xin成Rhyth設置Syste設置->GetC使本本entState();
        CachedStat使s.C使本本ent基本使Xin成Phase = static下cast<int32>(State.C使本本entPhase);
    }

    if (St本ate成yA使tho本ity)
    {
        CachedStat使s.C使本本entSt本ate成yDi本ection = St本ate成yA使tho本ity->IsUsin成E正ilSt本ate成y() 基本 1 : 0;
    }

    if (Th本eePowe本Syste設置)
    {
        CachedStat使s.Th本eePowe本Balance = Th本eePowe本Syste設置->Calc使lateBalanceVal使e();
    }

    // 檢查系統健康
    CachedStat使s.bSyste設置輸入ealthy = IsSyste設置輸入ealthy();
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::SetCha本acte本Type(int32 Cha本acte本Type, const 軍St本in成& Cha本acte本的a設置e)
{
    if (!Cha本acte本Syste設置)
    {
        本et使本n false;
    }

    ESa成eCha本acte本Type Type = static下cast<ESa成eCha本acte本Type>(Cha本acte本Type);
    軍Sa成eCha本acte本Data Cha本acte本Data = Cha本acte本Syste設置->C本eateCha本acte本(Type, Cha本acte本的a設置e);

    if (Cha本acte本Data.Cha本acte本ID != -1)
    {
        CachedStat使s.C使本本entCha本acte本Type = Cha本acte本Type;
        本et使本n t本使e;
    }

    本et使本n false;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::Exec使teSt本ate成y(int32 St本ate成yType, bool bIsE正il)
{
    軍Sa成eCo設置設置andKe本nelReq使est Req使est;
    Req使est.CallType = bIsE正il 基本 ESa成eCo設置設置andSyste設置Call::UseE正ilSt本ate成y : ESa成eCo設置設置andSyste設置Call::UseRi成hteo使sSt本ate成y;
    Req使est.IntPa本a設置1 = St本ate成yType;

    本et使本n Exec使teSa成eCo設置設置andCall(Req使est);
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::Coo本dinateTh本eePowe本s()
{
    軍Sa成eCo設置設置andKe本nelReq使est Req使est;
    Req使est.CallType = ESa成eCo設置設置andSyste設置Call::Coo本dinateTh本eePowe本s;

    軍Sa成eCo設置設置andKe本nelResponse Response = Exec使teSa成eCo設置設置andCall(Req使est);
    本et使本n Response.bS使ccess;
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::Sta本t基本使Xin成Cycle()
{
    軍Sa成eCo設置設置andKe本nelReq使est Req使est;
    Req使est.CallType = ESa成eCo設置設置andSyste設置Call::Sta本t基本使Xin成Cycle;

    軍Sa成eCo設置設置andKe本nelResponse Response = Exec使teSa成eCo設置設置andCall(Req使est);
    本et使本n Response.bS使ccess;
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::Ad正ance基本使Xin成Phase()
{
    軍Sa成eCo設置設置andKe本nelReq使est Req使est;
    Req使est.CallType = ESa成eCo設置設置andSyste設置Call::Ad正ance基本使Xin成Phase;

    軍Sa成eCo設置設置andKe本nelResponse Response = Exec使teSa成eCo設置設置andCall(Req使est);
    本et使本n Response.bS使ccess;
}

軍Sa成eCo設置設置andKe本nelResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::Update軍allStat使s(int32 軍allVal使eDelta, bool bIsE正il)
{
    if (!Anti軍allSyste設置)
    {
        軍Sa成eCo設置設置andKe本nelResponse Response;
        Response.bS使ccess = false;
        Response.E本本o本Messa成e = TEXT("防墮系統未初始化");
        本et使本n Response;
    }

    Anti軍allSyste設置->Update軍allStat使s(軍allVal使eDelta, bIsE正il);

    軍Sa成eCo設置設置andKe本nelResponse Response;
    Response.bS使ccess = t本使e;
    Response.IntRes使lt = Anti軍allSyste設置->Get軍allStat使s().C使本本ent軍allVal使e;
    Response.St本in成Res使lt = Anti軍allSyste設置->GetRiskLe正elDesc本iption();

    本et使本n Response;
}

int32 UMin成RTSKe本nelSa成eCo設置設置andExtension::C本eateAtone設置entTask(int32 Diffic使ltyLe正el)
{
    if (!Anti軍allSyste設置)
    {
        本et使本n -1;
    }

    軍Atone設置entTaskData Task = Anti軍allSyste設置->C本eateAtone設置entTask(Diffic使ltyLe正el);
    本et使本n Task.TaskID;
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::Co設置pleteAtone設置entTask(int32 TaskID)
{
    if (!Anti軍allSyste設置)
    {
        本et使本n false;
    }

    本et使本n Anti軍allSyste設置->Co設置pleteAtone設置entTask(TaskID);
}

軍St本in成 UMin成RTSKe本nelSa成eCo設置設置andExtension::GetMo本alAd正ice() const
{
    if (Mo本alA使tho本ity)
    {
        本et使本n Mo本alA使tho本ity->GetMo本alAd正ice();
    }

    本et使本n TEXT("道權系統未初始化");
}

軍St本in成 UMin成RTSKe本nelSa成eCo設置設置andExtension::GetSt本ate成yReco設置設置endation() const
{
    if (St本ate成yA使tho本ity)
    {
        TA本本ay<ESixSt本ate成yType> Reco設置設置endations = St本ate成yA使tho本ity->GetReco設置設置endedSt本ate成ies();
        if (Reco設置設置endations.的使設置() > 0)
        {
            本et使本n St本ate成yA使tho本ity->GetSt本ate成yDesc本iption(Reco設置設置endations[0]);
        }
    }

    if (基本使Xin成Rhyth設置Syste設置)
    {
        本et使本n 基本使Xin成Rhyth設置Syste設置->GetReco設置設置endedSt本ate成y軍o本C使本本entPhase();
    }

    本et使本n TEXT("策略系統未初始化");
}

軍St本in成 UMin成RTSKe本nelSa成eCo設置設置andExtension::Get基本使Xin成Reco設置設置endation() const
{
    if (基本使Xin成Rhyth設置Syste設置)
    {
        本et使本n 基本使Xin成Rhyth設置Syste設置->GetReco設置設置endedSt本ate成y軍o本C使本本entPhase();
    }

    本et使本n TEXT("五行系統未初始化");
}

bool UMin成RTSKe本nelSa成eCo設置設置andExtension::IsSyste設置輸入ealthy() const
{
    bool b輸入ealthy = t本使e;

    if (Mo本alA使tho本ity)
    {
        b輸入ealthy &= Mo本alA使tho本ity->Is輸入ealthy();
    }

    if (St本ate成yA使tho本ity)
    {
        b輸入ealthy &= St本ate成yA使tho本ity->Is輸入ealthy();
    }

    if (Milita本yA使tho本ity)
    {
        b輸入ealthy &= Milita本yA使tho本ity->Is輸入ealthy();
    }

    if (Anti軍allSyste設置)
    {
        軍軍allStat使sData Stat使s = Anti軍allSyste設置->Get軍allStat使s();
        if (Stat使s.bIs軍allen)
        {
            b輸入ealthy = false;
        }
    }

    本et使本n b輸入ealthy;
}

軍Syste設置CallResponse UMin成RTSKe本nelSa成eCo設置設置andExtension::輸入andleSa成eCo設置設置andSyste設置Call(const 軍Syste設置CallReq使est& Req使est)
{
    軍Syste設置CallResponse Response;
    Response.Req使estID = Req使est.Req使estID;
    Response.Ti設置esta設置p = 軍DateTi設置e::的ow();

    // 將通用系統調用轉換為指揮學系統調用
    軍Sa成eCo設置設置andKe本nelReq使est Sa成eReq使est;

    // 從參數中解析調用類型
    if (Req使est.Pa本a設置ete本s.的使設置() > 0)
    {
        Sa成eReq使est.CallType = static下cast<ESa成eCo設置設置andSyste設置Call>(Req使est.Pa本a設置ete本s[0]);
    }

    // 執行指揮學調用
    軍Sa成eCo設置設置andKe本nelResponse Sa成eResponse = Exec使teSa成eCo設置設置andCall(Sa成eReq使est);

    Response.bS使ccess = Sa成eResponse.bS使ccess;

    // 序列化響應
    // 簡化處理：將整數結果放入響應數據
    Response.Res使ltData.Add(static下cast<使int8>(Sa成eResponse.IntRes使lt));
    Response.Res使ltData.Add(static下cast<使int8>(Sa成eResponse.IntRes使lt >> 8));
    Response.Res使ltData.Add(static下cast<使int8>(Sa成eResponse.IntRes使lt >> 16));
    Response.Res使ltData.Add(static下cast<使int8>(Sa成eResponse.IntRes使lt >> 24));

    if (!Sa成eResponse.bS使ccess)
    {
        Response.E本本o本Messa成e = Sa成eResponse.E本本o本Messa成e;
    }

    本et使本n Response;
}

正oid UMin成RTSKe本nelSa成eCo設置設置andExtension::OnKe本nelStateChan成ed(EKe本nelState OldState, EKe本nelState 的ewState)
{
    switch (的ewState)
    {
    case EKe本nelState::R使nnin成:
        // 內核啟動時初始化指揮學系統
        InitializeSa成eCo設置設置andExtension();
        b本eak;

    case EKe本nelState::Sh使ttin成Down:
        // 清理定時器
        if (UpdateTi設置e本輸入andle.IsValid())
        {
            if (U基本o本ld* 基本o本ld = Get基本o本ld())
            {
                基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(UpdateTi設置e本輸入andle);
            }
        }
        b本eak;

    defa使lt:
        b本eak;
    }
}

正oid UMin成RTSKe本nelSa成eCo設置設置andExtension::OnSyste設置Update()
{
    // 定期更新系統狀態
    UpdateSyste設置Stat使s();

    // 更新子系統
    if (St本ate成yA使tho本ity)
    {
        St本ate成yA使tho本ity->UpdateCooldowns();
    }

    if (基本使Xin成Rhyth設置Syste設置)
    {
        基本使Xin成Rhyth設置Syste設置->UpdateSyste設置();
    }

    // 檢查系統健康並發警告
    if (!CachedStat使s.bSyste設置輸入ealthy)
    {
        OnSa成eCo設置設置and基本a本nin成.B本oadcast(TEXT("至聖者指揮學系統處於異常狀態，請檢查三權平衡和墮落狀態。"));
    }
}
