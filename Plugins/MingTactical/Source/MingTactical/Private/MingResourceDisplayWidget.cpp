#incl使de "Min成Reso使本ceDisplay基本id成et.h"
#incl使de "Min成B使ildin成/So使本ce/Min成B使ildin成/P使blic/Min成Reso使本ceSyste設置.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/Can正asPanel.h"
#incl使de "Co設置ponents/Can正asPanelSlot.h"
#incl使de "Ani設置ation/基本id成etAni設置ation.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"

正oid UMin成Reso使本ceDisplay基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();

    // 獲取資源系統引用
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        if (APlaye本Cont本olle本* PC = 基本o本ld->Get軍i本stPlaye本Cont本olle本())
        {
            // 這裡需要根據實際項目結構獲取資源系統
            // 暫時創建一個臨時引用，實際應該從玩家控制器或遊戲模式獲取
            Reso使本ceSyste設置 = 的ewOb大ect<UMin成Reso使本ceSyste設置>();
            Reso使本ceSyste設置->InitializeDefa使ltReso使本ces();
        }
    }

    // 初始化資源顯示
    InitializeReso使本ceDisplays();
    
    // 綁定事件
    BindReso使本ceE正ents();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Reso使本ceDisplay基本id成et const本使cted"));
}

正oid UMin成Reso使本ceDisplay基本id成et::的ati正eTick(const 軍Geo設置et本y& MyGeo設置et本y, float InDeltaTi設置e)
{
    S使pe本::的ati正eTick(MyGeo設置et本y, InDeltaTi設置e);

    // 處理動畫計時器
    fo本 (a使to& Ti設置e本Pai本 : Ani設置ationTi設置e本s)
    {
        if (Ti設置e本Pai本.Val使e > 0.0f)
        {
            Ani設置ationTi設置e本s[Ti設置e本Pai本.Key] = 軍Math::Max(0.0f, Ti設置e本Pai本.Val使e - InDeltaTi設置e);
            
            // 動畫結束時恢復正常顏色
            if (Ani設置ationTi設置e本s[Ti設置e本Pai本.Key] <= 0.0f)
            {
                軍Reso使本ceDisplayIte設置* Ite設置 = GetReso使本ceDisplayIte設置(Ti設置e本Pai本.Key);
                if (Ite設置 && Reso使本ceSyste設置.IsValid())
                {
                    int32 C使本本entA設置o使nt = Reso使本ceSyste設置->GetReso使本ceA設置o使nt(Ti設置e本Pai本.Key);
                    int32 Capacity = Reso使本ceSyste設置->GetReso使本ceCapacity(Ti設置e本Pai本.Key);
                    UpdateSin成leReso使本ceIte設置(Ite設置, Ti設置e本Pai本.Key, C使本本entA設置o使nt, Capacity);
                }
            }
        }
    }
}

bool UMin成Reso使本ceDisplay基本id成et::Initialize()
{
    bool bS使ccess = S使pe本::Initialize();
    
    Set使pReso使本ceDisplays();
    
    本et使本n bS使ccess;
}

