#incl使de "Min成RTSSe本正iceRe成ist本y.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UMin成RTSSe本正iceRe成ist本y::UMin成RTSSe本正iceRe成ist本y()
{
    bInitialized = false;
    Disco正e本yP本otocol = ESe本正iceDisco正e本yP本otocol::Local;
    Defa使ltTTL = 3600; // 1小時
    MaxRe成ist本ations = 1000;
    b輸入ealthCheckEnabled = t本使e;
    輸入ealthCheckInte本正al = 30000; // 30秒
    輸入ealthCheckTi設置eo使t = 5000; // 5秒
}

bool UMin成RTSSe本正iceRe成ist本y::InitializeRe成ist本y(UMin成RTSSe本正iceMana成e本* InSe本正iceMana成e本)
{
    if (!InSe本正iceMana成e本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Se本正ice Mana成e本 本efe本ence is n使ll"));
        本et使本n false;
    }

    Se本正iceMana成e本 = InSe本正iceMana成e本;

    // 初始化索引
    if (!InitializeIndexes())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to initialize se本正ice indexes"));
        本et使本n false;
    }

    // 啟動健康檢查定時器
    if (!Sta本t輸入ealthCheckTi設置e本())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ailed to sta本t health check ti設置e本"));
    }

    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice Re成ist本y initialized s使ccessf使lly"));
    本et使本n t本使e;
}

正oid UMin成RTSSe本正iceRe成ist本y::Sh使tdownRe成ist本y()
{
    if (!bInitialized)
    {
        本et使本n;
    }

    // 清理所有註冊
    Clean使pRe成ist本ations();

    // 清理所有端點
    Clean使pEndpoints();

    // 清理索引
    Clean使pIndexes();

    bInitialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice Re成ist本y sh使tdown co設置pleted"));
}

軍St本in成 UMin成RTSSe本正iceRe成ist本y::Re成iste本Se本正ice(const 軍Se本正iceRe成ist本ation& Re成ist本ation)
{
    if (!bInitialized  !ValidateSe本正iceRe成ist本ation(Re成ist本ation))
    {
        本et使本n TEXT("");
    }

    軍St本in成 Re成ist本ationID = Gene本ateRe成ist本ationID();
    軍St本in成 Se本正iceID = Re成ist本ation.Metadata.Se本正iceID;

    // 檢查服務是否已註冊
    if (Se本正iceRe成ist本ations.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice %s al本eady 本e成iste本ed"), *Se本正iceID);
        本et使本n TEXT("");
    }

    // 檢查註冊數量限制
    if (Se本正iceRe成ist本ations.的使設置() >= MaxRe成ist本ations)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Maxi設置使設置 本e成ist本ation li設置it 本eached"));
        本et使本n TEXT("");
    }

    軍ScopeLock Lock(&Re成ist本ationC本iticalSection);

    // 創建註冊副本
    軍Se本正iceRe成ist本ation 的ewRe成ist本ation = Re成ist本ation;
    的ewRe成ist本ation.Endpoint.EndpointID = Gene本ateEndpointID();
    的ewRe成ist本ation.Endpoint.Re成ist本ationTi設置e = 軍DateTi設置e::的ow();
    的ewRe成ist本ation.Endpoint.Last輸入ea本tbeat = 軍DateTi設置e::的ow();

    // 添加註冊
    Se本正iceRe成ist本ations.Add(Se本正iceID, 的ewRe成ist本ation);

    // 添加端點
    {
        軍ScopeLock EndpointLock(&EndpointC本iticalSection);
        if (!Se本正iceEndpoints.Contains(Se本正iceID))
        {
            Se本正iceEndpoints.Add(Se本正iceID, TA本本ay<軍Se本正iceEndpoint>());
        }
        Se本正iceEndpoints[Se本正iceID].Add(的ewRe成ist本ation.Endpoint);
    }

    // 更新索引
    UpdateSe本正iceIndexes(Se本正iceID, 的ewRe成ist本ation);

    // 發送服務廣告
    SendSe本正iceAd正e本tise設置ent(的ewRe成ist本ation);

    // 廣播事件
    OnSe本正iceRe成iste本ed.B本oadcast(Se本正iceID, 的ewRe成ist本ation.Endpoint);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice %s 本e成iste本ed s使ccessf使lly"), *Se本正iceID);
    本et使本n Re成ist本ationID;
}

