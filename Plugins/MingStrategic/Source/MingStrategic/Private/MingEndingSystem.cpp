#include "MingEndingSystem.h"
#include "Engine/World.h"

UMingEndingSystem::UMingEndingSystem()
{
    WorldContext = GetWorld();
    InitializeEndings();
}

void UMingEndingSystem::InitializeEndings()
{
    Endings.Empty();
    VictoryConditions.Empty();
    DefeatConditions.Empty();
    UnlockedEndings.Empty();
    EndingScores.Empty();
    CurrentEndingID.Empty();
    
    SetupHistoricalEndings();
    SetupAlternateEndings();
    SetupSecretEndings();
    SetupBadEndings();
    
    UE_LOG(LogTemp, Log, TEXT("Ending system initialized with %d endings"), Endings.Num());
}

void UMingEndingSystem::SetupHistoricalEndings()
{
    // Historical Victory - Republic of China Established
    FMingEnding RepublicEnding;
    RepublicEnding.EndingID = TEXT("REPUBLIC_VICTORY");
    RepublicEnding.EndingTitle = TEXT("共和勝利");
    RepublicEnding.Description = TEXT("成功建立中華民國，實現三民主義理想");
    RepublicEnding.LongDescription = TEXT("在您的領導下，革命成功推翻清朝統治，建立了亞洲第一個共和國。孫中山的理想得以實現，中國開始了新的歷史篇章。雖然前路依然艱難，但民主共和的種子已經播下。");
    RepublicEnding.EndingType = EMingEndingType::Historical;
    RepublicEnding.EndingImage = TEXT("UI/Endings/Republic_Victory");
    RepublicEnding.EndingMusic = TEXT("Audio/Music/Republic_Victory");
    RepublicEnding.EndingDuration = 45.0f;
    RepublicEnding.bIsSecret = false;
    RepublicEnding.bIsUnlocked = true;
    
    FMingEndingRequirement Req1;
    Req1.ConditionType = EMingEndingCondition::PoliticalUnity;
    Req1.ThresholdValue = 0.8f;
    Req1.bIsRequired = true;
    Req1.Weight = 1.0f;
    
    FMingEndingRequirement Req2;
    Req2.ConditionType = EMingEndingCondition::MilitaryVictory;
    Req2.ThresholdValue = 0.7f;
    Req2.bIsRequired = true;
    Req2.Weight = 0.8f;
    
    FMingEndingRequirement Req3;
    Req3.ConditionType = EMingEndingCondition::HistoricalAccuracy;
    Req3.ThresholdValue = 0.6f;
    Req3.bIsRequired = false;
    Req3.Weight = 0.5f;
    
    RepublicEnding.Requirements.Add(Req1);
    RepublicEnding.Requirements.Add(Req2);
    RepublicEnding.Requirements.Add(Req3);
    
    Endings.Add(RepublicEnding.EndingID, RepublicEnding);
    
    // Northern Expedition Victory
    FMingEnding NorthernVictory;
    NorthernVictory.EndingID = TEXT("NORTHERN_EXPEDITION_VICTORY");
    NorthernVictory.EndingTitle = TEXT("北伐成功");
    NorthernVictory.Description = TEXT("成功完成北伐，統一中國大部分地區");
    NorthernVictory.LongDescription = TEXT("北伐戰爭勝利，軍閥割據時代結束。國民政府成功統一中國，為現代化建設奠定了基礎。雖然內部矛盾依然存在，但國家統一的大局已定。");
    NorthernVictory.EndingType = EMingEndingType::Historical;
    NorthernVictory.EndingImage = TEXT("UI/Endings/Northern_Victory");
    NorthernVictory.EndingMusic = TEXT("Audio/Music/Northern_Victory");
    NorthernVictory.EndingDuration = 40.0f;
    NorthernVictory.bIsSecret = false;
    NorthernVictory.bIsUnlocked = true;
    
    FMingEndingRequirement Req4;
    Req4.ConditionType = EMingEndingCondition::TerritoryLoss;
    Req4.ConditionParameter = TEXT("LessThan");
    Req4.ThresholdValue = 0.2f;
    Req4.bIsRequired = true;
    Req4.Weight = 1.0f;
    
    FMingEndingRequirement Req5;
    Req5.ConditionType = EMingEndingCondition::MilitaryVictory;
    Req5.ThresholdValue = 0.8f;
    Req5.bIsRequired = true;
    Req5.Weight = 0.9f;
    
    NorthernVictory.Requirements.Add(Req4);
    NorthernVictory.Requirements.Add(Req5);
    
    Endings.Add(NorthernVictory.EndingID, NorthernVictory);
    
    // Anti-Japanese Victory
    FMingEnding AntiJapaneseVictory;
    AntiJapaneseVictory.EndingID = TEXT("ANTI_JAPANESE_VICTORY");
    AntiJapaneseVictory.EndingTitle = TEXT("抗戰勝利");
    AntiJapaneseVictory.Description = TEXT("成功抵抗日本侵略，取得抗戰勝利");
    AntiJapaneseVictory.LongDescription = TEXT("經過八年艱苦抗戰，中國終於取得了對日戰爭的勝利。雖然付出了巨大犧牲，但國家主權得以維護，民族尊嚴得以恢復。中國在國際上的地位顯著提升。");
    AntiJapaneseVictory.EndingType = EMingEndingType::Historical;
    AntiJapaneseVictory.EndingImage = TEXT("UI/Endings/AntiJapanese_Victory");
    AntiJapaneseVictory.EndingMusic = TEXT("Audio/Music/AntiJapanese_Victory");
    AntiJapaneseVictory.EndingDuration = 50.0f;
    AntiJapaneseVictory.bIsSecret = false;
    AntiJapaneseVictory.bIsUnlocked = true;
    
    FMingEndingRequirement Req6;
    Req6.ConditionType = EMingEndingCondition::TimeLimit;
    Req6.ConditionParameter = TEXT("GreaterThan");
    Req6.ThresholdValue = 8.0f; // Survive 8 years
    Req6.bIsRequired = true;
    Req6.Weight = 1.0f;
    
    AntiJapaneseVictory.Requirements.Add(Req6);
    
    Endings.Add(AntiJapaneseVictory.EndingID, AntiJapaneseVictory);
}

