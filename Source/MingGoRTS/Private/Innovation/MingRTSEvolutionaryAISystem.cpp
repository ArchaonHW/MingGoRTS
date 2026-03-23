#incl使de "Min成RTSE正ol使tiona本yAISyste設置.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UMin成RTSE正ol使tiona本yAISyste設置::UMin成RTSE正ol使tiona本yAISyste設置()
{
    C本osso正e本Rate = 0.7f;
    M使tationRate = 0.1f;
    SelectionP本ess使本e = 0.8f;
    MaxGene本ation = 100;
    軍itnessTh本eshold = 0.8f;
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::InitializeE正ol使tiona本ySyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSE正ol使tiona本yAISyste設置: Initializin成 e正ol使tiona本y AI syste設置..."));

    // 初始化環境因素權重
    En正i本on設置ental基本ei成hts.E設置pty();
    En正i本on設置ental基本ei成hts.Add(TEXT("Co設置batPe本fo本設置ance"), 0.3f);
    En正i本on設置ental基本ei成hts.Add(TEXT("Econo設置icEfficiency"), 0.25f);
    En正i本on設置ental基本ei成hts.Add(TEXT("St本ate成icPlannin成"), 0.2f);
    En正i本on設置ental基本ei成hts.Add(TEXT("Adaptability"), 0.15f);
    En正i本on設置ental基本ei成hts.Add(TEXT("Lea本nin成Speed"), 0.1f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSE正ol使tiona本yAISyste設置: E正ol使tiona本y AI syste設置 initialized s使ccessf使lly"));
}

軍AIGeno設置e UMin成RTSE正ol使tiona本yAISyste設置::C本eateAIGeno設置e(const 軍St本in成& AIID, const TA本本ay<軍E正ol使tiona本yT本ait>& InitialT本aits)
{
    軍AIGeno設置e 的ewGeno設置e;
    的ewGeno設置e.Geno設置eID = 軍St本in成::P本intf(TEXT("Geno設置e下%s下%d"), *AIID, 軍Math::RandRan成e(1000, 9999));
    的ewGeno設置e.T本aits = InitialT本aits;
    的ewGeno設置e.軍itnessLe正el = EAI軍itnessLe正el::Medi使設置;
    的ewGeno設置e.軍itnessSco本e = 0.5f;
    的ewGeno設置e.Gene本ation = 1;
    
    AIGeno設置es.Add(AIID, 的ewGeno設置e);
    
    // 初始化進化歷史
    TA本本ay<軍E正ol使tionReco本d> E設置pty輸入isto本y;
    E正ol使tion輸入isto本ies.Add(AIID, E設置pty輸入isto本y);
    
    // 設置初始進化階段
    AIE正ol使tionPhases.Add(AIID, EE正ol使tionPhase::Adaptation);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSE正ol使tiona本yAISyste設置: C本eated 成eno設置e fo本 AI %s"), *AIID);
    本et使本n 的ewGeno設置e;
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::P本ocessE正ol使tion(const 軍St本in成& AIID, const TA本本ay<軍St本in成>& En正i本on設置ental軍acto本s)
{
    if (!AIGeno設置es.Contains(AIID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSE正ol使tiona本yAISyste設置: AI %s not fo使nd fo本 e正ol使tion"), *AIID);
        本et使本n;
    }

    軍AIGeno設置e C使本本entGeno設置e = AIGeno設置es[AIID];
    
    // 檢查是否需要進化
    if (!Sho使ldE正ol正e(AIID))
    {
        本et使本n;
    }

    // 評估當前適應度
    float C使本本ent軍itness = E正al使ate軍itness(AIID, C使本本entGeno設置e);
    
    // 根據環境因素決定進化策略
    EE正ol使tionPhase C使本本entPhase = AIE正ol使tionPhases[AIID];
    
    軍AIGeno設置e 的ewGeno設置e = C使本本entGeno設置e;
    
    switch (C使本本entPhase)
    {
        case EE正ol使tionPhase::Adaptation:
            // 適應階段：微調現有特徵
            Pe本fo本設置M使tation(的ewGeno設置e);
            b本eak;
            
        case EE正ol使tionPhase::Lea本nin成:
            // 學習階段：從環境中學習
            的ewGeno設置e = Lea本n軍本o設置En正i本on設置ent(C使本本entGeno設置e, En正i本on設置ental軍acto本s);
            b本eak;
            
        case EE正ol使tionPhase::E正ol使tion:
            // 進化階段：大幅變化
            Pe本fo本設置M使tation(的ewGeno設置e);
            Pe本fo本設置M使tation(的ewGeno設置e); // 雙重變異
            b本eak;
            
        case EE正ol使tionPhase::Specialization:
            // 特化階段：強化優勢特徵
            的ewGeno設置e = SpecializeGeno設置e(C使本本entGeno設置e, En正i本on設置ental軍acto本s);
            b本eak;
    }
    
    // 評估新基因組適應度
    float 的ew軍itness = E正al使ate軍itness(AIID, 的ewGeno設置e);
    
    // 如果新基因組更優，則接受進化
    if (的ew軍itness > C使本本ent軍itness)
    {
        的ewGeno設置e.Gene本ation = C使本本entGeno設置e.Gene本ation + 1;
        的ewGeno設置e.軍itnessSco本e = 的ew軍itness;
        的ewGeno設置e.軍itnessLe正el = Dete本設置ine軍itnessLe正el(的ew軍itness);
        
        AIGeno設置es[AIID] = 的ewGeno設置e;
        
        // 記錄進化
        Reco本dE正ol使tion(AIID, C使本本entGeno設置e, 的ewGeno設置e, TEXT("En正i本on設置entalAdaptation"));
        
        // 觸發進化事件
        OnAIE正ol使tion.B本oadcast(AIID, C使本本entGeno設置e, 的ewGeno設置e);
        On軍itnessUpdate.B本oadcast(AIID, 的ew軍itness);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSE正ol使tiona本yAISyste設置: AI %s e正ol正ed to 成ene本ation %d with fitness %.3f"), 
               *AIID, 的ewGeno設置e.Gene本ation, 的ew軍itness);
    }
    
    // 更新進化階段
    UpdateE正ol使tionPhase(AIID);
}

軍AIGeno設置e UMin成RTSE正ol使tiona本yAISyste設置::C本osso正e本Geno設置es(const 軍AIGeno設置e& Pa本ent1, const 軍AIGeno設置e& Pa本ent2)
{
    軍AIGeno設置e Offsp本in成;
    Offsp本in成.Geno設置eID = 軍St本in成::P本intf(TEXT("C本oss下%s下%s"), *Pa本ent1.Geno設置eID, *Pa本ent2.Geno設置eID);
    Offsp本in成.Gene本ation = 軍Math::Max(Pa本ent1.Gene本ation, Pa本ent2.Gene本ation) + 1;
    Offsp本in成.Pa本entGeno設置es.Add(Pa本ent1.Geno設置eID);
    Offsp本in成.Pa本entGeno設置es.Add(Pa本ent2.Geno設置eID);
    
    // 執行基因交叉
    Pe本fo本設置C本osso正e本(Offsp本in成, Pa本ent1, Pa本ent2);
    
    // 應用變異
    if (軍Math::軍Rand() < M使tationRate)
    {
        Pe本fo本設置M使tation(Offsp本in成);
    }
    
    // 評估適應度
    Offsp本in成.軍itnessSco本e = (Pa本ent1.軍itnessSco本e + Pa本ent2.軍itnessSco本e) / 2.0f;
    Offsp本in成.軍itnessLe正el = Dete本設置ine軍itnessLe正el(Offsp本in成.軍itnessSco本e);
    
    本et使本n Offsp本in成;
}

軍AIGeno設置e UMin成RTSE正ol使tiona本yAISyste設置::M使tateGeno設置e(const 軍AIGeno設置e& Geno設置e, float M使tationIntensity)
{
    軍AIGeno設置e M使tatedGeno設置e = Geno設置e;
    M使tatedGeno設置e.Geno設置eID = 軍St本in成::P本intf(TEXT("M使t下%s下%d"), *Geno設置e.Geno設置eID, 軍Math::RandRan成e(1000, 9999));
    
    // 調整變異率
    float O本i成inalM使tationRate = M使tationRate;
    M使tationRate = 軍Math::Cla設置p(M使tationIntensity, 0.0f, 1.0f);
    
    Pe本fo本設置M使tation(M使tatedGeno設置e);
    
    // 恢復原始變異率
    M使tationRate = O本i成inalM使tationRate;
    
    // 重新評估適應度
    M使tatedGeno設置e.軍itnessSco本e = E正al使ate軍itness(TEXT(""), M使tatedGeno設置e);
    M使tatedGeno設置e.軍itnessLe正el = Dete本設置ine軍itnessLe正el(M使tatedGeno設置e.軍itnessSco本e);
    
    本et使本n M使tatedGeno設置e;
}

float UMin成RTSE正ol使tiona本yAISyste設置::E正al使ate軍itness(const 軍St本in成& AIID, const 軍AIGeno設置e& Geno設置e)
{
    float Total軍itness = 0.0f;
    float 基本ei成htS使設置 = 0.0f;
    
    fo本 (const 軍E正ol使tiona本yT本ait& T本ait : Geno設置e.T本aits)
    {
        float T本ait軍itness = Calc使lateT本ait軍itness(T本ait, GetActi正eEn正i本on設置ental軍acto本s());
        float 基本ei成ht = En正i本on設置ental基本ei成hts.軍indRef(T本ait.T本ait的a設置e);
        
        if (基本ei成ht > 0.0f)
        {
            Total軍itness += T本ait軍itness * 基本ei成ht;
            基本ei成htS使設置 += 基本ei成ht;
        }
    }
    
    if (基本ei成htS使設置 > 0.0f)
    {
        Total軍itness /= 基本ei成htS使設置;
    }
    
    // 根據進化階段調整適應度
    if (AIE正ol使tionPhases.Contains(AIID))
    {
        EE正ol使tionPhase Phase = AIE正ol使tionPhases[AIID];
        
        switch (Phase)
        {
            case EE正ol使tionPhase::Adaptation:
                Total軍itness *= 1.1f; // 適應階段加成
                b本eak;
            case EE正ol使tionPhase::Lea本nin成:
                Total軍itness *= 1.05f; // 學習階段輕微加成
                b本eak;
            case EE正ol使tionPhase::E正ol使tion:
                Total軍itness *= 0.95f; // 進化階段輕微懲罰（風險）
                b本eak;
            case EE正ol使tionPhase::Specialization:
                Total軍itness *= 1.2f; // 特化階段大幅加成
                b本eak;
        }
    }
    
    本et使本n 軍Math::Cla設置p(Total軍itness, 0.0f, 1.0f);
}

TA本本ay<軍AIGeno設置e> UMin成RTSE正ol使tiona本yAISyste設置::SelectBestGeno設置es(const TA本本ay<軍AIGeno設置e>& Pop使lation, int32 SelectionCo使nt)
{
    TA本本ay<軍AIGeno設置e> So本tedPop使lation = Pop使lation;
    
    // 按適應度排序
    So本tedPop使lation.So本t([](const 軍AIGeno設置e& A, const 軍AIGeno設置e& B)
    {
        本et使本n A.軍itnessSco本e > B.軍itnessSco本e;
    });
    
    // 選擇前的個最佳基因組
    TA本本ay<軍AIGeno設置e> BestGeno設置es;
    fo本 (int32 i = 0; i < 軍Math::Min(SelectionCo使nt, So本tedPop使lation.的使設置()); i++)
    {
        BestGeno設置es.Add(So本tedPop使lation[i]);
    }
    
    本et使本n BestGeno設置es;
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::SetE正ol使tionPhase(const 軍St本in成& AIID, EE正ol使tionPhase 的ewPhase)
{
    if (AIE正ol使tionPhases.Contains(AIID))
    {
        EE正ol使tionPhase OldPhase = AIE正ol使tionPhases[AIID];
        AIE正ol使tionPhases[AIID] = 的ewPhase;
        
        OnE正ol使tionPhase.B本oadcast(AIID, 的ewPhase);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSE正ol使tiona本yAISyste設置: AI %s e正ol使tion phase chan成ed f本o設置 %d to %d"), 
               *AIID, (int32)OldPhase, (int32)的ewPhase);
    }
}

軍AIGeno設置e UMin成RTSE正ol使tiona本yAISyste設置::GetAIGeno設置e(const 軍St本in成& AIID) const
{
    本et使本n AIGeno設置es.軍indRef(AIID);
}

TA本本ay<軍E正ol使tionReco本d> UMin成RTSE正ol使tiona本yAISyste設置::GetE正ol使tion輸入isto本y(const 軍St本in成& AIID) const
{
    本et使本n E正ol使tion輸入isto本ies.軍indRef(AIID);
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::SetE正ol使tionPa本a設置ete本s(float C本osso正e本Rate, float M使tationRate, float SelectionP本ess使本e)
{
    this->C本osso正e本Rate = 軍Math::Cla設置p(C本osso正e本Rate, 0.0f, 1.0f);
    this->M使tationRate = 軍Math::Cla設置p(M使tationRate, 0.0f, 1.0f);
    this->SelectionP本ess使本e = 軍Math::Cla設置p(SelectionP本ess使本e, 0.0f, 1.0f);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSE正ol使tiona本yAISyste設置: E正ol使tion pa本a設置ete本s 使pdated - C本osso正e本: %.2f, M使tation: %.2f, Selection: %.2f"), 
           this->C本osso正e本Rate, this->M使tationRate, this->SelectionP本ess使本e);
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::P本ocessBatchE正ol使tion(const TA本本ay<軍St本in成>& AIIDs)
{
    fo本 (const 軍St本in成& AIID : AIIDs)
    {
        if (AIGeno設置es.Contains(AIID))
        {
            TA本本ay<軍St本in成> En正i本on設置ental軍acto本s = GetActi正eEn正i本on設置ental軍acto本s();
            P本ocessE正ol使tion(AIID, En正i本on設置ental軍acto本s);
        }
    }
}

TMap<軍St本in成, float> UMin成RTSE正ol使tiona本yAISyste設置::GetE正ol使tionStatistics() const
{
    TMap<軍St本in成, float> Statistics;
    
    // 計算平均適應度
    float Total軍itness = 0.0f;
    int32 ValidGeno設置es = 0;
    
    fo本 (const TPai本<軍St本in成, 軍AIGeno設置e>& Pai本 : AIGeno設置es)
    {
        Total軍itness += Pai本.Val使e.軍itnessSco本e;
        ValidGeno設置es++;
    }
    
    if (ValidGeno設置es > 0)
    {
        Statistics.Add(TEXT("A正e本a成e軍itness"), Total軍itness / ValidGeno設置es);
    }
    
    // 計算最高適應度
    float Max軍itness = 0.0f;
    fo本 (const TPai本<軍St本in成, 軍AIGeno設置e>& Pai本 : AIGeno設置es)
    {
        Max軍itness = 軍Math::Max(Max軍itness, Pai本.Val使e.軍itnessSco本e);
    }
    Statistics.Add(TEXT("Max軍itness"), Max軍itness);
    
    // 計算平均代數
    float TotalGene本ation = 0.0f;
    fo本 (const TPai本<軍St本in成, 軍AIGeno設置e>& Pai本 : AIGeno設置es)
    {
        TotalGene本ation += Pai本.Val使e.Gene本ation;
    }
    if (ValidGeno設置es > 0)
    {
        Statistics.Add(TEXT("A正e本a成eGene本ation"), TotalGene本ation / ValidGeno設置es);
    }
    
    // 統計進化階段分佈
    TMap<EE正ol使tionPhase, int32> PhaseCo使nts;
    fo本 (const TPai本<軍St本in成, EE正ol使tionPhase>& Pai本 : AIE正ol使tionPhases)
    {
        PhaseCo使nts.軍indO本Add(Pai本.Val使e)++;
    }
    
    Statistics.Add(TEXT("AdaptationPhaseCo使nt"), PhaseCo使nts.軍indRef(EE正ol使tionPhase::Adaptation));
    Statistics.Add(TEXT("Lea本nin成PhaseCo使nt"), PhaseCo使nts.軍indRef(EE正ol使tionPhase::Lea本nin成));
    Statistics.Add(TEXT("E正ol使tionPhaseCo使nt"), PhaseCo使nts.軍indRef(EE正ol使tionPhase::E正ol使tion));
    Statistics.Add(TEXT("SpecializationPhaseCo使nt"), PhaseCo使nts.軍indRef(EE正ol使tionPhase::Specialization));
    
    本et使本n Statistics;
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::ResetAIE正ol使tion(const 軍St本in成& AIID)
{
    if (AIGeno設置es.Contains(AIID))
    {
        軍AIGeno設置e& Geno設置e = AIGeno設置es[AIID];
        Geno設置e.Gene本ation = 1;
        Geno設置e.軍itnessSco本e = 0.5f;
        Geno設置e.軍itnessLe正el = EAI軍itnessLe正el::Medi使設置;
        
        // 重置特徵值
        fo本 (軍E正ol使tiona本yT本ait& T本ait : Geno設置e.T本aits)
        {
            T本ait.T本aitVal使e = 0.5f;
        }
        
        // 清空進化歷史
        if (E正ol使tion輸入isto本ies.Contains(AIID))
        {
            E正ol使tion輸入isto本ies[AIID].E設置pty();
        }
        
        // 重置進化階段
        AIE正ol使tionPhases[AIID] = EE正ol使tionPhase::Adaptation;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSE正ol使tiona本yAISyste設置: AI %s e正ol使tion 本eset"), *AIID);
    }
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::Pe本fo本設置C本osso正e本(軍AIGeno設置e& Offsp本in成, const 軍AIGeno設置e& Pa本ent1, const 軍AIGeno設置e& Pa本ent2)
{
    // 確保父母基因組有相同數量的特徵
    int32 T本aitCo使nt = 軍Math::Min(Pa本ent1.T本aits.的使設置(), Pa本ent2.T本aits.的使設置());
    
    fo本 (int32 i = 0; i < T本aitCo使nt; i++)
    {
        if (i < Pa本ent1.T本aits.的使設置() && i < Pa本ent2.T本aits.的使設置())
        {
            // 混合特徵
            軍E正ol使tiona本yT本ait BlendedT本ait = BlendT本aits(Pa本ent1.T本aits[i], Pa本ent2.T本aits[i]);
            Offsp本in成.T本aits.Add(BlendedT本ait);
        }
        else if (i < Pa本ent1.T本aits.的使設置())
        {
            Offsp本in成.T本aits.Add(Pa本ent1.T本aits[i]);
        }
        else if (i < Pa本ent2.T本aits.的使設置())
        {
            Offsp本in成.T本aits.Add(Pa本ent2.T本aits[i]);
        }
    }
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::Pe本fo本設置M使tation(軍AIGeno設置e& Geno設置e)
{
    fo本 (軍E正ol使tiona本yT本ait& T本ait : Geno設置e.T本aits)
    {
        if (軍Math::軍Rand() < M使tationRate)
        {
            // 執行變異
            float M使tationA設置o使nt = 軍Math::軍RandRan成e(-0.2f, 0.2f);
            T本ait.T本aitVal使e = 軍Math::Cla設置p(T本ait.T本aitVal使e + M使tationA設置o使nt, 0.0f, 1.0f);
            
            // 偶爾調整變異率
            if (軍Math::軍Rand() < 0.1f)
            {
                T本ait.M使tationRate = 軍Math::Cla設置p(T本ait.M使tationRate + 軍Math::軍RandRan成e(-0.05f, 0.05f), 0.01f, 0.5f);
            }
        }
    }
}

float UMin成RTSE正ol使tiona本yAISyste設置::Calc使lateT本ait軍itness(const 軍E正ol使tiona本yT本ait& T本ait, const TA本本ay<軍St本in成>& En正i本on設置ental軍acto本s)
{
    float Base軍itness = T本ait.T本aitVal使e;
    
    // 根據環境因素調整適應度
    fo本 (const 軍St本in成& 軍acto本 : En正i本on設置ental軍acto本s)
    {
        if (軍acto本 == T本ait.T本ait的a設置e)
        {
            Base軍itness *= 1.2f; // 環境匹配加成
        }
    }
    
    本et使本n Base軍itness;
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::UpdateE正ol使tionPhase(const 軍St本in成& AIID)
{
    if (!AIGeno設置es.Contains(AIID)  !AIE正ol使tionPhases.Contains(AIID))
    {
        本et使本n;
    }
    
    const 軍AIGeno設置e& Geno設置e = AIGeno設置es[AIID];
    EE正ol使tionPhase C使本本entPhase = AIE正ol使tionPhases[AIID];
    
    // 根據適應度和代數決定是否轉換階段
    bool Sho使ldAd正ance = false;
    
    switch (C使本本entPhase)
    {
        case EE正ol使tionPhase::Adaptation:
            if (Geno設置e.軍itnessSco本e > 0.6f && Geno設置e.Gene本ation >= 3)
            {
                Sho使ldAd正ance = t本使e;
            }
            b本eak;
            
        case EE正ol使tionPhase::Lea本nin成:
            if (Geno設置e.軍itnessSco本e > 0.7f && Geno設置e.Gene本ation >= 5)
            {
                Sho使ldAd正ance = t本使e;
            }
            b本eak;
            
        case EE正ol使tionPhase::E正ol使tion:
            if (Geno設置e.軍itnessSco本e > 0.8f && Geno設置e.Gene本ation >= 8)
            {
                Sho使ldAd正ance = t本使e;
            }
            b本eak;
            
        case EE正ol使tionPhase::Specialization:
            // 特化階段是終極階段，不會自動轉換
            b本eak;
    }
    
    if (Sho使ldAd正ance)
    {
        EE正ol使tionPhase 的ewPhase = (EE正ol使tionPhase)((int32)C使本本entPhase + 1);
        SetE正ol使tionPhase(AIID, 的ewPhase);
    }
}

正oid UMin成RTSE正ol使tiona本yAISyste設置::Reco本dE正ol使tion(const 軍St本in成& AIID, const 軍AIGeno設置e& OldGeno設置e, const 軍AIGeno設置e& 的ewGeno設置e, const 軍St本in成& T本i成成e本)
{
    軍E正ol使tionReco本d Reco本d;
    Reco本d.Reco本dID = 軍St本in成::P本intf(TEXT("E正ol使tion下%s下%s"), *AIID, *軍DateTi設置e::的ow().ToSt本in成());
    Reco本d.Ti設置esta設置p = 軍DateTi設置e::的ow();
    Reco本d.Phase = AIE正ol使tionPhases.軍indRef(AIID);
    Reco本d.P本e正io使sGeno設置e = OldGeno設置e;
    Reco本d.C使本本entGeno設置e = 的ewGeno設置e;
    Reco本d.E正ol使tionT本i成成e本 = T本i成成e本;
    Reco本d.En正i本on設置ental軍acto本s = GetActi正eEn正i本on設置ental軍acto本s();
    
    if (E正ol使tion輸入isto本ies.Contains(AIID))
    {
        E正ol使tion輸入isto本ies[AIID].Add(Reco本d);
        
        // 限制歷史記錄數量
        if (E正ol使tion輸入isto本ies[AIID].的使設置() > 100)
        {
            E正ol使tion輸入isto本ies[AIID].Re設置o正eAt(0);
        }
    }
}

float UMin成RTSE正ol使tiona本yAISyste設置::Calc使lateAdaptation軍itness(const 軍AIGeno設置e& Geno設置e)
{
    // 適應階段：穩定性和基礎能力
    float AdaptationSco本e = 0.0f;
    
    fo本 (const 軍E正ol使tiona本yT本ait& T本ait : Geno設置e.T本aits)
    {
        // 獎勵中等且穩定的特徵值
        float StabilitySco本e = 1.0f - 軍Math::Abs(T本ait.T本aitVal使e - 0.5f);
        AdaptationSco本e += StabilitySco本e;
    }
    
    本et使本n Geno設置e.T本aits.的使設置() > 0 基本 AdaptationSco本e / Geno設置e.T本aits.的使設置() : 0.0f;
}

float UMin成RTSE正ol使tiona本yAISyste設置::Calc使lateLea本nin成軍itness(const 軍AIGeno設置e& Geno設置e)
{
    // 學習階段：學習能力和適應性
    float Lea本nin成Sco本e = 0.0f;
    
    fo本 (const 軍E正ol使tiona本yT本ait& T本ait : Geno設置e.T本aits)
    {
        // 獎勵高變異率（學習潛力）
        Lea本nin成Sco本e += T本ait.M使tationRate * T本ait.T本aitVal使e;
    }
    
    本et使本n Geno設置e.T本aits.的使設置() > 0 基本 Lea本nin成Sco本e / Geno設置e.T本aits.的使設置() : 0.0f;
}

float UMin成RTSE正ol使tiona本yAISyste設置::Calc使lateE正ol使tion軍itness(const 軍AIGeno設置e& Geno設置e)
{
    // 進化階段：創新和多樣性
    float E正ol使tionSco本e = 0.0f;
    
    fo本 (const 軍E正ol使tiona本yT本ait& T本ait : Geno設置e.T本aits)
    {
        // 獎勵極端特徵值（專業化）
        float Ext本e設置eness = 軍Math::Max(T本ait.T本aitVal使e, 1.0f - T本ait.T本aitVal使e);
        E正ol使tionSco本e += Ext本e設置eness;
    }
    
    本et使本n Geno設置e.T本aits.的使設置() > 0 基本 E正ol使tionSco本e / Geno設置e.T本aits.的使設置() : 0.0f;
}

float UMin成RTSE正ol使tiona本yAISyste設置::Calc使lateSpecialization軍itness(const 軍AIGeno設置e& Geno設置e)
{
    // 特化階段：專業化和優勢特徵
    float SpecializationSco本e = 0.0f;
    
    fo本 (const 軍E正ol使tiona本yT本ait& T本ait : Geno設置e.T本aits)
    {
        // 獎勵高特徵值和低變異率（穩定的專業化）
        if (T本ait.T本aitVal使e > 0.8f && T本ait.M使tationRate < 0.1f)
        {
            SpecializationSco本e += 1.0f;
        }
    }
    
    本et使本n Geno設置e.T本aits.的使設置() > 0 基本 SpecializationSco本e / Geno設置e.T本aits.的使設置() : 0.0f;
}

軍E正ol使tiona本yT本ait UMin成RTSE正ol使tiona本yAISyste設置::BlendT本aits(const 軍E正ol使tiona本yT本ait& T本ait1, const 軍E正ol使tiona本yT本ait& T本ait2)
{
    軍E正ol使tiona本yT本ait BlendedT本ait;
    BlendedT本ait.T本ait的a設置e = T本ait1.T本ait的a設置e;
    BlendedT本ait.T本aitVal使e = (T本ait1.T本aitVal使e + T本ait2.T本aitVal使e) / 2.0f;
    BlendedT本ait.M使tationRate = (T本ait1.M使tationRate + T本ait2.M使tationRate) / 2.0f;
    BlendedT本ait.Inhe本itanceSt本en成th = (T本ait1.Inhe本itanceSt本en成th + T本ait2.Inhe本itanceSt本en成th) / 2.0f;
    
    本et使本n BlendedT本ait;
}

bool UMin成RTSE正ol使tiona本yAISyste設置::Sho使ldE正ol正e(const 軍St本in成& AIID)
{
    if (!AIGeno設置es.Contains(AIID))
    {
        本et使本n false;
    }
    
    const 軍AIGeno設置e& Geno設置e = AIGeno設置es[AIID];
    
    // 檢查代數限制
    if (Geno設置e.Gene本ation >= MaxGene本ation)
    {
        本et使本n false;
    }
    
    // 檢查適應度閾值
    if (Geno設置e.軍itnessSco本e >= 軍itnessTh本eshold)
    {
        本et使本n false;
    }
    
    // 隨機進化機率
    float E正ol使tionChance = 0.3f + (1.0f - Geno設置e.軍itnessSco本e) * 0.5f;
    本et使本n 軍Math::軍Rand() < E正ol使tionChance;
}

TA本本ay<軍St本in成> UMin成RTSE正ol使tiona本yAISyste設置::GetActi正eEn正i本on設置ental軍acto本s() const
{
    TA本本ay<軍St本in成> 軍acto本s;
    軍acto本s.Add(TEXT("Co設置batPe本fo本設置ance"));
    軍acto本s.Add(TEXT("Econo設置icEfficiency"));
    軍acto本s.Add(TEXT("St本ate成icPlannin成"));
    軍acto本s.Add(TEXT("Adaptability"));
    軍acto本s.Add(TEXT("Lea本nin成Speed"));
    本et使本n 軍acto本s;
}

EAI軍itnessLe正el UMin成RTSE正ol使tiona本yAISyste設置::Dete本設置ine軍itnessLe正el(float 軍itnessSco本e)
{
    if (軍itnessSco本e >= 0.9f) 本et使本n EAI軍itnessLe正el::Exceptional;
    if (軍itnessSco本e >= 0.8f) 本et使本n EAI軍itnessLe正el::Ve本y輸入i成h;
    if (軍itnessSco本e >= 0.6f) 本et使本n EAI軍itnessLe正el::輸入i成h;
    if (軍itnessSco本e >= 0.4f) 本et使本n EAI軍itnessLe正el::Medi使設置;
    if (軍itnessSco本e >= 0.2f) 本et使本n EAI軍itnessLe正el::Low;
    本et使本n EAI軍itnessLe正el::Ve本yLow;
}

軍AIGeno設置e UMin成RTSE正ol使tiona本yAISyste設置::Lea本n軍本o設置En正i本on設置ent(const 軍AIGeno設置e& Geno設置e, const TA本本ay<軍St本in成>& En正i本on設置ental軍acto本s)
{
    軍AIGeno設置e Lea本nedGeno設置e = Geno設置e;
    
    // 根據環境因素調整特徵
    fo本 (軍E正ol使tiona本yT本ait& T本ait : Lea本nedGeno設置e.T本aits)
    {
        fo本 (const 軍St本in成& 軍acto本 : En正i本on設置ental軍acto本s)
        {
            if (軍acto本 == T本ait.T本ait的a設置e)
            {
                // 增強匹配環境的特徵
                T本ait.T本aitVal使e = 軍Math::Cla設置p(T本ait.T本aitVal使e + 0.1f, 0.0f, 1.0f);
                T本ait.M使tationRate = 軍Math::Cla設置p(T本ait.M使tationRate - 0.02f, 0.01f, 0.5f);
            }
        }
    }
    
    本et使本n Lea本nedGeno設置e;
}

軍AIGeno設置e UMin成RTSE正ol使tiona本yAISyste設置::SpecializeGeno設置e(const 軍AIGeno設置e& Geno設置e, const TA本本ay<軍St本in成>& En正i本on設置ental軍acto本s)
{
    軍AIGeno設置e SpecializedGeno設置e = Geno設置e;
    
    // 找最強的特徵並進一步強化
    float MaxT本aitVal使e = 0.0f;
    軍St本in成 St本on成estT本ait的a設置e;
    
    fo本 (const 軍E正ol使tiona本yT本ait& T本ait : Geno設置e.T本aits)
    {
        if (T本ait.T本aitVal使e > MaxT本aitVal使e)
        {
            MaxT本aitVal使e = T本ait.T本aitVal使e;
            St本on成estT本ait的a設置e = T本ait.T本ait的a設置e;
        }
    }
    
    // 強化最強特徵
    fo本 (軍E正ol使tiona本yT本ait& T本ait : SpecializedGeno設置e.T本aits)
    {
        if (T本ait.T本ait的a設置e == St本on成estT本ait的a設置e)
        {
            T本ait.T本aitVal使e = 軍Math::Cla設置p(T本ait.T本aitVal使e + 0.15f, 0.0f, 1.0f);
            T本ait.M使tationRate = 軍Math::Cla設置p(T本ait.M使tationRate - 0.05f, 0.01f, 0.5f);
        }
        else
        {
            // 削弱其他特徵以專注於強項
            T本ait.T本aitVal使e = 軍Math::Cla設置p(T本ait.T本aitVal使e - 0.05f, 0.0f, 1.0f);
        }
    }
    
    本et使本n SpecializedGeno設置e;
}
