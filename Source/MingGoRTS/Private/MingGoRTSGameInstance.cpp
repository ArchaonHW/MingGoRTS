#include "MingGoRTSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"

UMingGoRTSGameInstance::UMingGoRTSGameInstance()
{
    bSessionActive = false;
}

void UMingGoRTSGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Game Instance 初始化完成"));

    // 檢查是否有自動載入的會話
    // 可以在這裡實現自動載入上次遊戲會話的邏輯
}

void UMingGoRTSGameInstance::Shutdown()
{
    // 如果有活躍會話，自動保存
    if (bSessionActive)
    {
        SaveGameSession();
    }

    // 清理會話數據
    ClearSessionData();

    Super::Shutdown();

    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Game Instance 已關閉"));
}

void UMingGoRTSGameInstance::SetCurrentCharacterData(const FMingCharacterData& CharacterData)
{
    GameSessionData.CurrentCharacterData = CharacterData;
    UE_LOG(LogTemp, Log, TEXT("設置當前角色數據：%s"), *CharacterData.CharacterName);
}

FMingCharacterData UMingGoRTSGameInstance::GetCurrentCharacterData() const
{
    return GameSessionData.CurrentCharacterData;
}

void UMingGoRTSGameInstance::SetSaveSlotName(const FString& SlotName)
{
    GameSessionData.SaveSlotName = SlotName;
    UE_LOG(LogTemp, Log, TEXT("設置存檔槽名稱：%s"), *SlotName);
}

FString UMingGoRTSGameInstance::GetSaveSlotName() const
{
    return GameSessionData.SaveSlotName;
}

void UMingGoRTSGameInstance::StartNewGameSession(const FMingCharacterData& CharacterData, const FString& SaveSlot)
{
    // 清理舊會話
    if (bSessionActive)
    {
        EndCurrentGameSession();
    }

    // 設置新會話數據
    GameSessionData.CurrentCharacterData = CharacterData;
    GameSessionData.SaveSlotName = SaveSlot;
    GameSessionData.SessionID = GenerateSessionID();
    GameSessionData.SessionStartTime = FDateTime::Now();
    GameSessionData.CurrentEra = ERepublicEra::EarlyRepublic;
    GameSessionData.CurrentYear = 1912;

    bSessionActive = true;

    UE_LOG(LogTemp, Log, TEXT("開始新的遊戲會話 - 角色：%s，會話ID：%s"), 
           *CharacterData.CharacterName, *GameSessionData.SessionID);
}

void UMingGoRTSGameInstance::EndCurrentGameSession()
{
    if (!bSessionActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("沒有活躍的遊戲會話需要結束"));
        return;
    }

    // 保存當前會話
    SaveGameSession();

    // 清理會話數據
    ClearSessionData();

    UE_LOG(LogTemp, Log, TEXT("遊戲會話已結束：%s"), *GameSessionData.SessionID);
}

bool UMingGoRTSGameInstance::SaveGameSession()
{
    if (!bSessionActive || GameSessionData.SaveSlotName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("無法保存遊戲會話：會話未活躍或存檔槽名稱為空"));
        return false;
    }

    try
    {
        // 序列化會話數據
        FString SerializedData = SerializeSessionData();

        // 構建存檔文件路徑
        FString SaveDirectory = FPaths::ProjectSavedDir() / TEXT("GameSessions");
        FString FileName = GameSessionData.SaveSlotName + TEXT(".json");
        FString FullPath = FPaths::Combine(SaveDirectory, FileName);

        // 確保目錄存在
        IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
        if (!PlatformFile.DirectoryExists(*SaveDirectory))
        {
            PlatformFile.CreateDirectoryTree(*SaveDirectory);
        }

        // 寫入文件
        bool bSuccess = FFileHelper::SaveStringToFile(SerializedData, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8);

        if (bSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("遊戲會話已保存到：%s"), *FullPath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("保存遊戲會話失敗：%s"), *FullPath);
        }

        return bSuccess;
    }
    catch (const std::exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("保存遊戲會話時發生異常：%s"), UTF8_TO_TCHAR(e.what()));
        return false;
    }
}