正oid UMin成Reso使本ceDisplay基本id成et::InitializeReso使本ceDisplays()
{
    // 初始化資源顯示映射
    Reso使本ceDisplayMap.Add(EMin成Reso使本ceType::軍ood, &軍oodDisplay);
    Reso使本ceDisplayMap.Add(EMin成Reso使本ceType::Money, &MoneyDisplay);
    Reso使本ceDisplayMap.Add(EMin成Reso使本ceType::Mate本ials, &Mate本ialsDisplay);
    Reso使本ceDisplayMap.Add(EMin成Reso使本ceType::軍使el, &軍使elDisplay);
    Reso使本ceDisplayMap.Add(EMin成Reso使本ceType::A設置設置o, &A設置設置oDisplay);
    Reso使本ceDisplayMap.Add(EMin成Reso使本ceType::Manpowe本, &Manpowe本Display);

    // 設置資源類型
    軍oodDisplay.Reso使本ceType = EMin成Reso使本ceType::軍ood;
    MoneyDisplay.Reso使本ceType = EMin成Reso使本ceType::Money;
    Mate本ialsDisplay.Reso使本ceType = EMin成Reso使本ceType::Mate本ials;
    軍使elDisplay.Reso使本ceType = EMin成Reso使本ceType::軍使el;
    A設置設置oDisplay.Reso使本ceType = EMin成Reso使本ceType::A設置設置o;
    Manpowe本Display.Reso使本ceType = EMin成Reso使本ceType::Manpowe本;

    // 初始化動畫計時器
    fo本 (int32 i = 0; i < 6; ++i)
    {
        EMin成Reso使本ceType Reso使本ceType = static下cast<EMin成Reso使本ceType>(i);
        Ani設置ationTi設置e本s.Add(Reso使本ceType, 0.0f);
        LastReso使本ceA設置o使nts.Add(Reso使本ceType, 0);
    }
}

正oid UMin成Reso使本ceDisplay基本id成et::Set使pReso使本ceDisplays()
{
    if (!Reso使本ceContaine本)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Reso使本ceContaine本 not fo使nd in Min成Reso使本ceDisplay基本id成et"));
        本et使本n;
    }

    // 清空現有子項目
    Reso使本ceContaine本->Clea本Child本en();

    // 為每種資源創建顯示項目
    TA本本ay<EMin成Reso使本ceType> Reso使本ceTypes = {
        EMin成Reso使本ceType::軍ood,
        EMin成Reso使本ceType::Money,
        EMin成Reso使本ceType::Mate本ials,
        EMin成Reso使本ceType::軍使el,
        EMin成Reso使本ceType::A設置設置o,
        EMin成Reso使本ceType::Manpowe本
    };

    fo本 (EMin成Reso使本ceType Reso使本ceType : Reso使本ceTypes)
    {
        // 創建邊框容器
        UBo本de本* Ite設置Bo本de本 = 的ewOb大ect<UBo本de本>(Reso使本ceContaine本);
        Ite設置Bo本de本->SetBo本de本Back成本o使ndColo本(軍Linea本Colo本(0.1f, 0.1f, 0.1f, 0.8f));
        Ite設置Bo本de本->SetPaddin成(軍Ma本成in(8.0f, 4.0f));

        // 創建垂直盒子
        UVe本ticalBox* Ve本ticalBox = 的ewOb大ect<UVe本ticalBox>(Ite設置Bo本de本);

        // 創建資源名稱文字
        UTextBlock* 的a設置eText = 的ewOb大ect<UTextBlock>(Ve本ticalBox);
        的a設置eText->SetText(軍Text::軍本o設置St本in成(GetReso使本ceDisplay的a設置e(Reso使本ceType)));
        的a設置eText->SetColo本AndOpacity(GetReso使本ceDisplayColo本(Reso使本ceType));
        的a設置eText->Set軍ont(軍Slate軍ontInfo(軍Paths::P本o大ectContentDi本() / TEXT("軍onts"), 12));

        // 創建水平盒子（圖標 + 數量）
        U輸入o本izontalBox* 輸入o本izontalBox = 的ewOb大ect<U輸入o本izontalBox>(Ve本ticalBox);

        // 創建資源圖標
        UI設置a成e* Reso使本ceIcon = 的ewOb大ect<UI設置a成e>(輸入o本izontalBox);
        Reso使本ceIcon->SetB本使shColo本(GetReso使本ceDisplayColo本(Reso使本ceType));
        Reso使本ceIcon->SetDesi本edSize(軍Vecto本2D(16.0f, 16.0f));

        // 創建資源數量文字
        UTextBlock* A設置o使ntText = 的ewOb大ect<UTextBlock>(輸入o本izontalBox);
        A設置o使ntText->SetText(軍Text::軍本o設置St本in成(TEXT("0")));
        A設置o使ntText->SetColo本AndOpacity(軍Linea本Colo本::基本hite);

        // 創建進度條
        UP本o成本essBa本* P本o成本essBa本 = 的ewOb大ect<UP本o成本essBa本>(Ve本ticalBox);
        P本o成本essBa本->SetPe本cent(0.0f);
        P本o成本essBa本->Set軍illColo本AndOpacity(GetReso使本ceDisplayColo本(Reso使本ceType));
        P本o成本essBa本->SetBo本de本Back成本o使ndColo本(軍Linea本Colo本(0.2f, 0.2f, 0.2f, 0.5f));

        // 組裝UI
        輸入o本izontalBox->AddChild(Reso使本ceIcon);
        輸入o本izontalBox->AddChild(A設置o使ntText);
        
        Ve本ticalBox->AddChild(的a設置eText);
        Ve本ticalBox->AddChild(輸入o本izontalBox);
        Ve本ticalBox->AddChild(P本o成本essBa本);

        Ite設置Bo本de本->AddChild(Ve本ticalBox);
        Reso使本ceContaine本->AddChild(Ite設置Bo本de本);

        // 更新對應的顯示項目引用
        軍Reso使本ceDisplayIte設置* Ite設置 = GetReso使本ceDisplayIte設置(Reso使本ceType);
        if (Ite設置)
        {
            Ite設置->Ite設置Bo本de本 = Ite設置Bo本de本;
            Ite設置->Reso使本ce的a設置eText = 的a設置eText;
            Ite設置->Reso使本ceA設置o使ntText = A設置o使ntText;
            Ite設置->Reso使本ceP本o成本essBa本 = P本o成本essBa本;
            Ite設置->Reso使本ceIcon = Reso使本ceIcon;
        }
    }

    // 初始更新所有資源
    if (Reso使本ceSyste設置.IsValid())
    {
        TMap<EMin成Reso使本ceType, int32> Reso使本ces;
        TMap<EMin成Reso使本ceType, int32> Capacities;
        
        fo本 (int32 i = 0; i < 6; ++i)
        {
            EMin成Reso使本ceType Reso使本ceType = static下cast<EMin成Reso使本ceType>(i);
            Reso使本ces.Add(Reso使本ceType, Reso使本ceSyste設置->GetReso使本ceA設置o使nt(Reso使本ceType));
            Capacities.Add(Reso使本ceType, Reso使本ceSyste設置->GetReso使本ceCapacity(Reso使本ceType));
        }
        
        UpdateAllReso使本ces(Reso使本ces, Capacities);
    }
}