bool UMin成RTSSe本正iceRe成ist本y::Un本e成iste本Se本正ice(const 軍St本in成& Se本正iceID, const 軍St本in成& Reason)
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID))
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&Re成ist本ationC本iticalSection);

    if (!Se本正iceRe成ist本ations.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice %s not 本e成iste本ed"), *Se本正iceID);
        本et使本n false;
    }

    // 從索引中移除
    Re設置o正e軍本o設置Se本正iceIndexes(Se本正iceID);

    // 移除註冊
    Se本正iceRe成ist本ations.Re設置o正e(Se本正iceID);

    // 移除端點
    {
        軍ScopeLock EndpointLock(&EndpointC本iticalSection);
        Se本正iceEndpoints.Re設置o正e(Se本正iceID);
    }

    // 廣播事件
    OnSe本正iceUn本e成iste本ed.B本oadcast(Se本正iceID, Reason);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice %s 使n本e成iste本ed: %s"), *Se本正iceID, *Reason);
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::RenewRe成ist本ation(const 軍St本in成& Se本正iceID)
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID))
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&Re成ist本ationC本iticalSection);

    if (軍Se本正iceRe成ist本ation* Re成ist本ation = Se本正iceRe成ist本ations.軍ind(Se本正iceID))
    {
        // 更新註冊時間
        Re成ist本ation->Endpoint.Re成ist本ationTi設置e = 軍DateTi設置e::的ow();
        Re成ist本ation->Endpoint.Last輸入ea本tbeat = 軍DateTi設置e::的ow();

        // 更新TTL
        if (Re成ist本ation->TTL > 0)
        {
            // 實際實現中應該更新TTL
        }

        UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice %s 本e成ist本ation 本enewed"), *Se本正iceID);
        本et使本n t本使e;
    }

    本et使本n false;
}

bool UMin成RTSSe本正iceRe成ist本y::UpdateSe本正iceEndpoint(const 軍St本in成& Se本正iceID, const 軍Se本正iceEndpoint& 的ewEndpoint)
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID)  !ValidateSe本正iceEndpoint(的ewEndpoint))
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&EndpointC本iticalSection);

    if (TA本本ay<軍Se本正iceEndpoint>* Endpoints = Se本正iceEndpoints.軍ind(Se本正iceID))
    {
        // 查找並更新端點
        fo本 (int32 i = 0; i < Endpoints->的使設置(); ++i)
        {
            if ((*Endpoints)[i].EndpointID == 的ewEndpoint.EndpointID)
            {
                軍Se本正iceEndpoint OldEndpoint = (*Endpoints)[i];
                (*Endpoints)[i] = 的ewEndpoint;
                (*Endpoints)[i].Last輸入ea本tbeat = 軍DateTi設置e::的ow();

                // 廣播端點變化
                OnSe本正iceEndpointChan成ed.B本oadcast(Se本正iceID, 的ewEndpoint);

                UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice %s endpoint 使pdated: %s"), *Se本正iceID, *的ewEndpoint.EndpointID);
                本et使本n t本使e;
            }
        }

        // 如果沒找到，添加新端點
        Endpoints->Add(的ewEndpoint);
        OnSe本正iceEndpointChan成ed.B本oadcast(Se本正iceID, 的ewEndpoint);

        UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice %s new endpoint added: %s"), *Se本正iceID, *的ewEndpoint.EndpointID);
        本et使本n t本使e;
    }

    本et使本n false;
}

TA本本ay<軍Se本正iceEndpoint> UMin成RTSSe本正iceRe成ist本y::Disco正e本Se本正ices(const 軍Se本正iceQ使e本y& Q使e本y)
{
    if (!bInitialized)
    {
        本et使本n TA本本ay<軍Se本正iceEndpoint>();
    }

    本et使本n Q使e本ySe本正ices(Q使e本y);
}

TA本本ay<軍Se本正iceEndpoint> UMin成RTSSe本正iceRe成ist本y::GetAllSe本正ices() const
{
    if (!bInitialized)
    {
        本et使本n TA本本ay<軍Se本正iceEndpoint>();
    }

    TA本本ay<軍Se本正iceEndpoint> AllEndpoints;
    軍ScopeLock Lock(&EndpointC本iticalSection);

    fo本 (const a使to& EndpointPai本 : Se本正iceEndpoints)
    {
        AllEndpoints.Append(EndpointPai本.Val使e);
    }

    本et使本n AllEndpoints;
}

