#include "MingGoRTSHistoricalRoleplay.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSHistoricalRoleplay::UMingGoRTSHistoricalRoleplay()
{
    bIsInitialized = false;
}

void UMingGoRTSHistoricalRoleplay::InitializeRoleplaySystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("角色扮演系統已經初始化"));
        return;
    }

    // 初始化情境庫
    InitializeSituationLibrary();

    // 構建情境映射
    SituationMap.Empty();
    for (const FHistoricalSituation& Situation : AllSituations)
    {
        SituationMap.Add(Situation.SituationID, Situation);
        
        // 構建角色情境映射
        for (const FString& KeyFigure : Situation.KeyFigures)
        {
            if (!CharacterSituationMap.Contains(KeyFigure))
            {
                CharacterSituationMap.Add(KeyFigure, TArray<FString>());
            }
            CharacterSituationMap[KeyFigure].Add(Situation.SituationID);
        }
    }

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("角色扮演系統初始化完成，共載入 %d 個情境"), AllSituations.Num());
}

FString UMingGoRTSHistoricalRoleplay::StartRoleplaySession(const FString& CharacterID, const FMingCharacterData& PlayerCharacter)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("角色扮演系統未初始化"));
        return TEXT("");
    }

    // 生成會話ID
    FString SessionID = GenerateSessionID(CharacterID);

    // 創建新會話
    FRoleplaySession NewSession;
    NewSession.SessionID = SessionID;
    NewSession.CharacterID = CharacterID;
    NewSession.CurrentSituationID = TEXT("");
    NewSession.HistoricalAccuracyScore = 0.0f;
    NewSession.DecisionQualityScore = 0.0f;
    NewSession.ImmersionScore = 0.0f;
    NewSession.StartTime = FDateTime::Now();
    NewSession.bIsActive = true;

    // 設置當前玩家角色
    CurrentPlayerCharacter = PlayerCharacter;

    // 添加到活動會話
    ActiveSessions.Add(SessionID, NewSession);

    // 廣播會話開始事件
    OnRoleplaySessionStarted.Broadcast(SessionID, CharacterID);

    UE_LOG(LogTemp, Log, TEXT("開始角色扮演會話：%s，角色：%s"), *SessionID, *CharacterID);
    return SessionID;
}

bool UMingGoRTSHistoricalRoleplay::EndRoleplaySession(const FString& SessionID)
{
    if (!ActiveSessions.Contains(SessionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("會話不存在：%s"), *SessionID);
        return false;
    }

    FRoleplaySession& Session = ActiveSessions[SessionID];
    Session.EndTime = FDateTime::Now();
    Session.bIsActive = false;

    // 計算最終沉浸感評分
    Session.ImmersionScore = CalculateImmersionScore(SessionID);

    // 廣播會話結束事件
    OnRoleplaySessionEnded.Broadcast(SessionID, Session);

    UE_LOG(LogTemp, Log, TEXT("結束角色扮演會話：%s"), *SessionID);
    return true;
}

FRoleplaySession UMingGoRTSHistoricalRoleplay::GetCurrentSession() const
{
    // 返回第一個活動會話
    for (const auto& SessionPair : ActiveSessions)
    {
        if (SessionPair.Value.bIsActive)
        {
            return SessionPair.Value;
        }
    }
    return FRoleplaySession();
}

TArray<FHistoricalSituation> UMingGoRTSHistoricalRoleplay::GetAvailableSituations(const FString& CharacterID) const
{
    TArray<FHistoricalSituation> AvailableSituations;
    
    if (CharacterSituationMap.Contains(CharacterID))
    {
        const TArray<FString>& SituationIDs = CharacterSituationMap[CharacterID];
        for (const FString& SituationID : SituationIDs)
        {
            if (IsSituationAvailable(SituationID, CharacterID))
            {
                if (SituationMap.Contains(SituationID))
                {
                    AvailableSituations.Add(SituationMap[SituationID]);
                }
            }
        }
    }
    
    return AvailableSituations;
}

