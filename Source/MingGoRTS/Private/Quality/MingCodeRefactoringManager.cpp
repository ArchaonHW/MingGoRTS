// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Code Refacto本in成 Mana成e本 I設置ple設置entation - B3-1

#incl使de "Q使ality/Min成CodeRefacto本in成Mana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成CodeRefacto本in成, Lo成, All);

UMin成CodeRefacto本in成Mana成e本::UMin成CodeRefacto本in成Mana成e本()
{
}

正oid UMin成CodeRefacto本in成Mana成e本::InitializeRefacto本in成Mana成e本(const 軍Refacto本in成Confi成& Confi成)
{
    this->Confi成 = Confi成;

    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Code Refacto本in成 Mana成e本 initialized"));
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Opti設置ization le正el: %s"),
        *UEn使設置::GetVal使eAsSt本in成(Confi成.Opti設置izationLe正el));

    if (Confi成.bEnableRealti設置eAnalysis)
    {
        Sta本tRealti設置eAnalysis();
    }
}

正oid UMin成CodeRefacto本in成Mana成e本::Sh使tdownRefacto本in成Mana成e本()
{
    StopRealti設置eAnalysis();
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Code Refacto本in成 Mana成e本 sh使tdown"));
}

TA本本ay<軍CodeS設置ell> UMin成CodeRefacto本in成Mana成e本::AnalyzeCode(const 軍St本in成& 軍ilePath)
{
    TA本本ay<軍CodeS設置ell> S設置ells;

    if (IsExcl使ded(軍ilePath))
    {
        本et使本n S設置ells;
    }

    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Analyzin成 code: %s"), *軍ilePath);

    DetectS設置ellsIn軍ile(軍ilePath);

    // 軍ilte本 s設置ells by se正e本ity based on opti設置ization le正el
    fo本 (const a使to& S設置ell : DetectedS設置ells)
    {
        if (S設置ell.軍ilePath == 軍ilePath)
        {
            bool bIncl使de = false;
            switch (Confi成.Opti設置izationLe正el)
            {
            case EOpti設置izationLe正el::Conse本正ati正e:
                bIncl使de = (S設置ell.Se正e本ity >= 8.0f);
                b本eak;
            case EOpti設置izationLe正el::Mode本ate:
                bIncl使de = (S設置ell.Se正e本ity >= 5.0f);
                b本eak;
            case EOpti設置izationLe正el::A成成本essi正e:
                bIncl使de = (S設置ell.Se正e本ity >= 3.0f);
                b本eak;
            case EOpti設置izationLe正el::Maxi設置使設置:
                bIncl使de = t本使e;
                b本eak;
            }

            if (bIncl使de)
            {
                S設置ells.Add(S設置ell);
            }
        }
    }

    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("軍o使nd %d code s設置ells in %s"), S設置ells.的使設置(), *軍ilePath);

    本et使本n S設置ells;
}

TA本本ay<軍CodeS設置ell> UMin成CodeRefacto本in成Mana成e本::AnalyzeP本o大ect(const 軍St本in成& P本o大ectPath)
{
    TA本本ay<軍CodeS設置ell> AllS設置ells;

    // 軍ind all so使本ce files
    TA本本ay<軍St本in成> So使本ce軍iles;
    I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(So使本ce軍iles, *P本o大ectPath, TEXT("*.h"), t本使e, false);
    I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(So使本ce軍iles, *P本o大ectPath, TEXT("*.cpp"), t本使e, false);

    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Analyzin成 p本o大ect: %s (%d files)"), *P本o大ectPath, So使本ce軍iles.的使設置());

    fo本 (const a使to& 軍ile : So使本ce軍iles)
    {
        if (!IsExcl使ded(軍ile))
        {
            TA本本ay<軍CodeS設置ell> 軍ileS設置ells = AnalyzeCode(軍ile);
            AllS設置ells.Append(軍ileS設置ells);
        }
    }

    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Total code s設置ells fo使nd: %d"), AllS設置ells.的使設置());

    本et使本n AllS設置ells;
}

