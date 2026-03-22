# UE5 编辑器内容生成操作指南

## 概述

本指南介绍如何在 Unreal Engine 5 编辑器中使用 MingGoRTS AI 内容生成系统创建游戏画面、影片、图像、音乐和音效。

---

## 一、启动内容生成系统

### 1.1 打开 AI 内容生成面板

1. 启动 UE5 编辑器并加载 MingGoRTS 项目
2. 在菜单栏选择 **Window → Developer Tools → AI Content Generator**
3. 或使用快捷键 **Alt + Shift + A**

### 1.2 系统初始化

```cpp
// 在编辑器控制台执行
AIContentManager.Initialize
```

系统会自动加载配置并准备 AI 服务连接。

---

## 二、批次生成游戏画面与素材

### 2.1 批次生成概述

批次生成功能允许您一次性生成多个相关的游戏画面和素材，大大提高内容创建效率。支持以下批次类型：

- **场景批次**: 生成多个相关场景
- **角色批次**: 生成角色及其相关素材
- **建筑批次**: 生成建筑群及其装饰
- **UI批次**: 生成完整的UI界面套件
- **音效批次**: 生成相关的音效和音乐
- **完整项目批次**: 生成整个游戏项目的完整素材

### 2.2 打开批次生成面板

1. 在 AI Content Generator 面板中选择 **Batch Generation** 标签
2. 或使用快捷键 **Alt + Shift + B**

### 2.3 创建批次生成任务

#### 步骤 1: 选择批次类型

**可用批次类型:**
- **Scene Batch**: 场景批次生成
- **Character Batch**: 角色批次生成  
- **Building Batch**: 建筑批次生成
- **UI Batch**: UI界面批次生成
- **Audio Batch**: 音频批次生成
- **Project Batch**: 完整项目批次生成

#### 步骤 2: 配置批次参数

```cpp
// 批次配置示例
FAIBatchConfiguration BatchConfig;
BatchConfig.BatchType = EBatchType::SceneBatch;
BatchConfig.BatchName = "Ming Dynasty Battle Scenes";
BatchConfig.ItemCount = 5; // 生成5个场景
BatchConfig.Quality = EQualityLevel::High;
BatchConfig.Style = EArtStyle::Realistic;
BatchConfig.Theme = "Ming Dynasty Warfare";
```

**主要参数:**
- **Batch Name**: 批次任务名称
- **Item Count**: 生成数量 (1-50)
- **Quality Level**: Draft / Standard / High / Ultra
- **Art Style**: 写实 / 水墨 / 油画 / 像素艺术 / 卡通
- **Theme**: 主题描述
- **Output Format**: 输出格式 (PNG/EXR for images, MP3/WAV for audio)
- **Resolution**: 分辨率设置 (1080p/4K/8K)

#### 步骤 3: 设置生成规则

**场景批次规则:**
```cpp
// 场景变化规则
BatchConfig.VariationRules.Add(EVariationType::TimeOfDay); // 不同时间
BatchConfig.VariationRules.Add(EVariationType::Weather);    // 不同天气
BatchConfig.VariationRules.Add(EVariationType::Camera);    // 不同角度
BatchConfig.VariationRules.Add(EVariationType::Lighting);  // 不同光照
```

**角色批次规则:**
```cpp
// 角色变化规则
BatchConfig.VariationRules.Add(EVariationType::Pose);       // 不同姿势
BatchConfig.VariationRules.Add(EVariationType::Equipment); // 不同装备
BatchConfig.VariationRules.Add(EVariationType::Expression); // 不同表情
BatchConfig.VariationRules.Add(EVariationType::Action);    // 不同动作
```

#### 步骤 4: 配置依赖关系

```cpp
// 设置素材依赖关系
BatchConfig.Dependencies.Add(EAssetType::Texture, EAssetType::Material);
BatchConfig.Dependencies.Add(EAssetType::Mesh, EAssetType::Texture);
BatchConfig.Dependencies.Add(EAssetType::Scene, EAssetType::Lighting);
```

### 2.4 执行批次生成

#### 方法 1: 使用面板界面

1. 点击 **Start Batch Generation** 按钮
2. 系统显示进度条和预估时间
3. 可以随时暂停或取消批次任务

#### 方法 2: 使用代码

```cpp
// 启动批次生成
FString BatchID = AIContentManager.StartBatchGeneration(BatchConfig);

// 监控批次进度
AIContentManager.OnBatchProgress.AddDynamic(this, &AMyActor::OnBatchProgress);

// 批次完成回调
AIContentManager.OnBatchCompleted.AddDynamic(this, &AMyActor::OnBatchCompleted);
```

### 2.5 批次生成监控

#### 实时监控面板

**显示信息:**
- 当前生成进度 (百分比)
- 已完成项目列表
- 当前正在生成的项目
- 剩余预估时间
- 错误和警告信息

#### 进度回调示例

```cpp
UFUNCTION()
void AMyActor::OnBatchProgress(const FString& BatchID, int32 Completed, int32 Total)
{
    float Progress = (float)Completed / (float)Total * 100.0f;
    UE_LOG(LogTemp, Log, TEXT("Batch %s Progress: %.1f%%"), *BatchID, Progress);
}

UFUNCTION()
void AMyActor::OnBatchCompleted(const FString& BatchID, const FAIBatchResult& Result)
{
    UE_LOG(LogTemp, Log, TEXT("Batch %s Completed: %d items, %d errors"), 
           *BatchID, Result.SuccessCount, Result.ErrorCount);
    
    // 处理生成的素材
    for (const FAIGeneratedAsset& Asset : Result.GeneratedAssets)
    {
        ProcessGeneratedAsset(Asset);
    }
}
```

