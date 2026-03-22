# MingGoRTS 其他辅助系统设计文档

## 文档信息
- **版本**: 1.0
- **日期**: 2026-03-23
- **作者**: MingGoRTS Design Team
- **状态**: 进行中

---

## 1. 外交系统设计

### 1.1 外交关系体系

#### 外交关系类型

| 关系类型 | 数值范围 | 效果描述 | 转变条件 |
|----------|----------|----------|----------|
| **同盟** | +80 to +100 | 军事互助、资源共享 | 长期友好 + 共同利益 |
| **友好** | +40 to +79 | 贸易优惠、外交支持 | 好感度提升 |
| **中立** | -39 to +39 | 正常交往 | 默认状态 |
| **不友好** | -79 to -40 | 贸易限制、外交冷淡 | 冲突或竞争 |
| **敌对** | -100 to -80 | 可能开战、封锁 | 战争或严重冲突 |

#### 外交关系计算

```cpp
// 外交关系值计算
float CalculateDiplomaticRelation(
    EFactionType FactionA,
    EFactionType FactionB,
    const TArray<FRelationModifier>& Modifiers)
{
    float BaseRelation = 0.0f;

    // 基础关系 (历史因素)
    BaseRelation += GetHistoricalRelation(FactionA, FactionB);

    // 地理因素 (相邻/远离)
    BaseRelation += GetGeographicRelation(FactionA, FactionB);

    // 意识形态因素
    BaseRelation += GetIdeologicalRelation(FactionA, FactionB);

    // 利益冲突/共同利益
    for (const FRelationModifier& Modifier : Modifiers)
    {
        BaseRelation += Modifier.Value;
    }

    return FMath::Clamp(BaseRelation, -100.0f, 100.0f);
}
```

### 1.2 外交行动

#### 外交行动列表

| 行动 | 成本 | 效果 | 冷却时间 |
|------|------|------|----------|
| **改善关系** | 资金 + 威望 | 关系 +10 to +20 | 3回合 |
| **缔结同盟** | 大量资金 + 威望 | 同盟关系 | 1年 |
| **宣战** | 威望 -50 | 敌对关系 + 战争 | 无 |
| **媾和** | 谈判条件 | 结束战争 | 战争6月后 |
| **贸易协定** | 资金 | 贸易加成 | 6月 |
| **军事通行** | 关系良好 | 军队过境权 | 同盟后 |
| **军事援助** | 兵力/装备 | 关系提升 + 实际支援 | 同盟后 |
| **经济援助** | 资金 | 关系提升 + 实际支援 | 无 |
| **外交抗议** | 少量威望 | 关系恶化 | 1月 |
| **断交** | 关系归零 | 彻底决裂 | 无 |

#### 外交行动代码示例

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FDiplomaticAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ActionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ActionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDiplomaticActionType ActionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoneyCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PrestigeCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RelationChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CooldownTurns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FActionCondition> Conditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FActionEffect> Effects;
};