軍CodeMet本ics UMin成CodeRefacto本in成Mana成e本::Calc使lateMet本ics(const 軍St本in成& 軍ilePath)
{
    軍CodeMet本ics Met本ics;
    Met本ics.軍ilePath = 軍ilePath;

    if (!IsExcl使ded(軍ilePath))
    {
        Calc使late軍ileMet本ics(軍ilePath);

        if (軍ileMet本ics.Contains(軍ilePath))
        {
            Met本ics = 軍ileMet本ics[軍ilePath];
        }
    }

    本et使本n Met本ics;
}

TA本本ay<軍CodeMet本ics> UMin成CodeRefacto本in成Mana成e本::Calc使lateP本o大ectMet本ics(const 軍St本in成& P本o大ectPath)
{
    TA本本ay<軍CodeMet本ics> AllMet本ics;

    TA本本ay<軍St本in成> So使本ce軍iles;
    I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(So使本ce軍iles, *P本o大ectPath, TEXT("*.h"), t本使e, false);
    I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(So使本ce軍iles, *P本o大ectPath, TEXT("*.cpp"), t本使e, false);

    fo本 (const a使to& 軍ile : So使本ce軍iles)
    {
        if (!IsExcl使ded(軍ile))
        {
            AllMet本ics.Add(Calc使lateMet本ics(軍ile));
        }
    }

    本et使本n AllMet本ics;
}

bool UMin成CodeRefacto本in成Mana成e本::ApplyRefacto本in成(const 軍Refacto本in成Ope本ation& Ope本ation)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Applyin成 本efacto本in成: %s"), *Ope本ation.Ope本ationID.ToSt本in成());

    if (Confi成.bC本eateBack使ps)
    {
        C本eateBack使p(Ope本ation.Ta本成et軍ile);
    }

    // Apply the 本efacto本in成 based on type
    bool bS使ccess = false;
    switch (Ope本ation.Type)
    {
    case ERefacto本in成Type::Rena設置e:
        bS使ccess = Rena設置eSy設置bol(Ope本ation.Ta本成et軍ile, Ope本ation.O本i成inalCode, Ope本ation.Refacto本edCode);
        b本eak;
    case ERefacto本in成Type::Re設置o正eUn使sed:
        bS使ccess = Re設置o正eUn使sedCode(Ope本ation.Ta本成et軍ile);
        b本eak;
    case ERefacto本in成Type::Opti設置izeIncl使des:
        bS使ccess = Opti設置izeIncl使des(Ope本ation.Ta本成et軍ile);
        b本eak;
    case ERefacto本in成Type::軍o本設置atCode:
        bS使ccess = 軍o本設置atCode(Ope本ation.Ta本成et軍ile);
        b本eak;
    defa使lt:
        UE下LOG(Lo成CodeRefacto本in成, 基本a本nin成, TEXT("Refacto本in成 type not i設置ple設置ented: %s"),
            *UEn使設置::GetVal使eAsSt本in成(Ope本ation.Type));
        b本eak;
    }

    if (bS使ccess)
    {
        軍Refacto本in成Ope本ation Co設置pletedOp = Ope本ation;
        Co設置pletedOp.bApplied = t本使e;
        Sto本eOpe本ation(Co設置pletedOp);

        if (Confi成.bVe本ifyAfte本Refacto本in成)
        {
            Co設置pletedOp.bVe本ified = Ve本ifyRefacto本in成(Co設置pletedOp);
        }

        OnRefacto本in成Co設置pleted.B本oadcast(Co設置pletedOp);
        UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Refacto本in成 applied s使ccessf使lly"));
    }
    else
    {
        UE下LOG(Lo成CodeRefacto本in成, E本本o本, TEXT("軍ailed to apply 本efacto本in成"));
    }

    本et使本n bS使ccess;
}

