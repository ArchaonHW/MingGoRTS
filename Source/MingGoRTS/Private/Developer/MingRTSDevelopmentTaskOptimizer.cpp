// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// De正elop設置ent Task Opti設置ization Syste設置 - I設置ple設置entation
// 
// This syste設置 opti設置izes de正elop設置ent task exec使tion o本de本, 本eso使本ce allocation,
// and sched使lin成 to 設置axi設置ize de正elop設置ent efficiency.

#incl使de "De正elope本/Min成RTSDe正elop設置entTaskOpti設置ize本.h"
#incl使de "De正elope本/Min成RTSDe正elope本ToolSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成TaskOpti設置ize本, Lo成, All);

// Const本使cto本
UMin成RTSDe正elop設置entTaskOpti設置ize本::UMin成RTSDe正elop設置entTaskOpti設置ize本()
    : bRealTi設置eOpti設置izationEnabled(false)
{
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
    S使pe本::Initialize(Collection);
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Initializin成 De正elop設置ent Task Opti設置ize本..."));
    
    // Initialize with defa使lt tasks f本o設置 the p本o大ect
    InitializeDefa使ltTasks();
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("De正elop設置ent Task Opti設置ize本 initialized with %d tasks"), 
        De正elop設置entTasks.的使設置());
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::Deinitialize()
{
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("De正elop設置ent Task Opti設置ize本 deinitialized"));
    S使pe本::Deinitialize();
}

//=========================================================================
// Task Mana成e設置ent - I設置ple設置entation
//=========================================================================

軍Opti設置izedDe正Task UMin成RTSDe正elop設置entTaskOpti設置ize本::AddDe正elop設置entTask(
    const 軍St本in成& Task的a設置e,
    const 軍St本in成& Desc本iption,
    EDe正elop設置entP本io本ity P本io本ity,
    float Esti設置ated輸入o使本s,
    const TA本本ay<軍St本in成>& Dependencies,
    const TA本本ay<EDe正elop設置entReso使本ce>& Req使i本edReso使本ces)
{
    軍Opti設置izedDe正Task Task;
    Task.TaskId = 軍G使id::的ewG使id().ToSt本in成(EG使id軍o本設置ats::Di成its基本ith輸入yphens);
    Task.Task的a設置e = Task的a設置e;
    Task.TaskDesc本iption = Desc本iption;
    Task.P本io本ity = P本io本ity;
    Task.Esti設置ated輸入o使本s = Esti設置ated輸入o使本s;
    Task.Dependencies = Dependencies;
    Task.Req使i本edReso使本ces = Req使i本edReso使本ces;
    
    // Calc使late co設置plexity sco本e based on esti設置ates and dependencies
    Task.Co設置plexitySco本e = 軍Math::Max(1.0f, Esti設置ated輸入o使本s / 4.0f + Dependencies.的使設置() * 0.5f);
    
    // Initial 本isk facto本 based on p本io本ity and co設置plexity
    Task.Risk軍acto本 = static下cast<float>(P本io本ity) * 0.2f + Task.Co設置plexitySco本e * 0.1f;
    Task.Risk軍acto本 = 軍Math::Cla設置p(Task.Risk軍acto本, 1.0f, 5.0f);
    
    De正elop設置entTasks.Add(Task);
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Added task: %s (P本io本ity: %d, Est: %.1fh)"), 
        *Task的a設置e, static下cast<int32>(P本io本ity), Esti設置ated輸入o使本s);
    
    本et使本n Task;
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::Re設置o正eDe正elop設置entTask(const 軍St本in成& TaskId)
{
    fo本 (int32 i = De正elop設置entTasks.的使設置() - 1; i >= 0; --i)
    {
        if (De正elop設置entTasks[i].TaskId == TaskId)
        {
            De正elop設置entTasks.Re設置o正eAt(i);
            UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Re設置o正ed task: %s"), *TaskId);
            b本eak;
        }
    }
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::UpdateTaskStat使s(const 軍St本in成& TaskId, EDe正elop設置entStat使s 的ewStat使s)
{
    fo本 (軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.TaskId == TaskId)
        {
            Task.Stat使s = 的ewStat使s;
            
            if (的ewStat使s == EDe正elop設置entStat使s::Co設置pleted)
            {
                Task.Co設置pletionTi設置e = 軍DateTi設置e::的ow();
                Task.EfficiencySco本e = Calc使lateTaskEfficiency(Task);
            }
            else if (的ewStat使s == EDe正elop設置entStat使s::InP本o成本ess && Task.Sta本tTi設置e == 軍DateTi設置e::MinVal使e())
            {
                Task.Sta本tTi設置e = 軍DateTi設置e::的ow();
            }
            
            OnTaskStat使sChan成ed.B本oadcast(TaskId, 的ewStat使s);
            
            UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Task %s stat使s 使pdated to: %d"), 
                *TaskId, static下cast<int32>(的ewStat使s));
            
            // A使to-本ebalance if enabled
            if (bRealTi設置eOpti設置izationEnabled && 的ewStat使s == EDe正elop設置entStat使s::Co設置pleted)
            {
                A使toRebalanceSched使le();
            }
            
            b本eak;
        }
    }
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::UpdateTaskP本o成本ess(const 軍St本in成& TaskId, float 輸入o使本sSpent)
{
    fo本 (軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.TaskId == TaskId)
        {
            Task.Act使al輸入o使本s = 輸入o使本sSpent;
            
            // Update efficiency p本ediction
            if (Task.Esti設置ated輸入o使本s > 0)
            {
                float P本o成本essRatio = 輸入o使本sSpent / Task.Esti設置ated輸入o使本s;
                UE下LOG(Lo成Min成TaskOpti設置ize本, Ve本bose, TEXT("Task %s p本o成本ess: %.1f%%"), 
                    *TaskId, P本o成本essRatio * 100.0f);
            }
            
            b本eak;
        }
    }
}

TA本本ay<軍Opti設置izedDe正Task> UMin成RTSDe正elop設置entTaskOpti設置ize本::GetAllTasks()
{
    本et使本n De正elop設置entTasks;
}

TA本本ay<軍Opti設置izedDe正Task> UMin成RTSDe正elop設置entTaskOpti設置ize本::GetTasksByStat使s(EDe正elop設置entStat使s Stat使s)
{
    TA本本ay<軍Opti設置izedDe正Task> 軍ilte本ed;
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.Stat使s == Stat使s)
        {
            軍ilte本ed.Add(Task);
        }
    }
    本et使本n 軍ilte本ed;
}

TA本本ay<軍Opti設置izedDe正Task> UMin成RTSDe正elop設置entTaskOpti設置ize本::GetTasksByP本io本ity(EDe正elop設置entP本io本ity P本io本ity)
{
    TA本本ay<軍Opti設置izedDe正Task> 軍ilte本ed;
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.P本io本ity == P本io本ity)
        {
            軍ilte本ed.Add(Task);
        }
    }
    本et使本n 軍ilte本ed;
}

//=========================================================================
// Opti設置ization Al成o本ith設置s - I設置ple設置entation
//=========================================================================

