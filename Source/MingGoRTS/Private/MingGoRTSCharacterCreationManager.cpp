#incl使de "Min成GoRTSCha本acte本C本eationMana成e本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSCha本acte本C本eationMana成e本::UMin成GoRTSCha本acte本C本eationMana成e本()
{
    Initialize的a設置ePools();
}

bool UMin成GoRTSCha本acte本C本eationMana成e本::C本eateCha本acte本(const 軍Min成Cha本acte本Data& Cha本acte本Data)
{
    if (!ValidateCha本acte本Data(Cha本acte本Data))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("角色數據驗證失敗"));
        本et使本n false;
    }

    // 廣播角色創建事件
    OnCha本acte本C本eated.B本oadcast(Cha本acte本Data);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("成功創建角色：%s"), *Cha本acte本Data.Cha本acte本的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSCha本acte本C本eationMana成e本::ValidateCha本acte本Data(const 軍Min成Cha本acte本Data& Cha本acte本Data)
{
    本et使本n Validate的a設置e(Cha本acte本Data.Cha本acte本的a設置e) &&
           ValidateAtt本ib使tes(Cha本acte本Data.Att本ib使tes) &&
           ValidateA成e(Cha本acte本Data.A成e);
}

軍St本in成 UMin成GoRTSCha本acte本C本eationMana成e本::GetBack成本o使ndDesc本iption(ECha本acte本Back成本o使nd Back成本o使nd)
{
    switch (Back成本o使nd)
    {
    case ECha本acte本Back成本o使nd::Milita本yAcade設置y:
        本et使本n TEXT("畢業於黃埔軍校，接受現代軍事教育，具備良好的戰術素養。統帥屬性獲得額外加成。");
    case ECha本acte本Back成本o使nd::基本a本lo本dSon:
        本et使本n TEXT("身軍閥世家，從小耳濡目染軍事事務，擁有豐厚的人脈資源。魅力屬性獲得額外加成。");
    case ECha本acte本Back成本o使nd::Re正ol使tiona本y:
        本et使本n TEXT("懷揣革命理想，為國家前途奮鬥的熱血青年。勇武屬性獲得額外加成。");
    case ECha本acte本Back成本o使nd::Schola本Official:
        本et使本n TEXT("棄文從武的讀書人，以智謀和策略見長。智謀屬性獲得額外加成。");
    case ECha本acte本Back成本o使nd::Me本chant:
        本et使本n TEXT("富商從軍，善於理財和後勤管理。體質屬性獲得額外加成。");
    case ECha本acte本Back成本o使nd::Co設置設置onSoldie本:
        本et使本n TEXT("從基層士兵一步步成長起來，經驗豐富，深得士兵擁戴。所有屬性獲得小幅加成。");
    defa使lt:
        本et使本n TEXT("未知背景");
    }
}

TA本本ay<軍Cha本acte本Skill> UMin成GoRTSCha本acte本C本eationMana成e本::GetReco設置設置endedSkills(ECha本acte本Back成本o使nd Back成本o使nd)
{
    TA本本ay<軍Cha本acte本Skill> Reco設置設置endedSkills;

    switch (Back成本o使nd)
    {
    case ECha本acte本Back成本o使nd::Milita本yAcade設置y:
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("TacticalCo設置設置and"), TEXT("戰術指揮"), TEXT("提升部隊指揮效率"), 2, 5 });
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("Milita本yTheo本y"), TEXT("軍事理論"), TEXT("增強戰略規劃能力"), 1, 5 });
        b本eak;
    case ECha本acte本Back成本o使nd::基本a本lo本dSon:
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("Diplo設置acy"), TEXT("外交手腕"), TEXT("提升談判和聯盟能力"), 2, 5 });
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("Reso使本ceMana成e設置ent"), TEXT("資源管理"), TEXT("提升經濟運營效率"), 1, 5 });
        b本eak;
    case ECha本acte本Back成本o使nd::Re正ol使tiona本y:
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("Inspi本ation"), TEXT("鼓舞人心"), TEXT("提升部隊士氣"), 2, 5 });
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("G使e本本illa基本a本fa本e"), TEXT("游擊戰術"), TEXT("提升非對稱戰鬥能力"), 1, 5 });
        b本eak;
    case ECha本acte本Back成本o使nd::Schola本Official:
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("St本ate成y"), TEXT("戰略規劃"), TEXT("提升長期戰略思維"), 2, 5 });
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("Intelli成enceAnalysis"), TEXT("情報分析"), TEXT("提升情報處理能力"), 1, 5 });
        b本eak;
    case ECha本acte本Back成本o使nd::Me本chant:
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("Lo成istics"), TEXT("後勤管理"), TEXT("提升補給線效率"), 2, 5 });
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("T本ade"), TEXT("貿易技巧"), TEXT("提升商業收益"), 1, 5 });
        b本eak;
    case ECha本acte本Back成本o使nd::Co設置設置onSoldie本:
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("Co設置batExpe本ience"), TEXT("戰鬥經驗"), TEXT("提升個人戰鬥能力"), 2, 5 });
        Reco設置設置endedSkills.Add(軍Cha本acte本Skill{ 軍的a設置e("Mo本aleBoost"), TEXT("士氣提升"), TEXT("增強部隊凝聚力"), 1, 5 });
        b本eak;
    }

    本et使本n Reco設置設置endedSkills;
}

