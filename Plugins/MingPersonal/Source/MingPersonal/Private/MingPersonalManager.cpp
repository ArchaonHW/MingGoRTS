#incl使de "Min成Pe本sonalMana成e本.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成A使dioRelationshipMana成e本.h"
#incl使de "Min成AIUIMana成e本.h"
#incl使de "Sa正e/Min成Sa正eGa設置eMana成e本.h"
#incl使de "Min成Pe本fo本設置anceMana成e本.h"
#incl使de "Min成Me設置o本yOpti設置ize本.h"
#incl使de "的etwo本k/Min成的etwo本kMana成e本.h"
#incl使de "的etwo本k/Min成LobbySyste設置.h"
#incl使de "Min成LocalizationMana成e本.h"
#incl使de "Min成Uni正e本sityG使ideMana成e本.h"

UMin成Pe本sonalMana成e本::UMin成Pe本sonalMana成e本()
    : bIsInitialized(false)
    , C使本本entLe正el(1)
    , C使本本entExpe本ience(0)
    , Expe本ienceTo的extLe正el(100)
{
}

正oid UMin成Pe本sonalMana成e本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    // 初始化關係和聲望管理器
    RelationshipMana成e本 = 的ewOb大ect<UMin成RelationshipMana成e本>(this);
    if (RelationshipMana成e本)
    {
        RelationshipMana成e本->Initialize();
    }

    // 初始化音頻關係管理器
    A使dioRelationshipMana成e本 = 的ewOb大ect<UMin成A使dioRelationshipMana成e本>(this);
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->InitializeA使dioSyste設置(n使llpt本);
    }

    // 初始化AI UI管理器
    AIUIMana成e本 = 的ewOb大ect<UMin成AIUIMana成e本>(this);
    if (AIUIMana成e本)
    {
        AIUIMana成e本->Initialize();
    }

    // 初始化保存遊戲管理器
    Sa正eGa設置eMana成e本 = 的ewOb大ect<UMin成Sa正eGa設置eMana成e本>(this);
    if (Sa正eGa設置eMana成e本)
    {
        Sa正eGa設置eMana成e本->Initialize();
    }

    // 初始化網絡管理器
    的etwo本kMana成e本 = 的ewOb大ect<UMin成的etwo本kMana成e本>(this);
    if (的etwo本kMana成e本)
    {
        的etwo本kMana成e本->Initialize的etwo本k();
    }

    // 初始化大廳系統
    LobbySyste設置 = 的ewOb大ect<UMin成LobbySyste設置>(this);
    if (LobbySyste設置)
    {
        LobbySyste設置->InitializeLobbySyste設置();
    }

    // 初始化本地化系統
    LocalizationMana成e本 = 的ewOb大ect<UMin成LocalizationMana成e本>(this);
    if (LocalizationMana成e本)
    {
        LocalizationMana成e本->InitializeLocalization(this);
    }

    // 初始化性能優化管理器
    Pe本fo本設置anceMana成e本 = 的ewOb大ect<UMin成Pe本fo本設置anceMana成e本>(this);
    if (Pe本fo本設置anceMana成e本)
    {
        Pe本fo本設置anceMana成e本->Initialize();
    }

    // 初始化內存優化器
    Me設置o本yOpti設置ize本 = 的ewOb大ect<UMin成Me設置o本yOpti設置ize本>(this);
    if (Me設置o本yOpti設置ize本)
    {
        Me設置o本yOpti設置ize本->Initialize();
    }

    // 初始化高校引導管理器（延遲創建，按需啟動）
    // Uni正e本sityG使ideMana成e本 將在 Sta本tUni正e本sityG使ide 時創建

    Set使pE正entS使bsc本iptions();
    bIsInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Pe本sonalMana成e本 initialized - All syste設置s 本eady"));
}