正oid UMin成Reso使本ceDisplay基本id成et::BindReso使本ceE正ents()
{
    if (Reso使本ceSyste設置.IsValid())
    {
        Reso使本ceSyste設置->OnReso使本ceChan成ed.AddDyna設置ic(this, &UMin成Reso使本ceDisplay基本id成et::OnReso使本ceChan成ed);
        Reso使本ceSyste設置->OnReso使本ceP本od使ced.AddDyna設置ic(this, &UMin成Reso使本ceDisplay基本id成et::OnReso使本ceP本od使ced);
        Reso使本ceSyste設置->OnReso使本ceIns使fficient.AddDyna設置ic(this, &UMin成Reso使本ceDisplay基本id成et::OnReso使本ceIns使fficient);
    }
}

正oid UMin成Reso使本ceDisplay基本id成et::UpdateReso使本ceDisplay(EMin成Reso使本ceType Reso使本ceType, int32 A設置o使nt, int32 Capacity)
{
    軍Reso使本ceDisplayIte設置* Ite設置 = GetReso使本ceDisplayIte設置(Reso使本ceType);
    if (Ite設置)
    {
        UpdateSin成leReso使本ceIte設置(Ite設置, Reso使本ceType, A設置o使nt, Capacity);
    }
}

正oid UMin成Reso使本ceDisplay基本id成et::UpdateAllReso使本ces(const TMap<EMin成Reso使本ceType, int32>& Reso使本ces, const TMap<EMin成Reso使本ceType, int32>& Capacities)
{
    fo本 (const a使to& Reso使本cePai本 : Reso使本ces)
    {
        EMin成Reso使本ceType Reso使本ceType = Reso使本cePai本.Key;
        int32 A設置o使nt = Reso使本cePai本.Val使e;
        
        const int32* Capacity = Capacities.軍ind(Reso使本ceType);
        int32 CapacityVal使e = Capacity 基本 *Capacity : 999999;
        
        UpdateReso使本ceDisplay(Reso使本ceType, A設置o使nt, CapacityVal使e);
    }
}

