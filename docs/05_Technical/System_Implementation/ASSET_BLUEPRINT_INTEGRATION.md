# MingGoRTS 素材与蓝图代码关联系统

## 文档信息
- **版本**: 1.0
- **日期**: 2026-03-23
- **作者**: MingGoRTS Technical Team
- **状态**: 实施中

---

## 1. 系统架构总览

### 1.1 资产引用架构图

```
┌─────────────────────────────────────────────────────────────────┐
│                     MingGoRTS Asset System                       │
├─────────────────────────────────────────────────────────────────┤
│  ┌──────────────────┐      ┌──────────────────┐                  │
│  │ Asset Manager    │◄────►│ Blueprint Bridge   │                │
│  │ - Asset Registry │      │ - BP Integration   │                │
│  │ - Dynamic Load   │      │ - Event Binding    │                │
│  │ - Asset Pool     │      │ - Property Link    │                │
│  └────────┬─────────┘      └────────┬─────────┘                  │
│           │                          │                         │
│           ▼                          ▼                         │
│  ┌──────────────────┐      ┌──────────────────┐                  │
│  │ C++ Backend      │      │ Blueprint Frontend│                 │
│  │ - Asset Classes  │      │ - UI Widgets      │                 │
│  │ - Loaders        │      │ - Anim Instances  │                 │
│  │ - Processors     │      │ - Effect Systems  │                 │
│  └────────┬─────────┘      └────────┬─────────┘                  │
│           │                          │                         │
│           └──────────┬─────────────┘                           │
│                      ▼                                         │
│  ┌──────────────────────────────────────────────────┐         │
│  │               Content Browser                     │         │
│  │  /Game/Art/Characters  /Game/Art/Buildings      │         │
│  │  /Game/Blueprints/Units  /Game/Blueprints/UI      │         │
│  └──────────────────────────────────────────────────┘         │
└─────────────────────────────────────────────────────────────────┘
```

### 1.2 核心组件

| 组件 | 类型 | 功能 | 蓝图可访问性 |
|------|------|------|-------------|
| **MingAssetManager** | C++ | 资产加载、缓存、管理 | 完全暴露 |
| **MingBlueprintBridge** | C++ | C++与蓝图通信桥梁 | 完全暴露 |
| **MingFactionAssetLoader** | C++ | 势力专属资产加载 | 部分暴露 |
| **MingMaterialInstanceFactory** | C++ | 动态材质创建 | 完全暴露 |
| **MingAnimInstance** | C++ | 动画实例基类 | 可继承扩展 |
| **MingAssetReference** | C++ | 资产引用结构 | 蓝图可用 |

---

## 2. 资产引用系统

### 2.1 资产引用数据结构

```cpp
// 资产引用核心结构
USTRUCT(BlueprintType)
struct MINGGORTS_API FMingAssetReference
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    FName AssetID;                    // 唯一资产标识

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    TSoftObjectPtr<UObject> AssetPtr; // 软引用，延迟加载

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    EFactionType Faction;             // 所属势力

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    EAssetType AssetType;             // 资产类型

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    FGameplayTagContainer AssetTags;  // 资产标签

    // 同步加载资产
    UFUNCTION(BlueprintCallable, Category = "Asset")
    UObject* LoadAssetSync();

    // 异步加载资产
    UFUNCTION(BlueprintCallable, Category = "Asset")
    void LoadAssetAsync(FOnAssetLoaded Delegate);

    // 获取资产路径
    UFUNCTION(BlueprintPure, Category = "Asset")
    FString GetAssetPath() const;

    bool IsValid() const { return !AssetID.IsNone() && AssetPtr.IsValid(); }
};

// 资产类型枚举
UENUM(BlueprintType)
enum class EAssetType : uint8
{
    Character,      // 角色模型
    Building,       // 建筑模型
    Weapon,         // 武器模型
    Prop,           // 道具模型
    Texture,        // 贴图
    Material,       // 材质
    Animation,      // 动画
    Sound,          // 音效
    Effect,         // 特效
    UI,             // UI资源
    Blueprint,      // 蓝图
    DataTable,      // 数据表
    Custom
};
```

### 2.2 资产管理器

