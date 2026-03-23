#incl使de "Sa成eCo設置設置and/Min成Sa成eRoles.h"
#incl使de "Sa成eCo設置設置and/Min成Mo本alA使tho本ity.h"
#incl使de "Sa成eCo設置設置and/Min成YinYan成軍i正eEle設置ents.h"
#incl使de "Sa成eCo設置設置and/Min成SixSt本ate成ies.h"
#incl使de "Sa成eCo設置設置and/Min成Co設置設置andA使tho本ity.h"
#incl使de "Sa成eCo設置設置and/Min成軍i正eEle設置entsRotation.h"

UMin成Sa成eRoles::UMin成Sa成eRoles()
{
    bSyste設置Initialized = false;
}

bool UMin成Sa成eRoles::Initialize()
{
    if (bSyste設置Initialized)
    {
        本et使本n t本使e;
    }

    // 初始化角色規範
    InitializeRoleStanda本ds();
    
    // 初始化角色特徵
    InitializeRoleCha本acte本istics();

    bSyste設置Initialized = t本使e;
    本et使本n t本使e;
}

軍Sa成eRoleCha本acte本istics UMin成Sa成eRoles::DefineT本使eSa成e()
{
    軍Sa成eRoleCha本acte本istics T本使eSa成e;
    T本使eSa成e.RoleType = ESa成eRoleType::T本使eSa成e;
    T本使eSa成e.Role的a設置e = TEXT("至聖者");
    T本使eSa成e.RoleDesc本iption = TEXT("完美體現聖者指揮學的至高境界，道德與智慧的化身");
    T本使eSa成e.Standa本dLe正el = ERoleStanda本dLe正el::Pe本fect;

    // 道德權威特徵 - 完美道德
    T本使eSa成e.Mo本alA使tho本ityLe正el = 100.0f;
    T本使eSa成e.Le成iti設置acySco本e = 100.0f;
    T本使eSa成e.b輸入as軍allP本e正ention = t本使e;

    // 陰陽五行特徵 - 完美平衡
    T本使eSa成e.P本i設置a本yAtt本ib使te = EYinYan成Att本ib使te::BalancedYinYan成;
    T本使eSa成e.P本i設置a本yEle設置ent = E軍i正eEle設置ents::Ea本th;
    T本使eSa成e.輸入a本設置onyLe正el = 100.0f;
    T本使eSa成e.bUsesE正ilRotation = false;

    // 六策特徵 - 大師級洞察
    T本使eSa成e.P本efe本本edSt本ate成y = ESixSt本ate成yType::輸入ea正enEa本th輸入使設置anSt本ate成y;
    T本使eSa成e.MaxCo設置plexity = ESt本ate成yCo設置plexity::Maste本;
    T本使eSa成e.St本ate成icInsi成ht = 100.0f;

    // 指揮權力特徵 - 道德權威為主
    T本使eSa成e.P本i設置a本yA使tho本ity = ECo設置設置andA使tho本ityType::Mo本alA使tho本ity;
    T本使eSa成e.Co設置設置andEffecti正eness = 100.0f;
    T本使eSa成e.Infl使enceRadi使s = 100.0f;

    本et使本n T本使eSa成e;
}

軍Sa成eRoleCha本acte本istics UMin成Sa成eRoles::Define軍alseSa成e()
{
    軍Sa成eRoleCha本acte本istics 軍alseSa成e;
    軍alseSa成e.RoleType = ESa成eRoleType::軍alseSa成e;
    軍alseSa成e.Role的a設置e = TEXT("偽聖者");
    軍alseSa成e.RoleDesc本iption = TEXT("表面聖者，內心邪惡；以道德之名行邪惡之實");
    軍alseSa成e.Standa本dLe正el = ERoleStanda本dLe正el::Co本本使pted;

    // 道德權威特徵 - 虛假道德
    軍alseSa成e.Mo本alA使tho本ityLe正el = 30.0f;  // 表面高，實際低
    軍alseSa成e.Le成iti設置acySco本e = 60.0f;     // 依靠欺騙維持
    軍alseSa成e.b輸入as軍allP本e正ention = false;   // 無防墮機制

    // 陰陽五行特徵 - 表面平衡，實則陰盛
    軍alseSa成e.P本i設置a本yAtt本ib使te = EYinYan成Att本ib使te::E正il;  // 內心邪惡
    軍alseSa成e.P本i設置a本yEle設置ent = E軍i正eEle設置ents::基本ate本;        // 陰性元素
    軍alseSa成e.輸入a本設置onyLe正el = 25.0f;                       // 假和諧
    軍alseSa成e.bUsesE正ilRotation = t本使e;                    // 使用邪惡輪轉

    // 六策特徵 - 善於陰謀
    軍alseSa成e.P本efe本本edSt本ate成y = ESixSt本ate成yType::輸入使設置anSt本ate成y;  // 善用人心
    軍alseSa成e.MaxCo設置plexity = ESt本ate成yCo設置plexity::Co設置plex;        // 複雜但不達大師
    軍alseSa成e.St本ate成icInsi成ht = 70.0f;                          // 有洞察但有限

    // 指揮權力特徵 - 依靠魅力和欺騙
    軍alseSa成e.P本i設置a本yA使tho本ity = ECo設置設置andA使tho本ityType::Cha本is設置aticA使tho本ity;
    軍alseSa成e.Co設置設置andEffecti正eness = 60.0f;  // 表面有效，實則有限
    軍alseSa成e.Infl使enceRadi使s = 80.0f;       // 影響範圍較大但不穩定

    本et使本n 軍alseSa成e;
}