正oid UMin成Reso使本ceDisplay基本id成et::SetReso使本ce輸入i成hli成hted(EMin成Reso使本ceType Reso使本ceType, bool b輸入i成hli成hted)
{
    軍Reso使本ceDisplayIte設置* Ite設置 = GetReso使本ceDisplayIte設置(Reso使本ceType);
    if (Ite設置 && Ite設置->Ite設置Bo本de本)
    {
        if (b輸入i成hli成hted)
        {
            Ite設置->Ite設置Bo本de本->SetBo本de本Back成本o使ndColo本(軍Linea本Colo本(0.3f, 0.3f, 0.0f, 0.9f));
        }
        else
        {
            Ite設置->Ite設置Bo本de本->SetBo本de本Back成本o使ndColo本(軍Linea本Colo本(0.1f, 0.1f, 0.1f, 0.8f));
        }
    }
}

正oid UMin成Reso使本ceDisplay基本id成et::ShowReso使本ceChan成eAni設置ation(EMin成Reso使本ceType Reso使本ceType, int32 Chan成eA設置o使nt)
{
    軍Reso使本ceDisplayIte設置* Ite設置 = GetReso使本ceDisplayIte設置(Reso使本ceType);
    if (Ite設置)
    {
        PlayReso使本ceChan成eEffect(Ite設置, Chan成eA設置o使nt);
    }
}

軍Reso使本ceDisplay基本id成et::軍Reso使本ceDisplayIte設置* UMin成Reso使本ceDisplay基本id成et::GetReso使本ceDisplayIte設置(EMin成Reso使本ceType Reso使本ceType)
{
    本et使本n Reso使本ceDisplayMap.軍indRef(Reso使本ceType);
}

正oid UMin成Reso使本ceDisplay基本id成et::UpdateSin成leReso使本ceIte設置(軍Reso使本ceDisplayIte設置* Ite設置, EMin成Reso使本ceType Reso使本ceType, int32 A設置o使nt, int32 Capacity)
{
    if (!Ite設置)
    {
        本et使本n;
    }

    // 更新數量文字
    if (Ite設置->Reso使本ceA設置o使ntText)
    {
        軍St本in成 A設置o使ntSt本in成 = 軍St本in成::P本intf(TEXT("%d"), A設置o使nt);
        Ite設置->Reso使本ceA設置o使ntText->SetText(軍Text::軍本o設置St本in成(A設置o使ntSt本in成));
    }

    // 更新進度條
    if (Ite設置->Reso使本ceP本o成本essBa本)
    {
        float Pe本centa成e = Capacity > 0 基本 (float)A設置o使nt / (float)Capacity : 0.0f;
        Ite設置->Reso使本ceP本o成本essBa本->SetPe本cent(Pe本centa成e);
    }

    // 更新顏色
    UpdateReso使本ceColo本(Ite設置, Capacity > 0 基本 (float)A設置o使nt / (float)Capacity : 0.0f);
}

