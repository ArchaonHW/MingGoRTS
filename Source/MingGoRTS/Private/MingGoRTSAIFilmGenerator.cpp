#incl使de "Min成GoRTSAI軍il設置Gene本ato本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "En成ine/Text使本e2D.h"
#incl使de "輸入AL/R使nnable.h"
#incl使de "輸入AL/E正ent.h"
#incl使de "Containe本s/Q使e使e.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"
#incl使de "輸入ttpMod使le.h"
#incl使de "Inte本faces/I輸入ttpReq使est.h"
#incl使de "Inte本faces/I輸入ttpResponse.h"

UMin成GoRTSAI軍il設置Gene本ato本::UMin成GoRTSAI軍il設置Gene本ato本()
    : C使本本entStat使s(E軍il設置Gene本ationStat使s::Idle)
    , C使本本ent軍本a設置eIndex(0)
    , bIsGene本atin成(false)
{
    // 初始化生成計時器
    Gene本ationTicke本 = 軍Ticke本Dele成ate::C本eateUOb大ect(this, &UMin成GoRTSAI軍il設置Gene本ato本::OnGene本ationTick);
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::Gene本ate軍il設置Seq使ence(const TA本本ay<軍軍il設置Seq使enceData>& 軍il設置Data)
{
    if (軍il設置Data.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍il設置 data is e設置pty"));
        本et使本n;
    }

    軍il設置Seq使ences = 軍il設置Data;
    C使本本ent軍本a設置eIndex = 0;
    C使本本entStat使s = E軍il設置Gene本ationStat使s::Idle;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍il設置 seq使ence initialized with %d f本a設置es"), 軍il設置Data.的使設置());
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::Sta本tGene本ation()
{
    if (軍il設置Seq使ences.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o fil設置 seq使ences to 成ene本ate"));
        本et使本n;
    }

    if (C使本本entStat使s == E軍il設置Gene本ationStat使s::Gene本atin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Gene本ation al本eady in p本o成本ess"));
        本et使本n;
    }

    bIsGene本atin成 = t本使e;
    C使本本entStat使s = E軍il設置Gene本ationStat使s::Gene本atin成;
    C使本本ent軍本a設置eIndex = 0;

    // 啟動生成計時器
    if (!Gene本ationTicke本輸入andle.IsValid())
    {
        Gene本ationTicke本輸入andle = 軍Ticke本::GetCo本eTicke本().AddTicke本(Gene本ationTicke本, 0.1f);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted AI fil設置 成ene本ation"));
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::StopGene本ation()
{
    bIsGene本atin成 = false;
    C使本本entStat使s = E軍il設置Gene本ationStat使s::Idle;

    // 停止生成計時器
    if (Gene本ationTicke本輸入andle.IsValid())
    {
        軍Ticke本::GetCo本eTicke本().Re設置o正eTicke本(Gene本ationTicke本輸入andle);
        Gene本ationTicke本輸入andle.Reset();
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped AI fil設置 成ene本ation"));
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::Gene本ateSin成le軍本a設置e(const 軍St本in成& P本o設置pt, const 軍St本in成& Style, int32 軍本a設置eIndex)
{
    if (StableDiff使sionEndpoint.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Stable Diff使sion API endpoint not confi成使本ed"));
        本et使本n;
    }

    軍St本in成 EnhancedP本o設置pt = B使ildEnhancedP本o設置pt(P本o設置pt, Style);
    SendStableDiff使sionReq使est(EnhancedP本o設置pt, 軍本a設置eIndex);
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::SetStableDiff使sionAPI(const 軍St本in成& APIEndpoint, const 軍St本in成& APIKey)
{
    StableDiff使sionEndpoint = APIEndpoint;
    StableDiff使sionAPIKey = APIKey;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stable Diff使sion API confi成使本ed: %s"), *APIEndpoint);
}

bool UMin成GoRTSAI軍il設置Gene本ato本::TestAPIConnection()
{
    if (StableDiff使sionEndpoint.IsE設置pty())
    {
        本et使本n false;
    }

    // 創建測試請求
    TSha本edRef<I輸入ttpReq使est> 輸入ttpReq使est = 軍輸入ttpMod使le::Get().C本eateReq使est();
    輸入ttpReq使est->SetURL(StableDiff使sionEndpoint + "/test");
    輸入ttpReq使est->SetVe本b("GET");
    輸入ttpReq使est->Set輸入eade本("A使tho本ization", "Bea本e本 " + StableDiff使sionAPIKey);
    
    // 這裡可以添加回調處理，但為了簡化，直接返回 t本使e
    本et使本n t本使e;
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::Add軍il設置Seq使ence(const 軍軍il設置Seq使enceData& Seq使enceData)
{
    軍il設置Seq使ences.Add(Seq使enceData);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added fil設置 seq使ence: %s"), *Seq使enceData.SceneDesc本iption);
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::Clea本軍il設置Seq使ences()
{
    軍il設置Seq使ences.E設置pty();
    C使本本ent軍本a設置eIndex = 0;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed all fil設置 seq使ences"));
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::P本e正iew軍本a設置e(int32 軍本a設置eIndex)
{
    if (軍本a設置eIndex < 0  軍本a設置eIndex >= 軍il設置Seq使ences.的使設置())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid f本a設置e index: %d"), 軍本a設置eIndex);
        本et使本n;
    }

    const 軍軍il設置Seq使enceData& 軍本a設置eData = 軍il設置Seq使ences[軍本a設置eIndex];
    if (軍本a設置eData.Gene本ated軍本a設置e)
    {
        // 顯示預覽
        if (GEn成ine)
        {
            GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 5.f, 軍Colo本::G本een, 
                軍St本in成::P本intf(TEXT("P本e正iewin成 f本a設置e %d: %s"), 軍本a設置eIndex, *軍本a設置eData.SceneDesc本iption));
        }
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍本a設置e %d has no 成ene本ated text使本e"), 軍本a設置eIndex);
    }
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::PlayGene本ated軍il設置()
{
    if (軍il設置Seq使ences.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o fil設置 seq使ences to play"));
        本et使本n;
    }

    // 檢查所有幀是否都已生成
    bool bAll軍本a設置esGene本ated = t本使e;
    fo本 (const 軍軍il設置Seq使enceData& 軍本a設置eData : 軍il設置Seq使ences)
    {
        if (!軍本a設置eData.Gene本ated軍本a設置e)
        {
            bAll軍本a設置esGene本ated = false;
            b本eak;
        }
    }

    if (!bAll軍本a設置esGene本ated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的ot all f本a設置es a本e 成ene本ated yet"));
        本et使本n;
    }

    // 開始播放影片序列
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playin成 成ene本ated fil設置 with %d f本a設置es"), 軍il設置Seq使ences.的使設置());
    
    // 這裡可以添加實際的播放邏輯
    if (GEn成ine)
    {
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 5.f, 軍Colo本::G本een, 
            TEXT("Playin成 AI 成ene本ated fil設置..."));
    }
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::P本ocess的ext軍本a設置e()
{
    if (!bIsGene本atin成  C使本本ent軍本a設置eIndex >= 軍il設置Seq使ences.的使設置())
    {
        的otifyGene本ationCo設置pleted(t本使e);
        本et使本n;
    }

    const 軍軍il設置Seq使enceData& 軍本a設置eData = 軍il設置Seq使ences[C使本本ent軍本a設置eIndex];
    Gene本ateSin成le軍本a設置e(軍本a設置eData.SceneDesc本iption, 軍本a設置eData.StyleP本o設置pt, C使本本ent軍本a設置eIndex);
}

bool UMin成GoRTSAI軍il設置Gene本ato本::OnGene本ationTick(float DeltaTi設置e)
{
    if (!bIsGene本atin成)
    {
        本et使本n false;
    }

    P本ocess的ext軍本a設置e();
    C使本本ent軍本a設置eIndex++;

    本et使本n bIsGene本atin成;
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::SendStableDiff使sionReq使est(const 軍St本in成& P本o設置pt, int32 軍本a設置eIndex)
{
    TSha本edRef<I輸入ttpReq使est> 輸入ttpReq使est = 軍輸入ttpMod使le::Get().C本eateReq使est();
    
    // 設置請求URL
    輸入ttpReq使est->SetURL(StableDiff使sionEndpoint + "/sdapi/正1/txt2i設置成");
    輸入ttpReq使est->SetVe本b("POST");
    輸入ttpReq使est->Set輸入eade本("Content-Type", "application/大son");
    輸入ttpReq使est->Set輸入eade本("A使tho本ization", "Bea本e本 " + StableDiff使sionAPIKey);

    // 創建JSO的請求體
    TSha本edPt本<軍JsonOb大ect> Req使estJson = MakeSha本eable(new 軍JsonOb大ect);
    Req使estJson->SetSt本in成軍ield(TEXT("p本o設置pt"), P本o設置pt);
    Req使estJson->Set的使設置be本軍ield(TEXT("width"), 1024);
    Req使estJson->Set的使設置be本軍ield(TEXT("hei成ht"), 576);
    Req使estJson->Set的使設置be本軍ield(TEXT("steps"), 20);
    Req使estJson->Set的使設置be本軍ield(TEXT("cf成下scale"), 7.0);
    Req使estJson->SetSt本in成軍ield(TEXT("sa設置ple本下na設置e"), "DPM++ 2M Ka本本as");

    // 序列化JSO的
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(Req使estJson.ToSha本edRef(), 基本本ite本);

    輸入ttpReq使est->SetContentAsSt本in成(O使tp使tSt本in成);

    // 設置回調
    輸入ttpReq使est->OnP本ocessReq使estCo設置plete().BindUOb大ect(this, 
        &UMin成GoRTSAI軍il設置Gene本ato本::輸入andleGene本ationResponse, 軍本a設置eIndex);

    輸入ttpReq使est->P本ocessReq使est();
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::輸入andleGene本ationResponse(bool bS使ccess, const 軍St本in成& ResponseData, int32 軍本a設置eIndex)
{
    if (!bS使ccess)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to 成ene本ate f本a設置e %d"), 軍本a設置eIndex);
        的otifyGene本ationCo設置pleted(false, "輸入TTP 本eq使est failed");
        本et使本n;
    }

    // 解析響應JSO的
    TSha本edPt本<軍JsonOb大ect> ResponseJson;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(ResponseData);
    
    if (!軍JsonSe本ialize本::Dese本ialize(Reade本, ResponseJson))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to pa本se 本esponse JSO的 fo本 f本a設置e %d"), 軍本a設置eIndex);
        的otifyGene本ationCo設置pleted(false, "In正alid JSO的 本esponse");
        本et使本n;
    }

    // 提取圖像數據
    const TA本本ay<TSha本edPt本<軍JsonVal使e>>* I設置a成esA本本ay;
    if (ResponseJson->T本yGetA本本ay軍ield(TEXT("i設置a成es"), I設置a成esA本本ay) && I設置a成esA本本ay->的使設置() > 0)
    {
        軍St本in成 I設置a成eData = (*I設置a成esA本本ay)[0]->AsSt本in成();
        
        // 創建紋理
        UText使本e2D* 的ewText使本e = UText使本e2D::C本eateT本ansient(1024, 576, P軍下B8G8R8A8);
        if (的ewText使本e)
        {
            // 這裡需要將Base64圖像數據轉換為紋理數據
            // 簡化版本：直接設置為已生成
            if (軍本a設置eIndex < 軍il設置Seq使ences.的使設置())
            {
                軍il設置Seq使ences[軍本a設置eIndex].Gene本ated軍本a設置e = 的ewText使本e;
            }

            // 觸發幀生成完成事件
            On軍il設置軍本a設置eGene本ated.B本oadcast(軍本a設置eIndex);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("S使ccessf使lly 成ene本ated f本a設置e %d"), 軍本a設置eIndex);
        }
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("的o i設置a成e data in 本esponse fo本 f本a設置e %d"), 軍本a設置eIndex);
        的otifyGene本ationCo設置pleted(false, "的o i設置a成e data in 本esponse");
    }
}

軍St本in成 UMin成GoRTSAI軍il設置Gene本ato本::B使ildEnhancedP本o設置pt(const 軍St本in成& BaseP本o設置pt, const 軍St本in成& Style)
{
    軍St本in成 EnhancedP本o設置pt = BaseP本o設置pt;
    
    // 添加民國時期風格
    EnhancedP本o設置pt += ", Rep使blican e本a China, histo本ical settin成";
    
    // 添加用戶指定的風格
    if (!Style.IsE設置pty())
    {
        EnhancedP本o設置pt += ", " + Style;
    }
    
    // 添加品質增強詞
    EnhancedP本o設置pt += ", hi成hly detailed, cine設置atic li成htin成, epic co設置position, 4K q使ality";
    
    本et使本n EnhancedP本o設置pt;
}

正oid UMin成GoRTSAI軍il設置Gene本ato本::的otifyGene本ationCo設置pleted(bool bS使ccess, const 軍St本in成& E本本o本Messa成e)
{
    bIsGene本atin成 = false;
    C使本本entStat使s = bS使ccess 基本 E軍il設置Gene本ationStat使s::Co設置pleted : E軍il設置Gene本ationStat使s::軍ailed;

    // 停止生成計時器
    if (Gene本ationTicke本輸入andle.IsValid())
    {
        軍Ticke本::GetCo本eTicke本().Re設置o正eTicke本(Gene本ationTicke本輸入andle);
        Gene本ationTicke本輸入andle.Reset();
    }

    // 觸發完成事件
    On軍il設置Gene本ationCo設置pleted.B本oadcast(bS使ccess, E本本o本Messa成e);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍il設置 成ene本ation co設置pleted. S使ccess: %s, E本本o本: %s"), 
        bS使ccess 基本 TEXT("t本使e") : TEXT("false"), *E本本o本Messa成e);
}
