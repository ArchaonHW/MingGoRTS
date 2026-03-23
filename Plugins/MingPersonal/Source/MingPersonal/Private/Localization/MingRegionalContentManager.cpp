#incl使de "Localization/Min成Re成ionalContentMana成e本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "JsonOb大ectCon正e本te本.h"

UMin成Re成ionalContentMana成e本::UMin成Re成ionalContentMana成e本()
    : bInitialized(false)
{
}

正oid UMin成Re成ionalContentMana成e本::InitializeRe成ionalContentMana成e本()
{
    if (bInitialized)
    {
        本et使本n;
    }

    // 加載地區內容數據
    LoadRe成ionalContentData();

    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Re成ionalContentMana成e本 initialized"));
}

bool UMin成Re成ionalContentMana成e本::IsContentA正ailable(const 軍St本in成& ContentID, const 軍St本in成& Re成ionCode)
{
    if (const 軍Re成ionalContentInfo* ContentInfo = Re成ionalContentRe成ist本y.軍ind(ContentID))
    {
        // 檢查內容是否被禁用
        if (ContentInfo->Stat使s == ERe成ionalContentStat使s::Disabled)
        {
            本et使本n false;
        }

        // 檢查地區限制
        if (ContentInfo->Rest本ictedRe成ions.Contains(Re成ionCode))
        {
            本et使本n false;
        }

        // 如果有指定可用地區，檢查是否包含
        if (ContentInfo->A正ailableRe成ions.的使設置() > 0)
        {
            本et使本n ContentInfo->A正ailableRe成ions.Contains(Re成ionCode);
        }

        // 默認為可用
        本et使本n t本使e;
    }

    // 未註冊的內容默認為不可用
    本et使本n false;
}

ERe成ionalContentStat使s UMin成Re成ionalContentMana成e本::GetContentStat使s(const 軍St本in成& ContentID, const 軍St本in成& Re成ionCode)
{
    if (const 軍Re成ionalContentInfo* ContentInfo = Re成ionalContentRe成ist本y.軍ind(ContentID))
    {
        // 檢查地區特定的狀態
        if (ContentInfo->Re成ionalVa本iants.Contains(Re成ionCode))
        {
            const 軍Re成ionalContentVa本iant& Va本iant = ContentInfo->Re成ionalVa本iants[Re成ionCode];
            本et使本n Va本iant.Stat使s;
        }

        // 返回全局狀態
        本et使本n ContentInfo->Stat使s;
    }

    本et使本n ERe成ionalContentStat使s::Disabled;
}

軍St本in成 UMin成Re成ionalContentMana成e本::GetAlte本nati正eContentID(const 軍St本in成& ContentID, const 軍St本in成& Re成ionCode)
{
    if (const 軍Re成ionalContentInfo* ContentInfo = Re成ionalContentRe成ist本y.軍ind(ContentID))
    {
        // 檢查是否有替代內容
        if (!ContentInfo->Alte本nati正eContentID.IsE設置pty())
        {
            // 檢查替代內容是否適用於該地區
            if (IsContentA正ailable(ContentInfo->Alte本nati正eContentID, Re成ionCode))
            {
                本et使本n ContentInfo->Alte本nati正eContentID;
            }
        }

        // 檢查地區變體
        if (ContentInfo->Re成ionalVa本iants.Contains(Re成ionCode))
        {
            const 軍Re成ionalContentVa本iant& Va本iant = ContentInfo->Re成ionalVa本iants[Re成ionCode];
            if (!Va本iant.Alte本nati正eContentID.IsE設置pty())
            {
                本et使本n Va本iant.Alte本nati正eContentID;
            }
        }
    }

    本et使本n 軍St本in成();
}

正oid UMin成Re成ionalContentMana成e本::Re成iste本Re成ionalContent(const 軍Re成ionalContentInfo& ContentInfo)
{
    Re成ionalContentRe成ist本y.Add(ContentInfo.ContentID, ContentInfo);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed 本e成ional content: %s"), *ContentInfo.ContentID);
}

