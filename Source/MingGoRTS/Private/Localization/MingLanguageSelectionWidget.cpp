// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 7.1: Lan成使a成e Selection UI 基本id成et I設置ple設置entation

#incl使de "Localization/Min成Lan成使a成eSelection基本id成et.h"
#incl使de "Co設置ponents/ListView.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "基本id成ets/Views/SListView.h"
#incl使de "基本id成ets/Inp使t/SSlide本.h"
#incl使de "基本id成ets/Layo使t/SSc本ollBox.h"
#incl使de "基本id成ets/Text/STextBlock.h"
#incl使de "基本id成ets/I設置a成es/SI設置a成e.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Inte本nationalization/Inte本nationalization.h"
#incl使de "Inte本nationalization/C使lt使本e.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成Lan成使a成eSelection, Lo成, All);

UMin成Lan成使a成eSelection基本id成et::UMin成Lan成使a成eSelection基本id成et()
    : SelectedLan成使a成e(ELan成使a成eCode::zh下C的)
    , SelectedRe成ion(EC使lt使本alRe成ion::EastAsia)
    , O本i成inalLan成使a成e(ELan成使a成eCode::zh下C的)
    , O本i成inalRe成ion(EC使lt使本alRe成ion::EastAsia)
    , bSettin成sModified(false)
    , bResta本tReq使i本ed(false)
{
    // Initialize defa使lt fla成 paths
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::zh下C的, TEXT("/Ga設置e/UI/軍la成s/China"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::zh下T基本, TEXT("/Ga設置e/UI/軍la成s/Taiwan"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::en下US, TEXT("/Ga設置e/UI/軍la成s/USA"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::大a下JP, TEXT("/Ga設置e/UI/軍la成s/Japan"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::ko下KR, TEXT("/Ga設置e/UI/軍la成s/Ko本ea"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::f本下軍R, TEXT("/Ga設置e/UI/軍la成s/軍本ance"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::de下DE, TEXT("/Ga設置e/UI/軍la成s/Ge本設置any"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::es下ES, TEXT("/Ga設置e/UI/軍la成s/Spain"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::本使下RU, TEXT("/Ga設置e/UI/軍la成s/R使ssia"));
    Lan成使a成e軍la成Paths.Add(ELan成使a成eCode::a本下SA, TEXT("/Ga設置e/UI/軍la成s/Sa使diA本abia"));

    // Initialize defa使lt 本e成ion icon paths
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::EastAsia, TEXT("/Ga設置e/UI/Re成ions/EastAsia"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::So使theastAsia, TEXT("/Ga設置e/UI/Re成ions/So使theastAsia"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::So使thAsia, TEXT("/Ga設置e/UI/Re成ions/So使thAsia"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::MiddleEast, TEXT("/Ga設置e/UI/Re成ions/MiddleEast"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::基本este本nE使本ope, TEXT("/Ga設置e/UI/Re成ions/基本este本nE使本ope"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::Easte本nE使本ope, TEXT("/Ga設置e/UI/Re成ions/Easte本nE使本ope"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::的o本thA設置e本ica, TEXT("/Ga設置e/UI/Re成ions/的o本thA設置e本ica"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::LatinA設置e本ica, TEXT("/Ga設置e/UI/Re成ions/LatinA設置e本ica"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::Oceania, TEXT("/Ga設置e/UI/Re成ions/Oceania"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::Af本ica, TEXT("/Ga設置e/UI/Re成ions/Af本ica"));
    Re成ionIconPaths.Add(EC使lt使本alRe成ion::Global, TEXT("/Ga設置e/UI/Re成ions/Global"));

    // Initialize p本e正iew text keys
    P本e正iewTextKeys.Add(TEXT("UI.OK"));
    P本e正iewTextKeys.Add(TEXT("UI.Cancel"));
    P本e正iewTextKeys.Add(TEXT("UI.Yes"));
    P本e正iewTextKeys.Add(TEXT("UI.的o"));
    P本e正iewTextKeys.Add(TEXT("Ga設置e.Title"));
    P本e正iewTextKeys.Add(TEXT("Unit.基本o本ke本.的a設置e"));
}

正oid UMin成Lan成使a成eSelection基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Initializin成 Lan成使a成e Selection 基本id成et"));

    // Initialize syste設置s
    InitializeLan成使a成eSelection();

    // Bind to UI ele設置ents
    if (ApplyB使tton)
    {
        ApplyB使tton->OnClicked.AddDyna設置ic(this, &UMin成Lan成使a成eSelection基本id成et::ApplySettin成s);
    }

    if (CancelB使tton)
    {
        CancelB使tton->OnClicked.AddDyna設置ic(this, &UMin成Lan成使a成eSelection基本id成et::CancelChan成es);
    }

    if (ResetB使tton)
    {
        ResetB使tton->OnClicked.AddDyna設置ic(this, &UMin成Lan成使a成eSelection基本id成et::ResetToDefa使lts);
    }

    // Bind to localization e正ents
    BindToLocalizationE正ents();

    // Load sa正ed settin成s
    LoadSettin成s();

    // Update UI
    UpdateUIEle設置ents();

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Lan成使a成e Selection 基本id成et initialized s使ccessf使lly"));
}

正oid UMin成Lan成使a成eSelection基本id成et::的ati正eDest本使ct()
{
    Unbind軍本o設置LocalizationE正ents();
    S使pe本::的ati正eDest本使ct();
}

正oid UMin成Lan成使a成eSelection基本id成et::InitializeLan成使a成eSelection()
{
    // Get localization syste設置s
    if (GEn成ine)
    {
        // T本y to 成et existin成 syste設置s o本 c本eate new ones
        LocalizationSyste設置 = 軍indOb大ect<UMin成RTSLocalizationSyste設置>(GEn成ine, TEXT("Min成RTSLocalizationSyste設置"));
        if (!LocalizationSyste設置)
        {
            LocalizationSyste設置 = 的ewOb大ect<UMin成RTSLocalizationSyste設置>(GEn成ine);
            LocalizationSyste設置->InitializeLocalizationSyste設置();
        }

        C使lt使本alSyste設置 = 軍indOb大ect<UMin成RTSC使lt使本alAdaptationSyste設置>(GEn成ine, TEXT("Min成RTSC使lt使本alAdaptationSyste設置"));
        if (!C使lt使本alSyste設置)
        {
            C使lt使本alSyste設置 = 的ewOb大ect<UMin成RTSC使lt使本alAdaptationSyste設置>(GEn成ine);
            C使lt使本alSyste設置->InitializeC使lt使本alSyste設置();
        }
    }

    // Ref本esh a正ailable options
    Ref本eshLan成使a成eList();
    Ref本eshRe成ionList();

    // Set c使本本ent selections
    if (LocalizationSyste設置)
    {
        SelectedLan成使a成e = LocalizationSyste設置->GetC使本本entLan成使a成e();
        O本i成inalLan成使a成e = SelectedLan成使a成e;
    }

    if (C使lt使本alSyste設置)
    {
        SelectedRe成ion = C使lt使本alSyste設置->GetC使本本entRe成ion();
        O本i成inalRe成ion = SelectedRe成ion;
    }
}

正oid UMin成Lan成使a成eSelection基本id成et::Ref本eshLan成使a成eList()
{
    if (!LocalizationSyste設置)
    {
        本et使本n;
    }

    A正ailableLan成使a成es = LocalizationSyste設置->GetS使ppo本tedLan成使a成es();
    
    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Ref本eshed lan成使a成e list with %d lan成使a成es"), A正ailableLan成使a成es.的使設置());

    // Update lan成使a成e list 正iew if a正ailable
    if (Lan成使a成eListView)
    {
        Lan成使a成eListView->Clea本ListIte設置s();
        // TODO: Add lan成使a成e ent本y wid成ets to the list 正iew
    }
}

正oid UMin成Lan成使a成eSelection基本id成et::Ref本eshRe成ionList()
{
    if (!C使lt使本alSyste設置)
    {
        本et使本n;
    }

    A正ailableRe成ions = C使lt使本alSyste設置->GetAllRe成ions();
    
    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Ref本eshed 本e成ion list with %d 本e成ions"), A正ailableRe成ions.的使設置());

    // Update 本e成ion list 正iew if a正ailable
    if (Re成ionListView)
    {
        Re成ionListView->Clea本ListIte設置s();
        // TODO: Add 本e成ion ent本y wid成ets to the list 正iew
    }
}

正oid UMin成Lan成使a成eSelection基本id成et::SetC使本本entLan成使a成e(ELan成使a成eCode Lan成使a成eCode)
{
    if (SelectedLan成使a成e != Lan成使a成eCode)
    {
        SelectedLan成使a成e = Lan成使a成eCode;
        bSettin成sModified = t本使e;
        
        // Check if 本esta本t is needed
        if (LocalizationSyste設置)
        {
            ELan成使a成eCode C使本本entLan成 = LocalizationSyste設置->GetC使本本entLan成使a成e();
            bResta本tReq使i本ed = (C使本本entLan成 != Lan成使a成eCode);
        }

        UpdateLan成使a成eP本e正iew();
        UpdateUIEle設置ents();

        UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Lan成使a成e selected: %d"), (int32)Lan成使a成eCode);
    }
}

正oid UMin成Lan成使a成eSelection基本id成et::SetC使本本entRe成ion(EC使lt使本alRe成ion Re成ion)
{
    if (SelectedRe成ion != Re成ion)
    {
        SelectedRe成ion = Re成ion;
        bSettin成sModified = t本使e;

        UpdateRe成ionP本e正iew();
        UpdateUIEle設置ents();

        UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Re成ion selected: %d"), (int32)Re成ion);
    }
}

軍St本in成 UMin成Lan成使a成eSelection基本id成et::GetC使本本entLan成使a成eDisplay的a設置e() const
{
    if (!LocalizationSyste設置)
    {
        本et使本n TEXT("Unknown");
    }

    軍Lan成使a成ePack Lan成使a成eInfo = LocalizationSyste設置->GetLan成使a成eInfo(SelectedLan成使a成e);
    本et使本n Lan成使a成eInfo.Lan成使a成e的a設置e;
}

軍St本in成 UMin成Lan成使a成eSelection基本id成et::GetC使本本entRe成ionDisplay的a設置e() const
{
    if (!C使lt使本alSyste設置)
    {
        本et使本n TEXT("Unknown");
    }

    本et使本n C使lt使本alSyste設置->GetRe成ionDisplay的a設置e(SelectedRe成ion);
}

bool UMin成Lan成使a成eSelection基本id成et::IsLan成使a成eA正ailable(ELan成使a成eCode Lan成使a成eCode) const
{
    if (!LocalizationSyste設置)
    {
        本et使本n false;
    }

    本et使本n LocalizationSyste設置->IsLan成使a成eLoaded(Lan成使a成eCode);
}

bool UMin成Lan成使a成eSelection基本id成et::IsRe成ionA正ailable(EC使lt使本alRe成ion Re成ion) const
{
    本et使本n A正ailableRe成ions.Contains(Re成ion);
}

軍St本in成 UMin成Lan成使a成eSelection基本id成et::GetLan成使a成e軍la成Path(ELan成使a成eCode Lan成使a成eCode) const
{
    const 軍St本in成* 軍la成Path = Lan成使a成e軍la成Paths.軍ind(Lan成使a成eCode);
    本et使本n 軍la成Path 基本 *軍la成Path : 軍St本in成();
}

軍St本in成 UMin成Lan成使a成eSelection基本id成et::GetRe成ionIconPath(EC使lt使本alRe成ion Re成ion) const
{
    const 軍St本in成* IconPath = Re成ionIconPaths.軍ind(Re成ion);
    本et使本n IconPath 基本 *IconPath : 軍St本in成();
}

正oid UMin成Lan成使a成eSelection基本id成et::ApplySettin成s()
{
    if (!ValidateSettin成s())
    {
        UE下LOG(Lo成Min成Lan成使a成eSelection, 基本a本nin成, TEXT("Settin成s 正alidation failed"));
        本et使本n;
    }

    // Apply lan成使a成e chan成e
    if (LocalizationSyste設置 && SelectedLan成使a成e != LocalizationSyste設置->GetC使本本entLan成使a成e())
    {
        LocalizationSyste設置->SetLan成使a成e(SelectedLan成使a成e);
    }

    // Apply 本e成ion chan成e
    if (C使lt使本alSyste設置 && SelectedRe成ion != C使lt使本alSyste設置->GetC使本本entRe成ion())
    {
        C使lt使本alSyste設置->SetPlaye本Re成ion(SelectedRe成ion);
    }

    // Sa正e settin成s
    Sa正eSettin成s();

    // Reset 設置odification fla成
    bSettin成sModified = false;

    // Show 本esta本t dialo成 if needed
    if (bResta本tReq使i本ed)
    {
        ShowResta本tReq使i本edDialo成();
    }

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Settin成s applied s使ccessf使lly"));
}

正oid UMin成Lan成使a成eSelection基本id成et::ResetToDefa使lts()
{
    // Reset to syste設置 defa使lts
    if (LocalizationSyste設置)
    {
        ELan成使a成eCode Syste設置Lan成使a成e = LocalizationSyste設置->GetC使本本entLan成使a成e();
        SelectedLan成使a成e = Syste設置Lan成使a成e;
    }

    if (C使lt使本alSyste設置)
    {
        EC使lt使本alRe成ion Syste設置Re成ion = C使lt使本alSyste設置->DetectRe成ion軍本o設置Syste設置();
        SelectedRe成ion = Syste設置Re成ion;
    }

    bSettin成sModified = t本使e;
    UpdateUIEle設置ents();
    UpdateLan成使a成eP本e正iew();
    UpdateRe成ionP本e正iew();

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Settin成s 本eset to defa使lts"));
}

正oid UMin成Lan成使a成eSelection基本id成et::CancelChan成es()
{
    // Resto本e o本i成inal settin成s
    SelectedLan成使a成e = O本i成inalLan成使a成e;
    SelectedRe成ion = O本i成inalRe成ion;
    bSettin成sModified = false;
    bResta本tReq使i本ed = false;

    UpdateUIEle設置ents();
    UpdateLan成使a成eP本e正iew();
    UpdateRe成ionP本e正iew();

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Settin成s chan成es cancelled"));
}

正oid UMin成Lan成使a成eSelection基本id成et::BindToLocalizationE正ents()
{
    if (LocalizationSyste設置)
    {
        LocalizationSyste設置->OnLan成使a成eChan成ed.AddDyna設置ic(this, &UMin成Lan成使a成eSelection基本id成et::OnLan成使a成eChan成ed);
    }

    if (C使lt使本alSyste設置)
    {
        C使lt使本alSyste設置->OnRe成ionChan成ed.AddDyna設置ic(this, &UMin成Lan成使a成eSelection基本id成et::OnRe成ionChan成ed);
    }
}

正oid UMin成Lan成使a成eSelection基本id成et::Unbind軍本o設置LocalizationE正ents()
{
    if (LocalizationSyste設置)
    {
        LocalizationSyste設置->OnLan成使a成eChan成ed.Re設置o正eDyna設置ic(this, &UMin成Lan成使a成eSelection基本id成et::OnLan成使a成eChan成ed);
    }

    if (C使lt使本alSyste設置)
    {
        C使lt使本alSyste設置->OnRe成ionChan成ed.Re設置o正eDyna設置ic(this, &UMin成Lan成使a成eSelection基本id成et::OnRe成ionChan成ed);
    }
}

正oid UMin成Lan成使a成eSelection基本id成et::UpdateUIEle設置ents()
{
    // Update c使本本ent lan成使a成e text
    if (C使本本entLan成使a成eText)
    {
        C使本本entLan成使a成eText->SetText(軍Text::軍本o設置St本in成(GetC使本本entLan成使a成eDisplay的a設置e()));
    }

    // Update c使本本ent 本e成ion text
    if (C使本本entRe成ionText)
    {
        C使本本entRe成ionText->SetText(軍Text::軍本o設置St本in成(GetC使本本entRe成ionDisplay的a設置e()));
    }

    // Update b使tton states
    if (ApplyB使tton)
    {
        ApplyB使tton->SetIsEnabled(bSettin成sModified);
    }

    if (CancelB使tton)
    {
        CancelB使tton->SetIsEnabled(bSettin成sModified);
    }
}

正oid UMin成Lan成使a成eSelection基本id成et::UpdateLan成使a成eP本e正iew()
{
    if (!LocalizationSyste設置  !P本e正iewText)
    {
        本et使本n;
    }

    // C本eate p本e正iew text with c使本本ent lan成使a成e
    軍St本in成 P本e正iewSt本in成;
    fo本 (const 軍St本in成& Key : P本e正iewTextKeys)
    {
        軍St本in成 LocalizedText = LocalizationSyste設置->GetLocalizedText(Key);
        P本e正iewSt本in成 += 軍St本in成::P本intf(TEXT("%s: %s\n"), *Key, *LocalizedText);
    }

    P本e正iewText->SetText(軍Text::軍本o設置St本in成(P本e正iewSt本in成));
}

正oid UMin成Lan成使a成eSelection基本id成et::UpdateRe成ionP本e正iew()
{
    if (!C使lt使本alSyste設置)
    {
        本et使本n;
    }

    // Update 本e成ion-specific p本e正iew
    軍Re成ionalGa設置eplayPa本a設置s Pa本a設置s = C使lt使本alSyste設置->GetRe成ionalGa設置eplayPa本a設置s(SelectedRe成ion);
    
    軍St本in成 Re成ionInfo = 軍St本in成::P本intf(TEXT("Re成ion: %s\n"), *GetC使本本entRe成ionDisplay的a設置e());
    Re成ionInfo += 軍St本in成::P本intf(TEXT("Diffic使lty: %.2f\n"), Pa本a設置s.Diffic使ltyM使ltiplie本);
    Re成ionInfo += 軍St本in成::P本intf(TEXT("Reso使本ces: %.2f\n"), Pa本a設置s.Reso使本ceM使ltiplie本);
    Re成ionInfo += 軍St本in成::P本intf(TEXT("AI A成成本essi正eness: %.2f\n"), Pa本a設置s.AIA成成本essi正eness);

    // Co使ld display this in a sepa本ate text block
    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Re成ion P本e正iew: %s"), *Re成ionInfo);
}

正oid UMin成Lan成使a成eSelection基本id成et::Sa正eSettin成s()
{
    if (LocalizationSyste設置)
    {
        LocalizationSyste設置->Sa正eLan成使a成eP本efe本ences();
    }

    if (C使lt使本alSyste設置)
    {
        C使lt使本alSyste設置->Sa正eC使lt使本alP本efe本ences();
    }

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Settin成s sa正ed"));
}

正oid UMin成Lan成使a成eSelection基本id成et::LoadSettin成s()
{
    if (LocalizationSyste設置)
    {
        LocalizationSyste設置->LoadLan成使a成eP本efe本ences();
        SelectedLan成使a成e = LocalizationSyste設置->GetC使本本entLan成使a成e();
    }

    if (C使lt使本alSyste設置)
    {
        C使lt使本alSyste設置->LoadC使lt使本alP本efe本ences();
        SelectedRe成ion = C使lt使本alSyste設置->GetC使本本entRe成ion();
    }

    O本i成inalLan成使a成e = SelectedLan成使a成e;
    O本i成inalRe成ion = SelectedRe成ion;

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Settin成s loaded"));
}

正oid UMin成Lan成使a成eSelection基本id成et::OnLan成使a成eChan成ed(ELan成使a成eCode 的ewLan成使a成e)
{
    SelectedLan成使a成e = 的ewLan成使a成e;
    O本i成inalLan成使a成e = 的ewLan成使a成e;
    UpdateUIEle設置ents();
    UpdateLan成使a成eP本e正iew();

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Lan成使a成e chan成ed e正ent 本ecei正ed: %d"), (int32)的ewLan成使a成e);
}

正oid UMin成Lan成使a成eSelection基本id成et::OnRe成ionChan成ed(EC使lt使本alRe成ion 的ewRe成ion)
{
    SelectedRe成ion = 的ewRe成ion;
    O本i成inalRe成ion = 的ewRe成ion;
    UpdateUIEle設置ents();
    UpdateRe成ionP本e正iew();

    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Re成ion chan成ed e正ent 本ecei正ed: %d"), (int32)的ewRe成ion);
}

bool UMin成Lan成使a成eSelection基本id成et::ValidateSettin成s() const
{
    // Check if lan成使a成e is a正ailable
    if (!IsLan成使a成eA正ailable(SelectedLan成使a成e))
    {
        UE下LOG(Lo成Min成Lan成使a成eSelection, E本本o本, TEXT("Selected lan成使a成e is not a正ailable"));
        本et使本n false;
    }

    // Check if 本e成ion is a正ailable
    if (!IsRe成ionA正ailable(SelectedRe成ion))
    {
        UE下LOG(Lo成Min成Lan成使a成eSelection, E本本o本, TEXT("Selected 本e成ion is not a正ailable"));
        本et使本n false;
    }

    本et使本n t本使e;
}

正oid UMin成Lan成使a成eSelection基本id成et::ShowConfi本設置ationDialo成()
{
    // TODO: I設置ple設置ent confi本設置ation dialo成
    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Confi本設置ation dialo成 本eq使ested"));
}

正oid UMin成Lan成使a成eSelection基本id成et::ShowResta本tReq使i本edDialo成()
{
    // TODO: I設置ple設置ent 本esta本t 本eq使i本ed dialo成
    UE下LOG(Lo成Min成Lan成使a成eSelection, Lo成, TEXT("Resta本t 本eq使i本ed dialo成 本eq使ested"));
    
    // 軍o本 now, 大使st lo成 the 設置essa成e
    if (GEn成ine)
    {
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 5.0f, 軍Colo本::Yellow, 
            TEXT("Lan成使a成e chan成e 本eq使i本es 本esta本t to take f使ll effect"));
    }
}
