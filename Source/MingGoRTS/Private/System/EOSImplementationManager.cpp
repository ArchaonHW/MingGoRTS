#incl使de "EOSI設置ple設置entationMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UEOSI設置ple設置entationMana成e本::UEOSI設置ple設置entationMana成e本()
    : C使本本entPhase(EOSI設置ple設置entationPhase::Plannin成)
    , O正e本allP本o成本ess(0.0f)
{
}

正oid UEOSI設置ple設置entationMana成e本::InitializeI設置ple設置entationMana成e本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 EOS I設置ple設置entation Mana成e本"));
    
    // 初始化階段信息
    InitializePhaseInfos();
    
    // 記錄實施開始時間
    I設置ple設置entationSta本tTi設置e = 軍DateTi設置e::的ow();
    
    // 計算初始進度
    UpdateO正e本allP本o成本ess();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("EOS I設置ple設置entation Mana成e本 initialized s使ccessf使lly"));
}

正oid UEOSI設置ple設置entationMana成e本::InitializePhaseInfos()
{
    PhaseInfos.E設置pty();
    
    // 階段一：基礎內核開發 (4週)
    軍EOSPhaseInfo Ke本nelPhase;
    Ke本nelPhase.Phase = EOSI設置ple設置entationPhase::Ke本nel;
    Ke本nelPhase.Phase的a設置e = TEXT("基礎內核開發");
    Ke本nelPhase.Desc本iption = TEXT("開發作業系統核心組件，包括內核、進程管理、內存管理和文件系統");
    Ke本nelPhase.D使本ation基本eeks = 4;
    Ke本nelPhase.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Ke本nelPhase.O正e本allP本o成本ess = 0.0f;
    
    // 內核開發任務
    軍EOSPhaseTask Ke本nelTask1;
    Ke本nelTask1.Task的a設置e = TEXT("內核框架設計");
    Ke本nelTask1.Desc本iption = TEXT("設計作業系統內核架構和核心接口");
    Ke本nelTask1.Esti設置atedDays = 5;
    Ke本nelTask1.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Ke本nelTask1.Assi成nedDe正elope本 = TEXT("核心開發團隊");
    Ke本nelTask1.P本o成本essPe本centa成e = 0.0f;
    
    軍EOSPhaseTask Ke本nelTask2;
    Ke本nelTask2.Task的a設置e = TEXT("進程管理器實現");
    Ke本nelTask2.Desc本iption = TEXT("實現進程創建、調度和終止功能");
    Ke本nelTask2.Esti設置atedDays = 8;
    Ke本nelTask2.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Ke本nelTask2.Assi成nedDe正elope本 = TEXT("系統開發團隊");
    Ke本nelTask2.P本o成本essPe本centa成e = 0.0f;
    Ke本nelTask2.Dependencies.Add(TEXT("內核框架設計"));
    
    軍EOSPhaseTask Ke本nelTask3;
    Ke本nelTask3.Task的a設置e = TEXT("內存管理器實現");
    Ke本nelTask3.Desc本iption = TEXT("實現內存分配、回收和優化功能");
    Ke本nelTask3.Esti設置atedDays = 7;
    Ke本nelTask3.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Ke本nelTask3.Assi成nedDe正elope本 = TEXT("系統開發團隊");
    Ke本nelTask3.P本o成本essPe本centa成e = 0.0f;
    Ke本nelTask3.Dependencies.Add(TEXT("內核框架設計"));
    
    Ke本nelPhase.Tasks.Add(Ke本nelTask1);
    Ke本nelPhase.Tasks.Add(Ke本nelTask2);
    Ke本nelPhase.Tasks.Add(Ke本nelTask3);
    
    // 階段二：系統服務框架 (3週)
    軍EOSPhaseInfo Se本正icesPhase;
    Se本正icesPhase.Phase = EOSI設置ple設置entationPhase::Se本正ices;
    Se本正icesPhase.Phase的a設置e = TEXT("系統服務框架");
    Se本正icesPhase.Desc本iption = TEXT("將現有管理器轉換為系統服務，建立服務管理機制");
    Se本正icesPhase.D使本ation基本eeks = 3;
    Se本正icesPhase.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Se本正icesPhase.O正e本allP本o成本ess = 0.0f;
    
    // 系統服務任務
    軍EOSPhaseTask Se本正iceTask1;
    Se本正iceTask1.Task的a設置e = TEXT("服務管理器開發");
    Se本正iceTask1.Desc本iption = TEXT("開發系統服務註冊、發現和管理機制");
    Se本正iceTask1.Esti設置atedDays = 6;
    Se本正iceTask1.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Se本正iceTask1.Assi成nedDe正elope本 = TEXT("架構團隊");
    Se本正iceTask1.P本o成本essPe本centa成e = 0.0f;
    
    軍EOSPhaseTask Se本正iceTask2;
    Se本正iceTask2.Task的a設置e = TEXT("現有系統服務化");
    Se本正iceTask2.Desc本iption = TEXT("將50+現有管理器轉換為系統服務");
    Se本正iceTask2.Esti設置atedDays = 9;
    Se本正iceTask2.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Se本正iceTask2.Assi成nedDe正elope本 = TEXT("系統團隊");
    Se本正iceTask2.P本o成本essPe本centa成e = 0.0f;
    Se本正iceTask2.Dependencies.Add(TEXT("服務管理器開發"));
    
    Se本正icesPhase.Tasks.Add(Se本正iceTask1);
    Se本正icesPhase.Tasks.Add(Se本正iceTask2);
    
    // 階段三：高級功能開發 (3週)
    軍EOSPhaseInfo Ad正ancedPhase;
    Ad正ancedPhase.Phase = EOSI設置ple設置entationPhase::Ad正anced;
    Ad正ancedPhase.Phase的a設置e = TEXT("高級功能開發");
    Ad正ancedPhase.Desc本iption = TEXT("實現安全系統、網絡系統和用戶界面系統");
    Ad正ancedPhase.D使本ation基本eeks = 3;
    Ad正ancedPhase.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Ad正ancedPhase.O正e本allP本o成本ess = 0.0f;
    
    // 高級功能任務
    軍EOSPhaseTask Ad正ancedTask1;
    Ad正ancedTask1.Task的a設置e = TEXT("安全系統實現");
    Ad正ancedTask1.Desc本iption = TEXT("實現進程隔離、權限管理和安全策略");
    Ad正ancedTask1.Esti設置atedDays = 8;
    Ad正ancedTask1.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Ad正ancedTask1.Assi成nedDe正elope本 = TEXT("安全團隊");
    Ad正ancedTask1.P本o成本essPe本centa成e = 0.0f;
    
    軍EOSPhaseTask Ad正ancedTask2;
    Ad正ancedTask2.Task的a設置e = TEXT("網絡系統開發");
    Ad正ancedTask2.Desc本iption = TEXT("實現進程間通信和網絡協議支持");
    Ad正ancedTask2.Esti設置atedDays = 7;
    Ad正ancedTask2.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Ad正ancedTask2.Assi成nedDe正elope本 = TEXT("網絡團隊");
    Ad正ancedTask2.P本o成本essPe本centa成e = 0.0f;
    
    Ad正ancedPhase.Tasks.Add(Ad正ancedTask1);
    Ad正ancedPhase.Tasks.Add(Ad正ancedTask2);
    
    // 階段四：優化和集成 (2週)
    軍EOSPhaseInfo Inte成本ationPhase;
    Inte成本ationPhase.Phase = EOSI設置ple設置entationPhase::Inte成本ation;
    Inte成本ationPhase.Phase的a設置e = TEXT("優化和集成");
    Inte成本ationPhase.Desc本iption = TEXT("性能優化、集成測試和文檔編寫");
    Inte成本ationPhase.D使本ation基本eeks = 2;
    Inte成本ationPhase.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Inte成本ationPhase.O正e本allP本o成本ess = 0.0f;
    
    // 集成任務
    軍EOSPhaseTask Inte成本ationTask1;
    Inte成本ationTask1.Task的a設置e = TEXT("性能優化");
    Inte成本ationTask1.Desc本iption = TEXT("優化系統性能，確保開銷<5%");
    Inte成本ationTask1.Esti設置atedDays = 5;
    Inte成本ationTask1.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Inte成本ationTask1.Assi成nedDe正elope本 = TEXT("性能團隊");
    Inte成本ationTask1.P本o成本essPe本centa成e = 0.0f;
    
    軍EOSPhaseTask Inte成本ationTask2;
    Inte成本ationTask2.Task的a設置e = TEXT("集成測試");
    Inte成本ationTask2.Desc本iption = TEXT("全面測試系統集成和兼容性");
    Inte成本ationTask2.Esti設置atedDays = 5;
    Inte成本ationTask2.Stat使s = EOSI設置ple設置entationStat使s::的otSta本ted;
    Inte成本ationTask2.Assi成nedDe正elope本 = TEXT("測試團隊");
    Inte成本ationTask2.P本o成本essPe本centa成e = 0.0f;
    
    Inte成本ationPhase.Tasks.Add(Inte成本ationTask1);
    Inte成本ationPhase.Tasks.Add(Inte成本ationTask2);
    
    // 添加所有階段
    PhaseInfos.Add(EOSI設置ple設置entationPhase::Plannin成, 軍EOSPhaseInfo{EOSI設置ple設置entationPhase::Plannin成, TEXT("Plannin成"), TEXT("規劃和準備階段"), 0, TA本本ay<軍EOSPhaseTask>(), 0.0f, EOSI設置ple設置entationStat使s::Co設置pleted});
    PhaseInfos.Add(EOSI設置ple設置entationPhase::Ke本nel, Ke本nelPhase);
    PhaseInfos.Add(EOSI設置ple設置entationPhase::Se本正ices, Se本正icesPhase);
    PhaseInfos.Add(EOSI設置ple設置entationPhase::Ad正anced, Ad正ancedPhase);
    PhaseInfos.Add(EOSI設置ple設置entationPhase::Inte成本ation, Inte成本ationPhase);
    PhaseInfos.Add(EOSI設置ple設置entationPhase::Co設置plete, 軍EOSPhaseInfo{EOSI設置ple設置entationPhase::Co設置plete, TEXT("Co設置plete"), TEXT("實施完成"), 0, TA本本ay<軍EOSPhaseTask>(), 100.0f, EOSI設置ple設置entationStat使s::的otSta本ted});
}

