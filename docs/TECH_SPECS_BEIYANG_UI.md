# 北洋政府UI套件制作技术文档

## 基本信息

**资产名称**: T_Beiyang_UI_[Name]  
**资产类型**: UI美术资产 (RTS游戏)  
**所属势力**: 北洋政府  
**制作周期**: 4天  
**负责人**: UI美术师  
**审核人**: UI主设计师 / 主美术

---

## 第一部分: UI清单一览

| 类别 | 项目 | 数量 | 尺寸 | 优先级 |
|------|------|------|------|--------|
| 单位图标 | 4个单位图标 | 4个 | 64x64 | P0 |
| 建筑图标 | 6个建筑图标 | 6个 | 64x64 | P0 |
| 势力徽章 | 北洋徽章 | 1个 | 256x256 | P0 |
| UI边框 | 势力边框 | 5个 | 变尺寸 | P1 |
| UI背景 | 面板背景 | 3个 | 变尺寸 | P1 |
| 加载画面 | 势力加载图 | 1个 | 1920x1080 | P1 |
| 胜利/失败 | 结束画面 | 2个 | 1920x1080 | P2 |
| 按钮 | 势力按钮样式 | 3个 | 变尺寸 | P1 |
| 血条 | 单位血条样式 | 1套 | 变尺寸 | P1 |

---

## 第二部分: 风格规范

### 2.1 北洋UI风格特征

```
核心特征:
├── 色彩系统
│   ├── 主色: 灰蓝色 (RGB 90, 107, 124)
│   ├── 强调色: 金色 (RGB 255, 215, 0)
│   ├── 辅助色: 深灰 (RGB 60, 60, 65)
│   └── 背景色: 暗灰蓝 (RGB 45, 50, 60)
├── 图形元素
│   ├── 中式边框纹样 (简化)
│   ├── 西式几何装饰
│   ├── 北洋军徽图案
│   └── 旗帜元素
├── 字体风格
│   ├── 中文: 庄重楷体/仿宋
│   ├── 英文: 衬线体 (正式)
│   └── 数字: 等宽清晰
└── 质感表现
    ├── 金属质感 (金色装饰)
    ├── 布料纹理 (背景)
    ├── 皮革质感 (按钮)
    └── 磨砂效果
```

### 2.2 图标设计风格

```
单位图标风格:
- 手绘风格 illustration
- 轮廓清晰可辨
- 色彩体现势力
- 单位特征明确
- 统一角度 (45度俯视)

建筑图标风格:
- 简化建筑外观
- 识别性优先
- 与单位图标统一风格
- 建筑类型特征
```

---

## 第三部分: 详细规格

### 3.1 单位图标 (4个)

```
尺寸: 64x64 像素
格式: PNG (透明背景)
风格: 手绘 illustration

1. 北洋步兵图标
   - 文件名: T_Icon_Beiyang_Infantry.png
   - 内容: 步兵半身像，持汉阳造步枪
   - 配色: 灰蓝制服，金色装饰线
   - 背景: 圆形金色边框

2. 北洋骑兵图标
   - 文件名: T_Icon_Beiyang_Cavalry.png
   - 内容: 骑马军官侧面
   - 配色: 深蓝制服，马匹棕色
   - 背景: 圆形金色边框

3. 北洋炮兵图标
   - 文件名: T_Icon_Beiyang_Artillery.png
   - 内容: 火炮侧面 + 炮兵
   - 配色: 工装灰，金属深色
   - 背景: 圆形金色边框

4. 北洋军官图标
   - 文件名: T_Icon_Beiyang_Officer.png
   - 内容: 军官半身像，配剑
   - 配色: 深藏蓝制服，金色装饰
   - 背景: 圆形金色边框 (加粗，体现英雄)

通用图标规范:
- 统一光照: 左上光源
- 统一角度: 45度俯视
- 统一比例: 头部约占40%
- 边框: 2像素金色描边
```

### 3.2 建筑图标 (6个)

```
尺寸: 64x64 像素
格式: PNG (透明背景)
风格: 简化建筑外观 illustration

1. 北洋指挥部图标
   - 文件名: T_Icon_Beiyang_HQ.png
   - 内容: 两层主楼 + 门廊
   - 特征: 旗杆、徽章位置

2. 北洋兵营图标
   - 文件名: T_Icon_Beiyang_Barracks.png
   - 内容: 长条建筑
   - 特征: 多窗户、整齐

3. 北洋军工厂图标
   - 文件名: T_Icon_Beiyang_Arsenal.png
   - 内容: 厂房 + 烟囱
   - 特征: 工业感

4. 北洋资源站图标
   - 文件名: T_Icon_Beiyang_Resource.png
   - 内容: 仓库 + 储罐
   - 特征: 储存设施

5. 北洋防御工事图标
   - 文件名: T_Icon_Beiyang_Defense.png
   - 内容: 碉堡 + 射击孔
   - 特征: 防御性外观

6. 北洋仓库图标
   - 文件名: T_Icon_Beiyang_Depot.png
   - 内容: 简化仓库
   - 特征: 简洁实用

建筑图标规范:
- 统一角度: 45度俯视
- 简化处理: 保留主要特征
- 统一边框: 方形金色边框
```

