#include "SageBrain/MingSageCharacterSelectionUI.h"
#include "SageBrain/MingSageCharacterTraits.h"
#include "SageBrain/MingDemonKingTraits.h"
#include "SageBrain/MingFalseSageTraits.h"
#include "Engine/Engine.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

UMingSageCharacterSelectionUI::UMingSageCharacterSelectionUI()
{
    SelectedCharacterType = ESageCharacterType::None;
    bIsSelectionVisible = false;
    bAllowCustomNaming = true;
    bShowPreview = true;
    PreviewTransitionDuration = 0.5f;
    bEnableRandomSelection = true;
    bRequireCharacterName = true;
}

void UMingSageCharacterSelectionUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitializeCharacterData();
    InitializeSelectionUI();
}

void UMingSageCharacterSelectionUI::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingSageCharacterSelectionUI::InitializeSelectionUI()
{
    bIsSelectionVisible = false;
    SelectedCharacterType = ESageCharacterType::None;
    CustomCharacterName = "";
    
    InitializeCharacterData();
    
    OnSelectionUIInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Character Selection UI initialized"));
}

void UMingSageCharacterSelectionUI::ShowCharacterSelection()
{
    bIsSelectionVisible = true;
    ApplyCharacterFilters();
    
    UE_LOG(LogTemp, Log, TEXT("Character Selection UI shown"));
}

void UMingSageCharacterSelectionUI::HideCharacterSelection()
{
    bIsSelectionVisible = false;
    OnSelectionUIHidden();
    
    UE_LOG(LogTemp, Log, TEXT("Character Selection UI hidden"));
}

void UMingSageCharacterSelectionUI::SelectCharacter(ESageCharacterType CharacterType)
{
    if (CharacterType == ESageCharacterType::None)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot select None character type"));
        return;
    }

    SelectedCharacterType = CharacterType;
    UpdateCharacterPreview();
    
    OnCharacterSelected.Broadcast(CharacterType);
    OnCharacterTypeSelected(CharacterType);
    
    UE_LOG(LogTemp, Log, TEXT("Character selected: %d"), (int32)CharacterType);
}

void UMingSageCharacterSelectionUI::PreviewCharacter(ESageCharacterType CharacterType)
{
    if (CharacterType == ESageCharacterType::None)
    {
        return;
    }

    for (const FCharacterSelectionData& Data : CharacterOptions)
    {
        if (Data.CharacterType == CharacterType)
        {
            CurrentPreviewData = Data;
            OnCharacterPreviewChanged.Broadcast(CurrentPreviewData);
            break;
        }
    }
}

void UMingSageCharacterSelectionUI::ConfirmSelection()
{
    if (!CanConfirmSelection())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot confirm selection - requirements not met"));
        return;
    }

    FString FinalCharacterName = CustomCharacterName;
    if (FinalCharacterName.IsEmpty())
    {
        FinalCharacterName = CurrentPreviewData.CharacterName;
    }

    OnSelectionConfirmed.Broadcast(SelectedCharacterType, FinalCharacterName);
    OnSelectionConfirmedEvent(SelectedCharacterType, FinalCharacterName);
    
    UE_LOG(LogTemp, Log, TEXT("Character selection confirmed: %d - %s"), (int32)SelectedCharacterType, *FinalCharacterName);
}

void UMingSageCharacterSelectionUI::CancelSelection()
{
    SelectedCharacterType = ESageCharacterType::None;
    CustomCharacterName = "";
    CurrentPreviewData = FCharacterSelectionData();
    
    UE_LOG(LogTemp, Log, TEXT("Character selection cancelled"));
}

void UMingSageCharacterSelectionUI::SetCustomCharacterName(const FString& CharacterName)
{
    CustomCharacterName = CharacterName;
}

TArray<FCharacterSelectionData> UMingSageCharacterSelectionUI::GetAllCharacterOptions() const
{
    return CharacterOptions;
}

void UMingSageCharacterSelectionUI::FilterCharacterOptions(const TArray<ESageCharacterType>& AllowedTypes)
{
    AllowedCharacterTypes = AllowedTypes;
    ApplyCharacterFilters();
}

bool UMingSageCharacterSelectionUI::CanConfirmSelection() const
{
    if (SelectedCharacterType == ESageCharacterType::None)
    {
        return false;
    }

    if (bRequireCharacterName && CustomCharacterName.IsEmpty() && CurrentPreviewData.CharacterName.IsEmpty())
    {
        return false;
    }

    if (AllowedCharacterTypes.Num() > 0 && !AllowedCharacterTypes.Contains(SelectedCharacterType))
    {
        return false;
    }

    return true;
}

void UMingSageCharacterSelectionUI::RandomCharacterSelection()
{
    if (!bEnableRandomSelection)
    {
        return;
    }

    TArray<ESageCharacterType> AvailableTypes;
    if (AllowedCharacterTypes.Num() > 0)
    {
        AvailableTypes = AllowedCharacterTypes;
    }
    else
    {
        AvailableTypes = {ESageCharacterType::Sage, ESageCharacterType::DemonKing, ESageCharacterType::FalseSage};
    }

    if (AvailableTypes.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AvailableTypes.Num() - 1);
        SelectCharacter(AvailableTypes[RandomIndex]);
    }
}