bool UMingGoRTSHistoricalRoleplay::StartSituation(const FString& SessionID, const FString& SituationID)
{
    if (!ActiveSessions.Contains(SessionID))
    {
        UE_LOG(LogTemp, Error, TEXT("會話不存在：%s"), *SessionID);
        return false;
    }

    if (!SituationMap.Contains(SituationID))
    {
        UE_LOG(LogTemp, Error, TEXT("情境不存在：%s"), *SituationID);
        return false;
    }

    FRoleplaySession& Session = ActiveSessions[SessionID];
    const FString& CharacterID = Session.CharacterID;

    // 檢查情境是否可用
    if (!IsSituationAvailable(SituationID, CharacterID))
    {
        UE_LOG(LogTemp, Warning, TEXT("情境不可用：%s"), *SituationID);
        return false;
    }

    // 設置當前情境
    Session.CurrentSituationID = SituationID;

    UE_LOG(LogTemp, Log, TEXT("開始情境：%s，會話：%s"), *SituationID, *SessionID);
    return true;
}

FHistoricalSituation UMingGoRTSHistoricalRoleplay::GetCurrentSituation(const FString& SessionID) const
{
    if (ActiveSessions.Contains(SessionID))
    {
        const FRoleplaySession& Session = ActiveSessions[SessionID];
        if (SituationMap.Contains(Session.CurrentSituationID))
        {
            return SituationMap[Session.CurrentSituationID];
        }
    }
    
    return FHistoricalSituation();
}

bool UMingGoRTSHistoricalRoleplay::MakeDecision(const FString& SessionID, int32 DecisionIndex)
{
    if (!ActiveSessions.Contains(SessionID))
    {
        UE_LOG(LogTemp, Error, TEXT("會話不存在：%s"), *SessionID);
        return false;
    }

    FRoleplaySession& Session = ActiveSessions[SessionID];
    const FHistoricalSituation& CurrentSituation = GetCurrentSituation(SessionID);

    if (DecisionIndex < 0 || DecisionIndex >= CurrentSituation.DecisionOptions.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("無效的決策索引：%d"), DecisionIndex);
        return false;
    }

    const FDecisionOption& SelectedOption = CurrentSituation.DecisionOptions[DecisionIndex];

    // 檢查決策是否可用
    if (!ValidateDecisionOption(SelectedOption, Session.CharacterID))
    {
        UE_LOG(LogTemp, Warning, TEXT("決策選項不可用：%d"), DecisionIndex);
        return false;
    }

    // 記錄決策
    RecordDecisionHistory(SessionID, SelectedOption.OptionText);

    // 計算決策結果
    TArray<FDecisionOutcome> Outcomes = CalculateDecisionOutcomes(SessionID, DecisionIndex);

    // 應用決策結果
    ApplyDecisionOutcomes(SessionID, Outcomes);

    // 更新會話評分
    UpdateSessionScores(SessionID, DecisionIndex);

    // 添加到已完成情境
    Session.CompletedSituations.Add(Session.CurrentSituationID);

    // 廣播決策事件
    OnDecisionMade.Broadcast(SessionID, Session.CurrentSituationID, SelectedOption.OptionText);

    UE_LOG(LogTemp, Log, TEXT("做出決策：%s，會話：%s"), *SelectedOption.OptionText, *SessionID);
    return true;
}

TArray<FDecisionOption> UMingGoRTSHistoricalRoleplay::GetAvailableDecisionOptions(const FString& SessionID) const
{
    TArray<FDecisionOption> AvailableOptions;
    
    if (ActiveSessions.Contains(SessionID))
    {
        const FRoleplaySession& Session = ActiveSessions[SessionID];
        const FHistoricalSituation& CurrentSituation = GetCurrentSituation(SessionID);
        
        for (const FDecisionOption& Option : CurrentSituation.DecisionOptions)
        {
            if (ValidateDecisionOption(Option, Session.CharacterID))
            {
                AvailableOptions.Add(Option);
            }
        }
    }
    
    return AvailableOptions;
}

