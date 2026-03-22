# 北洋政府建筑资产3D制作技术文档

## 基本信息

**资产名称**: SM_Beiyang_Building_[Name]  
**资产类型**: 建筑资产 (RTS游戏)  
**所属势力**: 北洋政府  
**制作周期**: 12天 (6座建筑)  
**负责人**: 3D环境美术师 (2人)  
**审核人**: 主美术 / 场景美术总监

---

## 第一部分: 建筑清单一览

| 建筑 | 类型 | 尺寸 (米) | 复杂度 | 工天数 | 优先级 |
|------|------|-----------|--------|--------|--------|
| 北洋指挥部 | 主基地 | 40x30x15 | 极高 | 4天 | P0 |
| 北洋兵营 | 兵营 | 25x15x8 | 高 | 2天 | P0 |
| 北洋军工厂 | 军械厂 | 30x20x10 | 高 | 2天 | P0 |
| 北洋资源站 | 资源建筑 | 20x12x6 | 中 | 2天 | P1 |
| 北洋防御工事 | 防御塔 | 8x8x5 | 中 | 1天 | P1 |
| 北洋仓库 | 储存建筑 | 15x10x6 | 低 | 1天 | P2 |

---

## 第二部分: 通用建筑规范

### 2.1 中西合璧风格指南

```
北洋建筑特征:
├── 主体结构
│   ├── 西式框架结构
│   ├── 红砖墙体
│   └── 混凝土基础
├── 屋顶
│   ├── 西式坡屋顶
│   ├── 中式翘角元素 (可选)
│   └── 铁皮/瓦片材质
├── 门窗
│   ├── 西式拱形门廊
│   ├── 中式格窗元素
│   └── 金属窗框
├── 装饰
│   ├── 西式柱子
│   ├── 中式雕花 (点缀)
│   └── 北洋旗帜
└── 功能标识
    ├── 军事用途标识
    ├── 势力徽章
    └── 警示标识
```

### 2.2 材质体系

```
主材质:
├── M_Beiyang_Brick_Red (红砖)
│   - 基础色: RGB(165, 85, 65)
│   - 粗糙度: 0.8
│   - 法线: 砖缝细节
├── M_Beiyang_Concrete (混凝土)
│   - 基础色: RGB(180, 180, 175)
│   - 粗糙度: 0.9
├── M_Beiyang_Wood_Dark (深色木材)
│   - 基础色: RGB(80, 60, 45)
│   - 粗糙度: 0.6
├── M_Beiyang_Metal_Painted (涂漆金属)
│   - 基础色: RGB(60, 70, 80) 军绿
│   - 粗糙度: 0.5
│   - 金属度: 0.7
├── M_Beiyang_Roof_Tile (瓦片屋顶)
│   - 基础色: RGB(70, 70, 75)
│   - 粗糙度: 0.85
└── M_Beiyang_Glass (玻璃窗)
    - 透明度: 0.3
    - 反射: 0.8
```

### 2.3 LOD系统

```
建筑LOD规范:

LOD0 (0-50米):
- 完整模型细节
- 所有装饰部件
- 窗户内部可见
- 完整材质

LOD1 (50-150米):
- 简化装饰
- 窗户平面化处理
- 合并小部件
- 材质简化

LOD2 (150-400米):
- 基本轮廓
- 主要体块
- 单一材质
- 无内部细节

LOD3 (400米+):
- 极简体块
-  billboard 或简化模型
- 纯色材质
```

---

## 第三部分: 各建筑详细规格

### 3.1 北洋指挥部 (主基地)

```
资产名称: SM_Beiyang_Headquarters
尺寸: 40m x 30m x 15m (两层)
多边形数: 35,000 tris (LOD0)

功能:
- 主基地建筑
- 科技研发
- 高级单位训练
- 战略指挥中心

建筑结构:
├── 主体建筑 (两层)
│   ├── 一楼: 指挥大厅、通讯室
│   ├── 二楼: 办公室、会议室
│   └── 天台: 观察哨
├── 门廊 (突出主入口)
│   ├── 四根立柱
│   ├── 拱形顶
│   └── 台阶
├── 塔楼 (一角)
│   ├── 三层高
│   ├── 瞭望台
│   └── 无线电天线
├── 附属设施
│   ├── 旗杆 (五色旗)
│   ├── 探照灯
│   ├── 沙袋掩体
│   └── 车辆停放区
└── 围墙/围栏

独特标识:
- 大型北洋徽章 (门廊上方)
- "北洋政府" 字样
- 无线电天线
- 探照灯 (夜间)

建造动画:
1. 地基浇筑
2. 墙体砌筑
3. 屋顶搭建
4. 门窗安装
5. 装饰/标识

受损状态:
- 25%: 窗户破碎
- 50%: 墙体裂缝
- 75%: 部分坍塌
- 100%: 废墟
```

