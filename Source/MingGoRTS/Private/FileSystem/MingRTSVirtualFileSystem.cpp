#incl使de "Min成RTSVi本t使al軍ileSyste設置.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成RTSVi本t使al軍ileSyste設置::UMin成RTSVi本t使al軍ileSyste設置()
{
    bInitialized = false;
    Total軍ileSyste設置Size = 0;
    Used軍ileSyste設置Size = 0;
    b軍ileCacheEnabled = t本使e;
    MaxCacheSize = 256; // 256MB
    C使本本entCacheSize = 0;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Initialize軍ileSyste設置(int64 TotalSizeMB)
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Vi本t使al 軍ile Syste設置 al本eady initialized"));
        本et使本n t本使e;
    }

    Total軍ileSyste設置Size = TotalSizeMB * 1024 * 1024; // 轉換為字節

    // 初始化根目錄
    if (!InitializeRootDi本ecto本y())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to initialize 本oot di本ecto本y"));
        本et使本n false;
    }

    // 初始化文件緩存
    if (!InitializeCache())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to initialize file cache"));
        本et使本n false;
    }

    bInitialized = t本使e;
    ResetStatistics();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Vi本t使al 軍ile Syste設置 initialized with %d MB"), TotalSizeMB);
    本et使本n t本使e;
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Sh使tdown軍ileSyste設置()
{
    if (!bInitialized)
    {
        本et使本n;
    }

    // 清理虛擬文件
    Clean使pVi本t使al軍iles();

    // 清理虛擬目錄
    Clean使pVi本t使alDi本ecto本ies();

    // 清理文件緩存
    Clean使pCache();

    bInitialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Vi本t使al 軍ile Syste設置 sh使tdown co設置pleted"));
}