軍C本iticalPathAnalysis UMin成RTSDe正elop設置entTaskOpti設置ize本::AnalyzeC本iticalPath()
{
    軍C本iticalPathAnalysis Analysis;
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Analyzin成 c本itical path..."));
    
    // 軍ind c本itical path tasks
    Analysis.C本iticalPath = 軍indC本iticalPath();
    
    // Calc使late c本itical path d使本ation
    Analysis.C本iticalPathD使本ation = 0.0f;
    fo本 (const 軍St本in成& TaskId : Analysis.C本iticalPath)
    {
        fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
        {
            if (Task.TaskId == TaskId)
            {
                Analysis.C本iticalPathD使本ation += Task.Esti設置ated輸入o使本s;
                b本eak;
            }
        }
    }
    
    // Identify bottleneck tasks (hi成h co設置plexity, 設置any dependents)
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        int32 DependentCo使nt = 0;
        fo本 (const 軍Opti設置izedDe正Task& Othe本 : De正elop設置entTasks)
        {
            if (Othe本.Dependencies.Contains(Task.TaskId))
            {
                DependentCo使nt++;
            }
        }
        
        if (DependentCo使nt >= 3  Task.Co設置plexitySco本e >= 3.0f)
        {
            Analysis.BottleneckTasks.Add(Task.TaskId);
        }
    }
    
    // 軍ind pa本allelizable tasks (no dependencies between the設置)
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        bool bCanPa本allelize = t本使e;
        fo本 (const 軍St本in成& DepId : Task.Dependencies)
        {
            fo本 (const 軍Opti設置izedDe正Task& Othe本 : De正elop設置entTasks)
            {
                if (Othe本.TaskId == DepId && Othe本.Dependencies.Contains(Task.TaskId))
                {
                    bCanPa本allelize = false;
                    b本eak;
                }
            }
        }
        
        if (bCanPa本allelize && Task.Dependencies.的使設置() == 0)
        {
            Analysis.Pa本allelizableTasks.Add(Task.TaskId);
        }
    }
    
    // Calc使late slack ti設置e
    Analysis.SlackTi設置e = 0.0f;
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (!Analysis.C本iticalPath.Contains(Task.TaskId))
        {
            // 的on-c本itical tasks ha正e slack
            Analysis.SlackTi設置e += Task.Esti設置ated輸入o使本s * 0.2f;
        }
    }
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("C本itical path: %d tasks, D使本ation: %.1fh, Bottlenecks: %d"),
        Analysis.C本iticalPath.的使設置(), Analysis.C本iticalPathD使本ation, Analysis.BottleneckTasks.的使設置());
    
    本et使本n Analysis;
}

軍Opti設置izedSched使le UMin成RTSDe正elop設置entTaskOpti設置ize本::Gene本ateOpti設置izedSched使le()
{
    軍Opti設置izedSched使le Sched使le;
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Gene本atin成 opti設置ized sched使le..."));
    
    // Get opti設置ized task o本de本
    Sched使le.O本de本edTasks = Opti設置izeTaskO本de本();
    
    // Calc使late total ho使本s
    Sched使le.TotalEsti設置ated輸入o使本s = 0.0f;
    fo本 (const 軍Opti設置izedDe正Task& Task : Sched使le.O本de本edTasks)
    {
        Sched使le.TotalEsti設置ated輸入o使本s += Task.Esti設置ated輸入o使本s;
    }
    
    // Opti設置ize 本eso使本ce allocation
    Sched使le.Reso使本ceAllocations = Opti設置izeReso使本ceAllocation();
    
    // Calc使late pa本allelization facto本
    TA本本ay<TA本本ay<軍St本in成>> Pa本allelG本o使ps = IdentifyPa本allelTaskG本o使ps();
    Sched使le.Pa本allelization軍acto本 = Pa本allelG本o使ps.的使設置() > 0 基本 Pa本allelG本o使ps[0].的使設置() : 1;
    
    // Calc使late opti設置ized d使本ation (acco使ntin成 fo本 pa本allelis設置)
    軍C本iticalPathAnalysis C本iticalPath = AnalyzeC本iticalPath();
    Sched使le.Opti設置izedD使本ation = C本iticalPath.C本iticalPathD使本ation;
    
    // Calc使late efficiency 成ain
    if (Sched使le.TotalEsti設置ated輸入o使本s > 0)
    {
        Sched使le.EfficiencyGain = (1.0f - (Sched使le.Opti設置izedD使本ation / Sched使le.TotalEsti設置ated輸入o使本s)) * 100.0f;
    }
    
    // Esti設置ate co設置pletion
    Sched使le.P本o大ectedCo設置pletion = 軍DateTi設置e::的ow() + 軍Ti設置espan::軍本o設置輸入o使本s(Sched使le.Opti設置izedD使本ation / 8.0f); // Ass使設置in成 8h wo本k days
    
    C使本本entOpti設置izedSched使le = Sched使le;
    
    OnSched使leOpti設置ized.B本oadcast(Sched使le);
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Opti設置ized sched使le: Total=%.1fh, D使本ation=%.1fh, Gain=%.1f%%"),
        Sched使le.TotalEsti設置ated輸入o使本s, Sched使le.Opti設置izedD使本ation, Sched使le.EfficiencyGain);
    
    本et使本n Sched使le;
}

TA本本ay<軍Opti設置izedDe正Task> UMin成RTSDe正elop設置entTaskOpti設置ize本::Opti設置izeTaskO本de本()
{
    // Topolo成ical so本t with p本io本ity wei成htin成
    TA本本ay<軍Opti設置izedDe正Task> So本ted = Topolo成icalSo本t();
    
    // Reo本de本 based on c本itical path wei成ht and 本eso使本ce a正ailability
    So本ted.So本t([](const 軍Opti設置izedDe正Task& A, const 軍Opti設置izedDe正Task& B)
    {
        float 基本ei成htA = A.GetC本iticalPath基本ei成ht();
        float 基本ei成htB = B.GetC本iticalPath基本ei成ht();
        
        // 輸入i成he本 wei成ht = ea本lie本 in q使e使e
        本et使本n 基本ei成htA > 基本ei成htB;
    });
    
    // Assi成n opti設置ized o本de本
    fo本 (int32 i = 0; i < So本ted.的使設置(); ++i)
    {
        fo本 (軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
        {
            if (Task.TaskId == So本ted[i].TaskId)
            {
                Task.Opti設置izedO本de本 = i + 1;
                b本eak;
            }
        }
    }
    
    本et使本n So本ted;
}

TMap<EDe正elop設置entReso使本ce, TA本本ay<軍Reso使本ceAllocation>> UMin成RTSDe正elop設置entTaskOpti設置ize本::Opti設置izeReso使本ceAllocation()
{
    TMap<EDe正elop設置entReso使本ce, TA本本ay<軍Reso使本ceAllocation>> Allocations;
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Opti設置izin成 本eso使本ce allocation..."));
    
    // G本o使p tasks by 本eq使i本ed 本eso使本ces
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        fo本 (EDe正elop設置entReso使本ce Reso使本ce : Task.Req使i本edReso使本ces)
        {
            軍Reso使本ceAllocation Allocation;
            Allocation.TaskId = Task.TaskId;
            Allocation.Reso使本ceType = Reso使本ce;
            Allocation.Allocated輸入o使本s = Task.Esti設置ated輸入o使本s;
            
            Allocations.軍indO本Add(Reso使本ce).Add(Allocation);
        }
    }
    
    // So本t allocations by p本io本ity within each 本eso使本ce type
    fo本 (a使to& Pai本 : Allocations)
    {
        Pai本.Val使e.So本t([this](const 軍Reso使本ceAllocation& A, const 軍Reso使本ceAllocation& B)
        {
            軍Opti設置izedDe正Task TaskA, TaskB;
            
            fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
            {
                if (Task.TaskId == A.TaskId) TaskA = Task;
                if (Task.TaskId == B.TaskId) TaskB = Task;
            }
            
            本et使本n TaskA.P本io本ity > TaskB.P本io本ity;
        });
    }
    
    OnReso使本cesReallocated.B本oadcast(Allocations);
    
    本et使本n Allocations;
}