### 3.2 北洋兵营

```
资产名称: SM_Beiyang_Barracks
尺寸: 25m x 15m x 8m (单层长条)
多边形数: 20,000 tris (LOD0)

功能:
- 步兵训练
- 单位驻扎
- 基础医疗

建筑结构:
├── 主楼 (长条形)
│   ├── 宿舍区 (多窗户)
│   ├── 食堂 (大窗户)
│   ├── 储藏室
│   └── 武器库
├── 训练场 (建筑前)
│   ├── 操练区域
│   ├── 靶场标识
│   └── 器材架
├── 旗杆
│   └── 北洋旗帜
└── 围栏/大门

独特标识:
- 训练场标识
- 整齐排列的窗户
- 门口岗哨

建造动画:
1. 地基
2. 墙体
3. 屋顶
4. 内部装修
```

### 3.3 北洋军工厂

```
资产名称: SM_Beiyang_Arsenal
尺寸: 30m x 20m x 10m (单层高挑)
多边形数: 28,000 tris (LOD0)

功能:
- 武器装备生产
- 车辆维修
- 弹药制造

建筑结构:
├── 主厂房 (大跨度)
│   ├── 车间大门 (滑动门)
│   ├── 排烟窗 (屋顶)
│   └── 吊装设备
├── 烟囱
│   ├── 红砖烟囱
│   └── 高度 15m
├── 附属建筑
│   ├── 原料仓库
│   ├── 成品仓库
│   └── 办公小间
├── 轨道/运输设施
│   └── 材料运输轨道
└── 外部设施
    ├── 原料堆放
    ├── 车辆停放
    └── 警戒围栏

独特标识:
- 高大烟囱
- 大型车间门
- 吊装设备
- 工业氛围

特效:
- 烟囱烟雾 (工作时)
- 车间灯光 (夜间)
- 火花 (受损时)
```

### 3.4 北洋资源站

```
资产名称: SM_Beiyang_ResourceStation
尺寸: 20m x 12m x 6m
多边形数: 15,000 tris (LOD0)

功能:
- 资源收集
- 物资储存
- 运输中转

建筑结构:
├── 主仓库
│   ├── 卷帘门
│   ├── 卸货平台
│   └── 通风窗
├── 运输设施
│   ├── 轨道连接
│   ├── 车辆通道
│   └── 装卸区
├── 储罐/储存设施
│   ├── 油料罐
│   ├── 水塔
│   └── 物资堆
└── 控制室
    └── 小办公室

独特标识:
- 大型储罐
- 运输轨道
- 物资堆放
- 工业设备
```

### 3.5 北洋防御工事

```
资产名称: SM_Beiyang_DefenseTower
尺寸: 8m x 8m x 5m
多边形数: 12,000 tris (LOD0)

功能:
- 区域防御
- 警戒观察
- 火力支援点

建筑结构:
├── 主体碉堡
│   ├── 混凝土墙体 (厚重)
│   ├── 射击孔
│   └── 弧形顶部
├── 观察塔 (可选)
│   ├── 高架平台
│   ├── 护栏
│   └── 探照灯
├── 防御设施
│   ├── 沙袋掩体
│   ├── 铁丝网
│   └── 拒马
└── 入口
    └── 加固门

独特标识:
- 厚重混凝土质感
- 射击孔
- 军事氛围
- 防御性外观

武器系统:
- 内置机枪/火炮
- 射击动画
- 火光特效
```

### 3.6 北洋仓库

```
资产名称: SM_Beiyang_Depot
尺寸: 15m x 10m x 6m
多边形数: 10,000 tris (LOD0)

功能:
- 通用物资储存
- 备用装备
- 后勤支援

建筑结构:
├── 主仓库 (简化设计)
│   ├── 双开门
│   ├── 小窗
│   └── 通风口
├── 外部堆放
│   ├── 物资箱
│   ├── 油桶
│   └── 备用材料
└── 围栏

独特标识:
- 简洁实用外观
- 物资堆放
- 后勤功能

简化设计:
- 最低复杂度
- 基础功能
- 快速建造
```

---

## 第四部分: 技术实现

### 4.1 模块化组件系统

```
创建可复用建筑组件:

基础组件:
├── BP_BuildingModule_Wall (墙体)
├── BP_BuildingModule_Roof (屋顶)
├── BP_BuildingModule_Window (窗户)
├── BP_BuildingModule_Door (门)
├── BP_BuildingModule_Pillar (柱子)
├── BP_BuildingModule_Stairs (楼梯)
└── BP_BuildingModule_Decoration (装饰)

北洋专属组件:
├── BP_Beiyang_Emblem (徽章)
├── BP_Beiyang_Flag (旗帜)
├── BP_Beiyang_Sandbag (沙袋)
├── BP_Beiyang_Spotlight (探照灯)
└── BP_Beiyang_Antenna (天线)
```