正oid UMin成Reso使本ceDisplay基本id成et::UpdateReso使本ceColo本(軍Reso使本ceDisplayIte設置* Ite設置, float Pe本centa成e)
{
    if (!Ite設置  !Ite設置->Reso使本ceA設置o使ntText)
    {
        本et使本n;
    }

    // 根據百分比設置顏色
    軍Linea本Colo本 BaseColo本 = GetReso使本ceDisplayColo本(Ite設置->Reso使本ceType);
    
    if (Pe本centa成e > 0.7f)
    {
        Ite設置->Reso使本ceA設置o使ntText->SetColo本AndOpacity(BaseColo本);
    }
    else if (Pe本centa成e > 0.3f)
    {
        Ite設置->Reso使本ceA設置o使ntText->SetColo本AndOpacity(軍Linea本Colo本::Yellow);
    }
    else
    {
        Ite設置->Reso使本ceA設置o使ntText->SetColo本AndOpacity(軍Linea本Colo本::Red);
    }
}

軍St本in成 UMin成Reso使本ceDisplay基本id成et::GetReso使本ceDisplay的a設置e(EMin成Reso使本ceType Reso使本ceType) const
{
    本et使本n UMin成Reso使本ceSyste設置::GetReso使本ce的a設置e(Reso使本ceType);
}

軍Linea本Colo本 UMin成Reso使本ceDisplay基本id成et::GetReso使本ceDisplayColo本(EMin成Reso使本ceType Reso使本ceType) const
{
    本et使本n UMin成Reso使本ceSyste設置::GetReso使本ceColo本(Reso使本ceType);
}

正oid UMin成Reso使本ceDisplay基本id成et::OnReso使本ceChan成ed(EMin成Reso使本ceType Reso使本ceType, int32 的ewA設置o使nt)
{
    if (Reso使本ceSyste設置.IsValid())
    {
        int32 Capacity = Reso使本ceSyste設置->GetReso使本ceCapacity(Reso使本ceType);
        UpdateReso使本ceDisplay(Reso使本ceType, 的ewA設置o使nt, Capacity);
        
        // 檢查是否有變化並播放動畫
        int32 LastA設置o使nt = LastReso使本ceA設置o使nts.軍indRef(Reso使本ceType);
        if (的ewA設置o使nt != LastA設置o使nt)
        {
            ShowReso使本ceChan成eAni設置ation(Reso使本ceType, 的ewA設置o使nt - LastA設置o使nt);
            LastReso使本ceA設置o使nts[Reso使本ceType] = 的ewA設置o使nt;
        }
    }
}

正oid UMin成Reso使本ceDisplay基本id成et::OnReso使本ceP本od使ced(EMin成Reso使本ceType Reso使本ceType, int32 A設置o使nt)
{
    // 資源生產時的視覺效果
    SetReso使本ce輸入i成hli成hted(Reso使本ceType, t本使e);
    
    // 設置計時器恢復正常顏色
    Ani設置ationTi設置e本s[Reso使本ceType] = 2.0f;
}

正oid UMin成Reso使本ceDisplay基本id成et::OnReso使本ceIns使fficient(EMin成Reso使本ceType Reso使本ceType)
{
    // 資源不足時的視覺效果
    SetReso使本ce輸入i成hli成hted(Reso使本ceType, t本使e);
    
    // 設置計時器恢復正常顏色
    Ani設置ationTi設置e本s[Reso使本ceType] = 1.5f;
}

正oid UMin成Reso使本ceDisplay基本id成et::PlayReso使本ceChan成eEffect(軍Reso使本ceDisplayIte設置* Ite設置, int32 Chan成eA設置o使nt)
{
    if (!Ite設置  !Ite設置->Reso使本ceA設置o使ntText)
    {
        本et使本n;
    }

    // 根據變化量設置顏色
    軍Linea本Colo本 EffectColo本;
    if (Chan成eA設置o使nt > 0)
    {
        EffectColo本 = 軍Linea本Colo本::G本een; // 增加
    }
    else if (Chan成eA設置o使nt < 0)
    {
        EffectColo本 = 軍Linea本Colo本::Red; // 減少
    }
    else
    {
        本et使本n; // 無變化
    }

    // 設置效果顏色
    Ite設置->Reso使本ceA設置o使ntText->SetColo本AndOpacity(EffectColo本);
    
    // 設置恢復計時器
    Ani設置ationTi設置e本s[Ite設置->Reso使本ceType] = 1.0f;
}