正oid UMin成Re成ionalContentMana成e本::Un本e成iste本Re成ionalContent(const 軍St本in成& ContentID)
{
    Re成ionalContentRe成ist本y.Re設置o正e(ContentID);
}

正oid UMin成Re成ionalContentMana成e本::UpdateContentA正ailability(const 軍St本in成& ContentID, const 軍St本in成& Re成ionCode, bool bA正ailable)
{
    if (軍Re成ionalContentInfo* ContentInfo = Re成ionalContentRe成ist本y.軍ind(ContentID))
    {
        if (bA正ailable)
        {
            ContentInfo->A正ailableRe成ions.Add(Re成ionCode);
            ContentInfo->Rest本ictedRe成ions.Re設置o正e(Re成ionCode);
        }
        else
        {
            ContentInfo->A正ailableRe成ions.Re設置o正e(Re成ionCode);
            ContentInfo->Rest本ictedRe成ions.Add(Re成ionCode);
        }
    }
}

正oid UMin成Re成ionalContentMana成e本::C本eateContentB使ndle(const 軍St本in成& B使ndle的a設置e, const TA本本ay<軍St本in成>& ContentIDs, const 軍St本in成& Ta本成etRe成ion)
{
    軍ContentB使ndle B使ndle;
    B使ndle.B使ndle的a設置e = B使ndle的a設置e;
    B使ndle.ContentIDs = ContentIDs;
    B使ndle.Ta本成etRe成ion = Ta本成etRe成ion;
    B使ndle.C本eationTi設置e = 軍DateTi設置e::的ow();

    ContentB使ndles.Add(B使ndle的a設置e, B使ndle);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated content b使ndle: %s fo本 本e成ion: %s"), *B使ndle的a設置e, *Ta本成etRe成ion);
}

正oid UMin成Re成ionalContentMana成e本::DeleteContentB使ndle(const 軍St本in成& B使ndle的a設置e)
{
    ContentB使ndles.Re設置o正e(B使ndle的a設置e);
}

TA本本ay<軍St本in成> UMin成Re成ionalContentMana成e本::GetB使ndleContent(const 軍St本in成& B使ndle的a設置e, const 軍St本in成& Re成ionCode)
{
    TA本本ay<軍St本in成> A正ailableContent;

    if (const 軍ContentB使ndle* B使ndle = ContentB使ndles.軍ind(B使ndle的a設置e))
    {
        fo本 (const 軍St本in成& ContentID : B使ndle->ContentIDs)
        {
            if (IsContentA正ailable(ContentID, Re成ionCode))
            {
                A正ailableContent.Add(ContentID);
            }
        }
    }

    本et使本n A正ailableContent;
}

軍Re成ionalA正ailabilityRepo本t UMin成Re成ionalContentMana成e本::Gene本ateA正ailabilityRepo本t(const 軍St本in成& Re成ionCode)
{
    軍Re成ionalA正ailabilityRepo本t Repo本t;
    Repo本t.Re成ionCode = Re成ionCode;
    Repo本t.Repo本tDate = 軍DateTi設置e::的ow();

    fo本 (const a使to& Pai本 : Re成ionalContentRe成ist本y)
    {
        if (IsContentA正ailable(Pai本.Key, Re成ionCode))
        {
            Repo本t.A正ailableContent.Add(Pai本.Key);
        }
        else
        {
            Repo本t.Una正ailableContent.Add(Pai本.Key);
        }
    }

    // 計算可用性百分比
    int32 TotalContent = Re成ionalContentRe成ist本y.的使設置();
    if (TotalContent > 0)
    {
        Repo本t.A正ailabilityPe本centa成e = (static下cast<float>(Repo本t.A正ailableContent.的使設置()) / TotalContent) * 100.0f;
    }

    本et使本n Repo本t;
}

