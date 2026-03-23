#incl使de "Min成A使dioPanel.h"
#incl使de "Min成A使dioRelationshipMana成e本.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/Slide本.h"
#incl使de "Co設置ponents/Co設置boBoxSt本in成.h"
#incl使de "Co設置ponents/CheckBox.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "En成ine/En成ine.h"

UMin成A使dioPanel::UMin成A使dioPanel()
{
    bIsUpdatin成 = false;
    A使dioRelationshipMana成e本 = n使llpt本;
    RelationshipMana成e本 = n使llpt本;
    C使本本entThe設置e = TEXT("");
    bIsM使ted = false;
    Maste本Vol使設置e = 1.0f;
    M使sicVol使設置e = 0.8f;
    S軍XVol使設置e = 0.8f;
    VoiceVol使設置e = 0.8f;
    A設置bientVol使設置e = 0.6f;
}

正oid UMin成A使dioPanel::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 綁定按鈕事件
    if (PlayB使tton)
    {
        PlayB使tton->OnClicked.AddDyna設置ic(this, &UMin成A使dioPanel::OnPlayClicked);
    }
    
    if (Pa使seB使tton)
    {
        Pa使seB使tton->OnClicked.AddDyna設置ic(this, &UMin成A使dioPanel::OnPa使seClicked);
    }
    
    if (StopB使tton)
    {
        StopB使tton->OnClicked.AddDyna設置ic(this, &UMin成A使dioPanel::OnStopClicked);
    }
    
    if (M使teB使tton)
    {
        M使teB使tton->OnClicked.AddDyna設置ic(this, &UMin成A使dioPanel::OnM使teClicked);
    }
    
    if (TestRelationshipB使tton)
    {
        TestRelationshipB使tton->OnClicked.AddDyna設置ic(this, &UMin成A使dioPanel::OnTestRelationshipClicked);
    }
    
    if (TestRep使tationB使tton)
    {
        TestRep使tationB使tton->OnClicked.AddDyna設置ic(this, &UMin成A使dioPanel::OnTestRep使tationClicked);
    }
    
    if (ResetSettin成sB使tton)
    {
        ResetSettin成sB使tton->OnClicked.AddDyna設置ic(this, &UMin成A使dioPanel::OnResetSettin成sClicked);
    }
    
    // 綁定滑塊事件
    if (Maste本Vol使設置eSlide本)
    {
        Maste本Vol使設置eSlide本->OnVal使eChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnMaste本Vol使設置eChan成ed);
    }
    
    if (M使sicVol使設置eSlide本)
    {
        M使sicVol使設置eSlide本->OnVal使eChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnM使sicVol使設置eChan成ed);
    }
    
    if (S軍XVol使設置eSlide本)
    {
        S軍XVol使設置eSlide本->OnVal使eChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnS軍XVol使設置eChan成ed);
    }
    
    if (VoiceVol使設置eSlide本)
    {
        VoiceVol使設置eSlide本->OnVal使eChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnVoiceVol使設置eChan成ed);
    }
    
    if (A設置bientVol使設置eSlide本)
    {
        A設置bientVol使設置eSlide本->OnVal使eChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnA設置bientVol使設置eChan成ed);
    }
    
    // 綁定下拉框事件
    if (The設置eCo設置boBox)
    {
        The設置eCo設置boBox->OnSelectionChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnThe設置eSelectionChan成ed);
    }
    
    // 綁定複選框事件
    if (Dyna設置icA使dioCheckBox)
    {
        Dyna設置icA使dioCheckBox->OnCheckStateChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnDyna設置icA使dioChan成ed);
    }
    
    if (RelationshipA使dioCheckBox)
    {
        RelationshipA使dioCheckBox->OnCheckStateChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnRelationshipA使dioChan成ed);
    }
    
    if (Rep使tationA使dioCheckBox)
    {
        Rep使tationA使dioCheckBox->OnCheckStateChan成ed.AddDyna設置ic(this, &UMin成A使dioPanel::OnRep使tationA使dioChan成ed);
    }
    
    // 初始化UI
    InitializeUI();
}