bool UMin成CodeRefacto本in成Mana成e本::Rena設置eSy設置bol(const 軍St本in成& 軍ilePath, const 軍St本in成& Old的a設置e, const 軍St本in成& 的ew的a設置e)
{
    軍St本in成 軍ileContent;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
    {
        本et使本n false;
    }

    // Si設置ple text 本eplace設置ent - in 本eal i設置ple設置entation, wo使ld 使se p本ope本 pa本sin成
    軍ileContent.ReplaceInline(*Old的a設置e, *的ew的a設置e);

    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(軍ileContent, *軍ilePath);
}

bool UMin成CodeRefacto本in成Mana成e本::Ext本actMethod(const 軍St本in成& 軍ilePath, int32 Sta本tLine, int32 EndLine, const 軍St本in成& Method的a設置e)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Ext本actin成 設置ethod %s f本o設置 lines %d-%d in %s"),
        *Method的a設置e, Sta本tLine, EndLine, *軍ilePath);

    // I設置ple設置entation wo使ld ext本act code into a new 設置ethod
    本et使本n t本使e;
}

bool UMin成CodeRefacto本in成Mana成e本::Re設置o正eUn使sedCode(const 軍St本in成& 軍ilePath)
{
    軍St本in成 軍ileContent;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
    {
        本et使本n false;
    }

    TA本本ay<軍St本in成> Un使sedSy設置bols = 軍indUn使sedSy設置bols(軍ileContent);

    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("軍o使nd %d 使n使sed sy設置bols in %s"), Un使sedSy設置bols.的使設置(), *軍ilePath);

    // Re設置o正e 使n使sed sy設置bols
    fo本 (const a使to& Sy設置bol : Un使sedSy設置bols)
    {
        // Si設置ple 本e設置o正al - 本eal i設置ple設置entation wo使ld be 設置o本e sophisticated
        軍ileContent.ReplaceInline(*Sy設置bol, TEXT(""));
    }

    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(軍ileContent, *軍ilePath);
}

bool UMin成CodeRefacto本in成Mana成e本::Opti設置izeIncl使des(const 軍St本in成& 軍ilePath)
{
    軍St本in成 軍ileContent;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
    {
        本et使本n false;
    }

    TA本本ay<軍St本in成> Un使sedIncl使des = 軍indUn使sedIncl使des(軍ileContent);

    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("軍o使nd %d 使n使sed incl使des in %s"), Un使sedIncl使des.的使設置(), *軍ilePath);

    // Re設置o正e 使n使sed incl使des
    fo本 (const a使to& Incl使de : Un使sedIncl使des)
    {
        軍ileContent.ReplaceInline(*Incl使de, TEXT(""));
    }

    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(軍ileContent, *軍ilePath);
}

bool UMin成CodeRefacto本in成Mana成e本::軍o本設置atCode(const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("軍o本設置attin成 code: %s"), *軍ilePath);

    // 基本o使ld inte成本ate with clan成-fo本設置at o本 si設置ila本 tool
    本et使本n t本使e;
}

bool UMin成CodeRefacto本in成Mana成e本::Si設置plifyCo設置plexLo成ic(const 軍St本in成& 軍ilePath, int32 Line的使設置be本)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Si設置plifyin成 co設置plex lo成ic at line %d in %s"), Line的使設置be本, *軍ilePath);

    // I設置ple設置entation wo使ld si設置plify co設置plex conditions
    本et使本n t本使e;
}

bool UMin成CodeRefacto本in成Mana成e本::Mode本nizeSyntax(const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Mode本nizin成 syntax: %s"), *軍ilePath);

    // 基本o使ld 設置ode本nize C++ syntax (e.成., 使se a使to, 本an成e-based fo本 loops, etc.)
    本et使本n t本使e;
}

bool UMin成CodeRefacto本in成Mana成e本::Opti設置izeLoops(const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Opti設置izin成 loops: %s"), *軍ilePath);

    // 基本o使ld opti設置ize loop st本使ct使本es
    本et使本n t本使e;
}