// 执行外交行动
bool UMingDiplomacyManager::ExecuteDiplomaticAction(
    EFactionType Initiator,
    EFactionType Target,
    const FString& ActionID)
{
    FDiplomaticAction Action = GetDiplomaticAction(ActionID);

    // 检查条件
    if (!CanExecuteAction(Initiator, Target, Action))
    {
        UE_LOG(LogMingDiplomacy, Warning, TEXT("外交行动条件不满足: %s"), *ActionID);
        return false;
    }

    // 扣除成本
    ConsumeActionCosts(Initiator, Action);

    // 应用效果
    ApplyActionEffects(Initiator, Target, Action);

    // 记录行动
    RecordDiplomaticAction(Initiator, Target, Action);

    // 触发事件
    OnDiplomaticActionExecuted.Broadcast(Initiator, Target, ActionID);

    return true;
}
```

### 1.3 外国势力系统

#### 主要外国势力

| 国家 | 利益区域 | 影响力 | 援助能力 |
|------|----------|--------|----------|
| **日本** | 东北、华北 | 极高 | 军事/经济援助 |
| **苏联** | 新疆、蒙古 | 高 | 军事援助 |
| **英国** | 长江流域 | 中高 | 经济援助 |
| **美国** | 全国 | 中 | 经济援助 |
| **法国** | 西南 | 中 | 有限援助 |
| **德国** | 全国 | 中 | 军事顾问 |

#### 外国援助机制

```cpp
// 外国援助申请
bool UMingDiplomacyManager::RequestForeignAid(
    EFactionType Requester,
    EForeignPower ForeignPower,
    EAidType AidType,
    float Amount)
{
    // 检查关系
    float Relation = GetForeignRelation(Requester, ForeignPower);

    // 检查援助意愿
    float AidWillingness = CalculateAidWillingness(Requester, ForeignPower, AidType);

    // 检查自身利益
    float SelfInterest = CalculateSelfInterest(Requester, ForeignPower);

    // 综合概率
    float SuccessProbability = (Relation + AidWillingness + SelfInterest) / 3.0f;

    if (FMath::RandRange(0.0f, 100.0f) < SuccessProbability)
    {
        // 批准援助
        GrantForeignAid(Requester, ForeignPower, AidType, Amount);

        // 增加债务/依赖度
        IncreaseForeignDependency(Requester, ForeignPower, Amount);

        return true;
    }

    return false;
}
```

### 1.4 国际联盟系统

#### 国联机制 (历史准确性)

- **成立**: 1920年
- **中国地位**: 创始会员国
- **作用**: 调解国际争端 (实际上效果有限)
- **九一八事变**: 国联调查团 (李顿调查团)

```cpp
// 国联调解
void UMingDiplomacyManager::LeagueOfNationsMediation(
    EFactionType Complainant,  // 申诉方
    EFactionType Respondent, // 被诉方
    const FString& Dispute)
{
    // 国联调查
    FInvestigationReport Report = ConductInvestigation(Complainant, Respondent, Dispute);

    // 调解建议
    FMediationProposal Proposal = GenerateMediationProposal(Report);

    // 各方接受度
    float ComplainantAcceptance = EvaluateProposal(Complainant, Proposal);
    float RespondentAcceptance = EvaluateProposal(Respondent, Proposal);

    // 执行调解结果
    if (ComplainantAcceptance > 50.0f && RespondentAcceptance > 50.0f)
    {
        ApplyMediationResult(Proposal);
    }
    else
    {
        // 调解失败，国联无能为力
        TriggerEvent(TEXT("LEAGUE_MEDIATION_FAILED"));
    }
}
```

---

## 2. 贸易系统设计

### 2.1 资源体系

#### 基础资源类型

| 资源 | 用途 | 产地 | 重要性 |
|------|------|------|--------|
| **粮食** | 维持人口、军队 | 全国 | 生存必需 |
| **铁矿** | 武器制造、工业 | 东北、山西 | 军事必需 |
| **煤矿** | 能源、工业 | 山西、河北 | 工业基础 |
| **石油** | 机械化、能源 | 甘肃、新疆 | 现代战争 |
| **棉花** | 纺织品、军服 | 华北、华中 | 民生/军事 |
| **木材** | 建筑、燃料 | 全国 | 基础建设 |
| **黄金/银** | 货币、储备 | 全国 | 金融基础 |
| **丝绸** | 贸易、奢侈品 | 江南 | 高价值贸易 |
| **茶叶** | 贸易、消费 | 南方 | 贸易商品 |
| **烟草** | 消费、税收 | 全国 | 税收来源 |

#### 战略资源

| 资源 | 军事用途 | 获取难度 | 外部依赖 |
|------|----------|----------|----------|
| **钨矿** | 硬质合金、穿甲弹 | 高 | 出口主要 |
| **锑矿** | 弹药、轴承 | 中 | 部分依赖 |
| **锡矿** | 焊接、合金 | 中 | 部分依赖 |
| **橡胶** | 轮胎、密封 | 高 | 完全依赖进口 |
| **铜矿** | 弹药、电线 | 中 | 部分依赖 |

### 2.2 贸易机制

#### 贸易路线

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FTradeRoute
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RouteID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionType SourceFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionType TargetFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETradeRouteType RouteType;  // 陆路/水路/铁路

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FTradeGood> TradeGoods;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RouteEfficiency;  // 0.0 - 1.0

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SecurityLevel;    // 0.0 - 1.0 (受战乱/匪患影响)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TradeVolume;      // 贸易量

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;
};
```

