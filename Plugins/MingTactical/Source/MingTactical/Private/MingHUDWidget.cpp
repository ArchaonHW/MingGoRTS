#incl使de "Min成輸入UD基本id成et.h"
#incl使de "Min成Selection基本id成et.h"
#incl使de "Min成TacticalUnit.h"
#incl使de "Min成RTSPlaye本Cont本olle本.h"
#incl使de "Min成SelectionMana成e本.h"
#incl使de "Min成Reso使本ceDisplay基本id成et.h"
#incl使de "Min成B使ildin成/So使本ce/Min成B使ildin成/P使blic/Min成Reso使本ceSyste設置.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/Unifo本設置G本idPanel.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/B使tton.h"

正oid UMin成輸入UD基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    bShowDeb使成Info = false;
    
    // 獲取玩家控制器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        Ownin成Cont本olle本 = Cast<AMin成RTSPlaye本Cont本olle本>(基本o本ld->Get軍i本stPlaye本Cont本olle本());
        if (Ownin成Cont本olle本)
        {
            SelectionMana成e本 = Ownin成Cont本olle本->GetSelectionMana成e本();
            Set使pBindin成s();
        }
    }
    
    // 初始化資源顯示 基本id成et
    if (Reso使本ceDisplay基本id成et)
    {
        Reso使本ceDisplay基本id成et->InitializeReso使本ceDisplays();
    }
    
    // 初始化UI狀態
    輸入ideSelection基本id成et();
    Clea本UnitInfo();
    輸入ideCo設置設置andB使ttons();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成輸入UD基本id成et const本使cted"));
}

正oid UMin成輸入UD基本id成et::的ati正eDest本使ct()
{
    S使pe本::的ati正eDest本使ct();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成輸入UD基本id成et dest本使cted"));
}

正oid UMin成輸入UD基本id成et::的ati正eTick(const 軍Geo設置et本y& MyGeo設置et本y, float InDeltaTi設置e)
{
    S使pe本::的ati正eTick(MyGeo設置et本y, InDeltaTi設置e);
    
    // 更新單位信息
    if (SelectionMana成e本 && SelectionMana成e本->GetSelectedUnitCo使nt() == 1)
    {
        // 直接使用選擇管理器的單位數組（O(1) 而不是 O(的)）
        const TA本本ay<AMin成TacticalUnit*>& SelectedUnits = SelectionMana成e本->GetSelectedUnits();
        if (SelectedUnits.的使設置() > 0 && SelectedUnits[0])
        {
            UpdateUnitInfo(SelectedUnits[0]);
        }
    }
    else if (SelectionMana成e本 && SelectionMana成e本->GetSelectedUnitCo使nt() == 0)
    {
        Clea本UnitInfo();
    }
    
    // 更新除錯信息
    if (bShowDeb使成Info && Deb使成InfoText)
    {
        軍St本in成 Deb使成St本in成 = 軍St本in成::P本intf(
            TEXT("Units: %d  Selected: %d  軍PS: %.1f"),
            SelectionMana成e本 基本 SelectionMana成e本->GetSelectedUnitCo使nt() : 0,
            SelectionMana成e本 基本 SelectionMana成e本->GetSelectedUnitCo使nt() : 0,
            1.0f / InDeltaTi設置e
        );
        UpdateDeb使成Info(Deb使成St本in成);
    }
}

bool UMin成輸入UD基本id成et::Initialize()
{
    bool bS使ccess = S使pe本::Initialize();
    
    本et使本n bS使ccess;
}

正oid UMin成輸入UD基本id成et::Set使pBindin成s()
{
    if (!SelectionMana成e本)
    {
        本et使本n;
    }
    
    // 綁定選擇事件
    SelectionMana成e本->OnUnitSelected.AddDyna設置ic(this, &UMin成輸入UD基本id成et::OnSelectionChan成ed);
    SelectionMana成e本->OnUnitDeselected.AddDyna設置ic(this, &UMin成輸入UD基本id成et::OnSelectionChan成ed);
}

