#include "MingGoRTSHistoricalSimulation.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSHistoricalSimulation::UMingGoRTSHistoricalSimulation()
{
    bIsInitialized = false;
}

void UMingGoRTSHistoricalSimulation::InitializeSimulationSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("歷史模擬系統已經初始化"));
        return;
    }

    // 初始化模擬庫
    InitializeSimulationLibrary();

    // 構建模擬映射
    SimulationMap.Empty();
    for (const FHistoricalSimulation& Simulation : AllSimulations)
    {
        SimulationMap.Add(Simulation.SimulationID, Simulation);
    }

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("歷史模擬系統初始化完成，共載入 %d 個模擬"), AllSimulations.Num());
}

TArray<FHistoricalSimulation> UMingGoRTSHistoricalSimulation::GetAllSimulations() const
{
    return AllSimulations;
}

FHistoricalSimulation UMingGoRTSHistoricalSimulation::GetSimulation(const FString& SimulationID) const
{
    if (const FHistoricalSimulation* Simulation = SimulationMap.Find(SimulationID))
    {
        return *Simulation;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到模擬 ID：%s"), *SimulationID);
    return FHistoricalSimulation();
}

bool UMingGoRTSHistoricalSimulation::StartSimulation(const FString& SimulationID, const TMap<FString, float>& PlayerDecisions)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("歷史模擬系統未初始化"));
        return false;
    }

    if (!SimulationMap.Contains(SimulationID))
    {
        UE_LOG(LogTemp, Error, TEXT("模擬不存在：%s"), *SimulationID);
        return false;
    }

    if (ActiveSimulations.Contains(SimulationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("模擬已在運行：%s"), *SimulationID);
        return false;
    }

    // 驗證參數
    if (!ValidateSimulationParameters(SimulationID, PlayerDecisions))
    {
        UE_LOG(LogTemp, Error, TEXT("模擬參數驗證失敗：%s"), *SimulationID);
        return false;
    }

    // 設置模擬狀態
    ActiveSimulations.Add(SimulationID, ESimulationPhase::Setup);

    // 初始化參數
    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    TArray<FSimulationParameter> InitialParameters = Simulation.InitialParameters;
    
    // 應用玩家決策
    for (const auto& Decision : PlayerDecisions)
    {
        for (FSimulationParameter& Param : InitialParameters)
        {
            if (Param.ParameterName == Decision.Key)
            {
                Param.Value = Decision.Value;
                break;
            }
        }
    }
    
    SimulationParameters.Add(SimulationID, InitialParameters);

    // 初始化歷史記錄
    TArray<FString> History;
    History.Add(FString::Printf(TEXT("模擬開始：%s"), *SimulationID));
    SimulationHistories.Add(SimulationID, History);

    // 設置為執行階段
    ActiveSimulations[SimulationID] = ESimulationPhase::Execution;

    // 廣播模擬開始事件
    OnSimulationStarted.Broadcast(SimulationID, Simulation.SimulationType);

    UE_LOG(LogTemp, Log, TEXT("開始歷史模擬：%s"), *SimulationID);
    return true;
}

bool UMingGoRTSHistoricalSimulation::PauseSimulation(const FString& SimulationID)
{
    if (!ActiveSimulations.Contains(SimulationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("模擬不存在或未運行：%s"), *SimulationID);
        return false;
    }

    ESimulationPhase CurrentPhase = ActiveSimulations[SimulationID];
    if (CurrentPhase == ESimulationPhase::Execution)
    {
        ActiveSimulations[SimulationID] = ESimulationPhase::Analysis;
        UE_LOG(LogTemp, Log, TEXT("模擬已暫停：%s"), *SimulationID);
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("模擬無法暫停，當前狀態：%s"), *StaticEnum<ESimulationPhase>()->GetValueAsString(CurrentPhase));
    return false;
}

bool UMingGoRTSHistoricalSimulation::ResumeSimulation(const FString& SimulationID)
{
    if (!ActiveSimulations.Contains(SimulationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("模擬不存在或未運行：%s"), *SimulationID);
        return false;
    }

    ESimulationPhase CurrentPhase = ActiveSimulations[SimulationID];
    if (CurrentPhase == ESimulationPhase::Analysis)
    {
        ActiveSimulations[SimulationID] = ESimulationPhase::Execution;
        UE_LOG(LogTemp, Log, TEXT("模擬已繼續：%s"), *SimulationID);
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("模擬無法繼續，當前狀態：%s"), *StaticEnum<ESimulationPhase>()->GetValueAsString(CurrentPhase));
    return false;
}

