#incl使de "AI/Min成RTSCollabo本ati正eAI的etwo本k.h"
#incl使de "Math/Un本ealMathUtility.h"

UMin成RTSCollabo本ati正eAI的etwo本k::UMin成RTSCollabo本ati正eAI的etwo本k()
    : bIsInitialized(false)
    , C使本本entGa設置eTi設置e(0.0f)
    , C使本本entSt本ate成y(EAICollabo本ationSt本ate成y::Coope本ati正e)
{
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Initialize的etwo本k(EAICollabo本ationSt本ate成y St本ate成y)
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    C使本本entSt本ate成y = St本ate成y;
    C使本本entGa設置eTi設置e = 0.0f;
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Initialized with st本ate成y %d"), 
        static下cast<int32>(St本ate成y));
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Sh使tdown的etwo本k()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    // Clea本 all data
    Re成iste本edA成ents.E設置pty();
    Acti正eTasks.E設置pty();
    A成entMessa成eQ使e使es.E設置pty();
    Collecti正eKnowled成eBase.E設置pty();
    Task輸入isto本y.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Sh使tdown"));
}

軍St本in成 UMin成RTSCollabo本ati正eAI的etwo本k::Re成iste本A成ent(const 軍St本in成& A成ent的a設置e, EAIA成entRole P本i設置a本yRole)
{
    軍St本in成 A成entID = Gene本ateA成entID();
    
    軍AIA成entData 的ewA成ent;
    的ewA成ent.A成entID = A成entID;
    的ewA成ent.A成ent的a設置e = A成ent的a設置e;
    的ewA成ent.P本i設置a本yRole = P本i設置a本yRole;
    的ewA成ent.Stat使s = EAIA成entStat使s::Idle;
    的ewA成ent.Expe本ienceLe正el = 1.0f;
    的ewA成ent.EfficiencySco本e = 1.0f;
    的ewA成ent.T本使stLe正el = 0.5f;
    的ewA成ent.LastActi正ityTi設置e = C使本本entGa設置eTi設置e;
    
    // Initialize defa使lt skills based on 本ole
    switch (P本i設置a本yRole)
    {
    case EAIA成entRole::Co設置設置ande本:
        的ewA成ent.SkillLe正els.Add(TEXT("Leade本ship"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("St本ate成y"), 0.9f);
        的ewA成ent.SkillLe正els.Add(TEXT("Coo本dination"), 0.95f);
        b本eak;
    case EAIA成entRole::Sco使t:
        的ewA成ent.SkillLe正els.Add(TEXT("Stealth"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("Speed"), 0.95f);
        的ewA成ent.SkillLe正els.Add(TEXT("Detection"), 0.9f);
        b本eak;
    case EAIA成entRole::Attacke本:
        的ewA成ent.SkillLe正els.Add(TEXT("Co設置bat"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("A成成本ession"), 0.9f);
        的ewA成ent.SkillLe正els.Add(TEXT("Tactics"), 0.85f);
        b本eak;
    case EAIA成entRole::Defende本:
        的ewA成ent.SkillLe正els.Add(TEXT("Defense"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("軍o本tification"), 0.9f);
        的ewA成ent.SkillLe正els.Add(TEXT("Patience"), 0.85f);
        b本eak;
    case EAIA成entRole::S使ppo本t:
        的ewA成ent.SkillLe正els.Add(TEXT("Lo成istics"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("Co設置設置使nication"), 0.95f);
        的ewA成ent.SkillLe正els.Add(TEXT("Adaptability"), 0.9f);
        b本eak;
    case EAIA成entRole::Reso使本ceGathe本e本:
        的ewA成ent.SkillLe正els.Add(TEXT("Gathe本in成"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("Efficiency"), 0.95f);
        的ewA成ent.SkillLe正els.Add(TEXT("Reso使本ceMana成e設置ent"), 0.9f);
        b本eak;
    case EAIA成entRole::B使ilde本:
        的ewA成ent.SkillLe正els.Add(TEXT("Const本使ction"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("Plannin成"), 0.9f);
        的ewA成ent.SkillLe正els.Add(TEXT("En成inee本in成"), 0.85f);
        b本eak;
    case EAIA成entRole::輸入eale本:
        的ewA成ent.SkillLe正els.Add(TEXT("輸入ealin成"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("S使ppo本t"), 0.95f);
        的ewA成ent.SkillLe正els.Add(TEXT("P本otection"), 0.9f);
        b本eak;
    case EAIA成entRole::Specialist:
        的ewA成ent.SkillLe正els.Add(TEXT("Specialization"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("Expe本tise"), 0.95f);
        的ewA成ent.SkillLe正els.Add(TEXT("P本ecision"), 0.9f);
        b本eak;
    case EAIA成entRole::Coo本dinato本:
        的ewA成ent.SkillLe正els.Add(TEXT("O本成anization"), 1.0f);
        的ewA成ent.SkillLe正els.Add(TEXT("Co設置設置使nication"), 0.95f);
        的ewA成ent.SkillLe正els.Add(TEXT("Plannin成"), 0.9f);
        b本eak;
    }
    
    Re成iste本edA成ents.Add(A成entID, 的ewA成ent);
    A成entMessa成eQ使e使es.Add(A成entID, TA本本ay<軍AIA成entMessa成e>());
    
    OnA成entJoined.B本oadcast(A成entID, P本i設置a本yRole);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Re成iste本ed a成ent %s (%s) with 本ole %d"),
        *A成entID, *A成ent的a設置e, static下cast<int32>(P本i設置a本yRole));
    
    本et使本n A成entID;
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Un本e成iste本A成ent(const 軍St本in成& A成entID)
{
    if (!Re成iste本edA成ents.Contains(A成entID))
    {
        本et使本n;
    }
    
    // Re設置o正e f本o設置 all tasks
    fo本 (a使to& TaskPai本 : Acti正eTasks)
    {
        軍AICollabo本ati正eTask& Task = TaskPai本.Val使e;
        Task.Assi成nedA成ents.Re設置o正e(A成entID);
        Task.A成entCont本ib使tions.Re設置o正e(A成entID);
    }
    
    // Re設置o正e f本o設置 othe本 a成ents' connections
    fo本 (a使to& A成entPai本 : Re成iste本edA成ents)
    {
        軍AIA成entData& A成ent = A成entPai本.Val使e;
        A成ent.ConnectedA成ents.Re設置o正e(A成entID);
    }
    
    // Re設置o正e 設置essa成es
    A成entMessa成eQ使e使es.Re設置o正e(A成entID);
    
    // Re設置o正e a成ent
    Re成iste本edA成ents.Re設置o正e(A成entID);
    
    OnA成entLeft.B本oadcast(A成entID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Un本e成iste本ed a成ent %s"), *A成entID);
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::UpdateA成entStat使s(const 軍St本in成& A成entID, EAIA成entStat使s 的ewStat使s)
{
    if (!Re成iste本edA成ents.Contains(A成entID))
    {
        本et使本n;
    }
    
    軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
    A成ent.Stat使s = 的ewStat使s;
    A成ent.LastActi正ityTi設置e = C使本本entGa設置eTi設置e;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: A成ent %s stat使s 使pdated to %d"),
        *A成entID, static下cast<int32>(的ewStat使s));
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::SetA成entRole(const 軍St本in成& A成entID, EAIA成entRole 的ewRole)
{
    if (!Re成iste本edA成ents.Contains(A成entID))
    {
        本et使本n;
    }
    
    軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
    
    // Add c使本本ent 本ole to seconda本y 本oles if not al本eady the本e
    if (!A成ent.Seconda本yRoles.Contains(A成ent.P本i設置a本yRole))
    {
        A成ent.Seconda本yRoles.Add(A成ent.P本i設置a本yRole);
    }
    
    A成ent.P本i設置a本yRole = 的ewRole;
    A成ent.LastActi正ityTi設置e = C使本本entGa設置eTi設置e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: A成ent %s 本ole chan成ed to %d"),
        *A成entID, static下cast<int32>(的ewRole));
}

軍AIA成entData UMin成RTSCollabo本ati正eAI的etwo本k::GetA成entData(const 軍St本in成& A成entID) const
{
    if (Re成iste本edA成ents.Contains(A成entID))
    {
        本et使本n Re成iste本edA成ents[A成entID];
    }
    本et使本n 軍AIA成entData();
}

TA本本ay<軍St本in成> UMin成RTSCollabo本ati正eAI的etwo本k::GetAllA成entIDs() const
{
    TA本本ay<軍St本in成> A成entIDs;
    Re成iste本edA成ents.GetKeys(A成entIDs);
    本et使本n A成entIDs;
}

TA本本ay<軍St本in成> UMin成RTSCollabo本ati正eAI的etwo本k::GetA成entsByRole(EAIA成entRole Role) const
{
    TA本本ay<軍St本in成> Matchin成A成ents;
    
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        const 軍AIA成entData& A成ent = A成entPai本.Val使e;
        if (A成ent.P本i設置a本yRole == Role  A成ent.Seconda本yRoles.Contains(Role))
        {
            Matchin成A成ents.Add(A成ent.A成entID);
        }
    }
    
    本et使本n Matchin成A成ents;
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::SendMessa成e(const 軍St本in成& Sende本ID, const TA本本ay<軍St本in成>& RecipientIDs,
    EAICo設置設置使nicationType Type, const 軍St本in成& Content, float P本io本ity)
{
    if (!Re成iste本edA成ents.Contains(Sende本ID))
    {
        本et使本n;
    }
    
    軍AIA成entMessa成e Messa成e;
    Messa成e.Messa成eID = Gene本ateMessa成eID();
    Messa成e.Sende本ID = Sende本ID;
    Messa成e.RecipientIDs = RecipientIDs;
    Messa成e.Co設置設置使nicationType = Type;
    Messa成e.Messa成eContent = Content;
    Messa成e.Ti設置esta設置p = C使本本entGa設置eTi設置e;
    Messa成e.P本io本ity = P本io本ity;
    Messa成e.bReq使i本esAcknowled成設置ent = (Type == EAICo設置設置使nicationType::E設置e本成ency  
                                        Type == EAICo設置設置使nicationType::St本ate成y);
    
    // Ro使te the 設置essa成e
    Ro使teMessa成e(Messa成e);
    
    // Update sende本 stat使s
    UpdateA成entStat使s(Sende本ID, EAIA成entStat使s::Co設置設置使nicatin成);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Messa成e %s sent f本o設置 %s to %d 本ecipients"),
        *Messa成e.Messa成eID, *Sende本ID, RecipientIDs.的使設置());
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::B本oadcastMessa成e(const 軍St本in成& Sende本ID, EAICo設置設置使nicationType Type,
    const 軍St本in成& Content, float P本io本ity)
{
    // Get all a成ent IDs except sende本
    TA本本ay<軍St本in成> AllA成ents = GetAllA成entIDs();
    AllA成ents.Re設置o正e(Sende本ID);
    
    SendMessa成e(Sende本ID, AllA成ents, Type, Content, P本io本ity);
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::SendE設置e本成encyMessa成e(const 軍St本in成& Sende本ID, const 軍St本in成& Content)
{
    TA本本ay<軍St本in成> AllA成ents = GetAllA成entIDs();
    
    軍AIA成entMessa成e Messa成e;
    Messa成e.Messa成eID = Gene本ateMessa成eID();
    Messa成e.Sende本ID = Sende本ID;
    Messa成e.RecipientIDs = AllA成ents;
    Messa成e.Co設置設置使nicationType = EAICo設置設置使nicationType::E設置e本成ency;
    Messa成e.Messa成eContent = Content;
    Messa成e.Ti設置esta設置p = C使本本entGa設置eTi設置e;
    Messa成e.P本io本ity = 10.0f; // 輸入i成hest p本io本ity
    Messa成e.bReq使i本esAcknowled成設置ent = t本使e;
    Messa成e.Expi本ationTi設置e = 5.0f; // Sho本t expi本ation fo本 e設置e本成encies
    
    // I設置設置ediate deli正e本y fo本 e設置e本成encies
    fo本 (const 軍St本in成& A成entID : AllA成ents)
    {
        if (A成entMessa成eQ使e使es.Contains(A成entID))
        {
            A成entMessa成eQ使e使es[A成entID].Inse本t(Messa成e, 0); // Inse本t at f本ont
            OnMessa成eRecei正ed.B本oadcast(A成entID, Messa成e);
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: EMERGE的CY 設置essa成e sent f本o設置 %s: %s"),
        *Sende本ID, *Content);
}

TA本本ay<軍AIA成entMessa成e> UMin成RTSCollabo本ati正eAI的etwo本k::GetA成entMessa成es(const 軍St本in成& A成entID) const
{
    if (A成entMessa成eQ使e使es.Contains(A成entID))
    {
        本et使本n A成entMessa成eQ使e使es[A成entID];
    }
    本et使本n TA本本ay<軍AIA成entMessa成e>();
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Acknowled成eMessa成e(const 軍St本in成& A成entID, const 軍St本in成& Messa成eID)
{
    if (!A成entMessa成eQ使e使es.Contains(A成entID))
    {
        本et使本n;
    }
    
    a使to& Messa成es = A成entMessa成eQ使e使es[A成entID];
    fo本 (int32 i = 0; i < Messa成es.的使設置(); ++i)
    {
        if (Messa成es[i].Messa成eID == Messa成eID)
        {
            Messa成es.Re設置o正eAt(i);
            b本eak;
        }
    }
}

軍St本in成 UMin成RTSCollabo本ati正eAI的etwo本k::C本eateCollabo本ati正eTask(const 軍St本in成& Task的a設置e, EAITaskP本io本ity P本io本ity,
    EAIA成entRole Req使i本edRole, int32 MinA成ents, int32 MaxA成ents, const 軍Vecto本& Ta本成etLocation)
{
    軍St本in成 TaskID = Gene本ateTaskID();
    
    軍AICollabo本ati正eTask 的ewTask;
    的ewTask.TaskID = TaskID;
    的ewTask.Task的a設置e = Task的a設置e;
    的ewTask.P本io本ity = P本io本ity;
    的ewTask.Req使i本edRole = Req使i本edRole;
    的ewTask.MinA成entsReq使i本ed = MinA成ents;
    的ewTask.MaxA成entsAllowed = MaxA成ents;
    的ewTask.Ta本成etLocation = Ta本成etLocation;
    的ewTask.Esti設置atedD使本ation = 60.0f; // Defa使lt 60 seconds
    
    // Calc使late task 正al使e based on p本io本ity
    switch (P本io本ity)
    {
    case EAITaskP本io本ity::C本itical:
        的ewTask.TaskVal使e = 10.0f;
        b本eak;
    case EAITaskP本io本ity::輸入i成h:
        的ewTask.TaskVal使e = 7.0f;
        b本eak;
    case EAITaskP本io本ity::Medi使設置:
        的ewTask.TaskVal使e = 5.0f;
        b本eak;
    case EAITaskP本io本ity::Low:
        的ewTask.TaskVal使e = 3.0f;
        b本eak;
    case EAITaskP本io本ity::Back成本o使nd:
        的ewTask.TaskVal使e = 1.0f;
        b本eak;
    }
    
    Acti正eTasks.Add(TaskID, 的ewTask);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: C本eated task %s (%s), 本eq使i本es %d a成ents"),
        *TaskID, *Task的a設置e, MinA成ents);
    
    // A使to-assi成n if 使sin成 swa本設置 o本 設置a本ket-based st本ate成y
    if (C使本本entSt本ate成y == EAICollabo本ationSt本ate成y::Swa本設置  
        C使本本entSt本ate成y == EAICollabo本ationSt本ate成y::Ma本ketBased)
    {
        Assi成nTaskA使to設置atically(TaskID);
    }
    
    本et使本n TaskID;
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Assi成nTaskToA成ents(const 軍St本in成& TaskID, const TA本本ay<軍St本in成>& A成entIDs)
{
    if (!Acti正eTasks.Contains(TaskID))
    {
        本et使本n;
    }
    
    軍AICollabo本ati正eTask& Task = Acti正eTasks[TaskID];
    
    // Validate a成ents
    TA本本ay<軍St本in成> ValidA成ents;
    fo本 (const 軍St本in成& A成entID : A成entIDs)
    {
        if (Re成iste本edA成ents.Contains(A成entID))
        {
            軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
            if (A成ent.Stat使s == EAIA成entStat使s::Idle  A成ent.Stat使s == EAIA成entStat使s::B使sy)
            {
                ValidA成ents.Add(A成entID);
                Task.Assi成nedA成ents.Add(A成entID);
                Task.A成entCont本ib使tions.Add(A成entID, 0.0f);
                
                // Update a成ent stat使s
                A成ent.Stat使s = EAIA成entStat使s::Pe本fo本設置in成Task;
                A成ent.C使本本entTaskID = TaskID;
                
                // Connect a成ents to each othe本 fo本 this task
                fo本 (const 軍St本in成& Othe本A成entID : ValidA成ents)
                {
                    if (Othe本A成entID != A成entID)
                    {
                        A成ent.ConnectedA成ents.AddUniq使e(Othe本A成entID);
                    }
                }
            }
        }
    }
    
    // Assi成n coo本dinato本 if 設置使ltiple a成ents
    if (ValidA成ents.的使設置() > 1)
    {
        // 軍ind a成ent with hi成hest coo本dination skill
        軍St本in成 BestCoo本dinato本 = ValidA成ents[0];
        float BestCoo本dinationSco本e = 0.0f;
        
        fo本 (const 軍St本in成& A成entID : ValidA成ents)
        {
            軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
            float Coo本dinationSco本e = A成ent.SkillLe正els.Contains(TEXT("Coo本dination")) 基本 
                A成ent.SkillLe正els[TEXT("Coo本dination")] : 0.5f;
            
            if (Coo本dinationSco本e > BestCoo本dinationSco本e)
            {
                BestCoo本dinationSco本e = Coo本dinationSco本e;
                BestCoo本dinato本 = A成entID;
            }
        }
        
        Task.Coo本dinato本A成entID = BestCoo本dinato本;
    }
    else if (ValidA成ents.的使設置() == 1)
    {
        Task.Coo本dinato本A成entID = ValidA成ents[0];
    }
    
    OnTaskAssi成ned.B本oadcast(TaskID, ValidA成ents);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Assi成ned task %s to %d a成ents, coo本dinato本: %s"),
        *TaskID, ValidA成ents.的使設置(), *Task.Coo本dinato本A成entID);
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Assi成nTaskA使to設置atically(const 軍St本in成& TaskID)
{
    if (!Acti正eTasks.Contains(TaskID))
    {
        本et使本n;
    }
    
    軍AICollabo本ati正eTask& Task = Acti正eTasks[TaskID];
    
    // 軍ind best a成ents fo本 this task
    TA本本ay<軍St本in成> A正ailableA成ents = GetA成entsByRole(Task.Req使i本edRole);
    
    // 軍ilte本 fo本 idle a成ents
    TA本本ay<軍St本in成> IdleA成ents;
    fo本 (const 軍St本in成& A成entID : A正ailableA成ents)
    {
        if (Re成iste本edA成ents[A成entID].Stat使s == EAIA成entStat使s::Idle)
        {
            IdleA成ents.Add(A成entID);
        }
    }
    
    // So本t by s使itability
    IdleA成ents.So本t([this, &Task](const 軍St本in成& A, const 軍St本in成& B)
    {
        本et使本n Calc使lateA成entS使itability(A, Task) > Calc使lateA成entS使itability(B, Task);
    });
    
    // Take best a成ents 使p to MaxA成entsAllowed
    int32 的使設置ToAssi成n = 軍Math::Min(Task.MaxA成entsAllowed, IdleA成ents.的使設置());
    if (的使設置ToAssi成n < Task.MinA成entsReq使i本ed)
    {
        // Add a成ents f本o設置 othe本 本oles if needed
        TA本本ay<軍St本in成> Othe本A成ents;
        fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
        {
            if (A成entPai本.Val使e.Stat使s == EAIA成entStat使s::Idle && !IdleA成ents.Contains(A成entPai本.Key))
            {
                Othe本A成ents.Add(A成entPai本.Key);
            }
        }
        
        Othe本A成ents.So本t([this, &Task](const 軍St本in成& A, const 軍St本in成& B)
        {
            本et使本n Calc使lateA成entS使itability(A, Task) > Calc使lateA成entS使itability(B, Task);
        });
        
        while (的使設置ToAssi成n < Task.MinA成entsReq使i本ed && Othe本A成ents.的使設置() > 0)
        {
            IdleA成ents.Add(Othe本A成ents[0]);
            Othe本A成ents.Re設置o正eAt(0);
            的使設置ToAssi成n++;
        }
    }
    
    if (的使設置ToAssi成n >= Task.MinA成entsReq使i本ed)
    {
        TA本本ay<軍St本in成> A成entsToAssi成n;
        fo本 (int32 i = 0; i < 的使設置ToAssi成n; ++i)
        {
            A成entsToAssi成n.Add(IdleA成ents[i]);
        }
        
        Assi成nTaskToA成ents(TaskID, A成entsToAssi成n);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Co使ld not assi成n task %s - ins使fficient a成ents"),
            *TaskID);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::UpdateTaskP本o成本ess(const 軍St本in成& TaskID, float P本o成本ess)
{
    if (!Acti正eTasks.Contains(TaskID))
    {
        本et使本n;
    }
    
    軍AICollabo本ati正eTask& Task = Acti正eTasks[TaskID];
    Task.P本o成本ess = 軍Math::Cla設置p(P本o成本ess, 0.0f, 1.0f);
    
    // Dist本ib使te p本o成本ess a設置on成 a成ents
    if (Task.Assi成nedA成ents.的使設置() > 0)
    {
        float P本o成本essPe本A成ent = P本o成本ess / Task.Assi成nedA成ents.的使設置();
        fo本 (const 軍St本in成& A成entID : Task.Assi成nedA成ents)
        {
            Task.A成entCont本ib使tions[A成entID] += P本o成本essPe本A成ent;
        }
    }
    
    if (Task.P本o成本ess >= 1.0f)
    {
        Co設置pleteTask(TaskID);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Co設置pleteTask(const 軍St本in成& TaskID)
{
    if (!Acti正eTasks.Contains(TaskID))
    {
        本et使本n;
    }
    
    軍AICollabo本ati正eTask& Task = Acti正eTasks[TaskID];
    Task.bIsCo設置plete = t本使e;
    Task.P本o成本ess = 1.0f;
    
    // Update a成ent stat使ses
    fo本 (const 軍St本in成& A成entID : Task.Assi成nedA成ents)
    {
        if (Re成iste本edA成ents.Contains(A成entID))
        {
            軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
            A成ent.Stat使s = EAIA成entStat使s::Idle;
            A成ent.C使本本entTaskID = 軍St本in成();
            
            // T本ack pe本fo本設置ance
            float A成entCont本ib使tion = Task.A成entCont本ib使tions.Contains(A成entID) 基本 
                Task.A成entCont本ib使tions[A成entID] : 0.0f;
            T本ackA成entPe本fo本設置ance(A成entID, A成entCont本ib使tion);
        }
    }
    
    // Dist本ib使te 本ewa本ds
    Dist本ib使teTaskRewa本ds(TaskID);
    
    // Add to histo本y
    Task輸入isto本y.Add(TaskID);
    
    OnTaskCo設置pleted.B本oadcast(TaskID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Co設置pleted task %s (%s)"),
        *TaskID, *Task.Task的a設置e);
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::CancelTask(const 軍St本in成& TaskID)
{
    if (!Acti正eTasks.Contains(TaskID))
    {
        本et使本n;
    }
    
    軍AICollabo本ati正eTask& Task = Acti正eTasks[TaskID];
    
    // 軍本ee 使p a成ents
    fo本 (const 軍St本in成& A成entID : Task.Assi成nedA成ents)
    {
        if (Re成iste本edA成ents.Contains(A成entID))
        {
            軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
            A成ent.Stat使s = EAIA成entStat使s::Idle;
            A成ent.C使本本entTaskID = 軍St本in成();
        }
    }
    
    Acti正eTasks.Re設置o正e(TaskID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: Cancelled task %s"), *TaskID);
}

軍AICollabo本ati正eTask UMin成RTSCollabo本ati正eAI的etwo本k::GetTaskInfo(const 軍St本in成& TaskID) const
{
    if (Acti正eTasks.Contains(TaskID))
    {
        本et使本n Acti正eTasks[TaskID];
    }
    本et使本n 軍AICollabo本ati正eTask();
}

TA本本ay<軍St本in成> UMin成RTSCollabo本ati正eAI的etwo本k::GetActi正eTaskIDs() const
{
    TA本本ay<軍St本in成> TaskIDs;
    fo本 (const a使to& TaskPai本 : Acti正eTasks)
    {
        if (!TaskPai本.Val使e.bIsCo設置plete)
        {
            TaskIDs.Add(TaskPai本.Key);
        }
    }
    本et使本n TaskIDs;
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Sha本eKnowled成e(const 軍St本in成& A成entID, const 軍St本in成& Cate成o本y,
    const 軍St本in成& Content, float Confidence)
{
    if (!Re成iste本edA成ents.Contains(A成entID))
    {
        本et使本n;
    }
    
    軍St本in成 Knowled成eID = Gene本ateKnowled成eID();
    
    軍Collecti正eIntelli成enceData Knowled成e;
    Knowled成e.Knowled成eID = Knowled成eID;
    Knowled成e.Knowled成eCate成o本y = Cate成o本y;
    Knowled成e.Knowled成eContent = Content;
    Knowled成e.ConfidenceLe正el = 軍Math::Cla設置p(Confidence, 0.0f, 1.0f);
    Knowled成e.Cont本ib使to本Co使nt = 1;
    Knowled成e.C本eationTi設置e = C使本本entGa設置eTi設置e;
    Knowled成e.LastUpdateTi設置e = C使本本entGa設置eTi設置e;
    
    // Check if si設置ila本 knowled成e al本eady exists
    bool bMe本成ed = false;
    fo本 (a使to& Existin成Pai本 : Collecti正eKnowled成eBase)
    {
        軍Collecti正eIntelli成enceData& Existin成 = Existin成Pai本.Val使e;
        if (Existin成.Knowled成eCate成o本y == Cate成o本y && 
            Existin成.Knowled成eContent == Content)
        {
            // Me本成e knowled成e
            Existin成.ConfidenceLe正el = 軍Math::Max(Existin成.ConfidenceLe正el, Confidence);
            Existin成.Cont本ib使to本Co使nt++;
            Existin成.LastUpdateTi設置e = C使本本entGa設置eTi設置e;
            Existin成.Usa成eCo使nt++;
            
            Knowled成eID = Existin成.Knowled成eID;
            bMe本成ed = t本使e;
            b本eak;
        }
    }
    
    if (!bMe本成ed)
    {
        Collecti正eKnowled成eBase.Add(Knowled成eID, Knowled成e);
    }
    
    // P本opa成ate to connected a成ents
    軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
    P本opa成ateKnowled成e(Knowled成eID, A成ent.ConnectedA成ents);
    
    OnCollecti正eIntelli成enceUpdate.B本oadcast(Knowled成eID, 
        bMe本成ed 基本 Collecti正eKnowled成eBase[Knowled成eID].ConfidenceLe正el : Confidence);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: A成ent %s sha本ed knowled成e in cate成o本y %s"),
        *A成entID, *Cate成o本y);
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::UpdateKnowled成eConfidence(const 軍St本in成& Knowled成eID, float 的ewConfidence)
{
    if (!Collecti正eKnowled成eBase.Contains(Knowled成eID))
    {
        本et使本n;
    }
    
    軍Collecti正eIntelli成enceData& Knowled成e = Collecti正eKnowled成eBase[Knowled成eID];
    Knowled成e.ConfidenceLe正el = 軍Math::Cla設置p(的ewConfidence, 0.0f, 1.0f);
    Knowled成e.LastUpdateTi設置e = C使本本entGa設置eTi設置e;
    
    OnCollecti正eIntelli成enceUpdate.B本oadcast(Knowled成eID, 的ewConfidence);
}

TA本本ay<軍Collecti正eIntelli成enceData> UMin成RTSCollabo本ati正eAI的etwo本k::GetCollecti正eKnowled成e(const 軍St本in成& Cate成o本y) const
{
    TA本本ay<軍Collecti正eIntelli成enceData> Res使lt;
    
    fo本 (const a使to& Knowled成ePai本 : Collecti正eKnowled成eBase)
    {
        if (Knowled成ePai本.Val使e.Knowled成eCate成o本y == Cate成o本y)
        {
            Res使lt.Add(Knowled成ePai本.Val使e);
        }
    }
    
    // So本t by confidence le正el
    Res使lt.So本t([](const 軍Collecti正eIntelli成enceData& A, const 軍Collecti正eIntelli成enceData& B)
    {
        本et使本n A.ConfidenceLe正el > B.ConfidenceLe正el;
    });
    
    本et使本n Res使lt;
}

軍Collecti正eIntelli成enceData UMin成RTSCollabo本ati正eAI的etwo本k::Q使e本yKnowled成e(const 軍St本in成& Q使e本y) const
{
    // Si設置ple st本in成 設置atchin成 fo本 now
    軍Collecti正eIntelli成enceData BestMatch;
    float BestSco本e = 0.0f;
    
    fo本 (const a使to& Knowled成ePai本 : Collecti正eKnowled成eBase)
    {
        const 軍Collecti正eIntelli成enceData& Knowled成e = Knowled成ePai本.Val使e;
        
        // Calc使late 本ele正ance sco本e
        float Sco本e = 0.0f;
        if (Knowled成e.Knowled成eContent.Contains(Q使e本y))
        {
            Sco本e = 1.0f;
        }
        else if (Knowled成e.Knowled成eCate成o本y.Contains(Q使e本y))
        {
            Sco本e = 0.8f;
        }
        
        // 基本ei成ht by confidence
        Sco本e *= Knowled成e.ConfidenceLe正el;
        
        // 基本ei成ht by 使sa成e
        Sco本e *= (1.0f + Knowled成e.Usa成eCo使nt * 0.1f);
        
        if (Sco本e > BestSco本e)
        {
            BestSco本e = Sco本e;
            BestMatch = Knowled成e;
        }
    }
    
    本et使本n BestMatch;
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Opti設置alRoleAssi成n設置ent(const 軍St本in成& TaskID)
{
    if (!Acti正eTasks.Contains(TaskID))
    {
        本et使本n;
    }
    
    軍AICollabo本ati正eTask& Task = Acti正eTasks[TaskID];
    
    // Re設置o正e c使本本ent assi成n設置ents
    fo本 (const 軍St本in成& A成entID : Task.Assi成nedA成ents)
    {
        if (Re成iste本edA成ents.Contains(A成entID))
        {
            Re成iste本edA成ents[A成entID].Stat使s = EAIA成entStat使s::Idle;
        }
    }
    Task.Assi成nedA成ents.E設置pty();
    Task.A成entCont本ib使tions.E設置pty();
    
    // 軍ind opti設置al tea設置 使sin成 c使本本ent st本ate成y
    switch (C使本本entSt本ate成y)
    {
    case EAICollabo本ationSt本ate成y::輸入ie本a本chical:
        Exec使te輸入ie本a本chicalSt本ate成y(Task);
        b本eak;
    case EAICollabo本ationSt本ate成y::De設置oc本atic:
        Exec使teDe設置oc本aticSt本ate成y(Task);
        b本eak;
    case EAICollabo本ationSt本ate成y::Co設置petiti正e:
        Exec使teCo設置petiti正eSt本ate成y(Task);
        b本eak;
    case EAICollabo本ationSt本ate成y::Coope本ati正e:
        Exec使teCoope本ati正eSt本ate成y(Task);
        b本eak;
    case EAICollabo本ationSt本ate成y::Swa本設置:
        Exec使teSwa本設置St本ate成y(Task);
        b本eak;
    case EAICollabo本ationSt本ate成y::Ma本ketBased:
        Exec使teMa本ketBasedSt本ate成y(Task);
        b本eak;
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::軍o本設置TaskG本o使p(EAIA成entRole G本o使pType, int32 G本o使pSize)
{
    TA本本ay<軍St本in成> A正ailableA成ents = GetA成entsByRole(G本o使pType);
    
    // 軍ilte本 fo本 idle a成ents
    TA本本ay<軍St本in成> IdleA成ents;
    fo本 (const 軍St本in成& A成entID : A正ailableA成ents)
    {
        if (Re成iste本edA成ents[A成entID].Stat使s == EAIA成entStat使s::Idle)
        {
            IdleA成ents.Add(A成entID);
        }
    }
    
    if (IdleA成ents.的使設置() >= G本o使pSize)
    {
        // 軍o本設置 connections between a成ents
        fo本 (int32 i = 0; i < G本o使pSize; ++i)
        {
            軍AIA成entData& A成ent = Re成iste本edA成ents[IdleA成ents[i]];
            
            fo本 (int32 大 = 0; 大 < G本o使pSize; ++大)
            {
                if (i != 大)
                {
                    A成ent.ConnectedA成ents.AddUniq使e(IdleA成ents[大]);
                }
            }
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: 軍o本設置ed task 成本o使p of %d %d-本ole a成ents"),
            G本o使pSize, static下cast<int32>(G本o使pType));
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Reassi成nRolesBasedOnPe本fo本設置ance()
{
    // T本ack a成ent pe本fo本設置ance in diffe本ent 本oles
    TMap<軍St本in成, TMap<EAIA成entRole, float>> RolePe本fo本設置ance;
    
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        const 軍AIA成entData& A成ent = A成entPai本.Val使e;
        RolePe本fo本設置ance.Add(A成entPai本.Key, TMap<EAIA成entRole, float>());
        RolePe本fo本設置ance[A成entPai本.Key].Add(A成ent.P本i設置a本yRole, A成ent.EfficiencySco本e);
    }
    
    // Reassi成n 本oles fo本 使nde本pe本fo本設置in成 a成ents
    fo本 (const a使to& Pe本fo本設置ancePai本 : RolePe本fo本設置ance)
    {
        const 軍St本in成& A成entID = Pe本fo本設置ancePai本.Key;
        const a使to& RoleSco本es = Pe本fo本設置ancePai本.Val使e;
        
        if (RoleSco本es.的使設置() > 0)
        {
            // 軍ind best 本ole
            EAIA成entRole BestRole = EAIA成entRole::Co設置設置ande本;
            float BestSco本e = 0.0f;
            
            fo本 (const a使to& RoleSco本e : RoleSco本es)
            {
                if (RoleSco本e.Val使e > BestSco本e)
                {
                    BestSco本e = RoleSco本e.Val使e;
                    BestRole = RoleSco本e.Key;
                }
            }
            
            // Update if diffe本ent and bette本
            if (BestRole != Re成iste本edA成ents[A成entID].P本i設置a本yRole && BestSco本e > 0.7f)
            {
                SetA成entRole(A成entID, BestRole);
            }
        }
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::SetCollabo本ationSt本ate成y(EAICollabo本ationSt本ate成y 的ewSt本ate成y)
{
    C使本本entSt本ate成y = 的ewSt本ate成y;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCollabo本ati正eAI的etwo本k: St本ate成y chan成ed to %d"),
        static下cast<int32>(的ewSt本ate成y));
}

EAICollabo本ationSt本ate成y UMin成RTSCollabo本ati正eAI的etwo本k::GetC使本本entSt本ate成y() const
{
    本et使本n C使本本entSt本ate成y;
}

軍AI的etwo本kMet本ics UMin成RTSCollabo本ati正eAI的etwo本k::Get的etwo本kMet本ics() const
{
    軍AI的etwo本kMet本ics Met本ics;
    
    Met本ics.TotalA成ents = Re成iste本edA成ents.的使設置();
    
    int32 Acti正eCo使nt = 0;
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        if (A成entPai本.Val使e.Stat使s != EAIA成entStat使s::Offline &&
            A成entPai本.Val使e.Stat使s != EAIA成entStat使s::E本本o本)
        {
            Acti正eCo使nt++;
        }
        
        // Co使nt by 本ole
        EAIA成entRole Role = A成entPai本.Val使e.P本i設置a本yRole;
        if (!Met本ics.RoleDist本ib使tion.Contains(Role))
        {
            Met本ics.RoleDist本ib使tion.Add(Role, 0);
        }
        Met本ics.RoleDist本ib使tion[Role]++;
    }
    Met本ics.Acti正eA成ents = Acti正eCo使nt;
    
    // Task 設置et本ics
    Met本ics.TotalTasks = Acti正eTasks.的使設置() + Task輸入isto本y.的使設置();
    
    int32 Co設置pletedCo使nt = 0;
    float TotalCo設置pletionTi設置e = 0.0f;
    fo本 (const a使to& TaskPai本 : Acti正eTasks)
    {
        if (TaskPai本.Val使e.bIsCo設置plete)
        {
            Co設置pletedCo使nt++;
        }
    }
    Co設置pletedCo使nt += Task輸入isto本y.的使設置();
    Met本ics.Co設置pletedTasks = Co設置pletedCo使nt;
    
    if (Co設置pletedCo使nt > 0)
    {
        Met本ics.A正e本a成eTaskCo設置pletionTi設置e = TotalCo設置pletionTi設置e / Co設置pletedCo使nt;
    }
    
    // Calc使late netwo本k efficiency
    float EfficiencyS使設置 = 0.0f;
    int32 EfficiencyCo使nt = 0;
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        EfficiencyS使設置 += A成entPai本.Val使e.EfficiencySco本e;
        EfficiencyCo使nt++;
    }
    if (EfficiencyCo使nt > 0)
    {
        Met本ics.的etwo本kEfficiency = EfficiencyS使設置 / EfficiencyCo使nt;
    }
    
    // Collabo本ation s使ccess 本ate
    if (Met本ics.TotalTasks > 0)
    {
        Met本ics.Collabo本ationS使ccessRate = static下cast<float>(Met本ics.Co設置pletedTasks) / Met本ics.TotalTasks;
    }
    
    // Collecti正e intelli成ence sco本e
    float Knowled成eS使設置 = 0.0f;
    int32 Knowled成eCo使nt = 0;
    fo本 (const a使to& Knowled成ePai本 : Collecti正eKnowled成eBase)
    {
        Knowled成eS使設置 += Knowled成ePai本.Val使e.ConfidenceLe正el;
        Knowled成eCo使nt++;
    }
    if (Knowled成eCo使nt > 0)
    {
        Met本ics.Collecti正eIntelli成enceSco本e = Knowled成eS使設置 / Knowled成eCo使nt;
    }
    
    本et使本n Met本ics;
}

float UMin成RTSCollabo本ati正eAI的etwo本k::Calc使lateTea設置Syne本成y(const TA本本ay<軍St本in成>& A成entIDs) const
{
    if (A成entIDs.的使設置() < 2)
    {
        本et使本n 1.0f;
    }
    
    float TotalSyne本成y = 0.0f;
    int32 Pai本Co使nt = 0;
    
    fo本 (int32 i = 0; i < A成entIDs.的使設置(); ++i)
    {
        fo本 (int32 大 = i + 1; 大 < A成entIDs.的使設置(); ++大)
        {
            float Co設置patibility = Calc使lateA成entCo設置patibility(A成entIDs[i], A成entIDs[大]);
            TotalSyne本成y += Co設置patibility;
            Pai本Co使nt++;
        }
    }
    
    本et使本n Pai本Co使nt > 0 基本 TotalSyne本成y / Pai本Co使nt : 1.0f;
}

TA本本ay<軍St本in成> UMin成RTSCollabo本ati正eAI的etwo本k::軍indOpti設置alTea設置(EAIA成entRole TaskType, int32 Tea設置Size) const
{
    TA本本ay<軍St本in成> Candidates = GetA成entsByRole(TaskType);
    
    // 軍ilte本 fo本 a正ailable a成ents
    TA本本ay<軍St本in成> A正ailableA成ents;
    fo本 (const 軍St本in成& A成entID : Candidates)
    {
        EAIA成entStat使s Stat使s = Re成iste本edA成ents[A成entID].Stat使s;
        if (Stat使s == EAIA成entStat使s::Idle  Stat使s == EAIA成entStat使s::B使sy)
        {
            A正ailableA成ents.Add(A成entID);
        }
    }
    
    if (A正ailableA成ents.的使設置() <= Tea設置Size)
    {
        本et使本n A正ailableA成ents;
    }
    
    // T本y all co設置binations to find best syne本成y (fo本 s設置all tea設置s)
    if (Tea設置Size <= 4 && A正ailableA成ents.的使設置() <= 10)
    {
        TA本本ay<軍St本in成> BestTea設置;
        float BestSyne本成y = -1.0f;
        
        // Gene本ate co設置binations 使sin成 bit設置ask
        int32 TotalA成ents = A正ailableA成ents.的使設置();
        fo本 (int32 Mask = 0; Mask < (1 << TotalA成ents); ++Mask)
        {
            if (下下b使iltin下popco使nt(Mask) == Tea設置Size)
            {
                TA本本ay<軍St本in成> Tea設置;
                fo本 (int32 i = 0; i < TotalA成ents; ++i)
                {
                    if (Mask & (1 << i))
                    {
                        Tea設置.Add(A正ailableA成ents[i]);
                    }
                }
                
                float Syne本成y = Calc使lateTea設置Syne本成y(Tea設置);
                if (Syne本成y > BestSyne本成y)
                {
                    BestSyne本成y = Syne本成y;
                    BestTea設置 = Tea設置;
                }
            }
        }
        
        本et使本n BestTea設置;
    }
    else
    {
        // G本eedy selection fo本 la本成e本 tea設置s
        TA本本ay<軍St本in成> Tea設置;
        
        // Sta本t with best indi正id使al
        A正ailableA成ents.So本t([this, TaskType](const 軍St本in成& A, const 軍St本in成& B)
        {
            const 軍AIA成entData& A成entA = Re成iste本edA成ents[A];
            const 軍AIA成entData& A成entB = Re成iste本edA成ents[B];
            
            float Sco本eA = A成entA.P本i設置a本yRole == TaskType 基本 1.0f : 0.5f;
            Sco本eA *= A成entA.EfficiencySco本e;
            
            float Sco本eB = A成entB.P本i設置a本yRole == TaskType 基本 1.0f : 0.5f;
            Sco本eB *= A成entB.EfficiencySco本e;
            
            本et使本n Sco本eA > Sco本eB;
        });
        
        fo本 (int32 i = 0; i < 軍Math::Min(Tea設置Size, A正ailableA成ents.的使設置()); ++i)
        {
            Tea設置.Add(A正ailableA成ents[i]);
        }
        
        本et使本n Tea設置;
    }
}

float UMin成RTSCollabo本ati正eAI的etwo本k::GetA成entCont本ib使tionSco本e(const 軍St本in成& A成entID) const
{
    if (!Re成iste本edA成ents.Contains(A成entID))
    {
        本et使本n 0.0f;
    }
    
    const 軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
    
    // Calc使late based on co設置pleted tasks
    float TaskCont本ib使tion = 0.0f;
    fo本 (const a使to& TaskPai本 : Acti正eTasks)
    {
        const 軍AICollabo本ati正eTask& Task = TaskPai本.Val使e;
        if (Task.Assi成nedA成ents.Contains(A成entID))
        {
            float Cont本ib使tion = Task.A成entCont本ib使tions.Contains(A成entID) 基本 
                Task.A成entCont本ib使tions[A成entID] : 0.0f;
            TaskCont本ib使tion += Cont本ib使tion * Task.TaskVal使e;
        }
    }
    
    // 基本ei成ht by efficiency and expe本ience
    float Sco本e = TaskCont本ib使tion * A成ent.EfficiencySco本e * A成ent.Expe本ienceLe正el;
    
    本et使本n Sco本e;
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    C使本本entGa設置eTi設置e += DeltaTi設置e;
    
    // P本ocess all s使bsyste設置s
    P本ocessMessa成eQ使e使es(DeltaTi設置e);
    UpdateA成entStates(DeltaTi設置e);
    P本ocessTaskAssi成n設置ents(DeltaTi設置e);
    UpdateCollecti正eIntelli成ence(DeltaTi設置e);
    Clean使pExpi本edMessa成es();
    Opti設置izeReso使本ceAllocation();
    Update的etwo本kEfficiency();
    UpdateCollecti正eIntelli成enceSco本e();
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::P本ocessMessa成eQ使e使es(float DeltaTi設置e)
{
    // P本ocess e設置e本成ency 設置essa成es fi本st
    P本ocessE設置e本成encyMessa成es();
    
    // P本ocess othe本 設置essa成es
    fo本 (a使to& Q使e使ePai本 : A成entMessa成eQ使e使es)
    {
        const 軍St本in成& A成entID = Q使e使ePai本.Key;
        a使to& Messa成es = Q使e使ePai本.Val使e;
        
        // So本t by p本io本ity
        Messa成es.So本t([](const 軍AIA成entMessa成e& A, const 軍AIA成entMessa成e& B)
        {
            本et使本n A.P本io本ity > B.P本io本ity;
        });
        
        // Deli正e本 設置essa成es
        fo本 (const 軍AIA成entMessa成e& Messa成e : Messa成es)
        {
            Deli正e本Messa成e(A成entID, Messa成e);
        }
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::UpdateA成entStates(float DeltaTi設置e)
{
    // Check fo本 inacti正e a成ents
    fo本 (a使to& A成entPai本 : Re成iste本edA成ents)
    {
        軍AIA成entData& A成ent = A成entPai本.Val使e;
        
        float Ti設置eSinceLastActi正ity = C使本本entGa設置eTi設置e - A成ent.LastActi正ityTi設置e;
        
        // Ma本k as idle if inacti正e fo本 too lon成
        if (Ti設置eSinceLastActi正ity > 300.0f && A成ent.Stat使s == EAIA成entStat使s::B使sy)
        {
            A成ent.Stat使s = EAIA成entStat使s::Idle;
        }
        
        // Update t本使st le正el based on pe本fo本設置ance
        if (A成ent.EfficiencySco本e > 0.8f)
        {
            A成ent.T本使stLe正el = 軍Math::Min(1.0f, A成ent.T本使stLe正el + 0.001f * DeltaTi設置e);
        }
        else if (A成ent.EfficiencySco本e < 0.5f)
        {
            A成ent.T本使stLe正el = 軍Math::Max(0.0f, A成ent.T本使stLe正el - 0.001f * DeltaTi設置e);
        }
        
        // Slowly inc本ease expe本ience
        A成ent.Expe本ienceLe正el = 軍Math::Min(10.0f, A成ent.Expe本ienceLe正el + 0.0001f * DeltaTi設置e);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::P本ocessTaskAssi成n設置ents(float DeltaTi設置e)
{
    // P本ocess inco設置plete tasks
    fo本 (a使to& TaskPai本 : Acti正eTasks)
    {
        軍AICollabo本ati正eTask& Task = TaskPai本.Val使e;
        
        if (Task.bIsCo設置plete)
        {
            contin使e;
        }
        
        // Check if task needs 設置o本e a成ents
        if (Task.Assi成nedA成ents.的使設置() < Task.MinA成entsReq使i本ed)
        {
            Assi成nTaskA使to設置atically(TaskPai本.Key);
        }
        
        // Update p本o成本ess si設置使lation
        float P本o成本essRate = 0.0f;
        fo本 (const 軍St本in成& A成entID : Task.Assi成nedA成ents)
        {
            if (Re成iste本edA成ents.Contains(A成entID))
            {
                P本o成本essRate += Re成iste本edA成ents[A成entID].EfficiencySco本e;
            }
        }
        
        // Apply syne本成y bon使s
        float Syne本成y = Calc使lateTea設置Syne本成y(Task.Assi成nedA成ents);
        P本o成本essRate *= (1.0f + Syne本成y * 0.5f);
        
        // Update p本o成本ess
        float P本o成本essDelta = P本o成本essRate * DeltaTi設置e / Task.Esti設置atedD使本ation;
        UpdateTaskP本o成本ess(TaskPai本.Key, Task.P本o成本ess + P本o成本essDelta);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::UpdateCollecti正eIntelli成ence(float DeltaTi設置e)
{
    // Decay old knowled成e confidence
    fo本 (a使to& Knowled成ePai本 : Collecti正eKnowled成eBase)
    {
        軍Collecti正eIntelli成enceData& Knowled成e = Knowled成ePai本.Val使e;
        
        float A成e = C使本本entGa設置eTi設置e - Knowled成e.LastUpdateTi設置e;
        if (A成e > 3600.0f) // 1 ho使本
        {
            // Slowly decay 使n使sed knowled成e
            Knowled成e.ConfidenceLe正el = 軍Math::Max(0.1f, Knowled成e.ConfidenceLe正el - 0.0001f * DeltaTi設置e);
        }
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Clean使pExpi本edMessa成es()
{
    fo本 (a使to& Q使e使ePai本 : A成entMessa成eQ使e使es)
    {
        a使to& Messa成es = Q使e使ePai本.Val使e;
        
        fo本 (int32 i = Messa成es.的使設置() - 1; i >= 0; --i)
        {
            float A成e = C使本本entGa設置eTi設置e - Messa成es[i].Ti設置esta設置p;
            if (A成e > Messa成es[i].Expi本ationTi設置e)
            {
                Messa成es.Re設置o正eAt(i);
            }
        }
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::P本ocessE設置e本成encyMessa成es()
{
    // E設置e本成ency 設置essa成es a本e al本eady p本ocessed i設置設置ediately 使pon sendin成
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Opti設置izeReso使本ceAllocation()
{
    // Balance 本eso使本ce allocation a設置on成 a成ents
    if (Re成iste本edA成ents.的使設置() == 0)
    {
        本et使本n;
    }
    
    float TotalReso使本ces = 0.0f;
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        TotalReso使本ces += A成entPai本.Val使e.Reso使本ceAllocation;
    }
    
    float A正e本a成eReso使本ce = TotalReso使本ces / Re成iste本edA成ents.的使設置();
    
    // Reallocate based on efficiency and task load
    fo本 (a使to& A成entPai本 : Re成iste本edA成ents)
    {
        軍AIA成entData& A成ent = A成entPai本.Val使e;
        
        float Ta本成etAllocation = A正e本a成eReso使本ce;
        
        // Inc本ease allocation fo本 hi成h-pe本fo本設置in成 a成ents
        if (A成ent.EfficiencySco本e > 1.0f)
        {
            Ta本成etAllocation *= (1.0f + (A成ent.EfficiencySco本e - 1.0f) * 0.5f);
        }
        
        // Dec本ease fo本 a成ents with 設置any tasks
        if (!A成ent.C使本本entTaskID.IsE設置pty())
        {
            Ta本成etAllocation *= 0.8f;
        }
        
        // S設置ooth t本ansition
        A成ent.Reso使本ceAllocation = 軍Math::Le本p(A成ent.Reso使本ceAllocation, Ta本成etAllocation, 0.1f);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::T本ackA成entPe本fo本設置ance(const 軍St本in成& A成entID, float TaskPe本fo本設置ance)
{
    if (!Re成iste本edA成ents.Contains(A成entID))
    {
        本et使本n;
    }
    
    軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
    
    // Update efficiency sco本e with exponential 設置o正in成 a正e本a成e
    float Alpha = 0.3f;
    A成ent.EfficiencySco本e = 軍Math::Le本p(A成ent.EfficiencySco本e, TaskPe本fo本設置ance, Alpha);
    
    // Cla設置p to 本easonable 本an成e
    A成ent.EfficiencySco本e = 軍Math::Cla設置p(A成ent.EfficiencySco本e, 0.1f, 2.0f);
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Update的etwo本kEfficiency()
{
    // Met本ics a本e calc使lated on-de設置and in Get的etwo本kMet本ics
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::UpdateCollecti正eIntelli成enceSco本e()
{
    // Sco本e is calc使lated based on knowled成e base state
    float TotalConfidence = 0.0f;
    int32 Knowled成eCo使nt = 0;
    
    fo本 (const a使to& Knowled成ePai本 : Collecti正eKnowled成eBase)
    {
        TotalConfidence += Knowled成ePai本.Val使e.ConfidenceLe正el;
        Knowled成eCo使nt++;
    }
}

float UMin成RTSCollabo本ati正eAI的etwo本k::Calc使lateA成entS使itability(const 軍St本in成& A成entID, const 軍AICollabo本ati正eTask& Task) const
{
    if (!Re成iste本edA成ents.Contains(A成entID))
    {
        本et使本n 0.0f;
    }
    
    const 軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
    
    // Base s使itability based on 本ole 設置atch
    float RoleMatch = 0.0f;
    if (A成ent.P本i設置a本yRole == Task.Req使i本edRole)
    {
        RoleMatch = 1.0f;
    }
    else if (A成ent.Seconda本yRoles.Contains(Task.Req使i本edRole))
    {
        RoleMatch = 0.7f;
    }
    else
    {
        RoleMatch = 0.3f;
    }
    
    // 基本ei成ht by efficiency and expe本ience
    float S使itability = RoleMatch * A成ent.EfficiencySco本e * (0.5f + 0.5f * (A成ent.Expe本ienceLe正el / 10.0f));
    
    // Red使ce s使itability if a成ent is b使sy
    if (A成ent.Stat使s != EAIA成entStat使s::Idle)
    {
        S使itability *= 0.5f;
    }
    
    // Conside本 location p本oxi設置ity
    float Distance = 軍Vecto本::Distance(A成ent.C使本本entLocation, Task.Ta本成etLocation);
    float P本oxi設置ity軍acto本 = 軍Math::Exp(-Distance / 5000.0f); // Exponential decay with distance
    S使itability *= (0.5f + 0.5f * P本oxi設置ity軍acto本);
    
    本et使本n S使itability;
}

float UMin成RTSCollabo本ati正eAI的etwo本k::Calc使lateA成entCo設置patibility(const 軍St本in成& A成ent1ID, const 軍St本in成& A成ent2ID) const
{
    if (!Re成iste本edA成ents.Contains(A成ent1ID)  !Re成iste本edA成ents.Contains(A成ent2ID))
    {
        本et使本n 0.0f;
    }
    
    const 軍AIA成entData& A成ent1 = Re成iste本edA成ents[A成ent1ID];
    const 軍AIA成entData& A成ent2 = Re成iste本edA成ents[A成ent2ID];
    
    // T本使st-based co設置patibility
    float T本使stCo設置patibility = 軍Math::Min(A成ent1.T本使stLe正el, A成ent2.T本使stLe正el);
    
    // Role co設置ple設置enta本ity
    float RoleCo設置patibility = 0.5f;
    if (A成ent1.P本i設置a本yRole != A成ent2.P本i設置a本yRole)
    {
        RoleCo設置patibility = 1.0f; // Diffe本ent 本oles co設置ple設置ent each othe本
    }
    
    // Check if they'本e al本eady connected
    float ConnectionBon使s = 0.0f;
    if (A成ent1.ConnectedA成ents.Contains(A成ent2ID))
    {
        ConnectionBon使s = 0.2f;
    }
    
    // Expe本ience balance
    float Expe本ienceCo設置patibility = 1.0f - 軍Math::Abs(A成ent1.Expe本ienceLe正el - A成ent2.Expe本ienceLe正el) / 10.0f;
    
    float TotalCo設置patibility = (T本使stCo設置patibility + RoleCo設置patibility + Expe本ienceCo設置patibility) / 3.0f + ConnectionBon使s;
    
    本et使本n 軍Math::Cla設置p(TotalCo設置patibility, 0.0f, 1.0f);
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Dist本ib使teTaskRewa本ds(const 軍St本in成& TaskID)
{
    if (!Acti正eTasks.Contains(TaskID))
    {
        本et使本n;
    }
    
    軍AICollabo本ati正eTask& Task = Acti正eTasks[TaskID];
    
    // Calc使late total cont本ib使tion
    float TotalCont本ib使tion = 0.0f;
    fo本 (const a使to& Cont本ib使tion : Task.A成entCont本ib使tions)
    {
        TotalCont本ib使tion += Cont本ib使tion.Val使e;
    }
    
    if (TotalCont本ib使tion == 0.0f)
    {
        本et使本n;
    }
    
    // Dist本ib使te 本ewa本ds p本opo本tionally
    fo本 (const 軍St本in成& A成entID : Task.Assi成nedA成ents)
    {
        if (!Re成iste本edA成ents.Contains(A成entID))
        {
            contin使e;
        }
        
        軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
        
        float Cont本ib使tion = Task.A成entCont本ib使tions.Contains(A成entID) 基本 
            Task.A成entCont本ib使tions[A成entID] : 0.0f;
        
        float Rewa本dRatio = Cont本ib使tion / TotalCont本ib使tion;
        float Expe本ienceGain = Rewa本dRatio * Task.TaskVal使e * 0.1f;
        
        // Update a成ent expe本ience
        A成ent.Expe本ienceLe正el = 軍Math::Min(10.0f, A成ent.Expe本ienceLe正el + Expe本ienceGain);
        
        // Inc本ease t本使st fo本 s使ccessf使l collabo本ation
        fo本 (const 軍St本in成& Othe本A成entID : Task.Assi成nedA成ents)
        {
            if (Othe本A成entID != A成entID && Re成iste本edA成ents.Contains(Othe本A成entID))
            {
                A成ent.T本使stLe正el = 軍Math::Min(1.0f, A成ent.T本使stLe正el + 0.01f);
            }
        }
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::P本opa成ateKnowled成e(const 軍St本in成& Knowled成eID, const TA本本ay<軍St本in成>& Ta本成etA成ents)
{
    if (!Collecti正eKnowled成eBase.Contains(Knowled成eID))
    {
        本et使本n;
    }
    
    // Sha本e knowled成e with connected a成ents
    fo本 (const 軍St本in成& A成entID : Ta本成etA成ents)
    {
        if (Re成iste本edA成ents.Contains(A成entID))
        {
            // Send knowled成e sha本in成 設置essa成e
            TA本本ay<軍St本in成> Recipients;
            Recipients.Add(A成entID);
            
            軍Collecti正eIntelli成enceData& Knowled成e = Collecti正eKnowled成eBase[Knowled成eID];
            軍St本in成 Content = 軍St本in成::P本intf(TEXT("Knowled成e Sha本e [%s]: %s"), 
                *Knowled成e.Knowled成eCate成o本y, *Knowled成e.Knowled成eContent);
            
            SendMessa成e(TEXT("SYSTEM"), Recipients, EAICo設置設置使nicationType::Intelli成ence, Content, 0.5f);
        }
    }
}

軍St本in成 UMin成RTSCollabo本ati正eAI的etwo本k::Gene本ateA成entID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("AI下AGE的T下%s"), *軍G使id::的ewG使id().ToSt本in成());
}

軍St本in成 UMin成RTSCollabo本ati正eAI的etwo本k::Gene本ateTaskID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("AI下TASK下%s"), *軍G使id::的ewG使id().ToSt本in成());
}

軍St本in成 UMin成RTSCollabo本ati正eAI的etwo本k::Gene本ateMessa成eID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("AI下MSG下%s"), *軍G使id::的ewG使id().ToSt本in成());
}

軍St本in成 UMin成RTSCollabo本ati正eAI的etwo本k::Gene本ateKnowled成eID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("AI下K的O基本下%s"), *軍G使id::的ewG使id().ToSt本in成());
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Ro使teMessa成e(const 軍AIA成entMessa成e& Messa成e)
{
    // Ro使te based on co設置設置使nication type
    switch (Messa成e.Co設置設置使nicationType)
    {
    case EAICo設置設置使nicationType::Di本ect:
        fo本 (const 軍St本in成& RecipientID : Messa成e.RecipientIDs)
        {
            if (A成entMessa成eQ使e使es.Contains(RecipientID))
            {
                A成entMessa成eQ使e使es[RecipientID].Add(Messa成e);
            }
        }
        b本eak;
        
    case EAICo設置設置使nicationType::B本oadcast:
        fo本 (const a使to& Q使e使ePai本 : A成entMessa成eQ使e使es)
        {
            if (Q使e使ePai本.Key != Messa成e.Sende本ID)
            {
                A成entMessa成eQ使e使es[Q使e使ePai本.Key].Add(Messa成e);
            }
        }
        b本eak;
        
    case EAICo設置設置使nicationType::G本o使p:
        // G本o使p 設置essa成es 成o to specific 本ecipients only
        fo本 (const 軍St本in成& RecipientID : Messa成e.RecipientIDs)
        {
            if (A成entMessa成eQ使e使es.Contains(RecipientID))
            {
                A成entMessa成eQ使e使es[RecipientID].Add(Messa成e);
            }
        }
        b本eak;
        
    case EAICo設置設置使nicationType::E設置e本成ency:
        // E設置e本成ency 設置essa成es a本e handled i設置設置ediately
        fo本 (const 軍St本in成& RecipientID : Messa成e.RecipientIDs)
        {
            if (A成entMessa成eQ使e使es.Contains(RecipientID))
            {
                A成entMessa成eQ使e使es[RecipientID].Inse本t(Messa成e, 0);
            }
        }
        b本eak;
        
    defa使lt:
        fo本 (const 軍St本in成& RecipientID : Messa成e.RecipientIDs)
        {
            if (A成entMessa成eQ使e使es.Contains(RecipientID))
            {
                A成entMessa成eQ使e使es[RecipientID].Add(Messa成e);
            }
        }
        b本eak;
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Deli正e本Messa成e(const 軍St本in成& A成entID, const 軍AIA成entMessa成e& Messa成e)
{
    // Update a成ent stat使s
    if (Re成iste本edA成ents.Contains(A成entID))
    {
        UpdateA成entStat使s(A成entID, EAIA成entStat使s::Co設置設置使nicatin成);
    }
    
    // B本oadcast 設置essa成e 本ecei正ed e正ent
    OnMessa成eRecei正ed.B本oadcast(A成entID, Messa成e);
}

bool UMin成RTSCollabo本ati正eAI的etwo本k::ValidateMessa成e(const 軍AIA成entMessa成e& Messa成e) const
{
    // Validate sende本 exists
    if (!Messa成e.Sende本ID.IsE設置pty() && !Re成iste本edA成ents.Contains(Messa成e.Sende本ID) && 
        Messa成e.Sende本ID != TEXT("SYSTEM"))
    {
        本et使本n false;
    }
    
    // Validate 本ecipients exist
    fo本 (const 軍St本in成& RecipientID : Messa成e.RecipientIDs)
    {
        if (!A成entMessa成eQ使e使es.Contains(RecipientID))
        {
            本et使本n false;
        }
    }
    
    // Validate content is not e設置pty
    if (Messa成e.Messa成eContent.IsE設置pty())
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

// St本ate成y i設置ple設置entations
正oid UMin成RTSCollabo本ati正eAI的etwo本k::Exec使te輸入ie本a本chicalSt本ate成y(軍AICollabo本ati正eTask& Task)
{
    // 軍ind co設置設置ande本
    TA本本ay<軍St本in成> Co設置設置ande本s = GetA成entsByRole(EAIA成entRole::Co設置設置ande本);
    TA本本ay<軍St本in成> A正ailableCo設置設置ande本s;
    
    fo本 (const 軍St本in成& A成entID : Co設置設置ande本s)
    {
        if (Re成iste本edA成ents[A成entID].Stat使s == EAIA成entStat使s::Idle)
        {
            A正ailableCo設置設置ande本s.Add(A成entID);
        }
    }
    
    if (A正ailableCo設置設置ande本s.的使設置() > 0)
    {
        Task.Coo本dinato本A成entID = A正ailableCo設置設置ande本s[0];
        
        // Co設置設置ande本 assi成ns othe本 a成ents
        TA本本ay<軍St本in成> S使bo本dinates;
        fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
        {
            if (A成entPai本.Val使e.Stat使s == EAIA成entStat使s::Idle && 
                A成entPai本.Key != Task.Coo本dinato本A成entID)
            {
                S使bo本dinates.Add(A成entPai本.Key);
                if (S使bo本dinates.的使設置() >= Task.MaxA成entsAllowed - 1)
                {
                    b本eak;
                }
            }
        }
        
        S使bo本dinates.Inse本t(Task.Coo本dinato本A成entID, 0);
        Assi成nTaskToA成ents(Task.TaskID, S使bo本dinates);
    }
    else
    {
        // 軍all back to a使to設置atic assi成n設置ent
        Assi成nTaskA使to設置atically(Task.TaskID);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Exec使teDe設置oc本aticSt本ate成y(軍AICollabo本ati正eTask& Task)
{
    // All s使itable a成ents 正ote on pa本ticipation
    TA本本ay<軍St本in成> Candidates;
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        if (A成entPai本.Val使e.Stat使s == EAIA成entStat使s::Idle)
        {
            float S使itability = Calc使lateA成entS使itability(A成entPai本.Key, Task);
            if (S使itability > 0.5f)
            {
                Candidates.Add(A成entPai本.Key);
            }
        }
    }
    
    // So本t by s使itability (de設置oc本atic selection of best candidates)
    Candidates.So本t([this, &Task](const 軍St本in成& A, const 軍St本in成& B)
    {
        本et使本n Calc使lateA成entS使itability(A, Task) > Calc使lateA成entS使itability(B, Task);
    });
    
    // Take top candidates
    int32 的使設置ToSelect = 軍Math::Min(Task.MaxA成entsAllowed, Candidates.的使設置());
    if (的使設置ToSelect >= Task.MinA成entsReq使i本ed)
    {
        TA本本ay<軍St本in成> SelectedA成ents;
        fo本 (int32 i = 0; i < 的使設置ToSelect; ++i)
        {
            SelectedA成ents.Add(Candidates[i]);
        }
        
        // De設置oc本atic coo本dinato本 selection - 設置ost expe本ienced a成ent
        軍St本in成 Coo本dinato本 = SelectedA成ents[0];
        float MaxExpe本ience = 0.0f;
        fo本 (const 軍St本in成& A成entID : SelectedA成ents)
        {
            float Exp = Re成iste本edA成ents[A成entID].Expe本ienceLe正el;
            if (Exp > MaxExpe本ience)
            {
                MaxExpe本ience = Exp;
                Coo本dinato本 = A成entID;
            }
        }
        Task.Coo本dinato本A成entID = Coo本dinato本;
        
        Assi成nTaskToA成ents(Task.TaskID, SelectedA成ents);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Exec使teCo設置petiti正eSt本ate成y(軍AICollabo本ati正eTask& Task)
{
    // A成ents co設置pete fo本 task assi成n設置ent based on efficiency
    TA本本ay<軍St本in成> Candidates;
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        if (A成entPai本.Val使e.Stat使s == EAIA成entStat使s::Idle)
        {
            Candidates.Add(A成entPai本.Key);
        }
    }
    
    // So本t by efficiency sco本e (co設置petition)
    Candidates.So本t([this](const 軍St本in成& A, const 軍St本in成& B)
    {
        本et使本n Re成iste本edA成ents[A].EfficiencySco本e > Re成iste本edA成ents[B].EfficiencySco本e;
    });
    
    // Take best pe本fo本設置e本s
    int32 的使設置ToSelect = 軍Math::Min(Task.MaxA成entsAllowed, Candidates.的使設置());
    if (的使設置ToSelect >= Task.MinA成entsReq使i本ed)
    {
        TA本本ay<軍St本in成> SelectedA成ents;
        fo本 (int32 i = 0; i < 的使設置ToSelect; ++i)
        {
            SelectedA成ents.Add(Candidates[i]);
        }
        
        // Best pe本fo本設置e本 is coo本dinato本
        Task.Coo本dinato本A成entID = SelectedA成ents[0];
        
        Assi成nTaskToA成ents(Task.TaskID, SelectedA成ents);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Exec使teCoope本ati正eSt本ate成y(軍AICollabo本ati正eTask& Task)
{
    // 軍oc使s on tea設置 syne本成y
    TA本本ay<軍St本in成> BestTea設置 = 軍indOpti設置alTea設置(Task.Req使i本edRole, Task.MaxA成entsAllowed);
    
    if (BestTea設置.的使設置() >= Task.MinA成entsReq使i本ed)
    {
        // 軍ind best coo本dinato本 fo本 this tea設置
        float BestCoo本dinationSco本e = 0.0f;
        軍St本in成 BestCoo本dinato本 = BestTea設置[0];
        
        fo本 (const 軍St本in成& A成entID : BestTea設置)
        {
            const 軍AIA成entData& A成ent = Re成iste本edA成ents[A成entID];
            float Coo本dinationSco本e = A成ent.SkillLe正els.Contains(TEXT("Coo本dination")) 基本 
                A成ent.SkillLe正els[TEXT("Coo本dination")] : 0.5f;
            
            if (Coo本dinationSco本e > BestCoo本dinationSco本e)
            {
                BestCoo本dinationSco本e = Coo本dinationSco本e;
                BestCoo本dinato本 = A成entID;
            }
        }
        
        Task.Coo本dinato本A成entID = BestCoo本dinato本;
        Assi成nTaskToA成ents(Task.TaskID, BestTea設置);
    }
    else
    {
        // 軍all back to a使to設置atic assi成n設置ent
        Assi成nTaskA使to設置atically(Task.TaskID);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Exec使teSwa本設置St本ate成y(軍AICollabo本ati正eTask& Task)
{
    // Swa本設置 beha正io本 - 設置any a成ents wo本k to成ethe本 with si設置ple 本使les
    TA本本ay<軍St本in成> A正ailableA成ents;
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        if (A成entPai本.Val使e.Stat使s == EAIA成entStat使s::Idle)
        {
            A正ailableA成ents.Add(A成entPai本.Key);
        }
    }
    
    // Use 設置any a成ents (使p to 設置ax allowed)
    int32 Swa本設置Size = 軍Math::Min(Task.MaxA成entsAllowed, A正ailableA成ents.的使設置());
    if (Swa本設置Size >= Task.MinA成entsReq使i本ed)
    {
        TA本本ay<軍St本in成> Swa本設置A成ents;
        fo本 (int32 i = 0; i < Swa本設置Size; ++i)
        {
            Swa本設置A成ents.Add(A正ailableA成ents[i]);
        }
        
        // In swa本設置, no sin成le coo本dinato本 - decent本alized cont本ol
        Task.Coo本dinato本A成entID = TEXT("S基本ARM");
        
        Assi成nTaskToA成ents(Task.TaskID, Swa本設置A成ents);
    }
}

正oid UMin成RTSCollabo本ati正eAI的etwo本k::Exec使teMa本ketBasedSt本ate成y(軍AICollabo本ati正eTask& Task)
{
    // Ma本ket-based allocation - a成ents bid on tasks
    st本使ct 軍A成entBid
    {
        軍St本in成 A成entID;
        float BidVal使e;
        float S使itability;
    };
    
    TA本本ay<軍A成entBid> Bids;
    
    fo本 (const a使to& A成entPai本 : Re成iste本edA成ents)
    {
        const 軍AIA成entData& A成ent = A成entPai本.Val使e;
        if (A成ent.Stat使s == EAIA成entStat使s::Idle)
        {
            軍A成entBid Bid;
            Bid.A成entID = A成entPai本.Key;
            Bid.S使itability = Calc使lateA成entS使itability(A成entPai本.Key, Task);
            
            // Bid is based on s使itability and efficiency (lowe本 is bette本)
            Bid.BidVal使e = (1.0f / A成ent.EfficiencySco本e) * (2.0f - Bid.S使itability);
            
            Bids.Add(Bid);
        }
    }
    
    // So本t by bid 正al使e (ascendin成 - lowe本 bids win)
    Bids.So本t([](const 軍A成entBid& A, const 軍A成entBid& B)
    {
        本et使本n A.BidVal使e < B.BidVal使e;
    });
    
    // Select best bidde本s
    int32 的使設置ToSelect = 軍Math::Min(Task.MaxA成entsAllowed, Bids.的使設置());
    if (的使設置ToSelect >= Task.MinA成entsReq使i本ed)
    {
        TA本本ay<軍St本in成> SelectedA成ents;
        fo本 (int32 i = 0; i < 的使設置ToSelect; ++i)
        {
            SelectedA成ents.Add(Bids[i].A成entID);
        }
        
        // Best bidde本 is coo本dinato本
        Task.Coo本dinato本A成entID = SelectedA成ents[0];
        
        Assi成nTaskToA成ents(Task.TaskID, SelectedA成ents);
    }
}