TArray<FDecisionOutcome> UMingGoRTSHistoricalRoleplay::CalculateDecisionOutcomes(const FString& SessionID, int32 DecisionIndex) const
{
    TArray<FDecisionOutcome> Outcomes;
    
    if (ActiveSessions.Contains(SessionID))
    {
        const FRoleplaySession& Session = ActiveSessions[SessionID];
        const FHistoricalSituation& CurrentSituation = GetCurrentSituation(SessionID);
        
        if (DecisionIndex >= 0 && DecisionIndex < CurrentSituation.DecisionOptions.Num())
        {
            const FDecisionOption& SelectedOption = CurrentSituation.DecisionOptions[DecisionIndex];
            
            // 基於決策類型和影響生成結果
            FDecisionOutcome Outcome;
            Outcome.OutcomeType = TEXT("DecisionImpact");
            Outcome.Description = FString::Printf(TEXT("決策：%s 的影響"), *SelectedOption.OptionText);
            Outcome.ImpactValue = static_cast<float>(SelectedOption.Impact) * 10.0f;
            Outcome.AffectedArea = CurrentSituation.Location;
            Outcome.TimeDelay = TEXT("Immediate");
            
            Outcomes.Add(Outcome);
        }
    }
    
    return Outcomes;
}

float UMingGoRTSHistoricalRoleplay::EvaluateDecision(const FString& SessionID, int32 DecisionIndex) const
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return 0.0f;
    }

    const FRoleplaySession& Session = ActiveSessions[SessionID];
    const FHistoricalSituation& CurrentSituation = GetCurrentSituation(SessionID);

    if (DecisionIndex < 0 || DecisionIndex >= CurrentSituation.DecisionOptions.Num())
    {
        return 0.0f;
    }

    const FDecisionOption& SelectedOption = CurrentSituation.DecisionOptions[DecisionIndex];

    // 綜合評分：歷史準確性 + 決策質量
    float HistoricalScore = CalculateHistoricalAccuracy(SelectedOption);
    float QualityScore = CalculateDecisionQuality(SelectedOption, Session.CharacterID);

    return (HistoricalScore + QualityScore) / 2.0f;
}

float UMingGoRTSHistoricalRoleplay::GetHistoricalAccuracyScore(const FString& SessionID) const
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return 0.0f;
    }

    return ActiveSessions[SessionID].HistoricalAccuracyScore;
}

float UMingGoRTSHistoricalRoleplay::GetDecisionQualityScore(const FString& SessionID) const
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return 0.0f;
    }

    return ActiveSessions[SessionID].DecisionQualityScore;
}

float UMingGoRTSHistoricalRoleplay::GetImmersionScore(const FString& SessionID) const
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return 0.0f;
    }

    return ActiveSessions[SessionID].ImmersionScore;
}

TArray<FString> UMingGoRTSHistoricalRoleplay::GetSessionHistory(const FString& SessionID) const
{
    if (ActiveSessions.Contains(SessionID))
    {
        return ActiveSessions[SessionID].DecisionHistory;
    }
    
    return TArray<FString>();
}

TMap<FString, float> UMingGoRTSHistoricalRoleplay::GetRoleplayStatistics(const FString& SessionID) const
{
    TMap<FString, float> Statistics;
    
    if (ActiveSessions.Contains(SessionID))
    {
        const FRoleplaySession& Session = ActiveSessions[SessionID];
        
        Statistics.Add(TEXT("HistoricalAccuracy"), Session.HistoricalAccuracyScore);
        Statistics.Add(TEXT("DecisionQuality"), Session.DecisionQualityScore);
        Statistics.Add(TEXT("Immersion"), Session.ImmersionScore);
        Statistics.Add(TEXT("SituationsCompleted"), static_cast<float>(Session.CompletedSituations.Num()));
        Statistics.Add(TEXT("DecisionsMade"), static_cast<float>(Session.DecisionHistory.Num()));
    }
    
    return Statistics;
}