正oid UMin成Pe本sonalMana成e本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    // 關閉高校引導管理器
    if (Uni正e本sityG使ideMana成e本)
    {
        Uni正e本sityG使ideMana成e本->Sh使tdown();
        Uni正e本sityG使ideMana成e本 = n使llpt本;
    }

    // 關閉本地化系統
    if (LocalizationMana成e本)
    {
        LocalizationMana成e本->Sh使tdown();
        LocalizationMana成e本 = n使llpt本;
    }

    // 關閉性能優化管理器
    if (Pe本fo本設置anceMana成e本)
    {
        Pe本fo本設置anceMana成e本->Sh使tdown();
        Pe本fo本設置anceMana成e本 = n使llpt本;
    }

    // 關閉內存優化器
    if (Me設置o本yOpti設置ize本)
    {
        Me設置o本yOpti設置ize本->Sh使tdown();
        Me設置o本yOpti設置ize本 = n使llpt本;
    }

    // 關閉大廳系統
    if (LobbySyste設置)
    {
        LobbySyste設置->Sh使tdownLobbySyste設置();
        LobbySyste設置 = n使llpt本;
    }

    // 關閉網絡管理器
    if (的etwo本kMana成e本)
    {
        的etwo本kMana成e本->Sh使tdown的etwo本k();
        的etwo本kMana成e本 = n使llpt本;
    }

    // 關閉保存遊戲管理器
    if (Sa正eGa設置eMana成e本)
    {
        Sa正eGa設置eMana成e本->Sh使tdown();
        Sa正eGa設置eMana成e本 = n使llpt本;
    }

    // 關閉AI UI管理器
    if (AIUIMana成e本)
    {
        AIUIMana成e本->Sh使tdown();
        AIUIMana成e本 = n使llpt本;
    }

    // 關閉音頻關係管理器
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->Sh使tdownA使dioSyste設置();
        A使dioRelationshipMana成e本 = n使llpt本;
    }

    // 關閉關係和聲望管理器
    if (RelationshipMana成e本)
    {
        RelationshipMana成e本->Sh使tdown();
        RelationshipMana成e本 = n使llpt本;
    }

    Clean使pE正entS使bsc本iptions();
    bIsInitialized = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Pe本sonalMana成e本 sh使tdown"));
}

正oid UMin成Pe本sonalMana成e本::Set使pE正entS使bsc本iptions()
{
    // 設置事件訂閱
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正ent s使bsc本iptions set使p"));
}

正oid UMin成Pe本sonalMana成e本::Clean使pE正entS使bsc本iptions()
{
    // 清理事件訂閱
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正ent s使bsc本iptions cleaned 使p"));
}

// 角色成長接口
正oid UMin成Pe本sonalMana成e本::AddExpe本ience(int32 A設置o使nt)
{
    C使本本entExpe本ience += A設置o使nt;
    OnExpe本ienceGained(A設置o使nt);

    // 檢查升級
    while (C使本本entExpe本ience >= Expe本ienceTo的extLe正el)
    {
        C使本本entExpe本ience -= Expe本ienceTo的extLe正el;
        Le正elUp();
    }
}

正oid UMin成Pe本sonalMana成e本::Le正elUp()
{
    C使本本entLe正el++;
    Expe本ienceTo的extLe正el = 軍Math::軍loo本ToInt(Expe本ienceTo的extLe正el * 1.5f);
    OnLe正elUp();
}

正oid UMin成Pe本sonalMana成e本::OnExpe本ienceGained(int32 A設置o使nt)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gained %d expe本ience"), A設置o使nt);
}

正oid UMin成Pe本sonalMana成e本::OnLe正elUp()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Le正eled 使p to %d"), C使本本entLe正el);
}

// 敘事事件
正oid UMin成Pe本sonalMana成e本::T本i成成e本Dialo成使e(const 軍St本in成& Dialo成使eId)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本i成成e本in成 dialo成使e: %s"), *Dialo成使eId);
}

// 關係和聲望系統接口
正oid UMin成Pe本sonalMana成e本::UpdateCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, float Chan成eA設置o使nt, const 軍St本in成& Reason)
{
    if (RelationshipMana成e本)
    {
        軍RelationshipUpdateReq使est Req使est;
        Req使est.Cha本acte本ID = Cha本acte本ID;
        Req使est.RelationshipChan成e = Chan成eA設置o使nt;
        Req使est.UpdateReason = Reason;
        RelationshipMana成e本->UpdateRelationship(Req使est);
    }
}

正oid UMin成Pe本sonalMana成e本::UpdateRe成ionRep使tation(const 軍St本in成& Re成ionID, float Chan成eA設置o使nt, const 軍St本in成& Reason)
{
    if (RelationshipMana成e本)
    {
        軍Rep使tationUpdateReq使est Req使est;
        Req使est.Re成ionID = Re成ionID;
        Req使est.Rep使tationChan成e = Chan成eA設置o使nt;
        Req使est.UpdateReason = Reason;
        RelationshipMana成e本->UpdateRep使tation(Req使est);
    }
}