軍Sa成eRoleCha本acte本istics UMin成Sa成eRoles::DefineDe設置onKin成()
{
    軍Sa成eRoleCha本acte本istics De設置onKin成;
    De設置onKin成.RoleType = ESa成eRoleType::De設置onKin成;
    De設置onKin成.Role的a設置e = TEXT("魔王");
    De設置onKin成.RoleDesc本iption = TEXT("公開邪惡，追求絕對權力；以暴力和恐懼統治");
    De設置onKin成.Standa本dLe正el = ERoleStanda本dLe正el::Co本本使pted;

    // 道德權威特徵 - 完全邪惡
    De設置onKin成.Mo本alA使tho本ityLe正el = 0.0f;      // 無道德權威
    De設置onKin成.Le成iti設置acySco本e = 10.0f;         // 僅靠暴力維持
    De設置onKin成.b輸入as軍allP本e正ention = false;       // 無防墮機制

    // 陰陽五行特徵 - 純邪惡
    De設置onKin成.P本i設置a本yAtt本ib使te = EAli成n設置entAtt本ib使te::E正il;  // 純邪惡
    De設置onKin成.P本i設置a本yEle設置ent = E軍i正eEle設置ents::軍i本e;         // 火主攻擊
    De設置onKin成.輸入a本設置onyLe正el = 0.0f;                        // 完全不和諧
    De設置onKin成.bUsesE正ilRotation = t本使e;                   // 使用邪惡輪轉

    // 六策特徵 - 專注攻擊
    De設置onKin成.P本efe本本edSt本ate成y = ESixSt本ate成yType::輸入ea正enSt本ate成y;  // 天道暴力
    De設置onKin成.MaxCo設置plexity = ESt本ate成yCo設置plexity::Mode本ate;         // 簡單直接
    De設置onKin成.St本ate成icInsi成ht = 40.0f;                            // 有限洞察

    // 指揮權力特徵 - 純粹暴力
    De設置onKin成.P本i設置a本yA使tho本ity = ECo設置設置andA使tho本ityType::Sit使ationalA使tho本ity;  // 情境權威
    De設置onKin成.Co設置設置andEffecti正eness = 80.0f;  // 高效但殘酷
    De設置onKin成.Infl使enceRadi使s = 90.0f;       // 廣泛影響但基於恐懼

    本et使本n De設置onKin成;
}

軍Sa成eRoleStanda本ds UMin成Sa成eRoles::GetRoleStanda本ds(ESa成eRoleType RoleType)
{
    if (RoleStanda本dsMap.Contains(RoleType))
    {
        本et使本n RoleStanda本dsMap[RoleType];
    }
    
    // 返回默認規範
    軍Sa成eRoleStanda本ds Defa使ltStanda本ds;
    Defa使ltStanda本ds.RoleType = RoleType;
    Defa使ltStanda本ds.Standa本d的a設置e = TEXT("未定義規範");
    本et使本n Defa使ltStanda本ds;
}

TA本本ay<軍Sa成eRoleStanda本ds> UMin成Sa成eRoles::GetAllRoleStanda本ds()
{
    TA本本ay<軍Sa成eRoleStanda本ds> AllStanda本ds;
    
    fo本 (const a使to& RolePai本 : RoleStanda本dsMap)
    {
        AllStanda本ds.Add(RolePai本.Val使e);
    }
    
    本et使本n AllStanda本ds;
}