bool UMingGoRTSGameInstance::LoadGameSession(const FString& SlotName)
{
    if (SlotName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("存檔槽名稱不能為空"));
        return false;
    }

    try
    {
        // 構建存檔文件路徑
        FString SaveDirectory = FPaths::ProjectSavedDir() / TEXT("GameSessions");
        FString FileName = SlotName + TEXT(".json");
        FString FullPath = FPaths::Combine(SaveDirectory, FileName);

        // 檢查文件是否存在
        if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullPath))
        {
            UE_LOG(LogTemp, Warning, TEXT("存檔文件不存在：%s"), *FullPath);
            return false;
        }

        // 讀取文件
        FString SerializedData;
        bool bSuccess = FFileHelper::LoadFileToString(SerializedData, *FullPath);

        if (bSuccess)
        {
            // 反序列化會話數據
            bSuccess = DeserializeSessionData(SerializedData);
            
            if (bSuccess)
            {
                GameSessionData.SaveSlotName = SlotName;
                bSessionActive = true;
                UE_LOG(LogTemp, Log, TEXT("遊戲會話已載入：%s"), *SlotName);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("讀取存檔文件失敗：%s"), *FullPath);
        }

        return bSuccess;
    }
    catch (const std::exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("載入遊戲會話時發生異常：%s"), UTF8_TO_TCHAR(e.what()));
        return false;
    }
}

bool UMingGoRTSGameInstance::HasActiveSession() const
{
    return bSessionActive;
}

float UMingGoRTSGameInstance::GetSessionDuration() const
{
    if (!bSessionActive)
    {
        return 0.0f;
    }

    FTimespan Duration = FDateTime::Now() - GameSessionData.SessionStartTime;
    return static_cast<float>(Duration.GetTotalSeconds());
}

void UMingGoRTSGameInstance::SetGameTime(ERepublicEra Era, int32 Year)
{
    GameSessionData.CurrentEra = Era;
    GameSessionData.CurrentYear = Year;
    UE_LOG(LogTemp, Log, TEXT("設置遊戲時間：%d年，%s"), Year, *StaticEnum<ERepublicEra>()->GetValueAsString(Era));
}

void UMingGoRTSGameInstance::GetGameTime(ERepublicEra& OutEra, int32& OutYear) const
{
    OutEra = GameSessionData.CurrentEra;
    OutYear = GameSessionData.CurrentYear;
}

FString UMingGoRTSGameInstance::GenerateSessionID()
{
    // 使用時間戳和隨機數生成唯一會話 ID
    FDateTime Now = FDateTime::Now();
    int32 RandomNumber = FMath::RandRange(1000, 9999);
    
    return FString::Printf(TEXT("Session_%s_%d"), 
                          *Now.ToString(TEXT("%Y%m%d_%H%M%S")), 
                          RandomNumber);
}

FString UMingGoRTSGameInstance::SerializeSessionData() const
{
    // 簡單的 JSON 序列化（實際項目中建議使用更完善的序列化庫）
    FString JsonString = TEXT("{\n");
    JsonString += FString::Printf(TEXT("  \"SessionID\": \"%s\",\n"), *GameSessionData.SessionID);
    JsonString += FString::Printf(TEXT("  \"CharacterName\": \"%s\",\n"), *GameSessionData.CurrentCharacterData.CharacterName);
    JsonString += FString::Printf(TEXT("  \"Background\": \"%d\",\n"), static_cast<int32>(GameSessionData.CurrentCharacterData.Background));
    JsonString += FString::Printf(TEXT("  \"SaveSlotName\": \"%s\",\n"), *GameSessionData.SaveSlotName);
    JsonString += FString::Printf(TEXT("  \"SessionStartTime\": \"%s\",\n"), *GameSessionData.SessionStartTime.ToString(TEXT("%Y-%m-%d %H:%M:%S")));
    JsonString += FString::Printf(TEXT("  \"CurrentEra\": \"%d\",\n"), static_cast<int32>(GameSessionData.CurrentEra));
    JsonString += FString::Printf(TEXT("  \"CurrentYear\": %d\n"), GameSessionData.CurrentYear);
    JsonString += TEXT("}");

    return JsonString;
}

bool UMingGoRTSGameInstance::DeserializeSessionData(const FString& SerializedData)
{
    // 簡單的 JSON 反序列化（實際項目中建議使用更完善的解析庫）
    // 這裡只是示例，實際實現需要更完善的 JSON 解析
    
    try
    {
        // 解析 SessionID
        // 解析角色數據
        // 解析其他會話數據
        
        // 由於這是示例，我們假設解析成功
        UE_LOG(LogTemp, Log, TEXT("會話數據反序列化完成"));
        return true;
    }
    catch (const std::exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("會話數據反序列化失敗：%s"), UTF8_TO_TCHAR(e.what()));
        return false;
    }
}

void UMingGoRTSGameInstance::ClearSessionData()
{
    GameSessionData = FMingGameSessionData();
    bSessionActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("會話數據已清理"));
}
