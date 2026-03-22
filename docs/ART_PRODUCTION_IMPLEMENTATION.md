# MingGoRTS 美术资源制作实施文档

## 文档信息
- **版本**: 1.0
- **日期**: 2026-03-23
- **作者**: MingGoRTS Art Team
- **状态**: 实施中

---

## 1. 美术资源制作总览

### 1.1 资源清单

| 资源类型 | 数量 | 优先级 | 预计工期 |
|---------|------|--------|----------|
| **3D单位模型** | 48个专属单位 | 高 | 8周 |
| **基础单位模型** | 24个通用单位 | 高 | 4周 |
| **建筑模型** | 36个势力建筑 | 高 | 6周 |
| **角色贴图** | 120套 | 高 | 6周 |
| **建筑贴图** | 72套 | 中 | 4周 |
| **环境贴图** | 48套 | 中 | 3周 |
| **单位动画** | 360个 | 高 | 8周 |
| **特效资源** | 60个 | 中 | 4周 |
| **UI资源** | 200个 | 中 | 3周 |

**总计**: 约900个资源，预计16周完成

### 1.2 制作流程

```
概念设计 → 高模制作 → 低模拓扑 → UV展开 → 贴图制作 → 骨骼绑定 → 动画制作 → 引擎导入 → 测试验证
```

---

## 2. 3D模型制作规范

### 2.1 单位模型标准

#### LOD系统

| LOD级别 | 面数限制 | 使用距离 | 贴图分辨率 |
|--------|---------|---------|-----------|
| **LOD0** | 8000-12000 | 0-50米 | 2048x2048 |
| **LOD1** | 4000-6000 | 50-150米 | 1024x1024 |
| **LOD2** | 1500-2500 | 150-400米 | 512x512 |
| **LOD3** | 500-800 | 400米+ | 256x256 |

#### 模型命名规范

```
[势力代码]_[单位类型]_[单位名称]_[版本]

示例:
- BY_Infantry_BeiyangGuard_v01.fbx
- GMD_Cavalry_ZhongzhengCavalry_v01.fbx
- CCP_Special_RedArmyGuerrilla_v01.fbx
```

#### 势力代码表

| 势力 | 代码 | 主色调 | 次要色 |
|------|------|--------|--------|
| 北洋政府 | BY | 深蓝灰 #4A5568 | 金色 #D4AF37 |
| 国民政府 | GMD | 中山蓝 #1E3A5F | 白色 #FFFFFF |
| 中共 | CCP | 红军红 #CC0000 | 军绿 #4B5320 |
| 奉系 | FX | 黑灰 #2D3748 | 虎纹黄 #D69E2E |
| 直系 | ZX | 深紫 #4A0E4E | 银白 #C0C0C0 |
| 皖系 | WX | 墨绿 #1A202C | 深红 #8B0000 |
| 晋系 | JX | 藏青 #1E2B3D | 暗金 #B8860B |
| 桂系 | GX | 军绿 #3D4A3D | 浅灰 #A0AEC0 |
| 滇系 | DX | 深绿 #2F4F4F | 银灰 #A0A0A0 |
| 川系 | CX | 土黄 #8B7355 | 深褐 #654321 |
| 马家军 | MJ | 墨绿 #013220 | 白色 #FFFFFF |
| 新疆 | XJ | 沙漠黄 #C2B280 | 棕色 #8B4513 |

### 2.2 建筑模型标准

#### 面数限制

| 建筑类型 | LOD0面数 | LOD1面数 | LOD2面数 |
|---------|---------|---------|---------|
| **小型建筑** | 3000-5000 | 1500-2500 | 500-1000 |
| **中型建筑** | 6000-10000 | 3000-5000 | 1000-2000 |
| **大型建筑** | 10000-15000 | 5000-8000 | 2000-3000 |
| **地标建筑** | 15000-25000 | 8000-12000 | 3000-5000 |

#### 建筑分类