### 2.6 批次生成结果管理

#### 结果查看

1. 在 **Batch Results** 标签中查看所有完成的批次
2. 点击批次名称查看详细结果
3. 可以预览、下载或重新生成任何项目

#### 批次导出

```cpp
// 导出整个批次
AIContentManager.ExportBatch(BatchID, "/Game/GeneratedContent/Batches/");

// 导出特定格式
AIContentManager.ExportBatchAs(BatchID, EExportFormat::UnrealPackage);
```

#### 批次分享

```cpp
// 创建批次分享包
FString ShareID = AIContentManager.CreateBatchSharePackage(BatchID);

// 分享给团队成员
AIContentManager.ShareBatchWithTeam(ShareID, TeamMemberIDs);
```

---

## 三、高级批次生成功能

### 3.1 智能批次模板

系统提供预定义的批次模板，快速开始生成：

#### 模板列表

1. **Complete RTS Game**: 完整RTS游戏素材包
2. **Ming Dynasty Theme**: 明朝主题素材包
3. **Battle Scenes**: 战斗场景系列
4. **Character Pack**: 角色素材包
5. **UI Kit**: UI界面套件
6. **Audio Pack**: 音效音乐包

#### 使用模板

```cpp
// 使用预定义模板
FAIBatchTemplate Template = AIContentManager.GetBatchTemplate("Complete RTS Game");
FAIBatchConfiguration Config = AIContentManager.CreateConfigFromTemplate(Template);

// 自定义模板参数
Config.ItemCount = 10;
Config.Quality = EQualityLevel::Ultra;
Config.Style = EArtStyle::Photorealistic;

// 启动生成
FString BatchID = AIContentManager.StartBatchGeneration(Config);
```

### 3.2 条件批次生成

基于条件智能生成素材：

```cpp
// 设置生成条件
FAIBatchCondition Condition;
Condition.ConditionType = EConditionType::GameContext;
Condition.Context = "Player reached level 10";
Condition.RequiredAssets = {EAssetType::Weapon, EAssetType::Armor};

// 条件触发批次
AIContentManager.RegisterConditionalBatch(Condition, BatchConfig);
```

### 3.3 迭代优化批次

基于反馈自动优化生成结果：

```cpp
// 启用迭代优化
BatchConfig.bEnableIterativeOptimization = true;
BatchConfig.MaxIterations = 3;
BatchConfig.OptimizationCriteria = {EOptimizationCriteria::VisualQuality, 
                                   EOptimizationCriteria::Performance};

// 人工反馈
AIContentManager.ProvideBatchFeedback(BatchID, "The lighting is too dark");
```

---

## 四、批次生成最佳实践

### 4.1 批次规划建议

1. **分批生成**: 大型项目分成多个小批次
2. **优先级设置**: 优先生成关键素材
3. **依赖管理**: 合理设置素材依赖关系
4. **质量平衡**: 根据用途选择合适的质量级别

### 4.2 性能优化

1. **并行生成**: 启用多线程并行生成
2. **缓存利用**: 利用已有素材缓存
3. **资源管理**: 监控系统资源使用
4. **网络优化**: 优化API调用频率

### 4.3 质量控制

1. **预览检查**: 生成前预览配置
2. **中途验证**: 生成过程中验证质量
3. **批量审核**: 批次完成后统一审核
4. **版本管理**: 保留多个版本供选择

---

## 五、生成游戏画面/场景

### 2.1 快速生成场景

**步骤:**
1. 在 AI Content Generator 面板中选择 **Scene** 标签
2. 选择场景类型：
   - 战斗场景 (Battle)
   - 村庄场景 (Village)
   - 宫殿场景 (Palace)
   - 森林场景 (Forest)
   - 自定义 (Custom)

3. 设置参数：
   - **Complexity**: Simple / Medium / Complex / Epic
   - **Style**: 写实 / 水墨 / 油画 / 像素艺术
   - **Description**: 输入场景描述（例如："明朝战场，黄昏时分，烟雾弥漫"）

4. 点击 **Generate Scene** 按钮

### 2.2 使用预设模板

```cpp
// 在蓝图编辑器中调用
Get AI Content Manager → Generate Scene From Template
- Template: Battle Scene
- Theme: Ming Dynasty Epic
- Quality: High
```

**可用模板:**
- `MingBattleField` - 明朝战场
- `MingVillage` - 明朝村庄
- `MingPalace` - 明朝宫殿
- `MingForest` - 竹林山林
- `MingCity` - 古代城市

### 2.3 场景变体生成

1. 生成基础场景后，点击 **Generate Variants**
2. 设置变体数量（2-5个）
3. 系统会自动生成相似但不同的场景版本
4. 在预览窗口中比较选择最佳版本

---

## 三、生成影片

### 3.1 电影级场景渲染

**步骤:**
1. 切换到 **Cinematic** 标签
2. 选择场景或创建新场景
3. 设置相机路径：
   - 点击 **Add Camera Point** 添加相机关键点
   - 调整相机位置和角度
   - 设置移动速度

4. 配置渲染设置：
   - **Resolution**: 1920x1080 / 3840x2160 / 7680x4320
   - **Frame Rate**: 30fps / 60fps
   - **Duration**: 设置影片长度（秒）
   - **Quality**: Standard / High / Ultra / Cinematic

5. 点击 **Render Cinematic** 开始渲染

### 3.2 实时预览

```cpp
// 控制台命令
AI.RenderPreview [SceneName]
```