```cpp
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    // 单例获取
    UFUNCTION(BlueprintPure, Category = "Asset Manager", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
    static UMingAssetManager* Get();

    // 注册资产
    UFUNCTION(BlueprintCallable, Category = "Asset Manager")
    void RegisterAsset(const FMingAssetReference& AssetRef);

    // 获取资产
    UFUNCTION(BlueprintCallable, Category = "Asset Manager", meta = (DeterminesOutputType = "AssetClass"))
    UObject* GetAsset(const FName& AssetID, TSubclassOf<UObject> AssetClass);

    // 批量加载势力资产
    UFUNCTION(BlueprintCallable, Category = "Asset Manager")
    void LoadFactionAssets(EFactionType Faction, EAssetType Type, FOnAssetsLoaded Delegate);

    // 获取势力主材质
    UFUNCTION(BlueprintCallable, Category = "Asset Manager")
    UMaterialInterface* GetFactionMasterMaterial(EFactionType Faction);

    // 创建动态材质实例
    UFUNCTION(BlueprintCallable, Category = "Asset Manager")
    UMaterialInstanceDynamic* CreateFactionMaterialInstance(EFactionType Faction, UMeshComponent* TargetMesh);

    // 资产缓存管理
    UFUNCTION(BlueprintCallable, Category = "Asset Manager")
    void ClearAssetCache();

    UFUNCTION(BlueprintCallable, Category = "Asset Manager")
    void PreloadAssetsForLevel(const FName& LevelName);

private:
    UPROPERTY()
    TMap<FName, FMingAssetReference> AssetRegistry;

    UPROPERTY()
    TMap<EFactionType, TObjectPtr<UMaterialInterface>> FactionMasterMaterials;

    UPROPERTY()
    TObjectPtr<UAssetCache> AssetCache;

    void InitializeFactionMaterials();
    void OnAssetLoadedAsync(FName AssetID, UObject* LoadedAsset);
};

// 实现示例
void UMingAssetManager::InitializeFactionMaterials()
{
    // 为每个势力加载主材质
    TArray<EFactionType> AllFactions;
    UEnum::GetAllEnumNames(TEXT("EFactionType"), AllFactions);

    for (EFactionType Faction : AllFactions)
    {
        FString MaterialPath = FString::Printf(
            TEXT("/Game/Art/Materials/Factions/M_%s_Master"),
            *UEnum::GetValueAsString(Faction)
        );

        UMaterialInterface* MasterMaterial = LoadObject<UMaterialInterface>(
            nullptr, *MaterialPath
        );

        if (MasterMaterial)
        {
            FactionMasterMaterials.Add(Faction, MasterMaterial);
        }
    }
}

UMaterialInstanceDynamic* UMingAssetManager::CreateFactionMaterialInstance(
    EFactionType Faction, UMeshComponent* TargetMesh)
{
    UMaterialInterface** MasterMat = FactionMasterMaterials.Find(Faction);
    if (!MasterMat || !*MasterMat)
    {
        UE_LOG(LogMingAsset, Warning, TEXT("Master material not found for faction: %s"),
            *UEnum::GetValueAsString(Faction));
        return nullptr;
    }

    // 创建动态材质实例
    UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(*MasterMat, TargetMesh);

    // 设置势力颜色参数
    FLinearColor FactionColor = GetFactionColor(Faction);
    MID->SetVectorParameterValue(TEXT("FactionColor"), FactionColor);

    // 应用到目标网格
    if (TargetMesh)
    {
        TargetMesh->SetMaterial(0, MID);
    }

    return MID;
}
```

---

## 3. 蓝图绑定系统

### 3.1 蓝图桥接器