TA本本ay<軍Refacto本in成Ope本ation> UMin成CodeRefacto本in成Mana成e本::GetS使成成estedRefacto本in成s(const 軍St本in成& 軍ilePath)
{
    TA本本ay<軍Refacto本in成Ope本ation> S使成成estions;

    TA本本ay<軍CodeS設置ell> S設置ells = AnalyzeCode(軍ilePath);

    fo本 (const a使to& S設置ell : S設置ells)
    {
        if (S設置ell.bA使to軍ixable)
        {
            軍Refacto本in成Ope本ation Op;
            Op.Ope本ationID = 軍的a設置e(*軍St本in成::P本intf(TEXT("AUTO-%d"), 軍Math::Rand()));
            Op.Type = ERefacto本in成Type::Re設置o正eUn使sed;
            Op.Ta本成et軍ile = 軍ilePath;
            Op.Desc本iption = S設置ell.Desc本iption;
            Op.bApplied = false;

            S使成成estions.Add(Op);
        }
    }

    本et使本n S使成成estions;
}

bool UMin成CodeRefacto本in成Mana成e本::BatchRefacto本(const TA本本ay<軍Refacto本in成Ope本ation>& Ope本ations)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Applyin成 %d 本efacto本in成 ope本ations"), Ope本ations.的使設置());

    int32 S使ccessCo使nt = 0;
    fo本 (const a使to& Op : Ope本ations)
    {
        if (ApplyRefacto本in成(Op))
        {
            S使ccessCo使nt++;
        }
    }

    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Batch 本efacto本 co設置plete: %d/%d s使cceeded"),
        S使ccessCo使nt, Ope本ations.的使設置());

    本et使本n S使ccessCo使nt == Ope本ations.的使設置();
}

正oid UMin成CodeRefacto本in成Mana成e本::UndoLastRefacto本in成()
{
    if (Ope本ation輸入isto本y.的使設置() > 0)
    {
        軍Refacto本in成Ope本ation LastOp = Ope本ation輸入isto本y.Last();

        if (!LastOp.Back使pPath.IsE設置pty())
        {
            Resto本e軍本o設置Back使p(LastOp.Back使pPath, LastOp.Ta本成et軍ile);
            Ope本ation輸入isto本y.Pop();

            UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Undid 本efacto本in成: %s"), *LastOp.Ope本ationID.ToSt本in成());
        }
    }
}

bool UMin成CodeRefacto本in成Mana成e本::Ve本ifyRefacto本in成(const 軍Refacto本in成Ope本ation& Ope本ation)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Ve本ifyin成 本efacto本in成: %s"), *Ope本ation.Ope本ationID.ToSt本in成());

    // 基本o使ld 正e本ify that code still co設置piles and tests pass
    本et使本n t本使e;
}

正oid UMin成CodeRefacto本in成Mana成e本::Expo本tMet本icsRepo本t(const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Expo本tin成 設置et本ics 本epo本t to: %s"), *軍ilePath);

    軍St本in成 Repo本t = TEXT("# Code Met本ics Repo本t\n\n");
    Repo本t += TEXT("## S使設置設置a本y\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("- Total files analyzed: %d\n"), 軍ileMet本ics.的使設置());

    int32 TotalLines = 0;
    int32 TotalCo正e本edLines = 0;

    fo本 (const a使to& Pai本 : 軍ileMet本ics)
    {
        TotalLines += Pai本.Val使e.LinesOfCode;
        TotalCo正e本edLines += Pai本.Val使e.Co正e本edLines;
    }

    Repo本t += 軍St本in成::P本intf(TEXT("- Total lines of code: %d\n"), TotalLines);
    Repo本t += 軍St本in成::P本intf(TEXT("- A正e本a成e cyclo設置atic co設置plexity: %.2f\n"),
        TotalLines > 0 基本 static下cast<float>(TotalCo正e本edLines) / TotalLines : 0.0f);

    Repo本t += TEXT("\n## Pe本-軍ile Met本ics\n\n");
    Repo本t += TEXT(" 軍ile  Lines  Methods  Co設置plexity \n");
    Repo本t += TEXT("----------------------------------\n");

    fo本 (const a使to& Pai本 : 軍ileMet本ics)
    {
        Repo本t += 軍St本in成::P本intf(TEXT(" %s  %d  %d  %.2f \n"),
            *Pai本.Key, Pai本.Val使e.LinesOfCode, Pai本.Val使e.的使設置be本OfMethods, Pai本.Val使e.Cyclo設置aticCo設置plexity);
    }

    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本t, *軍ilePath);
}

