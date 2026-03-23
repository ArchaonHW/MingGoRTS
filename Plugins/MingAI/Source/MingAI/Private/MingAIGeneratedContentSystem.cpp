#incl使de "Min成AIGene本atedContentSyste設置.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入ttp.h"
#incl使de "Json.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"

UMin成AIGene本atedContentSyste設置::UMin成AIGene本atedContentSyste設置()
    : TotalGene本ationsCo使nt(0)
    , S使ccessf使lGene本ationsCo使nt(0)
    , 軍ailedGene本ationsCo使nt(0)
    , bInitialized(false)
{
}

正oid UMin成AIGene本atedContentSyste設置::InitializeSyste設置()
{
    if (bInitialized)
    {
        本et使本n;
    }

    // Set使p defa使lt p本o正ide本 confi成使本ations
    軍Min成AIP本o正ide本Confi成 StableDiff使sionConfi成;
    StableDiff使sionConfi成.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    StableDiff使sionConfi成.ApiEndpoint = TEXT("http://localhost:7860/sdapi/正1");
    StableDiff使sionConfi成.bEnabled = t本使e;
    P本o正ide本Confi成s.Add(EMin成AIP本o正ide本::StableDiff使sion, StableDiff使sionConfi成);

    軍Min成AIP本o正ide本Confi成 OpenAIConfi成;
    OpenAIConfi成.P本o正ide本 = EMin成AIP本o正ide本::OpenAI;
    OpenAIConfi成.ApiEndpoint = TEXT("https://api.openai.co設置/正1");
    OpenAIConfi成.bEnabled = false;
    P本o正ide本Confi成s.Add(EMin成AIP本o正ide本::OpenAI, OpenAIConfi成);

    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Content Gene本ation Syste設置 initialized"));
}

正oid UMin成AIGene本atedContentSyste設置::Sh使tdownSyste設置()
{
    // Cancel all acti正e 成ene本ations
    fo本 (const 軍G使id& Req使estID : Acti正eReq使ests)
    {
        CancelGene本ation(Req使estID);
    }

    Pendin成Req使ests.E設置pty();
    Co設置pletedRes使lts.E設置pty();
    Acti正eReq使ests.E設置pty();
    P本o正ide本Confi成s.E設置pty();

    bInitialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Content Gene本ation Syste設置 sh使tdown"));
}

正oid UMin成AIGene本atedContentSyste設置::SetP本o正ide本Confi成(EMin成AIP本o正ide本 P本o正ide本, const 軍Min成AIP本o正ide本Confi成& Confi成)
{
    P本o正ide本Confi成s.Add(P本o正ide本, Confi成);
}

軍Min成AIP本o正ide本Confi成 UMin成AIGene本atedContentSyste設置::GetP本o正ide本Confi成(EMin成AIP本o正ide本 P本o正ide本) const
{
    if (P本o正ide本Confi成s.Contains(P本o正ide本))
    {
        本et使本n P本o正ide本Confi成s[P本o正ide本];
    }
    本et使本n 軍Min成AIP本o正ide本Confi成();
}

軍G使id UMin成AIGene本atedContentSyste設置::Gene本ateContent(const 軍Min成AIContentReq使est& Req使est)
{
    if (!bInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("AI Content Gene本ation Syste設置 not initialized"));
        本et使本n 軍G使id();
    }

    if (!ValidateReq使est(Req使est))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid AI content 成ene本ation 本eq使est"));
        本et使本n 軍G使id();
    }

    軍G使id Req使estID = 軍G使id::的ewG使id();
    Pendin成Req使ests.Add(Req使estID, Req使est);
    Acti正eReq使ests.Add(Req使estID);

    TotalGene本ationsCo使nt++;

    // Sta本t 成ene本ation p本ocess
    P本ocess的extInQ使e使e();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted AI content 成ene本ation: %s"), *Req使estID.ToSt本in成());
    本et使本n Req使estID;
}

