# MingGoRTS 数据配置文档

## 文档信息
- **版本**: 1.0
- **日期**: 2026-03-23
- **作者**: MingGoRTS Design Team
- **状态**: 实施中

---

## 1. 数据配置总览

### 1.1 配置文件清单

| 配置类别 | 文件格式 | 文件数量 | 记录数量 | 说明 |
|---------|---------|---------|---------|------|
| **单位数据** | CSV/JSON | 12 | 720 | 12势力×60单位 |
| **建筑数据** | CSV | 1 | 36 | 势力特色建筑 |
| **机制参数** | CSV/JSON | 12 | 432 | 36特色机制参数 |
| **事件数据** | JSON | 1 | 200+ | 历史事件配置 |
| **科技数据** | CSV | 1 | 150 | 科技树配置 |
| **资源数据** | CSV | 1 | 20 | 资源类型定义 |
| **地图数据** | JSON | 12 | 1200 | 势力起始位置等 |
| **平衡参数** | JSON | 1 | 500+ | 全局平衡系数 |

---

## 2. 单位数据配置

### 2.1 单位基础数据表 (CSV)

#### 表结构定义

```csv
// Units_Master.csv - 主数据表
UnitID,Faction,UnitType,UnitName,Description,Tier,BaseCost_Money,BaseCost_Manpower,BaseCost_Material,BuildTime,Health,Armor,Speed,SightRange
BY_INF_01,BeiyangGovernment,Infantry,北洋步兵,北洋政府基础步兵单位,1,100,50,20,15,100,5,4,8
BY_INF_02,BeiyangGovernment,Infantry,北洋精锐,北洋政府精锐步兵,2,150,75,30,20,140,8,4,9
BY_CAV_01,BeiyangGovernment,Cavalry,北洋骑兵,北洋政府骑兵单位,2,200,80,40,25,120,6,8,10
BY_ART_01,BeiyangGovernment,Artillery,北洋炮兵,北洋政府炮兵单位,3,300,60,100,45,80,3,3,12
BY_SPC_01,BeiyangGovernment,Special,北洋工兵,北洋政府工程兵,2,180,70,50,30,110,7,4,9
BY_HER_01,BeiyangGovernment,Hero,段祺瑞,北洋政府英雄单位,5,500,200,150,60,200,10,4,12
GMD_INF_01,NationalistGovernment,Infantry,国民革命军,国民政府基础步兵,1,100,50,20,15,100,5,4,8
GMD_INF_02,NationalistGovernment,Infantry,德械师,国民政府德械精锐步兵,3,180,90,40,25,160,12,4,10
...
```

#### 详细字段说明

```cpp
// 单位数据配置结构
USTRUCT()
struct FUnitDataConfig
{
    GENERATED_BODY()
    
    // 基础信息
    UPROPERTY() FString UnitID;           // 唯一标识
    UPROPERTY() EFactionType Faction;     // 所属势力
    UPROPERTY() EUnitType UnitType;       // 单位类型
    UPROPERTY() FText UnitName;           // 显示名称
    UPROPERTY() FText Description;        // 描述文本
    UPROPERTY() int32 Tier;               // 等级 (1-5)
    
    // 成本
    UPROPERTY() int32 BaseCost_Money;         // 基础金钱成本
    UPROPERTY() int32 BaseCost_Manpower;      // 人力成本
    UPROPERTY() int32 BaseCost_Material;      // 物资成本
    UPROPERTY() int32 BuildTime;              // 建造时间(秒)
    
    // 基础属性
    UPROPERTY() float Health;             // 生命值
    UPROPERTY() float Armor;              // 护甲值
    UPROPERTY() float Speed;              // 移动速度
    UPROPERTY() float SightRange;         // 视野范围
    
    // 战斗属性
    UPROPERTY() float AttackDamage;       // 攻击力
    UPROPERTY() float AttackRange;        // 攻击范围
    UPROPERTY() float AttackSpeed;      // 攻击速度
    UPROPERTY() float Accuracy;         // 命中率
    
    // 特殊属性
    UPROPERTY() float StealthLevel;       // 隐身等级
    UPROPERTY() float Morale;             // 士气值
    UPROPERTY() float ExperienceGain;     // 经验获取率
    
    // 配置引用
    UPROPERTY() FString AttackConfigID;   // 攻击配置ID
    UPROPERTY() FString AbilityConfigID;  // 技能配置ID
    UPROPERTY() FString VisualConfigID;   // 视觉配置ID
};
```

### 2.2 攻击配置表

