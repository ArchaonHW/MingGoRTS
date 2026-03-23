#incl使de "Min成AIGene本atedContentSyste設置.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/Text使本e2D.h"
#incl使de "En成ine/StaticMesh.h"
#incl使de "Mate本ials/Mate本ialInstanceDyna設置ic.h"
#incl使de "So使nd/So使ndBase.h"
#incl使de "Co設置ponents/SceneCapt使本eCo設置ponent2D.h"
#incl使de "Co設置ponents/StaticMeshCo設置ponent.h"
#incl使de "Co設置ponents/A使dioCo設置ponent.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "輸入ttpMod使le.h"
#incl使de "Inte本faces/I輸入ttpReq使est.h"
#incl使de "Inte本faces/I輸入ttpResponse.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"
#incl使de "Kis設置et/Kis設置etRende本in成Lib本a本y.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"

UMin成AIGene本atedContentSyste設置::UMin成AIGene本atedContentSyste設置()
    : Defa使ltModelP本o正ide本(EAIModelP本o正ide本::StableDiff使sion)
    , Defa使ltQ使ality(EGene本ationQ使ality::Standa本d)
    , O使tp使tDi本ecto本y(TEXT("/Ga設置e/AIGene本ated/"))
    , MaxConc使本本entGene本ations(3)
    , bEnableCache(t本使e)
    , bA使toSa正eRes使lts(t本使e)
    , CacheExpi本ationTi設置e(3600.0f)
    , bIsInitialized(false)
    , C使本本entGene本ationCo使nt(0)
{
}

正oid UMin成AIGene本atedContentSyste設置::InitializeAIGene本ationSyste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI Gene本ation Syste設置 al本eady initialized"));
        本et使本n;
    }

    // 設置默認API密鑰（從環境變量或配置文件讀取）
    APIKeys.Add(EAIModelP本o正ide本::StableDiff使sion, TEXT("yo使本-stable-diff使sion-key"));
    APIKeys.Add(EAIModelP本o正ide本::DALL下E, TEXT("yo使本-dalle-key"));
    APIKeys.Add(EAIModelP本o正ide本::Mid大o使本ney, TEXT("yo使本-設置id大o使本ney-key"));
    APIKeys.Add(EAIModelP本o正ide本::AIVA, TEXT("yo使本-ai正a-key"));
    APIKeys.Add(EAIModelP本o正ide本::Ele正enLabs, TEXT("yo使本-ele正enlabs-key"));
    APIKeys.Add(EAIModelP本o正ide本::OpenAI, TEXT("yo使本-openai-key"));

    // 初始化統計
    fo本 (int32 i = 0; i < (int32)EAIGene本ationType::Voice + 1; ++i)
    {
        EAIGene本ationType Type = (EAIGene本ationType)i;
        A正e本a成eGene本ationTi設置es.Add(Type, 0.0f);
        TotalGene本atedCo使nts.Add(Type, 0);
    }

    // 加載預設
    InitializeDefa使ltP本esets();

    // 確保輸目錄存在
    軍St本in成 軍使llO使tp使tPath = 軍Paths::P本o大ectDi本() / O使tp使tDi本ecto本y;
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    if (!Platfo本設置軍ile.Di本ecto本yExists(*軍使llO使tp使tPath))
    {
        Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*軍使llO使tp使tPath);
    }

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Gene本ation Syste設置 initialized"));
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateI設置a成e(const 軍St本in成& P本o設置pt, const 軍St本in成& 的e成ati正eP本o設置pt, int32 基本idth, int32 輸入ei成ht, EGene本ationQ使ality Q使ality)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::I設置a成e;
    Req使est.ModelP本o正ide本 = Defa使ltModelP本o正ide本;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.的e成ati正eP本o設置pt = 的e成ati正eP本o設置pt;
    Req使est.基本idth = 基本idth;
    Req使est.輸入ei成ht = 輸入ei成ht;
    Req使est.Q使ality = Q使ality;
    Req使est.bIsAsync = false;

    P本ocessGene本ationReq使est(Req使est);
    本et使本n Req使est.Req使estID;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateI設置a成eAsync(const 軍AIGene本ationReq使est& Req使est)
{
    軍AIGene本ationReq使est ModifiedReq使est = Req使est;
    ModifiedReq使est.Req使estID = Gene本ateReq使estID();
    ModifiedReq使est.bIsAsync = t本使e;

    Acti正eReq使ests.Add(ModifiedReq使est.Req使estID, ModifiedReq使est);
    Req使estP本o成本ess.Add(ModifiedReq使est.Req使estID, 0.0f);

    // 異步處理
    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, ModifiedReq使est]()
    {
        P本ocessGene本ationReq使est(ModifiedReq使est);
    });

    本et使本n ModifiedReq使est.Req使estID;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateVideo(const 軍St本in成& P本o設置pt, float D使本ation, int32 軍PS, EGene本ationQ使ality Q使ality)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::Video;
    Req使est.ModelP本o正ide本 = Defa使ltModelP本o正ide本;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.D使本ation = D使本ation;
    Req使est.Q使ality = Q使ality;
    Req使est.bIsAsync = t本使e;

    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("fps"), 軍St本in成::軍本o設置Int(軍PS));

    Acti正eReq使ests.Add(Req使est.Req使estID, Req使est);
    Req使estP本o成本ess.Add(Req使est.Req使estID, 0.0f);

    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        P本ocessGene本ationReq使est(Req使est);
    });

    本et使本n Req使est.Req使estID;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateM使sic(const 軍St本in成& Style, float D使本ation, EGene本ationQ使ality Q使ality)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::M使sic;
    Req使est.ModelP本o正ide本 = EAIModelP本o正ide本::AIVA;
    Req使est.P本o設置pt = Style;
    Req使est.D使本ation = D使本ation;
    Req使est.Q使ality = Q使ality;
    Req使est.bIsAsync = t本使e;

    Acti正eReq使ests.Add(Req使est.Req使estID, Req使est);
    Req使estP本o成本ess.Add(Req使est.Req使estID, 0.0f);

    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        P本ocessGene本ationReq使est(Req使est);
    });

    本et使本n Req使est.Req使estID;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateSo使ndEffect(const 軍St本in成& Desc本iption, float D使本ation, EGene本ationQ使ality Q使ality)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::So使ndEffect;
    Req使est.ModelP本o正ide本 = EAIModelP本o正ide本::StableDiff使sion; // 使用支持音效的模型
    Req使est.P本o設置pt = Desc本iption;
    Req使est.D使本ation = D使本ation;
    Req使est.Q使ality = Q使ality;
    Req使est.bIsAsync = t本使e;

    Acti正eReq使ests.Add(Req使est.Req使estID, Req使est);
    Req使estP本o成本ess.Add(Req使est.Req使estID, 0.0f);

    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        P本ocessGene本ationReq使est(Req使est);
    });

    本et使本n Req使est.Req使estID;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateVoice(const 軍St本in成& Text, const 軍St本in成& VoiceID, EGene本ationQ使ality Q使ality)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::Voice;
    Req使est.ModelP本o正ide本 = EAIModelP本o正ide本::Ele正enLabs;
    Req使est.P本o設置pt = Text;
    Req使est.Q使ality = Q使ality;
    Req使est.bIsAsync = t本使e;

    if (!VoiceID.IsE設置pty())
    {
        Req使est.AdditionalPa本a設置ete本s.Add(TEXT("正oice下id"), VoiceID);
    }

    Acti正eReq使ests.Add(Req使est.Req使estID, Req使est);
    Req使estP本o成本ess.Add(Req使est.Req使estID, 0.0f);

    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        P本ocessGene本ationReq使est(Req使est);
    });

    本et使本n Req使est.Req使estID;
}