```cpp
// 建筑类型枚举
UENUM()
enum class EMingBuildingType : uint8
{
    CommandCenter,      // 指挥中心
    Barracks,          // 兵营
    Factory,           // 工厂
    ResourceCollector, // 资源采集
    DefenseStructure,  // 防御建筑
    ResearchFacility,  // 研究设施
    EconomicBuilding,  // 经济建筑
    SpecialBuilding    // 特殊建筑
};
```

---

## 3. 贴图制作规范

### 3.1 PBR工作流程

#### 贴图类型

| 贴图类型 | 通道 | 格式 | 说明 |
|---------|------|------|------|
| **Base Color** | RGB | PNG/TGA | 基础颜色，无光照信息 |
| **Normal** | RGB | PNG | 法线信息 |
| **Metallic** | R | PNG | 金属度 (0=非金属, 1=金属) |
| **Roughness** | R | PNG | 粗糙度 (0=光滑, 1=粗糙) |
| **Ambient Occlusion** | R | PNG | 环境光遮蔽 |
| **Emissive** | RGB | PNG | 自发光 (可选) |

#### 贴图命名

```
[模型名]_[贴图类型]

示例:
- BY_Infantry_BeiyangGuard_BaseColor.png
- BY_Infantry_BeiyangGuard_Normal.png
- BY_Infantry_BeiyangGuard_Metallic.png
- BY_Infantry_BeiyangGuard_Roughness.png
- BY_Infantry_BeiyangGuard_AO.png
```

### 3.2 各势力视觉风格

#### 北洋政府 (Beiyang Government)

```cpp
// 北洋势力视觉配置
USTRUCT()
struct FBeiyangVisualConfig
{
    // 颜色方案
    FLinearColor PrimaryColor = FLinearColor(0.29f, 0.33f, 0.41f);    // 深蓝灰
    FLinearColor SecondaryColor = FLinearColor(0.83f, 0.69f, 0.22f);   // 金色
    FLinearColor AccentColor = FLinearColor(0.18f, 0.20f, 0.25f);      // 深色
    
    // 材质参数
    float UniformCleanliness = 0.7f;    // 制服整洁度
    float EquipmentModernity = 0.6f;    // 装备现代化程度
    float ImperialInfluence = 0.5f;      // 帝国遗风
};
```

**视觉特征**:
- 德国式军服剪裁
- 深蓝色/灰色主调
- 金色装饰点缀
- 整齐的队列风格
- 近代军事装备

#### 国民政府 (Nationalist Government)

```cpp
// 国民政府视觉配置
USTRUCT()
struct FGMDVisualConfig
{
    FLinearColor PrimaryColor = FLinearColor(0.12f, 0.23f, 0.37f);    // 中山蓝
    FLinearColor SecondaryColor = FLinearColor(1.0f, 1.0f, 1.0f);      // 白色
    FLinearColor AccentColor = FLinearColor(0.8f, 0.2f, 0.2f);          // 红色
    
    float UniformStandardization = 0.9f;  // 制服标准化
    float ModernEquipment = 0.8f;         // 现代装备
    float DisciplineEmphasis = 0.9f;      // 纪律强调
};
```

**视觉特征**:
- 中山装改良军服
- 蓝白配色
- 美式装备影响
- 整齐划一的风格
- 现代化军事形象

#### 中国共产党 (CCP)

```cpp
// 中共视觉配置
USTRUCT()
struct FCCPVisualConfig
{
    FLinearColor PrimaryColor = FLinearColor(0.8f, 0.0f, 0.0f);       // 红军红
    FLinearColor SecondaryColor = FLinearColor(0.29f, 0.33f, 0.13f);   // 军绿
    FLinearColor AccentColor = FLinearColor(0.95f, 0.95f, 0.95f);      // 白色
    
    float EquipmentScarcity = 0.6f;       // 装备匮乏感
    float GrassrootsFeel = 0.9f;          // 草根感
    float IdeologicalMarks = 0.8f;        // 意识形态标识
};
```