```cpp
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingBlueprintBridge : public UObject
{
    GENERATED_BODY()

public:
    // 在蓝图中调用C++函数
    UFUNCTION(BlueprintCallable, Category = "Blueprint Bridge", meta = (CallableInEditor = "true"))
    static void ApplyFactionVisualsToActor(AActor* TargetActor, EFactionType Faction);

    // 绑定资产到蓝图组件
    UFUNCTION(BlueprintCallable, Category = "Blueprint Bridge")
    static void BindAssetToComponent(USceneComponent* Component, const FMingAssetReference& AssetRef);

    // 获取势力专属蓝图类
    UFUNCTION(BlueprintCallable, Category = "Blueprint Bridge", meta = (DeterminesOutputType = "BaseClass"))
    static TSubclassOf<AActor> GetFactionBlueprintClass(EFactionType Faction, EUnitType UnitType, TSubclassOf<AActor> BaseClass);

    // 在运行时创建蓝图实例
    UFUNCTION(BlueprintCallable, Category = "Blueprint Bridge", meta = (WorldContext = "WorldContextObject"))
    static AActor* SpawnBlueprintActor(UObject* WorldContextObject, TSubclassOf<AActor> BlueprintClass, const FTransform& SpawnTransform);

    // 获取资产引用信息（用于蓝图显示）
    UFUNCTION(BlueprintPure, Category = "Blueprint Bridge")
    static FText GetAssetDisplayName(const FMingAssetReference& AssetRef);

    // 验证资产有效性
    UFUNCTION(BlueprintPure, Category = "Blueprint Bridge")
    static bool ValidateAssetReference(const FMingAssetReference& AssetRef, FString& OutError);
};

// 实现
void UMingBlueprintBridge::ApplyFactionVisualsToActor(AActor* TargetActor, EFactionType Faction)
{
    if (!TargetActor)
    {
        UE_LOG(LogMingBlueprint, Warning, TEXT("ApplyFactionVisualsToActor: TargetActor is null"));
        return;
    }

    // 获取所有静态网格组件
    TArray<UStaticMeshComponent*> MeshComponents;
    TargetActor->GetComponents<UStaticMeshComponent>(MeshComponents);

    UMingAssetManager* AssetManager = UMingAssetManager::Get();
    if (!AssetManager)
    {
        return;
    }

    // 为每个网格应用势力材质
    for (UStaticMeshComponent* MeshComp : MeshComponents)
    {
        // 创建并应用动态材质
        UMaterialInstanceDynamic* MID = AssetManager->CreateFactionMaterialInstance(Faction, MeshComp);

        if (MID)
        {
            // 设置额外的材质参数
            ApplyFactionMaterialParameters(MID, Faction);
        }
    }

    // 应用势力特效
    ApplyFactionEffectsToActor(TargetActor, Faction);
}

void UMingBlueprintBridge::ApplyFactionMaterialParameters(UMaterialInstanceDynamic* MID, EFactionType Faction)
{
    // 根据势力设置不同的材质参数
    switch (Faction)
    {
        case EFactionType::BeiyangGovernment:
            MID->SetScalarParameterValue(TEXT("WearAmount"), 0.3f);    // 磨损程度
            MID->SetScalarParameterValue(TEXT("Cleanliness"), 0.7f); // 整洁度
            break;

        case EFactionType::ChineseCommunistParty:
            MID->SetScalarParameterValue(TEXT("WearAmount"), 0.6f);    // 更多磨损
            MID->SetScalarParameterValue(TEXT("ClothTexture"), 1.0f); // 土布材质
            break;

        case EFactionType::NationalistGovernment:
            MID->SetScalarParameterValue(TEXT("WearAmount"), 0.2f);    // 较少磨损
            MID->SetScalarParameterValue(TEXT("Standardization"), 1.0f); // 标准化
            break;

        default:
            break;
    }
}
```

### 3.2 势力资产加载器