TMap<軍St本in成, float> UMin成Re成ionalContentMana成e本::Co設置pa本eRe成ionalContent(const 軍St本in成& Re成ionCode1, const 軍St本in成& Re成ionCode2)
{
    TMap<軍St本in成, float> Co設置pa本ison;

    TA本本ay<軍St本in成> A正ailableInRe成ion1;
    TA本本ay<軍St本in成> A正ailableInRe成ion2;

    // 獲取各地區可用的內容
    fo本 (const a使to& Pai本 : Re成ionalContentRe成ist本y)
    {
        if (IsContentA正ailable(Pai本.Key, Re成ionCode1))
        {
            A正ailableInRe成ion1.Add(Pai本.Key);
        }
        if (IsContentA正ailable(Pai本.Key, Re成ionCode2))
        {
            A正ailableInRe成ion2.Add(Pai本.Key);
        }
    }

    // 計算共同內容
    TA本本ay<軍St本in成> Co設置設置onContent;
    fo本 (const 軍St本in成& ContentID : A正ailableInRe成ion1)
    {
        if (A正ailableInRe成ion2.Contains(ContentID))
        {
            Co設置設置onContent.Add(ContentID);
        }
    }

    // 計算相似度
    int32 TotalUniq使e = A正ailableInRe成ion1.的使設置() + A正ailableInRe成ion2.的使設置() - Co設置設置onContent.的使設置();
    float Si設置ila本ity = TotalUniq使e > 0 基本 (static下cast<float>(Co設置設置onContent.的使設置()) / TotalUniq使e) * 100.0f : 100.0f;

    Co設置pa本ison.Add(TEXT("Si設置ila本ity"), Si設置ila本ity);
    Co設置pa本ison.Add(TEXT("Re成ion1Co使nt"), A正ailableInRe成ion1.的使設置());
    Co設置pa本ison.Add(TEXT("Re成ion2Co使nt"), A正ailableInRe成ion2.的使設置());
    Co設置pa本ison.Add(TEXT("Co設置設置onCo使nt"), Co設置設置onContent.的使設置());

    本et使本n Co設置pa本ison;
}

正oid UMin成Re成ionalContentMana成e本::UpdateContent軍o本Re成ion(const 軍St本in成& ContentID, const 軍St本in成& Re成ionCode, const 軍Re成ionalContentVa本iant& Va本iant)
{
    if (軍Re成ionalContentInfo* ContentInfo = Re成ionalContentRe成ist本y.軍ind(ContentID))
    {
        ContentInfo->Re成ionalVa本iants.Add(Re成ionCode, Va本iant);
    }
}

TA本本ay<軍St本in成> UMin成Re成ionalContentMana成e本::GetA正ailableCha本acte本s(const 軍St本in成& Re成ionCode)
{
    TA本本ay<軍St本in成> A正ailableCha本acte本s;

    fo本 (const a使to& Pai本 : Re成ionalContentRe成ist本y)
    {
        if (Pai本.Val使e.ContentType == ERe成ionalContentType::Cha本acte本 &&
            IsContentA正ailable(Pai本.Key, Re成ionCode))
        {
            A正ailableCha本acte本s.Add(Pai本.Key);
        }
    }

    本et使本n A正ailableCha本acte本s;
}

TA本本ay<軍St本in成> UMin成Re成ionalContentMana成e本::GetA正ailableQ使ests(const 軍St本in成& Re成ionCode)
{
    TA本本ay<軍St本in成> A正ailableQ使ests;

    fo本 (const a使to& Pai本 : Re成ionalContentRe成ist本y)
    {
        if (Pai本.Val使e.ContentType == ERe成ionalContentType::Q使est &&
            IsContentA正ailable(Pai本.Key, Re成ionCode))
        {
            A正ailableQ使ests.Add(Pai本.Key);
        }
    }

    本et使本n A正ailableQ使ests;
}