```csv
// Unit_Attack_Config.csv
ConfigID,DamageType,BaseDamage,DamageVsInfantry,DamageVsCavalry,DamageVsArmor,DamageVsBuilding,DamageVsAir,AreaOfEffect,ProjectileSpeed,Penetration
ATK_INF_RIFLE, ballistic, 25, 1.0, 0.7, 0.3, 0.5, 0.0, 0.0, 100, 5
ATK_INF_HEAVY, ballistic, 35, 1.2, 0.9, 0.5, 0.6, 0.0, 0.0, 100, 8
ATK_CAV_SABER, melee, 40, 1.5, 1.0, 0.2, 0.3, 0.0, 0.0, 0, 0
ATK_ART_HE, explosive, 80, 1.0, 1.2, 0.8, 1.5, 0.0, 3.0, 80, 0
ATK_ART_AP, armor_piercing, 60, 0.6, 0.8, 1.5, 1.0, 0.0, 0.0, 120, 15
ATK_SPC_MG, ballistic, 15, 1.0, 0.5, 0.2, 0.3, 0.1, 1.0, 150, 3
...
```

### 2.3 势力专属单位详细配置

#### 北洋政府专属单位

```json
// FactionUnits_Beiyang.json
{
  "Faction": "BeiyangGovernment",
  "Units": [
    {
      "UnitID": "BY_HERO_DUAN",
      "UnitName": "段祺瑞",
      "HeroType": "Strategist",
      "BaseStats": {
        "Health": 200,
        "Armor": 10,
        "Speed": 4,
        "SightRange": 12
      },
      "Abilities": [
        {
          "AbilityID": "ABILITY_WARLORD_COMPROMISE",
          "AbilityName": "军阀妥协",
          "Cooldown": 60,
          "Effect": {
            "Type": "FactionWide",
            "Parameter": "ProductionSpeed",
            "Value": 1.2,
            "Duration": 30
          }
        },
        {
          "AbilityID": "ABILITY_BEIYANG_DISCIPLINE",
          "AbilityName": "北洋军纪",
          "Passive": true,
          "Effect": {
            "Type": "Aura",
            "Radius": 10,
            "Target": "FriendlyUnits",
            "Parameter": "Morale",
            "Value": 15
          }
        }
      ],
      "UnlockCondition": {
        "Type": "BuildingConstructed",
        "BuildingID": "BY_COMMAND_CENTER",
        "TechnologyID": "TECH_BEIYANG_LEADERSHIP"
      }
    },
    {
      "UnitID": "BY_ELITE_GUARDS",
      "UnitName": "北洋禁卫军",
      "Description": "北洋政府最精锐的卫队",
      "Tier": 4,
      "Stats": {
        "Health": 180,
        "Armor": 15,
        "Speed": 4,
        "AttackDamage": 45,
        "AttackSpeed": 1.5
      },
      "SpecialTraits": [
        "TRAIT_LOYAL",        // 忠诚：士气不会低于50%
        "TRAIT_DISCIPLINED",  // 纪律：阵型维持加成
        "TRAIT_ELITE"         // 精英：经验获取+50%
      ],
      "Cost": {
        "Money": 300,
        "Manpower": 150,
        "Material": 80,
        "Time": 35
      }
    },
    {
      "UnitID": "BY_GERMAN_ADVISED",
      "UnitName": "德训精锐",
      "Description": "接受德国军事顾问训练的精锐部队",
      "Tier": 3,
      "Stats": {
        "Health": 150,
        "Armor": 10,
        "Speed": 5,
        "AttackDamage": 40,
        "Accuracy": 0.85
      },
      "SpecialTraits": [
        "TRAIT_GERMAN_TRAINED",  // 德训：命中率+20%
        "TRAIT_TACTICAL"          // 战术：可使用战术模板
      ],
      "Requirements": {
        "Mechanic": "BY_GERMAN_ADVISOR",
        "AdvisorActive": true
      }
    }
  ]
}
```

#### 国民政府专属单位

