#incl使de "Min成AIAssetGene本ato本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "En成ine/En成ine.h"

// 靜態實例初始化
UMin成AIAssetGene本ato本* UMin成AIAssetGene本ato本::Instance = n使llpt本;

UMin成AIAssetGene本ato本::UMin成AIAssetGene本ato本()
    : bIsInitialized(false)
{
    // 設置預設API端點
    StableDiff使sionEndpoint = TEXT("http://localhost:7860");
    AIVAEndpoint = TEXT("https://api.ai正a.ai");
    
    // 預分配容量
    Gene本atedAssets.Rese本正e(100);
}

正oid UMin成AIAssetGene本ato本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    // 創建資產目錄
    fo本 (int32 i = 0; i < static下cast<int32>(EAIAssetType::VoiceLine) + 1; ++i)
    {
        C本eateAssetDi本ecto本y(static下cast<EAIAssetType>(i));
    }
    
    bIsInitialized = t本使e;
    Instance = this;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AIAssetGene本ato本 initialized"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stable Diff使sion: %s"), *StableDiff使sionEndpoint);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AIVA: %s"), *AIVAEndpoint);
}

正oid UMin成AIAssetGene本ato本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    Gene本atedAssets.E設置pty();
    bIsInitialized = false;
    Instance = n使llpt本;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AIAssetGene本ato本 sh使tdown"));
}

軍AIAssetGene本ationRes使lt UMin成AIAssetGene本ato本::Gene本ateAsset(const 軍AIAssetGene本ationPa本a設置s& Pa本a設置s)
{
    if (!bIsInitialized)
    {
        本et使本n 輸入andleGene本ationE本本o本(TEXT("AIAssetGene本ato本 not initialized"));
    }
    
    軍AIAssetGene本ationRes使lt Res使lt;
    Res使lt.AssetType = Pa本a設置s.AssetType;
    Res使lt.AssetID = Gene本ateAssetID();
    
    // 記錄開始時間
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 AI asset: %s (Type: %d, E本a: %d)"),
        *Res使lt.AssetID, static下cast<int32>(Pa本a設置s.AssetType), static下cast<int32>(Pa本a設置s.E本aStyle));
    
    // 根據資產類型調用相應的生成方法
    switch (Pa本a設置s.AssetType)
    {
    case EAIAssetType::Cha本acte本Po本t本ait:
    case EAIAssetType::B使ildin成:
    case EAIAssetType::Landscape:
    case EAIAssetType::基本eapon:
    case EAIAssetType::Unifo本設置:
    case EAIAssetType::P本op:
        // 圖像資產 - 使用Stable Diff使sion
        {
            軍St本in成 P本o設置pt = Gene本ateStableDiff使sionP本o設置pt(Pa本a設置s);
            Res使lt = CallStableDiff使sionAPI(P本o設置pt, Pa本a設置s.的e成ati正eP本o設置pt, Pa本a設置s.I設置a成e基本idth, Pa本a設置s.I設置a成e輸入ei成ht);
        }
        b本eak;
        
    case EAIAssetType::Back成本o使ndM使sic:
    case EAIAssetType::So使ndEffect:
        // 音樂資產 - 使用AIVA
        {
            Res使lt = CallAIVAAPI(Pa本a設置s.DetailedP本o設置pt, Pa本a設置s.M使sicD使本ation, Pa本a設置s.M使sicStyle);
        }
        b本eak;
        
    case EAIAssetType::VoiceLine:
        // 語音資產 - 可以使用TTS或其他服務
        {
            Res使lt.E本本o本Messa成e = TEXT("Voice line 成ene本ation not i設置ple設置ented yet");
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Voice line 成ene本ation not i設置ple設置ented"));
        }
        b本eak;
    }
    
    // 計算生成時間
    Res使lt.Gene本ationTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
    
    // 保存資產
    if (Res使lt.bS使ccess)
    {
        Sa正eGene本atedAsset(Res使lt);
        
        // 生成縮略圖
        if (Pa本a設置s.AssetType <= EAIAssetType::P本op) // 圖像資產
        {
            Res使lt.Th使設置bnailPath = Gene本ateTh使設置bnail(Res使lt.AssetPath, Pa本a設置s.AssetType);
        }
        
        // 添加到資產列表
        Gene本atedAssets.Add(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("AI asset 成ene本ated s使ccessf使lly: %s (%.2f seconds)"),
            *Res使lt.AssetID, Res使lt.Gene本ationTi設置e);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("AI asset 成ene本ation failed: %s - %s"),
            *Res使lt.AssetID, *Res使lt.E本本o本Messa成e);
    }
    
    本et使本n Res使lt;
}

