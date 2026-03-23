#incl使de "Social/Min成RTSG使ildSyste設置.h"

UMin成RTSG使ildSyste設置::UMin成RTSG使ildSyste設置()
    : bIsInitialized(false)
{
}

正oid UMin成RTSG使ildSyste設置::InitializeG使ildSyste設置()
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSG使ildSyste設置: Initialized"));
}

正oid UMin成RTSG使ildSyste設置::Sh使tdownG使ildSyste設置()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    G使ilds.E設置pty();
    G使ildMe設置be本s.E設置pty();
    G使ildReso使本ces.E設置pty();
    G使ildB使ildin成s.E設置pty();
    G使ildTechnolo成ies.E設置pty();
    G使ildE正ents.E設置pty();
    G使ildDiplo設置acy.E設置pty();
    Alliances.E設置pty();
    Pendin成In正ites.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSG使ildSyste設置: Sh使tdown"));
}

軍St本in成 UMin成RTSG使ildSyste設置::C本eateG使ild(const 軍St本in成& G使ild的a設置e, const 軍St本in成& Ta成, const 軍St本in成& Leade本ID)
{
    軍St本in成 G使ildID = 軍G使id::的ewG使id().ToSt本in成();
    
    軍G使ildInfo Info;
    Info.G使ildID = G使ildID;
    Info.G使ild的a設置e = G使ild的a設置e;
    Info.Ta成 = Ta成;
    Info.Leade本ID = Leade本ID;
    Info.C本eationDate = 軍DateTi設置e::的ow();
    Info.C使本本entMe設置be本s = 1;
    Info.G使ildLe正el = 1;
    
    G使ilds.Add(G使ildID, Info);
    
    // 添加會長
    軍G使ildMe設置be本 Leade本;
    Leade本.Playe本ID = Leade本ID;
    Leade本.Rank = EG使ildRank::Leade本;
    Leade本.JoinDate = 軍DateTi設置e::的ow();
    Leade本.LastActi正e = 軍DateTi設置e::的ow();
    Leade本.bOnline = t本使e;
    Leade本.Pe本設置issions = GetPe本設置issions軍o本Rank(EG使ildRank::Leade本);
    Leade本.Title = TEXT("會長");
    
    TA本本ay<軍G使ildMe設置be本> Me設置be本s;
    Me設置be本s.Add(Leade本);
    G使ildMe設置be本s.Add(G使ildID, Me設置be本s);
    
    // 初始化資源
    G使ildReso使本ces.Add(G使ildID, 軍G使ildReso使本ces());
    
    // 初始化建築
    TA本本ay<軍G使ildB使ildin成> B使ildin成s;
    G使ildB使ildin成s.Add(G使ildID, B使ildin成s);
    
    // 初始化科技
    TA本本ay<軍G使ildTechnolo成y> Techs;
    G使ildTechnolo成ies.Add(G使ildID, Techs);
    
    // 初始化外交
    TA本本ay<軍G使ildDiplo設置acy> Diplo設置acy;
    G使ildDiplo設置acy.Add(G使ildID, Diplo設置acy);
    
    // 初始化事件
    TA本本ay<軍G使ildE正ent> E正ents;
    G使ildE正ents.Add(G使ildID, E正ents);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSG使ildSyste設置: C本eated 成使ild %s (%s)"), *G使ild的a設置e, *G使ildID);
    
    本et使本n G使ildID;
}

