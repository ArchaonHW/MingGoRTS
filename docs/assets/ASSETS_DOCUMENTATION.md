# MingGoRTS 資產文檔

## 概述

MingGoRTS 的資產系統涵蓋了遊戲所需的所有視覺和音頻資源，包括 3D 模型、紋理、材質、動畫、音效、音樂和 UI 資產。本文檔詳細介紹了資產的分類、規範、製作流程和管理系統。

## 目錄

- [資產分類](#資產分類)
- [製作規範](#製作規範)
- [資產管理](#資產管理)
- [3D 模型資產](#3d-模型資產)
- [紋理和材質](#紋理和材質)
- [動畫資產](#動畫資產)
- [音頻資產](#音頻資產)
- [UI 資產](#ui-資產)
- [特效資產](#特效資產)
- [場景資產](#場景資產)
- [AI 生成資產](#ai-生成資產)
- [性能優化](#性能優化)
- [版本控制](#版本控制)

---

## 資產分類

### 資產類型結構

```
MingGoRTS 資產
├── 3D Assets (3D 資產)
│   ├── Characters (角色)
│   ├── Buildings (建築)
│   ├── Units (單位)
│   ├── Props (道具)
│   ├── Weapons (武器)
│   └── Vehicles (載具)
├── Textures (紋理)
│   ├── Characters (角色紋理)
│   ├── Buildings (建築紋理)
│   ├── Environment (環境紋理)
│   ├── UI (UI 紋理)
│   └── Effects (特效紋理)
├── Materials (材質)
│   ├── Character Materials (角色材質)
│   ├── Building Materials (建築材質)
│   ├── Environment Materials (環境材質)
│   └── Effect Materials (特效材質)
├── Animations (動畫)
│   ├── Character Animations (角色動畫)
│   ├── Unit Animations (單位動畫)
│   ├── Building Animations (建築動畫)
│   └── Effect Animations (特效動畫)
├── Audio (音頻)
│   ├── Music (音樂)
│   ├── Sound Effects (音效)
│   ├── Voice Lines (語音)
│   └── Ambient Sounds (環境音)
├── UI Assets (UI 資產)
│   ├── Icons (圖標)
│   ├── Buttons (按鈕)
│   ├── Panels (面板)
│   └── HUD Elements (HUD 元素)
├── VFX (視覺特效)
│   ├── Particle Effects (粒子特效)
│   ├── Post-Process Effects (後處理特效)
│   └── Shader Effects (着色器特效)
└── Levels (關卡)
    ├── Battle Maps (戰役地圖)
    ├── Campaign Maps (戰役地圖)
    └── Tutorial Maps (教程地圖)
```

### 資產命名規範

#### 命名格式
```
[AssetType]_[Category]_[SubCategory]_[SpecificName]_[Variant]_[LOD]
```

#### 示例
- `CHR_Ming_Warrior_Heavy_01_LOD0` - 明朝重甲戰士角色高質量版本
- `BLD_Ming_Palace_Main_01_LOD0` - 明朝宮殿主建築高質量版本
- `TEX_Ming_Wall_Stone_01_D` - 明朝石牆紋理漫反射貼圖
- `MAT_Ming_Gold_01` - 明朝黃金材質
- `ANI_Ming_Warrior_Sword_Attack_01` - 明朝戰士劍擊動畫
- `SFX_Ming_Sword_Metal_Hit_01` - 明朝劍擊金屬音效
- `UI_Ming_Icon_Resource_Gold_01` - 明朝黃金資源圖標
- `VFX_Ming_Explosion_Fire_01` - 明朝火焰爆炸特效

---

## 製作規範

### 3D 模型規範

#### 多邊形數量限制
| 資產類型 | LOD0 (高) | LOD1 (中) | LOD2 (低) | LOD3 (最低) |
|----------|-----------|-----------|-----------|-------------|
| 主角角色 | 15,000 | 8,000 | 3,000 | 1,000 |
| 次要角色 | 8,000 | 4,000 | 1,500 | 500 |
| 單位 | 3,000 | 1,500 | 800 | 300 |
| 建築 | 20,000 | 10,000 | 5,000 | 2,000 |
| 道具 | 2,000 | 1,000 | 500 | 200 |
| 武器 | 1,500 | 800 | 400 | 200 |

#### UV 展開規範
- **UV 空間利用率**: 85-95%
- **UV 縫隙**: 最小化，避免在可見區域
- **UV 島數量**: 合理控制，便於紋理繪製
- **UV 重疊**: 僅在對稱部分允許重疊

#### 拓扑結構
- **四邊面為主**: 盡量使用四邊面，避免三角面
- **布線均勻**: 避免過度拉伸的三角面
- **邊流**: 順循肌肉結構和布料走向
- **關節區域**: 在關節處增加環形邊

### 紋理規範

#### 紋理分辨率
| 紋理類型 | 高質量 | 中質量 | 低質量 | 最低質量 |
|----------|--------|--------|--------|----------|
| 角色漫反射 | 2048x2048 | 1024x1024 | 512x512 | 256x256 |
| 角色法線 | 2048x2048 | 1024x1024 | 512x512 | 256x256 |
| 建築漫反射 | 4096x4096 | 2048x2048 | 1024x1024 | 512x512 |
| 建築法線 | 2048x2048 | 1024x1024 | 512x512 | 256x256 |
| 道具漫反射 | 1024x1024 | 512x512 | 256x256 | 128x128 |
| UI 圖標 | 256x256 | 128x128 | 64x64 | 32x32 |

#### 紋理格式
- **漫反射**: BC7 (DX11) / ASTC (移動端)
- **法線**: BC5 (DX11) / ASTC (移動端)
- **金屬度/粗糙度**: BC4 (單通道) / ASTC (移動端)
- **遮罩**: BC4 (單通道) / ASTC (移動端)

#### 紋理通道分配
- **漫反射 (RGB)**: 基礎顏色
- **漫反射 (A)**: 透明度 (可選)
- **法線 (RGB)**: 法線向量
- **金屬度 (R)**: 金屬度值
- **粗糙度 (G)**: 粗糙度值
- **遮罩 (B)**: 遮罩值

---

## 資產管理

### 資產數據庫

#### 資產元數據結構
```cpp
// 資產元數據
USTRUCT(BlueprintType)
struct MINGASSETS_API FMingAssetMetadata
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Subcategory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Tags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CreatedBy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CreatedDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LastModifiedBy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastModifiedDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Version;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAssetDependency> Dependencies;
};
```

#### 資產依賴關係
```cpp
// 資產依賴關係
USTRUCT(BlueprintType)
struct MINGASSETS_API FMingAssetDependency
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DependencyAssetID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDependencyType DependencyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;
};
```

### 資產加載系統

#### 資產加載器
```cpp
// 資產加載器
UCLASS()
class MINGASSETS_API UMingAssetLoader : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, UObject*> LoadedAssets;
    TQueue<FMingAssetLoadRequest> LoadQueue;
    TMap<FString, FMingAssetMetadata> AssetMetadata;

public:
    // 異步加載資產
    UFUNCTION(BlueprintCallable, Category = "Asset Loader")
    void LoadAssetAsync(const FString& AssetID, const FMingOnAssetLoaded& OnLoaded);

    // 同步加載資產
    UFUNCTION(BlueprintCallable, Category = "Asset Loader")
    UObject* LoadAssetSync(const FString& AssetID);

    // 卸載資產
    UFUNCTION(BlueprintCallable, Category = "Asset Loader")
    void UnloadAsset(const FString& AssetID);

    // 預載入資產
    UFUNCTION(BlueprintCallable, Category = "Asset Loader")
    void PreloadAssets(const TArray<FString>& AssetIDs);

    // 獲取資產元數據
    UFUNCTION(BlueprintCallable, Category = "Asset Loader")
    FMingAssetMetadata GetAssetMetadata(const FString& AssetID) const;

    // 檢查資產是否已加載
    UFUNCTION(BlueprintCallable, Category = "Asset Loader")
    bool IsAssetLoaded(const FString& AssetID) const;

private:
    void ProcessLoadQueue();
    void OnAssetLoadedCompleted(UObject* LoadedAsset, const FString& AssetID);
};
```

---

## 3D 模型資產

### 角色模型

#### 明朝角色特點
- **服飾風格**: 基於明朝歷史文獻的服飾設計
- **面部特徵**: 符合明朝時期的人物特徵
- **裝備風格**: 明朝特色的武器和防具
- **動作風格**: 符合明朝武術和戰鬥風格

#### 角色分類
1. **皇室成員**
   - 皇帝、皇后、太子、公主
   - 朝臣、太監、宮女
   - 特點：華麗服飾、精緻配件

2. **軍事人員**
   - 將軍、校尉、士兵
   - 特點：盔甲、武器、軍事裝備

3. **平民百姓**
   - 農民、工匠、商人
   - 特點：簡樸服飾、生活用品

4. **宗教人士**
   - 道士、僧侶、祭祀
   - 特點：宗教服飾、法器

### 建築模型

#### 明朝建築風格
- **宮殿建築**: 故宮風格的宏大建築
- **民居建築**: 四合院、江南民居
- **宗教建築**: 寺廟、道觀、祠堂
- **軍事建築**: 城牆、箭塔、關隘

#### 建築組件
1. **結構組件**
   - 柱子、橑、斗拱
   - 屋頂、屋脊、瓦片
   - 牆體、門窗、台基

2. **裝飾組件**
   - 雕刻、彩繪、裝飾
   - 欄杆、台階、石獅

3. **功能組件**
   - 家具、陳設、器具
   - 照明、取暖、通風設備

### 單位模型

#### 軍事單位
1. **步兵單位**
   - 長槍兵、刀盾兵、弓箭手
   - 特點：標準化裝備、隊形整齊

2. **騎兵單位**
   - 輕騎兵、重騎兵、弓騎兵
   - 特點：機動性強、衝擊力大

3. **攻城單位**
   - 投石車、攻城錘、雲梯
   - 特點：大型器械、攻城專用

#### 平民單位
1. **生產單位**
   - 農民、礦工、伐木工
   - 特點：生產工具、工作服飾

2. **服務單位**
   - 商人、工匠、學者
   - 特點：專業工具、身份標識

---

## 紋理和材質

### 紋理製作流程

#### 1. 概念設計
- 參考明朝歷史資料
- 繪製概念圖和色彩方案
- 確定風格方向

#### 2. 高模製作
- 使用 ZBrush 等軟件製作高模
- 添加細節和紋理
- 烘焙法線貼圖

#### 3. 低模製作
- 拓扑重建和優化
- UV 展開和排列
- 烘焙各種貼圖

#### 4. 紋理繪製
- 使用 Substance Painter 繪製紋理
- 創作 PBR 材質
- 添加細節和效果

### 材質系統

#### PBR 材質參數
```cpp
// PBR 材質參數
USTRUCT(BlueprintType)
struct MINGASSETS_API FMingPBRMaterialParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* BaseColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* NormalMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* MetallicMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* RoughnessMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* AmbientOcclusionMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MetallicValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RoughnessValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmissiveStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor EmissiveColor;
};
```

#### 材質實例
```cpp
// 動態材質實例
UCLASS()
class MINGASSETS_API UMingDynamicMaterialInstance : public UMaterialInstanceDynamic
{
    GENERATED_BODY()

public:
    // 設置基礎顏色
    UFUNCTION(BlueprintCallable, Category = "Material")
    void SetBaseColor(const FLinearColor& Color);

    // 設置金屬度
    UFUNCTION(BlueprintCallable, Category = "Material")
    void SetMetallic(float Value);

    // 設置粗糙度
    UFUNCTION(BlueprintCallable, Category = "Material")
    void SetRoughness(float Value);

    // 設置自發光
    UFUNCTION(BlueprintCallable, Category = "Material")
    void SetEmissive(const FLinearColor& Color, float Strength);

    // 添加紋理參數
    UFUNCTION(BlueprintCallable, Category = "Material")
    void SetTextureParameter(const FString& ParameterName, UTexture* Texture);

    // 添加向量參數
    UFUNCTION(BlueprintCallable, Category = "Material")
    void SetVectorParameter(const FString& ParameterName, const FLinearColor& Value);

    // 添加標量參數
    UFUNCTION(BlueprintCallable, Category = "Material")
    void SetScalarParameter(const FString& ParameterName, float Value);
};
```

---

## 動畫資產

### 動畫分類

#### 角色動畫
1. **移動動畫**
   - 行走、跑步、跳躍
   - 潛行、游泳、飛行
   - 騎馬、駕駛

2. **戰鬥動畫**
   - 攻擊、防禦、格擋
   - 受傷、死亡、復活
   - 技能釋放、必殺技

3. **交互動畫**
   - 開門、拾取、使用
   - 對話、交易、祈禱
   - 建造、修理、升級

#### 建築動畫
1. **建設動畫**
   - 地基建造、牆體搭建
   - 屋頂安裝、裝飾添加
   - 完成效果、慶祝動畫

2. **功能動畫**
   - 門窗開關、機械運轉
   - 燈光變化、煙霧效果
   - 損壞、修復、升級

3. **環境動畫**
   - 旗幟飄揚、水流動
   - 樹木搖曳、草葉擺動
   - 天氣變化、日夜循環

### 動畫系統

#### 動畫藍圖
```cpp
// 動畫藍圖管理器
UCLASS()
class MINGASSETS_API UMingAnimationManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, UAnimMontage*> AnimationMontages;
    TMap<FString, UAnimSequence*> AnimationSequences;

public:
    // 註冊動畫蒙太奇
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void RegisterAnimationMontage(const FString& MontageID, UAnimMontage* Montage);

    // 註冊動畫序列
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void RegisterAnimationSequence(const FString& SequenceID, UAnimSequence* Sequence);

    // 播放動畫蒙太奇
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void PlayAnimationMontage(ACharacter* Character, const FString& MontageID);

    // 播放動畫序列
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void PlayAnimationSequence(ACharacter* Character, const FString& SequenceID);

    // 停止動畫
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void StopAnimation(ACharacter* Character);

    // 混合動畫
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void BlendAnimations(ACharacter* Character, const FString& BaseAnimation, const FString& BlendAnimation, float BlendTime);
};
```

#### 動畫狀態機
```cpp
// 動畫狀態機
UCLASS()
class MINGASSETS_API UMingAnimationStateMachine : public UObject
{
    GENERATED_BODY()

private:
    TMap<FString, UMingAnimationState*> States;
    FString CurrentStateID;
    TMap<FString, TArray<FString>> StateTransitions;

public:
    // 添加狀態
    UFUNCTION(BlueprintCallable, Category = "Animation State Machine")
    void AddState(const FString& StateID, UMingAnimationState* State);

    // 添加狀態轉換
    UFUNCTION(BlueprintCallable, Category = "Animation State Machine")
    void AddStateTransition(const FString& FromState, const FString& ToState);

    // 設置當前狀態
    UFUNCTION(BlueprintCallable, Category = "Animation State Machine")
    void SetCurrentState(const FString& StateID);

    // 獲取當前狀態
    UFUNCTION(BlueprintCallable, Category = "Animation State Machine")
    FString GetCurrentState() const { return CurrentStateID; }

    // 檢查是否可以轉換
    UFUNCTION(BlueprintCallable, Category = "Animation State Machine")
    bool CanTransitionTo(const FString& TargetState) const;

    // 執行狀態轉換
    UFUNCTION(BlueprintCallable, Category = "Animation State Machine")
    void TransitionTo(const FString& TargetState);
};
```

---

## 音頻資產

### 音頻分類

#### 音樂資產
1. **背景音樂**
   - 主題音樂、環境音樂
   - 戰鬥音樂、和平音樂
   - 情緒音樂、場景音樂

2. **互動音樂**
   - 勝利音樂、失敗音樂
   - 升級音樂、成就音樂
   - 事件音樂、劇情音樂

#### 音效資產
1. **環境音效**
   - 風聲、雨聲、雷聲
   - 鳥鳴、蟲鳴、水流
   - 城市噪音、鄉村聲音

2. **動作音效**
   - 腳步聲、跑步聲
   - 攻擊聲、受傷聲
   - 建造聲、破壞聲

3. **UI 音效**
   - 按鈕聲、提示聲
   - 錯誤聲、成功聲
   - 通知聲、警告聲

#### 語音資產
1. **角色語音**
   - 對話語音、戰鬥喊聲
   - 受傷呼喊、死亡慘叫
   - 命令回應、狀態報告

2. **旁白語音**
   - 劇情旁白、教程旁白
   - 歷史介紹、文化解說
   - 系統提示、幫助信息

### 音頻系統

#### 音頻管理器
```cpp
// 音頻管理器
UCLASS()
class MINGASSETS_API UMingAudioAssetManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, USoundBase*> SoundAssets;
    TMap<FString, USoundCue*> SoundCues;
    TMap<FString, UAudioComponent*> PlayingComponents;

public:
    // 註冊音頻資產
    UFUNCTION(BlueprintCallable, Category = "Audio Asset Manager")
    void RegisterSoundAsset(const FString& SoundID, USoundBase* Sound);

    // 註冊音頻提示
    UFUNCTION(BlueprintCallable, Category = "Audio Asset Manager")
    void RegisterSoundCue(const FString& CueID, USoundCue* Cue);

    // 播放音頻
    UFUNCTION(BlueprintCallable, Category = "Audio Asset Manager")
    UAudioComponent* PlaySound(const FString& SoundID, const FVector& Location = FVector::ZeroVector, float VolumeMultiplier = 1.0f);

    // 播放音頻提示
    UFUNCTION(BlueprintCallable, Category = "Audio Asset Manager")
    UAudioComponent* PlaySoundCue(const FString& CueID, AActor* Actor = nullptr);

    // 停止音頻
    UFUNCTION(BlueprintCallable, Category = "Audio Asset Manager")
    void StopSound(const FString& SoundID);

    // 設置音頻參數
    UFUNCTION(BlueprintCallable, Category = "Audio Asset Manager")
    void SetSoundParameters(const FString& SoundID, float Volume, float Pitch, float Pan);

    // 獲取音頻資產
    UFUNCTION(BlueprintCallable, Category = "Audio Asset Manager")
    USoundBase* GetSoundAsset(const FString& SoundID) const;
};
```

---

## UI 資產

### UI 資產分類

#### 圖標資產
1. **單位圖標**
   - 角色頭像、單位圖標
   - 建築圖標、資源圖標
   - 技能圖標、物品圖標

2. **功能圖標**
   - 按鈕圖標、菜單圖標
   - 狀態圖標、提示圖標
   - 導航圖標、工具圖標

#### 面板資產
1. **背景面板**
   - 主背景、邊框裝飾
   - 分隔線、角標裝飾
   - 透明度變化、漸變效果

2. **交互元素**
   - 按鈕、滑塊、複選框
   - 下拉菜單、列表框
   - 輸入框、標籤頁

#### HUD 資產
1. **戰鬥 HUD**
   - 生命條、法力條
   - 經驗條、等級顯示
   - 小地圖、羅盤

2. **資源 HUD**
   - 資源顯示、經濟指標
   - 時間顯示、日期顯示
   - 任務提示、事件通知

### UI 資產系統

#### UI 資產管理器
```cpp
// UI 資產管理器
UCLASS()
class MINGASSETS_API UMingUIAssetManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, UTexture2D*> UIIcons;
    TMap<FString, UMaterialInterface*> UIMaterials;
    TMap<FString, UWidgetBlueprint*> UIWidgets;

public:
    // 註冊 UI 圖標
    UFUNCTION(BlueprintCallable, Category = "UI Asset Manager")
    void RegisterUIIcon(const FString& IconID, UTexture2D* Icon);

    // 註冊 UI 材質
    UFUNCTION(BlueprintCallable, Category = "UI Asset Manager")
    void RegisterUIMaterial(const FString& MaterialID, UMaterialInterface* Material);

    // 註冊 UI 組件
    UFUNCTION(BlueprintCallable, Category = "UI Asset Manager")
    void RegisterUIWidget(const FString& WidgetID, UWidgetBlueprint* Widget);

    // 獲取 UI 圖標
    UFUNCTION(BlueprintCallable, Category = "UI Asset Manager")
    UTexture2D* GetUIIcon(const FString& IconID) const;

    // 獲取 UI 材質
    UFUNCTION(BlueprintCallable, Category = "UI Asset Manager")
    UMaterialInterface* GetUIMaterial(const FString& MaterialID) const;

    // 創建 UI 組件
    UFUNCTION(BlueprintCallable, Category = "UI Asset Manager")
    UUserWidget* CreateUIWidget(const FString& WidgetID, APlayerController* OwningController);

    // 動態加載 UI 資產
    UFUNCTION(BlueprintCallable, Category = "UI Asset Manager")
    void LoadUIAssetsAsync(const TArray<FString>& AssetIDs, const FMingOnUIAssetsLoaded& OnLoaded);
};
```

---

## 特效資產

### 視覺特效分類

#### 粒子特效
1. **戰鬥特效**
   - 劍光、箭矢、魔法
   - 爆炸、煙霧、火焰
   - 血液、傷害、治療

2. **環境特效**
   - 雨雪、風沙、霧霾
   - 光影、水波、植被
   - 天氣變化、季節效果

3. **UI 特效**
   - 按鈕高亮、選擇效果
   - 數字跳動、進度條
   - 通知閃爍、警告提示

#### 後處理特效
1. **色彩調整**
   - 色彩校正、對比度
   - 飽和度、色相調整
   - 濾鏡效果、藝術風格

2. **光學效果**
   - 暈影、光暈、眩光
   - 運動模糊、景深
   - 抗鋸齒、銳化

### 特效系統

#### 特效管理器
```cpp
// 特效管理器
UCLASS()
class MINGASSETS_API UMingVFXManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, UParticleSystem*> ParticleEffects;
    TMap<FString, UMaterialInterface*> PostProcessMaterials;
    TMap<FString, UNiagaraSystem*> NiagaraSystems;

public:
    // 註冊粒子特效
    UFUNCTION(BlueprintCallable, Category = "VFX Manager")
    void RegisterParticleEffect(const FString& EffectID, UParticleSystem* Effect);

    // 註冊後處理材質
    UFUNCTION(BlueprintCallable, Category = "VFX Manager")
    void RegisterPostProcessMaterial(const FString& MaterialID, UMaterialInterface* Material);

    // 註冊 Niagara 系統
    UFUNCTION(BlueprintCallable, Category = "VFX Manager")
    void RegisterNiagaraSystem(const FString& SystemID, UNiagaraSystem* System);

    // 播放粒子特效
    UFUNCTION(BlueprintCallable, Category = "VFX Manager")
    UParticleSystemComponent* PlayParticleEffect(const FString& EffectID, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator);

    // 應用後處理效果
    UFUNCTION(BlueprintCallable, Category = "VFX Manager")
    void ApplyPostProcessEffect(const FString& MaterialID, float Intensity = 1.0f);

    // 播放 Niagara 系統
    UFUNCTION(BlueprintCallable, Category = "VFX Manager")
    UNiagaraComponent* PlayNiagaraSystem(const FString& SystemID, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator);

    // 停止特效
    UFUNCTION(BlueprintCallable, Category = "VFX Manager")
    void StopEffect(const FString& EffectID);
};
```

---

## 場景資產

### 場景分類

#### 戰役地圖
1. **明朝城池**
   - 北京城、南京城
   - 地方城池、邊關要塞
   - 宮殿建築、民居區域

2. **戰場地圖**
   - 平原戰場、山地戰場
   - 河流戰場、森林戰場
   - 攻城戰場、防禦戰場

3. **特殊地圖**
   - 訓練場、競技場
   - 歷史戰役重現
   - 挑戰地圖、試煉地圖

#### 環場景
1. **自然環境**
   - 山川河流、森林草原
   - 沙漠戈壁、冰雪世界
   - 海岸島嶼、地下洞穴

2. **人文環境**
   - 村莊鎮集、商業街區
   - 宗教場所、學府書院
   - 工坊礦場、農田牧場

### 場景系統

#### 場景管理器
```cpp
// 場景管理器
UCLASS()
class MINGASSETS_API UMingSceneManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, ULevel*> LoadedLevels;
    TMap<FString, FMingSceneMetadata> SceneMetadata;

public:
    // 加載場景
    UFUNCTION(BlueprintCallable, Category = "Scene Manager")
    bool LoadScene(const FString& SceneID, bool bAsync = true);

    // 卸載場景
    UFUNCTION(BlueprintCallable, Category = "Scene Manager")
    void UnloadScene(const FString& SceneID);

    // 獲取場景元數據
    UFUNCTION(BlueprintCallable, Category = "Scene Manager")
    FMingSceneMetadata GetSceneMetadata(const FString& SceneID) const;

    // 預載入場景
    UFUNCTION(BlueprintCallable, Category = "Scene Manager")
    void PreloadScene(const FString& SceneID);

    // 流式加載場景
    UFUNCTION(BlueprintCallable, Category = "Scene Manager")
    void StreamLoadScene(const FString& SceneID, const FVector& StreamLocation);

    // 設置場景參數
    UFUNCTION(BlueprintCallable, Category = "Scene Manager")
    void SetSceneParameters(const FString& SceneID, const FMingSceneParameters& Parameters);
};
```

---

## AI 生成資產

### AI 資產生成系統

#### 自動化資產生成
```cpp
// AI 資產生成器
UCLASS()
class MINGASSETS_API UMingAIAssetGenerator : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, FMingAssetGenerationConfig> GenerationConfigs;
    TQueue<FMingAssetGenerationRequest> GenerationQueue;

public:
    // 生成 3D 模型
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void Generate3DModel(const FString& ModelID, const FString& Description, const FMingOnAssetGenerated& OnGenerated);

    // 生成紋理
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void GenerateTexture(const FString& TextureID, const FString& Description, const FMingOnAssetGenerated& OnGenerated);

    // 生成音頻
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void GenerateAudio(const FString& AudioID, const FString& Description, const FMingOnAssetGenerated& OnGenerated);

    // 生成動畫
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void GenerateAnimation(const FString& AnimationID, const FString& Description, const FMingOnAssetGenerated& OnGenerated);

    // 批量生成資產
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void GenerateAssetBatch(const TArray<FMingAssetGenerationRequest>& Requests);

    // 獲取生成狀態
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    EMingAssetGenerationStatus GetGenerationStatus(const FString& AssetID) const;

private:
    void ProcessGenerationQueue();
    void OnAssetGenerationCompleted(const FString& AssetID, UObject* GeneratedAsset);
};
```

#### 資產質量控制
```cpp
// 資產質量控制
UCLASS()
class MINGASSETS_API UMingAssetQualityController : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, FMingAssetQualityMetrics> QualityMetrics;

public:
    // 評估資產質量
    UFUNCTION(BlueprintCallable, Category = "Asset Quality Controller")
    FMingAssetQualityMetrics EvaluateAssetQuality(UObject* Asset);

    // 自動優化資產
    UFUNCTION(BlueprintCallable, Category = "Asset Quality Controller")
    UObject* OptimizeAsset(UObject* Asset, const FMingAssetOptimizationParams& Params);

    // 驗證資產規範
    UFUNCTION(BlueprintCallable, Category = "Asset Quality Controller")
    bool ValidateAssetStandards(UObject* Asset, const FMingAssetStandards& Standards);

    // 生成質量報告
    UFUNCTION(BlueprintCallable, Category = "Asset Quality Controller")
    FString GenerateQualityReport(const FString& AssetID) const;

    // 設置質量標準
    UFUNCTION(BlueprintCallable, Category = "Asset Quality Controller")
    void SetQualityStandards(const FMingAssetStandards& Standards);
};
```

---

## 性能優化

### 資產優化策略

#### 1. LOD 系統
- **多級別細節**: 根據距離調整模型精度
- **動態切換**: 無縫的 LOD 切換
- **記憶體管理**: 智能的 LOD 資產加載

#### 2. 紋理優化
- **紋理壓縮**: 使用高效的紋理格式
- **紋理圖集**: 合併小紋理減少 Draw Call
- **Mipmap 生成**: 自動生成多級別紋理

#### 3. 動畫優化
- **動畫壓縮**: 使用高效的動畫壓縮
- **動畫融合**: 減少動畫切換的開銷
- **骨骼優化**: 優化骨骼數量和權重

#### 4. 音頻優化
- **音頻壓縮**: 使用高效的音頻格式
- **音頻流式**: 大音頻文件的流式加載
- **音頻池**: 重複使用音頻組件

### 性能監控

#### 資產性能監控器
```cpp
// 資產性能監控器
UCLASS()
class MINGASSETS_API UMingAssetPerformanceMonitor : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, FMingAssetPerformanceData> PerformanceData;
    float LastUpdateTime;

public:
    // 開始監控
    UFUNCTION(BlueprintCallable, Category = "Asset Performance Monitor")
    void StartMonitoring(const FString& AssetID);

    // 停止監控
    UFUNCTION(BlueprintCallable, Category = "Asset Performance Monitor")
    void StopMonitoring(const FString& AssetID);

    // 獲取性能數據
    UFUNCTION(BlueprintCallable, Category = "Asset Performance Monitor")
    FMingAssetPerformanceData GetPerformanceData(const FString& AssetID) const;

    // 生成性能報告
    UFUNCTION(BlueprintCallable, Category = "Asset Performance Monitor")
    FString GeneratePerformanceReport() const;

    // 檢查性能問題
    UFUNCTION(BlueprintCallable, Category = "Asset Performance Monitor")
    TArray<FMingPerformanceIssue> CheckPerformanceIssues();

private:
    void UpdatePerformanceData();
    void AnalyzePerformanceTrends();
};
```

---

## 版本控制

### 資產版本管理

#### 版本控制系統
```cpp
// 資產版本控制系統
UCLASS()
class MINGASSETS_API UMingAssetVersionControl : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, FMingAssetVersionInfo> AssetVersions;
    FString CurrentVersion;

public:
    // 創建新版本
    UFUNCTION(BlueprintCallable, Category = "Asset Version Control")
    FString CreateNewVersion(const FString& AssetID, const FString& Description);

    // 獲取資產版本
    UFUNCTION(BlueprintCallable, Category = "Asset Version Control")
    FMingAssetVersionInfo GetAssetVersion(const FString& AssetID, const FString& Version = "") const;

    // 回滾到指定版本
    UFUNCTION(BlueprintCallable, Category = "Asset Version Control")
    bool RollbackToVersion(const FString& AssetID, const FString& TargetVersion);

    // 比較版本差異
    UFUNCTION(BlueprintCallable, Category = "Asset Version Control")
    TArray<FMingAssetVersionDiff> CompareVersions(const FString& AssetID, const FString& Version1, const FString& Version2);

    // 獲取版本歷史
    UFUNCTION(BlueprintCallable, Category = "Asset Version Control")
    TArray<FMingAssetVersionInfo> GetVersionHistory(const FString& AssetID) const;

    // 合併版本
    UFUNCTION(BlueprintCallable, Category = "Asset Version Control")
    bool MergeVersions(const FString& AssetID, const FString& SourceVersion, const FString& TargetVersion);
};
```

#### 資產同步系統
```cpp
// 資產同步系統
UCLASS()
class MINGASSETS_API UMingAssetSyncSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, FMingAssetSyncStatus> SyncStatus;
    FString ServerURL;

public:
    // 同步資產到服務器
    UFUNCTION(BlueprintCallable, Category = "Asset Sync System")
    void SyncAssetToServer(const FString& AssetID, const FMingOnAssetSynced& OnSynced);

    // 從服務器同步資產
    UFUNCTION(BlueprintCallable, Category = "Asset Sync System")
    void SyncAssetFromServer(const FString& AssetID, const FMingOnAssetSynced& OnSynced);

    // 批量同步
    UFUNCTION(BlueprintCallable, Category = "Asset Sync System")
    void BatchSyncAssets(const TArray<FString>& AssetIDs, const FMingOnBatchSynced& OnSynced);

    // 檢查同步狀態
    UFUNCTION(BlueprintCallable, Category = "Asset Sync System")
    FMingAssetSyncStatus GetSyncStatus(const FString& AssetID) const;

    // 解決同步衝突
    UFUNCTION(BlueprintCallable, Category = "Asset Sync System")
    bool ResolveSyncConflict(const FString& AssetID, EMingConflictResolution Resolution);

    // 設置同步服務器
    UFUNCTION(BlueprintCallable, Category = "Asset Sync System")
    void SetSyncServer(const FString& ServerURL);
};
```

---

## 總結

MingGoRTS 的資產系統是一個完整、高效、可擴展的資產管理框架，涵蓋了遊戲開發所需的所有資產類型。通過標準化的製作規範、智能化的管理系統和自動化的優化策略，確保了資產的高質量和高效使用。

### 主要特點

1. **完整的資產分類** - 涵蓋 3D 模型、紋理、動畫、音頻、UI、特效等所有類型
2. **標準化製作規範** - 統一的命名規範、製作流程和質量標準
3. **智能化管理系統** - 自動化的資產加載、版本控制和性能監控
4. **AI 生成支持** - 集成 AI 技術的自動化資產生成
5. **性能優化** - LOD 系統、紋理優化、動畫壓縮等優化策略
6. **版本控制** - 完整的版本管理和同步系統

### 未來發展

資產系統將繼續發展和改進：
- 更先進的 AI 資產生成技術
- 雲端資產管理和協作
- 實時資產優化和調整
- 跨平台資產兼容性
- 自動化質量控制和測試

這個資產系統為 MingGoRTS 的開發提供了堅實的基礎，確保了遊戲資產的高質量和高效管理，同時為未來的功能擴展提供了靈活的支持。

---

*本文檔最後更新：2026年3月23日*