void UMingEndingSystem::SetupAlternateEndings()
{
    // Early Republic - Faster Victory
    FMingEnding EarlyRepublic;
    EarlyRepublic.EndingID = TEXT("EARLY_REPUBLIC");
    EarlyRepublic.EndingTitle = TEXT("早期共和");
    EarlyRepublic.Description = TEXT("比歷史更早建立共和國");
    EarlyRepublic.LongDescription = TEXT("在您的英明領導下，革命比歷史記載更早取得成功。這為中國的現代化爭取了寶貴時間，但也可能帶來意想不到的挑戰。歷史的車輪因您的選擇而改變。");
    EarlyRepublic.EndingType = EMingEndingType::Alternate;
    EarlyRepublic.EndingImage = TEXT("UI/Endings/Early_Republic");
    EarlyRepublic.EndingMusic = TEXT("Audio/Music/Alternate_Victory");
    EarlyRepublic.EndingDuration = 35.0f;
    EarlyRepublic.bIsSecret = false;
    EarlyRepublic.bIsUnlocked = false;
    
    FMingEndingRequirement Req1;
    Req1.ConditionType = EMingEndingCondition::TimeLimit;
    Req1.ConditionParameter = TEXT("LessThan");
    Req1.ThresholdValue = 2.0f; // Republic established in less than 2 years
    Req1.bIsRequired = true;
    Req1.Weight = 1.0f;
    
    EarlyRepublic.Requirements.Add(Req1);
    
    Endings.Add(EarlyRepublic.EndingID, EarlyRepublic);
    
    // Peaceful Unification
    FMingEnding PeacefulUnity;
    PeacefulUnity.EndingID = TEXT("PEACEFUL_UNIFICATION");
    PeacefulUnity.EndingTitle = TEXT("和平統一");
    PeacefulUnity.EndingDescription = TEXT("通過外交手段實現國家統一");
    PeacefulUnity.LongDescription = TEXT("您選擇了和平而非戰爭，通過談判和妥協實現了國家統一。雖然過程艱難，但避免了血腥的內戰，為國家保存了實力。這是一條少有人走的道路，但您成功了。");
    PeacefulUnity.EndingType = EMingEndingType::Alternate;
    PeacefulUnity.EndingImage = TEXT("UI/Endings/Peaceful_Unity");
    PeacefulUnity.EndingMusic = TEXT("Audio/Music/Peaceful_Victory");
    PeacefulUnity.EndingDuration = 40.0f;
    PeacefulUnity.bIsSecret = false;
    PeacefulUnity.bIsUnlocked = false;
    
    FMingEndingRequirement Req2;
    Req2.ConditionType = EMingEndingCondition::MilitaryVictory;
    Req2.ConditionParameter = TEXT("LessThan");
    Req2.ThresholdValue = 0.3f; // Low military victory requirement
    Req2.bIsRequired = true;
    Req2.Weight = 1.0f;
    
    FMingEndingRequirement Req3;
    Req3.ConditionType = EMingEndingCondition::PoliticalUnity;
    Req3.ThresholdValue = 0.9f;
    Req3.bIsRequired = true;
    Req3.Weight = 1.0f;
    
    PeacefulUnity.Requirements.Add(Req2);
    PeacefulUnity.Requirements.Add(Req3);
    
    Endings.Add(PeacefulUnity.EndingID, PeacefulUnity);
    
    // Economic Prosperity
    FMingEnding EconomicProsperity;
    EconomicProsperity.EndingID = TEXT("ECONOMIC_PROSPERITY");
    EconomicProsperity.EndingTitle = TEXT("經濟繁榮");
    EconomicProsperity.Description = TEXT("重視經濟發展，實現國家富強");
    EconomicProsperity.LongDescription = TEXT("您將重心放在經濟建設上，通過發展工業、商業和金融，使中國走上了繁榮之路。雖然軍事上可能有所不足，但經濟的強盛為國家的長遠發展奠定了堅實基礎。");
    EconomicProsperity.EndingType = EMingEndingType::Alternate;
    EconomicProsperity.EndingImage = TEXT("UI/Endings/Economic_Prosperity");
    EconomicProsperity.EndingMusic = TEXT("Audio/Music/Economic_Victory");
    EconomicProsperity.EndingDuration = 38.0f;
    EconomicProsperity.bIsSecret = false;
    EconomicProsperity.bIsUnlocked = false;
    
    FMingEndingRequirement Req4;
    Req4.ConditionType = EMingEndingCondition::EconomicProsperity;
    Req4.ThresholdValue = 0.8f;
    Req4.bIsRequired = true;
    Req4.Weight = 1.0f;
    
    EconomicProsperity.Requirements.Add(Req4);
    
    Endings.Add(EconomicProsperity.EndingID, EconomicProsperity);
}

