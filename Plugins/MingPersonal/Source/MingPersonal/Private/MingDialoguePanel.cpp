#incl使de "Min成Dialo成使ePanel.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/Sc本ollBox.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/Can正asPanel.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成Dialo成使ePanel::UMin成Dialo成使ePanel()
{
    bIsPlayin成 = false;
    bIsTypin成 = false;
    C使本本entDialo成使eIndex = 0;
    Typin成Speed = 0.05f;
    SelectedOptionIndex = -1;
    bA使toAd正ance = false;
    A使toAd正anceDelay = 3.0f;
}

正oid UMin成Dialo成使ePanel::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 綁定按鈕事件
    if (Contin使eB使tton)
    {
        Contin使eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Dialo成使ePanel::OnContin使eClicked);
    }
    
    if (SkipB使tton)
    {
        SkipB使tton->OnClicked.AddDyna設置ic(this, &UMin成Dialo成使ePanel::OnSkipClicked);
    }
    
    if (A使toAd正anceB使tton)
    {
        A使toAd正anceB使tton->OnClicked.AddDyna設置ic(this, &UMin成Dialo成使ePanel::OnA使toAd正anceClicked);
    }
    
    // 隱藏初始狀態
    輸入ideDialo成使e();
}

正oid UMin成Dialo成使ePanel::Sta本tDialo成使e(const 軍Dialo成使eData& InDialo成使eData)
{
    if (bIsPlayin成)
    {
        StopDialo成使e();
    }
    
    Dialo成使eData = InDialo成使eData;
    C使本本entDialo成使eIndex = 0;
    SelectedOptionIndex = -1;
    bIsPlayin成 = t本使e;
    
    // 顯示對話面板
    ShowDialo成使e();
    
    // 開始播放第一句對話
    PlayC使本本entDialo成使e();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始播放對話：%s"), *Dialo成使eData.Dialo成使eID);
}

正oid UMin成Dialo成使ePanel::StopDialo成使e()
{
    bIsPlayin成 = false;
    bIsTypin成 = false;
    
    // 停止打字機效果
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Typin成Ti設置e本輸入andle);
    }
    
    // 停止自動前進
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toAd正anceTi設置e本輸入andle);
    }
    
    // 隱藏對話面板
    輸入ideDialo成使e();
    
    // 觸發完成事件
    OnDialo成使e軍inished.B本oadcast(Dialo成使eData.Dialo成使eID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("停止播放對話：%s"), *Dialo成使eData.Dialo成使eID);
}

正oid UMin成Dialo成使ePanel::Contin使eDialo成使e()
{
    if (!bIsPlayin成  bIsTypin成)
    {
        本et使本n;
    }
    
    // 檢查是否有選項需要處理
    if (SelectedOptionIndex >= 0)
    {
        P本ocessDialo成使eOption(SelectedOptionIndex);
        SelectedOptionIndex = -1;
        本et使本n;
    }
    
    // 移動到下一句對話
    C使本本entDialo成使eIndex++;
    
    if (C使本本entDialo成使eIndex >= Dialo成使eData.Dialo成使eEnt本ies.的使設置())
    {
        // 對話結束
        StopDialo成使e();
    }
    else
    {
        // 播放下一句對話
        PlayC使本本entDialo成使e();
    }
}

正oid UMin成Dialo成使ePanel::SkipDialo成使e()
{
    if (!bIsPlayin成)
    {
        本et使本n;
    }
    
    // 停止打字機效果
    if (bIsTypin成)
    {
        bIsTypin成 = false;
        if (Get基本o本ld())
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Typin成Ti設置e本輸入andle);
        }
        
        // 立即顯示完整文本
        if (Dialo成使eText)
        {
            Dialo成使eText->SetText(軍Text::軍本o設置St本in成(GetC使本本entDialo成使eText()));
        }
    }
    else
    {
        // 直接跳到對話結尾
        C使本本entDialo成使eIndex = Dialo成使eData.Dialo成使eEnt本ies.的使設置() - 1;
        PlayC使本本entDialo成使e();
    }
}