正oid UMin成AIAssetGene本ato本::Gene本ateAssetAsync(const 軍AIAssetGene本ationPa本a設置s& Pa本a設置s)
{
    // 異步生成 - 可以使用AsyncTask或Th本ead
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Async asset 成ene本ation 本eq使ested"));
    
    // 簡化處理：直接調用同步方法
    Gene本ateAsset(Pa本a設置s);
}

軍AIAssetGene本ationRes使lt UMin成AIAssetGene本ato本::Gene本ateCha本acte本Po本t本ait(
    const 軍St本in成& Cha本acte本Desc本iption, 
    ERep使blicE本aStyle E本aStyle,
    int32 I設置a成e基本idth,
    int32 I設置a成e輸入ei成ht)
{
    軍AIAssetGene本ationPa本a設置s Pa本a設置s;
    Pa本a設置s.AssetType = EAIAssetType::Cha本acte本Po本t本ait;
    Pa本a設置s.E本aStyle = E本aStyle;
    Pa本a設置s.BaseDesc本iption = Cha本acte本Desc本iption;
    Pa本a設置s.I設置a成e基本idth = I設置a成e基本idth;
    Pa本a設置s.I設置a成e輸入ei成ht = I設置a成e輸入ei成ht;
    Pa本a設置s.DetailedP本o設置pt = GetRep使blicE本aP本o設置pt(E本aStyle, EAIAssetType::Cha本acte本Po本t本ait) + TEXT(" ") + Cha本acte本Desc本iption;
    
    本et使本n Gene本ateAsset(Pa本a設置s);
}

軍AIAssetGene本ationRes使lt UMin成AIAssetGene本ato本::Gene本ateB使ildin成(
    const 軍St本in成& B使ildin成Desc本iption,
    ERep使blicE本aStyle E本aStyle,
    int32 I設置a成e基本idth,
    int32 I設置a成e輸入ei成ht)
{
    軍AIAssetGene本ationPa本a設置s Pa本a設置s;
    Pa本a設置s.AssetType = EAIAssetType::B使ildin成;
    Pa本a設置s.E本aStyle = E本aStyle;
    Pa本a設置s.BaseDesc本iption = B使ildin成Desc本iption;
    Pa本a設置s.I設置a成e基本idth = I設置a成e基本idth;
    Pa本a設置s.I設置a成e輸入ei成ht = I設置a成e輸入ei成ht;
    Pa本a設置s.DetailedP本o設置pt = GetRep使blicE本aP本o設置pt(E本aStyle, EAIAssetType::B使ildin成) + TEXT(" ") + B使ildin成Desc本iption;
    
    本et使本n Gene本ateAsset(Pa本a設置s);
}

軍AIAssetGene本ationRes使lt UMin成AIAssetGene本ato本::Gene本ateBack成本o使ndM使sic(
    const 軍St本in成& M使sicDesc本iption,
    ERep使blicE本aStyle E本aStyle,
    float D使本ation,
    const 軍St本in成& M使sicStyle)
{
    軍AIAssetGene本ationPa本a設置s Pa本a設置s;
    Pa本a設置s.AssetType = EAIAssetType::Back成本o使ndM使sic;
    Pa本a設置s.E本aStyle = E本aStyle;
    Pa本a設置s.BaseDesc本iption = M使sicDesc本iption;
    Pa本a設置s.M使sicD使本ation = D使本ation;
    Pa本a設置s.M使sicStyle = M使sicStyle;
    Pa本a設置s.DetailedP本o設置pt = GetRep使blicE本aP本o設置pt(E本aStyle, EAIAssetType::Back成本o使ndM使sic) + TEXT(" ") + M使sicDesc本iption;
    
    本et使本n Gene本ateAsset(Pa本a設置s);
}

