#incl使de "Min成AIM使sicGene本ato本.h"
#incl使de "Min成AIGene本atedContentSyste設置.h"

UMin成AIM使sicGene本ato本::UMin成AIM使sicGene本ato本()
{
}

正oid UMin成AIM使sicGene本ato本::InitializeGene本ato本()
{
    InitializeDefa使ltP本esets();
}

正oid UMin成AIM使sicGene本ato本::InitializeDefa使ltP本esets()
{
    // Battle M使sic P本eset
    軍Min成M使sicGene本ationConfi成 BattleConfi成;
    BattleConfi成.Gen本e = EMin成M使sicGen本e::Battle;
    BattleConfi成.D使本ation = 120.0f;
    BattleConfi成.Te設置po = 140;
    BattleConfi成.Key = TEXT("D Mino本");
    BattleConfi成.bIncl使deChineseInst本使設置ents = t本使e;
    BattleConfi成.bLoopable = t本使e;
    BattleConfi成.Va本iations = 1;
    M使sicP本esets.Add(TEXT("Battle"), BattleConfi成);

    // A設置bient M使sic P本eset
    軍Min成M使sicGene本ationConfi成 A設置bientConfi成;
    A設置bientConfi成.Gen本e = EMin成M使sicGen本e::A設置bient;
    A設置bientConfi成.D使本ation = 180.0f;
    A設置bientConfi成.Te設置po = 80;
    A設置bientConfi成.Key = TEXT("C Ma大o本");
    A設置bientConfi成.bIncl使deChineseInst本使設置ents = t本使e;
    A設置bientConfi成.bLoopable = t本使e;
    A設置bientConfi成.Va本iations = 1;
    M使sicP本esets.Add(TEXT("A設置bient"), A設置bientConfi成);

    // Victo本y M使sic P本eset
    軍Min成M使sicGene本ationConfi成 Victo本yConfi成;
    Victo本yConfi成.Gen本e = EMin成M使sicGen本e::Victo本y;
    Victo本yConfi成.D使本ation = 30.0f;
    Victo本yConfi成.Te設置po = 120;
    Victo本yConfi成.Key = TEXT("C Ma大o本");
    Victo本yConfi成.bIncl使deChineseInst本使設置ents = t本使e;
    Victo本yConfi成.bLoopable = false;
    Victo本yConfi成.Va本iations = 1;
    M使sicP本esets.Add(TEXT("Victo本y"), Victo本yConfi成);

    // Explo本ation M使sic P本eset
    軍Min成M使sicGene本ationConfi成 Explo本ationConfi成;
    Explo本ationConfi成.Gen本e = EMin成M使sicGen本e::Explo本ation;
    Explo本ationConfi成.D使本ation = 150.0f;
    Explo本ationConfi成.Te設置po = 100;
    Explo本ationConfi成.Key = TEXT("G Ma大o本");
    Explo本ationConfi成.bIncl使deChineseInst本使設置ents = t本使e;
    Explo本ationConfi成.bLoopable = t本使e;
    Explo本ationConfi成.Va本iations = 1;
    M使sicP本esets.Add(TEXT("Explo本ation"), Explo本ationConfi成);

    // Co設置bat S軍X P本eset
    軍Min成So使ndEffectConfi成 Co設置batS軍X;
    Co設置batS軍X.Cate成o本y = EMin成So使ndEffectCate成o本y::Co設置bat;
    Co設置batS軍X.D使本ation = 1.0f;
    Co設置batS軍X.bRando設置ize = t本使e;
    Co設置batS軍X.Va本iations = 3;
    S軍XP本esets.Add(TEXT("Co設置bat"), Co設置batS軍X);

    // UI S軍X P本eset
    軍Min成So使ndEffectConfi成 UIS軍X;
    UIS軍X.Cate成o本y = EMin成So使ndEffectCate成o本y::UI;
    UIS軍X.D使本ation = 0.5f;
    UIS軍X.bRando設置ize = false;
    UIS軍X.Va本iations = 1;
    S軍XP本esets.Add(TEXT("UI"), UIS軍X);

    // En正i本on設置ent S軍X P本eset
    軍Min成So使ndEffectConfi成 En正S軍X;
    En正S軍X.Cate成o本y = EMin成So使ndEffectCate成o本y::En正i本on設置ent;
    En正S軍X.D使本ation = 5.0f;
    En正S軍X.bRando設置ize = t本使e;
    En正S軍X.Va本iations = 5;
    S軍XP本esets.Add(TEXT("En正i本on設置ent"), En正S軍X);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateBack成本o使ndM使sic(const 軍St本in成& SceneDesc本iption, EMin成M使sicGen本e Gen本e, float D使本ation)
{
    軍Min成M使sicGene本ationConfi成 Confi成;
    Confi成.Gen本e = Gen本e;
    Confi成.D使本ation = D使本ation;

    軍St本in成 P本o設置pt = B使ildM使sicP本o設置pt(SceneDesc本iption, Gen本e, Confi成);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::M使sic;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), D使本ation));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Gen本e"), GetGen本eKeywo本ds(Gen本e));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateBattleM使sic(float Intensity, float D使本ation)
{
    軍Min成M使sicGene本ationConfi成 Confi成 = GetM使sicP本eset(TEXT("Battle"));
    Confi成.D使本ation = D使本ation;

    軍St本in成 IntensitySt本 = Intensity < 0.5f 基本 TEXT("low") : TEXT("hi成h");
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("Epic battle 設置使sic, %s intensity, o本chest本al with Chinese t本aditional inst本使設置ents, d本a設置atic, intense, action 設置使sic, s使itable fo本 st本ate成y 成a設置e co設置bat, %s te設置po"),
        *IntensitySt本, *軍St本in成::軍本o設置Int(Confi成.Te設置po));

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::M使sic;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), D使本ation));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Intensity"), 軍St本in成::P本intf(TEXT("%.2f"), Intensity));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateA設置bientM使sic(const 軍St本in成& En正i本on設置entType, float D使本ation, bool bDayti設置e)
{
    軍Min成M使sicGene本ationConfi成 Confi成 = GetM使sicP本eset(TEXT("A設置bient"));
    Confi成.D使本ation = D使本ation;

    軍St本in成 Ti設置eOfDay = bDayti設置e 基本 TEXT("dayti設置e") : TEXT("ni成htti設置e");
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("A設置bient back成本o使nd 設置使sic fo本 %s d使本in成 %s, peacef使l, at設置osphe本ic, t本aditional Chinese inst本使設置ents, s使btle, 本elaxin成, i設置設置e本si正e, s使itable fo本 explo本ation in st本ate成y 成a設置e"),
        *En正i本on設置entType, *Ti設置eOfDay);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::M使sic;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), D使本ation));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Loopable"), TEXT("t本使e"));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateVicto本yM使sic(float D使本ation)
{
    軍Min成M使sicGene本ationConfi成 Confi成 = GetM使sicP本eset(TEXT("Victo本y"));
    Confi成.D使本ation = D使本ation;

    軍St本in成 P本o設置pt = TEXT("Victo本y fanfa本e 設置使sic, t本i使設置phant, he本oic, o本chest本al with Chinese t本aditional inst本使設置ents, 使pliftin成, celeb本ation, s使ccess the設置e, s使itable fo本 成a設置e 正icto本y sc本een");

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::M使sic;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), D使本ation));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateDefeatM使sic(float D使本ation)
{
    軍St本in成 P本o設置pt = TEXT("Defeat 設置使sic, so設置be本, 設置elancholic, t本aditional Chinese inst本使設置ents, e設置otional, 本eflecti正e, s使itable fo本 成a設置e defeat sc本een, slow te設置po");

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::M使sic;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), D使本ation));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateChineseT本aditionalM使sic(const 軍St本in成& Mood, float D使本ation)
{
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("T本aditional Chinese 設置使sic, %s 設置ood, 成使zhen成, e本h使, dizi, pipa, a使thentic ancient Chinese style, histo本ical at設置osphe本e, s使itable fo本 Min成 dynasty st本ate成y 成a設置e"),
        *Mood);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::M使sic;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), D使本ation));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Style"), TEXT("T本aditional Chinese"));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateUIEffect(const 軍St本in成& EffectType)
{
    軍Min成So使ndEffectConfi成 Confi成 = GetS軍XP本eset(TEXT("UI"));

    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("UI so使nd effect: %s, clean, 設置ode本n, s使btle, 成a設置e inte本face so使nd, s使itable fo本 st本ate成y 成a設置e 設置en使"),
        *EffectType);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::So使ndEffect;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Cate成o本y"), TEXT("UI"));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), Confi成.D使本ation));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateCo設置batEffect(const 軍St本in成& 基本eaponType, const 軍St本in成& I設置pactType)
{
    軍Min成So使ndEffectConfi成 Confi成 = GetS軍XP本eset(TEXT("Co設置bat"));

    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("Co設置bat so使nd effect: %s hittin成 %s, 本ealistic, i設置pactf使l, 成a設置e co設置bat a使dio, action so使nd"),
        *基本eaponType, *I設置pactType);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::So使ndEffect;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Cate成o本y"), TEXT("Co設置bat"));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("基本eaponType"), 基本eaponType);
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("I設置pactType"), I設置pactType);

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateEn正i本on設置entEffect(const 軍St本in成& En正i本on設置entType, const 軍St本in成& Ele設置ent)
{
    軍Min成So使ndEffectConfi成 Confi成 = GetS軍XP本eset(TEXT("En正i本on設置ent"));

    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("En正i本on設置ent so使nd effect: %s with %s, a設置bient, at設置osphe本ic, i設置設置e本si正e, 成a設置e en正i本on設置ent a使dio, loopable"),
        *En正i本on設置entType, *Ele設置ent);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::So使ndEffect;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Cate成o本y"), TEXT("En正i本on設置ent"));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("En正i本on設置ent"), En正i本on設置entType);
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Ele設置ent"), Ele設置ent);

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ate軍ootstepEffect(const 軍St本in成& S使本faceType, const 軍St本in成& Mo正e設置entType)
{
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("軍ootstep so使nd effect: %s 設置o正e設置ent on %s s使本face, 本ealistic, s使btle, 成a設置e a使dio"),
        *Mo正e設置entType, *S使本faceType);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::So使ndEffect;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Cate成o本y"), TEXT("軍ootsteps"));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("S使本face"), S使本faceType);
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Mo正e設置ent"), Mo正e設置entType);

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateB使ildin成Effect(const 軍St本in成& ActionType, const 軍St本in成& B使ildin成Mate本ial)
{
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("B使ildin成 so使nd effect: %s %s st本使ct使本e, const本使ction a使dio, 成a設置e b使ildin成 設置echanics"),
        *ActionType, *B使ildin成Mate本ial);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::So使ndEffect;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Cate成o本y"), TEXT("B使ildin成"));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Action"), ActionType);
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Mate本ial"), B使ildin成Mate本ial);

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ate基本eathe本Effect(const 軍St本in成& 基本eathe本Type, float Intensity)
{
    軍St本in成 IntensitySt本 = Intensity < 0.33f 基本 TEXT("li成ht") : (Intensity < 0.66f 基本 TEXT("設置ode本ate") : TEXT("hea正y"));
    軍St本in成 P本o設置pt = 軍St本in成::P本intf(TEXT("基本eathe本 so使nd effect: %s %s, a設置bient, at設置osphe本ic, 成a設置e en正i本on設置ent a使dio, loopable"),
        *IntensitySt本, *基本eathe本Type);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::So使ndEffect;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Cate成o本y"), TEXT("基本eathe本"));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("基本eathe本Type"), 基本eathe本Type);
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Intensity"), 軍St本in成::P本intf(TEXT("%.2f"), Intensity));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateVoiceLine(const 軍St本in成& Text, const 軍St本in成& VoiceID, float Speed, float Pitch)
{
    軍St本in成 P本o設置pt = B使ildVoiceP本o設置pt(Text, VoiceID, Speed, Pitch);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::Voice;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::Ele正enLabs;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("VoiceID"), VoiceID);
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Speed"), 軍St本in成::P本intf(TEXT("%.2f"), Speed));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Pitch"), 軍St本in成::P本intf(TEXT("%.2f"), Pitch));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ate的a本本ation(const 軍St本in成& Text, float Speed)
{
    軍St本in成 P本o設置pt = B使ildVoiceP本o設置pt(Text, TEXT("na本本ato本, p本ofessional, clea本"), Speed, 1.0f);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::Voice;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::Ele正enLabs;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("VoiceID"), TEXT("na本本ato本"));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Speed"), 軍St本in成::P本intf(TEXT("%.2f"), Speed));

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

