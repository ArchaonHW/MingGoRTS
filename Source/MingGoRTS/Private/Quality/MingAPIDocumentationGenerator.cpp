// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// API Doc使設置entation Gene本ato本 I設置ple設置entation - B3-4

#incl使de "Q使ality/Min成APIDoc使設置entationGene本ato本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成APIDoc, Lo成, All);

UMin成APIDoc使設置entationGene本ato本::UMin成APIDoc使設置entationGene本ato本()
{
}

正oid UMin成APIDoc使設置entationGene本ato本::InitializeGene本ato本(const 軍Doc使設置entationConfi成& Confi成)
{
    this->Confi成 = Confi成;

    UE下LOG(Lo成APIDoc, Lo成, TEXT("API Doc使設置entation Gene本ato本 initialized"));
    UE下LOG(Lo成APIDoc, Lo成, TEXT("O使tp使t fo本設置at: %s"), *UEn使設置::GetVal使eAsSt本in成(Confi成.O使tp使t軍o本設置at));
    UE下LOG(Lo成APIDoc, Lo成, TEXT("O使tp使t path: %s"), *Confi成.O使tp使tPath);

    // Initialize de正elope本 成使ide st本使ct使本e
    De正elope本G使ide.G使ideTitle = 軍St本in成::P本intf(TEXT("%s De正elope本 G使ide"), *Confi成.P本o大ect的a設置e);
    De正elope本G使ide.Int本od使ction = TEXT("基本elco設置e to the de正elope本 成使ide.");
}

正oid UMin成APIDoc使設置entationGene本ato本::Sh使tdownGene本ato本()
{
    CancelSched使ledGene本ation();
    UE下LOG(Lo成APIDoc, Lo成, TEXT("API Doc使設置entation Gene本ato本 sh使tdown"));
}

bool UMin成APIDoc使設置entationGene本ato本::Gene本ateDoc使設置entation()
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 doc使設置entation..."));

    Pa本seSo使本ce軍iles();

    bool bS使ccess = t本使e;

    if (Confi成.Doc使設置entTypes.Contains(EDoc使設置entType::APICode))
    {
        bS使ccess &= Gene本ateAPIDoc使設置entation();
    }

    if (Confi成.Doc使設置entTypes.Contains(EDoc使設置entType::De正elope本G使ide))
    {
        bS使ccess &= Gene本ateDe正elope本G使ide();
    }

    if (Confi成.Doc使設置entTypes.Contains(EDoc使設置entType::Use本Man使al))
    {
        bS使ccess &= Gene本ateUse本Man使al();
    }

    if (bS使ccess)
    {
        OnDoc使設置entationGene本ated.B本oadcast(Confi成.O使tp使tPath);
        UE下LOG(Lo成APIDoc, Lo成, TEXT("Doc使設置entation 成ene本ated s使ccessf使lly"));
    }
    else
    {
        UE下LOG(Lo成APIDoc, E本本o本, TEXT("Doc使設置entation 成ene本ation failed"));
    }

    本et使本n bS使ccess;
}

bool UMin成APIDoc使設置entationGene本ato本::Gene本ateAPIDoc使設置entation()
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 API doc使設置entation..."));

    軍St本in成 O使tp使tPath = 軍Paths::Co設置bine(Confi成.O使tp使tPath, TEXT("API"));
    I軍ileMana成e本::Get().MakeDi本ecto本y(*O使tp使tPath, t本使e);

    switch (Confi成.O使tp使t軍o本設置at)
    {
    case EDoc使設置entation軍o本設置at::Ma本kdown:
        Gene本ateMa本kdownO使tp使t(O使tp使tPath);
        b本eak;
    case EDoc使設置entation軍o本設置at::輸入TML:
        Gene本ate輸入TMLO使tp使t(O使tp使tPath);
        b本eak;
    case EDoc使設置entation軍o本設置at::PD軍:
        Gene本atePD軍O使tp使t(O使tp使tPath);
        b本eak;
    case EDoc使設置entation軍o本設置at::XML:
        Gene本ateXMLO使tp使t(O使tp使tPath);
        b本eak;
    case EDoc使設置entation軍o本設置at::JSO的:
        Gene本ateJSO的O使tp使t(O使tp使tPath);
        b本eak;
    case EDoc使設置entation軍o本設置at::Doxy成en:
        Gene本ateDoxy成enO使tp使t(O使tp使tPath);
        b本eak;
    defa使lt:
        Gene本ateMa本kdownO使tp使t(O使tp使tPath);
        b本eak;
    }

    if (Confi成.bGene本ateIndex)
    {
        C本eateIndex軍ile(O使tp使tPath);
    }

    UE下LOG(Lo成APIDoc, Lo成, TEXT("API doc使設置entation 成ene本ated at: %s"), *O使tp使tPath);

    本et使本n t本使e;
}

bool UMin成APIDoc使設置entationGene本ato本::Gene本ateDe正elope本G使ide()
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 de正elope本 成使ide..."));

    軍St本in成 O使tp使tPath = 軍Paths::Co設置bine(Confi成.O使tp使tPath, TEXT("De正elope本G使ide.設置d"));

    Gene本ateDe正elope本G使ideMa本kdown(O使tp使tPath);

    UE下LOG(Lo成APIDoc, Lo成, TEXT("De正elope本 成使ide 成ene本ated at: %s"), *O使tp使tPath);

    本et使本n t本使e;
}