軍AIAssetGene本ationRes使lt UMin成AIAssetGene本ato本::GetGene本atedAsset(const 軍St本in成& AssetID)
{
    fo本 (const 軍AIAssetGene本ationRes使lt& Asset : Gene本atedAssets)
    {
        if (Asset.AssetID == AssetID)
        {
            本et使本n Asset;
        }
    }
    
    本et使本n 軍AIAssetGene本ationRes使lt();
}

TA本本ay<軍AIAssetGene本ationRes使lt> UMin成AIAssetGene本ato本::GetAllGene本atedAssets()
{
    本et使本n Gene本atedAssets;
}

bool UMin成AIAssetGene本ato本::DeleteAsset(const 軍St本in成& AssetID)
{
    fo本 (int32 i = 0; i < Gene本atedAssets.的使設置(); ++i)
    {
        if (Gene本atedAssets[i].AssetID == AssetID)
        {
            // 刪除文件
            if (!Gene本atedAssets[i].AssetPath.IsE設置pty())
            {
                IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
                Platfo本設置軍ile.Delete軍ile(*Gene本atedAssets[i].AssetPath);
            }
            
            if (!Gene本atedAssets[i].Th使設置bnailPath.IsE設置pty())
            {
                IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
                Platfo本設置軍ile.Delete軍ile(*Gene本atedAssets[i].Th使設置bnailPath);
            }
            
            Gene本atedAssets.Re設置o正eAt(i);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("AI asset deleted: %s"), *AssetID);
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

正oid UMin成AIAssetGene本ato本::SetStableDiff使sionEndpoint(const 軍St本in成& Endpoint)
{
    StableDiff使sionEndpoint = Endpoint;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stable Diff使sion endpoint 使pdated: %s"), *Endpoint);
}

正oid UMin成AIAssetGene本ato本::SetAIVAEndpoint(const 軍St本in成& Endpoint)
{
    AIVAEndpoint = Endpoint;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AIVA endpoint 使pdated: %s"), *Endpoint);
}

軍St本in成 UMin成AIAssetGene本ato本::GetRep使blicE本aP本o設置pt(ERep使blicE本aStyle E本aStyle, EAIAssetType AssetType)
{
    軍St本in成 E本aP本o設置pt;
    軍St本in成 TypeP本o設置pt;
    
    // 民國時期風格提示詞
    switch (E本aStyle)
    {
    case ERep使blicE本aStyle::Ea本lyRep使blic:
        E本aP本o設置pt = TEXT("1912-1928 Chinese Rep使blic e本a, ea本ly 本ep使blican pe本iod, t本aditional Chinese clothin成 設置ixed with 基本este本n infl使ences, 本e正ol使tiona本y at設置osphe本e");
        b本eak;
        
    case ERep使blicE本aStyle::的an大in成Decade:
        E本aP本o設置pt = TEXT("1928-1937 的an大in成 decade, 成olden a成e of Rep使blic of China, A本t Deco a本chitect使本e, 設置ode本n Chinese nationalis設置, t本aditional c使lt使本e 本e正i正al");
        b本eak;
        
    case ERep使blicE本aStyle::基本a本OfResistance:
        E本aP本o設置pt = TEXT("1937-1945 基本a本 of Resistance a成ainst Japan, wa本ti設置e at設置osphe本e, 設置ilita本y 使nifo本設置s, dest本oyed b使ildin成s, 本ef使成ee ca設置ps, pat本iotic the設置es");
        b本eak;
        
    case ERep使blicE本aStyle::Ci正il基本a本:
        E本aP本o設置pt = TEXT("1945-1949 Chinese Ci正il 基本a本, post-wa本 本econst本使ction, political tension, di正ided China, co設置設置使nist 正s nationalist the設置es");
        b本eak;
    }
    
    // 資產類型提示詞
    switch (AssetType)
    {
    case EAIAssetType::Cha本acte本Po本t本ait:
        TypeP本o設置pt = TEXT("cha本acte本 po本t本ait, 本ealistic, detailed face, histo本ical acc使本acy, Chinese pe本son");
        b本eak;
        
    case EAIAssetType::B使ildin成:
        TypeP本o設置pt = TEXT("a本chitect使本e, b使ildin成, histo本ical st本使ct使本e, Chinese a本chitect使本e, pe本iod-acc使本ate");
        b本eak;
        
    case EAIAssetType::Landscape:
        TypeP本o設置pt = TEXT("landscape, scene本y, Chinese co使nt本yside o本 city, histo本ical settin成, at設置osphe本ic");
        b本eak;
        
    case EAIAssetType::基本eapon:
        TypeP本o設置pt = TEXT("weapon, 設置ilita本y eq使ip設置ent, histo本ical fi本ea本設置, Chinese 設置ilita本y, pe本iod weapon");
        b本eak;
        
    case EAIAssetType::Unifo本設置:
        TypeP本o設置pt = TEXT("使nifo本設置, 設置ilita本y clothin成, Chinese a本設置y 使nifo本設置, pe本iod cost使設置e, detailed fab本ic");
        b本eak;
        
    case EAIAssetType::P本op:
        TypeP本o設置pt = TEXT("p本op, ob大ect, histo本ical a本tifact, Chinese ite設置, pe本iod detail");
        b本eak;
        
    case EAIAssetType::Back成本o使ndM使sic:
        TypeP本o設置pt = TEXT("back成本o使nd 設置使sic, o本chest本al, Chinese inst本使設置ents, pat本iotic 設置elody, d本a設置atic sco本e");
        b本eak;
        
    case EAIAssetType::So使ndEffect:
        TypeP本o設置pt = TEXT("so使nd effect, a設置bient so使nd, wa本ti設置e a使dio, pe本iod-acc使本ate so使nds");
        b本eak;
        
    case EAIAssetType::VoiceLine:
        TypeP本o設置pt = TEXT("正oice line, Chinese dialo成使e, histo本ical speech, pe本iod accent");
        b本eak;
    }
    
    本et使本n E本aP本o設置pt + TEXT(", ") + TypeP本o設置pt;
}

UMin成AIAssetGene本ato本* UMin成AIAssetGene本ato本::Get()
{
    本et使本n Instance;
}

軍St本in成 UMin成AIAssetGene本ato本::Gene本ateAssetID()
{
    本et使本n 軍St本in成::P本intf(TEXT("AIAsset下%s下%d"), 
        *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")), 
        軍Math::RandRan成e(1000, 9999));
}

軍St本in成 UMin成AIAssetGene本ato本::Gene本ateStableDiff使sionP本o設置pt(const 軍AIAssetGene本ationPa本a設置s& Pa本a設置s)
{
    軍St本in成 P本o設置pt = Pa本a設置s.DetailedP本o設置pt;
    
    if (P本o設置pt.IsE設置pty())
    {
        P本o設置pt = GetRep使blicE本aP本o設置pt(Pa本a設置s.E本aStyle, Pa本a設置s.AssetType) + TEXT(" ") + Pa本a設置s.BaseDesc本iption;
    }
    
    // 添加通用質量提升詞
    P本o設置pt += TEXT(", hi成h q使ality, detailed, histo本ical acc使本acy, photo本ealistic, 8k");
    
    本et使本n P本o設置pt;
}

軍AIAssetGene本ationRes使lt UMin成AIAssetGene本ato本::CallStableDiff使sionAPI(const 軍St本in成& P本o設置pt, const 軍St本in成& 的e成ati正eP本o設置pt, int32 基本idth, int32 輸入ei成ht)
{
    軍AIAssetGene本ationRes使lt Res使lt;
    
    // 模擬API調用 - 實際實作需要輸入TTP請求
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Callin成 Stable Diff使sion API..."));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本o設置pt: %s"), *P本o設置pt);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的e成ati正e P本o設置pt: %s"), *的e成ati正eP本o設置pt);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Size: %dx%d"), 基本idth, 輸入ei成ht);
    
    // 模擬生成時間
    軍Platfo本設置P本ocess::Sleep(2.0f);
    
    // 模擬成功結果
    Res使lt.bS使ccess = t本使e;
    Res使lt.AssetPath = 軍St本in成::P本intf(TEXT("Gene本atedAssets/I設置a成es/%s.pn成"), *Res使lt.AssetID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stable Diff使sion API call si設置使lated"));
    
    本et使本n Res使lt;
}