bool UMingGoRTSHistoricalRoleplay::SaveRoleplayData(const FString& SaveSlotName)
{
    // TODO: 實現角色扮演數據保存
    UE_LOG(LogTemp, Log, TEXT("保存角色扮演數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSHistoricalRoleplay::LoadRoleplayData(const FString& SaveSlotName)
{
    // TODO: 實現角色扮演數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入角色扮演數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSHistoricalRoleplay::InitializeSituationLibrary()
{
    CreatePoliticalDecisionSituations();
    CreateMilitaryStrategySituations();
    CreateDiplomaticNegotiationSituations();
    CreateEconomicPolicySituations();
    CreateSocialReformSituations();
    CreatePersonalChoiceSituations();
}

void UMingGoRTSHistoricalRoleplay::CreatePoliticalDecisionSituations()
{
    // 北伐統一決策
    FHistoricalSituation NorthernExpedition;
    NorthernExpedition.SituationID = GenerateSituationID(TEXT("NorthernExpedition"), ERoleplayType::PoliticalDecision);
    NorthernExpedition.Title = TEXT("北伐統一決策");
    NorthernExpedition.Description = TEXT("1926年，國民革命軍準備北伐，作為蔣介石，你需要決定北伐的策略和路線。這個決策將影響中國的統一進程和未來的政治格局。");
    NorthernExpedition.HistoricalContext = TEXT("國民革命軍北伐是統一中國的重要軍事行動");
    NorthernExpedition.TimePeriod = TEXT("1926-1928");
    NorthernExpedition.Location = TEXT("廣州/南京");
    NorthernExpedition.RoleplayType = ERoleplayType::PoliticalDecision;
    NorthernExpedition.CharacterRole = TEXT("國民革命軍總司令");
    NorthernExpedition.KeyFigures.Add(TEXT("ChiangKaiShek"));
    NorthernExpedition.KeyFigures.Add(TEXT("ZhouEnlai"));
    NorthernExpedition.KeyFigures.Add(TEXT("ZhangZuolin"));
    NorthernExpedition.AvailableResources.Add(TEXT("國民革命軍"));
    NorthernExpedition.AvailableResources.Add(TEXT("蘇聯援助"));
    NorthernExpedition.AvailableResources.Add(TEXT("民眾支持"));
    NorthernExpedition.Constraints.Add(TEXT("軍閥割據"));
    NorthernExpedition.Constraints.Add(TEXT("資源有限"));
    NorthernExpedition.Constraints.Add(TEXT("政治分歧"));
    NorthernExpedition.bIsCriticalMoment = true;
    NorthernExpedition.bCanRepeat = false;

    // 決策選項
    FDecisionOption Option1;
    Option1.OptionText = TEXT("採取激進策略，快速進攻");
    Option1.OptionDescription = TEXT("集中兵力快速進攻，爭取在短期內統一中國");
    Option1.Alignment = EDecisionAlignment::Historical;
    Option1.Impact = EDecisionImpact::National;
    Option1.HistoricalAccuracy = 0.9f;
    Option1.DifficultyRating = 0.7f;
    Option1.RequiredSkills.Add(TEXT("Leadership"));
    Option1.RequiredSkills.Add(TEXT("TacticalCommand"));
    Option1.Consequences.Add(TEXT("快速統一"));
    Option1.Consequences.Add(TEXT("高傷亡"));
    Option1.RiskFactors.Add(TEXT("過度擴張"));
    
    NorthernExpedition.DecisionOptions.Add(Option1);

    FDecisionOption Option2;
    Option2.OptionText = TEXT("採取穩妥策略，逐步推進");
    Option2.OptionDescription = TEXT("分階段推進，先穩固後方再向北進攻");
    Option2.Alignment = EDecisionAlignment::Alternative;
    Option2.Impact = EDecisionImpact::National;
    Option2.HistoricalAccuracy = 0.6f;
    Option2.DifficultyRating = 0.5f;
    Option2.RequiredSkills.Add(TEXT("Leadership"));
    Option2.RequiredSkills.Add(TEXT("StrategicPlanning"));
    Option2.Consequences.Add(TEXT("穩定發展"));
    Option2.Consequences.Add(TEXT("時間延長"));
    Option2.RiskFactors.Add(TEXT("機會錯失"));
    
    NorthernExpedition.DecisionOptions.Add(Option2);

    AllSituations.Add(NorthernExpedition);
}

void UMingGoRTSHistoricalRoleplay::CreateMilitaryStrategySituations()
{
    // 西安事變決策
    FHistoricalSituation XianIncident;
    XianIncident.SituationID = GenerateSituationID(TEXT("XianIncident"), ERoleplayType::MilitaryStrategy);
    XianIncident.Title = TEXT("西安事變決策");
    XianIncident.Description = TEXT("1936年12月，張學良和楊虎城在西安扣留蔣介石，要求停止內戰一致抗日。作為蔣介石，你需要在這個危急時刻做出決策。");
    XianIncident.HistoricalContext = TEXT("西安事變是中國現代史的重要轉折點");
    XianIncident.TimePeriod = TEXT("1936-12");
    XianIncident.Location = TEXT("西安");
    XianIncident.RoleplayType = ERoleplayType::MilitaryStrategy;
    XianIncident.CharacterRole = TEXT("國民政府主席");
    XianIncident.KeyFigures.Add(TEXT("ChiangKaiShek"));
    XianIncident.KeyFigures.Add(TEXT("ZhangXueliang"));
    XianIncident.KeyFigures.Add(TEXT("YangHucheng"));
    XianIncident.KeyFigures.Add(TEXT("ZhouEnlai"));
    XianIncident.bIsCriticalMoment = true;
    XianIncident.bCanRepeat = false;

    FDecisionOption XiOption1;
    XiOption1.OptionText = TEXT("接受停止內戰，一致抗日");
    XiOption1.OptionDescription = TEXT("同意張學良的要求，建立抗日民族統一戰線");
    XiOption1.Alignment = EDecisionAlignment::Historical;
    XiOption1.Impact = EDecisionImpact::Historical;
    XiOption1.HistoricalAccuracy = 0.95f;
    XiOption1.DifficultyRating = 0.8f;
    XiOption1.RequiredSkills.Add(TEXT("Diplomacy"));
    XiOption1.RequiredSkills.Add(TEXT("Leadership"));
    XiOption1.Consequences.Add(TEXT("抗日統一"));
    XiOption1.Consequences.Add(TEXT("國共合作"));
    XiOption1.RiskFactors.Add(TEXT("權力削弱"));
    
    XianIncident.DecisionOptions.Add(XiOption1);

    AllSituations.Add(XianIncident);
}

void UMingGoRTSHistoricalRoleplay::CreateDiplomaticNegotiationSituations()
{
    // 外交談判情境
    FHistoricalSituation DiplomaticNegotiation;
    DiplomaticNegotiation.SituationID = GenerateSituationID(TEXT("DiplomaticNegotiation"), ERoleplayType::DiplomaticNegotiation);
    DiplomaticNegotiation.Title = TEXT("外交談判");
    DiplomaticNegotiation.Description = TEXT("作為周恩來，你需要在國際會議上代表中國進行外交談判，爭取國際支持。");
    DiplomaticNegotiation.HistoricalContext = TEXT("抗戰期間的外交努力");
    DiplomaticNegotiation.TimePeriod = TEXT("1937-1945");
    DiplomaticNegotiation.Location = TEXT("日內瓦/紐約");
    DiplomaticNegotiation.RoleplayType = ERoleplayType::DiplomaticNegotiation;
    DiplomaticNegotiation.CharacterRole = TEXT("外交代表");
    DiplomaticNegotiation.KeyFigures.Add(TEXT("ZhouEnlai"));
    DiplomaticNegotiation.KeyFigures.Add(TEXT("SoongMeiLing"));
    DiplomaticNegotiation.bIsCriticalMoment = false;
    DiplomaticNegotiation.bCanRepeat = true;

    FDecisionOption DiplOption1;
    DiplOption1.OptionText = TEXT("強調正義立場");
    DiplOption1.OptionDescription = TEXT("堅持中國的正義立場，爭取道德支持");
    DiplOption1.Alignment = EDecisionAlignment::Historical;
    DiplOption1.Impact = EDecisionImpact::International;
    DiplOption1.HistoricalAccuracy = 0.8f;
    DiplOption1.DifficultyRating = 0.6f;
    DiplOption1.RequiredSkills.Add(TEXT("Diplomacy"));
    DiplOption1.RequiredSkills.Add(TEXT("Charisma"));
    DiplOption1.Consequences.Add(TEXT("道義支持"));
    DiplOption1.Consequences.Add(TEXT("國際同情"));
    
    DiplomaticNegotiation.DecisionOptions.Add(DiplOption1);

    AllSituations.Add(DiplomaticNegotiation);
}

void UMingGoRTSHistoricalRoleplay::CreateEconomicPolicySituations()
{
    // 經濟政策情境
    FHistoricalSituation EconomicPolicy;
    EconomicPolicy.SituationID = GenerateSituationID(TEXT("EconomicPolicy"), ERoleplayType::EconomicPolicy);
    EconomicPolicy.Title = TEXT("經濟政策制定");
    EconomicPolicy.Description = TEXT("作為經濟決策者，你需要制定抗戰時期的經濟政策，平衡軍需和民生。");
    EconomicPolicy.HistoricalContext = TEXT("抗戰時期的經濟困難");
    EconomicPolicy.TimePeriod = TEXT("1937-1945");
    EconomicPolicy.Location = TEXT("重慶");
    EconomicPolicy.RoleplayType = ERoleplayType::EconomicPolicy;
    EconomicPolicy.CharacterRole = TEXT("經濟部長");
    EconomicPolicy.KeyFigures.Add(TEXT("EconomicMinister"));
    EconomicPolicy.bIsCriticalMoment = false;
    EconomicPolicy.bCanRepeat = true;

    FDecisionOption EconOption1;
    EconOption1.OptionText = TEXT("優先軍事生產");
    EconOption1.OptionDescription = TEXT("集中資源發展軍事工業，保障戰爭需求");
    EconOption1.Alignment = EDecisionAlignment::Historical;
    EconOption1.Impact = EDecisionImpact::National;
    EconOption1.HistoricalAccuracy = 0.7f;
    EconOption1.DifficultyRating = 0.5f;
    EconOption1.RequiredSkills.Add(TEXT("EconomicManagement"));
    EconOption1.Consequences.Add(TEXT("軍事增強"));
    EconOption1.Consequences.Add(TEXT("民生困難"));
    
    EconomicPolicy.DecisionOptions.Add(EconOption1);

    AllSituations.Add(EconomicPolicy);
}

void UMingGoRTSHistoricalRoleplay::CreateSocialReformSituations()
{
    // 社會改革情境
    FHistoricalSituation SocialReform;
    SocialReform.SituationID = GenerateSituationID(TEXT("SocialReform"), ERoleplayType::SocialReform);
    SocialReform.Title = TEXT("社會改革");
    SocialReform.Description = TEXT("作為社會改革者，你需要推動教育改革和社會進步。");
    SocialReform.HistoricalContext = TEXT("新文化運動的延續");
    SocialReform.TimePeriod = TEXT("1920-1930");
    SocialReform.Location = TEXT("北京/上海");
    SocialReform.RoleplayType = ERoleplayType::SocialReform;
    SocialReform.CharacterRole = TEXT("教育家");
    SocialReform.KeyFigures.Add(TEXT("Educator"));
    SocialReform.KeyFigures.Add(TEXT("Intellectual"));
    SocialReform.bIsCriticalMoment = false;
    SocialReform.bCanRepeat = true;

    FDecisionOption SocialOption1;
    SocialOption1.OptionText = TEXT("推動新式教育");
    SocialOption1.OptionDescription = TEXT("引進西方教育理念，改革傳統教育制度");
    SocialOption1.Alignment = EDecisionAlignment::Historical;
    SocialOption1.Impact = EDecisionImpact::National;
    SocialOption1.HistoricalAccuracy = 0.8f;
    SocialOption1.DifficultyRating = 0.6f;
    SocialOption1.RequiredSkills.Add(TEXT("Intelligence"));
    SocialOption1.Consequences.Add(TEXT("教育現代化"));
    SocialOption1.Consequences.Add(TEXT("文化衝突"));
    
    SocialReform.DecisionOptions.Add(SocialOption1);

    AllSituations.Add(SocialReform);
}

void UMingGoRTSHistoricalRoleplay::CreatePersonalChoiceSituations()
{
    // 個人選擇情境
    FHistoricalSituation PersonalChoice;
    PersonalChoice.SituationID = GenerateSituationID(TEXT("PersonalChoice"), ERoleplayType::PersonalChoice);
    PersonalChoice.Title = TEXT("個人抉擇");
    PersonalChoice.Description = TEXT("作為年輕軍官，你需要在家國之間做出選擇。");
    PersonalChoice.HistoricalContext = TEXT("個人命運與國家興衰");
    PersonalChoice.TimePeriod = TEXT("1920-1940");
    PersonalChoice.Location = TEXT("家鄉/戰場");
    PersonalChoice.RoleplayType = ERoleplayType::PersonalChoice;
    PersonalChoice.CharacterRole = TEXT("軍官");
    PersonalChoice.KeyFigures.Add(TEXT("YoungOfficer"));
    PersonalChoice.KeyFigures.Add(TEXT("Family"));
    PersonalChoice.bIsCriticalMoment = false;
    PersonalChoice.bCanRepeat = true;

    FDecisionOption PersonalOption1;
    PersonalOption1.OptionText = TEXT("投身革命");
    PersonalOption1.OptionDescription = TEXT("選擇革命道路，為國家奉獻");
    PersonalOption1.Alignment = EDecisionAlignment::Historical;
    PersonalOption1.Impact = EDecisionImpact::Personal;
    PersonalOption1.HistoricalAccuracy = 0.7f;
    PersonalOption1.DifficultyRating = 0.4f;
    PersonalOption1.RequiredSkills.Add(TEXT("Courage"));
    PersonalOption1.RequiredSkills.Add(TEXT("Patriotism"));
    PersonalOption1.Consequences.Add(TEXT("理想實現"));
    PersonalOption1.Consequences.Add(TEXT("家庭犧牲"));
    
    PersonalChoice.DecisionOptions.Add(PersonalOption1);

    AllSituations.Add(PersonalChoice);
}

bool UMingGoRTSHistoricalRoleplay::ValidateDecisionOption(const FDecisionOption& Option, const FString& CharacterID) const
{
    if (!Option.bIsAvailable)
    {
        return false;
    }

    // 檢查技能需求
    TArray<FString> CharacterSkills = GetCharacterSkills(CharacterID);
    for (const FString& RequiredSkill : Option.RequiredSkills)
    {
        if (!CharacterSkills.Contains(RequiredSkill))
        {
            return false;
        }
    }

    // 檢查前置條件
    return CheckDecisionPrerequisites(Option, CharacterID);
}

void UMingGoRTSHistoricalRoleplay::ApplyDecisionOutcomes(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes)
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return;
    }

    // 更新角色關係
    UpdateCharacterRelationships(SessionID, Outcomes);

    // 觸發後續事件
    TriggerFollowUpEvents(SessionID, Outcomes);

    UE_LOG(LogTemp, Log, TEXT("應用決策結果，會話：%s"), *SessionID);
}

void UMingGoRTSHistoricalRoleplay::UpdateSessionScores(const FString& SessionID, int32 DecisionIndex)
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return;
    }

    FRoleplaySession& Session = ActiveSessions[SessionID];
    const FDecisionOption& SelectedOption = GetCurrentSituation(SessionID).DecisionOptions[DecisionIndex];

    // 更新歷史準確性評分
    float HistoricalScore = CalculateHistoricalAccuracy(SelectedOption);
    Session.HistoricalAccuracyScore = (Session.HistoricalAccuracyScore + HistoricalScore) / 2.0f;

    // 更新決策質量評分
    float QualityScore = CalculateDecisionQuality(SelectedOption, Session.CharacterID);
    Session.DecisionQualityScore = (Session.DecisionQualityScore + QualityScore) / 2.0f;
}