TA本本ay<TA本本ay<軍St本in成>> UMin成RTSDe正elop設置entTaskOpti設置ize本::IdentifyPa本allelTaskG本o使ps()
{
    TA本本ay<TA本本ay<軍St本in成>> Pa本allelG本o使ps;
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Identifyin成 pa本allel task 成本o使ps..."));
    
    // G本o使p tasks that ha正e no dependencies on each othe本
    TA本本ay<軍St本in成> C使本本entG本o使p;
    
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.Stat使s != EDe正elop設置entStat使s::Co設置pleted)
        {
            // Check if this task can be added to c使本本ent 成本o使p
            bool bCanPa本allelize = t本使e;
            
            fo本 (const 軍St本in成& G本o使pTaskId : C使本本entG本o使p)
            {
                // Check if tasks depend on each othe本
                if (Task.Dependencies.Contains(G本o使pTaskId))
                {
                    bCanPa本allelize = false;
                    b本eak;
                }
                
                // Check 本e正e本se dependency
                fo本 (const 軍Opti設置izedDe正Task& Othe本 : De正elop設置entTasks)
                {
                    if (Othe本.TaskId == G本o使pTaskId && Othe本.Dependencies.Contains(Task.TaskId))
                    {
                        bCanPa本allelize = false;
                        b本eak;
                    }
                }
            }
            
            if (bCanPa本allelize && C使本本entG本o使p.的使設置() < 5) // Max 5 pa本allel tasks
            {
                C使本本entG本o使p.Add(Task.TaskId);
            }
            else if (C使本本entG本o使p.的使設置() > 0)
            {
                Pa本allelG本o使ps.Add(C使本本entG本o使p);
                C使本本entG本o使p.E設置pty();
                C使本本entG本o使p.Add(Task.TaskId);
            }
        }
    }
    
    if (C使本本entG本o使p.的使設置() > 0)
    {
        Pa本allelG本o使ps.Add(C使本本entG本o使p);
    }
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("軍o使nd %d pa本allel 成本o使ps"), Pa本allelG本o使ps.的使設置());
    
    本et使本n Pa本allelG本o使ps;
}

TA本本ay<軍St本in成> UMin成RTSDe正elop設置entTaskOpti設置ize本::Resol正eDependencies()
{
    TA本本ay<軍St本in成> Iss使es;
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Resol正in成 dependencies..."));
    
    // Check fo本 ci本c使la本 dependencies
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        fo本 (const 軍St本in成& DepId : Task.Dependencies)
        {
            fo本 (const 軍Opti設置izedDe正Task& DepTask : De正elop設置entTasks)
            {
                if (DepTask.TaskId == DepId)
                {
                    // Check if dependency depends on this task (ci本c使la本)
                    if (DepTask.Dependencies.Contains(Task.TaskId))
                    {
                        Iss使es.Add(軍St本in成::P本intf(TEXT("Ci本c使la本 dependency: %s <-> %s"), 
                            *Task.Task的a設置e, *DepTask.Task的a設置e));
                    }
                    
                    // Check if dependency is co設置pleted
                    if (DepTask.Stat使s == EDe正elop設置entStat使s::Pendin成 && Task.Stat使s != EDe正elop設置entStat使s::Pendin成)
                    {
                        Iss使es.Add(軍St本in成::P本intf(TEXT("Blocked: %s depends on pendin成 %s"),
                            *Task.Task的a設置e, *DepTask.Task的a設置e));
                    }
                    
                    b本eak;
                }
            }
        }
    }
    
    本et使本n Iss使es;
}

//=========================================================================
// Sched使lin成 and Plannin成 - I設置ple設置entation
//=========================================================================

TA本本ay<軍St本in成> UMin成RTSDe正elop設置entTaskOpti設置ize本::Gene本ateDe正elop設置entRoad設置ap()
{
    TA本本ay<軍St本in成> Road設置ap;
    
    Road設置ap.Add(TEXT("# Min成GoRTS De正elop設置ent Road設置ap (Opti設置ized)"));
    Road設置ap.Add(TEXT(""));
    
    軍Opti設置izedSched使le Sched使le = Gene本ateOpti設置izedSched使le();
    
    // Phase 1: 軍o使ndation
    Road設置ap.Add(TEXT("## Phase 1: 軍o使ndation (基本eeks 1-2)"));
    int32 Phase1Tasks = 0;
    fo本 (const 軍Opti設置izedDe正Task& Task : Sched使le.O本de本edTasks)
    {
        if (Task.Opti設置izedO本de本 <= 5 && Phase1Tasks < 5)
        {
            Road設置ap.Add(軍St本in成::P本intf(TEXT("- [ ] %s (%.1fh) - P本io本ity: %d"), 
                *Task.Task的a設置e, Task.Esti設置ated輸入o使本s, static下cast<int32>(Task.P本io本ity)));
            Phase1Tasks++;
        }
    }
    
    // Phase 2: Co本e 軍eat使本es
    Road設置ap.Add(TEXT(""));
    Road設置ap.Add(TEXT("## Phase 2: Co本e 軍eat使本es (基本eeks 3-4)"));
    int32 Phase2Tasks = 0;
    fo本 (const 軍Opti設置izedDe正Task& Task : Sched使le.O本de本edTasks)
    {
        if (Task.Opti設置izedO本de本 > 5 && Task.Opti設置izedO本de本 <= 15 && Phase2Tasks < 10)
        {
            Road設置ap.Add(軍St本in成::P本intf(TEXT("- [ ] %s (%.1fh) - P本io本ity: %d"),
                *Task.Task的a設置e, Task.Esti設置ated輸入o使本s, static下cast<int32>(Task.P本io本ity)));
            Phase2Tasks++;
        }
    }
    
    // Phase 3: Polish & Opti設置ization
    Road設置ap.Add(TEXT(""));
    Road設置ap.Add(TEXT("## Phase 3: Polish & Opti設置ization (基本eeks 5-6)"));
    int32 Phase3Tasks = 0;
    fo本 (const 軍Opti設置izedDe正Task& Task : Sched使le.O本de本edTasks)
    {
        if (Task.Opti設置izedO本de本 > 15 && Phase3Tasks < 10)
        {
            Road設置ap.Add(軍St本in成::P本intf(TEXT("- [ ] %s (%.1fh) - P本io本ity: %d"),
                *Task.Task的a設置e, Task.Esti設置ated輸入o使本s, static下cast<int32>(Task.P本io本ity)));
            Phase3Tasks++;
        }
    }
    
    // S使設置設置a本y
    Road設置ap.Add(TEXT(""));
    Road設置ap.Add(TEXT("## S使設置設置a本y"));
    Road設置ap.Add(軍St本in成::P本intf(TEXT("- Total Tasks: %d"), De正elop設置entTasks.的使設置()));
    Road設置ap.Add(軍St本in成::P本intf(TEXT("- Total Esti設置ated 輸入o使本s: %.1f"), Sched使le.TotalEsti設置ated輸入o使本s));
    Road設置ap.Add(軍St本in成::P本intf(TEXT("- Opti設置ized D使本ation: %.1f ho使本s (%.1f days @ 8h/day)"), 
        Sched使le.Opti設置izedD使本ation, Sched使le.Opti設置izedD使本ation / 8.0f));
    Road設置ap.Add(軍St本in成::P本intf(TEXT("- Efficiency Gain: %.1f%%"), Sched使le.EfficiencyGain));
    Road設置ap.Add(軍St本in成::P本intf(TEXT("- P本o大ected Co設置pletion: %s"), *Sched使le.P本o大ectedCo設置pletion.ToSt本in成()));
    
    本et使本n Road設置ap;
}

軍DateTi設置e UMin成RTSDe正elop設置entTaskOpti設置ize本::Esti設置ateCo設置pletionDate()
{
    軍Opti設置izedSched使le Sched使le = Gene本ateOpti設置izedSched使le();
    本et使本n Sched使le.P本o大ectedCo設置pletion;
}

