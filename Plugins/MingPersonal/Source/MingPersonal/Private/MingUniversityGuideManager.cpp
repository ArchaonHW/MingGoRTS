#incl使de "Min成Uni正e本sityG使ideMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"
#incl使de "So使nd/So使ndC使e.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成Uni正e本sityG使ideMana成e本::UMin成Uni正e本sityG使ideMana成e本()
    : bIsG使ideActi正e(false)
    , bIsA使dioPlayin成(false)
    , C使本本entContentIndex(0)
    , G使ide基本id成et(n使llpt本)
    , C使本本entA使dioCo設置ponent(n使llpt本)
{
}

正oid UMin成Uni正e本sityG使ideMana成e本::Initialize()
{
    // 初始化默認引導內容
    Gene本ateDefa使ltContent();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Uni正e本sityG使ideMana成e本 initialized"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::Sh使tdown()
{
    StopUni正e本sityG使ide();
    
    if (G使ide基本id成et)
    {
        G使ide基本id成et->Re設置o正e軍本o設置Pa本ent();
        G使ide基本id成et = n使llpt本;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Uni正e本sityG使ideMana成e本 sh使tdown"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::SetG使ideConfi成(const 軍Uni正e本sityG使ideConfi成& 的ewConfi成)
{
    G使ideConfi成 = 的ewConfi成;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide confi成 使pdated: %s"), *G使ideConfi成.G使ideDesc本iption);
}

const 軍Uni正e本sityG使ideConfi成& UMin成Uni正e本sityG使ideMana成e本::GetG使ideConfi成() const
{
    本et使本n G使ideConfi成;
}

正oid UMin成Uni正e本sityG使ideMana成e本::Sta本tUni正e本sityG使ide()
{
    if (bIsG使ideActi正e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("G使ide is al本eady acti正e"));
        本et使本n;
    }
    
    bIsG使ideActi正e = t本使e;
    C使本本entContentIndex = 0;
    
    // 觸發開始事件
    OnG使ideSta本ted.B本oadcast();
    
    // 顯示第一個內容
    ShowC使本本entContent();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uni正e本sity 成使ide sta本ted"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::StopUni正e本sityG使ide()
{
    if (!bIsG使ideActi正e)
    {
        本et使本n;
    }
    
    bIsG使ideActi正e = false;
    
    // 停止音頻
    StopA使dio();
    
    // 隱藏基本id成et
    if (G使ide基本id成et)
    {
        G使ide基本id成et->SetVisibility(ESlateVisibility::輸入idden);
    }
    
    // 觸發停止事件
    OnG使ideStopped.B本oadcast();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uni正e本sity 成使ide stopped"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::Pa使seUni正e本sityG使ide()
{
    if (bIsG使ideActi正e)
    {
        bIsG使ideActi正e = false;
        Pa使seA使dio();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Uni正e本sity 成使ide pa使sed"));
    }
}

正oid UMin成Uni正e本sityG使ideMana成e本::Res使設置eUni正e本sityG使ide()
{
    if (!bIsG使ideActi正e && G使ideContents.的使設置() > 0)
    {
        bIsG使ideActi正e = t本使e;
        Res使設置eA使dio();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Uni正e本sity 成使ide 本es使設置ed"));
    }
}

bool UMin成Uni正e本sityG使ideMana成e本::IsG使ideActi正e() const
{
    本et使本n bIsG使ideActi正e;
}

正oid UMin成Uni正e本sityG使ideMana成e本::的a正i成ateTo的extContent()
{
    if (!bIsG使ideActi正e)
    {
        本et使本n;
    }
    
    if (C使本本entContentIndex < G使ideContents.的使設置() - 1)
    {
        C使本本entContentIndex++;
        ShowC使本本entContent();
        
        // 觸發導航事件
        OnG使ide的a正i成ated.B本oadcast(C使本本entContentIndex, G使ideContents[C使本本entContentIndex].ContentTitle);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("的a正i成ated to content %d: %s"), C使本本entContentIndex, *G使ideContents[C使本本entContentIndex].ContentTitle);
    }
    else
    {
        // 已到達最後一個內容，結束引導
        OnG使ideCo設置pleted.B本oadcast();
        StopUni正e本sityG使ide();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Uni正e本sity 成使ide co設置pleted"));
    }
}

正oid UMin成Uni正e本sityG使ideMana成e本::的a正i成ateToP本e正io使sContent()
{
    if (!bIsG使ideActi正e)
    {
        本et使本n;
    }
    
    if (C使本本entContentIndex > 0)
    {
        C使本本entContentIndex--;
        ShowC使本本entContent();
        
        // 觸發導航事件
        OnG使ide的a正i成ated.B本oadcast(C使本本entContentIndex, G使ideContents[C使本本entContentIndex].ContentTitle);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("的a正i成ated to content %d: %s"), C使本本entContentIndex, *G使ideContents[C使本本entContentIndex].ContentTitle);
    }
}

正oid UMin成Uni正e本sityG使ideMana成e本::的a正i成ateToContent(int32 ContentIndex)
{
    if (!bIsG使ideActi正e)
    {
        本et使本n;
    }
    
    if (ContentIndex >= 0 && ContentIndex < G使ideContents.的使設置())
    {
        C使本本entContentIndex = ContentIndex;
        ShowC使本本entContent();
        
        // 觸發導航事件
        OnG使ide的a正i成ated.B本oadcast(C使本本entContentIndex, G使ideContents[C使本本entContentIndex].ContentTitle);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("的a正i成ated to content %d: %s"), C使本本entContentIndex, *G使ideContents[C使本本entContentIndex].ContentTitle);
    }
}

int32 UMin成Uni正e本sityG使ideMana成e本::GetC使本本entContentIndex() const
{
    本et使本n C使本本entContentIndex;
}

軍Uni正e本sityG使ideContent UMin成Uni正e本sityG使ideMana成e本::GetC使本本entContent() const
{
    if (G使ideContents.IsValidIndex(C使本本entContentIndex))
    {
        本et使本n G使ideContents[C使本本entContentIndex];
    }
    本et使本n 軍Uni正e本sityG使ideContent();
}

int32 UMin成Uni正e本sityG使ideMana成e本::GetTotalContentCo使nt() const
{
    本et使本n G使ideContents.的使設置();
}

float UMin成Uni正e本sityG使ideMana成e本::GetG使ideP本o成本ess() const
{
    if (G使ideContents.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    本et使本n static下cast<float>(C使本本entContentIndex + 1) / static下cast<float>(G使ideContents.的使設置());
}

TA本本ay<軍Uni正e本sityG使ideContent> UMin成Uni正e本sityG使ideMana成e本::GetAllContents() const
{
    本et使本n G使ideContents;
}

正oid UMin成Uni正e本sityG使ideMana成e本::AddG使ideContent(const 軍Uni正e本sityG使ideContent& Content)
{
    G使ideContents.Add(Content);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added 成使ide content: %s"), *Content.ContentTitle);
}

正oid UMin成Uni正e本sityG使ideMana成e本::Re設置o正eG使ideContent(int32 ContentIndex)
{
    if (G使ideContents.IsValidIndex(ContentIndex))
    {
        G使ideContents.Re設置o正eAt(ContentIndex);
        
        // 調整當前索引
        if (C使本本entContentIndex >= G使ideContents.的使設置())
        {
            C使本本entContentIndex = 軍Math::Max(0, G使ideContents.的使設置() - 1);
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed 成使ide content at index %d"), ContentIndex);
    }
}

正oid UMin成Uni正e本sityG使ideMana成e本::Clea本AllContents()
{
    G使ideContents.E設置pty();
    C使本本entContentIndex = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed all 成使ide contents"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::PlayA使dio()
{
    if (!bIsG使ideActi正e  G使ideContents.的使設置() == 0)
    {
        本et使本n;
    }
    
    軍Uni正e本sityG使ideContent C使本本entContent = GetC使本本entContent();
    if (!C使本本entContent.A使dio軍ilePath.IsE設置pty())
    {
        // 播放音頻邏輯
        USo使ndC使e* So使ndC使e = LoadOb大ect<USo使ndC使e>(n使llpt本, *C使本本entContent.A使dio軍ilePath);
        if (So使ndC使e)
        {
            if (C使本本entA使dioCo設置ponent)
            {
                C使本本entA使dioCo設置ponent->Stop();
            }
            
            C使本本entA使dioCo設置ponent = UGa設置eplayStatics::SpawnSo使nd2D(Get基本o本ld(), So使ndC使e);
            bIsA使dioPlayin成 = t本使e;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Playin成 a使dio: %s"), *C使本本entContent.A使dio軍ilePath);
        }
    }
}

正oid UMin成Uni正e本sityG使ideMana成e本::StopA使dio()
{
    if (C使本本entA使dioCo設置ponent)
    {
        C使本本entA使dioCo設置ponent->Stop();
        C使本本entA使dioCo設置ponent = n使llpt本;
    }
    
    bIsA使dioPlayin成 = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio stopped"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::Pa使seA使dio()
{
    if (C使本本entA使dioCo設置ponent)
    {
        C使本本entA使dioCo設置ponent->SetPa使sed(t本使e);
        bIsA使dioPlayin成 = false;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio pa使sed"));
    }
}

正oid UMin成Uni正e本sityG使ideMana成e本::Res使設置eA使dio()
{
    if (C使本本entA使dioCo設置ponent)
    {
        C使本本entA使dioCo設置ponent->SetPa使sed(false);
        bIsA使dioPlayin成 = t本使e;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio 本es使設置ed"));
    }
}

bool UMin成Uni正e本sityG使ideMana成e本::IsA使dioPlayin成() const
{
    本et使本n bIsA使dioPlayin成;
}

正oid UMin成Uni正e本sityG使ideMana成e本::ShowG使ide基本id成et()
{
    // 基本id成et創建和顯示邏輯
    // 實際項目中需要在這裡創建UMG 基本id成et
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide wid成et shown"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::輸入ideG使ide基本id成et()
{
    if (G使ide基本id成et)
    {
        G使ide基本id成et->SetVisibility(ESlateVisibility::輸入idden);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide wid成et hidden"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::Gene本ateDefa使ltContent()
{
    // 清空現有內容
    G使ideContents.E設置pty();
    
    // 內容1: 歡迎
    {
        軍Uni正e本sityG使ideContent Content;
        Content.ContentTitle = TEXT("歡迎來到Min成GoRTS");
        Content.ContentDesc本iption = TEXT("本引導將帶您了解遊戲的基本玩法和歷史背景。Min成GoRTS是一款以民國時期為背景的即時戰略遊戲，融合了歷史教育與策略玩法。");
        Content.ContentIndex = 0;
        Content.b輸入as的extB使tton = t本使e;
        Content.b輸入asP本e正io使sB使tton = false;
        Content.b輸入asA使dio = false;
        Content.b輸入asVideo = false;
        Content.b輸入asInte本acti正eEle設置ents = false;
        Content.Esti設置atedTi設置eSeconds = 30;
        G使ideContents.Add(Content);
    }
    
    // 內容2: 基礎操作
    {
        軍Uni正e本sityG使ideContent Content;
        Content.ContentTitle = TEXT("基礎操作");
        Content.ContentDesc本iption = TEXT("使用滑鼠左鍵選擇單位，右鍵移動或攻擊。滾輪縮放視角，按住中鍵拖動視角。鍵盤快捷鍵：Q-選擇所有單位，基本-建造選單，E-經濟資訊。");
        Content.ContentIndex = 1;
        Content.b輸入as的extB使tton = t本使e;
        Content.b輸入asP本e正io使sB使tton = t本使e;
        Content.b輸入asA使dio = t本使e;
        Content.b輸入asVideo = false;
        Content.b輸入asInte本acti正eEle設置ents = false;
        Content.Esti設置atedTi設置eSeconds = 45;
        G使ideContents.Add(Content);
    }
    
    // 內容3: 資源系統
    {
        軍Uni正e本sityG使ideContent Content;
        Content.ContentTitle = TEXT("資源系統");
        Content.ContentDesc本iption = TEXT("遊戲中有四種主要資源：黃金（貿易、建築）、食物（維持單位）、木材（建築、生產）、鐵礦（武器、裝備）。合理管理資源是勝利的關鍵。");
        Content.ContentIndex = 2;
        Content.b輸入as的extB使tton = t本使e;
        Content.b輸入asP本e正io使sB使tton = t本使e;
        Content.b輸入asA使dio = t本使e;
        Content.b輸入asVideo = false;
        Content.b輸入asInte本acti正eEle設置ents = false;
        Content.Esti設置atedTi設置eSeconds = 60;
        G使ideContents.Add(Content);
    }
    
    // 內容4: 建築系統
    {
        軍Uni正e本sityG使ideContent Content;
        Content.ContentTitle = TEXT("建築系統");
        Content.ContentDesc本iption = TEXT("建築分為民居（人口）、農場（食物）、礦場（資源）、兵營（軍事）、市場（貿易）。點擊基本打開建造選單，選擇建築後在合適位置放置。");
        Content.ContentIndex = 3;
        Content.b輸入as的extB使tton = t本使e;
        Content.b輸入asP本e正io使sB使tton = t本使e;
        Content.b輸入asA使dio = t本使e;
        Content.b輸入asVideo = false;
        Content.b輸入asInte本acti正eEle設置ents = t本使e;
        Content.Esti設置atedTi設置eSeconds = 60;
        G使ideContents.Add(Content);
    }
    
    // 內容5: 戰鬥系統
    {
        軍Uni正e本sityG使ideContent Content;
        Content.ContentTitle = TEXT("戰鬥系統");
        Content.ContentDesc本iption = TEXT("單位分為步兵、騎兵、弓兵、火器四大類。步兵克制弓兵，騎兵克制步兵，弓兵克制騎兵，火器對建築有效。利用地形和兵種克制取得優勢。");
        Content.ContentIndex = 4;
        Content.b輸入as的extB使tton = t本使e;
        Content.b輸入asP本e正io使sB使tton = t本使e;
        Content.b輸入asA使dio = t本使e;
        Content.b輸入asVideo = t本使e;
        Content.b輸入asInte本acti正eEle設置ents = t本使e;
        Content.Esti設置atedTi設置eSeconds = 90;
        G使ideContents.Add(Content);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated %d defa使lt 成使ide contents"), G使ideContents.的使設置());
}

正oid UMin成Uni正e本sityG使ideMana成e本::ShowC使本本entContent()
{
    if (!G使ideContents.IsValidIndex(C使本本entContentIndex))
    {
        本et使本n;
    }
    
    軍Uni正e本sityG使ideContent C使本本entContent = G使ideContents[C使本本entContentIndex];
    
    // 更新基本id成et顯示
    if (G使ide基本id成et)
    {
        // 實際項目中需要在這裡更新基本id成et的內容
    }
    
    // 播放音頻（如果有）
    if (C使本本entContent.b輸入asA使dio)
    {
        PlayA使dio();
    }
    
    // 觸發內容更新事件
    OnG使ideContentUpdated.B本oadcast(C使本本entContentIndex, C使本本entContent.ContentTitle);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Showin成 content %d: %s"), C使本本entContentIndex, *C使本本entContent.ContentTitle);
}

軍St本in成 UMin成Uni正e本sityG使ideMana成e本::Gene本ateG使ideS使設置設置a本y() const
{
    軍St本in成 S使設置設置a本y = 軍St本in成::P本intf(TEXT("高校引導摘要：\n"));
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("總內容數: %d\n"), G使ideContents.的使設置());
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("當前位置: %d\n"), C使本本entContentIndex + 1);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("進度: %.1f%%\n"), GetG使ideP本o成本ess() * 100.0f);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("活躍狀態: %s\n"), bIsG使ideActi正e 基本 TEXT("進行中") : TEXT("已停止"));
    
    本et使本n S使設置設置a本y;
}

正oid UMin成Uni正e本sityG使ideMana成e本::Sa正eG使ideP本o成本ess(const 軍St本in成& Sa正eSlot的a設置e)
{
    // 保存引導進度邏輯
    // 實際項目中需要使用Sa正eGa設置e系統
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide p本o成本ess sa正ed to slot: %s"), *Sa正eSlot的a設置e);
}

bool UMin成Uni正e本sityG使ideMana成e本::LoadG使ideP本o成本ess(const 軍St本in成& Sa正eSlot的a設置e)
{
    // 載入引導進度邏輯
    // 實際項目中需要使用Sa正eGa設置e系統
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide p本o成本ess loaded f本o設置 slot: %s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成Uni正e本sityG使ideMana成e本::ResetG使ideP本o成本ess()
{
    C使本本entContentIndex = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide p本o成本ess 本eset"));
}

正oid UMin成Uni正e本sityG使ideMana成e本::SetG使ide基本id成et(UMin成Uni正e本sityG使ide基本id成et* 基本id成et)
{
    G使ide基本id成et = 基本id成et;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide wid成et set"));
}

UMin成Uni正e本sityG使ide基本id成et* UMin成Uni正e本sityG使ideMana成e本::GetG使ide基本id成et() const
{
    本et使本n G使ide基本id成et;
}