bool UMin成APIDoc使設置entationGene本ato本::Gene本ateUse本Man使al()
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 使se本 設置an使al..."));

    軍St本in成 O使tp使tPath = 軍Paths::Co設置bine(Confi成.O使tp使tPath, TEXT("Use本Man使al.設置d"));

    軍St本in成 Content = TEXT("# Use本 Man使al\n\n");
    Content += 軍St本in成::P本intf(TEXT("## %s 正%s\n\n"), *Confi成.P本o大ect的a設置e, *Confi成.P本o大ectVe本sion);
    Content += TEXT("### Table of Contents\n\n");
    Content += TEXT("1. Gettin成 Sta本ted\n");
    Content += TEXT("2. Installation\n");
    Content += TEXT("3. Basic Usa成e\n");
    Content += TEXT("4. Ad正anced 軍eat使本es\n");
    Content += TEXT("5. T本o使bleshootin成\n\n");

    Content += TEXT("## Gettin成 Sta本ted\n\n");
    Content += TEXT("基本elco設置e to Min成GoRTS! This 成使ide will help yo使 成et sta本ted.\n\n");

    Content += TEXT("## Installation\n\n");
    Content += TEXT("1. Download the latest 本elease\n");
    Content += TEXT("2. R使n the installe本\n");
    Content += TEXT("3. 軍ollow the set使p wiza本d\n\n");

    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Content, *O使tp使tPath);

    UE下LOG(Lo成APIDoc, Lo成, TEXT("Use本 設置an使al 成ene本ated at: %s"), *O使tp使tPath);

    本et使本n t本使e;
}

正oid UMin成APIDoc使設置entationGene本ato本::AddDoc使設置entSection(const 軍Doc使設置entSection& Section)
{
    De正elope本G使ide.Sections.Add(Section);
    OnG使ideSectionAdded.B本oadcast(Section);

    UE下LOG(Lo成APIDoc, Lo成, TEXT("Added doc使設置ent section: %s"), *Section.SectionTitle);
}

正oid UMin成APIDoc使設置entationGene本ato本::AddCodeExa設置ple(const 軍CodeExa設置ple& Exa設置ple)
{
    CodeExa設置ples.Add(Exa設置ple);
    De正elope本G使ide.Exa設置ples.Add(Exa設置ple);
    OnExa設置pleAdded.B本oadcast(Exa設置ple);

    UE下LOG(Lo成APIDoc, Lo成, TEXT("Added code exa設置ple: %s"), *Exa設置ple.Title);
}

軍APIDoc使設置entation UMin成APIDoc使設置entationGene本ato本::Ext本actDoc使設置entation(const 軍St本in成& 軍ilePath, const 軍St本in成& Sy設置bol的a設置e)
{
    軍St本in成 軍ileContent;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
    {
        本et使本n 軍APIDoc使設置entation();
    }

    軍APIDoc使設置entation Doc;
    Doc.Sy設置bol的a設置e = Sy設置bol的a設置e;
    Doc.軍ilePath = 軍ilePath;
    Doc.Desc本iption = TEXT("Doc使設置entation ext本acted f本o設置 so使本ce code.");

    // Ext本act co設置設置ents abo正e the sy設置bol
    TA本本ay<軍St本in成> Lines;
    軍ileContent.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    fo本 (int32 i = 0; i < Lines.的使設置(); ++i)
    {
        if (Lines[i].Contains(Sy設置bol的a設置e))
        {
            Doc.Line的使設置be本 = i + 1;
            軍St本in成 Co設置設置ents = Ext本actCo設置設置ents(軍ileContent, i);
            Doc.Desc本iption = Pa本seDesc本iption(Co設置設置ents);
            Doc.DetailedDesc本iption = Pa本seDetailedDesc本iption(Co設置設置ents);
            Doc.Pa本a設置ete本s = Pa本sePa本a設置ete本s(Co設置設置ents);
            Doc.Ret使本nDesc本iption = Pa本seRet使本nDesc本iption(Co設置設置ents);
            Doc.Exa設置ples = Pa本seExa設置ples(Co設置設置ents);
            Doc.SeeAlso = Pa本seSeeAlso(Co設置設置ents);

            // Detect sy設置bol type
            if (Lines[i].Contains(TEXT("class "))  Lines[i].Contains(TEXT("UCLASS")))
            {
                Doc.Type = ESy設置bolType::Class;
            }
            else if (Lines[i].Contains(TEXT("st本使ct "))  Lines[i].Contains(TEXT("USTRUCT")))
            {
                Doc.Type = ESy設置bolType::St本使ct;
            }
            else if (Lines[i].Contains(TEXT("en使設置 "))  Lines[i].Contains(TEXT("UE的UM")))
            {
                Doc.Type = ESy設置bolType::En使設置;
            }
            else if (Lines[i].Contains(TEXT("U軍U的CTIO的"))  (Lines[i].Contains(TEXT("正oid ")) && Lines[i].Contains(TEXT("("))))
            {
                Doc.Type = ESy設置bolType::軍使nction;
            }

            b本eak;
        }
    }

    Doc使設置entedSy設置bols.Add(Doc);
    OnSy設置bolDoc使設置ented.B本oadcast(Doc);

    本et使本n Doc;
}

