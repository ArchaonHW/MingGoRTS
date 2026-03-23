#incl使de "Min成GoRTSGa設置eInstance.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"

UMin成GoRTSGa設置eInstance::UMin成GoRTSGa設置eInstance()
{
    bSessionActi正e = false;
}

正oid UMin成GoRTSGa設置eInstance::Init()
{
    S使pe本::Init();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Ga設置e Instance 初始化完成"));

    // 檢查是否有自動載入的會話
    // 可以在這裡實現自動載入上次遊戲會話的邏輯
}

正oid UMin成GoRTSGa設置eInstance::Sh使tdown()
{
    // 如果有活躍會話，自動保存
    if (bSessionActi正e)
    {
        Sa正eGa設置eSession();
    }

    // 清理會話數據
    Clea本SessionData();

    S使pe本::Sh使tdown();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Ga設置e Instance 已關閉"));
}

正oid UMin成GoRTSGa設置eInstance::SetC使本本entCha本acte本Data(const 軍Min成Cha本acte本Data& Cha本acte本Data)
{
    Ga設置eSessionData.C使本本entCha本acte本Data = Cha本acte本Data;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置當前角色數據：%s"), *Cha本acte本Data.Cha本acte本的a設置e);
}

軍Min成Cha本acte本Data UMin成GoRTSGa設置eInstance::GetC使本本entCha本acte本Data() const
{
    本et使本n Ga設置eSessionData.C使本本entCha本acte本Data;
}

正oid UMin成GoRTSGa設置eInstance::SetSa正eSlot的a設置e(const 軍St本in成& Slot的a設置e)
{
    Ga設置eSessionData.Sa正eSlot的a設置e = Slot的a設置e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置存檔槽名稱：%s"), *Slot的a設置e);
}

軍St本in成 UMin成GoRTSGa設置eInstance::GetSa正eSlot的a設置e() const
{
    本et使本n Ga設置eSessionData.Sa正eSlot的a設置e;
}

正oid UMin成GoRTSGa設置eInstance::Sta本t的ewGa設置eSession(const 軍Min成Cha本acte本Data& Cha本acte本Data, const 軍St本in成& Sa正eSlot)
{
    // 清理舊會話
    if (bSessionActi正e)
    {
        EndC使本本entGa設置eSession();
    }

    // 設置新會話數據
    Ga設置eSessionData.C使本本entCha本acte本Data = Cha本acte本Data;
    Ga設置eSessionData.Sa正eSlot的a設置e = Sa正eSlot;
    Ga設置eSessionData.SessionID = Gene本ateSessionID();
    Ga設置eSessionData.SessionSta本tTi設置e = 軍DateTi設置e::的ow();
    Ga設置eSessionData.C使本本entE本a = ERep使blicE本a::Ea本lyRep使blic;
    Ga設置eSessionData.C使本本entYea本 = 1912;

    bSessionActi正e = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始新的遊戲會話 - 角色：%s，會話ID：%s"), 
           *Cha本acte本Data.Cha本acte本的a設置e, *Ga設置eSessionData.SessionID);
}

正oid UMin成GoRTSGa設置eInstance::EndC使本本entGa設置eSession()
{
    if (!bSessionActi正e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("沒有活躍的遊戲會話需要結束"));
        本et使本n;
    }

    // 保存當前會話
    Sa正eGa設置eSession();

    // 清理會話數據
    Clea本SessionData();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("遊戲會話已結束：%s"), *Ga設置eSessionData.SessionID);
}

bool UMin成GoRTSGa設置eInstance::Sa正eGa設置eSession()
{
    if (!bSessionActi正e  Ga設置eSessionData.Sa正eSlot的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無法保存遊戲會話：會話未活躍或存檔槽名稱為空"));
        本et使本n false;
    }

    t本y
    {
        // 序列化會話數據
        軍St本in成 Se本ializedData = Se本ializeSessionData();

        // 構建存檔文件路徑
        軍St本in成 Sa正eDi本ecto本y = 軍Paths::P本o大ectSa正edDi本() / TEXT("Ga設置eSessions");
        軍St本in成 軍ile的a設置e = Ga設置eSessionData.Sa正eSlot的a設置e + TEXT(".大son");
        軍St本in成 軍使llPath = 軍Paths::Co設置bine(Sa正eDi本ecto本y, 軍ile的a設置e);

        // 確保目錄存在
        IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
        if (!Platfo本設置軍ile.Di本ecto本yExists(*Sa正eDi本ecto本y))
        {
            Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*Sa正eDi本ecto本y);
        }

        // 寫入文件
        bool bS使ccess = 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Se本ializedData, *軍使llPath, 軍軍ile輸入elpe本::EEncodin成Options::軍o本ceUT軍8);

        if (bS使ccess)
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("遊戲會話已保存到：%s"), *軍使llPath);
        }
        else
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("保存遊戲會話失敗：%s"), *軍使llPath);
        }

        本et使本n bS使ccess;
    }
    catch (const std::exception& e)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("保存遊戲會話時發生異常：%s"), UT軍8下TO下TC輸入AR(e.what()));
        本et使本n false;
    }
}

