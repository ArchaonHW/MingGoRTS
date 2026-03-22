#include "MingGoRTSHistoricalCharacters.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSHistoricalCharacters::UMingGoRTSHistoricalCharacters()
{
    bIsInitialized = false;
}

void UMingGoRTSHistoricalCharacters::InitializeCharacterSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("歷史人物系統已經初始化"));
        return;
    }

    // 初始化歷史人物庫
    InitializeHistoricalCharacterLibrary();

    // 構建角色映射
    CharacterMap.Empty();
    for (const FMingHistoricalCharacter& Character : AllCharacters)
    {
        CharacterMap.Add(Character.CharacterID, Character);
        
        // 初始化角色關係
        if (!CharacterRelationships.Contains(Character.CharacterID))
        {
            CharacterRelationships.Add(Character.CharacterID, 50.0f); // 默認中立
        }
        
        // 初始化角色可用性
        if (!CharacterAvailability.Contains(Character.CharacterID))
        {
            CharacterAvailability.Add(Character.CharacterID, Character.bIsAvailable);
        }
    }

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("歷史人物系統初始化完成，共載入 %d 個人物"), AllCharacters.Num());
}

TArray<FMingHistoricalCharacter> UMingGoRTSHistoricalCharacters::GetAllCharacters() const
{
    return AllCharacters;
}

FMingHistoricalCharacter UMingGoRTSHistoricalCharacters::GetCharacter(const FString& CharacterID) const
{
    if (const FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID))
    {
        return *Character;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到歷史人物 ID：%s"), *CharacterID);
    return FMingHistoricalCharacter();
}

float UMingGoRTSHistoricalCharacters::GetCharacterRelationship(const FString& CharacterID) const
{
    if (const float* Relationship = CharacterRelationships.Find(CharacterID))
    {
        return *Relationship;
    }
    
    return 50.0f; // 默認中立關係
}

void UMingGoRTSHistoricalCharacters::UpdateCharacterRelationship(const FString& CharacterID, float RelationshipChange)
{
    float CurrentRelationship = GetCharacterRelationship(CharacterID);
    float NewRelationship = FMath::Clamp(CurrentRelationship + RelationshipChange, 0.0f, 100.0f);
    
    CharacterRelationships.Add(CharacterID, NewRelationship);
    
    // 廣播關係變化事件
    OnCharacterRelationshipChanged.Broadcast(CharacterID, NewRelationship);
    
    UE_LOG(LogTemp, Log, TEXT("角色 %s 關係更新：%.1f -> %.1f"), *CharacterID, CurrentRelationship, NewRelationship);
}

TArray<FMingHistoricalCharacter> UMingGoRTSHistoricalCharacters::GetAvailableCharacters() const
{
    TArray<FMingHistoricalCharacter> AvailableCharacters;
    
    for (const FMingHistoricalCharacter& Character : AllCharacters)
    {
        if (Character.bIsAvailable && Character.bIsAlive)
        {
            AvailableCharacters.Add(Character);
        }
    }
    
    return AvailableCharacters;
}

TArray<FMingHistoricalCharacter> UMingGoRTSHistoricalCharacters::GetCharactersByFaction(EHistoricalFaction Faction) const
{
    TArray<FMingHistoricalCharacter> FactionCharacters;
    
    for (const FMingHistoricalCharacter& Character : AllCharacters)
    {
        if (Character.Faction == Faction && Character.bIsAvailable)
        {
            FactionCharacters.Add(Character);
        }
    }
    
    return FactionCharacters;
}

TArray<FMingHistoricalCharacter> UMingGoRTSHistoricalCharacters::GetCharactersByRole(ECharacterRole Role) const
{
    TArray<FMingHistoricalCharacter> RoleCharacters;
    
    for (const FMingHistoricalCharacter& Character : AllCharacters)
    {
        if (Character.Role == Role && Character.bIsAvailable)
        {
            RoleCharacters.Add(Character);
        }
    }
    
    return RoleCharacters;
}