UText使本e2D* UMin成AIGene本atedContentSyste設置::Gene本ateText使本e(const 軍St本in成& Desc本iption, int32 基本idth, int32 輸入ei成ht)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::Text使本e;
    Req使est.ModelP本o正ide本 = Defa使ltModelP本o正ide本;
    Req使est.P本o設置pt = Desc本iption;
    Req使est.基本idth = 基本idth;
    Req使est.輸入ei成ht = 輸入ei成ht;
    Req使est.bIsAsync = false;

    軍AIGene本ationRes使lt Res使lt;
    Res使lt.Req使estID = Req使est.Req使estID;
    Res使lt.bS使ccess = false;

    // 檢查緩存
    if (bEnableCache)
    {
        Res使lt = Load軍本o設置Cache(Req使est);
        if (Res使lt.bS使ccess)
        {
            本et使本n Res使lt.Gene本atedText使本e;
        }
    }

    // 生成紋理
    UText使本e2D* Gene本atedText使本e = Gene本ateI設置a成eInte本nal(Req使est);
    
    Res使lt.bS使ccess = (Gene本atedText使本e != n使llpt本);
    Res使lt.Gene本atedText使本e = Gene本atedText使本e;

    if (Res使lt.bS使ccess)
    {
        CacheRes使lt(Res使lt);
        UpdateStatistics(EAIGene本ationType::Text使本e, 0.0f);
    }

    本et使本n Gene本atedText使本e;
}

UStaticMesh* UMin成AIGene本atedContentSyste設置::Gene本ateMesh(const 軍St本in成& Desc本iption, float Co設置plexity)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::Mesh;
    Req使est.ModelP本o正ide本 = EAIModelP本o正ide本::StableDiff使sion;
    Req使est.P本o設置pt = Desc本iption;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("co設置plexity"), 軍St本in成::Sanitize軍loat(Co設置plexity));
    Req使est.bIsAsync = false;

    UStaticMesh* Gene本atedMesh = Gene本ateMeshInte本nal(Req使est);
    
    if (Gene本atedMesh)
    {
        UpdateStatistics(EAIGene本ationType::Mesh, 0.0f);
    }

    本et使本n Gene本atedMesh;
}

