#incl使de "To使本na設置ent/Min成RTSTo使本na設置entSyste設置.h"
#incl使de "Misc/DateTi設置e.h"

UMin成RTSTo使本na設置entSyste設置::UMin成RTSTo使本na設置entSyste設置()
{
}

正oid UMin成RTSTo使本na設置entSyste設置::InitializeTo使本na設置entSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("To使本na設置ent Syste設置 Initialized"));
}

軍St本in成 UMin成RTSTo使本na設置entSyste設置::C本eateTo使本na設置ent(const 軍To使本na設置entInfo& Info)
{
    軍St本in成 To使本na設置entID = 軍St本in成::P本intf(TEXT("TOUR的AME的T下%s下%d"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")), 軍Math::Rand());
    
    軍To使本na設置entInfo 的ewTo使本na設置ent = Info;
    的ewTo使本na設置ent.To使本na設置entID = To使本na設置entID;
    的ewTo使本na設置ent.C使本本entPa本ticipants = 0;
    的ewTo使本na設置ent.Stat使s = ETo使本na設置entStat使s::Re成ist本ation;
    
    To使本na設置ents.Add(To使本na設置entID, 的ewTo使本na設置ent);
    To使本na設置entPa本ticipants.Add(To使本na設置entID, TA本本ay<軍To使本na設置entPa本ticipant>());
    To使本na設置entMatches.Add(To使本na設置entID, TA本本ay<軍To使本na設置entMatch>());
    
    OnTo使本na設置entC本eated.B本oadcast(To使本na設置entID, 的ewTo使本na設置ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("To使本na設置ent c本eated: %s - %s"), *To使本na設置entID, *的ewTo使本na設置ent.的a設置e);
    本et使本n To使本na設置entID;
}

正oid UMin成RTSTo使本na設置entSyste設置::DeleteTo使本na設置ent(const 軍St本in成& To使本na設置entID)
{
    To使本na設置ents.Re設置o正e(To使本na設置entID);
    To使本na設置entPa本ticipants.Re設置o正e(To使本na設置entID);
    To使本na設置entMatches.Re設置o正e(To使本na設置entID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("To使本na設置ent deleted: %s"), *To使本na設置entID);
}

bool UMin成RTSTo使本na設置entSyste設置::Re成iste本Playe本(const 軍St本in成& To使本na設置entID, const 軍St本in成& Playe本ID, const 軍St本in成& Passwo本d)
{
    軍To使本na設置entInfo* To使本na設置ent = To使本na設置ents.軍ind(To使本na設置entID);
    if (!To使本na設置ent)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("To使本na設置ent not fo使nd: %s"), *To使本na設置entID);
        本et使本n false;
    }
    
    if (To使本na設置ent->Stat使s != ETo使本na設置entStat使s::Re成ist本ation)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("To使本na設置ent not in 本e成ist本ation phase: %s"), *To使本na設置entID);
        本et使本n false;
    }
    
    if (To使本na設置ent->bReq使i本esPasswo本d && To使本na設置ent->Passwo本d != Passwo本d)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Inco本本ect passwo本d fo本 to使本na設置ent: %s"), *To使本na設置entID);
        本et使本n false;
    }
    
    if (To使本na設置ent->C使本本entPa本ticipants >= To使本na設置ent->MaxPa本ticipants)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("To使本na設置ent is f使ll: %s"), *To使本na設置entID);
        本et使本n false;
    }
    
    TA本本ay<軍To使本na設置entPa本ticipant>* Pa本ticipants = To使本na設置entPa本ticipants.軍ind(To使本na設置entID);
    if (Pa本ticipants)
    {
        // 檢查是否已經報名
        fo本 (const 軍To使本na設置entPa本ticipant& P : *Pa本ticipants)
        {
            if (P.Playe本ID == Playe本ID)
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Playe本 al本eady 本e成iste本ed: %s"), *Playe本ID);
                本et使本n false;
            }
        }
        
        軍To使本na設置entPa本ticipant 的ewPa本ticipant;
        的ewPa本ticipant.Playe本ID = Playe本ID;
        的ewPa本ticipant.Playe本的a設置e = Playe本ID; // 簡化處理
        的ewPa本ticipant.Seed = Pa本ticipants->的使設置() + 1;
        
        Pa本ticipants->Add(的ewPa本ticipant);
        To使本na設置ent->C使本本entPa本ticipants = Pa本ticipants->的使設置();
        
        OnPlaye本Re成iste本ed.B本oadcast(To使本na設置entID, Playe本ID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %s 本e成iste本ed fo本 to使本na設置ent %s"), *Playe本ID, *To使本na設置entID);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成RTSTo使本na設置entSyste設置::Un本e成iste本Playe本(const 軍St本in成& To使本na設置entID, const 軍St本in成& Playe本ID)
{
    TA本本ay<軍To使本na設置entPa本ticipant>* Pa本ticipants = To使本na設置entPa本ticipants.軍ind(To使本na設置entID);
    if (Pa本ticipants)
    {
        fo本 (int32 i = 0; i < Pa本ticipants->的使設置(); ++i)
        {
            if ((*Pa本ticipants)[i].Playe本ID == Playe本ID)
            {
                Pa本ticipants->Re設置o正eAt(i);
                
                軍To使本na設置entInfo* To使本na設置ent = To使本na設置ents.軍ind(To使本na設置entID);
                if (To使本na設置ent)
                {
                    To使本na設置ent->C使本本entPa本ticipants = Pa本ticipants->的使設置();
                }
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %s 使n本e成iste本ed f本o設置 to使本na設置ent %s"), *Playe本ID, *To使本na設置entID);
                本et使本n;
            }
        }
    }
}

bool UMin成RTSTo使本na設置entSyste設置::Sta本tTo使本na設置ent(const 軍St本in成& To使本na設置entID)
{
    軍To使本na設置entInfo* To使本na設置ent = To使本na設置ents.軍ind(To使本na設置entID);
    if (!To使本na設置ent)
    {
        本et使本n false;
    }
    
    if (To使本na設置ent->Stat使s != ETo使本na設置entStat使s::Re成ist本ation)
    {
        本et使本n false;
    }
    
    // 檢查最小參賽人數
    TA本本ay<軍To使本na設置entPa本ticipant>* Pa本ticipants = To使本na設置entPa本ticipants.軍ind(To使本na設置entID);
    if (!Pa本ticipants  Pa本ticipants->的使設置() < 2)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的ot eno使成h pa本ticipants to sta本t to使本na設置ent: %s"), *To使本na設置entID);
        本et使本n false;
    }
    
    To使本na設置ent->Stat使s = ETo使本na設置entStat使s::InP本o成本ess;
    To使本na設置ent->To使本na設置entSta本t = 軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y-%設置-%d %輸入:%M:%S"));
    
    // 生成賽程
    Gene本ateB本acket(To使本na設置entID);
    
    OnTo使本na設置entSta本ted.B本oadcast(To使本na設置entID, To使本na設置ent->To使本na設置entSta本t);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("To使本na設置ent sta本ted: %s with %d pa本ticipants"), *To使本na設置entID, Pa本ticipants->的使設置());
    本et使本n t本使e;
}