bool UMingGoRTSHistoricalSimulation::StopSimulation(const FString& SimulationID)
{
    if (!ActiveSimulations.Contains(SimulationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("模擬不存在或未運行：%s"), *SimulationID);
        return false;
    }

    // 生成最終結果
    GenerateSimulationResult(SimulationID);

    // 設置為結果階段
    ActiveSimulations[SimulationID] = ESimulationPhase::Results;

    // 記錄停止事件
    RecordSimulationEvent(SimulationID, TEXT("模擬停止"));

    UE_LOG(LogTemp, Log, TEXT("模擬已停止：%s"), *SimulationID);
    return true;
}

bool UMingGoRTSHistoricalSimulation::InterveneInSimulation(const FString& SimulationID, const FString& ParameterName, float NewValue)
{
    if (!ActiveSimulations.Contains(SimulationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("模擬不存在或未運行：%s"), *SimulationID);
        return false;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    if (!Simulation.bCanIntervene)
    {
        UE_LOG(LogTemp, Warning, TEXT("模擬不允許干預：%s"), *SimulationID);
        return false;
    }

    if (!SimulationParameters.Contains(SimulationID))
    {
        UE_LOG(LogTemp, Error, TEXT("模擬參數不存在：%s"), *SimulationID);
        return false;
    }

    // 更新參數值
    SetParameterValue(SimulationID, ParameterName, NewValue);

    // 記錄干預事件
    RecordSimulationEvent(SimulationID, FString::Printf(TEXT("玩家干預：%s = %f"), *ParameterName, NewValue));

    UE_LOG(LogTemp, Log, TEXT("模擬干預成功：%s，參數：%s = %f"), *SimulationID, *ParameterName, NewValue);
    return true;
}

ESimulationPhase UMingGoRTSHistoricalSimulation::GetSimulationPhase(const FString& SimulationID) const
{
    if (const ESimulationPhase* Phase = ActiveSimulations.Find(SimulationID))
    {
        return *Phase;
    }
    
    return ESimulationPhase::Setup;
}

TArray<FSimulationParameter> UMingGoRTSHistoricalSimulation::GetCurrentParameters(const FString& SimulationID) const
{
    if (const TArray<FSimulationParameter>* Parameters = SimulationParameters.Find(SimulationID))
    {
        return *Parameters;
    }
    
    return TArray<FSimulationParameter>();
}

float UMingGoRTSHistoricalSimulation::GetSimulationProgress(const FString& SimulationID) const
{
    if (!SimulationMap.Contains(SimulationID))
    {
        return 0.0f;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    // 簡化實現：基於當前階段計算進度
    if (ActiveSimulations.Contains(SimulationID))
    {
        ESimulationPhase Phase = ActiveSimulations[SimulationID];
        switch (Phase)
        {
        case ESimulationPhase::Setup:
            return 0.1f;
        case ESimulationPhase::Execution:
            return 0.5f;
        case ESimulationPhase::Analysis:
            return 0.8f;
        case ESimulationPhase::Results:
            return 1.0f;
        }
    }
    
    return 0.0f;
}

FSimulationResult UMingGoRTSHistoricalSimulation::GetSimulationResult(const FString& SimulationID) const
{
    if (const FSimulationResult* Result = SimulationResults.Find(SimulationID))
    {
        return *Result;
    }
    
    return FSimulationResult();
}

TArray<FString> UMingGoRTSHistoricalSimulation::GetSimulationHistory(const FString& SimulationID) const
{
    if (const TArray<FString>* History = SimulationHistories.Find(SimulationID))
    {
        return *History;
    }
    
    return TArray<FString>();
}

TArray<float> UMingGoRTSHistoricalSimulation::PredictSimulationOutcome(const FString& SimulationID, const TMap<FString, float>& ParameterChanges) const
{
    TArray<float> Predictions;
    
    if (!SimulationMap.Contains(SimulationID))
    {
        return Predictions;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    // 簡化實現：基於參數變化預測結果
    float SuccessProbability = 0.5f; // 基礎成功率
    
    // 計算參數影響
    for (const auto& Change : ParameterChanges)
    {
        // 根據參數類型調整成功率
        if (Change.Key.Contains(TEXT("Military")))
        {
            SuccessProbability += Change.Value * 0.1f;
        }
        else if (Change.Key.Contains(TEXT("Economic")))
        {
            SuccessProbability += Change.Value * 0.15f;
        }
        else if (Change.Key.Contains(TEXT("Political")))
        {
            SuccessProbability += Change.Value * 0.2f;
        }
    }
    
    // 限制在0-1範圍內
    SuccessProbability = FMath::Clamp(SuccessProbability, 0.0f, 1.0f);
    
    Predictions.Add(SuccessProbability);
    Predictions.Add(SuccessProbability * 100.0f); // 轉換為百分比
    Predictions.Add((1.0f - SuccessProbability) * 100.0f); // 失敗概率
    
    return Predictions;
}

float UMingGoRTSHistoricalSimulation::CompareWithHistoricalBaseline(const FString& SimulationID) const
{
    if (!SimulationMap.Contains(SimulationID))
    {
        return 0.0f;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    // 簡化實現：基於模擬結果與歷史基準的比較
    if (SimulationResults.Contains(SimulationID))
    {
        const FSimulationResult& Result = SimulationResults[SimulationID];
        return Result.HistoricalAccuracy;
    }
    
    return 0.0f;
}

TMap<FString, float> UMingGoRTSHistoricalSimulation::GetSimulationStatistics(const FString& SimulationID) const
{
    TMap<FString, float> Statistics;
    
    if (!SimulationMap.Contains(SimulationID))
    {
        return Statistics;
    }

    // 基本統計
    Statistics.Add(TEXT("Progress"), GetSimulationProgress(SimulationID));
    Statistics.Add(TEXT("HistoricalAccuracy"), CompareWithHistoricalBaseline(SimulationID));
    
    if (SimulationResults.Contains(SimulationID))
    {
        const FSimulationResult& Result = SimulationResults[SimulationID];
        Statistics.Add(TEXT("FinalScore"), Result.FinalScore);
        Statistics.Add(TEXT("SuccessRate"), Result.bSuccess ? 1.0f : 0.0f);
    }
    
    return Statistics;
}

bool UMingGoRTSHistoricalSimulation::SaveSimulationData(const FString& SaveSlotName)
{
    // TODO: 實現模擬數據保存
    UE_LOG(LogTemp, Log, TEXT("保存模擬數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSHistoricalSimulation::LoadSimulationData(const FString& SaveSlotName)
{
    // TODO: 實現模擬數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入模擬數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSHistoricalSimulation::InitializeSimulationLibrary()
{
    CreatePoliticalSimulations();
    CreateMilitarySimulations();
    CreateEconomicSimulations();
    CreateSocialSimulations();
    CreateCulturalSimulations();
    CreateInternationalSimulations();
}

void UMingGoRTSHistoricalSimulation::CreatePoliticalSimulations()
{
    // 北伐統一模擬
    FHistoricalSimulation NorthernExpedition;
    NorthernExpedition.SimulationID = GenerateSimulationID(TEXT("NorthernExpedition"), ESimulationType::Political);
    NorthernExpedition.Title = TEXT("北伐統一模擬");
    NorthernExpedition.Description = TEXT("模擬1926-1928年北伐戰爭的政治決策和統一進程");
    NorthernExpedition.SimulationType = ESimulationType::Political;
    NorthernExpedition.HistoricalPeriod = TEXT("1926-1928");
    NorthernExpedition.GeographicScope = TEXT("中國全境");
    NorthernExpedition.SimulationDuration = 24.0f; // 24個月
    NorthernExpedition.MaxIterations = 100;
    NorthernExpedition.bIsRealTime = false;
    NorthernExpedition.bCanIntervene = true;

    // 初始參數
    FSimulationParameter MilitaryStrength;
    MilitaryStrength.ParameterName = TEXT("MilitaryStrength");
    MilitaryStrength.Value = 60.0f;
    MilitaryStrength.MinValue = 0.0f;
    MilitaryStrength.MaxValue = 100.0f;
    MilitaryStrength.Unit = TEXT("%");
    MilitaryStrength.Description = TEXT("軍事實力");
    NorthernExpedition.InitialParameters.Add(MilitaryStrength);

    FSimulationParameter PoliticalSupport;
    PoliticalSupport.ParameterName = TEXT("PoliticalSupport");
    PoliticalSupport.Value = 70.0f;
    PoliticalSupport.MinValue = 0.0f;
    PoliticalSupport.MaxValue = 100.0f;
    PoliticalSupport.Unit = TEXT("%");
    PoliticalSupport.Description = TEXT("政治支持度");
    NorthernExpedition.InitialParameters.Add(PoliticalSupport);

    FSimulationParameter EconomicResources;
    EconomicResources.ParameterName = TEXT("EconomicResources");
    EconomicResources.Value = 50.0f;
    EconomicResources.MinValue = 0.0f;
    EconomicResources.MaxValue = 100.0f;
    EconomicResources.Unit = TEXT("%");
    EconomicResources.Description = TEXT("經濟資源");
    NorthernExpedition.InitialParameters.Add(EconomicResources);

    // 成功條件
    NorthernExpedition.SuccessConditions.Add(TEXT("MilitaryStrength > 80"));
    NorthernExpedition.SuccessConditions.Add(TEXT("PoliticalSupport > 75"));
    NorthernExpedition.SuccessConditions.Add(TEXT("EconomicResources > 60"));

    // 失敗條件
    NorthernExpedition.FailureConditions.Add(TEXT("MilitaryStrength < 30"));
    NorthernExpedition.FailureConditions.Add(TEXT("PoliticalSupport < 40"));

    // 可能事件
    FSimulationEvent WarlordResistance;
    WarlordResistance.EventID = TEXT("WarlordResistance");
    WarlordResistance.EventName = TEXT("軍閥抵抗");
    WarlordResistance.Description = TEXT("軍閥勢力抵抗北伐軍");
    WarlordResistance.TriggerProbability = 0.3f;
    WarlordResistance.Consequences.Add(TEXT("MilitaryStrength -= 10"));
    WarlordResistance.Consequences.Add(TEXT("PoliticalSupport -= 5"));
    NorthernExpedition.PossibleEvents.Add(WarlordResistance);

    FSimulationEvent ForeignSupport;
    ForeignSupport.EventID = TEXT("ForeignSupport");
    ForeignSupport.EventName = TEXT("外國支持");
    ForeignSupport.Description = TEXT("獲得外國政府的支持");
    ForeignSupport.TriggerProbability = 0.2f;
    ForeignSupport.Consequences.Add(TEXT("EconomicResources += 15"));
    ForeignSupport.Consequences.Add(TEXT("PoliticalSupport += 10"));
    NorthernExpedition.PossibleEvents.Add(ForeignSupport);

    AllSimulations.Add(NorthernExpedition);
}

void UMingGoRTSHistoricalSimulation::CreateMilitarySimulations()
{
    // 西安事變模擬
    FHistoricalSimulation XianIncident;
    XianIncident.SimulationID = GenerateSimulationID(TEXT("XianIncident"), ESimulationType::Military);
    XianIncident.Title = TEXT("西安事變模擬");
    XianIncident.Description = TEXT("模擬1936年西安事變的軍事和政治決策");
    XianIncident.SimulationType = ESimulationType::Military;
    XianIncident.HistoricalPeriod = TEXT("1936-12");
    XianIncident.GeographicScope = TEXT("西安");
    XianIncident.SimulationDuration = 1.0f; // 1個月
    XianIncident.MaxIterations = 50;
    XianIncident.bIsRealTime = false;
    XianIncident.bCanIntervene = true;

    // 初始參數
    FSimulationParameter NationalUnity;
    NationalUnity.ParameterName = TEXT("NationalUnity");
    NationalUnity.Value = 30.0f;
    NationalUnity.MinValue = 0.0f;
    NationalUnity.MaxValue = 100.0f;
    NationalUnity.Unit = TEXT("%");
    NationalUnity.Description = TEXT("國民團結度");
    XianIncident.InitialParameters.Add(NationalUnity);

    FSimulationParameter AntiJapaneseSentiment;
    AntiJapaneseSentiment.ParameterName = TEXT("AntiJapaneseSentiment");
    AntiJapaneseSentiment.Value = 80.0f;
    AntiJapaneseSentiment.MinValue = 0.0f;
    AntiJapaneseSentiment.MaxValue = 100.0f;
    AntiJapaneseSentiment.Unit = TEXT("%");
    AntiJapaneseSentiment.Description = TEXT("抗日情緒");
    XianIncident.InitialParameters.Add(AntiJapaneseSentiment);

    // 成功條件
    XianIncident.SuccessConditions.Add(TEXT("NationalUnity > 70"));
    XianIncident.SuccessConditions.Add(TEXT("AntiJapaneseSentiment > 85"));

    AllSimulations.Add(XianIncident);
}

void UMingGoRTSHistoricalSimulation::CreateEconomicSimulations()
{
    // 抗戰經濟模擬
    FHistoricalSimulation WarEconomy;
    WarEconomy.SimulationID = GenerateSimulationID(TEXT("WarEconomy"), ESimulationType::Economic);
    WarEconomy.Title = TEXT("抗戰經濟模擬");
    WarEconomy.Description = TEXT("模擬抗戰時期的經濟政策和資源分配");
    WarEconomy.SimulationType = ESimulationType::Economic;
    WarEconomy.HistoricalPeriod = TEXT("1937-1945");
    WarEconomy.GeographicScope = TEXT("中國後方");
    WarEconomy.SimulationDuration = 96.0f; // 96個月
    WarEconomy.MaxIterations = 200;
    WarEconomy.bIsRealTime = false;
    WarEconomy.bCanIntervene = true;

    // 初始參數
    FSimulationParameter IndustrialProduction;
    IndustrialProduction.ParameterName = TEXT("IndustrialProduction");
    IndustrialProduction.Value = 40.0f;
    IndustrialProduction.MinValue = 0.0f;
    IndustrialProduction.MaxValue = 100.0f;
    IndustrialProduction.Unit = TEXT("%");
    IndustrialProduction.Description = TEXT("工業生產");
    WarEconomy.InitialParameters.Add(IndustrialProduction);

    FSimulationParameter AgriculturalOutput;
    AgriculturalOutput.ParameterName = TEXT("AgriculturalOutput");
    AgriculturalOutput.Value = 60.0f;
    AgriculturalOutput.MinValue = 0.0f;
    AgriculturalOutput.MaxValue = 100.0f;
    AgriculturalOutput.Unit = TEXT("%");
    AgriculturalOutput.Description = TEXT("農業產出");
    WarEconomy.InitialParameters.Add(AgriculturalOutput);

    AllSimulations.Add(WarEconomy);
}

void UMingGoRTSHistoricalSimulation::CreateSocialSimulations()
{
    // 社會改革模擬
    FHistoricalSimulation SocialReform;
    SocialReform.SimulationID = GenerateSimulationID(TEXT("SocialReform"), ESimulationType::Social);
    SocialReform.Title = TEXT("社會改革模擬");
    SocialReform.Description = TEXT("模擬新文化運動時期的社會改革");
    SocialReform.SimulationType = ESimulationType::Social;
    SocialReform.HistoricalPeriod = TEXT("1920-1930");
    SocialReform.GeographicScope = TEXT("主要城市");
    SocialReform.SimulationDuration = 120.0f; // 120個月
    SocialReform.MaxIterations = 150;
    SocialReform.bIsRealTime = false;
    SocialReform.bCanIntervene = true;

    // 初始參數
    FSimulationParameter EducationLevel;
    EducationLevel.ParameterName = TEXT("EducationLevel");
    EducationLevel.Value = 30.0f;
    EducationLevel.MinValue = 0.0f;
    EducationLevel.MaxValue = 100.0f;
    EducationLevel.Unit = TEXT("%");
    EducationLevel.Description = TEXT("教育水平");
    SocialReform.InitialParameters.Add(EducationLevel);

    FSimulationParameter SocialStability;
    SocialStability.ParameterName = TEXT("SocialStability");
    SocialStability.Value = 70.0f;
    SocialStability.MinValue = 0.0f;
    SocialStability.MaxValue = 100.0f;
    SocialStability.Unit = TEXT("%");
    SocialStability.Description = TEXT("社會穩定性");
    SocialReform.InitialParameters.Add(SocialStability);

    AllSimulations.Add(SocialReform);
}

void UMingGoRTSHistoricalSimulation::CreateCulturalSimulations()
{
    // 文化發展模擬
    FHistoricalSimulation CulturalDevelopment;
    CulturalDevelopment.SimulationID = GenerateSimulationID(TEXT("CulturalDevelopment"), ESimulationType::Cultural);
    CulturalDevelopment.Title = TEXT("文化發展模擬");
    CulturalDevelopment.Description = TEXT("模擬民國時期的文化發展和藝術創作");
    CulturalDevelopment.SimulationType = ESimulationType::Cultural;
    CulturalDevelopment.HistoricalPeriod = TEXT("1912-1949");
    CulturalDevelopment.GeographicScope = TEXT("全國");
    CulturalDevelopment.SimulationDuration = 444.0f; // 444個月
    CulturalDevelopment.MaxIterations = 300;
    CulturalDevelopment.bIsRealTime = false;
    CulturalDevelopment.bCanIntervene = true;

    // 初始參數
    FSimulationParameter ArtisticFreedom;
    ArtisticFreedom.ParameterName = TEXT("ArtisticFreedom");
    ArtisticFreedom.Value = 60.0f;
    ArtisticFreedom.MinValue = 0.0f;
    ArtisticFreedom.MaxValue = 100.0f;
    ArtisticFreedom.Unit = TEXT("%");
    ArtisticFreedom.Description = TEXT("藝術自由度");
    CulturalDevelopment.InitialParameters.Add(ArtisticFreedom);

    FSimulationParameter CulturalInfluence;
    CulturalInfluence.ParameterName = TEXT("CulturalInfluence");
    CulturalInfluence.Value = 50.0f;
    CulturalInfluence.MinValue = 0.0f;
    CulturalInfluence.MaxValue = 100.0f;
    CulturalInfluence.Unit = TEXT("%");
    CulturalInfluence.Description = TEXT("文化影響力");
    CulturalDevelopment.InitialParameters.Add(CulturalInfluence);

    AllSimulations.Add(CulturalDevelopment);
}

void UMingGoRTSHistoricalSimulation::CreateInternationalSimulations()
{
    // 國際關係模擬
    FHistoricalSimulation InternationalRelations;
    InternationalRelations.SimulationID = GenerateSimulationID(TEXT("InternationalRelations"), ESimulationType::International);
    InternationalRelations.Title = TEXT("國際關係模擬");
    InternationalRelations.Description = TEXT("模擬民國時期的國際關係和外交政策");
    InternationalRelations.SimulationType = ESimulationType::International;
    InternationalRelations.HistoricalPeriod = TEXT("1912-1949");
    InternationalRelations.GeographicScope = TEXT("全球");
    InternationalRelations.SimulationDuration = 444.0f; // 444個月
    InternationalRelations.MaxIterations = 250;
    InternationalRelations.bIsRealTime = false;
    InternationalRelations.bCanIntervene = true;

    // 初始參數
    FSimulationParameter DiplomaticRelations;
    DiplomaticRelations.ParameterName = TEXT("DiplomaticRelations");
    DiplomaticRelations.Value = 50.0f;
    DiplomaticRelations.MinValue = 0.0f;
    DiplomaticRelations.MaxValue = 100.0f;
    DiplomaticRelations.Unit = TEXT("%");
    DiplomaticRelations.Description = TEXT("外交關係");
    InternationalRelations.InitialParameters.Add(DiplomaticRelations);

    FSimulationParameter InternationalSupport;
    InternationalSupport.ParameterName = TEXT("InternationalSupport");
    InternationalSupport.Value = 40.0f;
    InternationalSupport.MinValue = 0.0f;
    InternationalSupport.MaxValue = 100.0f;
    InternationalSupport.Unit = TEXT("%");
    InternationalSupport.Description = TEXT("國際支持");
    InternationalRelations.InitialParameters.Add(InternationalSupport);

    AllSimulations.Add(InternationalRelations);
}

void UMingGoRTSHistoricalSimulation::ExecuteSimulationStep(const FString& SimulationID)
{
    if (!ActiveSimulations.Contains(SimulationID) || !SimulationParameters.Contains(SimulationID))
    {
        return;
    }

    ESimulationPhase CurrentPhase = ActiveSimulations[SimulationID];
    if (CurrentPhase != ESimulationPhase::Execution)
    {
        return;
    }

    // 處理模擬事件
    ProcessSimulationEvents(SimulationID);

    // 計算參數變化
    CalculateParameterChanges(SimulationID);

    // 檢查成功條件
    if (CheckSuccessConditions(SimulationID))
    {
        GenerateSimulationResult(SimulationID);
        ActiveSimulations[SimulationID] = ESimulationPhase::Results;
        return;
    }

    // 檢查失敗條件
    if (CheckFailureConditions(SimulationID))
    {
        GenerateSimulationResult(SimulationID);
        ActiveSimulations[SimulationID] = ESimulationPhase::Results;
        return;
    }

    // 更新進度
    UpdateSimulationProgress(SimulationID);
}

void UMingGoRTSHistoricalSimulation::ProcessSimulationEvents(const FString& SimulationID)
{
    if (!SimulationMap.Contains(SimulationID))
    {
        return;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    for (const FSimulationEvent& Event : Simulation.PossibleEvents)
    {
        float TriggerProbability = CalculateEventTriggerProbability(SimulationID, Event);
        
        if (FMath::RandRange(0.0f, 1.0f) < TriggerProbability)
        {
            TriggerSimulationEvent(SimulationID, Event);
        }
    }
}

void UMingGoRTSHistoricalSimulation::TriggerSimulationEvent(const FString& SimulationID, const FSimulationEvent& Event)
{
    // 應用事件後果
    ApplyEventConsequences(SimulationID, Event);

    // 記錄事件
    RecordSimulationEvent(SimulationID, Event.EventName);

    // 廣播事件觸發
    OnSimulationEventTriggered.Broadcast(SimulationID, Event.EventID, Event.EventName);

    UE_LOG(LogTemp, Log, TEXT("模擬事件觸發：%s - %s"), *SimulationID, *Event.EventName);
}

void UMingGoRTSHistoricalSimulation::CalculateParameterChanges(const FString& SimulationID)
{
    if (!SimulationParameters.Contains(SimulationID))
    {
        return;
    }

    TArray<FSimulationParameter>& Parameters = SimulationParameters[SimulationID];
    
    // 簡化實現：基於時間和隨機因素調整參數
    for (FSimulationParameter& Param : Parameters)
    {
        float RandomChange = FMath::RandRange(-2.0f, 2.0f);
        Param.Value = FMath::Clamp(Param.Value + RandomChange, Param.MinValue, Param.MaxValue);
    }
}

bool UMingGoRTSHistoricalSimulation::CheckSuccessConditions(const FString& SimulationID) const
{
    if (!SimulationMap.Contains(SimulationID))
    {
        return false;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    for (const FString& Condition : Simulation.SuccessConditions)
    {
        // 簡化實現：檢查基本條件
        if (Condition.Contains(TEXT(">")))
        {
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT(">"), true);
            if (Parts.Num() == 2)
            {
                float ParamValue = GetParameterValue(SimulationID, Parts[0]);
                float RequiredValue = FCString::Atof(*Parts[1]);
                if (ParamValue <= RequiredValue)
                {
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool UMingGoRTSHistoricalSimulation::CheckFailureConditions(const FString& SimulationID) const
{
    if (!SimulationMap.Contains(SimulationID))
    {
        return false;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    for (const FString& Condition : Simulation.FailureConditions)
    {
        // 簡化實現：檢查基本條件
        if (Condition.Contains(TEXT("<")))
        {
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT("<"), true);
            if (Parts.Num() == 2)
            {
                float ParamValue = GetParameterValue(SimulationID, Parts[0]);
                float RequiredValue = FCString::Atof(*Parts[1]);
                if (ParamValue < RequiredValue)
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void UMingGoRTSHistoricalSimulation::GenerateSimulationResult(const FString& SimulationID)
{
    if (!SimulationMap.Contains(SimulationID))
    {
        return;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    FSimulationResult Result;
    Result.ResultID = FString::Printf(TEXT("Result_%s"), *SimulationID);
    Result.SimulationID = SimulationID;
    Result.bSuccess = CheckSuccessConditions(SimulationID);
    Result.StartTime = FDateTime::Now();
    Result.EndTime = FDateTime::Now();
    
    // 計算最終分數
    if (SimulationParameters.Contains(SimulationID))
    {
        const TArray<FSimulationParameter>& FinalParams = SimulationParameters[SimulationID];
        Result.FinalParameters = FinalParams;
        
        float TotalScore = 0.0f;
        for (const FSimulationParameter& Param : FinalParams)
        {
            TotalScore += Param.Value;
        }
        Result.FinalScore = TotalScore / FinalParams.Num();
    }
    
    // 計算歷史準確性
    Result.HistoricalAccuracy = CalculateHistoricalAccuracy(SimulationID);
    
    // 計算玩家影響
    Result.PlayerImpact = CalculatePlayerImpact(SimulationID);
    
    // 生成結果描述
    Result.OutcomeDescription = GenerateSimulationReport(SimulationID);
    
    SimulationResults.Add(SimulationID, Result);

    // 廣播模擬完成
    OnSimulationCompleted.Broadcast(SimulationID, Result);

    UE_LOG(LogTemp, Log, TEXT("模擬結果生成：%s，成功：%s"), *SimulationID, Result.bSuccess ? TEXT("是") : TEXT("否"));
}

float UMingGoRTSHistoricalSimulation::CalculateHistoricalAccuracy(const FString& SimulationID) const
{
    if (!SimulationMap.Contains(SimulationID))
    {
        return 0.0f;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    // 簡化實現：基於結果與歷史基準的比較
    if (SimulationResults.Contains(SimulationID))
    {
        const FSimulationResult& Result = SimulationResults[SimulationID];
        
        // 基於成功狀態和分數計算準確性
        float BaseAccuracy = Result.bSuccess ? 0.8f : 0.3f;
        float ScoreBonus = (Result.FinalScore / 100.0f) * 0.2f;
        
        return BaseAccuracy + ScoreBonus;
    }
    
    return 0.0f;
}

float UMingGoRTSHistoricalSimulation::CalculatePlayerImpact(const FString& SimulationID) const
{
    if (!SimulationHistories.Contains(SimulationID))
    {
        return 0.0f;
    }

    const TArray<FString>& History = SimulationHistories[SimulationID];
    
    // 基於干預次數計算影響
    int32 InterventionCount = 0;
    for (const FString& Event : History)
    {
        if (Event.Contains(TEXT("玩家干預")))
        {
            InterventionCount++;
        }
    }
    
    return FMath::Clamp(static_cast<float>(InterventionCount) / 10.0f, 0.0f, 1.0f);
}

void UMingGoRTSHistoricalSimulation::UpdateSimulationProgress(const FString& SimulationID)
{
    // 簡化實現：基於時間更新進度
    if (SimulationParameters.Contains(SimulationID))
    {
        // 這裡可以添加更複雜的進度計算邏輯
        UE_LOG(LogTemp, Log, TEXT("更新模擬進度：%s"), *SimulationID);
    }
}

void UMingGoRTSHistoricalSimulation::RecordSimulationEvent(const FString& SimulationID, const FString& EventName)
{
    if (SimulationHistories.Contains(SimulationID))
    {
        TArray<FString>& History = SimulationHistories[SimulationID];
        FString EventRecord = FString::Printf(TEXT("[%s] %s"), *FDateTime::Now().ToString(), *EventName);
        History.Add(EventRecord);
        
        // 限制歷史記錄數量
        if (History.Num() > 100)
        {
            History.RemoveAt(0);
        }
    }
}

float UMingGoRTSHistoricalSimulation::GetParameterValue(const FString& SimulationID, const FString& ParameterName) const
{
    if (SimulationParameters.Contains(SimulationID))
    {
        const TArray<FSimulationParameter>& Parameters = SimulationParameters[SimulationID];
        for (const FSimulationParameter& Param : Parameters)
        {
            if (Param.ParameterName == ParameterName)
            {
                return Param.Value;
            }
        }
    }
    
    return 0.0f;
}

void UMingGoRTSHistoricalSimulation::SetParameterValue(const FString& SimulationID, const FString& ParameterName, float Value)
{
    if (SimulationParameters.Contains(SimulationID))
    {
        TArray<FSimulationParameter>& Parameters = SimulationParameters[SimulationID];
        for (FSimulationParameter& Param : Parameters)
        {
            if (Param.ParameterName == ParameterName)
            {
                Param.Value = FMath::Clamp(Value, Param.MinValue, Param.MaxValue);
                break;
            }
        }
    }
}

FString UMingGoRTSHistoricalSimulation::GenerateSimulationID(const FString& BaseName, ESimulationType Type) const
{
    return FString::Printf(TEXT("%s_%s"), *GetSimulationTypePrefix(Type), *BaseName);
}

FString UMingGoRTSHistoricalSimulation::GetSimulationTypePrefix(ESimulationType Type) const
{
    switch (Type)
    {
    case ESimulationType::Political:
        return TEXT("POL");
    case ESimulationType::Military:
        return TEXT("MIL");
    case ESimulationType::Economic:
        return TEXT("ECO");
    case ESimulationType::Social:
        return TEXT("SOC");
    case ESimulationType::Cultural:
        return TEXT("CUL");
    case ESimulationType::International:
        return TEXT("INT");
    default:
        return TEXT("SIM");
    }
}

bool UMingGoRTSHistoricalSimulation::ValidateSimulationParameters(const FString& SimulationID, const TMap<FString, float>& Parameters) const
{
    if (!SimulationMap.Contains(SimulationID))
    {
        return false;
    }

    const FHistoricalSimulation& Simulation = SimulationMap[SimulationID];
    
    // 檢查所有必需參數
    for (const FSimulationParameter& Param : Simulation.InitialParameters)
    {
        if (!Parameters.Contains(Param.ParameterName))
        {
            return false;
        }
    }
    
    return true;
}

float UMingGoRTSHistoricalSimulation::CalculateEventTriggerProbability(const FString& SimulationID, const FSimulationEvent& Event) const
{
    // 簡化實現：基於基礎概率和參數條件計算
    float BaseProbability = Event.TriggerProbability;
    
    // 根據參數條件調整概率
    for (const FSimulationParameter& RequiredParam : Event.RequiredParameters)
    {
        float CurrentValue = GetParameterValue(SimulationID, RequiredParam.ParameterName);
        float Ratio = CurrentValue / 100.0f;
        BaseProbability *= Ratio;
    }
    
    return FMath::Clamp(BaseProbability, 0.0f, 1.0f);
}

void UMingGoRTSHistoricalSimulation::ApplyEventConsequences(const FString& SimulationID, const FSimulationEvent& Event)
{
    for (const FString& Consequence : Event.Consequences)
    {
        // 解析後果字符串並應用
        if (Consequence.Contains(TEXT("+=")))
        {
            TArray<FString> Parts;
            Consequence.ParseIntoArray(Parts, TEXT("+="), true);
            if (Parts.Num() == 2)
            {
                float CurrentValue = GetParameterValue(SimulationID, Parts[0]);
                float Change = FCString::Atof(*Parts[1]);
                SetParameterValue(SimulationID, Parts[0], CurrentValue + Change);
            }
        }
        else if (Consequence.Contains(TEXT("-=")))
        {
            TArray<FString> Parts;
            Consequence.ParseIntoArray(Parts, TEXT("-="), true);
            if (Parts.Num() == 2)
            {
                float CurrentValue = GetParameterValue(SimulationID, Parts[0]);
                float Change = FCString::Atof(*Parts[1]);
                SetParameterValue(SimulationID, Parts[0], CurrentValue - Change);
            }
        }
    }
}

FString UMingGoRTSHistoricalSimulation::GenerateSimulationReport(const FString& SimulationID) const
{
    FString Report = FString::Printf(TEXT("模擬報告：%s\n"), *SimulationID);
    
    if (SimulationResults.Contains(SimulationID))
    {
        const FSimulationResult& Result = SimulationResults[SimulationID];
        Report += FString::Printf(TEXT("結果：%s\n"), Result.bSuccess ? TEXT("成功") : TEXT("失敗"));
        Report += FString::Printf(TEXT("分數：%.1f\n"), Result.FinalScore);
        Report += FString::Printf(TEXT("歷史準確性：%.1f%%\n"), Result.HistoricalAccuracy * 100.0f);
        Report += FString::Printf(TEXT("玩家影響：%.1f%%\n"), Result.PlayerImpact * 100.0f);
    }
    
    return Report;
}