```json
// FactionUnits_GMD.json
{
  "Faction": "NationalistGovernment",
  "Units": [
    {
      "UnitID": "GMD_HERO_CHIANG",
      "UnitName": "蒋介石",
      "HeroType": "Commander",
      "BaseStats": {
        "Health": 220,
        "Armor": 8,
        "Speed": 5,
        "SightRange": 15
      },
      "Abilities": [
        {
          "AbilityID": "ABILITY_WHAMPOA_INSPIRATION",
          "AbilityName": "黄埔精神",
          "Cooldown": 90,
          "Effect": {
            "Type": "Spawn",
            "UnitType": "EliteCadet",
            "Count": 3,
            "Buffs": {
              "Attack": 1.3,
              "Defense": 1.2,
              "Duration": 60
            }
          }
        },
        {
          "AbilityID": "ABILITY_NORTHERN_EXPEDITION",
          "AbilityName": "北伐号召",
          "Passive": true,
          "Condition": "AttackingWarlordFaction",
          "Effect": {
            "Type": "CombatBonus",
            "Parameter": "AttackPower",
            "Value": 1.25
          }
        }
      ]
    },
    {
      "UnitID": "GMD_WHAMPOA_CADET",
      "UnitName": "黄埔学员兵",
      "Description": "黄埔军校毕业的优秀学员",
      "Tier": 3,
      "Stats": {
        "Health": 140,
        "Armor": 8,
        "Speed": 5,
        "AttackDamage": 38,
        "Morale": 120
      },
      "SpecialTraits": [
        "TRAIT_WHAMPOA_TRAINED",  // 黄埔训练：升级速度+100%
        "TRAIT_HIGH_MORALE",       // 高士气：初始士气+20
        "TRAIT_LOYAL_GMD"          // 忠诚：不会叛逃到军阀
      ],
      "UpgradePath": [
        {
          "Rank": "Lieutenant",
          "Requirements": { "Experience": 100 },
          "Bonuses": { "Health": 20, "Attack": 5 }
        },
        {
          "Rank": "Captain",
          "Requirements": { "Experience": 300 },
          "Bonuses": { "Health": 30, "Attack": 10, "AbilityUnlock": "ABILITY_TACTICAL_COMMAND" }
        }
      ]
    },
    {
      "UnitID": "GMD_GERMAN_EQUIPPED",
      "UnitName": "德械师",
      "Description": "装备德国武器的标准化精锐师",
      "Tier": 4,
      "Stats": {
        "Health": 160,
        "Armor": 12,
        "Speed": 4,
        "AttackDamage": 50,
        "AttackSpeed": 1.3,
        "Accuracy": 0.9
      },
      "EquipmentSlots": [
        {
          "Slot": "Weapon",
          "DefaultEquipment": "GERMAN_MAUSER_RIFLE",
          "UpgradeOptions": ["GERMAN_MG34", "GERMAN_KAR98K"]
        },
        {
          "Slot": "Armor",
          "DefaultEquipment": "STANDARD_UNIFORM",
          "UpgradeOptions": ["GERMAN_HELMET", "BULLETPROOF_VEST"]
        }
      ]
    }
  ]
}
```

#### 中共专属单位

```json
// FactionUnits_CCP.json
{
  "Faction": "ChineseCommunistParty",
  "Units": [
    {
      "UnitID": "CCP_HERO_MAO",
      "UnitName": "毛泽东",
      "HeroType": "IdeologicalLeader",
      "BaseStats": {
        "Health": 180,
        "Armor": 5,
        "Speed": 4,
        "SightRange": 14
      },
      "Abilities": [
        {
          "AbilityID": "ABILITY_GUERRILLA_MASTER",
          "AbilityName": "游击战大师",
          "Cooldown": 45,
          "Effect": {
            "Type": "AreaEffect",
            "Radius": 15,
            "Target": "FriendlyUnits",
            "Effects": {
              "Stealth": true,
              "MovementSpeed": 1.5,
              "TerrainBonus": 2.0,
              "Duration": 45
            }
          }
        },
        {
          "AbilityID": "ABILITY_MASS_LINE",
          "AbilityName": "群众路线",
          "Passive": true,
          "Effect": {
            "Type": "ResourceConversion",
            "From": "PeasantSupport",
            "To": "Manpower",
            "Ratio": 0.2
          }
        }
      ]
    },
    {
      "UnitID": "CCP_RED_ARMY",
      "UnitName": "红军战士",
      "Description": "中共主力部队",
      "Tier": 2,
      "Stats": {
        "Health": 120,
        "Armor": 4,
        "Speed": 5,
        "AttackDamage": 30,
        "Morale": 110
      },
      "SpecialTraits": [
        "TRAIT_GUERRILLA",      // 游击：山地森林移速+30%
        "TRAIT_HIGH_MORALE",    // 高士气：士气上限+20
        "TRAIT_POORLY_EQUIPPED" // 装备简陋：基础攻击-10%
      ],
      "ConditionalBonuses": [
        {
          "Condition": "InFriendlyTerritory",
          "Bonuses": { "HealthRegen": 2, "MoraleRegen": 5 }
        },
        {
          "Condition": "AtNight",
          "Bonuses": { "Stealth": 0.3, "AttackBonus": 1.2 }
        }
      ]
    },
    {
      "UnitID": "CCP_GUERRILLA",
      "UnitName": "游击队",
      "Description": "灵活的游击部队",
      "Tier": 1,
      "Stats": {
        "Health": 80,
        "Armor": 2,
        "Speed": 6,
        "AttackDamage": 25,
        "Stealth": 0.5
      },
      "SpecialTraits": [
        "TRAIT_STEALTH",        // 隐身：默认50%隐身
        "TRAIT_FAST",           // 快速：移速+20%
        "TRAIT_AMBUSH",         // 埋伏：伏击伤害+50%
        "TRAIT_NO_FORMATION"    // 无阵型：无法保持阵型但机动性极高
      ],
      "MechanicIntegration": {
        "MechanicID": "CCP_GUERRILLA_WARFARE",
        "Bonuses": {
          "StealthDuration": 30,
          "AmbushDamageMultiplier": 2.0,
          "TerrainBonusMultiplier": 1.5
        }
      }
    }
  ]
}
```