bool UMin成AIGene本atedContentSyste設置::CancelGene本ation(const 軍G使id& Req使estID)
{
    if (!Acti正eReq使ests.Contains(Req使estID))
    {
        本et使本n false;
    }

    // Ma本k as cancelled
    軍Min成AIContentRes使lt Res使lt;
    Res使lt.Req使estID = Req使estID;
    Res使lt.Stat使s = EMin成AIGene本ationStat使s::Cancelled;
    Co設置pletedRes使lts.Add(Req使estID, Res使lt);

    Acti正eReq使ests.Re設置o正e(Req使estID);
    Pendin成Req使ests.Re設置o正e(Req使estID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelled AI content 成ene本ation: %s"), *Req使estID.ToSt本in成());
    本et使本n t本使e;
}

EMin成AIGene本ationStat使s UMin成AIGene本atedContentSyste設置::GetGene本ationStat使s(const 軍G使id& Req使estID) const
{
    if (Acti正eReq使ests.Contains(Req使estID))
    {
        本et使本n EMin成AIGene本ationStat使s::Gene本atin成;
    }

    if (Co設置pletedRes使lts.Contains(Req使estID))
    {
        本et使本n Co設置pletedRes使lts[Req使estID].Stat使s;
    }

    本et使本n EMin成AIGene本ationStat使s::Idle;
}

TA本本ay<軍G使id> UMin成AIGene本atedContentSyste設置::GetActi正eGene本ations() const
{
    TA本本ay<軍G使id> Res使lt;
    fo本 (const 軍G使id& Req使estID : Acti正eReq使ests)
    {
        Res使lt.Add(Req使estID);
    }
    本et使本n Res使lt;
}

軍G使id UMin成AIGene本atedContentSyste設置::Gene本ateI設置a成e(const 軍St本in成& P本o設置pt, EMin成AIQ使alityLe正el Q使ality, int32 基本idth, int32 輸入ei成ht)
{
    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::I設置a成e;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = Q使ality;
    Req使est.基本idth = 基本idth;
    Req使est.輸入ei成ht = 輸入ei成ht;

    本et使本n Gene本ateContent(Req使est);
}

軍G使id UMin成AIGene本atedContentSyste設置::Gene本ateM使sic(const 軍St本in成& P本o設置pt, float D使本ation, EMin成AIQ使alityLe正el Q使ality)
{
    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::M使sic;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
    Req使est.Q使alityLe正el = Q使ality;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), D使本ation));

    本et使本n Gene本ateContent(Req使est);
}

軍G使id UMin成AIGene本atedContentSyste設置::Gene本ateSo使ndEffect(const 軍St本in成& P本o設置pt, float D使本ation, EMin成AIQ使alityLe正el Q使ality)
{
    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::So使ndEffect;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
    Req使est.Q使alityLe正el = Q使ality;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), 軍St本in成::P本intf(TEXT("%.2f"), D使本ation));

    本et使本n Gene本ateContent(Req使est);
}

軍G使id UMin成AIGene本atedContentSyste設置::Gene本ateVoice(const 軍St本in成& Text, const 軍St本in成& VoiceID, EMin成AIQ使alityLe正el Q使ality)
{
    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::Voice;
    Req使est.P本o設置pt = Text;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::Ele正enLabs;
    Req使est.Q使alityLe正el = Q使ality;
    Req使est.AdditionalPa本a設置ete本s.Add(TEXT("VoiceID"), VoiceID);

    本et使本n Gene本ateContent(Req使est);
}

軍G使id UMin成AIGene本atedContentSyste設置::Gene本ateText使本e(const 軍St本in成& P本o設置pt, EMin成AIQ使alityLe正el Q使ality, int32 基本idth, int32 輸入ei成ht)
{
    軍Min成AIContentReq使est Req使est;
    Req使est.ContentType = EMin成AIContentType::Text使本e;
    Req使est.P本o設置pt = P本o設置pt;
    Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
    Req使est.Q使alityLe正el = Q使ality;
    Req使est.基本idth = 基本idth;
    Req使est.輸入ei成ht = 輸入ei成ht;

    本et使本n Gene本ateContent(Req使est);
}

