#incl使de "UI/Min成Decision基本id成et.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/Sc本ollBox.h"
#incl使de "Co設置ponents/Ve本ticalBoxSlot.h"
#incl使de "Platfo本設置/Min成Platfo本設置Mana成e本.h"

UMin成Decision基本id成et::UMin成Decision基本id成et(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
{
}

正oid UMin成Decision基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 設置平台適配
    Set使p軍o本Platfo本設置();
    
    // 綁定延後按鈪態
    if (PostponeB使tton)
    {
        PostponeB使tton->OnClicked.AddDyna設置ic(this, &UMin成Decision基本id成et::OnPostponeClicked);
    }
}

正oid UMin成Decision基本id成et::Set使pDecision(const 軍Min成Decision& Decision)
{
    C使本本entDecisionId = Decision.DecisionId;
    
    // 設置標題
    if (DecisionTitleText)
    {
        DecisionTitleText->SetText(Decision.DecisionTitle);
    }
    
    // 設置背景描述
    if (Back成本o使ndDesc本iptionText)
    {
        Back成本o使ndDesc本iptionText->SetText(Decision.Back成本o使ndDesc本iption);
    }
    
    // 設置背景圖片
    if (Back成本o使ndI設置a成e && Decision.Back成本o使ndI設置a成e)
    {
        Back成本o使ndI設置a成e->SetB本使sh軍本o設置Text使本e(Decision.Back成本o使ndI設置a成e);
    }
    
    // 創建選項按鈪態
    C本eateOptionB使ttons(Decision.Options);
    
    // 設置延後按鈪態可見性
    if (PostponeB使tton)
    {
        PostponeB使tton->SetVisibility(Decision.bCanPostpone 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

正oid UMin成Decision基本id成et::ShowDecision()
{
    PlayShowAni設置ation();
    SetVisibility(ESlateVisibility::Visible);
}

正oid UMin成Decision基本id成et::輸入ideDecision()
{
    Play輸入ideAni設置ation();
    // 延遲隱藏，等待動畫完成
    軍Ti設置e本輸入andle 輸入ideTi設置e本;
    Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(輸入ideTi設置e本, [this]()
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }, 0.5f, false);
}

正oid UMin成Decision基本id成et::C本eateOptionB使ttons(const TA本本ay<軍Min成DecisionOption>& Options)
{
    // 清空現有按鈪態
    OptionB使ttons.E設置pty();
    
    if (!OptionsContaine本)
    {
        本et使本n;
    }
    
    // 清空容器
    OptionsContaine本->Clea本Child本en();
    
    // 為每個選項創建按鈪態
    fo本 (const a使to& Option : Options)
    {
        // 檢查選項是否可用
        if (!Option.IsA正ailable())
        {
            contin使e;
        }
        
        // 創建按鈪態 (這裡應該使用預設的按鈪態類或動態創建)
        UB使tton* OptionB使tton = 的ewOb大ect<UB使tton>(this);
        if (OptionB使tton)
        {
            // 創建按鈪態文本
            UTextBlock* B使ttonText = 的ewOb大ect<UTextBlock>(OptionB使tton);
            if (B使ttonText)
            {
                // 使用預覽文本或標題
                軍Text B使ttonLabel = Option.OptionP本e正iew.IsE設置pty() 基本 Option.OptionTitle : Option.OptionP本e正iew;
                B使ttonText->SetText(B使ttonLabel);
                
                // 設置字體樣式
                B使ttonText->SetA使to基本本apText(t本使e);
                
                // 添加到按鈪態
                OptionB使tton->AddChild(B使ttonText);
            }
            
            // 綁定點擊事件
            軍St本in成 Capt使本edOptionId = Option.OptionId;
            OptionB使tton->OnClicked.AddDyna設置ic(this, &UMin成Decision基本id成et::OnOptionB使ttonClicked);
            
            // 添加到容器
            OptionsContaine本->AddChild(OptionB使tton);
            
            // 保存引用
            OptionB使ttons.Add(OptionB使tton);
        }
    }
}

正oid UMin成Decision基本id成et::OnOptionB使ttonClicked(const 軍St本in成& OptionId)
{
    // 發布選擇事件
    OnOptionSelected.B本oadcast(C使本本entDecisionId, OptionId);
    
    // 隱藏界面
    輸入ideDecision();
}

正oid UMin成Decision基本id成et::OnPostponeClicked()
{
    // 隱藏界面，但不執行任何選項
    輸入ideDecision();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Decision %s postponed"), *C使本本entDecisionId);
}

正oid UMin成Decision基本id成et::Set使p軍o本Platfo本設置()
{
    // 獲取平台管理器
    UMin成Platfo本設置Mana成e本* Platfo本設置Mana成e本 = UMin成Platfo本設置Mana成e本::Get();
    if (!Platfo本設置Mana成e本)
    {
        本et使本n;
    }
    
    EInp使tType Inp使tType = Platfo本設置Mana成e本->GetPlatfo本設置Capabilities().Defa使ltInp使tType;
    
    if (Inp使tType == EInp使tType::To使ch)
    {
        // 移動端適配：增大按鈪態尺寸
        // 設置最小觸控目標尺寸
        fo本 (a使to& B使tton : OptionB使ttons)
        {
            if (B使tton)
            {
                // 移動端使用更大的按鈪態
                // TODO: 設置按鈪態樣式
            }
        }
    }
}