```cpp
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingFactionAssetLoader : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Faction Assets")
    void InitializeForFaction(EFactionType Faction);

    // 加载单位资产
    UFUNCTION(BlueprintCallable, Category = "Faction Assets", meta = (DeterminesOutputType = "AssetClass"))
    UObject* LoadUnitAsset(const FName& UnitID, TSubclassOf<UObject> AssetClass);

    // 加载建筑资产
    UFUNCTION(BlueprintCallable, Category = "Faction Assets")
    UStaticMesh* LoadBuildingMesh(const FName& BuildingID);

    // 获取单位动画蓝图
    UFUNCTION(BlueprintCallable, Category = "Faction Assets")
    TSubclassOf<UAnimInstance> GetUnitAnimBlueprint(const FName& UnitID);

    // 加载势力特效
    UFUNCTION(BlueprintCallable, Category = "Faction Assets")
    UParticleSystem* LoadFactionEffect(const FName& EffectID);

    // 异步批量加载
    UFUNCTION(BlueprintCallable, Category = "Faction Assets")
    void LoadAssetsBatch(const TArray<FName>& AssetIDs, FOnBatchAssetsLoaded Delegate);

private:
    UPROPERTY()
    EFactionType CurrentFaction;

    UPROPERTY()
    TMap<FName, FMingAssetReference> FactionAssetMap;

    FString GetFactionAssetPath(const FName& AssetID, EAssetType Type);
};

// 实现
void UMingFactionAssetLoader::InitializeForFaction(EFactionType Faction)
{
    CurrentFaction = Faction;
    FactionAssetMap.Empty();

    // 加载势力资产清单
    FString ManifestPath = FString::Printf(
        TEXT("/Game/Data/AssetManifests/Manifest_%s"),
        *UEnum::GetValueAsString(Faction)
    );

    UDataTable* ManifestTable = LoadObject<UDataTable>(nullptr, *ManifestPath);
    if (ManifestTable)
    {
        // 解析资产清单
        TArray<FAssetManifestRow*> Rows;
        ManifestTable->GetAllRows(TEXT("InitializeForFaction"), Rows);

        for (FAssetManifestRow* Row : Rows)
        {
            FMingAssetReference AssetRef;
            AssetRef.AssetID = Row->AssetID;
            AssetRef.Faction = Faction;
            AssetRef.AssetType = Row->AssetType;
            AssetRef.AssetPtr = TSoftObjectPtr<UObject>(FSoftObjectPath(Row->AssetPath));

            FactionAssetMap.Add(Row->AssetID, AssetRef);
        }
    }
}

UObject* UMingFactionAssetLoader::LoadUnitAsset(const FName& UnitID, TSubclassOf<UObject> AssetClass)
{
    FMingAssetReference* AssetRef = FactionAssetMap.Find(UnitID);
    if (AssetRef)
    {
        return AssetRef->LoadAssetSync();
    }

    // 如果清单中没有，尝试动态构建路径
    FString DynamicPath = GetFactionAssetPath(UnitID, EAssetType::Character);
    return StaticLoadObject(AssetClass, nullptr, *DynamicPath);
}

FString UMingFactionAssetLoader::GetFactionAssetPath(const FName& AssetID, EAssetType Type)
{
    FString FactionCode = GetFactionCode(CurrentFaction);
    FString TypeFolder = GetAssetTypeFolder(Type);

    return FString::Printf(
        TEXT("/Game/Art/%s/%s/%s_%s"),
        *TypeFolder,
        *FactionCode,
        *FactionCode,
        *AssetID.ToString()
    );
}
```

---

## 4. 单位资产蓝图配置

### 4.1 单位资产数据结构

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FUnitAssetConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Assets")
    FName UnitID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Assets")
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Assets")
    TSoftObjectPtr<UStaticMesh> StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Assets")
    TSubclassOf<UAnimInstance> AnimationBlueprint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Assets")
    TArray<FMingAssetReference> WeaponAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Assets")
    FMingAssetReference DeathEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Assets")
    FMingAssetReference SelectionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Assets")
    TMap<EUnitAnimationState, UAnimSequence*> AnimationMap;

    // 应用配置到蓝图实例
    UFUNCTION(BlueprintCallable, Category = "Unit Assets")
    void ApplyToActor(AActor* UnitActor);
};

void FUnitAssetConfig::ApplyToActor(AActor* UnitActor)
{
    if (!UnitActor)
        return;

    // 设置网格
    USkeletalMeshComponent* SkeletalComp = UnitActor->FindComponentByClass<USkeletalMeshComponent>();
    if (SkeletalComp && SkeletalMesh.IsValid())
    {
        SkeletalComp->SetSkeletalMesh(SkeletalMesh.LoadSynchronous());

        // 设置动画蓝图
        if (AnimationBlueprint)
        {
            SkeletalComp->SetAnimInstanceClass(AnimationBlueprint);
        }
    }

    // 应用武器
    for (const FMingAssetReference& WeaponAsset : WeaponAssets)
    {
        // 附加武器到指定插槽
        AttachWeaponToUnit(UnitActor, WeaponAsset);
    }

    // 设置特效组件
    SetupEffectComponents(UnitActor);
}
```

### 4.2 单位蓝图基类

```cpp
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API AMingUnitCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMingUnitCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FName UnitID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    EFactionType Faction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FUnitAssetConfig AssetConfig;

    // 蓝图可调用的资产应用函数
    UFUNCTION(BlueprintCallable, Category = "Unit|Assets")
    void ApplyAssetConfig();

    UFUNCTION(BlueprintCallable, Category = "Unit|Assets")
    void SetFactionVisuals(EFactionType NewFaction);

    UFUNCTION(BlueprintCallable, Category = "Unit|Assets")
    void PlayFactionIdleAnimation();

    // 动态加载资产（运行时）
    UFUNCTION(BlueprintCallable, Category = "Unit|Assets")
    void LoadAssetsAsync(FOnUnitAssetsLoaded Delegate);

    // 资产加载完成回调
    UFUNCTION()
    void OnAssetsLoaded();

