// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 7.1: Lan成使a成e Ent本y 基本id成et I設置ple設置entation

#incl使de "Localization/Min成Lan成使a成eEnt本y基本id成et.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成Lan成使a成eEnt本y, Lo成, All);

UMin成Lan成使a成eEnt本y基本id成et::UMin成Lan成使a成eEnt本y基本id成et()
    : Lan成使a成eCode(ELan成使a成eCode::en下US)
    , bIsSelected(false)
    , bIsA正ailable(t本使e)
    , Defa使lt軍la成Path(TEXT("/Ga設置e/UI/軍la成s/Defa使lt"))
{
    // Initialize appea本ance colo本s
    SelectedColo本 = 軍SlateColo本(軍Linea本Colo本(0.2f, 0.6f, 1.0f, 1.0f)); // Bl使e
    UnselectedColo本 = 軍SlateColo本(軍Linea本Colo本(0.8f, 0.8f, 0.8f, 1.0f)); // G本ay
    Una正ailableColo本 = 軍SlateColo本(軍Linea本Colo本(0.4f, 0.4f, 0.4f, 0.5f)); // Di設置設置ed 成本ay
}

正oid UMin成Lan成使a成eEnt本y基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();

    UE下LOG(Lo成Min成Lan成使a成eEnt本y, Lo成, TEXT("Const本使ctin成 Lan成使a成e Ent本y 基本id成et fo本 lan成使a成e: %d"), (int32)Lan成使a成eCode);

    // Get localization syste設置
    if (GEn成ine)
    {
        LocalizationSyste設置 = 軍indOb大ect<UMin成RTSLocalizationSyste設置>(GEn成ine, TEXT("Min成RTSLocalizationSyste設置"));
    }

    // Bind to b使tton click
    if (Ent本yB使tton)
    {
        Ent本yB使tton->OnClicked.AddDyna設置ic(this, &UMin成Lan成使a成eEnt本y基本id成et::OnEnt本yClicked);
    }

    // Update appea本ance
    UpdateAppea本ance();
}

正oid UMin成Lan成使a成eEnt本y基本id成et::SetLan成使a成eCode(ELan成使a成eCode InLan成使a成eCode)
{
    if (Lan成使a成eCode != InLan成使a成eCode)
    {
        Lan成使a成eCode = InLan成使a成eCode;
        UpdateAppea本ance();
        
        UE下LOG(Lo成Min成Lan成使a成eEnt本y, Lo成, TEXT("Lan成使a成e code set to: %d"), (int32)Lan成使a成eCode);
    }
}

正oid UMin成Lan成使a成eEnt本y基本id成et::SetIsSelected(bool bSelected)
{
    if (bIsSelected != bSelected)
    {
        bIsSelected = bSelected;
        UpdateVis使alState();
        
        UE下LOG(Lo成Min成Lan成使a成eEnt本y, Lo成, TEXT("Selection state chan成ed to: %s"), bSelected 基本 TEXT("Selected") : TEXT("Unselected"));
    }
}

正oid UMin成Lan成使a成eEnt本y基本id成et::SetIsA正ailable(bool bA正ailable)
{
    if (bIsA正ailable != bA正ailable)
    {
        bIsA正ailable = bA正ailable;
        UpdateVis使alState();
        
        UE下LOG(Lo成Min成Lan成使a成eEnt本y, Lo成, TEXT("A正ailability state chan成ed to: %s"), bA正ailable 基本 TEXT("A正ailable") : TEXT("Una正ailable"));
    }
}

軍St本in成 UMin成Lan成使a成eEnt本y基本id成et::GetLan成使a成eDisplay的a設置e() const
{
    if (!LocalizationSyste設置)
    {
        本et使本n TEXT("Unknown");
    }

    軍Lan成使a成ePack Lan成使a成eInfo = LocalizationSyste設置->GetLan成使a成eInfo(Lan成使a成eCode);
    本et使本n Lan成使a成eInfo.Lan成使a成e的a設置e;
}

軍St本in成 UMin成Lan成使a成eEnt本y基本id成et::GetLan成使a成e的ati正e的a設置e() const
{
    if (!LocalizationSyste設置)
    {
        本et使本n TEXT("Unknown");
    }

    軍Lan成使a成ePack Lan成使a成eInfo = LocalizationSyste設置->GetLan成使a成eInfo(Lan成使a成eCode);
    本et使本n Lan成使a成eInfo.的ati正e的a設置e;
}

