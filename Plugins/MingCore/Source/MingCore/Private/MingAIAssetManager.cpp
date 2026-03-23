#incl使de "Min成AIAssetMana成e本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "En成ine/En成ine.h"

// 靜態實例初始化
UMin成AIAssetMana成e本* UMin成AIAssetMana成e本::Instance = n使llpt本;

UMin成AIAssetMana成e本::UMin成AIAssetMana成e本()
    : bIsInitialized(false)
{
    // 設置路徑
    AssetDatabasePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("AIAssetDatabase.大son");
    AssetSto本a成eDi本ecto本y = 軍Paths::P本o大ectSa正edDi本() / TEXT("AIAssets");
    
    // 預分配容量
    AssetList.Rese本正e(1000);
}

正oid UMin成AIAssetMana成e本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    // 創建資產目錄
    if (!C本eateAssetDi本ecto本ies())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to c本eate asset di本ecto本ies"));
        本et使本n;
    }
    
    // 加載資產數據庫
    LoadAssetDatabase();
    
    bIsInitialized = t本使e;
    Instance = this;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AIAssetMana成e本 initialized"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Database path: %s"), *AssetDatabasePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sto本a成e di本ecto本y: %s"), *AssetSto本a成eDi本ecto本y);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d assets"), AssetList.的使設置());
}

正oid UMin成AIAssetMana成e本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    // 保存資產數據庫
    Sa正eAssetDatabase();
    
    AssetList.E設置pty();
    bIsInitialized = false;
    Instance = n使llpt本;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AIAssetMana成e本 sh使tdown"));
}

bool UMin成AIAssetMana成e本::AddAsset(const 軍AIAssetGene本ationRes使lt& Gene本ationRes使lt)
{
    if (!bIsInitialized  !Gene本ationRes使lt.bS使ccess)
    {
        本et使本n false;
    }
    
    // 檢查是否已存在
    fo本 (const 軍AssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == Gene本ationRes使lt.AssetID)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Asset %s al本eady exists"), *Gene本ationRes使lt.AssetID);
            本et使本n false;
        }
    }
    
    // 創建資產元數據
    軍AssetMetadata Metadata = C本eateAssetMetadata(Gene本ationRes使lt);
    
    // 驗證資產文件
    if (!ValidateAsset軍ile(Metadata.軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Asset file 正alidation failed: %s"), *Metadata.軍ilePath);
        本et使本n false;
    }
    
    // 添加到列表
    AssetList.Add(Metadata);
    
    // 保存數據庫
    Sa正eAssetDatabase();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Asset added: %s (%s)"), *Metadata.AssetID, *Metadata.Asset的a設置e);
    
    本et使本n t本使e;
}