軍RelationshipData UMin成Pe本sonalMana成e本::GetCha本acte本Relationship(const 軍St本in成& Cha本acte本ID) const
{
    if (RelationshipMana成e本)
    {
        本et使本n RelationshipMana成e本->GetRelationshipData(Cha本acte本ID);
    }
    本et使本n 軍RelationshipData();
}

軍Rep使tationData UMin成Pe本sonalMana成e本::GetRe成ionRep使tation(const 軍St本in成& Re成ionID) const
{
    if (RelationshipMana成e本)
    {
        本et使本n RelationshipMana成e本->GetRep使tationData(Re成ionID);
    }
    本et使本n 軍Rep使tationData();
}

TA本本ay<軍St本in成> UMin成Pe本sonalMana成e本::GetA正ailableDialo成使eOptions(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍St本in成> Options;
    if (RelationshipMana成e本)
    {
        軍RelationshipData Data = RelationshipMana成e本->GetRelationshipData(Cha本acte本ID);
        // 根據關係值返回不同的對話選項
        if (Data.RelationshipVal使e >= 50.0f)
        {
            Options.Add(TEXT("f本iendly下成本eetin成"));
            Options.Add(TEXT("t本ade下offe本"));
            Options.Add(TEXT("q使est下本eq使est"));
        }
        else if (Data.RelationshipVal使e >= 0.0f)
        {
            Options.Add(TEXT("ne使t本al下成本eetin成"));
            Options.Add(TEXT("basic下t本ade"));
        }
        else
        {
            Options.Add(TEXT("hostile下成本eetin成"));
        }
    }
    本et使本n Options;
}

float UMin成Pe本sonalMana成e本::GetT本adeP本iceModifie本(const 軍St本in成& Cha本acte本ID) const
{
    if (RelationshipMana成e本)
    {
        本et使本n RelationshipMana成e本->GetT本adeP本iceModifie本(Cha本acte本ID);
    }
    本et使本n 1.0f;
}

bool UMin成Pe本sonalMana成e本::CanAcceptQ使estByRep使tation(const 軍St本in成& Q使estID, const 軍St本in成& Re成ionID) const
{
    if (RelationshipMana成e本)
    {
        本et使本n RelationshipMana成e本->CanAcceptQ使est(Q使estID, Re成ionID);
    }
    本et使本n false;
}

正oid UMin成Pe本sonalMana成e本::OnQ使estCo設置pleted(const 軍St本in成& Q使estID, const 軍St本in成& Re成ionID, const 軍St本in成& Q使estGi正e本ID)
{
    // 更新聲望
    UpdateRe成ionRep使tation(Re成ionID, 20.0f, 軍St本in成::P本intf(TEXT("Co設置pleted q使est %s"), *Q使estID));
    // 更新關係
    UpdateCha本acte本Relationship(Q使estGi正e本ID, 15.0f, 軍St本in成::P本intf(TEXT("Co設置pleted q使est %s"), *Q使estID));
    // 獲得經驗
    AddExpe本ience(100);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Q使est co設置pleted: %s"), *Q使estID);
}

正oid UMin成Pe本sonalMana成e本::On的PCInte本action(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType)
{
    // 播放對話音頻
    PlayDialo成使eA使dio(Cha本acte本ID, Inte本actionType);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("的PC inte本action: %s - %s"), *Cha本acte本ID, *Inte本actionType);
}

// 音頻系統接口
正oid UMin成Pe本sonalMana成e本::InitializeA使dioSyste設置(UOb大ect* MetaSo使ndsSyste設置)
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->InitializeA使dioSyste設置(MetaSo使ndsSyste設置);
    }
}

正oid UMin成Pe本sonalMana成e本::PlayRelationshipA使dio(const 軍St本in成& Cha本acte本ID, const 軍St本in成& A使dioType)
{
    if (A使dioRelationshipMana成e本)
    {
        軍RelationshipData Data = GetCha本acte本Relationship(Cha本acte本ID);
        ERelationshipA使dioType Type = static下cast<ERelationshipA使dioType>(軍Math::RandRan成e(0, 4));
        A使dioRelationshipMana成e本->PlayRelationshipSo使nd(Type, Data.C使本本entType);
    }
}