protected:
    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UParticleSystemComponent> FactionEffectComponent;

    void AttachFactionWeapon();
    void SpawnFactionEffect();
};

void AMingUnitCharacter::ApplyAssetConfig()
{
    // 应用资产配置
    AssetConfig.ApplyToActor(this);

    // 应用势力材质
    UMingBlueprintBridge::ApplyFactionVisualsToActor(this, Faction);

    // 更新动画状态
    if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
    {
        if (UMingUnitAnimInstance* MingAnim = Cast<UMingUnitAnimInstance>(AnimInst))
        {
            MingAnim->InitializeForUnit(UnitID, Faction);
        }
    }
}

void AMingUnitCharacter::SetFactionVisuals(EFactionType NewFaction)
{
    Faction = NewFaction;

    // 重新应用材质
    UMingAssetManager* AssetManager = UMingAssetManager::Get();
    if (AssetManager)
    {
        // 为所有网格组件创建新的材质实例
        TArray<UMeshComponent*> MeshComponents;
        GetComponents<UMeshComponent>(MeshComponents);

        for (UMeshComponent* MeshComp : MeshComponents)
        {
            AssetManager->CreateFactionMaterialInstance(Faction, MeshComp);
        }
    }

    // 更新特效
    SpawnFactionEffect();
}
```

---

## 5. 建筑资产蓝图配置

### 5.1 建筑资产数据结构

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FBuildingAssetConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Assets")
    FName BuildingID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Assets")
    TArray<TSoftObjectPtr<UStaticMesh>> LODMeshes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Assets")
    TSoftObjectPtr<UStaticMesh> ConstructionMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Assets")
    FMingAssetReference ConstructionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Assets")
    FMingAssetReference DestructionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Assets")
    FMingAssetReference ActiveEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Assets")
    TMap<EBuildingState, UAnimSequence*> StateAnimations;

    // 应用到建筑Actor
    UFUNCTION(BlueprintCallable, Category = "Building Assets")
    void ApplyToBuilding(AActor* BuildingActor);
};
```

### 5.2 建筑蓝图基类

```cpp
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API AMingBuilding : public AActor
{
    GENERATED_BODY()

public:
    AMingBuilding();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FName BuildingID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    EFactionType Faction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FBuildingAssetConfig AssetConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> BuildingMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UParticleSystemComponent> EffectComponent;

    // 蓝图可调用的建筑函数
    UFUNCTION(BlueprintCallable, Category = "Building|Assets")
    void ApplyBuildingAssets();

    UFUNCTION(BlueprintCallable, Category = "Building|State")
    void SetBuildingState(EBuildingState NewState);

    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    void StartConstruction();

    UFUNCTION(BlueprintCallable, Category = "Building|Construction")
    void CompleteConstruction();

    UFUNCTION(BlueprintNativeEvent, Category = "Building|Effects")
    void PlayConstructionEffect();

    UFUNCTION(BlueprintNativeEvent, Category = "Building|Effects")
    void PlayDestructionEffect();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Building")
    EBuildingState CurrentState;

    void UpdateMeshForState();
    void UpdateEffectForState();
};
```

---

## 6. 特效与UI资产关联

### 6.1 特效资产系统