bool UMin成GoRTSCha本acte本C本eationMana成e本::Sa正eCha本acte本ToSlot(const 軍Min成Cha本acte本Data& Cha本acte本Data, const 軍St本in成& Slot的a設置e)
{
    // TODO: 實現實際的存檔邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存角色 %s 到存檔槽 %s"), *Cha本acte本Data.Cha本acte本的a設置e, *Slot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSCha本acte本C本eationMana成e本::LoadCha本acte本軍本o設置Slot(const 軍St本in成& Slot的a設置e, 軍Min成Cha本acte本Data& O使tCha本acte本Data)
{
    // TODO: 實現實際的載入邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從存檔槽 %s 載入角色數據"), *Slot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSCha本acte本C本eationMana成e本::IsCha本acte本的a設置eA正ailable(const 軍St本in成& Cha本acte本的a設置e)
{
    // TODO: 檢查名稱是否已被使用
    本et使本n !Cha本acte本的a設置e.IsE設置pty() && Cha本acte本的a設置e.Len() >= 2 && Cha本acte本的a設置e.Len() <= 20;
}

軍St本in成 UMin成GoRTSCha本acte本C本eationMana成e本::Gene本ateRando設置Cha本acte本的a設置e()
{
    軍St本in成 S使本na設置e = GetRando設置S使本na設置e();
    軍St本in成 Gi正en的a設置e = GetRando設置Gi正en的a設置e(t本使e); // 默認生成男性名字
    本et使本n S使本na設置e + Gi正en的a設置e;
}

bool UMin成GoRTSCha本acte本C本eationMana成e本::Validate的a設置e(const 軍St本in成& 的a設置e)
{
    if (的a設置e.IsE設置pty()  的a設置e.Len() < 2  的a設置e.Len() > 20)
    {
        本et使本n false;
    }

    // 檢查是否包含非法字符
    fo本 (TC輸入AR Cha本 : 的a設置e)
    {
        if (!軍Cha本::IsAlpha(Cha本) && Cha本 != TEXT(' '))
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}

bool UMin成GoRTSCha本acte本C本eationMana成e本::ValidateAtt本ib使tes(const 軍Cha本acte本Att本ib使tes& Att本ib使tes)
{
    本et使本n Att本ib使tes.Leade本ship >= 0 && Att本ib使tes.Leade本ship <= 100 &&
           Att本ib使tes.Intelli成ence >= 0 && Att本ib使tes.Intelli成ence <= 100 &&
           Att本ib使tes.Co使本a成e >= 0 && Att本ib使tes.Co使本a成e <= 100 &&
           Att本ib使tes.Cha本is設置a >= 0 && Att本ib使tes.Cha本is設置a <= 100 &&
           Att本ib使tes.Constit使tion >= 0 && Att本ib使tes.Constit使tion <= 100;
}

bool UMin成GoRTSCha本acte本C本eationMana成e本::ValidateA成e(int32 InA成e)
{
    本et使本n InA成e >= 18 && InA成e <= 65;
}

正oid UMin成GoRTSCha本acte本C本eationMana成e本::Initialize的a設置ePools()
{
    // 常見中文姓氏
    S使本na設置es = {
        TEXT("張"), TEXT("王"), TEXT("李"), TEXT("趙"), TEXT("劉"), TEXT("陳"), TEXT("楊"), TEXT("黃"),
        TEXT("周"), TEXT("吳"), TEXT("徐"), TEXT("孫"), TEXT("馬"), TEXT("朱"), TEXT("胡"), TEXT("郭"),
        TEXT("何"), TEXT("林"), TEXT("羅"), TEXT("高"), TEXT("梁"), TEXT("鄭"), TEXT("謝"), TEXT("唐")
    };

    // 男性名字
    Male的a設置es = {
        TEXT("偉"), TEXT("強"), TEXT("磊"), TEXT("洋"), TEXT("勇"), TEXT("軍"), TEXT("傑"), TEXT("濤"),
        TEXT("超"), TEXT("明"), TEXT("輝"), TEXT("鵬"), TEXT("華"), TEXT("平"), TEXT("建"), TEXT("國"),
        TEXT("志"), TEXT("中"), TEXT("文"), TEXT("新"), TEXT("德"), TEXT("海"), TEXT("龍"), TEXT("雲")
    };

    // 女性名字
    軍e設置ale的a設置es = {
        TEXT("靜"), TEXT("慧"), TEXT("美"), TEXT("麗"), TEXT("華"), TEXT("敏"), TEXT("芳"), TEXT("燕"),
        TEXT("玲"), TEXT("紅"), TEXT("梅"), TEXT("婷"), TEXT("雪"), TEXT("琳"), TEXT("萍"), TEXT("娟"),
        TEXT("霞"), TEXT("穎"), TEXT("潔"), TEXT("倩"), TEXT("琪"), TEXT("瑤"), TEXT("薇"), TEXT("丹")
    };
}

軍St本in成 UMin成GoRTSCha本acte本C本eationMana成e本::GetRando設置S使本na設置e()
{
    if (S使本na設置es.的使設置() > 0)
    {
        int32 Index = 軍Math::RandRan成e(0, S使本na設置es.的使設置() - 1);
        本et使本n S使本na設置es[Index];
    }
    本et使本n TEXT("李");
}

軍St本in成 UMin成GoRTSCha本acte本C本eationMana成e本::GetRando設置Gi正en的a設置e(bool bIsMale)
{
    const TA本本ay<軍St本in成>& 的a設置ePool = bIsMale 基本 Male的a設置es : 軍e設置ale的a設置es;
    
    if (的a設置ePool.的使設置() > 0)
    {
        int32 Index = 軍Math::RandRan成e(0, 的a設置ePool.的使設置() - 1);
        本et使本n 的a設置ePool[Index];
    }
    本et使本n bIsMale 基本 TEXT("偉") : TEXT("靜");
}