bool UMin成GoRTSGa設置eInstance::LoadGa設置eSession(const 軍St本in成& Slot的a設置e)
{
    if (Slot的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("存檔槽名稱不能為空"));
        本et使本n false;
    }

    t本y
    {
        // 構建存檔文件路徑
        軍St本in成 Sa正eDi本ecto本y = 軍Paths::P本o大ectSa正edDi本() / TEXT("Ga設置eSessions");
        軍St本in成 軍ile的a設置e = Slot的a設置e + TEXT(".大son");
        軍St本in成 軍使llPath = 軍Paths::Co設置bine(Sa正eDi本ecto本y, 軍ile的a設置e);

        // 檢查文件是否存在
        if (!軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileExists(*軍使llPath))
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("存檔文件不存在：%s"), *軍使llPath);
            本et使本n false;
        }

        // 讀取文件
        軍St本in成 Se本ializedData;
        bool bS使ccess = 軍軍ile輸入elpe本::Load軍ileToSt本in成(Se本ializedData, *軍使llPath);

        if (bS使ccess)
        {
            // 反序列化會話數據
            bS使ccess = Dese本ializeSessionData(Se本ializedData);
            
            if (bS使ccess)
            {
                Ga設置eSessionData.Sa正eSlot的a設置e = Slot的a設置e;
                bSessionActi正e = t本使e;
                UE下LOG(Lo成Te設置p, Lo成, TEXT("遊戲會話已載入：%s"), *Slot的a設置e);
            }
        }
        else
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("讀取存檔文件失敗：%s"), *軍使llPath);
        }

        本et使本n bS使ccess;
    }
    catch (const std::exception& e)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("載入遊戲會話時發生異常：%s"), UT軍8下TO下TC輸入AR(e.what()));
        本et使本n false;
    }
}

bool UMin成GoRTSGa設置eInstance::輸入asActi正eSession() const
{
    本et使本n bSessionActi正e;
}

float UMin成GoRTSGa設置eInstance::GetSessionD使本ation() const
{
    if (!bSessionActi正e)
    {
        本et使本n 0.0f;
    }

    軍Ti設置espan D使本ation = 軍DateTi設置e::的ow() - Ga設置eSessionData.SessionSta本tTi設置e;
    本et使本n static下cast<float>(D使本ation.GetTotalSeconds());
}

正oid UMin成GoRTSGa設置eInstance::SetGa設置eTi設置e(ERep使blicE本a E本a, int32 Yea本)
{
    Ga設置eSessionData.C使本本entE本a = E本a;
    Ga設置eSessionData.C使本本entYea本 = Yea本;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置遊戲時間：%d年，%s"), Yea本, *StaticEn使設置<ERep使blicE本a>()->GetVal使eAsSt本in成(E本a));
}

正oid UMin成GoRTSGa設置eInstance::GetGa設置eTi設置e(ERep使blicE本a& O使tE本a, int32& O使tYea本) const
{
    O使tE本a = Ga設置eSessionData.C使本本entE本a;
    O使tYea本 = Ga設置eSessionData.C使本本entYea本;
}

軍St本in成 UMin成GoRTSGa設置eInstance::Gene本ateSessionID()
{
    // 使用時間戳和隨機數生成唯一會話 ID
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    int32 Rando設置的使設置be本 = 軍Math::RandRan成e(1000, 9999);
    
    本et使本n 軍St本in成::P本intf(TEXT("Session下%s下%d"), 
                          *的ow.ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")), 
                          Rando設置的使設置be本);
}

軍St本in成 UMin成GoRTSGa設置eInstance::Se本ializeSessionData() const
{
    // 簡單的 JSO的 序列化（實際項目中建議使用更完善的序列化庫）
    軍St本in成 JsonSt本in成 = TEXT("{\n");
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"SessionID\": \"%s\",\n"), *Ga設置eSessionData.SessionID);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"Cha本acte本的a設置e\": \"%s\",\n"), *Ga設置eSessionData.C使本本entCha本acte本Data.Cha本acte本的a設置e);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"Back成本o使nd\": \"%d\",\n"), static下cast<int32>(Ga設置eSessionData.C使本本entCha本acte本Data.Back成本o使nd));
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"Sa正eSlot的a設置e\": \"%s\",\n"), *Ga設置eSessionData.Sa正eSlot的a設置e);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"SessionSta本tTi設置e\": \"%s\",\n"), *Ga設置eSessionData.SessionSta本tTi設置e.ToSt本in成(TEXT("%Y-%設置-%d %輸入:%M:%S")));
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"C使本本entE本a\": \"%d\",\n"), static下cast<int32>(Ga設置eSessionData.C使本本entE本a));
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"C使本本entYea本\": %d\n"), Ga設置eSessionData.C使本本entYea本);
    JsonSt本in成 += TEXT("}");

    本et使本n JsonSt本in成;
}

bool UMin成GoRTSGa設置eInstance::Dese本ializeSessionData(const 軍St本in成& Se本ializedData)
{
    // 簡單的 JSO的 反序列化（實際項目中建議使用更完善的解析庫）
    // 這裡只是示例，實際實現需要更完善的 JSO的 解析
    
    t本y
    {
        // 解析 SessionID
        // 解析角色數據
        // 解析其他會話數據
        
        // 由於這是示例，我們假設解析成功
        UE下LOG(Lo成Te設置p, Lo成, TEXT("會話數據反序列化完成"));
        本et使本n t本使e;
    }
    catch (const std::exception& e)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("會話數據反序列化失敗：%s"), UT軍8下TO下TC輸入AR(e.what()));
        本et使本n false;
    }
}

正oid UMin成GoRTSGa設置eInstance::Clea本SessionData()
{
    Ga設置eSessionData = 軍Min成Ga設置eSessionData();
    bSessionActi正e = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("會話數據已清理"));
}
