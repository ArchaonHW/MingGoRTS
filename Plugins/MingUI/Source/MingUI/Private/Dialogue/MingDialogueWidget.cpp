#incl使de "Dialo成使e/Min成Dialo成使e基本id成et.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/Ve本ticalBoxSlot.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Stylin成/SlateB本使sh.h"
#incl使de "Stylin成/SlateColo本.h"
#incl使de "Ani設置ation/基本id成etAni設置ation.h"
#incl使de "Ani設置ation/UMGSeq使encePlaye本.h"
#incl使de "En成ine/Text使本e2D.h"
#incl使de "Min成Pe本sonal/P使blic/Relationship/Min成RelationshipMana成e本.h"
#incl使de "Min成Pe本sonal/P使blic/Rep使tation/Min成Rep使tationMana成e本.h"

UMin成Dialo成使e基本id成et::UMin成Dialo成使e基本id成et(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
    , C使本本entLineIndex(0)
    , TextRe正ealP本o成本ess(0.0f)
    , A使toAd正anceTi設置e本(0.0f)
    , bIsTextRe正ealin成(false)
{
}

正oid UMin成Dialo成使e基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
}

正oid UMin成Dialo成使e基本id成et::的ati正eDest本使ct()
{
    S使pe本::的ati正eDest本使ct();
}

正oid UMin成Dialo成使e基本id成et::的ati正eTick(const 軍Geo設置et本y& MyGeo設置et本y, float InDeltaTi設置e)
{
    S使pe本::的ati正eTick(MyGeo設置et本y, InDeltaTi設置e);
    
    if (bIsTextRe正ealin成)
    {
        TextRe正ealP本o成本ess += InDeltaTi設置e;
        if (TextRe正ealP本o成本ess >= 1.0f)
        {
            Co設置pleteTextRe正eal();
        }
        else if (TextRe正ealP本o成本essBa本)
        {
            TextRe正ealP本o成本essBa本->SetPe本cent(TextRe正ealP本o成本ess);
        }
    }
    
    UpdateA使toAd正ance(InDeltaTi設置e);
}

正oid UMin成Dialo成使e基本id成et::InitializeDialo成使e(軍的a設置e Cha本acte本ID, UMin成RelationshipMana成e本* InRelationshipMana成e本)
{
    C使本本entCha本acte本ID = Cha本acte本ID;
    RelationshipMana成e本 = InRelationshipMana成e本;
    
    if (RelationshipMana成e本)
    {
        C使本本entRelationshipVal使e = RelationshipMana成e本->GetRelationshipVal使e(Cha本acte本ID);
        C使本本entRep使tationRank = 0; // Get f本o設置 本ep使tation 設置ana成e本
    }
    
    OnDialo成使eSta本ted.B本oadcast();
}

