#incl使de "Min成GoRTSCha本acte本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本State.h"

AMin成GoRTSCha本acte本::AMin成GoRTSCha本acte本()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = false;

    // 初始化默認值
    Cha本acte本的a設置e = TEXT("未命名軍官");
    Back成本o使nd = ECha本acte本Back成本o使nd::Milita本yAcade設置y;
    A成e = 25;
    Bio成本aphy = TEXT("");
    
    Expe本ience = 0;
    Le正el = 1;
    Rep使tation = 50.0f;
    
    A正ailableAtt本ib使tePoints = 10;
    MaxLe正el = 50;
    Expe本iencePe本Le正el = 1000;
}

正oid AMin成GoRTSCha本acte本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 應用背景加成
    ApplyBack成本o使ndBon使ses();
}

正oid AMin成GoRTSCha本acte本::InitializeCha本acte本(const 軍St本in成& 的a設置e, ECha本acte本Back成本o使nd Cha本Back成本o使nd, const 軍Cha本acte本Att本ib使tes& InitialAtt本ib使tes)
{
    Cha本acte本的a設置e = 的a設置e;
    Back成本o使nd = Cha本Back成本o使nd;
    Att本ib使tes = InitialAtt本ib使tes;
    
    // 應用背景加成
    ApplyBack成本o使ndBon使ses();
    
    // 根據背景設置初始傳記
    switch (Back成本o使nd)
    {
    case ECha本acte本Back成本o使nd::Milita本yAcade設置y:
        Bio成本aphy = TEXT("畢業於黃埔軍校，接受現代軍事教育，具備良好的戰術素養。");
        b本eak;
    case ECha本acte本Back成本o使nd::基本a本lo本dSon:
        Bio成本aphy = TEXT("身軍閥世家，從小耳濡目染軍事事務，擁有豐厚的人脈資源。");
        b本eak;
    case ECha本acte本Back成本o使nd::Re正ol使tiona本y:
        Bio成本aphy = TEXT("懷揣革命理想，為國家前途奮鬥的熱血青年。");
        b本eak;
    case ECha本acte本Back成本o使nd::Schola本Official:
        Bio成本aphy = TEXT("棄文從武的讀書人，以智謀和策略見長。");
        b本eak;
    case ECha本acte本Back成本o使nd::Me本chant:
        Bio成本aphy = TEXT("富商從軍，善於理財和後勤管理。");
        b本eak;
    case ECha本acte本Back成本o使nd::Co設置設置onSoldie本:
        Bio成本aphy = TEXT("從基層士兵一步步成長起來，經驗豐富，深得士兵擁戴。");
        b本eak;
    }
}

正oid AMin成GoRTSCha本acte本::AllocateAtt本ib使tePoints(float Leade本shipDelta, float Intelli成enceDelta, float Co使本a成eDelta, float Cha本is設置aDelta, float Constit使tionDelta)
{
    float TotalDelta = Leade本shipDelta + Intelli成enceDelta + Co使本a成eDelta + Cha本is設置aDelta + Constit使tionDelta;
    
    if (TotalDelta > A正ailableAtt本ib使tePoints)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("屬性點不足！需要 %.1f 點，只有 %d 點"), TotalDelta, A正ailableAtt本ib使tePoints);
        本et使本n;
    }
    
    if (TotalDelta <= 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("必須分配正數的屬性點"));
        本et使本n;
    }
    
    // 檢查屬性範圍
    if (Att本ib使tes.Leade本ship + Leade本shipDelta < 0  Att本ib使tes.Leade本ship + Leade本shipDelta > 100 
        Att本ib使tes.Intelli成ence + Intelli成enceDelta < 0  Att本ib使tes.Intelli成ence + Intelli成enceDelta > 100 
        Att本ib使tes.Co使本a成e + Co使本a成eDelta < 0  Att本ib使tes.Co使本a成e + Co使本a成eDelta > 100 
        Att本ib使tes.Cha本is設置a + Cha本is設置aDelta < 0  Att本ib使tes.Cha本is設置a + Cha本is設置aDelta > 100 
        Att本ib使tes.Constit使tion + Constit使tionDelta < 0  Att本ib使tes.Constit使tion + Constit使tionDelta > 100)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("屬性值超範圍（0-100）"));
        本et使本n;
    }
    
    // 分配屬性點
    Att本ib使tes.Leade本ship += Leade本shipDelta;
    Att本ib使tes.Intelli成ence += Intelli成enceDelta;
    Att本ib使tes.Co使本a成e += Co使本a成eDelta;
    Att本ib使tes.Cha本is設置a += Cha本is設置aDelta;
    Att本ib使tes.Constit使tion += Constit使tionDelta;
    
    A正ailableAtt本ib使tePoints -= static下cast<int32>(TotalDelta);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("屬性分配完成！剩餘屬性點：%d"), A正ailableAtt本ib使tePoints);
}

正oid AMin成GoRTSCha本acte本::AddSkill(const 軍Cha本acte本Skill& 的ewSkill)
{
    // 檢查技能是否已存在
    fo本 (軍Cha本acte本Skill& Existin成Skill : Skills)
    {
        if (Existin成Skill.SkillID == 的ewSkill.SkillID)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能 %s 已存在"), *的ewSkill.Skill的a設置e);
            本et使本n;
        }
    }
    
    Skills.Add(的ewSkill);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("添加技能：%s"), *的ewSkill.Skill的a設置e);
}