TA本本ay<軍APIDoc使設置entation> UMin成APIDoc使設置entationGene本ato本::Ext本actAllDoc使設置entation(const 軍St本in成& So使本cePath)
{
    TA本本ay<軍APIDoc使設置entation> AllDocs;

    TA本本ay<軍St本in成> So使本ce軍iles;
    I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(So使本ce軍iles, *So使本cePath, TEXT("*.h"), t本使e, false);

    UE下LOG(Lo成APIDoc, Lo成, TEXT("Ext本actin成 doc使設置entation f本o設置 %d files"), So使本ce軍iles.的使設置());

    int32 P本o成本ess = 0;
    fo本 (const a使to& 軍ile : So使本ce軍iles)
    {
        軍St本in成 軍ileContent;
        if (軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ile))
        {
            // 軍ind all UCLASS, USTRUCT, UE的UM, U軍U的CTIO的 decla本ations
            TA本本ay<軍St本in成> Lines;
            軍ileContent.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

            fo本 (int32 i = 0; i < Lines.的使設置(); ++i)
            {
                if (Lines[i].Contains(TEXT("UCLASS"))  Lines[i].Contains(TEXT("USTRUCT")) 
                    Lines[i].Contains(TEXT("UE的UM"))  Lines[i].Contains(TEXT("U軍U的CTIO的")))
                {
                    // Ext本act sy設置bol na設置e f本o設置 next line
                    if (i + 1 < Lines.的使設置())
                    {
                        軍St本in成 Sy設置bolLine = Lines[i + 1];
                        // Si設置ple pa本sin成 to ext本act na設置e
                        軍St本in成 Sy設置bol的a設置e = Sy設置bolLine.T本i設置Sta本tAndEnd();

                        軍APIDoc使設置entation Doc = Ext本actDoc使設置entation(軍ile, Sy設置bol的a設置e);
                        if (!Doc.Sy設置bol的a設置e.IsE設置pty())
                        {
                            AllDocs.Add(Doc);
                        }
                    }
                }
            }
        }

        P本o成本ess++;
        float Pe本cent = static下cast<float>(P本o成本ess) / So使本ce軍iles.的使設置() * 100.0f;
        的otifyP本o成本ess(Pe本cent);
    }

    UE下LOG(Lo成APIDoc, Lo成, TEXT("Ext本acted %d doc使設置ented sy設置bols"), AllDocs.的使設置());

    本et使本n AllDocs;
}

bool UMin成APIDoc使設置entationGene本ato本::Expo本tTo軍o本設置at(const 軍St本in成& O使tp使tPath, EDoc使設置entation軍o本設置at 軍o本設置at)
{
    EDoc使設置entation軍o本設置at Old軍o本設置at = Confi成.O使tp使t軍o本設置at;
    Confi成.O使tp使t軍o本設置at = 軍o本設置at;

    bool bS使ccess = Gene本ateDoc使設置entation();

    Confi成.O使tp使t軍o本設置at = Old軍o本設置at;

    本et使本n bS使ccess;
}

正oid UMin成APIDoc使設置entationGene本ato本::SetTe設置plate(const 軍St本in成& Te設置platePath)
{
    Confi成.Te設置platePath = Te設置platePath;
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Te設置plate set: %s"), *Te設置platePath);
}

正oid UMin成APIDoc使設置entationGene本ato本::UpdateConfi成使本ation(const 軍Doc使設置entationConfi成& 的ewConfi成)
{
    Confi成 = 的ewConfi成;
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Confi成使本ation 使pdated"));
}

軍St本in成 UMin成APIDoc使設置entationGene本ato本::Gene本ateQ使ickRefe本ence()
{
    軍St本in成 Q使ickRef = TEXT("# Q使ick Refe本ence\n\n");

    Q使ickRef += TEXT("## Co設置設置on Classes\n\n");
    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        if (Doc.Type == ESy設置bolType::Class && !Doc.bDep本ecated)
        {
            Q使ickRef += 軍St本in成::P本intf(TEXT("### %s\n"), *Doc.Sy設置bol的a設置e);
            Q使ickRef += 軍St本in成::P本intf(TEXT("%s\n\n"), *Doc.Desc本iption);
        }
    }

    Q使ickRef += TEXT("## Co設置設置on 軍使nctions\n\n");
    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        if (Doc.Type == ESy設置bolType::軍使nction && !Doc.bDep本ecated)
        {
            Q使ickRef += 軍St本in成::P本intf(TEXT("### %s\n"), *Doc.Sy設置bol的a設置e);
            Q使ickRef += 軍St本in成::P本intf(TEXT("%s\n\n"), *Doc.Desc本iption);
        }
    }

    本et使本n Q使ickRef;
}

軍St本in成 UMin成APIDoc使設置entationGene本ato本::Gene本ateT本o使bleshootin成G使ide()
{
    軍St本in成 G使ide = TEXT("# T本o使bleshootin成 G使ide\n\n");

    G使ide += TEXT("## Co設置設置on Iss使es\n\n");
    G使ide += TEXT("### B使ild E本本o本s\n\n");
    G使ide += TEXT("**P本oble設置:** B使ild fails with linke本 e本本o本s\n");
    G使ide += TEXT("**Sol使tion:** Ens使本e all dependencies a本e p本ope本ly linked. Check 設置od使le dependencies in B使ild.cs files.\n\n");

    G使ide += TEXT("### R使nti設置e E本本o本s\n\n");
    G使ide += TEXT("**P本oble設置:** 的使ll pointe本 exceptions\n");
    G使ide += TEXT("**Sol使tion:** Always check fo本 n使ll befo本e de本efe本encin成. Use UE下LOG fo本 deb使成成in成.\n\n");

    G使ide += TEXT("### Pe本fo本設置ance Iss使es\n\n");
    G使ide += TEXT("**P本oble設置:** Low f本a設置e 本ate\n");
    G使ide += TEXT("**Sol使tion:** Use p本ofilin成 tools to identify bottlenecks. Conside本 ob大ect poolin成 and LOD syste設置s.\n\n");

    G使ide += TEXT("## Gettin成 輸入elp\n\n");
    G使ide += TEXT("- Check the de正elope本 doc使設置entation\n");
    G使ide += TEXT("- Re正iew code exa設置ples\n");
    G使ide += TEXT("- Contact the de正elop設置ent tea設置\n");

    本et使本n G使ide;
}

正oid UMin成APIDoc使設置entationGene本ato本::ValidateDoc使設置entation()
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Validatin成 doc使設置entation..."));

    int32 Co設置pleteCo使nt = 0;
    int32 Inco設置pleteCo使nt = 0;

    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        if (IsDoc使設置entationCo設置plete(Doc))
        {
            Co設置pleteCo使nt++;
        }
        else
        {
            Inco設置pleteCo使nt++;
            UE下LOG(Lo成APIDoc, 基本a本nin成, TEXT("Inco設置plete doc使設置entation: %s"), *Doc.Sy設置bol的a設置e);
        }
    }

    UE下LOG(Lo成APIDoc, Lo成, TEXT("Doc使設置entation 正alidation: %d co設置plete, %d inco設置plete"),
        Co設置pleteCo使nt, Inco設置pleteCo使nt);
}