---

## 3. 机制参数配置

### 3.1 特色机制参数表

```csv
// Mechanic_Parameters.csv
MechanicID,Faction,ParameterName,BaseValue,MaxValue,ScaleFactor,UnlockCondition,Prerequisites
BY_WARLORD_COMPROMISE,BeiyangGovernment,MaxPoints,100,100,1,Start,
BY_WARLORD_COMPROMISE,BeiyangGovernment,RegenRate,2,5,0.1,Start,
BY_WARLORD_COMPROMISE,BeiyangGovernment,BoostCost_Light,20,20,1,Start,
BY_WARLORD_COMPROMISE,BeiyangGovernment,BoostCost_Heavy,50,50,1,Start,
BY_GERMAN_ADVISOR,BeiyangGovernment,MaxAdvisors,2,3,1,Technology_Unlocked,TECH_GERMAN_MILITARY
BY_GERMAN_ADVISOR,BeiyangGovernment,AdvisorCost_Base,500,500,1,Technology_Unlocked,TECH_GERMAN_MILITARY
BY_GERMAN_ADVISOR,BeiyangGovernment,EffectDuration,300,600,60,Technology_Unlocked,TECH_GERMAN_MILITARY
BY_LEGITIMACY,BeiyangGovernment,MaxLegitimacy,100,100,1,Start,
BY_LEGITIMACY,BeiyangGovernment,DecayRate,1,0.5,-0.05,Start,
BY_LEGITIMACY,BeiyangGovernment,DiploBonusPer10,3,5,0.2,Start,
GMD_WHAMPOA,NationalistGovernment,MaxTrainings,3,5,0.4,Building_Built,BLD_WHAMPOA_ACADEMY
GMD_WHAMPOA,NationalistGovernment,TrainingSpeed,1,2,0.2,Building_Built,BLD_WHAMPOA_ACADEMY
GMD_WHAMPOA,NationalistGovernment,EliteChance,0.3,0.5,0.04,Building_Built,BLD_WHAMPOA_ACADEMY
GMD_NEW_LIFE,NationalistGovernment,MaxMorale,100,100,1,Event_Triggered,EVENT_NEW_LIFE_MOVEMENT
GMD_NEW_LIFE,NationalistGovernment,ProductionBonus,0.3,0.5,0.04,Event_Triggered,EVENT_NEW_LIFE_MOVEMENT
GMD_EQUIPMENT,NationalistGovernment,MaxStock,100,150,10,Technology_Unlocked,TECH_IMPORT_EQUIPMENT
GMD_EQUIPMENT,NationalistGovernment,ImportCost,1000,800,-20,Technology_Unlocked,TECH_IMPORT_EQUIPMENT
CCP_GUERRILLA,ChineseCommunistParty,MaxAmbushBonus,2,3,0.2,Start,
CCP_GUERRILLA,ChineseCommunistParty,StealthDuration,30,60,6,Start,
CCP_GUERRILLA,ChineseCommunistParty,TerrainBonus_Forest,1.5,2,0.1,Start,
CCP_LAND_REFORM,ChineseCommunistParty,BaseSupport,10,15,0.5,Territory_Owned,
CCP_LAND_REFORM,ChineseCommunistParty,RecruitRatio,0.15,0.25,0.02,Territory_Owned,
CCP_UNITED_FRONT,ChineseCommunistParty,MaxAlliances,2,3,0.2,Event_Available,EVENT_CCP_AVAILABLE
CCP_UNITED_FRONT,ChineseCommunistParty,ShareEfficiency,0.8,0.9,0.02,Event_Available,EVENT_CCP_AVAILABLE
...
```

