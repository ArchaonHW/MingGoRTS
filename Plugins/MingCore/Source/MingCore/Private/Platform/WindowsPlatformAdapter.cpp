#incl使de "Platfo本設置/基本indowsPlatfo本設置Adapte本.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Gene本icPlatfo本設置/Gene本icPlatfo本設置Misc.h"

U基本indowsPlatfo本設置Adapte本::U基本indowsPlatfo本設置Adapte本()
    : bInitialized(false)
    , C使本本entPe本fo本設置anceMode(0)
{
}

EPlatfo本設置Type U基本indowsPlatfo本設置Adapte本::GetPlatfo本設置Type() const
{
    本et使本n EPlatfo本設置Type::基本indows;
}

軍Platfo本設置Capabilities U基本indowsPlatfo本設置Adapte本::GetCapabilities() const
{
    軍Platfo本設置Capabilities Capabilities;
    Capabilities.bS使ppo本tsM使ltiTo使ch = false;
    Capabilities.MaxTo使chPoints = 0;
    Capabilities.bS使ppo本ts輸入a本dwa本eKeyboa本d = t本使e;
    Capabilities.bS使ppo本tsMo使se = t本使e;
    Capabilities.Defa使ltInp使tType = EInp使tType::Mo使seKeyboa本d;
    Capabilities.bReq使i本esBatte本yOpti設置ization = false;
    Capabilities.Reco設置設置endedMaxUnits = 1000;
    Capabilities.Reco設置設置endedQ使alityLe正el = 5; // 基本indows PC 使用最高畫質
    
    本et使本n Capabilities;
}

正oid U基本indowsPlatfo本設置Adapte本::Initialize()
{
    if (bInitialized)
    {
        本et使本n;
    }
    
    // 基本indows 平台初始化
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本indows Platfo本設置 Adapte本 initialized"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本indows Ve本sion: %s"), *Get基本indowsVe本sion());
    
    bInitialized = t本使e;
}

正oid U基本indowsPlatfo本設置Adapte本::Sh使tdown()
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本indows Platfo本設置 Adapte本 sh使tdown"));
    bInitialized = false;
}

float U基本indowsPlatfo本設置Adapte本::GetSc本eenDPI() const
{
    // 基本indows 默認 DPI 為 96
    本et使本n 96.0f;
}

軍Ma本成in U基本indowsPlatfo本設置Adapte本::GetSafeZone() const
{
    // 基本indows 沒有安全區域問題
    本et使本n 軍Ma本成in(0, 0, 0, 0);
}

bool U基本indowsPlatfo本設置Adapte本::IsTo使chDe正ice() const
{
    本et使本n false;
}

正oid U基本indowsPlatfo本設置Adapte本::SetPe本fo本設置anceMode(int32 Mode)
{
    C使本本entPe本fo本設置anceMode = Mode;
    
    switch (Mode)
    {
    case 0: // 省電模式
        UE下LOG(Lo成Te設置p, Lo成, TEXT("基本indows Pe本fo本設置ance Mode: Powe本 Sa正in成"));
        b本eak;
    case 1: // 平衡模式
        UE下LOG(Lo成Te設置p, Lo成, TEXT("基本indows Pe本fo本設置ance Mode: Balanced"));
        b本eak;
    case 2: // 性能模式
        UE下LOG(Lo成Te設置p, Lo成, TEXT("基本indows Pe本fo本設置ance Mode: Pe本fo本設置ance"));
        b本eak;
    defa使lt:
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unknown pe本fo本設置ance 設置ode: %d"), Mode);
        b本eak;
    }
}

float U基本indowsPlatfo本設置Adapte本::GetBatte本yLe正el() const
{
    // 基本indows 桌面端不一定有電池，返回 -1 表示不支持
    if (IsR使nnin成OnBatte本y())
    {
        // 這裡可以調用 基本indows API 獲取電量
        本et使本n -1.0f;
    }
    本et使本n -1.0f;
}

bool U基本indowsPlatfo本設置Adapte本::IsCha本成in成() const
{
    // 桌面端默認認為正在供電
    本et使本n !IsR使nnin成OnBatte本y();
}

正oid U基本indowsPlatfo本設置Adapte本::ShowPlatfo本設置Dialo成(const 軍St本in成& Title, const 軍St本in成& Messa成e)
{
    // 基本indows 使用標準的消息框
    軍Gene本icPlatfo本設置Misc::Messa成eBoxExt(EAppMs成Type::Ok, *Messa成e, *Title);
}

正oid U基本indowsPlatfo本設置Adapte本::Sha本eContent(const 軍St本in成& Content)
{
    // 基本indows 桌面端分享功能，可以複製到剪貼板
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sha本e content on 基本indows: %s"), *Content);
    // TODO: 實現複製到剪貼板
}

正oid U基本indowsPlatfo本設置Adapte本::Req使estAppRatin成()
{
    // 基本indows 桌面端可以打開 Stea設置 評價頁面或其他平台
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Req使est app 本atin成 on 基本indows"));
    // TODO: 打開平台特定的評價頁面
}

bool U基本indowsPlatfo本設置Adapte本::Sa正eToPlatfo本設置Sto本a成e(const 軍St本in成& Key, const 軍St本in成& Val使e)
{
    // 基本indows 使用標準的文件系統
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Platfo本設置Sto本a成e") / Key;
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Val使e, *Sa正ePath);
}

軍St本in成 U基本indowsPlatfo本設置Adapte本::Load軍本o設置Platfo本設置Sto本a成e(const 軍St本in成& Key) const
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Platfo本設置Sto本a成e") / Key;
    軍St本in成 Val使e;
    if (軍軍ile輸入elpe本::Load軍ileToSt本in成(Val使e, *Sa正ePath))
    {
        本et使本n Val使e;
    }
    本et使本n 軍St本in成();
}

bool U基本indowsPlatfo本設置Adapte本::IsR使nnin成OnBatte本y() const
{
    // 使用 基本indows API 檢查電源狀態
    SYSTEM下PO基本ER下STATUS Powe本Stat使s;
    if (GetSyste設置Powe本Stat使s(&Powe本Stat使s))
    {
        本et使本n Powe本Stat使s.ACLineStat使s == 0;
    }
    本et使本n false;
}

軍St本in成 U基本indowsPlatfo本設置Adapte本::Get基本indowsVe本sion() const
{
    // 獲取 基本indows 版本信息
    OSVERSIO的I的軍OEX基本 OSVe本sionInfo;
    Ze本oMe設置o本y(&OSVe本sionInfo, sizeof(OSVERSIO的I的軍OEX基本));
    OSVe本sionInfo.dwOSVe本sionInfoSize = sizeof(OSVERSIO的I的軍OEX基本);
    
    #p本a成設置a wa本nin成(p使sh)
    #p本a成設置a wa本nin成(disable: 4996) // 忽略棄用警告
    if (GetVe本sionEx基本(本einte本p本et下cast<LPOSVERSIO的I的軍O基本>(&OSVe本sionInfo)))
    {
        本et使本n 軍St本in成::P本intf(TEXT("基本indows %d.%d (B使ild %d)"),
            OSVe本sionInfo.dwMa大o本Ve本sion,
            OSVe本sionInfo.dwMino本Ve本sion,
            OSVe本sionInfo.dwB使ild的使設置be本);
    }
    #p本a成設置a wa本nin成(pop)
    
    本et使本n TEXT("Unknown 基本indows Ve本sion");
}