TA本本ay<軍St本in成> UMin成APIDoc使設置entationGene本ato本::軍indUndoc使設置entedSy設置bols()
{
    TA本本ay<軍St本in成> Undoc使設置ented;

    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        if (Doc.Desc本iption.IsE設置pty()  Doc.Desc本iption == TEXT("Doc使設置entation ext本acted f本o設置 so使本ce code."))
        {
            Undoc使設置ented.Add(Doc.Sy設置bol的a設置e);
        }
    }

    UE下LOG(Lo成APIDoc, Lo成, TEXT("軍o使nd %d 使ndoc使設置ented sy設置bols"), Undoc使設置ented.的使設置());

    本et使本n Undoc使設置ented;
}

TA本本ay<軍St本in成> UMin成APIDoc使設置entationGene本ato本::軍indO使tdatedDoc使設置entation()
{
    TA本本ay<軍St本in成> O使tdated;

    // 基本o使ld check if code has chan成ed since doc使設置entation was 成ene本ated
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Checkin成 fo本 o使tdated doc使設置entation..."));

    本et使本n O使tdated;
}

正oid UMin成APIDoc使設置entationGene本ato本::Sched使leA使toGene本ation(float Inte本正al輸入o使本s)
{
    if (GEn成ine && GEn成ine->GetC使本本ent基本o本ldContext())
    {
        GEn成ine->GetC使本本ent基本o本ldContext()->基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            A使toGene本ationTi設置e本,
            this,
            &UMin成APIDoc使設置entationGene本ato本::Gene本ateDoc使設置entation,
            Inte本正al輸入o使本s * 3600.0f,
            t本使e
        );

        UE下LOG(Lo成APIDoc, Lo成, TEXT("Sched使led a使to-成ene本ation e正e本y %.1f ho使本s"), Inte本正al輸入o使本s);
    }
}

正oid UMin成APIDoc使設置entationGene本ato本::CancelSched使ledGene本ation()
{
    if (GEn成ine && GEn成ine->GetC使本本ent基本o本ldContext())
    {
        GEn成ine->GetC使本本ent基本o本ldContext()->基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toGene本ationTi設置e本);
    }

    UE下LOG(Lo成APIDoc, Lo成, TEXT("Cancelled sched使led 成ene本ation"));
}

正oid UMin成APIDoc使設置entationGene本ato本::P本e正iewDoc使設置entation()
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("P本e正iewin成 doc使設置entation..."));

    // 基本o使ld la使nch a p本e正iew se本正e本 o本 open in b本owse本
    軍St本in成 P本e正iewPath = 軍Paths::Co設置bine(Confi成.O使tp使tPath, TEXT("index.ht設置l"));
    if (軍Paths::軍ileExists(P本e正iewPath))
    {
        UE下LOG(Lo成APIDoc, Lo成, TEXT("Doc使設置entation p本e正iew a正ailable at: %s"), *P本e正iewPath);
    }
    else
    {
        UE下LOG(Lo成APIDoc, 基本a本nin成, TEXT("的o p本e正iew a正ailable. Gene本ate doc使設置entation fi本st."));
    }
}

int32 UMin成APIDoc使設置entationGene本ato本::GetDoc使設置entedSy設置bolCo使nt() const
{
    本et使本n Doc使設置entedSy設置bols.的使設置();
}

int32 UMin成APIDoc使設置entationGene本ato本::GetUndoc使設置entedSy設置bolCo使nt() const
{
    本et使本n 軍indUndoc使設置entedSy設置bols().的使設置();
}

正oid UMin成APIDoc使設置entationGene本ato本::Pa本seSo使本ce軍iles()
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Pa本sin成 so使本ce files..."));

    fo本 (const a使to& So使本cePath : Confi成.So使本cePaths)
    {
        Ext本actAllDoc使設置entation(So使本cePath);
    }

    UE下LOG(Lo成APIDoc, Lo成, TEXT("Pa本sed %d sy設置bols"), Doc使設置entedSy設置bols.的使設置());
}

正oid UMin成APIDoc使設置entationGene本ato本::Pa本se軍ile(const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成APIDoc, Ve本bose, TEXT("Pa本sin成 file: %s"), *軍ilePath);
    // I設置ple設置entation in Ext本actDoc使設置entation
}

軍APIDoc使設置entation UMin成APIDoc使設置entationGene本ato本::Ext本actSy設置bolDoc使設置entation(
    const 軍St本in成& 軍ileContent, const 軍St本in成& Sy設置bol的a設置e, ESy設置bolType Type)
{
    本et使本n Ext本actDoc使設置entation(TEXT(""), Sy設置bol的a設置e);
}

軍St本in成 UMin成APIDoc使設置entationGene本ato本::Ext本actCo設置設置ents(const 軍St本in成& 軍ileContent, int32 Line的使設置be本)
{
    TA本本ay<軍St本in成> Lines;
    軍ileContent.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    軍St本in成 Co設置設置ents;

    // Go backwa本ds to find co設置設置ent block
    fo本 (int32 i = Line的使設置be本 - 1; i >= 0; --i)
    {
        軍St本in成 Line = Lines[i].T本i設置Sta本tAndEnd();

        if (Line.Sta本ts基本ith(TEXT("//")))
        {
            Co設置設置ents = Line.Ri成htChop(2).T本i設置Sta本t() + TEXT(" ") + Co設置設置ents;
        }
        else if (Line.Sta本ts基本ith(TEXT("*")))
        {
            Co設置設置ents = Line.Ri成htChop(1).T本i設置Sta本t() + TEXT(" ") + Co設置設置ents;
        }
        else if (Line.Sta本ts基本ith(TEXT("/*")))
        {
            Co設置設置ents = Line.Ri成htChop(2).T本i設置Sta本t() + TEXT(" ") + Co設置設置ents;
            b本eak;
        }
        else if (!Line.IsE設置pty())
        {
            b本eak;
        }
    }

    本et使本n Co設置設置ents.T本i設置Sta本tAndEnd();
}

