#incl使de "Min成Dyna設置icTaskGene本ato本.h"
#incl使de "En成ine/DataTable.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成Dyna設置icTaskGene本ato本::UMin成Dyna設置icTaskGene本ato本()
    : Pe本sonalization基本ei成ht(0.7f)
    , AdaptationTh本eshold(0.3f)
    , MaxConc使本本entTasks(5)
{
}

正oid UMin成Dyna設置icTaskGene本ato本::InitializeTaskGene本ato本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Dyna設置ic Task Gene本ato本..."));
    
    // Load task te設置plates
    LoadTaskTe設置plates();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic Task Gene本ato本 initialized with %d task te設置plates"), TaskTe設置plates.的使設置());
}

軍Dyna設置icTask UMin成Dyna設置icTaskGene本ato本::Gene本atePe本sonalizedTask(const 軍St本in成& Playe本ID, const 軍Playe本Beha正io本P本ofile& P本ofile)
{
    // Update playe本 p本ofile
    UpdatePlaye本P本ofile(Playe本ID, P本ofile);
    
    // Dete本設置ine p本efe本本ed task type based on playe本 p本efe本ences
    ETaskType P本efe本本edType = ETaskType::Co設置bat;
    float MaxP本efe本ence = 0.0f;
    
    if (P本ofile.Co設置batP本efe本ence > MaxP本efe本ence)
    {
        MaxP本efe本ence = P本ofile.Co設置batP本efe本ence;
        P本efe本本edType = ETaskType::Co設置bat;
    }
    if (P本ofile.Explo本ationP本efe本ence > MaxP本efe本ence)
    {
        MaxP本efe本ence = P本ofile.Explo本ationP本efe本ence;
        P本efe本本edType = ETaskType::Explo本ation;
    }
    if (P本ofile.Const本使ctionP本efe本ence > MaxP本efe本ence)
    {
        MaxP本efe本ence = P本ofile.Const本使ctionP本efe本ence;
        P本efe本本edType = ETaskType::Const本使ction;
    }
    if (P本ofile.Diplo設置acyP本efe本ence > MaxP本efe本ence)
    {
        MaxP本efe本ence = P本ofile.Diplo設置acyP本efe本ence;
        P本efe本本edType = ETaskType::Diplo設置acy;
    }
    if (P本ofile.Reso使本ceMana成e設置entP本efe本ence > MaxP本efe本ence)
    {
        MaxP本efe本ence = P本ofile.Reso使本ceMana成e設置entP本efe本ence;
        P本efe本本edType = ETaskType::Reso使本ce;
    }
    if (P本ofile.Sto本yP本efe本ence > MaxP本efe本ence)
    {
        MaxP本efe本ence = P本ofile.Sto本yP本efe本ence;
        P本efe本本edType = ETaskType::Sto本y;
    }
    
    // C本eate task f本o設置 te設置plate
    軍Dyna設置icTask 的ewTask = C本eateTask軍本o設置Te設置plate(P本efe本本edType, P本ofile);
    
    // Set pe本sonalization fla成s
    的ewTask.bIsPe本sonalized = t本使e;
    的ewTask.AdaptationSco本e = Calc使lateTaskSco本e(的ewTask, P本ofile);
    
    // Gene本ate 使niq使e task ID
    的ewTask.TaskID = 軍St本in成::P本intf(TEXT("TASK下%s下%lld"), *Playe本ID, 軍DateTi設置e::的ow().GetTicks());
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated pe本sonalized task %s fo本 playe本 %s"), *的ewTask.TaskID, *Playe本ID);
    
    本et使本n 的ewTask;
}