TArray<FMingHistoricalCharacter> UMingGoRTSHistoricalCharacters::GetCharactersAtLocation(const FString& Location) const
{
    TArray<FMingHistoricalCharacter> LocationCharacters;
    
    for (const FMingHistoricalCharacter& Character : AllCharacters)
    {
        if (Character.KnownLocations.Contains(Location) && Character.bIsAvailable)
        {
            LocationCharacters.Add(Character);
        }
    }
    
    return LocationCharacters;
}

TArray<FMingHistoricalCharacter> UMingGoRTSHistoricalCharacters::GetCharactersInEvent(const FString& EventID) const
{
    TArray<FMingHistoricalCharacter> EventCharacters;
    
    for (const FMingHistoricalCharacter& Character : AllCharacters)
    {
        if (Character.KeyEvents.Contains(EventID) && Character.bIsAvailable)
        {
            EventCharacters.Add(Character);
        }
    }
    
    return EventCharacters;
}

bool UMingGoRTSHistoricalCharacters::IsCharacterAvailable(const FString& CharacterID) const
{
    if (const bool* bAvailable = CharacterAvailability.Find(CharacterID))
    {
        return *bAvailable;
    }
    
    return false;
}

void UMingGoRTSHistoricalCharacters::SetCharacterAvailability(const FString& CharacterID, bool bIsAvailable)
{
    CharacterAvailability.Add(CharacterID, bIsAvailable);
    
    // 廣播可用性變化事件
    OnCharacterAvailabilityChanged.Broadcast(CharacterID, bIsAvailable);
    
    UE_LOG(LogTemp, Log, TEXT("角色 %s 可用性設置為：%s"), *CharacterID, bIsAvailable ? TEXT("可用") : TEXT("不可用"));
}

TArray<FString> UMingGoRTSHistoricalCharacters::GetCharacterDialogueOptions(const FString& CharacterID) const
{
    TArray<FString> DialogueOptions;
    
    if (CharacterMap.Contains(CharacterID))
    {
        const FMingHistoricalCharacter& Character = CharacterMap[CharacterID];
        DialogueOptions = Character.DialogueIDs;
    }
    
    return DialogueOptions;
}

FString UMingGoRTSHistoricalCharacters::GenerateCharacterResponse(const FString& CharacterID, EDialogueAttitude PlayerAttitude, const FString& Context) const
{
    if (!CharacterMap.Contains(CharacterID))
    {
        return TEXT("我無法理解你的意思。");
    }
    
    const FMingHistoricalCharacter& Character = CharacterMap[CharacterID];
    return GenerateResponseText(Character, PlayerAttitude, Context);
}

float UMingGoRTSHistoricalCharacters::CalculateCharacterInfluence(const FString& CharacterID) const
{
    if (!CharacterMap.Contains(CharacterID))
    {
        return 0.0f;
    }
    
    const FMingHistoricalCharacter& Character = CharacterMap[CharacterID];
    
    // 基礎影響力
    float BaseInfluence = Character.Influence;
    
    // 角色關係加成
    float RelationshipBonus = (GetCharacterRelationship(CharacterID) - 50.0f) * 0.2f;
    
    // 角色重要性加成
    float RoleBonus = 0.0f;
    switch (Character.Role)
    {
    case ECharacterRole::Leader:
        RoleBonus = 20.0f;
        break;
    case ECharacterRole::Military:
        RoleBonus = 15.0f;
        break;
    case ECharacterRole::Political:
        RoleBonus = 12.0f;
        break;
    case ECharacterRole::Diplomatic:
        RoleBonus = 10.0f;
        break;
    default:
        RoleBonus = 5.0f;
        break;
    }
    
    return FMath::Clamp(BaseInfluence + RelationshipBonus + RoleBonus, 0.0f, 100.0f);
}