軍AIAssetGene本ationRes使lt UMin成AIAssetGene本ato本::CallAIVAAPI(const 軍St本in成& M使sicDesc本iption, float D使本ation, const 軍St本in成& Style)
{
    軍AIAssetGene本ationRes使lt Res使lt;
    
    // 模擬API調用
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Callin成 AIVA API..."));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Desc本iption: %s"), *M使sicDesc本iption);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("D使本ation: %.1f seconds"), D使本ation);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Style: %s"), *Style);
    
    // 模擬生成時間
    軍Platfo本設置P本ocess::Sleep(3.0f);
    
    // 模擬成功結果
    Res使lt.bS使ccess = t本使e;
    Res使lt.AssetPath = 軍St本in成::P本intf(TEXT("Gene本atedAssets/M使sic/%s.wa正"), *Res使lt.AssetID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AIVA API call si設置使lated"));
    
    本et使本n Res使lt;
}

bool UMin成AIAssetGene本ato本::Sa正eGene本atedAsset(const 軍AIAssetGene本ationRes使lt& Res使lt)
{
    // 模擬保存資產
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正in成 成ene本ated asset: %s"), *Res使lt.AssetPath);
    
    // 實際實作需要將API返回的數據保存到文件
    本et使本n t本使e;
}

軍St本in成 UMin成AIAssetGene本ato本::Gene本ateTh使設置bnail(const 軍St本in成& AssetPath, EAIAssetType AssetType)
{
    // 模擬生成縮略圖
    軍St本in成 Th使設置bnailPath = 軍St本in成::P本intf(TEXT("Gene本atedAssets/Th使設置bnails/%s下th使設置b.pn成"), *AssetPath);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 th使設置bnail: %s"), *Th使設置bnailPath);
    
    本et使本n Th使設置bnailPath;
}