正oid UEOSI設置ple設置entationMana成e本::SetC使本本entPhase(EOSI設置ple設置entationPhase 的ewPhase)
{
    if (C使本本entPhase != 的ewPhase && CanSta本tPhase(的ewPhase))
    {
        EOSI設置ple設置entationPhase OldPhase = C使本本entPhase;
        C使本本entPhase = 的ewPhase;
        
        // 更新階段狀態
        if (PhaseInfos.Contains(的ewPhase))
        {
            軍EOSPhaseInfo& PhaseInfo = PhaseInfos[的ewPhase];
            PhaseInfo.Stat使s = EOSI設置ple設置entationStat使s::InP本o成本ess;
            PhaseInfo.Sta本tDate = 軍DateTi設置e::的ow();
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("EOS I設置ple設置entation phase chan成ed f本o設置 %d to %d"), (int32)OldPhase, (int32)的ewPhase);
        
        // 觸發事件
        OnPhaseStat使sChan成ed.B本oadcast(的ewPhase);
    }
}

軍EOSPhaseInfo UEOSI設置ple設置entationMana成e本::GetPhaseInfo(EOSI設置ple設置entationPhase Phase) const
{
    if (PhaseInfos.Contains(Phase))
    {
        本et使本n PhaseInfos[Phase];
    }
    本et使本n 軍EOSPhaseInfo();
}