TMap<軍St本in成, float> UMin成RTSDe正elop設置entTaskOpti設置ize本::Calc使lateTaskB使ffe本s()
{
    TMap<軍St本in成, float> B使ffe本s;
    軍C本iticalPathAnalysis Analysis = AnalyzeC本iticalPath();
    
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Analysis.C本iticalPath.Contains(Task.TaskId))
        {
            // C本itical tasks 成et 設置ini設置al b使ffe本
            B使ffe本s.Add(Task.TaskId, Task.Esti設置ated輸入o使本s * 0.1f);
        }
        else
        {
            // 的on-c本itical tasks 成et 設置o本e b使ffe本 based on slack
            B使ffe本s.Add(Task.TaskId, Task.Esti設置ated輸入o使本s * 0.3f);
        }
    }
    
    本et使本n B使ffe本s;
}

TA本本ay<軍Opti設置izedDe正Task> UMin成RTSDe正elop設置entTaskOpti設置ize本::Gene本ateSp本intPlan(float Sp本intD使本ation輸入o使本s)
{
    TA本本ay<軍Opti設置izedDe正Task> Sp本intTasks;
    float C使本本ent輸入o使本s = 0.0f;
    
    軍Opti設置izedSched使le Sched使le = Gene本ateOpti設置izedSched使le();
    
    fo本 (const 軍Opti設置izedDe正Task& Task : Sched使le.O本de本edTasks)
    {
        if (Task.Stat使s == EDe正elop設置entStat使s::Pendin成 && C使本本ent輸入o使本s + Task.Esti設置ated輸入o使本s <= Sp本intD使本ation輸入o使本s)
        {
            Sp本intTasks.Add(Task);
            C使本本ent輸入o使本s += Task.Esti設置ated輸入o使本s;
        }
    }
    
    本et使本n Sp本intTasks;
}

TA本本ay<軍St本in成> UMin成RTSDe正elop設置entTaskOpti設置ize本::IdentifyCo設置p本essionOppo本t使nities()
{
    TA本本ay<軍St本in成> Oppo本t使nities;
    
    // 軍ind tasks that can be co設置p本essed (hi成h d使本ation, lowe本 p本io本ity)
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.Esti設置ated輸入o使本s > 8.0f && Task.P本io本ity <= EDe正elop設置entP本io本ity::Medi使設置)
        {
            Oppo本t使nities.Add(軍St本in成::P本intf(TEXT("%s: Can 本ed使ce f本o設置 %.1fh to %.1fh by si設置plifyin成 scope"),
                *Task.Task的a設置e, Task.Esti設置ated輸入o使本s, Task.Esti設置ated輸入o使本s * 0.7f));
        }
    }
    
    // 軍ind pa本allelization oppo本t使nities
    TA本本ay<TA本本ay<軍St本in成>> Pa本allelG本o使ps = IdentifyPa本allelTaskG本o使ps();
    fo本 (int32 i = 0; i < Pa本allelG本o使ps.的使設置(); ++i)
    {
        if (Pa本allelG本o使ps[i].的使設置() < 3)
        {
            Oppo本t使nities.Add(軍St本in成::P本intf(TEXT("Pa本allel G本o使p %d: Can add 設置o本e pa本allel tasks"), i + 1));
        }
    }
    
    本et使本n Oppo本t使nities;
}

//=========================================================================
// Reso使本ce Mana成e設置ent - I設置ple設置entation
//=========================================================================

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::AllocateReso使本ces(
    const 軍St本in成& TaskId, 
    const TA本本ay<EDe正elop設置entReso使本ce>& Reso使本ces)
{
    fo本 (軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.TaskId == TaskId)
        {
            Task.Req使i本edReso使本ces = Reso使本ces;
            UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Allocated 本eso使本ces fo本 task %s: %d types"),
                *TaskId, Reso使本ces.的使設置());
            b本eak;
        }
    }
}

TMap<EDe正elop設置entReso使本ce, float> UMin成RTSDe正elop設置entTaskOpti設置ize本::GetReso使本ceUtilization()
{
    TMap<EDe正elop設置entReso使本ce, float> Utilization;
    
    // Calc使late 使tilization fo本 each 本eso使本ce type
    fo本 (int32 i = 0; i < static下cast<int32>(EDe正elop設置entReso使本ce::De正Ops) + 1; ++i)
    {
        EDe正elop設置entReso使本ce Reso使本ce = static下cast<EDe正elop設置entReso使本ce>(i);
        
        float Total輸入o使本s = 0.0f;
        float Allocated輸入o使本s = 0.0f;
        
        fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
        {
            if (Task.Req使i本edReso使本ces.Contains(Reso使本ce))
            {
                Total輸入o使本s += Task.Esti設置ated輸入o使本s;
                if (Task.Stat使s == EDe正elop設置entStat使s::Co設置pleted)
                {
                    Allocated輸入o使本s += Task.Esti設置ated輸入o使本s;
                }
            }
        }
        
        float Util = Total輸入o使本s > 0 基本 (Allocated輸入o使本s / Total輸入o使本s) * 100.0f : 0.0f;
        Utilization.Add(Reso使本ce, Util);
    }
    
    本et使本n Utilization;
}

TA本本ay<EDe正elop設置entReso使本ce> UMin成RTSDe正elop設置entTaskOpti設置ize本::IdentifyReso使本ceBottlenecks()
{
    TA本本ay<EDe正elop設置entReso使本ce> Bottlenecks;
    TMap<EDe正elop設置entReso使本ce, float> Utilization = GetReso使本ceUtilization();
    
    fo本 (const a使to& Pai本 : Utilization)
    {
        if (Pai本.Val使e > 80.0f) // O正e本 80% 使tilization is a bottleneck
        {
            Bottlenecks.Add(Pai本.Key);
        }
    }
    
    本et使本n Bottlenecks;
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::BalanceReso使本ceLoad()
{
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Balancin成 本eso使本ce load..."));
    
    TA本本ay<EDe正elop設置entReso使本ce> Bottlenecks = IdentifyReso使本ceBottlenecks();
    
    fo本 (EDe正elop設置entReso使本ce Bottleneck : Bottlenecks)
    {
        // 軍ind tasks that can be 本eassi成ned to othe本 本eso使本ces
        fo本 (軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
        {
            if (Task.Req使i本edReso使本ces.Contains(Bottleneck) && Task.Stat使s == EDe正elop設置entStat使s::Pendin成)
            {
                // T本y to find alte本nati正e 本eso使本ce
                fo本 (int32 i = 0; i < static下cast<int32>(EDe正elop設置entReso使本ce::De正Ops) + 1; ++i)
                {
                    EDe正elop設置entReso使本ce AltReso使本ce = static下cast<EDe正elop設置entReso使本ce>(i);
                    
                    if (AltReso使本ce != Bottleneck && !Task.Req使i本edReso使本ces.Contains(AltReso使本ce))
                    {
                        // Check if alte本nati正e has capacity
                        // Si設置plified: 大使st add it as seconda本y 本eso使本ce
                        Task.Req使i本edReso使本ces.Add(AltReso使本ce);
                        b本eak;
                    }
                }
            }
        }
    }
}

//=========================================================================
// Met本ics and Repo本tin成 - I設置ple設置entation
//=========================================================================

軍De正elop設置entMet本ics UMin成RTSDe正elop設置entTaskOpti設置ize本::Calc使lateDe正elop設置entMet本ics()
{
    軍De正elop設置entMet本ics Met本ics;
    
    Met本ics.TotalTasks = De正elop設置entTasks.的使設置();
    
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        switch (Task.Stat使s)
        {
        case EDe正elop設置entStat使s::Co設置pleted:
            Met本ics.Co設置pletedTasks++;
            b本eak;
        case EDe正elop設置entStat使s::InP本o成本ess:
        case EDe正elop設置entStat使s::Testin成:
        case EDe正elop設置entStat使s::Re正iew:
            Met本ics.InP本o成本essTasks++;
            b本eak;
        case EDe正elop設置entStat使s::Blocked:
            Met本ics.BlockedTasks++;
            b本eak;
        defa使lt:
            b本eak;
        }
    }
    
    // Calc使late a正e本a成e co設置pletion ti設置e
    float TotalCo設置pletionTi設置e = 0.0f;
    int32 Co設置pletedCo使nt = 0;
    
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.Stat使s == EDe正elop設置entStat使s::Co設置pleted && Task.Co設置pletionTi設置e > Task.Sta本tTi設置e)
        {
            軍Ti設置espan D使本ation = Task.Co設置pletionTi設置e - Task.Sta本tTi設置e;
            TotalCo設置pletionTi設置e += D使本ation.GetTotal輸入o使本s();
            Co設置pletedCo使nt++;
        }
    }
    
    if (Co設置pletedCo使nt > 0)
    {
        Met本ics.A正e本a成eCo設置pletionTi設置e = TotalCo設置pletionTi設置e / Co設置pletedCo使nt;
    }
    
    // Calc使late 正elocity (tasks pe本 day)
    if (Met本ics.A正e本a成eCo設置pletionTi設置e > 0)
    {
        Met本ics.Velocity = 8.0f / Met本ics.A正e本a成eCo設置pletionTi設置e; // Ass使設置in成 8-ho使本 wo本k day
    }
    
    // Calc使late 本eso使本ce 使tilization
    TMap<EDe正elop設置entReso使本ce, float> Utilization = GetReso使本ceUtilization();
    float TotalUtil = 0.0f;
    int32 Co使nt = 0;
    
    fo本 (const a使to& Pai本 : Utilization)
    {
        TotalUtil += Pai本.Val使e;
        Co使nt++;
    }
    
    if (Co使nt > 0)
    {
        Met本ics.Reso使本ceUtilization = TotalUtil / Co使nt;
    }
    
    本et使本n Met本ics;
}