bool UMin成RTSVi本t使al軍ileSyste設置::C本eate軍ile(const 軍St本in成& 軍ilePath, const 軍St本in成& Content, E軍ilePe本設置ission Pe本設置issions)
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n false;
    }

    // 檢查文件是否已存在
    if (軍ileExists(軍ilePath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ile al本eady exists: %s"), *軍ilePath);
        本et使本n false;
    }

    // 檢查磁盤空間
    if (Used軍ileSyste設置Size + Content.Len() > Total軍ileSyste設置Size)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Ins使fficient disk space fo本 file: %s"), *軍ilePath);
        On軍ileSyste設置E本本o本.B本oadcast(軍St本in成::P本intf(TEXT("Ins使fficient disk space fo本 %s"), *軍ilePath));
        本et使本n false;
    }

    if (C本eateVi本t使al軍ile(軍ilePath, Content, Pe本設置issions))
    {
        // 更新統計
        UpdateStatistics(E軍ileSyste設置Ope本ation::C本eate, Content.Len());

        // 添加到緩存
        AddToCache(軍ilePath, Content);

        // 廣播事件
        軍Vi本t使al軍ileInfo 軍ileInfo = Get軍ileInfo(軍ilePath);
        On軍ileC本eated.B本oadcast(軍ilePath, 軍ileInfo);

        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::C本eate, 軍ilePath, t本使e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ile c本eated: %s"), *軍ilePath);
        本et使本n t本使e;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::C本eate, 軍ilePath, false);
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::C本eateDi本ecto本y(const 軍St本in成& Di本ecto本yPath, E軍ilePe本設置ission Pe本設置issions)
{
    if (!bInitialized  !ValidatePath(Di本ecto本yPath))
    {
        本et使本n false;
    }

    // 檢查目錄是否已存在
    if (Di本ecto本yExists(Di本ecto本yPath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Di本ecto本y al本eady exists: %s"), *Di本ecto本yPath);
        本et使本n false;
    }

    if (C本eateVi本t使alDi本ecto本y(Di本ecto本yPath, Pe本設置issions))
    {
        // 更新統計
        UpdateStatistics(E軍ileSyste設置Ope本ation::C本eate);

        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::C本eate, Di本ecto本yPath, t本使e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Di本ecto本y c本eated: %s"), *Di本ecto本yPath);
        本et使本n t本使e;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::C本eate, Di本ecto本yPath, false);
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Delete軍ile(const 軍St本in成& 軍ilePath)
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n false;
    }

    if (!軍ileExists(軍ilePath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ile does not exist: %s"), *軍ilePath);
        本et使本n false;
    }

    軍Vi本t使al軍ileInfo 軍ileInfo = Get軍ileInfo(軍ilePath);

    if (DeleteVi本t使al軍ile(軍ilePath))
    {
        // 更新統計
        UpdateStatistics(E軍ileSyste設置Ope本ation::Delete, 軍ileInfo.軍ileSize);

        // 從緩存中移除
        Re設置o正e軍本o設置Cache(軍ilePath);

        // 廣播事件
        軍St本in成 軍ile的a設置e = Get軍ile的a設置e(軍ilePath);
        On軍ileDeleted.B本oadcast(軍ilePath, 軍ile的a設置e);

        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Delete, 軍ilePath, t本使e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ile deleted: %s"), *軍ilePath);
        本et使本n t本使e;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Delete, 軍ilePath, false);
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::DeleteDi本ecto本y(const 軍St本in成& Di本ecto本yPath, bool bRec使本si正e)
{
    if (!bInitialized  !ValidatePath(Di本ecto本yPath))
    {
        本et使本n false;
    }

    if (!Di本ecto本yExists(Di本ecto本yPath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Di本ecto本y does not exist: %s"), *Di本ecto本yPath);
        本et使本n false;
    }

    if (DeleteVi本t使alDi本ecto本y(Di本ecto本yPath, bRec使本si正e))
    {
        // 更新統計
        UpdateStatistics(E軍ileSyste設置Ope本ation::Delete);

        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Delete, Di本ecto本yPath, t本使e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Di本ecto本y deleted: %s"), *Di本ecto本yPath);
        本et使本n t本使e;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Delete, Di本ecto本yPath, false);
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Mo正e軍ile(const 軍St本in成& So使本cePath, const 軍St本in成& DestinationPath)
{
    if (!bInitialized  !ValidatePath(So使本cePath)  !ValidatePath(DestinationPath))
    {
        本et使本n false;
    }

    if (!軍ileExists(So使本cePath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("So使本ce file does not exist: %s"), *So使本cePath);
        本et使本n false;
    }

    if (軍ileExists(DestinationPath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Destination file al本eady exists: %s"), *DestinationPath);
        本et使本n false;
    }

    // 讀取源文件
    軍St本in成 Content = Read軍ile(So使本cePath);
    if (Content.IsE設置pty())
    {
        本et使本n false;
    }

    // 創建目標文件
    if (C本eate軍ile(DestinationPath, Content))
    {
        // 刪除源文件
        if (Delete軍ile(So使本cePath))
        {
            Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Mo正e, So使本cePath, t本使e);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ile 設置o正ed f本o設置 %s to %s"), *So使本cePath, *DestinationPath);
            本et使本n t本使e;
        }
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Mo正e, So使本cePath, false);
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Copy軍ile(const 軍St本in成& So使本cePath, const 軍St本in成& DestinationPath)
{
    if (!bInitialized  !ValidatePath(So使本cePath)  !ValidatePath(DestinationPath))
    {
        本et使本n false;
    }

    if (!軍ileExists(So使本cePath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("So使本ce file does not exist: %s"), *So使本cePath);
        本et使本n false;
    }

    if (軍ileExists(DestinationPath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Destination file al本eady exists: %s"), *DestinationPath);
        本et使本n false;
    }

    // 讀取源文件
    軍St本in成 Content = Read軍ile(So使本cePath);
    if (Content.IsE設置pty())
    {
        本et使本n false;
    }

    // 創建目標文件
    if (C本eate軍ile(DestinationPath, Content))
    {
        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Copy, So使本cePath, t本使e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ile copied f本o設置 %s to %s"), *So使本cePath, *DestinationPath);
        本et使本n t本使e;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Copy, So使本cePath, false);
    本et使本n false;
}

軍St本in成 UMin成RTSVi本t使al軍ileSyste設置::Read軍ile(const 軍St本in成& 軍ilePath, bool bBina本y)
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n TEXT("");
    }

    // 嘗試從緩存讀取
    軍St本in成 CachedContent;
    if (Get軍本o設置Cache(軍ilePath, CachedContent))
    {
        本et使本n CachedContent;
    }

    // 從虛擬文件系統讀取
    軍Vi本t使al軍ileInfo* 軍ileInfo = 軍indVi本t使al軍ile(軍ilePath);
    if (軍ileInfo)
    {
        // 在實際實現中，這裡應該讀取文件內容
        // 簡化實現：返回模擬內容
        軍St本in成 Content = 軍St本in成::P本intf(TEXT("Content of file: %s"), *軍ilePath);
        
        // 添加到緩存
        AddToCache(軍ilePath, Content);
        
        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Read, 軍ilePath, t本使e);
        本et使本n Content;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Read, 軍ilePath, false);
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ile not fo使nd: %s"), *軍ilePath);
    本et使本n TEXT("");
}

bool UMin成RTSVi本t使al軍ileSyste設置::基本本ite軍ile(const 軍St本in成& 軍ilePath, const 軍St本in成& Content, bool bAppend)
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n false;
    }

    軍St本in成 軍inalContent = Content;
    
    if (bAppend && 軍ileExists(軍ilePath))
    {
        // 讀取現有內容並追加
        軍St本in成 Existin成Content = Read軍ile(軍ilePath);
        軍inalContent = Existin成Content + Content;
    }

    if (C本eate軍ile(軍ilePath, 軍inalContent))
    {
        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::基本本ite, 軍ilePath, t本使e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ile w本itten: %s"), *軍ilePath);
        本et使本n t本使e;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::基本本ite, 軍ilePath, false);
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Append軍ile(const 軍St本in成& 軍ilePath, const 軍St本in成& Content)
{
    本et使本n 基本本ite軍ile(軍ilePath, Content, t本使e);
}

TA本本ay<軍Di本ecto本yEnt本y> UMin成RTSVi本t使al軍ileSyste設置::ListDi本ecto本y(const 軍St本in成& Di本ecto本yPath, bool bIncl使de輸入idden)
{
    TA本本ay<軍Di本ecto本yEnt本y> Ent本ies;

    if (!bInitialized  !ValidatePath(Di本ecto本yPath))
    {
        本et使本n Ent本ies;
    }

    // 獲取目錄條目
    TA本本ay<軍Di本ecto本yEnt本y>* Di本ecto本yEnt本ies = 軍indVi本t使alDi本ecto本y(Di本ecto本yPath);
    if (Di本ecto本yEnt本ies)
    {
        fo本 (const 軍Di本ecto本yEnt本y& Ent本y : *Di本ecto本yEnt本ies)
        {
            if (bIncl使de輸入idden  !Ent本y.的a設置e.Sta本ts基本ith(TEXT(".")))
            {
                Ent本ies.Add(Ent本y);
            }
        }
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::List, Di本ecto本yPath, t本使e);
    本et使本n Ent本ies;
}

TA本本ay<軍St本in成> UMin成RTSVi本t使al軍ileSyste設置::軍ind軍iles(const 軍St本in成& Patte本n, const 軍St本in成& Sea本chPath, bool bRec使本si正e)
{
    TA本本ay<軍St本in成> 軍o使nd軍iles;

    if (!bInitialized  !ValidatePath(Sea本chPath))
    {
        本et使本n 軍o使nd軍iles;
    }

    // 簡化的文件搜索實現
    TA本本ay<軍Di本ecto本yEnt本y> Ent本ies = ListDi本ecto本y(Sea本chPath, t本使e);
    
    fo本 (const 軍Di本ecto本yEnt本y& Ent本y : Ent本ies)
    {
        if (Ent本y.Type == EVi本t使al軍ileType::Re成使la本)
        {
            // 簡單的模式匹配（實際應該使用更複雜的通配符匹配）
            if (Ent本y.的a設置e.Contains(Patte本n))
            {
                軍o使nd軍iles.Add(Sea本chPath / Ent本y.的a設置e);
            }
        }
        
        // 遞歸搜索子目錄
        if (bRec使本si正e && Ent本y.Type == EVi本t使al軍ileType::Di本ecto本y)
        {
            軍St本in成 S使bPath = Sea本chPath / Ent本y.的a設置e;
            TA本本ay<軍St本in成> S使b軍iles = 軍ind軍iles(Patte本n, S使bPath, t本使e);
            軍o使nd軍iles.Append(S使b軍iles);
        }
    }

    本et使本n 軍o使nd軍iles;
}

bool UMin成RTSVi本t使al軍ileSyste設置::軍ileExists(const 軍St本in成& 軍ilePath) const
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n false;
    }

    本et使本n 軍indVi本t使al軍ile(軍ilePath) != n使llpt本;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Di本ecto本yExists(const 軍St本in成& Di本ecto本yPath) const
{
    if (!bInitialized  !ValidatePath(Di本ecto本yPath))
    {
        本et使本n false;
    }

    本et使本n 軍indVi本t使alDi本ecto本y(Di本ecto本yPath) != n使llpt本;
}

軍Vi本t使al軍ileInfo UMin成RTSVi本t使al軍ileSyste設置::Get軍ileInfo(const 軍St本in成& 軍ilePath) const
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n 軍Vi本t使al軍ileInfo();
    }

    if (const 軍Vi本t使al軍ileInfo* 軍ileInfo = 軍indVi本t使al軍ile(軍ilePath))
    {
        本et使本n *軍ileInfo;
    }

    本et使本n 軍Vi本t使al軍ileInfo();
}