预览窗口会显示实时渲染效果，可即时调整参数。

### 3.3 导出设置

- **Format**: MP4 / AVI / MOV / Image Sequence
- **Codec**: H.264 / H.265 / ProRes
- **Audio**: 包含背景音乐和音效

---

## 四、生成图像

### 4.1 单张图像生成

**步骤:**
1. 选择 **Image** 标签
2. 输入图像描述：
   ```
   示例：明朝将军，身穿铠甲，手持长枪，
         背景是战场，写实风格，4K质量
   ```

3. 设置参数：
   - **Resolution**: 1024x1024 / 1920x1080 / 3840x2160
   - **Aspect Ratio**: 1:1 / 16:9 / 21:9
   - **Style**: 写实 / 风格化 / 水墨 / 油画
   - **Quality**: Draft / Standard / High / Ultra

4. 点击 **Generate Image**

### 4.2 批量生成

1. 点击 **Batch Generate** 按钮
2. 导入提示词列表（CSV格式）
3. 设置输出文件夹
4. 点击 **Start Batch** 开始批量生成

### 4.3 图像编辑与优化

生成后的图像可以在内置编辑器中调整：
- **Brightness/Contrast**: 亮度/对比度
- **Saturation**: 饱和度
- **Sharpness**: 锐度
- **Color Grading**: 色彩分级
- **Filters**: 滤镜效果

---

## 五、生成音乐

### 5.1 背景音乐生成

**步骤:**
1. 切换到 **Music** 标签
2. 选择音乐类型：
   - **Epic Orchestral** - 史诗管弦乐
   - **Traditional Chinese** - 传统民乐
   - **Ambient** - 环境音乐
   - **Battle** - 战斗音乐
   - **Peaceful** - 平和音乐

3. 设置参数：
   - **Duration**: 30s / 60s / 120s / 300s
   - **Tempo**: 慢速 / 中速 / 快速 / 自适应
   - **Mood**: 平静 / 紧张 / 史诗 / 神秘
   - **Instruments**: 选择乐器组合
     * 古筝 / 琵琶 / 二胡 / 笛子
     * 交响乐团 / 电子音乐

4. 输入风格描述：
   ```
   示例：民国时期战争背景音乐，
         结合传统中国乐器和管弦乐，
         悲壮而激昂，适合北伐场景
   ```

5. 点击 **Generate Music**

### 5.2 民國时期主题音乐

```cpp
// 使用预设主题
Get AI Content Manager → Generate Republic Era Theme
- Theme: Northern Expedition
- Style: Epic Traditional
- Duration: 180s
```

**可用主题:**
- `NorthernExpedition` - 北伐主题
- `ShanghaiNight` - 上海夜生活
- `WarlordConflict` - 军阀混战
- `RevolutionaryPassion` - 革命激情
- `AncientTradition` - 古代传统

### 5.3 音乐变奏与循环

1. 生成基础音乐后，点击 **Create Variations**
2. 选择变奏类型：
   - 情绪变奏（平静/激烈）
   - 乐器变奏（不同乐器组合）
   - 速度变奏（快/慢）

3. 点击 **Create Loop** 创建无缝循环版本

---

## 六、生成音效

### 6.1 环境音效生成

**步骤:**
1. 选择 **Sound Effect** 标签
2. 选择音效类别：
   - **Environment**: 环境音（雨声、风声、街道）
   - **UI**: 界面音效（点击、提示、错误）
   - **Combat**: 战斗音效（武器、爆炸、喊杀）
   - **Character**: 角色音效（脚步声、呼吸声）
   - **Building**: 建筑音效（建造、破坏）

3. 输入音效描述：
   ```
   示例：古代战场环境音，
         包含远处战鼓声、马蹄声、风声，
         循环播放，3分钟长度
   ```

4. 设置参数：
   - **Duration**: 1s / 5s / 30s / 60s / Loop
   - **Intensity**: 低 / 中 / 高 / 动态
   - **3D Audio**: 是否支持3D空间音效

5. 点击 **Generate Sound**

### 6.2 民國时期特色音效

**预设音效库:**

| 音效名称 | 描述 | 使用场景 |
|---------|------|---------|
| `OldShanghaiStreet` | 老上海街道环境音 | 城市地图 |
| `Battlefield1920s` | 1920年代战场 | 战斗场景 |
| `TraditionalVillage` | 传统村庄环境 | 村庄地图 |
| `PalaceCeremony` | 宫廷仪式音效 | 剧情事件 |
| `TrainWhistle` | 蒸汽火车汽笛 | 交通场景 |

### 6.3 音效混合与层叠

1. 生成多个音效后，点击 **Mix Audio**
2. 在时间轴上排列音效层
3. 调整每层音量：
   - Master Volume: 主音量
   - Layer 1-4: 各层音量
   - Crossfade: 交叉淡入淡出

4. 导出为最终音效文件

---

## 七、高级功能

### 7.1 内容项目管理

1. 点击 **Project** 标签
2. 创建新项目：
   ```
   Project Name: MingGoRTS_Campaign_1
   Content Types: Image, Music, Sound
   Theme: Republican Era War
   ```

3. 添加内容提示词列表
4. 点击 **Generate Project** 批量生成所有内容

### 7.2 质量与风格控制

**质量级别:**
- **Draft**: 快速预览，15秒内生成
- **Standard**: 标准质量，平衡速度与质量
- **High**: 高质量，适合正式使用
- **Ultra**: 超高质量，最佳视觉效果
- **Cinematic**: 电影级，专业输出