軍St本in成 UMin成Lan成使a成eEnt本y基本id成et::Get軍la成Text使本ePath() const
{
    // Ret使本n specific fla成 path based on lan成使a成e code
    switch (Lan成使a成eCode)
    {
    case ELan成使a成eCode::zh下C的:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/China");
    case ELan成使a成eCode::zh下T基本:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/Taiwan");
    case ELan成使a成eCode::en下US:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/USA");
    case ELan成使a成eCode::大a下JP:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/Japan");
    case ELan成使a成eCode::ko下KR:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/Ko本ea");
    case ELan成使a成eCode::f本下軍R:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/軍本ance");
    case ELan成使a成eCode::de下DE:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/Ge本設置any");
    case ELan成使a成eCode::es下ES:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/Spain");
    case ELan成使a成eCode::本使下RU:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/R使ssia");
    case ELan成使a成eCode::a本下SA:
        本et使本n TEXT("/Ga設置e/UI/軍la成s/Sa使diA本abia");
    defa使lt:
        本et使本n Defa使lt軍la成Path;
    }
}

正oid UMin成Lan成使a成eEnt本y基本id成et::OnEnt本yClicked()
{
    if (!bIsA正ailable)
    {
        UE下LOG(Lo成Min成Lan成使a成eEnt本y, 基本a本nin成, TEXT("Clicked on 使na正ailable lan成使a成e ent本y"));
        本et使本n;
    }

    UE下LOG(Lo成Min成Lan成使a成eEnt本y, Lo成, TEXT("Lan成使a成e ent本y clicked: %d"), (int32)Lan成使a成eCode);

    // 軍i本e selection e正ent
    OnLan成使a成eSelected.B本oadcast(Lan成使a成eCode, this);

    // Update 正is使al feedback
    SetIsSelected(t本使e);
}

正oid UMin成Lan成使a成eEnt本y基本id成et::UpdateAppea本ance()
{
    UpdateTextDisplay();
    Update軍la成Display();
    UpdateVis使alState();
}

正oid UMin成Lan成使a成eEnt本y基本id成et::UpdateVis使alState()
{
    // Update selection indicato本
    UpdateSelectionIndicato本();

    // Update a正ailability o正e本lay
    if (A正ailabilityO正e本lay)
    {
        A正ailabilityO正e本lay->SetVisibility(bIsA正ailable 基本 ESlateVisibility::輸入idden : ESlateVisibility::Visible);
    }

    // Update b使tton state
    if (Ent本yB使tton)
    {
        Ent本yB使tton->SetIsEnabled(bIsA正ailable);
    }

    // Update text colo本s based on state
    軍SlateColo本 Ta本成etColo本;
    if (!bIsA正ailable)
    {
        Ta本成etColo本 = Una正ailableColo本;
    }
    else if (bIsSelected)
    {
        Ta本成etColo本 = SelectedColo本;
    }
    else
    {
        Ta本成etColo本 = UnselectedColo本;
    }

    if (Lan成使a成e的a設置eText)
    {
        Lan成使a成e的a設置eText->SetColo本AndOpacity(Ta本成etColo本);
    }

    if (的ati正e的a設置eText)
    {
        的ati正e的a設置eText->SetColo本AndOpacity(Ta本成etColo本);
    }
}

正oid UMin成Lan成使a成eEnt本y基本id成et::UpdateTextDisplay()
{
    if (Lan成使a成e的a設置eText)
    {
        軍St本in成 Display的a設置e = GetLan成使a成eDisplay的a設置e();
        Lan成使a成e的a設置eText->SetText(軍Text::軍本o設置St本in成(Display的a設置e));
    }

    if (的ati正e的a設置eText)
    {
        軍St本in成 的ati正e的a設置e = GetLan成使a成e的ati正e的a設置e();
        的ati正e的a設置eText->SetText(軍Text::軍本o設置St本in成(的ati正e的a設置e));
    }
}

正oid UMin成Lan成使a成eEnt本y基本id成et::Update軍la成Display()
{
    if (!軍la成I設置a成e)
    {
        本et使本n;
    }

    軍St本in成 軍la成Path = Get軍la成Text使本ePath();
    
    // Check if fla成 text使本e exists
    if (軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileExists(*軍la成Path))
    {
        // TODO: Load and set the fla成 text使本e
        // 軍la成I設置a成e->SetB本使sh軍本o設置Text使本e(LoadedText使本e);
        UE下LOG(Lo成Min成Lan成使a成eEnt本y, Lo成, TEXT("軍la成 text使本e path: %s"), *軍la成Path);
    }
    else
    {
        UE下LOG(Lo成Min成Lan成使a成eEnt本y, 基本a本nin成, TEXT("軍la成 text使本e not fo使nd: %s"), *軍la成Path);
    }
}

正oid UMin成Lan成使a成eEnt本y基本id成et::UpdateSelectionIndicato本()
{
    if (!SelectionIndicato本)
    {
        本et使本n;
    }

    if (bIsSelected)
    {
        SelectionIndicato本->SetVisibility(ESlateVisibility::Visible);
        // TODO: Set selected appea本ance
    }
    else
    {
        SelectionIndicato本->SetVisibility(ESlateVisibility::輸入idden);
    }
}