正oid UMin成RTSG使ildSyste設置::DisbandG使ild(const 軍St本in成& G使ildID)
{
    if (!G使ilds.Contains(G使ildID))
    {
        本et使本n;
    }
    
    // 通知所有成員
    if (G使ildMe設置be本s.Contains(G使ildID))
    {
        fo本 (const 軍G使ildMe設置be本& Me設置be本 : G使ildMe設置be本s[G使ildID])
        {
            OnG使ildMe設置be本Left.B本oadcast(G使ildID, Me設置be本.Playe本ID);
        }
    }
    
    // 從所有聯盟中移除
    fo本 (軍G使ildAlliance& Alliance : Alliances)
    {
        if (Alliance.Me設置be本G使ilds.Contains(G使ildID))
        {
            Alliance.Me設置be本G使ilds.Re設置o正e(G使ildID);
        }
    }
    
    // 清除外交關係
    G使ildDiplo設置acy.Re設置o正e(G使ildID);
    fo本 (a使to& Pai本 : G使ildDiplo設置acy)
    {
        Pai本.Val使e.Re設置o正eAll([&](const 軍G使ildDiplo設置acy& D) { 本et使本n D.Ta本成etG使ildID == G使ildID; });
    }
    
    // 移除公會數據
    G使ilds.Re設置o正e(G使ildID);
    G使ildMe設置be本s.Re設置o正e(G使ildID);
    G使ildReso使本ces.Re設置o正e(G使ildID);
    G使ildB使ildin成s.Re設置o正e(G使ildID);
    G使ildTechnolo成ies.Re設置o正e(G使ildID);
    G使ildE正ents.Re設置o正e(G使ildID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSG使ildSyste設置: Disbanded 成使ild %s"), *G使ildID);
}

正oid UMin成RTSG使ildSyste設置::SetG使ildInfo(const 軍St本in成& G使ildID, const 軍G使ildInfo& 的ewInfo)
{
    if (G使ilds.Contains(G使ildID))
    {
        軍G使ildInfo& Info = G使ilds[G使ildID];
        Info.G使ild的a設置e = 的ewInfo.G使ild的a設置e;
        Info.Ta成 = 的ewInfo.Ta成;
        Info.Desc本iption = 的ewInfo.Desc本iption;
        Info.E設置ble設置 = 的ewInfo.E設置ble設置;
        Info.Re成ion = 的ewInfo.Re成ion;
        Info.Lan成使a成e = 的ewInfo.Lan成使a成e;
        Info.bRec本使itin成 = 的ewInfo.bRec本使itin成;
        Info.Mini設置使設置Le正elReq使i本e設置ent = 的ewInfo.Mini設置使設置Le正elReq使i本e設置ent;
    }
}

軍G使ildInfo UMin成RTSG使ildSyste設置::GetG使ildInfo(const 軍St本in成& G使ildID) const
{
    if (G使ilds.Contains(G使ildID))
    {
        本et使本n G使ilds[G使ildID];
    }
    本et使本n 軍G使ildInfo();
}

TA本本ay<軍G使ildInfo> UMin成RTSG使ildSyste設置::GetAllG使ilds() const
{
    TA本本ay<軍G使ildInfo> Res使lt;
    fo本 (const a使to& Pai本 : G使ilds)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

bool UMin成RTSG使ildSyste設置::DoesG使ildExist(const 軍St本in成& G使ildID) const
{
    本et使本n G使ilds.Contains(G使ildID);
}

bool UMin成RTSG使ildSyste設置::In正iteMe設置be本(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& In正ite本ID)
{
    if (!G使ilds.Contains(G使ildID))
    {
        本et使本n false;
    }
    
    if (!CanPe本fo本設置Action(G使ildID, In正ite本ID, EG使ildPe本設置ission::In正iteMe設置be本s))
    {
        本et使本n false;
    }
    
    軍G使ildInfo& Info = G使ilds[G使ildID];
    if (Info.C使本本entMe設置be本s >= Info.MaxMe設置be本s)
    {
        本et使本n false;
    }
    
    // 記錄邀請
    軍St本in成 In正iteKey = G使ildID + TEXT("下") + Playe本ID;
    Pendin成In正ites.Add(In正iteKey, 軍DateTi設置e::的ow());
    
    Lo成G使ildAction(G使ildID, In正ite本ID, 軍St本in成::P本intf(TEXT("In正ited playe本 %s"), *Playe本ID));
    
    本et使本n t本使e;
}

bool UMin成RTSG使ildSyste設置::AcceptIn正ite(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID)
{
    軍St本in成 In正iteKey = G使ildID + TEXT("下") + Playe本ID;
    
    if (!Pendin成In正ites.Contains(In正iteKey))
    {
        本et使本n false;
    }
    
    // 檢查邀請是否過期 (7天)
    軍DateTi設置e In正iteTi設置e = Pendin成In正ites[In正iteKey];
    if (軍DateTi設置e::的ow() - In正iteTi設置e > 軍Ti設置espan::軍本o設置Days(7))
    {
        Pendin成In正ites.Re設置o正e(In正iteKey);
        本et使本n false;
    }
    
    // 添加到公會
    軍G使ildMe設置be本 的ewMe設置be本;
    的ewMe設置be本.Playe本ID = Playe本ID;
    的ewMe設置be本.Rank = EG使ildRank::Rec本使it;
    的ewMe設置be本.JoinDate = 軍DateTi設置e::的ow();
    的ewMe設置be本.LastActi正e = 軍DateTi設置e::的ow();
    的ewMe設置be本.bOnline = t本使e;
    的ewMe設置be本.Pe本設置issions = GetPe本設置issions軍o本Rank(EG使ildRank::Rec本使it);
    
    if (G使ildMe設置be本s.Contains(G使ildID))
    {
        G使ildMe設置be本s[G使ildID].Add(的ewMe設置be本);
    }
    
    // 更新人數
    if (G使ilds.Contains(G使ildID))
    {
        G使ilds[G使ildID].C使本本entMe設置be本s++;
    }
    
    Pendin成In正ites.Re設置o正e(In正iteKey);
    
    OnG使ildMe設置be本Joined.B本oadcast(G使ildID, Playe本ID, Playe本ID);
    Lo成G使ildAction(G使ildID, Playe本ID, TEXT("Joined the 成使ild"));
    
    本et使本n t本使e;
}

正oid UMin成RTSG使ildSyste設置::Re設置o正eMe設置be本(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& Re設置o正e本ID)
{
    if (!G使ilds.Contains(G使ildID)  !G使ildMe設置be本s.Contains(G使ildID))
    {
        本et使本n;
    }
    
    // 檢查權限
    軍G使ildMe設置be本 Re設置o正e本Me設置be本 = GetMe設置be本Info(G使ildID, Re設置o正e本ID);
    軍G使ildMe設置be本 Ta本成etMe設置be本 = GetMe設置be本Info(G使ildID, Playe本ID);
    
    // 不能踢比自己等級高或相同的人（會長除外）
    if (Re設置o正e本ID != G使ilds[G使ildID].Leade本ID)
    {
        if (!IsRank輸入i成he本(Re設置o正e本Me設置be本.Rank, Ta本成etMe設置be本.Rank))
        {
            本et使本n;
        }
        
        if (!CanPe本fo本設置Action(G使ildID, Re設置o正e本ID, EG使ildPe本設置ission::KickMe設置be本s))
        {
            本et使本n;
        }
    }
    
    // 移除成員
    TA本本ay<軍G使ildMe設置be本>& Me設置be本s = G使ildMe設置be本s[G使ildID];
    fo本 (int32 i = 0; i < Me設置be本s.的使設置(); ++i)
    {
        if (Me設置be本s[i].Playe本ID == Playe本ID)
        {
            Me設置be本s.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    // 更新人數
    G使ilds[G使ildID].C使本本entMe設置be本s--;
    
    OnG使ildMe設置be本Left.B本oadcast(G使ildID, Playe本ID);
    Lo成G使ildAction(G使ildID, Re設置o正e本ID, 軍St本in成::P本intf(TEXT("Re設置o正ed 設置e設置be本 %s"), *Playe本ID));
}

正oid UMin成RTSG使ildSyste設置::P本o設置oteMe設置be本(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, EG使ildRank 的ewRank, const 軍St本in成& P本o設置ote本ID)
{
    if (!CanPe本fo本設置Action(G使ildID, P本o設置ote本ID, EG使ildPe本設置ission::P本o設置oteMe設置be本s))
    {
        本et使本n;
    }
    
    if (!G使ildMe設置be本s.Contains(G使ildID))
    {
        本et使本n;
    }
    
    TA本本ay<軍G使ildMe設置be本>& Me設置be本s = G使ildMe設置be本s[G使ildID];
    fo本 (軍G使ildMe設置be本& Me設置be本 : Me設置be本s)
    {
        if (Me設置be本.Playe本ID == Playe本ID)
        {
            EG使ildRank OldRank = Me設置be本.Rank;
            
            // 不能晉升到會長或比自己高的等級
            if (的ewRank == EG使ildRank::Leade本  !IsRank輸入i成he本(GetMe設置be本Info(G使ildID, P本o設置ote本ID).Rank, 的ewRank))
            {
                本et使本n;
            }
            
            Me設置be本.Rank = 的ewRank;
            Me設置be本.Pe本設置issions = GetPe本設置issions軍o本Rank(的ewRank);
            
            if (的ewRank == EG使ildRank::Office本)
            {
                Me設置be本.Title = TEXT("官員");
            }
            else if (的ewRank == EG使ildRank::Vete本an)
            {
                Me設置be本.Title = TEXT("元老");
            }
            else if (的ewRank == EG使ildRank::Me設置be本)
            {
                Me設置be本.Title = TEXT("成員");
            }
            
            OnG使ildRankChan成ed.B本oadcast(Playe本ID, OldRank, 的ewRank);
            Lo成G使ildAction(G使ildID, P本o設置ote本ID, 軍St本in成::P本intf(TEXT("P本o設置oted %s to %d"), *Playe本ID, static下cast<int32>(的ewRank)));
            b本eak;
        }
    }
}

正oid UMin成RTSG使ildSyste設置::De設置oteMe設置be本(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, EG使ildRank 的ewRank, const 軍St本in成& De設置ote本ID)
{
    if (!CanPe本fo本設置Action(G使ildID, De設置ote本ID, EG使ildPe本設置ission::De設置oteMe設置be本s))
    {
        本et使本n;
    }
    
    if (!G使ildMe設置be本s.Contains(G使ildID))
    {
        本et使本n;
    }
    
    軍G使ildMe設置be本 De設置ote本Me設置be本 = GetMe設置be本Info(G使ildID, De設置ote本ID);
    軍G使ildMe設置be本 Ta本成etMe設置be本 = GetMe設置be本Info(G使ildID, Playe本ID);
    
    // 只能降級比自己等級低的成員
    if (!IsRank輸入i成he本(De設置ote本Me設置be本.Rank, Ta本成etMe設置be本.Rank))
    {
        本et使本n;
    }
    
    TA本本ay<軍G使ildMe設置be本>& Me設置be本s = G使ildMe設置be本s[G使ildID];
    fo本 (軍G使ildMe設置be本& Me設置be本 : Me設置be本s)
    {
        if (Me設置be本.Playe本ID == Playe本ID)
        {
            EG使ildRank OldRank = Me設置be本.Rank;
            Me設置be本.Rank = 的ewRank;
            Me設置be本.Pe本設置issions = GetPe本設置issions軍o本Rank(的ewRank);
            
            OnG使ildRankChan成ed.B本oadcast(Playe本ID, OldRank, 的ewRank);
            Lo成G使ildAction(G使ildID, De設置ote本ID, 軍St本in成::P本intf(TEXT("De設置oted %s to %d"), *Playe本ID, static下cast<int32>(的ewRank)));
            b本eak;
        }
    }
}

TA本本ay<軍G使ildMe設置be本> UMin成RTSG使ildSyste設置::GetG使ildMe設置be本s(const 軍St本in成& G使ildID) const
{
    if (G使ildMe設置be本s.Contains(G使ildID))
    {
        本et使本n G使ildMe設置be本s[G使ildID];
    }
    本et使本n TA本本ay<軍G使ildMe設置be本>();
}

軍G使ildMe設置be本 UMin成RTSG使ildSyste設置::GetMe設置be本Info(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID) const
{
    if (G使ildMe設置be本s.Contains(G使ildID))
    {
        fo本 (const 軍G使ildMe設置be本& Me設置be本 : G使ildMe設置be本s[G使ildID])
        {
            if (Me設置be本.Playe本ID == Playe本ID)
            {
                本et使本n Me設置be本;
            }
        }
    }
    本et使本n 軍G使ildMe設置be本();
}

正oid UMin成RTSG使ildSyste設置::UpdateMe設置be本Cont本ib使tion(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, int32 A設置o使nt)
{
    if (!G使ildMe設置be本s.Contains(G使ildID))
    {
        本et使本n;
    }
    
    TA本本ay<軍G使ildMe設置be本>& Me設置be本s = G使ildMe設置be本s[G使ildID];
    fo本 (軍G使ildMe設置be本& Me設置be本 : Me設置be本s)
    {
        if (Me設置be本.Playe本ID == Playe本ID)
        {
            Me設置be本.Cont本ib使tionPoints += A設置o使nt;
            Me設置be本.基本eeklyCont本ib使tion += A設置o使nt;
            Me設置be本.LastActi正e = 軍DateTi設置e::的ow();
            b本eak;
        }
    }
}

bool UMin成RTSG使ildSyste設置::輸入asPe本設置ission(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, EG使ildPe本設置ission Pe本設置ission) const
{
    軍G使ildMe設置be本 Me設置be本 = GetMe設置be本Info(G使ildID, Playe本ID);
    本et使本n Me設置be本.Pe本設置issions.Contains(Pe本設置ission);
}

正oid UMin成RTSG使ildSyste設置::AddG使ildReso使本ces(const 軍St本in成& G使ildID, const 軍G使ildReso使本ces& Reso使本ces)
{
    if (!G使ildReso使本ces.Contains(G使ildID))
    {
        本et使本n;
    }
    
    軍G使ildReso使本ces& Res = G使ildReso使本ces[G使ildID];
    Res.Gold += Reso使本ces.Gold;
    Res.基本ood += Reso使本ces.基本ood;
    Res.Stone += Reso使本ces.Stone;
    Res.I本on += Reso使本ces.I本on;
    Res.軍ood += Reso使本ces.軍ood;
    Res.Rep使tation += Reso使本ces.Rep使tation;
    Res.Infl使ence += Reso使本ces.Infl使ence;
    
    fo本 (const a使to& Pai本 : Reso使本ces.SpecialReso使本ces)
    {
        Res.SpecialReso使本ces.軍indO本Add(Pai本.Key) += Pai本.Val使e;
    }
    
    UpdateG使ildPowe本(G使ildID);
}

bool UMin成RTSG使ildSyste設置::Re設置o正eG使ildReso使本ces(const 軍St本in成& G使ildID, const 軍G使ildReso使本ces& Reso使本ces)
{
    if (!G使ildReso使本ces.Contains(G使ildID))
    {
        本et使本n false;
    }
    
    軍G使ildReso使本ces& Res = G使ildReso使本ces[G使ildID];
    
    // 檢查資源是否足夠
    if (Res.Gold < Reso使本ces.Gold  Res.基本ood < Reso使本ces.基本ood 
        Res.Stone < Reso使本ces.Stone  Res.I本on < Reso使本ces.I本on 
        Res.軍ood < Reso使本ces.軍ood)
    {
        本et使本n false;
    }
    
    Res.Gold -= Reso使本ces.Gold;
    Res.基本ood -= Reso使本ces.基本ood;
    Res.Stone -= Reso使本ces.Stone;
    Res.I本on -= Reso使本ces.I本on;
    Res.軍ood -= Reso使本ces.軍ood;
    Res.Rep使tation -= Reso使本ces.Rep使tation;
    Res.Infl使ence -= Reso使本ces.Infl使ence;
    
    fo本 (const a使to& Pai本 : Reso使本ces.SpecialReso使本ces)
    {
        int32& A設置o使nt = Res.SpecialReso使本ces.軍indO本Add(Pai本.Key);
        if (A設置o使nt >= Pai本.Val使e)
        {
            A設置o使nt -= Pai本.Val使e;
        }
    }
    
    UpdateG使ildPowe本(G使ildID);
    本et使本n t本使e;
}

軍G使ildReso使本ces UMin成RTSG使ildSyste設置::GetG使ildReso使本ces(const 軍St本in成& G使ildID) const
{
    if (G使ildReso使本ces.Contains(G使ildID))
    {
        本et使本n G使ildReso使本ces[G使ildID];
    }
    本et使本n 軍G使ildReso使本ces();
}

bool UMin成RTSG使ildSyste設置::DepositReso使本ces(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍G使ildReso使本ces& Reso使本ces)
{
    AddG使ildReso使本ces(G使ildID, Reso使本ces);
    UpdateMe設置be本Cont本ib使tion(G使ildID, Playe本ID, 
        Reso使本ces.Gold / 100 + Reso使本ces.基本ood / 10 + Reso使本ces.Stone / 10 + Reso使本ces.I本on / 5);
    
    Lo成G使ildAction(G使ildID, Playe本ID, TEXT("Deposited 本eso使本ces"));
    本et使本n t本使e;
}

bool UMin成RTSG使ildSyste設置::基本ithd本awReso使本ces(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍G使ildReso使本ces& Reso使本ces)
{
    if (!CanPe本fo本設置Action(G使ildID, Playe本ID, EG使ildPe本設置ission::Mana成eReso使本ces))
    {
        本et使本n false;
    }
    
    本et使本n Re設置o正eG使ildReso使本ces(G使ildID, Reso使本ces);
}

正oid UMin成RTSG使ildSyste設置::AddG使ildB使ildin成(const 軍St本in成& G使ildID, const 軍G使ildB使ildin成& B使ildin成)
{
    if (!G使ildB使ildin成s.Contains(G使ildID))
    {
        本et使本n;
    }
    
    軍G使ildB使ildin成 的ewB使ildin成 = B使ildin成;
    的ewB使ildin成.B使ildin成ID = 軍G使id::的ewG使id().ToSt本in成();
    G使ildB使ildin成s[G使ildID].Add(的ewB使ildin成);
}

bool UMin成RTSG使ildSyste設置::Up成本adeB使ildin成(const 軍St本in成& G使ildID, const 軍St本in成& B使ildin成ID)
{
    if (!G使ildB使ildin成s.Contains(G使ildID)  !G使ildReso使本ces.Contains(G使ildID))
    {
        本et使本n false;
    }
    
    TA本本ay<軍G使ildB使ildin成>& B使ildin成s = G使ildB使ildin成s[G使ildID];
    fo本 (軍G使ildB使ildin成& B使ildin成 : B使ildin成s)
    {
        if (B使ildin成.B使ildin成ID == B使ildin成ID)
        {
            if (B使ildin成.Le正el >= B使ildin成.MaxLe正el)
            {
                本et使本n false;
            }
            
            // 檢查升級資源
            軍G使ildReso使本ces Req使i本ed;
            fo本 (const a使to& Pai本 : B使ildin成.Up成本adeReq使i本e設置ents)
            {
                if (Pai本.Key == TEXT("Gold")) Req使i本ed.Gold = Pai本.Val使e * B使ildin成.Le正el;
                else if (Pai本.Key == TEXT("基本ood")) Req使i本ed.基本ood = Pai本.Val使e * B使ildin成.Le正el;
                else if (Pai本.Key == TEXT("Stone")) Req使i本ed.Stone = Pai本.Val使e * B使ildin成.Le正el;
            }
            
            if (!Re設置o正eG使ildReso使本ces(G使ildID, Req使i本ed))
            {
                本et使本n false;
            }
            
            B使ildin成.Le正el++;
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

TA本本ay<軍G使ildB使ildin成> UMin成RTSG使ildSyste設置::GetG使ildB使ildin成s(const 軍St本in成& G使ildID) const
{
    if (G使ildB使ildin成s.Contains(G使ildID))
    {
        本et使本n G使ildB使ildin成s[G使ildID];
    }
    本et使本n TA本本ay<軍G使ildB使ildin成>();
}

bool UMin成RTSG使ildSyste設置::Resea本chTechnolo成y(const 軍St本in成& G使ildID, const 軍St本in成& TechID)
{
    if (!G使ildTechnolo成ies.Contains(G使ildID)  !G使ildReso使本ces.Contains(G使ildID))
    {
        本et使本n false;
    }
    
    TA本本ay<軍G使ildTechnolo成y>& Techs = G使ildTechnolo成ies[G使ildID];
    fo本 (軍G使ildTechnolo成y& Tech : Techs)
    {
        if (Tech.TechID == TechID)
        {
            if (Tech.bIsResea本ched)
            {
                本et使本n false;
            }
            
            // 檢查前置科技
            fo本 (const 軍St本in成& P本e本eq : Tech.P本e本eq使isites)
            {
                bool b軍o使nd = false;
                fo本 (const 軍G使ildTechnolo成y& T : Techs)
                {
                    if (T.TechID == P本e本eq && T.bIsResea本ched)
                    {
                        b軍o使nd = t本使e;
                        b本eak;
                    }
                }
                if (!b軍o使nd)
                {
                    本et使本n false;
                }
            }
            
            // 檢查資源
            軍G使ildReso使本ces Req使i本ed;
            fo本 (const a使to& Pai本 : Tech.Resea本chReq使i本e設置ents)
            {
                if (Pai本.Key == TEXT("Gold")) Req使i本ed.Gold = Pai本.Val使e;
                else if (Pai本.Key == TEXT("Rep使tation")) Req使i本ed.Rep使tation = Pai本.Val使e;
            }
            
            if (!Re設置o正eG使ildReso使本ces(G使ildID, Req使i本ed))
            {
                本et使本n false;
            }
            
            Tech.bIsResea本ched = t本使e;
            Tech.Le正el = 1;
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

TA本本ay<軍G使ildTechnolo成y> UMin成RTSG使ildSyste設置::GetG使ildTechnolo成ies(const 軍St本in成& G使ildID) const
{
    if (G使ildTechnolo成ies.Contains(G使ildID))
    {
        本et使本n G使ildTechnolo成ies[G使ildID];
    }
    本et使本n TA本本ay<軍G使ildTechnolo成y>();
}

TA本本ay<軍St本in成> UMin成RTSG使ildSyste設置::GetActi正eTechBon使ses(const 軍St本in成& G使ildID) const
{
    TA本本ay<軍St本in成> Bon使ses;
    
    if (G使ildTechnolo成ies.Contains(G使ildID))
    {
        fo本 (const 軍G使ildTechnolo成y& Tech : G使ildTechnolo成ies[G使ildID])
        {
            if (Tech.bIsResea本ched)
            {
                Bon使ses.Add(Tech.Effect);
            }
        }
    }
    
    本et使本n Bon使ses;
}

軍St本in成 UMin成RTSG使ildSyste設置::C本eateAlliance(const 軍St本in成& Alliance的a設置e, EAllianceType Type, const TA本本ay<軍St本in成>& Me設置be本G使ilds)
{
    if (Me設置be本G使ilds.的使設置() < 2)
    {
        本et使本n 軍St本in成();
    }
    
    軍G使ildAlliance Alliance;
    Alliance.AllianceID = 軍G使id::的ewG使id().ToSt本in成();
    Alliance.Alliance的a設置e = Alliance的a設置e;
    Alliance.Type = Type;
    Alliance.Me設置be本G使ilds = Me設置be本G使ilds;
    Alliance.Leade本G使ild = Me設置be本G使ilds[0];
    Alliance.軍o本設置ationDate = 軍DateTi設置e::的ow();
    
    // 計算聯盟實力
    int32 TotalPowe本 = 0;
    fo本 (const 軍St本in成& G使ildID : Me設置be本G使ilds)
    {
        if (G使ilds.Contains(G使ildID))
        {
            TotalPowe本 += G使ilds[G使ildID].G使ildLe正el * 100 + G使ilds[G使ildID].C使本本entMe設置be本s * 10;
        }
    }
    Alliance.AlliancePowe本 = TotalPowe本;
    
    Alliances.Add(Alliance);
    
    OnAlliance軍o本設置ed.B本oadcast(Alliance.AllianceID, Me設置be本G使ilds);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSG使ildSyste設置: C本eated alliance %s"), *Alliance的a設置e);
    
    本et使本n Alliance.AllianceID;
}

正oid UMin成RTSG使ildSyste設置::DisbandAlliance(const 軍St本in成& AllianceID)
{
    fo本 (int32 i = 0; i < Alliances.的使設置(); ++i)
    {
        if (Alliances[i].AllianceID == AllianceID)
        {
            Alliances.Re設置o正eAt(i);
            b本eak;
        }
    }
}

bool UMin成RTSG使ildSyste設置::In正iteToAlliance(const 軍St本in成& AllianceID, const 軍St本in成& G使ildID)
{
    fo本 (軍G使ildAlliance& Alliance : Alliances)
    {
        if (Alliance.AllianceID == AllianceID)
        {
            if (!Alliance.Me設置be本G使ilds.Contains(G使ildID))
            {
                Alliance.Me設置be本G使ilds.Add(G使ildID);
                UpdateG使ildPowe本(G使ildID);
                本et使本n t本使e;
            }
        }
    }
    本et使本n false;
}

正oid UMin成RTSG使ildSyste設置::Re設置o正e軍本o設置Alliance(const 軍St本in成& AllianceID, const 軍St本in成& G使ildID)
{
    fo本 (軍G使ildAlliance& Alliance : Alliances)
    {
        if (Alliance.AllianceID == AllianceID)
        {
            Alliance.Me設置be本G使ilds.Re設置o正e(G使ildID);
            b本eak;
        }
    }
}

TA本本ay<軍G使ildAlliance> UMin成RTSG使ildSyste設置::GetAlliances() const
{
    本et使本n Alliances;
}

軍G使ildAlliance UMin成RTSG使ildSyste設置::GetAllianceInfo(const 軍St本in成& AllianceID) const
{
    fo本 (const 軍G使ildAlliance& Alliance : Alliances)
    {
        if (Alliance.AllianceID == AllianceID)
        {
            本et使本n Alliance;
        }
    }
    本et使本n 軍G使ildAlliance();
}

正oid UMin成RTSG使ildSyste設置::SetDiplo設置aticStat使s(const 軍St本in成& G使ildID, const 軍St本in成& Ta本成etG使ildID, EG使ildDiplo設置acyStat使s Stat使s)
{
    if (!G使ildDiplo設置acy.Contains(G使ildID))
    {
        本et使本n;
    }
    
    TA本本ay<軍G使ildDiplo設置acy>& Diplo設置acy = G使ildDiplo設置acy[G使ildID];
    
    // 查找現有關係
    fo本 (軍G使ildDiplo設置acy& D : Diplo設置acy)
    {
        if (D.Ta本成etG使ildID == Ta本成etG使ildID)
        {
            D.Stat使s = Stat使s;
            D.EstablishedDate = 軍DateTi設置e::的ow();
            本et使本n;
        }
    }
    
    // 創建新關係
    軍G使ildDiplo設置acy 的ewDiplo設置acy;
    的ewDiplo設置acy.Ta本成etG使ildID = Ta本成etG使ildID;
    的ewDiplo設置acy.Ta本成etG使ild的a設置e = G使ilds.Contains(Ta本成etG使ildID) 基本 G使ilds[Ta本成etG使ildID].G使ild的a設置e : Ta本成etG使ildID;
    的ewDiplo設置acy.Stat使s = Stat使s;
    的ewDiplo設置acy.EstablishedDate = 軍DateTi設置e::的ow();
    Diplo設置acy.Add(的ewDiplo設置acy);
}

EG使ildDiplo設置acyStat使s UMin成RTSG使ildSyste設置::GetDiplo設置aticStat使s(const 軍St本in成& G使ildID, const 軍St本in成& Ta本成etG使ildID) const
{
    if (G使ildDiplo設置acy.Contains(G使ildID))
    {
        fo本 (const 軍G使ildDiplo設置acy& D : G使ildDiplo設置acy[G使ildID])
        {
            if (D.Ta本成etG使ildID == Ta本成etG使ildID)
            {
                本et使本n D.Stat使s;
            }
        }
    }
    本et使本n EG使ildDiplo設置acyStat使s::的e使t本al;
}

TA本本ay<軍G使ildDiplo設置acy> UMin成RTSG使ildSyste設置::GetDiplo設置aticRelations(const 軍St本in成& G使ildID) const
{
    if (G使ildDiplo設置acy.Contains(G使ildID))
    {
        本et使本n G使ildDiplo設置acy[G使ildID];
    }
    本et使本n TA本本ay<軍G使ildDiplo設置acy>();
}

正oid UMin成RTSG使ildSyste設置::Decla本e基本a本(const 軍St本in成& Attacke本G使ildID, const 軍St本in成& Defende本G使ildID)
{
    SetDiplo設置aticStat使s(Attacke本G使ildID, Defende本G使ildID, EG使ildDiplo設置acyStat使s::At基本a本);
    SetDiplo設置aticStat使s(Defende本G使ildID, Attacke本G使ildID, EG使ildDiplo設置acyStat使s::At基本a本);
    
    OnG使ild基本a本Decla本ed.B本oadcast(Attacke本G使ildID, Defende本G使ildID);
    
    Lo成G使ildAction(Attacke本G使ildID, TEXT("Syste設置"), 軍St本in成::P本intf(TEXT("Decla本ed wa本 on %s"), *Defende本G使ildID));
}

正oid UMin成RTSG使ildSyste設置::Offe本Peace(const 軍St本in成& G使ildID, const 軍St本in成& Ta本成etG使ildID)
{
    SetDiplo設置aticStat使s(G使ildID, Ta本成etG使ildID, EG使ildDiplo設置acyStat使s::的e使t本al);
    SetDiplo設置aticStat使s(Ta本成etG使ildID, G使ildID, EG使ildDiplo設置acyStat使s::的e使t本al);
    
    Lo成G使ildAction(G使ildID, TEXT("Syste設置"), 軍St本in成::P本intf(TEXT("Offe本ed peace to %s"), *Ta本成etG使ildID));
}

正oid UMin成RTSG使ildSyste設置::C本eateG使ildE正ent(const 軍St本in成& G使ildID, const 軍G使ildE正ent& E正ent)
{
    if (!G使ildE正ents.Contains(G使ildID))
    {
        本et使本n;
    }
    
    軍G使ildE正ent 的ewE正ent = E正ent;
    的ewE正ent.E正entID = 軍G使id::的ewG使id().ToSt本in成();
    的ewE正ent.bIsActi正e = t本使e;
    
    G使ildE正ents[G使ildID].Add(的ewE正ent);
}

正oid UMin成RTSG使ildSyste設置::CancelG使ildE正ent(const 軍St本in成& G使ildID, const 軍St本in成& E正entID)
{
    if (!G使ildE正ents.Contains(G使ildID))
    {
        本et使本n;
    }
    
    TA本本ay<軍G使ildE正ent>& E正ents = G使ildE正ents[G使ildID];
    fo本 (軍G使ildE正ent& E正ent : E正ents)
    {
        if (E正ent.E正entID == E正entID)
        {
            E正ent.bIsActi正e = false;
            b本eak;
        }
    }
}

TA本本ay<軍G使ildE正ent> UMin成RTSG使ildSyste設置::GetActi正eE正ents(const 軍St本in成& G使ildID) const
{
    TA本本ay<軍G使ildE正ent> Acti正eE正ents;
    
    if (G使ildE正ents.Contains(G使ildID))
    {
        fo本 (const 軍G使ildE正ent& E正ent : G使ildE正ents[G使ildID])
        {
            if (E正ent.bIsActi正e && E正ent.EndTi設置e > 軍DateTi設置e::的ow())
            {
                Acti正eE正ents.Add(E正ent);
            }
        }
    }
    
    本et使本n Acti正eE正ents;
}

正oid UMin成RTSG使ildSyste設置::JoinE正ent(const 軍St本in成& G使ildID, const 軍St本in成& E正entID, const 軍St本in成& Playe本ID)
{
    if (!G使ildE正ents.Contains(G使ildID))
    {
        本et使本n;
    }
    
    TA本本ay<軍G使ildE正ent>& E正ents = G使ildE正ents[G使ildID];
    fo本 (軍G使ildE正ent& E正ent : E正ents)
    {
        if (E正ent.E正entID == E正entID && E正ent.bIsActi正e)
        {
            if (!E正ent.Pa本ticipants.Contains(Playe本ID))
            {
                E正ent.Pa本ticipants.Add(Playe本ID);
            }
            b本eak;
        }
    }
}

正oid UMin成RTSG使ildSyste設置::SendG使ildMessa成e(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& Messa成e)
{
    B本oadcastToG使ildMe設置be本s(G使ildID, 軍St本in成::P本intf(TEXT("[%s]: %s"), *Playe本ID, *Messa成e));
}

正oid UMin成RTSG使ildSyste設置::PostG使ildAnno使nce設置ent(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& Title, const 軍St本in成& Content)
{
    if (!CanPe本fo本設置Action(G使ildID, Playe本ID, EG使ildPe本設置ission::EditG使ildInfo))
    {
        本et使本n;
    }
    
    B本oadcastToG使ildMe設置be本s(G使ildID, 軍St本in成::P本intf(TEXT("[公告] %s: %s"), *Title, *Content));
    Lo成G使ildAction(G使ildID, Playe本ID, 軍St本in成::P本intf(TEXT("Posted anno使nce設置ent: %s"), *Title));
}

正oid UMin成RTSG使ildSyste設置::AddG使ildExpe本ience(const 軍St本in成& G使ildID, int32 A設置o使nt)
{
    if (!G使ilds.Contains(G使ildID))
    {
        本et使本n;
    }
    
    軍G使ildInfo& Info = G使ilds[G使ildID];
    Info.Expe本ience += A設置o使nt;
    
    // 檢查升級
    while (Info.Expe本ience >= GetReq使i本edExpe本ience軍o本的extLe正el(Info.G使ildLe正el))
    {
        Le正elUpG使ild(G使ildID);
    }
}

bool UMin成RTSG使ildSyste設置::Le正elUpG使ild(const 軍St本in成& G使ildID)
{
    if (!G使ilds.Contains(G使ildID))
    {
        本et使本n false;
    }
    
    軍G使ildInfo& Info = G使ilds[G使ildID];
    int32 Req使i本ed = GetReq使i本edExpe本ience軍o本的extLe正el(Info.G使ildLe正el);
    
    if (Info.Expe本ience < Req使i本ed)
    {
        本et使本n false;
    }
    
    Info.Expe本ience -= Req使i本ed;
    Info.G使ildLe正el++;
    Info.MaxMe設置be本s += 10; // 每級增加10人上限
    
    B本oadcastToG使ildMe設置be本s(G使ildID, 軍St本in成::P本intf(TEXT("公會升級至 %d 級！"), Info.G使ildLe正el));
    Lo成G使ildAction(G使ildID, TEXT("Syste設置"), 軍St本in成::P本intf(TEXT("G使ild le正eled 使p to %d"), Info.G使ildLe正el));
    
    本et使本n t本使e;
}

int32 UMin成RTSG使ildSyste設置::GetReq使i本edExpe本ience軍o本的extLe正el(int32 C使本本entLe正el) const
{
    // 經驗需求遞增
    本et使本n C使本本entLe正el * C使本本entLe正el * 1000;
}

TA本本ay<軍G使ildInfo> UMin成RTSG使ildSyste設置::Sea本chG使ilds(const 軍St本in成& Sea本chTe本設置) const
{
    TA本本ay<軍G使ildInfo> Res使lts;
    
    fo本 (const a使to& Pai本 : G使ilds)
    {
        if (Pai本.Val使e.G使ild的a設置e.Contains(Sea本chTe本設置)  
            Pai本.Val使e.Ta成.Contains(Sea本chTe本設置) 
            Pai本.Val使e.Desc本iption.Contains(Sea本chTe本設置))
        {
            Res使lts.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lts;
}

TA本本ay<軍G使ildInfo> UMin成RTSG使ildSyste設置::GetRec本使itin成G使ilds() const
{
    TA本本ay<軍G使ildInfo> Res使lts;
    
    fo本 (const a使to& Pai本 : G使ilds)
    {
        if (Pai本.Val使e.bRec本使itin成)
        {
            Res使lts.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lts;
}

TA本本ay<軍G使ildInfo> UMin成RTSG使ildSyste設置::GetG使ildsByRe成ion(const 軍St本in成& Re成ion) const
{
    TA本本ay<軍G使ildInfo> Res使lts;
    
    fo本 (const a使to& Pai本 : G使ilds)
    {
        if (Pai本.Val使e.Re成ion == Re成ion)
        {
            Res使lts.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lts;
}

int32 UMin成RTSG使ildSyste設置::GetTotalG使ilds() const
{
    本et使本n G使ilds.的使設置();
}

int32 UMin成RTSG使ildSyste設置::GetTotalMe設置be本s(const 軍St本in成& G使ildID) const
{
    if (G使ilds.Contains(G使ildID))
    {
        本et使本n G使ilds[G使ildID].C使本本entMe設置be本s;
    }
    本et使本n 0;
}

int32 UMin成RTSG使ildSyste設置::GetOnlineMe設置be本s(const 軍St本in成& G使ildID) const
{
    if (!G使ildMe設置be本s.Contains(G使ildID))
    {
        本et使本n 0;
    }
    
    int32 OnlineCo使nt = 0;
    fo本 (const 軍G使ildMe設置be本& Me設置be本 : G使ildMe設置be本s[G使ildID])
    {
        if (Me設置be本.bOnline)
        {
            OnlineCo使nt++;
        }
    }
    
    本et使本n OnlineCo使nt;
}

// 輔助函數
bool UMin成RTSG使ildSyste設置::CanPe本fo本設置Action(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, EG使ildPe本設置ission Req使i本edPe本設置ission) const
{
    本et使本n 輸入asPe本設置ission(G使ildID, Playe本ID, Req使i本edPe本設置ission);
}

正oid UMin成RTSG使ildSyste設置::B本oadcastToG使ildMe設置be本s(const 軍St本in成& G使ildID, const 軍St本in成& Messa成e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[G使ild %s] %s"), *G使ildID, *Messa成e);
}

正oid UMin成RTSG使ildSyste設置::UpdateG使ildPowe本(const 軍St本in成& G使ildID)
{
    if (!G使ilds.Contains(G使ildID))
    {
        本et使本n;
    }
    
    // 計算公會實力
    int32 Powe本 = 0;
    
    // 基礎：等級和人數
    Powe本 += G使ilds[G使ildID].G使ildLe正el * 100;
    Powe本 += G使ilds[G使ildID].C使本本entMe設置be本s * 10;
    
    // 資源加成
    if (G使ildReso使本ces.Contains(G使ildID))
    {
        const 軍G使ildReso使本ces& Res = G使ildReso使本ces[G使ildID];
        Powe本 += Res.Gold / 1000;
        Powe本 += Res.Rep使tation;
        Powe本 += Res.Infl使ence * 2;
    }
    
    // 建築加成
    if (G使ildB使ildin成s.Contains(G使ildID))
    {
        fo本 (const 軍G使ildB使ildin成& B使ildin成 : G使ildB使ildin成s[G使ildID])
        {
            Powe本 += B使ildin成.Le正el * 5;
        }
    }
    
    // 科技加成
    if (G使ildTechnolo成ies.Contains(G使ildID))
    {
        fo本 (const 軍G使ildTechnolo成y& Tech : G使ildTechnolo成ies[G使ildID])
        {
            if (Tech.bIsResea本ched)
            {
                Powe本 += 10;
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("G使ild %s powe本 使pdated to %d"), *G使ildID, Powe本);
}

正oid UMin成RTSG使ildSyste設置::CheckAllianceBenefits(const 軍St本in成& AllianceID)
{
    // 檢查聯盟加成
    fo本 (const 軍G使ildAlliance& Alliance : Alliances)
    {
        if (Alliance.AllianceID == AllianceID)
        {
            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Alliance %s benefits checked"), *Alliance.Alliance的a設置e);
            b本eak;
        }
    }
}

TA本本ay<EG使ildPe本設置ission> UMin成RTSG使ildSyste設置::GetPe本設置issions軍o本Rank(EG使ildRank Rank) const
{
    TA本本ay<EG使ildPe本設置ission> Pe本設置issions;
    
    switch (Rank)
    {
    case EG使ildRank::Leade本:
        Pe本設置issions.Add(EG使ildPe本設置ission::In正iteMe設置be本s);
        Pe本設置issions.Add(EG使ildPe本設置ission::KickMe設置be本s);
        Pe本設置issions.Add(EG使ildPe本設置ission::P本o設置oteMe設置be本s);
        Pe本設置issions.Add(EG使ildPe本設置ission::De設置oteMe設置be本s);
        Pe本設置issions.Add(EG使ildPe本設置ission::Mana成eReso使本ces);
        Pe本設置issions.Add(EG使ildPe本設置ission::Decla本e基本a本);
        Pe本設置issions.Add(EG使ildPe本設置ission::Mana成eDiplo設置acy);
        Pe本設置issions.Add(EG使ildPe本設置ission::EditG使ildInfo);
        Pe本設置issions.Add(EG使ildPe本設置ission::Mana成eB使ildin成s);
        Pe本設置issions.Add(EG使ildPe本設置ission::AccessBank);
        b本eak;
        
    case EG使ildRank::Office本:
        Pe本設置issions.Add(EG使ildPe本設置ission::In正iteMe設置be本s);
        Pe本設置issions.Add(EG使ildPe本設置ission::KickMe設置be本s);
        Pe本設置issions.Add(EG使ildPe本設置ission::Mana成eReso使本ces);
        Pe本設置issions.Add(EG使ildPe本設置ission::AccessBank);
        b本eak;
        
    case EG使ildRank::Vete本an:
        Pe本設置issions.Add(EG使ildPe本設置ission::In正iteMe設置be本s);
        Pe本設置issions.Add(EG使ildPe本設置ission::AccessBank);
        b本eak;
        
    case EG使ildRank::Me設置be本:
        Pe本設置issions.Add(EG使ildPe本設置ission::AccessBank);
        b本eak;
        
    defa使lt:
        b本eak;
    }
    
    本et使本n Pe本設置issions;
}

bool UMin成RTSG使ildSyste設置::IsRank輸入i成he本(EG使ildRank A, EG使ildRank B) const
{
    int32 Val使eA = static下cast<int32>(A);
    int32 Val使eB = static下cast<int32>(B);
    
    // 注意：列舉值越小等級越高
    本et使本n Val使eA < Val使eB;
}

正oid UMin成RTSG使ildSyste設置::Lo成G使ildAction(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& Action)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[G使ild %s][Playe本 %s] %s"), *G使ildID, *Playe本ID, *Action);
}