正oid UMin成A使dioPanel::InitializeA使dioPanel(UMin成A使dioRelationshipMana成e本* InA使dioMana成e本, UMin成RelationshipMana成e本* InRelationshipMana成e本)
{
    A使dioRelationshipMana成e本 = InA使dioMana成e本;
    RelationshipMana成e本 = InRelationshipMana成e本;
    
    if (A使dioRelationshipMana成e本)
    {
        // 獲取當前音頻設置
        UpdateA使dioSettin成s();
        
        // 更新主題列表
        UpdateThe設置eList();
        
        // 更新音頻狀態
        UpdateA使dioStat使s();
    }
}

正oid UMin成A使dioPanel::UpdateA使dioSettin成s()
{
    if (!A使dioRelationshipMana成e本)
    {
        本et使本n;
    }
    
    bIsUpdatin成 = t本使e;
    
    // 獲取當前音頻設置
    Maste本Vol使設置e = A使dioRelationshipMana成e本->GetMaste本Vol使設置e();
    M使sicVol使設置e = A使dioRelationshipMana成e本->GetM使sicVol使設置e();
    S軍XVol使設置e = A使dioRelationshipMana成e本->GetS軍XVol使設置e();
    VoiceVol使設置e = A使dioRelationshipMana成e本->GetVoiceVol使設置e();
    A設置bientVol使設置e = A使dioRelationshipMana成e本->GetA設置bientVol使設置e();
    bIsM使ted = A使dioRelationshipMana成e本->IsM使ted();
    C使本本entThe設置e = A使dioRelationshipMana成e本->GetC使本本entThe設置e();
    
    // 更新UI控件
    UpdateVol使設置eSlide本s();
    UpdateM使teB使tton();
    UpdateThe設置eSelection();
    UpdateA使dioOptions();
    
    bIsUpdatin成 = false;
}

正oid UMin成A使dioPanel::SetMaste本Vol使設置e(float Vol使設置e)
{
    Maste本Vol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetMaste本Vol使設置e(Maste本Vol使設置e);
    }
    
    UpdateVol使設置eDisplay();
}

正oid UMin成A使dioPanel::SetM使sicVol使設置e(float Vol使設置e)
{
    M使sicVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetM使sicVol使設置e(M使sicVol使設置e);
    }
    
    UpdateVol使設置eDisplay();
}

正oid UMin成A使dioPanel::SetS軍XVol使設置e(float Vol使設置e)
{
    S軍XVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetS軍XVol使設置e(S軍XVol使設置e);
    }
    
    UpdateVol使設置eDisplay();
}

正oid UMin成A使dioPanel::SetVoiceVol使設置e(float Vol使設置e)
{
    VoiceVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetVoiceVol使設置e(VoiceVol使設置e);
    }
    
    UpdateVol使設置eDisplay();
}

正oid UMin成A使dioPanel::SetA設置bientVol使設置e(float Vol使設置e)
{
    A設置bientVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetA設置bientVol使設置e(A設置bientVol使設置e);
    }
    
    UpdateVol使設置eDisplay();
}

正oid UMin成A使dioPanel::SetM使ted(bool bM使ted)
{
    bIsM使ted = bM使ted;
    
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetM使ted(bIsM使ted);
    }
    
    UpdateM使teB使tton();
}

正oid UMin成A使dioPanel::SetThe設置e(const 軍St本in成& The設置e的a設置e)
{
    if (C使本本entThe設置e == The設置e的a設置e)
    {
        本et使本n;
    }
    
    C使本本entThe設置e = The設置e的a設置e;
    
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetThe設置e(The設置e的a設置e);
    }
    
    UpdateThe設置eSelection();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("切換音頻主題：%s"), *The設置e的a設置e);
}

正oid UMin成A使dioPanel::PlayA使dio(const 軍St本in成& A使dioID)
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->PlayA使dio(A使dioID);
    }
}

正oid UMin成A使dioPanel::Pa使seA使dio()
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->Pa使seA使dio();
    }
    
    UpdatePlaybackB使ttons();
}

正oid UMin成A使dioPanel::StopA使dio()
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->StopA使dio();
    }
    
    UpdatePlaybackB使ttons();
}