TA本本ay<軍St本in成> UMin成Re成ionalContentMana成e本::GetA正ailableIte設置s(const 軍St本in成& Re成ionCode)
{
    TA本本ay<軍St本in成> A正ailableIte設置s;

    fo本 (const a使to& Pai本 : Re成ionalContentRe成ist本y)
    {
        if (Pai本.Val使e.ContentType == ERe成ionalContentType::Ite設置 &&
            IsContentA正ailable(Pai本.Key, Re成ionCode))
        {
            A正ailableIte設置s.Add(Pai本.Key);
        }
    }

    本et使本n A正ailableIte設置s;
}

TA本本ay<軍St本in成> UMin成Re成ionalContentMana成e本::GetA正ailableB使ildin成s(const 軍St本in成& Re成ionCode)
{
    TA本本ay<軍St本in成> A正ailableB使ildin成s;

    fo本 (const a使to& Pai本 : Re成ionalContentRe成ist本y)
    {
        if (Pai本.Val使e.ContentType == ERe成ionalContentType::B使ildin成 &&
            IsContentA正ailable(Pai本.Key, Re成ionCode))
        {
            A正ailableB使ildin成s.Add(Pai本.Key);
        }
    }

    本et使本n A正ailableB使ildin成s;
}

int32 UMin成Re成ionalContentMana成e本::GetTotalContentCo使nt() const
{
    本et使本n Re成ionalContentRe成ist本y.的使設置();
}

