#include "UI/MingCharacterSelectionWidget.h"

UMingCharacterSelectionWidget::UMingCharacterSelectionWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsSelectionConfirmed = false;
    SelectionHintText = TEXT("請選擇您的角色類型");
}

void UMingCharacterSelectionWidget::InitializeCharacterSelection()
{
    // 初始化默認角色
    InitializeDefaultCharacters();
    
    // 觸發初始化事件
    OnWidgetInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Character selection widget initialized"));
}

void UMingCharacterSelectionWidget::SetAvailableCharacters(const TArray<FCharacterInfo>& Characters)
{
    AvailableCharacters = Characters;
    
    UE_LOG(LogTemp, Log, TEXT("Set %d available characters"), Characters.Num());
}

void UMingCharacterSelectionWidget::SelectCharacter(const FCharacterInfo& Character)
{
    CurrentSelectedCharacter = Character;
    
    // 更新UI顯示
    ShowCharacterDetails(Character);
    UpdateCharacterPreview(Character);
    DisplayCharacterStats(Character.CharacterStats);
    DisplaySpecialAbilities(Character.SpecialAbilities);
    
    // 觸發選擇事件
    OnCharacterSelected.Broadcast(Character);
    OnCharacterHighlighted(Character);
    
    UE_LOG(LogTemp, Log, TEXT("Character selected: %s"), *Character.CharacterName);
}

void UMingCharacterSelectionWidget::ConfirmSelection()
{
    if (CurrentSelectedCharacter.CharacterName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No character selected to confirm"));
        return;
    }
    
    bIsSelectionConfirmed = true;
    
    // 觸發確認事件
    OnCharacterSelectionConfirmed.Broadcast();
    OnSelectionConfirmed();
    
    UE_LOG(LogTemp, Log, TEXT("Character selection confirmed: %s"), *CurrentSelectedCharacter.CharacterName);
}

void UMingCharacterSelectionWidget::CancelSelection()
{
    bIsSelectionConfirmed = false;
    CurrentSelectedCharacter = FCharacterInfo();
    
    // 觸發取消事件
    OnCharacterSelectionCancelled.Broadcast();
    OnSelectionCancelled();
    
    UE_LOG(LogTemp, Log, TEXT("Character selection cancelled"));
}

void UMingCharacterSelectionWidget::InitializeDefaultCharacters()
{
    AvailableCharacters.Empty();
    
    // 添加聖者角色
    AvailableCharacters.Add(CreateSageCharacterInfo());
    
    // 添加魔王角色
    AvailableCharacters.Add(CreateDemonKingCharacterInfo());
    
    // 添加偽聖者角色
    AvailableCharacters.Add(CreateFalseSageCharacterInfo());
    
    UE_LOG(LogTemp, Log, TEXT("Initialized %d default characters"), AvailableCharacters.Num());
}

FCharacterInfo UMingCharacterSelectionWidget::CreateSageCharacterInfo() const
{
    FCharacterInfo SageInfo;
    SageInfo.CharacterName = TEXT("聖者");
    SageInfo.CharacterDescription = TEXT("以道德和智慧引導眾人的領袖。擁有強大的道德權威和感召力，能夠激勵追隨者並維護正義。適合喜歡正面領導風格的玩家。");
    SageInfo.CharacterType = TEXT("Sage");
    
    // 設置統計數據
    SageInfo.CharacterStats.Add(TEXT("道德權威"), 85.0f);
    SageInfo.CharacterStats.Add(TEXT("戰略思維"), 75.0f);
    SageInfo.CharacterStats.Add(TEXT("軍事指揮"), 60.0f);
    SageInfo.CharacterStats.Add(TEXT("魅力加成"), 30.0f);
    SageInfo.CharacterStats.Add(TEXT"學習速度"), 1.5f);
    SageInfo.CharacterStats.Add(TEXT("決策速度"), 1.2f);
    SageInfo.CharacterStats.Add(TEXT("風險容忍度"), 0.3f);
    
    // 設置特殊能力
    SageInfo.SpecialAbilities.Add(TEXT("道德感召"));
    SageInfo.SpecialAbilities.Add(TEXT("智慧啟迪"));
    SageInfo.SpecialAbilities.Add(TEXT("民心歸向"));
    SageInfo.SpecialAbilities.Add(TEXT("正義審判"));
    SageInfo.SpecialAbilities.Add(TEXT("聖光加持"));
    
    return SageInfo;
}