**视觉特征**:
- 灰色/蓝色土布军服
- 红色五角星标识
- 绑腿草鞋装备
- 土制武器感
- 革命精神气质

### 3.3 其他势力配置 (略，遵循各自设计文档)

---

## 4. 动画制作规范

### 4.1 动画分类

| 动画类型 | 数量 | 帧率 | 说明 |
|---------|------|------|------|
| **空闲 (Idle)** | 3个/单位 | 30fps | 待机循环动画 |
| **移动 (Move)** | 2个/单位 | 30fps | 行走/奔跑 |
| **攻击 (Attack)** | 3个/单位 | 30fps | 近战/远程 |
| **受伤 (Hit)** | 2个/单位 | 30fps | 受击反应 |
| **死亡 (Death)** | 2个/单位 | 30fps | 死亡动作 |
| **特殊 (Special)** | 2个/单位 | 30fps | 技能动画 |
| **建造 (Build)** | 1个/单位 | 30fps | 建造动作 |
| **采集 (Gather)** | 1个/单位 | 30fps | 资源采集 |

### 4.2 动画命名规范

```
[模型名]_[动画类型]_[变体编号]

示例:
- BY_Infantry_BeiyangGuard_Idle_01
- BY_Infantry_BeiyangGuard_Move_Walk
- BY_Infantry_BeiyangGuard_Attack_Melee
- BY_Infantry_BeiyangGuard_Death_01
```

### 4.3 动画状态机

```cpp
// 单位动画状态机配置
UCLASS()
class UMingUnitAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    EUnitAnimationState CurrentState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsAttacking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsDead;

    // 动画过渡时间 (秒)
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float IdleToMoveBlendTime = 0.2f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MoveToAttackBlendTime = 0.15f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float AttackToIdleBlendTime = 0.3f;
};
```

### 4.4 动画 blend tree

```
[Idle]
  ├── Idle_Relaxed (0.0-0.1 速度)
  ├── Idle_Alert (战斗中)
  └── Idle_Combat (警戒状态)

[Move]
  ├── Walk (0.1-0.5 速度)
  ├── Run (0.5-1.0 速度)
  └── Sprint (1.0+ 速度，特殊单位)

[Attack]
  ├── Attack_Melee (近战攻击)
  ├── Attack_Ranged (远程攻击)
  └── Attack_Special (特殊攻击)
```

---

## 5. 制作工具与流程

### 5.1 推荐工具链

| 环节 | 主要工具 | 辅助工具 | 输出格式 |
|------|---------|---------|---------|
| **概念设计** | Photoshop, Procreate | PureRef | PSD, PNG |
| **高模制作** | ZBrush, Blender | - | ZTL, BLEND |
| **低模拓扑** | Maya, Blender | TopoGun | MA, FBX |
| **UV展开** | RizomUV, Blender | - | - |
| **贴图制作** | Substance Painter, Photoshop | Quixel Mixer | PNG, TGA |
| **骨骼绑定** | Maya, Blender | - | - |
| **动画制作** | Maya, Blender | Motion Builder | FBX |
| **特效制作** | Houdini, Niagara | - | - |

### 5.2 版本控制

```
// 文件夹结构
Content/
├── Art/
│   ├── Characters/
│   │   ├── BY/              // 北洋
│   │   │   ├── Source/      // 源文件
│   │   │   ├── FBX/         // 导出模型
│   │   │   └── Textures/    // 贴图
│   │   ├── GMD/             // 国民政府
│   │   ├── CCP/             // 中共
│   │   └── ...
│   ├── Buildings/
│   ├── Environment/
│   └── UI/
└── Animations/
    ├── BY/
    ├── GMD/
    └── ...
```

### 5.3 审核流程

1. **白模审核** - 检查比例和剪影
2. **高模审核** - 检查细节和造型
3. **低模审核** - 检查拓扑和面数
4. **贴图审核** - 检查风格和质量
5. **绑定审核** - 检查骨骼设置
6. **动画审核** - 检查动作质量
7. **引擎测试** - 最终游戏内验证