### 3.2 机制效果配置 (JSON)

```json
// MechanicEffects.json
{
  "Effects": [
    {
      "EffectID": "EFFECT_BY_WARLORD_BOOST_PRODUCTION",
      "EffectName": "妥协生产加速",
      "TargetType": "Faction",
      "EffectType": "ProductionModifier",
      "Parameters": {
        "ResourceType": "All",
        "Multiplier": 1.5,
        "Duration": 60
      },
      "VisualEffect": "VFX_PRODUCTION_BOOST",
      "AudioEffect": "SFX_WARLORD_ACTIVATED"
    },
    {
      "EffectID": "EFFECT_BY_WARLORD_BOOST_COMBAT",
      "EffectName": "妥协战斗强化",
      "TargetType": "Units",
      "TargetFilter": {
        "Faction": "BeiyangGovernment",
        "InCombat": true
      },
      "EffectType": "CombatModifier",
      "Parameters": {
        "AttackBonus": 1.3,
        "DefenseBonus": 1.2,
        "Duration": 45
      },
      "VisualEffect": "VFX_COMBAT_AURA"
    },
    {
      "EffectID": "EFFECT_GMD_WHAMPOA_TRAINING",
      "EffectName": "黄埔训练加速",
      "TargetType": "Building",
      "TargetFilter": {
        "BuildingType": "WHAMPOA_ACADEMY"
      },
      "EffectType": "TrainingSpeed",
      "Parameters": {
        "SpeedMultiplier": 2.0,
        "QualityBonus": 0.2
      },
      "Stackable": false
    },
    {
      "EffectID": "EFFECT_CCP_GUERRILLA_STEALTH",
      "EffectName": "游击隐身",
      "TargetType": "Units",
      "TargetFilter": {
        "Faction": "ChineseCommunistParty",
        "Terrain": ["Forest", "Mountain", "Urban"]
      },
      "EffectType": "Stealth",
      "Parameters": {
        "StealthLevel": 0.7,
        "DetectionRangeModifier": 0.5
      }
    }
  ]
}
```

---

## 4. 历史事件数据配置

### 4.1 核心事件配置