**艺术风格:**
- **Photorealistic**: 照片级真实
- **Stylized**: 风格化
- **Watercolor**: 水彩画
- **Ink Wash**: 水墨画
- **Oil Painting**: 油画
- **Pixel Art**: 像素艺术
- **Low Poly**: 低多边形

### 7.3 缓存与复用

系统会自动缓存生成的内容：
- 点击 **Cache Manager** 查看缓存
- 可对相似请求复用已生成内容
- 设置缓存过期时间

---

## 八、快捷键汇总

| 快捷键 | 功能 |
|--------|------|
| `Alt + Shift + A` | 打开AI内容生成器 |
| `Alt + Shift + S` | 快速生成场景 |
| `Alt + Shift + I` | 快速生成图像 |
| `Alt + Shift + M` | 快速生成音乐 |
| `Alt + Shift + X` | 快速生成音效 |
| `Ctrl + R` | 重新生成上一个内容 |
| `Ctrl + E` | 导出当前内容 |
| `Ctrl + P` | 预览模式 |

---

## 九、故障排除

### 常见问题

**Q: 生成失败/超时**
- 检查网络连接
- 降低质量级别重试
- 检查API密钥是否有效

**Q: 内容与描述不符**
- 使用更具体的描述
- 添加负面提示词排除不想要的元素
- 尝试不同的艺术风格

**Q: 渲染质量不佳**
- 提高质量级别至 High 或 Ultra
- 检查原场景的光照设置
- 使用后处理效果增强

---

## 十、最佳实践

### 10.1 提示词编写技巧

**好的提示词:**
```
明朝将军，身穿华丽铠甲，手持长枪，
站在战场中央，背景是夕阳下的山脉，
写实风格，4K质量，电影级光照
```

**避免的提示词:**
```
一个古代的人，拿着武器，站在外面
```

### 10.2 工作流程建议

1. **原型阶段**: 使用 Draft 质量快速迭代
2. **测试阶段**: 使用 Standard 质量验证效果
3. **生产阶段**: 使用 High/Ultra 质量生成最终内容
4. **文档记录**: 保存所有提示词和参数设置

### 10.3 版本控制

生成的内容应加入版本控制：
- 使用语义化版本号 (v1.0.0)
- 记录生成参数和提示词
- 保留草稿版本便于对比

---

## 十一、遊戲畫面生成流程 | Game Scene Generation Workflow

### 11.1 MingGoRTS 專屬場景類型

**歷史戰役場景 | Historical Battle Scenes:**

| 場景類型 | 描述 | 建議複雜度 | 推薦風格 |
|---------|------|-----------|---------|
| 北伐戰場 | 1926年北伐戰爭場景 | Epic | 寫實 |
| 軍閥混戰 | 各地軍閥衝突場景 | Complex | 寫實 |
| 城市攻防 | 上海/南京等城市戰 | Complex | 寫實 |
| 鄉村游擊 | 農村游擊戰場景 | Medium | 水墨 |

**民生場景 | Civilian Scenes:**

| 場景類型 | 描述 | 建議複雜度 | 推薦風格 |
|---------|------|-----------|---------|
| 上海灘夜景 | 1920年代上海繁華夜景 | Complex | 寫實 |
| 傳統村莊 | 江南水鄉村莊 | Medium | 水彩 |
| 皇宮大殿 | 紫禁城/總統府 | Complex | 寫實 |
| 茶館酒肆 | 民間茶館酒樓 | Medium | 風格化 |

**單位展示場景 | Unit Showcase Scenes:**

```cpp
// 生成單位展示場景示例
FSceneRequirement UnitShowcase;
UnitShowcase.SceneType = ESceneType::Custom;
UnitShowcase.Description = TEXT("民國時期國民革命軍士兵，身穿灰色軍服，手持步槍，站姿挺拔，背景是北伐戰場廢墟，清晨霧氣彌漫，寫實風格");
UnitShowcase.Complexity = ESceneComplexity::High;
UnitShowcase.Style = ESceneStyle::Photorealistic;
UnitShowcase.bHasCharacters = true;
UnitShowcase.bHasEnvironment = true;
UnitShowcase.bHasEffects = true;

FString ScenePath = SceneGenerator->GenerateScene(UnitShowcase);
```

### 11.2 場景生成工作流程

**Step 1: 需求分析**
```
輸入：遊戲關卡設計需求
├── 場景類型 (戰鬥/民生/展示)
├── 歷史時期 (1920s/北伐/抗戰)
├── 視覺風格 (寫實/水墨/風格化)
├── 複雜度需求 (Simple/Medium/Complex/Epic)
└── 特殊要求 (天氣/時間/氛圍)
```

**Step 2: 自動素材生成**
```
觸發：SceneGenerator.GenerateScene()
├── 分析場景需求
├── 確定所需資產類型
│   ├── 角色模型 (士兵/平民/軍官)
│   ├── 建築物 (房屋/城牆/宮殿)
│   ├── 環境元素 (樹木/山脈/河流)
│   ├── 道具物件 (武器/家具/裝飾)
│   └── 特效元素 (煙霧/火光/塵埃)
├── 並行生成所有素材
└── 組裝完整場景
```

**Step 3: 質量驗證**
```
檢查項目：
✅ 歷史準確性 (服裝/武器/建築符合時代)
✅ 視覺一致性 (風格統一)
✅ 技術規格 (分辨率/格式/大小)
✅ 遊戲適用性 (性能/可玩性)
```

**Step 4: 導入引擎**
```cpp
// 將生成場景導入 UE5
UAIContentManager* ContentManager = GetAIContentManager();
ContentManager->ImportGeneratedScene(
    ScenePath,
    TEXT("/Game/Scenes/Campaign/NorthernExpedition"),
    EImportSettings::AutoOptimize
);
```