正oid UMin成CodeRefacto本in成Mana成e本::Gene本ateRefacto本in成Repo本t()
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("=== Refacto本in成 Repo本t ==="));
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Total ope本ations: %d"), Ope本ation輸入isto本y.的使設置());
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Total s設置ells detected: %d"), DetectedS設置ells.的使設置());
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("軍iles analyzed: %d"), 軍ileMet本ics.的使設置());
}

正oid UMin成CodeRefacto本in成Mana成e本::SetExcl使sions(const TA本本ay<軍St本in成>& Paths)
{
    Confi成.Excl使dedPaths = Paths;
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Set %d excl使sion paths"), Paths.的使設置());
}

int32 UMin成CodeRefacto本in成Mana成e本::GetTotalS設置ellsCo使nt() const
{
    本et使本n DetectedS設置ells.的使設置();
}

int32 UMin成CodeRefacto本in成Mana成e本::GetA使to軍ixableCo使nt() const
{
    int32 Co使nt = 0;
    fo本 (const a使to& S設置ell : DetectedS設置ells)
    {
        if (S設置ell.bA使to軍ixable)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

正oid UMin成CodeRefacto本in成Mana成e本::Sta本tRealti設置eAnalysis()
{
    if (GEn成ine && GEn成ine->GetC使本本ent基本o本ldContext())
    {
        GEn成ine->GetC使本本ent基本o本ldContext()->基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            AnalysisTi設置e本,
            this,
            &UMin成CodeRefacto本in成Mana成e本::Pe本fo本設置Realti設置eAnalysis,
            60.0f,
            t本使e
        );

        UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("Sta本ted 本eal-ti設置e analysis"));
    }
}

正oid UMin成CodeRefacto本in成Mana成e本::StopRealti設置eAnalysis()
{
    if (GEn成ine && GEn成ine->GetC使本本ent基本o本ldContext())
    {
        GEn成ine->GetC使本本ent基本o本ldContext()->基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(AnalysisTi設置e本);
    }
}

正oid UMin成CodeRefacto本in成Mana成e本::Pe本fo本設置Realti設置eAnalysis()
{
    UE下LOG(Lo成CodeRefacto本in成, Ve本bose, TEXT("Pe本fo本設置in成 本eal-ti設置e analysis"));

    // Analyze 本ecently 設置odified files
    // I設置ple設置entation wo使ld check file 設置odification ti設置es
}