FCharacterInfo UMingCharacterSelectionWidget::CreateDemonKingCharacterInfo() const
{
    FCharacterInfo DemonKingInfo;
    DemonKingInfo.CharacterName = TEXT("魔王");
    DemonKingInfo.CharacterDescription = TEXT("以力量和恐懼統治的暴君。擁有強大的軍事力量和威懾能力，能夠粉碎敵人的意志。適合喜歡強硬征服風格的玩家。");
    DemonKingInfo.CharacterType = TEXT("DemonKing");
    
    // 設置統計數據
    DemonKingInfo.CharacterStats.Add(TEXT("道德權威"), 20.0f);
    DemonKingInfo.CharacterStats.Add(TEXT("戰略思維"), 80.0f);
    DemonKingInfo.CharacterStats.Add(TEXT("軍事指揮"), 90.0f);
    DemonKingInfo.CharacterStats.Add(TEXT("魅力加成"), -10.0f);
    DemonKingInfo.CharacterStats.Add(TEXT("學習速度"), 1.0f);
    DemonKingInfo.CharacterStats.Add(TEXT("決策速度"), 1.8f);
    DemonKingInfo.CharacterStats.Add(TEXT("風險容忍度"), 0.8f);
    
    // 設置特殊能力
    DemonKingInfo.SpecialAbilities.Add(TEXT("魔威震懾"));
    DemonKingInfo.SpecialAbilities.Add(TEXT("黑暗統治"));
    DemonKingInfo.SpecialAbilities.Add(TEXT("恐懼散播"));
    DemonKingInfo.SpecialAbilities.Add(TEXT("邪惡腐化"));
    DemonKingInfo.SpecialAbilities.Add(TEXT("毀滅指令"));
    
    return DemonKingInfo;
}

FCharacterInfo UMingCharacterSelectionWidget::CreateFalseSageCharacterInfo() const
{
    FCharacterInfo FalseSageInfo;
    FalseSageInfo.CharacterName = TEXT("偽聖者");
    FalseSageInfo.CharacterDescription = TEXT("外表聖潔內心陰暗的操縱者。精通欺騙和陰謀，能在暗中操控局勢。適合喜歡隱蔽策略風格的玩家。");
    FalseSageInfo.CharacterType = TEXT("FalseSage");
    
    // 設置統計數據
    FalseSageInfo.CharacterStats.Add(TEXT("道德權威"), 40.0f);
    FalseSageInfo.CharacterStats.Add(TEXT("戰略思維"), 85.0f);
    FalseSageInfo.CharacterStats.Add(TEXT("軍事指揮"), 70.0f);
    FalseSageInfo.CharacterStats.Add(TEXT("魅力加成"), 15.0f);
    FalseSageInfo.CharacterStats.Add(TEXT("學習速度"), 1.3f);
    FalseSageInfo.CharacterStats.Add(TEXT("決策速度"), 1.5f);
    FalseSageInfo.CharacterStats.Add(TEXT("風險容忍度"), 0.6f);
    
    // 設置特殊能力
    FalseSageInfo.SpecialAbilities.Add(TEXT("偽善欺騙"));
    FalseSageInfo.SpecialAbilities.Add(TEXT("陰謀策劃"));
    FalseSageInfo.SpecialAbilities.Add(TEXT("暗中操控"));
    FalseSageInfo.SpecialAbilities.Add(TEXT("背叛突襲"));
    FalseSageInfo.SpecialAbilities.Add(TEXT("虛假宣傳"));
    
    return FalseSageInfo;
}

void UMingCharacterSelectionWidget::CompareCharacters(const FCharacterInfo& CharacterA, const FCharacterInfo& CharacterB)
{
    UE_LOG(LogTemp, Log, TEXT("Comparing characters: %s vs %s"), *CharacterA.CharacterName, *CharacterB.CharacterName);
    
    // 比較統計數據
    for (const auto& StatA : CharacterA.CharacterStats)
    {
        if (const float* ValueB = CharacterB.CharacterStats.Find(StatA.Key))
        {
            float Difference = StatA.Value - *ValueB;
            UE_LOG(LogTemp, Log, TEXT("%s: %s (%.2f) vs %s (%.2f) - Diff: %.2f"),
                *StatA.Key,
                *CharacterA.CharacterName, StatA.Value,
                *CharacterB.CharacterName, *ValueB,
                Difference);
        }
    }
}

FString UMingCharacterSelectionWidget::GetCharacterRecommendation(const FString& PlayStyle) const
{
    if (PlayStyle == TEXT("領導型") || PlayStyle == TEXT("正面") || PlayStyle == TEXT("支持"))
    {
        return TEXT("建議選擇聖者 - 適合喜歡正面領導和道德感召的玩家");
    }
    else if (PlayStyle == TEXT("征服型") || PlayStyle == TEXT("強硬") || PlayStyle == TEXT("戰鬥"))
    {
        return TEXT("建議選擇魔王 - 適合喜歡武力征服和威懾統治的玩家");
    }
    else if (PlayStyle == TEXT("策略型") || PlayStyle == TEXT("隱蔽") || PlayStyle == TEXT("操縱"))
    {
        return TEXT("建議選擇偽聖者 - 適合喜歡暗中操控和陰謀策略的玩家");
    }
    else if (PlayStyle == TEXT("平衡型") || PlayStyle == TEXT("全能"))
    {
        return TEXT("聖者或偽聖者都是平衡型玩家的好選擇，取決於您偏好正面還是隱蔽的風格");
    }
    
    return TEXT("請根據您的遊戲風格選擇適合的角色。聖者適合正面領導，魔王適合武力征服，偽聖者適合暗中操縱。");
}
