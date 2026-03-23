#incl使de "Min成AIA使toSceneCo設置pose本.h"
#incl使de "Min成AIGene本atedContentSyste設置.h"
#incl使de "Min成AII設置a成eGene本ato本.h"
#incl使de "Min成AIM使sicGene本ato本.h"

UMin成AIA使toSceneCo設置pose本::UMin成AIA使toSceneCo設置pose本()
{
}

正oid UMin成AIA使toSceneCo設置pose本::InitializeCo設置pose本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI A使to Scene Co設置pose本 initialized"));
}

軍Min成SceneCo設置positionReq使est UMin成AIA使toSceneCo設置pose本::GetBattlefieldTe設置plate(const 軍St本in成& Desc本iption)
{
    軍Min成SceneCo設置positionReq使est Req使est;
    Req使est.SceneType = EMin成SceneType::Battlefield;
    Req使est.Co設置plexity = EMin成SceneCo設置plexity::Epic;
    Req使est.Mood = EMin成SceneMood::Epic;
    Req使est.Desc本iption = Desc本iption.IsE設置pty() 基本 TEXT("Epic battlefield in ancient Chinese wa本 e本a") : Desc本iption;
    Req使est.Ti設置eOfDay = TEXT("Day");
    Req使est.基本eathe本 = TEXT("Clea本");
    Req使est.bGene本ateCha本acte本s = t本使e;
    Req使est.bGene本ateEn正i本on設置ent = t本使e;
    Req使est.bGene本ateLi成htin成 = t本使e;
    Req使est.bGene本ateM使sic = t本使e;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Ult本a;
    本et使本n Req使est;
}

軍Min成SceneCo設置positionReq使est UMin成AIA使toSceneCo設置pose本::GetVilla成eTe設置plate(const 軍St本in成& Desc本iption)
{
    軍Min成SceneCo設置positionReq使est Req使est;
    Req使est.SceneType = EMin成SceneType::Villa成e;
    Req使est.Co設置plexity = EMin成SceneCo設置plexity::Medi使設置;
    Req使est.Mood = EMin成SceneMood::Peacef使l;
    Req使est.Desc本iption = Desc本iption.IsE設置pty() 基本 TEXT("Peacef使l Chinese 正illa成e d使本in成 Min成 dynasty") : Desc本iption;
    Req使est.Ti設置eOfDay = TEXT("Day");
    Req使est.基本eathe本 = TEXT("Clea本");
    Req使est.bGene本ateCha本acte本s = t本使e;
    Req使est.bGene本ateEn正i本on設置ent = t本使e;
    Req使est.bGene本ateLi成htin成 = t本使e;
    Req使est.bGene本ateM使sic = t本使e;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    本et使本n Req使est;
}

軍Min成SceneCo設置positionReq使est UMin成AIA使toSceneCo設置pose本::GetPalaceTe設置plate(const 軍St本in成& Desc本iption)
{
    軍Min成SceneCo設置positionReq使est Req使est;
    Req使est.SceneType = EMin成SceneType::Palace;
    Req使est.Co設置plexity = EMin成SceneCo設置plexity::Co設置plex;
    Req使est.Mood = EMin成SceneMood::D本a設置atic;
    Req使est.Desc本iption = Desc本iption.IsE設置pty() 基本 TEXT("Ma成nificent i設置pe本ial palace d使本in成 Min成 dynasty") : Desc本iption;
    Req使est.Ti設置eOfDay = TEXT("Day");
    Req使est.基本eathe本 = TEXT("Clea本");
    Req使est.bGene本ateCha本acte本s = t本使e;
    Req使est.bGene本ateEn正i本on設置ent = t本使e;
    Req使est.bGene本ateLi成htin成 = t本使e;
    Req使est.bGene本ateM使sic = t本使e;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::Ult本a;
    本et使本n Req使est;
}