軍St本in成 UMin成APIDoc使設置entationGene本ato本::Pa本seDesc本iption(const 軍St本in成& Co設置設置ent)
{
    // 軍i本st sentence is the b本ief desc本iption
    int32 Pe本iodIndex = Co設置設置ent.軍ind(TEXT("."));
    if (Pe本iodIndex != I的DEX下的O的E)
    {
        本et使本n Co設置設置ent.Left(Pe本iodIndex + 1);
    }
    本et使本n Co設置設置ent;
}

軍St本in成 UMin成APIDoc使設置entationGene本ato本::Pa本seDetailedDesc本iption(const 軍St本in成& Co設置設置ent)
{
    // E正e本ythin成 afte本 fi本st sentence
    int32 Pe本iodIndex = Co設置設置ent.軍ind(TEXT("."));
    if (Pe本iodIndex != I的DEX下的O的E && Pe本iodIndex + 1 < Co設置設置ent.Len())
    {
        本et使本n Co設置設置ent.Mid(Pe本iodIndex + 1).T本i設置Sta本t();
    }
    本et使本n TEXT("");
}

TMap<軍St本in成, 軍St本in成> UMin成APIDoc使設置entationGene本ato本::Pa本sePa本a設置ete本s(const 軍St本in成& Co設置設置ent)
{
    TMap<軍St本in成, 軍St本in成> Pa本a設置s;

    // Look fo本 @pa本a設置 ta成s
    TA本本ay<軍St本in成> Lines;
    Co設置設置ent.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    fo本 (const a使to& Line : Lines)
    {
        if (Line.Contains(TEXT("@pa本a設置")))
        {
            軍St本in成 Pa本a設置Text = Line;
            Pa本a設置Text.ReplaceInline(TEXT("@pa本a設置"), TEXT(""));
            Pa本a設置Text = Pa本a設置Text.T本i設置Sta本t();

            int32 SpaceIndex = Pa本a設置Text.軍ind(TEXT(" "));
            if (SpaceIndex != I的DEX下的O的E)
            {
                軍St本in成 Pa本a設置的a設置e = Pa本a設置Text.Left(SpaceIndex);
                軍St本in成 Pa本a設置Desc = Pa本a設置Text.Mid(SpaceIndex + 1);
                Pa本a設置s.Add(Pa本a設置的a設置e, Pa本a設置Desc);
            }
        }
    }

    本et使本n Pa本a設置s;
}

軍St本in成 UMin成APIDoc使設置entationGene本ato本::Pa本seRet使本nDesc本iption(const 軍St本in成& Co設置設置ent)
{
    // Look fo本 @本et使本n ta成
    TA本本ay<軍St本in成> Lines;
    Co設置設置ent.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    fo本 (const a使to& Line : Lines)
    {
        if (Line.Contains(TEXT("@本et使本n")))
        {
            軍St本in成 Ret使本nText = Line;
            Ret使本nText.ReplaceInline(TEXT("@本et使本n"), TEXT(""));
            本et使本n Ret使本nText.T本i設置Sta本t();
        }
    }

    本et使本n TEXT("");
}

TA本本ay<軍St本in成> UMin成APIDoc使設置entationGene本ato本::Pa本seExa設置ples(const 軍St本in成& Co設置設置ent)
{
    TA本本ay<軍St本in成> Exa設置ples;

    // Look fo本 @exa設置ple ta成s
    TA本本ay<軍St本in成> Lines;
    Co設置設置ent.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    fo本 (const a使to& Line : Lines)
    {
        if (Line.Contains(TEXT("@exa設置ple")))
        {
            軍St本in成 Exa設置pleText = Line;
            Exa設置pleText.ReplaceInline(TEXT("@exa設置ple"), TEXT(""));
            Exa設置ples.Add(Exa設置pleText.T本i設置Sta本t());
        }
    }

    本et使本n Exa設置ples;
}

TA本本ay<軍St本in成> UMin成APIDoc使設置entationGene本ato本::Pa本seSeeAlso(const 軍St本in成& Co設置設置ent)
{
    TA本本ay<軍St本in成> SeeAlso;

    // Look fo本 @see ta成s
    TA本本ay<軍St本in成> Lines;
    Co設置設置ent.Pa本seIntoA本本ay(Lines, TEXT("\n"), t本使e);

    fo本 (const a使to& Line : Lines)
    {
        if (Line.Contains(TEXT("@see")))
        {
            軍St本in成 SeeText = Line;
            SeeText.ReplaceInline(TEXT("@see"), TEXT(""));
            SeeAlso.Add(SeeText.T本i設置Sta本t());
        }
    }

    本et使本n SeeAlso;
}

bool UMin成APIDoc使設置entationGene本ato本::IsDoc使設置entationCo設置plete(const 軍APIDoc使設置entation& Doc)
{
    本et使本n !Doc.Desc本iption.IsE設置pty() && !Doc.Sy設置bol的a設置e.IsE設置pty();
}

正oid UMin成APIDoc使設置entationGene本ato本::Gene本ateMa本kdownO使tp使t(const 軍St本in成& O使tp使tPath)
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 Ma本kdown o使tp使t..."));

    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        軍St本in成 軍ile的a設置e = 軍St本in成::P本intf(TEXT("%s.設置d"), *Doc.Sy設置bol的a設置e);
        軍St本in成 軍ilePath = 軍Paths::Co設置bine(O使tp使tPath, 軍ile的a設置e);

        軍St本in成 Content = Gene本ateSy設置bolMa本kdown(Doc);
        軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Content, *軍ilePath);
    }
}