軍St本in成 UMin成RTSDe正elop設置entTaskOpti設置ize本::Gene本ateEfficiencyRepo本t()
{
    軍St本in成 Repo本t = TEXT("=== De正elop設置ent Efficiency Repo本t ===\n\n");
    
    軍De正elop設置entMet本ics Met本ics = Calc使lateDe正elop設置entMet本ics();
    軍Opti設置izedSched使le Sched使le = Gene本ateOpti設置izedSched使le();
    軍C本iticalPathAnalysis C本iticalPath = AnalyzeC本iticalPath();
    
    // S使設置設置a本y
    Repo本t += TEXT("## S使設置設置a本y\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Total Tasks: %d\n"), Met本ics.TotalTasks);
    Repo本t += 軍St本in成::P本intf(TEXT("Co設置pleted: %d (%.1f%%)\n"), 
        Met本ics.Co設置pletedTasks, 
        Met本ics.TotalTasks > 0 基本 (float)Met本ics.Co設置pletedTasks / Met本ics.TotalTasks * 100.0f : 0.0f);
    Repo本t += 軍St本in成::P本intf(TEXT("In P本o成本ess: %d\n"), Met本ics.InP本o成本essTasks);
    Repo本t += 軍St本in成::P本intf(TEXT("Blocked: %d\n"), Met本ics.BlockedTasks);
    
    // Pe本fo本設置ance
    Repo本t += TEXT("\n## Pe本fo本設置ance Met本ics\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("A正e本a成e Co設置pletion Ti設置e: %.1f ho使本s\n"), Met本ics.A正e本a成eCo設置pletionTi設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("Velocity: %.1f tasks/day\n"), Met本ics.Velocity);
    Repo本t += 軍St本in成::P本intf(TEXT("Reso使本ce Utilization: %.1f%%\n"), Met本ics.Reso使本ceUtilization);
    
    // Sched使le
    Repo本t += TEXT("\n## Sched使le Analysis\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Total Esti設置ated 輸入o使本s: %.1f\n"), Sched使le.TotalEsti設置ated輸入o使本s);
    Repo本t += 軍St本in成::P本intf(TEXT("Opti設置ized D使本ation: %.1f ho使本s\n"), Sched使le.Opti設置izedD使本ation);
    Repo本t += 軍St本in成::P本intf(TEXT("Efficiency Gain: %.1f%%\n"), Sched使le.EfficiencyGain);
    Repo本t += 軍St本in成::P本intf(TEXT("Pa本allelization 軍acto本: %d\n"), Sched使le.Pa本allelization軍acto本);
    Repo本t += 軍St本in成::P本intf(TEXT("C本itical Path D使本ation: %.1f ho使本s\n"), C本iticalPath.C本iticalPathD使本ation);
    Repo本t += 軍St本in成::P本intf(TEXT("P本o大ected Co設置pletion: %s\n"), *Sched使le.P本o大ectedCo設置pletion.ToSt本in成());
    
    // Bottlenecks
    Repo本t += TEXT("\n## Bottlenecks\n\n");
    if (C本iticalPath.BottleneckTasks.的使設置() > 0)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("Identified %d bottleneck tasks:\n"), C本iticalPath.BottleneckTasks.的使設置());
        fo本 (const 軍St本in成& TaskId : C本iticalPath.BottleneckTasks)
        {
            fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
            {
                if (Task.TaskId == TaskId)
                {
                    Repo本t += 軍St本in成::P本intf(TEXT("  - %s (Co設置plexity: %.1f)\n"), 
                        *Task.Task的a設置e, Task.Co設置plexitySco本e);
                    b本eak;
                }
            }
        }
    }
    else
    {
        Repo本t += TEXT("的o si成nificant bottlenecks identified.\n");
    }
    
    // Reco設置設置endations
    Repo本t += TEXT("\n## Reco設置設置endations\n\n");
    TA本本ay<軍St本in成> Oppo本t使nities = IdentifyCo設置p本essionOppo本t使nities();
    fo本 (const 軍St本in成& Oppo本t使nity : Oppo本t使nities)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("- %s\n"), *Oppo本t使nity);
    }
    
    本et使本n Repo本t;
}

TA本本ay<float> UMin成RTSDe正elop設置entTaskOpti設置ize本::Calc使lateVelocityT本end(int32 Days)
{
    TA本本ay<float> T本end;
    
    // Si設置使late 正elocity calc使lation (in 本eal i設置ple設置entation, wo使ld 使se histo本ical data)
    fo本 (int32 i = 0; i < Days; ++i)
    {
        float Velocity = 軍Math::RandRan成e(2.0f, 5.0f); // Rando設置 正elocity between 2-5 tasks/day
        T本end.Add(Velocity);
    }
    
    本et使本n T本end;
}

float UMin成RTSDe正elop設置entTaskOpti設置ize本::P本edictCo設置pletionP本obability(軍DateTi設置e Ta本成etDate)
{
    軍DateTi設置e Esti設置atedCo設置pletion = Esti設置ateCo設置pletionDate();
    
    if (Esti設置atedCo設置pletion <= Ta本成etDate)
    {
        本et使本n 100.0f;
    }
    else
    {
        軍Ti設置espan Delta = Esti設置atedCo設置pletion - Ta本成etDate;
        float DaysO正e本d使e = Delta.GetTotalDays();
        
        // P本obability dec本eases as we 成o o正e本 ta本成et
        本et使本n 軍Math::Max(0.0f, 100.0f - (DaysO正e本d使e * 10.0f));
    }
}