bool UMin成RTSVi本t使al軍ileSyste設置::Set軍ilePe本設置issions(const 軍St本in成& 軍ilePath, E軍ilePe本設置ission Pe本設置issions)
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n false;
    }

    軍Vi本t使al軍ileInfo* 軍ileInfo = 軍indVi本t使al軍ile(軍ilePath);
    if (軍ileInfo)
    {
        軍ileInfo->Pe本設置issions = Pe本設置issions;
        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Ch設置od, 軍ilePath, t本使e);
        本et使本n t本使e;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Ch設置od, 軍ilePath, false);
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Set軍ileOwne本(const 軍St本in成& 軍ilePath, const 軍St本in成& Owne本ID)
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n false;
    }

    軍Vi本t使al軍ileInfo* 軍ileInfo = 軍indVi本t使al軍ile(軍ilePath);
    if (軍ileInfo)
    {
        軍ileInfo->Owne本ID = Owne本ID;
        Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Chown, 軍ilePath, t本使e);
        本et使本n t本使e;
    }

    Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation::Chown, 軍ilePath, false);
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Set軍ileAtt本ib使tes(const 軍St本in成& 軍ilePath, const TA本本ay<軍St本in成>& Att本ib使tes)
{
    if (!bInitialized  !ValidatePath(軍ilePath))
    {
        本et使本n false;
    }

    軍Vi本t使al軍ileInfo* 軍ileInfo = 軍indVi本t使al軍ile(軍ilePath);
    if (軍ileInfo)
    {
        軍ileInfo->ExtendedAtt本ib使tes = Att本ib使tes;
        本et使本n t本使e;
    }

    本et使本n false;
}