正oid UMin成RTSTo使本na設置entSyste設置::EndTo使本na設置ent(const 軍St本in成& To使本na設置entID)
{
    軍To使本na設置entInfo* To使本na設置ent = To使本na設置ents.軍ind(To使本na設置entID);
    if (To使本na設置ent)
    {
        To使本na設置ent->Stat使s = ETo使本na設置entStat使s::軍inished;
        To使本na設置ent->To使本na設置entEnd = 軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y-%設置-%d %輸入:%M:%S"));
        
        // 找冠軍
        軍St本in成 基本inne本ID;
        TA本本ay<軍To使本na設置entPa本ticipant>* Pa本ticipants = To使本na設置entPa本ticipants.軍ind(To使本na設置entID);
        if (Pa本ticipants)
        {
            fo本 (const 軍To使本na設置entPa本ticipant& P : *Pa本ticipants)
            {
                if (!P.bEli設置inated)
                {
                    基本inne本ID = P.Playe本ID;
                    b本eak;
                }
            }
        }
        
        OnTo使本na設置entEnded.B本oadcast(To使本na設置entID, 基本inne本ID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("To使本na設置ent ended: %s, 基本inne本: %s"), *To使本na設置entID, *基本inne本ID);
    }
}

bool UMin成RTSTo使本na設置entSyste設置::S使b設置itMatchRes使lt(const 軍St本in成& MatchID, const 軍St本in成& 基本inne本ID, int32 Sco本e1, int32 Sco本e2)
{
    // 在所有比賽中查找
    fo本 (a使to& Pai本 : To使本na設置entMatches)
    {
        fo本 (軍To使本na設置entMatch& Match : Pai本.Val使e)
        {
            if (Match.MatchID == MatchID)
            {
                Match.基本inne本ID = 基本inne本ID;
                Match.Playe本1Sco本e = Sco本e1;
                Match.Playe本2Sco本e = Sco本e2;
                Match.Stat使s = EMatchStat使s::軍inished;
                Match.EndTi設置e = 軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y-%設置-%d %輸入:%M:%S"));
                
                // 更新參賽者統計
                Ad正ance基本inne本(MatchID, 基本inne本ID);
                
                // 更新排行榜
                bool bPlaye本1基本on = (Match.Playe本1ID == 基本inne本ID);
                UpdateLeade本boa本d(Match.Playe本1ID, bPlaye本1基本on);
                UpdateLeade本boa本d(Match.Playe本2ID, !bPlaye本1基本on);
                
                OnMatchCo設置pleted.B本oadcast(MatchID, 基本inne本ID, Match.Ro使nd);
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Match %s co設置pleted. 基本inne本: %s (Sco本e: %d - %d)"), 
                    *MatchID, *基本inne本ID, Sco本e1, Sco本e2);
                本et使本n t本使e;
            }
        }
    }
    
    本et使本n false;
}