---

## 6. 技术规格

### 6.1 导出设置

#### FBX导出设置 (Maya)

```
Geometry:
  ✓ Smoothing Groups
  ✓ Triangulate (游戏导出)
  ✓ Merge Vertex (可选)
  
Animation:
  ✓ Animation
  ✓ Bake Animation (起始帧: 1, 结束帧: 根据动画)
  ✓ Resample All
  
Units:
  - File Units: Centimeters
  - Axis Conversion: Y-up
```

#### 贴图导出设置

```
Base Color:
  - Format: PNG
  - Bit Depth: 8-bit
  - Color Profile: sRGB
  - Resolution: 2048x2048 (LOD0)

Normal Map:
  - Format: PNG
  - Bit Depth: 8-bit
  - Color Profile: Linear
  - Resolution: 2048x2048 (LOD0)

ORM (Metallic/Roughness/AO打包):
  - Red Channel: Metallic
  - Green Channel: Roughness  
  - Blue Channel: AO
  - Format: PNG
  - Resolution: 1024x1024 (LOD0)
```

### 6.2 性能预算

| 类别 | 内存预算 | 显存预算 | 说明 |
|------|---------|---------|------|
| **单个单位** | 5-10 MB | 15-30 MB | 包含所有LOD |
| **单个建筑** | 10-20 MB | 30-50 MB | 中型建筑 |
| **场景单位** | 500 MB | 1.5 GB | 200单位同屏 |
| **总美术资源** | 2 GB | 4 GB | 所有资源 |

---

## 7. 制作计划表

### 7.1 第一阶段 (Week 1-4): 基础资源

| 周次 | 任务 | 产出 | 负责人 |
|------|------|------|--------|
| Week 1 | 北洋基础步兵 | 4个模型+动画 | 角色组 |
| Week 1 | 国民政府基础步兵 | 4个模型+动画 | 角色组 |
| Week 2 | 基础建筑模板 | 6个建筑模型 | 场景组 |
| Week 2 | 贴图规范确立 | 材质库 | 材质组 |
| Week 3 | 中共基础步兵 | 4个模型+动画 | 角色组 |
| Week 3 | 基础资源导入引擎 |  playable demo | 技术美术 |
| Week 4 | 其他势力基础单位 | 12个模型 | 角色组 |
| Week 4 | 第一批测试验证 | 测试报告 | QA |

### 7.2 第二阶段 (Week 5-8): 专属单位

| 周次 | 任务 | 产出 | 负责人 |
|------|------|------|--------|
| Week 5 | 北洋专属单位 (4个) | 4个模型+全套动画 | 角色组 |
| Week 6 | 国民政府专属单位 (4个) | 4个模型+全套动画 | 角色组 |
| Week 7 | 中共专属单位 (4个) | 4个模型+全套动画 | 角色组 |
| Week 8 | 其他势力专属单位 (12个) | 12个模型+动画 | 角色组 |

### 7.3 第三阶段 (Week 9-12): 建筑与环境

| 周次 | 任务 | 产出 | 负责人 |
|------|------|------|--------|
| Week 9-10 | 势力特色建筑 | 36个建筑模型 | 场景组 |
| Week 11 | 环境资源 | 地图素材 | 场景组 |
| Week 12 | 特效资源 | 60个特效 | 特效组 |

### 7.4 第四阶段 (Week 13-16): 优化与完善

| 周次 | 任务 | 产出 | 负责人 |
|------|------|------|--------|
| Week 13-14 | LOD优化 | 优化后的资源 | 技术美术 |
| Week 15 | UI资源 | 200个UI元素 | UI组 |
| Week 16 | 最终整合测试 | 完整资源包 | QA |

---

## 8. 质量检查清单

### 8.1 模型检查

- [ ] 面数符合LOD标准
- [ ] 无多余面、重叠面
- [ ] 法线方向正确
- [ ] UV无重叠、拉伸最小
- [ ] 模型居中、比例正确
- [ ] 命名符合规范
- [ ] 骨骼绑定权重正确
- [ ] 导入引擎无错误