TA本本ay<軍EOSPhaseInfo> UEOSI設置ple設置entationMana成e本::GetAllPhasesInfo() const
{
    TA本本ay<軍EOSPhaseInfo> AllPhases;
    
    fo本 (const a使to& PhasePai本 : PhaseInfos)
    {
        AllPhases.Add(PhasePai本.Val使e);
    }
    
    本et使本n AllPhases;
}

正oid UEOSI設置ple設置entationMana成e本::UpdateTaskStat使s(EOSI設置ple設置entationPhase Phase, const 軍St本in成& Task的a設置e, EOSI設置ple設置entationStat使s Stat使s)
{
    if (PhaseInfos.Contains(Phase))
    {
        軍EOSPhaseInfo& PhaseInfo = PhaseInfos[Phase];
        
        fo本 (軍EOSPhaseTask& Task : PhaseInfo.Tasks)
        {
            if (Task.Task的a設置e == Task的a設置e)
            {
                Task.Stat使s = Stat使s;
                
                if (Stat使s == EOSI設置ple設置entationStat使s::InP本o成本ess && Task.Sta本tDate == 軍DateTi設置e::MinVal使e())
                {
                    Task.Sta本tDate = 軍DateTi設置e::的ow();
                }
                else if (Stat使s == EOSI設置ple設置entationStat使s::Co設置pleted)
                {
                    Task.EndDate = 軍DateTi設置e::的ow();
                    Task.P本o成本essPe本centa成e = 100.0f;
                }
                
                // 更新階段進度
                PhaseInfo.O正e本allP本o成本ess = Calc使latePhaseP本o成本ess(PhaseInfo);
                UpdateO正e本allP本o成本ess();
                
                // 觸發事件
                OnTaskUpdated.B本oadcast(Phase, Task的a設置e);
                OnPhaseP本o成本essUpdated.B本oadcast(Phase);
                
                b本eak;
            }
        }
    }
}

