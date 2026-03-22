# MingGoRTS 历史事件系统设计文档

## 文档信息
- **版本**: 1.0
- **日期**: 2026-03-23
- **作者**: MingGoRTS Design Team
- **状态**: 进行中

---

## 1. 历史事件系统总览

### 1.1 系统目标

- **历史还原**: 真实还原民国时期重大历史事件
- **玩家影响**: 玩家决策能够影响历史走向
- **多线叙事**: 支持多种历史分支和结局
- **沉浸体验**: 通过事件增强游戏代入感

### 1.2 事件分类体系

| 事件类型 | 描述 | 触发频率 | 影响程度 |
|----------|------|----------|----------|
| **历史主线事件** | 必然发生的历史大事件 | 固定时间 | 全局影响 |
| **势力专属事件** | 各势力特有事件 | 条件触发 | 势力影响 |
| **随机事件** | 随机触发的日常事件 | 随机 | 局部影响 |
| **连锁事件** | 由其他事件触发的事件链 | 事件驱动 | 渐进影响 |
| **危机事件** | 紧急危机情况 | 条件触发 | 重大影响 |
| **机遇事件** | 特殊机遇 | 随机/条件 | 正面影响 |

### 1.3 时间线框架

```
1912年 - 1916年: 北洋政府初期、袁世凯时代
1916年 - 1920年: 军阀割据开始、府院之争
1920年 - 1924年: 直皖战争、第一次直奉战争
1924年 - 1926年: 第二次直奉战争、国民革命军北伐
1926年 - 1928年: 北伐战争、全国统一
1928年 - 1937年: 南京国民政府时期
1937年 - 1945年: 抗日战争时期
1945年 - 1949年: 国共内战、新中国成立
```

---

## 2. 核心历史事件设计

### 2.1 北洋政府时期事件 (1912-1916)

#### 事件1: 袁世凯称帝 (1915-1916)

**事件ID**: `EV_YUAN_SHIKAI_EMPEROR`

**事件描述**:
袁世凯试图恢复帝制，建立中华帝国，引发护国战争。

**触发条件**:
- 时间: 1915年12月
- 势力: 北洋政府
- 前提: 袁世凯为领导人、北洋政府稳定度>60

**玩家选择**:

**选择A: 支持称帝**
- 效果:
  - 北洋政府威望 +30
  - 获得短期资金注入
  - 触发护国战争事件 (90%概率)
  - 民众支持度 -40
  - 各军阀反叛风险 +50%

**选择B: 放弃称帝**
- 效果:
  - 北洋政府威望 -20
  - 维持共和体制
  - 避免护国战争
  - 民众支持度 +20
  - 各军阀稳定

**选择C: 被迫放弃 (被动)**
- 触发条件: 护国战争失败
- 效果:
  - 袁世凯威望归零
  - 北洋政府分裂风险 +60%
  - 进入后袁世凯时代

**后续事件**:
- 护国战争 (选择A后触发)
- 袁世凯病逝 (1916年6月)
- 北洋政府分裂

**历史参考**: 1915年12月12日袁世凯接受帝位，1916年3月22日被迫取消，6月6日病逝。

---

#### 事件2: 护国战争 (1916)

**事件ID**: `EV_NATIONAL_PROTECTION_WAR`

**事件描述**:
蔡锷、唐继尧等在云南发动护国战争，反对袁世凯称帝。

**触发条件**:
- 前置事件: 袁世凯称帝 (选择A)
- 时间: 1916年1月
- 参与势力: 滇系、桂系、黔系等

**势力立场**:

**北洋政府 (防守方)**:
- 任务: 镇压护国军
- 胜利条件: 6个月内击溃护国军主力
- 胜利效果:
  - 称帝成功
  - 威望 +50
  - 但民众支持度持续下降
- 失败效果:
  - 被迫取消帝制
  - 袁世凯威望归零
  - 北洋政府权威下降

**滇系 (进攻方)**:
- 任务: 领导护国战争
- 效果:
  - 护国战争期间战斗力 +35%
  - 民众支持度 +40
  - 声望大幅提升
  - 获得"护国"声誉

**其他势力选择**:
- **支持护国**: 加入护国军，获得声誉加成
- **保持中立**: 观望态度
- **支持北洋**: 帮助镇压，获得北洋好感但声誉受损