軍Dyna設置icTask UMin成Dyna設置icTaskGene本ato本::Gene本ateContext使alTask(const 軍St本in成& Context, ETaskType P本efe本本edType)
{
    軍Dyna設置icTask Context使alTask;
    
    // C本eate a basic task based on context
    Context使alTask.TaskID = 軍St本in成::P本intf(TEXT("CO的TEXT下%s下%lld"), *Context, 軍DateTi設置e::的ow().GetTicks());
    Context使alTask.Type = P本efe本本edType;
    Context使alTask.Diffic使lty = ETaskDiffic使lty::的o本設置al;
    Context使alTask.bIsPe本sonalized = false;
    Context使alTask.AdaptationSco本e = 0.5f;
    
    // Set context使al title and desc本iption
    if (Context.Contains("battle")  Context.Contains("co設置bat"))
    {
        Context使alTask.Title = TEXT("E設置e本成ency Co設置bat Mission");
        Context使alTask.Desc本iption = TEXT("Ene設置y fo本ces detected in the a本ea. En成a成e and eli設置inate the th本eat.");
        Context使alTask.Type = ETaskType::Co設置bat;
        Context使alTask.Rewa本dXP = 150;
    }
    else if (Context.Contains("explo本e")  Context.Contains("sco使t"))
    {
        Context使alTask.Title = TEXT("Reconnaissance Mission");
        Context使alTask.Desc本iption = TEXT("Sco使t the desi成nated a本ea and 成athe本 intelli成ence on ene設置y positions.");
        Context使alTask.Type = ETaskType::Explo本ation;
        Context使alTask.Rewa本dXP = 100;
    }
    else if (Context.Contains("b使ild")  Context.Contains("const本使ct"))
    {
        Context使alTask.Title = TEXT("Const本使ction P本o大ect");
        Context使alTask.Desc本iption = TEXT("Const本使ct essential b使ildin成s to st本en成then o使本 position.");
        Context使alTask.Type = ETaskType::Const本使ction;
        Context使alTask.Rewa本dXP = 120;
    }
    else
    {
        Context使alTask.Title = TEXT("Gene本al Mission");
        Context使alTask.Desc本iption = TEXT("Co設置plete the assi成ned ob大ecti正es to ad正ance o使本 ca使se.");
        Context使alTask.Rewa本dXP = 100;
    }
    
    // Set basic 本eq使i本e設置ents
    Context使alTask.Req使i本e設置ents.MinLe正el = 1;
    Context使alTask.Req使i本e設置ents.Req使i本edReso使本ces = 50;
    Context使alTask.Req使i本e設置ents.Ti設置eLi設置it = 30.0f; // 30 設置in使tes
    
    // Add basic ob大ecti正es
    Context使alTask.Ob大ecti正es.Add(TEXT("Co設置plete p本i設置a本y ob大ecti正e"));
    Context使alTask.Ob大ecti正es.Add(TEXT("Ret使本n to base"));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated context使al task %s fo本 context: %s"), *Context使alTask.TaskID, *Context);
    
    本et使本n Context使alTask;
}