void UMingEndingSystem::SetupSecretEndings()
{
    // Perfect Historical Accuracy
    FMingEnding PerfectHistorical;
    PerfectHistorical.EndingID = TEXT("PERFECT_HISTORICAL");
    PerfectHistorical.EndingTitle = TEXT("完美史詩");
    PerfectHistorical.Description = TEXT("100%還原歷史，完美重現民國歷程");
    PerfectHistorical.LongDescription = TEXT("您的每一個決策都與歷史高度吻合，完美重現了民國時期的歷史進程。這不僅需要對歷史的深刻理解，更需要精準的判斷和執行。您是真正的歷史見證者。");
    PerfectHistorical.EndingType = EMingEndingType::Secret;
    PerfectHistorical.EndingImage = TEXT("UI/Endings/Perfect_Historical");
    PerfectHistorical.EndingMusic = TEXT("Audio/Music/Secret_Victory");
    PerfectHistorical.EndingDuration = 60.0f;
    PerfectHistorical.bIsSecret = true;
    PerfectHistorical.bIsUnlocked = false;
    
    FMingEndingRequirement Req1;
    Req1.ConditionType = EMingEndingCondition::HistoricalAccuracy;
    Req1.ThresholdValue = 0.95f; // 95% historical accuracy
    Req1.bIsRequired = true;
    Req1.Weight = 1.0f;
    
    PerfectHistorical.Requirements.Add(Req1);
    
    Endings.Add(PerfectHistorical.EndingID, PerfectHistorical);
    
    // Cultural Renaissance
    FMingEnding CulturalRenaissance;
    CulturalRenaissance.EndingID = TEXT("CULTURAL_RENAISSANCE");
    CulturalRenaissance.EndingTitle = TEXT("文化復興");
    CulturalRenaissance.Description = TEXT("推動文化革新，實現思想解放");
    CulturalRenaissance.LongDescription = TEXT("在政治和軍事之外，您特別重視文化建設。通過推動新文化運動，促進思想解放，為中國的現代化注入了強大的精神力量。這是一條很少有人注意的道路，但您證明了它的重要性。");
    CulturalRenaissance.EndingType = EMingEndingType::Secret;
    CulturalRenaissance.EndingImage = TEXT("UI/Endings/Cultural_Renaissance");
    CulturalRenaissance.EndingMusic = TEXT("Audio/Music/Cultural_Victory");
    CulturalRenaissance.EndingDuration = 45.0f;
    CulturalRenaissance.bIsSecret = true;
    CulturalRenaissance.bIsUnlocked = false;
    
    FMingEndingRequirement Req2;
    Req2.ConditionType = EMingEndingCondition::CulturalAchievement;
    Req2.ThresholdValue = 0.9f;
    Req2.bIsRequired = true;
    Req2.Weight = 1.0f;
    
    CulturalRenaissance.Requirements.Add(Req2);
    
    Endings.Add(CulturalRenaissance.EndingID, CulturalRenaissance);
    
    // The Great Unifier
    FMingEnding GreatUnifier;
    GreatUnifier.EndingID = TEXT("GREAT_UNIFIER");
    GreatUnifier.EndingTitle = TEXT("大一統者");
    GreatUnifier.Description = TEXT("實現完全統一，超越歷史成就");
    GreatUnifier.LongDescription = TEXT("您不僅統一了大陸，還成功收復了台灣、香港等地，實現了真正的國家統一。這是歷史上從未有人達成的偉業。您將被後世永遠銘記為中華民族的統一者。");
    GreatUnifier.EndingType = EMingEndingType::Secret;
    GreatUnifier.EndingImage = TEXT("UI/Endings/Great_Unifier");
    GreatUnifier.EndingMusic = TEXT("Audio/Music/Ultimate_Victory");
    GreatUnifier.EndingDuration = 55.0f;
    GreatUnifier.bIsSecret = true;
    GreatUnifier.bIsUnlocked = false;
    
    FMingEndingRequirement Req3;
    Req3.ConditionType = EMingEndingCondition::TerritoryLoss;
    Req3.ConditionParameter = TEXT("Equals");
    Req3.ThresholdValue = 0.0f; // No territory lost
    Req3.bIsRequired = true;
    Req3.Weight = 1.0f;
    
    FMingEndingRequirement Req4;
    Req4.ConditionType = EMingEndingCondition::PoliticalUnity;
    Req4.ThresholdValue = 1.0f; // Complete unity
    Req4.bIsRequired = true;
    Req4.Weight = 1.0f;
    
    GreatUnifier.Requirements.Add(Req3);
    GreatUnifier.Requirements.Add(Req4);
    
    Endings.Add(GreatUnifier.EndingID, GreatUnifier);
}