### 3.3 势力徽章

```
尺寸: 256x256 像素 (主尺寸)
      128x128, 64x64, 32x32 (缩放版本)
格式: PNG (透明背景)

设计元素:
├── 中心图案
│   └── 北洋军徽 (简化)
├── 外圈
│   └── 金色装饰环
│   └── 中式回纹
├── 顶部
│   └── 五色旗元素
└── 底部
    └── "北洋" 字样

配色:
- 主色: 灰蓝 (RGB 90, 107, 124)
- 装饰: 金色 (RGB 255, 215, 0)
- 文字: 金色

用途:
- 势力选择界面
- 小地图标识
- 外交界面
- 成就/勋章
- 加载画面
```

### 3.4 UI边框与背景

```
UI边框套件 (5个):

1. 主面板边框
   - 文件名: T_UI_Beiyang_Frame_Main.png
   - 尺寸: 可平铺设计
   - 风格: 金色中式花纹边框
   - 用途: 主菜单、势力面板

2. 单位信息边框
   - 文件名: T_UI_Beiyang_Frame_Unit.png
   - 尺寸: 适应单位信息框
   - 风格: 简化金色边框
   - 用途: 单位选中信息

3. 建筑信息边框
   - 文件名: T_UI_Beiyang_Frame_Building.png
   - 尺寸: 适应建筑信息框
   - 风格: 简化金色边框
   - 用途: 建筑选中信息

4. 按钮边框 (普通)
   - 文件名: T_UI_Beiyang_Button_Normal.png
   - 尺寸: 9-slice 可缩放
   - 风格: 皮革质感 + 金边
   - 用途: 普通按钮

5. 按钮边框 (高亮)
   - 文件名: T_UI_Beiyang_Button_Highlight.png
   - 尺寸: 9-slice 可缩放
   - 风格: 发光金边效果
   - 用途: 选中/悬停按钮

背景套件 (3个):

1. 主背景
   - 文件名: T_UI_Beiyang_BG_Main.png
   - 尺寸: 1920x1080 或平铺设计
   - 风格: 暗灰蓝 + 纹理
   - 用途: 主菜单背景

2. 面板背景 (半透明)
   - 文件名: T_UI_Beiyang_BG_Panel.png
   - 尺寸: 可平铺
   - 风格: 半透明深色 + 边框
   - 用途: 游戏内面板

3. 子面板背景
   - 文件名: T_UI_Beiyang_BG_Sub.png
   - 尺寸: 可平铺
   - 风格: 更浅半透明
   - 用途: 子菜单/工具提示
```

### 3.5 加载与结束画面

```
加载画面:
- 文件名: T_Beiyang_LoadingScreen.png
- 尺寸: 1920x1080
- 格式: PNG/JPEG

内容:
├── 背景
│   └── 北洋建筑/军队场景
├── 中央
│   └── 势力徽章 (大)
│   └── "北洋政府" 标题
├── 底部
│   └── 加载进度条
│   └── 加载提示文字
└── 装饰
    └── 金色边框装饰

胜利画面:
- 文件名: T_Beiyang_Victory.png
- 尺寸: 1920x1080

内容:
- 金色胜利标语
- 北洋旗帜飘扬
- 军队庆祝场景
- "胜利" 大字

失败画面:
- 文件名: T_Beiyang_Defeat.png
- 尺寸: 1920x1080

内容:
- 灰暗色调
- 撤退/战败氛围
- "战败" 字样
- 重新挑战按钮
```

### 3.6 血条与选中标识

```
血条样式:
- 文件名: T_Beiyang_HealthBar.png
- 尺寸: 可缩放 (9-slice)

设计:
- 边框: 金色细边
- 背景: 深色半透明
- 血条: 红色渐变
- 样式: 简洁清晰

选中标识:
- 文件名: T_Beiyang_SelectionRing.png
- 尺寸: 适应不同单位

设计:
- 圆环: 金色
- 效果: 轻微发光
- 动画: 脉冲效果 (程序实现)

英雄单位标识:
- 文件名: T_Beiyang_HeroRing.png
- 设计: 更粗边框 + 特殊图案
```

---

## 第四部分: 技术规范

### 4.1 文件规范