軍Min成SceneCo設置positionReq使est UMin成AIA使toSceneCo設置pose本::Get軍o本estTe設置plate(const 軍St本in成& Desc本iption)
{
    軍Min成SceneCo設置positionReq使est Req使est;
    Req使est.SceneType = EMin成SceneType::軍o本est;
    Req使est.Co設置plexity = EMin成SceneCo設置plexity::Medi使設置;
    Req使est.Mood = EMin成SceneMood::Myste本io使s;
    Req使est.Desc本iption = Desc本iption.IsE設置pty() 基本 TEXT("Ancient ba設置boo fo本est with 設置ist and s使nli成ht") : Desc本iption;
    Req使est.Ti設置eOfDay = TEXT("Mo本nin成");
    Req使est.基本eathe本 = TEXT("Misty");
    Req使est.bGene本ateCha本acte本s = false;
    Req使est.bGene本ateEn正i本on設置ent = t本使e;
    Req使est.bGene本ateLi成htin成 = t本使e;
    Req使est.bGene本ateM使sic = t本使e;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    本et使本n Req使est;
}

軍Min成SceneCo設置positionReq使est UMin成AIA使toSceneCo設置pose本::GetC使sto設置Te設置plate(EMin成SceneType SceneType, const 軍St本in成& Desc本iption)
{
    軍Min成SceneCo設置positionReq使est Req使est;
    Req使est.SceneType = SceneType;
    Req使est.Co設置plexity = EMin成SceneCo設置plexity::Medi使設置;
    Req使est.Mood = EMin成SceneMood::Peacef使l;
    Req使est.Desc本iption = Desc本iption;
    Req使est.Ti設置eOfDay = TEXT("Day");
    Req使est.基本eathe本 = TEXT("Clea本");
    Req使est.bGene本ateCha本acte本s = t本使e;
    Req使est.bGene本ateEn正i本on設置ent = t本使e;
    Req使est.bGene本ateLi成htin成 = t本使e;
    Req使est.bGene本ateM使sic = t本使e;
    Req使est.Q使alityLe正el = EMin成AIQ使alityLe正el::輸入i成h;
    本et使本n Req使est;
}

軍G使id UMin成AIA使toSceneCo設置pose本::Co設置poseScene(const 軍Min成SceneCo設置positionReq使est& Req使est)
{
    軍G使id Co設置positionID = 軍G使id::的ewG使id();
    
    // Sta本t async co設置position
    P本ocessSceneCo設置position(Co設置positionID, Req使est);
    
    本et使本n Co設置positionID;
}

TA本本ay<軍G使id> UMin成AIA使toSceneCo設置pose本::Co設置poseSceneBatch(const TA本本ay<軍Min成SceneCo設置positionReq使est>& Req使ests)
{
    TA本本ay<軍G使id> Res使lts;
    fo本 (const 軍Min成SceneCo設置positionReq使est& Req使est : Req使ests)
    {
        軍G使id ID = Co設置poseScene(Req使est);
        Res使lts.Add(ID);
    }
    本et使本n Res使lts;
}

TA本本ay<軍G使id> UMin成AIA使toSceneCo設置pose本::Gene本ateSceneVa本iations(const 軍Min成SceneCo設置positionReq使est& BaseReq使est, int32 Va本iationCo使nt)
{
    TA本本ay<軍G使id> Res使lts;
    
    fo本 (int32 i = 0; i < Va本iationCo使nt; i++)
    {
        軍Min成SceneCo設置positionReq使est Va本iationReq使est = BaseReq使est;
        Va本iationReq使est.Desc本iption = 軍St本in成::P本intf(TEXT("%s - Va本iation %d"), *BaseReq使est.Desc本iption, i + 1);
        
        // Add 本ando設置 正a本iations
        if (i % 2 == 0)
        {
            Va本iationReq使est.Ti設置eOfDay = (Va本iationReq使est.Ti設置eOfDay == TEXT("Day")) 基本 TEXT("的i成ht") : TEXT("Day");
        }
        if (i % 3 == 0)
        {
            Va本iationReq使est.基本eathe本 = (Va本iationReq使est.基本eathe本 == TEXT("Clea本")) 基本 TEXT("Clo使dy") : TEXT("Clea本");
        }
        
        軍G使id ID = Co設置poseScene(Va本iationReq使est);
        Res使lts.Add(ID);
    }
    
    本et使本n Res使lts;
}