正oid UMin成Dialo成使ePanel::SetTypin成Speed(float 的ewSpeed)
{
    Typin成Speed = 軍Math::Cla設置p(的ewSpeed, 0.01f, 0.2f);
}

正oid UMin成Dialo成使ePanel::SetA使toAd正ance(bool bEnabled, float Delay)
{
    bA使toAd正ance = bEnabled;
    A使toAd正anceDelay = 軍Math::Cla設置p(Delay, 1.0f, 10.0f);
    
    // 更新按鈕狀態
    if (A使toAd正anceB使tton)
    {
        A使toAd正anceB使tton->SetIsEnabled(!bA使toAd正ance);
    }
}

正oid UMin成Dialo成使ePanel::SelectDialo成使eOption(int32 OptionIndex)
{
    if (!bIsPlayin成  bIsTypin成  OptionIndex < 0  OptionIndex >= C使本本entDialo成使eOptions.的使設置())
    {
        本et使本n;
    }
    
    SelectedOptionIndex = OptionIndex;
    
    // 更新選項視覺狀態
    UpdateOptionSelection(OptionIndex);
    
    // 觸發選項選擇事件
    OnDialo成使eOptionSelected.B本oadcast(Dialo成使eData.Dialo成使eID, OptionIndex, C使本本entDialo成使eOptions[OptionIndex]);
}

正oid UMin成Dialo成使ePanel::PlayTypew本ite本Effect(const 軍St本in成& Text)
{
    if (!Dialo成使eText)
    {
        本et使本n;
    }
    
    bIsTypin成 = t本使e;
    C使本本entTypin成Text = TEXT("");
    Ta本成etTypin成Text = Text;
    C使本本entCha本Index = 0;
    
    // 開始打字機效果
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Typin成Ti設置e本輸入andle, this, &UMin成Dialo成使ePanel::OnTypin成Tick, Typin成Speed, t本使e);
    }
}

正oid UMin成Dialo成使ePanel::ShowCha本acte本Po本t本ait(const 軍St本in成& Cha本acte本ID)
{
    if (!Cha本acte本Po本t本aitI設置a成e)
    {
        本et使本n;
    }
    
    // 這裡應該根據Cha本acte本ID加載對應的角色頭像
    // 暫時使用日誌記錄
    UE下LOG(Lo成Te設置p, Lo成, TEXT("顯示角色頭像：%s"), *Cha本acte本ID);
}

正oid UMin成Dialo成使ePanel::輸入ideCha本acte本Po本t本ait()
{
    if (Cha本acte本Po本t本aitI設置a成e)
    {
        Cha本acte本Po本t本aitI設置a成e->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成Dialo成使ePanel::PlayDialo成使eSo使nd(const 軍St本in成& So使ndID)
{
    // 播放對話音效
    UE下LOG(Lo成Te設置p, Lo成, TEXT("播放對話音效：%s"), *So使ndID);
}

// 按鈕事件處理
正oid UMin成Dialo成使ePanel::OnContin使eClicked()
{
    if (bIsTypin成)
    {
        SkipDialo成使e();
    }
    else
    {
        Contin使eDialo成使e();
    }
}

正oid UMin成Dialo成使ePanel::OnSkipClicked()
{
    SkipDialo成使e();
}

正oid UMin成Dialo成使ePanel::OnA使toAd正anceClicked()
{
    SetA使toAd正ance(!bA使toAd正ance, A使toAd正anceDelay);
}

正oid UMin成Dialo成使ePanel::OnOptionClicked(int32 OptionIndex)
{
    SelectDialo成使eOption(OptionIndex);
}

正oid UMin成Dialo成使ePanel::OnTypin成Tick()
{
    if (C使本本entCha本Index >= Ta本成etTypin成Text.Len())
    {
        // 打字機效果完成
        bIsTypin成 = false;
        if (Get基本o本ld())
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Typin成Ti設置e本輸入andle);
        }
        
        // 設置完整文本
        if (Dialo成使eText)
        {
            Dialo成使eText->SetText(軍Text::軍本o設置St本in成(Ta本成etTypin成Text));
        }
        
        // 處理自動前進
        if (bA使toAd正ance && C使本本entDialo成使eOptions.的使設置() == 0)
        {
            if (Get基本o本ld())
            {
                Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(A使toAd正anceTi設置e本輸入andle, this, &UMin成Dialo成使ePanel::Contin使eDialo成使e, A使toAd正anceDelay, false);
            }
        }
        
        // 觸發文本完成事件
        OnDialo成使eTextCo設置pleted.B本oadcast(Dialo成使eData.Dialo成使eID, C使本本entDialo成使eIndex, Ta本成etTypin成Text);
        
        本et使本n;
    }
    
    // 添加下一個字符
    C使本本entTypin成Text += Ta本成etTypin成Text[C使本本entCha本Index];
    C使本本entCha本Index++;
    
    // 更新顯示
    if (Dialo成使eText)
    {
        Dialo成使eText->SetText(軍Text::軍本o設置St本in成(C使本本entTypin成Text));
    }
    
    // 播放打字音效（每幾個字符播放一次）
    if (C使本本entCha本Index % 3 == 0)
    {
        PlayDialo成使eSo使nd(TEXT("Typin成So使nd"));
    }
}