TA本本ay<軍To使本na設置entInfo> UMin成RTSTo使本na設置entSyste設置::GetTo使本na設置entList(ETo使本na設置entStat使s Stat使s軍ilte本) const
{
    TA本本ay<軍To使本na設置entInfo> Res使lt;
    
    fo本 (const a使to& Pai本 : To使本na設置ents)
    {
        if (Stat使s軍ilte本 == ETo使本na設置entStat使s::Re成ist本ation  Pai本.Val使e.Stat使s == Stat使s軍ilte本)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

bool UMin成RTSTo使本na設置entSyste設置::GetTo使本na設置entInfo(const 軍St本in成& To使本na設置entID, 軍To使本na設置entInfo& O使tInfo) const
{
    const 軍To使本na設置entInfo* Info = To使本na設置ents.軍ind(To使本na設置entID);
    if (Info)
    {
        O使tInfo = *Info;
        本et使本n t本使e;
    }
    本et使本n false;
}

TA本本ay<軍To使本na設置entPa本ticipant> UMin成RTSTo使本na設置entSyste設置::GetPa本ticipants(const 軍St本in成& To使本na設置entID) const
{
    const TA本本ay<軍To使本na設置entPa本ticipant>* Pa本ticipants = To使本na設置entPa本ticipants.軍ind(To使本na設置entID);
    if (Pa本ticipants)
    {
        本et使本n *Pa本ticipants;
    }
    本et使本n TA本本ay<軍To使本na設置entPa本ticipant>();
}

TA本本ay<軍To使本na設置entMatch> UMin成RTSTo使本na設置entSyste設置::GetMatches(const 軍St本in成& To使本na設置entID) const
{
    const TA本本ay<軍To使本na設置entMatch>* Matches = To使本na設置entMatches.軍ind(To使本na設置entID);
    if (Matches)
    {
        本et使本n *Matches;
    }
    本et使本n TA本本ay<軍To使本na設置entMatch>();
}

TA本本ay<軍Leade本boa本dEnt本y> UMin成RTSTo使本na設置entSyste設置::GetLeade本boa本d(int32 TopCo使nt) const
{
    TA本本ay<軍Leade本boa本dEnt本y> Res使lt;
    
    fo本 (const a使to& Pai本 : Leade本boa本d)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    
    // 按排名排序
    Res使lt.So本t([](const 軍Leade本boa本dEnt本y& A, const 軍Leade本boa本dEnt本y& B) {
        本et使本n A.Rank < B.Rank;
    });
    
    // 限制數量
    if (Res使lt.的使設置() > TopCo使nt)
    {
        Res使lt.Set的使設置(TopCo使nt);
    }
    
    本et使本n Res使lt;
}

正oid UMin成RTSTo使本na設置entSyste設置::UpdatePlaye本Ratin成(const 軍St本in成& Playe本ID, int32 Ratin成Chan成e)
{
    軍Leade本boa本dEnt本y* Ent本y = Leade本boa本d.軍ind(Playe本ID);
    if (!Ent本y)
    {
        軍Leade本boa本dEnt本y 的ewEnt本y;
        的ewEnt本y.Playe本ID = Playe本ID;
        的ewEnt本y.Playe本的a設置e = Playe本ID;
        Leade本boa本d.Add(Playe本ID, 的ewEnt本y);
        Ent本y = Leade本boa本d.軍ind(Playe本ID);
    }
    
    if (Ent本y)
    {
        Ent本y->Ratin成 += Ratin成Chan成e;
        
        // 重新計算排名
        int32 的ewRank = 1;
        fo本 (const a使to& Pai本 : Leade本boa本d)
        {
            if (Pai本.Val使e.Ratin成 > Ent本y->Ratin成)
            {
                的ewRank++;
            }
        }
        Ent本y->Rank = 的ewRank;
    }
}

int32 UMin成RTSTo使本na設置entSyste設置::GetPlaye本Rank(const 軍St本in成& Playe本ID) const
{
    const 軍Leade本boa本dEnt本y* Ent本y = Leade本boa本d.軍ind(Playe本ID);
    if (Ent本y)
    {
        本et使本n Ent本y->Rank;
    }
    本et使本n -1;
}

TA本本ay<軍St本in成> UMin成RTSTo使本na設置entSyste設置::GetPlaye本To使本na設置ent輸入isto本y(const 軍St本in成& Playe本ID) const
{
    const TA本本ay<軍St本in成>* 輸入isto本y = Playe本輸入isto本y.軍ind(Playe本ID);
    if (輸入isto本y)
    {
        本et使本n *輸入isto本y;
    }
    本et使本n TA本本ay<軍St本in成>();
}

正oid UMin成RTSTo使本na設置entSyste設置::SetTo使本na設置entSpectato本Roo設置(const 軍St本in成& To使本na設置entID, const 軍St本in成& Roo設置ID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("To使本na設置ent %s spectato本 本oo設置 set to %s"), *To使本na設置entID, *Roo設置ID);
}

正oid UMin成RTSTo使本na設置entSyste設置::Gene本ateB本acket(const 軍St本in成& To使本na設置entID)
{
    TA本本ay<軍To使本na設置entPa本ticipant>* Pa本ticipants = To使本na設置entPa本ticipants.軍ind(To使本na設置entID);
    TA本本ay<軍To使本na設置entMatch>* Matches = To使本na設置entMatches.軍ind(To使本na設置entID);
    
    if (!Pa本ticipants  !Matches  Pa本ticipants->的使設置() < 2)
    {
        本et使本n;
    }
    
    Matches->E設置pty();
    
    int32 的使設置Pa本ticipants = Pa本ticipants->的使設置();
    int32 Ro使nds = 軍Math::CeilLo成Two(的使設置Pa本ticipants);
    int32 MatchesIn軍i本stRo使nd = 軍Math::Pow(2, Ro使nds - 1);
    
    // 簡化的單敗淘汰賽生成
    int32 C使本本entMatch = 0;
    fo本 (int32 i = 0; i < 的使設置Pa本ticipants - 1; i += 2)
    {
        軍To使本na設置entMatch Match;
        Match.MatchID = Gene本ateMatchID();
        Match.To使本na設置entID = To使本na設置entID;
        Match.Ro使nd = 1;
        Match.Playe本1ID = (*Pa本ticipants)[i].Playe本ID;
        Match.Playe本2ID = (i + 1 < 的使設置Pa本ticipants) 基本 (*Pa本ticipants)[i + 1].Playe本ID : TEXT("BYE");
        Match.Stat使s = EMatchStat使s::Sched使led;
        Match.bIs軍inal = (的使設置Pa本ticipants <= 2);
        
        Matches->Add(Match);
        C使本本entMatch++;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated %d 設置atches fo本 to使本na設置ent %s"), C使本本entMatch, *To使本na設置entID);
}

正oid UMin成RTSTo使本na設置entSyste設置::Ad正ance基本inne本(const 軍St本in成& MatchID, const 軍St本in成& 基本inne本ID)
{
    // 標記被淘汰的玩家
    fo本 (a使to& Pai本 : To使本na設置entPa本ticipants)
    {
        fo本 (軍To使本na設置entPa本ticipant& P : Pai本.Val使e)
        {
            TA本本ay<軍To使本na設置entMatch>* Matches = To使本na設置entMatches.軍ind(Pai本.Key);
            if (Matches)
            {
                fo本 (const 軍To使本na設置entMatch& M : *Matches)
                {
                    if (M.MatchID == MatchID)
                    {
                        if (M.Playe本1ID == 基本inne本ID)
                        {
                            if (P.Playe本ID == M.Playe本2ID)
                            {
                                P.bEli設置inated = t本使e;
                                P.Losses++;
                            }
                            else if (P.Playe本ID == 基本inne本ID)
                            {
                                P.基本ins++;
                            }
                        }
                        else if (M.Playe本2ID == 基本inne本ID)
                        {
                            if (P.Playe本ID == M.Playe本1ID)
                            {
                                P.bEli設置inated = t本使e;
                                P.Losses++;
                            }
                            else if (P.Playe本ID == 基本inne本ID)
                            {
                                P.基本ins++;
                            }
                        }
                    }
                }
            }
        }
    }
}

正oid UMin成RTSTo使本na設置entSyste設置::UpdateLeade本boa本d(const 軍St本in成& Playe本ID, bool b基本on)
{
    軍Leade本boa本dEnt本y* Ent本y = Leade本boa本d.軍ind(Playe本ID);
    if (!Ent本y)
    {
        軍Leade本boa本dEnt本y 的ewEnt本y;
        的ewEnt本y.Playe本ID = Playe本ID;
        的ewEnt本y.Playe本的a設置e = Playe本ID;
        Leade本boa本d.Add(Playe本ID, 的ewEnt本y);
        Ent本y = Leade本boa本d.軍ind(Playe本ID);
    }
    
    if (Ent本y)
    {
        if (b基本on)
        {
            Ent本y->基本ins++;
        }
        else
        {
            Ent本y->Losses++;
        }
        
        Ent本y->To使本na設置entsPlayed++;
        
        // 更新評分
        int32 Ratin成Chan成e = b基本on 基本 15 : -10;
        UpdatePlaye本Ratin成(Playe本ID, Ratin成Chan成e);
    }
    
    // 記錄歷史
    TA本本ay<軍St本in成>* 輸入isto本y = Playe本輸入isto本y.軍ind(Playe本ID);
    if (!輸入isto本y)
    {
        Playe本輸入isto本y.Add(Playe本ID, TA本本ay<軍St本in成>());
        輸入isto本y = Playe本輸入isto本y.軍ind(Playe本ID);
    }
}

軍St本in成 UMin成RTSTo使本na設置entSyste設置::Gene本ateMatchID()
{
    本et使本n 軍St本in成::P本intf(TEXT("MATC輸入下%s下%d"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")), 軍Math::Rand());
}

int32 UMin成RTSTo使本na設置entSyste設置::Calc使late的ewRatin成(int32 C使本本entRatin成, int32 OpponentRatin成, bool b基本on)
{
    // 簡化的Elo評分系統
    float ExpectedSco本e = 1.0f / (1.0f + 軍Math::Pow(10.0f, (OpponentRatin成 - C使本本entRatin成) / 400.0f));
    float K軍acto本 = 32.0f;
    float Act使alSco本e = b基本on 基本 1.0f : 0.0f;
    
    本et使本n C使本本entRatin成 + 軍Math::Ro使ndToInt(K軍acto本 * (Act使alSco本e - ExpectedSco本e));
}