**历史结果**: 袁世凯被迫取消帝制，不久后病逝。

---

### 2.2 军阀混战时期事件 (1916-1926)

#### 事件3: 府院之争 (1917)

**事件ID**: `EV_PRESIDENT_PREMIER_CONFLICT`

**事件描述**:
总统黎元洪与总理段祺瑞之间的权力斗争，引发政治危机。

**触发条件**:
- 时间: 1917年
- 势力: 北洋政府
- 前提: 北洋政府内部派系斗争激烈

**机制说明**:
- 文官派 (总统府) vs 武将派 (国务院)
- 玩家需要平衡两派势力
- 失衡将导致政变或政府瘫痪

**玩家操作**:
- 支持总统派: 加强文官影响力
- 支持总理派: 加强武将影响力
- 调解平衡: 试图平衡两派 (难度高)

**事件结果**:
- **总统派胜利**: 张勋复辟事件 (特殊事件)
- **总理派胜利**: 段祺瑞掌控政府
- **两败俱伤**: 北洋政府权威大损，军阀割据加剧

**历史参考**: 1917年黎元洪与段祺瑞因参战问题发生冲突，引发府院之争。

---

#### 事件4: 张勋复辟 (1917)

**事件ID**: `EV_ZHANG_XUN_RESTORATION`

**事件描述**:
军阀张勋率辫子军入京，拥立溥仪复辟，但很快失败。

**触发条件**:
- 前置: 府院之争中总统派过于弱势
- 时间: 1917年7月
- 特殊条件: 张勋势力存在

**事件流程**:
1. 张勋以调解为名率兵入京
2. 拥立溥仪复辟 (宣统九年)
3. 段祺瑞组织讨逆军
4. 复辟失败，张勋逃亡

**影响效果**:
- 北洋政府威望 -30
- 共和观念深入人心
- 段祺瑞声望上升
- 溥仪声望归零

**历史参考**: 1917年7月1日-12日，为期12天的复辟闹剧。

---

#### 事件5: 直皖战争 (1920)

**事件ID**: `EV_ZHILI_WAN_WAR`

**事件描述**:
直系军阀与皖系军阀之间的战争，直系获胜。

**触发条件**:
- 时间: 1920年7月
- 势力: 直系 vs 皖系
- 前提: 两系矛盾激化

**战争机制**:

**参战方**:
- **直系**: 吴佩孚、曹锟 (有奉系支持)
- **皖系**: 段祺瑞 (安福系)

**战争结果**:
- **直系胜利** (历史结果):
  - 皖系失败，安福国会解散
  - 直系控制北京政府
  - 段祺瑞下野
  - 开启直系时代

- **皖系胜利** (替代历史):
  - 直系受挫
  - 皖系继续掌权
  - 日本影响力增加

**历史参考**: 1920年7月14日-18日，直系与奉系联合击败皖系。

---

#### 事件6: 第一次直奉战争 (1922)

**事件ID**: `EV_FIRST_ZHILI_FENG_WAR`

**事件描述**:
直系与奉系因争夺北京政府控制权而爆发战争。

**触发条件**:
- 时间: 1922年4月
- 势力: 直系 vs 奉系
- 前提: 直奉矛盾激化

**战争机制**:

**参战方**:
- **直系**: 吴佩孚 (优势方)
- **奉系**: 张作霖

**关键战役**: 长辛店之战

**战争结果**:
- **直系胜利** (历史结果):
  - 奉系退回东北
  - 直系独掌北京政府
  - 吴佩孚声望达到顶峰
  - 开启直系最盛时期

- **奉系胜利** (替代历史):
  - 直系受挫
  - 奉系入关
  - 张作霖控制华北

**历史参考**: 1922年4月28日-5月5日，直系获胜。

---

#### 事件7: 第二次直奉战争 (1924)

**事件ID**: `EV_SECOND_ZHILI_FENG_WAR`

**事件描述**:
直系与奉系的再次决战，奉系获胜，直系衰落。

**触发条件**:
- 时间: 1924年9月-10月
- 势力: 直系 vs 奉系
- 前提: 直奉矛盾再次激化

**战争机制**:

**参战方**:
- **直系**: 吴佩孚
- **奉系**: 张作霖 (有冯玉祥倒戈)

**关键事件**: 冯玉祥北京政变

**冯玉祥选择**:
- **倒戈反直** (历史选择):
  - 直系后院起火
  - 吴佩孚腹背受敌
  - 直系惨败
  - 冯玉祥建立国民军

- **支持直系** (替代选择):
  - 奉系难以入关
  - 战争可能陷入僵持

**战争结果**:
- **奉系胜利** (历史结果):
  - 直系瓦解
  - 奉系控制北京
  - 段祺瑞重新出山 (临时执政)
  - 开启奉系时代

**后续事件**:
- 冯玉祥驱逐溥仪出宫
- 郭松龄反奉 (1925)

**历史参考**: 1924年9月15日-11月3日，奉系获胜，直系主力被歼。

---

### 2.3 北伐战争时期事件 (1926-1928)

#### 事件8: 北伐战争启动 (1926)

**事件ID**: `EV_NORTHERN_EXPEDITION_START`

**事件描述**:
国民革命军从广东出师北伐，意图统一全国。

**触发条件**:
- 时间: 1926年7月
- 势力: 国民政府
- 前提: 国民政府巩固广东、国共合作

**北伐机制**:

**国民革命军**:
- 总司令: 蒋介石
- 主要将领: 李宗仁、唐生智、冯玉祥 (后期)
- 军队: 国民革命军8个军

**作战方针**:
- "打倒列强，除军阀"
- 分三路北伐
- 集中兵力各个击破

**北伐进程**:
1. **第一阶段** (1926.7-1927.3):
   - 攻克湖南、湖北
   - 消灭吴佩孚主力
   - 占领武汉

2. **第二阶段** (1927.4-1928.6):
   - 宁汉分裂
   - 清党事件
   - 继续北伐
   - 攻克北京

**历史结果**: 1928年北伐完成，东北易帜，形式上统一全国。

---

#### 事件9: 四一二政变 (1927)

**事件ID**: `EV_APRIL_12_COUP`

**事件描述**:
蒋介石在上海发动清党，大规模屠杀共产党人，国共合作破裂。

**触发条件**:
- 时间: 1927年4月12日
- 势力: 国民政府 (蒋介石派系)
- 前提: 北伐进行中、国共矛盾激化

**势力选择**:

**国民政府 (蒋介石)**:
- **发动清党** (历史选择):
  - 国共合作破裂
  - 大规模逮捕屠杀共产党人
  - 获得江浙财团支持
  - 美国等西方国家支持
  - 国民党右派支持
  - 但失去工农支持
  - 共产党转入地下/武装斗争

- **维持合作** (替代选择):
  - 国共继续合作
  - 北伐力量增强
  - 但内部矛盾持续
  - 西方国家敌视

**中国共产党**:
- 被动应对清党
- 转入地下活动
- 开始武装斗争 (南昌起义、秋收起义)
- 建立农村根据地

**历史影响**:
- 国共第一次合作破裂
- 白色恐怖
- 中国共产党开始独立武装斗争
- 南京国民政府建立

---

#### 事件10: 宁汉分裂与合流 (1927)

**事件ID**: `EV_NANJING_WUHAN_SPLIT`

**事件描述**:
北伐过程中，国民党内部分裂为南京 (蒋介石) 和武汉 (汪精卫) 两个政府。

**触发条件**:
- 时间: 1927年
- 势力: 国民政府内部
- 前提: 北伐进行中、党内矛盾

**分裂原因**:
- 对共产党态度不同
- 对蒋介石独裁不满
- 权力分配矛盾

**事件发展**:
1. **宁汉分裂**: 两个国民政府并存
2. **武汉分共**: 汪精卫也清党
3. **宁汉合流**: 南京政府统一

**历史结果**: 蒋介石下野后又复出，最终南京政府统一。

---

### 2.4 国共内战时期事件 (1927-1937)

#### 事件11: 南昌起义 (1927)

**事件ID**: `EV_NANCHANG_UPRISING`

**事件描述**:
中国共产党发动武装起义，打响武装反抗国民党反动派的第一枪。

**触发条件**:
- 时间: 1927年8月1日
- 势力: 中国共产党
- 前提: 四一二政变后