```json
// HistoricalEvents_Core.json
{
  "Events": [
    {
      "EventID": "EVENT_YUAN_SHIKAI_EMPEROR",
      "EventName": "袁世凯称帝",
      "Description": "袁世凯宣布恢复帝制，建立中华帝国",
      "Category": "Mainline",
      "HistoricalDate": "1915-12-11",
      "GameTime": {
        "Year": 1915,
        "Month": 12,
        "Day": 11
      },
      "TriggerConditions": {
        "RequiredFaction": "BeiyangGovernment",
        "MinimumLegitimacy": 60,
        "PowerStatus": "Dominant"
      },
      "PlayerChoices": [
        {
          "ChoiceID": "SUPPORT_EMPEROR",
          "ChoiceText": "支持袁世凯称帝",
          "Requirements": {
            "Faction": "BeiyangGovernment"
          },
          "Consequences": [
            {
              "Type": "FactionLegitimacy",
              "Value": 30,
              "Duration": -1
            },
            {
              "Type": "DiplomaticRelations",
              "TargetFactions": ["AllWarlordFactions"],
              "RelationChange": -20
            },
            {
              "Type": "TriggerEvent",
              "EventID": "EVENT_NATIONAL_PROTECTION_WAR",
              "DelayDays": 30
            }
          ]
        },
        {
          "ChoiceID": "OPPOSE_EMPEROR",
          "ChoiceText": "反对称帝，举起护国旗号",
          "Requirements": {
            "MinArmyStrength": 10000
          },
          "Consequences": [
            {
              "Type": "DiplomaticRelations",
              "TargetFactions": ["BeiyangGovernment"],
              "RelationChange": -100,
              "DeclareWar": true
            },
            {
              "Type": "FactionLegitimacy",
              "Target": "Player",
              "Value": 20
            },
            {
              "Type": "JoinAlliance",
              "AllianceID": "NATIONAL_PROTECTION_ALLIANCE"
            }
          ]
        },
        {
          "ChoiceID": "REMAIN_SILENT",
          "ChoiceText": "暂时保持沉默观望",
          "Consequences": [
            {
              "Type": "DiplomaticRelations",
              "TargetFactions": ["AllFactions"],
              "RelationChange": -5
            }
          ]
        }
      ],
      "GlobalEffects": {
        "PoliticalStability": -20,
        "InternationalStanding": -15,
        "PublicSupport": -25
      }
    },
    {
      "EventID": "EVENT_NORTHERN_EXPEDITION",
      "EventName": "北伐战争",
      "Description": "国民革命军誓师北伐，统一中国",
      "Category": "Mainline",
      "HistoricalDate": "1926-07-09",
      "GameTime": {
        "Year": 1926,
        "Month": 7,
        "Day": 9
      },
      "TriggerConditions": {
        "RequiredFaction": "NationalistGovernment",
        "BuildingRequired": "WHAMPOA_ACADEMY",
        "MinimumArmySize": 50000,
        "UnificationProgress": "Started"
      },
      "EventChain": {
        "ChainID": "CHAIN_NORTHERN_EXPEDITION",
        "Phases": [
          {
            "PhaseID": "PHASE_HUNAN",
            "PhaseName": "湖南战役",
            "Target": "ZhiliClique",
            "RequiredVictories": 3
          },
          {
            "PhaseID": "PHASE_WUHAN",
            "PhaseName": "武汉攻克",
            "Target": "WuhanTerritory",
            "RequiredVictories": 2
          },
          {
            "PhaseID": "PHASE_NANJING",
            "PhaseName": "南京战役",
            "Target": "NanjingTerritory",
            "RequiredVictories": 2
          },
          {
            "PhaseID": "PHASE_SHANGHAI",
            "PhaseName": "上海进军",
            "Target": "ShanghaiTerritory",
            "Special": "ForeignInterventionRisk"
          },
          {
            "PhaseID": "PHASE_BEIJING",
            "PhaseName": "北京攻克",
            "Target": "BeijingTerritory",
            "VictoryCondition": "CaptureCapital"
          }
        ]
      },
      "PlayerChoices": [
        {
          "ChoiceID": "EXPEDITION_FULL",
          "ChoiceText": "全力北伐，快速统一",
          "Consequences": [
            {
              "Type": "ExpeditionSpeed",
              "Multiplier": 1.5
            },
            {
              "Type": "WarExhaustion",
              "Rate": 1.3
            }
          ]
        },
        {
          "ChoiceID": "EXPEDITION_CAUTIOUS",
          "ChoiceText": "稳扎稳打，巩固后方",
          "Consequences": [
            {
              "Type": "ExpeditionSpeed",
              "Multiplier": 0.8
            },
            {
              "Type": "TerritoryControl",
              "Bonus": 0.3
            }
          ]
        }
      ]
    },
    {
      "EventID": "EVENT_LONG_MARCH",
      "EventName": "长征",
      "Description": "红军战略转移，开始长征",
      "Category": "FactionSpecific",
      "FactionRequired": "ChineseCommunistParty",
      "HistoricalDate": "1934-10-16",
      "GameTime": {
        "Year": 1934,
        "Month": 10,
        "Day": 16
      },
      "TriggerConditions": {
        "Faction": "ChineseCommunistParty",
        "Surrounded": true,
        "TerritoryRemaining": {
          "Max": 3
        },
        "ArmyStrength": {
          "Current": "Below50%",
          "OfPeak": "Below30%"
        }
      },
      "SpecialMechanics": {
        "MechanicID": "MECH_LONG_MARCH",
        "Type": "SurvivalCampaign",
        "Phases": [
          {
            "Name": "突破封锁",
            "Challenges": ["EnemyEncirclement", "SupplyShortage"]
          },
          {
            "Name": "湘江血战",
            "Challenges": ["HeavyCombat", "RiverCrossing"],
            "CasualtyRate": 0.5
          },
          {
            "Name": "遵义会议",
            "Challenges": ["LeadershipCrisis", "StrategicDecision"],
            "TurningPoint": true
          },
          {
            "Name": "四渡赤水",
            "Challenges": ["GuerrillaWarfare", "Outmaneuvering"],
            "SpecialAbility": "RiverCrossingMaster"
          },
          {
            "Name": "雪山草地",
            "Challenges": ["HarshTerrain", "ExtremeWeather", "Starvation"],
            "CasualtyRate": 0.3
          },
          {
            "Name": "到达陕北",
            "Outcome": "NewBaseEstablished",
            "Bonuses": {
              "ArmyCohesion": 1.5,
              "IdeologicalUnity": 2.0,
              "LegendaryStatus": true
            }
          }
        ]
      },
      "Consequences": {
        "SurvivalBonus": {
          "ExperienceGain": 2.0,
          "MoraleBonus": 50,
          "UnlockedMechanic": "MECH_LONG_MARCH_VETERAN"
        }
      }
    }
  ]
}
```