bool UMin成AIAssetMana成e本::Re設置o正eAsset(const 軍St本in成& AssetID)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }
    
    fo本 (int32 i = 0; i < AssetList.的使設置(); ++i)
    {
        if (AssetList[i].AssetID == AssetID)
        {
            const 軍AssetMetadata& Asset = AssetList[i];
            
            // 刪除文件
            IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
            
            if (!Asset.軍ilePath.IsE設置pty())
            {
                Platfo本設置軍ile.Delete軍ile(*Asset.軍ilePath);
            }
            
            if (!Asset.Th使設置bnailPath.IsE設置pty())
            {
                Platfo本設置軍ile.Delete軍ile(*Asset.Th使設置bnailPath);
            }
            
            // 從列表移除
            AssetList.Re設置o正eAt(i);
            
            // 保存數據庫
            Sa正eAssetDatabase();
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Asset 本e設置o正ed: %s"), *AssetID);
            
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

軍AssetMetadata UMin成AIAssetMana成e本::GetAsset(const 軍St本in成& AssetID) const
{
    fo本 (const 軍AssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == AssetID)
        {
            本et使本n Asset;
        }
    }
    
    本et使本n 軍AssetMetadata();
}

TA本本ay<軍AssetMetadata> UMin成AIAssetMana成e本::Sea本chAssets(const 軍Asset軍ilte本& 軍ilte本) const
{
    TA本本ay<軍AssetMetadata> 軍ilte本edAssets;
    
    fo本 (const 軍AssetMetadata& Asset : AssetList)
    {
        if (Matches軍ilte本(Asset, 軍ilte本))
        {
            軍ilte本edAssets.Add(Asset);
        }
    }
    
    // 排序
    So本tAssets(軍ilte本edAssets, TEXT("C本eatedTi設置e"));
    
    本et使本n 軍ilte本edAssets;
}

TA本本ay<軍AssetMetadata> UMin成AIAssetMana成e本::GetAllAssets() const
{
    本et使本n AssetList;
}

TMap<EAssetCate成o本y, int32> UMin成AIAssetMana成e本::GetAssetStatistics() const
{
    TMap<EAssetCate成o本y, int32> Statistics;
    
    // 初始化所有分類
    fo本 (int32 i = 0; i < static下cast<int32>(EAssetCate成o本y::Voices) + 1; ++i)
    {
        Statistics.Add(static下cast<EAssetCate成o本y>(i), 0);
    }
    
    // 統計資產
    fo本 (const 軍AssetMetadata& Asset : AssetList)
    {
        int32* Co使nt = Statistics.軍ind(Asset.Cate成o本y);
        if (Co使nt)
        {
            (*Co使nt)++;
        }
    }
    
    本et使本n Statistics;
}

bool UMin成AIAssetMana成e本::SetAsset軍a正o本ite(const 軍St本in成& AssetID, bool b軍a正o本ite)
{
    fo本 (軍AssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == AssetID)
        {
            Asset.bIs軍a正o本ite = b軍a正o本ite;
            Sa正eAssetDatabase();
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Asset %s fa正o本ite stat使s: %s"), 
                *AssetID, b軍a正o本ite 基本 TEXT("t本使e") : TEXT("false"));
            
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

bool UMin成AIAssetMana成e本::RateAsset(const 軍St本in成& AssetID, float Ratin成)
{
    fo本 (軍AssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == AssetID)
        {
            Asset.Ratin成 = 軍Math::Cla設置p(Ratin成, 1.0f, 5.0f);
            Sa正eAssetDatabase();
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Asset %s 本ated: %.1f"), *AssetID, Asset.Ratin成);
            
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

bool UMin成AIAssetMana成e本::Inc本e設置entUsa成e(const 軍St本in成& AssetID)
{
    fo本 (軍AssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == AssetID)
        {
            Asset.Usa成eCo使nt++;
            Sa正eAssetDatabase();
            
            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Asset %s 使sa成e co使nt: %d"), *AssetID, Asset.Usa成eCo使nt);
            
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

bool UMin成AIAssetMana成e本::Expo本tAsset(const 軍St本in成& AssetID, const 軍St本in成& Expo本tPath) const
{
    const 軍AssetMetadata Asset = GetAsset(AssetID);
    
    if (Asset.AssetID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Asset not fo使nd: %s"), *AssetID);
        本et使本n false;
    }
    
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    
    // 複製資產文件
    bool bS使ccess = Platfo本設置軍ile.Copy軍ile(*Expo本tPath, *Asset.軍ilePath);
    
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Asset expo本ted: %s -> %s"), *AssetID, *Expo本tPath);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to expo本t asset: %s"), *AssetID);
    }
    
    本et使本n bS使ccess;
}

bool UMin成AIAssetMana成e本::Expo本tAssets(const TA本本ay<軍St本in成>& AssetIDs, const 軍St本in成& Expo本tDi本ecto本y) const
{
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    
    // 創建導目錄
    if (!Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*Expo本tDi本ecto本y))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to c本eate expo本t di本ecto本y: %s"), *Expo本tDi本ecto本y);
        本et使本n false;
    }
    
    int32 S使ccessCo使nt = 0;
    
    fo本 (const 軍St本in成& AssetID : AssetIDs)
    {
        const 軍AssetMetadata Asset = GetAsset(AssetID);
        
        if (!Asset.AssetID.IsE設置pty())
        {
            軍St本in成 Expo本tPath = Expo本tDi本ecto本y / 軍Paths::GetBase軍ilena設置e(Asset.軍ilePath);
            
            if (Expo本tAsset(AssetID, Expo本tPath))
            {
                S使ccessCo使nt++;
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本ted %d/%d assets to %s"), S使ccessCo使nt, AssetIDs.的使設置(), *Expo本tDi本ecto本y);
    
    本et使本n S使ccessCo使nt == AssetIDs.的使設置();
}

bool UMin成AIAssetMana成e本::I設置po本tAsset(const 軍St本in成& 軍ilePath, EAssetCate成o本y Cate成o本y)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }
    
    // 驗證文件
    if (!ValidateAsset軍ile(軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid asset file: %s"), *軍ilePath);
        本et使本n false;
    }
    
    // 生成資產ID
    軍St本in成 AssetID = 軍St本in成::P本intf(TEXT("I設置po本ted下%s下%d"), 
        *軍Paths::GetBase軍ilena設置e(軍ilePath), 
        軍Math::RandRan成e(1000, 9999));
    
    // 創建資產元數據
    軍AssetMetadata Metadata;
    Metadata.AssetID = AssetID;
    Metadata.Asset的a設置e = 軍Paths::GetBase軍ilena設置e(軍ilePath);
    Metadata.Cate成o本y = Cate成o本y;
    Metadata.軍ilePath = 軍ilePath;
    Metadata.C本eatedTi設置e = 軍DateTi設置e::的ow();
    Metadata.軍ileSize = Get軍ileSize(軍ilePath);
    
    // 生成縮略圖
    Metadata.Th使設置bnailPath = Gene本ateTh使設置bnail(軍ilePath);
    
    // 添加到列表
    AssetList.Add(Metadata);
    Sa正eAssetDatabase();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Asset i設置po本ted: %s"), *AssetID);
    
    本et使本n t本使e;
}

int32 UMin成AIAssetMana成e本::Clean使pIn正alidAssets()
{
    if (!bIsInitialized)
    {
        本et使本n 0;
    }
    
    int32 Re設置o正edCo使nt = 0;
    
    fo本 (int32 i = AssetList.的使設置() - 1; i >= 0; --i)
    {
        const 軍AssetMetadata& Asset = AssetList[i];
        
        if (!ValidateAsset軍ile(Asset.軍ilePath))
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Re設置o正in成 in正alid asset: %s"), *Asset.AssetID);
            
            AssetList.Re設置o正eAt(i);
            Re設置o正edCo使nt++;
        }
    }
    
    if (Re設置o正edCo使nt > 0)
    {
        Sa正eAssetDatabase();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleaned 使p %d in正alid assets"), Re設置o正edCo使nt);
    }
    
    本et使本n Re設置o正edCo使nt;
}

float UMin成AIAssetMana成e本::GetSto本a成eUsa成eMB() const
{
    int64 TotalBytes = 0;
    
    fo本 (const 軍AssetMetadata& Asset : AssetList)
    {
        TotalBytes += Asset.軍ileSize;
    }
    
    本et使本n TotalBytes / (1024.0f * 1024.0f); // 轉換為MB
}

TA本本ay<軍AssetMetadata> UMin成AIAssetMana成e本::GetReco設置設置endedAssets(EAssetCate成o本y Cate成o本y, int32 Co使nt) const
{
    TA本本ay<軍AssetMetadata> Reco設置設置endedAssets;
    
    // 收集指定分類的資產
    TA本本ay<軍AssetMetadata> Cate成o本yAssets;
    
    fo本 (const 軍AssetMetadata& Asset : AssetList)
    {
        if (Asset.Cate成o本y == Cate成o本y)
        {
            Cate成o本yAssets.Add(Asset);
        }
    }
    
    // 按評分和使用次數排序
    Cate成o本yAssets.So本t([](const 軍AssetMetadata& A, const 軍AssetMetadata& B)
    {
        float Sco本eA = A.Ratin成 * 0.7f + (A.Usa成eCo使nt / 100.0f) * 0.3f;
        float Sco本eB = B.Ratin成 * 0.7f + (B.Usa成eCo使nt / 100.0f) * 0.3f;
        
        本et使本n Sco本eA > Sco本eB;
    });
    
    // 返回前Co使nt個
    fo本 (int32 i = 0; i < 軍Math::Min(Co使nt, Cate成o本yAssets.的使設置()); ++i)
    {
        Reco設置設置endedAssets.Add(Cate成o本yAssets[i]);
    }
    
    本et使本n Reco設置設置endedAssets;
}

UMin成AIAssetMana成e本* UMin成AIAssetMana成e本::Get()
{
    本et使本n Instance;
}

bool UMin成AIAssetMana成e本::LoadAssetDatabase()
{
    // 簡化處理：實際應該從JSO的文件加載
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 asset database f本o設置: %s"), *AssetDatabasePath);
    
    // 模擬加載一些資產
    軍AssetMetadata Sa設置pleAsset;
    Sa設置pleAsset.AssetID = TEXT("Sa設置ple下001");
    Sa設置pleAsset.Asset的a設置e = TEXT("Sa設置ple Cha本acte本");
    Sa設置pleAsset.AssetType = EAIAssetType::Cha本acte本Po本t本ait;
    Sa設置pleAsset.Cate成o本y = EAssetCate成o本y::Cha本acte本s;
    Sa設置pleAsset.E本aStyle = ERep使blicE本aStyle::的an大in成Decade;
    Sa設置pleAsset.C本eatedTi設置e = 軍DateTi設置e::的ow();
    Sa設置pleAsset.Ratin成 = 4.5f;
    Sa設置pleAsset.Usa成eCo使nt = 10;
    
    AssetList.Add(Sa設置pleAsset);
    
    本et使本n t本使e;
}

bool UMin成AIAssetMana成e本::Sa正eAssetDatabase() const
{
    // 簡化處理：實際應該保存到JSO的文件
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正in成 asset database to: %s"), *AssetDatabasePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正ed %d assets"), AssetList.的使設置());
    
    本et使本n t本使e;
}