float UMingGoRTSHistoricalRoleplay::CalculateHistoricalAccuracy(const FDecisionOption& Decision) const
{
    return Decision.HistoricalAccuracy * 100.0f;
}

float UMingGoRTSHistoricalRoleplay::CalculateDecisionQuality(const FDecisionOption& Decision, const FString& CharacterID) const
{
    // 基於決策難度和角色技能匹配度計算質量
    float BaseQuality = (1.0f - Decision.DifficultyRating) * 100.0f;
    
    // 技能匹配加成
    TArray<FString> CharacterSkills = GetCharacterSkills(CharacterID);
    float SkillBonus = 0.0f;
    for (const FString& RequiredSkill : Decision.RequiredSkills)
    {
        if (CharacterSkills.Contains(RequiredSkill))
        {
            SkillBonus += 20.0f;
        }
    }

    return FMath::Clamp(BaseQuality + SkillBonus, 0.0f, 100.0f);
}

float UMingGoRTSHistoricalRoleplay::CalculateImmersionScore(const FString& SessionID) const
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return 0.0f;
    }

    const FRoleplaySession& Session = ActiveSessions[SessionID];
    
    // 基於決策數量和質量計算沉浸感
    float DecisionCount = static_cast<float>(Session.DecisionHistory.Num());
    float AverageQuality = (Session.HistoricalAccuracyScore + Session.DecisionQualityScore) / 2.0f;
    
    return FMath::Clamp((DecisionCount * 10.0f + AverageQuality) / 2.0f, 0.0f, 100.0f);
}