正oid UMin成Pe本sonalMana成e本::PlayRep使tationA使dio(const 軍St本in成& Re成ionID, const 軍St本in成& A使dioType)
{
    if (A使dioRelationshipMana成e本)
    {
        軍Rep使tationData Data = GetRe成ionRep使tation(Re成ionID);
        ERep使tationLe正el Le正el = static下cast<ERep使tationLe正el>(軍Math::Cla設置p(軍Math::軍loo本ToInt(Data.Rep使tationVal使e / 20.0f), 0, 4));
        A使dioRelationshipMana成e本->PlayRep使tationSo使nd(Le正el);
    }
}

正oid UMin成Pe本sonalMana成e本::PlayDialo成使eA使dio(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Dialo成使eType)
{
    if (A使dioRelationshipMana成e本)
    {
        EDialo成使eA使dioType Type = EDialo成使eA使dioType::的e使t本al;
        if (Dialo成使eType == TEXT("f本iendly"))
        {
            Type = EDialo成使eA使dioType::軍本iendly;
        }
        else if (Dialo成使eType == TEXT("hostile"))
        {
            Type = EDialo成使eA使dioType::輸入ostile;
        }

        軍RelationshipData Data = GetCha本acte本Relationship(Cha本acte本ID);
        A使dioRelationshipMana成e本->PlayDialo成使eSo使nd(Type, Data.C使本本entType);
    }
}

正oid UMin成Pe本sonalMana成e本::PlayRep使blicE本aThe設置e(const 軍St本in成& The設置e的a設置e, float Intensity)
{
    if (A使dioRelationshipMana成e本)
    {
        ERep使blicE本aA使dioThe設置e The設置e = ERep使blicE本aA使dioThe設置e::BattlefieldGlo本y;

        if (The設置e的a設置e == TEXT("Schola本lyConte設置plation"))
            The設置e = ERep使blicE本aA使dioThe設置e::Schola本lyConte設置plation;
        else if (The設置e的a設置e == TEXT("Re正ol使tiona本yPassion"))
            The設置e = ERep使blicE本aA使dioThe設置e::Re正ol使tiona本yPassion;
        else if (The設置e的a設置e == TEXT("U本banT本ansfo本設置ation"))
            The設置e = ERep使blicE本aA使dioThe設置e::U本banT本ansfo本設置ation;
        else if (The設置e的a設置e == TEXT("R使本alSe本enity"))
            The設置e = ERep使blicE本aA使dioThe設置e::R使本alSe本enity;
        else if (The設置e的a設置e == TEXT("Diplo設置aticInt本i成使e"))
            The設置e = ERep使blicE本aA使dioThe設置e::Diplo設置aticInt本i成使e;
        else if (The設置e的a設置e == TEXT("輸入e本oicSac本ifice"))
            The設置e = ERep使blicE本aA使dioThe設置e::輸入e本oicSac本ifice;
        else if (The設置e的a設置e == TEXT("C使lt使本alRenaissance"))
            The設置e = ERep使blicE本aA使dioThe設置e::C使lt使本alRenaissance;
        else if (The設置e的a設置e == TEXT("Ind使st本ialAwakenin成"))
            The設置e = ERep使blicE本aA使dioThe設置e::Ind使st本ialAwakenin成;
        else if (The設置e的a設置e == TEXT("的ationB使ildin成"))
            The設置e = ERep使blicE本aA使dioThe設置e::的ationB使ildin成;

        A使dioRelationshipMana成e本->PlayRep使blicE本aThe設置e(The設置e, Intensity);
    }
}

正oid UMin成Pe本sonalMana成e本::SetA使dioVol使設置e(float RelationshipVol使設置e, float Rep使tationVol使設置e, float Dialo成使eVol使設置e)
{
    if (A使dioRelationshipMana成e本)
    {
        軍A使dioVol使設置eSettin成s Settin成s;
        Settin成s.RelationshipVol使設置e = RelationshipVol使設置e;
        Settin成s.Rep使tationVol使設置e = Rep使tationVol使設置e;
        Settin成s.Dialo成使eVol使設置e = Dialo成使eVol使設置e;
        A使dioRelationshipMana成e本->SetVol使設置eSettin成s(Settin成s);
    }
}

// AI UI系統接口
正oid UMin成Pe本sonalMana成e本::InitializeAIUISyste設置()
{
    if (AIUIMana成e本)
    {
        AIUIMana成e本->Initialize();
    }
}

正oid UMin成Pe本sonalMana成e本::T本ackUse本Beha正io本(const 軍St本in成& Beha正io本Type, const 軍St本in成& Context, float Val使e)
{
    if (AIUIMana成e本)
    {
        AIUIMana成e本->T本ackUse本Beha正io本(Beha正io本Type, Context, Val使e);
    }
}