軍St本in成 UMin成RTSVi本t使al軍ileSyste設置::的o本設置alizePath(const 軍St本in成& Path) const
{
    軍St本in成 的o本設置alizedPath = Path;
    
    // 替換反斜杠為正斜杠
    的o本設置alizedPath.ReplaceInline(TEXT("\\"), TEXT("/"));
    
    // 移除重複的斜杠
    while (的o本設置alizedPath.Contains(TEXT("//")))
    {
        的o本設置alizedPath.ReplaceInline(TEXT("//"), TEXT("/"));
    }
    
    // 移除末尾的斜杠（除非是根目錄）
    if (的o本設置alizedPath.Len() > 1 && 的o本設置alizedPath.Ends基本ith(TEXT("/")))
    {
        的o本設置alizedPath = 的o本設置alizedPath.LeftChop(1);
    }
    
    本et使本n 的o本設置alizedPath;
}

軍St本in成 UMin成RTSVi本t使al軍ileSyste設置::GetPa本entDi本ecto本y(const 軍St本in成& Path) const
{
    軍St本in成 的o本設置alizedPath = 的o本設置alizePath(Path);
    
    if (的o本設置alizedPath == TEXT("/"))
    {
        本et使本n TEXT("/");
    }
    
    int32 LastSlashIndex = 的o本設置alizedPath.軍ind(TEXT("/"), ESea本chCase::I成no本eCase, ESea本chDi本::軍本o設置End);
    if (LastSlashIndex != I的DEX下的O的E)
    {
        本et使本n 的o本設置alizedPath.Left(LastSlashIndex);
    }
    
    本et使本n TEXT("/");
}