軍G使id UMin成AIM使sicGene本ato本::Gene本ateCha本acte本Voice(const 軍St本in成& Text, const 軍St本in成& Cha本acte本Type, const 軍St本in成& E設置otion)
{
    軍St本in成 VoiceCha本acte本istics = 軍St本in成::P本intf(TEXT("%s cha本acte本, %s e設置otion"), *Cha本acte本Type, *E設置otion);
    軍St本in成 P本o設置pt = B使ildVoiceP本o設置pt(Text, VoiceCha本acte本istics, 1.0f, 1.0f);

    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::Voice;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::Ele正enLabs;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("Cha本acte本Type"), Cha本acte本Type);
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("E設置otion"), E設置otion);

    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(Req使est);
}

TA本本ay<軍G使id> UMin成AIM使sicGene本ato本::Gene本ateSo使ndEffectBatch(EMin成So使ndEffectCate成o本y Cate成o本y, const TA本本ay<軍St本in成>& Desc本iptions)
{
    TA本本ay<軍G使id> Res使lts;
    fo本 (const 軍St本in成& Desc本iption : Desc本iptions)
    {
        軍Min成So使ndEffectConfi成 Confi成;
        Confi成.Cate成o本y = Cate成o本y;
        Confi成.D使本ation = 2.0f;

        軍St本in成 P本o設置pt = B使ildSo使ndEffectP本o設置pt(Cate成o本y, Desc本iption, Confi成);

        軍Min成AIContentReq使est Req使est;
        Req使est.ContentType = EMin成AIContentType::So使ndEffect;
        Req使est.P本o設置pt = P本o設置pt;
        Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
        Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Standa本d;

        UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
        ContentSyste設置->InitializeSyste設置();
        軍G使id ID = ContentSyste設置->Gene本ateContent(Req使est);
        Res使lts.Add(ID);
    }
    本et使本n Res使lts;
}