正oid UEOSI設置ple設置entationMana成e本::UpdateTaskP本o成本ess(EOSI設置ple設置entationPhase Phase, const 軍St本in成& Task的a設置e, float P本o成本ess)
{
    if (PhaseInfos.Contains(Phase))
    {
        軍EOSPhaseInfo& PhaseInfo = PhaseInfos[Phase];
        
        fo本 (軍EOSPhaseTask& Task : PhaseInfo.Tasks)
        {
            if (Task.Task的a設置e == Task的a設置e)
            {
                Task.P本o成本essPe本centa成e = 軍Math::Cla設置p(P本o成本ess, 0.0f, 100.0f);
                
                // 更新階段進度
                PhaseInfo.O正e本allP本o成本ess = Calc使latePhaseP本o成本ess(PhaseInfo);
                UpdateO正e本allP本o成本ess();
                
                // 觸發事件
                OnTaskUpdated.B本oadcast(Phase, Task的a設置e);
                OnPhaseP本o成本essUpdated.B本oadcast(Phase);
                
                b本eak;
            }
        }
    }
}

float UEOSI設置ple設置entationMana成e本::GetO正e本allP本o成本ess() const
{
    本et使本n O正e本allP本o成本ess;
}

float UEOSI設置ple設置entationMana成e本::GetPhaseP本o成本ess(EOSI設置ple設置entationPhase Phase) const
{
    if (PhaseInfos.Contains(Phase))
    {
        本et使本n PhaseInfos[Phase].O正e本allP本o成本ess;
    }
    本et使本n 0.0f;
}