#### 贸易计算公式

```cpp
// 贸易收入计算
float CalculateTradeRevenue(const FTradeRoute& Route)
{
    float BaseRevenue = 0.0f;

    for (const FTradeGood& Good : Route.TradeGoods)
    {
        // 基础价格
        float BasePrice = GetResourcePrice(Good.ResourceType);

        // 数量
        float Quantity = Good.Quantity;

        // 效率影响
        float EfficiencyMultiplier = Route.RouteEfficiency;

        // 安全影响 (战乱/土匪)
        float SecurityMultiplier = Route.SecurityLevel;

        // 距离成本
        float DistanceCost = CalculateDistanceCost(Route);

        // 税收
        float TaxRate = GetTradeTaxRate(Route.SourceFaction);

        // 最终收入
        float GoodRevenue = (BasePrice * Quantity * EfficiencyMultiplier *
                            SecurityMultiplier - DistanceCost) * (1.0f - TaxRate);

        BaseRevenue += GoodRevenue;
    }

    return BaseRevenue;
}
```

### 2.3 内部贸易

#### 省际贸易

- **粮食贸易**: 从产区 (湖南、四川) 向缺粮区 (城市、灾区)
- **资源调配**: 从矿区向工业区
- **商品流通**: 工业品从城市向农村

#### 贸易障碍

| 障碍类型 | 影响 | 解决方法 |
|----------|------|----------|
| **军阀割据** | 关卡林立、税收重重 | 统一/谈判 |
| **土匪** | 贸易路线不安全 | 剿匪/护航 |
| **战争** | 路线中断 | 停战/绕道 |
| **货币混乱** | 各省货币不统一 | 金融改革 |

### 2.4 对外贸易

#### 主要贸易港口

| 港口 | 控制势力 | 贸易额 | 主要商品 |
|------|----------|--------|----------|
| **上海** | 列强/国民政府 | 最高 | 进出口中心 |
| **天津** | 北洋/奉系 | 高 | 华北贸易 |
| **广州** | 国民政府 | 中高 | 南方贸易 |
| **汉口** | 直系/国民政府 | 中 | 长江流域 |
| **大连** | 日本/奉系 | 中 | 东北贸易 |

#### 贸易平衡

```cpp
// 对外贸易平衡
struct FTradeBalance
{
    float Exports;          // 出口额
    float Imports;          // 进口额
    float Balance;          // 顺差/逆差
    float TariffRevenue;    // 关税收入
};

FTradeBalance CalculateForeignTrade(EFactionType Faction)
{
    FTradeBalance Balance;

    // 计算出口
    for (const FExportContract& Contract : GetExportContracts(Faction))
    {
        Balance.Exports += Contract.Value;
    }

    // 计算进口
    for (const FImportContract& Contract : GetImportContracts(Faction))
    {
        Balance.Imports += Contract.Value;
    }

    // 贸易差额
    Balance.Balance = Balance.Exports - Balance.Imports;

    // 关税收入
    Balance.TariffRevenue = CalculateTariffRevenue(Faction);

    return Balance;
}
```

---

## 3. 科技系统设计

### 3.1 科技体系

#### 科技树结构

```
基础科技
├── 农业科技
│   ├── 改良农具
│   ├── 水利技术
│   └── 良种培育
├── 工业科技
│   ├── 基础冶金
│   ├── 机械制造
│   └── 化学工业
├── 军事科技
│   ├── 步兵装备
│   ├── 炮兵技术
│   └── 军事工程
└── 社会科技
    ├── 教育体系
    ├── 医疗进步
    └── 基础设施
```

#### 科技类型