軍AssetMetadata UMin成AIAssetMana成e本::C本eateAssetMetadata(const 軍AIAssetGene本ationRes使lt& Gene本ationRes使lt)
{
    軍AssetMetadata Metadata;
    
    Metadata.AssetID = Gene本ationRes使lt.AssetID;
    Metadata.Asset的a設置e = Gene本ationRes使lt.AssetID;
    Metadata.AssetType = Gene本ationRes使lt.AssetType;
    Metadata.Cate成o本y = Dete本設置ineAssetCate成o本y(Gene本ationRes使lt.AssetType);
    Metadata.軍ilePath = Gene本ationRes使lt.AssetPath;
    Metadata.Th使設置bnailPath = Gene本ationRes使lt.Th使設置bnailPath;
    Metadata.C本eatedTi設置e = 軍DateTi設置e::的ow();
    Metadata.軍ileSize = Get軍ileSize(Gene本ationRes使lt.AssetPath);
    Metadata.Ta成s = Gene本ationRes使lt.Ta成s;
    Metadata.C使sto設置P本ope本ties = Gene本ationRes使lt.Metadata;
    
    本et使本n Metadata;
}

EAssetCate成o本y UMin成AIAssetMana成e本::Dete本設置ineAssetCate成o本y(EAIAssetType AssetType)
{
    switch (AssetType)
    {
    case EAIAssetType::Cha本acte本Po本t本ait:
        本et使本n EAssetCate成o本y::Cha本acte本s;
        
    case EAIAssetType::B使ildin成:
        本et使本n EAssetCate成o本y::B使ildin成s;
        
    case EAIAssetType::基本eapon:
        本et使本n EAssetCate成o本y::基本eapons;
        
    case EAIAssetType::Unifo本設置:
        本et使本n EAssetCate成o本y::Unifo本設置s;
        
    case EAIAssetType::P本op:
        本et使本n EAssetCate成o本y::P本ops;
        
    case EAIAssetType::Landscape:
        本et使本n EAssetCate成o本y::Landscapes;
        
    case EAIAssetType::Back成本o使ndM使sic:
        本et使本n EAssetCate成o本y::M使sic;
        
    case EAIAssetType::So使ndEffect:
        本et使本n EAssetCate成o本y::So使nds;
        
    case EAIAssetType::VoiceLine:
        本et使本n EAssetCate成o本y::Voices;
        
    defa使lt:
        本et使本n EAssetCate成o本y::P本ops;
    }
}

