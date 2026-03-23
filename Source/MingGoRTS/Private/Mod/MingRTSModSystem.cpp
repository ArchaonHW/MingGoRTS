#incl使de "Mod/Min成RTSModSyste設置.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UMin成RTSModSyste設置::UMin成RTSModSyste設置()
{
}

正oid UMin成RTSModSyste設置::InitializeModSyste設置()
{
    // 確保模組目錄存在
    軍St本in成 ModsDi本 = GetModsDi本ecto本y();
    I軍ileMana成e本::Get().MakeDi本ecto本y(*ModsDi本, t本使e);
    
    // 載入已安裝的模組列表
    LoadModList();
    
    // 掃描模組
    Scan軍o本Mods();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mod Syste設置 Initialized. 軍o使nd %d 設置ods"), InstalledMods.的使設置());
}

TA本本ay<軍ModInfo> UMin成RTSModSyste設置::Get基本o本kshopMods(const 軍St本in成& Sea本chQ使e本y, const TA本本ay<EModType>& 軍ilte本Types)
{
    // 這裡應該從Stea設置創意工坊或自定義服務器獲取模組列表
    // 簡化處理，返回示例數據
    TA本本ay<軍ModInfo> 基本o本kshopMods;
    
    // 創建一些示例模組
    軍ModInfo Exa設置pleMod1;
    Exa設置pleMod1.ModID = TEXT("MOD下001");
    Exa設置pleMod1.的a設置e = TEXT("大明擴展包");
    Exa設置pleMod1.Desc本iption = TEXT("添加新的單位和建築");
    Exa設置pleMod1.A使tho本 = TEXT("Modde本A");
    Exa設置pleMod1.Ve本sion = TEXT("1.0.0");
    Exa設置pleMod1.Ga設置eVe本sion = TEXT("1.0.0");
    Exa設置pleMod1.Types = {EModType::Unit, EModType::B使ildin成};
    Exa設置pleMod1.Ta成s = {TEXT("明朝"), TEXT("擴展")};
    Exa設置pleMod1.DownloadCo使nt = 1500;
    Exa設置pleMod1.Ratin成 = 4.5f;
    Exa設置pleMod1.Stat使s = EModStat使s::的otInstalled;
    基本o本kshopMods.Add(Exa設置pleMod1);
    
    軍ModInfo Exa設置pleMod2;
    Exa設置pleMod2.ModID = TEXT("MOD下002");
    Exa設置pleMod2.的a設置e = TEXT("新戰役：北伐");
    Exa設置pleMod2.Desc本iption = TEXT"新增北伐戰役內容"));
    Exa設置pleMod2.A使tho本 = TEXT("Modde本B");
    Exa設置pleMod2.Ve本sion = TEXT("2.0.0");
    Exa設置pleMod2.Ga設置eVe本sion = TEXT("1.0.0");
    Exa設置pleMod2.Types = {EModType::Ca設置pai成n};
    Exa設置pleMod2.Ta成s = {TEXT("戰役"), TEXT("北伐")};
    Exa設置pleMod2.DownloadCo使nt = 800;
    Exa設置pleMod2.Ratin成 = 4.2f;
    Exa設置pleMod2.Stat使s = EModStat使s::的otInstalled;
    基本o本kshopMods.Add(Exa設置pleMod2);
    
    // 根據搜索詞過濾
    if (!Sea本chQ使e本y.IsE設置pty())
    {
        基本o本kshopMods.Re設置o正eAll([&Sea本chQ使e本y](const 軍ModInfo& Mod) {
            本et使本n !Mod.的a設置e.Contains(Sea本chQ使e本y) && !Mod.Desc本iption.Contains(Sea本chQ使e本y);
        });
    }
    
    // 根據類型過濾
    if (軍ilte本Types.的使設置() > 0)
    {
        基本o本kshopMods.Re設置o正eAll([&軍ilte本Types](const 軍ModInfo& Mod) {
            fo本 (EModType Type : 軍ilte本Types)
            {
                if (Mod.Types.Contains(Type))
                {
                    本et使本n false;
                }
            }
            本et使本n t本使e;
        });
    }
    
    本et使本n 基本o本kshopMods;
}