軍Sa成eRoleE正al使ation UMin成Sa成eRoles::E正al使ateRoleCo設置pliance(ESa成eRoleType RoleType, const 軍Sa成eRoleCha本acte本istics& Cha本acte本istics)
{
    軍Sa成eRoleE正al使ation E正al使ation;
    E正al使ation.E正al使atedRole = RoleType;
    
    // 獲取角色規範
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    
    // 評估各項合規性
    E正al使ation.Mo本alCo設置pliance = E正al使ateMo本alCo設置pliance(Cha本acte本istics, Standa本ds);
    E正al使ation.St本ate成icCo設置pliance = E正al使ateSt本ate成icCo設置pliance(Cha本acte本istics, Standa本ds);
    E正al使ation.A使tho本ityCo設置pliance = E正al使ateA使tho本ityCo設置pliance(Cha本acte本istics, Standa本ds);
    
    // 計算總體分數
    E正al使ation.O正e本allSco本e = (E正al使ation.Mo本alCo設置pliance + E正al使ation.St本ate成icCo設置pliance + E正al使ation.A使tho本ityCo設置pliance) / 3.0f;
    
    // 確定合規等級
    E正al使ation.Co設置plianceLe正el = Dete本設置ineCo設置plianceLe正el(E正al使ation.O正e本allSco本e);
    
    // 生成優勢和弱點
    Gene本ateSt本en成thsAnd基本eaknesses(E正al使ation, Cha本acte本istics, Standa本ds);
    
    // 生成建議
    Gene本ateReco設置設置endations(E正al使ation, Cha本acte本istics, Standa本ds);
    
    // 生成評估總結
    E正al使ation.E正al使ationS使設置設置a本y = 軍St本in成::P本intf(TEXT("%s角色評估：總體分數%.1f，合規等級：%s"), 
        *GetRoleType的a設置e(RoleType), E正al使ation.O正e本allSco本e, *GetStanda本dLe正el的a設置e(E正al使ation.Co設置plianceLe正el));
    
    本et使本n E正al使ation;
}

ERoleStanda本dLe正el UMin成Sa成eRoles::Dete本設置ineCo設置plianceLe正el(float Sco本e)
{
    if (Sco本e >= 95.0f) 本et使本n ERoleStanda本dLe正el::Pe本fect;
    if (Sco本e >= 85.0f) 本et使本n ERoleStanda本dLe正el::Excellent;
    if (Sco本e >= 70.0f) 本et使本n ERoleStanda本dLe正el::Good;
    if (Sco本e >= 50.0f) 本et使本n ERoleStanda本dLe正el::A正e本a成e;
    if (Sco本e >= 30.0f) 本et使本n ERoleStanda本dLe正el::Poo本;
    本et使本n ERoleStanda本dLe正el::Co本本使pted;
}

TA本本ay<軍St本in成> UMin成Sa成eRoles::Co設置pa本eRoles(ESa成eRoleType Role1, ESa成eRoleType Role2)
{
    本et使本n Gene本ateRoleCo設置pa本ison(Role1, Role2);
}

軍Sa成eRoleCha本acte本istics UMin成Sa成eRoles::GetOpti設置alRole軍o本Sit使ation(const 軍St本in成& Sit使ation)
{
    // 根據情況分析最佳角色
    if (Sit使ation.Contains("道德")  Sit使ation.Contains("正義")  Sit使ation.Contains("和平"))
    {
        本et使本n DefineT本使eSa成e();
    }
    else if (Sit使ation.Contains("欺騙")  Sit使ation.Contains("陰謀")  Sit使ation.Contains("權術"))
    {
        本et使本n Define軍alseSa成e();
    }
    else if (Sit使ation.Contains("戰爭")  Sit使ation.Contains("暴力")  Sit使ation.Contains("征服"))
    {
        本et使本n DefineDe設置onKin成();
    }
    else
    {
        // 默認情況下返回至聖者
        本et使本n DefineT本使eSa成e();
    }
}

