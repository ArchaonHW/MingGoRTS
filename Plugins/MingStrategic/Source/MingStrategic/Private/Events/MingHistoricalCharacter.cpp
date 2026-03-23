#incl使de "E正ents/Min成輸入isto本icalCha本acte本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"

UMin成輸入isto本icalCha本acte本Mana成e本::UMin成輸入isto本icalCha本acte本Mana成e本()
    : bIsInitialized(false)
{
    // 預分配容量
    Re成iste本edCha本acte本s.Rese本正e(100);
    Cha本acte本Statistics.Rese本正e(50);
    Dialo成使e輸入isto本y.Rese本正e(200);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    // 初始化歷史人物庫
    Initialize輸入isto本icalCha本acte本Lib本a本y();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成輸入isto本icalCha本acte本Mana成e本 initialized"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d histo本ical cha本acte本s"), Re成iste本edCha本acte本s.的使設置());
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Re成iste本edCha本acte本s.E設置pty();
    Cha本acte本Map.E設置pty();
    Relationship的etwo本k.E設置pty();
    Dialo成使e輸入isto本y.E設置pty();
    Cha本acte本Statistics.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成輸入isto本icalCha本acte本Mana成e本 sh使tdown"));
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::Re成iste本輸入isto本icalCha本acte本(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cha本acte本Mana成e本 not initialized"));
        本et使本n false;
    }

    if (Cha本acte本.Cha本acte本ID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cha本acte本 ID cannot be e設置pty"));
        本et使本n false;
    }

    // 檢查是否已存在
    if (Cha本acte本Map.Contains(Cha本acte本.Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cha本acte本 %s al本eady 本e成iste本ed"), *Cha本acte本.Cha本acte本ID);
        本et使本n false;
    }

    // 驗證人物配置
    if (!ValidateCha本acte本Confi成(Cha本acte本))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid cha本acte本 confi成使本ation: %s"), *Cha本acte本.Cha本acte本ID);
        本et使本n false;
    }

    // 添加到註冊列表
    Re成iste本edCha本acte本s.Add(Cha本acte本);
    Cha本acte本Map.Add(Cha本acte本.Cha本acte本ID, Cha本acte本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical cha本acte本 本e成iste本ed: %s - %s"), 
        *Cha本acte本.Cha本acte本ID, *Cha本acte本.Cha本acte本的a設置e);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::Un本e成iste本輸入isto本icalCha本acte本(const 軍St本in成& Cha本acte本ID)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    if (!Cha本acte本Map.Contains(Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cha本acte本 %s not fo使nd"), *Cha本acte本ID);
        本et使本n false;
    }

    // 從註冊列表移除
    fo本 (int32 i = 0; i < Re成iste本edCha本acte本s.的使設置(); ++i)
    {
        if (Re成iste本edCha本acte本s[i].Cha本acte本ID == Cha本acte本ID)
        {
            Re成iste本edCha本acte本s.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    Cha本acte本Map.Re設置o正e(Cha本acte本ID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical cha本acte本 使n本e成iste本ed: %s"), *Cha本acte本ID);
    
    本et使本n t本使e;
}

軍Min成輸入isto本icalCha本acte本 UMin成輸入isto本icalCha本acte本Mana成e本::Get輸入isto本icalCha本acte本(const 軍St本in成& Cha本acte本ID) const
{
    const 軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    本et使本n Cha本acte本 基本 *Cha本acte本 : 軍Min成輸入isto本icalCha本acte本();
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成輸入isto本icalCha本acte本Mana成e本::GetAll輸入isto本icalCha本acte本s() const
{
    本et使本n Re成iste本edCha本acte本s;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成輸入isto本icalCha本acte本Mana成e本::GetCha本acte本sBy軍action(ECha本acte本軍action 軍action) const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : Re成iste本edCha本acte本s)
    {
        if (Cha本acte本.軍action == 軍action)
        {
            Res使lt.Add(Cha本acte本);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成輸入isto本icalCha本acte本Mana成e本::GetCha本acte本sByP本ofession(ECha本acte本P本ofession P本ofession) const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : Re成iste本edCha本acte本s)
    {
        if (Cha本acte本.P本ofession == P本ofession)
        {
            Res使lt.Add(Cha本acte本);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成輸入isto本icalCha本acte本Mana成e本::GetKeyCha本acte本s() const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : Re成iste本edCha本acte本s)
    {
        if (Cha本acte本.bIsKeyCha本acte本)
        {
            Res使lt.Add(Cha本acte本);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成輸入isto本icalCha本acte本Mana成e本::GetRec本使itableCha本acte本s() const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : Re成iste本edCha本acte本s)
    {
        if (Cha本acte本.bIsRec本使itable && Cha本acte本.State == ECha本acte本State::Acti正e)
        {
            Res使lt.Add(Cha本acte本);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成輸入isto本icalCha本acte本Mana成e本::GetActi正eCha本acte本s() const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : Re成iste本edCha本acte本s)
    {
        if (Cha本acte本.State == ECha本acte本State::Acti正e)
        {
            Res使lt.Add(Cha本acte本);
        }
    }
    
    本et使本n Res使lt;
}

軍Cha本acte本Inte本actionRes使lt UMin成輸入isto本icalCha本acte本Mana成e本::Inte本act基本ithCha本acte本(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType, const 軍St本in成& Inte本actionDetails)
{
    軍Cha本acte本Inte本actionRes使lt Res使lt;
    Res使lt.Inte本actionType = Inte本actionType;
    Res使lt.Ta本成etCha本acte本ID = Cha本acte本ID;
    Res使lt.Inte本actionTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (!bIsInitialized)
    {
        Res使lt.Res使ltDesc本iption = TEXT("Cha本acte本Mana成e本 not initialized");
        本et使本n Res使lt;
    }

    const 軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Cha本acte本 %s not fo使nd"), *Cha本acte本ID);
        本et使本n Res使lt;
    }

    // 檢查互動條件
    if (!CheckInte本actionConditions(Cha本acte本ID, Inte本actionType))
    {
        Res使lt.Res使ltDesc本iption = TEXT("Inte本action conditions not 設置et");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Inte本actin成 with cha本acte本: %s - %s"), *Cha本acte本ID, *Inte本actionType);

    // 處理人物互動邏輯
    輸入andleCha本acte本Inte本action(Cha本acte本ID, Inte本actionType, Res使lt);
    
    // 應用互動效果
    ApplyInte本actionEffects(Cha本acte本ID, Inte本actionType, Res使lt);
    
    // 更新人物關係
    if (Inte本actionDetails.Contains(TEXT("ta本成et下cha本acte本")))
    {
        UpdateCha本acte本Relationships(Cha本acte本ID, Inte本actionDetails, Inte本actionType);
    }
    
    // 計算互動分數
    Res使lt.Inte本actionSco本e = Calc使lateInte本actionSco本e(Cha本acte本ID, Inte本actionType);
    
    // 更新統計
    int32* Co使nt = Cha本acte本Statistics.軍ind(TEXT("inte本actions"));
    if (Co使nt)
    {
        (*Co使nt)++;
    }
    else
    {
        Cha本acte本Statistics.Add(TEXT("inte本actions"), 1);
    }
    
    Res使lt.bS使ccess = t本使e;
    
    // 記錄人物日誌
    Lo成Cha本acte本E正ent(Cha本acte本ID, 軍St本in成::P本intf(TEXT("Inte本action: %s - %s"), *Inte本actionType, *Res使lt.Res使ltDesc本iption));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cha本acte本 inte本action co設置pleted: %s"), *Cha本acte本ID);
    
    本et使本n Res使lt;
}

軍Dialo成使eE正ent UMin成輸入isto本icalCha本acte本Mana成e本::Sta本tDialo成使e(const 軍St本in成& Speake本ID, const 軍St本in成& A使dienceID, const 軍St本in成& Dialo成使eTopic)
{
    軍Dialo成使eE正ent E正ent;
    E正ent.E正entID = 軍St本in成::P本intf(TEXT("dialo成使e下%s下%s下%d"), *Speake本ID, *A使dienceID, 軍DateTi設置e::的ow().GetMillisecond());
    E正ent.Speake本ID = Speake本ID;
    E正ent.A使dienceID = A使dienceID;
    E正ent.Dialo成使eTopic = Dialo成使eTopic;
    E正ent.Dialo成使eTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cha本acte本Mana成e本 not initialized"));
        本et使本n E正ent;
    }

    const 軍Min成輸入isto本icalCha本acte本* Speake本 = Cha本acte本Map.軍ind(Speake本ID);
    const 軍Min成輸入isto本icalCha本acte本* A使dience = Cha本acte本Map.軍ind(A使dienceID);
    
    if (!Speake本  !A使dience)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Speake本 o本 a使dience not fo使nd"));
        本et使本n E正ent;
    }

    // 處理對話邏輯
    輸入andleDialo成使eLo成ic(Speake本ID, A使dienceID, Dialo成使eTopic, E正ent);
    
    // 記錄對話事件
    Reco本dDialo成使eE正ent(E正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dialo成使e sta本ted: %s -> %s (%s)"), *Speake本ID, *A使dienceID, *Dialo成使eTopic);
    
    本et使本n E正ent;
}

軍Dialo成使eE正ent UMin成輸入isto本icalCha本acte本Mana成e本::P本ocessDialo成使eOption(const 軍St本in成& Dialo成使eE正entID, const 軍St本in成& OptionID)
{
    軍Dialo成使eE正ent Res使lt;
    Res使lt.E正entID = Dialo成使eE正entID;
    
    // 查找對話事件
    fo本 (const 軍Dialo成使eE正ent& E正ent : Dialo成使e輸入isto本y)
    {
        if (E正ent.E正entID == Dialo成使eE正entID)
        {
            Res使lt = E正ent;
            b本eak;
        }
    }
    
    // 處理對話選項
    fo本 (const 軍Dialo成使eOption& Option : Res使lt.Dialo成使eOptions)
    {
        if (Option.OptionID == OptionID)
        {
            // 應用選項後果
            fo本 (const 軍St本in成& Conseq使ence : Option.Conseq使ences)
            {
                // 簡化實作：記錄後果
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Dialo成使e conseq使ence: %s"), *Conseq使ence);
            }
            
            // 更新關係
            UpdateCha本acte本Relationships(Res使lt.Speake本ID, Res使lt.A使dienceID, TEXT("dialo成使e"));
            
            b本eak;
        }
    }
    
    本et使本n Res使lt;
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::Rec本使itCha本acte本(const 軍St本in成& Cha本acte本ID)
{
    const 軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cha本acte本 %s not fo使nd"), *Cha本acte本ID);
        本et使本n false;
    }

    if (!Cha本acte本->bIsRec本使itable)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cha本acte本 %s is not 本ec本使itable"), *Cha本acte本ID);
        本et使本n false;
    }

    // 檢查招募成本
    // 簡化實作：假設成本充足
    
    // 更新人物狀態
    UpdateCha本acte本State(Cha本acte本ID, ECha本acte本State::Acti正e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cha本acte本 本ec本使ited: %s"), *Cha本acte本ID);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::Dis設置issCha本acte本(const 軍St本in成& Cha本acte本ID)
{
    const 軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cha本acte本 %s not fo使nd"), *Cha本acte本ID);
        本et使本n false;
    }

    // 更新人物狀態
    UpdateCha本acte本State(Cha本acte本ID, ECha本acte本State::Inacti正e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cha本acte本 dis設置issed: %s"), *Cha本acte本ID);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::Up成本adeCha本acte本Skill(const 軍St本in成& Cha本acte本ID, ECha本acte本SkillType SkillType)
{
    軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cha本acte本 %s not fo使nd"), *Cha本acte本ID);
        本et使本n false;
    }

    // 查找並升級技能
    fo本 (軍Cha本acte本Skill& Skill : Cha本acte本->Skills)
    {
        if (Skill.SkillType == SkillType && Skill.SkillLe正el < 10)
        {
            Skill.SkillLe正el++;
            Skill.SkillExpe本ience = 0;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Cha本acte本 skill 使p成本aded: %s - %s to le正el %d"), 
                *Cha本acte本ID, *Skill.Skill的a設置e, Skill.SkillLe正el);
            
            本et使本n t本使e;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Skill not fo使nd o本 al本eady at 設置ax le正el: %s"), *Cha本acte本ID);
    本et使本n false;
}

TA本本ay<軍Cha本acte本Relationship> UMin成輸入isto本icalCha本acte本Mana成e本::GetCha本acte本Relationships(const 軍St本in成& Cha本acte本ID) const
{
    const TA本本ay<軍Cha本acte本Relationship>* Relationships = Relationship的etwo本k.軍ind(Cha本acte本ID);
    本et使本n Relationships 基本 *Relationships : TA本本ay<軍Cha本acte本Relationship>();
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::AddCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID, ECha本acte本RelationshipType RelationshipType, float St本en成th)
{
    軍Cha本acte本Relationship Relationship;
    Relationship.RelationshipType = RelationshipType;
    Relationship.Ta本成etCha本acte本ID = Ta本成etCha本acte本ID;
    Relationship.RelationshipSt本en成th = St本en成th;
    Relationship.EstablishedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    Relationship.Inte本actionCo使nt = 0;
    Relationship.LastInte本actionTi設置e = Relationship.EstablishedTi設置e;
    
    // 添加到關係網絡
    TA本本ay<軍Cha本acte本Relationship>* Relationships = Relationship的etwo本k.軍ind(Cha本acte本ID);
    if (!Relationships)
    {
        TA本本ay<軍Cha本acte本Relationship> 的ewRelationships;
        的ewRelationships.Add(Relationship);
        Relationship的etwo本k.Add(Cha本acte本ID, 的ewRelationships);
    }
    else
    {
        Relationships->Add(Relationship);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cha本acte本 本elationship added: %s -> %s (%s)"), 
        *Cha本acte本ID, *Ta本成etCha本acte本ID, *UEn使設置::GetVal使eAsSt本in成(RelationshipType));
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::UpdateCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID, float 的ewSt本en成th)
{
    TA本本ay<軍Cha本acte本Relationship>* Relationships = Relationship的etwo本k.軍ind(Cha本acte本ID);
    if (!Relationships)
    {
        本et使本n false;
    }
    
    fo本 (軍Cha本acte本Relationship& Relationship : *Relationships)
    {
        if (Relationship.Ta本成etCha本acte本ID == Ta本成etCha本acte本ID)
        {
            Relationship.RelationshipSt本en成th = 的ewSt本en成th;
            Relationship.LastInte本actionTi設置e = 軍Platfo本設置Ti設置e::Seconds();
            Relationship.Inte本actionCo使nt++;
            
            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Cha本acte本 本elationship 使pdated: %s -> %s (%.1f)"), 
                *Cha本acte本ID, *Ta本成etCha本acte本ID, 的ewSt本en成th);
            
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

TMap<軍St本in成, int32> UMin成輸入isto本icalCha本acte本Mana成e本::GetCha本acte本Statistics() const
{
    本et使本n Cha本acte本Statistics;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成輸入isto本icalCha本acte本Mana成e本::GetCha本acte本Infl使enceRankin成() const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> Res使lt = Re成iste本edCha本acte本s;
    
    // 按影響力排序
    Res使lt.So本t([this](const 軍Min成輸入isto本icalCha本acte本& A, const 軍Min成輸入isto本icalCha本acte本& B)
    {
        本et使本n Calc使lateCha本acte本Infl使ence(A) > Calc使lateCha本acte本Infl使ence(B);
    });
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成輸入isto本icalCha本acte本Mana成e本::GetReco設置設置endedCha本acte本s(const 軍St本in成& Playe本軍action) const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> Res使lt;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : Re成iste本edCha本acte本s)
    {
        if (Cha本acte本.bIsRec本使itable && Cha本acte本.State == ECha本acte本State::Acti正e)
        {
            float Sco本e = GetCha本acte本Reco設置設置endationSco本e(Cha本acte本, Playe本軍action);
            if (Sco本e > 0.5f)
            {
                Res使lt.Add(Cha本acte本);
            }
        }
    }
    
    // 按推薦分數排序
    Res使lt.So本t([this, &Playe本軍action](const 軍Min成輸入isto本icalCha本acte本& A, const 軍Min成輸入isto本icalCha本acte本& B)
    {
        本et使本n GetCha本acte本Reco設置設置endationSco本e(A, Playe本軍action) > GetCha本acte本Reco設置設置endationSco本e(B, Playe本軍action);
    });
    
    本et使本n Res使lt;
}

// === 內部方法實現 ===

正oid UMin成輸入isto本icalCha本acte本Mana成e本::Initialize輸入isto本icalCha本acte本Lib本a本y()
{
    C本eatePoliticalCha本acte本s();
    C本eateMilita本yCha本acte本s();
    C本eateB使sinessCha本acte本s();
    C本eateSchola本Cha本acte本s();
    C本eateRe正ol使tiona本yCha本acte本s();
    C本eateDiplo設置aticCha本acte本s();
    C本eateC使lt使本alCha本acte本s();
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::C本eatePoliticalCha本acte本s()
{
    // 孫中山
    軍Min成輸入isto本icalCha本acte本 S使nYatSen;
    S使nYatSen.Cha本acte本ID = TEXT("s使n下yat下sen");
    S使nYatSen.Cha本acte本的a設置e = TEXT("孫中山");
    S使nYatSen.Co使本tesy的a設置e = TEXT("逸仙");
    S使nYatSen.En成lish的a設置e = TEXT("S使n Yat-sen");
    S使nYatSen.Cha本acte本Desc本iption = TEXT("中國近代民主革命家，中華民國國父");
    S使nYatSen.Gende本 = ECha本acte本Gende本::Male;
    S使nYatSen.P本ofession = ECha本acte本P本ofession::Politician;
    S使nYatSen.軍action = ECha本acte本軍action::的ationalist;
    S使nYatSen.Bi本thYea本 = 1866;
    S使nYatSen.DeathYea本 = 1925;
    S使nYatSen.State = ECha本acte本State::Deceased;
    S使nYatSen.Att本ib使tes.Leade本ship = 95.0f;
    S使nYatSen.Att本ib使tes.Cha本is設置a = 90.0f;
    S使nYatSen.Att本ib使tes.Diplo設置acy = 85.0f;
    S使nYatSen.Att本ib使tes.St本ate成y = 80.0f;
    S使nYatSen.Att本ib使tes.O本ato本y = 95.0f;
    S使nYatSen.Infl使ence = 950.0f;
    S使nYatSen.Rep使tation = 980.0f;
    S使nYatSen.bIsKeyCha本acte本 = t本使e;
    S使nYatSen.bIsRec本使itable = false;
    S使nYatSen.Cha本acte本Ta成s.Add(TEXT("fo使nde本"));
    S使nYatSen.Cha本acte本Ta成s.Add(TEXT("本e正ol使tiona本y"));
    S使nYatSen.輸入isto本icalE正ents.Add(TEXT("xinhai下本e正ol使tion"));
    S使nYatSen.Achie正e設置ents.Add(TEXT("fo使nded下本ep使blic"));
    
    Re成iste本輸入isto本icalCha本acte本(S使nYatSen);

    // 蔣介石
    軍Min成輸入isto本icalCha本acte本 Chian成KaiShek;
    Chian成KaiShek.Cha本acte本ID = TEXT("chian成下kai下shek");
    Chian成KaiShek.Cha本acte本的a設置e = TEXT("蔣介石");
    Chian成KaiShek.Co使本tesy的a設置e = TEXT("介石");
    Chian成KaiShek.En成lish的a設置e = TEXT("Chian成 Kai-shek");
    Chian成KaiShek.Cha本acte本Desc本iption = TEXT("中華民國政治家、軍事家");
    Chian成KaiShek.Gende本 = ECha本acte本Gende本::Male;
    Chian成KaiShek.P本ofession = ECha本acte本P本ofession::Milita本y;
    Chian成KaiShek.軍action = ECha本acte本軍action::的ationalist;
    Chian成KaiShek.Bi本thYea本 = 1887;
    Chian成KaiShek.DeathYea本 = 1975;
    Chian成KaiShek.State = ECha本acte本State::Deceased;
    Chian成KaiShek.Att本ib使tes.Leade本ship = 85.0f;
    Chian成KaiShek.Att本ib使tes.Milita本y = 90.0f;
    Chian成KaiShek.Att本ib使tes.Ad設置inist本ation = 80.0f;
    Chian成KaiShek.Att本ib使tes.St本ate成y = 85.0f;
    Chian成KaiShek.Infl使ence = 850.0f;
    Chian成KaiShek.Rep使tation = 800.0f;
    Chian成KaiShek.bIsKeyCha本acte本 = t本使e;
    Chian成KaiShek.bIsRec本使itable = false;
    Chian成KaiShek.Cha本acte本Ta成s.Add(TEXT("設置ilita本y下leade本"));
    Chian成KaiShek.Cha本acte本Ta成s.Add(TEXT("nationalist"));
    Chian成KaiShek.輸入isto本icalE正ents.Add(TEXT("no本the本n下expedition"));
    Chian成KaiShek.輸入isto本icalE正ents.Add(TEXT("second下sino下大apanese下wa本"));
    Chian成KaiShek.Achie正e設置ents.Add(TEXT("使nified下china"));
    
    Re成iste本輸入isto本icalCha本acte本(Chian成KaiShek);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::C本eateMilita本yCha本acte本s()
{
    // 毛澤東
    軍Min成輸入isto本icalCha本acte本 MaoZedon成;
    MaoZedon成.Cha本acte本ID = TEXT("設置ao下zedon成");
    MaoZedon成.Cha本acte本的a設置e = TEXT("毛澤東");
    MaoZedon成.Co使本tesy的a設置e = TEXT("潤之");
    MaoZedon成.En成lish的a設置e = TEXT("Mao Zedon成");
    MaoZedon成.Cha本acte本Desc本iption = TEXT("中國共產黨主要創立人，中華人民共和國領袖");
    MaoZedon成.Gende本 = ECha本acte本Gende本::Male;
    MaoZedon成.P本ofession = ECha本acte本P本ofession::Re正ol使tiona本y;
    MaoZedon成.軍action = ECha本acte本軍action::Co設置設置使nist;
    MaoZedon成.Bi本thYea本 = 1893;
    MaoZedon成.DeathYea本 = 1976;
    MaoZedon成.State = ECha本acte本State::Deceased;
    MaoZedon成.Att本ib使tes.Leade本ship = 90.0f;
    MaoZedon成.Att本ib使tes.St本ate成y = 85.0f;
    MaoZedon成.Att本ib使tes.基本本itin成 = 80.0f;
    MaoZedon成.Att本ib使tes.Intelli成ence = 85.0f;
    MaoZedon成.Infl使ence = 950.0f;
    MaoZedon成.Rep使tation = 900.0f;
    MaoZedon成.bIsKeyCha本acte本 = t本使e;
    MaoZedon成.bIsRec本使itable = false;
    MaoZedon成.Cha本acte本Ta成s.Add(TEXT("co設置設置使nist下leade本"));
    MaoZedon成.Cha本acte本Ta成s.Add(TEXT("本e正ol使tiona本y"));
    MaoZedon成.輸入isto本icalE正ents.Add(TEXT("chinese下ci正il下wa本"));
    MaoZedon成.Achie正e設置ents.Add(TEXT("fo使nded下p本c"));
    
    Re成iste本輸入isto本icalCha本acte本(MaoZedon成);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::C本eateB使sinessCha本acte本s()
{
    // 張謇
    軍Min成輸入isto本icalCha本acte本 Zhan成X使elian成;
    Zhan成X使elian成.Cha本acte本ID = TEXT("zhan成下x使elian成");
    Zhan成X使elian成.Cha本acte本的a設置e = TEXT("張學良");
    Zhan成X使elian成.En成lish的a設置e = TEXT("Zhan成 X使elian成");
    Zhan成X使elian成.Cha本acte本Desc本iption = TEXT("東北軍閥，西安事變發動者");
    Zhan成X使elian成.Gende本 = ECha本acte本Gende本::Male;
    Zhan成X使elian成.P本ofession = ECha本acte本P本ofession::Milita本y;
    Zhan成X使elian成.軍action = ECha本acte本軍action::基本a本lo本d;
    Zhan成X使elian成.Bi本thYea本 = 1901;
    Zhan成X使elian成.DeathYea本 = 2001;
    Zhan成X使elian成.State = ECha本acte本State::Deceased;
    Zhan成X使elian成.Att本ib使tes.Milita本y = 75.0f;
    Zhan成X使elian成.Att本ib使tes.Diplo設置acy = 70.0f;
    Zhan成X使elian成.Att本ib使tes.Leade本ship = 70.0f;
    Zhan成X使elian成.Infl使ence = 600.0f;
    Zhan成X使elian成.Rep使tation = 650.0f;
    Zhan成X使elian成.bIsKeyCha本acte本 = t本使e;
    Zhan成X使elian成.bIsRec本使itable = t本使e;
    Zhan成X使elian成.Cha本acte本Ta成s.Add(TEXT("wa本lo本d"));
    Zhan成X使elian成.Cha本acte本Ta成s.Add(TEXT("xian下incident"));
    Zhan成X使elian成.輸入isto本icalE正ents.Add(TEXT("xian下incident"));
    Zhan成X使elian成.Achie正e設置ents.Add(TEXT("fo本ced下使nited下f本ont"));
    
    Re成iste本輸入isto本icalCha本acte本(Zhan成X使elian成);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::C本eateSchola本Cha本acte本s()
{
    // 胡適
    軍Min成輸入isto本icalCha本acte本 輸入使Shih;
    輸入使Shih.Cha本acte本ID = TEXT("h使下shih");
    輸入使Shih.Cha本acte本的a設置e = TEXT("胡適");
    輸入使Shih.En成lish的a設置e = TEXT("輸入使 Shih");
    輸入使Shih.Cha本acte本Desc本iption = TEXT("中國現代學者、思想家、文學家");
    輸入使Shih.Gende本 = ECha本acte本Gende本::Male;
    輸入使Shih.P本ofession = ECha本acte本P本ofession::Schola本;
    輸入使Shih.軍action = ECha本acte本軍action::的e使t本al;
    輸入使Shih.Bi本thYea本 = 1891;
    輸入使Shih.DeathYea本 = 1962;
    輸入使Shih.State = ECha本acte本State::Deceased;
    輸入使Shih.Att本ib使tes.Intelli成ence = 95.0f;
    輸入使Shih.Att本ib使tes.基本本itin成 = 90.0f;
    輸入使Shih.Att本ib使tes.Science = 85.0f;
    輸入使Shih.Att本ib使tes.Cha本is設置a = 80.0f;
    輸入使Shih.Infl使ence = 700.0f;
    輸入使Shih.Rep使tation = 750.0f;
    輸入使Shih.bIsKeyCha本acte本 = false;
    輸入使Shih.bIsRec本使itable = t本使e;
    輸入使Shih.Cha本acte本Ta成s.Add(TEXT("schola本"));
    輸入使Shih.Cha本acte本Ta成s.Add(TEXT("lite本a本y"));
    輸入使Shih.Achie正e設置ents.Add(TEXT("new下c使lt使本e下設置o正e設置ent"));
    
    Re成iste本輸入isto本icalCha本acte本(輸入使Shih);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::C本eateRe正ol使tiona本yCha本acte本s()
{
    // 宋慶齡
    軍Min成輸入isto本icalCha本acte本 Soon成Chin成Lin成;
    Soon成Chin成Lin成.Cha本acte本ID = TEXT("soon成下chin成下lin成");
    Soon成Chin成Lin成.Cha本acte本的a設置e = TEXT("宋慶齡");
    Soon成Chin成Lin成.En成lish的a設置e = TEXT("Soon成 Chin成-lin成");
    Soon成Chin成Lin成.Cha本acte本Desc本iption = TEXT("中華人民共和國名譽主席，孫中山夫人");
    Soon成Chin成Lin成.Gende本 = ECha本acte本Gende本::軍e設置ale;
    Soon成Chin成Lin成.P本ofession = ECha本acte本P本ofession::Politician;
    Soon成Chin成Lin成.軍action = ECha本acte本軍action::Co設置設置使nist;
    Soon成Chin成Lin成.Bi本thYea本 = 1893;
    Soon成Chin成Lin成.DeathYea本 = 1981;
    Soon成Chin成Lin成.State = ECha本acte本State::Deceased;
    Soon成Chin成Lin成.Att本ib使tes.Cha本is設置a = 85.0f;
    Soon成Chin成Lin成.Att本ib使tes.Diplo設置acy = 80.0f;
    Soon成Chin成Lin成.Att本ib使tes.Intelli成ence = 75.0f;
    Soon成Chin成Lin成.Infl使ence = 800.0f;
    Soon成Chin成Lin成.Rep使tation = 850.0f;
    Soon成Chin成Lin成.bIsKeyCha本acte本 = t本使e;
    Soon成Chin成Lin成.bIsRec本使itable = false;
    Soon成Chin成Lin成.Cha本acte本Ta成s.Add(TEXT("fe設置ale下leade本"));
    Soon成Chin成Lin成.Cha本acte本Ta成s.Add(TEXT("本e正ol使tiona本y"));
    Soon成Chin成Lin成.Achie正e設置ents.Add(TEXT("wo設置en下本i成hts下ad正ocate"));
    
    Re成iste本輸入isto本icalCha本acte本(Soon成Chin成Lin成);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::C本eateDiplo設置aticCha本acte本s()
{
    // 周恩來
    軍Min成輸入isto本icalCha本acte本 Zho使Enlai;
    Zho使Enlai.Cha本acte本ID = TEXT("zho使下enlai");
    Zho使Enlai.Cha本acte本的a設置e = TEXT("周恩來");
    Zho使Enlai.En成lish的a設置e = TEXT("Zho使 Enlai");
    Zho使Enlai.Cha本acte本Desc本iption = TEXT("中華人民共和國第一任國務院總理");
    Zho使Enlai.Gende本 = ECha本acte本Gende本::Male;
    Zho使Enlai.P本ofession = ECha本acte本P本ofession::Diplo設置at;
    Zho使Enlai.軍action = ECha本acte本軍action::Co設置設置使nist;
    Zho使Enlai.Bi本thYea本 = 1898;
    Zho使Enlai.DeathYea本 = 1976;
    Zho使Enlai.State = ECha本acte本State::Deceased;
    Zho使Enlai.Att本ib使tes.Diplo設置acy = 95.0f;
    Zho使Enlai.Att本ib使tes.Cha本is設置a = 90.0f;
    Zho使Enlai.Att本ib使tes.Ad設置inist本ation = 85.0f;
    Zho使Enlai.Att本ib使tes.Intelli成ence = 85.0f;
    Zho使Enlai.Infl使ence = 900.0f;
    Zho使Enlai.Rep使tation = 950.0f;
    Zho使Enlai.bIsKeyCha本acte本 = t本使e;
    Zho使Enlai.bIsRec本使itable = false;
    Zho使Enlai.Cha本acte本Ta成s.Add(TEXT("diplo設置at"));
    Zho使Enlai.Cha本acte本Ta成s.Add(TEXT("p本e設置ie本"));
    Zho使Enlai.Achie正e設置ents.Add(TEXT("established下diplo設置atic下本elations"));
    
    Re成iste本輸入isto本icalCha本acte本(Zho使Enlai);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::C本eateC使lt使本alCha本acte本s()
{
    // 魯迅
    軍Min成輸入isto本icalCha本acte本 L使X使n;
    L使X使n.Cha本acte本ID = TEXT("l使下x使n");
    L使X使n.Cha本acte本的a設置e = TEXT("魯迅");
    L使X使n.En成lish的a設置e = TEXT("L使 X使n");
    L使X使n.Cha本acte本Desc本iption = TEXT("中國現代文學家、思想家");
    L使X使n.Gende本 = ECha本acte本Gende本::Male;
    L使X使n.P本ofession = ECha本acte本P本ofession::A本tist;
    L使X使n.軍action = ECha本acte本軍action::的e使t本al;
    L使X使n.Bi本thYea本 = 1881;
    L使X使n.DeathYea本 = 1936;
    L使X使n.State = ECha本acte本State::Deceased;
    L使X使n.Att本ib使tes.基本本itin成 = 95.0f;
    L使X使n.Att本ib使tes.Intelli成ence = 85.0f;
    L使X使n.Att本ib使tes.Cha本is設置a = 80.0f;
    L使X使n.Infl使ence = 750.0f;
    L使X使n.Rep使tation = 800.0f;
    L使X使n.bIsKeyCha本acte本 = false;
    L使X使n.bIsRec本使itable = t本使e;
    L使X使n.Cha本acte本Ta成s.Add(TEXT("w本ite本"));
    L使X使n.Cha本acte本Ta成s.Add(TEXT("lite本a本y"));
    L使X使n.Achie正e設置ents.Add(TEXT("設置ode本n下chinese下lite本at使本e"));
    
    Re成iste本輸入isto本icalCha本acte本(L使X使n);
}

// === 互動處理方法 ===

正oid UMin成輸入isto本icalCha本acte本Mana成e本::輸入andleCha本acte本Inte本action(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType, 軍Cha本acte本Inte本actionRes使lt& Res使lt)
{
    軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        本et使本n;
    }

    // 根據互動類型處理
    if (Inte本actionType == TEXT("con正e本sation"))
    {
        Res使lt.Res使ltDesc本iption = TEXT("輸入ad a 設置eanin成f使l con正e本sation");
        Res使lt.GainedExpe本ience = 10;
        
        // 增加魅力和外交能力
        Cha本acte本->Att本ib使tes.Cha本is設置a = 軍Math::Cla設置p(Cha本acte本->Att本ib使tes.Cha本is設置a + 1.0f, 0.0f, 100.0f);
        Cha本acte本->Att本ib使tes.Diplo設置acy = 軍Math::Cla設置p(Cha本acte本->Att本ib使tes.Diplo設置acy + 0.5f, 0.0f, 100.0f);
    }
    else if (Inte本actionType == TEXT("debate"))
    {
        Res使lt.Res使ltDesc本iption = TEXT("En成a成ed in intellect使al debate");
        Res使lt.GainedExpe本ience = 15;
        
        // 增加智力演講能力
        Cha本acte本->Att本ib使tes.Intelli成ence = 軍Math::Cla設置p(Cha本acte本->Att本ib使tes.Intelli成ence + 1.5f, 0.0f, 100.0f);
        Cha本acte本->Att本ib使tes.O本ato本y = 軍Math::Cla設置p(Cha本acte本->Att本ib使tes.O本ato本y + 1.0f, 0.0f, 100.0f);
    }
    else if (Inte本actionType == TEXT("collabo本ation"))
    {
        Res使lt.Res使ltDesc本iption = TEXT("Collabo本ated on a p本o大ect");
        Res使lt.GainedExpe本ience = 20;
        
        // 增加行政和領導力
        Cha本acte本->Att本ib使tes.Ad設置inist本ation = 軍Math::Cla設置p(Cha本acte本->Att本ib使tes.Ad設置inist本ation + 1.0f, 0.0f, 100.0f);
        Cha本acte本->Att本ib使tes.Leade本ship = 軍Math::Cla設置p(Cha本acte本->Att本ib使tes.Leade本ship + 0.5f, 0.0f, 100.0f);
    }
    else
    {
        Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Inte本acted with: %s"), *Inte本actionType);
        Res使lt.GainedExpe本ience = 5;
    }

    // 增加經驗值
    Cha本acte本->Att本ib使tes.Expe本ience += Res使lt.GainedExpe本ience;
    
    // 檢查升級
    if (Cha本acte本->Att本ib使tes.Expe本ience >= Cha本acte本->Att本ib使tes.Le正el * 100)
    {
        Cha本acte本->Att本ib使tes.Le正el++;
        Cha本acte本->Att本ib使tes.Expe本ience = 0;
        Res使lt.Res使ltDesc本iption += 軍St本in成::P本intf(TEXT(" - Le正el 使p to %d!"), Cha本acte本->Att本ib使tes.Le正el);
    }
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::輸入andleDialo成使eLo成ic(const 軍St本in成& Speake本ID, const 軍St本in成& A使dienceID, const 軍St本in成& Dialo成使eTopic, 軍Dialo成使eE正ent& E正ent)
{
    const 軍Min成輸入isto本icalCha本acte本* Speake本 = Cha本acte本Map.軍ind(Speake本ID);
    const 軍Min成輸入isto本icalCha本acte本* A使dience = Cha本acte本Map.軍ind(A使dienceID);
    
    if (!Speake本  !A使dience)
    {
        本et使本n;
    }

    // 生成對話文本
    if (Dialo成使eTopic == TEXT("politics"))
    {
        E正ent.Dialo成使eText = 軍St本in成::P本intf(TEXT("%s disc使sses political 設置atte本s with %s"), *Speake本->Cha本acte本的a設置e, *A使dience->Cha本acte本的a設置e);
        E正ent.Dialo成使eMood = TEXT("se本io使s");
    }
    else if (Dialo成使eTopic == TEXT("設置ilita本y"))
    {
        E正ent.Dialo成使eText = 軍St本in成::P本intf(TEXT("%s talks abo使t 設置ilita本y st本ate成y with %s"), *Speake本->Cha本acte本的a設置e, *A使dience->Cha本acte本的a設置e);
        E正ent.Dialo成使eMood = TEXT("foc使sed");
    }
    else if (Dialo成使eTopic == TEXT("c使lt使本e"))
    {
        E正ent.Dialo成使eText = 軍St本in成::P本intf(TEXT("%s sha本es c使lt使本al insi成hts with %s"), *Speake本->Cha本acte本的a設置e, *A使dience->Cha本acte本的a設置e);
        E正ent.Dialo成使eMood = TEXT("本elaxed");
    }
    else
    {
        E正ent.Dialo成使eText = 軍St本in成::P本intf(TEXT("%s and %s ha正e a con正e本sation"), *Speake本->Cha本acte本的a設置e, *A使dience->Cha本acte本的a設置e);
        E正ent.Dialo成使eMood = TEXT("ne使t本al");
    }

    // 生成對話選項
    軍Dialo成使eOption Option1;
    Option1.OptionID = TEXT("a成本ee");
    Option1.OptionText = TEXT("I a成本ee with yo使本 point of 正iew");
    Option1.OptionDesc本iption = TEXT("Exp本ess a成本ee設置ent");
    Option1.基本ei成ht = 1.0f;
    
    軍Dialo成使eOption Option2;
    Option2.OptionID = TEXT("disa成本ee");
    Option2.OptionText = TEXT("I ha正e a diffe本ent pe本specti正e");
    Option2.OptionDesc本iption = TEXT("Exp本ess disa成本ee設置ent");
    Option2.基本ei成ht = 0.8f;
    
    軍Dialo成使eOption Option3;
    Option3.OptionID = TEXT("q使estion");
    Option3.OptionText = TEXT("Can yo使 elabo本ate on that基本");
    Option3.OptionDesc本iption = TEXT("Ask fo本 cla本ification");
    Option3.基本ei成ht = 0.6f;
    
    E正ent.Dialo成使eOptions.Add(Option1);
    E正ent.Dialo成使eOptions.Add(Option2);
    E正ent.Dialo成使eOptions.Add(Option3);
    
    // 設置對話重要性
    E正ent.Dialo成使eI設置po本tance = 0.5f;
    if (Speake本->bIsKeyCha本acte本  A使dience->bIsKeyCha本acte本)
    {
        E正ent.Dialo成使eI設置po本tance = 0.8f;
    }
}

float UMin成輸入isto本icalCha本acte本Mana成e本::Calc使lateInte本actionSco本e(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType) const
{
    const 軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        本et使本n 0.0f;
    }

    float Sco本e = 0.5f; // 基礎分數
    
    // 根據人物影響力加分
    Sco本e += Cha本acte本->Infl使ence / 1000.0f * 0.3f;
    
    // 根據互動類型加分
    if (Inte本actionType == TEXT("collabo本ation"))
    {
        Sco本e += 0.2f;
    }
    else if (Inte本actionType == TEXT("debate"))
    {
        Sco本e += 0.1f;
    }
    
    本et使本n 軍Math::Cla設置p(Sco本e, 0.0f, 1.0f);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::UpdateCha本acte本Relationships(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID, const 軍St本in成& Inte本actionType)
{
    // 獲取現有關係
    TA本本ay<軍Cha本acte本Relationship>* Relationships = Relationship的etwo本k.軍ind(Cha本acte本ID);
    if (!Relationships)
    {
        // 創建新關係
        AddCha本acte本Relationship(Cha本acte本ID, Ta本成etCha本acte本ID, ECha本acte本RelationshipType::Acq使aintance, 50.0f);
        本et使本n;
    }
    
    // 更新現有關係
    fo本 (軍Cha本acte本Relationship& Relationship : *Relationships)
    {
        if (Relationship.Ta本成etCha本acte本ID == Ta本成etCha本acte本ID)
        {
            float St本en成thChan成e = 0.0f;
            
            if (Inte本actionType == TEXT("con正e本sation"))
            {
                St本en成thChan成e = 5.0f;
            }
            else if (Inte本actionType == TEXT("collabo本ation"))
            {
                St本en成thChan成e = 10.0f;
            }
            else if (Inte本actionType == TEXT("debate"))
            {
                St本en成thChan成e = -2.0f; // 辯論可能降低關係
            }
            
            Relationship.RelationshipSt本en成th = 軍Math::Cla設置p(Relationship.RelationshipSt本en成th + St本en成thChan成e, 0.0f, 100.0f);
            Relationship.LastInte本actionTi設置e = 軍Platfo本設置Ti設置e::Seconds();
            Relationship.Inte本actionCo使nt++;
            
            // 更新關係類型
            if (Relationship.RelationshipSt本en成th >= 80.0f)
            {
                Relationship.RelationshipType = ECha本acte本RelationshipType::軍本iend;
            }
            else if (Relationship.RelationshipSt本en成th >= 60.0f)
            {
                Relationship.RelationshipType = ECha本acte本RelationshipType::Collea成使e;
            }
            else if (Relationship.RelationshipSt本en成th <= 20.0f)
            {
                Relationship.RelationshipType = ECha本acte本RelationshipType::Ri正al;
            }
            
            b本eak;
        }
    }
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::ApplyInte本actionEffects(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType, 軍Cha本acte本Inte本actionRes使lt& Res使lt)
{
    軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        本et使本n;
    }

    // 記錄影響的屬性
    if (Inte本actionType == TEXT("con正e本sation"))
    {
        Res使lt.AffectedAtt本ib使tes.Add(TEXT("cha本is設置a"), 1.0f);
        Res使lt.AffectedAtt本ib使tes.Add(TEXT("diplo設置acy"), 0.5f);
    }
    else if (Inte本actionType == TEXT("debate"))
    {
        Res使lt.AffectedAtt本ib使tes.Add(TEXT("intelli成ence"), 1.5f);
        Res使lt.AffectedAtt本ib使tes.Add(TEXT("o本ato本y"), 1.0f);
    }
    else if (Inte本actionType == TEXT("collabo本ation"))
    {
        Res使lt.AffectedAtt本ib使tes.Add(TEXT("ad設置inist本ation"), 1.0f);
        Res使lt.AffectedAtt本ib使tes.Add(TEXT("leade本ship"), 0.5f);
    }
    
    // 更新影響力
    Cha本acte本->Infl使ence = 軍Math::Cla設置p(Cha本acte本->Infl使ence + Res使lt.Inte本actionSco本e * 10.0f, 0.0f, 1000.0f);
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::CheckInte本actionConditions(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType) const
{
    const 軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        本et使本n false;
    }

    // 檢查人物狀態
    if (Cha本acte本->State != ECha本acte本State::Acti正e)
    {
        本et使本n false;
    }

    // 檢查健康狀況
    if (Cha本acte本->輸入ealth < 20.0f)
    {
        本et使本n false;
    }

    // 檢查精神狀態
    if (Cha本acte本->Mo本ale < 10.0f)
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

float UMin成輸入isto本icalCha本acte本Mana成e本::Calc使lateCha本acte本Infl使ence(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本) const
{
    float Infl使ence = 0.0f;
    
    // 基於屬性計算影響力
    Infl使ence += Cha本acte本.Att本ib使tes.Leade本ship * 2.0f;
    Infl使ence += Cha本acte本.Att本ib使tes.Cha本is設置a * 1.5f;
    Infl使ence += Cha本acte本.Att本ib使tes.Diplo設置acy * 1.5f;
    Infl使ence += Cha本acte本.Att本ib使tes.Intelli成ence * 1.0f;
    
    // 基於職業加分
    switch (Cha本acte本.P本ofession)
    {
    case ECha本acte本P本ofession::Politician:
        Infl使ence += 100.0f;
        b本eak;
    case ECha本acte本P本ofession::Milita本y:
        Infl使ence += 80.0f;
        b本eak;
    case ECha本acte本P本ofession::Diplo設置at:
        Infl使ence += 70.0f;
        b本eak;
    case ECha本acte本P本ofession::Re正ol使tiona本y:
        Infl使ence += 60.0f;
        b本eak;
    defa使lt:
        b本eak;
    }
    
    // 基於關鍵性加分
    if (Cha本acte本.bIsKeyCha本acte本)
    {
        Infl使ence += 200.0f;
    }
    
    本et使本n 軍Math::Cla設置p(Infl使ence, 0.0f, 1000.0f);
}

float UMin成輸入isto本icalCha本acte本Mana成e本::Calc使lateCha本acte本Rep使tation(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本) const
{
    float Rep使tation = 0.0f;
    
    // 基於屬性計算聲望
    Rep使tation += Cha本acte本.Att本ib使tes.Leade本ship * 1.5f;
    Rep使tation += Cha本acte本.Att本ib使tes.Cha本is設置a * 1.0f;
    Rep使tation += Cha本acte本.Att本ib使tes.Ad設置inist本ation * 1.0f;
    Rep使tation += Cha本acte本.Att本ib使tes.St本ate成y * 1.0f;
    
    // 基於成就加分
    Rep使tation += Cha本acte本.Achie正e設置ents.的使設置() * 50.0f;
    
    // 基於歷史事件加分
    Rep使tation += Cha本acte本.輸入isto本icalE正ents.的使設置() * 30.0f;
    
    本et使本n 軍Math::Cla設置p(Rep使tation, 0.0f, 1000.0f);
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::UpdateCha本acte本State(const 軍St本in成& Cha本acte本ID, ECha本acte本State 的ewState)
{
    軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        本et使本n;
    }
    
    Cha本acte本->State = 的ewState;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cha本acte本 state 使pdated: %s -> %s"), *Cha本acte本ID, *UEn使設置::GetVal使eAsSt本in成(的ewState));
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::Reco本dDialo成使eE正ent(const 軍Dialo成使eE正ent& E正ent)
{
    Dialo成使e輸入isto本y.Add(E正ent);
    
    // 限制歷史記錄數量
    if (Dialo成使e輸入isto本y.的使設置() > 1000)
    {
        Dialo成使e輸入isto本y.Re設置o正eAt(0);
    }
}

float UMin成輸入isto本icalCha本acte本Mana成e本::GetCha本acte本Reco設置設置endationSco本e(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本, const 軍St本in成& Playe本軍action) const
{
    float Sco本e = 0.5f; // 基礎分數
    
    // 基於陣營匹配度
    if (Playe本軍action == UEn使設置::GetVal使eAsSt本in成(Cha本acte本.軍action))
    {
        Sco本e += 0.3f;
    }
    else if (Cha本acte本.軍action == ECha本acte本軍action::的e使t本al)
    {
        Sco本e += 0.1f;
    }
    else
    {
        Sco本e -= 0.2f;
    }
    
    // 基於影響力
    Sco本e += Cha本acte本.Infl使ence / 1000.0f * 0.3f;
    
    // 基於可招募性
    if (Cha本acte本.bIsRec本使itable && Cha本acte本.State == ECha本acte本State::Acti正e)
    {
        Sco本e += 0.2f;
    }
    
    本et使本n 軍Math::Cla設置p(Sco本e, 0.0f, 1.0f);
}

bool UMin成輸入isto本icalCha本acte本Mana成e本::ValidateCha本acte本Confi成(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本) const
{
    if (Cha本acte本.Cha本acte本ID.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Cha本acte本.Cha本acte本的a設置e.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Cha本acte本.Bi本thYea本 <= 0  Cha本acte本.DeathYea本 < Cha本acte本.Bi本thYea本)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalCha本acte本Mana成e本::Lo成Cha本acte本E正ent(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Messa成e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[Cha本acte本 %s] %s"), *Cha本acte本ID, *Messa成e);
}

軍St本in成 UMin成輸入isto本icalCha本acte本Mana成e本::Gene本ateCha本acte本Repo本t(const 軍St本in成& Cha本acte本ID) const
{
    const 軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID);
    if (!Cha本acte本)
    {
        本et使本n TEXT("Cha本acte本 not fo使nd");
    }
    
    軍St本in成 Repo本t = TEXT("=== 人物報告 ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("姓名: %s (%s)\n"), *Cha本acte本->Cha本acte本的a設置e, *Cha本acte本->Co使本tesy的a設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("職業: %s\n"), *UEn使設置::GetVal使eAsSt本in成(Cha本acte本->P本ofession));
    Repo本t += 軍St本in成::P本intf(TEXT("陣營: %s\n"), *UEn使設置::GetVal使eAsSt本in成(Cha本acte本->軍action));
    Repo本t += 軍St本in成::P本intf(TEXT("生卒: %d-%d\n"), Cha本acte本->Bi本thYea本, Cha本acte本->DeathYea本);
    Repo本t += 軍St本in成::P本intf(TEXT("狀態: %s\n"), *UEn使設置::GetVal使eAsSt本in成(Cha本acte本->State));
    Repo本t += 軍St本in成::P本intf(TEXT("影響力: %.1f\n"), Cha本acte本->Infl使ence);
    Repo本t += 軍St本in成::P本intf(TEXT("聲望: %.1f\n"), Cha本acte本->Rep使tation);
    Repo本t += 軍St本in成::P本intf(TEXT("等級: %d\n"), Cha本acte本->Att本ib使tes.Le正el);
    Repo本t += 軍St本in成::P本intf(TEXT("經驗: %d\n"), Cha本acte本->Att本ib使tes.Expe本ience);
    
    本et使本n Repo本t;
}
