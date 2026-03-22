# Epic 9 历史事件实施总指南

## 概述

**目标**: 为12个势力实现完整的势力专属历史事件系统  
**适用范围**: 动态历史事件、势力专属事件、随机事件、连锁事件  
**开发周期**: 约2-3个月  
**团队配置**: 建议2-3人程序员 + 1名历史顾问

---

## 第一部分: 事件系统架构

### 1.1 核心事件架构

```
MingEventSystem (事件系统核心)
  ├─ 事件管理器 (EventManager)
  │   ├─ 事件注册/注销
  │   ├─ 事件触发器
  │   ├─ 事件调度器
  │   └─ 事件存储
  ├─ 事件条件系统 (ConditionSystem)
  │   ├─ 时间条件
  │   ├─ 势力条件
  │   ├─ 资源条件
  │   ├─ 关系条件
  │   └─ 随机条件
  ├─ 事件效果系统 (EffectSystem)
  │   ├─ 资源效果
  │   ├─ 关系效果
  │   ├─ 单位效果
  │   ├─ 建筑效果
  │   └─ 剧情效果
  └─ 势力事件管理器 (FactionEventManager)
      ├─ 北洋事件集
      ├─ 国民政府事件集
      ├─ 共产党事件集
      ├─ 军阀事件集
      └─ 边疆事件集
```

### 1.2 事件数据结构

```cpp
USTRUCT(BlueprintType)
struct FHistoricalEvent
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString EventID;                    // 唯一标识
    
    UPROPERTY()
    EFactionType Faction;               // 所属势力
    
    UPROPERTY()
    EEventType Type;                    // 事件类型
    
    UPROPERTY()
    FText Title;                        // 事件标题
    
    UPROPERTY()
    FText Description;                  // 事件描述
    
    UPROPERTY()
    TArray<FEventCondition> Conditions; // 触发条件
    
    UPROPERTY()
    TArray<FEventChoice> Choices;       // 玩家选择
    
    UPROPERTY()
    TArray<FEventEffect> Effects;       // 事件效果
    
    UPROPERTY()
    float Probability;                  // 基础触发概率
    
    UPROPERTY()
    bool bIsRepeatable;                 // 是否可重复
    
    UPROPERTY()
    float CooldownDays;                 // 冷却时间（游戏天数）
    
    UPROPERTY()
    TMap<EFactionType, float> FactionWeights; // 对AI势力的权重影响
};
```

---

## 第二部分: 势力专属事件实现

### 2.1 北洋政府事件集

#### 核心事件列表
```cpp
// 1. 袁世凯称帝
USTRUCT()
struct FEvent_YuanShikaiEmperor : public FHistoricalEvent
{
    // 条件: 北洋政府控制北京 + 声望高 + 1915-1916年
    // 效果: 内部不稳 + 其他势力敌视 + 可能的内战
    // 选择:
    //   A. 支持称帝 -> 获得短期强力加成，长期分裂风险
    //   B. 反对 -> 失去部分控制，维护统一
    //   C. 观望 -> 中性结果
};

// 2. 二十一条
USTRUCT()
struct FEvent_TwentyOneDemands : public FHistoricalEvent
{
    // 条件: 北洋政府 + 日本关系 + 1915年
    // 效果: 民族压力 vs 日本援助
    // 选择:
    //   A. 接受部分 -> 日本援助，民族声望下降
    //   B. 拒绝 -> 民族英雄，日本敌视
    //   C. 拖延 -> 外交博弈
};

// 3. 护国运动应对
USTRUCT()
struct FEvent_AntiMonarchyMovement : public FHistoricalEvent
{
    // 条件: 称帝后发生
    // 效果: 内战风险
    // 选择:
    //   A. 武力镇压 -> 军事冲突
    //   B. 取消帝制 -> 挽回部分声望
    //   C. 谈判 -> 妥协方案
};
```

### 2.2 国民政府事件集