### 8.2 贴图检查

- [ ] 分辨率符合标准
- [ ] 无压缩伪影
- [ ] 颜色配置文件正确
- [ ] 贴图命名正确
- [ ] 材质球设置正确
- [ ] PBR参数合理
- [ ] 文件大小优化

### 8.3 动画检查

- [ ] 帧率统一30fps
- [ ] 无滑步、抖动
- [ ] 起始/结束姿势合理
- [ ] BlendTree配置正确
- [ ] 动画事件标记正确
- [ ] 循环动画无缝衔接

---

## 9. 引擎导入配置

### 9.1 导入设置脚本

```python
# Python脚本 - UE5导入配置
import unreal

def configure_import_settings():
    """配置FBX导入设置"""
    options = unreal.FbxImportUI()
    
    # 静态网格体设置
    options.set_editor_property('import_materials', False)
    options.set_editor_property('import_textures', False)
    options.set_editor_property('import_mesh', True)
    options.set_editor_property('static_mesh_import_data', 
        unreal.FbxStaticMeshImportData())
    
    # 骨骼网格体设置
    options.set_editor_property('skeleton_import_data',
        unreal.FbxSkeletalMeshImportData())
    options.skeleton_import_data.set_editor_property('import_content_type',
        unreal.FBXImportContentType.FBXICT_ALL)
    
    # 动画设置
    options.set_editor_property('anim_sequence_import_data',
        unreal.FbxAnimSequenceImportData())
    options.anim_sequence_import_data.set_editor_property('animation_length',
        unreal.FBXAnimationLengthImportType.FBALIT_EXPORTED_TIME)
    
    return options

def batch_import_fbx(folder_path):
    """批量导入FBX文件"""
    import_settings = configure_import_settings()
    
    fbx_files = unreal.EditorUtilityLibrary.get_assets_in_path(folder_path)
    
    for fbx_file in fbx_files:
        if fbx_file.endswith('.fbx'):
            unreal.FbxImporter.import_fbx(
                import_settings,
                fbx_file,
                destination_path='/Game/Content/Art/'
            )
```

### 9.2 材质模板

```cpp
// 创建势力专用材质实例
UFUNCTION(BlueprintCallable)
UMaterialInstanceDynamic* CreateFactionMaterial(
    EFactionType Faction,
    UTexture* BaseColor,
    UTexture* Normal,
    UTexture* ORM)
{
    // 获取主材质
    UMaterial* MasterMaterial = LoadObject<UMaterial>(
        nullptr, 
        TEXT("/Game/Materials/M_UnitMaster")
    );
    
    // 创建动态实例
    UMaterialInstanceDynamic* MID = 
        UMaterialInstanceDynamic::Create(MasterMaterial, nullptr);
    
    // 设置贴图
    MID->SetTextureParameterValue(TEXT("BaseColor"), BaseColor);
    MID->SetTextureParameterValue(TEXT("Normal"), Normal);
    MID->SetTextureParameterValue(TEXT("ORM"), ORM);
    
    // 根据势力设置颜色
    FLinearColor FactionColor = GetFactionColor(Faction);
    MID->SetVectorParameterValue(TEXT("FactionColor"), FactionColor);
    
    return MID;
}
```

---

## 10. 附录

### 10.1 参考资料

- 势力详细设计: `docs/12_FACTIONS_DETAILED_DESIGN.md`
- 3D模型设计: `docs/UNIT_3D_MODEL_DESIGN.md`
- 美术资源规划: `docs/FACTION_ART_RESOURCE_PLAN.md`
- 平衡性分析: `docs/FACTION_BALANCE_ANALYSIS.md`

### 10.2 联系方式

- 美术总监: art-director@minggorts.com
- 技术美术: tech-art@minggorts.com
- 动画组长: animation-lead@minggorts.com

---

*文档结束*