```
命名规范:
- 图标: T_Icon_Beiyang_[Unit/Building].png
- UI元素: T_UI_Beiyang_[Type]_[Name].png
- 场景: T_Beiyang_[Type].png

文件夹结构:
Content/UI/Beiyang/
├── Icons/
│   ├── Units/
│   │   ├── T_Icon_Beiyang_Infantry.png
│   │   ├── T_Icon_Beiyang_Cavalry.png
│   │   ├── T_Icon_Beiyang_Artillery.png
│   │   └── T_Icon_Beiyang_Officer.png
│   └── Buildings/
│       ├── T_Icon_Beiyang_HQ.png
│       ├── T_Icon_Beiyang_Barracks.png
│       ├── T_Icon_Beiyang_Arsenal.png
│       ├── T_Icon_Beiyang_Resource.png
│       ├── T_Icon_Beiyang_Defense.png
│       └── T_Icon_Beiyang_Depot.png
├── Emblem/
│   └── T_Beiyang_Emblem.png
├── Frames/
│   ├── T_UI_Beiyang_Frame_Main.png
│   ├── T_UI_Beiyang_Frame_Unit.png
│   └── T_UI_Beiyang_Frame_Building.png
├── Backgrounds/
│   ├── T_UI_Beiyang_BG_Main.png
│   ├── T_UI_Beiyang_BG_Panel.png
│   └── T_UI_Beiyang_BG_Sub.png
├── Buttons/
│   ├── T_UI_Beiyang_Button_Normal.png
│   └── T_UI_Beiyang_Button_Highlight.png
├── Screens/
│   ├── T_Beiyang_LoadingScreen.png
│   ├── T_Beiyang_Victory.png
│   └── T_Beiyang_Defeat.png
└── HUD/
    ├── T_Beiyang_HealthBar.png
    ├── T_Beiyang_SelectionRing.png
    └── T_Beiyang_HeroRing.png
```

### 4.2 UE5导入设置

```
贴图导入设置:
- 无压缩: 否
- 压缩设置: UI (BC7)
- Mip Gen: 禁用 (UI不需要)
- sRGB: 是 (颜色贴图)

透明贴图:
- 使用 Alpha 通道
- 压缩设置: User Interface 2D

平铺贴图:
- 纹理组: UI
- 平铺设置: Wrap
```

### 4.3 蓝图集成

```
创建材质:
- M_UI_Beiyang_Icon (图标材质)
- M_UI_Beiyang_Transparent (半透明材质)
- M_UI_Beiyang_Frame (边框材质)

材质实例:
- MI_UI_Beiyang_Icon_Unit
- MI_UI_Beiyang_Icon_Building
- MI_UI_Beiyang_Frame_Gold

UI控件:
- WBP_Beiyang_UnitIcon
- WBP_Beiyang_BuildingIcon
- WBP_Beiyang_SelectionPanel
- WBP_Beiyang_HealthBar
```

---

## 第五部分: 制作流程

### Day 1: 图标制作
- [ ] 4个单位图标绘制
- [ ] 6个建筑图标绘制
- [ ] 风格统一检查

### Day 2: 徽章与边框
- [ ] 势力徽章设计
- [ ] 边框套件绘制
- [ ] 按钮样式制作

### Day 3: 背景与场景
- [ ] 加载画面背景
- [ ] 胜利/失败画面
- [ ] 面板背景制作

### Day 4: HUD元素与集成
- [ ] 血条设计
- [ ] 选中标识
- [ ] UE5导入测试
- [ ] 界面集成测试

---

## 第六部分: 验收标准

### 6.1 视觉验收

- [ ] 10个图标完成且风格统一
- [ ] 势力徽章识别度高
- [ ] UI元素风格一致
- [ ] 色彩符合北洋色调
- [ ] 中西合璧风格体现

### 6.2 技术验收

- [ ] 所有贴图导入UE5正常
- [ ] 透明通道正确
- [ ] 缩放版本清晰
- [ ] 9-slice边框可正确缩放
- [ ] UI控件可正常使用

### 6.3 游戏验收

- [ ] 图标在游戏中清晰可辨
- [ ] 选中效果正常
- [ ] 血条显示正确
- [ ] 加载画面显示正常
- [ ] 与其他势力UI区分明显

---

## 附录

### 交付清单

```
图标 (10个):
├── 4个单位图标
├── 6个建筑图标

徽章 (1个 + 缩放):
├── 256x256 (主)
├── 128x128
├── 64x64
└── 32x32

UI元素 (13个):
├── 5个边框
├── 3个背景
├── 2个按钮
├── 1个血条
├── 1个选中环
└── 1个英雄环

场景 (3个):
├── 1个加载画面
├── 1个胜利画面
└── 1个失败画面

总计: 27个文件 + 缩放版本
```

### 相关文档

- `EPIC_6_1_1_BEIYANG_CONCEPT_DESIGN.md` - 北洋概念设计 (UI章节)
- `EPIC_6_1_BEIYANG_ART_IMPLEMENTATION.md` - 实施总文档

---

*文档版本*: 1.0  
*创建日期*: 2026-03-23