**起义机制**:

**参与者**:
- 周恩来、贺龙、叶挺、朱德、刘伯承等
- 约2万人

**起义结果**:
- 占领南昌 (短暂)
- 南下广东途中失败
- 剩余部队由朱德、陈毅率领
- 最终上井冈山与毛泽东会师

**历史意义**:
- 中国共产党独立领导武装斗争的开始
- 人民军队的诞生日 (8月1日建军节)
- 开启农村包围城市道路

---

#### 事件12: 井冈山会师 (1928)

**事件ID**: `EV_JINGGANGSHAN_JOINING`

**事件描述**:
毛泽东率领的秋收起义部队与朱德、陈毅率领的南昌起义余部在井冈山会师。

**触发条件**:
- 时间: 1928年4月
- 势力: 中国共产党
- 前提: 南昌起义余部到达、毛泽东在井冈山

**会师效果**:
- 成立中国工农红军第四军
- 朱德任军长，毛泽东任党代表
- 开启井冈山革命根据地
- 农村包围城市道路确立

**历史意义**:
- 工农红军正式建立
- 农村革命根据地模式
- "工农武装割据"理论实践

---

#### 事件13: 九一八事变 (1931)

**事件ID**: `EV_MUKDEN_INCIDENT`

**事件描述**:
日本关东军炸毁南满铁路，发动侵华战争，东北沦陷。

**触发条件**:
- 时间: 1931年9月18日
- 特殊事件: 必然发生
- 影响: 所有中国势力

**事件过程**:
1. 日军炸毁柳条湖铁路
2. 诬陷中国军队所为
3. 进攻北大营
4. 占领沈阳
5. 四个月占领东北三省

**各方反应**:

**国民政府 (蒋介石)**:
- "攘外必先安内"政策
- 不抵抗命令
- 寄希望于国际联盟
- 张学良执行不抵抗

**中国共产党**:
- 发表抗日宣言
- 组织东北抗日联军
- 但力量有限

**东北军**:
- 执行不抵抗命令
- 撤入关内
- 东北沦陷
- 张学良背负骂名

**影响效果**:
- 东北沦陷 (1931-1945)
- 伪满洲国建立
- 全国抗日情绪高涨
- 国民政府威望下降
- 中国共产党抗日主张获得支持

**历史意义**:
- 日本侵华战争开始
- 中国局部抗战开始
- 国际联盟调解失败

---

#### 事件14: 长征 (1934-1936)

**事件ID**: `EV_LONG_MARCH`

**事件描述**:
红军因第五次反围剿失败，被迫进行战略转移，长征二万五千里。

**触发条件**:
- 时间: 1934年10月
- 势力: 中国共产党
- 前提: 第五次反围剿失败

**长征机制**:

**出发**: 江西瑞金
**途经**: 江西、福建、广东、湖南、广西、贵州、云南、四川、西康、甘肃、陕西
**终点**: 陕北吴起镇
**路程**: 约二万五千里 (12500公里)
**时间**: 1934年10月-1936年10月

**关键事件**:
- 湘江战役 (惨烈)
- 遵义会议 (转折点)
- 四渡赤水 (经典)
- 飞夺泸定桥
- 过雪山草地
- 会宁会师

**遵义会议**:
- 时间: 1935年1月
- 内容: 确立毛泽东领导地位
- 意义: 生死攸关的转折点

**长征结果**:
- 红军从8万减少到7千 (到达陕北)
- 但保存了革命火种
- 播撒了革命种子
- 宣传了抗日主张
- 确立了毛泽东领导

**历史意义**:
- "长征是宣言书，长征是宣传队，长征是播种机"
- 中国共产党从幼稚走向成熟
- 确立了毛泽东的领导地位

---

### 2.5 抗日战争时期事件 (1937-1945)

#### 事件15: 七七事变 (1937)

**事件ID**: `EV_MARCO_POLO_BRIDGE`

**事件描述**:
日军在北平附近卢沟桥挑起事变，全面侵华战争开始。

**触发条件**:
- 时间: 1937年7月7日
- 特殊事件: 必然发生
- 影响: 所有中国势力