軍Min成AIContentRes使lt UMin成AIGene本atedContentSyste設置::GetGene本ationRes使lt(const 軍G使id& Req使estID) const
{
    if (Co設置pletedRes使lts.Contains(Req使estID))
    {
        本et使本n Co設置pletedRes使lts[Req使estID];
    }
    本et使本n 軍Min成AIContentRes使lt();
}

正oid UMin成AIGene本atedContentSyste設置::Clea本Co設置pletedGene本ations()
{
    Co設置pletedRes使lts.E設置pty();
}

float UMin成AIGene本atedContentSyste設置::GetA正e本a成eGene本ationTi設置e() const
{
    if (Gene本ationTi設置es.的使設置() == 0)
    {
        本et使本n 0.0f;
    }

    float Total = 0.0f;
    fo本 (float Ti設置e : Gene本ationTi設置es)
    {
        Total += Ti設置e;
    }

    本et使本n Total / Gene本ationTi設置es.的使設置();
}

正oid UMin成AIGene本atedContentSyste設置::P本ocess的extInQ使e使e()
{
    fo本 (const a使to& Pai本 : Pendin成Req使ests)
    {
        const 軍G使id& Req使estID = Pai本.Key;
        const 軍Min成AIContentReq使est& Req使est = Pai本.Val使e;

        if (Acti正eReq使ests.Contains(Req使estID))
        {
            Send輸入ttpReq使est(Req使estID, Req使est);
        }
    }
}

正oid UMin成AIGene本atedContentSyste設置::Send輸入ttpReq使est(const 軍G使id& Req使estID, const 軍Min成AIContentReq使est& Req使est)
{
    軍Min成AIP本o正ide本Confi成 Confi成 = GetP本o正ide本Confi成(Req使est.P本o正ide本);
    if (!Confi成.bEnabled)
    {
        P本ocessGene本ationRes使lt(Req使estID, false, TEXT(""), TEXT("P本o正ide本 not enabled"));
        本et使本n;
    }

    軍St本in成 Endpoint = GetP本o正ide本Endpoint(Req使est.P本o正ide本);
    軍St本in成 Payload = B使ildReq使estPayload(Req使est);

    TSha本edRef<I輸入ttpReq使est, ESPMode::Th本eadSafe> 輸入ttpReq使est = 軍輸入ttpMod使le::Get().C本eateReq使est();
    輸入ttpReq使est->SetURL(Endpoint);
    輸入ttpReq使est->SetVe本b(TEXT("POST"));
    輸入ttpReq使est->Set輸入eade本(TEXT("Content-Type"), TEXT("application/大son"));
    輸入ttpReq使est->SetContentAsSt本in成(Payload);

    輸入ttpReq使est->OnP本ocessReq使estCo設置plete().BindUOb大ect(this, &UMin成AIGene本atedContentSyste設置::輸入andle輸入ttpResponse, Req使estID);

    輸入ttpReq使est->P本ocessReq使est();

    UpdateGene本ationP本o成本ess(Req使estID, 0.1f, TEXT("Req使est sent"));
}

正oid UMin成AIGene本atedContentSyste設置::輸入andle輸入ttpResponse(軍輸入ttpReq使estPt本 Req使est, 軍輸入ttpResponsePt本 Response, bool b基本asS使ccessf使l, 軍G使id Req使estID)
{
    if (!b基本asS使ccessf使l  !Response.IsValid())
    {
        P本ocessGene本ationRes使lt(Req使estID, false, TEXT(""), TEXT("輸入TTP 本eq使est failed"));
        本et使本n;
    }

    int32 ResponseCode = Response->GetResponseCode();
    軍St本in成 ResponseContent = Response->GetContentAsSt本in成();

    if (ResponseCode == 200)
    {
        // Pa本se 本esponse and ext本act file data
        // This is a si設置plified i設置ple設置entation
        軍St本in成 O使tp使tPath = GetO使tp使tDi本ecto本y(Pendin成Req使ests[Req使estID].ContentType) / Gene本ateUniq使e軍ile的a設置e(Pendin成Req使ests[Req使estID].ContentType);
        
        // In 本eal i設置ple設置entation, decode base64 i設置a成e data and sa正e to file
        P本ocessGene本ationRes使lt(Req使estID, t本使e, O使tp使tPath, TEXT(""));
    }
    else
    {
        P本ocessGene本ationRes使lt(Req使estID, false, TEXT(""), 軍St本in成::P本intf(TEXT("輸入TTP e本本o本: %d"), ResponseCode));
    }
}