void UMingEndingSystem::SetupBadEndings()
{
    // Republic Failed
    FMingEnding RepublicFailed;
    RepublicFailed.EndingID = TEXT("REPUBLIC_FAILED");
    RepublicFailed.EndingTitle = TEXT("共和失敗");
    RepublicFailed.Description = TEXT("革命失敗，清朝統治延續");
    RepublicFailed.LongDescription = TEXT("革命最終失敗，清朝得以延續統治。雖然革命的火種沒有完全熄滅，但中國的現代化進程被推遲了數十年。歷史走上了另一條軌道。");
    RepublicFailed.EndingType = EMingEndingType::Bad;
    RepublicFailed.EndingImage = TEXT("UI/Endings/Republic_Failed");
    RepublicFailed.EndingMusic = TEXT("Audio/Music/Defeat_Music");
    RepublicFailed.EndingDuration = 30.0f;
    RepublicFailed.bIsSecret = false;
    RepublicFailed.bIsUnlocked = true;
    
    FMingEndingRequirement Req1;
    Req1.ConditionType = EMingEndingCondition::MilitaryVictory;
    Req1.ThresholdValue = 0.3f;
    Req1.bIsRequired = true;
    Req1.Weight = 1.0f;
    
    RepublicFailed.Requirements.Add(Req1);
    
    Endings.Add(RepublicFailed.EndingID, RepublicFailed);
    
    // Warlord Era
    FMingEnding WarlordEra;
    WarlordEra.EndingID = TEXT("WARLORD_ERA");
    WarlordEra.EndingTitle = TEXT("軍閥混戰");
    WarlordEra.Description = TEXT("國家分裂，陷入軍閥混戰");
    WarlordEra.LongDescription = TEXT("中央政府權威崩潰，各地軍閥割據，國家陷入長期混戰。人民苦不堪言，國家建設停滯不前。中國的現代化進程遭遇重大挫折。");
    WarlordEra.EndingType = EMingEndingType::Bad;
    WarlordEra.EndingImage = TEXT("UI/Endings/Warlord_Era");
    WarlordEra.EndingMusic = TEXT("Audio/Music/Warlord_Music");
    WarlordEra.EndingDuration = 35.0f;
    WarlordEra.bIsSecret = false;
    WarlordEra.bIsUnlocked = true;
    
    FMingEndingRequirement Req2;
    Req2.ConditionType = EMingEndingCondition::PoliticalUnity;
    Req2.ThresholdValue = 0.4f;
    Req2.bIsRequired = true;
    Req2.Weight = 1.0f;
    
    FMingEndingRequirement Req3;
    Req3.ConditionType = EMingEndingCondition::TerritoryLoss;
    Req3.ThresholdValue = 0.5f;
    Req3.bIsRequired = true;
    Req3.Weight = 0.8f;
    
    WarlordEra.Requirements.Add(Req2);
    WarlordEra.Requirements.Add(Req3);
    
    Endings.Add(WarlordEra.EndingID, WarlordEra);
    
    // Resource Depletion
    FMingEnding ResourceDepletion;
    ResourceDepletion.EndingID = TEXT("RESOURCE_DEPLETION");
    ResourceDepletion.EndingTitle = TEXT("資源枯竭");
    ResourceDepletion.Description = TEXT("資源耗盡，國家崩潰");
    ResourceDepletion.LongDescription = TEXT("由於資源管理不當，國家經濟崩潰，社會秩序瓦解。即使是軍事上的勝利也無法挽回經濟的崩塌。中國的現代化努力付諸東流。");
    ResourceDepletion.EndingType = EMingEndingType::Bad;
    ResourceDepletion.EndingImage = TEXT("UI/Endings/Resource_Depletion");
    ResourceDepletion.EndingMusic = TEXT("Audio/Music/Economic_Collapse");
    ResourceDepletion.EndingDuration = 32.0f;
    ResourceDepletion.bIsSecret = false;
    ResourceDepletion.bIsUnlocked = true;
    
    FMingEndingRequirement Req4;
    Req4.ConditionType = EMingEndingCondition::ResourceDepletion;
    Req4.ThresholdValue = 0.8f;
    Req4.bIsRequired = true;
    Req4.Weight = 1.0f;
    
    ResourceDepletion.Requirements.Add(Req4);
    
    Endings.Add(ResourceDepletion.EndingID, ResourceDepletion);
}