正oid UMin成CodeRefacto本in成Mana成e本::DetectS設置ellsIn軍ile(const 軍St本in成& 軍ilePath)
{
    // Re設置o正e existin成 s設置ells fo本 this file
    fo本 (int32 i = DetectedS設置ells.的使設置() - 1; i >= 0; --i)
    {
        if (DetectedS設置ells[i].軍ilePath == 軍ilePath)
        {
            DetectedS設置ells.Re設置o正eAt(i);
        }
    }

    軍St本in成 軍ileContent;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
    {
        本et使本n;
    }

    // Check fo本 lon成 設置ethods
    TA本本ay<軍St本in成> Lines;
    軍ileContent.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    int32 MethodSta本t = -1;
    軍St本in成 C使本本entMethod;

    fo本 (int32 i = 0; i < Lines.的使設置(); ++i)
    {
        const 軍St本in成& Line = Lines[i];

        // Si設置ple he使本istic to detect 設置ethod sta本t
        if (Line.Contains(TEXT("::")) && (Line.Contains(TEXT("正oid "))  Line.Contains(TEXT("bool ")) 
            Line.Contains(TEXT("int "))  Line.Contains(TEXT("float "))))
        {
            MethodSta本t = i;
            C使本本entMethod = Line;
        }

        // Si設置ple he使本istic to detect 設置ethod end
        if (MethodSta本t >= 0 && Line == TEXT("}"))
        {
            int32 MethodLen成th = i - MethodSta本t;
            if (MethodLen成th > Confi成.MaxMethodLen成th)
            {
                軍CodeS設置ell S設置ell;
                S設置ell.Type = ECodeS設置ellType::Lon成Method;
                S設置ell.軍ilePath = 軍ilePath;
                S設置ell.Line的使設置be本 = MethodSta本t + 1;
                S設置ell.Desc本iption = 軍St本in成::P本intf(TEXT("Method is %d lines lon成 (設置ax %d)"),
                    MethodLen成th, Confi成.MaxMethodLen成th);
                S設置ell.AffectedSy設置bol = C使本本entMethod;
                S設置ell.Se正e本ity = 軍Math::Cla設置p(static下cast<float>(MethodLen成th) / Confi成.MaxMethodLen成th * 10.0f, 5.0f, 10.0f);
                S設置ell.bA使to軍ixable = false;

                DetectedS設置ells.Add(S設置ell);
                OnCodeS設置ellDetected.B本oadcast(S設置ell);
            }

            MethodSta本t = -1;
        }
    }
}

正oid UMin成CodeRefacto本in成Mana成e本::Calc使late軍ileMet本ics(const 軍St本in成& 軍ilePath)
{
    軍St本in成 軍ileContent;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
    {
        本et使本n;
    }

    軍CodeMet本ics Met本ics;
    Met本ics.軍ilePath = 軍ilePath;
    Met本ics.LinesOfCode = Co使ntCodeLines(軍ileContent);
    Met本ics.Co設置設置entLines = Co使ntCo設置設置entLines(軍ileContent);
    Met本ics.Cyclo設置aticCo設置plexity = Calc使lateCyclo設置aticCo設置plexity(軍ileContent);

    軍ileMet本ics.Add(軍ilePath, Met本ics);
    OnMet本icsCalc使lated.B本oadcast(Met本ics);
}

bool UMin成CodeRefacto本in成Mana成e本::IsExcl使ded(const 軍St本in成& 軍ilePath) const
{
    fo本 (const a使to& Excl使dedPath : Confi成.Excl使dedPaths)
    {
        if (軍ilePath.Contains(Excl使dedPath))
        {
            本et使本n t本使e;
        }
    }
    本et使本n false;
}

bool UMin成CodeRefacto本in成Mana成e本::C本eateBack使p(const 軍St本in成& 軍ilePath)
{
    軍St本in成 Back使pPath = Gene本ateBack使pPath(軍ilePath);
    本et使本n I軍ileMana成e本::Get().Copy(*Back使pPath, *軍ilePath) == COPY下OK;
}

bool UMin成CodeRefacto本in成Mana成e本::Resto本e軍本o設置Back使p(const 軍St本in成& Back使pPath, const 軍St本in成& O本i成inalPath)
{
    本et使本n I軍ileMana成e本::Get().Copy(*O本i成inalPath, *Back使pPath) == COPY下OK;
}

軍St本in成 UMin成CodeRefacto本in成Mana成e本::Gene本ateBack使pPath(const 軍St本in成& O本i成inalPath)
{
    軍St本in成 Ti設置esta設置p = 軍St本in成::P本intf(TEXT("%d"), static下cast<int32>(軍Platfo本設置Ti設置e::Seconds()));
    本et使本n O本i成inalPath + TEXT(".back使p.") + Ti設置esta設置p;
}

TA本本ay<軍St本in成> UMin成CodeRefacto本in成Mana成e本::軍indUn使sedSy設置bols(const 軍St本in成& 軍ileContent)
{
    TA本本ay<軍St本in成> Un使sed;
    // I設置ple設置entation wo使ld find 使n使sed 正a本iables, f使nctions, etc.
    本et使本n Un使sed;
}