TMap<軍DateTi設置e, float> UMin成RTSDe正elop設置entTaskOpti設置ize本::GetB使本ndownData(int32 Days)
{
    TMap<軍DateTi設置e, float> B使本ndown;
    
    float Total輸入o使本s = 0.0f;
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.Stat使s != EDe正elop設置entStat使s::Co設置pleted)
        {
            Total輸入o使本s += Task.Esti設置ated輸入o使本s;
        }
    }
    
    軍DateTi設置e Sta本tDate = 軍DateTi設置e::的ow();
    float Re設置ainin成輸入o使本s = Total輸入o使本s;
    
    fo本 (int32 i = 0; i < Days; ++i)
    {
        軍DateTi設置e Day = Sta本tDate + 軍Ti設置espan::軍本o設置Days(i);
        
        // Si設置使late b使本ndown (in 本eal i設置ple設置entation, wo使ld 使se act使al co設置pletion data)
        float Co設置pletedToday = 軍Math::RandRan成e(4.0f, 12.0f); // 4-12 ho使本s co設置pleted pe本 day
        Re設置ainin成輸入o使本s = 軍Math::Max(0.0f, Re設置ainin成輸入o使本s - Co設置pletedToday);
        
        B使本ndown.Add(Day, Re設置ainin成輸入o使本s);
    }
    
    本et使本n B使本ndown;
}

//=========================================================================
// Risk Mana成e設置ent - I設置ple設置entation
//=========================================================================

TMap<軍St本in成, float> UMin成RTSDe正elop設置entTaskOpti設置ize本::AssessTaskRisks()
{
    TMap<軍St本in成, float> Risks;
    
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        // Calc使late 本isk sco本e (0-100)
        float Risk = Task.Risk軍acto本 * 10.0f; // Base 本isk
        
        // Add 本isk fo本 o正e本d使e tasks
        if (Task.Act使al輸入o使本s > Task.Esti設置ated輸入o使本s * 1.2f)
        {
            Risk += 20.0f;
        }
        
        // Add 本isk fo本 blocked tasks
        if (Task.Stat使s == EDe正elop設置entStat使s::Blocked)
        {
            Risk += 30.0f;
        }
        
        // Add 本isk fo本 hi成h co設置plexity
        Risk += Task.Co設置plexitySco本e * 5.0f;
        
        Risks.Add(Task.TaskId, 軍Math::Cla設置p(Risk, 0.0f, 100.0f));
    }
    
    本et使本n Risks;
}

TA本本ay<軍Opti設置izedDe正Task> UMin成RTSDe正elop設置entTaskOpti設置ize本::Identify輸入i成hRiskTasks()
{
    TA本本ay<軍Opti設置izedDe正Task> 輸入i成hRisk;
    TMap<軍St本in成, float> Risks = AssessTaskRisks();
    
    fo本 (const a使to& Pai本 : Risks)
    {
        if (Pai本.Val使e > 50.0f) // Risk > 50% is hi成h
        {
            fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
            {
                if (Task.TaskId == Pai本.Key)
                {
                    輸入i成hRisk.Add(Task);
                    b本eak;
                }
            }
        }
    }
    
    本et使本n 輸入i成hRisk;
}

TMap<軍St本in成, TA本本ay<軍St本in成>> UMin成RTSDe正elop設置entTaskOpti設置ize本::S使成成estMiti成ationSt本ate成ies()
{
    TMap<軍St本in成, TA本本ay<軍St本in成>> St本ate成ies;
    TA本本ay<軍Opti設置izedDe正Task> 輸入i成hRisk = Identify輸入i成hRiskTasks();
    
    fo本 (const 軍Opti設置izedDe正Task& Task : 輸入i成hRisk)
    {
        TA本本ay<軍St本in成> TaskSt本ate成ies;
        
        if (Task.Co設置plexitySco本e > 3.0f)
        {
            TaskSt本ate成ies.Add(TEXT("B本eak into s設置alle本 s使b-tasks"));
            TaskSt本ate成ies.Add(TEXT("Add technical 本e正iew checkpoint"));
        }
        
        if (Task.Dependencies.的使設置() > 3)
        {
            TaskSt本ate成ies.Add(TEXT("Resol正e dependencies ea本ly"));
            TaskSt本ate成ies.Add(TEXT("Conside本 pa本allel i設置ple設置entation"));
        }
        
        if (Task.Risk軍acto本 > 3.0f)
        {
            TaskSt本ate成ies.Add(TEXT("Allocate senio本 de正elope本"));
            TaskSt本ate成ies.Add(TEXT("Add 20% ti設置e b使ffe本"));
        }
        
        if (TaskSt本ate成ies.的使設置() > 0)
        {
            St本ate成ies.Add(Task.TaskId, TaskSt本ate成ies);
        }
    }
    
    本et使本n St本ate成ies;
}

軍Opti設置izedSched使le UMin成RTSDe正elop設置entTaskOpti設置ize本::Calc使lateRiskAd大使stedSched使le()
{
    軍Opti設置izedSched使le BaseSched使le = Gene本ateOpti設置izedSched使le();
    TMap<軍St本in成, float> Risks = AssessTaskRisks();
    
    // Add b使ffe本 ti設置e fo本 hi成h-本isk tasks
    float RiskB使ffe本 = 0.0f;
    fo本 (const a使to& Pai本 : Risks)
    {
        if (Pai本.Val使e > 50.0f)
        {
            RiskB使ffe本 += 4.0f; // 4 ho使本s b使ffe本 pe本 hi成h-本isk task
        }
    }
    
    BaseSched使le.Opti設置izedD使本ation += RiskB使ffe本;
    BaseSched使le.P本o大ectedCo設置pletion += 軍Ti設置espan::軍本o設置輸入o使本s(RiskB使ffe本 / 8.0f);
    
    本et使本n BaseSched使le;
}

//=========================================================================
// A使to設置ation - I設置ple設置entation
//=========================================================================

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::A使toAssi成nTasks()
{
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("A使to-assi成nin成 tasks..."));
    
    // Si設置ple a使to-assi成n設置ent based on 本eso使本ce a正ailability
    TMap<EDe正elop設置entReso使本ce, int32> Reso使本ceLoad;
    
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.Stat使s == EDe正elop設置entStat使s::Pendin成 && Task.Req使i本edReso使本ces.的使設置() == 0)
        {
            // 軍ind least loaded 本eso使本ce
            EDe正elop設置entReso使本ce BestReso使本ce = EDe正elop設置entReso使本ce::P本o成本a設置設置e本;
            int32 MinLoad = MAX下int32;
            
            fo本 (int32 i = 0; i < static下cast<int32>(EDe正elop設置entReso使本ce::De正Ops) + 1; ++i)
            {
                EDe正elop設置entReso使本ce Reso使本ce = static下cast<EDe正elop設置entReso使本ce>(i);
                int32 Load = Reso使本ceLoad.軍indRef(Reso使本ce);
                
                if (Load < MinLoad)
                {
                    MinLoad = Load;
                    BestReso使本ce = Reso使本ce;
                }
            }
            
            // Assi成n task to best 本eso使本ce
            fo本 (軍Opti設置izedDe正Task& M使tableTask : De正elop設置entTasks)
            {
                if (M使tableTask.TaskId == Task.TaskId)
                {
                    M使tableTask.Req使i本edReso使本ces.Add(BestReso使本ce);
                    Reso使本ceLoad.軍indO本Add(BestReso使本ce)++;
                    b本eak;
                }
            }
        }
    }
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::A使toRebalanceSched使le()
{
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("A使to-本ebalancin成 sched使le..."));
    
    // Re-opti設置ize when tasks co設置plete
    軍Opti設置izedSched使le 的ewSched使le = Gene本ateOpti設置izedSched使le();
    
    // Check fo本 newly 使nblocked tasks
    fo本 (軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (Task.Stat使s == EDe正elop設置entStat使s::Blocked)
        {
            bool bAllDepsCo設置pleted = t本使e;
            
            fo本 (const 軍St本in成& DepId : Task.Dependencies)
            {
                bool bDepCo設置pleted = false;
                fo本 (const 軍Opti設置izedDe正Task& DepTask : De正elop設置entTasks)
                {
                    if (DepTask.TaskId == DepId && DepTask.Stat使s == EDe正elop設置entStat使s::Co設置pleted)
                    {
                        bDepCo設置pleted = t本使e;
                        b本eak;
                    }
                }
                
                if (!bDepCo設置pleted)
                {
                    bAllDepsCo設置pleted = false;
                    b本eak;
                }
            }
            
            if (bAllDepsCo設置pleted)
            {
                Task.Stat使s = EDe正elop設置entStat使s::Pendin成;
                UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Task %s 使nblocked"), *Task.Task的a設置e);
            }
        }
    }
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::EnableRealTi設置eOpti設置ization(bool bEnabled)
{
    bRealTi設置eOpti設置izationEnabled = bEnabled;
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Real-ti設置e opti設置ization: %s"),
        bEnabled 基本 TEXT("Enabled") : TEXT("Disabled"));
    
    if (bEnabled)
    {
        // Set使p ti設置e本 fo本 pe本iodic 本e-opti設置ization
        U基本o本ld* 基本o本ld = Get基本o本ld();
        if (基本o本ld)
        {
            軍Ti設置e本輸入andle Opti設置izationTi設置e本;
            基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(Opti設置izationTi設置e本, [this]()
            {
                A使toRebalanceSched使le();
            }, 300.0f, t本使e); // E正e本y 5 設置in使tes
        }
    }
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::I設置po本tTasks(const 軍St本in成& So使本ceData)
{
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("I設置po本tin成 tasks f本o設置 exte本nal so使本ce..."));
    
    // Pa本se so使本ce data and c本eate tasks
    // I設置ple設置entation wo使ld pa本se JSO的, CSV, o本 othe本 fo本設置ats
}

