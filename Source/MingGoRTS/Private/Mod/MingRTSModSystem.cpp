#include "Mod/MingRTSModSystem.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSModSystem::UMingRTSModSystem()
{
}

void UMingRTSModSystem::InitializeModSystem()
{
    // 確保模組目錄存在
    FString ModsDir = GetModsDirectory();
    IFileManager::Get().MakeDirectory(*ModsDir, true);
    
    // 載入已安裝的模組列表
    LoadModList();
    
    // 掃描模組
    ScanForMods();
    
    UE_LOG(LogTemp, Log, TEXT("Mod System Initialized. Found %d mods"), InstalledMods.Num());
}

TArray<FModInfo> UMingRTSModSystem::GetWorkshopMods(const FString& SearchQuery, const TArray<EModType>& FilterTypes)
{
    // 這裡應該從Steam創意工坊或自定義服務器獲取模組列表
    // 簡化處理，返回示例數據
    TArray<FModInfo> WorkshopMods;
    
    // 創建一些示例模組
    FModInfo ExampleMod1;
    ExampleMod1.ModID = TEXT("MOD_001");
    ExampleMod1.Name = TEXT("大明擴展包");
    ExampleMod1.Description = TEXT("添加新的單位和建築");
    ExampleMod1.Author = TEXT("ModderA");
    ExampleMod1.Version = TEXT("1.0.0");
    ExampleMod1.GameVersion = TEXT("1.0.0");
    ExampleMod1.Types = {EModType::Unit, EModType::Building};
    ExampleMod1.Tags = {TEXT("明朝"), TEXT("擴展")};
    ExampleMod1.DownloadCount = 1500;
    ExampleMod1.Rating = 4.5f;
    ExampleMod1.Status = EModStatus::NotInstalled;
    WorkshopMods.Add(ExampleMod1);
    
    FModInfo ExampleMod2;
    ExampleMod2.ModID = TEXT("MOD_002");
    ExampleMod2.Name = TEXT("新戰役：北伐");
    ExampleMod2.Description = TEXT"新增北伐戰役內容"));
    ExampleMod2.Author = TEXT("ModderB");
    ExampleMod2.Version = TEXT("2.0.0");
    ExampleMod2.GameVersion = TEXT("1.0.0");
    ExampleMod2.Types = {EModType::Campaign};
    ExampleMod2.Tags = {TEXT("戰役"), TEXT("北伐")};
    ExampleMod2.DownloadCount = 800;
    ExampleMod2.Rating = 4.2f;
    ExampleMod2.Status = EModStatus::NotInstalled;
    WorkshopMods.Add(ExampleMod2);
    
    // 根據搜索詞過濾
    if (!SearchQuery.IsEmpty())
    {
        WorkshopMods.RemoveAll([&SearchQuery](const FModInfo& Mod) {
            return !Mod.Name.Contains(SearchQuery) && !Mod.Description.Contains(SearchQuery);
        });
    }
    
    // 根據類型過濾
    if (FilterTypes.Num() > 0)
    {
        WorkshopMods.RemoveAll([&FilterTypes](const FModInfo& Mod) {
            for (EModType Type : FilterTypes)
            {
                if (Mod.Types.Contains(Type))
                {
                    return false;
                }
            }
            return true;
        });
    }
    
    return WorkshopMods;
}