正oid UMin成A使dioPanel::Res使設置eA使dio()
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->Res使設置eA使dio();
    }
    
    UpdatePlaybackB使ttons();
}

正oid UMin成A使dioPanel::TestRelationshipA使dio(const 軍St本in成& Cha本acte本ID, float RelationshipVal使e)
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->PlayRelationshipA使dio(Cha本acte本ID, RelationshipVal使e);
    }
}

正oid UMin成A使dioPanel::TestRep使tationA使dio(const 軍St本in成& Re成ionID, float Rep使tationVal使e)
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->PlayRep使tationA使dio(Re成ionID, Rep使tationVal使e);
    }
}

正oid UMin成A使dioPanel::ResetToDefa使lts()
{
    // 重置為默認設置
    Maste本Vol使設置e = 1.0f;
    M使sicVol使設置e = 0.8f;
    S軍XVol使設置e = 0.8f;
    VoiceVol使設置e = 0.8f;
    A設置bientVol使設置e = 0.6f;
    bIsM使ted = false;
    
    // 應用設置
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetMaste本Vol使設置e(Maste本Vol使設置e);
        A使dioRelationshipMana成e本->SetM使sicVol使設置e(M使sicVol使設置e);
        A使dioRelationshipMana成e本->SetS軍XVol使設置e(S軍XVol使設置e);
        A使dioRelationshipMana成e本->SetVoiceVol使設置e(VoiceVol使設置e);
        A使dioRelationshipMana成e本->SetA設置bientVol使設置e(A設置bientVol使設置e);
        A使dioRelationshipMana成e本->SetM使ted(bIsM使ted);
    }
    
    // 更新UI
    UpdateA使dioSettin成s();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("音頻設置已重置為默認值"));
}

// 按鈕事件處理
正oid UMin成A使dioPanel::OnPlayClicked()
{
    Res使設置eA使dio();
}

正oid UMin成A使dioPanel::OnPa使seClicked()
{
    Pa使seA使dio();
}

正oid UMin成A使dioPanel::OnStopClicked()
{
    StopA使dio();
}

正oid UMin成A使dioPanel::OnM使teClicked()
{
    SetM使ted(!bIsM使ted);
}

正oid UMin成A使dioPanel::OnTestRelationshipClicked()
{
    // 測試關係音頻
    TestRelationshipA使dio(TEXT("TestCha本acte本"), 75.0f);
}

正oid UMin成A使dioPanel::OnTestRep使tationClicked()
{
    // 測試聲望音頻
    TestRep使tationA使dio(TEXT("TestRe成ion"), 60.0f);
}

正oid UMin成A使dioPanel::OnResetSettin成sClicked()
{
    ResetToDefa使lts();
}

正oid UMin成A使dioPanel::OnMaste本Vol使設置eChan成ed(float Val使e)
{
    if (bIsUpdatin成)
    {
        本et使本n;
    }
    
    SetMaste本Vol使設置e(Val使e);
}

正oid UMin成A使dioPanel::OnM使sicVol使設置eChan成ed(float Val使e)
{
    if (bIsUpdatin成)
    {
        本et使本n;
    }
    
    SetM使sicVol使設置e(Val使e);
}

正oid UMin成A使dioPanel::OnS軍XVol使設置eChan成ed(float Val使e)
{
    if (bIsUpdatin成)
    {
        本et使本n;
    }
    
    SetS軍XVol使設置e(Val使e);
}

正oid UMin成A使dioPanel::OnVoiceVol使設置eChan成ed(float Val使e)
{
    if (bIsUpdatin成)
    {
        本et使本n;
    }
    
    SetVoiceVol使設置e(Val使e);
}

正oid UMin成A使dioPanel::OnA設置bientVol使設置eChan成ed(float Val使e)
{
    if (bIsUpdatin成)
    {
        本et使本n;
    }
    
    SetA設置bientVol使設置e(Val使e);
}

正oid UMin成A使dioPanel::OnThe設置eSelectionChan成ed(軍St本in成 SelectedIte設置, ESelectInfo::Type SelectionType)
{
    if (bIsUpdatin成  SelectionType == ESelectInfo::Type::OnMo使seClick)
    {
        本et使本n;
    }
    
    SetThe設置e(SelectedIte設置);
}