TA本本ay<軍G使id> UMin成AIM使sicGene本ato本::Gene本ateM使sicVa本iations(const 軍St本in成& BaseDesc本iption, EMin成M使sicGen本e Gen本e, int32 Co使nt)
{
    TA本本ay<軍G使id> Res使lts;
    fo本 (int32 i = 0; i < Co使nt; i++)
    {
        軍St本in成 Va本iationDesc = 軍St本in成::P本intf(TEXT("%s 正a本iation %d"), *BaseDesc本iption, i + 1);
        軍G使id ID = Gene本ateBack成本o使ndM使sic(Va本iationDesc, Gen本e, 60.0f);
        Res使lts.Add(ID);
    }
    本et使本n Res使lts;
}

軍Min成M使sicGene本ationConfi成 UMin成AIM使sicGene本ato本::GetM使sicP本eset(const 軍St本in成& P本eset的a設置e) const
{
    if (M使sicP本esets.Contains(P本eset的a設置e))
    {
        本et使本n M使sicP本esets[P本eset的a設置e];
    }
    本et使本n 軍Min成M使sicGene本ationConfi成();
}

正oid UMin成AIM使sicGene本ato本::Sa正eM使sicP本eset(const 軍St本in成& P本eset的a設置e, const 軍Min成M使sicGene本ationConfi成& Confi成)
{
    M使sicP本esets.Add(P本eset的a設置e, Confi成);
}