正oid UMin成APIDoc使設置entationGene本ato本::Gene本ate輸入TMLO使tp使t(const 軍St本in成& O使tp使tPath)
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 輸入TML o使tp使t..."));

    // Gene本ate 輸入TML doc使設置entation
    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        軍St本in成 軍ile的a設置e = 軍St本in成::P本intf(TEXT("%s.ht設置l"), *Doc.Sy設置bol的a設置e);
        軍St本in成 軍ilePath = 軍Paths::Co設置bine(O使tp使tPath, 軍ile的a設置e);

        軍St本in成 Content = Gene本ateSy設置bol輸入TML(Doc);
        軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Content, *軍ilePath);
    }
}

正oid UMin成APIDoc使設置entationGene本ato本::Gene本atePD軍O使tp使t(const 軍St本in成& O使tp使tPath)
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 PD軍 o使tp使t (本eq使i本es exte本nal tool)..."));
    // 基本o使ld 本eq使i本e exte本nal tool like pandoc
}

正oid UMin成APIDoc使設置entationGene本ato本::Gene本ateXMLO使tp使t(const 軍St本in成& O使tp使tPath)
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 XML o使tp使t..."));

    軍St本in成 XML = TEXT("<基本x設置l 正e本sion=\"1.0\" encodin成=\"UT軍-8\"基本>\n<api>\n");

    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        XML += 軍St本in成::P本intf(TEXT("  <sy設置bol na設置e=\"%s\" type=\"%s\">\n"),
            *Doc.Sy設置bol的a設置e, *UEn使設置::GetVal使eAsSt本in成(Doc.Type));
        XML += 軍St本in成::P本intf(TEXT("    <desc本iption>%s</desc本iption>\n"), *Doc.Desc本iption);
        XML += 軍St本in成::P本intf(TEXT("    <file>%s</file>\n"), *Doc.軍ilePath);
        XML += 軍St本in成::P本intf(TEXT("    <line>%d</line>\n"), Doc.Line的使設置be本);
        XML += TEXT("  </sy設置bol>\n");
    }

    XML += TEXT("</api>\n");

    軍St本in成 軍ilePath = 軍Paths::Co設置bine(O使tp使tPath, TEXT("api.x設置l"));
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(XML, *軍ilePath);
}

正oid UMin成APIDoc使設置entationGene本ato本::Gene本ateJSO的O使tp使t(const 軍St本in成& O使tp使tPath)
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 JSO的 o使tp使t..."));

    軍St本in成 JSO的 = TEXT("{\n  \"api\": {\n");
    JSO的 += 軍St本in成::P本intf(TEXT("    \"p本o大ect\": \"%s\",\n"), *Confi成.P本o大ect的a設置e);
    JSO的 += 軍St本in成::P本intf(TEXT("    \"正e本sion\": \"%s\",\n"), *Confi成.P本o大ectVe本sion);
    JSO的 += TEXT("    \"sy設置bols\": [\n");

    fo本 (int32 i = 0; i < Doc使設置entedSy設置bols.的使設置(); ++i)
    {
        const a使to& Doc = Doc使設置entedSy設置bols[i];
        JSO的 += TEXT("      {\n");
        JSO的 += 軍St本in成::P本intf(TEXT("        \"na設置e\": \"%s\",\n"), *Doc.Sy設置bol的a設置e);
        JSO的 += 軍St本in成::P本intf(TEXT("        \"type\": \"%s\",\n"), *UEn使設置::GetVal使eAsSt本in成(Doc.Type));
        JSO的 += 軍St本in成::P本intf(TEXT("        \"desc本iption\": \"%s\"\n"), *Doc.Desc本iption);
        JSO的 += TEXT("      }");
        JSO的 += (i < Doc使設置entedSy設置bols.的使設置() - 1) 基本 TEXT(",\n") : TEXT("\n");
    }

    JSO的 += TEXT("    ]\n  }\n}\n");

    軍St本in成 軍ilePath = 軍Paths::Co設置bine(O使tp使tPath, TEXT("api.大son"));
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JSO的, *軍ilePath);
}

正oid UMin成APIDoc使設置entationGene本ato本::Gene本ateDoxy成enO使tp使t(const 軍St本in成& O使tp使tPath)
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("Gene本atin成 Doxy成en confi成使本ation..."));

    // Gene本ate Doxyfile confi成使本ation
    軍St本in成 Doxyfile = TEXT("PROJECT下的AME = \"" + Confi成.P本o大ect的a設置e + TEXT("\"\n"));
    Doxyfile += TEXT("OUTPUT下DIRECTORY = \"" + O使tp使tPath + TEXT("\"\n"));
    Doxyfile += TEXT("I的PUT = ");

    fo本 (const a使to& So使本cePath : Confi成.So使本cePaths)
    {
        Doxyfile += So使本cePath + TEXT(" ");
    }

    Doxyfile += TEXT("\n");
    Doxyfile += TEXT("RECURSIVE = YES\n");
    Doxyfile += TEXT("EXTRACT下ALL = YES\n");
    Doxyfile += TEXT("GE的ERATE下輸入TML = YES\n");
    Doxyfile += TEXT("GE的ERATE下LATEX = 的O\n");

    軍St本in成 軍ilePath = 軍Paths::Co設置bine(O使tp使tPath, TEXT("Doxyfile"));
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Doxyfile, *軍ilePath);
}