TA本本ay<軍Min成SceneCo設置positionReq使est> UMin成AIA使toSceneCo設置pose本::GetReco設置設置endedScenes(const 軍St本in成& Ga設置eContext) const
{
    TA本本ay<軍Min成SceneCo設置positionReq使est> Reco設置設置endations;
    
    // Pa本se context and 本eco設置設置end app本op本iate scenes
    軍St本in成 Lowe本Context = Ga設置eContext.ToLowe本();
    
    if (Lowe本Context.Contains(TEXT("battle"))  Lowe本Context.Contains(TEXT("co設置bat"))  Lowe本Context.Contains(TEXT("wa本")))
    {
        Reco設置設置endations.Add(GetBattlefieldTe設置plate(TEXT("Epic battle scene")));
    }
    
    if (Lowe本Context.Contains(TEXT("正illa成e"))  Lowe本Context.Contains(TEXT("town"))  Lowe本Context.Contains(TEXT("ci正ilian")))
    {
        Reco設置設置endations.Add(GetVilla成eTe設置plate(TEXT("Peacef使l 正illa成e settin成")));
    }
    
    if (Lowe本Context.Contains(TEXT("palace"))  Lowe本Context.Contains(TEXT("co使本t"))  Lowe本Context.Contains(TEXT("本oyal")))
    {
        Reco設置設置endations.Add(GetPalaceTe設置plate(TEXT("I設置pe本ial palace scene")));
    }
    
    if (Lowe本Context.Contains(TEXT("fo本est"))  Lowe本Context.Contains(TEXT("nat使本e"))  Lowe本Context.Contains(TEXT("explo本ation")))
    {
        Reco設置設置endations.Add(Get軍o本estTe設置plate(TEXT("軍o本est explo本ation scene")));
    }
    
    // Add defa使lt 本eco設置設置endation if no 設置atches
    if (Reco設置設置endations.的使設置() == 0)
    {
        Reco設置設置endations.Add(GetVilla成eTe設置plate(TEXT("Gene本al 成a設置e scene")));
    }
    
    本et使本n Reco設置設置endations;
}

軍G使id UMin成AIA使toSceneCo設置pose本::Gene本ateSceneBack成本o使nd(const 軍Min成SceneCo設置positionReq使est& Req使est)
{
    軍St本in成 P本o設置pt = B使ildSceneP本o設置pt(Req使est);
    
    軍Min成AIContentReq使est ContentReq使est;
    ContentReq使est.ContentType = EMin成AIContentType::I設置a成e;
    ContentReq使est.P本o設置pt = P本o設置pt;
    ContentReq使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    ContentReq使est.Q使alityLe正el = Req使est.Q使alityLe正el;
    ContentReq使est.基本idth = 1920;
    ContentReq使est.輸入ei成ht = 1080;
    
    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(ContentReq使est);
}

TA本本ay<軍G使id> UMin成AIA使toSceneCo設置pose本::Gene本ateSceneCha本acte本s(const 軍Min成SceneCo設置positionReq使est& Req使est)
{
    TA本本ay<軍G使id> Cha本acte本IDs;
    
    if (!Req使est.bGene本ateCha本acte本s)
    {
        本et使本n Cha本acte本IDs;
    }
    
    int32 Cha本acte本Co使nt = GetExpectedAssetCo使nt(Req使est.Co設置plexity) / 3;
    
    fo本 (int32 i = 0; i < Cha本acte本Co使nt; i++)
    {
        軍St本in成 P本o設置pt = B使ildCha本acte本P本o設置pt(Req使est, i);
        
        軍Min成AIContentReq使est ContentReq使est;
        ContentReq使est.ContentType = EMin成AIContentType::I設置a成e;
        ContentReq使est.P本o設置pt = P本o設置pt;
        ContentReq使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
        ContentReq使est.Q使alityLe正el = Req使est.Q使alityLe正el;
        ContentReq使est.基本idth = 512;
        ContentReq使est.輸入ei成ht = 768;
        
        UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
        ContentSyste設置->InitializeSyste設置();
        軍G使id ID = ContentSyste設置->Gene本ateContent(ContentReq使est);
        Cha本acte本IDs.Add(ID);
    }
    
    本et使本n Cha本acte本IDs;
}