#### 核心事件列表
```cpp
// 1. 北伐开始
USTRUCT()
struct FEvent_NorthernExpedition : public FHistoricalEvent
{
    // 条件: 国民政府成立 + 准备就绪 + 1926年
    // 效果: 开启北伐战争
    // 特殊: 可联合或对抗其他势力
    // 选择:
    //   A. 全面北伐 -> 军事扩张
    //   B. 政治谈判 -> 和平统一可能性
    //   C. 巩固南方 -> 长期发展
};

// 2. 四一二政变
USTRUCT()
struct FEvent_ShanghaiMassacre : public FHistoricalEvent
{
    // 条件: 北伐进行中 + 共产党合作
    // 效果: 国共分裂
    // 选择:
    //   A. 清党 -> 失去共产党支持，获得右翼支持
    //   B. 继续合作 -> 内部矛盾
    //   C. 温和处理 -> 平衡方案
};

// 3. 南京国民政府成立
USTRUCT()
struct FEvent_NanjingGovernment : public FHistoricalEvent
{
    // 条件: 北伐成功
    // 效果: 法理正统性提升
    // 解锁: 更多外交选项
};
```

### 2.3 共产党事件集

#### 核心事件列表
```cpp
// 1. 南昌起义
USTRUCT()
struct FEvent_NanchangUprising : public FHistoricalEvent
{
    // 条件: 国共分裂后
    // 效果: 武装斗争开始
    // 选择:
    //   A. 发动起义 -> 建立红军
    //   B. 地下工作 -> 潜伏发展
    //   C. 撤退农村 -> 农村包围城市
};

// 2. 秋收起义
USTRUCT()
struct FEvent_AutumnHarvestUprising : public FHistoricalEvent
{
    // 条件: 特定年份 + 农村控制
    // 效果: 扩大根据地
    // 特殊: 开启游击战模式
};

// 3. 长征
USTRUCT()
struct FEvent_LongMarch : public FHistoricalEvent
{
    // 条件: 根据地失守 + 生存危机
    // 效果: 战略转移
    // 特殊: 史诗级事件链
    //   - 血战湘江
    //   - 遵义会议
    //   - 四渡赤水
    //   - 飞夺泸定桥
    //   - 过雪山草地
    //   - 到达陕北
    // 选择影响: 领导力、军队损失、目的地
};

// 4. 根据地建设
USTRUCT()
struct FEvent_BaseAreaConstruction : public FHistoricalEvent
{
    // 条件: 控制农村地区
    // 效果: 发展生产、建立政权
    // 重复性: 是，可多次触发
};
```

### 2.4 军阀专属事件

#### 奉系事件
```cpp
// 1. 皇姑屯事件
USTRUCT()
struct FEvent_HuanggutunIncident : public FHistoricalEvent
{
    // 条件: 张作霖时期 + 日本关系紧张
    // 效果: 领导人更替
    // 后续: 张学良继位事件
};

// 2. 东北易帜
USTRUCT()
struct FEvent_NortheastFlagChange : public FHistoricalEvent
{
    // 条件: 张作霖死后 + 张学良继位
    // 效果: 名义上服从国民政府
    // 选择:
    //   A. 易帜 -> 与国民政府关系改善
    //   B. 独立 -> 孤立但自治
    //   C. 日本合作 -> 成为傀儡
};

// 3. 九一八应对
USTRUCT()
struct FEvent_MukdenIncidentResponse : public FHistoricalEvent
{
    // 条件: 1931年 + 日本侵略
    // 效果: 东北沦陷或抵抗
    // 选择:
    //   A. 坚决抵抗 -> 战争状态
    //   B. 不抵抗 -> 失地，保存实力
    //   C. 有限抵抗 -> 平衡方案
};
```

#### 直系事件
```cpp
// 1. 吴佩孚东山再起
USTRUCT()
struct FEvent_WuPeifuComeback : public FHistoricalEvent
{
    // 条件: 直系衰败后 + 特定条件
    // 效果: 重建势力
    // 特殊: 文人军阀特色事件
};

// 2. 洛阳兵变
USTRUCT()
struct FEvent_LuoyangMutiny : public FHistoricalEvent
{
    // 条件: 内部不稳
    // 效果: 分裂或统一
};
```

#### 川系事件
```cpp
// 1. 防区制冲突
USTRUCT()
struct FEvent_SichuanDefenseZoneConflict : public FHistoricalEvent
{
    // 条件: 川系内部
    // 效果: 内战
    // 特点: 频繁发生
    // 重复性: 是
};

// 2. 刘湘统一四川
USTRUCT()
struct FEvent_LiuXiangUnifiesSichuan : public FHistoricalEvent
{
    // 条件: 击败其他川系军阀
    // 效果: 四川统一
    // 后续: 抗日出兵等事件
};
```