---

## 十二、素材規格與格式要求 | Asset Specifications

### 12.1 圖像素材規格

**角色立繪 | Character Portraits:**

| 用途 | 分辨率 | 格式 | 色彩空間 | 文件大小 |
|-----|-------|------|---------|---------|
| 對話頭像 | 512x512 | PNG | sRGB | < 500KB |
| 單位圖示 | 256x256 | PNG/TGA | sRGB | < 200KB |
| 角色插畫 | 2048x2048 | PNG | sRGB | < 5MB |
| 宣傳立繪 | 4096x4096 | PNG/TIFF | sRGB/Adobe RGB | < 20MB |

**場景圖片 | Scene Images:**

| 用途 | 分辨率 | 格式 | 建議風格 |
|-----|-------|------|---------|
| 載入畫面 | 1920x1080 | PNG/JPEG | 史詩寫實 |
| 關卡縮略圖 | 512x512 | PNG | 風格化 |
| 背景圖片 | 3840x2160 | PNG | 寫實/水墨 |
| 地圖紋理 | 2048x2048 | PNG/TGA | 俯視寫實 |

### 12.2 影片素材規格

**過場動畫 | Cinematic Videos:**

| 類型 | 分辨率 | 幀率 | 格式 | 編碼 |
|-----|-------|------|------|------|
| 劇情動畫 | 1920x1080 | 30fps | MP4 | H.264 |
| 宣傳影片 | 3840x2160 | 60fps | MP4/MOV | H.265/ProRes |
| UI背景動畫 | 1920x1080 | 30fps | WebM | VP9 |
| 教學影片 | 1920x1080 | 30fps | MP4 | H.264 |

### 12.3 音頻素材規格

**音樂 | Music:**

| 類型 | 格式 | 採樣率 | 位元深度 | 時長 |
|-----|------|-------|---------|------|
| 背景音樂 | OGG/MP3 | 44.1kHz | 16-bit | 2-5分鐘 |
| 戰鬥音樂 | OGG/WAV | 48kHz | 16-bit | 1-3分鐘 |
| 主題音樂 | WAV/FLAC | 48kHz | 24-bit | 3-8分鐘 |
| 循環音樂 | OGG | 44.1kHz | 16-bit | 30-60秒(無縫) |

**音效 | Sound Effects:**

| 類型 | 格式 | 採樣率 | 位元深度 | 特殊要求 |
|-----|------|-------|---------|---------|
| UI音效 | WAV/OGG | 44.1kHz | 16-bit | 短促清晰 |
| 環境音效 | OGG | 44.1kHz | 16-bit | 循環無縫 |
| 武器音效 | WAV | 48kHz | 16-bit | 立體聲/3D |
| 語音對話 | WAV/OGG | 48kHz | 24-bit | 清晰無雜音 |

### 12.4 MingGoRTS 專屬素材命名規範

**命名格式：**
```
[類型]_[類別]_[名稱]_[變體]_[版本].[擴展名]

示例：
- CHAR_Soldier_NorthernArmy_01_v1.png (角色-士兵-北伐軍)
- SCENE_Battle_NorthernExpedition_Dawn_v2.png (場景-戰鬥-北伐-黎明)
- MUSIC_Battle_EpicOrchestral_Loop_v1.ogg (音樂-戰鬥-史詩-循環)
- SFX_UI_ButtonClick_Confirm_v1.wav (音效-UI-按鈕-確認)
```

**類型前綴：**

| 前綴 | 類型 | 示例 |
|-----|------|------|
| CHAR | 角色 | CHAR_General_Zhongzheng |
| SCENE | 場景 | SCENE_City_Shanghai1920 |
| UNIT | 單位 | UNIT_Infantry_NRA |
| BLDG | 建築 | BLDG_Palace_Presidential |
| MUSIC | 音樂 | MUSIC_Theme_NorthernExpedition |
| SFX | 音效 | SFX_Battle_Gunfire_Rifle |
| UI | 界面 | UI_Icon_Resource_Food |
| VFX | 特效 | VFX_Explosion_Grenade |

---

## 十三、實際使用範例 | Practical Examples

### 13.1 生成北伐戰役完整素材包

**場景設定：** 1926年北伐戰爭 - 攻打武昌城

```cpp
// 創建項目
FAIContentProject NorthernExpeditionPack;
NorthernExpeditionPack.ProjectName = TEXT("NorthernExpedition_Wuchang_1926");
NorthernExpeditionPack.Theme = TEXT("RepublicanEra_War");
NorthernExpeditionPack.RequiredContentTypes = {
    EAIContentType::Scene,
    EAIContentType::Image,
    EAIContentType::Music,
    EAIContentType::SoundEffect
};

// 添加生成提示詞
NorthernExpeditionPack.ContentPrompts.Add({
    EAIContentType::Scene,
    TEXT("武昌城牆攻防戰場景，1926年北伐戰爭，國民革命軍攻打武昌城，城牆高大堅固，煙霧彌漫，炮火連天，寫實風格，史詩氛圍")
});

NorthernExpeditionPack.ContentPrompts.Add({
    EAIContentType::Image,
    TEXT("北伐軍士兵肖像，身穿灰色軍服，佩戴青天白日帽徽，手持步槍，堅毅表情，寫實風格，半身像")
});

NorthernExpeditionPack.ContentPrompts.Add({
    EAIContentType::Music,
    TEXT("北伐戰爭背景音樂，史詩管弦樂結合中國傳統樂器，激昂悲壯，適合攻城戰場景，民國時期風格")
});

NorthernExpeditionPack.ContentPrompts.Add({
    EAIContentType::SoundEffect,
    TEXT("1920年代戰場環境音，炮火聲、槍聲、喊殺聲、號角聲混雜，循環播放，3D空間音效")
});

// 生成項目
FString ProjectId = AIContentManager->CreateAIContentProject(NorthernExpeditionPack);
TArray<FString> GeneratedAssets = AIContentManager->GenerateProjectContent(ProjectId);
```