UMate本ialInte本face* UMin成AIGene本atedContentSyste設置::Gene本ateMate本ial(const 軍St本in成& Desc本iption, EGene本ationQ使ality Q使ality)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::Mate本ial;
    Req使est.ModelP本o正ide本 = Defa使ltModelP本o正ide本;
    Req使est.P本o設置pt = Desc本iption;
    Req使est.Q使ality = Q使ality;
    Req使est.bIsAsync = false;

    UMate本ialInte本face* Gene本atedMate本ial = Gene本ateMate本ialInte本nal(Req使est);
    
    if (Gene本atedMate本ial)
    {
        UpdateStatistics(EAIGene本ationType::Mate本ial, 0.0f);
    }

    本et使本n Gene本atedMate本ial;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateAni設置ation(const 軍St本in成& Desc本iption, float D使本ation)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::Ani設置ation;
    Req使est.ModelP本o正ide本 = Defa使ltModelP本o正ide本;
    Req使est.P本o設置pt = Desc本iption;
    Req使est.D使本ation = D使本ation;
    Req使est.bIsAsync = t本使e;

    Acti正eReq使ests.Add(Req使est.Req使estID, Req使est);
    Req使estP本o成本ess.Add(Req使est.Req使estID, 0.0f);

    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        P本ocessGene本ationReq使est(Req使est);
    });

    本et使本n Req使est.Req使estID;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateLe正el(const 軍St本in成& Desc本iption, int32 Size)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::Le正el;
    Req使est.ModelP本o正ide本 = EAIModelP本o正ide本::OpenAI;
    Req使est.P本o設置pt = Desc本iption;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("size"), 軍St本in成::軍本o設置Int(Size));
    Req使est.bIsAsync = t本使e;

    Acti正eReq使ests.Add(Req使est.Req使estID, Req使est);
    Req使estP本o成本ess.Add(Req使est.Req使estID, 0.0f);

    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        P本ocessGene本ationReq使est(Req使est);
    });

    本et使本n Req使est.Req使estID;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateUI(const 軍St本in成& Desc本iption, const 軍St本in成& UIType)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::UI;
    Req使est.ModelP本o正ide本 = Defa使ltModelP本o正ide本;
    Req使est.P本o設置pt = Desc本iption;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("使i下type"), UIType);
    Req使est.bIsAsync = t本使e;

    Acti正eReq使ests.Add(Req使est.Req使estID, Req使est);
    Req使estP本o成本ess.Add(Req使est.Req使estID, 0.0f);

    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        P本ocessGene本ationReq使est(Req使est);
    });

    本et使本n Req使est.Req使estID;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateText(const 軍St本in成& P本o設置pt, const 軍St本in成& Style)
{
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = Gene本ateReq使estID();
    Req使est.Gene本ationType = EAIGene本ationType::Text;
    Req使est.ModelP本o正ide本 = EAIModelP本o正ide本::OpenAI;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("style"), Style);
    Req使est.bIsAsync = t本使e;

    Acti正eReq使ests.Add(Req使est.Req使estID, Req使est);
    Req使estP本o成本ess.Add(Req使est.Req使estID, 0.0f);

    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        P本ocessGene本ationReq使est(Req使est);
    });

    本et使本n Req使est.Req使estID;
}

正oid UMin成AIGene本atedContentSyste設置::Gene本ateGa設置eScene(const 軍AIGa設置eSceneSet使p& SceneSet使p)
{
    軍St本in成 Scene的a設置e = SceneSet使p.Scene的a設置e.IsE設置pty() 基本 Gene本ateReq使estID() : SceneSet使p.Scene的a設置e;
    SceneSet使ps.Add(Scene的a設置e, SceneSet使p);

    // 生成場景所需的資產
    if (SceneSet使p.bA使toGene本ateAssets)
    {
        TA本本ay<軍AIGene本ationReq使est> AssetReq使ests;

        // 生成背景圖像
        軍AIGene本ationReq使est Back成本o使ndReq使est;
        Back成本o使ndReq使est.Req使estID = Gene本ateReq使estID();
        Back成本o使ndReq使est.Gene本ationType = EAIGene本ationType::I設置a成e;
        Back成本o使ndReq使est.P本o設置pt = 軍St本in成::P本intf(TEXT("%s %s %s %s"), 
                                                   *SceneSet使p.Location, 
                                                   *SceneSet使p.Ti設置eOfDay, 
                                                   *SceneSet使p.基本eathe本, 
                                                   *SceneSet使p.Mood);
        Back成本o使ndReq使est.基本idth = 1920;
        Back成本o使ndReq使est.輸入ei成ht = 1080;
        AssetReq使ests.Add(Back成本o使ndReq使est);

        // 生成環境音樂
        軍AIGene本ationReq使est M使sicReq使est;
        M使sicReq使est.Req使estID = Gene本ateReq使estID();
        M使sicReq使est.Gene本ationType = EAIGene本ationType::M使sic;
        M使sicReq使est.P本o設置pt = 軍St本in成::P本intf(TEXT("%s %s a設置bient 設置使sic"), *SceneSet使p.Mood, *SceneSet使p.Location);
        M使sicReq使est.D使本ation = 60.0f;
        AssetReq使ests.Add(M使sicReq使est);

        // 生成音效
        軍AIGene本ationReq使est So使ndReq使est;
        So使ndReq使est.Req使estID = Gene本ateReq使estID();
        So使ndReq使est.Gene本ationType = EAIGene本ationType::So使ndEffect;
        So使ndReq使est.P本o設置pt = 軍St本in成::P本intf(TEXT("%s %s en正i本on設置ental so使nds"), *SceneSet使p.Location, *SceneSet使p.基本eathe本);
        So使ndReq使est.D使本ation = 10.0f;
        AssetReq使ests.Add(So使ndReq使est);

        // 批量生成資產
        BatchGene本ateAssets(AssetReq使ests);
    }

    // 設置場景
    Gene本ateSceneLi成htin成(SceneSet使p);
    Gene本ateSceneEn正i本on設置ent(SceneSet使p);
    Gene本ateSceneEffects(SceneSet使p);
    Set使pSceneCa設置e本a(Scene的a設置e);

    的otifySceneGene本ated(Scene的a設置e);
}

正oid UMin成AIGene本atedContentSyste設置::Gene本ateGa設置eSceneAsync(const 軍AIGa設置eSceneSet使p& SceneSet使p)
{
    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, SceneSet使p]()
    {
        Gene本ateGa設置eScene(SceneSet使p);
    });
}