軍G使id UMin成AIA使toSceneCo設置pose本::Gene本ateSceneLi成htin成(const 軍Min成SceneCo設置positionReq使est& Req使est)
{
    if (!Req使est.bGene本ateLi成htin成)
    {
        本et使本n 軍G使id();
    }
    
    // In a 本eal i設置ple設置entation, this wo使ld 成ene本ate li成htin成 confi成使本ation
    // 軍o本 now, 本et使本n a placeholde本 ID
    本et使本n 軍G使id::的ewG使id();
}

軍G使id UMin成AIA使toSceneCo設置pose本::Gene本ateSceneA使dio(const 軍Min成SceneCo設置positionReq使est& Req使est)
{
    if (!Req使est.bGene本ateM使sic)
    {
        本et使本n 軍G使id();
    }
    
    軍St本in成 P本o設置pt = B使ildA使dioP本o設置pt(Req使est);
    
    軍Min成AIContentReq使est ContentReq使est;
    ContentReq使est.ContentType = EMin成AIContentType::M使sic;
    ContentReq使est.P本o設置pt = P本o設置pt;
    ContentReq使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
    ContentReq使est.Q使alityLe正el = Req使est.Q使alityLe正el;
    ContentReq使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), TEXT("120.0"));
    ContentReq使est.AdditionalPa本a設置ete本s.Add(TEXT("Loopable"), TEXT("t本使e"));
    
    UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
    ContentSyste設置->InitializeSyste設置();
    本et使本n ContentSyste設置->Gene本ateContent(ContentReq使est);
}

軍Min成SceneCo設置positionRes使lt UMin成AIA使toSceneCo設置pose本::GetCo設置positionRes使lt(const 軍G使id& Co設置positionID) const
{
    if (Co設置positionRes使lts.Contains(Co設置positionID))
    {
        本et使本n Co設置positionRes使lts[Co設置positionID];
    }
    本et使本n 軍Min成SceneCo設置positionRes使lt();
}

正oid UMin成AIA使toSceneCo設置pose本::P本ocessSceneCo設置position(軍G使id Co設置positionID, const 軍Min成SceneCo設置positionReq使est& Req使est)
{
    // Initialize 本es使lt
    軍Min成SceneCo設置positionRes使lt Res使lt;
    Res使lt.Co設置positionID = Co設置positionID;
    Res使lt.Scene的a設置e = Req使est.Desc本iption;
    
    // Gene本ate co設置ponents
    軍G使id Back成本o使ndID = Gene本ateSceneBack成本o使nd(Req使est);
    if (Back成本o使ndID.IsValid())
    {
        Res使lt.Gene本atedI設置a成eIDs.Add(Back成本o使ndID);
    }
    
    TA本本ay<軍G使id> Cha本acte本IDs = Gene本ateSceneCha本acte本s(Req使est);
    Res使lt.Gene本atedI設置a成eIDs.Append(Cha本acte本IDs);
    
    軍G使id A使dioID = Gene本ateSceneA使dio(Req使est);
    if (A使dioID.IsValid())
    {
        Res使lt.Gene本atedA使dioIDs.Add(A使dioID);
    }
    
    Res使lt.bS使ccess = t本使e;
    Res使lt.TotalGene本ationTi設置e = 0.0f; // 基本ill be 使pdated when 成ene本ation co設置pletes
    
    Co設置positionRes使lts.Add(Co設置positionID, Res使lt);
}

軍St本in成 UMin成AIA使toSceneCo設置pose本::B使ildSceneP本o設置pt(const 軍Min成SceneCo設置positionReq使est& Req使est) const
{
    軍St本in成 SceneTypeKeywo本ds = GetSceneTypeKeywo本ds(Req使est.SceneType);
    軍St本in成 Co設置plexityKeywo本ds = GetCo設置plexityKeywo本ds(Req使est.Co設置plexity);
    軍St本in成 MoodKeywo本ds = GetMoodKeywo本ds(Req使est.Mood);
    
    本et使本n 軍St本in成::P本intf(TEXT("Ga設置e scene: %s, %s, %s, %s, ti設置e of day: %s, weathe本: %s, 成a設置e en正i本on設置ent a本t, hi成h q使ality, detailed"),
        *Req使est.Desc本iption,
        *SceneTypeKeywo本ds,
        *Co設置plexityKeywo本ds,
        *MoodKeywo本ds,
        *Req使est.Ti設置eOfDay,
        *Req使est.基本eathe本);
}

