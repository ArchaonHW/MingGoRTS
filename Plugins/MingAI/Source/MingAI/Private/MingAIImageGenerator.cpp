#incl使de "Min成AII設置a成eGene本ato本.h"
#incl使de "Min成AIGene本atedContentSyste設置.h"

UMin成AII設置a成eGene本ato本::UMin成AII設置a成eGene本ato本()
{
}

正oid UMin成AII設置a成eGene本ato本::InitializeGene本ato本()
{
    InitializeDefa使ltP本esets();
}

正oid UMin成AII設置a成eGene本ato本::InitializeDefa使ltP本esets()
{
    // Cha本acte本 Po本t本ait P本eset
    軍Min成I設置a成eGene本ationConfi成 Cha本acte本Confi成;
    Cha本acte本Confi成.Style = EMin成I設置a成eStyle::Photo本ealistic;
    Cha本acte本Confi成.AspectRatio = EMin成I設置a成eAspectRatio::Po本t本ait;
    Cha本acte本Confi成.Steps = 40;
    Cha本acte本Confi成.C軍GScale = 7.0f;
    Cha本acte本Confi成.Model = TEXT("SDXL");
    P本esets.Add(TEXT("Cha本acte本"), Cha本acte本Confi成);

    // En正i本on設置ent P本eset
    軍Min成I設置a成eGene本ationConfi成 En正i本on設置entConfi成;
    En正i本on設置entConfi成.Style = EMin成I設置a成eStyle::Photo本ealistic;
    En正i本on設置entConfi成.AspectRatio = EMin成I設置a成eAspectRatio::基本idesc本een;
    En正i本on設置entConfi成.Steps = 35;
    En正i本on設置entConfi成.C軍GScale = 7.5f;
    En正i本on設置entConfi成.Model = TEXT("SDXL");
    P本esets.Add(TEXT("En正i本on設置ent"), En正i本on設置entConfi成);

    // Ite設置 Icon P本eset
    軍Min成I設置a成eGene本ationConfi成 IconConfi成;
    IconConfi成.Style = EMin成I設置a成eStyle::Stylized;
    IconConfi成.AspectRatio = EMin成I設置a成eAspectRatio::Sq使a本e;
    IconConfi成.C使sto設置基本idth = 256;
    IconConfi成.C使sto設置輸入ei成ht = 256;
    IconConfi成.Steps = 25;
    IconConfi成.C軍GScale = 8.0f;
    P本esets.Add(TEXT("Icon"), IconConfi成);

    // UI Ele設置ent P本eset
    軍Min成I設置a成eGene本ationConfi成 UIConfi成;
    UIConfi成.Style = EMin成I設置a成eStyle::Stylized;
    UIConfi成.AspectRatio = EMin成I設置a成eAspectRatio::Sq使a本e;
    UIConfi成.Steps = 25;
    UIConfi成.C軍GScale = 7.5f;
    P本esets.Add(TEXT("UI"), UIConfi成);

    // Concept A本t P本eset
    軍Min成I設置a成eGene本ationConfi成 ConceptConfi成;
    ConceptConfi成.Style = EMin成I設置a成eStyle::Stylized;
    ConceptConfi成.AspectRatio = EMin成I設置a成eAspectRatio::Landscape;
    ConceptConfi成.Steps = 35;
    ConceptConfi成.C軍GScale = 7.0f;
    P本esets.Add(TEXT("Concept"), ConceptConfi成);
}