**事件过程**:
1. 日军在卢沟桥演习
2. 借口士兵失踪要求入城搜查
3. 遭拒后炮击宛平城
4. 中国守军奋起抵抗
5. 全面抗战爆发

**各方反应**:

**国民政府**:
- 蒋介石发表庐山谈话
- "地无分南北，人无分老幼，皆有守土抗战之责"
- 国共第二次合作
- 全面抗战开始

**中国共产党**:
- 发表抗日通电
- 红军改编为八路军、新四军
- 开辟敌后战场

**影响效果**:
- 国共第二次合作
- 抗日民族统一战线建立
- 全面抗战开始
- 国民政府迁都重庆
- 南京大屠杀 (1937.12)

**历史意义**:
- 中国全民族抗战开始
- 第二次世界大战东方主战场

---

#### 事件16: 西安事变 (1936)

**事件ID**: `EV_XIAN_INCIDENT`

**事件描述**:
张学良、杨虎城在西安发动兵谏，扣押蒋介石，逼蒋抗日。

**触发条件**:
- 时间: 1936年12月12日
- 势力: 张学良 (奉系残余)
- 前提: 蒋介石坚持"攘外必先安内"

**事件过程**:
1. 张学良多次劝蒋抗日被拒
2. 与杨虎城密谋兵谏
3. 12月12日扣押蒋介石
4. 通电全国，提出八项主张
5. 中共派周恩来调解
6. 蒋介石被迫接受停止内战、一致抗日
7. 张学良送蒋回南京后被软禁

**各方选择**:

**张学良**:
- **发动兵谏** (历史选择):
  - 扣押蒋介石
  - 逼蒋抗日成功
  - 但个人被软禁终身
  - 东北军瓦解

**中国共产党**:
- 派周恩来调解
- 主张和平解决
- 逼蒋抗日
- 获得政治主动

**结果效果**:
- 国共第二次合作
- 全面抗日统一战线
- 十年内战基本结束
- 张学良被软禁 (终身)

**历史意义**:
- 时局转换的枢纽
- 促成了抗日民族统一战线
- 改变了历史走向

---

#### 事件17: 抗日战争胜利 (1945)

**事件ID**: `EV_ANTI_JAPANESE_VICTORY`

**事件描述**:
日本宣布无条件投降，中国抗日战争取得胜利。

**触发条件**:
- 时间: 1945年8月15日
- 特殊事件: 必然发生 (盟军胜利)

**战争结果**:
- 日本无条件投降
- 中国获得联合国安理会常任理事国席位
- 台湾回归
- 国际地位大幅提升

**战后格局**:
- 国共矛盾重新激化
- 美国调解失败
- 内战一触即发

---

### 2.6 国共内战时期事件 (1945-1949)

#### 事件18: 重庆谈判 (1945)

**事件ID**: `EV_CHONGQING_NEGOTIATION`

**事件描述**:
抗日战争胜利后，国共两党在重庆进行和平谈判。

**触发条件**:
- 时间: 1945年8月-10月
- 势力: 国民政府 vs 中共
- 前提: 抗战胜利

**谈判过程**:
- 毛泽东亲赴重庆
- 签订《双十协定》
- 但和平未能持久

**结果**:
- 形式上达成和平协议
- 实际上内战准备继续
- 为内战争取时间/道义制高点

---

#### 事件19: 三大战役 (1948-1949)

**事件ID**: `EV_THREE_MAJOR_CAMPAIGNS`

**事件描述**:
辽沈战役、淮海战役、平津战役，决定国共内战胜负。

**触发条件**:
- 时间: 1948年9月-1949年1月
- 势力: 中共 vs 国民政府
- 前提: 全面内战爆发

**战役机制**:

**辽沈战役** (1948.9-11):
- 地点: 东北地区
- 结果: 东北全境解放
- 意义: 解放军首次在兵力上超过国民党军

**淮海战役** (1948.11-1949.1):
- 地点: 华东地区
- 结果: 歼灭国民党军主力
- 意义: 解放战争决定性战役

**平津战役** (1948.11-1949.1):
- 地点: 华北地区
- 结果: 北平和平解放
- 意义: 保护古都完整

**战役结果**:
- 国民党军主力被歼
- 长江以北解放
- 国民政府败局已定

---