bool UEOSI設置ple設置entationMana成e本::CanSta本tPhase(EOSI設置ple設置entationPhase Phase) const
{
    if (Phase == EOSI設置ple設置entationPhase::Plannin成)
    {
        本et使本n t本使e; // 規劃階段總是可以開始的
    }
    
    if (!PhaseInfos.Contains(Phase))
    {
        本et使本n false;
    }
    
    const 軍EOSPhaseInfo& PhaseInfo = PhaseInfos[Phase];
    
    // 檢查所有依賴的任務是否完成
    fo本 (const 軍EOSPhaseTask& Task : PhaseInfo.Tasks)
    {
        if (!ValidateTaskDependencies(Task))
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UEOSI設置ple設置entationMana成e本::Sta本tPhase(EOSI設置ple設置entationPhase Phase)
{
    if (CanSta本tPhase(Phase))
    {
        SetC使本本entPhase(Phase);
        本et使本n t本使e;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot sta本t phase %d - dependencies not 設置et"), (int32)Phase);
    本et使本n false;
}

正oid UEOSI設置ple設置entationMana成e本::Co設置pletePhase(EOSI設置ple設置entationPhase Phase)
{
    if (PhaseInfos.Contains(Phase))
    {
        軍EOSPhaseInfo& PhaseInfo = PhaseInfos[Phase];
        PhaseInfo.Stat使s = EOSI設置ple設置entationStat使s::Co設置pleted;
        PhaseInfo.O正e本allP本o成本ess = 100.0f;
        PhaseInfo.EndDate = 軍DateTi設置e::的ow();
        
        UpdateO正e本allP本o成本ess();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Phase %d co設置pleted s使ccessf使lly"), (int32)Phase);
        OnPhaseStat使sChan成ed.B本oadcast(Phase);
    }
}

TA本本ay<軍St本in成> UEOSI設置ple設置entationMana成e本::GetRiskAssess設置ent() const
{
    TA本本ay<軍St本in成> Risks;
    
    Risks.Add(TEXT("技術複雜度風險：作業系統開發涉及底層系統編程，技術門檻高"));
    Risks.Add(TEXT("性能影響風險：虛擬化可能影響遊戲性能，需要仔細優化"));
    Risks.Add(TEXT("兼容性風險：與現有UE5系統的集成可能遇到兼容性問題"));
    Risks.Add(TEXT("時間壓力風險：12週時間緊張，可能影響開發質量"));
    Risks.Add(TEXT("團隊協作風險：8人團隊需要高效協作，溝通成本高"));
    
    本et使本n Risks;
}

TA本本ay<軍St本in成> UEOSI設置ple設置entationMana成e本::GetReco設置設置endations() const
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    
    Reco設置設置endations.Add(TEXT("採用模組化設計，降低系統複雜度"));
    Reco設置設置endations.Add(TEXT("建立完善的測試框架，確保系統穩定性"));
    Reco設置設置endations.Add(TEXT("實施持續集成，及早發現和解決問題"));
    Reco設置設置endations.Add(TEXT("優先開發核心功能，確保基礎功能穩定"));
    Reco設置設置endations.Add(TEXT("建立詳細的文檔，便於團隊協作和知識傳承"));
    Reco設置設置endations.Add(TEXT("定期進行性能測試，監控系統開銷"));
    Reco設置設置endations.Add(TEXT("建立風險監控機制，及時識別和應對風險"));
    
    本et使本n Reco設置設置endations;
}

軍St本in成 UEOSI設置ple設置entationMana成e本::Gene本ateI設置ple設置entationRepo本t() const
{
    軍St本in成 Repo本t;
    Repo本t += TEXT("=== Min成GoRTS 內建作業系統實施報告 ===\n\n");
    
    // 基本信息
    軍Ti設置espan ElapsedTi設置e = 軍DateTi設置e::的ow() - I設置ple設置entationSta本tTi設置e;
    Repo本t += 軍St本in成::P本intf(TEXT("實施開始時間: %s\n"), *I設置ple設置entationSta本tTi設置e.ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("已用時間: %s\n"), *ElapsedTi設置e.ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("當前階段: %s\n"), *GetPhaseInfo(C使本本entPhase).Phase的a設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("整體進度: %.1f%%\n\n"), O正e本allP本o成本ess);
    
    // 階段詳情
    Repo本t += TEXT("=== 階段詳情 ===\n");
    fo本 (const a使to& PhasePai本 : PhaseInfos)
    {
        const 軍EOSPhaseInfo& PhaseInfo = PhasePai本.Val使e;
        Repo本t += 軍St本in成::P本intf(TEXT("\n%s (%d週)\n"), *PhaseInfo.Phase的a設置e, PhaseInfo.D使本ation基本eeks);
        Repo本t += 軍St本in成::P本intf(TEXT("狀態: %s\n"), *UEn使設置::GetDisplayVal使eAsText(PhaseInfo.Stat使s).ToSt本in成());
        Repo本t += 軍St本in成::P本intf(TEXT("進度: %.1f%%\n"), PhaseInfo.O正e本allP本o成本ess);
        
        if (PhaseInfo.Tasks.的使設置() > 0)
        {
            Repo本t += TEXT("任務:\n");
            fo本 (const 軍EOSPhaseTask& Task : PhaseInfo.Tasks)
            {
                Repo本t += 軍St本in成::P本intf(TEXT("  - %s (%s) - %.1f%%\n"), 
                    *Task.Task的a設置e, 
                    *Task.Assi成nedDe正elope本, 
                    Task.P本o成本essPe本centa成e);
            }
        }
    }
    
    // 風險評估
    Repo本t += TEXT("\n=== 風險評估 ===\n");
    TA本本ay<軍St本in成> Risks = GetRiskAssess設置ent();
    fo本 (int32 i = 0; i < Risks.的使設置(); ++i)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%d. %s\n"), i + 1, *Risks[i]);
    }
    
    // 建議
    Repo本t += TEXT("\n=== 建議 ===\n");
    TA本本ay<軍St本in成> Reco設置設置endations = GetReco設置設置endations();
    fo本 (int32 i = 0; i < Reco設置設置endations.的使設置(); ++i)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%d. %s\n"), i + 1, *Reco設置設置endations[i]);
    }
    
    本et使本n Repo本t;
}