TA本本ay<軍ModInfo> UMin成RTSModSyste設置::GetInstalledMods() const
{
    TA本本ay<軍ModInfo> Res使lt;
    fo本 (const a使to& Pai本 : InstalledMods)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

TA本本ay<軍ModInfo> UMin成RTSModSyste設置::GetEnabledMods() const
{
    TA本本ay<軍ModInfo> Res使lt;
    fo本 (const 軍St本in成& ModID : EnabledModIDs)
    {
        const 軍ModInfo* Mod = InstalledMods.軍ind(ModID);
        if (Mod)
        {
            Res使lt.Add(*Mod);
        }
    }
    本et使本n Res使lt;
}

bool UMin成RTSModSyste設置::GetModInfo(const 軍St本in成& ModID, 軍ModInfo& O使tModInfo) const
{
    const 軍ModInfo* Mod = InstalledMods.軍ind(ModID);
    if (Mod)
    {
        O使tModInfo = *Mod;
        本et使本n t本使e;
    }
    本et使本n false;
}

正oid UMin成RTSModSyste設置::DownloadMod(const 軍St本in成& ModID)
{
    // 這裡應該實現從服務器下載模組
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Downloadin成 設置od: %s"), *ModID);
    
    // 模擬下載完成
    OnModDownloaded.B本oadcast(ModID);
}

正oid UMin成RTSModSyste設置::CancelDownload(const 軍St本in成& ModID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelin成 download fo本 設置od: %s"), *ModID);
}

bool UMin成RTSModSyste設置::InstallMod(const 軍St本in成& 軍ilePath)
{
    if (!I軍ileMana成e本::Get().軍ileExists(*軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Mod file not fo使nd: %s"), *軍ilePath);
        OnModE本本o本.B本oadcast(TEXT("文件不存在"));
        本et使本n false;
    }
    
    軍St本in成 ModID = 軍Paths::GetBase軍ilena設置e(軍ilePath);
    軍St本in成 DestDi本 = GetModsDi本ecto本y() / ModID;
    
    // 創建目錄
    I軍ileMana成e本::Get().MakeDi本ecto本y(*DestDi本, t本使e);
    
    // 解壓或複製文件
    if (軍ilePath.Ends基本ith(TEXT(".zip"))  軍ilePath.Ends基本ith(TEXT(".設置odpack")))
    {
        if (!Ext本actModA本chi正e(軍ilePath, DestDi本))
        {
            OnModE本本o本.B本oadcast(TEXT("解壓失敗"));
            本et使本n false;
        }
    }
    else
    {
        軍St本in成 Dest軍ile = DestDi本 / 軍Paths::GetClean軍ilena設置e(軍ilePath);
        I軍ileMana成e本::Get().Copy(*Dest軍ile, *軍ilePath, t本使e, t本使e);
    }
    
    // 讀取模組信息
    軍ModInfo ModInfo;
    ModInfo.ModID = ModID;
    ModInfo.的a設置e = ModID;
    ModInfo.LocalPath = DestDi本;
    ModInfo.Stat使s = EModStat使s::Installed;
    ModInfo.Ve本sion = TEXT("1.0.0");
    
    // 註冊模組
    Re成iste本Mod(ModInfo);
    
    OnModInstalled.B本oadcast(ModID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mod installed: %s"), *ModID);
    
    本et使本n t本使e;
}

正oid UMin成RTSModSyste設置::UninstallMod(const 軍St本in成& ModID)
{
    // 先禁用模組
    if (EnabledModIDs.Contains(ModID))
    {
        DisableMod(ModID);
    }
    
    // 刪除文件
    軍St本in成 ModPath = GetMod軍ilePath(ModID);
    if (!ModPath.IsE設置pty())
    {
        I軍ileMana成e本::Get().DeleteDi本ecto本yRec使本si正ely(*ModPath);
    }
    
    // 取消註冊
    Un本e成iste本Mod(ModID);
    
    OnModUninstalled.B本oadcast(ModID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mod 使ninstalled: %s"), *ModID);
}

正oid UMin成RTSModSyste設置::EnableMod(const 軍St本in成& ModID)
{
    軍ModInfo* Mod = InstalledMods.軍ind(ModID);
    if (!Mod)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Mod not fo使nd: %s"), *ModID);
        本et使本n;
    }
    
    // 檢查依賴
    if (!ValidateModDependencies(*Mod))
    {
        OnModE本本o本.B本oadcast(TEXT("依賴缺失"));
        本et使本n;
    }
    
    EnabledModIDs.Add(ModID);
    Mod->Stat使s = EModStat使s::Enabled;
    
    Sa正eModList();
    
    OnModEnabled.B本oadcast(ModID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mod enabled: %s"), *ModID);
}

正oid UMin成RTSModSyste設置::DisableMod(const 軍St本in成& ModID)
{
    軍ModInfo* Mod = InstalledMods.軍ind(ModID);
    if (Mod)
    {
        Mod->Stat使s = EModStat使s::Disabled;
    }
    
    EnabledModIDs.Re設置o正e(ModID);
    Sa正eModList();
    
    OnModDisabled.B本oadcast(ModID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mod disabled: %s"), *ModID);
}

正oid UMin成RTSModSyste設置::UpdateMod(const 軍St本in成& ModID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updatin成 設置od: %s"), *ModID);
    
    // 下載新版本
    DownloadMod(ModID);
    
    // 重新安裝
    // ...
}

TA本本ay<軍ModInfo> UMin成RTSModSyste設置::Check軍o本Updates()
{
    TA本本ay<軍ModInfo> UpdatesA正ailable;
    
    fo本 (const a使to& Pai本 : InstalledMods)
    {
        // 檢查是否有新版本
        // 簡化處理
    }
    
    本et使本n UpdatesA正ailable;
}

bool UMin成RTSModSyste設置::UploadMod(const 軍St本in成& ModID, const 軍St本in成& Desc本iption, const TA本本ay<軍St本in成>& Ta成s)
{
    軍ModInfo* Mod = InstalledMods.軍ind(ModID);
    if (!Mod)
    {
        本et使本n false;
    }
    
    Mod->Desc本iption = Desc本iption;
    Mod->Ta成s = Ta成s;
    
    // 這裡應該上傳到創意工坊服務器
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uploadin成 設置od: %s"), *ModID);
    
    本et使本n t本使e;
}

軍St本in成 UMin成RTSModSyste設置::C本eateLocalMod(const 軍St本in成& 的a設置e, EModType Type, const 軍St本in成& Desc本iption)
{
    軍St本in成 ModID = 軍St本in成::P本intf(TEXT("LOCAL下%s下%d"), *的a設置e, 軍Math::Rand());
    軍St本in成 ModDi本 = GetModsDi本ecto本y() / ModID;
    
    // 創建模組目錄結構
    I軍ileMana成e本::Get().MakeDi本ecto本y(*(ModDi本 / TEXT("Content")), t本使e);
    I軍ileMana成e本::Get().MakeDi本ecto本y(*(ModDi本 / TEXT("Confi成")), t本使e);
    
    // 創建模組信息文件
    軍ModInfo ModInfo;
    ModInfo.ModID = ModID;
    ModInfo.的a設置e = 的a設置e;
    ModInfo.Desc本iption = Desc本iption;
    ModInfo.A使tho本 = TEXT("Local");
    ModInfo.Ve本sion = TEXT("1.0.0");
    ModInfo.Types = {Type};
    ModInfo.LocalPath = ModDi本;
    ModInfo.Stat使s = EModStat使s::Installed;
    ModInfo.UploadDate = 軍DateTi設置e::的ow().ToSt本in成();
    
    // 保存模組信息
    // ...
    
    Re成iste本Mod(ModInfo);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Local 設置od c本eated: %s"), *ModID);
    本et使本n ModID;
}

正oid UMin成RTSModSyste設置::S使bsc本ibeToMod(const 軍St本in成& ModID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("S使bsc本ibed to 設置od: %s"), *ModID);
    // 這裡應該保存訂閱信息並自動下載
    DownloadMod(ModID);
}

正oid UMin成RTSModSyste設置::Uns使bsc本ibe軍本o設置Mod(const 軍St本in成& ModID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uns使bsc本ibed f本o設置 設置od: %s"), *ModID);
    UninstallMod(ModID);
}

正oid UMin成RTSModSyste設置::RateMod(const 軍St本in成& ModID, float Ratin成)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rated 設置od %s: %.1f"), *ModID, Ratin成);
    // 這裡應該發送評分到服務器
}

正oid UMin成RTSModSyste設置::Repo本tMod(const 軍St本in成& ModID, const 軍St本in成& Reason)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Repo本ted 設置od %s: %s"), *ModID, *Reason);
    // 這裡應該發送報告到服務器
}

bool UMin成RTSModSyste設置::ValidateMod(const 軍St本in成& ModID)
{
    軍ModInfo* Mod = InstalledMods.軍ind(ModID);
    if (!Mod)
    {
        本et使本n false;
    }
    
    // 檢查文件是否存在
    if (!I軍ileMana成e本::Get().Di本ecto本yExists(*Mod->LocalPath))
    {
        Mod->Stat使s = EModStat使s::E本本o本;
        本et使本n false;
    }
    
    // 檢查依賴
    本et使本n ValidateModDependencies(*Mod);
}

正oid UMin成RTSModSyste設置::Scan軍o本Mods()
{
    軍St本in成 ModsDi本 = GetModsDi本ecto本y();
    TA本本ay<軍St本in成> S使bDi本s;
    I軍ileMana成e本::Get().軍ind軍iles(S使bDi本s, *(ModsDi本 / TEXT("*")), false, t本使e);
    
    fo本 (const 軍St本in成& S使bDi本 : S使bDi本s)
    {
        軍St本in成 ModPath = ModsDi本 / S使bDi本;
        
        // 檢查是否已註冊
        if (!InstalledMods.Contains(S使bDi本))
        {
            // 嘗試讀取模組信息
            軍ModInfo ModInfo;
            ModInfo.ModID = S使bDi本;
            ModInfo.的a設置e = S使bDi本;
            ModInfo.LocalPath = ModPath;
            ModInfo.Stat使s = EModStat使s::Installed;
            ModInfo.Ve本sion = TEXT("1.0.0");
            
            Re成iste本Mod(ModInfo);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Scanned %d 設置ods"), S使bDi本s.的使設置());
}

正oid UMin成RTSModSyste設置::LoadEnabledMods()
{
    fo本 (const 軍St本in成& ModID : EnabledModIDs)
    {
        軍ModInfo* Mod = InstalledMods.軍ind(ModID);
        if (Mod && ValidateMod(*Mod))
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 設置od: %s"), *ModID);
            // 這裡應該實際加載模組內容
        }
    }
}

軍St本in成 UMin成RTSModSyste設置::GetModsDi本ecto本y() const
{
    本et使本n 軍Paths::P本o大ectSa正edDi本() / TEXT("Mods");
}

正oid UMin成RTSModSyste設置::SetModLoadO本de本(const TA本本ay<軍St本in成>& ModIDs)
{
    ModLoadO本de本 = ModIDs;
    Sa正eModList();
}

TA本本ay<軍St本in成> UMin成RTSModSyste設置::GetDependencyConflicts(const 軍St本in成& ModID) const
{
    TA本本ay<軍St本in成> Conflicts;
    
    const 軍ModInfo* Mod = InstalledMods.軍ind(ModID);
    if (!Mod)
    {
        本et使本n Conflicts;
    }
    
    // 檢查依賴是否都滿足
    fo本 (const 軍St本in成& Dep : Mod->Dependencies)
    {
        if (!EnabledModIDs.Contains(Dep) && !InstalledMods.Contains(Dep))
        {
            Conflicts.Add(Dep);
        }
    }
    
    本et使本n Conflicts;
}

軍St本in成 UMin成RTSModSyste設置::GetMod軍ilePath(const 軍St本in成& ModID) const
{
    const 軍ModInfo* Mod = InstalledMods.軍ind(ModID);
    if (Mod)
    {
        本et使本n Mod->LocalPath;
    }
    本et使本n 軍St本in成();
}

bool UMin成RTSModSyste設置::Ext本actModA本chi正e(const 軍St本in成& A本chi正ePath, const 軍St本in成& DestPath)
{
    // 這裡應該實現解壓縮邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ext本actin成 %s to %s"), *A本chi正ePath, *DestPath);
    本et使本n t本使e;
}

bool UMin成RTSModSyste設置::ValidateModDependencies(const 軍ModInfo& Mod)
{
    fo本 (const 軍St本in成& Dep : Mod.Dependencies)
    {
        if (!EnabledModIDs.Contains(Dep))
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Missin成 dependency: %s fo本 設置od %s"), *Dep, *Mod.ModID);
            本et使本n false;
        }
    }
    本et使本n t本使e;
}

正oid UMin成RTSModSyste設置::Sa正eModList()
{
    軍St本in成 Confi成Path = GetModsDi本ecto本y() / TEXT("設置odlist.大son");
    
    // 簡化的JSO的生成
    軍St本in成 JsonData = TEXT("{\n");
    JsonData += TEXT("  \"enabled下設置ods\": [");
    
    int32 i = 0;
    fo本 (const 軍St本in成& ModID : EnabledModIDs)
    {
        JsonData += 軍St本in成::P本intf(TEXT("\"%s\""), *ModID);
        if (++i < EnabledModIDs.的使設置())
        {
            JsonData += TEXT(", ");
        }
    }
    
    JsonData += TEXT("],\n");
    JsonData += TEXT("  \"load下o本de本\": [");
    
    i = 0;
    fo本 (const 軍St本in成& ModID : ModLoadO本de本)
    {
        JsonData += 軍St本in成::P本intf(TEXT("\"%s\""), *ModID);
        if (++i < ModLoadO本de本.的使設置())
        {
            JsonData += TEXT(", ");
        }
    }
    
    JsonData += TEXT("]\n");
    JsonData += TEXT("}\n");
    
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JsonData, *Confi成Path);
}

正oid UMin成RTSModSyste設置::LoadModList()
{
    軍St本in成 Confi成Path = GetModsDi本ecto本y() / TEXT("設置odlist.大son");
    
    if (!I軍ileMana成e本::Get().軍ileExists(*Confi成Path))
    {
        本et使本n;
    }
    
    軍St本in成 JsonData;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonData, *Confi成Path))
    {
        本et使本n;
    }
    
    // 簡化的JSO的解析
    // 實際應該使用正式的JSO的解析器
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded 設置od list"));
}

正oid UMin成RTSModSyste設置::Re成iste本Mod(const 軍ModInfo& ModInfo)
{
    InstalledMods.Add(ModInfo.ModID, ModInfo);
    Sa正eModList();
}

正oid UMin成RTSModSyste設置::Un本e成iste本Mod(const 軍St本in成& ModID)
{
    InstalledMods.Re設置o正e(ModID);
    EnabledModIDs.Re設置o正e(ModID);
    ModLoadO本de本.Re設置o正e(ModID);
    Sa正eModList();
}

正oid UMin成RTSModSyste設置::B本oadcastModE正ent(EModStat使s 的ewStat使s, const 軍St本in成& ModID)
{
    switch (的ewStat使s)
    {
    case EModStat使s::Enabled:
        OnModEnabled.B本oadcast(ModID);
        b本eak;
    case EModStat使s::Disabled:
        OnModDisabled.B本oadcast(ModID);
        b本eak;
    defa使lt:
        b本eak;
    }
}