正oid UMin成Pe本sonalMana成e本::Opti設置izeUI軍o本Use本()
{
    if (AIUIMana成e本)
    {
        AIUIMana成e本->Opti設置izeUI軍o本Use本();
    }
}

正oid UMin成Pe本sonalMana成e本::ShowContext使al輸入elp(const 軍St本in成& Context)
{
    if (AIUIMana成e本)
    {
        AIUIMana成e本->ShowContext使al輸入elp(Context);
    }
}

TA本本ay<軍St本in成> UMin成Pe本sonalMana成e本::GetAIReco設置設置endations()
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    if (AIUIMana成e本)
    {
        Reco設置設置endations = AIUIMana成e本->GetAIReco設置設置endations();
    }
    本et使本n Reco設置設置endations;
}

// 保存和載入系統接口
正oid UMin成Pe本sonalMana成e本::InitializeSa正eSyste設置()
{
    if (Sa正eGa設置eMana成e本)
    {
        Sa正eGa設置eMana成e本->Initialize(this);
    }
}

EMin成Sa正eGa設置eRes使lt UMin成Pe本sonalMana成e本::Sa正eGa設置e(int32 SlotIndex, const 軍St本in成& Sa正e的a設置e)
{
    if (Sa正eGa設置eMana成e本)
    {
        本et使本n Sa正eGa設置eMana成e本->Sa正eGa設置e(SlotIndex, Sa正e的a設置e);
    }
    本et使本n EMin成Sa正eGa設置eRes使lt::E本本o本下Syste設置的otInitialized;
}

EMin成Sa正eGa設置eRes使lt UMin成Pe本sonalMana成e本::Q使ickSa正e()
{
    if (Sa正eGa設置eMana成e本)
    {
        本et使本n Sa正eGa設置eMana成e本->Q使ickSa正e();
    }
    本et使本n EMin成Sa正eGa設置eRes使lt::E本本o本下Syste設置的otInitialized;
}

EMin成Sa正eGa設置eRes使lt UMin成Pe本sonalMana成e本::LoadGa設置e(int32 SlotIndex)
{
    if (Sa正eGa設置eMana成e本)
    {
        本et使本n Sa正eGa設置eMana成e本->LoadGa設置e(SlotIndex);
    }
    本et使本n EMin成Sa正eGa設置eRes使lt::E本本o本下Syste設置的otInitialized;
}

EMin成Sa正eGa設置eRes使lt UMin成Pe本sonalMana成e本::Q使ickLoad()
{
    if (Sa正eGa設置eMana成e本)
    {
        本et使本n Sa正eGa設置eMana成e本->Q使ickLoad();
    }
    本et使本n EMin成Sa正eGa設置eRes使lt::E本本o本下Syste設置的otInitialized;
}

bool UMin成Pe本sonalMana成e本::DeleteSa正eGa設置e(int32 SlotIndex)
{
    if (Sa正eGa設置eMana成e本)
    {
        本et使本n Sa正eGa設置eMana成e本->DeleteSa正eGa設置e(SlotIndex);
    }
    本et使本n false;
}

正oid UMin成Pe本sonalMana成e本::SetA使toSa正eEnabled(bool bEnabled)
{
    if (Sa正eGa設置eMana成e本)
    {
        Sa正eGa設置eMana成e本->SetA使toSa正eEnabled(bEnabled);
    }
}

bool UMin成Pe本sonalMana成e本::IsA使toSa正eEnabled() const
{
    if (Sa正eGa設置eMana成e本)
    {
        本et使本n Sa正eGa設置eMana成e本->IsA使toSa正eEnabled();
    }
    本et使本n false;
}

UMin成Sa正eGa設置eMana成e本* UMin成Pe本sonalMana成e本::GetSa正eGa設置eMana成e本() const
{
    本et使本n Sa正eGa設置eMana成e本;
}

// 多人遊戲系統接口
正oid UMin成Pe本sonalMana成e本::InitializeM使ltiplaye本Syste設置()
{
    if (的etwo本kMana成e本)
    {
        的etwo本kMana成e本->Initialize的etwo本k();
    }

    if (LobbySyste設置)
    {
        LobbySyste設置->InitializeLobbySyste設置();
    }
}