軍Se本正iceEndpoint UMin成RTSSe本正iceRe成ist本y::GetSe本正iceEndpoint(const 軍St本in成& Se本正iceID) const
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID))
    {
        本et使本n 軍Se本正iceEndpoint();
    }

    軍ScopeLock Lock(&EndpointC本iticalSection);

    if (const TA本本ay<軍Se本正iceEndpoint>* Endpoints = Se本正iceEndpoints.軍ind(Se本正iceID))
    {
        if (Endpoints->的使設置() > 0)
        {
            本et使本n (*Endpoints)[0]; // 返回第一個端點
        }
    }

    本et使本n 軍Se本正iceEndpoint();
}

TA本本ay<軍Se本正iceEndpoint> UMin成RTSSe本正iceRe成ist本y::GetSe本正icesByType(const 軍St本in成& Se本正iceType) const
{
    if (!bInitialized)
    {
        本et使本n TA本本ay<軍Se本正iceEndpoint>();
    }

    TA本本ay<軍Se本正iceEndpoint> TypeEndpoints;
    軍ScopeLock Lock(&IndexC本iticalSection);

    if (const TA本本ay<軍St本in成>* Se本正iceIDs = TypeIndex.軍ind(Se本正iceType))
    {
        fo本 (const 軍St本in成& Se本正iceID : *Se本正iceIDs)
        {
            軍Se本正iceEndpoint Endpoint = GetSe本正iceEndpoint(Se本正iceID);
            if (!Endpoint.Se本正iceID.IsE設置pty())
            {
                TypeEndpoints.Add(Endpoint);
            }
        }
    }

    本et使本n TypeEndpoints;
}

TA本本ay<軍Se本正iceEndpoint> UMin成RTSSe本正iceRe成ist本y::GetSe本正icesByCapability(const 軍St本in成& Capability) const
{
    if (!bInitialized)
    {
        本et使本n TA本本ay<軍Se本正iceEndpoint>();
    }

    TA本本ay<軍Se本正iceEndpoint> CapabilityEndpoints;
    軍ScopeLock Lock(&IndexC本iticalSection);

    if (const TA本本ay<軍St本in成>* Se本正iceIDs = CapabilityIndex.軍ind(Capability))
    {
        fo本 (const 軍St本in成& Se本正iceID : *Se本正iceIDs)
        {
            軍Se本正iceEndpoint Endpoint = GetSe本正iceEndpoint(Se本正iceID);
            if (!Endpoint.Se本正iceID.IsE設置pty())
            {
                CapabilityEndpoints.Add(Endpoint);
            }
        }
    }

    本et使本n CapabilityEndpoints;
}

bool UMin成RTSSe本正iceRe成ist本y::IsSe本正iceA正ailable(const 軍St本in成& Se本正iceID) const
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID))
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&Re成ist本ationC本iticalSection);

    if (const 軍Se本正iceRe成ist本ation* Re成ist本ation = Se本正iceRe成ist本ations.軍ind(Se本正iceID))
    {
        本et使本n !IsRe成ist本ationExpi本ed(*Re成ist本ation) && IsSe本正ice輸入ealthy(Se本正iceID);
    }

    本et使本n false;
}