TArray<FModInfo> UMingRTSModSystem::GetInstalledMods() const
{
    TArray<FModInfo> Result;
    for (const auto& Pair : InstalledMods)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FModInfo> UMingRTSModSystem::GetEnabledMods() const
{
    TArray<FModInfo> Result;
    for (const FString& ModID : EnabledModIDs)
    {
        const FModInfo* Mod = InstalledMods.Find(ModID);
        if (Mod)
        {
            Result.Add(*Mod);
        }
    }
    return Result;
}

bool UMingRTSModSystem::GetModInfo(const FString& ModID, FModInfo& OutModInfo) const
{
    const FModInfo* Mod = InstalledMods.Find(ModID);
    if (Mod)
    {
        OutModInfo = *Mod;
        return true;
    }
    return false;
}

void UMingRTSModSystem::DownloadMod(const FString& ModID)
{
    // 這裡應該實現從服務器下載模組
    UE_LOG(LogTemp, Log, TEXT("Downloading mod: %s"), *ModID);
    
    // 模擬下載完成
    OnModDownloaded.Broadcast(ModID);
}

void UMingRTSModSystem::CancelDownload(const FString& ModID)
{
    UE_LOG(LogTemp, Log, TEXT("Canceling download for mod: %s"), *ModID);
}

bool UMingRTSModSystem::InstallMod(const FString& FilePath)
{
    if (!IFileManager::Get().FileExists(*FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Mod file not found: %s"), *FilePath);
        OnModError.Broadcast(TEXT("文件不存在"));
        return false;
    }
    
    FString ModID = FPaths::GetBaseFilename(FilePath);
    FString DestDir = GetModsDirectory() / ModID;
    
    // 創建目錄
    IFileManager::Get().MakeDirectory(*DestDir, true);
    
    // 解壓或複製文件
    if (FilePath.EndsWith(TEXT(".zip")) || FilePath.EndsWith(TEXT(".modpack")))
    {
        if (!ExtractModArchive(FilePath, DestDir))
        {
            OnModError.Broadcast(TEXT("解壓失敗"));
            return false;
        }
    }
    else
    {
        FString DestFile = DestDir / FPaths::GetCleanFilename(FilePath);
        IFileManager::Get().Copy(*DestFile, *FilePath, true, true);
    }
    
    // 讀取模組信息
    FModInfo ModInfo;
    ModInfo.ModID = ModID;
    ModInfo.Name = ModID;
    ModInfo.LocalPath = DestDir;
    ModInfo.Status = EModStatus::Installed;
    ModInfo.Version = TEXT("1.0.0");
    
    // 註冊模組
    RegisterMod(ModInfo);
    
    OnModInstalled.Broadcast(ModID);
    UE_LOG(LogTemp, Log, TEXT("Mod installed: %s"), *ModID);
    
    return true;
}

void UMingRTSModSystem::UninstallMod(const FString& ModID)
{
    // 先禁用模組
    if (EnabledModIDs.Contains(ModID))
    {
        DisableMod(ModID);
    }
    
    // 刪除文件
    FString ModPath = GetModFilePath(ModID);
    if (!ModPath.IsEmpty())
    {
        IFileManager::Get().DeleteDirectoryRecursively(*ModPath);
    }
    
    // 取消註冊
    UnregisterMod(ModID);
    
    OnModUninstalled.Broadcast(ModID);
    UE_LOG(LogTemp, Log, TEXT("Mod uninstalled: %s"), *ModID);
}

void UMingRTSModSystem::EnableMod(const FString& ModID)
{
    FModInfo* Mod = InstalledMods.Find(ModID);
    if (!Mod)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mod not found: %s"), *ModID);
        return;
    }
    
    // 檢查依賴
    if (!ValidateModDependencies(*Mod))
    {
        OnModError.Broadcast(TEXT("依賴缺失"));
        return;
    }
    
    EnabledModIDs.Add(ModID);
    Mod->Status = EModStatus::Enabled;
    
    SaveModList();
    
    OnModEnabled.Broadcast(ModID);
    UE_LOG(LogTemp, Log, TEXT("Mod enabled: %s"), *ModID);
}

void UMingRTSModSystem::DisableMod(const FString& ModID)
{
    FModInfo* Mod = InstalledMods.Find(ModID);
    if (Mod)
    {
        Mod->Status = EModStatus::Disabled;
    }
    
    EnabledModIDs.Remove(ModID);
    SaveModList();
    
    OnModDisabled.Broadcast(ModID);
    UE_LOG(LogTemp, Log, TEXT("Mod disabled: %s"), *ModID);
}

void UMingRTSModSystem::UpdateMod(const FString& ModID)
{
    UE_LOG(LogTemp, Log, TEXT("Updating mod: %s"), *ModID);
    
    // 下載新版本
    DownloadMod(ModID);
    
    // 重新安裝
    // ...
}

TArray<FModInfo> UMingRTSModSystem::CheckForUpdates()
{
    TArray<FModInfo> UpdatesAvailable;
    
    for (const auto& Pair : InstalledMods)
    {
        // 檢查是否有新版本
        // 簡化處理
    }
    
    return UpdatesAvailable;
}

bool UMingRTSModSystem::UploadMod(const FString& ModID, const FString& Description, const TArray<FString>& Tags)
{
    FModInfo* Mod = InstalledMods.Find(ModID);
    if (!Mod)
    {
        return false;
    }
    
    Mod->Description = Description;
    Mod->Tags = Tags;
    
    // 這裡應該上傳到創意工坊服務器
    UE_LOG(LogTemp, Log, TEXT("Uploading mod: %s"), *ModID);
    
    return true;
}

FString UMingRTSModSystem::CreateLocalMod(const FString& Name, EModType Type, const FString& Description)
{
    FString ModID = FString::Printf(TEXT("LOCAL_%s_%d"), *Name, FMath::Rand());
    FString ModDir = GetModsDirectory() / ModID;
    
    // 創建模組目錄結構
    IFileManager::Get().MakeDirectory(*(ModDir / TEXT("Content")), true);
    IFileManager::Get().MakeDirectory(*(ModDir / TEXT("Config")), true);
    
    // 創建模組信息文件
    FModInfo ModInfo;
    ModInfo.ModID = ModID;
    ModInfo.Name = Name;
    ModInfo.Description = Description;
    ModInfo.Author = TEXT("Local");
    ModInfo.Version = TEXT("1.0.0");
    ModInfo.Types = {Type};
    ModInfo.LocalPath = ModDir;
    ModInfo.Status = EModStatus::Installed;
    ModInfo.UploadDate = FDateTime::Now().ToString();
    
    // 保存模組信息
    // ...
    
    RegisterMod(ModInfo);
    
    UE_LOG(LogTemp, Log, TEXT("Local mod created: %s"), *ModID);
    return ModID;
}

void UMingRTSModSystem::SubscribeToMod(const FString& ModID)
{
    UE_LOG(LogTemp, Log, TEXT("Subscribed to mod: %s"), *ModID);
    // 這裡應該保存訂閱信息並自動下載
    DownloadMod(ModID);
}

void UMingRTSModSystem::UnsubscribeFromMod(const FString& ModID)
{
    UE_LOG(LogTemp, Log, TEXT("Unsubscribed from mod: %s"), *ModID);
    UninstallMod(ModID);
}

void UMingRTSModSystem::RateMod(const FString& ModID, float Rating)
{
    UE_LOG(LogTemp, Log, TEXT("Rated mod %s: %.1f"), *ModID, Rating);
    // 這裡應該發送評分到服務器
}

void UMingRTSModSystem::ReportMod(const FString& ModID, const FString& Reason)
{
    UE_LOG(LogTemp, Log, TEXT("Reported mod %s: %s"), *ModID, *Reason);
    // 這裡應該發送報告到服務器
}

bool UMingRTSModSystem::ValidateMod(const FString& ModID)
{
    FModInfo* Mod = InstalledMods.Find(ModID);
    if (!Mod)
    {
        return false;
    }
    
    // 檢查文件是否存在
    if (!IFileManager::Get().DirectoryExists(*Mod->LocalPath))
    {
        Mod->Status = EModStatus::Error;
        return false;
    }
    
    // 檢查依賴
    return ValidateModDependencies(*Mod);
}

void UMingRTSModSystem::ScanForMods()
{
    FString ModsDir = GetModsDirectory();
    TArray<FString> SubDirs;
    IFileManager::Get().FindFiles(SubDirs, *(ModsDir / TEXT("*")), false, true);
    
    for (const FString& SubDir : SubDirs)
    {
        FString ModPath = ModsDir / SubDir;
        
        // 檢查是否已註冊
        if (!InstalledMods.Contains(SubDir))
        {
            // 嘗試讀取模組信息
            FModInfo ModInfo;
            ModInfo.ModID = SubDir;
            ModInfo.Name = SubDir;
            ModInfo.LocalPath = ModPath;
            ModInfo.Status = EModStatus::Installed;
            ModInfo.Version = TEXT("1.0.0");
            
            RegisterMod(ModInfo);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Scanned %d mods"), SubDirs.Num());
}

void UMingRTSModSystem::LoadEnabledMods()
{
    for (const FString& ModID : EnabledModIDs)
    {
        FModInfo* Mod = InstalledMods.Find(ModID);
        if (Mod && ValidateMod(*Mod))
        {
            UE_LOG(LogTemp, Log, TEXT("Loading mod: %s"), *ModID);
            // 這裡應該實際加載模組內容
        }
    }
}

FString UMingRTSModSystem::GetModsDirectory() const
{
    return FPaths::ProjectSavedDir() / TEXT("Mods");
}

void UMingRTSModSystem::SetModLoadOrder(const TArray<FString>& ModIDs)
{
    ModLoadOrder = ModIDs;
    SaveModList();
}

TArray<FString> UMingRTSModSystem::GetDependencyConflicts(const FString& ModID) const
{
    TArray<FString> Conflicts;
    
    const FModInfo* Mod = InstalledMods.Find(ModID);
    if (!Mod)
    {
        return Conflicts;
    }
    
    // 檢查依賴是否都滿足
    for (const FString& Dep : Mod->Dependencies)
    {
        if (!EnabledModIDs.Contains(Dep) && !InstalledMods.Contains(Dep))
        {
            Conflicts.Add(Dep);
        }
    }
    
    return Conflicts;
}

FString UMingRTSModSystem::GetModFilePath(const FString& ModID) const
{
    const FModInfo* Mod = InstalledMods.Find(ModID);
    if (Mod)
    {
        return Mod->LocalPath;
    }
    return FString();
}

bool UMingRTSModSystem::ExtractModArchive(const FString& ArchivePath, const FString& DestPath)
{
    // 這裡應該實現解壓縮邏輯
    UE_LOG(LogTemp, Log, TEXT("Extracting %s to %s"), *ArchivePath, *DestPath);
    return true;
}

bool UMingRTSModSystem::ValidateModDependencies(const FModInfo& Mod)
{
    for (const FString& Dep : Mod.Dependencies)
    {
        if (!EnabledModIDs.Contains(Dep))
        {
            UE_LOG(LogTemp, Warning, TEXT("Missing dependency: %s for mod %s"), *Dep, *Mod.ModID);
            return false;
        }
    }
    return true;
}

void UMingRTSModSystem::SaveModList()
{
    FString ConfigPath = GetModsDirectory() / TEXT("modlist.json");
    
    // 簡化的JSON生成
    FString JsonData = TEXT("{\n");
    JsonData += TEXT("  \"enabled_mods\": [");
    
    int32 i = 0;
    for (const FString& ModID : EnabledModIDs)
    {
        JsonData += FString::Printf(TEXT("\"%s\""), *ModID);
        if (++i < EnabledModIDs.Num())
        {
            JsonData += TEXT(", ");
        }
    }
    
    JsonData += TEXT("],\n");
    JsonData += TEXT("  \"load_order\": [");
    
    i = 0;
    for (const FString& ModID : ModLoadOrder)
    {
        JsonData += FString::Printf(TEXT("\"%s\""), *ModID);
        if (++i < ModLoadOrder.Num())
        {
            JsonData += TEXT(", ");
        }
    }
    
    JsonData += TEXT("]\n");
    JsonData += TEXT("}\n");
    
    FFileHelper::SaveStringToFile(JsonData, *ConfigPath);
}

void UMingRTSModSystem::LoadModList()
{
    FString ConfigPath = GetModsDirectory() / TEXT("modlist.json");
    
    if (!IFileManager::Get().FileExists(*ConfigPath))
    {
        return;
    }
    
    FString JsonData;
    if (!FFileHelper::LoadFileToString(JsonData, *ConfigPath))
    {
        return;
    }
    
    // 簡化的JSON解析
    // 實際應該使用正式的JSON解析器
    UE_LOG(LogTemp, Log, TEXT("Loaded mod list"));
}

void UMingRTSModSystem::RegisterMod(const FModInfo& ModInfo)
{
    InstalledMods.Add(ModInfo.ModID, ModInfo);
    SaveModList();
}

void UMingRTSModSystem::UnregisterMod(const FString& ModID)
{
    InstalledMods.Remove(ModID);
    EnabledModIDs.Remove(ModID);
    ModLoadOrder.Remove(ModID);
    SaveModList();
}

void UMingRTSModSystem::BroadcastModEvent(EModStatus NewStatus, const FString& ModID)
{
    switch (NewStatus)
    {
    case EModStatus::Enabled:
        OnModEnabled.Broadcast(ModID);
        break;
    case EModStatus::Disabled:
        OnModDisabled.Broadcast(ModID);
        break;
    default:
        break;
    }
}