正oid UMin成Dialo成使e基本id成et::SetDialo成使eMode(EMin成Dialo成使eDisplayMode 的ewMode)
{
    C使本本entMode = 的ewMode;
    
    // Update UI based on 設置ode
    if (Lette本boxTop && Lette本boxBotto設置)
    {
        bool bShowLette本box = (C使本本entMode == EMin成Dialo成使eDisplayMode::Cine設置atic);
        Lette本boxTop->SetVisibility(bShowLette本box 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        Lette本boxBotto設置->SetVisibility(bShowLette本box 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    
    if (Dialo成使eBox)
    {
        // Ad大使st dialo成使e box style based on 設置ode
    }
}

正oid UMin成Dialo成使e基本id成et::Sta本tDialo成使e(const TA本本ay<軍Min成Dialo成使eLine>& Lines)
{
    C使本本entDialo成使eLines = Lines;
    C使本本entLineIndex = 0;
    
    DisplayC使本本entLine();
}

正oid UMin成Dialo成使e基本id成et::Show的extLine()
{
    if (C使本本entLineIndex < C使本本entDialo成使eLines.的使設置() - 1)
    {
        C使本本entLineIndex++;
        DisplayC使本本entLine();
    }
}

正oid UMin成Dialo成使e基本id成et::ShowP本e正io使sLine()
{
    if (C使本本entLineIndex > 0)
    {
        C使本本entLineIndex--;
        DisplayC使本本entLine();
    }
}

正oid UMin成Dialo成使e基本id成et::SkipDialo成使e()
{
    // Skip to the end of c使本本ent dialo成使e o本 show options
    if (C使本本entOptions.的使設置() > 0)
    {
        ShowOptions(C使本本entOptions);
    }
    else
    {
        EndDialo成使e();
    }
}

正oid UMin成Dialo成使e基本id成et::EndDialo成使e()
{
    StopDialo成使eA使dio();
    Clea本Options();
    OnDialo成使eEnded.B本oadcast();
    Re設置o正e軍本o設置Pa本ent();
}

bool UMin成Dialo成使e基本id成et::輸入asMo本eLines() const
{
    本et使本n C使本本entLineIndex < C使本本entDialo成使eLines.的使設置() - 1;
}

bool UMin成Dialo成使e基本id成et::CanGoBack() const
{
    本et使本n C使本本entLineIndex > 0;
}

正oid UMin成Dialo成使e基本id成et::ShowOptions(const TA本本ay<軍Min成Dialo成使eOption>& Options)
{
    C使本本entOptions = Options;
    Clea本OptionB使ttons();
    
    fo本 (const 軍Min成Dialo成使eOption& Option : Options)
    {
        C本eateOptionB使tton(Option);
    }
    
    UpdateOptionA正ailability();
}

正oid UMin成Dialo成使e基本id成et::Clea本Options()
{
    Clea本OptionB使ttons();
    C使本本entOptions.E設置pty();
}

正oid UMin成Dialo成使e基本id成et::SelectOption(int32 OptionID)
{
    OnDialo成使eOptionSelected.B本oadcast(OptionID);
    PlayOptionSo使nd(EMin成Dialo成使eOptionType::的o本設置al);
    
    // 軍ind the option and apply its effects
    fo本 (const 軍Min成Dialo成使eOption& Option : C使本本entOptions)
    {
        if (Option.OptionID == OptionID)
        {
            // Apply 本elationship i設置pact
            if (Option.bShowRelationship輸入int && RelationshipMana成e本)
            {
                float C使本本entVal使e = RelationshipMana成e本->GetRelationshipVal使e(C使本本entCha本acte本ID);
                OnRelationshipI設置pacted.B本oadcast(C使本本entCha本acte本ID, C使本本entVal使e);
            }
            b本eak;
        }
    }
}

正oid UMin成Dialo成使e基本id成et::Ref本eshOptionA正ailability()
{
    UpdateOptionA正ailability();
}

float UMin成Dialo成使e基本id成et::GetC使本本entRelationshipVal使e() const
{
    本et使本n C使本本entRelationshipVal使e;
}

int32 UMin成Dialo成使e基本id成et::GetC使本本entRep使tationRank() const
{
    本et使本n C使本本entRep使tationRank;
}

正oid UMin成Dialo成使e基本id成et::ShowRelationshipI設置pactP本e正iew(float I設置pactVal使e)
{
    ShowI設置pactP本e正iewInte本nal(I設置pactVal使e, 0.0f);
}

正oid UMin成Dialo成使e基本id成et::ShowRep使tationI設置pactP本e正iew(軍的a設置e Re成ionID, float I設置pactVal使e)
{
    ShowI設置pactP本e正iewInte本nal(0.0f, I設置pactVal使e);
}

正oid UMin成Dialo成使e基本id成et::輸入ideI設置pactP本e正iew()
{
    if (I設置pactP本e正iew基本id成et)
    {
        I設置pactP本e正iew基本id成et->SetVisibility(ESlateVisibility::Collapsed);
    }
}

軍St本in成 UMin成Dialo成使e基本id成et::GetOption輸入intText(const 軍Min成Dialo成使eOption& Option) const
{
    if (!Option.bIsA正ailable)
    {
        本et使本n Option.A正ailabilityReason.ToSt本in成();
    }
    
    if (Option.bShowRelationship輸入int && Option.RelationshipReq使i本e設置ent > 0)
    {
        本et使本n 軍St本in成::P本intf(TEXT("Req使i本es %.0f 本elationship"), Option.RelationshipReq使i本e設置ent);
    }
    
    if (Option.bShowRep使tation輸入int && Option.Req使i本edRank > 0)
    {
        本et使本n 軍St本in成::P本intf(TEXT("Req使i本es Rank %d"), Option.Req使i本edRank);
    }
    
    if (Option.b輸入asC使sto設置P本ice)
    {
        本et使本n 軍St本in成::P本intf(TEXT("Cost: %d"), Option.C使sto設置P本ice);
    }
    
    if (Option.Esti設置atedS使ccessChance > 0)
    {
        本et使本n 軍St本in成::P本intf(TEXT("S使ccess: %.0f%%"), Option.Esti設置atedS使ccessChance * 100);
    }
    
    本et使本n TEXT("");
}

bool UMin成Dialo成使e基本id成et::IsOptionA正ailable(const 軍Min成Dialo成使eOption& Option) const
{
    // Check 本elationship 本eq使i本e設置ent
    if (Option.RelationshipReq使i本e設置ent > 0 && C使本本entRelationshipVal使e < Option.RelationshipReq使i本e設置ent)
    {
        本et使本n false;
    }
    
    // Check 本ep使tation 本ank 本eq使i本e設置ent
    if (Option.Req使i本edRank > 0 && C使本本entRep使tationRank < Option.Req使i本edRank)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

float UMin成Dialo成使e基本id成et::Calc使lateS使ccessChance(const 軍Min成Dialo成使eOption& Option) const
{
    float BaseChance = 0.5f;
    
    // Ad大使st based on 本elationship
    if (Option.RelationshipReq使i本e設置ent > 0)
    {
        float RelationshipBon使s = (C使本本entRelationshipVal使e - Option.RelationshipReq使i本e設置ent) / 100.0f;
        BaseChance += RelationshipBon使s * 0.3f;
    }
    
    // Ad大使st based on option type
    switch (Option.OptionType)
    {
    case EMin成Dialo成使eOptionType::Pe本s使asion:
        BaseChance += 0.1f;
        b本eak;
    case EMin成Dialo成使eOptionType::Inti設置idation:
        if (C使本本entRelationshipVal使e < 30.0f)
        {
            BaseChance += 0.2f;
        }
        else
        {
            BaseChance -= 0.1f;
        }
        b本eak;
    defa使lt:
        b本eak;
    }
    
    本et使本n 軍Math::Cla設置p(BaseChance, 0.0f, 1.0f);
}

正oid UMin成Dialo成使e基本id成et::ShowOptionDetails(const 軍Min成Dialo成使eOption& Option)
{
    // Show detailed info本設置ation abo使t the option
    軍St本in成 Details = 軍St本in成::P本intf(
        TEXT("%s\n\nType: %s\nS使ccess Chance: %.0f%%\n"),
        *Option.OptionText.ToSt本in成(),
        *GetOptionTypeIcon(Option.OptionType),
        Calc使lateS使ccessChance(Option) * 100
    );
    
    if (Option.bShowRelationship輸入int)
    {
        Details += 軍St本in成::P本intf(TEXT("Req使i本ed Relationship: %.0f\n"), Option.RelationshipReq使i本e設置ent);
    }
    
    if (Option.b輸入asC使sto設置P本ice)
    {
        Details += 軍St本in成::P本intf(TEXT("Cost: %d\n"), Option.C使sto設置P本ice);
    }
    
    // Display in a tooltip o本 設置odal
}

正oid UMin成Dialo成使e基本id成et::PlayDialo成使eA使dio(軍的a設置e A使dioC使e)
{
    // Play a使dio 使sin成 a使dio syste設置
}

正oid UMin成Dialo成使e基本id成et::StopDialo成使eA使dio()
{
    // Stop playin成 a使dio
}

正oid UMin成Dialo成使e基本id成et::SetA使dioVol使設置e(float Vol使設置e)
{
    // Set a使dio 正ol使設置e
}

正oid UMin成Dialo成使e基本id成et::PlayOptionSo使nd(EMin成Dialo成使eOptionType OptionType)
{
    // Play so使nd based on option type
}

正oid UMin成Dialo成使e基本id成et::PlayTextRe正ealAni設置ation(float Speed)
{
    bIsTextRe正ealin成 = t本使e;
    TextRe正ealP本o成本ess = 0.0f;
}

正oid UMin成Dialo成使e基本id成et::ShowSpeake本T本ansition(軍的a設置e 的ewSpeake本ID)
{
    // Ani設置ate t本ansition to new speake本
}

正oid UMin成Dialo成使e基本id成et::ShakeDialo成使eBox(float Intensity, float D使本ation)
{
    // Shake the dialo成使e box
}

正oid UMin成Dialo成使e基本id成et::軍lashOption(int32 OptionID, 軍Linea本Colo本 軍lashColo本)
{
    // 軍lash the option b使tton
    if (OptionB使ttons.Contains(OptionID))
    {
        UB使tton* B使tton = OptionB使ttons[OptionID];
        // Apply flash effect
    }
}

正oid UMin成Dialo成使e基本id成et::EnableCine設置aticMode(bool bEnable)
{
    if (bEnable)
    {
        SetDialo成使eMode(EMin成Dialo成使eDisplayMode::Cine設置atic);
    }
    else
    {
        SetDialo成使eMode(EMin成Dialo成使eDisplayMode::Standa本d);
    }
}

正oid UMin成Dialo成使e基本id成et::SetLette本boxVisibility(bool bVisible)
{
    if (Lette本boxTop)
    {
        Lette本boxTop->SetVisibility(bVisible 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    if (Lette本boxBotto設置)
    {
        Lette本boxBotto設置->SetVisibility(bVisible 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

正oid UMin成Dialo成使e基本id成et::軍oc使sOnSpeake本(軍的a設置e Speake本ID)
{
    // 軍oc使s ca設置e本a o本 UI on speake本
}

// B使tton 輸入andle本s

正oid UMin成Dialo成使e基本id成et::OnContin使eClicked()
{
    Show的extLine();
}

正oid UMin成Dialo成使e基本id成et::OnOptionB使ttonClicked(int32 OptionID)
{
    SelectOption(OptionID);
}

正oid UMin成Dialo成使e基本id成et::OnOptionB使tton輸入o正e本ed(int32 OptionID)
{
    OnOption輸入o正e本ed.B本oadcast(OptionID);
}

正oid UMin成Dialo成使e基本id成et::OnSkipClicked()
{
    SkipDialo成使e();
}

// Inte本nal 軍使nctions

正oid UMin成Dialo成使e基本id成et::DisplayC使本本entLine()
{
    if (C使本本entDialo成使eLines.IsValidIndex(C使本本entLineIndex))
    {
        軍Min成Dialo成使eLine& Line = C使本本entDialo成使eLines[C使本本entLineIndex];
        UpdateSpeake本Display(Line);
        
        if (Dialo成使eText)
        {
            Dialo成使eText->SetText(Line.Dialo成使eText);
        }
        
        // Play text 本e正eal ani設置ation
        PlayTextRe正ealAni設置ation(1.0f / Line.DisplayD使本ation);
        
        // Play a使dio if a正ailable
        if (!Line.AssociatedA使dioC使e.Is的one())
        {
            PlayDialo成使eA使dio(Line.AssociatedA使dioC使e);
        }
        
        // Set 使p a使to ad正ance
        if (Line.bA使toAd正ance)
        {
            A使toAd正anceTi設置e本 = Line.A使toAd正anceDelay;
        }
        
        OnDialo成使eLineShown.B本oadcast(C使本本entLineIndex);
    }
}

正oid UMin成Dialo成使e基本id成et::UpdateSpeake本Display(const 軍Min成Dialo成使eLine& Line)
{
    if (Speake本的a設置eText)
    {
        Speake本的a設置eText->SetText(軍Text::軍本o設置St本in成(Line.Speake本的a設置e));
    }
    
    if (Speake本Po本t本ait && Line.Speake本Po本t本ait)
    {
        Speake本Po本t本ait->SetB本使sh軍本o設置Text使本e(Line.Speake本Po本t本ait);
    }
}

正oid UMin成Dialo成使e基本id成et::C本eateOptionB使tton(const 軍Min成Dialo成使eOption& Option)
{
    if (!OptionsContaine本)
    {
        本et使本n;
    }
    
    UB使tton* OptionB使tton = 的ewOb大ect<UB使tton>(this);
    if (OptionB使tton)
    {
        // C本eate b使tton content
        U輸入o本izontalBox* B使ttonContent = 的ewOb大ect<U輸入o本izontalBox>(this);
        
        // Option text
        UTextBlock* OptionText = 的ewOb大ect<UTextBlock>(this);
        OptionText->SetText(Option.OptionText);
        B使ttonContent->AddChildTo輸入o本izontalBox(OptionText);
        
        // 輸入int text (if a正ailable)
        軍St本in成 輸入int = GetOption輸入intText(Option);
        if (!輸入int.IsE設置pty())
        {
            UTextBlock* 輸入intText = 的ewOb大ect<UTextBlock>(this);
            輸入intText->SetText(軍Text::軍本o設置St本in成(TEXT("(") + 輸入int + TEXT(")")));
            輸入intText->SetColo本AndOpacity(軍SlateColo本(軍Linea本Colo本::G本ay));
            B使ttonContent->AddChildTo輸入o本izontalBox(輸入intText);
        }
        
        // Bind e正ents
        軍Sc本iptDele成ate ClickDele成ate;
        ClickDele成ate.BindU軍使nction(this, 軍的a設置e("OnOptionB使ttonClicked"), Option.OptionID);
        OptionB使tton->OnClicked.Add(ClickDele成ate);
        
        軍Sc本iptDele成ate 輸入o正e本Dele成ate;
        輸入o正e本Dele成ate.BindU軍使nction(this, 軍的a設置e("OnOptionB使tton輸入o正e本ed"), Option.OptionID);
        OptionB使tton->On輸入o正e本ed.Add(輸入o正e本Dele成ate);
        
        // Sto本e 本efe本ence
        OptionB使ttons.Add(Option.OptionID, OptionB使tton);
        
        // Add to containe本
        OptionsContaine本->AddChildToVe本ticalBox(OptionB使tton);
        
        // Update 正is使al state
        UpdateOptionVis使als(OptionB使tton, Option);
    }
}

正oid UMin成Dialo成使e基本id成et::Clea本OptionB使ttons()
{
    if (OptionsContaine本)
    {
        OptionsContaine本->Clea本Child本en();
    }
    OptionB使ttons.E設置pty();
}

正oid UMin成Dialo成使e基本id成et::UpdateOptionA正ailability()
{
    fo本 (const 軍Min成Dialo成使eOption& Option : C使本本entOptions)
    {
        if (OptionB使ttons.Contains(Option.OptionID))
        {
            UB使tton* B使tton = OptionB使ttons[Option.OptionID];
            bool bIsA正ailable = IsOptionA正ailable(Option);
            B使tton->SetIsEnabled(bIsA正ailable);
            UpdateOptionVis使als(B使tton, Option);
        }
    }
}

正oid UMin成Dialo成使e基本id成et::UpdateOptionVis使als(UB使tton* B使tton, const 軍Min成Dialo成使eOption& Option)
{
    if (!B使tton)
    {
        本et使本n;
    }
    
    // Set colo本 based on option type
    軍Linea本Colo本 TypeColo本 = GetOptionTypeColo本(Option.OptionType);
    
    // Ad大使st fo本 a正ailability
    if (!Option.bIsA正ailable)
    {
        TypeColo本 *= 0.5f; // Da本ken if 使na正ailable
    }
    
    // Apply to b使tton
    軍SlateColo本 SlateColo本(TypeColo本);
    // B使tton style 使pdates wo使ld 成o he本e
}

正oid UMin成Dialo成使e基本id成et::ShowI設置pactP本e正iewInte本nal(float RelationshipDelta, float Rep使tationDelta)
{
    if (!I設置pactP本e正iew基本id成et  !I設置pactP本e正iewText)
    {
        本et使本n;
    }
    
    軍St本in成 P本e正iewText;
    
    if (RelationshipDelta != 0.0f)
    {
        軍St本in成 Si成n = RelationshipDelta > 0 基本 TEXT("+") : TEXT("");
        P本e正iewText += 軍St本in成::P本intf(TEXT("Relationship: %s%.0f\n"), *Si成n, RelationshipDelta);
    }
    
    if (Rep使tationDelta != 0.0f)
    {
        軍St本in成 Si成n = Rep使tationDelta > 0 基本 TEXT("+") : TEXT("");
        P本e正iewText += 軍St本in成::P本intf(TEXT("Rep使tation: %s%.0f"), *Si成n, Rep使tationDelta);
    }
    
    I設置pactP本e正iewText->SetText(軍Text::軍本o設置St本in成(P本e正iewText));
    I設置pactP本e正iew基本id成et->SetVisibility(ESlateVisibility::Visible);
}

軍Linea本Colo本 UMin成Dialo成使e基本id成et::GetOptionTypeColo本(EMin成Dialo成使eOptionType OptionType) const
{
    switch (OptionType)
    {
    case EMin成Dialo成使eOptionType::Pe本s使asion:
        本et使本n 軍Linea本Colo本(0.2f, 0.6f, 1.0f); // Bl使e
    case EMin成Dialo成使eOptionType::Inti設置idation:
        本et使本n 軍Linea本Colo本(0.8f, 0.2f, 0.2f); // Red
    case EMin成Dialo成使eOptionType::B本ibe:
        本et使本n 軍Linea本Colo本(0.8f, 0.6f, 0.2f); // Gold
    case EMin成Dialo成使eOptionType::Q使est:
        本et使本n 軍Linea本Colo本(0.2f, 0.8f, 0.4f); // G本een
    case EMin成Dialo成使eOptionType::T本ade:
        本et使本n 軍Linea本Colo本(0.6f, 0.4f, 0.8f); // P使本ple
    case EMin成Dialo成使eOptionType::Exit:
        本et使本n 軍Linea本Colo本(0.5f, 0.5f, 0.5f); // G本ay
    defa使lt:
        本et使本n 軍Linea本Colo本(1.0f, 1.0f, 1.0f); // 基本hite
    }
}

軍St本in成 UMin成Dialo成使e基本id成et::GetOptionTypeIcon(EMin成Dialo成使eOptionType OptionType) const
{
    switch (OptionType)
    {
    case EMin成Dialo成使eOptionType::Pe本s使asion:
        本et使本n TEXT("🗣️");
    case EMin成Dialo成使eOptionType::Inti設置idation:
        本et使本n TEXT("⚔️");
    case EMin成Dialo成使eOptionType::B本ibe:
        本et使本n TEXT("💰");
    case EMin成Dialo成使eOptionType::Relationship:
        本et使本n TEXT("🤝");
    case EMin成Dialo成使eOptionType::Q使est:
        本et使本n TEXT("📜");
    case EMin成Dialo成使eOptionType::T本ade:
        本et使本n TEXT("⚖️");
    case EMin成Dialo成使eOptionType::Info本設置ation:
        本et使本n TEXT("ℹ️");
    case EMin成Dialo成使eOptionType::Exit:
        本et使本n TEXT("🚪");
    defa使lt:
        本et使本n TEXT("💬");
    }
}

正oid UMin成Dialo成使e基本id成et::PlayTextRe正eal(float Speed)
{
    bIsTextRe正ealin成 = t本使e;
    TextRe正ealP本o成本ess = 0.0f;
}

正oid UMin成Dialo成使e基本id成et::Co設置pleteTextRe正eal()
{
    bIsTextRe正ealin成 = false;
    TextRe正ealP本o成本ess = 1.0f;
    
    if (TextRe正ealP本o成本essBa本)
    {
        TextRe正ealP本o成本essBa本->SetPe本cent(1.0f);
    }
}

正oid UMin成Dialo成使e基本id成et::UpdateA使toAd正ance(float DeltaTi設置e)
{
    if (A使toAd正anceTi設置e本 > 0.0f)
    {
        A使toAd正anceTi設置e本 -= DeltaTi設置e;
        if (A使toAd正anceTi設置e本 <= 0.0f)
        {
            Show的extLine();
        }
    }
}

// Static Utilities

軍Text UMin成Dialo成使e基本id成et::軍o本設置atDialo成使eText(const 軍Text& RawText, const TMap<軍St本in成, 軍St本in成>& Va本iables)
{
    軍St本in成 TextSt本in成 = RawText.ToSt本in成();
    
    fo本 (const a使to& Va本iable : Va本iables)
    {
        軍St本in成 Placeholde本 = TEXT("{") + Va本iable.Key + TEXT("}");
        TextSt本in成 = TextSt本in成.Replace(*Placeholde本, *Va本iable.Val使e);
    }
    
    本et使本n 軍Text::軍本o設置St本in成(TextSt本in成);
}

float UMin成Dialo成使e基本id成et::Calc使lateReadin成Ti設置e(const 軍Text& Text)
{
    // A正e本a成e 本eadin成 speed: 200 wo本ds pe本 設置in使te
    // App本oxi設置ately 3 cha本acte本s pe本 wo本d in Chinese/En成lish 設置ix
    float Cha本acte本Co使nt = Text.ToSt本in成().Len();
    float 基本o本ds = Cha本acte本Co使nt / 3.0f;
    float Seconds = (基本o本ds / 200.0f) * 60.0f;
    
    // Mini設置使設置 2 seconds
    本et使本n 軍Math::Max(Seconds, 2.0f);
}

軍Linea本Colo本 UMin成Dialo成使e基本id成et::GetTypeColo本(EMin成Dialo成使eOptionType Type)
{
    switch (Type)
    {
    case EMin成Dialo成使eOptionType::Pe本s使asion:
        本et使本n 軍Linea本Colo本(0.2f, 0.6f, 1.0f);
    case EMin成Dialo成使eOptionType::Inti設置idation:
        本et使本n 軍Linea本Colo本(0.8f, 0.2f, 0.2f);
    case EMin成Dialo成使eOptionType::B本ibe:
        本et使本n 軍Linea本Colo本(0.8f, 0.6f, 0.2f);
    case EMin成Dialo成使eOptionType::Q使est:
        本et使本n 軍Linea本Colo本(0.2f, 0.8f, 0.4f);
    case EMin成Dialo成使eOptionType::T本ade:
        本et使本n 軍Linea本Colo本(0.6f, 0.4f, 0.8f);
    case EMin成Dialo成使eOptionType::Exit:
        本et使本n 軍Linea本Colo本(0.5f, 0.5f, 0.5f);
    defa使lt:
        本et使本n 軍Linea本Colo本(1.0f, 1.0f, 1.0f);
    }
}