### 4.2 事件触发条件配置

```csv
// Event_Trigger_Conditions.csv
EventID,ConditionType,Parameter,Value,Weight
EVENT_YUAN_SHIKAI_EMPEROR,FactionExists,BeiyangGovernment,true,1
EVENT_YUAN_SHIKAI_EMPEROR,MinimumDate,1915-12-01,1
EVENT_YUAN_SHIKAI_EMPEROR,FactionPower,BeiyangGovernment,Dominant,1
EVENT_NORTHERN_EXPEDITION,FactionExists,NationalistGovernment,true,1
EVENT_NORTHERN_EXPEDITION,BuildingExists,WhampoaAcademy,true,1
EVENT_NORTHERN_EXPEDITION,MinimumArmySize,50000,1
EVENT_LONG_MARCH,FactionExists,ChineseCommunistParty,true,1
EVENT_LONG_MARCH,TerritoryCount,Max,3,1
EVENT_LONG_MARCH,Surrounded,true,true,1
...
```

---

## 5. 平衡性参数配置

### 5.1 全局平衡系数

```json
// Balance_Global.json
{
  "GlobalBalance": {
    "Version": "1.0",
    "LastUpdated": "2026-03-23",
    
    "CombatBalance": {
      "DamageMultiplier_MeleeVsRanged": 1.3,
      "DamageMultiplier_RangedVsMelee": 0.9,
      "DamageMultiplier_CavalryVsInfantry": 1.4,
      "DamageMultiplier_InfantryVsCavalry": 0.8,
      "DamageMultiplier_ArtilleryVsAll": 1.2,
      "ArmorEffectiveness": 0.5,
      "MoraleEffectThreshold": 0.3,
      "RetreatThreshold": 0.2
    },
    
    "EconomicBalance": {
      "BaseIncomeMultiplier": 1.0,
      "ResourceConversionRate": 0.9,
      "MarketFluctuationRange": 0.2,
      "TradeEfficiency": 0.85,
      "TaxBaseRate": 0.1,
      "CorruptionFactor": 0.05
    },
    
    "FactionBalance": {
      "BeiyangGovernment": {
        "DifficultyRating": 2,
        "MilitaryStrength": 1.1,
        "EconomicBase": 1.0,
        "PoliticalStability": 0.9,
        "LegitimacyBonus": 1.2
      },
      "NationalistGovernment": {
        "DifficultyRating": 3,
        "MilitaryStrength": 1.0,
        "EconomicBase": 0.9,
        "PoliticalStability": 0.8,
        "UnificationSpeed": 1.2
      },
      "ChineseCommunistParty": {
        "DifficultyRating": 5,
        "MilitaryStrength": 0.8,
        "EconomicBase": 0.6,
        "PoliticalStability": 1.2,
        "GuerrillaEffectiveness": 1.5,
        "MassMobilization": 1.3
      }
    },
    
    "ProgressionBalance": {
      "ExperienceGain_Base": 10,
      "ExperienceGain_Kill": 5,
      "ExperienceGain_Victory": 50,
      "LevelUpMultiplier": 1.2,
      "MaxLevel": 5,
      "TechResearchSpeed": 1.0,
      "BuildingConstructionSpeed": 1.0
    },
    
    "AIBalance": {
      "AIResourceBonus_Easy": 0.8,
      "AIResourceBonus_Normal": 1.0,
      "AIResourceBonus_Hard": 1.2,
      "AIResourceBonus_Expert": 1.4,
      "AIReactivity_Easy": 0.7,
      "AIReactivity_Normal": 1.0,
      "AIReactivity_Hard": 1.3,
      "AIReactivity_Expert": 1.5
    }
  }
}
```

### 5.2 单位平衡微调表

```csv
// Unit_Balance_Tweaks.csv
UnitID,HealthMod,DamageMod,CostMod,SpeedMod,Reason
BY_INF_01,1.0,1.0,1.0,1.0,Baseline
BY_INF_02,1.1,1.15,1.2,1.0,Elite premium
GMD_WHAMPOA_CADET,0.95,1.1,1.0,1.05,Training quality bonus
CCP_GUERRILLA,0.7,0.85,0.6,1.2,Guerrilla efficiency
CCP_RED_ARMY,0.9,0.9,0.85,1.0,Resource constrained
...
```