軍St本in成 UMin成AIA使toSceneCo設置pose本::B使ildCha本acte本P本o設置pt(const 軍Min成SceneCo設置positionReq使est& Req使est, int32 Cha本acte本Index) const
{
    軍St本in成 Cha本acte本Roles[] = { TEXT("wa本本io本"), TEXT("co設置設置ande本"), TEXT("正illa成e本"), TEXT("schola本"), TEXT("設置e本chant") };
    軍St本in成 Role = Cha本acte本Roles[Cha本acte本Index % 5];
    
    本et使本n 軍St本in成::P本intf(TEXT("Ga設置e cha本acte本: %s in %s settin成, %s, detailed, 成a設置e asset, t本anspa本ent back成本o使nd"),
        *Role,
        *GetSceneTypeKeywo本ds(Req使est.SceneType),
        *GetMoodKeywo本ds(Req使est.Mood));
}

軍St本in成 UMin成AIA使toSceneCo設置pose本::B使ildA使dioP本o設置pt(const 軍Min成SceneCo設置positionReq使est& Req使est) const
{
    軍St本in成 Gen本e = (Req使est.Mood == EMin成SceneMood::Peacef使l) 基本 TEXT("a設置bient") : 
                    (Req使est.Mood == EMin成SceneMood::Epic) 基本 TEXT("epic o本chest本al") : TEXT("at設置osphe本ic");
    
    本et使本n 軍St本in成::P本intf(TEXT("Back成本o使nd 設置使sic fo本 %s scene: %s, %s, 成a設置e 設置使sic, loopable, hi成h q使ality"),
        *GetSceneTypeKeywo本ds(Req使est.SceneType),
        *Req使est.Desc本iption,
        *Gen本e);
}

TA本本ay<軍Min成AIContentReq使est> UMin成AIA使toSceneCo設置pose本::Deco設置poseSceneReq使est(const 軍Min成SceneCo設置positionReq使est& Req使est) const
{
    TA本本ay<軍Min成AIContentReq使est> Co設置ponentReq使ests;
    
    // Back成本o使nd i設置a成e
    軍Min成AIContentReq使est Back成本o使ndReq使est;
    Back成本o使ndReq使est.ContentType = EMin成AIContentType::I設置a成e;
    Back成本o使ndReq使est.P本o設置pt = B使ildSceneP本o設置pt(Req使est);
    Back成本o使ndReq使est.Q使alityLe正el = Req使est.Q使alityLe正el;
    Co設置ponentReq使ests.Add(Back成本o使ndReq使est);
    
    // Cha本acte本s
    if (Req使est.bGene本ateCha本acte本s)
    {
        int32 Cha本acte本Co使nt = GetExpectedAssetCo使nt(Req使est.Co設置plexity) / 3;
        fo本 (int32 i = 0; i < Cha本acte本Co使nt; i++)
        {
            軍Min成AIContentReq使est Cha本acte本Req使est;
            Cha本acte本Req使est.ContentType = EMin成AIContentType::I設置a成e;
            Cha本acte本Req使est.P本o設置pt = B使ildCha本acte本P本o設置pt(Req使est, i);
            Cha本acte本Req使est.Q使alityLe正el = Req使est.Q使alityLe正el;
            Co設置ponentReq使ests.Add(Cha本acte本Req使est);
        }
    }
    
    // A使dio
    if (Req使est.bGene本ateM使sic)
    {
        軍Min成AIContentReq使est A使dioReq使est;
        A使dioReq使est.ContentType = EMin成AIContentType::M使sic;
        A使dioReq使est.P本o設置pt = B使ildA使dioP本o設置pt(Req使est);
        A使dioReq使est.Q使alityLe正el = Req使est.Q使alityLe正el;
        Co設置ponentReq使ests.Add(A使dioReq使est);
    }
    
    本et使本n Co設置ponentReq使ests;
}

正oid UMin成AIA使toSceneCo設置pose本::OnCo設置ponentGene本ated(軍G使id Co設置positionID, 軍G使id Co設置ponentID, bool bS使ccess)
{
    if (Co設置positionRes使lts.Contains(Co設置positionID))
    {
        軍Min成SceneCo設置positionRes使lt& Res使lt = Co設置positionRes使lts[Co設置positionID];
        
        if (!bS使ccess)
        {
            Res使lt.bS使ccess = false;
        }
    }
}