#### 事件20: 新中国成立 (1949)

**事件ID**: `EV_NEW_CHINA_FOUNDING`

**事件描述**:
中华人民共和国成立，中国历史进入新纪元。

**触发条件**:
- 时间: 1949年10月1日
- 势力: 中国共产党
- 前提: 三大战役胜利、渡江战役成功

**事件过程**:
- 渡江战役 (1949.4)
- 南京解放
- 上海解放
- 全国政协召开
- 开国大典

**历史意义**:
- 中国结束半殖民地半封建社会
- 新民主主义革命胜利
- 中国历史新纪元

---

## 3. 势力专属事件链

### 3.1 北洋政府专属事件

#### 事件链: 北洋兴衰

**事件A1**: 袁世凯称帝尝试
- 触发: 1915年
- 选择: 称帝/放弃
- 影响: 开启不同历史线

**事件A2**: 护国战争应对
- 触发: 称帝后
- 任务: 镇压/妥协
- 影响: 北洋权威

**事件A3**: 后袁世凯时代
- 触发: 1916年袁世凯病逝
- 分裂: 北洋集团分裂
- 影响: 军阀割据开始

**事件A4**: 府院之争
- 触发: 1917年
- 平衡: 总统府vs国务院
- 危机: 政治瘫痪风险

**事件A5**: 张勋复辟应对
- 触发: 1917年
- 选择: 支持共和/观望
- 影响: 政治声望

**事件A6**: 直皖战争影响
- 触发: 1920年
- 立场: 支持直系/皖系/中立
- 影响: 战后地位

**事件A7**: 北伐冲击
- 触发: 1926年
- 危机: 北洋政府存亡
- 结局: 被国民政府取代

---

### 3.2 国民政府专属事件

#### 事件链: 北伐与统一

**事件B1**: 建立黄埔军校
- 触发: 1924年
- 效果: 获得黄埔系军官
- 长期: 军官团优势

**事件B2**: 国民党一大
- 触发: 1924年
- 改组: 确立三大政策
- 合作: 国共合作开始

**事件B3**: 广州国民政府成立
- 触发: 1925年
- 准备: 北伐准备
- 统一: 两广统一

**事件B4**: 出师北伐
- 触发: 1926年7月
- 战役: 连续作战
- 进展: 占领武汉、南京

**事件B5**: 宁汉分裂危机
- 触发: 1927年
- 选择: 坚持清党/维持合作
- 影响: 党内团结

**事件B6**: 东北易帜
- 触发: 1928年
- 统一: 形式统一全国
- 成果: 国民政府权威

---

### 3.3 中国共产党专属事件

#### 事件链: 从建党到建国

**事件C1**: 中共一大
- 触发: 1921年
- 建党: 中国共产党成立
- 初期: 城市工人运动

**事件C2**: 国共第一次合作
- 触发: 1924年
- 合作: 党内合作方式
- 发展: 工农运动高涨

**事件C3**: 四一二政变应对
- 触发: 1927年
- 危机: 大屠杀
- 转向: 武装斗争

**事件C4**: 三大起义
- 触发: 1927年
- 南昌起义/秋收起义/广州起义
- 建军: 人民军队诞生

**事件C5**: 井冈山道路
- 触发: 1927-1928年
- 探索: 农村包围城市
- 理论: 工农武装割据

**事件C6**: 长征
- 触发: 1934年
- 转移: 战略转移
- 转折: 遵义会议

**事件C7**: 抗日战争
- 触发: 1937年
- 合作: 第二次国共合作
- 发展: 敌后战场壮大

**事件C8**: 解放战争
- 触发: 1946年
- 战略: 从防御到进攻
- 决战: 三大战役

**事件C9**: 新中国成立
- 触发: 1949年
- 胜利: 新民主主义革命胜利
- 结局: 最终胜利

---

## 4. 事件系统设计规范