TArray<FString> UMingGoRTSHistoricalCharacters::GetRecommendedDialogues(const FString& CharacterID, const FMingCharacterData& PlayerCharacter) const
{
    TArray<FString> RecommendedDialogues;
    
    if (!CharacterMap.Contains(CharacterID))
    {
        return RecommendedDialogues;
    }
    
    const FMingHistoricalCharacter& Character = CharacterMap[CharacterID];
    
    // 根據角色關係和玩家屬性推薦對話
    float Relationship = GetCharacterRelationship(CharacterID);
    
    // 高關係值推薦友好對話
    if (Relationship > 70.0f)
    {
        for (const FString& DialogueID : Character.DialogueIDs)
        {
            if (DialogueID.Contains(TEXT("Friendly")) || DialogueID.Contains(TEXT("Loyal")))
            {
                RecommendedDialogues.Add(DialogueID);
            }
        }
    }
    // 低關係值推薦中立的對話
    else if (Relationship < 30.0f)
    {
        for (const FString& DialogueID : Character.DialogueIDs)
        {
            if (DialogueID.Contains(TEXT("Neutral")) || DialogueID.Contains(TEXT("Formal")))
            {
                RecommendedDialogues.Add(DialogueID);
            }
        }
    }
    // 中等關係值推薦常規對話
    else
    {
        RecommendedDialogues = Character.DialogueIDs;
    }
    
    return RecommendedDialogues;
}