```cpp
USTRUCT(BlueprintType)
struct MINGGORTS_API FFactionEffectSet
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    EFactionType Faction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TMap<EEffectType, FMingAssetReference> EffectMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    FLinearColor FactionColor;

    // 获取势力特效
    UFUNCTION(BlueprintCallable, Category = "Effects")
    UParticleSystem* GetEffect(EEffectType Type);

    // 在位置生成特效
    UFUNCTION(BlueprintCallable, Category = "Effects", meta = (WorldContext = "WorldContextObject"))
    UParticleSystemComponent* SpawnEffectAtLocation(UObject* WorldContextObject, EEffectType Type, FVector Location);

    // 附加到组件
    UFUNCTION(BlueprintCallable, Category = "Effects")
    UParticleSystemComponent* AttachEffectToComponent(EEffectType Type, USceneComponent* Component, FName SocketName);
};
```

### 6.2 UI资产绑定

```cpp
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingUIAssetLibrary : public UObject
{
    GENERATED_BODY()

public:
    // 获取势力图标
    UFUNCTION(BlueprintCallable, Category = "UI Assets")
    UTexture2D* GetFactionIcon(EFactionType Faction);

    // 获取势力旗帜
    UFUNCTION(BlueprintCallable, Category = "UI Assets")
    UTexture2D* GetFactionFlag(EFactionType Faction);

    // 获取单位图标
    UFUNCTION(BlueprintCallable, Category = "UI Assets")
    UTexture2D* GetUnitIcon(const FName& UnitID);

    // 获取建筑图标
    UFUNCTION(BlueprintCallable, Category = "UI Assets")
    UTexture2D* GetBuildingIcon(const FName& BuildingID);

    // 获取势力主题颜色
    UFUNCTION(BlueprintPure, Category = "UI Assets")
    FLinearColor GetFactionThemeColor(EFactionType Faction);

    // 应用势力主题到UI
    UFUNCTION(BlueprintCallable, Category = "UI Assets")
    void ApplyFactionThemeToWidget(UUserWidget* Widget, EFactionType Faction);

private:
    UPROPERTY()
    TMap<EFactionType, TObjectPtr<UTexture2D>> FactionIcons;

    UPROPERTY()
    TMap<EFactionType, FLinearColor> FactionThemeColors;
};
```

---

## 7. 数据驱动的资产关联

### 7.1 资产配置数据表

```cpp
// 资产清单行结构
USTRUCT(BlueprintType)
struct FAssetManifestRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Manifest")
    FName AssetID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Manifest")
    EAssetType AssetType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Manifest")
    FString AssetPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Manifest")
    EFactionType Faction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Manifest")
    FGameplayTagContainer Tags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Manifest")
    bool bPreload = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Manifest")
    int32 LoadPriority = 0;
};

// 单位资产配置行
USTRUCT(BlueprintType)
struct FUnitAssetConfigRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Config")
    FName UnitID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Config")
    EFactionType Faction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Config")
    FString SkeletalMeshPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Config")
    FString AnimationBPPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Config")
    TArray<FString> WeaponPaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Config")
    FString DeathEffectPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Config")
    FLinearColor UniformColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Config")
    float UniformWearAmount = 0.0f;
};
```

### 7.2 CSV数据表示例

```csv
// AssetManifest_Beiyang.csv
AssetID,AssetType,AssetPath,Faction,Tags,bPreload,LoadPriority
BY_HERO_DUAN,Character,/Game/Art/Characters/BY/BY_Hero_Duan.BY_Hero_Duan,BeiyangGovernment,Hero;Elite,TRUE,100
BY_INF_01,Character,/Game/Art/Characters/BY/BY_Inf_01.BY_Inf_01,BeiyangGovernment,Infantry;Basic,FALSE,50
BY_ELITE_GUARDS,Character,/Game/Art/Characters/BY/BY_Elite_Guards.BY_Elite_Guards,BeiyangGovernment,Infantry;Elite,TRUE,80
BY_COMMAND_CENTER,Building,/Game/Art/Buildings/BY/BY_CommandCenter.BY_CommandCenter,BeiyangGovernment,Command;Important,TRUE,90
BY_BARRACKS,Building,/Game/Art/Buildings/BY/BY_Barracks.BY_Barracks,BeiyangGovernment,Barracks,FALSE,40
BY_INF_EFFECT,Effect,/Game/Art/Effects/BY/BY_Infantry_Effect.BY_Infantry_Effect,BeiyangGovernment,UnitEffect,FALSE,30
```

---

## 8. 编辑器工具集成

### 8.1 资产验证工具