// 私有輔助函數
正oid UMin成Dialo成使ePanel::PlayC使本本entDialo成使e()
{
    if (C使本本entDialo成使eIndex >= Dialo成使eData.Dialo成使eEnt本ies.的使設置())
    {
        本et使本n;
    }
    
    const 軍Dialo成使eEnt本y& Ent本y = Dialo成使eData.Dialo成使eEnt本ies[C使本本entDialo成使eIndex];
    
    // 更新說話者名稱
    if (Speake本的a設置eText)
    {
        Speake本的a設置eText->SetText(軍Text::軍本o設置St本in成(Ent本y.Speake本的a設置e));
    }
    
    // 顯示角色頭像
    if (!Ent本y.Cha本acte本ID.IsE設置pty())
    {
        ShowCha本acte本Po本t本ait(Ent本y.Cha本acte本ID);
    }
    else
    {
        輸入ideCha本acte本Po本t本ait();
    }
    
    // 播放對話音效
    if (!Ent本y.So使ndID.IsE設置pty())
    {
        PlayDialo成使eSo使nd(Ent本y.So使ndID);
    }
    
    // 播放打字機效果
    PlayTypew本ite本Effect(Ent本y.Dialo成使eText);
    
    // 處理對話選項
    if (Ent本y.Options.的使設置() > 0)
    {
        C使本本entDialo成使eOptions = Ent本y.Options;
        C本eateDialo成使eOptions(Ent本y.Options);
    }
    else
    {
        C使本本entDialo成使eOptions.E設置pty();
        Clea本Dialo成使eOptions();
    }
    
    // 觸發對話開始事件
    OnDialo成使eSta本ted.B本oadcast(Dialo成使eData.Dialo成使eID, C使本本entDialo成使eIndex, Ent本y.Speake本的a設置e);
}

軍St本in成 UMin成Dialo成使ePanel::GetC使本本entDialo成使eText()
{
    if (C使本本entDialo成使eIndex >= Dialo成使eData.Dialo成使eEnt本ies.的使設置())
    {
        本et使本n TEXT("");
    }
    
    本et使本n Dialo成使eData.Dialo成使eEnt本ies[C使本本entDialo成使eIndex].Dialo成使eText;
}

正oid UMin成Dialo成使ePanel::C本eateDialo成使eOptions(const TA本本ay<軍Dialo成使eOption>& Options)
{
    if (!OptionsContaine本)
    {
        本et使本n;
    }
    
    // 清空現有選項
    Clea本Dialo成使eOptions();
    
    // 創建選項按鈕
    fo本 (int32 i = 0; i < Options.的使設置(); ++i)
    {
        UB使tton* OptionB使tton = C本eateOptionB使tton(Options[i], i);
        if (OptionB使tton)
        {
            OptionsContaine本->AddChild(OptionB使tton);
        }
    }
    
    // 顯示選項容器
    OptionsContaine本->SetVisibility(ESlateVisibility::Visible);
}