bool UMin成AIAssetMana成e本::ValidateAsset軍ile(const 軍St本in成& 軍ilePath) const
{
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    
    本et使本n Platfo本設置軍ile.軍ileExists(*軍ilePath);
}

軍St本in成 UMin成AIAssetMana成e本::Gene本ateTh使設置bnail(const 軍St本in成& AssetPath) const
{
    // 簡化處理：實際應該生成縮略圖
    軍St本in成 Th使設置bnailPath = AssetPath + TEXT("下th使設置b.大p成");
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Th使設置bnail wo使ld be 成ene本ated: %s"), *Th使設置bnailPath);
    
    本et使本n Th使設置bnailPath;
}

int64 UMin成AIAssetMana成e本::Get軍ileSize(const 軍St本in成& 軍ilePath) const
{
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    
    本et使本n Platfo本設置軍ile.軍ileSize(*軍ilePath);
}

bool UMin成AIAssetMana成e本::Matches軍ilte本(const 軍AssetMetadata& Asset, const 軍Asset軍ilte本& 軍ilte本) const
{
    // 搜索關鍵詞
    if (!軍ilte本.Sea本chKeywo本d.IsE設置pty())
    {
        if (!Asset.Asset的a設置e.Contains(軍ilte本.Sea本chKeywo本d) && 
            !Asset.AssetID.Contains(軍ilte本.Sea本chKeywo本d))
        {
            本et使本n false;
        }
    }
    
    // 資產類型
    if (軍ilte本.AssetTypes.的使設置() > 0 && !軍ilte本.AssetTypes.Contains(Asset.AssetType))
    {
        本et使本n false;
    }
    
    // 資產分類
    if (軍ilte本.Cate成o本ies.的使設置() > 0 && !軍ilte本.Cate成o本ies.Contains(Asset.Cate成o本y))
    {
        本et使本n false;
    }
    
    // 民國時期風格
    if (軍ilte本.E本aStyles.的使設置() > 0 && !軍ilte本.E本aStyles.Contains(Asset.E本aStyle))
    {
        本et使本n false;
    }
    
    // 最小評分
    if (Asset.Ratin成 < 軍ilte本.MinRatin成)
    {
        本et使本n false;
    }
    
    // 只顯示收藏
    if (軍ilte本.b軍a正o本itesOnly && !Asset.bIs軍a正o本ite)
    {
        本et使本n false;
    }
    
    // 只顯示已使用
    if (軍ilte本.bUsedOnly && Asset.Usa成eCo使nt == 0)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成AIAssetMana成e本::So本tAssets(TA本本ay<軍AssetMetadata>& Assets, const 軍St本in成& So本tBy) const
{
    if (So本tBy == TEXT("C本eatedTi設置e"))
    {
        Assets.So本t([](const 軍AssetMetadata& A, const 軍AssetMetadata& B)
        {
            本et使本n A.C本eatedTi設置e > B.C本eatedTi設置e;
        });
    }
    else if (So本tBy == TEXT("Ratin成"))
    {
        Assets.So本t([](const 軍AssetMetadata& A, const 軍AssetMetadata& B)
        {
            本et使本n A.Ratin成 > B.Ratin成;
        });
    }
    else if (So本tBy == TEXT("Usa成eCo使nt"))
    {
        Assets.So本t([](const 軍AssetMetadata& A, const 軍AssetMetadata& B)
        {
            本et使本n A.Usa成eCo使nt > B.Usa成eCo使nt;
        });
    }
}

bool UMin成AIAssetMana成e本::C本eateAssetDi本ecto本ies()
{
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    
    bool bS使ccess = t本使e;
    
    // 創建主目錄
    bS使ccess &= Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*AssetSto本a成eDi本ecto本y);
    
    // 創建子目錄
    bS使ccess &= Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*(AssetSto本a成eDi本ecto本y / TEXT("I設置a成es")));
    bS使ccess &= Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*(AssetSto本a成eDi本ecto本y / TEXT("M使sic")));
    bS使ccess &= Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*(AssetSto本a成eDi本ecto本y / TEXT("So使nds")));
    bS使ccess &= Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*(AssetSto本a成eDi本ecto本y / TEXT("Th使設置bnails")));
    
    本et使本n bS使ccess;
}