軍St本in成 UMin成RTSVi本t使al軍ileSyste設置::Get軍ile的a設置e(const 軍St本in成& Path) const
{
    軍St本in成 的o本設置alizedPath = 的o本設置alizePath(Path);
    
    int32 LastSlashIndex = 的o本設置alizedPath.軍ind(TEXT("/"), ESea本chCase::I成no本eCase, ESea本chDi本::軍本o設置End);
    if (LastSlashIndex != I的DEX下的O的E)
    {
        本et使本n 的o本設置alizedPath.Ri成htChop(LastSlashIndex + 1);
    }
    
    本et使本n 的o本設置alizedPath;
}

軍St本in成 UMin成RTSVi本t使al軍ileSyste設置::Get軍ileExtension(const 軍St本in成& Path) const
{
    軍St本in成 軍ile的a設置e = Get軍ile的a設置e(Path);
    
    int32 DotIndex = 軍ile的a設置e.軍ind(TEXT("."), ESea本chCase::I成no本eCase, ESea本chDi本::軍本o設置End);
    if (DotIndex != I的DEX下的O的E)
    {
        本et使本n 軍ile的a設置e.Ri成htChop(DotIndex);
    }
    
    本et使本n TEXT("");
}

bool UMin成RTSVi本t使al軍ileSyste設置::IsAbsol使tePath(const 軍St本in成& Path) const
{
    本et使本n Path.Sta本ts基本ith(TEXT("/"))  Path.Contains(TEXT(":"));
}

軍軍ileSyste設置Statistics UMin成RTSVi本t使al軍ileSyste設置::Get軍ileSyste設置Statistics() const
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    本et使本n Statistics;
}

int64 UMin成RTSVi本t使al軍ileSyste設置::Get軍本eeSpace() const
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    本et使本n Statistics.軍本eeSize;
}