**生成結果：**
```
NorthernExpedition_Wuchang_1926/
├── Scenes/
│   └── SCENE_Battle_WuchangSiege_Dawn_v1.png (3840x2160)
├── Characters/
│   ├── CHAR_Soldier_NRA_Infantry_01_v1.png (512x512)
│   ├── CHAR_Soldier_NRA_Officer_01_v1.png (512x512)
│   └── CHAR_Soldier_WarlordArmy_01_v1.png (512x512)
├── Music/
│   ├── MUSIC_Battle_NorthernExpedition_Main_v1.ogg (3:45)
│   └── MUSIC_Battle_NorthernExpedition_Loop_v1.ogg (0:45, 無縫)
└── SFX/
    ├── SFX_Battle_Artillery_Fire_v1.wav
    ├── SFX_Battle_Rifle_Fire_v1.wav
    └── SFX_Ambient_Battlefield_Loop_v1.ogg
```

### 13.2 批量生成單位圖示

**需求：** 為 RTS 單位系統生成 20 個單位圖示

```cpp
// 定義單位類型
TArray<FString> UnitTypes = {
    TEXT("步兵-北伐軍"),
    TEXT("步兵-軍閥部隊"),
    TEXT("騎兵-偵察"),
    TEXT("炮兵-山炮連"),
    TEXT("機槍手-重機槍排"),
    // ... 更多單位
};

// 批量生成
for (const FString& UnitType : UnitTypes)
{
    FString Prompt = FString::Printf(
        TEXT("%s單位圖示，俯視角度，透明背景，風格化設計，清晰可辨識，適合RTS遊戲界面"),
        *UnitType
    );
    
    FAIContentRequest Request;
    Request.ContentType = EAIContentType::Image;
    Request.Prompt = Prompt;
    Request.Resolution = TEXT("256x256");
    Request.Style = EArtStyle::Stylized;
    Request.Quality = EAIQuality::Standard;
    
    FString AssetPath = AIContentManager->GenerateContent(Request);
}
```

### 13.3 生成動態戰場背景影片

**需求：** 主選單背景動態影片

```cpp
// 配置場景
FAISceneConfiguration MenuScene;
MenuScene.LocationDescription = TEXT("1920年代上海外灘夜景，黃浦江岸邊，歐式建築燈火輝煌，霓虹燈閃爍，車水馬龍，民國風情");
MenuScene.TimeOfDay = ETimeOfDay::Night;
MenuScene.Weather = EWeatherType::Clear;
MenuScene.Mood = ESceneMood::Mysterious;
MenuScene.CameraMovement = ECameraMovement::SlowPan;

// 生成電影級場景
AIContentManager->GenerateCinematicScene(
    TEXT("MingGoRTS_MainMenu_Background"),
    MenuScene,
    60.0f,  // 60秒循環
    ECinematicQuality::Cinematic,
    FVector2D(1920, 1080)
);
```

### 13.4 民國時期音效庫生成

**需求：** 建立完整的民國時期環境音效庫

```cpp
// 定義音效類別和提示詞
TMap<EAIContentType, TArray<FString>> SoundLibrary = {
    {EAIContentType::SoundEffect, {
        TEXT("老上海街道環境音，1920年代，黃包車聲、商販叫賣、行人腳步聲、汽車喇叭聲，循環，2分鐘"),
        TEXT("傳統茶館環境音，茶客聊天聲、茶杯碰撞聲、說書人聲、窗外街道聲，循環，2分鐘"),
        TEXT("火車站環境音，蒸汽火車汽笛聲、火車輪軌聲、人群喧鬧聲、廣播聲，循環，2分鐘"),
        TEXT("戰場環境音，炮火聲、槍聲、喊殺聲、號角聲、馬蹄聲，循環，2分鐘")
    }}
};

// 批量生成音效庫
for (const auto& Pair : SoundLibrary)
{
    for (const FString& Prompt : Pair.Value)
    {
        FAIContentRequest Request;
        Request.ContentType = EAIContentType::SoundEffect;
        Request.Prompt = Prompt;
        Request.AudioFormat = EAudioFormat::OGG;
        Request.Quality = EAIQuality::High;
        
        AIContentManager->QueueContentGeneration(Request);
    }
}

// 等待全部完成並導出
AIContentManager->ProcessGenerationQueue();
AIContentManager->ExportGeneratedContent(TEXT("/Game/Audio/RepublicEra_Ambient"));
```

---

## 十四、遊戲所需資產生成系統 | Game Asset Generation System

### 14.1 資產生成器概述

MingGoRTS 遊戲資產生成器是一個強大的系統，能夠生成遊戲所需的所有類型資產，包括遊戲畫面、影片、音樂、音效、圖檔，並包含專業的高校引導功能。

**系統特色：**
- **14種資產類別** - 覆蓋所有遊戲開發需求
- **8種資產用途** - 適應不同開發場景
- **9種高校內容類型** - 專業教育引導功能
- **5種質量級別** - 從草稿到電影級質量
- **10種藝術風格** - 多樣化視覺風格