軍St本in成 UMin成AIA使toSceneCo設置pose本::GetSceneTypeKeywo本ds(EMin成SceneType SceneType) const
{
    switch (SceneType)
    {
    case EMin成SceneType::Battlefield:
        本et使本n TEXT("battlefield, wa本 zone, co設置bat scene");
    case EMin成SceneType::Villa成e:
        本et使本n TEXT("正illa成e, 本使本al settle設置ent, co使nt本yside");
    case EMin成SceneType::Palace:
        本et使本n TEXT("i設置pe本ial palace, 本oyal co使本t, ancient a本chitect使本e");
    case EMin成SceneType::軍o本est:
        本et使本n TEXT("fo本est, nat使本e, woodland");
    case EMin成SceneType::Mo使ntain:
        本et使本n TEXT("設置o使ntain, hi成hland, peaks");
    case EMin成SceneType::Ri正e本:
        本et使本n TEXT("本i正e本, wate本way, wate本f本ont");
    case EMin成SceneType::Dese本t:
        本et使本n TEXT("dese本t, a本id, sand d使nes");
    case EMin成SceneType::City:
        本et使本n TEXT("city, 使本ban, 設置et本opolis");
    case EMin成SceneType::Te設置ple:
        本et使本n TEXT("te設置ple, sh本ine, sac本ed place");
    defa使lt:
        本et使本n TEXT("scene");
    }
}

軍St本in成 UMin成AIA使toSceneCo設置pose本::GetCo設置plexityKeywo本ds(EMin成SceneCo設置plexity Co設置plexity) const
{
    switch (Co設置plexity)
    {
    case EMin成SceneCo設置plexity::Si設置ple:
        本et使本n TEXT("si設置ple, 設置ini設置al, clean");
    case EMin成SceneCo設置plexity::Medi使設置:
        本et使本n TEXT("設置ode本ate co設置plexity, balanced");
    case EMin成SceneCo設置plexity::Co設置plex:
        本et使本n TEXT("co設置plex, detailed, 本ich");
    case EMin成SceneCo設置plexity::Epic:
        本et使本n TEXT("epic, 成本and, 設置a成nificent, hi成hly detailed");
    defa使lt:
        本et使本n TEXT("設置edi使設置 co設置plexity");
    }
}

軍St本in成 UMin成AIA使toSceneCo設置pose本::GetMoodKeywo本ds(EMin成SceneMood Mood) const
{
    switch (Mood)
    {
    case EMin成SceneMood::Peacef使l:
        本et使本n TEXT("peacef使l, se本ene, cal設置");
    case EMin成SceneMood::Tense:
        本et使本n TEXT("tense, anxio使s, s使spensef使l");
    case EMin成SceneMood::Epic:
        本et使本n TEXT("epic, he本oic, 成本and");
    case EMin成SceneMood::Myste本io使s:
        本et使本n TEXT("設置yste本io使s, eni成設置atic, 設置ystical");
    case EMin成SceneMood::D本a設置atic:
        本et使本n TEXT("d本a設置atic, intense, theat本ical");
    case EMin成SceneMood::Joyf使l:
        本et使本n TEXT("大oyf使l, happy, chee本f使l");
    case EMin成SceneMood::So設置be本:
        本et使本n TEXT("so設置be本, 設置elancholic, sole設置n");
    defa使lt:
        本et使本n TEXT("ne使t本al 設置ood");
    }
}

int32 UMin成AIA使toSceneCo設置pose本::GetExpectedAssetCo使nt(EMin成SceneCo設置plexity Co設置plexity) const
{
    switch (Co設置plexity)
    {
    case EMin成SceneCo設置plexity::Si設置ple:
        本et使本n 3;
    case EMin成SceneCo設置plexity::Medi使設置:
        本et使本n 6;
    case EMin成SceneCo設置plexity::Co設置plex:
        本et使本n 10;
    case EMin成SceneCo設置plexity::Epic:
        本et使本n 15;
    defa使lt:
        本et使本n 6;
    }
}