### 2.5 边疆势力事件

#### 马家军事件
```cpp
// 1. 河州事变
USTRUCT()
struct FEvent_HezhouIncident : public FHistoricalEvent
{
    // 条件: 马家军统治早期
    // 效果: 确立统治
};

// 2. 青海建省
USTRUCT()
struct FEvent_QinghaiProvince : public FHistoricalEvent
{
    // 条件: 马步芳控制青海
    // 效果: 正式建制
};
```

---

## 第三部分: 通用事件类型

### 3.1 随机事件池

#### 政治事件
```cpp
// 1. 内阁危机
USTRUCT()
struct FEvent_CabinetCrisis : public FHistoricalEvent
{
    // 随机触发
    // 效果: 政治不稳定
    // 选择: 重组内阁 / 维持现状 / 强人政治
};

// 2. 地方叛乱
USTRUCT()
struct FEvent_LocalRebellion : public FHistoricalEvent
{
    // 条件: 控制力低或经济差
    // 效果: 军事冲突
    // 重复性: 是
};
```

#### 经济事件
```cpp
// 1. 经济繁荣
USTRUCT()
struct FEvent_EconomicBoom : public FHistoricalEvent
{
    // 条件: 稳定统治 + 和平时期
    // 效果: 资源产量提升
    // 持续时间: 有限
};

// 2. 金融危机
USTRUCT()
struct FEvent_FinancialCrisis : public FHistoricalEvent
{
    // 随机触发
    // 效果: 资源短缺
    // 选择: 紧缩政策 / 印钞 / 外债
};

// 3. 饥荒
USTRUCT()
struct FEvent_Famine : public FHistoricalEvent
{
    // 条件: 天灾 + 准备不足
    // 效果: 人口损失 + 不稳定
    // 选择: 赈灾 / 忽视 / 强制征粮
};
```

#### 外交事件
```cpp
// 1. 外国援助
USTRUCT()
struct FEvent_ForeignAid : public FHistoricalEvent
{
    // 条件: 友好关系 + 需求
    // 效果: 获得资源/武器
};

// 2. 边境冲突
USTRUCT()
struct FEvent_BorderConflict : public FHistoricalEvent
{
    // 条件: 邻国关系紧张
    // 效果: 小规模战争
    // 选择: 开战 / 谈判 / 让步
};
```

#### 军事事件
```cpp
// 1. 兵变
USTRUCT()
struct FEvent_MilitaryMutiny : public FHistoricalEvent
{
    // 条件: 士气低 + 欠饷
    // 效果: 部队损失
    // 选择: 镇压 / 安抚 / 收买
};

// 2. 武器装备更新
USTRUCT()
struct FEvent_WeaponUpgrade : public FHistoricalEvent
{
    // 条件: 有工业基础 + 资金
    // 效果: 单位战斗力提升
};
```

### 3.2 连锁事件系统

#### 长征事件链示例
```cpp
// 事件链: 长征
USTRUCT()
struct FEventChain_LongMarch
{
    // 起点: 根据地失守
    TArray<FLongMarchStage> Stages;
    
    // Stage 1: 血战湘江
    //   - 条件: 突破封锁线
    //   - 效果: 损失惨重
    
    // Stage 2: 遵义会议
    //   - 条件: 到达遵义
    //   - 效果: 领导权变更
    //   - 选择: 军事路线 / 政治路线
    
    // Stage 3: 四渡赤水
    //   - 条件: 遵义后
    //   - 效果: 摆脱追兵
    //   - 特殊: 展示军事才能
    
    // Stage 4: 到达陕北
    //   - 条件: 完成所有阶段
    //   - 效果: 新的根据地
    //   - 结束事件链
};
```

---

## 第四部分: 技术实现

### 4.1 事件触发器