FString UMingGoRTSHistoricalRoleplay::GenerateSessionID(const FString& CharacterID) const
{
    return FString::Printf(TEXT("Session_%s_%s"), *CharacterID, *FDateTime::Now().ToString());
}

bool UMingGoRTSHistoricalRoleplay::IsSituationAvailable(const FString& SituationID, const FString& CharacterID) const
{
    // 簡化實現：檢查情境是否屬於該角色
    if (SituationMap.Contains(SituationID))
    {
        const FHistoricalSituation& Situation = SituationMap[SituationID];
        return Situation.KeyFigures.Contains(CharacterID);
    }
    
    return false;
}

TArray<FString> UMingGoRTSHistoricalRoleplay::GetCharacterSkills(const FString& CharacterID) const
{
    // 簡化實現：基於角色ID返回技能
    TArray<FString> Skills;
    
    if (CharacterID == TEXT("ChiangKaiShek"))
    {
        Skills.Add(TEXT("Leadership"));
        Skills.Add(TEXT("TacticalCommand"));
        Skills.Add(TEXT("Diplomacy"));
    }
    else if (CharacterID == TEXT("ZhouEnlai"))
    {
        Skills.Add(TEXT("Diplomacy"));
        Skills.Add(TEXT("Charisma"));
        Skills.Add(TEXT("Intelligence"));
    }
    else if (CharacterID == TEXT("ZhangXueliang"))
    {
        Skills.Add(TEXT("Courage"));
        Skills.Add(TEXT("Patriotism"));
        Skills.Add(TEXT("Leadership"));
    }
    
    return Skills;
}