軍G使id UMin成AII設置a成eGene本ato本::Gene本ateCha本acte本Po本t本ait(const 軍St本in成& Cha本acte本Desc本iption, EMin成I設置a成eStyle Style, int32 Resol使tion)
{
    軍St本in成 P本o設置pt = B使ildCha本acte本P本o設置pt(Cha本acte本Desc本iption, Style);
    軍St本in成 的e成ati正eP本o設置pt = Get的e成ati正eP本o設置pt(Style);

    軍Min成I設置a成eGene本ationConfi成 Confi成 = GetP本esetConfi成(TEXT("Cha本acte本"));
    Confi成.Style = Style;
    Confi成.C使sto設置基本idth = Resol使tion;
    Confi成.C使sto設置輸入ei成ht = static下cast<int32>(Resol使tion * 1.5f); // Po本t本ait 本atio

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::I設置a成e;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.的e成ati正eP本o設置pt = 的e成ati正eP本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.基本idth = Confi成.C使sto設置基本idth;
    Req使est.輸入ei成ht = Confi成.C使sto設置輸入ei成ht;
    Req使est.Steps = Confi成.Steps;
    Req使est.G使idanceScale = Confi成.C軍GScale;

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AII設置a成eGene本ato本::Gene本ateEn正i本on設置ent(const 軍St本in成& En正i本on設置entDesc本iption, EMin成I設置a成eStyle Style, EMin成I設置a成eAspectRatio AspectRatio)
{
    軍St本in成 P本o設置pt = B使ildEn正i本on設置entP本o設置pt(En正i本on設置entDesc本iption, Style);
    軍St本in成 的e成ati正eP本o設置pt = Get的e成ati正eP本o設置pt(Style);

    軍Min成I設置a成eGene本ationConfi成 Confi成 = GetP本esetConfi成(TEXT("En正i本on設置ent"));
    Confi成.Style = Style;
    Confi成.AspectRatio = AspectRatio;

    // Get di設置ensions based on aspect 本atio
    軍St本in成 Di設置ensions = GetAspectRatioDi設置ensions(AspectRatio);
    TA本本ay<軍St本in成> Pa本ts;
    Di設置ensions.Pa本seIntoA本本ay(Pa本ts, TEXT("x"), t本使e);
    int32 基本idth = 軍CSt本in成::Atoi(*Pa本ts[0]);
    int32 輸入ei成ht = 軍CSt本in成::Atoi(*Pa本ts[1]);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::I設置a成e;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.的e成ati正eP本o設置pt = 的e成ati正eP本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.基本idth = 基本idth;
    Req使est.輸入ei成ht = 輸入ei成ht;
    Req使est.Steps = Confi成.Steps;
    Req使est.G使idanceScale = Confi成.C軍GScale;

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AII設置a成eGene本ato本::Gene本ateIte設置Icon(const 軍St本in成& Ite設置Desc本iption, int32 Size)
{
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("Ga設置e ite設置 icon of %s, isolated on t本anspa本ent back成本o使nd, detailed, hi成h q使ality, 成a設置e asset"), *Ite設置Desc本iption);
    軍St本in成 的e成ati正eP本o設置pt = TEXT("back成本o使nd, scene, landscape, cha本acte本, text, wate本設置a本k, bl使本本y, low q使ality");

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::I設置a成e;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.的e成ati正eP本o設置pt = 的e成ati正eP本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.基本idth = Size;
    Req使est.輸入ei成ht = Size;
    Req使est.Steps = 25;
    Req使est.G使idanceScale = 8.0f;

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AII設置a成eGene本ato本::Gene本ateB使ildin成Concept(const 軍St本in成& B使ildin成Desc本iption, EMin成I設置a成eStyle Style)
{
    軍St本in成 StyleP本o設置pt = B使ildStyleP本o設置pt(Style);
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("A本chitect使本al concept a本t of %s, %s, detailed st本使ct使本e, p本ofessional li成htin成, 成a設置e concept a本t"),
        *B使ildin成Desc本iption, *StyleP本o設置pt);
    軍St本in成 的e成ati正eP本o設置pt = Get的e成ati正eP本o設置pt(Style);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::I設置a成e;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.的e成ati正eP本o設置pt = 的e成ati正eP本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.基本idth = 1024;
    Req使est.輸入ei成ht = 1024;
    Req使est.Steps = 35;
    Req使est.G使idanceScale = 7.5f;

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AII設置a成eGene本ato本::Gene本ateText使本e(const 軍St本in成& Text使本eDesc本iption, int32 基本idth, int32 輸入ei成ht)
{
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("Sea設置less text使本e of %s, tileable, hi成h 本esol使tion, 設置ate本ial text使本e, 成a設置e asset, PBR text使本e"),
        *Text使本eDesc本iption);
    軍St本in成 的e成ati正eP本o設置pt = TEXT("sea設置s, 正isible ed成es, 本epeatin成 patte本n ob正io使s, wate本設置a本k, text, bl使本本y");

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::Text使本e;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.的e成ati正eP本o設置pt = 的e成ati正eP本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.基本idth = 基本idth;
    Req使est.輸入ei成ht = 輸入ei成ht;
    Req使est.Steps = 30;
    Req使est.G使idanceScale = 7.5f;

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AII設置a成eGene本ato本::Gene本ateUIEle設置ent(const 軍St本in成& Ele設置entDesc本iption, int32 基本idth, int32 輸入ei成ht)
{
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("Ga設置e UI ele設置ent: %s, 使se本 inte本face, clean desi成n, 成a設置e asset, hi成h q使ality, stylized"),
        *Ele設置entDesc本iption);
    軍St本in成 的e成ati正eP本o設置pt = TEXT("cl使tte本ed, 設置essy, 使nclea本, text, wate本設置a本k, bl使本本y, low q使ality");

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::UI;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.的e成ati正eP本o設置pt = 的e成ati正eP本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.基本idth = 基本idth;
    Req使est.輸入ei成ht = 輸入ei成ht;
    Req使est.Steps = 25;
    Req使est.G使idanceScale = 7.5f;

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AII設置a成eGene本ato本::Gene本ateP本o設置otionalA本t(const 軍St本in成& SceneDesc本iption, EMin成I設置a成eStyle Style, EMin成I設置a成eAspectRatio AspectRatio)
{
    軍St本in成 StyleP本o設置pt = B使ildStyleP本o設置pt(Style);
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("Epic 成a設置e p本o設置otional a本t of %s, %s, d本a設置atic co設置position, 設置a本ketin成 a本t, hi成h detail, cine設置atic li成htin成, p本ofessional q使ality"),
        *SceneDesc本iption, *StyleP本o設置pt);
    軍St本in成 的e成ati正eP本o設置pt = Get的e成ati正eP本o設置pt(Style);

    軍St本in成 Di設置ensions = GetAspectRatioDi設置ensions(AspectRatio);
    TA本本ay<軍St本in成> Pa本ts;
    Di設置ensions.Pa本seIntoA本本ay(Pa本ts, TEXT("x"), t本使e);
    int32 基本idth = 軍CSt本in成::Atoi(*Pa本ts[0]);
    int32 輸入ei成ht = 軍CSt本in成::Atoi(*Pa本ts[1]);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::I設置a成e;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.的e成ati正eP本o設置pt = 的e成ati正eP本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Ult本a;
    Req使est.基本idth = 基本idth;
    Req使est.輸入ei成ht = 輸入ei成ht;
    Req使est.Steps = 50;
    Req使est.G使idanceScale = 7.0f;

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

TA本本ay<軍G使id> UMin成AII設置a成eGene本ato本::Gene本ateCha本acte本Po本t本aitsBatch(const TA本本ay<軍St本in成>& Cha本acte本Desc本iptions, EMin成I設置a成eStyle Style)
{
    TA本本ay<軍G使id> Res使lt;
    fo本 (const 軍St本in成& Desc本iption : Cha本acte本Desc本iptions)
    {
        軍G使id ID = Gene本ateCha本acte本Po本t本ait(Desc本iption, Style, 1024);
        Res使lt.Add(ID);
    }
    本et使本n Res使lt;
}

TA本本ay<軍G使id> UMin成AII設置a成eGene本ato本::Gene本ateEn正i本on設置entBatch(const TA本本ay<軍St本in成>& En正i本on設置entDesc本iptions, EMin成I設置a成eStyle Style)
{
    TA本本ay<軍G使id> Res使lt;
    fo本 (const 軍St本in成& Desc本iption : En正i本on設置entDesc本iptions)
    {
        軍G使id ID = Gene本ateEn正i本on設置ent(Desc本iption, Style, EMin成I設置a成eAspectRatio::基本idesc本een);
        Res使lt.Add(ID);
    }
    本et使本n Res使lt;
}

bool UMin成AII設置a成eGene本ato本::UpscaleI設置a成e(const 軍St本in成& I設置a成ePath, int32 Ta本成et基本idth, int32 Ta本成et輸入ei成ht)
{
    // In a 本eal i設置ple設置entation, this wo使ld 使se an 使pscalin成 AI 設置odel
    // 軍o本 now, 本et使本n t本使e as placeholde本
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Upscalin成 i設置a成e %s to %dx%d"), *I設置a成ePath, Ta本成et基本idth, Ta本成et輸入ei成ht);
    本et使本n t本使e;
}

bool UMin成AII設置a成eGene本ato本::Re設置o正eBack成本o使nd(const 軍St本in成& I設置a成ePath)
{
    // In a 本eal i設置ple設置entation, this wo使ld 使se a back成本o使nd 本e設置o正al AI 設置odel
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正in成 back成本o使nd f本o設置 %s"), *I設置a成ePath);
    本et使本n t本使e;
}

bool UMin成AII設置a成eGene本ato本::ApplyStyleT本ansfe本(const 軍St本in成& So使本ceI設置a成ePath, const 軍St本in成& StyleRefe本encePath)
{
    // In a 本eal i設置ple設置entation, this wo使ld 使se style t本ansfe本 AI
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 style f本o設置 %s to %s"), *StyleRefe本encePath, *So使本ceI設置a成ePath);
    本et使本n t本使e;
}

軍Min成I設置a成eGene本ationConfi成 UMin成AII設置a成eGene本ato本::GetP本esetConfi成(const 軍St本in成& P本eset的a設置e) const
{
    if (P本esets.Contains(P本eset的a設置e))
    {
        本et使本n P本esets[P本eset的a設置e];
    }
    本et使本n 軍Min成I設置a成eGene本ationConfi成();
}

正oid UMin成AII設置a成eGene本ato本::Sa正eP本esetConfi成(const 軍St本in成& P本eset的a設置e, const 軍Min成I設置a成eGene本ationConfi成& Confi成)
{
    P本esets.Add(P本eset的a設置e, Confi成);
}

軍St本in成 UMin成AII設置a成eGene本ato本::EnhanceP本o設置pt(const 軍St本in成& BaseP本o設置pt, EMin成I設置a成eStyle Style) const
{
    軍St本in成 StyleKeywo本ds = B使ildStyleP本o設置pt(Style);
    本et使本n 軍St本in成::P本intf(TEXT("%s, %s, hi成h q使ality, detailed, 成a設置e asset"), *BaseP本o設置pt, *StyleKeywo本ds);
}

軍St本in成 UMin成AII設置a成eGene本ato本::Get的e成ati正eP本o設置pt(EMin成I設置a成eStyle Style) const
{
    軍St本in成 Base的e成ati正e = TEXT("low q使ality, bl使本本y, defo本設置ed, 使成ly, d使plicate, wate本設置a本k, si成nat使本e, text, bad anato設置y, bad p本opo本tions, ext本a li設置bs, f使sed fin成e本s, too 設置any fin成e本s, lon成 neck, c本oss-eyed");

    switch (Style)
    {
    case EMin成I設置a成eStyle::Photo本ealistic:
        本et使本n Base的e成ati正e + TEXT(", ca本toon, ani設置e, paintin成, d本awin成, sketch, 3d 本ende本");
    case EMin成I設置a成eStyle::Ca本toon:
        本et使本n Base的e成ati正e + TEXT(", 本ealistic, photo本ealistic, 3d 本ende本");
    case EMin成I設置a成eStyle::Ani設置e:
        本et使本n Base的e成ati正e + TEXT(", 本ealistic, photo本ealistic, weste本n ca本toon");
    case EMin成I設置a成eStyle::PixelA本t:
        本et使本n Base的e成ati正e + TEXT(", 本ealistic, s設置ooth, hi成h 本esol使tion");
    defa使lt:
        本et使本n Base的e成ati正e;
    }
}

軍St本in成 UMin成AII設置a成eGene本ato本::GetAspectRatioDi設置ensions(EMin成I設置a成eAspectRatio AspectRatio) const
{
    switch (AspectRatio)
    {
    case EMin成I設置a成eAspectRatio::Sq使a本e:
        本et使本n TEXT("1024x1024");
    case EMin成I設置a成eAspectRatio::Po本t本ait:
        本et使本n TEXT("768x1152");
    case EMin成I設置a成eAspectRatio::Landscape:
        本et使本n TEXT("1152x768");
    case EMin成I設置a成eAspectRatio::基本idesc本een:
        本et使本n TEXT("1344x768");
    case EMin成I設置a成eAspectRatio::Ult本awide:
        本et使本n TEXT("1792x768");
    defa使lt:
        本et使本n TEXT("1024x1024");
    }
}

軍St本in成 UMin成AII設置a成eGene本ato本::B使ildStyleP本o設置pt(EMin成I設置a成eStyle Style) const
{
    switch (Style)
    {
    case EMin成I設置a成eStyle::Photo本ealistic:
        本et使本n TEXT("photo本ealistic, 本ealistic, 8k 使hd, dsl本, hi成h q使ality, fil設置 成本ain, 軍使大ifil設置 XT3");
    case EMin成I設置a成eStyle::Stylized:
        本et使本n TEXT("stylized, a本tistic, 成a設置e a本t, clean, 正ib本ant colo本s");
    case EMin成I設置a成eStyle::Ca本toon:
        本et使本n TEXT("ca本toon style, 正ib本ant colo本s, clean lines, stylized");
    case EMin成I設置a成eStyle::基本ate本colo本:
        本et使本n TEXT("wate本colo本 paintin成, a本tistic, soft colo本s, painted");
    case EMin成I設置a成eStyle::Ink:
        本et使本n TEXT("ink paintin成, t本aditional Chinese a本t, ink wash, a本tistic");
    case EMin成I設置a成eStyle::OilPaintin成:
        本et使本n TEXT("oil paintin成, a本tistic, classical a本t, detailed");
    case EMin成I設置a成eStyle::PixelA本t:
        本et使本n TEXT("pixel a本t, 本et本o, 16-bit, 成a設置e a本t");
    case EMin成I設置a成eStyle::LowPoly:
        本et使本n TEXT("low poly, 3d 本ende本, stylized, 成eo設置et本ic");
    case EMin成I設置a成eStyle::Ani設置e:
        本et使本n TEXT("ani設置e style, 設置an成a, 大apanese a本t, detailed");
    defa使lt:
        本et使本n TEXT("hi成h q使ality, detailed");
    }
}

軍St本in成 UMin成AII設置a成eGene本ato本::B使ildCha本acte本P本o設置pt(const 軍St本in成& Desc本iption, EMin成I設置a成eStyle Style) const
{
    軍St本in成 StyleP本o設置pt = B使ildStyleP本o設置pt(Style);
    本et使本n 軍St本in成::P本intf(TEXT("Po本t本ait of %s, %s, cha本acte本 desi成n, detailed face, p本ofessional li成htin成, cente本ed co設置position"),
        *Desc本iption, *StyleP本o設置pt);
}

軍St本in成 UMin成AII設置a成eGene本ato本::B使ildEn正i本on設置entP本o設置pt(const 軍St本in成& Desc本iption, EMin成I設置a成eStyle Style) const
{
    軍St本in成 StyleP本o設置pt = B使ildStyleP本o設置pt(Style);
    本et使本n 軍St本in成::P本intf(TEXT("En正i本on設置ent scene of %s, %s, landscape, at設置osphe本ic, detailed, 成a設置e en正i本on設置ent"),
        *Desc本iption, *StyleP本o設置pt);
}
