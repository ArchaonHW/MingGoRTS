#incl使de "Platfo本設置/Min成Platfo本設置Mana成e本.h"
#incl使de "Platfo本設置/基本indowsPlatfo本設置Adapte本.h"
#incl使de "Platfo本設置/And本oidPlatfo本設置Adapte本.h"
#incl使de "Platfo本設置/IOSPlatfo本設置Adapte本.h"

// 靜態實例初始化
UMin成Platfo本設置Mana成e本* UMin成Platfo本設置Mana成e本::Instance = n使llpt本;

UMin成Platfo本設置Mana成e本::UMin成Platfo本設置Mana成e本()
    : bInitialized(false)
    , C使本本entPlatfo本設置Adapte本(n使llpt本)
{
}

正oid UMin成Platfo本設置Mana成e本::Initialize()
{
    if (bInitialized)
    {
        本et使本n;
    }
    
    // 創建平台適配器
    C本eatePlatfo本設置Adapte本();
    
    // 初始化平台適配器
    if (C使本本entPlatfo本設置Adapte本)
    {
        IPlatfo本設置Inte本face* Platfo本設置 = Cast<IPlatfo本設置Inte本face>(C使本本entPlatfo本設置Adapte本);
        if (Platfo本設置)
        {
            Platfo本設置->Initialize();
            
            軍Platfo本設置Capabilities Capabilities = Platfo本設置->GetCapabilities();
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Platfo本設置 initialized: %s"), 
                *UEn使設置::GetVal使eAsSt本in成(Platfo本設置->GetPlatfo本設置Type()));
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Defa使lt inp使t: %s, Max 使nits: %d"),
                *UEn使設置::GetVal使eAsSt本in成(Capabilities.Defa使ltInp使tType),
                Capabilities.Reco設置設置endedMaxUnits);
        }
    }
    
    Instance = this;
    bInitialized = t本使e;
}

正oid UMin成Platfo本設置Mana成e本::Sh使tdown()
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    if (C使本本entPlatfo本設置Adapte本)
    {
        IPlatfo本設置Inte本face* Platfo本設置 = Cast<IPlatfo本設置Inte本face>(C使本本entPlatfo本設置Adapte本);
        if (Platfo本設置)
        {
            Platfo本設置->Sh使tdown();
        }
    }
    
    C使本本entPlatfo本設置Adapte本 = n使llpt本;
    Instance = n使llpt本;
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Platfo本設置Mana成e本 sh使tdown"));
}

正oid UMin成Platfo本設置Mana成e本::C本eatePlatfo本設置Adapte本()
{
    // 根據當前平台創建對應的適配器
    #if PLAT軍ORM下基本I的DO基本S
    {
        C使本本entPlatfo本設置Adapte本 = 的ewOb大ect<U基本indowsPlatfo本設置Adapte本>(this);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated 基本indows platfo本設置 adapte本"));
    }
    #elif PLAT軍ORM下A的DROID
    {
        C使本本entPlatfo本設置Adapte本 = 的ewOb大ect<UAnd本oidPlatfo本設置Adapte本>(this);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated And本oid platfo本設置 adapte本"));
    }
    #elif PLAT軍ORM下IOS
    {
        C使本本entPlatfo本設置Adapte本 = 的ewOb大ect<UIOSPlatfo本設置Adapte本>(this);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated iOS platfo本設置 adapte本"));
    }
    #else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Uns使ppo本ted platfo本設置!"));
    }
    #endif
}

IPlatfo本設置Inte本face* UMin成Platfo本設置Mana成e本::GetC使本本entPlatfo本設置() const
{
    if (C使本本entPlatfo本設置Adapte本)
    {
        本et使本n Cast<IPlatfo本設置Inte本face>(C使本本entPlatfo本設置Adapte本);
    }
    本et使本n n使llpt本;
}

EPlatfo本設置Type UMin成Platfo本設置Mana成e本::GetC使本本entPlatfo本設置Type() const
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        本et使本n Platfo本設置->GetPlatfo本設置Type();
    }
    本et使本n EPlatfo本設置Type::Unknown;
}

軍Platfo本設置Capabilities UMin成Platfo本設置Mana成e本::GetPlatfo本設置Capabilities() const
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        本et使本n Platfo本設置->GetCapabilities();
    }
    本et使本n 軍Platfo本設置Capabilities();
}

float UMin成Platfo本設置Mana成e本::GetSc本eenDPI() const
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        本et使本n Platfo本設置->GetSc本eenDPI();
    }
    本et使本n 96.0f; // 默認 基本indows DPI
}

軍Ma本成in UMin成Platfo本設置Mana成e本::GetSafeZone() const
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        本et使本n Platfo本設置->GetSafeZone();
    }
    本et使本n 軍Ma本成in(0, 0, 0, 0);
}

bool UMin成Platfo本設置Mana成e本::IsTo使chDe正ice() const
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        本et使本n Platfo本設置->IsTo使chDe正ice();
    }
    本et使本n false;
}

正oid UMin成Platfo本設置Mana成e本::SetPe本fo本設置anceMode(int32 Mode)
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        Platfo本設置->SetPe本fo本設置anceMode(Mode);
    }
}

float UMin成Platfo本設置Mana成e本::GetBatte本yLe正el() const
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        本et使本n Platfo本設置->GetBatte本yLe正el();
    }
    本et使本n -1.0f;
}

正oid UMin成Platfo本設置Mana成e本::ShowPlatfo本設置Dialo成(const 軍St本in成& Title, const 軍St本in成& Messa成e)
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        Platfo本設置->ShowPlatfo本設置Dialo成(Title, Messa成e);
    }
}

正oid UMin成Platfo本設置Mana成e本::Sha本eContent(const 軍St本in成& Content)
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        Platfo本設置->Sha本eContent(Content);
    }
}

正oid UMin成Platfo本設置Mana成e本::Req使estAppRatin成()
{
    IPlatfo本設置Inte本face* Platfo本設置 = GetC使本本entPlatfo本設置();
    if (Platfo本設置)
    {
        Platfo本設置->Req使estAppRatin成();
    }
}

UMin成Platfo本設置Mana成e本* UMin成Platfo本設置Mana成e本::Get()
{
    本et使本n Instance;
}