UText使本e2D* UMin成AIGene本atedContentSyste設置::Rende本SceneToText使本e(const 軍St本in成& Scene的a設置e, int32 基本idth, int32 輸入ei成ht)
{
    // 創建場景捕獲組件
    USceneCapt使本eCo設置ponent2D* SceneCapt使本e = 的ewOb大ect<USceneCapt使本eCo設置ponent2D>();
    SceneCapt使本e->Capt使本eSo使本ce = ESceneCapt使本eSo使本ce::SCS下軍inalColo本LDR;
    SceneCapt使本e->Text使本eTa本成et = 的ewOb大ect<UText使本eRende本Ta本成et2D>();
    SceneCapt使本e->Text使本eTa本成et->InitA使to軍o本設置at(基本idth, 輸入ei成ht);
    
    // 設置捕獲位置和角度
    SceneCapt使本e->Set基本o本ldLocation(軍Vecto本(0.0f, 0.0f, 500.0f));
    SceneCapt使本e->Set基本o本ldRotation(軍Rotato本(-90.0f, 0.0f, 0.0f));
    
    // 捕獲場景
    SceneCapt使本e->Capt使本eScene();
    
    // 獲取渲染結果
    UText使本e2D* Rende本edText使本e = SceneCapt使本e->Text使本eTa本成et;
    
    // 清理
    SceneCapt使本e->Dest本oyCo設置ponent();
    
    本et使本n Rende本edText使本e;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Rende本SceneToVideo(const 軍St本in成& Scene的a設置e, float D使本ation, int32 軍PS)
{
    軍St本in成 VideoID = Gene本ateReq使estID();
    
    // 設置視頻生成請求
    軍AIGene本ationReq使est Req使est;
    Req使est.Req使estID = VideoID;
    Req使est.Gene本ationType = EAIGene本ationType::Video;
    Req使est.P本o設置pt = 軍St本in成::P本intf(TEXT("Rende本 scene %s to 正ideo"), *Scene的a設置e);
    Req使est.D使本ation = D使本ation;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("fps"), 軍St本in成::軍本o設置Int(軍PS));
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("scene下na設置e"), Scene的a設置e);
    
    Acti正eReq使ests.Add(VideoID, Req使est);
    Req使estP本o成本ess.Add(VideoID, 0.0f);
    
    // 異步處理視頻渲染
    AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, Req使est]()
    {
        // 模擬視頻渲染過程
        float Total軍本a設置es = Req使est.D使本ation * 軍PS;
        fo本 (int32 軍本a設置e = 0; 軍本a設置e < Total軍本a設置es; ++軍本a設置e)
        {
            float P本o成本ess = (float)軍本a設置e / Total軍本a設置es;
            UpdateGene本ationP本o成本ess(Req使est.Req使estID, P本o成本ess);
            
            // 渲染單幀
            Rende本SceneToText使本e(Req使est.AdditionalPa本a設置ete本s.軍indRef(TEXT("scene下na設置e")), 1920, 1080);
            
            // 模擬渲染時間
            軍Platfo本設置P本ocess::Sleep(0.01f);
        }
        
        // 完成視頻生成
        軍AIGene本ationRes使lt Res使lt;
        Res使lt.Req使estID = Req使est.Req使estID;
        Res使lt.bS使ccess = t本使e;
        Res使lt.Gene本ated軍ilePath = 軍St本in成::P本intf(TEXT("%s下%s.設置p4"), *Req使est.Req使estID, *軍DateTi設置e::的ow().ToSt本in成());
        
        Co設置pleteGene本ation(Req使est.Req使estID, Res使lt);
    });
    
    本et使本n VideoID;
}

TA本本ay<軍St本in成> UMin成AIGene本atedContentSyste設置::BatchGene本ateAssets(const TA本本ay<軍AIGene本ationReq使est>& Req使ests)
{
    TA本本ay<軍St本in成> Req使estIDs;
    
    fo本 (const 軍AIGene本ationReq使est& Req使est : Req使ests)
    {
        軍St本in成 Req使estID = Gene本ateReq使estID();
        軍AIGene本ationReq使est ModifiedReq使est = Req使est;
        ModifiedReq使est.Req使estID = Req使estID;
        
        Acti正eReq使ests.Add(Req使estID, ModifiedReq使est);
        Req使estP本o成本ess.Add(Req使estID, 0.0f);
        Req使estIDs.Add(Req使estID);
        
        // 異步處理
        AsyncTask(E的a設置edTh本eads::AnyBack成本o使ndTh本ead的o本設置alTask, [this, ModifiedReq使est]()
        {
            P本ocessGene本ationReq使est(ModifiedReq使est);
        });
    }
    
    本et使本n Req使estIDs;
}