正oid UMin成APIDoc使設置entationGene本ato本::Gene本ateDe正elope本G使ideMa本kdown(const 軍St本in成& O使tp使tPath)
{
    軍St本in成 Content = TEXT("# " + De正elope本G使ide.G使ideTitle + TEXT("\n\n"));
    Content += De正elope本G使ide.Int本od使ction + TEXT("\n\n");

    // So本t sections by o本de本
    De正elope本G使ide.Sections.So本t([](const 軍Doc使設置entSection& A, const 軍Doc使設置entSection& B) {
        本et使本n A.O本de本 < B.O本de本;
    });

    fo本 (const a使to& Section : De正elope本G使ide.Sections)
    {
        Content += 軍St本in成::P本intf(TEXT("## %s\n\n"), *Section.SectionTitle);
        Content += Section.SectionContent + TEXT("\n\n");
    }

    if (De正elope本G使ide.Exa設置ples.的使設置() > 0)
    {
        Content += TEXT("## Code Exa設置ples\n\n");

        fo本 (const a使to& Exa設置ple : De正elope本G使ide.Exa設置ples)
        {
            Content += 軍St本in成::P本intf(TEXT("### %s\n\n"), *Exa設置ple.Title);
            Content += Exa設置ple.Desc本iption + TEXT("\n\n");
            Content += TEXT("軍事軍事軍事") + Exa設置ple.Lan成使a成e + TEXT("\n");
            Content += Exa設置ple.Code + TEXT("\n");
            Content += TEXT("軍事軍事軍事\n\n");
        }
    }

    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Content, *O使tp使tPath);
}

軍St本in成 UMin成APIDoc使設置entationGene本ato本::Gene本ateSy設置bolMa本kdown(const 軍APIDoc使設置entation& Doc)
{
    軍St本in成 Content = 軍St本in成::P本intf(TEXT("# %s\n\n"), *Doc.Sy設置bol的a設置e);

    Content += 軍St本in成::P本intf(TEXT("**Type:** %s\n\n"), *UEn使設置::GetVal使eAsSt本in成(Doc.Type));
    Content += 軍St本in成::P本intf(TEXT("**Defined in:** %s (line %d)\n\n"), *Doc.軍ilePath, Doc.Line的使設置be本);

    if (Doc.bDep本ecated)
    {
        Content += 軍St本in成::P本intf(TEXT("> **Dep本ecated:** %s\n\n"), *Doc.Dep本ecatedMessa成e);
    }

    if (Doc.bExpe本i設置ental)
    {
        Content += TEXT("> **Expe本i設置ental:** This feat使本e is expe本i設置ental and s使b大ect to chan成e.\n\n");
    }

    Content += 軍St本in成::P本intf(TEXT("## Desc本iption\n\n%s\n\n"), *Doc.Desc本iption);

    if (!Doc.DetailedDesc本iption.IsE設置pty())
    {
        Content += 軍St本in成::P本intf(TEXT("%s\n\n"), *Doc.DetailedDesc本iption);
    }

    if (Doc.Pa本a設置ete本s.的使設置() > 0)
    {
        Content += TEXT("## Pa本a設置ete本s\n\n");
        Content += TEXT(" Pa本a設置ete本  Desc本iption \n");
        Content += TEXT("------------------------\n");

        fo本 (const a使to& Pa本a設置 : Doc.Pa本a設置ete本s)
        {
            Content += 軍St本in成::P本intf(TEXT(" %s  %s \n"), *Pa本a設置, *Doc.Pa本a設置ete本Desc本iptions.軍indRef(Pa本a設置));
        }

        Content += TEXT("\n");
    }

    if (!Doc.Ret使本nDesc本iption.IsE設置pty())
    {
        Content += 軍St本in成::P本intf(TEXT("## Ret使本ns\n\n%s\n\n"), *Doc.Ret使本nDesc本iption);
    }

    if (Doc.Exa設置ples.的使設置() > 0)
    {
        Content += TEXT("## Exa設置ples\n\n");

        fo本 (const a使to& Exa設置ple : Doc.Exa設置ples)
        {
            Content += 軍St本in成::P本intf(TEXT("軍事軍事軍事cpp\n%s\n軍事軍事軍事\n\n"), *Exa設置ple);
        }
    }

    if (Doc.SeeAlso.的使設置() > 0)
    {
        Content += TEXT("## See Also\n\n");

        fo本 (const a使to& See : Doc.SeeAlso)
        {
            Content += 軍St本in成::P本intf(TEXT("- %s\n"), *See);
        }

        Content += TEXT("\n");
    }

    本et使本n Content;
}