### 14.2 資產類別詳解

#### 遊戲資產類型

| 類別 | 描述 | 典型用途 | 質量要求 |
|------|------|---------|---------|
| GameScene | 遊戲場景 | 關卡、背景、載入畫面 | High/Ultra |
| Character | 角色資產 | NPC、玩家角色、單位 | High/Ultra |
| Building | 建築資產 | 房屋、城牆、宮殿 | High/Ultra |
| Environment | 環境資產 | 地形、植被、天氣 | Standard/High |
| UI | 用戶界面 | 按鈕、面板、圖標 | Standard/High |
| Video | 影片資產 | 過場動畫、宣傳片 | Ultra/Cinematic |
| Music | 音樂資產 | 背景音樂、主題曲 | High/Ultra |
| SoundEffect | 音效資產 | 戰鬥音效、環境音 | Standard/High |
| Texture | 紋理資產 | 材質貼圖、表面紋理 | High/Ultra |
| Model | 3D模型 | 道具、武器、載具 | High/Ultra |
| Animation | 動畫資產 | 角色動作、特效動畫 | High/Ultra |
| Tutorial | 教學資產 | 教程、指南、幫助 | Standard/High |
| UniversityGuide | 高校引導 | 校園導覽、學術介紹 | Standard/High |

#### 資產用途分類

| 用途 | 描述 | 適用資產類型 |
|------|------|-------------|
| Gameplay | 遊戲玩法 | 所有類型 |
| Story | 劇情敘事 | 場景、角色、影片 |
| Tutorial | 教學指南 | 教學、UI、影片 |
| Marketing | 行銷推廣 | 影片、圖像、音樂 |
| Education | 教育培訓 | 高校引導、教學 |
| University | 高校專用 | 高校引導、學術內容 |
| Presentation | 演示展示 | 影片、UI、場景 |
| Documentation | 文檔記錄 | 圖像、文檔 |

### 14.3 快速開始指南

#### 初始化資產生成器

```cpp
// 在遊戲開始時初始化
UMingRTSGameAssetGenerator* AssetGenerator = NewObject<UMingRTSGameAssetGenerator>();
AssetGenerator->InitializeGameAssetGenerator(GetWorld());

// 綁定事件
AssetGenerator->OnGameAssetGenerated.AddDynamic(this, &AMyGameMode::OnAssetGenerated);
AssetGenerator->OnGameAssetGenerationCompleted.AddDynamic(this, &AMyGameMode::OnBatchCompleted);
```

#### 生成單個資產

```cpp
// 生成遊戲場景
FString SceneAssetID = AssetGenerator->GenerateGameScene(
    TEXT("Ming Dynasty Battle"),
    TEXT("Epic battle scene in Ming dynasty era with soldiers, flags, and dramatic lighting"),
    EQualityLevel::Ultra
);

// 生成角色資產
FString CharacterAssetID = AssetGenerator->GenerateCharacterAsset(
    TEXT("Ming General"),
    TEXT("Brave Ming dynasty general with traditional armor and commanding presence"),
    EQualityLevel::High
);

// 生成音樂資產
FString MusicAssetID = AssetGenerator->GenerateMusicAsset(
    TEXT("Battle Theme"),
    TEXT("Epic orchestral battle music with traditional Chinese instruments"),
    180.0f
);
```

#### 批量生成資產

```cpp
// 創建批量資產需求
TArray<FGameAssetRequirement> Requirements;

// 添加場景需求
FGameAssetRequirement SceneReq;
SceneReq.AssetCategory = EGameAssetCategory::GameScene;
SceneReq.AssetName = TEXT("Battle Scene 1");
SceneReq.Description = TEXT("First battle scene with Ming dynasty warriors");
SceneReq.Quality = EQualityLevel::High;
SceneReq.Style = EArtStyle::Photorealistic;
Requirements.Add(SceneReq);

// 添加角色需求
FGameAssetRequirement CharacterReq;
CharacterReq.AssetCategory = EGameAssetCategory::Character;
CharacterReq.AssetName = TEXT("General Li");
CharacterReq.Description = TEXT("Ming dynasty general with traditional armor");
CharacterReq.Quality = EQualityLevel::High;
CharacterReq.Style = EArtStyle::Photorealistic;
Requirements.Add(CharacterReq);

// 執行批量生成
FString BatchID = AssetGenerator->GenerateGameAssetsBatch(Requirements);
```

### 14.4 高校引導功能

#### 校園導覽生成

```cpp
// 設置高校引導配置
FUniversityGuideConfig GuideConfig;
GuideConfig.ContentType = EUniversityContentType::CampusTour;
GuideConfig.UniversityName = TEXT("Ming Dynasty University");
GuideConfig.Department = TEXT("History and Culture");
GuideConfig.TargetAudience = TEXT("Prospective Students");
GuideConfig.Duration = TEXT("10 minutes");
GuideConfig.Language = TEXT("Chinese");
GuideConfig.CulturalContext = TEXT("Ming Dynasty");
GuideConfig.bInteractive = true;
GuideConfig.bIncludeHistoricalContent = true;
GuideConfig.bIncludeStudentTestimonials = false;

// 生成高校引導
FString GuideID = AssetGenerator->GenerateUniversityGuide(GuideConfig);
```

#### 學術介紹生成