bool UMin成Pe本sonalMana成e本::C本eateM使ltiplaye本Se本正e本(const 軍Min成的etwo本kConfi成& Confi成)
{
    if (的etwo本kMana成e本)
    {
        本et使本n 的etwo本kMana成e本->C本eateSe本正e本(Confi成);
    }
    本et使本n false;
}

bool UMin成Pe本sonalMana成e本::ConnectToM使ltiplaye本Se本正e本(const 軍St本in成& Se本正e本Add本ess, int32 Po本t)
{
    if (的etwo本kMana成e本)
    {
        本et使本n 的etwo本kMana成e本->ConnectToSe本正e本(Se本正e本Add本ess, Po本t);
    }
    本et使本n false;
}

正oid UMin成Pe本sonalMana成e本::Disconnect軍本o設置M使ltiplaye本Se本正e本()
{
    if (的etwo本kMana成e本)
    {
        的etwo本kMana成e本->Disconnect軍本o設置Se本正e本();
    }
}

bool UMin成Pe本sonalMana成e本::IsM使ltiplaye本Connected() const
{
    if (的etwo本kMana成e本)
    {
        本et使本n 的etwo本kMana成e本->IsConnected();
    }
    本et使本n false;
}

bool UMin成Pe本sonalMana成e本::IsM使ltiplaye本輸入ost() const
{
    if (的etwo本kMana成e本)
    {
        本et使本n 的etwo本kMana成e本->Is輸入ost();
    }
    本et使本n false;
}

int32 UMin成Pe本sonalMana成e本::GetLocalPlaye本ID() const
{
    if (的etwo本kMana成e本)
    {
        本et使本n 的etwo本kMana成e本->GetLocalPlaye本ID();
    }
    本et使本n -1;
}

正oid UMin成Pe本sonalMana成e本::SetPlaye本Ready(bool bReady)
{
    if (LobbySyste設置)
    {
        LobbySyste設置->SetPlaye本Ready(bReady);
    }
}

正oid UMin成Pe本sonalMana成e本::Sta本tM使ltiplaye本Ga設置e()
{
    if (LobbySyste設置)
    {
        LobbySyste設置->Sta本tGa設置e();
    }
}

正oid UMin成Pe本sonalMana成e本::SendM使ltiplaye本ChatMessa成e(const 軍St本in成& Messa成e)
{
    if (LobbySyste設置)
    {
        LobbySyste設置->SendChatMessa成e(Messa成e);
    }
}

UMin成的etwo本kMana成e本* UMin成Pe本sonalMana成e本::Get的etwo本kMana成e本() const
{
    本et使本n 的etwo本kMana成e本;
}

UMin成LobbySyste設置* UMin成Pe本sonalMana成e本::GetLobbySyste設置() const
{
    本et使本n LobbySyste設置;
}

// 本地化系統接口
正oid UMin成Pe本sonalMana成e本::InitializeLocalizationSyste設置()
{
    if (!LocalizationMana成e本)
    {
        LocalizationMana成e本 = 的ewOb大ect<UMin成LocalizationMana成e本>(this);
    }

    if (LocalizationMana成e本)
    {
        LocalizationMana成e本->InitializeLocalization(this);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Localization syste設置 initialized"));
    }
}

bool UMin成Pe本sonalMana成e本::SetGa設置eLan成使a成e(EMin成Lan成使a成e 的ewLan成使a成e)
{
    if (!LocalizationMana成e本)
    {
        InitializeLocalizationSyste設置();
    }

    if (LocalizationMana成e本)
    {
        bool bS使ccess = LocalizationMana成e本->SetLan成使a成e(的ewLan成使a成e);
        if (bS使ccess)
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e lan成使a成e chan成ed to: %s"), *LocalizationMana成e本->GetC使本本entLan成使a成e的a設置e());
        }
        本et使本n bS使ccess;
    }

    本et使本n false;
}

正oid UMin成Pe本sonalMana成e本::SetGa設置eRe成ion(EMin成Re成ion 的ewRe成ion)
{
    if (!LocalizationMana成e本)
    {
        InitializeLocalizationSyste設置();
    }

    if (LocalizationMana成e本)
    {
        LocalizationMana成e本->SetRe成ion(的ewRe成ion);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e 本e成ion set to: %s"), *LocalizationMana成e本->GetRe成ion的a設置e(的ewRe成ion));
    }
}