bool UMingGoRTSHistoricalCharacters::SaveCharacterData(const FString& SaveSlotName)
{
    // TODO: 實現角色數據保存
    UE_LOG(LogTemp, Log, TEXT("保存角色數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSHistoricalCharacters::LoadCharacterData(const FString& SaveSlotName)
{
    // TODO: 實現角色數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入角色數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSHistoricalCharacters::InitializeHistoricalCharacterLibrary()
{
    CreateNationalistCharacters();
    CreateCommunistCharacters();
    CreateWarlordCharacters();
    CreateIndependentCharacters();
    CreateForeignCharacters();
    CreateCivilianCharacters();
}

void UMingGoRTSHistoricalCharacters::CreateNationalistCharacters()
{
    // 蔣介石
    FMingHistoricalCharacter ChiangKaiShek;
    ChiangKaiShek.CharacterID = GenerateCharacterID(TEXT("ChiangKaiShek"));
    ChiangKaiShek.CharacterName = TEXT("蔣介石");
    ChiangKaiShek.CourtesyName = TEXT("介石");
    ChiangKaiShek.BirthDate = TEXT("1887-10-31");
    ChiangKaiShek.DeathDate = TEXT("1975-04-05");
    ChiangKaiShek.Role = ECharacterRole::Leader;
    ChiangKaiShek.Faction = EHistoricalFaction::Nationalist;
    ChiangKaiShek.Title = TEXT("國民政府主席");
    ChiangKaiShek.Description = TEXT("中華民國國民黨總裁，領導北伐和抗戰的重要人物");
    ChiangKaiShek.PortraitPath = TEXT("/Game/Portraits/ChiangKaiShek.ChiangKaiShek");
    ChiangKaiShek.VoiceType = TEXT("Authoritative");
    ChiangKaiShek.bIsAlive = true;
    ChiangKaiShek.bIsAvailable = true;
    ChiangKaiShek.Influence = 95.0f;
    
    // 設置性格特徵
    ChiangKaiShek.Personality.Patriotism = 0.9f;
    ChiangKaiShek.Personality.Ambition = 0.8f;
    ChiangKaiShek.Personality.Conscientiousness = 0.7f;
    ChiangKaiShek.Personality.Extraversion = 0.6f;
    
    // 設置已知地點
    ChiangKaiShek.KnownLocations.Add(TEXT("Nanjing"));
    ChiangKaiShek.KnownLocations.Add(TEXT("Chongqing"));
    ChiangKaiShek.KnownLocations.Add(TEXT("Taipei"));
    
    // 設置關鍵事件
    ChiangKaiShek.KeyEvents.Add(TEXT("NorthernExpedition"));
    ChiangKaiShek.KeyEvents.Add(TEXT("SecondSinoJapaneseWar"));
    ChiangKaiShek.KeyEvents.Add(TEXT("ChineseCivilWar"));
    
    // 設置對話ID
    ChiangKaiShek.DialogueIDs.Add(TEXT("ChiangKaiShek_FirstMeeting"));
    ChiangKaiShek.DialogueIDs.Add(TEXT("ChiangKaiShek_MilitaryBriefing"));
    ChiangKaiShek.DialogueIDs.Add(TEXT("ChiangKaiShek_PoliticalDiscussion"));
    
    AllCharacters.Add(ChiangKaiShek);

    // 宋美齡
    FMingHistoricalCharacter SoongMeiLing;
    SoongMeiLing.CharacterID = GenerateCharacterID(TEXT("SoongMeiLing"));
    SoongMeiLing.CharacterName = TEXT("宋美齡");
    SoongMeiLing.CourtesyName = TEXT("美齡");
    SoongMeiLing.BirthDate = TEXT("1898-03-05");
    SoongMeiLing.DeathDate = TEXT("2003-10-23");
    SoongMeiLing.Role = ECharacterRole::Diplomatic;
    SoongMeiLing.Faction = EHistoricalFaction::Nationalist;
    SoongMeiLing.Title = TEXT("第一夫人");
    SoongMeiLing.Description = TEXT("蔣介石夫人，著名外交家，在國際舞台上為中國發聲");
    SoongMeiLing.PortraitPath = TEXT("/Game/Portraits/SoongMeiLing.SoongMeiLing");
    SoongMeiLing.VoiceType = TEXT("Elegant");
    SoongMeiLing.bIsAlive = true;
    SoongMeiLing.bIsAvailable = true;
    SoongMeiLing.Influence = 80.0f;
    
    SoongMeiLing.Personality.Extraversion = 0.8f;
    SoongMeiLing.Personality.Agreeableness = 0.7f;
    SoongMeiLing.Personality.Openness = 0.9f;
    
    SoongMeiLing.KnownLocations.Add(TEXT("Nanjing"));
    SoongMeiLing.KnownLocations.Add(TEXT("Washington"));
    SoongMeiLing.KnownLocations.Add(TEXT("NewYork"));
    
    SoongMeiLing.KeyEvents.Add(TEXT("DiplomaticMission"));
    SoongMeiLing.KeyEvents.Add(TEXT("InternationalConference"));
    
    SoongMeiLing.DialogueIDs.Add(TEXT("SoongMeiLing_DiplomaticTalk"));
    SoongMeiLing.DialogueIDs.Add(TEXT("SoongMeiLing_CulturalExchange"));
    
    AllCharacters.Add(SoongMeiLing);
}

void UMingGoRTSHistoricalCharacters::CreateCommunistCharacters()
{
    // 毛澤東
    FMingHistoricalCharacter MaoZedong;
    MaoZedong.CharacterID = GenerateCharacterID(TEXT("MaoZedong"));
    MaoZedong.CharacterName = TEXT("毛澤東");
    MaoZedong.CourtesyName = TEXT("潤之");
    MaoZedong.BirthDate = TEXT("1893-12-26");
    MaoZedong.DeathDate = TEXT("1976-09-09");
    MaoZedong.Role = ECharacterRole::Leader;
    MaoZedong.Faction = EHistoricalFaction::Communist;
    MaoZedong.Title = TEXT("中國共產黨主席");
    MaoZedong.Description = TEXT("中國共產黨創始人之一，領導中國革命的重要人物");
    MaoZedong.PortraitPath = TEXT("/Game/Portraits/MaoZedong.MaoZedong");
    MaoZedong.VoiceType = TEXT("Revolutionary");
    MaoZedong.bIsAlive = true;
    MaoZedong.bIsAvailable = true;
    MaoZedong.Influence = 90.0f;
    
    MaoZedong.Personality.Patriotism = 0.9f;
    MaoZedong.Personality.Ambition = 0.8f;
    MaoZedong.Personality.Openness = 0.7f;
    MaoZedong.Personality.Extraversion = 0.6f;
    
    MaoZedong.KnownLocations.Add(TEXT("Yan'an"));
    MaoZedong.KnownLocations.Add(TEXT("Beijing"));
    MaoZedong.KnownLocations.Add(TEXT("Shanghai"));
    
    MaoZedong.KeyEvents.Add(TEXT("LongMarch"));
    MaoZedong.KeyEvents.Add(TEXT("ChineseCivilWar"));
    MaoZedong.KeyEvents.Add(TEXT("CulturalRevolution"));
    
    MaoZedong.DialogueIDs.Add(TEXT("MaoZedong_FirstMeeting"));
    MaoZedong.DialogueIDs.Add(TEXT("MaoZedong_RevolutionaryTalk"));
    MaoZedong.DialogueIDs.Add(TEXT("MaoZedong_PhilosophicalDiscussion"));
    
    AllCharacters.Add(MaoZedong);

    // 周恩來
    FMingHistoricalCharacter ZhouEnlai;
    ZhouEnlai.CharacterID = GenerateCharacterID(TEXT("ZhouEnlai"));
    ZhouEnlai.CharacterName = TEXT("周恩來");
    ZhouEnlai.CourtesyName = TEXT("翔宇");
    ZhouEnlai.BirthDate = TEXT("1898-03-05");
    ZhouEnlai.DeathDate = TEXT("1976-01-08");
    ZhouEnlai.Role = ECharacterRole::Diplomatic;
    ZhouEnlai.Faction = EHistoricalFaction::Communist;
    ZhouEnlai.Title = TEXT("國務院總理");
    ZhouEnlai.Description = TEXT("中國共產黨重要領導人，著名外交家");
    ZhouEnlai.PortraitPath = TEXT("/Game/Portraits/ZhouEnlai.ZhouEnlai");
    ZhouEnlai.VoiceType = TEXT("Diplomatic");
    ZhouEnlai.bIsAlive = true;
    ZhouEnlai.bIsAvailable = true;
    ZhouEnlai.Influence = 85.0f;
    
    ZhouEnlai.Personality.Agreeableness = 0.8f;
    ZhouEnlai.Personality.Conscientiousness = 0.9f;
    ZhouEnlai.Personality.Extraversion = 0.7f;
    
    ZhouEnlai.KnownLocations.Add(TEXT("Beijing"));
    ZhouEnlai.KnownLocations.Add(TEXT("Geneva"));
    ZhouEnlai.KnownLocations.Add(TEXT("Bandung"));
    
    ZhouEnlai.KeyEvents.Add(TEXT("GenevaConference"));
    ZhouEnlai.KeyEvents.Add(TEXT("BandungConference"));
    
    ZhouEnlai.DialogueIDs.Add(TEXT("ZhouEnlai_DiplomaticTalk"));
    ZhouEnlai.DialogueIDs.Add(TEXT("ZhouEnlai_PolicyDiscussion"));
    
    AllCharacters.Add(ZhouEnlai);
}

void UMingGoRTSHistoricalCharacters::CreateWarlordCharacters()
{
    // 張作霖
    FMingHistoricalCharacter ZhangZuolin;
    ZhangZuolin.CharacterID = GenerateCharacterID(TEXT("ZhangZuolin"));
    ZhangZuolin.CharacterName = TEXT("張作霖");
    ZhangZuolin.CourtesyName = TEXT("雨亭");
    ZhangZuolin.BirthDate = TEXT("1875-03-19");
    ZhangZuolin.DeathDate = TEXT("1928-06-04");
    ZhangZuolin.Role = ECharacterRole::Military;
    ZhangZuolin.Faction = EHistoricalFaction::Warlord;
    ZhangZuolin.Title = TEXT("東三省巡閱使");
    ZhangZuolin.Description = TEXT("奉系軍閥首領，控制東北地區");
    ZhangZuolin.PortraitPath = TEXT("/Game/Portraits/ZhangZuolin.ZhangZuolin");
    ZhangZuolin.VoiceType = TEXT("Authoritative");
    ZhangZuolin.bIsAlive = false;
    ZhangZuolin.bIsAvailable = false; // 已去世
    ZhangZuolin.Influence = 75.0f;
    
    ZhangZuolin.Personality.Ambition = 0.8f;
    ZhangZuolin.Personality.Extraversion = 0.7f;
    ZhangZuolin.Personality.Conscientiousness = 0.6f;
    
    ZhangZuolin.KnownLocations.Add(TEXT("Shenyang"));
    ZhangZuolin.KnownLocations.Add(TEXT("Beijing"));
    
    ZhangZuolin.KeyEvents.Add(TEXT("FirstZhiliFengtianWar"));
    ZhangZuolin.KeyEvents.Add(TEXT("HuanggutunIncident"));
    
    AllCharacters.Add(ZhangZuolin);

    // 張學良
    FMingHistoricalCharacter ZhangXueliang;
    ZhangXueliang.CharacterID = GenerateCharacterID(TEXT("ZhangXueliang"));
    ZhangXueliang.CharacterName = TEXT("張學良");
    ZhangXueliang.CourtesyName = TEXT("漢卿");
    ZhangXueliang.BirthDate = TEXT("1901-06-03");
    ZhangXueliang.DeathDate = TEXT("2001-10-14");
    ZhangXueliang.Role = ECharacterRole::Military;
    ZhangXueliang.Faction = EHistoricalFaction::Warlord;
    ZhangXueliang.Title = TEXT("東三省邊防司令長官");
    ZhangXueliang.Description = TEXT("張作霖之子，西安事變的重要人物");
    ZhangXueliang.PortraitPath = TEXT("/Game/Portraits/ZhangXueliang.ZhangXueliang");
    ZhangXueliang.VoiceType = TEXT("YoungMilitary");
    ZhangXueliang.bIsAlive = true;
    ZhangXueliang.bIsAvailable = true;
    ZhangXueliang.Influence = 70.0f;
    
    ZhangXueliang.Personality.Patriotism = 0.8f;
    ZhangXueliang.Personality.Conscientiousness = 0.7f;
    ZhangXueliang.Personality.Openness = 0.6f;
    
    ZhangXueliang.KnownLocations.Add(TEXT("Shenyang"));
    ZhangXueliang.KnownLocations.Add(TEXT("Xi'an"));
    
    ZhangXueliang.KeyEvents.Add(TEXT("Xi'anIncident"));
    ZhangXueliang.KeyEvents.Add(TEXT("NortheastIncident"));
    
    ZhangXueliang.DialogueIDs.Add(TEXT("ZhangXueliang_FirstMeeting"));
    ZhangXueliang.DialogueIDs.Add(TEXT("ZhangXueliang_Xi'anTalk"));
    
    AllCharacters.Add(ZhangXueliang);
}

void UMingGoRTSHistoricalCharacters::CreateIndependentCharacters()
{
    // 魯迅
    FMingHistoricalCharacter LuXun;
    LuXun.CharacterID = GenerateCharacterID(TEXT("LuXun"));
    LuXun.CharacterName = TEXT("魯迅");
    LuXun.CourtesyName = TEXT("豫才");
    LuXun.BirthDate = TEXT("1881-09-25");
    LuXun.DeathDate = TEXT("1936-10-19");
    LuXun.Role = ECharacterRole::Cultural;
    LuXun.Faction = EHistoricalFaction::Independent;
    LuXun.Title = TEXT("文學家、思想家");
    LuXun.Description = TEXT("中國現代文學奠基人，新文化運動重要人物");
    LuXun.PortraitPath = TEXT("/Game/Portraits/LuXun.LuXun");
    LuXun.VoiceType = TEXT("Intellectual");
    LuXun.bIsAlive = false;
    LuXun.bIsAvailable = false; // 已去世
    LuXun.Influence = 65.0f;
    
    LuXun.Personality.Openness = 0.9f;
    LuXun.Personality.Conscientiousness = 0.8f;
    LuXun.Personality.Neuroticism = 0.6f;
    
    LuXun.KnownLocations.Add(TEXT("Beijing"));
    LuXun.KnownLocations.Add(TEXT("Shanghai"));
    LuXun.KnownLocations.Add(TEXT("Guangzhou"));
    
    LuXun.KeyEvents.Add(TEXT("NewCultureMovement"));
    LuXun.KeyEvents.Add(TEXT("MayFourthMovement"));
    
    AllCharacters.Add(LuXun);
}

void UMingGoRTSHistoricalCharacters::CreateForeignCharacters()
{
    // 陳納德
    FMingHistoricalCharacter Chennault;
    Chennault.CharacterID = GenerateCharacterID(TEXT("Chennault"));
    Chennault.CharacterName = TEXT("陳納德");
    Chennault.BirthDate = TEXT("1893-09-06");
    Chennault.DeathDate = TEXT("1958-07-27");
    Chennault.Role = ECharacterRole::Military;
    Chennault.Faction = EHistoricalFaction::Foreign;
    Chennault.Title = TEXT("飛虎隊指揮官");
    Chennault.Description = TEXT("美國飛行員，飛虎隊創始人，協助中國抗戰");
    Chennault.PortraitPath = TEXT("/Game/Portraits/Chennault.Chennault");
    Chennault.VoiceType = TEXT("AmericanMilitary");
    Chennault.bIsAlive = true;
    Chennault.bIsAvailable = true;
    Chennault.Influence = 60.0f;
    
    Chennault.Personality.Conscientiousness = 0.8f;
    Chennault.Personality.Agreeableness = 0.7f;
    Chennault.Personality.Extraversion = 0.6f;
    
    Chennault.KnownLocations.Add(TEXT("Kunming"));
    Chennault.KnownLocations.Add(TEXT("Chongqing"));
    Chennault.KnownLocations.Add(TEXT("Washington"));
    
    Chennault.KeyEvents.Add(TEXT("FlyingTigersFormation"));
    Chennault.KeyEvents.Add(TEXT("SecondSinoJapaneseWar"));
    
    Chennault.DialogueIDs.Add(TEXT("Chennault_MilitaryBriefing"));
    Chennault.DialogueIDs.Add(TEXT("Chennault_AviationTalk"));
    
    AllCharacters.Add(Chennault);
}

void UMingGoRTSHistoricalCharacters::CreateCivilianCharacters()
{
    // 普通村民
    FMingHistoricalCharacter Villager;
    Villager.CharacterID = GenerateCharacterID(TEXT("Villager"));
    Villager.CharacterName = TEXT("村民");
    Villager.Role = ECharacterRole::Civilian;
    Villager.Faction = EHistoricalFaction::Independent;
    Villager.Title = TEXT("普通農民");
    Villager.Description = TEXT("生活在農村的普通百姓，關心家園和收成");
    Villager.PortraitPath = TEXT("/Game/Portraits/Villager.Villager");
    Villager.VoiceType = TEXT("Rural");
    Villager.bIsAlive = true;
    Villager.bIsAvailable = true;
    Villager.Influence = 20.0f;
    
    Villager.Personality.Agreeableness = 0.7f;
    Villager.Personality.Conscientiousness = 0.6f;
    Villager.Personality.Extraversion = 0.5f;
    
    Villager.KnownLocations.Add(TEXT("RuralVillage"));
    Villager.KnownLocations.Add(TEXT("Town"));
    
    Villager.KeyEvents.Add(TEXT("LocalEvents"));
    
    Villager.DialogueIDs.Add(TEXT("Villager_DailyTalk"));
    Villager.DialogueIDs.Add(TEXT("Villager_QuestRequest"));
    
    AllCharacters.Add(Villager);
}

FString UMingGoRTSHistoricalCharacters::GenerateResponseText(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude, const FString& Context) const
{
    FString ResponseTemplate = GetResponseTemplate(Character, PlayerAttitude);
    
    // 根據角色性格和玩家態度生成回應
    if (PlayerAttitude == EDialogueAttitude::Friendly)
    {
        if (Character.Personality.Agreeableness > 0.7f)
        {
            return ResponseTemplate + TEXT("我很高興能與你交流。");
        }
        else if (Character.Personality.Extraversion > 0.6f)
        {
            return ResponseTemplate + TEXT("讓我們暢談一番！");
        }
    }
    else if (PlayerAttitude == EDialogueAttitude::Hostile)
    {
        if (Character.Personality.Neuroticism > 0.6f)
        {
            return ResponseTemplate + TEXT("你的態度讓我感到不安。");
        }
        else if (Character.Personality.Conscientiousness > 0.7f)
        {
            return ResponseTemplate + TEXT("我認為我們應該冷靜地討論問題。");
        }
    }
    else if (PlayerAttitude == EDialogueAttitude::Respectful)
    {
        if (Character.Role == ECharacterRole::Leader)
        {
            return ResponseTemplate + TEXT("你的敬意我感受到了。");
        }
        else if (Character.Personality.Extraversion > 0.5f)
        {
            return ResponseTemplate + TEXT("謝謝你的尊重。");
        }
    }
    
    return ResponseTemplate + TEXT("我明白你的意思。");
}

float UMingGoRTSHistoricalCharacters::CalculateRelationshipChange(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const
{
    float BaseChange = 0.0f;
    
    switch (PlayerAttitude)
    {
    case EDialogueAttitude::Friendly:
        BaseChange = 5.0f;
        break;
    case EDialogueAttitude::Respectful:
        BaseChange = 8.0f;
        break;
    case EDialogueAttitude::Neutral:
        BaseChange = 2.0f;
        break;
    case EDialogueAttitude::Suspicious:
        BaseChange = -3.0f;
        break;
    case EDialogueAttitude::Hostile:
        BaseChange = -8.0f;
        break;
    case EDialogueAttitude::Dismissive:
        BaseChange = -5.0f;
        break;
    }
    
    // 根據角色性格調整
    if (Character.Personality.Agreeableness > 0.7f)
    {
        BaseChange *= 1.2f;
    }
    else if (Character.Personality.Neuroticism > 0.6f)
    {
        BaseChange *= 1.5f;
    }
    
    return BaseChange;
}

bool UMingGoRTSHistoricalCharacters::CheckPersonalityMatch(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const
{
    // 檢查玩家態度是否與角色性格匹配
    switch (PlayerAttitude)
    {
    case EDialogueAttitude::Friendly:
        return Character.Personality.Agreeableness > 0.5f || Character.Personality.Extraversion > 0.5f;
    case EDialogueAttitude::Respectful:
        return Character.Role == ECharacterRole::Leader || Character.Personality.Conscientiousness > 0.6f;
    case EDialogueAttitude::Hostile:
        return Character.Personality.Neuroticism > 0.6f;
    default:
        return true;
    }
}

FString UMingGoRTSHistoricalCharacters::GetResponseTemplate(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const
{
    // 根據角色類型和玩家態度返回回應模板
    if (Character.Role == ECharacterRole::Leader)
    {
        switch (PlayerAttitude)
        {
        case EDialogueAttitude::Respectful:
            return TEXT("作為領袖，我認為...");
        case EDialogueAttitude::Friendly:
            return TEXT("我很欣賞你的熱情...");
        case EDialogueAttitude::Hostile:
            return TEXT("你的挑戰我已經注意到了...");
        default:
            return TEXT("關於這件事...");
        }
    }
    else if (Character.Role == ECharacterRole::Military)
    {
        switch (PlayerAttitude)
        {
        case EDialogueAttitude::Respectful:
            return TEXT("從軍事角度來看...");
        case EDialogueAttitude::Friendly:
            return TEXT("戰友，我認為...");
        case EDialogueAttitude::Hostile:
            return TEXT("你的威脅我已經記錄...");
        default:
            return TEXT("戰術上說...");
        }
    }
    else if (Character.Role == ECharacterRole::Diplomatic)
    {
        switch (PlayerAttitude)
        {
        case EDialogueAttitude::Respectful:
            return TEXT("在外交場合...");
        case EDialogueAttitude::Friendly:
            return TEXT("我很高興能與你交流...");
        case EDialogueAttitude::Hostile:
            return TEXT("你的言辭可能影響關係...");
        default:
            return TEXT("從外交角度...");
        }
    }
    
    return TEXT("我認為...");
}

FString UMingGoRTSHistoricalCharacters::GenerateCharacterID(const FString& BaseName) const
{
    return BaseName;
}

void UMingGoRTSHistoricalCharacters::UpdateCharacterInfluence(const FString& CharacterID)
{
    // TODO: 實現角色影響力更新邏輯
    UE_LOG(LogTemp, Log, TEXT("更新角色影響力：%s"), *CharacterID);
}

bool UMingGoRTSHistoricalCharacters::CheckHistoricalConsistency(const FString& CharacterID, const FString& CurrentDate) const
{
    if (!CharacterMap.Contains(CharacterID))
    {
        return false;
    }
    
    const FMingHistoricalCharacter& Character = CharacterMap[CharacterID];
    
    // 檢查角色是否在當前日期還在世
    if (!Character.DeathDate.IsEmpty())
    {
        // 簡化實現：如果角色有死亡日期，假設在1927年還在世
        // 實際實現需要更複雜的日期比較
        return Character.bIsAlive;
    }
    
    return true;
}