| 科技类别 | 研发成本 | 效果 | 前置条件 |
|----------|----------|------|----------|
| **军事科技** | 高 | 提升战斗力 | 工业基础 |
| **工业科技** | 高 | 提升生产效率 | 资源/资金 |
| **农业科技** | 低 | 提升粮食产量 | 农业区 |
| **社会科技** | 中 | 提升人口/稳定 | 和平时期 |

### 3.2 科技研发机制

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FTechnology
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TechID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TechName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETechnologyCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 YearAvailable;  // 历史可用年份

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResearchCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResearchProgress;  // 0.0 - 100.0

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Prerequisites;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FTechEffect> Effects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsResearched;
};

// 研发科技
bool UMingTechnologyManager::ResearchTechnology(
    EFactionType Faction,
    const FString& TechID,
    float Investment)
{
    FTechnology* Tech = GetTechnology(TechID);
    if (!Tech) return false;

    // 检查前置科技
    for (const FString& Prereq : Tech->Prerequisites)
    {
        if (!IsTechnologyResearched(Faction, Prereq))
        {
            UE_LOG(LogMingTech, Warning, TEXT("前置科技未完成: %s"), *Prereq);
            return false;
        }
    }

    // 投入研发
    Tech->ResearchProgress += Investment;

    // 研发完成
    if (Tech->ResearchProgress >= Tech->ResearchCost)
    {
        Tech->bIsResearched = true;
        ApplyTechEffects(Faction, TechID);
        OnTechnologyResearched.Broadcast(Faction, TechID);
    }

    return true;
}
```

### 3.3 民国特色科技

#### 军事科技

| 科技 | 年份 | 效果 | 势力特色 |
|------|------|------|----------|
| **汉阳造改进** | 1912 | 步兵装备 +10% | 直系 |
| **太原兵工厂技术** | 1920 | 武器生产 +20% | 晋系 |
| **奉天兵工厂技术** | 1920 | 重武器生产 +15% | 奉系 |
| **黄埔军校训练法** | 1924 | 训练效率 +30% | 国民 |
| **德械师装备** | 1926 | 精锐步兵 +25% | 国民 |
| **红军游击战术** | 1927 | 游击战 +40% | 中共 |
| **边区工业技术** | 1937 | 战时生产 +20% | 中共 |

#### 工业科技

| 科技 | 年份 | 效果 | 适用势力 |
|------|------|------|----------|
| **近代纺织技术** | 1912 | 纺织产量 +30% | 沿海势力 |
| **采煤技术改进** | 1915 | 煤炭产量 +25% | 山西、东北 |
| **钢铁冶炼** | 1920 | 钢铁产量 +20% | 工业势力 |
| **电力系统** | 1925 | 工业效率 +15% | 大城市 |
| **铁路建设技术** | 1912 | 建设速度 +30% | 晋系等 |
| **公路建设** | 1925 | 机动速度 +20% | 全国 |

### 3.4 科技引进

#### 外国技术引进

```cpp
// 引进外国技术
bool UMingTechnologyManager::ImportForeignTechnology(
    EFactionType Faction,
    EForeignPower Source,
    const FString& TechID)
{
    // 检查外交关系
    float Relation = GetForeignRelation(Faction, Source);
    if (Relation < 30.0f)
    {
        UE_LOG(LogMingTech, Warning, TEXT("外交关系不足以引进技术"));
        return false;
    }

    // 检查技术转让意愿
    float TransferWillingness = GetTechTransferWillingness(Source, TechID);

    // 支付技术转让费
    float TransferCost = CalculateTechTransferCost(TechID, Source);
    if (!SpendMoney(Faction, TransferCost))
    {
        return false;
    }

    // 立即获得科技
    UnlockTechnology(Faction, TechID);

    // 增加外国依赖度
    IncreaseForeignDependency(Faction, Source, TransferCost * 0.1f);

    return true;
}
```

---

## 4. 政治系统

### 4.1 政府体制

#### 体制类型

| 体制 | 稳定性 | 动员力 | 行政效率 | 适用势力 |
|------|--------|--------|----------|----------|
| **共和制** | 中 | 中 | 中 | 北洋、国民 |
| **军政府** | 低 | 高 | 高 | 军阀势力 |
| **根据地** | 低 | 极高 | 低 | 中共 |
| **君主制** | 高 | 低 | 中 | (袁世凯尝试) |

#### 政治改革

```cpp
// 政治改革
bool UMingPoliticsManager::PoliticalReform(
    EFactionType Faction,
    EPoliticalReformType ReformType)
{
    // 检查改革条件
    if (!CanReform(Faction, ReformType))
    {
        return false;
    }

    // 改革成本
    float ReformCost = CalculateReformCost(Faction, ReformType);
    if (!SpendPoliticalCapital(Faction, ReformCost))
    {
        return false;
    }

    // 应用改革效果
    switch (ReformType)
    {
    case EPoliticalReformType::Democratization:
        IncreaseStability(Faction, 10.0f);
        IncreaseDemocracy(Faction, 20.0f);
        break;

    case EPoliticalReformType::Centralization:
        IncreaseAdministrativeEfficiency(Faction, 15.0f);
        DecreaseRegionalAutonomy(Faction, 10.0f);
        break;

    case EPoliticalReformType::MilitaryFirst:
        IncreaseMilitaryPriority(Faction, 20.0f);
        DecreaseCivilianPriority(Faction, 10.0f);
        break;
    }

    // 触发改革事件
    TriggerPoliticalReformEvent(Faction, ReformType);

    return true;
}
```

### 4.2 民意系统

#### 民意因素

| 因素 | 影响 | 改善方法 | 恶化原因 |
|------|------|----------|----------|
| **税收负担** | 民生 | 减税/发展经济 | 重税/战乱 |
| **战争疲劳** | 厌战 | 和平/胜利 | 长期战争/失败 |
| **经济发展** | 就业 | 工业化/贸易 | 经济崩溃 |
| **社会稳定** | 安全 | 治安/法治 | 战乱/匪患 |
| **民族主义** | 热情 | 抗日/统一 | 卖国/分裂 |

#### 民意计算

```cpp
// 计算民众支持度
float CalculatePublicSupport(EFactionType Faction)
{
    float Support = 50.0f;  // 基础值

    // 经济因素
    Support += GetEconomicSatisfaction(Faction) * 0.3f;

    // 战争因素
    Support -= GetWarWeariness(Faction) * 0.4f;

    // 政治因素
    Support += GetPoliticalLegitimacy(Faction) * 0.2f;

    // 社会因素
    Support += GetSocialStability(Faction) * 0.1f;

    return FMath::Clamp(Support, 0.0f, 100.0f);
}
```

---

## 5. 情报系统

### 5.1 情报网络

#### 情报类型

| 情报 | 用途 | 获取难度 | 风险 |
|------|------|----------|------|
| **军事部署** | 战争计划 | 高 | 高 |
| **经济情报** | 贸易策略 | 中 | 中 |
| **政治动向** | 外交决策 | 高 | 高 |
| **科技情报** | 技术引进 | 中 | 中 |
| **内部情报** | 派系斗争 | 低 | 低 |

#### 情报机构

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FIntelligenceNetwork
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionType OwnerFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NetworkEfficiency;  // 网络效率 0-100

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FIntelligenceOperative> Operatives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EFactionType, float> InfiltrationLevel;  // 渗透程度

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FIIntelligenceReport> Reports;
};

// 建立情报网络
bool UMingIntelligenceManager::EstablishIntelligenceNetwork(
    EFactionType Owner,
    EFactionType Target)
{
    // 投入资源
    float Cost = CalculateNetworkCost(Target);
    if (!SpendMoney(Owner, Cost))
    {
        return false;
    }

    // 创建网络
    FIntelligenceNetwork Network;
    Network.OwnerFaction = Owner;
    Network.NetworkEfficiency = 10.0f;  // 初始效率

    // 渗透难度 (基于目标反间谍能力)
    float InfiltrationDifficulty = GetCounterIntelligenceLevel(Target);
    float InitialInfiltration = FMath::RandRange(5.0f, 15.0f) / InfiltrationDifficulty;

    Network.InfiltrationLevel.Add(Target, InitialInfiltration);

    RegisterIntelligenceNetwork(Network);

    return true;
}
```