void UMingSageCharacterSelectionUI::LoadCharacterPresets()
{
    InitializeCharacterData();
}

void UMingSageCharacterSelectionUI::SaveCurrentSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Character selection saved: %d"), (int32)SelectedCharacterType);
}

FString UMingSageCharacterSelectionUI::GetCharacterRecommendation() const
{
    if (AllowedCharacterTypes.Num() == 1)
    {
        ESageCharacterType OnlyType = AllowedCharacterTypes[0];
        switch (OnlyType)
        {
        case ESageCharacterType::Sage:
            return TEXT("聖者適合喜歡道德引導和團合作的玩家");
        case ESageCharacterType::DemonKing:
            return TEXT("魔王適合喜歡強力統治和心理戰術的玩家");
        case ESageCharacterType::FalseSage:
            return TEXT("偽聖者適合喜歡策略欺騙和暗中操控的玩家");
        default:
            break;
        }
    }
    
    return TEXT("根據您的遊戲風格選擇適合的角色類型");
}

void UMingSageCharacterSelectionUI::InitializeCharacterData()
{
    CharacterOptions.Empty();

    FCharacterSelectionData SageData;
    SageData.CharacterType = ESageCharacterType::Sage;
    SageData.CharacterName = TEXT("聖者");
    SageData.CharacterTitle = TEXT("道德引導者");
    SageData.CharacterDescription = TEXT("以道德和智慧引導眾人，擁有強大的感召力和正義力量。");
    SageData.CharacterTraits = UMingSageTraits::GetSageDefaultTraits();
    SageData.AvailableAbilities = UMingSageTraits::GetSageSpecialAbilities();
    SageData.Strengths = {"高道德威望", "強大感召力", "團隊合作", "正義力量"};
    SageData.Weaknesses = {"風險規避", "攻擊力較低", "依賴盟友"};
    CharacterOptions.Add(SageData);

    FCharacterSelectionData DemonKingData;
    DemonKingData.CharacterType = ESageCharacterType::DemonKing;
    DemonKingData.CharacterName = TEXT("魔王");
    DemonKingData.CharacterTitle = TEXT("恐懼統治者");
    DemonKingData.CharacterDescription = TEXT("以恐懼和力量統治，擅長黑暗魔法和心理戰術。");
    DemonKingData.CharacterTraits = UMingDemonKingTraits::GetDemonKingDefaultTraits();
    DemonKingData.AvailableAbilities = UMingDemonKingTraits::GetDemonKingSpecialAbilities();
    DemonKingData.Strengths = {"強大軍事指揮", "恐懼震懾", "獨立作戰", "高風險承受"};
    DemonKingData.Weaknesses = {"低道德威望", "負面魅力", "容易引起反抗"};
    CharacterOptions.Add(DemonKingData);

    FCharacterSelectionData FalseSageData;
    FalseSageData.CharacterType = ESageCharacterType::FalseSage;
    FalseSageData.CharacterName = TEXT("偽聖者");
    FalseSageData.CharacterTitle = TEXT("陰謀家");
    FalseSageData.CharacterDescription = TEXT("外表聖潔內心邪惡，精通欺騙和陰謀術。");
    FalseSageData.CharacterTraits = UMingFalseSageTraits::GetFalseSageDefaultTraits();
    FalseSageData.AvailableAbilities = UMingFalseSageTraits::GetFalseSageSpecialAbilities();
    FalseSageData.Strengths = {"高戰略思維", "欺騙能力", "暗中操控", "靈活應變"};
    FalseSageData.Weaknesses = {"道德風險", "容易被識破", "信譽問題"};
    CharacterOptions.Add(FalseSageData);
}

void UMingSageCharacterSelectionUI::UpdateCharacterPreview()
{
    for (const FCharacterSelectionData& Data : CharacterOptions)
    {
        if (Data.CharacterType == SelectedCharacterType)
        {
            CurrentPreviewData = Data;
            OnCharacterPreviewChanged.Broadcast(CurrentPreviewData);
            break;
        }
    }
}

void UMingSageCharacterSelectionUI::ValidateSelection()
{
    if (!CanConfirmSelection())
    {
        UE_LOG(LogTemp, Warning, TEXT("Selection validation failed"));
    }
}

void UMingSageCharacterSelectionUI::ApplyCharacterFilters()
{
    if (AllowedCharacterTypes.Num() == 0)
    {
        return;
    }

    TArray<FCharacterSelectionData> FilteredOptions;
    for (const FCharacterSelectionData& Data : CharacterOptions)
    {
        if (AllowedCharacterTypes.Contains(Data.CharacterType))
        {
            FilteredOptions.Add(Data);
        }
    }
    
    CharacterOptions = FilteredOptions;
}
