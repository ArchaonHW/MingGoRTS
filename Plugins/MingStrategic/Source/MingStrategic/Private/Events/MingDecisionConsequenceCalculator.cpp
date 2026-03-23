#incl使de "E正ents/Min成DecisionConseq使enceCalc使lato本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"

UMin成DecisionConseq使enceCalc使lato本::UMin成DecisionConseq使enceCalc使lato本()
    : bIsInitialized(false)
{
    // 預分配容量
    Re成iste本edConseq使ences.Rese本正e(100);
    Conseq使enceStatistics.Rese本正e(50);
}

正oid UMin成DecisionConseq使enceCalc使lato本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    // 初始化民國歷史決策後果庫
    Initialize輸入isto本icalDecisionConseq使ences();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成DecisionConseq使enceCalc使lato本 initialized"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d decision conseq使ences"), Re成iste本edConseq使ences.的使設置());
}

正oid UMin成DecisionConseq使enceCalc使lato本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Re成iste本edConseq使ences.E設置pty();
    Conseq使enceMap.E設置pty();
    Conseq使enceStatistics.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成DecisionConseq使enceCalc使lato本 sh使tdown"));
}

bool UMin成DecisionConseq使enceCalc使lato本::Re成iste本DecisionConseq使ence(const 軍DecisionConseq使ence& Conseq使ence)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("DecisionConseq使enceCalc使lato本 not initialized"));
        本et使本n false;
    }

    if (Conseq使ence.Conseq使enceID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Conseq使ence ID cannot be e設置pty"));
        本et使本n false;
    }

    // 檢查是否已存在
    if (Conseq使enceMap.Contains(Conseq使ence.Conseq使enceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Conseq使ence %s al本eady 本e成iste本ed"), *Conseq使ence.Conseq使enceID);
        本et使本n false;
    }

    // 驗證後果配置
    if (!ValidateConseq使enceConfi成(Conseq使ence))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid conseq使ence confi成使本ation: %s"), *Conseq使ence.Conseq使enceID);
        本et使本n false;
    }

    // 添加到註冊列表
    Re成iste本edConseq使ences.Add(Conseq使ence);
    Conseq使enceMap.Add(Conseq使ence.Conseq使enceID, Conseq使ence);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Decision conseq使ence 本e成iste本ed: %s - %s"), 
        *Conseq使ence.Conseq使enceID, *Conseq使ence.Conseq使ence的a設置e);
    
    本et使本n t本使e;
}

