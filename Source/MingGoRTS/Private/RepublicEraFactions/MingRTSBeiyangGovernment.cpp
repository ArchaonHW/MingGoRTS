#include "RepublicEraFactions/MingRTSBeiyangGovernment.h"
#include "Factions/FactionTypes.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingRTSBeiyangGovernment::UMingRTSBeiyangGovernment()
{
    FactionType = ERepublicEraFaction::BeiyangGovernment;
    bIsInitialized = false;
}

void UMingRTSBeiyangGovernment::InitializeFaction()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Beiyang Government faction..."));
    
    // 初始化基本屬性
    InitializeAttributes();
    
    // 初始化專屬單位
    InitializeUniqueUnits();
    
    // 初始化特色機制
    InitializeUniqueMechanics();
    
    // 初始化遊戲指南
    InitializeGameplayGuide();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("Beiyang Government faction initialized successfully"));
}

FFactionAttributes UMingRTSBeiyangGovernment::GetFactionAttributes() const
{
    FFactionAttributes Attributes;
    Attributes.FactionID = FName("BeiyangGovernment");
    Attributes.FactionName = FText::FromString(TEXT("北洋政府"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Beiyang Government"));
    Attributes.Difficulty = EFactionDifficulty::Easy;
    Attributes.StartingLocation = FVector2D(116.4f, 39.9f); // 北京
    Attributes.StartingYear = 1912;
    Attributes.EconomicPower = 85;
    Attributes.MilitaryPower = 90;
    Attributes.DiplomaticPower = 95;
    Attributes.CulturalPower = 80;
    Attributes.Stability = 60;
    Attributes.Corruption = 40;
    Attributes.PublicSupport = 50;
    
    return Attributes;
}

TArray<FFactionUnit> UMingRTSBeiyangGovernment::GetUniqueUnits() const
{
    return BeiyangUnits;
}

TArray<FFactionMechanic> UMingRTSBeiyangGovernment::GetUniqueMechanics() const
{
    return BeiyangMechanics;
}

FGameplayGuide UMingRTSBeiyangGovernment::GetGameplayGuide() const
{
    FGameplayGuide Guide;
    Guide.FactionName = TEXT("北洋政府");
    Guide.Description = TEXT("作為中華民國的合法政府，北洋政府擁有國際承認和外交優勢。利用正統地位和軍事實力，統一各路軍閥，重建國家秩序。");
    Guide.EarlyGameStrategy = TEXT("早期重點：1. 鞏固北京根據地；2. 利用外交優勢爭取外國支持；3. 建立正規軍隊；4. 控制關稅收入");
    Guide.MidGameStrategy = TEXT("中期重點：1. 擊敗主要軍閥對手；2. 擴大控制區域；3. 推進現代化建設；4. 維護國際關係");
    Guide.LateGameStrategy = TEXT("後期重點：1. 完成國家統一；2. 建立現代政府體制；3. 推動經濟發展；4. 應對外來威脅");
    Guide.CommonMistakes = TEXT("常見錯誤：1. 過度依賴外國貸款；2. 忽視內部穩定；3. 軍事擴張過快；4. 外交處理不當");
    Guide.TipsAndTricks = TEXT("實用技巧：1. 善用外交優勢；2. 平衡各派系關係；3. 重視海軍建設；4. 控制媒體輿論");
    
    return Guide;
}

void UMingRTSBeiyangGovernment::InitializeAttributes()
{
    // 設置北洋政府的基本屬性
    FactionName = TEXT("北洋政府");
    FactionNameEN = TEXT("Beiyang Government");
    DifficultyLevel = 2;
    StartingLocation = FVector2D(116.4f, 39.9f);
    StartingYear = 1912;
    HistoricalBackground = TEXT("1912年成立的中華民國中央政府，由袁世凱領導，控制華北地區，擁有正統性和外交優勢。");
    
    // 經濟特色
    EconomicFeatures.Add(TEXT("控制關稅"));
    EconomicFeatures.Add(TEXT("外國貸款"));
    EconomicFeatures.Add(TEXT("鐵路收入"));
    EconomicFeatures.Add(TEXT("工業基礎"));
    
    // 軍事特色
    MilitaryFeatures.Add(TEXT("北洋新軍"));
    MilitaryFeatures.Add(TEXT("德式訓練"));
    MilitaryFeatures.Add(TEXT("裝備精良"));
    MilitaryFeatures.Add(TEXT("海軍實力"));
    
    // 外交特色
    DiplomaticFeatures.Add(TEXT("國際承認"));
    DiplomaticFeatures.Add(TEXT("外交保護"));
    DiplomaticFeatures.Add(TEXT("條約權利"));
    DiplomaticFeatures.Add(TEXT("列強支持"));
    
    // 特色機制
    UniqueMechanics.Add(TEXT("中央正統"));
    UniqueMechanics.Add(TEXT("外交優勢"));
    UniqueMechanics.Add(TEXT("軍事強大"));
    UniqueMechanics.Add(TEXT("內部不穩定"));
}

void UMingRTSBeiyangGovernment::InitializeUniqueUnits()
{
    BeiyangUnits.Empty();
    
    // 北洋新軍步兵
    FFactionUnit BeiyangInfantry = CreateUnitDefinition(
        FName("BeiyangInfantry"),
        FText::FromString(TEXT("北洋新軍步兵")),
        FText::FromString(TEXT("接受德式訓練的現代化步兵，裝備精良，訓練有素")),
        EFactionUnitType::Infantry,
        FFactionUnitStats(80, 15, 12, 100, 1.0f, 2.0f)
    );
    BeiyangUnits.Add(BeiyangInfantry);
    
    // 北洋騎兵
    FFactionUnit BeiyangCavalry = CreateUnitDefinition(
        FName("BeiyangCavalry"),
        FText::FromString(TEXT("北洋騎兵")),
        FText::FromString(TEXT("傳統騎兵與現代騎兵結合，機動性強，適合快速突擊")),
        EFactionUnitType::Cavalry,
        FFactionUnitStats(120, 25, 8, 150, 1.5f, 3.0f)
    );
    BeiyangUnits.Add(BeiyangCavalry);
    
    // 北洋炮兵
    FFactionUnit BeiyangArtillery = CreateUnitDefinition(
        FName("BeiyangArtillery"),
        FText::FromString(TEXT("北洋炮兵")),
        FText::FromString(TEXT("裝備德式克虜伯大炮，火力強大，遠程打擊能力突出")),
        EFactionUnitType::Artillery,
        FFactionUnitStats(60, 45, 5, 200, 0.8f, 4.0f)
    );
    BeiyangUnits.Add(BeiyangArtillery);
    
    // 北洋海軍陸戰隊
    FFactionUnit BeiyangMarine = CreateUnitDefinition(
        FName("BeiyangMarine"),
        FText::FromString(TEXT("北洋海軍陸戰隊")),
        FText::FromString(TEXT("精銳海軍陸戰部隊，兩棲作戰能力強，裝備優良")),
        EFactionUnitType::Marine,
        FFactionUnitStats(90, 20, 15, 120, 1.1f, 2.5f)
    );
    BeiyangUnits.Add(BeiyangMarine);
}

void UMingRTSBeiyangGovernment::InitializeUniqueMechanics()
{
    BeiyangMechanics.Empty();
    
    // 中央正統機制
    FFactionMechanic CentralLegitimacy = CreateMechanicDefinition(
        FName("CentralLegitimacy"),
        FText::FromString(TEXT("中央正統")),
        FText::FromString(TEXT("作為合法中央政府，享有國際承認，外交行動成功率+20%，可獲得外國援助")),
        EFactionMechanicType::Diplomatic,
        1.2f
    );
    BeiyangMechanics.Add(CentralLegitimacy);
    
    // 外交優勢機制
    FFactionMechanic DiplomaticAdvantage = CreateMechanicDefinition(
        FName("DiplomaticAdvantage"),
        FText::FromString(TEXT("外交優勢")),
        FText::FromString(TEXT("與各國關係良好，可簽署有利條約，獲得外國貸款和軍事顧問")),
        EFactionMechanicType::Economic,
        1.3f
    );
    BeiyangMechanics.Add(DiplomaticAdvantage);
    
    // 軍事強大機制
    FFactionMechanic MilitaryPower = CreateMechanicDefinition(
        FName("MilitaryPower"),
        FText::FromString(TEXT("軍事強大")),
        FText::FromString(TEXT("擁有正規軍隊和海軍，單位訓練成本-15%，戰鬥力+10%")),
        EFactionMechanicType::Military,
        1.1f
    );
    BeiyangMechanics.Add(MilitaryPower);
    
    // 內部不穩定機制
    FFactionMechanic InternalInstability = CreateMechanicDefinition(
        FName("InternalInstability"),
        FText::FromString(TEXT("內部不穩定")),
        FText::FromString(TEXT("各派系鬥爭激烈，穩定性-20%，需要平衡各方勢力，但可能獲得額外支持")),
        EFactionMechanicType::Political,
        0.8f
    );
    BeiyangMechanics.Add(InternalInstability);
}

void UMingRTSBeiyangGovernment::InitializeGameplayGuide()
{
    // 遊戲指南已在GetGameplayGuide()中實現
    UE_LOG(LogTemp, Log, TEXT("Beiyang Government gameplay guide initialized"));
}

TArray<FString> UMingRTSBeiyangGovernment::GetAdvantages()
{
    TArray<FString> Advantages;
    Advantages.Add(TEXT("國際承認的合法地位"));
    Advantages.Add(TEXT("強大的外交影響力"));
    Advantages.Add(TEXT("精良的軍事裝備"));
    Advantages.Add(TEXT("豐厚的財政收入"));
    Advantages.Add(TEXT("現代化的軍事訓練"));
    return Advantages;
}

TArray<FString> UMingRTSBeiyangGovernment::GetDisadvantages()
{
    TArray<FString> Disadvantages;
    Disadvantages.Add(TEXT("內部派系鬥爭嚴重"));
    Disadvantages.Add(TEXT("依賴外國貸款"));
    Disadvantages.Add(TEXT("穩定性較差"));
    Disadvantages.Add(TEXT("改革阻力大"));
    Disadvantages.Add(TEXT("地方軍閥挑戰"));
    return Disadvantages;
}

TArray<FString> UMingRTSBeiyangGovernment::GetRecommendedStrategies()
{
    TArray<FString> Strategies;
    Strategies.Add(TEXT("利用外交優勢爭取國際支持"));
    Strategies.Add(TEXT("鞏固中央權威，打擊分裂勢力"));
    Strategies.Add(TEXT("推進軍事現代化"));
    Strategies.Add(TEXT("平衡各派系利益"));
    Strategies.Add(TEXT("控制經濟命脈"));
    return Strategies;
}

bool UMingRTSBeiyangGovernment::ExecuteUniqueMechanic(const FString& MechanicName, const TArray<FString>& Parameters)
{
    if (MechanicName == TEXT("CentralLegitimacy"))
    {
        // 執行中央正統機制
        UE_LOG(LogTemp, Log, TEXT("Executing Central Legitimacy mechanic for Beiyang Government"));
        // 增加外交成功率，獲得外國援助等
        return true;
    }
    else if (MechanicName == TEXT("DiplomaticAdvantage"))
    {
        // 執行外交優勢機制
        UE_LOG(LogTemp, Log, TEXT("Executing Diplomatic Advantage mechanic for Beiyang Government"));
        // 簽署條約，獲得貸款等
        return true;
    }
    else if (MechanicName == TEXT("MilitaryPower"))
    {
        // 執行軍事強大機制
        UE_LOG(LogTemp, Log, TEXT("Executing Military Power mechanic for Beiyang Government"));
        // 提升軍事實力，降低訓練成本等
        return true;
    }
    else if (MechanicName == TEXT("InternalInstability"))
    {
        // 處理內部不穩定機制
        UE_LOG(LogTemp, Log, TEXT("Handling Internal Instability mechanic for Beiyang Government"));
        // 處理派系鬥爭，平衡各方利益
        return true;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Unknown mechanic: %s"), *MechanicName);
    return false;
}
