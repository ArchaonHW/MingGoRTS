// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Uni正e本sity G使idance Syste設置 I設置ple設置entation - C3-1

#incl使de "Ed使cation/Min成Uni正e本sityG使idanceSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"
#incl使de "Se本ialization/JsonReade本.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成G使idance, Lo成, All);

UMin成Uni正e本sityG使idanceSyste設置::UMin成Uni正e本sityG使idanceSyste設置()
    : bG使idanceEnabled(t本使e)
    , bPa使sed(false)
{
}

正oid UMin成Uni正e本sityG使idanceSyste設置::InitializeG使idanceSyste設置()
{
    G使idanceContents.E設置pty();
    Lea本nin成Paths.E設置pty();
    St使dentP本ofiles.E設置pty();
    St使dentP本o成本ess.E設置pty();
    Q使izQ使estions.E設置pty();
    
    InitializeDefa使ltContent();
    InitializeDefa使ltLea本nin成Paths();
    InitializeDefa使ltQ使izzes();
    
    Lo成G使idance(TEXT("Uni正e本sity G使idance Syste設置 initialized"));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Sh使tdownG使idanceSyste設置()
{
    if (U基本o本ld* 基本o本ld = GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this, EGet基本o本ldE本本o本Mode::Ret使本n的使ll))
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Re設置inde本Ti設置e本);
    }
    
    fo本 (const a使to& St使dent : St使dentP本ofiles)
    {
        Sa正eP本o成本ess(St使dent.Key);
    }
    
    Lo成G使idance(TEXT("Uni正e本sity G使idance Syste設置 sh使tdown"));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Re成iste本St使dent(const 軍St本in成& St使dentID, const 軍St本in成& St使dent的a設置e, const 軍St本in成& ClassG本o使p)
{
    if (St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n;
    }
    
    軍St使dentP本ofile P本ofile;
    P本ofile.St使dentID = St使dentID;
    P本ofile.St使dent的a設置e = St使dent的a設置e;
    P本ofile.ClassG本o使p = ClassG本o使p;
    P本ofile.C使本本entLe正el = EG使idanceLe正el::Be成inne本;
    P本ofile.TotalExpe本iencePoints = 0;
    P本ofile.Co設置pletedContentCo使nt = 0;
    P本ofile.TotalSt使dy輸入o使本s = 0.0f;
    P本ofile.LastActi正ityTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    
    St使dentP本ofiles.Add(St使dentID, P本ofile);
    St使dentP本o成本ess.Add(St使dentID, TMap<軍St本in成, 軍G使idanceP本o成本ess>());
    
    LoadP本o成本ess(St使dentID);
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent 本e成iste本ed: %s (%s)"), *St使dent的a設置e, *St使dentID));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Un本e成iste本St使dent(const 軍St本in成& St使dentID)
{
    Sa正eP本o成本ess(St使dentID);
    
    St使dentP本ofiles.Re設置o正e(St使dentID);
    St使dentP本o成本ess.Re設置o正e(St使dentID);
    
    Acti正eSt使dents.Re設置o正e(St使dentID);
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent 使n本e成iste本ed: %s"), *St使dentID));
}

軍St使dentP本ofile UMin成Uni正e本sityG使idanceSyste設置::GetSt使dentP本ofile(const 軍St本in成& St使dentID)
{
    if (St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n St使dentP本ofiles[St使dentID];
    }
    
    本et使本n 軍St使dentP本ofile();
}

正oid UMin成Uni正e本sityG使idanceSyste設置::UpdateSt使dentP本ofile(const 軍St本in成& St使dentID, const 軍St使dentP本ofile& P本ofile)
{
    if (St使dentP本ofiles.Contains(St使dentID))
    {
        St使dentP本ofiles[St使dentID] = P本ofile;
        St使dentP本ofiles[St使dentID].LastActi正ityTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::AddG使idanceContent(const 軍G使idanceContent& Content)
{
    G使idanceContents.Add(Content.ContentID, Content);
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("G使idance content added: %s"), *Content.Title));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Re設置o正eG使idanceContent(const 軍St本in成& ContentID)
{
    G使idanceContents.Re設置o正e(ContentID);
}

軍G使idanceContent UMin成Uni正e本sityG使idanceSyste設置::GetG使idanceContent(const 軍St本in成& ContentID)
{
    if (G使idanceContents.Contains(ContentID))
    {
        本et使本n G使idanceContents[ContentID];
    }
    
    本et使本n 軍G使idanceContent();
}

TA本本ay<軍G使idanceContent> UMin成Uni正e本sityG使idanceSyste設置::GetAllG使idanceContent()
{
    TA本本ay<軍G使idanceContent> Res使lt;
    G使idanceContents.Gene本ateVal使eA本本ay(Res使lt);
    
    Res使lt.So本t([](const 軍G使idanceContent& A, const 軍G使idanceContent& B)
    {
        本et使本n A.O本de本Index < B.O本de本Index;
    });
    
    本et使本n Res使lt;
}

TA本本ay<軍G使idanceContent> UMin成Uni正e本sityG使idanceSyste設置::GetContentByCate成o本y(EG使idanceCate成o本y Cate成o本y)
{
    TA本本ay<軍G使idanceContent> Res使lt;
    
    fo本 (const a使to& Pai本 : G使idanceContents)
    {
        if (Pai本.Val使e.Cate成o本y == Cate成o本y)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍G使idanceContent> UMin成Uni正e本sityG使idanceSyste設置::GetContentByLe正el(EG使idanceLe正el Le正el)
{
    TA本本ay<軍G使idanceContent> Res使lt;
    
    fo本 (const a使to& Pai本 : G使idanceContents)
    {
        if (Pai本.Val使e.Le正el == Le正el)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Sta本tG使idanceContent(const 軍St本in成& St使dentID, const 軍St本in成& ContentID)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n;
    }
    
    if (!G使idanceContents.Contains(ContentID))
    {
        本et使本n;
    }
    
    軍G使idanceContent Content = G使idanceContents[ContentID];
    
    if (!IsContentUnlocked(St使dentID, ContentID))
    {
        Lo成G使idance(軍St本in成::P本intf(TEXT("Content %s is locked fo本 st使dent %s"), *ContentID, *St使dentID));
        本et使本n;
    }
    
    TMap<軍St本in成, 軍G使idanceP本o成本ess>& P本o成本essMap = St使dentP本o成本ess[St使dentID];
    
    if (!P本o成本essMap.Contains(ContentID))
    {
        軍G使idanceP本o成本ess P本o成本ess;
        P本o成本ess.ContentID = ContentID;
        P本o成本ess.Stat使s = EP本o成本essStat使s::InP本o成本ess;
        P本o成本ess.Sta本tTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
        P本o成本ess.Atte設置pts = 1;
        
        P本o成本essMap.Add(ContentID, P本o成本ess);
    }
    else
    {
        軍G使idanceP本o成本ess& P本o成本ess = P本o成本essMap[ContentID];
        P本o成本ess.Stat使s = EP本o成本essStat使s::InP本o成本ess;
        P本o成本ess.Atte設置pts++;
    }
    
    C使本本entContentID = ContentID;
    
    OnContentSta本ted.B本oadcast(ContentID);
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent %s sta本ted content: %s"), *St使dentID, *Content.Title));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Co設置pleteG使idanceContent(const 軍St本in成& St使dentID, const 軍St本in成& ContentID, float Sco本e)
{
    if (!St使dentP本o成本ess.Contains(St使dentID))
    {
        本et使本n;
    }
    
    TMap<軍St本in成, 軍G使idanceP本o成本ess>& P本o成本essMap = St使dentP本o成本ess[St使dentID];
    
    if (!P本o成本essMap.Contains(ContentID))
    {
        本et使本n;
    }
    
    軍G使idanceP本o成本ess& P本o成本ess = P本o成本essMap[ContentID];
    P本o成本ess.Stat使s = EP本o成本essStat使s::Co設置pleted;
    P本o成本ess.Co設置pletionPe本centa成e = 100.0f;
    P本o成本ess.Co設置pletionTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    P本o成本ess.Sco本e = Sco本e;
    
    軍G使idanceContent Content = G使idanceContents[ContentID];
    
    Awa本dExpe本iencePoints(St使dentID, Content.Expe本iencePoints);
    
    St使dentP本ofiles[St使dentID].Co設置pletedContentCo使nt++;
    St使dentP本ofiles[St使dentID].TotalSt使dy輸入o使本s += Content.Esti設置atedD使本ationMin使tes / 60.0f;
    
    UnlockP本e本eq使isiteContent(St使dentID, ContentID);
    CheckPathCo設置pletion(St使dentID, ContentID);
    E正al使ateAchie正e設置ents(St使dentID);
    
    OnContentCo設置pleted.B本oadcast(ContentID);
    
    Sa正eP本o成本ess(St使dentID);
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent %s co設置pleted content: %s (Sco本e: %.1f)"), 
        *St使dentID, *ContentID, Sco本e));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::UpdateContentP本o成本ess(const 軍St本in成& St使dentID, const 軍St本in成& ContentID, float P本o成本essPe本centa成e)
{
    if (!St使dentP本o成本ess.Contains(St使dentID))
    {
        本et使本n;
    }
    
    TMap<軍St本in成, 軍G使idanceP本o成本ess>& P本o成本essMap = St使dentP本o成本ess[St使dentID];
    
    if (P本o成本essMap.Contains(ContentID))
    {
        P本o成本essMap[ContentID].Co設置pletionPe本centa成e = 軍Math::Cla設置p(P本o成本essPe本centa成e, 0.0f, 100.0f);
        
        OnP本o成本essUpdated.B本oadcast(ContentID, P本o成本essPe本centa成e);
    }
}

軍G使idanceP本o成本ess UMin成Uni正e本sityG使idanceSyste設置::GetContentP本o成本ess(const 軍St本in成& St使dentID, const 軍St本in成& ContentID)
{
    if (St使dentP本o成本ess.Contains(St使dentID))
    {
        TMap<軍St本in成, 軍G使idanceP本o成本ess>& P本o成本essMap = St使dentP本o成本ess[St使dentID];
        
        if (P本o成本essMap.Contains(ContentID))
        {
            本et使本n P本o成本essMap[ContentID];
        }
    }
    
    軍G使idanceP本o成本ess E設置ptyP本o成本ess;
    E設置ptyP本o成本ess.ContentID = ContentID;
    E設置ptyP本o成本ess.Stat使s = EP本o成本essStat使s::Locked;
    本et使本n E設置ptyP本o成本ess;
}

TA本本ay<軍G使idanceP本o成本ess> UMin成Uni正e本sityG使idanceSyste設置::GetAllP本o成本ess軍o本St使dent(const 軍St本in成& St使dentID)
{
    TA本本ay<軍G使idanceP本o成本ess> Res使lt;
    
    if (St使dentP本o成本ess.Contains(St使dentID))
    {
        St使dentP本o成本ess[St使dentID].Gene本ateVal使eA本本ay(Res使lt);
    }
    
    本et使本n Res使lt;
}

正oid UMin成Uni正e本sityG使idanceSyste設置::AddLea本nin成Path(const 軍Lea本nin成Path& Path)
{
    Lea本nin成Paths.Add(Path.PathID, Path);
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("Lea本nin成 path added: %s"), *Path.的a設置e));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Re設置o正eLea本nin成Path(const 軍St本in成& PathID)
{
    Lea本nin成Paths.Re設置o正e(PathID);
}

軍Lea本nin成Path UMin成Uni正e本sityG使idanceSyste設置::GetLea本nin成Path(const 軍St本in成& PathID)
{
    if (Lea本nin成Paths.Contains(PathID))
    {
        本et使本n Lea本nin成Paths[PathID];
    }
    
    本et使本n 軍Lea本nin成Path();
}

TA本本ay<軍Lea本nin成Path> UMin成Uni正e本sityG使idanceSyste設置::GetAllLea本nin成Paths()
{
    TA本本ay<軍Lea本nin成Path> Res使lt;
    Lea本nin成Paths.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

正oid UMin成Uni正e本sityG使idanceSyste設置::En本ollSt使dentInPath(const 軍St本in成& St使dentID, const 軍St本in成& PathID)
{
    if (!St使dentP本ofiles.Contains(St使dentID)  !Lea本nin成Paths.Contains(PathID))
    {
        本et使本n;
    }
    
    if (!St使dentP本ofiles[St使dentID].Co設置pletedPaths.Contains(PathID))
    {
        Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent %s en本olled in path: %s"), *St使dentID, *PathID));
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Co設置pleteLea本nin成Path(const 軍St本in成& St使dentID, const 軍St本in成& PathID)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n;
    }
    
    if (!St使dentP本ofiles[St使dentID].Co設置pletedPaths.Contains(PathID))
    {
        St使dentP本ofiles[St使dentID].Co設置pletedPaths.Add(PathID);
        
        if (Lea本nin成Paths.Contains(PathID))
        {
            軍Lea本nin成Path Path = Lea本nin成Paths[PathID];
            Awa本dExpe本iencePoints(St使dentID, Path.TotalExpe本iencePoints);
            Awa本dAchie正e設置ent(St使dentID, 軍St本in成::P本intf(TEXT("path下%s"), *PathID), 
                軍St本in成::P本intf(TEXT("Co設置pleted: %s"), *Path.的a設置e));
        }
        
        OnPathCo設置pleted.B本oadcast(PathID);
        
        Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent %s co設置pleted lea本nin成 path: %s"), *St使dentID, *PathID));
    }
}

float UMin成Uni正e本sityG使idanceSyste設置::GetPathP本o成本ess軍o本St使dent(const 軍St本in成& St使dentID, const 軍St本in成& PathID)
{
    if (!St使dentP本ofiles.Contains(St使dentID)  !Lea本nin成Paths.Contains(PathID))
    {
        本et使本n 0.0f;
    }
    
    if (St使dentP本ofiles[St使dentID].Co設置pletedPaths.Contains(PathID))
    {
        本et使本n 100.0f;
    }
    
    軍Lea本nin成Path Path = Lea本nin成Paths[PathID];
    
    if (Path.ContentSeq使ence.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    int32 Co設置pletedCo使nt = 0;
    
    if (St使dentP本o成本ess.Contains(St使dentID))
    {
        TMap<軍St本in成, 軍G使idanceP本o成本ess>& P本o成本essMap = St使dentP本o成本ess[St使dentID];
        
        fo本 (const 軍St本in成& ContentID : Path.ContentSeq使ence)
        {
            if (P本o成本essMap.Contains(ContentID) && P本o成本essMap[ContentID].Stat使s == EP本o成本essStat使s::Co設置pleted)
            {
                Co設置pletedCo使nt++;
            }
        }
    }
    
    本et使本n (float)Co設置pletedCo使nt / Path.ContentSeq使ence.的使設置() * 100.0f;
}

軍G使idanceContent UMin成Uni正e本sityG使idanceSyste設置::Get的extReco設置設置endedContent(const 軍St本in成& St使dentID)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n 軍G使idanceContent();
    }
    
    TA本本ay<軍G使idanceContent> A正ailableContent = GetA正ailableContent(St使dentID);
    
    if (A正ailableContent.的使設置() > 0)
    {
        A正ailableContent.So本t([](const 軍G使idanceContent& A, const 軍G使idanceContent& B)
        {
            if (A.Le正el != B.Le正el)
            {
                本et使本n A.Le正el < B.Le正el;
            }
            本et使本n A.O本de本Index < B.O本de本Index;
        });
        
        本et使本n A正ailableContent[0];
    }
    
    本et使本n 軍G使idanceContent();
}

正oid UMin成Uni正e本sityG使idanceSyste設置::AddQ使izQ使estion(const 軍Q使izQ使estion& Q使estion)
{
    Q使izQ使estions.Add(Q使estion.Q使estionID, Q使estion);
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Re設置o正eQ使izQ使estion(const 軍St本in成& Q使estionID)
{
    Q使izQ使estions.Re設置o正e(Q使estionID);
}

TA本本ay<軍Q使izQ使estion> UMin成Uni正e本sityG使idanceSyste設置::GetQ使iz軍o本Cate成o本y(EG使idanceCate成o本y Cate成o本y, int32 Q使estionCo使nt)
{
    TA本本ay<軍Q使izQ使estion> Cate成o本yQ使estions;
    
    fo本 (const a使to& Pai本 : Q使izQ使estions)
    {
        if (Pai本.Val使e.Cate成o本y == Cate成o本y)
        {
            Cate成o本yQ使estions.Add(Pai本.Val使e);
        }
    }
    
    Cate成o本yQ使estions.So本t([](const 軍Q使izQ使estion& A, const 軍Q使izQ使estion& B)
    {
        本et使本n A.Diffic使lty < B.Diffic使lty;
    });
    
    while (Cate成o本yQ使estions.的使設置() > Q使estionCo使nt)
    {
        Cate成o本yQ使estions.Pop();
    }
    
    本et使本n Cate成o本yQ使estions;
}

軍Q使izRes使lt UMin成Uni正e本sityG使idanceSyste設置::S使b設置itQ使izAnswe本s(const 軍St本in成& St使dentID, const 軍St本in成& Q使izID, const TA本本ay<int32>& Answe本s)
{
    軍Q使izRes使lt Res使lt;
    Res使lt.Q使izID = Q使izID;
    Res使lt.TotalQ使estions = Answe本s.的使設置();
    Res使lt.Co設置pletionTi設置esta設置p = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    
    int32 Co本本ectCo使nt = 0;
    int32 PointsEa本ned = 0;
    
    int32 Index = 0;
    fo本 (const a使to& Pai本 : Q使izQ使estions)
    {
        if (Index >= Answe本s.的使設置())
        {
            b本eak;
        }
        
        if (Answe本s[Index] == Pai本.Val使e.Co本本ectAnswe本Index)
        {
            Co本本ectCo使nt++;
            PointsEa本ned += Pai本.Val使e.Points;
        }
        else
        {
            Res使lt.MissedQ使estions.Add(Pai本.Val使e.Q使estionID);
        }
        
        Index++;
    }
    
    Res使lt.Co本本ectAnswe本s = Co本本ectCo使nt;
    Res使lt.PointsEa本ned = PointsEa本ned;
    Res使lt.Sco本e = (Res使lt.TotalQ使estions > 0) 基本 ((float)Co本本ectCo使nt / Res使lt.TotalQ使estions * 100.0f) : 0.0f;
    
    Awa本dExpe本iencePoints(St使dentID, PointsEa本ned);
    
    OnQ使izCo設置pleted.B本oadcast(Res使lt);
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent %s co設置pleted q使iz %s (Sco本e: %.1f%%)"), 
        *St使dentID, *Q使izID, Res使lt.Sco本e));
    
    本et使本n Res使lt;
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Awa本dExpe本iencePoints(const 軍St本in成& St使dentID, int32 Points)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n;
    }
    
    軍St使dentP本ofile& P本ofile = St使dentP本ofiles[St使dentID];
    P本ofile.TotalExpe本iencePoints += Points;
    
    EG使idanceLe正el 的ewLe正el = Calc使lateLe正el軍本o設置XP(P本ofile.TotalExpe本iencePoints);
    
    if (的ewLe正el > P本ofile.C使本本entLe正el)
    {
        P本ofile.C使本本entLe正el = 的ewLe正el;
        OnLe正elUp.B本oadcast(的ewLe正el);
        
        Awa本dAchie正e設置ent(St使dentID, 軍St本in成::P本intf(TEXT("le正el下%d"), static下cast<int32>(的ewLe正el)), 
            軍St本in成::P本intf(TEXT("Reached %s Le正el"), *StaticEn使設置<EG使idanceLe正el>()->Get的a設置eSt本in成ByVal使e(static下cast<int32>(的ewLe正el))));
    }
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent %s awa本ded %d XP (Total: %d)"), 
        *St使dentID, Points, P本ofile.TotalExpe本iencePoints));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Awa本dAchie正e設置ent(const 軍St本in成& St使dentID, const 軍St本in成& Achie正e設置entID, const 軍St本in成& Achie正e設置ent的a設置e)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n;
    }
    
    軍St使dentP本ofile& P本ofile = St使dentP本ofiles[St使dentID];
    
    if (!P本ofile.Achie正e設置ents.Contains(Achie正e設置entID))
    {
        P本ofile.Achie正e設置ents.Add(Achie正e設置entID);
        
        OnAchie正e設置entUnlocked.B本oadcast(Achie正e設置entID);
        
        Lo成G使idance(軍St本in成::P本intf(TEXT("St使dent %s 使nlocked achie正e設置ent: %s"), *St使dentID, *Achie正e設置ent的a設置e));
    }
}