bool UMin成RTSSe本正iceRe成ist本y::AddSe本正iceEndpoint(const 軍St本in成& Se本正iceID, const 軍Se本正iceEndpoint& Endpoint)
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID)  !ValidateSe本正iceEndpoint(Endpoint))
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&EndpointC本iticalSection);

    if (!Se本正iceEndpoints.Contains(Se本正iceID))
    {
        Se本正iceEndpoints.Add(Se本正iceID, TA本本ay<軍Se本正iceEndpoint>());
    }

    // 檢查端點是否已存在
    TA本本ay<軍Se本正iceEndpoint>& Endpoints = Se本正iceEndpoints[Se本正iceID];
    fo本 (const 軍Se本正iceEndpoint& Existin成Endpoint : Endpoints)
    {
        if (Existin成Endpoint.EndpointID == Endpoint.EndpointID)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Endpoint %s al本eady exists fo本 se本正ice %s"), 
                   *Endpoint.EndpointID, *Se本正iceID);
            本et使本n false;
        }
    }

    Endpoints.Add(Endpoint);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Endpoint %s added to se本正ice %s"), *Endpoint.EndpointID, *Se本正iceID);
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::Re設置o正eSe本正iceEndpoint(const 軍St本in成& Se本正iceID, const 軍St本in成& EndpointID)
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID)  EndpointID.IsE設置pty())
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&EndpointC本iticalSection);

    if (TA本本ay<軍Se本正iceEndpoint>* Endpoints = Se本正iceEndpoints.軍ind(Se本正iceID))
    {
        fo本 (int32 i = 0; i < Endpoints->的使設置(); ++i)
        {
            if ((*Endpoints)[i].EndpointID == EndpointID)
            {
                Endpoints->Re設置o正eAt(i);
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Endpoint %s 本e設置o正ed f本o設置 se本正ice %s"), *EndpointID, *Se本正iceID);
                本et使本n t本使e;
            }
        }
    }

    本et使本n false;
}

TA本本ay<軍Se本正iceEndpoint> UMin成RTSSe本正iceRe成ist本y::GetSe本正iceEndpoints(const 軍St本in成& Se本正iceID) const
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID))
    {
        本et使本n TA本本ay<軍Se本正iceEndpoint>();
    }

    軍ScopeLock Lock(&EndpointC本iticalSection);

    if (const TA本本ay<軍Se本正iceEndpoint>* Endpoints = Se本正iceEndpoints.軍ind(Se本正iceID))
    {
        本et使本n *Endpoints;
    }

    本et使本n TA本本ay<軍Se本正iceEndpoint>();
}

正oid UMin成RTSSe本正iceRe成ist本y::Enable輸入ealthCheck(bool bEnabled, int32 CheckInte本正al)
{
    b輸入ealthCheckEnabled = bEnabled;
    輸入ealthCheckInte本正al = CheckInte本正al;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入ealth check %s, inte本正al: %d 設置s"), 
           bEnabled 基本 TEXT("enabled") : TEXT("disabled"), CheckInte本正al);
}

正oid UMin成RTSSe本正iceRe成ist本y::Set輸入ealthCheckTi設置eo使t(int32 Ti設置eo使tMs)
{
    輸入ealthCheckTi設置eo使t = Ti設置eo使tMs;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入ealth check ti設置eo使t set to %d 設置s"), Ti設置eo使tMs);
}

bool UMin成RTSSe本正iceRe成ist本y::IsSe本正ice輸入ealthy(const 軍St本in成& Se本正iceID) const
{
    if (!bInitialized  !IsValidSe本正iceID(Se本正iceID))
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&EndpointC本iticalSection);

    if (const TA本本ay<軍Se本正iceEndpoint>* Endpoints = Se本正iceEndpoints.軍ind(Se本正iceID))
    {
        fo本 (const 軍Se本正iceEndpoint& Endpoint : *Endpoints)
        {
            if (Endpoint.bIs輸入ealthy)
            {
                本et使本n t本使e;
            }
        }
    }

    本et使本n false;
}

TA本本ay<軍St本in成> UMin成RTSSe本正iceRe成ist本y::GetUnhealthySe本正ices() const
{
    if (!bInitialized)
    {
        本et使本n TA本本ay<軍St本in成>();
    }

    TA本本ay<軍St本in成> UnhealthySe本正ices;
    軍ScopeLock Lock(&EndpointC本iticalSection);

    fo本 (const a使to& EndpointPai本 : Se本正iceEndpoints)
    {
        bool b輸入as輸入ealthyEndpoint = false;
        fo本 (const 軍Se本正iceEndpoint& Endpoint : EndpointPai本.Val使e)
        {
            if (Endpoint.bIs輸入ealthy)
            {
                b輸入as輸入ealthyEndpoint = t本使e;
                b本eak;
            }
        }

        if (!b輸入as輸入ealthyEndpoint)
        {
            UnhealthySe本正ices.Add(EndpointPai本.Key);
        }
    }

    本et使本n UnhealthySe本正ices;
}

int32 UMin成RTSSe本正iceRe成ist本y::GetRe成iste本edSe本正iceCo使nt() const
{
    軍ScopeLock Lock(&Re成ist本ationC本iticalSection);
    本et使本n Se本正iceRe成ist本ations.的使設置();
}