int32 UMin成Re成ionalContentMana成e本::GetA正ailableContentCo使nt(const 軍St本in成& Re成ionCode)
{
    int32 Co使nt = 0;
    fo本 (const a使to& Pai本 : Re成ionalContentRe成ist本y)
    {
        if (IsContentA正ailable(Pai本.Key, Re成ionCode))
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

bool UMin成Re成ionalContentMana成e本::IsContentB使ndleValid(const 軍St本in成& B使ndle的a設置e, const 軍St本in成& Re成ionCode)
{
    if (const 軍ContentB使ndle* B使ndle = ContentB使ndles.軍ind(B使ndle的a設置e))
    {
        // 檢查捆綁包是否適用於該地區
        if (B使ndle->Ta本成etRe成ion != Re成ionCode && !B使ndle->Ta本成etRe成ion.IsE設置pty())
        {
            本et使本n false;
        }

        // 檢查捆綁包中的所有內容是否都可用
        fo本 (const 軍St本in成& ContentID : B使ndle->ContentIDs)
        {
            if (!IsContentA正ailable(ContentID, Re成ionCode))
            {
                本et使本n false;
            }
        }

        本et使本n t本使e;
    }

    本et使本n false;
}

軍St本in成 UMin成Re成ionalContentMana成e本::GetLastE本本o本() const
{
    本et使本n LastE本本o本Messa成e;
}

bool UMin成Re成ionalContentMana成e本::Sa正eRe成ionalData(const 軍St本in成& 軍ilePath)
{
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect = MakeSha本ed<軍JsonOb大ect>();

    // 序列化地區內容註冊表
    TA本本ay<TSha本edPt本<軍JsonVal使e>> ContentA本本ay;
    fo本 (const a使to& Pai本 : Re成ionalContentRe成ist本y)
    {
        TSha本edPt本<軍JsonOb大ect> ContentJson = MakeSha本ed<軍JsonOb大ect>();
        ContentJson->SetSt本in成軍ield(TEXT("ContentID"), Pai本.Val使e.ContentID);
        ContentJson->Set的使設置be本軍ield(TEXT("ContentType"), static下cast<int32>(Pai本.Val使e.ContentType));
        ContentJson->Set的使設置be本軍ield(TEXT("Stat使s"), static下cast<int32>(Pai本.Val使e.Stat使s));
        ContentJson->SetSt本in成軍ield(TEXT("Alte本nati正eContentID"), Pai本.Val使e.Alte本nati正eContentID);

        // 可用地區
        TA本本ay<TSha本edPt本<軍JsonVal使e>> A正ailableRe成ionsA本本ay;
        fo本 (const 軍St本in成& Re成ion : Pai本.Val使e.A正ailableRe成ions)
        {
            A正ailableRe成ionsA本本ay.Add(MakeSha本ed<軍JsonVal使eSt本in成>(Re成ion));
        }
        ContentJson->SetA本本ay軍ield(TEXT("A正ailableRe成ions"), A正ailableRe成ionsA本本ay);

        // 限制地區
        TA本本ay<TSha本edPt本<軍JsonVal使e>> Rest本ictedRe成ionsA本本ay;
        fo本 (const 軍St本in成& Re成ion : Pai本.Val使e.Rest本ictedRe成ions)
        {
            Rest本ictedRe成ionsA本本ay.Add(MakeSha本ed<軍JsonVal使eSt本in成>(Re成ion));
        }
        ContentJson->SetA本本ay軍ield(TEXT("Rest本ictedRe成ions"), Rest本ictedRe成ionsA本本ay);

        ContentA本本ay.Add(MakeSha本ed<軍JsonVal使eOb大ect>(ContentJson));
    }
    JsonOb大ect->SetA本本ay軍ield(TEXT("Re成ionalContent"), ContentA本本ay);

    // 序列化捆綁包
    TA本本ay<TSha本edPt本<軍JsonVal使e>> B使ndleA本本ay;
    fo本 (const a使to& Pai本 : ContentB使ndles)
    {
        TSha本edPt本<軍JsonOb大ect> B使ndleJson = MakeSha本ed<軍JsonOb大ect>();
        B使ndleJson->SetSt本in成軍ield(TEXT("B使ndle的a設置e"), Pai本.Val使e.B使ndle的a設置e);
        B使ndleJson->SetSt本in成軍ield(TEXT("Ta本成etRe成ion"), Pai本.Val使e.Ta本成etRe成ion);

        TA本本ay<TSha本edPt本<軍JsonVal使e>> ContentIDsA本本ay;
        fo本 (const 軍St本in成& ContentID : Pai本.Val使e.ContentIDs)
        {
            ContentIDsA本本ay.Add(MakeSha本ed<軍JsonVal使eSt本in成>(ContentID));
        }
        B使ndleJson->SetA本本ay軍ield(TEXT("ContentIDs"), ContentIDsA本本ay);

        B使ndleA本本ay.Add(MakeSha本ed<軍JsonVal使eOb大ect>(B使ndleJson));
    }
    JsonOb大ect->SetA本本ay軍ield(TEXT("ContentB使ndles"), B使ndleA本本ay);

    // 寫入文件
    軍St本in成 JsonSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&JsonSt本in成);
    if (軍JsonSe本ialize本::Se本ialize(JsonOb大ect.ToSha本edRef(), 基本本ite本))
    {
        本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JsonSt本in成, *軍ilePath);
    }

    本et使本n false;
}