```cpp
UCLASS()
class UMingAssetValidator : public UAssetValidationManager
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Asset Tools")
    bool ValidateFactionAssets(EFactionType Faction, FString& OutReport);

    UFUNCTION(BlueprintCallable, Category = "Asset Tools")
    bool FixBrokenAssetReferences(EFactionType Faction);

    UFUNCTION(BlueprintCallable, Category = "Asset Tools")
    void GenerateAssetManifest(EFactionType Faction);
};
```

### 8.2 资产浏览器扩展

```cpp
// 自定义Content Browser扩展，用于快速查找势力资产
class FMingContentBrowserExtensions
{
public:
    static void CreateFactionAssetFilter();
    static void AddFactionColorIndicator();
    static void RegisterAssetContextMenu();
};
```

---

## 9. 蓝图使用示例

### 9.1 在蓝图中动态加载单位

```
[BeginPlay]
    │
    ▼
[Get Asset Manager] ──► [Load Faction Assets]
    │                       │
    │                       ▼
    │               [Async Load Complete]
    │                       │
    ▼                       ▼
[Spawn Actor From Class] ◄┘
    │
    ▼
[Apply Faction Visuals]
    │
    ▼
[Set Material Parameters]
```

### 9.2 蓝图节点说明

| 节点名称 | 类别 | 输入 | 输出 | 说明 |
|---------|------|------|------|------|
| **Get Ming Asset Manager** | Utility | - | Asset Manager | 获取资产管理器 |
| **Load Unit Asset** | Asset | UnitID, Faction | Asset Object | 加载单位资产 |
| **Apply Faction Visuals** | Visual | Target Actor, Faction | - | 应用势力视觉 |
| **Create Faction Material** | Material | Faction, Target Mesh | Material Instance | 创建势力材质 |
| **Spawn Faction Effect** | Effect | Effect Type, Location | Particle Component | 生成势力特效 |
| **Get Faction Color** | Color | Faction | Linear Color | 获取势力颜色 |

---

## 10. 性能优化

### 10.1 异步加载策略

```cpp
// 异步资产加载队列
UCLASS()
class UMingAsyncAssetLoader : public UObject
{
    GENERATED_BODY()

public:
    void QueueAssetLoad(const FName& AssetID, FOnAssetLoaded Callback);
    void ProcessQueue(float DeltaTime);
    void SetMaxConcurrentLoads(int32 MaxCount);

private:
    TArray<FAssetLoadRequest> LoadQueue;
    TArray<FAssetLoadRequest> ActiveLoads;
    int32 MaxConcurrentLoads = 5;
};
```

### 10.2 资产内存池

```cpp
// 单位资产对象池
UCLASS()
class UMingUnitAssetPool : public UObject
{
    GENERATED_BODY()

public:
    AActor* AcquireUnit(const FName& UnitID, EFactionType Faction);
    void ReleaseUnit(AActor* Unit);
    void PreloadUnitAssets(const FName& UnitID, int32 Count);

private:
    TMap<FName, TArray<TObjectPtr<AActor>>> AvailableUnits;
    TMap<FName, FMingAssetReference> AssetConfigs;
};
```

---

## 附录

### A. 参考文档

- 美术资源制作: `docs/ART_PRODUCTION_IMPLEMENTATION.md`
- 机制系统实现: `docs/MECHANIC_SYSTEM_IMPLEMENTATION.md`
- 数据配置: `docs/DATA_CONFIGURATION.md`

### B. 蓝图资产路径规范

```
/Game/Blueprints/
├── Units/
│   ├── BP_MingUnitCharacter_Base.uasset
│   ├── BP_MingUnitCharacter_Beiyang.uasset
│   └── BP_MingUnitCharacter_GMD.uasset
├── Buildings/
│   ├── BP_MingBuilding_Base.uasset
│   └── BP_MingBuilding_CommandCenter.uasset
├── Effects/
│   ├── BP_MingFactionEffect_Base.uasset
│   └── BP_MingExplosionEffect.uasset
└── UI/
    ├── WBP_MingFactionWidget.uasset
    └── WBP_MingUnitPortrait.uasset
```

### C. 联系方式

- 技术美术: tech-art@minggorts.com
- 蓝图程序员: bp-dev@minggorts.com

---

*文档结束*