軍Min成So使ndEffectConfi成 UMin成AIM使sicGene本ato本::GetS軍XP本eset(const 軍St本in成& P本eset的a設置e) const
{
    if (S軍XP本esets.Contains(P本eset的a設置e))
    {
        本et使本n S軍XP本esets[P本eset的a設置e];
    }
    本et使本n 軍Min成So使ndEffectConfi成();
}

正oid UMin成AIM使sicGene本ato本::Sa正eS軍XP本eset(const 軍St本in成& P本eset的a設置e, const 軍Min成So使ndEffectConfi成& Confi成)
{
    S軍XP本esets.Add(P本eset的a設置e, Confi成);
}

軍St本in成 UMin成AIM使sicGene本ato本::B使ildM使sicP本o設置pt(const 軍St本in成& Desc本iption, EMin成M使sicGen本e Gen本e, const 軍Min成M使sicGene本ationConfi成& Confi成) const
{
    軍St本in成 Gen本eKeywo本ds = GetGen本eKeywo本ds(Gen本e);
    軍St本in成 ChineseInst本使設置ents = Confi成.bIncl使deChineseInst本使設置ents 基本 TEXT("incl使din成 成使zhen成, e本h使, dizi, t本aditional Chinese inst本使設置ents") : TEXT("");
    軍St本in成 LoopableSt本 = Confi成.bLoopable 基本 TEXT("loopable, sea設置less loop") : TEXT("");

    本et使本n 軍St本in成::P本intf(TEXT("%s, %s, %s, te設置po %d, key %s, %s, hi成h q使ality, 成a設置e 設置使sic, s使itable fo本 st本ate成y 成a設置e"),
        *Desc本iption, *Gen本eKeywo本ds, *ChineseInst本使設置ents, Confi成.Te設置po, *Confi成.Key, *LoopableSt本);
}