bool UMingGoRTSHistoricalRoleplay::CheckDecisionPrerequisites(const FDecisionOption& Option, const FString& CharacterID) const
{
    // 簡化實現：檢查基本條件
    return true;
}

void UMingGoRTSHistoricalRoleplay::RecordDecisionHistory(const FString& SessionID, const FString& DecisionText)
{
    if (ActiveSessions.Contains(SessionID))
    {
        FRoleplaySession& Session = ActiveSessions[SessionID];
        Session.DecisionHistory.Add(DecisionText);
        
        // 限制歷史記錄數量
        if (Session.DecisionHistory.Num() > 50)
        {
            Session.DecisionHistory.RemoveAt(0);
        }
    }
}

void UMingGoRTSHistoricalRoleplay::UpdateCharacterRelationships(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes)
{
    // TODO: 實現角色關係更新
    UE_LOG(LogTemp, Log, TEXT("更新角色關係，會話：%s"), *SessionID);
}

void UMingGoRTSHistoricalRoleplay::TriggerFollowUpEvents(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes)
{
    // TODO: 實現後續事件觸發
    UE_LOG(LogTemp, Log, TEXT("觸發後續事件，會話：%s"), *SessionID);
}

FString UMingGoRTSHistoricalRoleplay::GenerateSituationID(const FString& BaseName, ERoleplayType Type) const
{
    FString TypePrefix;
    switch (Type)
    {
    case ERoleplayType::PoliticalDecision:
        TypePrefix = TEXT("POL");
        break;
    case ERoleplayType::MilitaryStrategy:
        TypePrefix = TEXT("MIL");
        break;
    case ERoleplayType::DiplomaticNegotiation:
        TypePrefix = TEXT("DIP");
        break;
    case ERoleplayType::EconomicPolicy:
        TypePrefix = TEXT("ECO");
        break;
    case ERoleplayType::SocialReform:
        TypePrefix = TEXT("SOC");
        break;
    case ERoleplayType::PersonalChoice:
        TypePrefix = TEXT("PER");
        break;
    default:
        TypePrefix = TEXT("GEN");
        break;
    }

    return FString::Printf(TEXT("%s_%s"), *TypePrefix, *BaseName);
}