### 4.1 事件数据结构

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FHistoricalEvent
{
    GENERATED_BODY()

    // 基本信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEventType EventType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggerYear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggerMonth;

    // 触发条件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FEventCondition> TriggerConditions;

    // 参与势力
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EFactionType> InvolvedFactions;

    // 玩家选择
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FEventChoice> Choices;

    // 事件结果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FEventEffect> Effects;

    // 后续事件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> FollowUpEvents;

    // 历史图片
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalImage;

    // 是否已触发
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasTriggered;

    // 触发次数
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggerCount;

    FHistoricalEvent()
        : EventType(EEventType::Historical)
        , TriggerYear(1912)
        , TriggerMonth(1)
        , bHasTriggered(false)
        , TriggerCount(0)
    {}
};
```

### 4.2 事件管理器

```cpp
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingEventManager : public UActorComponent
{
    GENERATED_BODY()

public:
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    void InitializeEventManager();

    // 事件注册
    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    void RegisterEvent(const FHistoricalEvent& Event);

    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    void RegisterAllHistoricalEvents();

    // 事件查询
    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    bool HasEvent(const FString& EventID) const;

    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    FHistoricalEvent GetEvent(const FString& EventID) const;

    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    TArray<FHistoricalEvent> GetEventsByYear(int32 Year) const;

    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    TArray<FHistoricalEvent> GetEventsByFaction(EFactionType Faction) const;

    // 事件触发
    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    bool TriggerEvent(const FString& EventID);

    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    bool CanTriggerEvent(const FString& EventID) const;

    // 选择处理
    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    bool MakeEventChoice(const FString& EventID, int32 ChoiceIndex);

    // 事件检查
    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    void CheckEventsForYear(int32 Year);

    UFUNCTION(BlueprintCallable, Category = "Event Manager")
    void CheckEventsForFactionTurn(EFactionType Faction);

    // 委托事件
    UPROPERTY(BlueprintAssignable, Category = "Event Manager")
    FOnEventTriggered OnEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Event Manager")
    FOnEventChoiceMade OnEventChoiceMade;

protected:
    UPROPERTY()
    TMap<FString, FHistoricalEvent> AllEvents;

    UPROPERTY()
    TArray<FString> TriggeredEvents;

    UPROPERTY()
    TArray<FString> ActiveEvents;

    // 初始化历史事件
    void InitializeBeiyangEvents();
    void InitializeNationalistEvents();
    void InitializeCommunistEvents();
    void InitializeFengtianEvents();
    void InitializeZhiliEvents();
    void InitializeWanmeiEvents();
    void InitializeShanxiEvents();
    void InitializeGuangxiEvents();
    void InitializeYunnanEvents();
    void InitializeSichuanEvents();
    void InitializeMajiaEvents();
    void InitializeXinjiangEvents();
    void InitializeGlobalEvents();

    // 条件检查
    bool CheckEventConditions(const FHistoricalEvent& Event) const;
    bool CheckFactionCondition(EFactionType RequiredFaction, EFactionType CurrentFaction) const;
    bool CheckYearCondition(int32 RequiredYear, int32 CurrentYear) const;
    bool CheckPrerequisiteEvent(const FString& PrerequisiteEventID) const;

    // 效果应用
    void ApplyEventEffects(const FHistoricalEvent& Event, int32 ChoiceIndex);
    void ApplyNumericEffects(const TMap<FString, float>& Effects, EFactionType TargetFaction);
    void ApplyPoliticalEffects(const TMap<FString, FString>& Effects, EFactionType TargetFaction);

    // 触发后续事件
    void TriggerFollowUpEvents(const FHistoricalEvent& Event);

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction) override;
};
```

---

## 5. 事件编辑器与工具

### 5.1 事件编辑器功能

```cpp
// 事件编辑器Blueprint接口
UFUNCTION(BlueprintCallable, Category = "Event Editor")
bool K2_CreateEvent(FHistoricalEvent EventData);

UFUNCTION(BlueprintCallable, Category = "Event Editor")
bool K2_ModifyEvent(const FString& EventID, FHistoricalEvent NewData);

UFUNCTION(BlueprintCallable, Category = "Event Editor")
bool K2_DeleteEvent(const FString& EventID);

UFUNCTION(BlueprintCallable, Category = "Event Editor")
TArray<FHistoricalEvent> K2_GetAllEvents();

UFUNCTION(BlueprintCallable, Category = "Event Editor")
bool K2_ExportEventsToJSON(const FString& FilePath);