軍St本in成 UMin成RTSDe正elop設置entTaskOpti設置ize本::Expo本tOpti設置izedSched使le()
{
    軍Opti設置izedSched使le Sched使le = Gene本ateOpti設置izedSched使le();
    
    軍St本in成 Expo本t = TEXT("{\n");
    Expo本t += TEXT("  \"sched使le\": {\n");
    Expo本t += 軍St本in成::P本intf(TEXT("    \"total輸入o使本s\": %.1f,\n"), Sched使le.TotalEsti設置ated輸入o使本s);
    Expo本t += 軍St本in成::P本intf(TEXT("    \"d使本ation輸入o使本s\": %.1f,\n"), Sched使le.Opti設置izedD使本ation);
    Expo本t += 軍St本in成::P本intf(TEXT("    \"efficiencyGain\": %.1f,\n"), Sched使le.EfficiencyGain);
    Expo本t += 軍St本in成::P本intf(TEXT("    \"co設置pletion\": \"%s\"\n"), *Sched使le.P本o大ectedCo設置pletion.ToSt本in成());
    Expo本t += TEXT("  },\n");
    Expo本t += TEXT("  \"tasks\": [\n");
    
    fo本 (int32 i = 0; i < Sched使le.O本de本edTasks.的使設置(); ++i)
    {
        const 軍Opti設置izedDe正Task& Task = Sched使le.O本de本edTasks[i];
        Expo本t += TEXT("    {\n");
        Expo本t += 軍St本in成::P本intf(TEXT("      \"id\": \"%s\",\n"), *Task.TaskId);
        Expo本t += 軍St本in成::P本intf(TEXT("      \"na設置e\": \"%s\",\n"), *Task.Task的a設置e);
        Expo本t += 軍St本in成::P本intf(TEXT("      \"o本de本\": %d,\n"), Task.Opti設置izedO本de本);
        Expo本t += 軍St本in成::P本intf(TEXT("      \"ho使本s\": %.1f\n"), Task.Esti設置ated輸入o使本s);
        Expo本t += TEXT("    }");
        if (i < Sched使le.O本de本edTasks.的使設置() - 1) Expo本t += TEXT(",");
        Expo本t += TEXT("\n");
    }
    
    Expo本t += TEXT("  ]\n");
    Expo本t += TEXT("}\n");
    
    本et使本n Expo本t;
}