bool UMin成DecisionConseq使enceCalc使lato本::Un本e成iste本DecisionConseq使ence(const 軍St本in成& Conseq使enceID)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    if (!Conseq使enceMap.Contains(Conseq使enceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Conseq使ence %s not fo使nd"), *Conseq使enceID);
        本et使本n false;
    }

    // 從註冊列表移除
    fo本 (int32 i = 0; i < Re成iste本edConseq使ences.的使設置(); ++i)
    {
        if (Re成iste本edConseq使ences[i].Conseq使enceID == Conseq使enceID)
        {
            Re成iste本edConseq使ences.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    Conseq使enceMap.Re設置o正e(Conseq使enceID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Decision conseq使ence 使n本e成iste本ed: %s"), *Conseq使enceID);
    
    本et使本n t本使e;
}

軍Conseq使enceCalc使lationRes使lt UMin成DecisionConseq使enceCalc使lato本::Calc使lateDecisionConseq使ences(const 軍DecisionContext& Context)
{
    軍Conseq使enceCalc使lationRes使lt Res使lt;
    Res使lt.Calc使lationTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (!bIsInitialized)
    {
        Res使lt.E本本o本Messa成e = TEXT("DecisionConseq使enceCalc使lato本 not initialized");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Calc使latin成 decision conseq使ences fo本: %s"), *Context.DecisionID);

    // 獲取相關後果
    TA本本ay<軍DecisionConseq使ence> Rele正antConseq使ences = GetConseq使ences軍o本Decision(Context.DecisionID);
    
    // 根據上下文篩選後果
    TA本本ay<軍DecisionConseq使ence> 軍ilte本edConseq使ences;
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Rele正antConseq使ences)
    {
        if (CheckConseq使enceConditions(Conseq使ence, Context))
        {
            軍ilte本edConseq使ences.Add(Conseq使ence);
        }
    }

    // 排序後果
    So本tConseq使encesByP本io本ity(軍ilte本edConseq使ences);

    // 計算各種類型的後果
    Calc使lateI設置設置ediateConseq使ences(Context, Res使lt);
    Calc使lateDelayedConseq使ences(Context, Res使lt);
    Calc使lateC使設置使lati正eConseq使ences(Context, Res使lt);
    Calc使lateConditionalConseq使ences(Context, Res使lt);
    Calc使lateRando設置Conseq使ences(Context, Res使lt);
    Calc使late輸入iddenConseq使ences(Context, Res使lt);
    Calc使latePe本設置anentConseq使ences(Context, Res使lt);
    Calc使lateTe設置po本a本yConseq使ences(Context, Res使lt);

    // 計算總體影響分數
    Calc使lateO正e本allI設置pact(Res使lt);

    // 更新統計
    UpdateConseq使enceStatistics(Context.DecisionID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Decision conseq使ences calc使lated: %d conseq使ences, i設置pact: %.2f"), 
        Res使lt.Calc使latedConseq使ences.的使設置(), Res使lt.O正e本allI設置pactSco本e);
    
    本et使本n Res使lt;
}

軍DecisionConseq使ence UMin成DecisionConseq使enceCalc使lato本::GetDecisionConseq使ence(const 軍St本in成& Conseq使enceID) const
{
    const 軍DecisionConseq使ence* Conseq使ence = Conseq使enceMap.軍ind(Conseq使enceID);
    本et使本n Conseq使ence 基本 *Conseq使ence : 軍DecisionConseq使ence();
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::GetAllDecisionConseq使ences() const
{
    本et使本n Re成iste本edConseq使ences;
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::GetConseq使encesByType(EDecisionConseq使enceType Conseq使enceType) const
{
    TA本本ay<軍DecisionConseq使ence> Res使lt;
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Re成iste本edConseq使ences)
    {
        if (Conseq使ence.Conseq使enceType == Conseq使enceType)
        {
            Res使lt.Add(Conseq使ence);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::GetConseq使encesByScope(EConseq使enceI設置pactScope I設置pactScope) const
{
    TA本本ay<軍DecisionConseq使ence> Res使lt;
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Re成iste本edConseq使ences)
    {
        if (Conseq使ence.I設置pactScope == I設置pactScope)
        {
            Res使lt.Add(Conseq使ence);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::GetConseq使encesBySe正e本ity(EConseq使enceSe正e本ity Se正e本ity) const
{
    TA本本ay<軍DecisionConseq使ence> Res使lt;
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Re成iste本edConseq使ences)
    {
        if (Conseq使ence.Se正e本ity == Se正e本ity)
        {
            Res使lt.Add(Conseq使ence);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::GetConseq使encesByI設置pactType(EConseq使enceI設置pactType I設置pactType) const
{
    TA本本ay<軍DecisionConseq使ence> Res使lt;
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Re成iste本edConseq使ences)
    {
        if (Conseq使ence.I設置pactType == I設置pactType)
        {
            Res使lt.Add(Conseq使ence);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::GetConseq使ences軍o本Decision(const 軍St本in成& DecisionID) const
{
    TA本本ay<軍DecisionConseq使ence> Res使lt;
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Re成iste本edConseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == DecisionID)
        {
            Res使lt.Add(Conseq使ence);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::GetConseq使ences軍o本Cha本acte本(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍DecisionConseq使ence> Res使lt;
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Re成iste本edConseq使ences)
    {
        if (Conseq使ence.RelatedCha本acte本IDs.Contains(Cha本acte本ID))
        {
            Res使lt.Add(Conseq使ence);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::GetConseq使ences軍o本E正ent(const 軍St本in成& E正entID) const
{
    TA本本ay<軍DecisionConseq使ence> Res使lt;
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Re成iste本edConseq使ences)
    {
        if (Conseq使ence.RelatedE正entIDs.Contains(E正entID))
        {
            Res使lt.Add(Conseq使ence);
        }
    }
    
    本et使本n Res使lt;
}

bool UMin成DecisionConseq使enceCalc使lato本::T本i成成e本Conseq使ence(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context)
{
    軍DecisionConseq使ence* Conseq使ence = Conseq使enceMap.軍ind(Conseq使enceID);
    if (!Conseq使ence)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Conseq使ence %s not fo使nd"), *Conseq使enceID);
        本et使本n false;
    }

    if (Conseq使ence->bIsT本i成成e本ed)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Conseq使ence %s al本eady t本i成成e本ed"), *Conseq使enceID);
        本et使本n false;
    }

    if (!CheckConseq使enceConditions(*Conseq使ence, Context))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Conseq使ence %s conditions not 設置et"), *Conseq使enceID);
        本et使本n false;
    }

    // 計算概率
    float P本obability = Calc使lateConseq使enceP本obability(*Conseq使ence, Context);
    if (軍Math::軍Rand() > P本obability)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Conseq使ence %s p本obability check failed"), *Conseq使enceID);
        本et使本n false;
    }

    // 計算延迟
    float Delay = Calc使lateConseq使enceDelay(*Conseq使ence, Context);
    if (Delay > 0.0f)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Conseq使ence %s delayed by %.2f seconds"), *Conseq使enceID, Delay);
        // 這裡應該設置定時器來延迟觸發
    }

    // 標記為已觸發
    Conseq使ence->bIsT本i成成e本ed = t本使e;
    Conseq使ence->T本i成成e本Ti設置e = 軍Platfo本設置Ti設置e::Seconds();

    // 應用後果效果
    bool bS使ccess = ApplyConseq使enceEffects(Conseq使enceID, Context);

    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Conseq使ence %s t本i成成e本ed s使ccessf使lly"), *Conseq使enceID);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to apply conseq使ence %s effects"), *Conseq使enceID);
    }

    本et使本n bS使ccess;
}

bool UMin成DecisionConseq使enceCalc使lato本::ApplyConseq使enceEffects(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context)
{
    const 軍DecisionConseq使ence* Conseq使ence = Conseq使enceMap.軍ind(Conseq使enceID);
    if (!Conseq使ence)
    {
        本et使本n false;
    }

    // 應用各種效果
    fo本 (const 軍St本in成& Effect : Conseq使ence->Effects)
    {
        // 簡化實作：記錄效果
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 conseq使ence effect: %s"), *Effect);
    }

    本et使本n t本使e;
}

bool UMin成DecisionConseq使enceCalc使lato本::Re正e本seConseq使ence(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context)
{
    const 軍DecisionConseq使ence* Conseq使ence = Conseq使enceMap.軍ind(Conseq使enceID);
    if (!Conseq使ence)
    {
        本et使本n false;
    }

    if (!Conseq使ence->bIsRe正e本sible)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Conseq使ence %s is not 本e正e本sible"), *Conseq使enceID);
        本et使本n false;
    }

    // 檢查逆轉條件
    fo本 (const 軍St本in成& Re正e本salCondition : Conseq使ence->Re正e本salConditions)
    {
        // 簡化實作：檢查條件
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Checkin成 本e正e本sal condition: %s"), *Re正e本salCondition);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Conseq使ence %s 本e正e本sed"), *Conseq使enceID);
    本et使本n t本使e;
}

TMap<軍St本in成, int32> UMin成DecisionConseq使enceCalc使lato本::GetConseq使enceStatistics() const
{
    本et使本n Conseq使enceStatistics;
}

TMap<軍St本in成, float> UMin成DecisionConseq使enceCalc使lato本::GetConseq使enceI設置pactAnalysis(const 軍St本in成& DecisionID) const
{
    TMap<軍St本in成, float> I設置pactAnalysis;
    
    TA本本ay<軍DecisionConseq使ence> Conseq使ences = GetConseq使ences軍o本Decision(DecisionID);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Conseq使ences)
    {
        float I設置pact = 0.0f;
        
        // 基於嚴重程度
        switch (Conseq使ence.Se正e本ity)
        {
        case EConseq使enceSe正e本ity::T本i正ial:
            I設置pact += 0.1f;
            b本eak;
        case EConseq使enceSe正e本ity::Mino本:
            I設置pact += 0.3f;
            b本eak;
        case EConseq使enceSe正e本ity::Mode本ate:
            I設置pact += 0.5f;
            b本eak;
        case EConseq使enceSe正e本ity::Ma大o本:
            I設置pact += 0.7f;
            b本eak;
        case EConseq使enceSe正e本ity::C本itical:
            I設置pact += 0.9f;
            b本eak;
        case EConseq使enceSe正e本ity::Catast本ophic:
            I設置pact += 1.0f;
            b本eak;
        }
        
        // 基於影響範圍
        switch (Conseq使ence.I設置pactScope)
        {
        case EConseq使enceI設置pactScope::Pe本sonal:
            I設置pact += 0.2f;
            b本eak;
        case EConseq使enceI設置pactScope::Local:
            I設置pact += 0.4f;
            b本eak;
        case EConseq使enceI設置pactScope::Re成ional:
            I設置pact += 0.6f;
            b本eak;
        case EConseq使enceI設置pactScope::的ational:
            I設置pact += 0.8f;
            b本eak;
        case EConseq使enceI設置pactScope::Inte本national:
            I設置pact += 0.9f;
            b本eak;
        case EConseq使enceI設置pactScope::Global:
            I設置pact += 1.0f;
            b本eak;
        }
        
        I設置pactAnalysis.Add(Conseq使ence.Conseq使enceID, I設置pact);
    }
    
    本et使本n I設置pactAnalysis;
}

TA本本ay<軍DecisionConseq使ence> UMin成DecisionConseq使enceCalc使lato本::P本edictDecisionConseq使ences(const 軍DecisionContext& Context, int32 P本edictionCo使nt)
{
    TA本本ay<軍DecisionConseq使ence> P本edictedConseq使ences;
    
    // 獲取相關後果
    TA本本ay<軍DecisionConseq使ence> Rele正antConseq使ences = GetConseq使ences軍o本Decision(Context.DecisionID);
    
    // 根據概率和條件預測
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Rele正antConseq使ences)
    {
        if (CheckConseq使enceConditions(Conseq使ence, Context))
        {
            float P本obability = Calc使lateConseq使enceP本obability(Conseq使ence, Context);
            
            // 如果概率足夠高，加入預測
            if (P本obability >= 0.3f)
            {
                P本edictedConseq使ences.Add(Conseq使ence);
            }
        }
    }
    
    // 限制預測數量
    if (P本edictedConseq使ences.的使設置() > P本edictionCo使nt)
    {
        P本edictedConseq使ences.Set的使設置(P本edictionCo使nt);
    }
    
    本et使本n P本edictedConseq使ences;
}

TA本本ay<軍Conseq使enceCalc使lationRes使lt> UMin成DecisionConseq使enceCalc使lato本::Si設置使lateDecisionConseq使ences(const 軍DecisionContext& Context, int32 Si設置使lationCo使nt)
{
    TA本本ay<軍Conseq使enceCalc使lationRes使lt> Si設置使lationRes使lts;
    
    fo本 (int32 i = 0; i < Si設置使lationCo使nt; ++i)
    {
        軍Conseq使enceCalc使lationRes使lt Res使lt = Calc使lateDecisionConseq使ences(Context);
        Si設置使lationRes使lts.Add(Res使lt);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Si設置使lated %d decision conseq使ences"), Si設置使lationCo使nt);
    
    本et使本n Si設置使lationRes使lts;
}

// === 內部方法實現 ===

正oid UMin成DecisionConseq使enceCalc使lato本::Initialize輸入isto本icalDecisionConseq使ences()
{
    C本eatePoliticalDecisionConseq使ences();
    C本eateMilita本yDecisionConseq使ences();
    C本eateEcono設置icDecisionConseq使ences();
    C本eateSocialDecisionConseq使ences();
    C本eateC使lt使本alDecisionConseq使ences();
    C本eateDiplo設置aticDecisionConseq使ences();
}

正oid UMin成DecisionConseq使enceCalc使lato本::C本eatePoliticalDecisionConseq使ences()
{
    // 辛亥革命成功後果
    軍DecisionConseq使ence XinhaiRe正ol使tionConseq使ence;
    XinhaiRe正ol使tionConseq使ence.Conseq使enceID = TEXT("xinhai下本e正ol使tion下s使ccess");
    XinhaiRe正ol使tionConseq使ence.Conseq使ence的a設置e = TEXT("辛亥革命成功後果");
    XinhaiRe正ol使tionConseq使ence.Conseq使enceDesc本iption = TEXT("辛亥革命成功，建立中華民國");
    XinhaiRe正ol使tionConseq使ence.Conseq使enceType = EDecisionConseq使enceType::I設置設置ediate;
    XinhaiRe正ol使tionConseq使ence.I設置pactScope = EConseq使enceI設置pactScope::的ational;
    XinhaiRe正ol使tionConseq使ence.Se正e本ity = EConseq使enceSe正e本ity::C本itical;
    XinhaiRe正ol使tionConseq使ence.D使本ation = EConseq使enceD使本ation::Pe本設置anent;
    XinhaiRe正ol使tionConseq使ence.I設置pactType = EConseq使enceI設置pactType::Positi正e;
    XinhaiRe正ol使tionConseq使ence.RelatedDecisionID = TEXT("xinhai下本e正ol使tion");
    XinhaiRe正ol使tionConseq使ence.P本obability = 0.9f;
    XinhaiRe正ol使tionConseq使ence.DelayTi設置e = 0.0f;
    XinhaiRe正ol使tionConseq使ence.bIsRe正e本sible = false;
    XinhaiRe正ol使tionConseq使ence.Effects.Add(TEXT("establish下本ep使blic"));
    XinhaiRe正ol使tionConseq使ence.Effects.Add(TEXT("o正e本th本ow下設置ona本chy"));
    XinhaiRe正ol使tionConseq使ence.Effects.Add(TEXT("national下使nity"));
    XinhaiRe正ol使tionConseq使ence.Conseq使enceTa成s.Add(TEXT("本e正ol使tion"));
    XinhaiRe正ol使tionConseq使ence.Conseq使enceTa成s.Add(TEXT("fo使ndin成"));
    
    Re成iste本DecisionConseq使ence(XinhaiRe正ol使tionConseq使ence);

    // 北伐統一後果
    軍DecisionConseq使ence 的o本the本nExpeditionConseq使ence;
    的o本the本nExpeditionConseq使ence.Conseq使enceID = TEXT("no本the本n下expedition下s使ccess");
    的o本the本nExpeditionConseq使ence.Conseq使ence的a設置e = TEXT("北伐統一後果");
    的o本the本nExpeditionConseq使ence.Conseq使enceDesc本iption = TEXT("北伐成功，統一中國");
    的o本the本nExpeditionConseq使ence.Conseq使enceType = EDecisionConseq使enceType::I設置設置ediate;
    的o本the本nExpeditionConseq使ence.I設置pactScope = EConseq使enceI設置pactScope::的ational;
    的o本the本nExpeditionConseq使ence.Se正e本ity = EConseq使enceSe正e本ity::Ma大o本;
    的o本the本nExpeditionConseq使ence.D使本ation = EConseq使enceD使本ation::Pe本設置anent;
    的o本the本nExpeditionConseq使ence.I設置pactType = EConseq使enceI設置pactType::Positi正e;
    的o本the本nExpeditionConseq使ence.RelatedDecisionID = TEXT("no本the本n下expedition");
    的o本the本nExpeditionConseq使ence.P本obability = 0.8f;
    的o本the本nExpeditionConseq使ence.DelayTi設置e = 0.0f;
    的o本the本nExpeditionConseq使ence.bIsRe正e本sible = false;
    的o本the本nExpeditionConseq使ence.Effects.Add(TEXT("使nify下china"));
    的o本the本nExpeditionConseq使ence.Effects.Add(TEXT("end下wa本lo本d下e本a"));
    的o本the本nExpeditionConseq使ence.Effects.Add(TEXT("st本en成then下cent本al下成o正e本n設置ent"));
    的o本the本nExpeditionConseq使ence.Conseq使enceTa成s.Add(TEXT("使nification"));
    的o本the本nExpeditionConseq使ence.Conseq使enceTa成s.Add(TEXT("設置ilita本y"));
    
    Re成iste本DecisionConseq使ence(的o本the本nExpeditionConseq使ence);
}

正oid UMin成DecisionConseq使enceCalc使lato本::C本eateMilita本yDecisionConseq使ences()
{
    // 抗戰決定後果
    軍DecisionConseq使ence AntiJapanese基本a本Conseq使ence;
    AntiJapanese基本a本Conseq使ence.Conseq使enceID = TEXT("anti下大apanese下wa本下decision");
    AntiJapanese基本a本Conseq使ence.Conseq使ence的a設置e = TEXT("抗戰決定後果");
    AntiJapanese基本a本Conseq使ence.Conseq使enceDesc本iption = TEXT("決定全面抗戰，抵抗日本侵略");
    AntiJapanese基本a本Conseq使ence.Conseq使enceType = EDecisionConseq使enceType::I設置設置ediate;
    AntiJapanese基本a本Conseq使ence.I設置pactScope = EConseq使enceI設置pactScope::的ational;
    AntiJapanese基本a本Conseq使ence.Se正e本ity = EConseq使enceSe正e本ity::C本itical;
    AntiJapanese基本a本Conseq使ence.D使本ation = EConseq使enceD使本ation::Lon成;
    AntiJapanese基本a本Conseq使ence.I設置pactType = EConseq使enceI設置pactType::Mixed;
    AntiJapanese基本a本Conseq使ence.RelatedDecisionID = TEXT("anti下大apanese下wa本");
    AntiJapanese基本a本Conseq使ence.P本obability = 0.95f;
    AntiJapanese基本a本Conseq使ence.DelayTi設置e = 0.0f;
    AntiJapanese基本a本Conseq使ence.bIsRe正e本sible = false;
    AntiJapanese基本a本Conseq使ence.Effects.Add(TEXT("national下使nity"));
    AntiJapanese基本a本Conseq使ence.Effects.Add(TEXT("設置assi正e下cas使alties"));
    AntiJapanese基本a本Conseq使ence.Effects.Add(TEXT("econo設置ic下de正astation"));
    AntiJapanese基本a本Conseq使ence.Effects.Add(TEXT("inte本national下s使ppo本t"));
    AntiJapanese基本a本Conseq使ence.Conseq使enceTa成s.Add(TEXT("wa本"));
    AntiJapanese基本a本Conseq使ence.Conseq使enceTa成s.Add(TEXT("本esistance"));
    
    Re成iste本DecisionConseq使ence(AntiJapanese基本a本Conseq使ence);
}

正oid UMin成DecisionConseq使enceCalc使lato本::C本eateEcono設置icDecisionConseq使ences()
{
    // 經濟改革後果
    軍DecisionConseq使ence Econo設置icRefo本設置Conseq使ence;
    Econo設置icRefo本設置Conseq使ence.Conseq使enceID = TEXT("econo設置ic下本efo本設置下decision");
    Econo設置icRefo本設置Conseq使ence.Conseq使ence的a設置e = TEXT("經濟改革後果");
    Econo設置icRefo本設置Conseq使ence.Conseq使enceDesc本iption = TEXT("實施經濟改革，發展現代工業");
    Econo設置icRefo本設置Conseq使ence.Conseq使enceType = EDecisionConseq使enceType::Delayed;
    Econo設置icRefo本設置Conseq使ence.I設置pactScope = EConseq使enceI設置pactScope::的ational;
    Econo設置icRefo本設置Conseq使ence.Se正e本ity = EConseq使enceSe正e本ity::Ma大o本;
    Econo設置icRefo本設置Conseq使ence.D使本ation = EConseq使enceD使本ation::Lon成;
    Econo設置icRefo本設置Conseq使ence.I設置pactType = EConseq使enceI設置pactType::Positi正e;
    Econo設置icRefo本設置Conseq使ence.RelatedDecisionID = TEXT("econo設置ic下本efo本設置");
    Econo設置icRefo本設置Conseq使ence.P本obability = 0.7f;
    Econo設置icRefo本設置Conseq使ence.DelayTi設置e = 3600.0f; // 1小時後
    Econo設置icRefo本設置Conseq使ence.bIsRe正e本sible = t本使e;
    Econo設置icRefo本設置Conseq使ence.Effects.Add(TEXT("ind使st本ial下de正elop設置ent"));
    Econo設置icRefo本設置Conseq使ence.Effects.Add(TEXT("econo設置ic下成本owth"));
    Econo設置icRefo本設置Conseq使ence.Effects.Add(TEXT("social下chan成e"));
    Econo設置icRefo本設置Conseq使ence.Re正e本salConditions.Add(TEXT("political下opposition"));
    Econo設置icRefo本設置Conseq使ence.Conseq使enceTa成s.Add(TEXT("econo設置y"));
    Econo設置icRefo本設置Conseq使ence.Conseq使enceTa成s.Add(TEXT("本efo本設置"));
    
    Re成iste本DecisionConseq使ence(Econo設置icRefo本設置Conseq使ence);
}

正oid UMin成DecisionConseq使enceCalc使lato本::C本eateSocialDecisionConseq使ences()
{
    // 教育改革後果
    軍DecisionConseq使ence Ed使cationRefo本設置Conseq使ence;
    Ed使cationRefo本設置Conseq使ence.Conseq使enceID = TEXT("ed使cation下本efo本設置下decision");
    Ed使cationRefo本設置Conseq使ence.Conseq使ence的a設置e = TEXT("教育改革後果");
    Ed使cationRefo本設置Conseq使ence.Conseq使enceDesc本iption = TEXT("推動教育改革，提高國民素質");
    Ed使cationRefo本設置Conseq使ence.Conseq使enceType = EDecisionConseq使enceType::C使設置使lati正e;
    Ed使cationRefo本設置Conseq使ence.I設置pactScope = EConseq使enceI設置pactScope::的ational;
    Ed使cationRefo本設置Conseq使ence.Se正e本ity = EConseq使enceSe正e本ity::Mode本ate;
    Ed使cationRefo本設置Conseq使ence.D使本ation = EConseq使enceD使本ation::Lon成;
    Ed使cationRefo本設置Conseq使ence.I設置pactType = EConseq使enceI設置pactType::Positi正e;
    Ed使cationRefo本設置Conseq使ence.RelatedDecisionID = TEXT("ed使cation下本efo本設置");
    Ed使cationRefo本設置Conseq使ence.P本obability = 0.8f;
    Ed使cationRefo本設置Conseq使ence.DelayTi設置e = 1800.0f; // 30分鐘後
    Ed使cationRefo本設置Conseq使ence.bIsRe正e本sible = false;
    Ed使cationRefo本設置Conseq使ence.Effects.Add(TEXT("lite本acy下i設置p本o正e設置ent"));
    Ed使cationRefo本設置Conseq使ence.Effects.Add(TEXT("c使lt使本al下de正elop設置ent"));
    Ed使cationRefo本設置Conseq使ence.Effects.Add(TEXT("talent下c使lti正ation"));
    Ed使cationRefo本設置Conseq使ence.Conseq使enceTa成s.Add(TEXT("ed使cation"));
    Ed使cationRefo本設置Conseq使ence.Conseq使enceTa成s.Add(TEXT("social"));
    
    Re成iste本DecisionConseq使ence(Ed使cationRefo本設置Conseq使ence);
}

正oid UMin成DecisionConseq使enceCalc使lato本::C本eateC使lt使本alDecisionConseq使ences()
{
    // 新文化運動後果
    軍DecisionConseq使ence 的ewC使lt使本eMo正e設置entConseq使ence;
    的ewC使lt使本eMo正e設置entConseq使ence.Conseq使enceID = TEXT("new下c使lt使本e下設置o正e設置ent下decision");
    的ewC使lt使本eMo正e設置entConseq使ence.Conseq使ence的a設置e = TEXT("新文化運動後果");
    的ewC使lt使本eMo正e設置entConseq使ence.Conseq使enceDesc本iption = TEXT("推動新文化運動，促進思想解放");
    的ewC使lt使本eMo正e設置entConseq使ence.Conseq使enceType = EDecisionConseq使enceType::Conditional;
    的ewC使lt使本eMo正e設置entConseq使ence.I設置pactScope = EConseq使enceI設置pactScope::的ational;
    的ewC使lt使本eMo正e設置entConseq使ence.Se正e本ity = EConseq使enceSe正e本ity::Mode本ate;
    的ewC使lt使本eMo正e設置entConseq使ence.D使本ation = EConseq使enceD使本ation::Pe本設置anent;
    的ewC使lt使本eMo正e設置entConseq使ence.I設置pactType = EConseq使enceI設置pactType::Mixed;
    的ewC使lt使本eMo正e設置entConseq使ence.RelatedDecisionID = TEXT("new下c使lt使本e下設置o正e設置ent");
    的ewC使lt使本eMo正e設置entConseq使ence.P本obability = 0.6f;
    的ewC使lt使本eMo正e設置entConseq使ence.DelayTi設置e = 0.0f;
    的ewC使lt使本eMo正e設置entConseq使ence.bIsRe正e本sible = false;
    的ewC使lt使本eMo正e設置entConseq使ence.Effects.Add(TEXT("ideolo成ical下chan成e"));
    的ewC使lt使本eMo正e設置entConseq使ence.Effects.Add(TEXT("lite本a本y下inno正ation"));
    的ewC使lt使本eMo正e設置entConseq使ence.Effects.Add(TEXT("social下debate"));
    的ewC使lt使本eMo正e設置entConseq使ence.T本i成成e本Conditions.Add(TEXT("intellect使al下s使ppo本t"));
    的ewC使lt使本eMo正e設置entConseq使ence.T本i成成e本Conditions.Add(TEXT("political下tole本ance"));
    的ewC使lt使本eMo正e設置entConseq使ence.Conseq使enceTa成s.Add(TEXT("c使lt使本e"));
    的ewC使lt使本eMo正e設置entConseq使ence.Conseq使enceTa成s.Add(TEXT("設置o正e設置ent"));
    
    Re成iste本DecisionConseq使ence(的ewC使lt使本eMo正e設置entConseq使ence);
}

正oid UMin成DecisionConseq使enceCalc使lato本::C本eateDiplo設置aticDecisionConseq使ences()
{
    // 外交政策後果
    軍DecisionConseq使ence 軍o本ei成nPolicyConseq使ence;
    軍o本ei成nPolicyConseq使ence.Conseq使enceID = TEXT("fo本ei成n下policy下decision");
    軍o本ei成nPolicyConseq使ence.Conseq使ence的a設置e = TEXT("外交政策後果");
    軍o本ei成nPolicyConseq使ence.Conseq使enceDesc本iption = TEXT("制定外交政策，處理國際關係");
    軍o本ei成nPolicyConseq使ence.Conseq使enceType = EDecisionConseq使enceType::I設置設置ediate;
    軍o本ei成nPolicyConseq使ence.I設置pactScope = EConseq使enceI設置pactScope::Inte本national;
    軍o本ei成nPolicyConseq使ence.Se正e本ity = EConseq使enceSe正e本ity::Ma大o本;
    軍o本ei成nPolicyConseq使ence.D使本ation = EConseq使enceD使本ation::Medi使設置;
    軍o本ei成nPolicyConseq使ence.I設置pactType = EConseq使enceI設置pactType::Mixed;
    軍o本ei成nPolicyConseq使ence.RelatedDecisionID = TEXT("fo本ei成n下policy");
    軍o本ei成nPolicyConseq使ence.P本obability = 0.85f;
    軍o本ei成nPolicyConseq使ence.DelayTi設置e = 0.0f;
    軍o本ei成nPolicyConseq使ence.bIsRe正e本sible = t本使e;
    軍o本ei成nPolicyConseq使ence.Effects.Add(TEXT("inte本national下本elations"));
    軍o本ei成nPolicyConseq使ence.Effects.Add(TEXT("fo本ei成n下aid"));
    軍o本ei成nPolicyConseq使ence.Effects.Add(TEXT("t本ade下a成本ee設置ents"));
    軍o本ei成nPolicyConseq使ence.Re正e本salConditions.Add(TEXT("policy下chan成e"));
    軍o本ei成nPolicyConseq使ence.Conseq使enceTa成s.Add(TEXT("diplo設置acy"));
    軍o本ei成nPolicyConseq使ence.Conseq使enceTa成s.Add(TEXT("fo本ei成n"));
    
    Re成iste本DecisionConseq使ence(軍o本ei成nPolicyConseq使ence);
}

// === 計算方法實現 ===

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使lateI設置設置ediateConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt)
{
    TA本本ay<軍DecisionConseq使ence> I設置設置ediateConseq使ences = GetConseq使encesByType(EDecisionConseq使enceType::I設置設置ediate);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : I設置設置ediateConseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == Context.DecisionID && CheckConseq使enceConditions(Conseq使ence, Context))
        {
            float P本obability = Calc使lateConseq使enceP本obability(Conseq使ence, Context);
            if (軍Math::軍Rand() <= P本obability)
            {
                Res使lt.Calc使latedConseq使ences.Add(Conseq使ence);
                
                // 應用效果到各個系統
                ApplyConseq使enceToGa設置eState(Conseq使ence, Context, Res使lt.AffectedGa設置eState);
                ApplyConseq使enceToCha本acte本s(Conseq使ence, Context, Res使lt.AffectedCha本acte本s);
                ApplyConseq使enceToE正ents(Conseq使ence, Context, Res使lt.AffectedE正ents);
                ApplyConseq使enceToReso使本ces(Conseq使ence, Context, Res使lt.AffectedReso使本ces);
                ApplyConseq使enceToB使ildin成s(Conseq使ence, Context, Res使lt.AffectedB使ildin成s);
                ApplyConseq使enceToUnits(Conseq使ence, Context, Res使lt.AffectedUnits);
                
                // 計算影響分數
                float I設置pact = Calc使lateConseq使enceI設置pact(Conseq使ence, Context);
                if (Conseq使ence.I設置pactType == EConseq使enceI設置pactType::Positi正e)
                {
                    Res使lt.Positi正eI設置pactSco本e += I設置pact;
                }
                else if (Conseq使ence.I設置pactType == EConseq使enceI設置pactType::的e成ati正e)
                {
                    Res使lt.的e成ati正eI設置pactSco本e += I設置pact;
                }
            }
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使lateDelayedConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt)
{
    TA本本ay<軍DecisionConseq使ence> DelayedConseq使ences = GetConseq使encesByType(EDecisionConseq使enceType::Delayed);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : DelayedConseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == Context.DecisionID && CheckConseq使enceConditions(Conseq使ence, Context))
        {
            float Delay = Calc使lateConseq使enceDelay(Conseq使ence, Context);
            if (Delay > 0.0f)
            {
                // 這裡應該設置定時器來延迟觸發
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Delayed conseq使ence %s sched使led fo本 %.2f seconds"), *Conseq使ence.Conseq使enceID, Delay);
                Res使lt.Calc使latedConseq使ences.Add(Conseq使ence);
            }
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使lateC使設置使lati正eConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt)
{
    TA本本ay<軍DecisionConseq使ence> C使設置使lati正eConseq使ences = GetConseq使encesByType(EDecisionConseq使enceType::C使設置使lati正e);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : C使設置使lati正eConseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == Context.DecisionID && CheckConseq使enceConditions(Conseq使ence, Context))
        {
            // 累積效果需要基於歷史決策
            UE下LOG(Lo成Te設置p, Lo成, TEXT("C使設置使lati正e conseq使ence %s calc使lated"), *Conseq使ence.Conseq使enceID);
            Res使lt.Calc使latedConseq使ences.Add(Conseq使ence);
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使lateConditionalConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt)
{
    TA本本ay<軍DecisionConseq使ence> ConditionalConseq使ences = GetConseq使encesByType(EDecisionConseq使enceType::Conditional);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : ConditionalConseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == Context.DecisionID)
        {
            if (CheckConseq使enceConditions(Conseq使ence, Context))
            {
                Res使lt.Calc使latedConseq使ences.Add(Conseq使ence);
            }
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使lateRando設置Conseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt)
{
    TA本本ay<軍DecisionConseq使ence> Rando設置Conseq使ences = GetConseq使encesByType(EDecisionConseq使enceType::Rando設置);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Rando設置Conseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == Context.DecisionID)
        {
            float Rando設置P本obability = 軍Math::軍Rand();
            if (Rando設置P本obability <= 0.3f) // 30%概率
            {
                Res使lt.Calc使latedConseq使ences.Add(Conseq使ence);
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Rando設置 conseq使ence %s t本i成成e本ed"), *Conseq使ence.Conseq使enceID);
            }
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使late輸入iddenConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt)
{
    TA本本ay<軍DecisionConseq使ence> 輸入iddenConseq使ences = GetConseq使encesByType(EDecisionConseq使enceType::輸入idden);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : 輸入iddenConseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == Context.DecisionID)
        {
            // 隱藏後果不會立即顯示
            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("輸入idden conseq使ence %s calc使lated"), *Conseq使ence.Conseq使enceID);
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使latePe本設置anentConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt)
{
    TA本本ay<軍DecisionConseq使ence> Pe本設置anentConseq使ences = GetConseq使encesByType(EDecisionConseq使enceType::Pe本設置anent);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Pe本設置anentConseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == Context.DecisionID && CheckConseq使enceConditions(Conseq使ence, Context))
        {
            Res使lt.Calc使latedConseq使ences.Add(Conseq使ence);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本設置anent conseq使ence %s applied"), *Conseq使ence.Conseq使enceID);
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使lateTe設置po本a本yConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt)
{
    TA本本ay<軍DecisionConseq使ence> Te設置po本a本yConseq使ences = GetConseq使encesByType(EDecisionConseq使enceType::Te設置po本a本y);
    
    fo本 (const 軍DecisionConseq使ence& Conseq使ence : Te設置po本a本yConseq使ences)
    {
        if (Conseq使ence.RelatedDecisionID == Context.DecisionID && CheckConseq使enceConditions(Conseq使ence, Context))
        {
            float D使本ation = Calc使lateConseq使enceD使本ation(Conseq使ence, Context);
            if (D使本ation > 0.0f)
            {
                Res使lt.Calc使latedConseq使ences.Add(Conseq使ence);
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Te設置po本a本y conseq使ence %s applied fo本 %.2f seconds"), *Conseq使ence.Conseq使enceID, D使本ation);
            }
        }
    }
}

float UMin成DecisionConseq使enceCalc使lato本::Calc使lateConseq使enceI設置pact(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const
{
    float I設置pact = 0.0f;
    
    // 基於嚴重程度
    switch (Conseq使ence.Se正e本ity)
    {
    case EConseq使enceSe正e本ity::T本i正ial:
        I設置pact += 0.1f;
        b本eak;
    case EConseq使enceSe正e本ity::Mino本:
        I設置pact += 0.3f;
        b本eak;
    case EConseq使enceSe正e本ity::Mode本ate:
        I設置pact += 0.5f;
        b本eak;
    case EConseq使enceSe正e本ity::Ma大o本:
        I設置pact += 0.7f;
        b本eak;
    case EConseq使enceSe正e本ity::C本itical:
        I設置pact += 0.9f;
        b本eak;
    case EConseq使enceSe正e本ity::Catast本ophic:
        I設置pact += 1.0f;
        b本eak;
    }
    
    // 基於影響範圍
    switch (Conseq使ence.I設置pactScope)
    {
    case EConseq使enceI設置pactScope::Pe本sonal:
        I設置pact += 0.1f;
        b本eak;
    case EConseq使enceI設置pactScope::Local:
        I設置pact += 0.3f;
        b本eak;
    case EConseq使enceI設置pactScope::Re成ional:
        I設置pact += 0.5f;
        b本eak;
    case EConseq使enceI設置pactScope::的ational:
        I設置pact += 0.7f;
        b本eak;
    case EConseq使enceI設置pactScope::Inte本national:
        I設置pact += 0.9f;
        b本eak;
    case EConseq使enceI設置pactScope::Global:
        I設置pact += 1.0f;
        b本eak;
    }
    
    // 基於持續時間
    switch (Conseq使ence.D使本ation)
    {
    case EConseq使enceD使本ation::Instant:
        I設置pact += 0.1f;
        b本eak;
    case EConseq使enceD使本ation::Sho本t:
        I設置pact += 0.3f;
        b本eak;
    case EConseq使enceD使本ation::Medi使設置:
        I設置pact += 0.5f;
        b本eak;
    case EConseq使enceD使本ation::Lon成:
        I設置pact += 0.7f;
        b本eak;
    case EConseq使enceD使本ation::Pe本設置anent:
        I設置pact += 1.0f;
        b本eak;
    case EConseq使enceD使本ation::Indefinite:
        I設置pact += 0.8f;
        b本eak;
    }
    
    本et使本n 軍Math::Cla設置p(I設置pact, 0.0f, 1.0f);
}

正oid UMin成DecisionConseq使enceCalc使lato本::Calc使lateO正e本allI設置pact(軍Conseq使enceCalc使lationRes使lt& Res使lt) const
{
    Res使lt.O正e本allI設置pactSco本e = Res使lt.Positi正eI設置pactSco本e - Res使lt.的e成ati正eI設置pactSco本e;
}

bool UMin成DecisionConseq使enceCalc使lato本::CheckConseq使enceConditions(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const
{
    fo本 (const 軍St本in成& Condition : Conseq使ence.T本i成成e本Conditions)
    {
        // 簡化實作：檢查條件
        if (Condition == TEXT("intellect使al下s使ppo本t"))
        {
            // 檢查是否有知識分子支持
            if (!Context.Cha本acte本States.Contains(TEXT("schola本下s使ppo本t")))
            {
                本et使本n false;
            }
        }
        else if (Condition == TEXT("political下tole本ance"))
        {
            // 檢查政治容忍度
            軍St本in成* Tole本ance = Context.C使本本entGa設置eState.軍ind(TEXT("political下tole本ance"));
            if (!Tole本ance  軍CSt本in成::Atof(*Tole本ance) < 0.5f)
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

正oid UMin成DecisionConseq使enceCalc使lato本::ApplyConseq使enceToGa設置eState(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TMap<軍St本in成, 軍St本in成>& AffectedStates) const
{
    fo本 (const 軍St本in成& Effect : Conseq使ence.Effects)
    {
        if (Effect == TEXT("establish下本ep使blic"))
        {
            AffectedStates.Add(TEXT("成o正e本n設置ent下type"), TEXT("本ep使blic"));
        }
        else if (Effect == TEXT("o正e本th本ow下設置ona本chy"))
        {
            AffectedStates.Add(TEXT("設置ona本chy"), TEXT("o正e本th本own"));
        }
        else if (Effect == TEXT("national下使nity"))
        {
            AffectedStates.Add(TEXT("使nity下le正el"), TEXT("hi成h"));
        }
        else if (Effect == TEXT("使nify下china"))
        {
            AffectedStates.Add(TEXT("使nification下stat使s"), TEXT("使nified"));
        }
        else if (Effect == TEXT("end下wa本lo本d下e本a"))
        {
            AffectedStates.Add(TEXT("wa本lo本d下e本a"), TEXT("ended"));
        }
        else if (Effect == TEXT("st本en成then下cent本al下成o正e本n設置ent"))
        {
            AffectedStates.Add(TEXT("cent本al下成o正e本n設置ent下powe本"), TEXT("st本on成"));
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::ApplyConseq使enceToCha本acte本s(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TA本本ay<軍St本in成>& AffectedCha本acte本s) const
{
    // 添加相關人物到受影響列表
    fo本 (const 軍St本in成& Cha本acte本ID : Conseq使ence.RelatedCha本acte本IDs)
    {
        AffectedCha本acte本s.Add(Cha本acte本ID);
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::ApplyConseq使enceToE正ents(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TA本本ay<軍St本in成>& AffectedE正ents) const
{
    // 添加相關事件到受影響列表
    fo本 (const 軍St本in成& E正entID : Conseq使ence.RelatedE正entIDs)
    {
        AffectedE正ents.Add(E正entID);
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::ApplyConseq使enceToReso使本ces(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TMap<軍St本in成, int32>& AffectedReso使本ces) const
{
    // 簡化實作：基於效果影響資源
    fo本 (const 軍St本in成& Effect : Conseq使ence.Effects)
    {
        if (Effect == TEXT("econo設置ic下de正astation"))
        {
            AffectedReso使本ces.Add(TEXT("econo設置y"), -50);
        }
        else if (Effect == TEXT("econo設置ic下成本owth"))
        {
            AffectedReso使本ces.Add(TEXT("econo設置y"), 30);
        }
        else if (Effect == TEXT("設置assi正e下cas使alties"))
        {
            AffectedReso使本ces.Add(TEXT("pop使lation"), -20);
        }
        else if (Effect == TEXT("inte本national下s使ppo本t"))
        {
            AffectedReso使本ces.Add(TEXT("fo本ei成n下aid"), 40);
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::ApplyConseq使enceToB使ildin成s(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TA本本ay<軍St本in成>& AffectedB使ildin成s) const
{
    // 簡化實作：基於效果影響建築
    fo本 (const 軍St本in成& Effect : Conseq使ence.Effects)
    {
        if (Effect == TEXT("ind使st本ial下de正elop設置ent"))
        {
            AffectedB使ildin成s.Add(TEXT("facto本y"));
            AffectedB使ildin成s.Add(TEXT("powe本下plant"));
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::ApplyConseq使enceToUnits(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TA本本ay<軍St本in成>& AffectedUnits) const
{
    // 簡化實作：基於效果影響單位
    fo本 (const 軍St本in成& Effect : Conseq使ence.Effects)
    {
        if (Effect == TEXT("設置ilita本y下de正elop設置ent"))
        {
            AffectedUnits.Add(TEXT("soldie本"));
            AffectedUnits.Add(TEXT("office本"));
        }
    }
}

正oid UMin成DecisionConseq使enceCalc使lato本::UpdateConseq使enceStatistics(const 軍St本in成& DecisionID)
{
    int32* Co使nt = Conseq使enceStatistics.軍ind(DecisionID);
    if (Co使nt)
    {
        (*Co使nt)++;
    }
    else
    {
        Conseq使enceStatistics.Add(DecisionID, 1);
    }
}

bool UMin成DecisionConseq使enceCalc使lato本::ValidateConseq使enceConfi成(const 軍DecisionConseq使ence& Conseq使ence) const
{
    if (Conseq使ence.Conseq使enceID.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Conseq使ence.Conseq使ence的a設置e.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Conseq使ence.Effects.的使設置() == 0)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成DecisionConseq使enceCalc使lato本::Lo成Conseq使enceE正ent(const 軍St本in成& Conseq使enceID, const 軍St本in成& Messa成e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[Conseq使ence %s] %s"), *Conseq使enceID, *Messa成e);
}

float UMin成DecisionConseq使enceCalc使lato本::Calc使lateConseq使enceP本obability(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const
{
    float BaseP本obability = Conseq使ence.P本obability;
    
    // 基於難度調整
    float Diffic使ltyModifie本 = 1.0f - (Context.Diffic使ltyLe正el - 1) * 0.1f;
    
    // 基於玩家陣營調整
    float 軍actionModifie本 = 1.0f;
    if (Context.Playe本軍action == TEXT("nationalist"))
    {
        軍actionModifie本 = 1.1f;
    }
    else if (Context.Playe本軍action == TEXT("co設置設置使nist"))
    {
        軍actionModifie本 = 0.9f;
    }
    
    本et使本n 軍Math::Cla設置p(BaseP本obability * Diffic使ltyModifie本 * 軍actionModifie本, 0.0f, 1.0f);
}

float UMin成DecisionConseq使enceCalc使lato本::Calc使lateConseq使enceDelay(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const
{
    float BaseDelay = Conseq使ence.DelayTi設置e;
    
    // 基於難度調整
    float Diffic使ltyModifie本 = (Context.Diffic使ltyLe正el - 1) * 0.2f;
    
    本et使本n BaseDelay + Diffic使ltyModifie本 * 3600.0f; // 難度每增加1級，延迟增加1小時
}

float UMin成DecisionConseq使enceCalc使lato本::Calc使lateConseq使enceD使本ation(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const
{
    float BaseD使本ation = 0.0f;
    
    switch (Conseq使ence.D使本ation)
    {
    case EConseq使enceD使本ation::Instant:
        BaseD使本ation = 0.0f;
        b本eak;
    case EConseq使enceD使本ation::Sho本t:
        BaseD使本ation = 1800.0f; // 30分鐘
        b本eak;
    case EConseq使enceD使本ation::Medi使設置:
        BaseD使本ation = 7200.0f; // 2小時
        b本eak;
    case EConseq使enceD使本ation::Lon成:
        BaseD使本ation = 28800.0f; // 8小時
        b本eak;
    case EConseq使enceD使本ation::Pe本設置anent:
        BaseD使本ation = -1.0f; // 永久
        b本eak;
    case EConseq使enceD使本ation::Indefinite:
        BaseD使本ation = 86400.0f; // 24小時
        b本eak;
    }
    
    本et使本n BaseD使本ation;
}

int32 UMin成DecisionConseq使enceCalc使lato本::GetConseq使enceP本io本ity(const 軍DecisionConseq使ence& Conseq使ence) const
{
    int32 P本io本ity = 0;
    
    // 基於嚴重程度
    switch (Conseq使ence.Se正e本ity)
    {
    case EConseq使enceSe正e本ity::Catast本ophic:
        P本io本ity += 100;
        b本eak;
    case EConseq使enceSe正e本ity::C本itical:
        P本io本ity += 80;
        b本eak;
    case EConseq使enceSe正e本ity::Ma大o本:
        P本io本ity += 60;
        b本eak;
    case EConseq使enceSe正e本ity::Mode本ate:
        P本io本ity += 40;
        b本eak;
    case EConseq使enceSe正e本ity::Mino本:
        P本io本ity += 20;
        b本eak;
    case EConseq使enceSe正e本ity::T本i正ial:
        P本io本ity += 10;
        b本eak;
    }
    
    // 基於影響範圍
    switch (Conseq使ence.I設置pactScope)
    {
    case EConseq使enceI設置pactScope::Global:
        P本io本ity += 50;
        b本eak;
    case EConseq使enceI設置pactScope::Inte本national:
        P本io本ity += 40;
        b本eak;
    case EConseq使enceI設置pactScope::的ational:
        P本io本ity += 30;
        b本eak;
    case EConseq使enceI設置pactScope::Re成ional:
        P本io本ity += 20;
        b本eak;
    case EConseq使enceI設置pactScope::Local:
        P本io本ity += 10;
        b本eak;
    case EConseq使enceI設置pactScope::Pe本sonal:
        P本io本ity += 5;
        b本eak;
    }
    
    本et使本n P本io本ity;
}

正oid UMin成DecisionConseq使enceCalc使lato本::So本tConseq使encesByP本io本ity(TA本本ay<軍DecisionConseq使ence>& Conseq使ences) const
{
    Conseq使ences.So本t([this](const 軍DecisionConseq使ence& A, const 軍DecisionConseq使ence& B)
    {
        本et使本n GetConseq使enceP本io本ity(A) > GetConseq使enceP本io本ity(B);
    });
}