### 4.2 建造系统

```
蓝图逻辑: BP_BuildingMaster_Beiyang

状态:
- UnderConstruction: 建造中
- Operational: 运营中
- Damaged: 受损
- Destroyed: 被毁

建造阶段 (可视化):
1. Foundation (地基)
   - 混凝土基础
   - 建造进度 0-20%

2. Structure (结构)
   - 墙体搭建
   - 建造进度 20-60%

3. Roof (屋顶)
   - 屋顶安装
   - 建造进度 60-80%

4. Finish (装修)
   - 门窗安装
   - 装饰添加
   - 建造进度 80-100%

受损状态:
- 根据受损百分比显示不同模型状态
- 粒子效果 (火焰、烟雾)
- 材质变化 (烧焦、破损)
```

### 4.3 势力标识系统

```
所有建筑统一元素:

视觉标识:
- 五色旗 (飘扬)
- 北洋徽章
- 建筑风格一致性

颜色规范:
- 主色调: 灰蓝色 (RGB 90, 107, 124)
- 强调色: 金色 (装饰)
- 辅助色: 红砖色、军绿色

图标标识:
- 建筑类型图标
- 势力徽章小图标
- 状态指示器
```

---

## 第五部分: 文件规范

### 5.1 命名规范

```
建筑模型: SM_Beiyang_[BuildingName]
  例: SM_Beiyang_Headquarters, SM_Beiyang_Barracks

材质: M_Beiyang_[MaterialType]
  例: M_Beiyang_Brick_Red, M_Beiyang_Concrete

材质实例: MI_Beiyang_[Building]_[Part]
  例: MI_Beiyang_HQ_Wall, MI_Beiyang_Barracks_Roof

贴图: T_Beiyang_[Type]_[Name]
  例: T_Beiyang_Brick_BC, T_Beiyang_Roof_N

蓝图: BP_Beiyang_[Building]
  例: BP_Beiyang_Headquarters, BP_Beiyang_Barracks
```

### 5.2 文件夹结构

```
Content/Buildings/Beiyang/
├── Headquarters/
│   ├── Mesh/
│   ├── Materials/
│   ├── Textures/
│   └── Blueprints/
├── Barracks/
├── Arsenal/
├── ResourceStation/
├── DefenseTower/
├── Depot/
└── Shared/
    ├── Components/
    ├── Materials/
    ├── Textures/
    └── Decals/
```

---

## 第六部分: 验收标准

### 6.1 视觉验收

- [ ] 所有6座建筑完成
- [ ] 中西合璧风格统一
- [ ] 北洋标识清晰可见
- [ ] 材质质感正确
- [ ] LOD切换流畅

### 6.2 技术验收

- [ ] 建造动画完整
- [ ] 受损状态显示正确
- [ ] 碰撞体准确
- [ ] 性能符合预算
- [ ] 模块化组件可复用

### 6.3 游戏验收

- [ ] 建筑可被选中
- [ ] 建造流程正常
- [ ] 功能与类型匹配
- [ ] 与其他势力建筑区分明显
- [ ] 建造位置合理

---

## 附录

### 参考文档

- `EPIC_6_1_1_BEIYANG_CONCEPT_DESIGN.md` - 北洋概念设计
- `EPIC_6_3D_ASSET_PRODUCTION_MASTER_GUIDE.md` - 3D制作总指南

### 交付清单

```
6座建筑:
├── SM_Beiyang_Headquarters.fbx + .uasset
├── SM_Beiyang_Barracks.fbx + .uasset
├── SM_Beiyang_Arsenal.fbx + .uasset
├── SM_Beiyang_ResourceStation.fbx + .uasset
├── SM_Beiyang_DefenseTower.fbx + .uasset
└── SM_Beiyang_Depot.fbx + .uasset

蓝图:
├── BP_Beiyang_Headquarters.uasset
├── BP_Beiyang_Barracks.uasset
├── BP_Beiyang_Arsenal.uasset
├── BP_Beiyang_ResourceStation.uasset
├── BP_Beiyang_DefenseTower.uasset
├── BP_Beiyang_Depot.uasset
└── BP_BuildingMaster_Beiyang.uasset (父类)

材质与贴图:
├── M_Beiyang_Brick_Red.uasset
├── M_Beiyang_Concrete.uasset
├── M_Beiyang_Wood_Dark.uasset
├── M_Beiyang_Metal_Painted.uasset
├── M_Beiyang_Roof_Tile.uasset
├── M_Beiyang_Glass.uasset
└── 相关贴图 (T_*.uasset)
```

---

*文档版本*: 1.0  
*创建日期*: 2026-03-23