```cpp
UCLASS()
class UMingEventTrigger : public UObject
{
    UFUNCTION()
    void CheckEventConditions();
    
    UFUNCTION()
    bool EvaluateCondition(const FEventCondition& Condition);
    
    UFUNCTION()
    void TriggerEvent(FHistoricalEvent& Event);
    
    // 时间触发
    UFUNCTION()
    void OnGameDateChanged(FDateTime NewDate);
    
    // 势力触发
    UFUNCTION()
    void OnFactionStatusChanged(EFactionType Faction, EFactionStatus Status);
    
    // 战争触发
    UFUNCTION()
    void OnWarStarted(EFactionType Attacker, EFactionType Defender);
};
```

### 4.2 事件效果应用

```cpp
UCLASS()
class UMingEventEffectApplier : public UObject
{
    UFUNCTION()
    void ApplyEffect(const FEventEffect& Effect, EFactionType TargetFaction);
    
    UFUNCTION()
    void ApplyResourceEffect(const FResourceEffect& Effect);
    
    UFUNCTION()
    void ApplyRelationEffect(const FRelationEffect& Effect);
    
    UFUNCTION()
    void ApplyUnitEffect(const FUnitEffect& Effect);
    
    UFUNCTION()
    void ApplyBuildingEffect(const FBuildingEffect& Effect);
};
```

### 4.3 事件UI系统

```cpp
UCLASS()
class UMingEventUI : public UUserWidget
{
    UFUNCTION(BlueprintImplementableEvent)
    void ShowEventPopup(const FHistoricalEvent& Event);
    
    UFUNCTION(BlueprintImplementableEvent)
    void ShowEventChoices(const TArray<FEventChoice>& Choices);
    
    UFUNCTION(BlueprintImplementableEvent)
    void ShowEventResult(const FEventEffect& Effect);
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* EventTitle;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* EventDescription;
    
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ChoiceContainer;
};
```

---

## 第五部分: 平衡与测试

### 5.1 事件平衡参数

```cpp
struct FEventBalanceConfig
{
    // 触发频率
    float MinDaysBetweenEvents = 30.0f;  // 最小事件间隔
    float MaxActiveEvents = 5.0f;        // 最大同时活跃事件
    
    // 概率调整
    float PositiveEventBias = 0.4f;      // 正面事件倾向
    float NegativeEventBias = 0.4f;      // 负面事件倾向
    float NeutralEventBias = 0.2f;       // 中性事件倾向
    
    // 难度调整
    TMap<EGameDifficulty, float> EventProbabilityMultiplier;
};
```

### 5.2 事件测试清单

| 测试项 | 测试内容 | 通过标准 |
|--------|----------|----------|
| 触发测试 | 验证所有事件能正确触发 | 100%触发率 |
| 条件测试 | 验证条件判断准确 | 无误判 |
| 效果测试 | 验证效果正确应用 | 数值准确 |
| 选择测试 | 验证选择分支 | 分支完整 |
| UI测试 | 验证界面显示 | 显示正确 |
| 存档测试 | 验证事件状态保存 | 可正确恢复 |
| 性能测试 | 大量事件处理 | <100ms |

---

## 第六部分: 开发时程

### 6.1 开发时程 (3个月)

```
月份1: 基础系统
  ├─ Week 1-2: 事件系统架构
  ├─ Week 3: 触发器系统
  └─ Week 4: 效果系统 + UI框架

月份2: 势力事件
  ├─ Week 1-2: 北洋 + 国民政府 + 共产党事件
  ├─ Week 3: 军阀事件 (奉/直/皖/晋)
  └─ Week 4: 军阀事件 (桂/滇/川) + 边疆

月份3: 通用事件 + 优化
  ├─ Week 1: 随机事件池
  ├─ Week 2: 连锁事件系统
  ├─ Week 3: 平衡调整
  └─ Week 4: 测试 + 修复
```

---

## 附录

### A. 事件命名规范

```
势力专属: EVT_[势力]_[事件名]
  例: EVT_BEIYANG_YUANSHIKAI_EMPEROR

通用事件: EVT_GEN_[类型]_[事件名]
  例: EVT_GEN_ECONOMIC_BOOM

连锁事件: EVT_CHAIN_[链名]_[阶段]
  例: EVT_CHAIN_LONGMARCH_STAGE1
```

### B. 相关文档

- `12_FACTIONS_HISTORICAL_EVENTS.md` - 历史事件设计文档
- `EPIC_9_EVENT_CONFIGS.md` - 事件配置文件

---

*文档版本*: 1.0  
*创建日期*: 2026-03-23