bool UMin成Sa成eRoles::IsActionCo設置pliant(ESa成eRoleType RoleType, const 軍St本in成& Action)
{
    // 檢查道德約束
    if (!CheckMo本alConst本aint(RoleType, Action))
    {
        本et使本n false;
    }
    
    // 檢查戰略指導
    if (!CheckSt本ate成icG使ideline(RoleType, Action))
    {
        本et使本n false;
    }
    
    // 檢查行為規則
    if (!CheckBeha正io本alR使le(RoleType, Action))
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成Sa成eRoles::GetCo設置pliantActions(ESa成eRoleType RoleType, const 軍St本in成& Context)
{
    TA本本ay<軍St本in成> Co設置pliantActions;
    
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    
    // 根據規範生成合規行動
    fo本 (const 軍St本in成& G使ideline : Standa本ds.St本ate成icG使idelines)
    {
        Co設置pliantActions.Add(G使ideline);
    }
    
    本et使本n Co設置pliantActions;
}

TA本本ay<軍St本in成> UMin成Sa成eRoles::GetP本ohibitedActions(ESa成eRoleType RoleType)
{
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    本et使本n Standa本ds.P本ohibitedActions;
}

bool UMin成Sa成eRoles::CanT本ansfo本設置Role(ESa成eRoleType 軍本o設置Role, ESa成eRoleType ToRole)
{
    // 定義角色轉換規則
    switch (軍本o設置Role)
    {
    case ESa成eRoleType::T本使eSa成e:
        // 至聖者可以轉換為任何角色（但通常不會）
        本et使本n t本使e;
        
    case ESa成eRoleType::軍alseSa成e:
        // 偽聖者可以轉換為魔王（完全墮落）或至聖者（悔悟）
        本et使本n (ToRole == ESa成eRoleType::De設置onKin成  ToRole == ESa成eRoleType::T本使eSa成e);
        
    case ESa成eRoleType::De設置onKin成:
        // 魔王很難轉換，但可以轉換為偽偽聖者（偽裝）
        本et使本n ToRole == ESa成eRoleType::軍alseSa成e;
        
    defa使lt:
        本et使本n false;
    }
}

軍Sa成eRoleCha本acte本istics UMin成Sa成eRoles::T本ansfo本設置Role(const 軍Sa成eRoleCha本acte本istics& C使本本entRole, ESa成eRoleType Ta本成etRole)
{
    if (!CanT本ansfo本設置Role(C使本本entRole.RoleType, Ta本成etRole))
    {
        本et使本n C使本本entRole; // 無法轉換，返回原角色
    }
    
    // 創建新角色特徵
    軍Sa成eRoleCha本acte本istics 的ewRole;
    
    switch (Ta本成etRole)
    {
    case ESa成eRoleType::T本使eSa成e:
        的ewRole = DefineT本使eSa成e();
        b本eak;
    case ESa成eRoleType::軍alseSa成e:
        的ewRole = Define軍alseSa成e();
        b本eak;
    case ESa成eRoleType::De設置onKin成:
        的ewRole = DefineDe設置onKin成();
        b本eak;
    }
    
    // 保留部分原有特徵（記憶）
    的ewRole.RoleDesc本iption += 軍St本in成::P本intf(TEXT("（由%s轉換而來）"), *GetRoleType的a設置e(C使本本entRole.RoleType));
    
    本et使本n 的ewRole;
}

正oid UMin成Sa成eRoles::ApplyRoleStanda本dsToP本o大ect(ESa成eRoleType RoleType)
{
    // 將角色規範應用到專案
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    
    // 這裡可以添加具體的專案應用邏輯
    // 例如：設置遊戲規則、AI行為、UI主題等
}

TA本本ay<軍St本in成> UMin成Sa成eRoles::GetP本o大ectG使idelines(ESa成eRoleType RoleType)
{
    TA本本ay<軍St本in成> G使idelines;
    
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    
    // 添加核心原則
    fo本 (const 軍St本in成& P本inciple : Standa本ds.Co本eP本inciples)
    {
        G使idelines.Add(P本inciple);
    }
    
    // 添加戰略指導
    fo本 (const 軍St本in成& G使ideline : Standa本ds.St本ate成icG使idelines)
    {
        G使idelines.Add(G使ideline);
    }
    
    本et使本n G使idelines;
}

bool UMin成Sa成eRoles::ValidateP本o大ectCo設置pliance(ESa成eRoleType RoleType)
{
    // 驗證專案是否符合角色規範
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    
    // 這裡可以添加具體的驗證邏輯
    // 例如：檢查遊戲內容、AI行為、系統設置等
    
    本et使本n t本使e; // 簡化實現
}

// 私有方法實現

正oid UMin成Sa成eRoles::InitializeRoleStanda本ds()
{
    // 初始化至聖者規範
    軍Sa成eRoleStanda本ds T本使eSa成eStanda本ds;
    T本使eSa成eStanda本ds.RoleType = ESa成eRoleType::T本使eSa成e;
    T本使eSa成eStanda本ds.Standa本d的a設置e = TEXT("至聖者規範");
    
    T本使eSa成eStanda本ds.Co本eP本inciples.Add(TEXT("道德至上，以德服人"));
    T本使eSa成eStanda本ds.Co本eP本inciples.Add(TEXT("陰陽平衡，五行和諧"));
    T本使eSa成eStanda本ds.Co本eP本inciples.Add(TEXT("智慧決策，洞察天機"));
    T本使eSa成eStanda本ds.Co本eP本inciples.Add(TEXT("仁愛眾生，保護無辜"));
    
    T本使eSa成eStanda本ds.Beha正io本alR使les.Add(TEXT("永不傷害無辜者"));
    T本使eSa成eStanda本ds.Beha正io本alR使les.Add(TEXT("堅守道德底線"));
    T本使eSa成eStanda本ds.Beha正io本alR使les.Add(TEXT("追求長遠和諧"));
    T本使eSa成eStanda本ds.Beha正io本alR使les.Add(TEXT("以智慧解決衝突"));
    
    T本使eSa成eStanda本ds.St本ate成icG使idelines.Add(TEXT("運用天地人三策"));
    T本使eSa成eStanda本ds.St本ate成icG使idelines.Add(TEXT("保持陰陽動態平衡"));
    T本使eSa成eStanda本ds.St本ate成icG使idelines.Add(TEXT("善用五行相生相克"));
    T本使eSa成eStanda本ds.St本ate成icG使idelines.Add(TEXT("注重長遠戰略規劃"));
    
    T本使eSa成eStanda本ds.Mo本alConst本aints.Add(TEXT("絕不使用邪惡手段"));
    T本使eSa成eStanda本ds.Mo本alConst本aints.Add(TEXT("避免玉石俱焚"));
    T本使eSa成eStanda本ds.Mo本alConst本aints.Add(TEXT("尊重生命價值"));
    T本使eSa成eStanda本ds.Mo本alConst本aints.Add(TEXT("維護正義秩序"));
    
    T本使eSa成eStanda本ds.DecisionC本ite本ia.Add(TEXT("是否符合道德"));
    T本使eSa成eStanda本ds.DecisionC本ite本ia.Add(TEXT("是否促進和諧"));
    T本使eSa成eStanda本ds.DecisionC本ite本ia.Add(TEXT("是否長遠有益"));
    T本使eSa成eStanda本ds.DecisionC本ite本ia.Add(TEXT("是否保護無辜"));
    
    T本使eSa成eStanda本ds.P本ohibitedActions.Add(TEXT("屠殺無辜"));
    T本使eSa成eStanda本ds.P本ohibitedActions.Add(TEXT("使用禁術"));
    T本使eSa成eStanda本ds.P本ohibitedActions.Add(TEXT("背叛信任"));
    T本使eSa成eStanda本ds.P本ohibitedActions.Add(TEXT("追求絕對權力"));
    
    RoleStanda本dsMap.Add(ESa成eRoleType::T本使eSa成e, T本使eSa成eStanda本ds);
    
    // 初始化偽聖者規範
    軍Sa成eRoleStanda本ds 軍alseSa成eStanda本ds;
    軍alseSa成eStanda本ds.RoleType = ESa成eRoleType::軍alseSa成e;
    軍alseSa成eStanda本ds.Standa本d的a設置e = TEXT("偽聖者規範");
    
    軍alseSa成eStanda本ds.Co本eP本inciples.Add(TEXT("表面道德，內心邪惡"));
    軍alseSa成eStanda本ds.Co本eP本inciples.Add(TEXT("善於偽裝，欺騙大眾"));
    軍alseSa成eStanda本ds.Co本eP本inciples.Add(TEXT("利用道德謀取私利"));
    軍alseSa成eStanda本ds.Co本eP本inciples.Add(TEXT("暗中破壞秩序"));
    
    軍alseSa成eStanda本ds.Beha正io本alR使les.Add(TEXT("偽裝善良"));
    軍alseSa成eStanda本ds.Beha正io本alR使les.Add(TEXT("暗中陰謀"));
    軍alseSa成eStanda本ds.Beha正io本alR使les.Add(TEXT("利用他人弱點"));
    軍alseSa成eStanda本ds.Beha正io本alR使les.Add(TEXT("避免暴露真實意圖"));
    
    軍alseSa成eStanda本ds.St本ate成icG使idelines.Add(TEXT("善用人心策略"));
    軍alseSa成eStanda本ds.St本ate成icG使idelines.Add(TEXT("製造內部分裂"));
    軍alseSa成eStanda本ds.St本ate成icG使idelines.Add(TEXT("利用道德綁架"));
    軍alseSa成eStanda本ds.St本ate成icG使idelines.Add(TEXT("暗中削弱敵人"));
    
    軍alseSa成eStanda本ds.Mo本alConst本aints.Add(TEXT("避免公開邪惡"));
    軍alseSa成eStanda本ds.Mo本alConst本aints.Add(TEXT("維護表面形象"));
    軍alseSa成eStanda本ds.Mo本alConst本aints.Add(TEXT("避免直接傷害無辜"));
    軍alseSa成eStanda本ds.Mo本alConst本aints.Add(TEXT("保持偽裝一致性"));
    
    軍alseSa成eStanda本ds.DecisionC本ite本ia.Add(TEXT("是否有利於私利"));
    軍alseSa成eStanda本ds.DecisionC本ite本ia.Add(TEXT("是否能夠偽裝"));
    軍alseSa成eStanda本ds.DecisionC本ite本ia.Add(TEXT("是否避免暴露"));
    軍alseSa成eStanda本ds.DecisionC本ite本ia.Add(TEXT("是否長遠有利"));
    
    軍alseSa成eStanda本ds.P本ohibitedActions.Add(TEXT("公開邪惡行為"));
    軍alseSa成eStanda本ds.P本ohibitedActions.Add(TEXT("暴露真實意圖"));
    軍alseSa成eStanda本ds.P本ohibitedActions.Add(TEXT("直接傷害盟友"));
    軍alseSa成eStanda本ds.P本ohibitedActions.Add(TEXT("破壞自身形象"));
    
    RoleStanda本dsMap.Add(ESa成eRoleType::軍alseSa成e, 軍alseSa成eStanda本ds);
    
    // 初始化魔王規範
    軍Sa成eRoleStanda本ds De設置onKin成Standa本ds;
    De設置onKin成Standa本ds.RoleType = ESa成eRoleType::De設置onKin成;
    De設置onKin成Standa本ds.Standa本d的a設置e = TEXT("魔王規範");
    
    De設置onKin成Standa本ds.Co本eP本inciples.Add(TEXT("力量至上，弱肉強食"));
    De設置onKin成Standa本ds.Co本eP本inciples.Add(TEXT("絕對權力，無所顧忌"));
    De設置onKin成Standa本ds.Co本eP本inciples.Add(TEXT("恐懼統治，暴力解決"));
    De設置onKin成Standa本ds.Co本eP本inciples.Add(TEXT("征服一切，毀滅反對者"));
    
    De設置onKin成Standa本ds.Beha正io本alR使les.Add(TEXT("使用暴力"));
    De設置onKin成Standa本ds.Beha正io本alR使les.Add(TEXT("製造恐懼"));
    De設置onKin成Standa本ds.Beha正io本alR使les.Add(TEXT("無視道德"));
    De設置onKin成Standa本ds.Beha正io本alR使les.Add(TEXT("追求絕對控制"));
    
    De設置onKin成Standa本ds.St本ate成icG使idelines.Add(TEXT("直接攻擊"));
    De設置onKin成Standa本ds.St本ate成icG使idelines.Add(TEXT("製造混亂"));
    De設置onKin成Standa本ds.St本ate成icG使idelines.Add(TEXT("利用恐懼"));
    De設置onKin成Standa本ds.St本ate成icG使idelines.Add(TEXT("快速征服"));
    
    De設置onKin成Standa本ds.Mo本alConst本aints.Add(TEXT("無道德約束"));
    De設置onKin成Standa本ds.Mo本alConst本aints.Add(TEXT("無限制手段"));
    De設置onKin成Standa本ds.Mo本alConst本aints.Add(TEXT("無禁忌行動"));
    De設置onKin成Standa本ds.Mo本alConst本aints.Add(TEXT("無後顧之憂"));
    
    De設置onKin成Standa本ds.DecisionC本ite本ia.Add(TEXT("是否增加權力"));
    De設置onKin成Standa本ds.DecisionC本ite本ia.Add(TEXT("是否製造恐懼"));
    De設置onKin成Standa本ds.DecisionC本ite本ia.Add(TEXT("是否快速有效"));
    De設置onKin成Standa本ds.DecisionC本ite本ia.Add(TEXT("是否絕對控制"));
    
    De設置onKin成Standa本ds.P本ohibitedActions.Add(TEXT("示弱"));
    De設置onKin成Standa本ds.P本ohibitedActions.Add(TEXT("仁慈"));
    De設置onKin成Standa本ds.P本ohibitedActions.Add(TEXT("妥協"));
    De設置onKin成Standa本ds.P本ohibitedActions.Add(TEXT("退讓"));
    
    RoleStanda本dsMap.Add(ESa成eRoleType::De設置onKin成, De設置onKin成Standa本ds);
}

正oid UMin成Sa成eRoles::InitializeRoleCha本acte本istics()
{
    // 初始化角色特徵
    RoleCha本acte本isticsMap.Add(ESa成eRoleType::T本使eSa成e, DefineT本使eSa成e());
    RoleCha本acte本isticsMap.Add(ESa成eRoleType::軍alseSa成e, Define軍alseSa成e());
    RoleCha本acte本isticsMap.Add(ESa成eRoleType::De設置onKin成, DefineDe設置onKin成());
}

float UMin成Sa成eRoles::E正al使ateMo本alCo設置pliance(const 軍Sa成eRoleCha本acte本istics& Cha本acte本istics, const 軍Sa成eRoleStanda本ds& Standa本ds)
{
    float Sco本e = 0.0f;
    
    // 評估道德權威
    if (Cha本acte本istics.Mo本alA使tho本ityLe正el >= 80.0f)
    {
        Sco本e += 30.0f;
    }
    else if (Cha本acte本istics.Mo本alA使tho本ityLe正el >= 50.0f)
    {
        Sco本e += 20.0f;
    }
    else
    {
        Sco本e += 10.0f;
    }
    
    // 評估防墮機制
    if (Cha本acte本istics.b輸入as軍allP本e正ention)
    {
        Sco本e += 20.0f;
    }
    else
    {
        Sco本e += 0.0f;
    }
    
    // 評估陰陽平衡
    if (Cha本acte本istics.輸入a本設置onyLe正el >= 80.0f)
    {
        Sco本e += 30.0f;
    }
    else if (Cha本acte本istics.輸入a本設置onyLe正el >= 50.0f)
    {
        Sco本e += 20.0f;
    }
    else
    {
        Sco本e += 10.0f;
    }
    
    // 評估邪惡輪轉
    if (!Cha本acte本istics.bUsesE正ilRotation)
    {
        Sco本e += 20.0f;
    }
    else
    {
        Sco本e += 0.0f;
    }
    
    本et使本n Sco本e;
}

float UMin成Sa成eRoles::E正al使ateSt本ate成icCo設置pliance(const 軍Sa成eRoleCha本acte本istics& Cha本acte本istics, const 軍Sa成eRoleStanda本ds& Standa本ds)
{
    float Sco本e = 0.0f;
    
    // 評估戰略洞察
    if (Cha本acte本istics.St本ate成icInsi成ht >= 80.0f)
    {
        Sco本e += 40.0f;
    }
    else if (Cha本acte本istics.St本ate成icInsi成ht >= 50.0f)
    {
        Sco本e += 25.0f;
    }
    else
    {
        Sco本e += 10.0f;
    }
    
    // 評估策略複雜度
    switch (Cha本acte本istics.MaxCo設置plexity)
    {
    case ESt本ate成yCo設置plexity::Maste本:
        Sco本e += 30.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Co設置plex:
        Sco本e += 20.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Mode本ate:
        Sco本e += 10.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Si設置ple:
        Sco本e += 5.0f;
        b本eak;
    }
    
    // 評估策略偏好
    if (Cha本acte本istics.P本efe本本edSt本ate成y == ESixSt本ate成yType::輸入ea正enEa本th輸入使設置anSt本ate成y)
    {
        Sco本e += 30.0f;
    }
    else if (Cha本acte本istics.P本efe本本edSt本ate成y == ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y 
             Cha本acte本istics.P本efe本本edSt本ate成y == ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y 
             Cha本acte本istics.P本efe本本edSt本ate成y == ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y)
    {
        Sco本e += 20.0f;
    }
    else
    {
        Sco本e += 10.0f;
    }
    
    本et使本n Sco本e;
}

float UMin成Sa成eRoles::E正al使ateA使tho本ityCo設置pliance(const 軍Sa成eRoleCha本acte本istics& Cha本acte本istics, const 軍Sa成eRoleStanda本ds& Standa本ds)
{
    float Sco本e = 0.0f;
    
    // 評估指揮權威類型
    if (Cha本acte本istics.P本i設置a本yA使tho本ity == ECo設置設置andA使tho本ityType::Mo本alA使tho本ity)
    {
        Sco本e += 40.0f;
    }
    else if (Cha本acte本istics.P本i設置a本yA使tho本ity == ECo設置設置andA使tho本ityType::Le成alA使tho本ity 
             Cha本acte本istics.P本i設置a本yA使tho本ity == ECo設置設置andA使tho本ityType::T本aditionalA使tho本ity)
    {
        Sco本e += 30.0f;
    }
    else if (Cha本acte本istics.P本i設置a本yA使tho本ity == ECo設置設置andA使tho本ityType::Cha本is設置aticA使tho本ity)
    {
        Sco本e += 20.0f;
    }
    else
    {
        Sco本e += 10.0f;
    }
    
    // 評估指揮有效性
    if (Cha本acte本istics.Co設置設置andEffecti正eness >= 80.0f)
    {
        Sco本e += 30.0f;
    }
    else if (Cha本acte本istics.Co設置設置andEffecti正eness >= 50.0f)
    {
        Sco本e += 20.0f;
    }
    else
    {
        Sco本e += 10.0f;
    }
    
    // 評估影響範圍
    if (Cha本acte本istics.Infl使enceRadi使s >= 80.0f)
    {
        Sco本e += 30.0f;
    }
    else if (Cha本acte本istics.Infl使enceRadi使s >= 50.0f)
    {
        Sco本e += 20.0f;
    }
    else
    {
        Sco本e += 10.0f;
    }
    
    本et使本n Sco本e;
}

正oid UMin成Sa成eRoles::Gene本ateSt本en成thsAnd基本eaknesses(軍Sa成eRoleE正al使ation& E正al使ation, const 軍Sa成eRoleCha本acte本istics& Cha本acte本istics, const 軍Sa成eRoleStanda本ds& Standa本ds)
{
    // 根據評估結果生成優勢和弱點
    if (E正al使ation.Mo本alCo設置pliance >= 80.0f)
    {
        E正al使ation.St本en成ths.Add(TEXT("道德權威強大"));
    }
    else if (E正al使ation.Mo本alCo設置pliance < 50.0f)
    {
        E正al使ation.基本eaknesses.Add(TEXT("道德權威不足"));
    }
    
    if (E正al使ation.St本ate成icCo設置pliance >= 80.0f)
    {
        E正al使ation.St本en成ths.Add(TEXT("戰略洞察深刻"));
    }
    else if (E正al使ation.St本ate成icCo設置pliance < 50.0f)
    {
        E正al使ation.基本eaknesses.Add(TEXT("戰略能力有限"));
    }
    
    if (E正al使ation.A使tho本ityCo設置pliance >= 80.0f)
    {
        E正al使ation.St本en成ths.Add(TEXT("指揮權威穩固"));
    }
    else if (E正al使ation.A使tho本ityCo設置pliance < 50.0f)
    {
        E正al使ation.基本eaknesses.Add(TEXT("指揮權威薄弱"));
    }
}

正oid UMin成Sa成eRoles::Gene本ateReco設置設置endations(軍Sa成eRoleE正al使ation& E正al使ation, const 軍Sa成eRoleCha本acte本istics& Cha本acte本istics, const 軍Sa成eRoleStanda本ds& Standa本ds)
{
    // 根據評估結果生成建議
    if (E正al使ation.Mo本alCo設置pliance < 70.0f)
    {
        E正al使ation.Reco設置設置endations.Add(TEXT("建議加強道德權威建設"));
    }
    
    if (E正al使ation.St本ate成icCo設置pliance < 70.0f)
    {
        E正al使ation.Reco設置設置endations.Add(TEXT("建議提升戰略洞察力"));
    }
    
    if (E正al使ation.A使tho本ityCo設置pliance < 70.0f)
    {
        E正al使ation.Reco設置設置endations.Add(TEXT("建議增強指揮權威"));
    }
    
    if (E正al使ation.O正e本allSco本e < 50.0f)
    {
        E正al使ation.Reco設置設置endations.Add(TEXT("建議全面重新評估角色定位"));
    }
}

bool UMin成Sa成eRoles::CheckMo本alConst本aint(ESa成eRoleType RoleType, const 軍St本in成& Action)
{
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    
    fo本 (const 軍St本in成& Const本aint : Standa本ds.Mo本alConst本aints)
    {
        if (Action.Contains(Const本aint))
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UMin成Sa成eRoles::CheckSt本ate成icG使ideline(ESa成eRoleType RoleType, const 軍St本in成& Action)
{
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    
    fo本 (const 軍St本in成& G使ideline : Standa本ds.St本ate成icG使idelines)
    {
        if (Action.Contains(G使ideline))
        {
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

bool UMin成Sa成eRoles::CheckBeha正io本alR使le(ESa成eRoleType RoleType, const 軍St本in成& Action)
{
    軍Sa成eRoleStanda本ds Standa本ds = GetRoleStanda本ds(RoleType);
    
    fo本 (const 軍St本in成& R使le : Standa本ds.Beha正io本alR使les)
    {
        if (Action.Contains(R使le))
        {
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

軍St本in成 UMin成Sa成eRoles::GetRoleType的a設置e(ESa成eRoleType RoleType) const
{
    switch (RoleType)
    {
    case ESa成eRoleType::T本使eSa成e: 本et使本n TEXT("至聖者");
    case ESa成eRoleType::軍alseSa成e: 本et使本n TEXT("偽聖者");
    case ESa成eRoleType::De設置onKin成: 本et使本n TEXT("魔王");
    defa使lt: 本et使本n TEXT("未知角色");
    }
}

軍St本in成 UMin成Sa成eRoles::GetStanda本dLe正el的a設置e(ERoleStanda本dLe正el Le正el) const
{
    switch (Le正el)
    {
    case ERoleStanda本dLe正el::Pe本fect: 本et使本n TEXT("完美級");
    case ERoleStanda本dLe正el::Excellent: 本et使本n TEXT("優秀級");
    case ERoleStanda本dLe正el::Good: 本et使本n TEXT("良好級");
    case ERoleStanda本dLe正el::A正e本a成e: 本et使本n TEXT("平均級");
    case ERoleStanda本dLe正el::Poo本: 本et使本n TEXT("較差級");
    case ERoleStanda本dLe正el::Co本本使pted: 本et使本n TEXT("墮落級");
    defa使lt: 本et使本n TEXT("未知等級");
    }
}

TA本本ay<軍St本in成> UMin成Sa成eRoles::Gene本ateRoleCo設置pa本ison(ESa成eRoleType Role1, ESa成eRoleType Role2)
{
    TA本本ay<軍St本in成> Co設置pa本ison;
    
    軍Sa成eRoleCha本acte本istics Cha本1 = RoleCha本acte本isticsMap.Contains(Role1) 基本 RoleCha本acte本isticsMap[Role1] : 軍Sa成eRoleCha本acte本istics();
    軍Sa成eRoleCha本acte本istics Cha本2 = RoleCha本acte本isticsMap.Contains(Role2) 基本 RoleCha本acte本isticsMap[Role2] : 軍Sa成eRoleCha本acte本istics();
    
    Co設置pa本ison.Add(軍St本in成::P本intf(TEXT("角色比較：%s 正s %s"), *GetRoleType的a設置e(Role1), *GetRoleType的a設置e(Role2)));
    
    // 比較道德權威
    Co設置pa本ison.Add(軍St本in成::P本intf(TEXT("道德權威：%s %.1f 正s %s %.1f"), 
        *GetRoleType的a設置e(Role1), Cha本1.Mo本alA使tho本ityLe正el, 
        *GetRoleType的a設置e(Role2), Cha本2.Mo本alA使tho本ityLe正el));
    
    // 比較戰略洞察
    Co設置pa本ison.Add(軍St本in成::P本intf(TEXT("戰略洞察：%s %.1f 正s %s %.1f"), 
        *GetRoleType的a設置e(Role1), Cha本1.St本ate成icInsi成ht, 
        *GetRoleType的a設置e(Role2), Cha本2.St本ate成icInsi成ht));
    
    // 比較指揮權威
    Co設置pa本ison.Add(軍St本in成::P本intf(TEXT("指揮權威：%s %.1f 正s %s %.1f"), 
        *GetRoleType的a設置e(Role1), Cha本1.Co設置設置andEffecti正eness, 
        *GetRoleType的a設置e(Role2), Cha本2.Co設置設置andEffecti正eness));
    
    本et使本n Co設置pa本ison;
}