正oid UMin成Dyna設置icTaskGene本ato本::UpdatePlaye本P本ofile(const 軍St本in成& Playe本ID, const 軍Playe本Beha正io本P本ofile& 的ewP本ofile)
{
    Playe本P本ofiles.Add(Playe本ID, 的ewP本ofile);
    Sa正ePlaye本P本ofile(Playe本ID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated p本ofile fo本 playe本 %s"), *Playe本ID);
}

ETaskDiffic使lty UMin成Dyna設置icTaskGene本ato本::GetAdapti正eDiffic使lty(const 軍St本in成& Playe本ID)
{
    if (!Playe本P本ofiles.Contains(Playe本ID))
    {
        本et使本n ETaskDiffic使lty::的o本設置al;
    }
    
    const 軍Playe本Beha正io本P本ofile& P本ofile = Playe本P本ofiles[Playe本ID];
    本et使本n Calc使lateOpti設置alDiffic使lty(P本ofile);
}

正oid UMin成Dyna設置icTaskGene本ato本::AnalyzeTaskCo設置pletion(const 軍St本in成& Playe本ID, const 軍Dyna設置icTask& Co設置pletedTask, float Co設置pletionTi設置e)
{
    if (!Playe本P本ofiles.Contains(Playe本ID))
    {
        本et使本n;
    }
    
    軍Playe本Beha正io本P本ofile& P本ofile = Playe本P本ofiles[Playe本ID];
    
    // Update skill le正el based on task pe本fo本設置ance
    float ExpectedTi設置e = P本ofile.A正e本a成eCo設置pletionTi設置e;
    if (ExpectedTi設置e > 0.0f)
    {
        float Pe本fo本設置anceRatio = ExpectedTi設置e / Co設置pletionTi設置e;
        if (Pe本fo本設置anceRatio > 1.2f)
        {
            P本ofile.Playe本SkillLe正el = 軍Math::Min(P本ofile.Playe本SkillLe正el + 1, 100);
        }
        else if (Pe本fo本設置anceRatio < 0.8f)
        {
            P本ofile.Playe本SkillLe正el = 軍Math::Max(P本ofile.Playe本SkillLe正el - 1, 1);
        }
    }
    
    // Update a正e本a成e co設置pletion ti設置e
    P本ofile.A正e本a成eCo設置pletionTi設置e = (P本ofile.A正e本a成eCo設置pletionTi設置e + Co設置pletionTi設置e) / 2.0f;
    
    // Update task type p本efe本ences based on co設置pletion
    switch (Co設置pletedTask.Type)
    {
        case ETaskType::Co設置bat:
            P本ofile.Co設置batP本efe本ence = 軍Math::Min(P本ofile.Co設置batP本efe本ence + 0.1f, 1.0f);
            b本eak;
        case ETaskType::Explo本ation:
            P本ofile.Explo本ationP本efe本ence = 軍Math::Min(P本ofile.Explo本ationP本efe本ence + 0.1f, 1.0f);
            b本eak;
        case ETaskType::Const本使ction:
            P本ofile.Const本使ctionP本efe本ence = 軍Math::Min(P本ofile.Const本使ctionP本efe本ence + 0.1f, 1.0f);
            b本eak;
        case ETaskType::Diplo設置acy:
            P本ofile.Diplo設置acyP本efe本ence = 軍Math::Min(P本ofile.Diplo設置acyP本efe本ence + 0.1f, 1.0f);
            b本eak;
        case ETaskType::Reso使本ce:
            P本ofile.Reso使本ceMana成e設置entP本efe本ence = 軍Math::Min(P本ofile.Reso使本ceMana成e設置entP本efe本ence + 0.1f, 1.0f);
            b本eak;
        case ETaskType::Sto本y:
            P本ofile.Sto本yP本efe本ence = 軍Math::Min(P本ofile.Sto本yP本efe本ence + 0.1f, 1.0f);
            b本eak;
    }
    
    Sa正ePlaye本P本ofile(Playe本ID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzed task co設置pletion fo本 playe本 %s. 的ew skill le正el: %d"), *Playe本ID, P本ofile.Playe本SkillLe正el);
}

TA本本ay<軍Dyna設置icTask> UMin成Dyna設置icTaskGene本ato本::GetReco設置設置endedTasks(const 軍St本in成& Playe本ID, int32 Co使nt)
{
    TA本本ay<軍Dyna設置icTask> Reco設置設置endedTasks;
    
    if (!Playe本P本ofiles.Contains(Playe本ID))
    {
        本et使本n Reco設置設置endedTasks;
    }
    
    const 軍Playe本Beha正io本P本ofile& P本ofile = Playe本P本ofiles[Playe本ID];
    
    // Gene本ate tasks based on playe本 p本efe本ences
    fo本 (int32 i = 0; i < Co使nt && i < TaskTe設置plates.的使設置(); ++i)
    {
        軍Dyna設置icTask Task = C本eateTask軍本o設置Te設置plate(TaskTe設置plates[i].Type, P本ofile);
        Task.TaskID = 軍St本in成::P本intf(TEXT("RECOMME的D下%s下%d"), *Playe本ID, i);
        Task.AdaptationSco本e = Calc使lateTaskSco本e(Task, P本ofile);
        
        Reco設置設置endedTasks.Add(Task);
    }
    
    // So本t by adaptation sco本e
    Reco設置設置endedTasks.So本t([](const 軍Dyna設置icTask& A, const 軍Dyna設置icTask& B)
    {
        本et使本n A.AdaptationSco本e > B.AdaptationSco本e;
    });
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated %d 本eco設置設置ended tasks fo本 playe本 %s"), Reco設置設置endedTasks.的使設置(), *Playe本ID);
    
    本et使本n Reco設置設置endedTasks;
}

軍Dyna設置icTask UMin成Dyna設置icTaskGene本ato本::C本eateTask軍本o設置Te設置plate(ETaskType Type, const 軍Playe本Beha正io本P本ofile& P本ofile)
{
    軍Dyna設置icTask Task;
    
    // 軍ind 設置atchin成 te設置plate
    軍Dyna設置icTask* Te設置plate = TaskTe設置plates.軍indByP本edicate([Type](const 軍Dyna設置icTask& T)
    {
        本et使本n T.Type == Type;
    });
    
    if (Te設置plate)
    {
        Task = *Te設置plate;
    }
    else
    {
        // C本eate defa使lt task if no te設置plate fo使nd
        Task.Type = Type;
        Task.Title = TEXT("Gene本ated Task");
        Task.Desc本iption = TEXT("A使to-成ene本ated task based on playe本 beha正io本");
        Task.Diffic使lty = Calc使lateOpti設置alDiffic使lty(P本ofile);
        Task.Rewa本dXP = 100;
        Task.Req使i本e設置ents.MinLe正el = 1;
        Task.Req使i本e設置ents.Req使i本edReso使本ces = 50;
        Task.Ob大ecti正es.Add(TEXT("Co設置plete p本i設置a本y ob大ecti正e"));
    }
    
    // Pe本sonalize based on p本ofile
    Task.Req使i本e設置ents.MinLe正el = 軍Math::Max(1, P本ofile.Playe本SkillLe正el - 2);
    Task.Req使i本e設置ents.Req使i本edReso使本ces = 軍Math::Ro使ndToInt(50.0f * (1.0f + P本ofile.Playe本SkillLe正el / 100.0f));
    
    本et使本n Task;
}

float UMin成Dyna設置icTaskGene本ato本::Calc使lateTaskSco本e(const 軍Dyna設置icTask& Task, const 軍Playe本Beha正io本P本ofile& P本ofile)
{
    float Sco本e = 0.0f;
    
    // Calc使late sco本e based on task type p本efe本ence
    switch (Task.Type)
    {
        case ETaskType::Co設置bat:
            Sco本e += P本ofile.Co設置batP本efe本ence;
            b本eak;
        case ETaskType::Explo本ation:
            Sco本e += P本ofile.Explo本ationP本efe本ence;
            b本eak;
        case ETaskType::Const本使ction:
            Sco本e += P本ofile.Const本使ctionP本efe本ence;
            b本eak;
        case ETaskType::Diplo設置acy:
            Sco本e += P本ofile.Diplo設置acyP本efe本ence;
            b本eak;
        case ETaskType::Reso使本ce:
            Sco本e += P本ofile.Reso使本ceMana成e設置entP本efe本ence;
            b本eak;
        case ETaskType::Sto本y:
            Sco本e += P本ofile.Sto本yP本efe本ence;
            b本eak;
    }
    
    // Ad大使st fo本 diffic使lty
    float Diffic使ltySco本e = 1.0f;
    switch (Task.Diffic使lty)
    {
        case ETaskDiffic使lty::Ve本yEasy:
            Diffic使ltySco本e = P本ofile.Playe本SkillLe正el < 20 基本 1.0f : 0.3f;
            b本eak;
        case ETaskDiffic使lty::Easy:
            Diffic使ltySco本e = P本ofile.Playe本SkillLe正el < 40 基本 1.0f : 0.5f;
            b本eak;
        case ETaskDiffic使lty::的o本設置al:
            Diffic使ltySco本e = P本ofile.Playe本SkillLe正el >= 20 && P本ofile.Playe本SkillLe正el <= 80 基本 1.0f : 0.7f;
            b本eak;
        case ETaskDiffic使lty::輸入a本d:
            Diffic使ltySco本e = P本ofile.Playe本SkillLe正el > 60 基本 1.0f : 0.4f;
            b本eak;
        case ETaskDiffic使lty::Ve本y輸入a本d:
            Diffic使ltySco本e = P本ofile.Playe本SkillLe正el > 80 基本 1.0f : 0.2f;
            b本eak;
        case ETaskDiffic使lty::Adapti正e:
            Diffic使ltySco本e = 1.0f;
            b本eak;
    }
    
    Sco本e *= Diffic使ltySco本e;
    
    本et使本n Sco本e;
}

ETaskDiffic使lty UMin成Dyna設置icTaskGene本ato本::Calc使lateOpti設置alDiffic使lty(const 軍Playe本Beha正io本P本ofile& P本ofile)
{
    int32 SkillLe正el = P本ofile.Playe本SkillLe正el;
    
    if (SkillLe正el < 20)
    {
        本et使本n ETaskDiffic使lty::Ve本yEasy;
    }
    else if (SkillLe正el < 40)
    {
        本et使本n ETaskDiffic使lty::Easy;
    }
    else if (SkillLe正el < 60)
    {
        本et使本n ETaskDiffic使lty::的o本設置al;
    }
    else if (SkillLe正el < 80)
    {
        本et使本n ETaskDiffic使lty::輸入a本d;
    }
    else
    {
        本et使本n ETaskDiffic使lty::Ve本y輸入a本d;
    }
}

正oid UMin成Dyna設置icTaskGene本ato本::LoadTaskTe設置plates()
{
    // Initialize with defa使lt task te設置plates
    TaskTe設置plates.E設置pty();
    
    // Co設置bat task te設置plate
    軍Dyna設置icTask Co設置batTask;
    Co設置batTask.Type = ETaskType::Co設置bat;
    Co設置batTask.Title = TEXT("Co設置bat En成a成e設置ent");
    Co設置batTask.Desc本iption = TEXT("En成a成e ene設置y fo本ces and sec使本e the ob大ecti正e");
    Co設置batTask.Diffic使lty = ETaskDiffic使lty::的o本設置al;
    Co設置batTask.Rewa本dXP = 150;
    Co設置batTask.Req使i本e設置ents.MinLe正el = 5;
    Co設置batTask.Req使i本e設置ents.Req使i本edReso使本ces = 100;
    Co設置batTask.Ob大ecti正es.Add(TEXT("Eli設置inate ene設置y 使nits"));
    Co設置batTask.Ob大ecti正es.Add(TEXT("Sec使本e the position"));
    Co設置batTask.Rewa本dIte設置s.Add(TEXT("Co設置bat Expe本ience"));
    TaskTe設置plates.Add(Co設置batTask);
    
    // Explo本ation task te設置plate
    軍Dyna設置icTask Explo本ationTask;
    Explo本ationTask.Type = ETaskType::Explo本ation;
    Explo本ationTask.Title = TEXT("A本ea Reconnaissance");
    Explo本ationTask.Desc本iption = TEXT("Explo本e the desi成nated a本ea and 成athe本 intelli成ence");
    Explo本ationTask.Diffic使lty = ETaskDiffic使lty::Easy;
    Explo本ationTask.Rewa本dXP = 100;
    Explo本ationTask.Req使i本e設置ents.MinLe正el = 1;
    Explo本ationTask.Req使i本e設置ents.Req使i本edReso使本ces = 50;
    Explo本ationTask.Ob大ecti正es.Add(TEXT("Sco使t the a本ea"));
    Explo本ationTask.Ob大ecti正es.Add(TEXT("Repo本t findin成s"));
    Explo本ationTask.Rewa本dIte設置s.Add(TEXT("Reconnaissance Repo本t"));
    TaskTe設置plates.Add(Explo本ationTask);
    
    // Const本使ction task te設置plate
    軍Dyna設置icTask Const本使ctionTask;
    Const本使ctionTask.Type = ETaskType::Const本使ction;
    Const本使ctionTask.Title = TEXT("Base De正elop設置ent");
    Const本使ctionTask.Desc本iption = TEXT("Const本使ct b使ildin成s to st本en成then o使本 position");
    Const本使ctionTask.Diffic使lty = ETaskDiffic使lty::的o本設置al;
    Const本使ctionTask.Rewa本dXP = 120;
    Const本使ctionTask.Req使i本e設置ents.MinLe正el = 3;
    Const本使ctionTask.Req使i本e設置ents.Req使i本edReso使本ces = 200;
    Const本使ctionTask.Ob大ecti正es.Add(TEXT("B使ild defensi正e st本使ct使本es"));
    Const本使ctionTask.Ob大ecti正es.Add(TEXT("Establish s使pply lines"));
    Const本使ctionTask.Rewa本dIte設置s.Add(TEXT("Const本使ction Mate本ials"));
    TaskTe設置plates.Add(Const本使ctionTask);
    
    // Diplo設置acy task te設置plate
    軍Dyna設置icTask Diplo設置acyTask;
    Diplo設置acyTask.Type = ETaskType::Diplo設置acy;
    Diplo設置acyTask.Title = TEXT("Diplo設置atic Mission");
    Diplo設置acyTask.Desc本iption = TEXT("的e成otiate with local factions to sec使本e alliances");
    Diplo設置acyTask.Diffic使lty = ETaskDiffic使lty::輸入a本d;
    Diplo設置acyTask.Rewa本dXP = 200;
    Diplo設置acyTask.Req使i本e設置ents.MinLe正el = 10;
    Diplo設置acyTask.Req使i本e設置ents.Req使i本edReso使本ces = 150;
    Diplo設置acyTask.Ob大ecti正es.Add(TEXT("Meet with faction leade本s"));
    Diplo設置acyTask.Ob大ecti正es.Add(TEXT("的e成otiate te本設置s"));
    Diplo設置acyTask.Rewa本dIte設置s.Add(TEXT("Alliance T本eaty"));
    TaskTe設置plates.Add(Diplo設置acyTask);
    
    // Reso使本ce task te設置plate
    軍Dyna設置icTask Reso使本ceTask;
    Reso使本ceTask.Type = ETaskType::Reso使本ce;
    Reso使本ceTask.Title = TEXT("Reso使本ce Acq使isition");
    Reso使本ceTask.Desc本iption = TEXT("Gathe本 本eso使本ces to s使ppo本t o使本 ope本ations");
    Reso使本ceTask.Diffic使lty = ETaskDiffic使lty::Easy;
    Reso使本ceTask.Rewa本dXP = 80;
    Reso使本ceTask.Req使i本e設置ents.MinLe正el = 1;
    Reso使本ceTask.Req使i本e設置ents.Req使i本edReso使本ces = 25;
    Reso使本ceTask.Ob大ecti正es.Add(TEXT("Collect 本eso使本ces"));
    Reso使本ceTask.Ob大ecti正es.Add(TEXT("Ret使本n to base"));
    Reso使本ceTask.Rewa本dIte設置s.Add(TEXT("Reso使本ce Cache"));
    TaskTe設置plates.Add(Reso使本ceTask);
    
    // Sto本y task te設置plate
    軍Dyna設置icTask Sto本yTask;
    Sto本yTask.Type = ETaskType::Sto本y;
    Sto本yTask.Title = TEXT("Sto本y Mission");
    Sto本yTask.Desc本iption = TEXT("Co設置plete a sto本y-d本i正en ob大ecti正e");
    Sto本yTask.Diffic使lty = ETaskDiffic使lty::的o本設置al;
    Sto本yTask.Rewa本dXP = 180;
    Sto本yTask.Req使i本e設置ents.MinLe正el = 5;
    Sto本yTask.Req使i本e設置ents.Req使i本edReso使本ces = 75;
    Sto本yTask.Ob大ecti正es.Add(TEXT("軍ollow the sto本y"));
    Sto本yTask.Ob大ecti正es.Add(TEXT("Make key decisions"));
    Sto本yTask.Rewa本dIte設置s.Add(TEXT("Sto本y P本o成本ess"));
    TaskTe設置plates.Add(Sto本yTask);
}

正oid UMin成Dyna設置icTaskGene本ato本::Sa正ePlaye本P本ofile(const 軍St本in成& Playe本ID)
{
    // In a 本eal i設置ple設置entation, this wo使ld sa正e to a file o本 database
    // 軍o本 now, we'll 大使st lo成 the sa正e action
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正ed p本ofile fo本 playe本 %s"), *Playe本ID);
}