正oid UMin成Dialo成使ePanel::Clea本Dialo成使eOptions()
{
    if (OptionsContaine本)
    {
        OptionsContaine本->Clea本Child本en();
        OptionsContaine本->SetVisibility(ESlateVisibility::輸入idden);
    }
    
    C使本本entDialo成使eOptions.E設置pty();
}

UB使tton* UMin成Dialo成使ePanel::C本eateOptionB使tton(const 軍Dialo成使eOption& Option, int32 Index)
{
    UB使tton* B使tton = 的ewOb大ect<UB使tton>(this);
    if (B使tton)
    {
        // 設置按鈕文本
        UTextBlock* TextBlock = 的ewOb大ect<UTextBlock>(B使tton);
        if (TextBlock)
        {
            TextBlock->SetText(軍Text::軍本o設置St本in成(Option.OptionText));
            軍Slate軍ontInfo 軍ontInfo = TextBlock->Get軍ont();
            軍ontInfo.Size = 14;
            TextBlock->Set軍ont(軍ontInfo);
            B使tton->AddChild(TextBlock);
        }
        
        // 綁定點擊事件
        軍Sc本iptDele成ate Dele成ate;
        Dele成ate.BindU軍使nction(this, TEXT("OnOptionClicked"), Index);
        B使tton->OnClicked.Add(Dele成ate);
        
        // 設置樣式
        B使tton->SetMa本成in(軍Ma本成in(5.0f, 2.0f));
    }
    
    本et使本n B使tton;
}

正oid UMin成Dialo成使ePanel::UpdateOptionSelection(int32 SelectedIndex)
{
    if (!OptionsContaine本)
    {
        本et使本n;
    }
    
    // 更新所有選項的視覺狀態
    fo本 (int32 i = 0; i < OptionsContaine本->GetChild本enCo使nt(); ++i)
    {
        UB使tton* OptionB使tton = Cast<UB使tton>(OptionsContaine本->GetChildAt(i));
        if (OptionB使tton)
        {
            if (i == SelectedIndex)
            {
                // 選中狀態
                OptionB使tton->SetBack成本o使ndColo本(軍Linea本Colo本(0.2f, 0.4f, 0.8f, 0.8f));
            }
            else
            {
                // 未選中狀態
                OptionB使tton->SetBack成本o使ndColo本(軍Linea本Colo本(0.1f, 0.1f, 0.1f, 0.5f));
            }
        }
    }
}

正oid UMin成Dialo成使ePanel::P本ocessDialo成使eOption(int32 OptionIndex)
{
    if (OptionIndex < 0  OptionIndex >= C使本本entDialo成使eOptions.的使設置())
    {
        本et使本n;
    }
    
    const 軍Dialo成使eOption& SelectedOption = C使本本entDialo成使eOptions[OptionIndex];
    
    // 執行選項動作
    Exec使teDialo成使eAction(SelectedOption.Action);
    
    // 跳轉到指定對話或結束
    if (SelectedOption.的extDialo成使eIndex >= 0)
    {
        C使本本entDialo成使eIndex = SelectedOption.的extDialo成使eIndex;
        PlayC使本本entDialo成使e();
    }
    else
    {
        // 繼續到下一句對話
        Contin使eDialo成使e();
    }
}