```cpp
// 學術介紹配置
FUniversityGuideConfig AcademicConfig;
AcademicConfig.ContentType = EUniversityContentType::AcademicIntro;
AcademicConfig.UniversityName = TEXT("Ming Dynasty University");
AcademicConfig.Department = TEXT("Classical Chinese Literature");
AcademicConfig.TargetAudience = TEXT("International Students");
AcademicConfig.KeyTopics = {
    TEXT("Classical Poetry"),
    TEXT("Historical Documents"),
    TEXT("Calligraphy"),
    TEXT("Traditional Philosophy")
};

// 生成學術介紹
FString AcademicID = AssetGenerator->GenerateUniversityGuide(AcademicConfig);
```

### 14.5 資產管理與導出

#### 獲取生成的資產

```cpp
// 獲取單個資產
FGeneratedGameAsset Asset = AssetGenerator->GetGeneratedAsset(AssetID);

// 獲取所有資產
TArray<FGeneratedGameAsset> AllAssets = AssetGenerator->GetAllGeneratedAssets();

// 按類別獲取資產
TArray<FGeneratedGameAsset> SceneAssets = AssetGenerator->GetAssetsByCategory(EGameAssetCategory::GameScene);

// 按用途獲取資產
TArray<FGeneratedGameAsset> GameplayAssets = AssetGenerator->GetAssetsByPurpose(EGameAssetPurpose::Gameplay);
```

#### 導出資產

```cpp
// 導出單個資產
bool bSuccess = AssetGenerator->ExportAsset(AssetID, TEXT("/Game/GeneratedAssets/Exports/"));

// 批量導出資產
TArray<FString> AssetIDs = {AssetID1, AssetID2, AssetID3};
bool bBatchSuccess = AssetGenerator->ExportAssetsBatch(AssetIDs, TEXT("/Game/GeneratedAssets/BatchExport/"));
```

### 14.6 事件處理與監控

#### 資產生成事件

```cpp
UFUNCTION()
void AMyGameMode::OnAssetGenerated(const FString& AssetID, const FGeneratedGameAsset& Asset)
{
    UE_LOG(LogTemp, Log, TEXT("Asset Generated: %s - %s"), *AssetID, *Asset.AssetName);
    
    // 處理生成的資產
    ProcessGeneratedAsset(Asset);
}

UFUNCTION()
void AMyGameMode::OnBatchCompleted(const FString& RequestID, const TArray<FGeneratedGameAsset>& GeneratedAssets)
{
    UE_LOG(LogTemp, Log, TEXT("Batch Completed: %s - %d assets"), *RequestID, GeneratedAssets.Num());
    
    // 處理批量生成的資產
    for (const FGeneratedGameAsset& Asset : GeneratedAssets)
    {
        ProcessGeneratedAsset(Asset);
    }
}

UFUNCTION()
void AMyGameMode::OnUniversityGuideGenerated(const FString& GuideID, const FGeneratedGameAsset& GuideAsset)
{
    UE_LOG(LogTemp, Log, TEXT("University Guide Generated: %s"), *GuideID);
    
    // 處理高校引導資產
    ProcessUniversityGuide(GuideAsset);
}
```

#### 進度監控

```cpp
UFUNCTION()
void AMyGameMode::OnAssetGenerationProgress(const FString& AssetID, int32 Progress)
{
    // 更新UI進度條
    UpdateProgressBar(Progress);
    
    // 記錄生成日誌
    UE_LOG(LogTemp, Log, TEXT("Asset Generation Progress: %s - %d%%"), *AssetID, Progress);
}
```

### 14.7 資產統計與分析

#### 獲取統計信息

```cpp
// 獲取資產統計
TMap<EGameAssetCategory, int32> AssetStatistics = AssetGenerator->GetAssetStatistics();

// 獲取生成統計
TMap<FString, float> GenerationStatistics = AssetGenerator->GetGenerationStatistics();

// 顯示統計信息
for (const auto& Pair : AssetStatistics)
{
    FString CategoryName = AssetGenerator->GetAssetCategoryName(Pair.Key);
    UE_LOG(LogTemp, Log, TEXT("%s: %d assets"), *CategoryName, Pair.Value);
}

for (const auto& Pair : GenerationStatistics)
{
    UE_LOG(LogTemp, Log, TEXT("%s: %.2f"), *Pair.Key, Pair.Value);
}
```

### 14.8 最佳實踐

#### 資產規劃

1. **需求分析** - 詳細分析資產需求和用途
2. **質量選擇** - 根據用途選擇合適的質量級別
3. **批量生成** - 合理使用批量生成提高效率
4. **版本管理** - 建立完善的版本管理系統

#### 性能優化

1. **並行處理** - 利用系統的並行生成能力
2. **智能緩存** - 避免重複生成相同資產
3. **資源管理** - 監控系統資源使用情況
4. **異步處理** - 使用異步處理避免阻塞

#### 質量控制

1. **預覽檢查** - 生成前預覽和檢查配置
2. **多輪優化** - 使用多輪生成優化質量
3. **用戶反饋** - 收集用戶反饋持續改進
4. **標準化** - 建立統一的質量標準

### 14.9 故障排除

#### 常見問題

**Q: 資產生成失敗**
- 檢查網絡連接
- 驗證資產需求配置
- 檢查系統資源使用
- 降低質量級別重試

**Q: 生成質量不佳**
- 提高質量級別
- 改進描述文本
- 調整藝術風格
- 增加詳細參數

**Q: 高校引導內容不完整**
- 檢查配置參數
- 確認內容類型設置
- 驗證目標受眾定義
- 添加更多主題內容

**Q: 批量生成效率低**
- 減少並行任務數量
- 優化資產需求順序
- 使用智能緩存
- 分批處理大型項目

---

**最後更新:** 2026-03-22  
**適用版本:** MingGoRTS v0.8+  
**UE版本:** Unreal Engine 5.4+