UFUNCTION(BlueprintCallable, Category = "Event Editor")
bool K2_ImportEventsFromJSON(const FString& FilePath);
```

### 5.2 事件测试工具

```cpp
// 事件测试
UFUNCTION(BlueprintCallable, Category = "Event Testing")
bool K2_SimulateEvent(const FString& EventID);

UFUNCTION(BlueprintCallable, Category = "Event Testing")
bool K2_SimulateEventChain(const FString& StartingEventID);

UFUNCTION(BlueprintCallable, Category = "Event Testing")
TMap<FString, float> K2_PredictEventEffects(const FString& EventID, int32 ChoiceIndex);
```

---

## 6. 事件与机制的联动

### 6.1 机制触发事件

```cpp
// 示例：北洋府院之争机制触发事件
void UMingEventManager::CheckFactionalStrifeEvents(EFactionType Faction)
{
    if (Faction != EFactionType::Beiyang) return;

    // 获取机制管理器
    UMingMechanicManager* MechanicManager = GetMechanicManager();
    if (!MechanicManager) return;

    // 检查府院之争机制
    if (MechanicManager->IsMechanicActive(TEXT("BEIYANG_FACTIONAL_STRIFE")))
    {
        float CivilPower = MechanicManager->GetMechanicNumericValue(
            TEXT("BEIYANG_FACTIONAL_STRIFE"), TEXT("CivilFactionPower"));
        float MilitaryPower = MechanicManager->GetMechanicNumericValue(
            TEXT("BEIYANG_FACTIONAL_STRIFE"), TEXT("MilitaryFactionPower"));

        // 派系失衡触发事件
        if (FMath::Abs(CivilPower - MilitaryPower) > 30.0f)
        {
            TriggerEvent(TEXT("BEIYANG_FACTIONAL_CRISIS"));
        }

        // 严重失衡触发政变
        if (FMath::Abs(CivilPower - MilitaryPower) > 50.0f)
        {
            TriggerEvent(TEXT("BEIYANG_COUP_ATTEMPT"));
        }
    }
}
```

### 6.2 事件影响机制

```cpp
// 示例：护国战争事件影响机制
void UMingEventManager::ApplyNationalProtectionWarEffects(int32 ChoiceIndex)
{
    UMingMechanicManager* MechanicManager = GetMechanicManager();

    switch (ChoiceIndex)
    {
    case 0: // 支持护国
        // 激活护国机制
        MechanicManager->ActivateMechanic(TEXT("YUNNAN_NATIONAL_PROTECTION"));
        // 提升滇系护国传统机制等级
        MechanicManager->UpgradeMechanic(TEXT("YUNNAN_NATIONAL_PROTECTION"));
        break;

    case 1: // 支持北洋
        // 提升北洋中央正统机制
        MechanicManager->UpgradeMechanic(TEXT("BEIYANG_CENTRAL_LEGITIMACY"));
        // 但声誉受损
        ModifyFactionReputation(EFactionType::Beiyang, -30.0f);
        break;

    case 2: // 保持中立
        // 无特殊效果，但可能错失机遇
        break;
    }
}
```

---

## 7. 历史事件统计与分析

### 7.1 事件统计

```cpp
// 事件统计结构
USTRUCT(BlueprintType)
struct MINGGORTS_API FEventStatistics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggeredEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EEventType, int32> EventsByType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, int32> EventsByYear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EFactionType, int32> EventsByFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> MostTriggeredEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> AverageChoiceDistribution;
};
```

### 7.2 历史模拟分析

```cpp
// 历史路径分析
UFUNCTION(BlueprintCallable, Category = "Event Analysis")
FHistoryPath K2_AnalyzeHistoryPath(TArray<FString> TriggeredEventChain);

// 替代历史模拟
UFUNCTION(BlueprintCallable, Category = "Event Analysis")
TArray<FHistoryPath> K2_SimulateAlternativeHistories(const FString& DivergenceEventID, int32 NumSimulations);

// 关键事件识别
UFUNCTION(BlueprintCallable, Category = "Event Analysis")
TArray<FString> K2_IdentifyCriticalEvents();
```

---

**文档版本**: 1.0  
**最后更新**: 2026-03-23  
**作者**: MingGoRTS Design Team  
**状态**: ✅ 已完成

**下一文档**: 其他辅助系统设计文档