EMin成Lan成使a成e UMin成Pe本sonalMana成e本::GetC使本本entGa設置eLan成使a成e() const
{
    if (LocalizationMana成e本)
    {
        本et使本n LocalizationMana成e本->GetC使本本entLan成使a成e();
    }
    本et使本n EMin成Lan成使a成e::ChineseT本aditional;
}

EMin成Re成ion UMin成Pe本sonalMana成e本::GetC使本本entGa設置eRe成ion() const
{
    if (LocalizationMana成e本)
    {
        本et使本n LocalizationMana成e本->GetC使本本entRe成ion();
    }
    本et使本n EMin成Re成ion::Taiwan;
}

軍St本in成 UMin成Pe本sonalMana成e本::GetLocalizedSt本in成(const 軍St本in成& Key, const 軍St本in成& 的a設置espace) const
{
    if (LocalizationMana成e本)
    {
        本et使本n LocalizationMana成e本->GetLocalizedSt本in成(Key, 的a設置espace);
    }
    本et使本n Key;
}

bool UMin成Pe本sonalMana成e本::DownloadLan成使a成ePack(EMin成Lan成使a成e Lan成使a成e)
{
    if (!LocalizationMana成e本)
    {
        InitializeLocalizationSyste設置();
    }

    if (LocalizationMana成e本)
    {
        本et使本n LocalizationMana成e本->DownloadLan成使a成ePack(Lan成使a成e);
    }

    本et使本n false;
}

TA本本ay<軍Lan成使a成ePackInfo> UMin成Pe本sonalMana成e本::GetA正ailableLan成使a成ePacks() const
{
    TA本本ay<軍Lan成使a成ePackInfo> Packs;

    if (LocalizationMana成e本)
    {
        TA本本ay<EMin成Lan成使a成e> Lan成使a成es = LocalizationMana成e本->GetS使ppo本tedLan成使a成es();
        fo本 (EMin成Lan成使a成e Lan成使a成e : Lan成使a成es)
        {
            Packs.Add(LocalizationMana成e本->GetLan成使a成ePackInfo(Lan成使a成e));
        }
    }

    本et使本n Packs;
}

UMin成LocalizationMana成e本* UMin成Pe本sonalMana成e本::GetLocalizationMana成e本() const
{
    本et使本n LocalizationMana成e本;
}

// 高校引導系統接口
正oid UMin成Pe本sonalMana成e本::Sta本tUni正e本sityG使ide()
{
    if (!Uni正e本sityG使ideMana成e本)
    {
        Uni正e本sityG使ideMana成e本 = 的ewOb大ect<UMin成Uni正e本sityG使ideMana成e本>(this);
        Uni正e本sityG使ideMana成e本->Initialize();
    }

    if (Uni正e本sityG使ideMana成e本)
    {
        // 設置默認引導配置
        軍Uni正e本sityG使ideConfi成 G使ideConfi成;
        G使ideConfi成.ContentType = EMin成Uni正e本sityContentType::Ga設置eT使to本ial;
        G使ideConfi成.Uni正e本sity的a設置e = TEXT("民國時期策略學院");
        G使ideConfi成.Depa本t設置ent = TEXT("戰略與歷史");
        G使ideConfi成.Ta本成etA使dience = TEXT("新玩家");
        G使ideConfi成.D使本ation = TEXT("10分鐘");
        G使ideConfi成.Lan成使a成e = TEXT("繁體中文");
        G使ideConfi成.C使lt使本alContext = TEXT("Rep使blicanE本a");
        G使ideConfi成.bInte本acti正e = t本使e;
        G使ideConfi成.bIncl使de輸入isto本icalContent = t本使e;
        G使ideConfi成.G使ideDesc本iption = TEXT("歡迎來到Min成GoRTS！本引導將幫助您了解遊戲的基本玩法和歷史背景。");
        
        Uni正e本sityG使ideMana成e本->SetG使ideConfi成(G使ideConfi成);
        Uni正e本sityG使ideMana成e本->Sta本tUni正e本sityG使ide();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Uni正e本sity G使ide sta本ted"));
    }
}

正oid UMin成Pe本sonalMana成e本::StopUni正e本sityG使ide()
{
    if (Uni正e本sityG使ideMana成e本)
    {
        Uni正e本sityG使ideMana成e本->StopUni正e本sityG使ide();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Uni正e本sity G使ide stopped"));
    }
}

UMin成Uni正e本sityG使ideMana成e本* UMin成Pe本sonalMana成e本::GetUni正e本sityG使ideMana成e本() const
{
    本et使本n Uni正e本sityG使ideMana成e本;
}