正oid UMin成AIGene本atedContentSyste設置::Sa正eGene本atedAsset(const 軍St本in成& Req使estID, const 軍St本in成& AssetPath)
{
    if (Gene本ationRes使lts.Contains(Req使estID))
    {
        const 軍AIGene本ationRes使lt& Res使lt = Gene本ationRes使lts[Req使estID];
        
        // 保存資產到指定路徑
        軍St本in成 軍使llPath = 軍Paths::P本o大ectDi本() / AssetPath;
        
        if (Res使lt.Gene本atedText使本e)
        {
            // 保存紋理
            UKis設置etRende本in成Lib本a本y::Expo本tRende本Ta本成et(Res使lt.Gene本atedText使本e, 軍使llPath);
        }
        else if (Res使lt.Gene本atedSo使nd)
        {
            // 保存音頻
            // 實現音頻保存邏輯
        }
        else if (Res使lt.Gene本atedMesh)
        {
            // 保存模型
            // 實現模型保存邏輯
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正ed 成ene本ated asset %s to %s"), *Req使estID, *AssetPath);
    }
}

UText使本e2D* UMin成AIGene本atedContentSyste設置::LoadGene本atedText使本e(const 軍St本in成& AssetPath)
{
    軍St本in成 軍使llPath = 軍Paths::P本o大ectDi本() / AssetPath;
    本et使本n LoadOb大ect<UText使本e2D>(n使llpt本, *軍使llPath);
}

USo使ndBase* UMin成AIGene本atedContentSyste設置::LoadGene本atedSo使nd(const 軍St本in成& AssetPath)
{
    軍St本in成 軍使llPath = 軍Paths::P本o大ectDi本() / AssetPath;
    本et使本n LoadOb大ect<USo使ndBase>(n使llpt本, *軍使llPath);
}

UStaticMesh* UMin成AIGene本atedContentSyste設置::LoadGene本atedMesh(const 軍St本in成& AssetPath)
{
    軍St本in成 軍使llPath = 軍Paths::P本o大ectDi本() / AssetPath;
    本et使本n LoadOb大ect<UStaticMesh>(n使llpt本, *軍使llPath);
}

bool UMin成AIGene本atedContentSyste設置::IsGene本ationInP本o成本ess(const 軍St本in成& Req使estID) const
{
    本et使本n Acti正eReq使ests.Contains(Req使estID);
}

float UMin成AIGene本atedContentSyste設置::GetGene本ationP本o成本ess(const 軍St本in成& Req使estID) const
{
    if (Req使estP本o成本ess.Contains(Req使estID))
    {
        本et使本n Req使estP本o成本ess[Req使estID];
    }
    本et使本n 0.0f;
}

軍AIGene本ationRes使lt UMin成AIGene本atedContentSyste設置::GetGene本ationRes使lt(const 軍St本in成& Req使estID) const
{
    if (Gene本ationRes使lts.Contains(Req使estID))
    {
        本et使本n Gene本ationRes使lts[Req使estID];
    }
    本et使本n 軍AIGene本ationRes使lt();
}

TA本本ay<軍St本in成> UMin成AIGene本atedContentSyste設置::GetActi正eReq使ests() const
{
    TA本本ay<軍St本in成> Acti正eReq使estIDs;
    
    fo本 (const a使to& Req使estPai本 : Acti正eReq使ests)
    {
        Acti正eReq使estIDs.Add(Req使estPai本.Key);
    }
    
    本et使本n Acti正eReq使estIDs;
}

正oid UMin成AIGene本atedContentSyste設置::SetModelP本o正ide本(EAIModelP本o正ide本 P本o正ide本, const 軍St本in成& APIKey)
{
    Defa使ltModelP本o正ide本 = P本o正ide本;
    if (!APIKey.IsE設置pty())
    {
        APIKeys.Add(P本o正ide本, APIKey);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set defa使lt 設置odel p本o正ide本 to %d"), (int32)P本o正ide本);
}

正oid UMin成AIGene本atedContentSyste設置::SetDefa使ltQ使ality(EGene本ationQ使ality Q使ality)
{
    Defa使ltQ使ality = Q使ality;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set defa使lt q使ality to %d"), (int32)Q使ality);
}

正oid UMin成AIGene本atedContentSyste設置::SetO使tp使tDi本ecto本y(const 軍St本in成& Di本ecto本y)
{
    O使tp使tDi本ecto本y = Di本ecto本y;
    
    // 確保目錄存在
    軍St本in成 軍使llO使tp使tPath = 軍Paths::P本o大ectDi本() / O使tp使tDi本ecto本y;
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    if (!Platfo本設置軍ile.Di本ecto本yExists(*軍使llO使tp使tPath))
    {
        Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*軍使llO使tp使tPath);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set o使tp使t di本ecto本y to %s"), *Di本ecto本y);
}

正oid UMin成AIGene本atedContentSyste設置::LoadGene本ationP本esets()
{
    // 加載預設模板
    // 這裡可以從配置文件或數據庫加載預設
}

軍AIGene本ationReq使est UMin成AIGene本atedContentSyste設置::GetP本esetReq使est(const 軍St本in成& P本eset的a設置e) const
{
    // 返回預設請求
    軍AIGene本ationReq使est P本esetReq使est;
    // 實現預設加載邏輯
    本et使本n P本esetReq使est;
}

正oid UMin成AIGene本atedContentSyste設置::Sa正eP本esetReq使est(const 軍St本in成& P本eset的a設置e, const 軍AIGene本ationReq使est& Req使est)
{
    // 保存預設請求
    // 實現預設保存邏輯
}

float UMin成AIGene本atedContentSyste設置::GetA正e本a成eGene本ationTi設置e(EAIGene本ationType Gene本ationType) const
{
    if (A正e本a成eGene本ationTi設置es.Contains(Gene本ationType))
    {
        本et使本n A正e本a成eGene本ationTi設置es[Gene本ationType];
    }
    本et使本n 0.0f;
}

int32 UMin成AIGene本atedContentSyste設置::GetTotalGene本atedCo使nt(EAIGene本ationType Gene本ationType) const
{
    if (TotalGene本atedCo使nts.Contains(Gene本ationType))
    {
        本et使本n TotalGene本atedCo使nts[Gene本ationType];
    }
    本et使本n 0;
}

float UMin成AIGene本atedContentSyste設置::GetSyste設置Load() const
{
    本et使本n (float)C使本本entGene本ationCo使nt / MaxConc使本本entGene本ations;
}

正oid UMin成AIGene本atedContentSyste設置::Clea本Cache()
{
    // 清理緩存
    Gene本ationRes使lts.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed 成ene本ation cache"));
}

正oid UMin成AIGene本atedContentSyste設置::CancelGene本ation(const 軍St本in成& Req使estID)
{
    if (Acti正eReq使ests.Contains(Req使estID))
    {
        Acti正eReq使ests.Re設置o正e(Req使estID);
        Req使estP本o成本ess.Re設置o正e(Req使estID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelled 成ene本ation %s"), *Req使estID);
    }
}

正oid UMin成AIGene本atedContentSyste設置::CancelAllGene本ations()
{
    Acti正eReq使ests.E設置pty();
    Req使estP本o成本ess.E設置pty();
    C使本本entGene本ationCo使nt = 0;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelled all 成ene本ations"));
}

正oid UMin成AIGene本atedContentSyste設置::InitializeDefa使ltP本esets()
{
    // 初始化默認預設
    // 可以在這裡設置一些常用的生成參數
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateReq使estID()
{
    本et使本n 軍St本in成::P本intf(TEXT("REQ下%s下%d"), *軍DateTi設置e::的ow().ToSt本in成(), 軍Math::RandRan成e(1000, 9999));
}

正oid UMin成AIGene本atedContentSyste設置::P本ocessGene本ationReq使est(const 軍AIGene本ationReq使est& Req使est)
{
    if (!ValidateReq使est(Req使est))
    {
        軍AIGene本ationRes使lt Res使lt;
        Res使lt.Req使estID = Req使est.Req使estID;
        Res使lt.bS使ccess = false;
        Res使lt.E本本o本Messa成e = TEXT("In正alid 本eq使est");
        Co設置pleteGene本ation(Req使est.Req使estID, Res使lt);
        本et使本n;
    }

    // 檢查緩存
    if (bEnableCache)
    {
        軍AIGene本ationRes使lt CachedRes使lt = Load軍本o設置Cache(Req使est);
        if (CachedRes使lt.bS使ccess)
        {
            Co設置pleteGene本ation(Req使est.Req使estID, CachedRes使lt);
            本et使本n;
        }
    }

    // 根據生成類型處理請求
    軍AIGene本ationRes使lt Res使lt;
    Res使lt.Req使estID = Req使est.Req使estID;
    Res使lt.bS使ccess = false;

    do使ble Sta本tTi設置e = 軍DateTi設置e::的ow().GetTicks();

    switch (Req使est.Gene本ationType)
    {
        case EAIGene本ationType::I設置a成e:
            Res使lt.Gene本atedText使本e = Gene本ateI設置a成eInte本nal(Req使est);
            Res使lt.bS使ccess = (Res使lt.Gene本atedText使本e != n使llpt本);
            b本eak;
            
        case EAIGene本ationType::M使sic:
            Res使lt.Gene本atedSo使nd = Gene本ateM使sicInte本nal(Req使est);
            Res使lt.bS使ccess = (Res使lt.Gene本atedSo使nd != n使llpt本);
            b本eak;
            
        case EAIGene本ationType::So使ndEffect:
            Res使lt.Gene本atedSo使nd = Gene本ateSo使ndEffectInte本nal(Req使est);
            Res使lt.bS使ccess = (Res使lt.Gene本atedSo使nd != n使llpt本);
            b本eak;
            
        case EAIGene本ationType::Mesh:
            Res使lt.Gene本atedMesh = Gene本ateMeshInte本nal(Req使est);
            Res使lt.bS使ccess = (Res使lt.Gene本atedMesh != n使llpt本);
            b本eak;
            
        case EAIGene本ationType::Mate本ial:
            Res使lt.Gene本atedMate本ial = Gene本ateMate本ialInte本nal(Req使est);
            Res使lt.bS使ccess = (Res使lt.Gene本atedMate本ial != n使llpt本);
            b本eak;
            
        defa使lt:
            // 處理其他類型
            b本eak;
    }

    do使ble EndTi設置e = 軍DateTi設置e::的ow().GetTicks();
    Res使lt.Gene本ationTi設置e = (EndTi設置e - Sta本tTi設置e) / ETi設置espan::TicksPe本Second;

    if (Res使lt.bS使ccess)
    {
        CacheRes使lt(Res使lt);
        UpdateStatistics(Req使est.Gene本ationType, Res使lt.Gene本ationTi設置e);
    }

    Co設置pleteGene本ation(Req使est.Req使estID, Res使lt);
}

正oid UMin成AIGene本atedContentSyste設置::UpdateGene本ationP本o成本ess(const 軍St本in成& Req使estID, float P本o成本ess)
{
    if (Req使estP本o成本ess.Contains(Req使estID))
    {
        Req使estP本o成本ess[Req使estID] = 軍Math::Cla設置p(P本o成本ess, 0.0f, 1.0f);
        的otifyGene本ationP本o成本ess(Req使estID, P本o成本ess);
    }
}

正oid UMin成AIGene本atedContentSyste設置::Co設置pleteGene本ation(const 軍St本in成& Req使estID, const 軍AIGene本ationRes使lt& Res使lt)
{
    Gene本ationRes使lts.Add(Req使estID, Res使lt);
    Acti正eReq使ests.Re設置o正e(Req使estID);
    Req使estP本o成本ess.Re設置o正e(Req使estID);
    
    C使本本entGene本ationCo使nt = 軍Math::Max(0, C使本本entGene本ationCo使nt - 1);
    
    的otifyGene本ationCo設置pleted(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pleted 成ene本ation %s: %s"), *Req使estID, Res使lt.bS使ccess 基本 TEXT("S使ccess") : TEXT("軍ailed"));
}

正oid UMin成AIGene本atedContentSyste設置::的otifyGene本ationCo設置pleted(const 軍AIGene本ationRes使lt& Res使lt)
{
    OnAIGene本ationCo設置pleted.B本oadcast(Res使lt);
}

正oid UMin成AIGene本atedContentSyste設置::的otifyGene本ationP本o成本ess(const 軍St本in成& Req使estID, float P本o成本ess)
{
    OnAIGene本ationP本o成本ess.B本oadcast(Req使estID, P本o成本ess);
}

正oid UMin成AIGene本atedContentSyste設置::的otifySceneGene本ated(const 軍St本in成& Scene的a設置e)
{
    OnSceneGene本ated.B本oadcast(Scene的a設置e);
}

UText使本e2D* UMin成AIGene本atedContentSyste設置::Gene本ateI設置a成eInte本nal(const 軍AIGene本ationReq使est& Req使est)
{
    // 實現圖像生成邏輯
    // 這裡可以調用Stable Diff使sion API或其他圖像生成服務
    
    // 創建示例紋理
    UText使本e2D* Gene本atedText使本e = UText使本e2D::C本eateT本ansient(Req使est.基本idth, Req使est.輸入ei成ht);
    Gene本atedText使本e->UpdateReso使本ce();
    
    // 填充示例數據（實際實現中應該調用AI服務）
    軍Text使本e2DMipMap* MipMap = &Gene本atedText使本e->GetPlatfo本設置Data()->Mips[0];
    正oid* Text使本eData = MipMap->B使lkData.Lock(LOCK下READ下基本RITE);
    軍Me設置o本y::Me設置ze本o(Text使本eData, Req使est.基本idth * Req使est.輸入ei成ht * 4);
    MipMap->B使lkData.Unlock();
    Gene本atedText使本e->UpdateReso使本ce();
    
    本et使本n Gene本atedText使本e;
}

USo使ndBase* UMin成AIGene本atedContentSyste設置::Gene本ateM使sicInte本nal(const 軍AIGene本ationReq使est& Req使est)
{
    // 實現音樂生成邏輯
    // 調用AIVA API或其他音樂生成服務
    
    // 創建示例音頻資源
    USo使ndBase* Gene本atedSo使nd = 的ewOb大ect<USo使ndBase>();
    
    本et使本n Gene本atedSo使nd;
}

USo使ndBase* UMin成AIGene本atedContentSyste設置::Gene本ateSo使ndEffectInte本nal(const 軍AIGene本ationReq使est& Req使est)
{
    // 實現音效生成邏輯
    
    USo使ndBase* Gene本atedSo使nd = 的ewOb大ect<USo使ndBase>();
    
    本et使本n Gene本atedSo使nd;
}

UStaticMesh* UMin成AIGene本atedContentSyste設置::Gene本ateMeshInte本nal(const 軍AIGene本ationReq使est& Req使est)
{
    // 實現模型生成邏輯
    // 可以調用3D生成API或使用程序化生成
    
    UStaticMesh* Gene本atedMesh = 的ewOb大ect<UStaticMesh>();
    
    本et使本n Gene本atedMesh;
}

UMate本ialInte本face* UMin成AIGene本atedContentSyste設置::Gene本ateMate本ialInte本nal(const 軍AIGene本ationReq使est& Req使est)
{
    // 實現材質生成邏輯
    
    UMate本ialInstanceDyna設置ic* Gene本atedMate本ial = UMate本ialInstanceDyna設置ic::C本eate(n使llpt本);
    
    本et使本n Gene本atedMate本ial;
}

正oid UMin成AIGene本atedContentSyste設置::Gene本ateSceneLi成htin成(const 軍AIGa設置eSceneSet使p& SceneSet使p)
{
    // 實現場景光照設置
    // 根據時間和天氣設置光照參數
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated li成htin成 fo本 scene %s"), *SceneSet使p.Scene的a設置e);
}

正oid UMin成AIGene本atedContentSyste設置::Gene本ateSceneEn正i本on設置ent(const 軍AIGa設置eSceneSet使p& SceneSet使p)
{
    // 實現場景環境設置
    // 根據地點和描述生成環境元素
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated en正i本on設置ent fo本 scene %s"), *SceneSet使p.Scene的a設置e);
}

正oid UMin成AIGene本atedContentSyste設置::Gene本ateSceneEffects(const 軍AIGa設置eSceneSet使p& SceneSet使p)
{
    // 實現場景特效設置
    // 根據天氣和情緒添加特效
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated effects fo本 scene %s"), *SceneSet使p.Scene的a設置e);
}

正oid UMin成AIGene本atedContentSyste設置::Set使pSceneCa設置e本a(const 軍St本in成& Scene的a設置e)
{
    // 實現攝像機設置
    // 設置場景的攝像機位置和參數
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set使p ca設置e本a fo本 scene %s"), *Scene的a設置e);
}

軍St本in成 UMin成AIGene本atedContentSyste設置::GetModelAPIEndpoint(EAIModelP本o正ide本 P本o正ide本) const
{
    switch (P本o正ide本)
    {
        case EAIModelP本o正ide本::StableDiff使sion:
            本et使本n TEXT("https://api.stability.ai/正1/成ene本ation/stable-diff使sion-xl-1024-正1-0/text-to-i設置a成e");
        case EAIModelP本o正ide本::DALL下E:
            本et使本n TEXT("https://api.openai.co設置/正1/i設置a成es/成ene本ations");
        case EAIModelP本o正ide本::Mid大o使本ney:
            本et使本n TEXT("https://api.設置id大o使本ney.co設置/正1/i設置a成ine");
        case EAIModelP本o正ide本::AIVA:
            本et使本n TEXT("https://api.ai正a.ai/正1/成ene本ate");
        case EAIModelP本o正ide本::Ele正enLabs:
            本et使本n TEXT("https://api.ele正enlabs.io/正1/text-to-speech");
        case EAIModelP本o正ide本::OpenAI:
            本et使本n TEXT("https://api.openai.co設置/正1/co設置pletions");
        defa使lt:
            本et使本n TEXT("");
    }
}

軍St本in成 UMin成AIGene本atedContentSyste設置::B使ildAPIReq使est(const 軍AIGene本ationReq使est& Req使est) const
{
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect = MakeSha本eable(new 軍JsonOb大ect);
    
    JsonOb大ect->SetSt本in成軍ield(TEXT("p本o設置pt"), Req使est.P本o設置pt);
    JsonOb大ect->SetSt本in成軍ield(TEXT("ne成ati正e下p本o設置pt"), Req使est.的e成ati正eP本o設置pt);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("width"), Req使est.基本idth);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("hei成ht"), Req使est.輸入ei成ht);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("d使本ation"), Req使est.D使本ation);
    
    // 添加額外參數
    fo本 (const a使to& Pa本a設置 : Req使est.AdditionalPa本a設置ete本s)
    {
        JsonOb大ect->SetSt本in成軍ield(Pa本a設置.Key, Pa本a設置.Val使e);
    }
    
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(JsonOb大ect.ToSha本edRef(), 基本本ite本);
    
    本et使本n O使tp使tSt本in成;
}

軍AIGene本ationRes使lt UMin成AIGene本atedContentSyste設置::Pa本seAPIResponse(const 軍St本in成& Response, const 軍St本in成& Req使estID) const
{
    軍AIGene本ationRes使lt Res使lt;
    Res使lt.Req使estID = Req使estID;
    
    // 解析API響應
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(Response);
    
    if (軍JsonSe本ialize本::Dese本ialize(Reade本, JsonOb大ect) && JsonOb大ect.IsValid())
    {
        // 解析成功響應
        Res使lt.bS使ccess = t本使e;
        
        // 提取生成的資產URL或其他信息
        if (JsonOb大ect->輸入as軍ield(TEXT("data")))
        {
            // 處理圖像生成響應
        }
    }
    else
    {
        Res使lt.bS使ccess = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to pa本se API 本esponse");
    }
    
    本et使本n Res使lt;
}

bool UMin成AIGene本atedContentSyste設置::ValidateReq使est(const 軍AIGene本ationReq使est& Req使est) const
{
    if (Req使est.P本o設置pt.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (!APIKeys.Contains(Req使est.ModelP本o正ide本)  APIKeys[Req使est.ModelP本o正ide本].IsE設置pty())
    {
        本et使本n false;
    }
    
    if (C使本本entGene本ationCo使nt >= MaxConc使本本entGene本ations)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成AIGene本atedContentSyste設置::CacheRes使lt(const 軍AIGene本ationRes使lt& Res使lt)
{
    if (bEnableCache)
    {
        Gene本ationRes使lts.Add(Res使lt.Req使estID, Res使lt);
    }
}

軍AIGene本ationRes使lt UMin成AIGene本atedContentSyste設置::Load軍本o設置Cache(const 軍AIGene本ationReq使est& Req使est) const
{
    // 檢查是否有緩存的結果
    // 這裡可以實現基於p本o設置pt的緩存查找
    
    本et使本n 軍AIGene本ationRes使lt();
}

正oid UMin成AIGene本atedContentSyste設置::UpdateStatistics(EAIGene本ationType Gene本ationType, float Gene本ationTi設置e)
{
    // 更新統計信息
    if (A正e本a成eGene本ationTi設置es.Contains(Gene本ationType))
    {
        float C使本本entA正e本a成e = A正e本a成eGene本ationTi設置es[Gene本ationType];
        int32 C使本本entCo使nt = TotalGene本atedCo使nts[Gene本ationType];
        
        // 計算新的平均值
        float 的ewA正e本a成e = (C使本本entA正e本a成e * C使本本entCo使nt + Gene本ationTi設置e) / (C使本本entCo使nt + 1);
        A正e本a成eGene本ationTi設置es[Gene本ationType] = 的ewA正e本a成e;
        TotalGene本atedCo使nts[Gene本ationType] = C使本本entCo使nt + 1;
    }
    else
    {
        A正e本a成eGene本ationTi設置es.Add(Gene本ationType, Gene本ationTi設置e);
        TotalGene本atedCo使nts.Add(Gene本ationType, 1);
    }
}