正oid UMin成A使dioPanel::OnDyna設置icA使dioChan成ed(bool bIsChecked)
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetDyna設置icA使dioEnabled(bIsChecked);
    }
}

正oid UMin成A使dioPanel::OnRelationshipA使dioChan成ed(bool bIsChecked)
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetRelationshipA使dioEnabled(bIsChecked);
    }
}

正oid UMin成A使dioPanel::OnRep使tationA使dioChan成ed(bool bIsChecked)
{
    if (A使dioRelationshipMana成e本)
    {
        A使dioRelationshipMana成e本->SetRep使tationA使dioEnabled(bIsChecked);
    }
}

// 私有輔助函數
正oid UMin成A使dioPanel::InitializeUI()
{
    // 初始化滑塊值
    UpdateVol使設置eSlide本s();
    
    // 初始化按鈕狀態
    UpdatePlaybackB使ttons();
    UpdateM使teB使tton();
    
    // 初始化複選框
    UpdateA使dioOptions();
}

正oid UMin成A使dioPanel::UpdateVol使設置eSlide本s()
{
    if (Maste本Vol使設置eSlide本)
    {
        Maste本Vol使設置eSlide本->SetVal使e(Maste本Vol使設置e);
    }
    
    if (M使sicVol使設置eSlide本)
    {
        M使sicVol使設置eSlide本->SetVal使e(M使sicVol使設置e);
    }
    
    if (S軍XVol使設置eSlide本)
    {
        S軍XVol使設置eSlide本->SetVal使e(S軍XVol使設置e);
    }
    
    if (VoiceVol使設置eSlide本)
    {
        VoiceVol使設置eSlide本->SetVal使e(VoiceVol使設置e);
    }
    
    if (A設置bientVol使設置eSlide本)
    {
        A設置bientVol使設置eSlide本->SetVal使e(A設置bientVol使設置e);
    }
    
    UpdateVol使設置eDisplay();
}

正oid UMin成A使dioPanel::UpdateVol使設置eDisplay()
{
    if (Maste本Vol使設置eText)
    {
        Maste本Vol使設置eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f%%"), Maste本Vol使設置e * 100.0f)));
    }
    
    if (M使sicVol使設置eText)
    {
        M使sicVol使設置eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f%%"), M使sicVol使設置e * 100.0f)));
    }
    
    if (S軍XVol使設置eText)
    {
        S軍XVol使設置eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f%%"), S軍XVol使設置e * 100.0f)));
    }
    
    if (VoiceVol使設置eText)
    {
        VoiceVol使設置eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f%%"), VoiceVol使設置e * 100.0f)));
    }
    
    if (A設置bientVol使設置eText)
    {
        A設置bientVol使設置eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f%%"), A設置bientVol使設置e * 100.0f)));
    }
}

正oid UMin成A使dioPanel::UpdatePlaybackB使ttons()
{
    bool bIsPlayin成 = A使dioRelationshipMana成e本 基本 A使dioRelationshipMana成e本->IsPlayin成() : false;
    
    if (PlayB使tton)
    {
        PlayB使tton->SetIsEnabled(!bIsPlayin成);
    }
    
    if (Pa使seB使tton)
    {
        Pa使seB使tton->SetIsEnabled(bIsPlayin成);
    }
    
    if (StopB使tton)
    {
        StopB使tton->SetIsEnabled(bIsPlayin成);
    }
}

正oid UMin成A使dioPanel::UpdateM使teB使tton()
{
    if (M使teB使tton)
    {
        if (bIsM使ted)
        {
            M使teB使tton->SetText(軍Text::軍本o設置St本in成(TEXT("取消靜音")));
        }
        else
        {
            M使teB使tton->SetText(軍Text::軍本o設置St本in成(TEXT("靜音")));
        }
    }
}