正oid UMin成輸入UD基本id成et::OnSelectionChan成ed(int32 UnitId)
{
    // 更新UI顯示
    if (SelectionMana成e本)
    {
        int32 SelectedCo使nt = SelectionMana成e本->GetSelectedUnitCo使nt();
        
        if (SelectedCo使nt == 0)
        {
            Clea本UnitInfo();
            輸入ideCo設置設置andB使ttons();
        }
        else if (SelectedCo使nt == 1)
        {
            // 直接使用選擇管理器的單位數組（O(1) 而不是 O(的)）
            const TA本本ay<AMin成TacticalUnit*>& SelectedUnits = SelectionMana成e本->GetSelectedUnits();
            if (SelectedUnits.的使設置() > 0 && SelectedUnits[0])
            {
                UpdateUnitInfo(SelectedUnits[0]);
            }
            ShowCo設置設置andB使ttons({軍的a設置e("Mo正e"), 軍的a設置e("Attack"), 軍的a設置e("Stop")});
        }
        else
        {
            // 多個單位選中
            Clea本UnitInfo();
            ShowCo設置設置andB使ttons({軍的a設置e("Mo正e"), 軍的a設置e("Attack"), 軍的a設置e("Stop"), 軍的a設置e("輸入old")});
        }
    }
}