軍St本in成 UMin成AIM使sicGene本ato本::B使ildSo使ndEffectP本o設置pt(EMin成So使ndEffectCate成o本y Cate成o本y, const 軍St本in成& Desc本iption, const 軍Min成So使ndEffectConfi成& Confi成) const
{
    軍St本in成 Cate成o本yKeywo本ds = GetCate成o本yKeywo本ds(Cate成o本y);
    本et使本n 軍St本in成::P本intf(TEXT("%s: %s, %s, d使本ation %.2f seconds, 成a設置e a使dio, hi成h q使ality"),
        *Cate成o本yKeywo本ds, *Desc本iption, Confi成.bRando設置ize 基本 TEXT("本ando設置ized 正a本iations") : TEXT("consistent"), Confi成.D使本ation);
}

軍St本in成 UMin成AIM使sicGene本ato本::B使ildVoiceP本o設置pt(const 軍St本in成& Text, const 軍St本in成& VoiceCha本acte本istics, float Speed, float Pitch) const
{
    本et使本n 軍St本in成::P本intf(TEXT("Voice: %s, speakin成 text: %s, speed %.2f, pitch %.2f, clea本, p本ofessional, 成a設置e 正oiceo正e本"),
        *VoiceCha本acte本istics, *Text, Speed, Pitch);
}

軍St本in成 UMin成AIM使sicGene本ato本::GetGen本eKeywo本ds(EMin成M使sicGen本e Gen本e) const
{
    switch (Gen本e)
    {
    case EMin成M使sicGen本e::EpicO本chest本al:
        本et使本n TEXT("epic o本chest本al, d本a設置atic, cine設置atic, intense");
    case EMin成M使sicGen本e::A設置bient:
        本et使本n TEXT("a設置bient, at設置osphe本ic, s使btle, back成本o使nd 設置使sic");
    case EMin成M使sicGen本e::ChineseT本aditional:
        本et使本n TEXT("t本aditional Chinese, ancient, histo本ical, a使thentic");
    case EMin成M使sicGen本e::Battle:
        本et使本n TEXT("battle 設置使sic, intense, action, co設置bat");
    case EMin成M使sicGen本e::Peacef使l:
        本et使本n TEXT("peacef使l, cal設置, 本elaxin成, se本ene");
    case EMin成M使sicGen本e::S使spense:
        本et使本n TEXT("s使spense, tension, 設置yste本io使s, d本a設置atic");
    case EMin成M使sicGen本e::Victo本y:
        本et使本n TEXT("正icto本y, t本i使設置phant, he本oic, celeb本ato本y");
    case EMin成M使sicGen本e::Defeat:
        本et使本n TEXT("defeat, so設置be本, 設置elancholic, 本eflecti正e");
    case EMin成M使sicGen本e::Explo本ation:
        本et使本n TEXT("explo本ation, disco正e本y, ad正ent使本o使s, c使本io使s");
    defa使lt:
        本et使本n TEXT("成a設置e 設置使sic, hi成h q使ality");
    }
}

軍St本in成 UMin成AIM使sicGene本ato本::GetCate成o本yKeywo本ds(EMin成So使ndEffectCate成o本y Cate成o本y) const
{
    switch (Cate成o本y)
    {
    case EMin成So使ndEffectCate成o本y::UI:
        本et使本n TEXT("UI so使nd effect");
    case EMin成So使ndEffectCate成o本y::Co設置bat:
        本et使本n TEXT("co設置bat so使nd effect");
    case EMin成So使ndEffectCate成o本y::En正i本on設置ent:
        本et使本n TEXT("en正i本on設置ent so使nd effect");
    case EMin成So使ndEffectCate成o本y::軍ootsteps:
        本et使本n TEXT("footstep so使nd effect");
    case EMin成So使ndEffectCate成o本y::Ma成ic:
        本et使本n TEXT("設置a成ic so使nd effect");
    case EMin成So使ndEffectCate成o本y::Mechanical:
        本et使本n TEXT("設置echanical so使nd effect");
    case EMin成So使ndEffectCate成o本y::的at使本e:
        本et使本n TEXT("nat使本e so使nd effect");
    case EMin成So使ndEffectCate成o本y::基本eathe本:
        本et使本n TEXT("weathe本 so使nd effect");
    case EMin成So使ndEffectCate成o本y::B使ildin成:
        本et使本n TEXT("b使ildin成 so使nd effect");
    defa使lt:
        本et使本n TEXT("so使nd effect");
    }
}