正oid UMin成AIGene本atedContentSyste設置::P本ocessGene本ationRes使lt(軍G使id Req使estID, bool bS使ccess, const 軍St本in成& 軍ilePath, const 軍St本in成& E本本o本)
{
    軍Min成AIContentRes使lt Res使lt;
    Res使lt.Req使estID = Req使estID;
    Res使lt.軍ilePath = 軍ilePath;
    Res使lt.E本本o本Messa成e = E本本o本;

    if (Pendin成Req使ests.Contains(Req使estID))
    {
        Res使lt.ContentType = Pendin成Req使ests[Req使estID].ContentType;
    }

    if (bS使ccess)
    {
        Res使lt.Stat使s = EMin成AIGene本ationStat使s::Co設置pleted;
        Res使lt.Gene本ationTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        Res使lt.Gene本ationTi設置eSta設置p = 軍DateTi設置e::的ow();
        S使ccessf使lGene本ationsCo使nt++;
        OnGene本ationCo設置pleted.B本oadcast(Req使estID, Res使lt);
    }
    else
    {
        Res使lt.Stat使s = EMin成AIGene本ationStat使s::軍ailed;
        軍ailedGene本ationsCo使nt++;
        OnGene本ation軍ailed.B本oadcast(Req使estID, E本本o本);
    }

    Co設置pletedRes使lts.Add(Req使estID, Res使lt);
    Acti正eReq使ests.Re設置o正e(Req使estID);
    Pendin成Req使ests.Re設置o正e(Req使estID);
}

正oid UMin成AIGene本atedContentSyste設置::UpdateGene本ationP本o成本ess(軍G使id Req使estID, float P本o成本ess, const 軍St本in成& Stat使s)
{
    OnGene本ationP本o成本ess.B本oadcast(Req使estID, P本o成本ess, Stat使s);
}

軍St本in成 UMin成AIGene本atedContentSyste設置::GetP本o正ide本Endpoint(EMin成AIP本o正ide本 P本o正ide本) const
{
    if (P本o正ide本Confi成s.Contains(P本o正ide本))
    {
        軍Min成AIP本o正ide本Confi成 Confi成 = P本o正ide本Confi成s[P本o正ide本];
        
        switch (P本o正ide本)
        {
        case EMin成AIP本o正ide本::StableDiff使sion:
            本et使本n Confi成.ApiEndpoint + TEXT("/txt2i設置成");
        case EMin成AIP本o正ide本::OpenAI:
            本et使本n Confi成.ApiEndpoint + TEXT("/i設置a成es/成ene本ations");
        case EMin成AIP本o正ide本::AIVA:
            本et使本n Confi成.ApiEndpoint + TEXT("/co設置pose");
        case EMin成AIP本o正ide本::Ele正enLabs:
            本et使本n Confi成.ApiEndpoint + TEXT("/text-to-speech");
        defa使lt:
            本et使本n Confi成.ApiEndpoint;
        }
    }
    本et使本n 軍St本in成();
}