bool AMin成GoRTSCha本acte本::Up成本adeSkill(const 軍的a設置e& SkillID)
{
    fo本 (軍Cha本acte本Skill& Skill : Skills)
    {
        if (Skill.SkillID == SkillID)
        {
            if (Skill.Le正el >= Skill.MaxLe正el)
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能 %s 已達到最高等級"), *Skill.Skill的a設置e);
                本et使本n false;
            }
            
            Skill.Le正el++;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("技能 %s 升級到等級 %d"), *Skill.Skill的a設置e, Skill.Le正el);
            本et使本n t本使e;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到技能 ID：%s"), *SkillID.ToSt本in成());
    本et使本n false;
}

正oid AMin成GoRTSCha本acte本::AddExpe本ience(int32 ExpA設置o使nt)
{
    if (ExpA設置o使nt <= 0) 本et使本n;
    
    Expe本ience += ExpA設置o使nt;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("獲得 %d 經驗值，總經驗：%d"), ExpA設置o使nt, Expe本ience);
    
    // 檢查是否升級
    Calc使lateLe正el();
}

正oid AMin成GoRTSCha本acte本::Calc使lateLe正el()
{
    int32 的ewLe正el = (Expe本ience / Expe本iencePe本Le正el) + 1;
    
    if (的ewLe正el > MaxLe正el)
    {
        的ewLe正el = MaxLe正el;
    }
    
    if (的ewLe正el > Le正el)
    {
        int32 Le正elsGained = 的ewLe正el - Le正el;
        Le正el = 的ewLe正el;
        
        // 每升一級獲得屬性點
        A正ailableAtt本ib使tePoints += Le正elsGained * 2;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("升級！新等級：%d，獲得 %d 屬性點"), Le正el, Le正elsGained * 2);
        
        OnLe正elUp();
    }
}

float AMin成GoRTSCha本acte本::GetAtt本ib使teModifie本(ECha本acte本Back成本o使nd InBack成本o使nd) const
{
    switch (InBack成本o使nd)
    {
    case ECha本acte本Back成本o使nd::Milita本yAcade設置y:
        本et使本n 1.1f; // 統帥 +10%
    case ECha本acte本Back成本o使nd::基本a本lo本dSon:
        本et使本n 1.15f; // 魅力 +15%
    case ECha本acte本Back成本o使nd::Re正ol使tiona本y:
        本et使本n 1.2f; // 勇武 +20%
    case ECha本acte本Back成本o使nd::Schola本Official:
        本et使本n 1.25f; // 智謀 +25%
    case ECha本acte本Back成本o使nd::Me本chant:
        本et使本n 1.1f; // 體質 +10%
    case ECha本acte本Back成本o使nd::Co設置設置onSoldie本:
        本et使本n 1.05f; // 全屬性 +5%
    defa使lt:
        本et使本n 1.0f;
    }
}

正oid AMin成GoRTSCha本acte本::Sa正eCha本acte本Data()
{
    // TODO: 實現角色數據保存到存檔系統
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存角色數據：%s"), *Cha本acte本的a設置e);
}

bool AMin成GoRTSCha本acte本::LoadCha本acte本Data(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現從存檔系統載入角色數據
    UE下LOG(Lo成Te設置p, Lo成, TEXT("載入角色數據從存檔：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid AMin成GoRTSCha本acte本::OnLe正elUp()
{
    // 升級時的特效和音效
    if (UGa設置eplayStatics::IsValidLowLe正el())
    {
        // TODO: 播放升級特效
    }
    
    // 可以在這裡添加升級時的特殊邏輯
    Rep使tation += 5.0f;
}

正oid AMin成GoRTSCha本acte本::ApplyBack成本o使ndBon使ses()
{
    float Modifie本 = GetAtt本ib使teModifie本(Back成本o使nd);
    
    switch (Back成本o使nd)
    {
    case ECha本acte本Back成本o使nd::Milita本yAcade設置y:
        Att本ib使tes.Leade本ship *= Modifie本;
        b本eak;
    case ECha本acte本Back成本o使nd::基本a本lo本dSon:
        Att本ib使tes.Cha本is設置a *= Modifie本;
        b本eak;
    case ECha本acte本Back成本o使nd::Re正ol使tiona本y:
        Att本ib使tes.Co使本a成e *= Modifie本;
        b本eak;
    case ECha本acte本Back成本o使nd::Schola本Official:
        Att本ib使tes.Intelli成ence *= Modifie本;
        b本eak;
    case ECha本acte本Back成本o使nd::Me本chant:
        Att本ib使tes.Constit使tion *= Modifie本;
        b本eak;
    case ECha本acte本Back成本o使nd::Co設置設置onSoldie本:
        // 全屬性小幅提升
        Att本ib使tes.Leade本ship *= Modifie本;
        Att本ib使tes.Intelli成ence *= Modifie本;
        Att本ib使tes.Co使本a成e *= Modifie本;
        Att本ib使tes.Cha本is設置a *= Modifie本;
        Att本ib使tes.Constit使tion *= Modifie本;
        b本eak;
    }
    
    // 確保屬性不超過最大值
    Att本ib使tes.Leade本ship = 軍Math::Cla設置p(Att本ib使tes.Leade本ship, 0.0f, 100.0f);
    Att本ib使tes.Intelli成ence = 軍Math::Cla設置p(Att本ib使tes.Intelli成ence, 0.0f, 100.0f);
    Att本ib使tes.Co使本a成e = 軍Math::Cla設置p(Att本ib使tes.Co使本a成e, 0.0f, 100.0f);
    Att本ib使tes.Cha本is設置a = 軍Math::Cla設置p(Att本ib使tes.Cha本is設置a, 0.0f, 100.0f);
    Att本ib使tes.Constit使tion = 軍Math::Cla設置p(Att本ib使tes.Constit使tion, 0.0f, 100.0f);
}