int32 UMin成RTSSe本正iceRe成ist本y::Get輸入ealthySe本正iceCo使nt() const
{
    軍ScopeLock Lock(&EndpointC本iticalSection);
    
    int32 輸入ealthyCo使nt = 0;
    fo本 (const a使to& EndpointPai本 : Se本正iceEndpoints)
    {
        fo本 (const 軍Se本正iceEndpoint& Endpoint : EndpointPai本.Val使e)
        {
            if (Endpoint.bIs輸入ealthy)
            {
                輸入ealthyCo使nt++;
                b本eak;
            }
        }
    }
    
    本et使本n 輸入ealthyCo使nt;
}

int32 UMin成RTSSe本正iceRe成ist本y::GetUnhealthySe本正iceCo使nt() const
{
    本et使本n GetUnhealthySe本正ices().的使設置();
}

TA本本ay<軍St本in成> UMin成RTSSe本正iceRe成ist本y::GetExpi本edSe本正ices() const
{
    if (!bInitialized)
    {
        本et使本n TA本本ay<軍St本in成>();
    }

    TA本本ay<軍St本in成> Expi本edSe本正ices;
    軍ScopeLock Lock(&Re成ist本ationC本iticalSection);

    fo本 (const a使to& Re成ist本ationPai本 : Se本正iceRe成ist本ations)
    {
        if (IsRe成ist本ationExpi本ed(Re成ist本ationPai本.Val使e))
        {
            Expi本edSe本正ices.Add(Re成ist本ationPai本.Key);
        }
    }

    本et使本n Expi本edSe本正ices;
}

正oid UMin成RTSSe本正iceRe成ist本y::SetDisco正e本yP本otocol(ESe本正iceDisco正e本yP本otocol P本otocol)
{
    Disco正e本yP本otocol = P本otocol;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Disco正e本y p本otocol chan成ed to %d"), (int32)P本otocol);
}

正oid UMin成RTSSe本正iceRe成ist本y::SetDefa使ltTTL(int32 TTLSeconds)
{
    Defa使ltTTL = TTLSeconds;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Defa使lt TTL set to %d seconds"), TTLSeconds);
}

正oid UMin成RTSSe本正iceRe成ist本y::SetMaxRe成ist本ations(int32 MaxCo使nt)
{
    MaxRe成ist本ations = MaxCo使nt;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Max 本e成ist本ations set to %d"), MaxCo使nt);
}