void UMingEndingSystem::CheckEndingConditions()
{
    for (auto& Pair : Endings)
    {
        const FString& EndingID = Pair.Key;
        const FMingEnding& Ending = Pair.Value;
        
        if (!Ending.bHasBeenTriggered && AreEndingRequirementsMet(EndingID))
        {
            TriggerEnding(EndingID);
            break; // Only trigger one ending at a time
        }
    }
}

void UMingEndingSystem::TriggerEnding(const FString& EndingID)
{
    FMingEnding* Ending = FindEnding(EndingID);
    if (!Ending)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ending not found: %s"), *EndingID);
        return;
    }
    
    if (Ending->bHasBeenTriggered)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ending already triggered: %s"), *EndingID);
        return;
    }
    
    ProcessEnding(*Ending);
}

void UMingEndingSystem::ForceEnding(const FString& EndingID)
{
    // Force trigger regardless of requirements
    FMingEnding* Ending = FindEnding(EndingID);
    if (Ending)
    {
        ProcessEnding(*Ending);
    }
}

void UMingEndingSystem::ProcessEnding(const FMingEnding& Ending)
{
    CurrentEndingID = Ending.EndingID;
    
    // Mark as triggered
    FMingEnding* MutableEnding = FindEnding(Ending.EndingID);
    if (MutableEnding)
    {
        MutableEnding->bHasBeenTriggered = true;
        MutableEnding->TriggerTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
        MutableEnding->EndingScore = CalculateEndingScore(Ending.EndingID);
    }
    
    // Unlock ending
    UnlockEnding(Ending.EndingID);
    
    // Record statistics
    RecordEndingStats(Ending.EndingID);
    
    // Trigger appropriate events
    OnEndingTriggered.Broadcast(Ending, MutableEnding->EndingScore);
    
    if (Ending.EndingType == EMingEndingType::Victory || 
        Ending.EndingType == EMingEndingType::Historical ||
        Ending.EndingType == EMingEndingType::Alternate)
    {
        OnVictoryAchieved.Broadcast(Ending.EndingID);
    }
    else
    {
        OnDefeatSuffered.Broadcast(Ending.EndingID);
    }
    
    if (Ending.bIsSecret && !UnlockedEndings.Contains(Ending.EndingID))
    {
        OnSecretEndingUnlocked.Broadcast(Ending.EndingID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Ending triggered: %s (%s)"), *Ending.EndingTitle, *GetEndingTypeName(Ending.EndingType));
}

TArray<FMingEnding> UMingEndingSystem::GetAllEndings() const
{
    TArray<FMingEnding> Result;
    for (const auto& Pair : Endings)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FMingEnding> UMingEndingSystem::GetAvailableEndings() const
{
    TArray<FMingEnding> Result;
    for (const auto& Pair : Endings)
    {
        const FMingEnding& Ending = Pair.Value;
        
        if (!Ending.bHasBeenTriggered && AreEndingRequirementsMet(Ending.EndingID))
        {
            Result.Add(Ending);
        }
    }
    return Result;
}

TArray<FMingEnding> UMingEndingSystem::GetUnlockedEndings() const
{
    TArray<FMingEnding> Result;
    for (const auto& Pair : Endings)
    {
        const FMingEnding& Ending = Pair.Value;
        
        if (UnlockedEndings.Contains(Ending.EndingID))
        {
            Result.Add(Ending);
        }
    }
    return Result;
}

FMingEnding UMingEndingSystem::GetEnding(const FString& EndingID) const
{
    const FMingEnding* Ending = Endings.Find(EndingID);
    return Ending ? *Ending : FMingEnding();
}

FMingEnding UMingEndingSystem::GetCurrentEnding() const
{
    return GetEnding(CurrentEndingID);
}

float UMingEndingSystem::CalculateEndingScore(const FString& EndingID) const
{
    const FMingEnding* Ending = Endings.Find(EndingID);
    if (!Ending)
    {
        return 0.0f;
    }
    
    float Score = 0.0f;
    float TotalWeight = 0.0f;
    
    for (const FMingEndingRequirement& Requirement : Ending->Requirements)
    {
        if (EvaluateCondition(Requirement))
        {
            Score += Requirement.Weight;
        }
        TotalWeight += Requirement.Weight;
    }
    
    return TotalWeight > 0.0f ? Score / TotalWeight : 0.0f;
}

bool UMingEndingSystem::AreEndingRequirementsMet(const FString& EndingID) const
{
    const FMingEnding* Ending = Endings.Find(EndingID);
    if (!Ending)
    {
        return false;
    }
    
    for (const FMingEndingRequirement& Requirement : Ending->Requirements)
    {
        if (Requirement.bIsRequired && !EvaluateCondition(Requirement))
        {
            return false;
        }
    }
    
    return true;
}

TArray<FMingEndingRequirement> UMingEndingSystem::GetFailedRequirements(const FString& EndingID) const
{
    TArray<FMingEndingRequirement> Failed;
    
    const FMingEnding* Ending = Endings.Find(EndingID);
    if (Ending)
    {
        for (const FMingEndingRequirement& Requirement : Ending->Requirements)
        {
            if (!EvaluateCondition(Requirement))
            {
                Failed.Add(Requirement);
            }
        }
    }
    
    return Failed;
}

float UMingEndingSystem::GetEndingProgress(const FString& EndingID) const
{
    const FMingEnding* Ending = Endings.Find(EndingID);
    if (!Ending)
    {
        return 0.0f;
    }
    
    int32 MetCount = 0;
    int32 TotalCount = Ending->Requirements.Num();
    
    for (const FMingEndingRequirement& Requirement : Ending->Requirements)
    {
        if (EvaluateCondition(Requirement))
        {
            MetCount++;
        }
    }
    
    return TotalCount > 0 ? (float)MetCount / TotalCount : 0.0f;
}

void UMingEndingSystem::SetVictoryCondition(EMingEndingCondition Condition, float Value)
{
    VictoryConditions.Add(Condition, Value);
}

void UMingEndingSystem::SetDefeatCondition(EMingEndingCondition Condition, float Value)
{
    DefeatConditions.Add(Condition, Value);
}

bool UMingEndingSystem::IsVictoryConditionMet(EMingEndingCondition Condition) const
{
    const float* Value = VictoryConditions.Find(Condition);
    if (!Value)
    {
        return false;
    }
    
    float CurrentValue = GetConditionValue(Condition);
    return CurrentValue >= *Value;
}

bool UMingEndingSystem::IsDefeatConditionMet(EMingEndingCondition Condition) const
{
    const float* Value = DefeatConditions.Find(Condition);
    if (!Value)
    {
        return false;
    }
    
    float CurrentValue = GetConditionValue(Condition);
    return CurrentValue <= *Value;
}

TMap<FString, float> UMingEndingSystem::GetAllEndingScores() const
{
    return EndingScores;
}

int32 UMingEndingSystem::GetUnlockedEndingCount() const
{
    return UnlockedEndings.Num();
}

int32 UMingEndingSystem::GetTotalEndingCount() const
{
    return Endings.Num();
}

float UMingEndingSystem::GetCompletionPercentage() const
{
    int32 Total = GetTotalEndingCount();
    return Total > 0 ? (float)GetUnlockedEndingCount() / Total : 0.0f;
}

FString UMingEndingSystem::GetBestEnding() const
{
    FString BestEndingID;
    float BestScore = -1.0f;
    
    for (const auto& Pair : EndingScores)
    {
        if (Pair.Value > BestScore)
        {
            BestScore = Pair.Value;
            BestEndingID = Pair.Key;
        }
    }
    
    return BestEndingID;
}

void UMingEndingSystem::UnlockSecretEnding(const FString& EndingID)
{
    FMingEnding* Ending = FindEnding(EndingID);
    if (Ending && Ending->bIsSecret)
    {
        Ending->bIsUnlocked = true;
        OnSecretEndingUnlocked.Broadcast(EndingID);
        UE_LOG(LogTemp, Log, TEXT("Secret ending unlocked: %s"), *Ending->EndingTitle);
    }
}

TArray<FMingEnding> UMingEndingSystem::GetSecretEndings() const
{
    TArray<FMingEnding> Result;
    for (const auto& Pair : Endings)
    {
        if (Pair.Value.bIsSecret)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

bool UMingEndingSystem::IsEndingSecret(const FString& EndingID) const
{
    const FMingEnding* Ending = Endings.Find(EndingID);
    return Ending ? Ending->bIsSecret : false;
}

FString UMingEndingSystem::GetEndingTypeName(EMingEndingType Type)
{
    switch (Type)
    {
    case EMingEndingType::Victory: return TEXT("勝利");
    case EMingEndingType::Defeat: return TEXT("失敗");
    case EMingEndingType::Historical: return TEXT("歷史");
    case EMingEndingType::Alternate: return TEXT("分支");
    case EMingEndingType::Secret: return TEXT("秘密");
    case EMingEndingType::Bad: return TEXT("壞結局");
    default: return TEXT("未知");
    }
}

FString UMingEndingSystem::GetConditionTypeName(EMingEndingCondition Condition)
{
    switch (Condition)
    {
    case EMingEndingCondition::MilitaryVictory: return TEXT("軍事勝利");
    case EMingEndingCondition::PoliticalUnity: return TEXT("政治統一");
    case EMingEndingCondition::EconomicProsperity: return TEXT("經濟繁榮");
    case EMingEndingCondition::CulturalAchievement: return TEXT("文化成就");
    case EMingEndingCondition::HistoricalAccuracy: return TEXT("歷史準確");
    case EMingEndingCondition::TimeLimit: return TEXT("時間限制");
    case EMingEndingCondition::PlayerDeath: return TEXT("玩家死亡");
    case EMingEndingCondition::ResourceDepletion: return TEXT("資源枯竭");
    case EMingEndingCondition::TerritoryLoss: return TEXT("領土損失");
    case EMingEndingCondition::AllianceBetrayal: return TEXT("聯盟背叛");
    default: return TEXT("未知");
    }
}

FString UMingEndingSystem::SaveEndingData() const
{
    FString Result = TEXT("{\n");
    Result += TEXT("  \"unlocked_endings\": [");
    
    bool First = true;
    for (const FString& EndingID : UnlockedEndings)
    {
        if (!First) Result += TEXT(",");
        Result += FString::Printf(TEXT("\"%s\""), *EndingID);
        First = false;
    }
    
    Result += TEXT("],\n");
    Result += TEXT("  \"ending_scores\": {\n");
    
    First = true;
    for (const auto& Pair : EndingScores)
    {
        if (!First) Result += TEXT(",\n");
        Result += FString::Printf(TEXT("    \"%s\": %.2f"), *Pair.Key, Pair.Value);
        First = false;
    }
    
    Result += TEXT("\n  },\n");
    Result += FString::Printf(TEXT("  \"completion_percentage\": %.2f\n"), GetCompletionPercentage());
    Result += TEXT("}\n");
    
    return Result;
}

void UMingEndingSystem::LoadEndingData(const FString& JsonString)
{
    // Parse JSON and restore ending data
    UE_LOG(LogTemp, Log, TEXT("Loading ending data"));
}

FMingEnding* UMingEndingSystem::FindEnding(const FString& EndingID)
{
    return Endings.Find(EndingID);
}

bool UMingEndingSystem::EvaluateCondition(const FMingEndingRequirement& Requirement) const
{
    float CurrentValue = GetConditionValue(Requirement.ConditionType);
    
    // Simple comparison logic
    if (Requirement.ConditionParameter.IsEmpty() || Requirement.ConditionParameter == TEXT("GreaterThan"))
    {
        return CurrentValue >= Requirement.ThresholdValue;
    }
    else if (Requirement.ConditionParameter == TEXT("LessThan"))
    {
        return CurrentValue <= Requirement.ThresholdValue;
    }
    else if (Requirement.ConditionParameter == TEXT("Equals"))
    {
        return FMath::IsNearlyEqual(CurrentValue, Requirement.ThresholdValue, 0.01f);
    }
    
    return CurrentValue >= Requirement.ThresholdValue;
}

float UMingEndingSystem::GetConditionValue(EMingEndingCondition Condition) const
{
    // This would get actual game state values
    // For now, return placeholder values
    switch (Condition)
    {
    case EMingEndingCondition::MilitaryVictory: return 0.7f;
    case EMingEndingCondition::PoliticalUnity: return 0.6f;
    case EMingEndingCondition::EconomicProsperity: return 0.5f;
    case EMingEndingCondition::CulturalAchievement: return 0.4f;
    case EMingEndingCondition::HistoricalAccuracy: return 0.8f;
    case EMingEndingCondition::TimeLimit: return 5.0f;
    case EMingEndingCondition::PlayerDeath: return 0.0f;
    case EMingEndingCondition::ResourceDepletion: return 0.2f;
    case EMingEndingCondition::TerritoryLoss: return 0.3f;
    case EMingEndingCondition::AllianceBetrayal: return 0.1f;
    default: return 0.0f;
    }
}

void UMingEndingSystem::UpdateEndingScores()
{
    CalculateAllEndingScores();
}

void UMingEndingSystem::UnlockEnding(const FString& EndingID)
{
    if (!UnlockedEndings.Contains(EndingID))
    {
        UnlockedEndings.Add(EndingID);
        
        // Calculate and store score
        float Score = CalculateEndingScore(EndingID);
        EndingScores.Add(EndingID, Score);
    }
}

void UMingEndingSystem::RecordEndingStats(const FString& EndingID)
{
    // This would record ending statistics for achievements etc.
    UE_LOG(LogTemp, Log, TEXT("Recorded ending statistics: %s"), *EndingID);
}

void UMingEndingSystem::CalculateAllEndingScores()
{
    for (const auto& Pair : Endings)
    {
        const FString& EndingID = Pair.Key;
        float Score = CalculateEndingScore(EndingID);
        EndingScores.Add(EndingID, Score);
    }
}