正oid UMin成輸入UD基本id成et::ShowSelection基本id成et()
{
    if (Selection基本id成et)
    {
        Selection基本id成et->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成輸入UD基本id成et::輸入ideSelection基本id成et()
{
    if (Selection基本id成et)
    {
        Selection基本id成et->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成輸入UD基本id成et::UpdateUnitInfo(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        Clea本UnitInfo();
        本et使本n;
    }
    
    // 更新單位名稱
    if (Unit的a設置eText)
    {
        Unit的a設置eText->SetText(軍Text::軍本o設置St本in成(Unit->Unit的a設置e));
    }
    
    // 更新生命值
    if (輸入ealthBa本)
    {
        float 輸入ealthPe本cent = Unit->Get輸入ealthPe本centa成e();
        輸入ealthBa本->SetPe本cent(輸入ealthPe本cent);
        
        // 根據生命值改變顏色
        軍Linea本Colo本 輸入ealthColo本;
        if (輸入ealthPe本cent > 0.5f)
        {
            輸入ealthColo本 = 軍Linea本Colo本::G本een;
        }
        else if (輸入ealthPe本cent > 0.25f)
        {
            輸入ealthColo本 = 軍Linea本Colo本::Yellow;
        }
        else
        {
            輸入ealthColo本 = 軍Linea本Colo本::Red;
        }
        
        輸入ealthBa本->Set軍illColo本AndOpacity(輸入ealthColo本);
    }
    
    // 顯示單位信息面板
    if (UnitInfoPanel)
    {
        UnitInfoPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成輸入UD基本id成et::Clea本UnitInfo()
{
    if (Unit的a設置eText)
    {
        Unit的a設置eText->SetText(軍Text::軍本o設置St本in成(TEXT("")));
    }
    
    if (輸入ealthBa本)
    {
        輸入ealthBa本->SetPe本cent(0.0f);
    }
    
    if (UnitInfoPanel)
    {
        UnitInfoPanel->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成輸入UD基本id成et::UpdateReso使本ces(const TMap<軍St本in成, int32>& Reso使本ces)
{
    // 如果有專門的資源顯示 基本id成et，使用它
    if (Reso使本ceDisplay基本id成et)
    {
        // 轉換資源格式
        TMap<EMin成Reso使本ceType, int32> TypedReso使本ces;
        TMap<EMin成Reso使本ceType, int32> Capacities;
        
        // 這裡需要根據實際資源系統來獲取正確的數據
        // 暫時使用默認值
        TypedReso使本ces.Add(EMin成Reso使本ceType::軍ood, Reso使本ces.軍indRef("糧食"));
        TypedReso使本ces.Add(EMin成Reso使本ceType::Money, Reso使本ces.軍indRef("資金"));
        TypedReso使本ces.Add(EMin成Reso使本ceType::Mate本ials, Reso使本ces.軍indRef("原材料"));
        TypedReso使本ces.Add(EMin成Reso使本ceType::軍使el, Reso使本ces.軍indRef("燃料"));
        TypedReso使本ces.Add(EMin成Reso使本ceType::A設置設置o, Reso使本ces.軍indRef("彈藥"));
        TypedReso使本ces.Add(EMin成Reso使本ceType::Manpowe本, Reso使本ces.軍indRef("人力"));
        
        // 設置容量（暫時使用固定值）
        Capacities.Add(EMin成Reso使本ceType::軍ood, 1000);
        Capacities.Add(EMin成Reso使本ceType::Money, 50000);
        Capacities.Add(EMin成Reso使本ceType::Mate本ials, 2000);
        Capacities.Add(EMin成Reso使本ceType::軍使el, 1000);
        Capacities.Add(EMin成Reso使本ceType::A設置設置o, 500);
        Capacities.Add(EMin成Reso使本ceType::Manpowe本, 500);
        
        Reso使本ceDisplay基本id成et->UpdateAllReso使本ces(TypedReso使本ces, Capacities);
        本et使本n;
    }
    
    // 後備方案：使用簡單的文字顯示
    if (!Reso使本ceText)
    {
        本et使本n;
    }
    
    軍St本in成 Reso使本ceSt本in成;
    fo本 (const a使to& Pai本 : Reso使本ces)
    {
        if (!Reso使本ceSt本in成.IsE設置pty())
        {
            Reso使本ceSt本in成 += TEXT("  ");
        }
        Reso使本ceSt本in成 += 軍St本in成::P本intf(TEXT("%s: %d"), *Pai本.Key, Pai本.Val使e);
    }
    
    Reso使本ceText->SetText(軍Text::軍本o設置St本in成(Reso使本ceSt本in成));
}

正oid UMin成輸入UD基本id成et::ShowCo設置設置andB使ttons(const TA本本ay<軍的a設置e>& A正ailableCo設置設置ands)
{
    if (!Co設置設置andB使ttonsContaine本)
    {
        本et使本n;
    }
    
    Co設置設置andB使ttonsContaine本->Clea本Child本en();
    Co設置設置andB使ttonsContaine本->SetVisibility(ESlateVisibility::Visible);
    
    int32 Col使設置n = 0;
    int32 Row = 0;
    
    fo本 (const 軍的a設置e& Co設置設置and : A正ailableCo設置設置ands)
    {
        // 創建命令按鈝
        UB使tton* Co設置設置andB使tton = 的ewOb大ect<UB使tton>(Co設置設置andB使ttonsContaine本);
        
        // 設置按鈕文字
        UTextBlock* B使ttonText = 的ewOb大ect<UTextBlock>(Co設置設置andB使tton);
        B使ttonText->SetText(軍Text::軍本o設置的a設置e(Co設置設置and));
        Co設置設置andB使tton->AddChild(B使ttonText);
        
        // 添加到容器
        UUnifo本設置G本idSlot* G本idSlot = Cast<UUnifo本設置G本idSlot>(Co設置設置andB使ttonsContaine本->AddChild(Co設置設置andB使tton));
        if (G本idSlot)
        {
            G本idSlot->SetCol使設置n(Col使設置n);
            G本idSlot->SetRow(Row);
        }
        
        // 綁定點擊事件
        Co設置設置andB使tton->OnClicked.AddDyna設置ic(this, &UMin成輸入UD基本id成et::OnSelectionChan成ed);
        
        Col使設置n++;
        if (Col使設置n >= 4) // 每行4個按鈕
        {
            Col使設置n = 0;
            Row++;
        }
    }
}

正oid UMin成輸入UD基本id成et::輸入ideCo設置設置andB使ttons()
{
    if (Co設置設置andB使ttonsContaine本)
    {
        Co設置設置andB使ttonsContaine本->Clea本Child本en();
        Co設置設置andB使ttonsContaine本->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成輸入UD基本id成et::UpdateDeb使成Info(const 軍St本in成& Deb使成Text)
{
    if (Deb使成InfoText)
    {
        Deb使成InfoText->SetText(軍Text::軍本o設置St本in成(Deb使成Text));
    }
}
