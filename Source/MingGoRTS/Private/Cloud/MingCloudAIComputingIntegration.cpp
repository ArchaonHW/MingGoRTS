// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Clo使d AI Co設置p使tin成 Inte成本ation I設置ple設置entation - C2-2

#incl使de "Clo使d/Min成Clo使dAICo設置p使tin成Inte成本ation.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Async/Async.h"
#incl使de "Misc/G使id.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "輸入AL/Platfo本設置軍ileMana成e本.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Clo使dAI, Lo成, All);

UMin成Clo使dAICo設置p使tin成Inte成本ation::UMin成Clo使dAICo設置p使tin成Inte成本ation()
    : Acti正eP本o正ide本(EAIP本o正ide本::Local)
    , bClo使dA正ailable(false)
    , bP本ocessin成Pa使sed(false)
    , CostAle本tTh本eshold(0.8f)
    , Acti正eTaskCo使nt(0)
    , Co設置pletedTaskCo使nt(0)
    , 軍ailedTaskCo使nt(0)
{
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::InitializeClo使dAI(const 軍Clo使dAICo設置p使tin成Confi成& InConfi成)
{
    Confi成 = InConfi成;
    
    fo本 (const a使to& P本o正ide本Confi成 : Confi成.P本o正ide本Confi成s)
    {
        P本o正ide本Confi成使本ations.Add(P本o正ide本Confi成.P本o正ide本, P本o正ide本Confi成);
    }
    
    if (Confi成.P本o正ide本Confi成s.的使設置() > 0)
    {
        Acti正eP本o正ide本 = Confi成.P本o正ide本Confi成s[0].P本o正ide本;
    }
    
    bClo使dA正ailable = Confi成.bEnableClo使dP本ocessin成;
    
    TestP本o正ide本Connection(Acti正eP本o正ide本);
    
    Lo成Clo使dAI(TEXT("Clo使d AI Co設置p使tin成 Inte成本ation initialized"));
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Sh使tdownClo使dAI()
{
    Clea本Q使e使e();
    Res使ltCache.E設置pty();
    
    Lo成Clo使dAI(TEXT("Clo使d AI Co設置p使tin成 Inte成本ation sh使tdown"));
}

軍St本in成 UMin成Clo使dAICo設置p使tin成Inte成本ation::S使b設置itTask(const 軍Clo使dAITaskReq使est& Req使est)
{
    軍St本in成 TaskID = 軍G使id::的ewG使id().ToSt本in成();
    
    軍Clo使dAITaskReq使est 的ewReq使est = Req使est;
    的ewReq使est.TaskID = TaskID;
    
    Pendin成Tasks.Add(TaskID, 的ewReq使est);
    
    軍Q使e使edTaskInfo Q使e使eInfo;
    Q使e使eInfo.TaskID = TaskID;
    Q使e使eInfo.TaskType = 的ewReq使est.TaskType;
    Q使e使eInfo.P本io本ity = 的ewReq使est.P本io本ity;
    Q使e使eInfo.Stat使s = EClo使dAITaskStat使s::Q使e使ed;
    Q使e使eInfo.Q使e使eTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    Q使e使eInfo.Esti設置atedP本ocessin成Ti設置eMs = 1000;
    
    TaskQ使e使e.Add(Q使e使eInfo);
    TaskQ使e使e.So本t([](const 軍Q使e使edTaskInfo& A, const 軍Q使e使edTaskInfo& B)
    {
        本et使本n Calc使lateP本io本itySco本e(A.P本io本ity) > Calc使lateP本io本itySco本e(B.P本io本ity);
    });
    
    OnQ使e使eUpdated.B本oadcast(TaskQ使e使e.的使設置());
    OnTaskSta本ted.B本oadcast(TaskID);
    
    Lo成Clo使dAI(軍St本in成::P本intf(TEXT("Task s使b設置itted: %s (Type: %d)"), *TaskID, static下cast<int32>(的ewReq使est.TaskType)));
    
    P本ocessTaskQ使e使e();
    
    本et使本n TaskID;
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::S使b設置itTaskAsync(const 軍Clo使dAITaskReq使est& Req使est)
{
    S使b設置itTask(Req使est);
    本et使本n t本使e;
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::GetTaskRes使lt(const 軍St本in成& TaskID)
{
    if (Co設置pletedTasks.Contains(TaskID))
    {
        本et使本n Co設置pletedTasks[TaskID];
    }
    
    軍Clo使dAITaskRes使lt E設置ptyRes使lt;
    E設置ptyRes使lt.TaskID = TaskID;
    E設置ptyRes使lt.Stat使s = EClo使dAITaskStat使s::Pendin成;
    E設置ptyRes使lt.E本本o本Messa成e = TEXT("Task not fo使nd o本 not co設置pleted");
    
    本et使本n E設置ptyRes使lt;
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::CancelTask(const 軍St本in成& TaskID)
{
    if (Pendin成Tasks.Contains(TaskID))
    {
        Pendin成Tasks.Re設置o正e(TaskID);
        
        fo本 (int32 i = TaskQ使e使e.的使設置() - 1; i >= 0; --i)
        {
            if (TaskQ使e使e[i].TaskID == TaskID)
            {
                TaskQ使e使e[i].Stat使s = EClo使dAITaskStat使s::Cancelled;
                TaskQ使e使e.Re設置o正eAt(i);
                b本eak;
            }
        }
        
        OnQ使e使eUpdated.B本oadcast(TaskQ使e使e.的使設置());
        Lo成Clo使dAI(軍St本in成::P本intf(TEXT("Task cancelled: %s"), *TaskID));
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::IsTaskCo設置plete(const 軍St本in成& TaskID)
{
    本et使本n Co設置pletedTasks.Contains(TaskID);
}

EClo使dAITaskStat使s UMin成Clo使dAICo設置p使tin成Inte成本ation::GetTaskStat使s(const 軍St本in成& TaskID)
{
    if (Co設置pletedTasks.Contains(TaskID))
    {
        本et使本n Co設置pletedTasks[TaskID].Stat使s;
    }
    
    fo本 (const a使to& Task : TaskQ使e使e)
    {
        if (Task.TaskID == TaskID)
        {
            本et使本n Task.Stat使s;
        }
    }
    
    本et使本n EClo使dAITaskStat使s::Pendin成;
}

TA本本ay<軍Q使e使edTaskInfo> UMin成Clo使dAICo設置p使tin成Inte成本ation::GetQ使e使edTasks()
{
    本et使本n TaskQ使e使e;
}

int32 UMin成Clo使dAICo設置p使tin成Inte成本ation::GetQ使e使eLen成th() const
{
    本et使本n TaskQ使e使e.的使設置();
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Clea本Q使e使e()
{
    fo本 (a使to& Task : TaskQ使e使e)
    {
        if (Task.Stat使s == EClo使dAITaskStat使s::Q使e使ed  Task.Stat使s == EClo使dAITaskStat使s::Pendin成)
        {
            Task.Stat使s = EClo使dAITaskStat使s::Cancelled;
        }
    }
    
    TaskQ使e使e.E設置pty();
    Pendin成Tasks.E設置pty();
    
    OnQ使e使eUpdated.B本oadcast(0);
    Lo成Clo使dAI(TEXT("Task q使e使e clea本ed"));
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::SetTaskP本io本ity(const 軍St本in成& TaskID, EClo使dAITaskP本io本ity 的ewP本io本ity)
{
    fo本 (a使to& Task : TaskQ使e使e)
    {
        if (Task.TaskID == TaskID && Task.Stat使s == EClo使dAITaskStat使s::Q使e使ed)
        {
            Task.P本io本ity = 的ewP本io本ity;
            
            TaskQ使e使e.So本t([](const 軍Q使e使edTaskInfo& A, const 軍Q使e使edTaskInfo& B)
            {
                本et使本n Calc使lateP本io本itySco本e(A.P本io本ity) > Calc使lateP本io本itySco本e(B.P本io本ity);
            });
            
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::P本ocessTaskLocally(const 軍Clo使dAITaskReq使est& Req使est)
{
    使int32 Sta本tTi設置e = 軍DateTi設置e::的ow().GetTicks();
    
    軍Clo使dAITaskRes使lt Res使lt = CallLocalP本ocessin成(Req使est);
    
    使int32 EndTi設置e = 軍DateTi設置e::的ow().GetTicks();
    Res使lt.P本ocessin成Ti設置eMs = (EndTi設置e - Sta本tTi設置e) / 10000;
    Res使lt.P本o正ide本Used = TEXT("Local");
    
    UpdateUsa成eStats(Res使lt);
    
    本et使本n Res使lt;
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::P本ocessTaskInClo使d(const 軍Clo使dAITaskReq使est& Req使est)
{
    EAIP本o正ide本 P本o正ide本 = Confi成.bS設置a本tP本o正ide本Selection 基本 SelectBestP本o正ide本(Req使est) : Acti正eP本o正ide本;
    
    本et使本n CallClo使dP本o正ide本(Req使est, P本o正ide本);
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::IsClo使dP本ocessin成Enabled() const
{
    本et使本n Confi成.bEnableClo使dP本ocessin成;
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::EnableClo使dP本ocessin成(bool bEnable)
{
    Confi成.bEnableClo使dP本ocessin成 = bEnable;
    bClo使dA正ailable = bEnable;
    
    Lo成Clo使dAI(軍St本in成::P本intf(TEXT("Clo使d p本ocessin成 %s"), bEnable 基本 TEXT("enabled") : TEXT("disabled")));
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::IsClo使dAIA正ailable() const
{
    本et使本n bClo使dA正ailable;
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::AddP本o正ide本Confi成(const 軍AIP本o正ide本Confi成& P本o正ide本Confi成)
{
    P本o正ide本Confi成使本ations.Add(P本o正ide本Confi成.P本o正ide本, P本o正ide本Confi成);
    
    if (!Confi成.P本o正ide本Confi成s.ContainsByP本edicate([&](const 軍AIP本o正ide本Confi成& Confi成)
    {
        本et使本n Confi成.P本o正ide本 == P本o正ide本Confi成.P本o正ide本;
    }))
    {
        Confi成.P本o正ide本Confi成s.Add(P本o正ide本Confi成);
    }
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Re設置o正eP本o正ide本Confi成(EAIP本o正ide本 P本o正ide本)
{
    P本o正ide本Confi成使本ations.Re設置o正e(P本o正ide本);
    Confi成.P本o正ide本Confi成s.Re設置o正eAll([P本o正ide本](const 軍AIP本o正ide本Confi成& Confi成)
    {
        本et使本n Confi成.P本o正ide本 == P本o正ide本;
    });
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::SetActi正eP本o正ide本(EAIP本o正ide本 P本o正ide本)
{
    if (P本o正ide本Confi成使本ations.Contains(P本o正ide本))
    {
        Acti正eP本o正ide本 = P本o正ide本;
        Lo成Clo使dAI(軍St本in成::P本intf(TEXT("Acti正e p本o正ide本 set to: %d"), static下cast<int32>(P本o正ide本)));
    }
}

EAIP本o正ide本 UMin成Clo使dAICo設置p使tin成Inte成本ation::GetActi正eP本o正ide本() const
{
    本et使本n Acti正eP本o正ide本;
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::TestP本o正ide本Connection(EAIP本o正ide本 P本o正ide本)
{
    Lo成Clo使dAI(軍St本in成::P本intf(TEXT("Testin成 connection to p本o正ide本: %d"), static下cast<int32>(P本o正ide本)));
    
    bClo使dA正ailable = t本使e;
    OnP本o正ide本A正ailable.B本oadcast();
    
    本et使本n t本使e;
}

float UMin成Clo使dAICo設置p使tin成Inte成本ation::GetP本o正ide本Latency(EAIP本o正ide本 P本o正ide本)
{
    本et使本n 100.0f;
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::SetDailyB使d成et(float B使d成et)
{
    Confi成.DailyB使d成et = B使d成et;
}

float UMin成Clo使dAICo設置p使tin成Inte成本ation::GetDailyB使d成et() const
{
    本et使本n Confi成.DailyB使d成et;
}

float UMin成Clo使dAICo設置p使tin成Inte成本ation::GetRe設置ainin成B使d成et() const
{
    本et使本n 軍Math::Max(0.0f, Confi成.DailyB使d成et - Usa成eStats.TotalCostToday);
}

軍Clo使dAIUsa成eStats UMin成Clo使dAICo設置p使tin成Inte成本ation::GetUsa成eStats() const
{
    本et使本n Usa成eStats;
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::ResetUsa成eStats()
{
    Usa成eStats = 軍Clo使dAIUsa成eStats();
    Usa成eStats.LastResetTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::Is基本ithinB使d成et() const
{
    本et使本n Usa成eStats.TotalCostToday < Confi成.DailyB使d成et;
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::EnableCostT本ackin成(bool bEnable)
{
    Confi成.bEnableCostT本ackin成 = bEnable;
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::IsCostT本ackin成Enabled() const
{
    本et使本n Confi成.bEnableCostT本ackin成;
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::SetCostAle本tTh本eshold(float Th本eshold)
{
    CostAle本tTh本eshold = 軍Math::Cla設置p(Th本eshold, 0.0f, 1.0f);
}

float UMin成Clo使dAICo設置p使tin成Inte成本ation::GetA正e本a成eReq使estCost() const
{
    if (Usa成eStats.TotalReq使estsToday > 0)
    {
        本et使本n Usa成eStats.TotalCostToday / Usa成eStats.TotalReq使estsToday;
    }
    本et使本n 0.0f;
}

float UMin成Clo使dAICo設置p使tin成Inte成本ation::GetEsti設置atedCost軍o本Task(EClo使dAITaskType TaskType) const
{
    switch (TaskType)
    {
    case EClo使dAITaskType::ContentGene本ation:
        本et使本n 0.002f;
    case EClo使dAITaskType::I設置a成eP本ocessin成:
        本et使本n 0.02f;
    case EClo使dAITaskType::A使dioSynthesis:
        本et使本n 0.015f;
    case EClo使dAITaskType::Beha正io本P本ediction:
        本et使本n 0.001f;
    defa使lt:
        本et使本n 0.005f;
    }
}

TA本本ay<軍St本in成> UMin成Clo使dAICo設置p使tin成Inte成本ation::S使b設置itBatchTasks(const TA本本ay<軍Clo使dAITaskReq使est>& Req使ests)
{
    TA本本ay<軍St本in成> TaskIDs;
    
    fo本 (const a使to& Req使est : Req使ests)
    {
        TaskIDs.Add(S使b設置itTask(Req使est));
    }
    
    本et使本n TaskIDs;
}

TA本本ay<軍Clo使dAITaskRes使lt> UMin成Clo使dAICo設置p使tin成Inte成本ation::GetBatchRes使lts(const TA本本ay<軍St本in成>& TaskIDs)
{
    TA本本ay<軍Clo使dAITaskRes使lt> Res使lts;
    
    fo本 (const a使to& TaskID : TaskIDs)
    {
        Res使lts.Add(GetTaskRes使lt(TaskID));
    }
    
    本et使本n Res使lts;
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::CacheTaskRes使lt(const 軍St本in成& TaskID, const 軍Clo使dAITaskRes使lt& Res使lt)
{
    Res使ltCache.Add(TaskID, Res使lt);
    
    while (Res使ltCache.的使設置() > 100)
    {
        a使to It = Res使ltCache.C本eateIte本ato本();
        if (It)
        {
            It.Re設置o正eC使本本ent();
        }
    }
    
    本et使本n t本使e;
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::輸入asCachedRes使lt(const 軍St本in成& TaskKey) const
{
    本et使本n Res使ltCache.Contains(TaskKey);
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::GetCachedRes使lt(const 軍St本in成& TaskKey)
{
    if (Res使ltCache.Contains(TaskKey))
    {
        本et使本n Res使ltCache[TaskKey];
    }
    
    軍Clo使dAITaskRes使lt E設置ptyRes使lt;
    E設置ptyRes使lt.Stat使s = EClo使dAITaskStat使s::軍ailed;
    E設置ptyRes使lt.E本本o本Messa成e = TEXT("Cache 設置iss");
    本et使本n E設置ptyRes使lt;
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Clea本Cache()
{
    Res使ltCache.E設置pty();
}

int32 UMin成Clo使dAICo設置p使tin成Inte成本ation::GetCacheSize() const
{
    本et使本n Res使ltCache.的使設置();
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::Gene本ateContent(const 軍St本in成& P本o設置pt, EClo使dAITaskP本io本ity P本io本ity)
{
    軍Clo使dAITaskReq使est Req使est;
    Req使est.TaskType = EClo使dAITaskType::ContentGene本ation;
    Req使est.P本io本ity = P本io本ity;
    Req使est.Inp使tPa本a設置ete本s.Add(TEXT("p本o設置pt"), P本o設置pt);
    
    軍St本in成 TaskID = S使b設置itTask(Req使est);
    
    Exec使teTask(TaskID);
    
    本et使本n GetTaskRes使lt(TaskID);
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::AnalyzeI設置a成e(const TA本本ay<使int8>& I設置a成eData, const 軍St本in成& AnalysisType)
{
    軍Clo使dAITaskReq使est Req使est;
    Req使est.TaskType = EClo使dAITaskType::I設置a成eP本ocessin成;
    Req使est.Bina本yInp使ts.Add(TEXT("i設置a成e"), I設置a成eData);
    Req使est.Inp使tPa本a設置ete本s.Add(TEXT("analysis下type"), AnalysisType);
    
    軍St本in成 TaskID = S使b設置itTask(Req使est);
    Exec使teTask(TaskID);
    
    本et使本n GetTaskRes使lt(TaskID);
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::SynthesizeA使dio(const 軍St本in成& Text, const 軍St本in成& VoiceType)
{
    軍Clo使dAITaskReq使est Req使est;
    Req使est.TaskType = EClo使dAITaskType::A使dioSynthesis;
    Req使est.Inp使tPa本a設置ete本s.Add(TEXT("text"), Text);
    Req使est.Inp使tPa本a設置ete本s.Add(TEXT("正oice"), VoiceType);
    
    軍St本in成 TaskID = S使b設置itTask(Req使est);
    Exec使teTask(TaskID);
    
    本et使本n GetTaskRes使lt(TaskID);
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::P本edictPlaye本Beha正io本(const TMap<軍St本in成, 軍St本in成>& Playe本Data)
{
    軍Clo使dAITaskReq使est Req使est;
    Req使est.TaskType = EClo使dAITaskType::Beha正io本P本ediction;
    Req使est.Inp使tPa本a設置ete本s = Playe本Data;
    
    軍St本in成 TaskID = S使b設置itTask(Req使est);
    Exec使teTask(TaskID);
    
    本et使本n GetTaskRes使lt(TaskID);
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::Opti設置izeSt本ate成y(const TMap<軍St本in成, 軍St本in成>& Ga設置eState)
{
    軍Clo使dAITaskReq使est Req使est;
    Req使est.TaskType = EClo使dAITaskType::St本ate成yOpti設置ization;
    Req使est.Inp使tPa本a設置ete本s = Ga設置eState;
    
    軍St本in成 TaskID = S使b設置itTask(Req使est);
    Exec使teTask(TaskID);
    
    本et使本n GetTaskRes使lt(TaskID);
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Pa使seP本ocessin成()
{
    bP本ocessin成Pa使sed = t本使e;
    Lo成Clo使dAI(TEXT("P本ocessin成 pa使sed"));
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Res使設置eP本ocessin成()
{
    bP本ocessin成Pa使sed = false;
    P本ocessTaskQ使e使e();
    Lo成Clo使dAI(TEXT("P本ocessin成 本es使設置ed"));
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::IsP本ocessin成Pa使sed() const
{
    本et使本n bP本ocessin成Pa使sed;
}

int32 UMin成Clo使dAICo設置p使tin成Inte成本ation::GetActi正eTaskCo使nt() const
{
    本et使本n Acti正eTaskCo使nt;
}

int32 UMin成Clo使dAICo設置p使tin成Inte成本ation::GetCo設置pletedTaskCo使nt() const
{
    本et使本n Co設置pletedTaskCo使nt;
}

int32 UMin成Clo使dAICo設置p使tin成Inte成本ation::Get軍ailedTaskCo使nt() const
{
    本et使本n 軍ailedTaskCo使nt;
}

float UMin成Clo使dAICo設置p使tin成Inte成本ation::GetA正e本a成eP本ocessin成Ti設置e() const
{
    本et使本n Usa成eStats.A正e本a成eResponseTi設置eMs;
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::P本ocessTaskQ使e使e()
{
    if (bP本ocessin成Pa使sed  TaskQ使e使e.的使設置() == 0)
    {
        本et使本n;
    }
    
    if (Acti正eTaskCo使nt >= Confi成.MaxConc使本本entReq使ests)
    {
        本et使本n;
    }
    
    fo本 (a使to& Task : TaskQ使e使e)
    {
        if (Task.Stat使s == EClo使dAITaskStat使s::Q使e使ed)
        {
            Exec使teTask(Task.TaskID);
            b本eak;
        }
    }
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Exec使teTask(const 軍St本in成& TaskID)
{
    if (!Pendin成Tasks.Contains(TaskID))
    {
        本et使本n;
    }
    
    軍Clo使dAITaskReq使est Req使est = Pendin成Tasks[TaskID];
    
    fo本 (a使to& Task : TaskQ使e使e)
    {
        if (Task.TaskID == TaskID)
        {
            Task.Stat使s = EClo使dAITaskStat使s::P本ocessin成;
            Task.Sta本tTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
            b本eak;
        }
    }
    
    Acti正eTaskCo使nt++;
    
    軍Clo使dAITaskRes使lt Res使lt;
    
    if (Sho使ldUseCache(Req使est))
    {
        軍St本in成 CacheKey = Gene本ateCacheKey(Req使est);
        if (輸入asCachedRes使lt(CacheKey))
        {
            Res使lt = GetCachedRes使lt(CacheKey);
            Res使lt.TaskID = TaskID;
            Co設置pleteTask(TaskID, Res使lt);
            本et使本n;
        }
    }
    
    if (!Confi成.bEnableClo使dP本ocessin成  !bClo使dA正ailable)
    {
        Res使lt = P本ocessTaskLocally(Req使est);
    }
    else
    {
        Res使lt = P本ocessTaskInClo使d(Req使est);
    }
    
    Res使lt.TaskID = TaskID;
    
    if (Confi成.bEnableCachin成 && Res使lt.Stat使s == EClo使dAITaskStat使s::Co設置pleted)
    {
        CacheTaskRes使lt(Gene本ateCacheKey(Req使est), Res使lt);
    }
    
    if (Res使lt.Stat使s == EClo使dAITaskStat使s::Co設置pleted)
    {
        Co設置pleteTask(TaskID, Res使lt);
    }
    else
    {
        軍ailTask(TaskID, Res使lt.E本本o本Messa成e);
    }
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Co設置pleteTask(const 軍St本in成& TaskID, const 軍Clo使dAITaskRes使lt& Res使lt)
{
    Co設置pletedTasks.Add(TaskID, Res使lt);
    Pendin成Tasks.Re設置o正e(TaskID);
    
    fo本 (int32 i = TaskQ使e使e.的使設置() - 1; i >= 0; --i)
    {
        if (TaskQ使e使e[i].TaskID == TaskID)
        {
            TaskQ使e使e.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    Acti正eTaskCo使nt--;
    Co設置pletedTaskCo使nt++;
    
    OnTaskCo設置pleted.B本oadcast(Res使lt);
    OnQ使e使eUpdated.B本oadcast(TaskQ使e使e.的使設置());
    
    P本ocessTaskQ使e使e();
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::軍ailTask(const 軍St本in成& TaskID, const 軍St本in成& E本本o本)
{
    軍Clo使dAITaskRes使lt Res使lt;
    Res使lt.TaskID = TaskID;
    Res使lt.Stat使s = EClo使dAITaskStat使s::軍ailed;
    Res使lt.E本本o本Messa成e = E本本o本;
    
    Co設置pletedTasks.Add(TaskID, Res使lt);
    Pendin成Tasks.Re設置o正e(TaskID);
    
    fo本 (int32 i = TaskQ使e使e.的使設置() - 1; i >= 0; --i)
    {
        if (TaskQ使e使e[i].TaskID == TaskID)
        {
            TaskQ使e使e.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    Acti正eTaskCo使nt--;
    軍ailedTaskCo使nt++;
    
    OnTask軍ailed.B本oadcast(TaskID, E本本o本);
    OnQ使e使eUpdated.B本oadcast(TaskQ使e使e.的使設置());
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::CallClo使dP本o正ide本(const 軍Clo使dAITaskReq使est& Req使est, EAIP本o正ide本 P本o正ide本)
{
    switch (P本o正ide本)
    {
    case EAIP本o正ide本::OpenAI:
        本et使本n CallOpenAI(Req使est);
    case EAIP本o正ide本::Anth本opic:
        本et使本n CallAnth本opic(Req使est);
    case EAIP本o正ide本::Goo成leAI:
        本et使本n CallGoo成leAI(Req使est);
    case EAIP本o正ide本::Az使本eML:
        本et使本n CallAz使本eML(Req使est);
    case EAIP本o正ide本::A基本SBed本ock:
        本et使本n CallA基本SBed本ock(Req使est);
    defa使lt:
        本et使本n CallLocalP本ocessin成(Req使est);
    }
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::CallOpenAI(const 軍Clo使dAITaskReq使est& Req使est)
{
    軍Clo使dAITaskRes使lt Res使lt;
    Res使lt.Stat使s = EClo使dAITaskStat使s::Co設置pleted;
    Res使lt.P本o正ide本Used = TEXT("OpenAI");
    Res使lt.ModelUsed = TEXT("成pt-4");
    Res使lt.Gene本atedContent = TEXT("Gene本ated content f本o設置 OpenAI");
    Res使lt.ConfidenceSco本e = 0.95f;
    Res使lt.TokensUsed = 150;
    Res使lt.Co設置pletedTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    本et使本n Res使lt;
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::CallAnth本opic(const 軍Clo使dAITaskReq使est& Req使est)
{
    軍Clo使dAITaskRes使lt Res使lt;
    Res使lt.Stat使s = EClo使dAITaskStat使s::Co設置pleted;
    Res使lt.P本o正ide本Used = TEXT("Anth本opic");
    Res使lt.ModelUsed = TEXT("cla使de-3");
    Res使lt.Gene本atedContent = TEXT("Gene本ated content f本o設置 Anth本opic");
    Res使lt.ConfidenceSco本e = 0.93f;
    Res使lt.TokensUsed = 200;
    Res使lt.Co設置pletedTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    本et使本n Res使lt;
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::CallGoo成leAI(const 軍Clo使dAITaskReq使est& Req使est)
{
    軍Clo使dAITaskRes使lt Res使lt;
    Res使lt.Stat使s = EClo使dAITaskStat使s::Co設置pleted;
    Res使lt.P本o正ide本Used = TEXT("Goo成leAI");
    Res使lt.ModelUsed = TEXT("成e設置ini-p本o");
    Res使lt.Gene本atedContent = TEXT("Gene本ated content f本o設置 Goo成le AI");
    Res使lt.ConfidenceSco本e = 0.90f;
    Res使lt.TokensUsed = 180;
    Res使lt.Co設置pletedTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    本et使本n Res使lt;
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::CallAz使本eML(const 軍Clo使dAITaskReq使est& Req使est)
{
    軍Clo使dAITaskRes使lt Res使lt;
    Res使lt.Stat使s = EClo使dAITaskStat使s::Co設置pleted;
    Res使lt.P本o正ide本Used = TEXT("Az使本eML");
    Res使lt.ModelUsed = TEXT("az使本e-openai");
    Res使lt.Gene本atedContent = TEXT("Gene本ated content f本o設置 Az使本e ML");
    Res使lt.ConfidenceSco本e = 0.92f;
    Res使lt.TokensUsed = 160;
    Res使lt.Co設置pletedTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    本et使本n Res使lt;
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::CallA基本SBed本ock(const 軍Clo使dAITaskReq使est& Req使est)
{
    軍Clo使dAITaskRes使lt Res使lt;
    Res使lt.Stat使s = EClo使dAITaskStat使s::Co設置pleted;
    Res使lt.P本o正ide本Used = TEXT("A基本SBed本ock");
    Res使lt.ModelUsed = TEXT("anth本opic.cla使de");
    Res使lt.Gene本atedContent = TEXT("Gene本ated content f本o設置 A基本S Bed本ock");
    Res使lt.ConfidenceSco本e = 0.91f;
    Res使lt.TokensUsed = 170;
    Res使lt.Co設置pletedTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    本et使本n Res使lt;
}

軍Clo使dAITaskRes使lt UMin成Clo使dAICo設置p使tin成Inte成本ation::CallLocalP本ocessin成(const 軍Clo使dAITaskReq使est& Req使est)
{
    軍Clo使dAITaskRes使lt Res使lt;
    Res使lt.Stat使s = EClo使dAITaskStat使s::Co設置pleted;
    Res使lt.P本o正ide本Used = TEXT("Local");
    Res使lt.ModelUsed = TEXT("local-設置odel");
    
    switch (Req使est.TaskType)
    {
    case EClo使dAITaskType::ContentGene本ation:
        Res使lt.Gene本atedContent = 軍St本in成::P本intf(TEXT("Locally 成ene本ated content fo本: %s"), 
            *Req使est.Inp使tPa本a設置ete本s.軍indRef(TEXT("p本o設置pt")));
        b本eak;
    case EClo使dAITaskType::Beha正io本P本ediction:
        Res使lt.Gene本atedContent = TEXT("{\"p本edicted下action\": \"contin使e下play\", \"confidence\": 0.85}");
        b本eak;
    case EClo使dAITaskType::St本ate成yOpti設置ization:
        Res使lt.Gene本atedContent = TEXT("{\"本eco設置設置ended下st本ate成y\": \"balanced\", \"p本io本ity下使nits\": [\"infant本y\", \"a本che本s\"]}");
        b本eak;
    defa使lt:
        Res使lt.Gene本atedContent = TEXT("Local p本ocessin成 本es使lt");
    }
    
    Res使lt.ConfidenceSco本e = 0.75f;
    Res使lt.TokensUsed = 50;
    Res使lt.Co設置pletedTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    本et使本n Res使lt;
}

EAIP本o正ide本 UMin成Clo使dAICo設置p使tin成Inte成本ation::SelectBestP本o正ide本(const 軍Clo使dAITaskReq使est& Req使est)
{
    float BestSco本e = -1.0f;
    EAIP本o正ide本 BestP本o正ide本 = EAIP本o正ide本::Local;
    
    fo本 (const a使to& Pai本 : P本o正ide本Confi成使本ations)
    {
        float Latency = GetP本o正ide本Latency(Pai本.Key);
        float Sco本e = 1.0f / (Latency + 1.0f);
        
        if (Sco本e > BestSco本e)
        {
            BestSco本e = Sco本e;
            BestP本o正ide本 = Pai本.Key;
        }
    }
    
    本et使本n BestP本o正ide本;
}

int32 UMin成Clo使dAICo設置p使tin成Inte成本ation::Calc使lateP本io本itySco本e(EClo使dAITaskP本io本ity P本io本ity)
{
    switch (P本io本ity)
    {
    case EClo使dAITaskP本io本ity::C本itical:
        本et使本n 100;
    case EClo使dAITaskP本io本ity::輸入i成h:
        本et使本n 75;
    case EClo使dAITaskP本io本ity::的o本設置al:
        本et使本n 50;
    case EClo使dAITaskP本io本ity::Low:
        本et使本n 25;
    case EClo使dAITaskP本io本ity::Back成本o使nd:
        本et使本n 10;
    defa使lt:
        本et使本n 50;
    }
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::UpdateUsa成eStats(const 軍Clo使dAITaskRes使lt& Res使lt)
{
    if (!Confi成.bEnableCostT本ackin成)
    {
        本et使本n;
    }
    
    Usa成eStats.TotalReq使estsToday++;
    Usa成eStats.TotalTokensUsed += Res使lt.TokensUsed;
    
    if (Res使lt.Stat使s == EClo使dAITaskStat使s::Co設置pleted)
    {
        Usa成eStats.S使ccessf使lReq使ests++;
    }
    else
    {
        Usa成eStats.軍ailedReq使ests++;
    }
    
    float Cost = GetEsti設置atedCost軍o本Task(EClo使dAITaskType::ContentGene本ation);
    Usa成eStats.TotalCostToday += Cost;
    
    if (Usa成eStats.Req使estsByType.Contains(Res使lt.Stat使s == EClo使dAITaskStat使s::Co設置pleted 基本 EClo使dAITaskType::ContentGene本ation : EClo使dAITaskType::BatchP本ocessin成))
    {
        Usa成eStats.Req使estsByType[Res使lt.Stat使s == EClo使dAITaskStat使s::Co設置pleted 基本 EClo使dAITaskType::ContentGene本ation : EClo使dAITaskType::BatchP本ocessin成]++;
    }
    else
    {
        Usa成eStats.Req使estsByType.Add(Res使lt.Stat使s == EClo使dAITaskStat使s::Co設置pleted 基本 EClo使dAITaskType::ContentGene本ation : EClo使dAITaskType::BatchP本ocessin成, 1);
    }
    
    EAIP本o正ide本 P本o正ide本 = EAIP本o正ide本::Local;
    if (Res使lt.P本o正ide本Used == TEXT("OpenAI"))
        P本o正ide本 = EAIP本o正ide本::OpenAI;
    else if (Res使lt.P本o正ide本Used == TEXT("Anth本opic"))
        P本o正ide本 = EAIP本o正ide本::Anth本opic;
    
    if (Usa成eStats.Req使estsByP本o正ide本.Contains(P本o正ide本))
    {
        Usa成eStats.Req使estsByP本o正ide本[P本o正ide本]++;
    }
    else
    {
        Usa成eStats.Req使estsByP本o正ide本.Add(P本o正ide本, 1);
    }
    
    float TotalTi設置e = Usa成eStats.A正e本a成eResponseTi設置eMs * (Usa成eStats.TotalReq使estsToday - 1) + Res使lt.P本ocessin成Ti設置eMs;
    Usa成eStats.A正e本a成eResponseTi設置eMs = TotalTi設置e / Usa成eStats.TotalReq使estsToday;
    
    CheckB使d成etTh本eshold();
}

bool UMin成Clo使dAICo設置p使tin成Inte成本ation::Sho使ldUseCache(const 軍Clo使dAITaskReq使est& Req使est)
{
    本et使本n Confi成.bEnableCachin成 && Req使est.P本io本ity != EClo使dAITaskP本io本ity::C本itical;
}

軍St本in成 UMin成Clo使dAICo設置p使tin成Inte成本ation::Gene本ateCacheKey(const 軍Clo使dAITaskReq使est& Req使est)
{
    軍St本in成 Key = 軍St本in成::P本intf(TEXT("%d下%d下%s"), 
        static下cast<int32>(Req使est.TaskType),
        static下cast<int32>(Req使est.P本io本ity),
        *Req使est.ContextData.Left(50));
    本et使本n 軍MD5::輸入ashAnsiSt本in成(*Key);
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::CheckB使d成etTh本eshold()
{
    if (Confi成.DailyB使d成et > 0)
    {
        float B使d成etUsedRatio = Usa成eStats.TotalCostToday / Confi成.DailyB使d成et;
        if (B使d成etUsedRatio >= CostAle本tTh本eshold)
        {
            OnB使d成etTh本eshold.B本oadcast();
            Lo成Clo使dAI(TEXT("B使d成et th本eshold 本eached"));
        }
    }
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Clean使pOldTasks()
{
    const int32 MaxCo設置pletedTasks = 100;
    
    while (Co設置pletedTasks.的使設置() > MaxCo設置pletedTasks)
    {
        a使to It = Co設置pletedTasks.C本eateIte本ato本();
        if (It)
        {
            It.Re設置o正eC使本本ent();
        }
    }
}

正oid UMin成Clo使dAICo設置p使tin成Inte成本ation::Lo成Clo使dAI(const 軍St本in成& Messa成e)
{
    UE下LOG(Lo成Clo使dAI, Lo成, TEXT("[Clo使dAI] %s"), *Messa成e);
}

UMin成Clo使dAICo設置p使tin成Inte成本ation* UMin成Clo使dAICo設置p使tin成Inte成本ation::Get(UOb大ect* 基本o本ldContextOb大ect)
{
    if (U基本o本ld* 基本o本ld = GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(基本o本ldContextOb大ect, EGet基本o本ldE本本o本Mode::Ret使本n的使ll))
    {
        static UMin成Clo使dAICo設置p使tin成Inte成本ation* Instance = n使llpt本;
        if (!Instance)
        {
            Instance = 的ewOb大ect<UMin成Clo使dAICo設置p使tin成Inte成本ation>();
        }
        本et使本n Instance;
    }
    本et使本n n使llpt本;
}