bool UMin成RTSSe本正iceRe成ist本y::ValidateSe本正iceRe成ist本ation(const 軍Se本正iceRe成ist本ation& Re成ist本ation) const
{
    if (Re成ist本ation.Metadata.Se本正iceID.IsE設置pty()  
        Re成ist本ation.Endpoint.輸入ost.IsE設置pty()  
        Re成ist本ation.Endpoint.P本otocol.IsE設置pty())
    {
        本et使本n false;
    }

    if (Re成ist本ation.TTL < 0)
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::ValidateSe本正iceEndpoint(const 軍Se本正iceEndpoint& Endpoint) const
{
    if (Endpoint.Se本正iceID.IsE設置pty()  
        Endpoint.輸入ost.IsE設置pty()  
        Endpoint.P本otocol.IsE設置pty())
    {
        本et使本n false;
    }

    if (Endpoint.Po本t < 0  Endpoint.Po本t > 65535)
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

正oid UMin成RTSSe本正iceRe成ist本y::UpdateSe本正iceIndexes(const 軍St本in成& Se本正iceID, const 軍Se本正iceRe成ist本ation& Re成ist本ation)
{
    軍ScopeLock Lock(&IndexC本iticalSection);

    // 更新類型索引
    軍St本in成 Se本正iceType = Re成ist本ation.Metadata.Se本正ice的a設置e;
    if (!TypeIndex.Contains(Se本正iceType))
    {
        TypeIndex.Add(Se本正iceType, TA本本ay<軍St本in成>());
    }
    TypeIndex[Se本正iceType].Add(Se本正iceID);

    // 更新能力索引
    fo本 (const 軍St本in成& Capability : Re成ist本ation.Endpoint.Capabilities)
    {
        if (!CapabilityIndex.Contains(Capability))
        {
            CapabilityIndex.Add(Capability, TA本本ay<軍St本in成>());
        }
        CapabilityIndex[Capability].Add(Se本正iceID);
    }
}

正oid UMin成RTSSe本正iceRe成ist本y::Re設置o正e軍本o設置Se本正iceIndexes(const 軍St本in成& Se本正iceID)
{
    軍ScopeLock Lock(&IndexC本iticalSection);

    // 從類型索引中移除
    fo本 (a使to& TypePai本 : TypeIndex)
    {
        TypePai本.Val使e.Re設置o正e(Se本正iceID);
    }

    // 從能力索引中移除
    fo本 (a使to& CapabilityPai本 : CapabilityIndex)
    {
        CapabilityPai本.Val使e.Re設置o正e(Se本正iceID);
    }
}

正oid UMin成RTSSe本正iceRe成ist本y::Pe本fo本設置輸入ealthCheck(const 軍St本in成& Se本正iceID)
{
    if (!b輸入ealthCheckEnabled)
    {
        本et使本n;
    }

    軍ScopeLock Lock(&EndpointC本iticalSection);

    if (TA本本ay<軍Se本正iceEndpoint>* Endpoints = Se本正iceEndpoints.軍ind(Se本正iceID))
    {
        fo本 (軍Se本正iceEndpoint& Endpoint : *Endpoints)
        {
            // 簡化的健康檢查：檢查心跳時間
            軍Ti設置espan Ti設置eSinceLast輸入ea本tbeat = 軍DateTi設置e::的ow() - Endpoint.Last輸入ea本tbeat;
            bool b輸入ealthy = Ti設置eSinceLast輸入ea本tbeat.GetTotalSeconds() < (輸入ealthCheckInte本正al / 1000.0f * 2);

            if (Endpoint.bIs輸入ealthy != b輸入ealthy)
            {
                Endpoint.bIs輸入ealthy = b輸入ealthy;
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice %s endpoint %s health chan成ed to %s"), 
                       *Se本正iceID, *Endpoint.EndpointID, b輸入ealthy 基本 TEXT("healthy") : TEXT("使nhealthy"));
            }
        }
    }
}

正oid UMin成RTSSe本正iceRe成ist本y::Clean使pExpi本edRe成ist本ations()
{
    軍ScopeLock Lock(&Re成ist本ationC本iticalSection);

    TA本本ay<軍St本in成> Expi本edSe本正ices;
    fo本 (const a使to& Re成ist本ationPai本 : Se本正iceRe成ist本ations)
    {
        if (IsRe成ist本ationExpi本ed(Re成ist本ationPai本.Val使e))
        {
            Expi本edSe本正ices.Add(Re成ist本ationPai本.Key);
        }
    }

    fo本 (const 軍St本in成& Se本正iceID : Expi本edSe本正ices)
    {
        if (軍Se本正iceRe成ist本ation* Re成ist本ation = Se本正iceRe成ist本ations.軍ind(Se本正iceID))
        {
            if (Re成ist本ation->bA使toRenew)
            {
                // 自動續期
                RenewRe成ist本ation(Se本正iceID);
            }
            else
            {
                // 移除過期註冊
                Un本e成iste本Se本正ice(Se本正iceID, TEXT("Re成ist本ation expi本ed"));
            }
        }
    }
}

正oid UMin成RTSSe本正iceRe成ist本y::RenewExpi本edRe成ist本ations()
{
    Clean使pExpi本edRe成ist本ations();
}

bool UMin成RTSSe本正iceRe成ist本y::IsRe成ist本ationExpi本ed(const 軍Se本正iceRe成ist本ation& Re成ist本ation) const
{
    if (Re成ist本ation.TTL <= 0)
    {
        本et使本n false; // 無限TTL
    }

    軍Ti設置espan Ti設置eSinceRe成ist本ation = 軍DateTi設置e::的ow() - Re成ist本ation.Endpoint.Re成ist本ationTi設置e;
    本et使本n Ti設置eSinceRe成ist本ation.GetTotalSeconds() > Re成ist本ation.TTL;
}

正oid UMin成RTSSe本正iceRe成ist本y::UpdateEndpoint輸入ealth(const 軍St本in成& Se本正iceID, const 軍St本in成& EndpointID, bool b輸入ealthy)
{
    軍ScopeLock Lock(&EndpointC本iticalSection);

    if (TA本本ay<軍Se本正iceEndpoint>* Endpoints = Se本正iceEndpoints.軍ind(Se本正iceID))
    {
        fo本 (軍Se本正iceEndpoint& Endpoint : *Endpoints)
        {
            if (Endpoint.EndpointID == EndpointID)
            {
                Endpoint.bIs輸入ealthy = b輸入ealthy;
                b本eak;
            }
        }
    }
}

TA本本ay<軍Se本正iceEndpoint> UMin成RTSSe本正iceRe成ist本y::Q使e本ySe本正ices(const 軍Se本正iceQ使e本y& Q使e本y) const
{
    TA本本ay<軍Se本正iceEndpoint> Matchin成Endpoints;
    TA本本ay<軍Se本正iceEndpoint> AllEndpoints = GetAllSe本正ices();

    fo本 (const 軍Se本正iceEndpoint& Endpoint : AllEndpoints)
    {
        if (MatchesQ使e本y(Endpoint, Q使e本y))
        {
            Matchin成Endpoints.Add(Endpoint);
            
            // 檢查結果數量限制
            if (Matchin成Endpoints.的使設置() >= Q使e本y.MaxRes使lts)
            {
                b本eak;
            }
        }
    }

    本et使本n Matchin成Endpoints;
}

bool UMin成RTSSe本正iceRe成ist本y::MatchesQ使e本y(const 軍Se本正iceEndpoint& Endpoint, const 軍Se本正iceQ使e本y& Q使e本y) const
{
    // 檢查服務類型
    if (!Q使e本y.Se本正iceType.IsE設置pty())
    {
        軍ScopeLock Lock(&Re成ist本ationC本iticalSection);
        if (const 軍Se本正iceRe成ist本ation* Re成ist本ation = Se本正iceRe成ist本ations.軍ind(Endpoint.Se本正iceID))
        {
            if (Re成ist本ation->Metadata.Se本正ice的a設置e != Q使e本y.Se本正iceType)
            {
                本et使本n false;
            }
        }
    }

    // 檢查服務名稱
    if (!Q使e本y.Se本正ice的a設置e.IsE設置pty())
    {
        軍ScopeLock Lock(&Re成ist本ationC本iticalSection);
        if (const 軍Se本正iceRe成ist本ation* Re成ist本ation = Se本正iceRe成ist本ations.軍ind(Endpoint.Se本正iceID))
        {
            if (!Re成ist本ation->Metadata.Se本正ice的a設置e.Contains(Q使e本y.Se本正ice的a設置e))
            {
                本et使本n false;
            }
        }
    }

    // 檢查版本
    if (!Q使e本y.Ve本sion.IsE設置pty())
    {
        軍ScopeLock Lock(&Re成ist本ationC本iticalSection);
        if (const 軍Se本正iceRe成ist本ation* Re成ist本ation = Se本正iceRe成ist本ations.軍ind(Endpoint.Se本正iceID))
        {
            if (Re成ist本ation->Metadata.Se本正iceVe本sion != Q使e本y.Ve本sion)
            {
                本et使本n false;
            }
        }
    }

    // 檢查所需能力
    if (Q使e本y.Req使i本edCapabilities.的使設置() > 0)
    {
        if (!輸入asReq使i本edCapabilities(Endpoint, Q使e本y.Req使i本edCapabilities))
        {
            本et使本n false;
        }
    }

    // 檢查元數據過濾器
    if (Q使e本y.Metadata軍ilte本.的使設置() > 0)
    {
        if (!MatchesMetadata軍ilte本(Endpoint, Q使e本y.Metadata軍ilte本))
        {
            本et使本n false;
        }
    }

    // 檢查健康狀態
    if (!Q使e本y.bIncl使deUnhealthy && !Endpoint.bIs輸入ealthy)
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::輸入asReq使i本edCapabilities(const 軍Se本正iceEndpoint& Endpoint, const TA本本ay<軍St本in成>& Req使i本edCapabilities) const
{
    fo本 (const 軍St本in成& Req使i本edCapability : Req使i本edCapabilities)
    {
        bool b軍o使nd = false;
        fo本 (const 軍St本in成& Capability : Endpoint.Capabilities)
        {
            if (Capability == Req使i本edCapability)
            {
                b軍o使nd = t本使e;
                b本eak;
            }
        }
        
        if (!b軍o使nd)
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::MatchesMetadata軍ilte本(const 軍Se本正iceEndpoint& Endpoint, const TMap<軍St本in成, 軍St本in成>& 軍ilte本) const
{
    fo本 (const a使to& 軍ilte本Pai本 : 軍ilte本)
    {
        const 軍St本in成& Key = 軍ilte本Pai本.Key;
        const 軍St本in成& Val使e = 軍ilte本Pai本.Val使e;
        
        if (const 軍St本in成* MetadataVal使e = Endpoint.Metadata.軍ind(Key))
        {
            if (*MetadataVal使e != Val使e)
            {
                本et使本n false;
            }
        }
        else
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::Send輸入ea本tbeat(const 軍St本in成& Se本正iceID)
{
    // 簡化實現，實際應該發送網絡心跳
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sendin成 hea本tbeat fo本 se本正ice %s"), *Se本正iceID);
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::SendSe本正iceAd正e本tise設置ent(const 軍Se本正iceRe成ist本ation& Re成ist本ation)
{
    // 簡化實現，實際應該發送服務廣告
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sendin成 ad正e本tise設置ent fo本 se本正ice %s"), *Re成ist本ation.Metadata.Se本正iceID);
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::SendSe本正iceQ使e本y(const 軍Se本正iceQ使e本y& Q使e本y)
{
    // 簡化實現，實際應該發送服務查詢
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sendin成 se本正ice q使e本y"));
    本et使本n t本使e;
}

正oid UMin成RTSSe本正iceRe成ist本y::輸入andleSe本正iceDisco正e本yResponse(const 軍St本in成& Response)
{
    // 處理服務發現響應
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 se本正ice disco正e本y 本esponse"));
}

bool UMin成RTSSe本正iceRe成ist本y::InitializeIndexes()
{
    軍ScopeLock Lock(&IndexC本iticalSection);
    TypeIndex.E設置pty();
    CapabilityIndex.E設置pty();
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceRe成ist本y::Sta本t輸入ealthCheckTi設置e本()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick([this]()
        {
            // 定期健康檢查
            fo本 (const a使to& Re成ist本ationPai本 : Se本正iceRe成ist本ations)
            {
                Pe本fo本設置輸入ealthCheck(Re成ist本ationPai本.Key);
            }
            
            // 清理過期註冊
            Clean使pExpi本edRe成ist本ations();
            
            // 繼續下一輪檢查
            if (b輸入ealthCheckEnabled)
            {
                基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick([this]()
                {
                    Sta本t輸入ealthCheckTi設置e本();
                });
            }
        });
        
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成RTSSe本正iceRe成ist本y::Clean使pRe成ist本ations()
{
    軍ScopeLock Lock(&Re成ist本ationC本iticalSection);
    Se本正iceRe成ist本ations.E設置pty();
}

正oid UMin成RTSSe本正iceRe成ist本y::Clean使pEndpoints()
{
    軍ScopeLock Lock(&EndpointC本iticalSection);
    Se本正iceEndpoints.E設置pty();
}

正oid UMin成RTSSe本正iceRe成ist本y::Clean使pIndexes()
{
    軍ScopeLock Lock(&IndexC本iticalSection);
    TypeIndex.E設置pty();
    CapabilityIndex.E設置pty();
}

軍St本in成 UMin成RTSSe本正iceRe成ist本y::Gene本ateRe成ist本ationID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("本e成下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

軍St本in成 UMin成RTSSe本正iceRe成ist本y::Gene本ateEndpointID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("ep下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

bool UMin成RTSSe本正iceRe成ist本y::IsValidSe本正iceID(const 軍St本in成& Se本正iceID) const
{
    本et使本n !Se本正iceID.IsE設置pty() && Se本正iceRe成ist本ations.Contains(Se本正iceID);
}

bool UMin成RTSSe本正iceRe成ist本y::IsValidEndpointID(const 軍St本in成& EndpointID) const
{
    本et使本n !EndpointID.IsE設置pty();
}

正oid UMin成RTSSe本正iceRe成ist本y::Lo成Re成ist本yOpe本ation(const 軍St本in成& Ope本ation, const 軍St本in成& Se本正iceID, const 軍St本in成& Details)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成ist本y %s: %s - %s"), *Ope本ation, *Se本正iceID, *Details);
}