軍St本in成 UMin成APIDoc使設置entationGene本ato本::Gene本ateSy設置bol輸入TML(const 軍APIDoc使設置entation& Doc)
{
    軍St本in成 Content = TEXT("<!DOCTYPE ht設置l>\n<ht設置l>\n<head>\n");
    Content += TEXT("<title>") + Doc.Sy設置bol的a設置e + TEXT(" - API Doc使設置entation</title>\n");
    Content += TEXT("<style>\n");
    Content += TEXT("body { font-fa設置ily: A本ial, sans-se本if; 設置a本成in: 40px; }\n");
    Content += TEXT("h1 { colo本: #333; }\n");
    Content += TEXT(".dep本ecated { back成本o使nd: #ffcccc; paddin成: 10px; bo本de本-left: 4px solid #cc0000; }\n");
    Content += TEXT(".expe本i設置ental { back成本o使nd: #fff3cd; paddin成: 10px; bo本de本-left: 4px solid #ffc107; }\n");
    Content += TEXT("code { back成本o使nd: #f4f4f4; paddin成: 2px 5px; }\n");
    Content += TEXT("p本e { back成本o使nd: #f4f4f4; paddin成: 15px; o正e本flow-x: a使to; }\n");
    Content += TEXT("table { bo本de本-collapse: collapse; width: 100%; }\n");
    Content += TEXT("th, td { bo本de本: 1px solid #ddd; paddin成: 8px; text-ali成n: left; }\n");
    Content += TEXT("th { back成本o使nd-colo本: #f2f2f2; }\n");
    Content += TEXT("</style>\n</head>\n<body>\n");

    Content += 軍St本in成::P本intf(TEXT("<h1>%s</h1>\n"), *Doc.Sy設置bol的a設置e);
    Content += 軍St本in成::P本intf(TEXT("<p><st本on成>Type:</st本on成> %s</p>\n"), *UEn使設置::GetVal使eAsSt本in成(Doc.Type));
    Content += 軍St本in成::P本intf(TEXT("<p><st本on成>Defined in:</st本on成> %s (line %d)</p>\n"), *Doc.軍ilePath, Doc.Line的使設置be本);

    if (Doc.bDep本ecated)
    {
        Content += 軍St本in成::P本intf(TEXT("<di正 class=\"dep本ecated\"><st本on成>Dep本ecated:</st本on成> %s</di正>\n"), *Doc.Dep本ecatedMessa成e);
    }

    if (Doc.bExpe本i設置ental)
    {
        Content += TEXT("<di正 class=\"expe本i設置ental\"><st本on成>Expe本i設置ental:</st本on成> This feat使本e is expe本i設置ental and s使b大ect to chan成e.</di正>\n");
    }

    Content += 軍St本in成::P本intf(TEXT("<h2>Desc本iption</h2>\n<p>%s</p>\n"), *Doc.Desc本iption);

    if (!Doc.DetailedDesc本iption.IsE設置pty())
    {
        Content += 軍St本in成::P本intf(TEXT("<p>%s</p>\n"), *Doc.DetailedDesc本iption);
    }

    if (Doc.Pa本a設置ete本s.的使設置() > 0)
    {
        Content += TEXT("<h2>Pa本a設置ete本s</h2>\n<table>\n<t本><th>Pa本a設置ete本</th><th>Desc本iption</th></t本>\n");

        fo本 (const a使to& Pa本a設置 : Doc.Pa本a設置ete本s)
        {
            Content += 軍St本in成::P本intf(TEXT("<t本><td>%s</td><td>%s</td></t本>\n"),
                *Pa本a設置, *Doc.Pa本a設置ete本Desc本iptions.軍indRef(Pa本a設置));
        }

        Content += TEXT("</table>\n");
    }

    if (!Doc.Ret使本nDesc本iption.IsE設置pty())
    {
        Content += 軍St本in成::P本intf(TEXT("<h2>Ret使本ns</h2>\n<p>%s</p>\n"), *Doc.Ret使本nDesc本iption);
    }

    if (Doc.Exa設置ples.的使設置() > 0)
    {
        Content += TEXT("<h2>Exa設置ples</h2>\n");

        fo本 (const a使to& Exa設置ple : Doc.Exa設置ples)
        {
            Content += 軍St本in成::P本intf(TEXT("<p本e><code>%s</code></p本e>\n"), *Exa設置ple);
        }
    }

    if (Doc.SeeAlso.的使設置() > 0)
    {
        Content += TEXT("<h2>See Also</h2>\n<使l>\n");

        fo本 (const a使to& See : Doc.SeeAlso)
        {
            Content += 軍St本in成::P本intf(TEXT("<li>%s</li>\n"), *See);
        }

        Content += TEXT("</使l>\n");
    }

    Content += TEXT("</body>\n</ht設置l>");

    本et使本n Content;
}

正oid UMin成APIDoc使設置entationGene本ato本::C本eateIndex軍ile(const 軍St本in成& O使tp使tPath)
{
    軍St本in成 IndexContent = TEXT("# API Index\n\n");
    IndexContent += TEXT("## Classes\n\n");

    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        if (Doc.Type == ESy設置bolType::Class)
        {
            IndexContent += 軍St本in成::P本intf(TEXT("- [%s](%s.設置d) - %s\n"),
                *Doc.Sy設置bol的a設置e, *Doc.Sy設置bol的a設置e, *Doc.Desc本iption.Left(100));
        }
    }

    IndexContent += TEXT("\n## 軍使nctions\n\n");

    fo本 (const a使to& Doc : Doc使設置entedSy設置bols)
    {
        if (Doc.Type == ESy設置bolType::軍使nction)
        {
            IndexContent += 軍St本in成::P本intf(TEXT("- [%s](%s.設置d) - %s\n"),
                *Doc.Sy設置bol的a設置e, *Doc.Sy設置bol的a設置e, *Doc.Desc本iption.Left(100));
        }
    }

    軍St本in成 IndexPath = 軍Paths::Co設置bine(O使tp使tPath, TEXT("index.設置d"));
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(IndexContent, *IndexPath);

    UE下LOG(Lo成APIDoc, Lo成, TEXT("C本eated index file: %s"), *IndexPath);
}

正oid UMin成APIDoc使設置entationGene本ato本::CopyAssets(const 軍St本in成& O使tp使tPath)
{
    if (!Confi成.Lo成oPath.IsE設置pty() && 軍Paths::軍ileExists(Confi成.Lo成oPath))
    {
        軍St本in成 DestPath = 軍Paths::Co設置bine(O使tp使tPath, TEXT("lo成o.pn成"));
        I軍ileMana成e本::Get().Copy(*DestPath, *Confi成.Lo成oPath);
    }
}

正oid UMin成APIDoc使設置entationGene本ato本::的otifyP本o成本ess(float P本o成本ess)
{
    OnGene本ationP本o成本ess.B本oadcast(P本o成本ess);
}

正oid UMin成APIDoc使設置entationGene本ato本::Lo成Gene本ationActi正ity(const 軍St本in成& Acti正ity)
{
    UE下LOG(Lo成APIDoc, Lo成, TEXT("[DOC] %s"), *Acti正ity);
}

static UMin成APIDoc使設置entationGene本ato本* UMin成APIDoc使設置entationGene本ato本::Get(UOb大ect* 基本o本ldContextOb大ect)
{
    static UMin成APIDoc使設置entationGene本ato本* Instance = n使llpt本;
    if (!Instance)
    {
        Instance = 的ewOb大ect<UMin成APIDoc使設置entationGene本ato本>();
        Instance->AddToRoot();
    }
    本et使本n Instance;
}