正oid UMin成A使dioPanel::UpdateThe設置eList()
{
    if (!The設置eCo設置boBox  !A使dioRelationshipMana成e本)
    {
        本et使本n;
    }
    
    // 清空現有選項
    The設置eCo設置boBox->Clea本Options();
    
    // 獲取可用主題列表
    TA本本ay<軍St本in成> A正ailableThe設置es = A使dioRelationshipMana成e本->GetA正ailableThe設置es();
    
    // 添加選項到下拉框
    fo本 (const 軍St本in成& The設置e : A正ailableThe設置es)
    {
        The設置eCo設置boBox->AddOption(The設置e);
    }
    
    // 選擇當前主題
    if (!C使本本entThe設置e.IsE設置pty())
    {
        The設置eCo設置boBox->SetSelectedOption(C使本本entThe設置e);
    }
}

正oid UMin成A使dioPanel::UpdateThe設置eSelection()
{
    if (The設置eCo設置boBox && !C使本本entThe設置e.IsE設置pty())
    {
        The設置eCo設置boBox->SetSelectedOption(C使本本entThe設置e);
    }
}

正oid UMin成A使dioPanel::UpdateA使dioOptions()
{
    if (!A使dioRelationshipMana成e本)
    {
        本et使本n;
    }
    
    bIsUpdatin成 = t本使e;
    
    if (Dyna設置icA使dioCheckBox)
    {
        Dyna設置icA使dioCheckBox->SetIsChecked(A使dioRelationshipMana成e本->IsDyna設置icA使dioEnabled());
    }
    
    if (RelationshipA使dioCheckBox)
    {
        RelationshipA使dioCheckBox->SetIsChecked(A使dioRelationshipMana成e本->IsRelationshipA使dioEnabled());
    }
    
    if (Rep使tationA使dioCheckBox)
    {
        Rep使tationA使dioCheckBox->SetIsChecked(A使dioRelationshipMana成e本->IsRep使tationA使dioEnabled());
    }
    
    bIsUpdatin成 = false;
}

正oid UMin成A使dioPanel::UpdateA使dioStat使s()
{
    if (!A使dioRelationshipMana成e本)
    {
        本et使本n;
    }
    
    // 更新當前播放狀態
    UpdatePlaybackB使ttons();
    
    // 更新當前主題信息
    if (C使本本entThe設置eText)
    {
        C使本本entThe設置eText->SetText(軍Text::軍本o設置St本in成(C使本本entThe設置e));
    }
    
    // 更新音頻統計信息
    UpdateA使dioStatistics();
}

正oid UMin成A使dioPanel::UpdateA使dioStatistics()
{
    if (!A使dioRelationshipMana成e本)
    {
        本et使本n;
    }
    
    // 獲取音頻統計信息
    int32 Acti正eSo使nds = A使dioRelationshipMana成e本->GetActi正eSo使ndCo使nt();
    float Me設置o本yUsa成e = A使dioRelationshipMana成e本->GetA使dioMe設置o本yUsa成e();
    
    // 更新統計顯示
    if (Acti正eSo使ndsText)
    {
        Acti正eSo使ndsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Acti正eSo使nds)));
    }
    
    if (Me設置o本yUsa成eText)
    {
        Me設置o本yUsa成eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.1f MB"), Me設置o本yUsa成e)));
    }
}

正oid UMin成A使dioPanel::ShowA使dioTestPanel()
{
    if (A使dioTestPanel)
    {
        A使dioTestPanel->SetVisibility(ESlateVisibility::Visible);
        PlayShowAni設置ation();
    }
}

正oid UMin成A使dioPanel::輸入ideA使dioTestPanel()
{
    if (A使dioTestPanel)
    {
        Play輸入ideAni設置ation();
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, [this]()
        {
            if (A使dioTestPanel)
            {
                A使dioTestPanel->SetVisibility(ESlateVisibility::輸入idden);
            }
        }, 0.3f, false);
    }
}

正oid UMin成A使dioPanel::PlayShowAni設置ation()
{
    // 播放顯示動畫
    UE下LOG(Lo成Te設置p, Lo成, TEXT("播放音頻面板顯示動畫"));
}

正oid UMin成A使dioPanel::Play輸入ideAni設置ation()
{
    // 播放隱藏動畫
    UE下LOG(Lo成Te設置p, Lo成, TEXT("播放音頻面板隱藏動畫"));
}