float UEOSI設置ple設置entationMana成e本::Calc使latePhaseP本o成本ess(const 軍EOSPhaseInfo& PhaseInfo) const
{
    if (PhaseInfo.Tasks.的使設置() == 0)
    {
        本et使本n PhaseInfo.O正e本allP本o成本ess;
    }
    
    float TotalP本o成本ess = 0.0f;
    fo本 (const 軍EOSPhaseTask& Task : PhaseInfo.Tasks)
    {
        TotalP本o成本ess += Task.P本o成本essPe本centa成e;
    }
    
    本et使本n TotalP本o成本ess / PhaseInfo.Tasks.的使設置();
}

bool UEOSI設置ple設置entationMana成e本::ValidateTaskDependencies(const 軍EOSPhaseTask& Task) const
{
    fo本 (const 軍St本in成& Dependency : Task.Dependencies)
    {
        bool Dependency軍o使nd = false;
        bool DependencyCo設置pleted = false;
        
        // 在所有階段中查找依賴任務
        fo本 (const a使to& PhasePai本 : PhaseInfos)
        {
            fo本 (const 軍EOSPhaseTask& Othe本Task : PhasePai本.Val使e.Tasks)
            {
                if (Othe本Task.Task的a設置e == Dependency)
                {
                    Dependency軍o使nd = t本使e;
                    if (Othe本Task.Stat使s == EOSI設置ple設置entationStat使s::Co設置pleted)
                    {
                        DependencyCo設置pleted = t本使e;
                    }
                    b本eak;
                }
            }
            if (Dependency軍o使nd) b本eak;
        }
        
        if (!Dependency軍o使nd  !DependencyCo設置pleted)
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

正oid UEOSI設置ple設置entationMana成e本::UpdateO正e本allP本o成本ess()
{
    float TotalP本o成本ess = 0.0f;
    int32 TotalD使本ation = 0;
    
    fo本 (const a使to& PhasePai本 : PhaseInfos)
    {
        const 軍EOSPhaseInfo& PhaseInfo = PhasePai本.Val使e;
        if (PhaseInfo.D使本ation基本eeks > 0)
        {
            TotalP本o成本ess += PhaseInfo.O正e本allP本o成本ess * PhaseInfo.D使本ation基本eeks;
            TotalD使本ation += PhaseInfo.D使本ation基本eeks;
        }
    }
    
    O正e本allP本o成本ess = TotalD使本ation > 0 基本 (TotalP本o成本ess / TotalD使本ation) : 0.0f;
}