bool UMin成Uni正e本sityG使idanceSyste設置::CheckAndPe本fo本設置Le正elUp(const 軍St本in成& St使dentID)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n false;
    }
    
    軍St使dentP本ofile& P本ofile = St使dentP本ofiles[St使dentID];
    EG使idanceLe正el 的ewLe正el = Calc使lateLe正el軍本o設置XP(P本ofile.TotalExpe本iencePoints);
    
    if (的ewLe正el > P本ofile.C使本本entLe正el)
    {
        P本ofile.C使本本entLe正el = 的ewLe正el;
        OnLe正elUp.B本oadcast(的ewLe正el);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成Uni正e本sityG使idanceSyste設置::ShowG使idancePop使p(const 軍St本in成& St使dentID, const 軍St本in成& ContentID)
{
    if (G使idanceContents.Contains(ContentID))
    {
        軍G使idanceContent Content = G使idanceContents[ContentID];
        
        Lo成G使idance(軍St本in成::P本intf(TEXT("Showin成 成使idance pop使p fo本 st使dent %s: %s"), *St使dentID, *Content.Title));
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::ShowTooltip(const 軍St本in成& St使dentID, const 軍St本in成& Ta本成etEle設置entID, const 軍St本in成& Messa成e)
{
    Lo成G使idance(軍St本in成::P本intf(TEXT("Showin成 tooltip on %s: %s"), *Ta本成etEle設置entID, *Messa成e));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::輸入i成hli成htEle設置ent(const 軍St本in成& St使dentID, const 軍St本in成& Ele設置entID, float D使本ation)
{
    Lo成G使idance(軍St本in成::P本intf(TEXT("輸入i成hli成htin成 ele設置ent %s fo本 %.1f seconds"), *Ele設置entID, D使本ation));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Sta本tInte本acti正eDe設置o(const 軍St本in成& St使dentID, const 軍St本in成& De設置oID)
{
    Lo成G使idance(軍St本in成::P本intf(TEXT("Sta本tin成 inte本acti正e de設置o %s fo本 st使dent %s"), *De設置oID, *St使dentID));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::EnableSt使dyRe設置inde本s(bool bEnable, float Re設置inde本Inte本正al輸入o使本s)
{
    if (U基本o本ld* 基本o本ld = GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this, EGet基本o本ldE本本o本Mode::Ret使本n的使ll))
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Re設置inde本Ti設置e本);
        
        if (bEnable)
        {
            基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(Re設置inde本Ti設置e本, this, &UMin成Uni正e本sityG使idanceSyste設置::SendRe設置inde本的otification, 
                Re設置inde本Inte本正al輸入o使本s * 3600.0f, t本使e);
        }
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Book設置a本kContent(const 軍St本in成& St使dentID, const 軍St本in成& ContentID, bool bBook設置a本k)
{
    if (!St使dentP本o成本ess.Contains(St使dentID))
    {
        本et使本n;
    }
    
    TMap<軍St本in成, 軍G使idanceP本o成本ess>& P本o成本essMap = St使dentP本o成本ess[St使dentID];
    
    if (P本o成本essMap.Contains(ContentID))
    {
        P本o成本essMap[ContentID].bBook設置a本ked = bBook設置a本k;
    }
}

TA本本ay<軍G使idanceContent> UMin成Uni正e本sityG使idanceSyste設置::GetBook設置a本kedContent(const 軍St本in成& St使dentID)
{
    TA本本ay<軍G使idanceContent> Res使lt;
    
    if (St使dentP本o成本ess.Contains(St使dentID))
    {
        TMap<軍St本in成, 軍G使idanceP本o成本ess>& P本o成本essMap = St使dentP本o成本ess[St使dentID];
        
        fo本 (const a使to& Pai本 : P本o成本essMap)
        {
            if (Pai本.Val使e.bBook設置a本ked && G使idanceContents.Contains(Pai本.Key))
            {
                Res使lt.Add(G使idanceContents[Pai本.Key]);
            }
        }
    }
    
    本et使本n Res使lt;
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Sea本chContent(const 軍St本in成& Sea本chQ使e本y, TA本本ay<軍G使idanceContent>& O使tRes使lts)
{
    O使tRes使lts.E設置pty();
    
    軍St本in成 Lowe本Q使e本y = Sea本chQ使e本y.ToLowe本();
    
    fo本 (const a使to& Pai本 : G使idanceContents)
    {
        if (Pai本.Val使e.Title.ToLowe本().Contains(Lowe本Q使e本y) 
            Pai本.Val使e.Desc本iption.ToLowe本().Contains(Lowe本Q使e本y))
        {
            O使tRes使lts.Add(Pai本.Val使e);
        }
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Gene本ateSt使dyPlan(const 軍St本in成& St使dentID, int32 Ta本成et輸入o使本sPe本基本eek)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n;
    }
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("Gene本ated st使dy plan fo本 st使dent %s (%d ho使本s/week)"), 
        *St使dentID, Ta本成et輸入o使本sPe本基本eek));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Expo本tP本o成本essRepo本t(const 軍St本in成& St使dentID, const 軍St本in成& 軍ilePath)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n;
    }
    
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect = MakeSha本eable(new 軍JsonOb大ect);
    
    軍St使dentP本ofile P本ofile = St使dentP本ofiles[St使dentID];
    JsonOb大ect->SetSt本in成軍ield(TEXT("St使dentID"), P本ofile.St使dentID);
    JsonOb大ect->SetSt本in成軍ield(TEXT("St使dent的a設置e"), P本ofile.St使dent的a設置e);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("TotalXP"), P本ofile.TotalExpe本iencePoints);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("Co設置pletedContent"), P本ofile.Co設置pletedContentCo使nt);
    
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(JsonOb大ect.ToSha本edRef(), 基本本ite本);
    
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(O使tp使tSt本in成, *軍ilePath);
    
    Lo成G使idance(軍St本in成::P本intf(TEXT("P本o成本ess 本epo本t expo本ted fo本 st使dent %s to %s"), *St使dentID, *軍ilePath));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::I設置po本tP本o成本essRepo本t(const 軍St本in成& 軍ilePath)
{
    軍St本in成 JsonSt本in成;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonSt本in成, *軍ilePath))
    {
        本et使本n;
    }
    
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JsonSt本in成);
    
    if (軍JsonSe本ialize本::Dese本ialize(Reade本, JsonOb大ect))
    {
        Lo成G使idance(TEXT("P本o成本ess 本epo本t i設置po本ted"));
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Pa使seG使idance()
{
    bPa使sed = t本使e;
    Lo成G使idance(TEXT("G使idance pa使sed"));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Res使設置eG使idance()
{
    bPa使sed = false;
    Lo成G使idance(TEXT("G使idance 本es使設置ed"));
}

正oid UMin成Uni正e本sityG使idanceSyste設置::SkipC使本本entContent()
{
    if (!C使本本entContentID.IsE設置pty())
    {
        Lo成G使idance(軍St本in成::P本intf(TEXT("Content skipped: %s"), *C使本本entContentID));
        C使本本entContentID.E設置pty();
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::SetG使idanceEnabled(bool bEnabled)
{
    bG使idanceEnabled = bEnabled;
}

bool UMin成Uni正e本sityG使idanceSyste設置::IsG使idanceEnabled() const
{
    本et使本n bG使idanceEnabled;
}

int32 UMin成Uni正e本sityG使idanceSyste設置::GetTotalContentCo使nt() const
{
    本et使本n G使idanceContents.的使設置();
}

int32 UMin成Uni正e本sityG使idanceSyste設置::GetCo設置pletedContentCo使nt(const 軍St本in成& St使dentID)
{
    if (St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n St使dentP本ofiles[St使dentID].Co設置pletedContentCo使nt;
    }
    本et使本n 0;
}

float UMin成Uni正e本sityG使idanceSyste設置::GetO正e本allP本o成本ess軍o本St使dent(const 軍St本in成& St使dentID)
{
    if (G使idanceContents.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    本et使本n (float)GetCo設置pletedContentCo使nt(St使dentID) / G使idanceContents.的使設置() * 100.0f;
}

正oid UMin成Uni正e本sityG使idanceSyste設置::InitializeDefa使ltContent()
{
    軍G使idanceContent Content1;
    Content1.ContentID = TEXT("t使to本ial下001");
    Content1.Title = TEXT("基本elco設置e to Min成GoRTS");
    Content1.Desc本iption = TEXT("Int本od使ction to the 成a設置e and basic cont本ols");
    Content1.Cate成o本y = EG使idanceCate成o本y::T使to本ial;
    Content1.Le正el = EG使idanceLe正el::Be成inne本;
    Content1.Type = EG使idanceType::Video;
    Content1.Esti設置atedD使本ationMin使tes = 5.0f;
    Content1.Expe本iencePoints = 10;
    Content1.O本de本Index = 0;
    AddG使idanceContent(Content1);
    
    軍G使idanceContent Content2;
    Content2.ContentID = TEXT("t使to本ial下002");
    Content2.Title = TEXT("Basic Unit Cont本ol");
    Content2.Desc本iption = TEXT("Lea本n how to select and 設置o正e 使nits");
    Content2.Cate成o本y = EG使idanceCate成o本y::Ga設置eMechanics;
    Content2.Le正el = EG使idanceLe正el::Be成inne本;
    Content2.Type = EG使idanceType::Inte本acti正eDe設置o;
    Content2.P本e本eq使isites.Add(TEXT("t使to本ial下001"));
    Content2.Esti設置atedD使本ationMin使tes = 10.0f;
    Content2.Expe本iencePoints = 20;
    Content2.O本de本Index = 1;
    Content2.bIsInte本acti正e = t本使e;
    AddG使idanceContent(Content2);
    
    軍G使idanceContent Content3;
    Content3.ContentID = TEXT("histo本y下001");
    Content3.Title = TEXT("The Rep使blican E本a");
    Content3.Desc本iption = TEXT("輸入isto本ical context of the 成a設置e settin成");
    Content3.Cate成o本y = EG使idanceCate成o本y::輸入isto本icalContext;
    Content3.Le正el = EG使idanceLe正el::Be成inne本;
    Content3.Type = EG使idanceType::Encyclopedia;
    Content3.Esti設置atedD使本ationMin使tes = 15.0f;
    Content3.Expe本iencePoints = 15;
    Content3.O本de本Index = 2;
    AddG使idanceContent(Content3);
    
    軍G使idanceContent Content4;
    Content4.ContentID = TEXT("st本ate成y下001");
    Content4.Title = TEXT("Basic St本ate成y");
    Content4.Desc本iption = TEXT("Int本od使ction to RTS st本ate成y concepts");
    Content4.Cate成o本y = EG使idanceCate成o本y::St本ate成yG使ide;
    Content4.Le正el = EG使idanceLe正el::Inte本設置ediate;
    Content4.Type = EG使idanceType::StepByStep;
    Content4.P本e本eq使isites.Add(TEXT("t使to本ial下002"));
    Content4.Esti設置atedD使本ationMin使tes = 20.0f;
    Content4.Expe本iencePoints = 30;
    Content4.O本de本Index = 3;
    AddG使idanceContent(Content4);
    
    軍G使idanceContent Content5;
    Content5.ContentID = TEXT("c使lt使本e下001");
    Content5.Title = TEXT("Rep使blican E本a C使lt使本e");
    Content5.Desc本iption = TEXT("C使lt使本al aspects of the ti設置e pe本iod");
    Content5.Cate成o本y = EG使idanceCate成o本y::C使lt使本alEd使cation;
    Content5.Le正el = EG使idanceLe正el::Inte本設置ediate;
    Content5.Type = EG使idanceType::Encyclopedia;
    Content5.Esti設置atedD使本ationMin使tes = 25.0f;
    Content5.Expe本iencePoints = 25;
    Content5.O本de本Index = 4;
    AddG使idanceContent(Content5);
}

正oid UMin成Uni正e本sityG使idanceSyste設置::InitializeDefa使ltLea本nin成Paths()
{
    軍Lea本nin成Path Path1;
    Path1.PathID = TEXT("be成inne本下path");
    Path1.的a設置e = TEXT("Be成inne本's Jo使本ney");
    Path1.Desc本iption = TEXT("Co設置plete int本od使ction to Min成GoRTS");
    Path1.Cate成o本y = EG使idanceCate成o本y::T使to本ial;
    Path1.Ta本成etLe正el = EG使idanceLe正el::Ele設置enta本y;
    Path1.ContentSeq使ence.Add(TEXT("t使to本ial下001"));
    Path1.ContentSeq使ence.Add(TEXT("t使to本ial下002"));
    Path1.ContentSeq使ence.Add(TEXT("histo本y下001"));
    Path1.Esti設置atedTotal輸入o使本s = 0.5f;
    Path1.TotalExpe本iencePoints = 45;
    Path1.Ce本tificate的a設置e = TEXT("Min成GoRTS Be成inne本 Ce本tificate");
    AddLea本nin成Path(Path1);
    
    軍Lea本nin成Path Path2;
    Path2.PathID = TEXT("st本ate成y下path");
    Path2.的a設置e = TEXT("St本ate成y Maste本");
    Path2.Desc本iption = TEXT("Maste本 st本ate成ic 成a設置eplay");
    Path2.Cate成o本y = EG使idanceCate成o本y::St本ate成yG使ide;
    Path2.Ta本成etLe正el = EG使idanceLe正el::Ad正anced;
    Path2.ContentSeq使ence.Add(TEXT("t使to本ial下002"));
    Path2.ContentSeq使ence.Add(TEXT("st本ate成y下001"));
    Path2.Esti設置atedTotal輸入o使本s = 1.0f;
    Path2.TotalExpe本iencePoints = 50;
    Path2.Ce本tificate的a設置e = TEXT("St本ate成y Maste本 Ce本tificate");
    AddLea本nin成Path(Path2);
}

正oid UMin成Uni正e本sityG使idanceSyste設置::InitializeDefa使ltQ使izzes()
{
    軍Q使izQ使estion Q1;
    Q1.Q使estionID = TEXT("q使iz下001");
    Q1.Q使estion = TEXT("基本hat e本a is Min成GoRTS set in基本");
    Q1.Options.Add(TEXT("Qin成 Dynasty"));
    Q1.Options.Add(TEXT("Rep使blican E本a"));
    Q1.Options.Add(TEXT("Min成 Dynasty"));
    Q1.Options.Add(TEXT("Mode本n Day"));
    Q1.Co本本ectAnswe本Index = 1;
    Q1.Explanation = TEXT("Min成GoRTS is set in the Rep使blican E本a of Chinese histo本y (1912-1949)");
    Q1.Cate成o本y = EG使idanceCate成o本y::輸入isto本icalContext;
    Q1.Diffic使lty = 1;
    Q1.Points = 10;
    AddQ使izQ使estion(Q1);
    
    軍Q使izQ使estion Q2;
    Q2.Q使estionID = TEXT("q使iz下002");
    Q2.Q使estion = TEXT("基本hich of the followin成 is a basic RTS st本ate成y基本");
    Q2.Options.Add(TEXT("Reso使本ce 成athe本in成"));
    Q2.Options.Add(TEXT("Unit 設置ic本o-設置ana成e設置ent"));
    Q2.Options.Add(TEXT("Base b使ildin成"));
    Q2.Options.Add(TEXT("All of the abo正e"));
    Q2.Co本本ectAnswe本Index = 3;
    Q2.Explanation = TEXT("All options a本e f使nda設置ental aspects of RTS 成a設置eplay");
    Q2.Cate成o本y = EG使idanceCate成o本y::St本ate成yG使ide;
    Q2.Diffic使lty = 2;
    Q2.Points = 15;
    AddQ使izQ使estion(Q2);
}

正oid UMin成Uni正e本sityG使idanceSyste設置::SendRe設置inde本的otification()
{
    OnSt使dyRe設置inde本.B本oadcast();
}

正oid UMin成Uni正e本sityG使idanceSyste設置::UnlockP本e本eq使isiteContent(const 軍St本in成& St使dentID, const 軍St本in成& Co設置pletedContentID)
{
    fo本 (const a使to& Pai本 : G使idanceContents)
    {
        if (Pai本.Val使e.P本e本eq使isites.Contains(Co設置pletedContentID))
        {
            bool bAllP本e本eqsMet = t本使e;
            
            fo本 (const 軍St本in成& P本e本eq : Pai本.Val使e.P本e本eq使isites)
            {
                if (!St使dentP本o成本ess.Contains(St使dentID) 
                    !St使dentP本o成本ess[St使dentID].Contains(P本e本eq) 
                    St使dentP本o成本ess[St使dentID][P本e本eq].Stat使s != EP本o成本essStat使s::Co設置pleted)
                {
                    bAllP本e本eqsMet = false;
                    b本eak;
                }
            }
            
            if (bAllP本e本eqsMet && St使dentP本o成本ess.Contains(St使dentID))
            {
                if (!St使dentP本o成本ess[St使dentID].Contains(Pai本.Key))
                {
                    軍G使idanceP本o成本ess 的ewP本o成本ess;
                    的ewP本o成本ess.ContentID = Pai本.Key;
                    的ewP本o成本ess.Stat使s = EP本o成本essStat使s::A正ailable;
                    St使dentP本o成本ess[St使dentID].Add(Pai本.Key, 的ewP本o成本ess);
                }
            }
        }
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::CheckPathCo設置pletion(const 軍St本in成& St使dentID, const 軍St本in成& ContentID)
{
    fo本 (const a使to& Pai本 : Lea本nin成Paths)
    {
        if (Pai本.Val使e.ContentSeq使ence.Contains(ContentID))
        {
            float P本o成本ess = GetPathP本o成本ess軍o本St使dent(St使dentID, Pai本.Key);
            
            if (P本o成本ess >= 100.0f)
            {
                Co設置pleteLea本nin成Path(St使dentID, Pai本.Key);
            }
        }
    }
}

正oid UMin成Uni正e本sityG使idanceSyste設置::E正al使ateAchie正e設置ents(const 軍St本in成& St使dentID)
{
    if (!St使dentP本ofiles.Contains(St使dentID))
    {
        本et使本n;
    }
    
    軍St使dentP本ofile& P本ofile = St使dentP本ofiles[St使dentID];
    
    if (P本ofile.Co設置pletedContentCo使nt >= 1)
    {
        Awa本dAchie正e設置ent(St使dentID, TEXT("fi本st下step"), TEXT("軍i本st Steps"));
    }
    
    if (P本ofile.Co設置pletedContentCo使nt >= 10)
    {
        Awa本dAchie正e設置ent(St使dentID, TEXT("dedicated下lea本ne本"), TEXT("Dedicated Lea本ne本"));
    }
    
    if (P本ofile.TotalExpe本iencePoints >= 100)
    {
        Awa本dAchie正e設置ent(St使dentID, TEXT("xp下100"), TEXT("Cent使本y Cl使b"));
    }
}

bool UMin成Uni正e本sityG使idanceSyste設置::IsContentUnlocked(const 軍St本in成& St使dentID, const 軍St本in成& ContentID)
{
    if (!G使idanceContents.Contains(ContentID))
    {
        本et使本n false;
    }
    
    軍G使idanceContent Content = G使idanceContents[ContentID];
    
    if (Content.P本e本eq使isites.的使設置() == 0)
    {
        本et使本n t本使e;
    }
    
    if (!St使dentP本o成本ess.Contains(St使dentID))
    {
        本et使本n false;
    }
    
    TMap<軍St本in成, 軍G使idanceP本o成本ess>& P本o成本essMap = St使dentP本o成本ess[St使dentID];
    
    fo本 (const 軍St本in成& P本e本eq : Content.P本e本eq使isites)
    {
        if (!P本o成本essMap.Contains(P本e本eq)  P本o成本essMap[P本e本eq].Stat使s != EP本o成本essStat使s::Co設置pleted)
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

TA本本ay<軍G使idanceContent> UMin成Uni正e本sityG使idanceSyste設置::GetA正ailableContent(const 軍St本in成& St使dentID)
{
    TA本本ay<軍G使idanceContent> Res使lt;
    
    fo本 (const a使to& Pai本 : G使idanceContents)
    {
        if (IsContentUnlocked(St使dentID, Pai本.Key))
        {
            bool bAl本eadyCo設置pleted = false;
            
            if (St使dentP本o成本ess.Contains(St使dentID) && St使dentP本o成本ess[St使dentID].Contains(Pai本.Key))
            {
                if (St使dentP本o成本ess[St使dentID][Pai本.Key].Stat使s == EP本o成本essStat使s::Co設置pleted)
                {
                    bAl本eadyCo設置pleted = t本使e;
                }
            }
            
            if (!bAl本eadyCo設置pleted)
            {
                Res使lt.Add(Pai本.Val使e);
            }
        }
    }
    
    本et使本n Res使lt;
}

EG使idanceLe正el UMin成Uni正e本sityG使idanceSyste設置::Calc使lateLe正el軍本o設置XP(int32 TotalXP)
{
    if (TotalXP >= 500)
    {
        本et使本n EG使idanceLe正el::Expe本t;
    }
    else if (TotalXP >= 300)
    {
        本et使本n EG使idanceLe正el::Ad正anced;
    }
    else if (TotalXP >= 150)
    {
        本et使本n EG使idanceLe正el::Inte本設置ediate;
    }
    else if (TotalXP >= 50)
    {
        本et使本n EG使idanceLe正el::Ele設置enta本y;
    }
    
    本et使本n EG使idanceLe正el::Be成inne本;
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Sa正eP本o成本ess(const 軍St本in成& St使dentID)
{
}

正oid UMin成Uni正e本sityG使idanceSyste設置::LoadP本o成本ess(const 軍St本in成& St使dentID)
{
}

正oid UMin成Uni正e本sityG使idanceSyste設置::Lo成G使idance(const 軍St本in成& Messa成e)
{
    UE下LOG(Lo成G使idance, Lo成, TEXT("[G使idance] %s"), *Messa成e);
}

UMin成Uni正e本sityG使idanceSyste設置* UMin成Uni正e本sityG使idanceSyste設置::Get(UOb大ect* 基本o本ldContextOb大ect)
{
    if (U基本o本ld* 基本o本ld = GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(基本o本ldContextOb大ect, EGet基本o本ldE本本o本Mode::Ret使本n的使ll))
    {
        static UMin成Uni正e本sityG使idanceSyste設置* Instance = n使llpt本;
        if (!Instance)
        {
            Instance = 的ewOb大ect<UMin成Uni正e本sityG使idanceSyste設置>();
        }
        本et使本n Instance;
    }
    本et使本n n使llpt本;
}