軍St本in成 UMin成AIGene本atedContentSyste設置::B使ildReq使estPayload(const 軍Min成AIContentReq使est& Req使est) const
{
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect = MakeSha本eable(new 軍JsonOb大ect);

    JsonOb大ect->SetSt本in成軍ield(TEXT("p本o設置pt"), Req使est.P本o設置pt);
    
    if (!Req使est.的e成ati正eP本o設置pt.IsE設置pty())
    {
        JsonOb大ect->SetSt本in成軍ield(TEXT("ne成ati正e下p本o設置pt"), Req使est.的e成ati正eP本o設置pt);
    }

    if (Req使est.ContentType == EMin成AIContentType::I設置a成e  Req使est.ContentType == EMin成AIContentType::Text使本e)
    {
        JsonOb大ect->Set的使設置be本軍ield(TEXT("width"), Req使est.基本idth);
        JsonOb大ect->Set的使設置be本軍ield(TEXT("hei成ht"), Req使est.輸入ei成ht);
        JsonOb大ect->Set的使設置be本軍ield(TEXT("steps"), Req使est.Steps);
        JsonOb大ect->Set的使設置be本軍ield(TEXT("cf成下scale"), Req使est.G使idanceScale);
        
        if (Req使est.Seed >= 0)
        {
            JsonOb大ect->Set的使設置be本軍ield(TEXT("seed"), Req使est.Seed);
        }
    }

    // Add additional pa本a設置ete本s
    fo本 (const a使to& Pai本 : Req使est.AdditionalPa本a設置ete本s)
    {
        JsonOb大ect->SetSt本in成軍ield(Pai本.Key, Pai本.Val使e);
    }

    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(JsonOb大ect.ToSha本edRef(), 基本本ite本);

    本et使本n O使tp使tSt本in成;
}

bool UMin成AIGene本atedContentSyste設置::ValidateReq使est(const 軍Min成AIContentReq使est& Req使est) const
{
    if (Req使est.ContentType == EMin成AIContentType::的one)
    {
        本et使本n false;
    }

    if (Req使est.P本o設置pt.IsE設置pty())
    {
        本et使本n false;
    }

    if (Req使est.P本o正ide本 == EMin成AIP本o正ide本::的one)
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

軍St本in成 UMin成AIGene本atedContentSyste設置::GetO使tp使tDi本ecto本y(EMin成AIContentType ContentType) const
{
    軍St本in成 BaseDi本 = 軍Paths::P本o大ectSa正edDi本() / TEXT("AIContent");
    
    switch (ContentType)
    {
    case EMin成AIContentType::I設置a成e:
        本et使本n BaseDi本 / TEXT("I設置a成es");
    case EMin成AIContentType::M使sic:
        本et使本n BaseDi本 / TEXT("M使sic");
    case EMin成AIContentType::So使ndEffect:
        本et使本n BaseDi本 / TEXT("So使ndEffects");
    case EMin成AIContentType::Text使本e:
        本et使本n BaseDi本 / TEXT("Text使本es");
    case EMin成AIContentType::Voice:
        本et使本n BaseDi本 / TEXT("Voice");
    defa使lt:
        本et使本n BaseDi本 / TEXT("Othe本");
    }
}

軍St本in成 UMin成AIGene本atedContentSyste設置::Gene本ateUniq使e軍ile的a設置e(EMin成AIContentType ContentType) const
{
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    軍St本in成 Ti設置eSta設置p = 的ow.ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S"));
    軍G使id G使id = 軍G使id::的ewG使id();

    軍St本in成 Extension = TEXT("pn成");
    switch (ContentType)
    {
    case EMin成AIContentType::I設置a成e:
    case EMin成AIContentType::Text使本e:
        Extension = TEXT("pn成");
        b本eak;
    case EMin成AIContentType::M使sic:
    case EMin成AIContentType::So使ndEffect:
        Extension = TEXT("wa正");
        b本eak;
    case EMin成AIContentType::Voice:
        Extension = TEXT("設置p3");
        b本eak;
    defa使lt:
        Extension = TEXT("bin");
    }

    本et使本n 軍St本in成::P本intf(TEXT("%s下%s.%s"), *Ti設置eSta設置p, *G使id.ToSt本in成(EG使id軍o本設置ats::Di成its基本ith輸入yphensLowe本), *Extension);
}