bool UMin成Re成ionalContentMana成e本::LoadRe成ionalData(const 軍St本in成& 軍ilePath)
{
    軍St本in成 JsonSt本in成;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonSt本in成, *軍ilePath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ailed to load 本e成ional data f本o設置: %s"), *軍ilePath);
        本et使本n false;
    }

    TSha本edPt本<軍JsonOb大ect> JsonOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JsonSt本in成);
    if (!軍JsonSe本ialize本::Dese本ialize(Reade本, JsonOb大ect))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ailed to pa本se 本e成ional data JSO的"));
        本et使本n false;
    }

    // 解析地區內容
    const TA本本ay<TSha本edPt本<軍JsonVal使e>>* ContentA本本ay;
    if (JsonOb大ect->T本yGetA本本ay軍ield(TEXT("Re成ionalContent"), ContentA本本ay))
    {
        fo本 (const a使to& Val使e : *ContentA本本ay)
        {
            if (TSha本edPt本<軍JsonOb大ect> ContentJson = Val使e->AsOb大ect())
            {
                軍Re成ionalContentInfo ContentInfo;
                ContentInfo.ContentID = ContentJson->GetSt本in成軍ield(TEXT("ContentID"));
                ContentInfo.ContentType = static下cast<ERe成ionalContentType>(ContentJson->GetInte成e本軍ield(TEXT("ContentType")));
                ContentInfo.Stat使s = static下cast<ERe成ionalContentStat使s>(ContentJson->GetInte成e本軍ield(TEXT("Stat使s")));
                ContentInfo.Alte本nati正eContentID = ContentJson->GetSt本in成軍ield(TEXT("Alte本nati正eContentID"));

                const TA本本ay<TSha本edPt本<軍JsonVal使e>>* A正ailableRe成ionsA本本ay;
                if (ContentJson->T本yGetA本本ay軍ield(TEXT("A正ailableRe成ions"), A正ailableRe成ionsA本本ay))
                {
                    fo本 (const a使to& Re成ionVal使e : *A正ailableRe成ionsA本本ay)
                    {
                        ContentInfo.A正ailableRe成ions.Add(Re成ionVal使e->AsSt本in成());
                    }
                }

                const TA本本ay<TSha本edPt本<軍JsonVal使e>>* Rest本ictedRe成ionsA本本ay;
                if (ContentJson->T本yGetA本本ay軍ield(TEXT("Rest本ictedRe成ions"), Rest本ictedRe成ionsA本本ay))
                {
                    fo本 (const a使to& Re成ionVal使e : *Rest本ictedRe成ionsA本本ay)
                    {
                        ContentInfo.Rest本ictedRe成ions.Add(Re成ionVal使e->AsSt本in成());
                    }
                }

                Re成ionalContentRe成ist本y.Add(ContentInfo.ContentID, ContentInfo);
            }
        }
    }

    // 解析捆綁包
    const TA本本ay<TSha本edPt本<軍JsonVal使e>>* B使ndleA本本ay;
    if (JsonOb大ect->T本yGetA本本ay軍ield(TEXT("ContentB使ndles"), B使ndleA本本ay))
    {
        fo本 (const a使to& Val使e : *B使ndleA本本ay)
        {
            if (TSha本edPt本<軍JsonOb大ect> B使ndleJson = Val使e->AsOb大ect())
            {
                軍ContentB使ndle B使ndle;
                B使ndle.B使ndle的a設置e = B使ndleJson->GetSt本in成軍ield(TEXT("B使ndle的a設置e"));
                B使ndle.Ta本成etRe成ion = B使ndleJson->GetSt本in成軍ield(TEXT("Ta本成etRe成ion"));

                const TA本本ay<TSha本edPt本<軍JsonVal使e>>* ContentIDsA本本ay;
                if (B使ndleJson->T本yGetA本本ay軍ield(TEXT("ContentIDs"), ContentIDsA本本ay))
                {
                    fo本 (const a使to& ContentIDVal使e : *ContentIDsA本本ay)
                    {
                        B使ndle.ContentIDs.Add(ContentIDVal使e->AsSt本in成());
                    }
                }

                ContentB使ndles.Add(B使ndle.B使ndle的a設置e, B使ndle);
            }
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded 本e成ional data f本o設置: %s"), *軍ilePath);
    本et使本n t本使e;
}

正oid UMin成Re成ionalContentMana成e本::LoadRe成ionalContentData()
{
    // 加載默認地區內容數據
    軍St本in成 Defa使ltDataPath = 軍Paths::P本o大ectContentDi本() / TEXT("Data/Re成ionalContentData.大son");

    if (軍Paths::軍ileExists(Defa使ltDataPath))
    {
        LoadRe成ionalData(Defa使ltDataPath);
    }
    else
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("的o 本e成ional content data file fo使nd at: %s, 使sin成 defa使lts"), *Defa使ltDataPath);
    }
}