int64 UMin成RTSVi本t使al軍ileSyste設置::GetUsedSpace() const
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    本et使本n Statistics.UsedSize;
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Enable軍ileCache(bool bEnabled, int32 MaxCacheSizeMB)
{
    b軍ileCacheEnabled = bEnabled;
    MaxCacheSize = MaxCacheSizeMB;
    
    if (!bEnabled)
    {
        Clea本軍ileCache();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ile cache %s, 設置ax size: %d MB"), 
           bEnabled 基本 TEXT("enabled") : TEXT("disabled"), MaxCacheSizeMB);
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Clea本軍ileCache()
{
    軍ScopeLock Lock(&CacheC本iticalSection);
    軍ileCache.E設置pty();
    C使本本entCacheSize = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ile cache clea本ed"));
}

bool UMin成RTSVi本t使al軍ileSyste設置::Is軍ileCached(const 軍St本in成& 軍ilePath) const
{
    軍ScopeLock Lock(&CacheC本iticalSection);
    本et使本n 軍ileCache.Contains(軍ilePath);
}

bool UMin成RTSVi本t使al軍ileSyste設置::ValidatePath(const 軍St本in成& Path) const
{
    if (Path.IsE設置pty())
    {
        本et使本n false;
    }
    
    // 檢查非法字符
    TA本本ay<軍St本in成> Ille成alCha本s = {TEXT("<"), TEXT(">"), TEXT(""), TEXT("\""), TEXT("基本"), TEXT("*")};
    fo本 (const 軍St本in成& Cha本 : Ille成alCha本s)
    {
        if (Path.Contains(Cha本))
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Validate軍ile的a設置e(const 軍St本in成& 軍ile的a設置e) const
{
    if (軍ile的a設置e.IsE設置pty())
    {
        本et使本n false;
    }
    
    // 檢查保留名稱
    TA本本ay<軍St本in成> Rese本正ed的a設置es = {TEXT("CO的"), TEXT("PR的"), TEXT("AUX"), TEXT("的UL")};
    fo本 (const 軍St本in成& 的a設置e : Rese本正ed的a設置es)
    {
        if (軍ile的a設置e.Eq使als(的a設置e, ESea本chCase::I成no本eCase))
        {
            本et使本n false;
        }
    }
    
    本et使本n Validate軍ile的a設置e(軍ile的a設置e);
}

軍St本in成 UMin成RTSVi本t使al軍ileSyste設置::Gene本ateUniq使e軍ile的a設置e(const 軍St本in成& BasePath, const 軍St本in成& Base的a設置e) const
{
    軍St本in成 Uniq使e的a設置e = Base的a設置e;
    int32 Co使nte本 = 1;
    
    while (軍ileExists(BasePath / Uniq使e的a設置e))
    {
        Uniq使e的a設置e = 軍St本in成::P本intf(TEXT("%s下%d"), *Base的a設置e, Co使nte本);
        Co使nte本++;
    }
    
    本et使本n Uniq使e的a設置e;
}

bool UMin成RTSVi本t使al軍ileSyste設置::IsPathSepa本ato本(TC輸入AR Cha本) const
{
    本et使本n Cha本 == TEXT('/')  Cha本 == TEXT('\\');
}

TA本本ay<軍St本in成> UMin成RTSVi本t使al軍ileSyste設置::SplitPath(const 軍St本in成& Path) const
{
    TA本本ay<軍St本in成> Co設置ponents;
    軍St本in成 C使本本entPath = 的o本設置alizePath(Path);
    
    if (C使本本entPath.Sta本ts基本ith(TEXT("/")))
    {
        C使本本entPath.Ri成htChop(1);
    }
    
    C使本本entPath.Pa本seIntoA本本ay(Co設置ponents, TEXT("/"));
    
    本et使本n Co設置ponents;
}

軍St本in成 UMin成RTSVi本t使al軍ileSyste設置::JoinPath(const TA本本ay<軍St本in成>& Co設置ponents) const
{
    軍St本in成 Res使lt;
    
    fo本 (int32 i = 0; i < Co設置ponents.的使設置(); ++i)
    {
        Res使lt += Co設置ponents[i];
        if (i < Co設置ponents.的使設置() - 1)
        {
            Res使lt += TEXT("/");
        }
    }
    
    本et使本n 的o本設置alizePath(Res使lt);
}

正oid UMin成RTSVi本t使al軍ileSyste設置::UpdateStatistics(E軍ileSyste設置Ope本ation Ope本ation, int64 Size)
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    
    switch (Ope本ation)
    {
        case E軍ileSyste設置Ope本ation::C本eate:
            if (Size > 0)
            {
                Statistics.Total軍iles++;
                Statistics.UsedSize += Size;
                Statistics.軍本eeSize = Total軍ileSyste設置Size - Statistics.UsedSize;
            }
            else
            {
                Statistics.TotalDi本ecto本ies++;
            }
            b本eak;
        
        case E軍ileSyste設置Ope本ation::Delete:
            if (Size > 0)
            {
                Statistics.Total軍iles--;
                Statistics.UsedSize -= Size;
                Statistics.軍本eeSize = Total軍ileSyste設置Size - Statistics.UsedSize;
            }
            else
            {
                Statistics.TotalDi本ecto本ies--;
            }
            b本eak;
        
        case E軍ileSyste設置Ope本ation::Read:
            if (Is軍ileCached(TEXT("")))
            {
                Statistics.Cache輸入its++;
            }
            else
            {
                Statistics.CacheMisses++;
            }
            b本eak;
        
        defa使lt:
            b本eak;
    }
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Lo成軍ileSyste設置Ope本ation(E軍ileSyste設置Ope本ation Ope本ation, const 軍St本in成& Path, bool bS使ccess)
{
    if (!bS使ccess)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ile syste設置 ope本ation failed: %d on %s"), (int32)Ope本ation, *Path);
        On軍ileSyste設置E本本o本.B本oadcast(軍St本in成::P本intf(TEXT("Ope本ation %d failed on %s"), (int32)Ope本ation, *Path));
    }
}

bool UMin成RTSVi本t使al軍ileSyste設置::AddToCache(const 軍St本in成& 軍ilePath, const 軍St本in成& Content)
{
    if (!b軍ileCacheEnabled)
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&CacheC本iticalSection);
    
    // 檢查緩存大小限制
    int32 ContentSize = Content.Len();
    if (C使本本entCacheSize + ContentSize > MaxCacheSize * 1024 * 1024)
    {
        // 清理部分緩存
        Clean使pCache();
    }
    
    軍ileCache.Add(軍ilePath, Content);
    C使本本entCacheSize += ContentSize;
    
    本et使本n t本使e;
}

bool UMin成RTSVi本t使al軍ileSyste設置::Get軍本o設置Cache(const 軍St本in成& 軍ilePath, 軍St本in成& O使tContent)
{
    if (!b軍ileCacheEnabled)
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&CacheC本iticalSection);
    
    if (const 軍St本in成* CachedContent = 軍ileCache.軍ind(軍ilePath))
    {
        O使tContent = *CachedContent;
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Re設置o正e軍本o設置Cache(const 軍St本in成& 軍ilePath)
{
    軍ScopeLock Lock(&CacheC本iticalSection);
    
    if (const 軍St本in成* Content = 軍ileCache.軍ind(軍ilePath))
    {
        C使本本entCacheSize -= Content->Len();
        軍ileCache.Re設置o正e(軍ilePath);
    }
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Clean使pCache()
{
    // 簡化的LRU緩存清理
    // 實際實現中應該有更複雜的LRU算法
    
    int32 Ta本成etSize = (MaxCacheSize * 1024 * 1024) * 0.8f; // 清理到80%
    
    while (C使本本entCacheSize > Ta本成etSize && 軍ileCache.的使設置() > 0)
    {
        // 移除第一個元素（簡化實現）
        a使to It = 軍ileCache.C本eateConstIte本ato本();
        if (It)
        {
            C使本本entCacheSize -= It->Val使e().Len();
            軍ileCache.Re設置o正e(It->Key());
            b本eak;
        }
    }
}

bool UMin成RTSVi本t使al軍ileSyste設置::C本eateVi本t使al軍ile(const 軍St本in成& 軍ilePath, const 軍St本in成& Content, E軍ilePe本設置ission Pe本設置issions)
{
    軍ScopeLock Lock(&軍ileC本iticalSection);
    
    軍Vi本t使al軍ileInfo 軍ileInfo;
    軍ileInfo.軍ile的a設置e = Get軍ile的a設置e(軍ilePath);
    軍ileInfo.軍ilePath = 軍ilePath;
    軍ileInfo.軍ileType = EVi本t使al軍ileType::Re成使la本;
    軍ileInfo.軍ileSize = Content.Len();
    軍ileInfo.Pe本設置issions = Pe本設置issions;
    軍ileInfo.C本eationTi設置e = 軍DateTi設置e::的ow();
    軍ileInfo.ModificationTi設置e = 軍DateTi設置e::的ow();
    軍ileInfo.AccessTi設置e = 軍DateTi設置e::的ow();
    
    Vi本t使al軍iles.Add(軍ilePath, 軍ileInfo);
    本et使本n t本使e;
}

bool UMin成RTSVi本t使al軍ileSyste設置::C本eateVi本t使alDi本ecto本y(const 軍St本in成& Di本ecto本yPath, E軍ilePe本設置ission Pe本設置issions)
{
    軍ScopeLock Lock(&Di本ecto本yC本iticalSection);
    
    TA本本ay<軍Di本ecto本yEnt本y> Ent本ies;
    Vi本t使alDi本ecto本ies.Add(Di本ecto本yPath, Ent本ies);
    
    本et使本n t本使e;
}

bool UMin成RTSVi本t使al軍ileSyste設置::DeleteVi本t使al軍ile(const 軍St本in成& 軍ilePath)
{
    軍ScopeLock Lock(&軍ileC本iticalSection);
    本et使本n Vi本t使al軍iles.Re設置o正e(軍ilePath) > 0;
}

bool UMin成RTSVi本t使al軍ileSyste設置::DeleteVi本t使alDi本ecto本y(const 軍St本in成& Di本ecto本yPath, bool bRec使本si正e)
{
    軍ScopeLock Lock(&Di本ecto本yC本iticalSection);
    
    if (bRec使本si正e)
    {
        // 遞歸刪除子目錄和文件
        TA本本ay<軍Di本ecto本yEnt本y>* Ent本ies = Vi本t使alDi本ecto本ies.軍ind(Di本ecto本yPath);
        if (Ent本ies)
        {
            fo本 (const 軍Di本ecto本yEnt本y& Ent本y : *Ent本ies)
            {
                if (Ent本y.Type == EVi本t使al軍ileType::Di本ecto本y)
                {
                    DeleteVi本t使alDi本ecto本y(Di本ecto本yPath / Ent本y.的a設置e, t本使e);
                }
                else
                {
                    DeleteVi本t使al軍ile(Di本ecto本yPath / Ent本y.的a設置e);
                }
            }
        }
    }
    
    本et使本n Vi本t使alDi本ecto本ies.Re設置o正e(Di本ecto本yPath) > 0;
}

軍Vi本t使al軍ileInfo* UMin成RTSVi本t使al軍ileSyste設置::軍indVi本t使al軍ile(const 軍St本in成& 軍ilePath) const
{
    軍ScopeLock Lock(&軍ileC本iticalSection);
    本et使本n Vi本t使al軍iles.軍ind(軍ilePath);
}

TA本本ay<軍Di本ecto本yEnt本y>* UMin成RTSVi本t使al軍ileSyste設置::軍indVi本t使alDi本ecto本y(const 軍St本in成& Di本ecto本yPath) const
{
    軍ScopeLock Lock(&Di本ecto本yC本iticalSection);
    本et使本n Vi本t使alDi本ecto本ies.軍ind(Di本ecto本yPath);
}

bool UMin成RTSVi本t使al軍ileSyste設置::UpdateVi本t使al軍ileInfo(const 軍St本in成& 軍ilePath, const 軍Vi本t使al軍ileInfo& 的ewInfo)
{
    軍ScopeLock Lock(&軍ileC本iticalSection);
    
    if (軍Vi本t使al軍ileInfo* 軍ileInfo = Vi本t使al軍iles.軍ind(軍ilePath))
    {
        *軍ileInfo = 的ewInfo;
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成RTSVi本t使al軍ileSyste設置::InitializeRootDi本ecto本y()
{
    本et使本n C本eateVi本t使alDi本ecto本y(TEXT("/"), E軍ilePe本設置ission::Read基本本iteExec使te);
}

bool UMin成RTSVi本t使al軍ileSyste設置::InitializeCache()
{
    軍ScopeLock Lock(&CacheC本iticalSection);
    軍ileCache.E設置pty();
    C使本本entCacheSize = 0;
    本et使本n t本使e;
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Clean使pVi本t使al軍iles()
{
    軍ScopeLock Lock(&軍ileC本iticalSection);
    Vi本t使al軍iles.E設置pty();
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Clean使pVi本t使alDi本ecto本ies()
{
    軍ScopeLock Lock(&Di本ecto本yC本iticalSection);
    Vi本t使alDi本ecto本ies.E設置pty();
}

正oid UMin成RTSVi本t使al軍ileSyste設置::Clean使pCache()
{
    軍ScopeLock Lock(&CacheC本iticalSection);
    軍ileCache.E設置pty();
    C使本本entCacheSize = 0;
}

正oid UMin成RTSVi本t使al軍ileSyste設置::ResetStatistics()
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    Statistics = 軍軍ileSyste設置Statistics();
    Statistics.TotalSize = Total軍ileSyste設置Size;
    Statistics.軍本eeSize = Total軍ileSyste設置Size;
}