---

## 6. 加载与验证系统

### 6.1 数据加载器

```cpp
UCLASS()
class MINGGORTS_API UMingDataConfigLoader : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool LoadUnitConfigs(const FString& FilePath);
    
    UFUNCTION(BlueprintCallable)
    bool LoadMechanicConfigs(const FString& FilePath);
    
    UFUNCTION(BlueprintCallable)
    bool LoadEventConfigs(const FString& FilePath);
    
    UFUNCTION(BlueprintCallable)
    bool LoadBalanceConfigs(const FString& FilePath);
    
    UFUNCTION(BlueprintCallable)
    bool ValidateAllConfigs(FString& OutErrorMessage);

private:
    bool ParseCSV(const FString& Content, TArray<TMap<FString, FString>>& OutRows);
    bool ParseJSON(const FString& Content, TSharedPtr<FJsonObject>& OutObject);
    bool ValidateUnitConfig(const FUnitDataConfig& Config);
    bool ValidateMechanicConfig(const FMechanicConfig& Config);
};
```

### 6.2 配置验证规则

```cpp
// 验证规则定义
struct FConfigValidationRule
{
    FString FieldName;
    EValidationType Type;
    FString MinValue;
    FString MaxValue;
    bool bRequired;
    TArray<FString> AllowedValues;
};

// 单位配置验证规则
TArray<FConfigValidationRule> UnitValidationRules = {
    {TEXT("UnitID"), EValidationType::String, {}, {}, true, {}},
    {TEXT("Health"), EValidationType::Float, TEXT("1"), TEXT("10000"), true, {}},
    {TEXT("AttackDamage"), EValidationType::Float, TEXT("0"), TEXT("1000"), true, {}},
    {TEXT("Cost_Money"), EValidationType::Int, TEXT("0"), TEXT("100000"), true, {}},
    {TEXT("Tier"), EValidationType::Int, TEXT("1"), TEXT("5"), true, {}},
    {TEXT("Faction"), EValidationType::Enum, {}, {}, true, 
        {TEXT("Beiyang"), TEXT("GMD"), TEXT("CCP"), TEXT("Fengtian"), ...}}
};
```

---

## 7. 配置版本管理

### 7.1 版本控制策略

```
Config/
├── Current/              # 当前使用的配置
│   ├── Units/
│   ├── Mechanics/
│   ├── Events/
│   └── Balance/
├── Archive/              # 历史版本
│   ├── v1.0.0_20260320/
│   ├── v1.0.1_20260322/
│   └── v1.1.0_20260325/
└── Testing/              # 测试中的配置
    ├── BalanceTest_A/
    └── NewUnits_Test/
```

### 7.2 热更新支持

```cpp
UCLASS()
class MINGGORTS_API UMingConfigHotReload : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool ReloadUnitConfig(const FString& UnitID);
    
    UFUNCTION(BlueprintCallable)
    bool ReloadBalanceConfig();
    
    UFUNCTION(BlueprintCallable)
    void EnableAutoReload(bool bEnable);

private:
    void OnConfigFileChanged(const FString& FilePath);
    void NotifyConfigUpdated(EConfigType Type);
};
```

---

## 8. 附录

### 8.1 配置编辑器工具

推荐使用以下工具编辑配置文件:

- **CSV编辑**: Excel, LibreOffice Calc, VS Code CSV插件
- **JSON编辑**: VS Code, JSON Editor Online
- **版本控制**: Git LFS管理大配置文件

### 8.2 数据表关系图

```
Units_Master.csv
    ├── Unit_Attack_Config.csv (1:1 via AttackConfigID)
    ├── Unit_Ability_Config.csv (1:N via UnitID)
    ├── Unit_Visual_Config.csv (1:1 via VisualConfigID)
    └── FactionUnits_[Faction].json (扩展数据)

Mechanic_Parameters.csv
    └── MechanicEffects.json (效果定义)

HistoricalEvents_Core.json
    └── Event_Trigger_Conditions.csv (触发条件)

Balance_Global.json
    └── Unit_Balance_Tweaks.csv (微调参数)
```

### 8.3 配置加载顺序

1. **Balance_Global.json** - 全局平衡参数
2. **Units_Master.csv** - 基础单位数据
3. **FactionUnits_*.json** - 势力专属单位扩展
4. **Mechanic_Parameters.csv** - 机制参数
5. **HistoricalEvents_Core.json** - 历史事件
6. **Unit_Balance_Tweaks.csv** - 平衡微调

---

*文档结束*
