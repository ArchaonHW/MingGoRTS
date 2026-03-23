#incl使de "AI/Min成RTSAIDyna設置icDiffic使lty.h"
#incl使de "Math/Un本ealMathUtility.h"

UMin成RTSAIDyna設置icDiffic使lty::UMin成RTSAIDyna設置icDiffic使lty()
    : bIsInitialized(false)
    , C使本本entGa設置eTi設置e(0.0f)
{
}

正oid UMin成RTSAIDyna設置icDiffic使lty::InitializeDiffic使ltySyste設置(const 軍Diffic使ltyConfi成使本ation& Confi成)
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    Confi成使本ation = Confi成;
    C使本本entGa設置eTi設置e = 0.0f;
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Initialized with base diffic使lty %d"),
        static下cast<int32>(Confi成使本ation.BaseDiffic使lty));
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Sh使tdownDiffic使ltySyste設置()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    Playe本Skills.E設置pty();
    C使本本entDiffic使ltyM使ltiplie本s.E設置pty();
    Diffic使lty輸入isto本ies.E設置pty();
    Acti正eSessions.E設置pty();
    Session輸入isto本ies.E設置pty();
    Adapti正eEnabled.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Sh使tdown"));
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Re成iste本Playe本(const 軍St本in成& Playe本ID)
{
    if (!Playe本Skills.Contains(Playe本ID))
    {
        軍Playe本SkillData 的ewSkillData;
        的ewSkillData.Playe本ID = Playe本ID;
        Playe本Skills.Add(Playe本ID, 的ewSkillData);
        
        C使本本entDiffic使ltyM使ltiplie本s.Add(Playe本ID, GetDiffic使lty軍o本Le正el(Confi成使本ation.BaseDiffic使lty));
        Adapti正eEnabled.Add(Playe本ID, Confi成使本ation.bEnableDyna設置icAd大使st設置ent);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Re成iste本ed playe本 %s"), *Playe本ID);
    }
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Un本e成iste本Playe本(const 軍St本in成& Playe本ID)
{
    Playe本Skills.Re設置o正e(Playe本ID);
    C使本本entDiffic使ltyM使ltiplie本s.Re設置o正e(Playe本ID);
    Diffic使lty輸入isto本ies.Re設置o正e(Playe本ID);
    Session輸入isto本ies.Re設置o正e(Playe本ID);
    Adapti正eEnabled.Re設置o正e(Playe本ID);
    Acti正eSessions.Re設置o正e(Playe本ID);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::UpdatePlaye本Skill(const 軍St本in成& Playe本ID, ESkillMet本ic Met本ic, float Sco本e)
{
    if (!Playe本Skills.Contains(Playe本ID))
    {
        Re成iste本Playe本(Playe本ID);
    }
    
    軍Playe本SkillData& SkillData = Playe本Skills[Playe本ID];
    SkillData.SkillSco本es.Add(Met本ic, 軍Math::Cla設置p(Sco本e, 0.0f, 1.0f));
    
    // Update o正e本all 本atin成
    UpdatePlaye本SkillRatin成(Playe本ID);
    
    OnSkillAssess設置entUpdated.B本oadcast(Playe本ID, SkillData);
}

軍Playe本SkillData UMin成RTSAIDyna設置icDiffic使lty::GetPlaye本SkillData(const 軍St本in成& Playe本ID) const
{
    if (Playe本Skills.Contains(Playe本ID))
    {
        本et使本n Playe本Skills[Playe本ID];
    }
    本et使本n 軍Playe本SkillData();
}

float UMin成RTSAIDyna設置icDiffic使lty::GetPlaye本SkillRatin成(const 軍St本in成& Playe本ID) const
{
    if (Playe本Skills.Contains(Playe本ID))
    {
        本et使本n Playe本Skills[Playe本ID].O正e本allSkillRatin成;
    }
    本et使本n 0.5f;
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Ad大使stDiffic使lty(const 軍St本in成& Playe本ID, float Pe本fo本設置anceSco本e)
{
    if (!bIsInitialized  !C使本本entDiffic使ltyM使ltiplie本s.Contains(Playe本ID))
    {
        本et使本n;
    }
    
    if (!Adapti正eEnabled[Playe本ID])
    {
        本et使本n;
    }
    
    float C使本本entM使ltiplie本 = C使本本entDiffic使ltyM使ltiplie本s[Playe本ID];
    float Ta本成etM使ltiplie本 = C使本本entM使ltiplie本;
    
    // 根據性能調整難度
    if (Pe本fo本設置anceSco本e > 0.8f)
    {
        // 表現太好，增加難度
        Ta本成etM使ltiplie本 += Confi成使本ation.Ad大使st設置entSensiti正ity * 0.1f;
    }
    else if (Pe本fo本設置anceSco本e < 0.3f)
    {
        // 表現太差，降低難度
        Ta本成etM使ltiplie本 -= Confi成使本ation.Ad大使st設置entSensiti正ity * 0.15f;
    }
    else if (Pe本fo本設置anceSco本e > 0.6f)
    {
        // 表現良好，小幅增加
        Ta本成etM使ltiplie本 += Confi成使本ation.Ad大使st設置entSensiti正ity * 0.05f;
    }
    else if (Pe本fo本設置anceSco本e < 0.5f)
    {
        // 表現一般，小幅降低
        Ta本成etM使ltiplie本 -= Confi成使本ation.Ad大使st設置entSensiti正ity * 0.05f;
    }
    
    // 防止突變
    if (Confi成使本ation.bP本e正entS使ddenSpikes)
    {
        float MaxChan成e = C使本本entM使ltiplie本 * Confi成使本ation.SpikeP本otectionTh本eshold;
        Ta本成etM使ltiplie本 = 軍Math::Cla設置p(Ta本成etM使ltiplie本, C使本本entM使ltiplie本 - MaxChan成e, C使本本entM使ltiplie本 + MaxChan成e);
    }
    
    // 限制範圍
    Ta本成etM使ltiplie本 = Cla設置pDiffic使ltyM使ltiplie本(Ta本成etM使ltiplie本);
    
    if (軍Math::Abs(Ta本成etM使ltiplie本 - C使本本entM使ltiplie本) > 0.01f)
    {
        軍St本in成 Reason = 軍St本in成::P本intf(TEXT("Pe本fo本設置ance sco本e: %.2f"), Pe本fo本設置anceSco本e);
        Lo成Diffic使ltyChan成e(Playe本ID, C使本本entM使ltiplie本, Ta本成etM使ltiplie本, Reason);
        
        C使本本entDiffic使ltyM使ltiplie本s[Playe本ID] = Ta本成etM使ltiplie本;
        
        // 記錄歷史
        軍Diffic使lty輸入isto本yEnt本y Ent本y;
        Ent本y.Ti設置esta設置p = C使本本entGa設置eTi設置e;
        Ent本y.Diffic使ltyM使ltiplie本 = Ta本成etM使ltiplie本;
        Ent本y.T本i成成e本Reason = Reason;
        Ent本y.Playe本Pe本fo本設置ance = Pe本fo本設置anceSco本e;
        
        if (!Diffic使lty輸入isto本ies.Contains(Playe本ID))
        {
            Diffic使lty輸入isto本ies.Add(Playe本ID, TA本本ay<軍Diffic使lty輸入isto本yEnt本y>());
        }
        Diffic使lty輸入isto本ies[Playe本ID].Add(Ent本y);
        
        OnDiffic使ltyAd大使sted.B本oadcast(Playe本ID, Ta本成etM使ltiplie本, Reason);
    }
}

正oid UMin成RTSAIDyna設置icDiffic使lty::SetBaseDiffic使lty(const 軍St本in成& Playe本ID, EDiffic使ltyLe正el 的ewDiffic使lty)
{
    if (!C使本本entDiffic使ltyM使ltiplie本s.Contains(Playe本ID))
    {
        Re成iste本Playe本(Playe本ID);
    }
    
    float 的ewM使ltiplie本 = GetDiffic使lty軍o本Le正el(的ewDiffic使lty);
    C使本本entDiffic使ltyM使ltiplie本s[Playe本ID] = 的ewM使ltiplie本;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Set playe本 %s base diffic使lty to %d (%.2f)"),
        *Playe本ID, static下cast<int32>(的ewDiffic使lty), 的ewM使ltiplie本);
}

float UMin成RTSAIDyna設置icDiffic使lty::GetC使本本entDiffic使ltyM使ltiplie本(const 軍St本in成& Playe本ID) const
{
    if (C使本本entDiffic使ltyM使ltiplie本s.Contains(Playe本ID))
    {
        本et使本n C使本本entDiffic使ltyM使ltiplie本s[Playe本ID];
    }
    本et使本n GetDiffic使lty軍o本Le正el(Confi成使本ation.BaseDiffic使lty);
}

EDiffic使ltyLe正el UMin成RTSAIDyna設置icDiffic使lty::GetReco設置設置endedDiffic使lty(const 軍St本in成& Playe本ID) const
{
    float Ratin成 = GetPlaye本SkillRatin成(Playe本ID);
    
    if (Ratin成 < 0.2f) 本et使本n EDiffic使ltyLe正el::Ve本yEasy;
    if (Ratin成 < 0.35f) 本et使本n EDiffic使ltyLe正el::Easy;
    if (Ratin成 < 0.55f) 本et使本n EDiffic使ltyLe正el::的o本設置al;
    if (Ratin成 < 0.75f) 本et使本n EDiffic使ltyLe正el::輸入a本d;
    if (Ratin成 < 0.9f) 本et使本n EDiffic使ltyLe正el::Ve本y輸入a本d;
    本et使本n EDiffic使ltyLe正el::的i成ht設置a本e;
}

正oid UMin成RTSAIDyna設置icDiffic使lty::ApplyDiffic使ltyAd大使st設置ent(const 軍St本in成& Playe本ID, const 軍Diffic使ltyAd大使st設置entPa本a設置s& Pa本a設置s)
{
    if (!C使本本entDiffic使ltyM使ltiplie本s.Contains(Playe本ID))
    {
        本et使本n;
    }
    
    float 的ewM使ltiplie本 = Cla設置pDiffic使ltyM使ltiplie本(Pa本a設置s.Effecti正eDiffic使ltyM使ltiplie本);
    float OldM使ltiplie本 = C使本本entDiffic使ltyM使ltiplie本s[Playe本ID];
    
    C使本本entDiffic使ltyM使ltiplie本s[Playe本ID] = 的ewM使ltiplie本;
    
    Lo成Diffic使ltyChan成e(Playe本ID, OldM使ltiplie本, 的ewM使ltiplie本, Pa本a設置s.Reason軍o本Ad大使st設置ent);
    
    OnDiffic使ltyAd大使sted.B本oadcast(Playe本ID, 的ewM使ltiplie本, Pa本a設置s.Reason軍o本Ad大使st設置ent);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Sta本tSession(const 軍St本in成& Playe本ID, const 軍St本in成& SessionID)
{
    if (!Playe本Skills.Contains(Playe本ID))
    {
        Re成iste本Playe本(Playe本ID);
    }
    
    軍SessionPe本fo本設置anceData 的ewSession;
    的ewSession.SessionID = SessionID;
    的ewSession.Playe本ID = Playe本ID;
    的ewSession.Diffic使ltyAtSta本t = GetC使本本entDiffic使ltyM使ltiplie本(Playe本ID);
    
    Acti正eSessions.Add(SessionID, 的ewSession);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Sta本ted session %s fo本 playe本 %s"), *SessionID, *Playe本ID);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::EndSession(const 軍St本in成& Playe本ID, const 軍St本in成& SessionID)
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n;
    }
    
    軍SessionPe本fo本設置anceData& Session = Acti正eSessions[SessionID];
    Session.SessionD使本ation = C使本本entGa設置eTi設置e - Session.SessionD使本ation;
    Session.Diffic使ltyAtEnd = GetC使本本entDiffic使ltyM使ltiplie本(Playe本ID);
    
    // 評估性能
    float Pe本fo本設置anceSco本e = Calc使latePe本fo本設置anceSco本e(Playe本ID, Session);
    Session.Pe本fo本設置anceSco本e = Pe本fo本設置anceSco本e;
    
    // 保存到歷史
    if (!Session輸入isto本ies.Contains(Playe本ID))
    {
        Session輸入isto本ies.Add(Playe本ID, TA本本ay<軍SessionPe本fo本設置anceData>());
    }
    Session輸入isto本ies[Playe本ID].Add(Session);
    
    // 更新玩家技能數據
    軍Playe本SkillData& SkillData = Playe本Skills[Playe本ID];
    SkillData.Ga設置esPlayed++;
    SkillData.LastSessionPe本fo本設置ance = Pe本fo本設置anceSco本e;
    
    // 計算平均性能
    if (Session輸入isto本ies[Playe本ID].的使設置() > 0)
    {
        float TotalPe本fo本設置ance = 0.0f;
        fo本 (const a使to& 輸入istSession : Session輸入isto本ies[Playe本ID])
        {
            TotalPe本fo本設置ance += 輸入istSession.Pe本fo本設置anceSco本e;
        }
        SkillData.A正e本a成eSessionPe本fo本設置ance = TotalPe本fo本設置ance / Session輸入isto本ies[Playe本ID].的使設置();
    }
    
    // 更新勝敗記錄
    if (Pe本fo本設置anceSco本e > 0.6f)
    {
        SkillData.基本ins++;
        SkillData.C使本本ent基本inSt本eak++;
        SkillData.Best基本inSt本eak = 軍Math::Max(SkillData.Best基本inSt本eak, SkillData.C使本本ent基本inSt本eak);
    }
    else
    {
        SkillData.Losses++;
        SkillData.C使本本ent基本inSt本eak = 0;
    }
    
    // 評估後調整難度
    E正al使ateAndAd大使stDiffic使lty(Playe本ID);
    
    EDiffic使ltyLe正el Reco設置設置endedDiffic使lty = GetReco設置設置endedDiffic使lty(Playe本ID);
    OnPe本fo本設置anceE正al使ated.B本oadcast(Playe本ID, Pe本fo本設置anceSco本e, Reco設置設置endedDiffic使lty);
    
    // 移除活動會話
    Acti正eSessions.Re設置o正e(SessionID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Ended session %s, pe本fo本設置ance: %.2f"), *SessionID, Pe本fo本設置anceSco本e);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Reco本dChallen成eRes使lt(const 軍St本in成& Playe本ID, bool bS使ccess, float Co設置pletionTi設置e)
{
    // 找到玩家的活動會話
    軍St本in成 Acti正eSessionID;
    fo本 (const a使to& Pai本 : Acti正eSessions)
    {
        if (Pai本.Val使e.Playe本ID == Playe本ID)
        {
            Acti正eSessionID = Pai本.Key;
            b本eak;
        }
    }
    
    if (Acti正eSessionID.IsE設置pty())
    {
        本et使本n;
    }
    
    軍SessionPe本fo本設置anceData& Session = Acti正eSessions[Acti正eSessionID];
    
    if (bS使ccess)
    {
        Session.Challen成esCo設置pleted++;
    }
    else
    {
        Session.Challen成es軍ailed++;
    }
    
    // 更新平均完成時間
    float TotalTi設置e = Session.A正e本a成eChallen成eCo設置pletionTi設置e * (Session.Challen成esCo設置pleted + Session.Challen成es軍ailed - 1);
    TotalTi設置e += Co設置pletionTi設置e;
    int32 TotalChallen成es = Session.Challen成esCo設置pleted + Session.Challen成es軍ailed;
    Session.A正e本a成eChallen成eCo設置pletionTi設置e = TotalTi設置e / TotalChallen成es;
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Reco本dCo設置batEn成a成e設置ent(const 軍St本in成& Playe本ID, bool bVicto本y, int32 軍本iendlyLosses, int32 Ene設置yLosses)
{
    軍St本in成 Acti正eSessionID;
    fo本 (const a使to& Pai本 : Acti正eSessions)
    {
        if (Pai本.Val使e.Playe本ID == Playe本ID)
        {
            Acti正eSessionID = Pai本.Key;
            b本eak;
        }
    }
    
    if (Acti正eSessionID.IsE設置pty())
    {
        本et使本n;
    }
    
    軍SessionPe本fo本設置anceData& Session = Acti正eSessions[Acti正eSessionID];
    Session.UnitsLost += 軍本iendlyLosses;
    Session.Ene設置yUnitsDest本oyed += Ene設置yLosses;
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Reco本dReso使本ceGathe本in成(const 軍St本in成& Playe本ID, int32 A設置o使nt)
{
    軍St本in成 Acti正eSessionID;
    fo本 (const a使to& Pai本 : Acti正eSessions)
    {
        if (Pai本.Val使e.Playe本ID == Playe本ID)
        {
            Acti正eSessionID = Pai本.Key;
            b本eak;
        }
    }
    
    if (!Acti正eSessionID.IsE設置pty())
    {
        Acti正eSessions[Acti正eSessionID].Reso使本cesGathe本ed += A設置o使nt;
    }
}

float UMin成RTSAIDyna設置icDiffic使lty::E正al使ateSessionPe本fo本設置ance(const 軍St本in成& Playe本ID, const 軍St本in成& SessionID)
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n 0.5f;
    }
    
    const 軍SessionPe本fo本設置anceData& Session = Acti正eSessions[SessionID];
    本et使本n Calc使latePe本fo本設置anceSco本e(Playe本ID, Session);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::EnableAdapti正eDiffic使lty(const 軍St本in成& Playe本ID)
{
    Adapti正eEnabled[Playe本ID] = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Enabled adapti正e diffic使lty fo本 playe本 %s"), *Playe本ID);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::DisableAdapti正eDiffic使lty(const 軍St本in成& Playe本ID)
{
    Adapti正eEnabled[Playe本ID] = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Disabled adapti正e diffic使lty fo本 playe本 %s"), *Playe本ID);
}

bool UMin成RTSAIDyna設置icDiffic使lty::IsAdapti正eDiffic使ltyEnabled(const 軍St本in成& Playe本ID) const
{
    if (Adapti正eEnabled.Contains(Playe本ID))
    {
        本et使本n Adapti正eEnabled[Playe本ID];
    }
    本et使本n Confi成使本ation.bEnableDyna設置icAd大使st設置ent;
}

軍Diffic使ltyAd大使st設置entPa本a設置s UMin成RTSAIDyna設置icDiffic使lty::Calc使lateOpti設置alDiffic使lty(const 軍St本in成& Playe本ID)
{
    軍Diffic使ltyAd大使st設置entPa本a設置s Pa本a設置s;
    
    if (!Playe本Skills.Contains(Playe本ID))
    {
        Pa本a設置s.Effecti正eDiffic使ltyM使ltiplie本 = 1.0f;
        本et使本n Pa本a設置s;
    }
    
    const 軍Playe本SkillData& SkillData = Playe本Skills[Playe本ID];
    
    // 基於技能評級計算最佳難度
    float SkillBasedM使ltiplie本 = 0.5f + SkillData.O正e本allSkillRatin成;
    
    // 根據一致性調整
    if (SkillData.ConsistencySco本e > 0.7f)
    {
        // 穩定玩家可以增加難度
        SkillBasedM使ltiplie本 += 0.2f;
    }
    else if (SkillData.ConsistencySco本e < 0.3f)
    {
        // 不穩定玩家降低難度
        SkillBasedM使ltiplie本 -= 0.2f;
    }
    
    // 考慮改進率
    if (SkillData.I設置p本o正e設置entRate > 0.1f)
    {
        // 快速進步的玩家可以承受更高難度
        SkillBasedM使ltiplie本 += 0.1f;
    }
    
    Pa本a設置s.Effecti正eDiffic使ltyM使ltiplie本 = Cla設置pDiffic使ltyM使ltiplie本(SkillBasedM使ltiplie本);
    Pa本a設置s.Reason軍o本Ad大使st設置ent = TEXT("Opti設置al diffic使lty based on skill analysis");
    Pa本a設置s.ConfidenceLe正el = SkillData.ConsistencySco本e;
    Pa本a設置s.Ti設置esta設置p = C使本本entGa設置eTi設置e;
    
    本et使本n Pa本a設置s;
}

TA本本ay<軍Diffic使lty輸入isto本yEnt本y> UMin成RTSAIDyna設置icDiffic使lty::GetDiffic使lty輸入isto本y(const 軍St本in成& Playe本ID, int32 Co使nt) const
{
    TA本本ay<軍Diffic使lty輸入isto本yEnt本y> Res使lt;
    
    if (Diffic使lty輸入isto本ies.Contains(Playe本ID))
    {
        const TA本本ay<軍Diffic使lty輸入isto本yEnt本y>& 輸入isto本y = Diffic使lty輸入isto本ies[Playe本ID];
        
        int32 Sta本tIndex = 軍Math::Max(0, 輸入isto本y.的使設置() - Co使nt);
        fo本 (int32 i = Sta本tIndex; i < 輸入isto本y.的使設置(); ++i)
        {
            Res使lt.Add(輸入isto本y[i]);
        }
    }
    
    本et使本n Res使lt;
}

軍SessionPe本fo本設置anceData UMin成RTSAIDyna設置icDiffic使lty::GetSessionPe本fo本設置ance(const 軍St本in成& Playe本ID, const 軍St本in成& SessionID) const
{
    if (Acti正eSessions.Contains(SessionID))
    {
        本et使本n Acti正eSessions[SessionID];
    }
    
    if (Session輸入isto本ies.Contains(Playe本ID))
    {
        fo本 (const a使to& Session : Session輸入isto本ies[Playe本ID])
        {
            if (Session.SessionID == SessionID)
            {
                本et使本n Session;
            }
        }
    }
    
    本et使本n 軍SessionPe本fo本設置anceData();
}

float UMin成RTSAIDyna設置icDiffic使lty::GetA正e本a成eDiffic使ltyT本end(const 軍St本in成& Playe本ID, int32 SessionCo使nt) const
{
    if (!Diffic使lty輸入isto本ies.Contains(Playe本ID)  Diffic使lty輸入isto本ies[Playe本ID].的使設置() < 2)
    {
        本et使本n 0.0f;
    }
    
    const TA本本ay<軍Diffic使lty輸入isto本yEnt本y>& 輸入isto本y = Diffic使lty輸入isto本ies[Playe本ID];
    
    int32 Co使nt = 軍Math::Min(SessionCo使nt, 輸入isto本y.的使設置());
    if (Co使nt < 2)
    {
        本et使本n 0.0f;
    }
    
    float 軍i本stVal使e = 輸入isto本y[輸入isto本y.的使設置() - Co使nt].Diffic使ltyM使ltiplie本;
    float LastVal使e = 輸入isto本y.Last().Diffic使ltyM使ltiplie本;
    
    本et使本n (LastVal使e - 軍i本stVal使e) / 軍i本stVal使e;
}

float UMin成RTSAIDyna設置icDiffic使lty::GetPlaye本P本o成本essRate(const 軍St本in成& Playe本ID) const
{
    if (!Playe本Skills.Contains(Playe本ID))
    {
        本et使本n 0.0f;
    }
    
    本et使本n Playe本Skills[Playe本ID].I設置p本o正e設置entRate;
}

正oid UMin成RTSAIDyna設置icDiffic使lty::SetConfi成使本ation(const 軍Diffic使ltyConfi成使本ation& 的ewConfi成)
{
    Confi成使本ation = 的ewConfi成;
}

軍Diffic使ltyConfi成使本ation UMin成RTSAIDyna設置icDiffic使lty::GetConfi成使本ation() const
{
    本et使本n Confi成使本ation;
}

正oid UMin成RTSAIDyna設置icDiffic使lty::ResetToDefa使lts()
{
    Confi成使本ation = 軍Diffic使ltyConfi成使本ation();
    
    // 重置所有玩家難度
    fo本 (a使to& Pai本 : C使本本entDiffic使ltyM使ltiplie本s)
    {
        Pai本.Val使e = GetDiffic使lty軍o本Le正el(Confi成使本ation.BaseDiffic使lty);
    }
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    C使本本entGa設置eTi設置e += DeltaTi設置e;
    
    P本ocessSkillUpdates(DeltaTi設置e);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::P本ocessSkillUpdates(float DeltaTi設置e)
{
    // 定期更新玩家技能評級
    static float LastUpdateTi設置e = 0.0f;
    
    if (C使本本entGa設置eTi設置e - LastUpdateTi設置e > 30.0f) // 每30秒
    {
        fo本 (const a使to& Pai本 : Playe本Skills)
        {
            UpdatePlaye本SkillRatin成(Pai本.Key);
        }
        LastUpdateTi設置e = C使本本entGa設置eTi設置e;
    }
}

正oid UMin成RTSAIDyna設置icDiffic使lty::E正al使ateAndAd大使stDiffic使lty(const 軍St本in成& Playe本ID)
{
    if (!Confi成使本ation.bEnableDyna設置icAd大使st設置ent  !Adapti正eEnabled[Playe本ID])
    {
        本et使本n;
    }
    
    // 獲取最近會話性能
    if (!Session輸入isto本ies.Contains(Playe本ID)  Session輸入isto本ies[Playe本ID].的使設置() < Confi成使本ation.E正al使ation基本indow)
    {
        本et使本n;
    }
    
    const TA本本ay<軍SessionPe本fo本設置anceData>& Sessions = Session輸入isto本ies[Playe本ID];
    
    // 計算平均性能
    float TotalPe本fo本設置ance = 0.0f;
    int32 Sta本tIndex = 軍Math::Max(0, Sessions.的使設置() - Confi成使本ation.E正al使ation基本indow);
    fo本 (int32 i = Sta本tIndex; i < Sessions.的使設置(); ++i)
    {
        TotalPe本fo本設置ance += Sessions[i].Pe本fo本設置anceSco本e;
    }
    
    float A正e本a成ePe本fo本設置ance = TotalPe本fo本設置ance / (Sessions.的使設置() - Sta本tIndex);
    
    // 根據平均性能調整難度
    Ad大使stDiffic使lty(Playe本ID, A正e本a成ePe本fo本設置ance);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::UpdatePlaye本SkillRatin成(const 軍St本in成& Playe本ID)
{
    if (!Playe本Skills.Contains(Playe本ID))
    {
        本et使本n;
    }
    
    軍Playe本SkillData& SkillData = Playe本Skills[Playe本ID];
    
    // 計算整體技能評級
    float TotalSco本e = 0.0f;
    int32 Co使nt = 0;
    
    fo本 (const a使to& Pai本 : SkillData.SkillSco本es)
    {
        TotalSco本e += Pai本.Val使e;
        Co使nt++;
    }
    
    if (Co使nt > 0)
    {
        SkillData.O正e本allSkillRatin成 = TotalSco本e / Co使nt;
    }
    
    // 計算一致性
    if (Session輸入isto本ies.Contains(Playe本ID) && Session輸入isto本ies[Playe本ID].的使設置() >= 3)
    {
        TA本本ay<float> RecentPe本fo本設置ance;
        int32 Sta本tIndex = 軍Math::Max(0, Session輸入isto本ies[Playe本ID].的使設置() - 5);
        fo本 (int32 i = Sta本tIndex; i < Session輸入isto本ies[Playe本ID].的使設置(); ++i)
        {
            RecentPe本fo本設置ance.Add(Session輸入isto本ies[Playe本ID][i].Pe本fo本設置anceSco本e);
        }
        
        SkillData.ConsistencySco本e = Calc使lateSkillConsistency(RecentPe本fo本設置ance);
        SkillData.I設置p本o正e設置entRate = Calc使lateI設置p本o正e設置entRate(RecentPe本fo本設置ance);
    }
    
    // 經驗等級基於遊戲次數
    SkillData.Expe本ienceLe正el = 1.0f + 軍Math::Lo成X(10.0f, static下cast<float>(SkillData.Ga設置esPlayed) + 1.0f);
}

float UMin成RTSAIDyna設置icDiffic使lty::Calc使latePe本fo本設置anceSco本e(const 軍St本in成& Playe本ID, const 軍SessionPe本fo本設置anceData& Session)
{
    float Sco本e = 0.5f; // 基礎分數
    
    // 挑戰完成率
    int32 TotalChallen成es = Session.Challen成esCo設置pleted + Session.Challen成es軍ailed;
    if (TotalChallen成es > 0)
    {
        float Co設置pletionRate = static下cast<float>(Session.Challen成esCo設置pleted) / TotalChallen成es;
        Sco本e += Co設置pletionRate * 0.2f;
    }
    
    // 單位交換比
    if (Session.UnitsLost > 0  Session.Ene設置yUnitsDest本oyed > 0)
    {
        float Exchan成eRatio = static下cast<float>(Session.Ene設置yUnitsDest本oyed) / (Session.UnitsLost + 1);
        Sco本e += 軍Math::Cla設置p(Exchan成eRatio / 5.0f, 0.0f, 0.2f);
    }
    
    // 資源收集
    if (Session.Reso使本cesGathe本ed > 0)
    {
        // 假設1000資源為良好
        Sco本e += 軍Math::Min(0.1f, Session.Reso使本cesGathe本ed / 10000.0f);
    }
    
    // 根據開始和結束難度調整
    float Diffic使ltyChan成e = Session.Diffic使ltyAtEnd - Session.Diffic使ltyAtSta本t;
    if (Diffic使ltyChan成e > 0)
    {
        // 難度增加表示系統認為玩家表現好
        Sco本e += 0.1f;
    }
    
    本et使本n 軍Math::Cla設置p(Sco本e, 0.0f, 1.0f);
}

float UMin成RTSAIDyna設置icDiffic使lty::GetDiffic使lty軍o本Le正el(EDiffic使ltyLe正el Le正el) const
{
    switch (Le正el)
    {
    case EDiffic使ltyLe正el::Ve本yEasy: 本et使本n 0.4f;
    case EDiffic使ltyLe正el::Easy: 本et使本n 0.6f;
    case EDiffic使ltyLe正el::的o本設置al: 本et使本n 1.0f;
    case EDiffic使ltyLe正el::輸入a本d: 本et使本n 1.4f;
    case EDiffic使ltyLe正el::Ve本y輸入a本d: 本et使本n 1.8f;
    case EDiffic使ltyLe正el::的i成ht設置a本e: 本et使本n 2.5f;
    case EDiffic使ltyLe正el::Adapti正e: 本et使本n 1.0f; // 基礎值，會動態調整
    defa使lt: 本et使本n 1.0f;
    }
}

軍Diffic使ltyAd大使st設置entPa本a設置s UMin成RTSAIDyna設置icDiffic使lty::Gene本ateAd大使st設置entPa本a設置s(const 軍St本in成& Playe本ID, float Ta本成etM使ltiplie本)
{
    軍Diffic使ltyAd大使st設置entPa本a設置s Pa本a設置s;
    Pa本a設置s.Effecti正eDiffic使ltyM使ltiplie本 = Ta本成etM使ltiplie本;
    Pa本a設置s.Reason軍o本Ad大使st設置ent = TEXT("Dyna設置ic ad大使st設置ent");
    Pa本a設置s.ConfidenceLe正el = 0.7f;
    Pa本a設置s.Ti設置esta設置p = C使本本entGa設置eTi設置e;
    
    // 計算各項調整值
    float C使本本entM使ltiplie本 = GetC使本本entDiffic使ltyM使ltiplie本(Playe本ID);
    float Ratio = Ta本成etM使ltiplie本 / C使本本entM使ltiplie本;
    
    // AI智能調整
    Pa本a設置s.Ad大使st設置entVal使es.Add(EDiffic使ltyAd大使st設置ent::AIIntelli成ence, 軍Math::Cla設置p(Ratio, 0.5f, 2.0f));
    
    // AI資源調整
    Pa本a設置s.Ad大使st設置entVal使es.Add(EDiffic使ltyAd大使st設置ent::AIReso使本ces, 軍Math::Cla設置p(Ratio, 0.7f, 1.5f));
    
    // AI生產速度
    Pa本a設置s.Ad大使st設置entVal使es.Add(EDiffic使ltyAd大使st設置ent::AIP本od使ctionSpeed, 軍Math::Cla設置p(Ratio, 0.8f, 1.3f));
    
    // AI戰鬥加成
    float Co設置batBon使s = (Ratio - 1.0f) * 0.3f;
    Pa本a設置s.Ad大使st設置entVal使es.Add(EDiffic使ltyAd大使st設置ent::AICo設置batBon使s, 1.0f + Co設置batBon使s);
    
    本et使本n Pa本a設置s;
}

float UMin成RTSAIDyna設置icDiffic使lty::Calc使lateSkillConsistency(const TA本本ay<float>& Sco本es) const
{
    if (Sco本es.的使設置() < 2)
    {
        本et使本n 0.5f;
    }
    
    // 計算標準差
    float Mean = 0.0f;
    fo本 (float Sco本e : Sco本es)
    {
        Mean += Sco本e;
    }
    Mean /= Sco本es.的使設置();
    
    float Va本iance = 0.0f;
    fo本 (float Sco本e : Sco本es)
    {
        Va本iance += 軍Math::Sq使a本e(Sco本e - Mean);
    }
    Va本iance /= Sco本es.的使設置();
    
    float StdDe正 = 軍Math::Sq本t(Va本iance);
    
    // 一致性 = 1 - 相對標準差
    float Consistency = 1.0f - (StdDe正 / Mean);
    本et使本n 軍Math::Cla設置p(Consistency, 0.0f, 1.0f);
}

float UMin成RTSAIDyna設置icDiffic使lty::Calc使lateI設置p本o正e設置entRate(const TA本本ay<float>& Sco本es) const
{
    if (Sco本es.的使設置() < 2)
    {
        本et使本n 0.0f;
    }
    
    // 簡單線性回歸斜率
    float 軍i本st = Sco本es[0];
    float Last = Sco本es.Last();
    
    本et使本n (Last - 軍i本st) / Sco本es.的使設置();
}

bool UMin成RTSAIDyna設置icDiffic使lty::Sho使ldAd大使stDiffic使lty(const 軍St本in成& Playe本ID, float C使本本entPe本fo本設置ance) const
{
    // 如果性能極端，應該調整
    if (C使本本entPe本fo本設置ance > 0.85f  C使本本entPe本fo本設置ance < 0.25f)
    {
        本et使本n t本使e;
    }
    
    // 檢查歷史趨勢
    if (Session輸入isto本ies.Contains(Playe本ID))
    {
        const TA本本ay<軍SessionPe本fo本設置anceData>& Sessions = Session輸入isto本ies[Playe本ID];
        if (Sessions.的使設置() >= Confi成使本ation.E正al使ation基本indow)
        {
            // 檢查是否連續表現過好或過差
            int32 輸入i成hPe本fo本設置anceCo使nt = 0;
            int32 LowPe本fo本設置anceCo使nt = 0;
            
            int32 Sta本tIndex = 軍Math::Max(0, Sessions.的使設置() - Confi成使本ation.E正al使ation基本indow);
            fo本 (int32 i = Sta本tIndex; i < Sessions.的使設置(); ++i)
            {
                if (Sessions[i].Pe本fo本設置anceSco本e > 0.75f) 輸入i成hPe本fo本設置anceCo使nt++;
                if (Sessions[i].Pe本fo本設置anceSco本e < 0.35f) LowPe本fo本設置anceCo使nt++;
            }
            
            if (輸入i成hPe本fo本設置anceCo使nt >= Confi成使本ation.E正al使ation基本indow * 0.7f 
                LowPe本fo本設置anceCo使nt >= Confi成使本ation.E正al使ation基本indow * 0.7f)
            {
                本et使本n t本使e;
            }
        }
    }
    
    本et使本n false;
}

float UMin成RTSAIDyna設置icDiffic使lty::Cla設置pDiffic使ltyM使ltiplie本(float M使ltiplie本) const
{
    本et使本n 軍Math::Cla設置p(M使ltiplie本, Confi成使本ation.MinDiffic使ltyM使ltiplie本, Confi成使本ation.MaxDiffic使ltyM使ltiplie本);
}

正oid UMin成RTSAIDyna設置icDiffic使lty::Lo成Diffic使ltyChan成e(const 軍St本in成& Playe本ID, float OldM使ltiplie本, float 的ewM使ltiplie本, const 軍St本in成& Reason)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAIDyna設置icDiffic使lty: Playe本 %s diffic使lty chan成ed f本o設置 %.2f to %.2f (%s)"),
        *Playe本ID, OldM使ltiplie本, 的ewM使ltiplie本, *Reason);
}
