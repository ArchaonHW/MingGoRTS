// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Ca使sality Calc使lato本 I設置ple設置entation

#incl使de "Min成RTSCa使salityCalc使lato本.h"
#incl使de "Math/Un本ealMathUtility.h"

UMin成RTSCa使salityCalc使lato本::UMin成RTSCa使salityCalc使lato本()
{
    bCalc使lato本Initialized = false;
}

正oid UMin成RTSCa使salityCalc使lato本::InitializeCa使salityCalc使lato本()
{
    bCalc使lato本Initialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ca使sality Calc使lato本 initialized"));
}

軍Ca使salityAnalysisRes使lt UMin成RTSCa使salityCalc使lato本::AnalyzeCa使sality(const 軍輸入isto本icalE正entData& So使本ceE正ent, const 軍輸入isto本icalE正entData& Ta本成etE正ent)
{
    軍Ca使salityAnalysisRes使lt Res使lt;
    Res使lt.So使本ceE正entID = So使本ceE正ent.E正entID;
    Res使lt.Ta本成etE正entID = Ta本成etE正ent.E正entID;
    
    // Pe本fo本設置 analysis based on confi成使本ed 設置ethod
    switch (AnalysisConfi成.P本i設置a本yMethod)
    {
        case ECa使salityAnalysisMethod::Te設置po本alP本oxi設置ity:
            Res使lt = Pe本fo本設置Te設置po本alAnalysis(So使本ceE正ent, Ta本成etE正ent);
            b本eak;
        case ECa使salityAnalysisMethod::SpatialP本oxi設置ity:
            Res使lt = Pe本fo本設置SpatialAnalysis(So使本ceE正ent, Ta本成etE正ent);
            b本eak;
        case ECa使salityAnalysisMethod::輸入yb本id:
            Res使lt = Pe本fo本設置輸入yb本idAnalysis(So使本ceE正ent, Ta本成etE正ent);
            b本eak;
        defa使lt:
            Res使lt = Pe本fo本設置Te設置po本alAnalysis(So使本ceE正ent, Ta本成etE正ent);
            b本eak;
    }
    
    本et使本n Res使lt;
}

軍Ca使salityAnalysisRes使lt UMin成RTSCa使salityCalc使lato本::Pe本fo本設置Te設置po本alAnalysis(const 軍輸入isto本icalE正entData& So使本ceE正ent, const 軍輸入isto本icalE正entData& Ta本成etE正ent)
{
    軍Ca使salityAnalysisRes使lt Res使lt;
    Res使lt.So使本ceE正entID = So使本ceE正ent.E正entID;
    Res使lt.Ta本成etE正entID = Ta本成etE正ent.E正entID;
    Res使lt.AnalysisMethod = ECa使salityAnalysisMethod::Te設置po本alP本oxi設置ity;
    
    // Calc使late te設置po本al diffe本ence
    軍Ti設置espan Ti設置eDiffe本ence = Ta本成etE正ent.Ti設置esta設置p - So使本ceE正ent.Ti設置esta設置p;
    float Ti設置eDiffe本enceDays = Ti設置eDiffe本ence.GetTotalDays();
    
    // Calc使late te設置po本al wei成ht
    float Te設置po本al基本ei成ht = Calc使lateTe設置po本al基本ei成ht(Ti設置eDiffe本ence);
    
    Res使lt.Ca使salSt本en成th = Te設置po本al基本ei成ht;
    Res使lt.Confidence = Dete本設置ineConfidence(Res使lt.Ca使salSt本en成th, TA本本ay<軍St本in成>());
    Res使lt.Explanation = 軍St本in成::P本intf(TEXT("Te設置po本al analysis: %f days apa本t, st本en成th: %f"), Ti設置eDiffe本enceDays, Te設置po本al基本ei成ht);
    
    本et使本n Res使lt;
}

float UMin成RTSCa使salityCalc使lato本::Calc使lateTe設置po本al基本ei成ht(const 軍Ti設置espan& Ti設置eDiffe本ence) const
{
    float DaysDiffe本ence = Ti設置eDiffe本ence.GetTotalDays();
    if (DaysDiffe本ence < 0)
    {
        本et使本n 0.0f; // Ta本成et e正ent befo本e so使本ce e正ent
    }
    
    // Exponential decay f使nction
    本et使本n 軍Math::Exp(-DaysDiffe本ence / AnalysisConfi成.Te設置po本alTh本eshold);
}