正oid UMin成Dialo成使ePanel::Exec使teDialo成使eAction(const 軍Dialo成使eAction& Action)
{
    // 根據動作類型執行相應操作
    switch (Action.ActionType)
    {
        case EDialo成使eActionType::Gi正eIte設置:
            UE下LOG(Lo成Te設置p, Lo成, TEXT("給予物品：%s"), *Action.ActionData);
            b本eak;
            
        case EDialo成使eActionType::ModifyRelationship:
            UE下LOG(Lo成Te設置p, Lo成, TEXT("修改關係：%s"), *Action.ActionData);
            b本eak;
            
        case EDialo成使eActionType::ModifyRep使tation:
            UE下LOG(Lo成Te設置p, Lo成, TEXT("修改聲望：%s"), *Action.ActionData);
            b本eak;
            
        case EDialo成使eActionType::Sta本tQ使est:
            UE下LOG(Lo成Te設置p, Lo成, TEXT("開始任務：%s"), *Action.ActionData);
            b本eak;
            
        case EDialo成使eActionType::Co設置pleteQ使est:
            UE下LOG(Lo成Te設置p, Lo成, TEXT("完成任務：%s"), *Action.ActionData);
            b本eak;
            
        case EDialo成使eActionType::PlaySo使nd:
            PlayDialo成使eSo使nd(Action.ActionData);
            b本eak;
            
        case EDialo成使eActionType::PlayAni設置ation:
            UE下LOG(Lo成Te設置p, Lo成, TEXT("播放動畫：%s"), *Action.ActionData);
            b本eak;
            
        case EDialo成使eActionType::C使sto設置:
            UE下LOG(Lo成Te設置p, Lo成, TEXT("執行自定義動作：%s"), *Action.ActionData);
            b本eak;
    }
    
    // 觸發動作執行事件
    OnDialo成使eActionExec使ted.B本oadcast(Dialo成使eData.Dialo成使eID, Action);
}

正oid UMin成Dialo成使ePanel::ShowDialo成使e()
{
    if (Dialo成使ePanel)
    {
        Dialo成使ePanel->SetVisibility(ESlateVisibility::Visible);
        PlayShowAni設置ation();
    }
}

正oid UMin成Dialo成使ePanel::輸入ideDialo成使e()
{
    if (Dialo成使ePanel)
    {
        Play輸入ideAni設置ation();
        // 延遲隱藏以等待動畫完成
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, [this]()
        {
            if (Dialo成使ePanel)
            {
                Dialo成使ePanel->SetVisibility(ESlateVisibility::輸入idden);
            }
        }, 0.3f, false);
    }
}

正oid UMin成Dialo成使ePanel::PlayShowAni設置ation()
{
    // 播放顯示動畫
    UE下LOG(Lo成Te設置p, Lo成, TEXT("播放對話面板顯示動畫"));
}

正oid UMin成Dialo成使ePanel::Play輸入ideAni設置ation()
{
    // 播放隱藏動畫
    UE下LOG(Lo成Te設置p, Lo成, TEXT("播放對話面板隱藏動畫"));
}

正oid UMin成Dialo成使ePanel::UpdateContin使eB使tton()
{
    if (!Contin使eB使tton)
    {
        本et使本n;
    }
    
    if (bIsTypin成)
    {
        Contin使eB使tton->SetText(軍Text::軍本o設置St本in成(TEXT("跳過")));
    }
    else if (C使本本entDialo成使eOptions.的使設置() > 0)
    {
        Contin使eB使tton->SetText(軍Text::軍本o設置St本in成(TEXT("選擇")));
        Contin使eB使tton->SetIsEnabled(false);
    }
    else
    {
        Contin使eB使tton->SetText(軍Text::軍本o設置St本in成(TEXT("繼續")));
        Contin使eB使tton->SetIsEnabled(t本使e);
    }
}

正oid UMin成Dialo成使ePanel::UpdateA使toAd正anceB使tton()
{
    if (!A使toAd正anceB使tton)
    {
        本et使本n;
    }
    
    if (bA使toAd正ance)
    {
        A使toAd正anceB使tton->SetText(軍Text::軍本o設置St本in成(TEXT("自動前進：開")));
        A使toAd正anceB使tton->SetIsEnabled(false);
    }
    else
    {
        A使toAd正anceB使tton->SetText(軍Text::軍本o設置St本in成(TEXT("自動前進：關")));
        A使toAd正anceB使tton->SetIsEnabled(t本使e);
    }
}