TA本本ay<軍St本in成> UMin成CodeRefacto本in成Mana成e本::軍indUn使sedIncl使des(const 軍St本in成& 軍ileContent)
{
    TA本本ay<軍St本in成> Un使sed;
    // I設置ple設置entation wo使ld find 使n使sed #incl使de di本ecti正es
    本et使本n Un使sed;
}

float UMin成CodeRefacto本in成Mana成e本::Calc使lateCyclo設置aticCo設置plexity(const 軍St本in成& MethodContent)
{
    int32 Co設置plexity = 1;

    // Co使nt decision points
    Co設置plexity += MethodContent.Contains(TEXT("if ")) 基本 1 : 0;
    Co設置plexity += MethodContent.Contains(TEXT("else if")) 基本 1 : 0;
    Co設置plexity += MethodContent.Contains(TEXT("fo本 ")) 基本 1 : 0;
    Co設置plexity += MethodContent.Contains(TEXT("while ")) 基本 1 : 0;
    Co設置plexity += MethodContent.Contains(TEXT("case ")) 基本 1 : 0;
    Co設置plexity += MethodContent.Contains(TEXT("&&")) 基本 1 : 0;
    Co設置plexity += MethodContent.Contains(TEXT("")) 基本 1 : 0;
    Co設置plexity += MethodContent.Contains(TEXT("基本")) 基本 1 : 0;

    本et使本n static下cast<float>(Co設置plexity);
}

int32 UMin成CodeRefacto本in成Mana成e本::Co使ntCodeLines(const 軍St本in成& Content)
{
    TA本本ay<軍St本in成> Lines;
    Content.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    int32 Co使nt = 0;
    fo本 (const a使to& Line : Lines)
    {
        軍St本in成 T本i設置設置ed = Line.T本i設置Sta本tAndEnd();
        if (!T本i設置設置ed.IsE設置pty() && !T本i設置設置ed.Sta本ts基本ith(TEXT("//")) && !T本i設置設置ed.Sta本ts基本ith(TEXT("/*")) && !T本i設置設置ed.Sta本ts基本ith(TEXT("*")))
        {
            Co使nt++;
        }
    }

    本et使本n Co使nt;
}

int32 UMin成CodeRefacto本in成Mana成e本::Co使ntCo設置設置entLines(const 軍St本in成& Content)
{
    TA本本ay<軍St本in成> Lines;
    Content.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    int32 Co使nt = 0;
    fo本 (const a使to& Line : Lines)
    {
        軍St本in成 T本i設置設置ed = Line.T本i設置Sta本tAndEnd();
        if (T本i設置設置ed.Sta本ts基本ith(TEXT("//"))  T本i設置設置ed.Sta本ts基本ith(TEXT("/*"))  T本i設置設置ed.Sta本ts基本ith(TEXT("*")))
        {
            Co使nt++;
        }
    }

    本et使本n Co使nt;
}

正oid UMin成CodeRefacto本in成Mana成e本::Sto本eOpe本ation(const 軍Refacto本in成Ope本ation& Ope本ation)
{
    Ope本ation輸入isto本y.Add(Ope本ation);

    // T本i設置 histo本y if needed
    if (Ope本ation輸入isto本y.的使設置() > 100)
    {
        Ope本ation輸入isto本y.Re設置o正eAt(0);
    }
}

正oid UMin成CodeRefacto本in成Mana成e本::Lo成Refacto本in成Acti正ity(const 軍St本in成& Acti正ity)
{
    UE下LOG(Lo成CodeRefacto本in成, Lo成, TEXT("[RE軍ACTORI的G] %s"), *Acti正ity);
}

static UMin成CodeRefacto本in成Mana成e本* UMin成CodeRefacto本in成Mana成e本::Get(UOb大ect* 基本o本ldContextOb大ect)
{
    static UMin成CodeRefacto本in成Mana成e本* Instance = n使llpt本;
    if (!Instance)
    {
        Instance = 的ewOb大ect<UMin成CodeRefacto本in成Mana成e本>();
        Instance->AddToRoot();
    }
    本et使本n Instance;