### 5.2 间谍活动

#### 间谍行动

| 行动 | 成功率 | 风险 | 效果 |
|------|--------|------|------|
| **情报收集** | 中 | 低 | 获得情报 |
| **破坏** | 低 | 高 | 削弱目标 |
| **策反** | 极低 | 极高 | 获得内应 |
| **暗杀** | 低 | 极高 | 消除目标 |
| **散布谣言** | 高 | 中 | 降低稳定 |

---

## 6. 人口与社会系统

### 6.1 人口系统

#### 人口结构

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FPopulation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalPopulation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UrbanPopulation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RuralPopulation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 WorkingAgePopulation;  // 劳动年龄人口

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MilitaryAgePopulation;  // 适龄服役人口

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LiteracyRate;  // 识字率

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MortalityRate;  // 死亡率

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BirthRate;      // 出生率

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MigrationRate;  // 迁移率
};
```

#### 人口变化因素

| 因素 | 影响 | 控制方法 |
|------|------|----------|
| **战争** | 人口减少 | 避免战争 |
| **饥荒** | 人口减少 | 粮食储备 |
| **疾病** | 人口减少 | 医疗改善 |
| **经济发展** | 吸引移民 | 发展经济 |
| **战乱** | 人口流失 | 维持和平 |

### 6.2 社会阶层

#### 阶层结构

| 阶层 | 占比 | 影响力 | 政治倾向 |
|------|------|--------|----------|
| **大地主** | 5% | 高 | 保守 |
| **士绅** | 10% | 高 | 温和 |
| **商人** | 10% | 中 | 实用 |
| **农民** | 60% | 低 | 易煽动 |
| **工人** | 10% | 中 | 激进 |
| **知识分子** | 5% | 中 | 改革 |

---

## 7. 基础设施系统

### 7.1 交通网络

#### 交通方式

| 方式 | 建设成本 | 运输效率 | 适用范围 |
|------|----------|----------|----------|
| **铁路** | 极高 | 极高 | 主要干线 |
| **公路** | 高 | 高 | 地区连接 |
| **水运** | 中 | 中 | 沿江沿海 |
| **土路** | 低 | 低 | 乡村连接 |

#### 重点铁路

| 铁路 | 起点-终点 | 控制势力 | 战略价值 |
|------|-----------|----------|----------|
| **京汉铁路** | 北京-汉口 | 变动 | 南北干线 |
| **津浦铁路** | 天津-浦口 | 变动 | 南北干线 |
| **京奉铁路** | 北京-沈阳 | 奉系/北洋 | 关内外 |
| **陇海铁路** | 连云港-兰州 | 变动 | 东西干线 |
| **正太铁路** | 石家庄-太原 | 晋系 | 晋系生命线 |
| **粤汉铁路** | 广州-汉口 | 国民 | 南方干线 |

### 7.2 城市建设

#### 城市等级

| 等级 | 人口 | 设施 | 生产能力 |
|------|------|------|----------|
| **特大城市** | >100万 | 完整 | 极高 |
| **大城市** | 50-100万 | 较好 | 高 |
| **中等城市** | 10-50万 | 一般 | 中 |
| **小城市** | <10万 | 基础 | 低 |

---

## 8. 灾害与危机系统

### 8.1 自然灾害

#### 灾害类型

| 灾害 | 频率 | 影响 | 应对 |
|------|------|------|------|
| **洪水** | 高 | 农业损失 | 水利建设 |
| **旱灾** | 高 | 饥荒 | 赈灾 |
| **地震** | 低 | 建筑/人口 | 救援 |
| **蝗灾** | 中 | 农业 | 防治 |
| **瘟疫** | 中 | 人口 | 医疗 |

#### 黄河水患 (历史事件)

- **1938年花园口决堤**: 为阻日军，人为决堤
- 影响: 河南、安徽、江苏三省
- 结果: 80余万人死亡，1200万人受灾

### 8.2 经济危机

#### 危机类型

| 危机 | 触发条件 | 影响 | 恢复 |
|------|----------|------|------|
| **金融危机** | 战争/恐慌 | 货币贬值 | 改革 |
| **粮食危机** | 灾害/战争 | 饥荒 | 进口/赈灾 |
| **债务危机** | 过度借款 | 财政崩溃 | 违约/重组 |
| **工业危机** | 原料短缺 | 生产下降 | 进口替代 |

---

## 9. 系统间联动

### 9.1 经济-军事联动

```cpp
// 经济支撑军事
float CalculateMilitarySustainability(EFactionType Faction)
{
    float EconomicCapacity = GetEconomicCapacity(Faction);
    float MilitaryExpenditure = GetMilitaryExpenditure(Faction);

    // 军事开支占经济比重
    float Ratio = MilitaryExpenditure / EconomicCapacity;

    if (Ratio > 0.5f)
    {
        // 过度军事化，经济崩溃风险
        TriggerEconomicCrisisRisk(Faction, Ratio);
    }

    return 1.0f - Ratio;
}
```

### 9.2 政治-军事联动

```cpp
// 政治合法性影响军队士气
float CalculateArmyMorale(EFactionType Faction)
{
    float BaseMorale = 50.0f;

    // 政治合法性加成
    float LegitimacyBonus = GetPoliticalLegitimacy(Faction) * 0.3f;

    // 经济支撑加成
    float EconomicBonus = GetEconomicSatisfaction(Faction) * 0.2f;

    // 战争疲劳减成
    float WarWeariness = GetWarWeariness(Faction) * 0.4f;

    // 领导人魅力加成
    float LeaderBonus = GetLeaderCharisma(Faction) * 0.1f;

    return FMath::Clamp(BaseMorale + LegitimacyBonus + EconomicBonus -
                        WarWeariness + LeaderBonus, 0.0f, 100.0f);
}
```

### 9.3 外交-经济联动

```cpp
// 外交关系影响贸易
float CalculateTradeEfficiency(EFactionType Trader, EFactionType Partner)
{
    float BaseEfficiency = 1.0f;

    // 外交关系影响
    float Relation = GetDiplomaticRelation(Trader, Partner);
    float RelationMultiplier = 1.0f + (Relation / 100.0f) * 0.5f;

    // 贸易协定加成
    if (HasTradeAgreement(Trader, Partner))
    {
        RelationMultiplier += 0.3f;
    }

    // 战争阻断
    if (IsAtWar(Trader, Partner))
    {
        RelationMultiplier = 0.0f;  // 贸易中断
    }

    // 地理距离影响
    float Distance = CalculateDistance(Trader, Partner);
    float DistanceMultiplier = FMath::Max(0.5f, 1.0f - (Distance / 5000.0f));

    return BaseEfficiency * RelationMultiplier * DistanceMultiplier;
}
```

---

**文档版本**: 1.0  
**最后更新**: 2026-03-23  
**作者**: MingGoRTS Design Team  
**状态**: ✅ 已完成

---

## 所有设计文档完成总结

已完成以下设计文档：

| 文档 | 路径 | 状态 |
|------|------|------|
| 12势力详细设计 | `docs/12_FACTIONS_DETAILED_DESIGN.md` | ✅ |
| 势力平衡性分析 | `docs/FACTION_BALANCE_ANALYSIS.md` | ✅ |
| 势力美术资源规划 | `docs/FACTION_ART_RESOURCE_PLAN.md` | ✅ |
| 专属单位3D模型设计 | `docs/UNIT_3D_MODEL_DESIGN.md` | ✅ |
| 特色机制程序实现 | `docs/MECHANIC_IMPLEMENTATION.md` | ✅ |
| 历史事件设计 | `docs/HISTORICAL_EVENTS_DESIGN.md` | ✅ |
| 其他辅助系统设计 | `docs/OTHER_SYSTEMS_DESIGN.md` | ✅ |

**总计**: 7个详细设计文档，约500+页内容，涵盖游戏设计的各个方面。