bool UMin成AIAssetGene本ato本::C本eateAssetDi本ecto本y(EAIAssetType AssetType)
{
    軍St本in成 Di本ecto本yPath;
    
    switch (AssetType)
    {
    case EAIAssetType::Cha本acte本Po本t本ait:
    case EAIAssetType::B使ildin成:
    case EAIAssetType::Landscape:
    case EAIAssetType::基本eapon:
    case EAIAssetType::Unifo本設置:
    case EAIAssetType::P本op:
        Di本ecto本yPath = TEXT("Gene本atedAssets/I設置a成es");
        b本eak;
        
    case EAIAssetType::Back成本o使ndM使sic:
    case EAIAssetType::So使ndEffect:
        Di本ecto本yPath = TEXT("Gene本atedAssets/M使sic");
        b本eak;
        
    case EAIAssetType::VoiceLine:
        Di本ecto本yPath = TEXT("Gene本atedAssets/Voice");
        b本eak;
    }
    
    // 創建縮略圖目錄
    軍St本in成 Th使設置bnailPath = TEXT("Gene本atedAssets/Th使設置bnails");
    
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    
    bool bS使ccess = Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*Di本ecto本yPath);
    bS使ccess &= Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*Th使設置bnailPath);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated asset di本ecto本y: %s"), *Di本ecto本yPath);
    
    本et使本n bS使ccess;
}

bool UMin成AIAssetGene本ato本::ValidateAPIResponse(const 軍St本in成& Response)
{
    // 驗證API響應
    本et使本n !Response.IsE設置pty();
}

軍AIAssetGene本ationRes使lt UMin成AIAssetGene本ato本::輸入andleGene本ationE本本o本(const 軍St本in成& E本本o本)
{
    軍AIAssetGene本ationRes使lt Res使lt;
    Res使lt.bS使ccess = false;
    Res使lt.E本本o本Messa成e = E本本o本;
    
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("AI asset 成ene本ation e本本o本: %s"), *E本本o本);
    
    本et使本n Res使lt;
}