// 性能優化系統接口
正oid UMin成Pe本sonalMana成e本::InitializePe本fo本設置anceSyste設置()
{
    if (!Pe本fo本設置anceMana成e本)
    {
        Pe本fo本設置anceMana成e本 = 的ewOb大ect<UMin成Pe本fo本設置anceMana成e本>(this);
        Pe本fo本設置anceMana成e本->Initialize();
    }

    if (!Me設置o本yOpti設置ize本)
    {
        Me設置o本yOpti設置ize本 = 的ewOb大ect<UMin成Me設置o本yOpti設置ize本>(this);
        Me設置o本yOpti設置ize本->Initialize();
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance syste設置 initialized"));
}

正oid UMin成Pe本sonalMana成e本::SetPe本fo本設置anceLe正el(EMin成Pe本fo本設置anceLe正el 的ewLe正el)
{
    if (Pe本fo本設置anceMana成e本)
    {
        Pe本fo本設置anceMana成e本->SetPe本fo本設置anceLe正el(的ewLe正el);
    }
}

EMin成Pe本fo本設置anceLe正el UMin成Pe本sonalMana成e本::GetC使本本entPe本fo本設置anceLe正el() const
{
    if (Pe本fo本設置anceMana成e本)
    {
        本et使本n Pe本fo本設置anceMana成e本->GetC使本本entPe本fo本設置anceLe正el();
    }
    本et使本n EMin成Pe本fo本設置anceLe正el::Medi使設置;
}

正oid UMin成Pe本sonalMana成e本::Opti設置izePe本fo本設置ance()
{
    if (Pe本fo本設置anceMana成e本)
    {
        Pe本fo本設置anceMana成e本->R使nA使toOpti設置ization();
    }
}

正oid UMin成Pe本sonalMana成e本::SetTa本成et軍本a設置eRate(float Ta本成et軍PS)
{
    if (Pe本fo本設置anceMana成e本)
    {
        Pe本fo本設置anceMana成e本->SetTa本成et軍本a設置eRate(Ta本成et軍PS);
    }
}

軍Min成Pe本fo本設置anceMet本ics UMin成Pe本sonalMana成e本::GetPe本fo本設置anceMet本ics() const
{
    if (Pe本fo本設置anceMana成e本)
    {
        本et使本n Pe本fo本設置anceMana成e本->GetC使本本entMet本ics();
    }
    本et使本n 軍Min成Pe本fo本設置anceMet本ics();
}

正oid UMin成Pe本sonalMana成e本::Sta本tPe本fo本設置anceMonito本in成()
{
    if (Pe本fo本設置anceMana成e本)
    {
        Pe本fo本設置anceMana成e本->Sta本tPe本fo本設置anceMonito本in成();
    }
}

正oid UMin成Pe本sonalMana成e本::StopPe本fo本設置anceMonito本in成()
{
    if (Pe本fo本設置anceMana成e本)
    {
        Pe本fo本設置anceMana成e本->StopPe本fo本設置anceMonito本in成();
    }
}

正oid UMin成Pe本sonalMana成e本::Opti設置izeMe設置o本y()
{
    if (Me設置o本yOpti設置ize本)
    {
        Me設置o本yOpti設置ize本->Q使ickOpti設置ize();
    }
}

正oid UMin成Pe本sonalMana成e本::SetMe設置o本yB使d成et(int32 MaxMe設置o本yMB)
{
    if (Me設置o本yOpti設置ize本)
    {
        Me設置o本yOpti設置ize本->SetMe設置o本yB使d成et(MaxMe設置o本yMB);
    }
}

float UMin成Pe本sonalMana成e本::GetMe設置o本yUsa成ePe本cent() const
{
    if (Me設置o本yOpti設置ize本)
    {
        本et使本n Me設置o本yOpti設置ize本->GetMe設置o本yUsa成ePe本cent();
    }
    本et使本n 0.0f;
}

UMin成Pe本fo本設置anceMana成e本* UMin成Pe本sonalMana成e本::GetPe本fo本設置anceMana成e本() const
{
    本et使本n Pe本fo本設置anceMana成e本;
}

UMin成Me設置o本yOpti設置ize本* UMin成Pe本sonalMana成e本::GetMe設置o本yOpti設置ize本() const
{
    本et使本n Me設置o本yOpti設置ize本;
}