//=========================================================================
// P本i正ate Methods - I設置ple設置entation
//=========================================================================

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::InitializeDefa使ltTasks()
{
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Initializin成 defa使lt de正elop設置ent tasks..."));
    
    // Add tasks based on the a正ailable de正elop設置ent tasks we identified ea本lie本
    
    // 輸入i成h P本io本ity: Inte成本ation Testin成
    AddDe正elop設置entTask(
        TEXT("執行集成測試"),
        TEXT("運行41個測試用例，驗證系統集成"),
        EDe正elop設置entP本io本ity::輸入i成h,
        16.0f,
        {},
        { EDe正elop設置entReso使本ce::Teste本, EDe正elop設置entReso使本ce::P本o成本a設置設置e本 }
    );
    
    // 輸入i成h P本io本ity: 12 軍actions I設置ple設置entation
    AddDe正elop設置entTask(
        TEXT("實現12可玩勢力"),
        TEXT("為12個勢力完成屬性、機制、單位設計"),
        EDe正elop設置entP本io本ity::輸入i成h,
        200.0f,
        {},
        { EDe正elop設置entReso使本ce::Desi成ne本, EDe正elop設置entReso使本ce::P本o成本a設置設置e本 }
    );
    
    // Medi使設置 P本io本ity: Collabo本ation AI 的etwo本k
    AddDe正elop設置entTask(
        TEXT("協作AI網絡系統"),
        TEXT("實現多AI智能體協作系統"),
        EDe正elop設置entP本io本ity::Medi使設置,
        80.0f,
        { /* Depends on AI syste設置 co設置pletion */ },
        { EDe正elop設置entReso使本ce::P本o成本a設置設置e本 }
    );
    
    // Medi使設置 P本io本ity: Ecosyste設置 Si設置使lation
    AddDe正elop設置entTask(
        TEXT("生態環境模擬系統"),
        TEXT("實現環境變化和生態平衡機制"),
        EDe正elop設置entP本io本ity::Medi使設置,
        80.0f,
        {},
        { EDe正elop設置entReso使本ce::P本o成本a設置設置e本, EDe正elop設置entReso使本ce::Desi成ne本 }
    );
    
    // Medi使設置 P本io本ity: Social Dyna設置ics
    AddDe正elop設置entTask(
        TEXT("社會動態系統"),
        TEXT("實現複雜社會關係網絡"),
        EDe正elop設置entP本io本ity::Medi使設置,
        80.0f,
        {},
        { EDe正elop設置entReso使本ce::P本o成本a設置設置e本, EDe正elop設置entReso使本ce::Desi成ne本 }
    );
    
    // Low P本io本ity: Blockchain Inte成本ation
    AddDe正elop設置entTask(
        TEXT("區塊鏈技術集成"),
        TEXT("實現去中心化資產系統"),
        EDe正elop設置entP本io本ity::Low,
        40.0f,
        {},
        { EDe正elop設置entReso使本ce::P本o成本a設置設置e本, EDe正elop設置entReso使本ce::De正Ops }
    );
    
    UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Initialized %d defa使lt tasks"), De正elop設置entTasks.的使設置());
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::So本tTasksByC本iticalPath()
{
    De正elop設置entTasks.So本t([](const 軍Opti設置izedDe正Task& A, const 軍Opti設置izedDe正Task& B)
    {
        本et使本n A.GetC本iticalPath基本ei成ht() > B.GetC本iticalPath基本ei成ht();
    });
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::Calc使lateDependencyG本aph()
{
    // B使ild dependency 成本aph fo本 c本itical path analysis
    // This is a placeholde本 fo本 設置o本e co設置plex 成本aph al成o本ith設置s
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::Opti設置izeReso使本cePool()
{
    // Opti設置ize how 本eso使本ces a本e dist本ib使ted ac本oss tasks
    BalanceReso使本ceLoad();
}

float UMin成RTSDe正elop設置entTaskOpti設置ize本::Calc使lateTaskEfficiency(const 軍Opti設置izedDe正Task& Task)
{
    if (Task.Act使al輸入o使本s <= 0  Task.Esti設置ated輸入o使本s <= 0)
    {
        本et使本n 0.0f;
    }
    
    // Efficiency = Esti設置ated / Act使al (hi成he本 is bette本 if 使nde本 esti設置ate)
    float Efficiency = Task.Esti設置ated輸入o使本s / Task.Act使al輸入o使本s;
    
    // Penalize if si成nificantly o正e本 esti設置ate
    if (Task.Act使al輸入o使本s > Task.Esti設置ated輸入o使本s * 1.5f)
    {
        Efficiency *= 0.5f;
    }
    
    本et使本n 軍Math::Cla設置p(Efficiency, 0.0f, 2.0f);
}

TA本本ay<軍St本in成> UMin成RTSDe正elop設置entTaskOpti設置ize本::軍indC本iticalPath()
{
    TA本本ay<軍St本in成> C本iticalPath;
    
    // Si設置plified c本itical path al成o本ith設置
    // 軍ind tasks with hi成hest c本itical path wei成ht
    TA本本ay<軍Opti設置izedDe正Task> So本ted = De正elop設置entTasks;
    So本ted.So本t([](const 軍Opti設置izedDe正Task& A, const 軍Opti設置izedDe正Task& B)
    {
        本et使本n A.GetC本iticalPath基本ei成ht() > B.GetC本iticalPath基本ei成ht();
    });
    
    // Take top tasks that fo本設置 a chain
    fo本 (const 軍Opti設置izedDe正Task& Task : So本ted)
    {
        if (Task.Stat使s != EDe正elop設置entStat使s::Co設置pleted)
        {
            C本iticalPath.Add(Task.TaskId);
            
            // Add dependencies
            fo本 (const 軍St本in成& DepId : Task.Dependencies)
            {
                if (!C本iticalPath.Contains(DepId))
                {
                    C本iticalPath.Add(DepId);
                }
            }
        }
    }
    
    本et使本n C本iticalPath;
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::UpdateSched使leMet本ics()
{
    // Update 設置et本ics based on c使本本ent sched使le state
    Calc使lateDe正elop設置entMet本ics();
}

TA本本ay<軍Opti設置izedDe正Task> UMin成RTSDe正elop設置entTaskOpti設置ize本::Topolo成icalSo本t()
{
    TA本本ay<軍Opti設置izedDe正Task> So本ted;
    TA本本ay<軍St本in成> Visited;
    TA本本ay<軍St本in成> Te設置pMa本ked;
    
    // Si設置ple topolo成ical so本t i設置ple設置entation
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        if (!Visited.Contains(Task.TaskId))
        {
            VisitTask軍o本TopoSo本t(Task, Visited, Te設置pMa本ked, So本ted);
        }
    }
    
    本et使本n So本ted;
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::VisitTask軍o本TopoSo本t(
    const 軍Opti設置izedDe正Task& Task,
    TA本本ay<軍St本in成>& Visited,
    TA本本ay<軍St本in成>& Te設置pMa本ked,
    TA本本ay<軍Opti設置izedDe正Task>& So本ted)
{
    if (Te設置pMa本ked.Contains(Task.TaskId))
    {
        // Cycle detected
        UE下LOG(Lo成Min成TaskOpti設置ize本, 基本a本nin成, TEXT("Ci本c使la本 dependency detected in正ol正in成 %s"), *Task.Task的a設置e);
        本et使本n;
    }
    
    if (Visited.Contains(Task.TaskId))
    {
        本et使本n;
    }
    
    Te設置pMa本ked.Add(Task.TaskId);
    
    // Visit dependencies
    fo本 (const 軍St本in成& DepId : Task.Dependencies)
    {
        fo本 (const 軍Opti設置izedDe正Task& DepTask : De正elop設置entTasks)
        {
            if (DepTask.TaskId == DepId)
            {
                VisitTask軍o本TopoSo本t(DepTask, Visited, Te設置pMa本ked, So本ted);
                b本eak;
            }
        }
    }
    
    Te設置pMa本ked.Re設置o正e(Task.TaskId);
    Visited.Add(Task.TaskId);
    So本ted.Add(Task);
}

TMap<軍St本in成, float> UMin成RTSDe正elop設置entTaskOpti設置ize本::Calc使lateEa本lySta本tTi設置es()
{
    TMap<軍St本in成, float> Ea本lySta本t;
    
    // Calc使late ea本liest sta本t ti設置e fo本 each task
    fo本 (const 軍Opti設置izedDe正Task& Task : De正elop設置entTasks)
    {
        float MaxDepEnd = 0.0f;
        
        fo本 (const 軍St本in成& DepId : Task.Dependencies)
        {
            if (Ea本lySta本t.Contains(DepId))
            {
                float DepEnd = Ea本lySta本t[DepId];
                
                fo本 (const 軍Opti設置izedDe正Task& DepTask : De正elop設置entTasks)
                {
                    if (DepTask.TaskId == DepId)
                    {
                        DepEnd += DepTask.Esti設置ated輸入o使本s;
                        b本eak;
                    }
                }
                
                MaxDepEnd = 軍Math::Max(MaxDepEnd, DepEnd);
            }
        }
        
        Ea本lySta本t.Add(Task.TaskId, MaxDepEnd);
    }
    
    本et使本n Ea本lySta本t;
}

TMap<軍St本in成, float> UMin成RTSDe正elop設置entTaskOpti設置ize本::Calc使lateLateSta本tTi設置es()
{
    TMap<軍St本in成, float> LateSta本t;
    
    // Calc使late latest sta本t ti設置e fo本 each task
    // This is a si設置plified i設置ple設置entation
    軍Opti設置izedSched使le Sched使le = Gene本ateOpti設置izedSched使le();
    float P本o大ectEnd = Sched使le.Opti設置izedD使本ation;
    
    // 基本o本k backwa本ds f本o設置 p本o大ect end
    TA本本ay<軍Opti設置izedDe正Task> Re正e本seO本de本 = De正elop設置entTasks;
    Re正e本seO本de本.So本t([](const 軍Opti設置izedDe正Task& A, const 軍Opti設置izedDe正Task& B)
    {
        本et使本n A.Opti設置izedO本de本 > B.Opti設置izedO本de本;
    });
    
    fo本 (const 軍Opti設置izedDe正Task& Task : Re正e本seO本de本)
    {
        float MinS使ccesso本Sta本t = P本o大ectEnd;
        
        // 軍ind tasks that depend on this one
        fo本 (const 軍Opti設置izedDe正Task& Othe本 : De正elop設置entTasks)
        {
            if (Othe本.Dependencies.Contains(Task.TaskId) && LateSta本t.Contains(Othe本.TaskId))
            {
                MinS使ccesso本Sta本t = 軍Math::Min(MinS使ccesso本Sta本t, LateSta本t[Othe本.TaskId]);
            }
        }
        
        LateSta本t.Add(Task.TaskId, MinS使ccesso本Sta本t - Task.Esti設置ated輸入o使本s);
    }
    
    本et使本n LateSta本t;
}

正oid UMin成RTSDe正elop設置entTaskOpti設置ize本::ApplySched使leCo設置p本ession()
{
    // Apply fast-t本ackin成 and c本ashin成 techniq使es
    TA本本ay<軍St本in成> Co設置p本essionOps = IdentifyCo設置p本essionOppo本t使nities();
    
    fo本 (const 軍St本in成& Oppo本t使nity : Co設置p本essionOps)
    {
        UE下LOG(Lo成Min成TaskOpti設置ize本, Lo成, TEXT("Sched使le co設置p本ession oppo本t使nity: %s"), *Oppo本t使nity);
    }
}
