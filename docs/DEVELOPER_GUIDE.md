# MingGoRTS 開發者指南 / MingGoRTS Developer Guide

## 🚀 開發環境設置 / Development Environment Setup

### 系統要求 / System Requirements

#### 最低配置 / Minimum Requirements
- **操作系統 / Operating System**: Windows 10 64-bit 或更高 / Windows 10 64-bit or higher
- **處理器 / Processor**: Intel Core i5-8400 或 AMD Ryzen 5 2600 / Intel Core i5-8400 or AMD Ryzen 5 2600
- **內存 / Memory**: 16GB RAM
- **顯卡 / Graphics**: NVIDIA GTX 1060 或 AMD RX 580 / NVIDIA GTX 1060 or AMD RX 580
- **存儲 / Storage**: 100GB 可用空間 / 100GB available space
- **網絡 / Network**: 寬帶互聯網連接 / Broadband internet connection

#### 推薦配置 / Recommended Requirements
- **操作系統 / Operating System**: Windows 11 64-bit / Windows 11 64-bit
- **處理器 / Processor**: Intel Core i7-12700K 或 AMD Ryzen 7 5800X / Intel Core i7-12700K or AMD Ryzen 7 5800X
- **內存 / Memory**: 32GB RAM
- **顯卡 / Graphics**: NVIDIA RTX 3070 或 AMD RX 6700 XT / NVIDIA RTX 3070 or AMD RX 6700 XT
- **存儲 / Storage**: 200GB SSD 可用空間 / 200GB SSD available space
- **網絡 / Network**: 高速寬帶連接 / High-speed broadband connection

### 軟件安裝 / Software Installation

#### Unreal Engine 5.4+ 安裝
```bash
# 1. 下載 Epic Games Launcher
# Download Epic Games Launcher
https://www.epicgames.com/store/download

# 2. 安裝 Unreal Engine 5.4
# Install Unreal Engine 5.4
- 打開 Epic Games Launcher / Open Epic Games Launcher
- 進入 Unreal Engine 標籤 / Go to Unreal Engine tab
- 安裝 UE 5.4 或更高版本 / Install UE 5.4 or higher

# 3. 安裝必要組件
# Install necessary components
- Visual Studio 2022 (C++ 開發工具 / C++ development tools)
- Windows 10 SDK
- .NET Framework 4.7.2 或更高 / .NET Framework 4.7.2 or higher
```

#### 項目克隆與設置 / Project Clone and Setup
```bash
# 1. 克隆項目倉庫
# Clone project repository
git clone https://github.com/minggorts/MingGoRTS.git
cd MingGoRTS

# 2. 創建開發分支
# Create development branch
git checkout -b development

# 3. 生成項目文件
# Generate project files
# 在 UE5 中打開項目 / Open project in UE5
# 或使用命令行 / Or use command line
"C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="MingGoRTS.uproject" -game -rocket -progress

# 4. 編譯項目
# Compile project
# 在 UE5 編輯器中 / In UE5 editor
# 或使用 Visual Studio / Or use Visual Studio
```

---

## 🏗️ 項目架構 / Project Architecture

### 插件結構 / Plugin Structure

```
MingGoRTS/
├── Source/                          # 源代碼 / Source Code
│   ├── MingGoRTS/                   # 主項目 / Main Project
│   ├── MingCore/                    # 核心系統插件 / Core Systems Plugin
│   ├── MingPersonal/                # 個人系統插件 / Personal Systems Plugin
│   ├── MingTactical/                # 戰術系統插件 / Tactical Systems Plugin
│   ├── MingStrategic/               # 策略系統插件 / Strategic Systems Plugin
│   ├── MingBuilding/                # 建築系統插件 / Building Systems Plugin
│   ├── MingAudio/                   # 音頻系統插件 / Audio Systems Plugin
│   └── MingAchievements/            # 成就系統插件 / Achievement Systems Plugin
├── Content/                         # 遊戲資源 / Game Assets
│   ├── Blueprints/                  # 藍圖 / Blueprints
│   ├── Materials/                   # 材質 / Materials
│   ├── Meshes/                      # 網格 / Meshes
│   ├── Textures/                    # 貼圖 / Textures
│   ├── Audio/                       # 音頻 / Audio
│   └── Maps/                        # 地圖 / Maps
├── Config/                          # 配置文件 / Configuration Files
├── Plugins/                         # 插件 / Plugins
└── Tools/                           # 工具 / Tools
```

### 核心系統 / Core Systems

#### MingCore 插件 / MingCore Plugin
```cpp
// MingCore.Build.cs
PublicDependencyModuleNames.AddRange(new string[] 
{
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "EnhancedInput",
    "GameplayTags",
    "NavigationSystem",
    "AIModule",
    "UMG",
    "Slate",
    "SlateCore"
});
```

**主要功能 / Main Functions**:
- RTS 戰鬥系統 / RTS Combat System
- 單位管理 / Unit Management
- 路徑規劃 / Path Planning
- 基礎 AI / Basic AI
- 經濟系統 / Economic System
- 建築系統 / Building System

#### MingPersonal 插件 / MingPersonal Plugin
```cpp
// MingPersonal.Build.cs
PublicDependencyModuleNames.AddRange(new string[] 
{
    "Core",
    "CoreUObject",
    "Engine",
    "MingCore",
    "GameplayTags",
    "UMG",
    "Slate"
});
```

**主要功能 / Main Functions**:
- 角色關係系統 / Character Relationship System
- 聲望系統 / Reputation System
- 音頻關係管理 / Audio Relationship Management
- UI 系統 / UI System
- 任務系統 / Quest System

---

## 💻 編碼規範 / Coding Standards

### C++ 編碼規範 / C++ Coding Standards

#### 命名約定 / Naming Conventions

##### 類命名 / Class Naming
```cpp
// 類名使用 PascalCase，以適當前綴開始
// Class names use PascalCase, start with appropriate prefix
class MINGCORE_API UMingRTSUnitSelector : public UObject
{
    GENERATED_BODY()
};

class MINGPERSONAL_API UMingRelationshipManager : public UObject
{
    GENERATED_BODY()
};
```

##### 函數命名 / Function Naming
```cpp
// 函數名使用 PascalCase
// Function names use PascalCase
UFUNCTION(BlueprintCallable, Category = "Unit Selection")
void SelectUnits(const TArray<AActor*>& Units, ESelectionType SelectionType);

UFUNCTION(BlueprintPure, Category = "Unit Selection")
TArray<AActor*> GetSelectedUnits() const;
```

##### 變量命名 / Variable Naming
```cpp
// 成員變量使用 CamelCase，以適當類型前綴開始
// Member variables use CamelCase, start with appropriate type prefix
UPROPERTY()
TArray<AActor*> SelectedUnits;

UPROPERTY()
float CurrentHealth;

UPROPERTY()
bool bIsSelected;

// 局部變量使用 camelCase
// Local variables use camelCase
float currentHealth = 100.0f;
bool isSelected = false;
```

##### 枚舉命名 / Enum Naming
```cpp
// 枚舉名使用 PascalCase，以 E 前綴開始
// Enum names use PascalCase, start with E prefix
UENUM(BlueprintType)
enum class ESelectionType : uint8
{
    Single,
    Multiple,
    Box,
    Group
};

// 枚舉值使用 PascalCase
// Enum values use PascalCase
UENUM(BlueprintType)
enum class EUnitState : uint8
{
    Idle,
    Moving,
    Attacking,
    Defending
};
```

#### 代碼組織 / Code Organization

##### 頭文件組織 / Header File Organization
```cpp
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Containers/Array.h"
#include "MingRTSUnitSelector.generated.h"

// 前向聲明 / Forward declarations
class AActor;
class UMeshComponent;
class AController;
```

##### 實現文件組織 / Implementation File Organization
```cpp
#include "MingRTSUnitSelector.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"

// 構造函數 / Constructor
UMingRTSUnitSelector::UMingRTSUnitSelector()
{
    // 初始化代碼 / Initialization code
}

// 公共函數 / Public functions
void UMingRTSUnitSelector::SelectUnits(const TArray<AActor*>& Units, ESelectionType SelectionType)
{
    // 實現代碼 / Implementation code
}

// 私有函數 / Private functions
void UMingRTSUnitSelector::UpdateSelectionVisuals()
{
    // 實現代碼 / Implementation code
}
```

### Blueprint 編碼規範 / Blueprint Coding Standards

#### 命名約定 / Naming Conventions
- **變量 / Variables**: 使用描述性名稱，以類型前綴開始 / Use descriptive names, start with type prefix
  - `bIsSelected` (布爾值 / Boolean)
  - `CurrentHealth` (數值 / Number)
  - `SelectedUnits` (數組 / Array)
  - `PlayerController` (對象引用 / Object reference)

- **函數 / Functions**: 使用動詞開頭的描述性名稱 / Use descriptive names starting with verbs
  - `SelectUnits`
  - `UpdateHealth`
  - `CheckVictoryCondition`

- **事件 / Events**: 使用 On 前綴 / Use On prefix
  - `OnUnitSelected`
  - `OnHealthChanged`
  - `OnGameStarted`

#### 結構組織 / Structure Organization
```cpp
// 函數節點組織 / Function node organization
1. 輸入驗證 / Input validation
2. 數據處理 / Data processing
3. 條件邏輯 / Conditional logic
4. 執行操作 / Execute operations
5. 輸出結果 / Output results

// 變量組織 / Variable organization
1. 輸入參數 / Input parameters
2. 局部變量 / Local variables
3. 輸出變量 / Output variables
```

---

## 🔧 開發工具 / Development Tools

### 編譯工具 / Build Tools

#### Unreal Build Tool (UBT)
```bash
# 編譯項目 / Compile project
UnrealBuildTool.exe MingGoRTSEditor Win64 Development Project="MingGoRTS.uproject"

# 編譯插件 / Compile plugin
UnrealBuildTool.exe MingCoreEditor Win64 Development Project="MingGoRTS.uproject"

# 清理編譯 / Clean build
UnrealBuildTool.exe MingGoRTSEditor Win64 Development Project="MingGoRTS.uproject" -clean
```

#### Visual Studio 設置 / Visual Studio Setup
```xml
<!-- MingGoRTS.vcxproj -->
<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='DevelopmentEditor|x64'">
    <OutDir>$(ProjectDir)Binaries\Win64\</OutDir>
    <IntDir>$(ProjectDir)Intermediate\ProjectFiles\$(ProjectName).dir\Development\x64\</IntDir>
    <IncludePath>$(ProjectDir)Source;$(UEEngineDir)Source;$(IncludePath)</IncludePath>
</PropertyGroup>
```

### 調試工具 / Debugging Tools

#### 日誌系統 / Logging System
```cpp
// 日誌宏 / Logging macros
UE_LOG(LogTemp, Log, TEXT("This is a log message"));
UE_LOG(LogTemp, Warning, TEXT("This is a warning message"));
UE_LOG(LogTemp, Error, TEXT("This is an error message"));

// 自定義日誌分類 / Custom log category
DEFINE_LOG_CATEGORY_STATIC(LogMingRTS, Log, All);

UE_LOG(LogMingRTS, Log, TEXT("MingGoRTS specific log"));
UE_LOG(LogMingRTS, Warning, TEXT("MingGoRTS warning"));
UE_LOG(LogMingRTS, Error, TEXT("MingGoRTS error"));
```

#### 調試配置 / Debug Configuration
```cpp
// 調試宏 / Debug macros
#if WITH_EDITOR
    UE_LOG(LogTemp, Log, TEXT("Editor mode debug info"));
#endif

#if UE_BUILD_DEBUG
    UE_LOG(LogTemp, Log, TEXT("Debug build info"));
#endif

#if UE_BUILD_DEVELOPMENT
    UE_LOG(LogTemp, Log, TEXT("Development build info"));
#endif

// 條件編譯調試 / Conditional compilation debug
#define MING_DEBUG 1

#if MING_DEBUG
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS debug info"));
#endif
```

### 測試工具 / Testing Tools

#### 自動化測試 / Automated Testing
```cpp
// 測試類 / Test class
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingRTSUnitTest, "MingGoRTS.UnitTest", 
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMingRTSUnitTest::RunTest(const FString& Parameters)
{
    // 測試邏輯 / Test logic
    TestTrue(TEXT("Unit selection should work"), TestUnitSelection());
    TestTrue(TEXT("Pathfinding should work"), TestPathfinding());
    
    return true;
}
```

#### 性能分析 / Performance Profiling
```cpp
// 性能分析宏 / Performance profiling macros
DECLARE_CYCLE_STAT(TEXT("MingRTS_Update"), STAT_MingRTS_Update, STATGROUP_Game);

void UMingRTSUnitManager::Tick(float DeltaTime)
{
    SCOPE_CYCLE_COUNTER(STAT_MingRTS_Update);
    
    // 更新邏輯 / Update logic
    UpdateUnits(DeltaTime);
    UpdateSelection(DeltaTime);
}

// 內存分析 / Memory analysis
#if WITH_EDITOR
    void UMingRTSUnitManager::PostLoad()
    {
        Super::PostLoad();
        
        // 內存使用統計 / Memory usage statistics
        int32 MemoryUsage = sizeof(*this);
        UE_LOG(LogTemp, Log, TEXT("MingRTSUnitManager memory usage: %d bytes"), MemoryUsage);
    }
#endif
```

---

## 🎮 系統開發 / System Development

### RTS 戰鬥系統開發 / RTS Combat System Development

#### 單位選擇器開發 / Unit Selector Development
```cpp
// MingRTSUnitSelector.h
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSUnitSelector : public UObject
{
    GENERATED_BODY()

public:
    // 選擇單位 / Select units
    UFUNCTION(BlueprintCallable, Category = "Unit Selection")
    void SelectUnits(const TArray<AActor*>& Units, ESelectionType SelectionType);
    
    // 獲取選中單位 / Get selected units
    UFUNCTION(BlueprintPure, Category = "Unit Selection")
    TArray<AActor*> GetSelectedUnits() const { return SelectedUnits; }
    
    // 清除選擇 / Clear selection
    UFUNCTION(BlueprintCallable, Category = "Unit Selection")
    void ClearSelection();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    TArray<AActor*> SelectedUnits;
    
    UPROPERTY()
    ESelectionType CurrentSelectionType;
    
    // 選擇邏輯 / Selection logic
    void ProcessSingleSelection(AActor* Unit);
    void ProcessMultipleSelection(const TArray<AActor*>& Units);
    void ProcessBoxSelection(const FVector& BoxStart, const FVector& BoxEnd);
    
    // 視覺反饋 / Visual feedback
    void UpdateSelectionVisuals();
    void HighlightSelectedUnits();
    void ClearSelectionHighlights();
};
```

```cpp
// MingRTSUnitSelector.cpp
void UMingRTSUnitSelector::SelectUnits(const TArray<AActor*>& Units, ESelectionType SelectionType)
{
    CurrentSelectionType = SelectionType;
    
    switch (SelectionType)
    {
    case ESelectionType::Single:
        if (Units.Num() > 0)
        {
            ProcessSingleSelection(Units[0]);
        }
        break;
        
    case ESelectionType::Multiple:
        ProcessMultipleSelection(Units);
        break;
        
    case ESelectionType::Box:
        // 框選邏輯在另一個函數中處理 / Box selection logic handled in another function
        break;
        
    case ESelectionType::Group:
        ProcessGroupSelection(Units);
        break;
    }
    
    UpdateSelectionVisuals();
}

void UMingRTSUnitSelector::ProcessSingleSelection(AActor* Unit)
{
    ClearSelection();
    SelectedUnits.Add(Unit);
    
    // 觸發選擇事件 / Trigger selection event
    OnUnitSelected.Broadcast(Unit);
}

void UMingRTSUnitSelector::UpdateSelectionVisuals()
{
    // 清除之前的高亮 / Clear previous highlights
    ClearSelectionHighlights();
    
    // 高亮選中的單位 / Highlight selected units
    HighlightSelectedUnits();
}
```

#### 路徑規劃器開發 / Path Planner Development
```cpp
// MingRTSPathfinder.h
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSPathfinder : public UObject
{
    GENERATED_BODY()

public:
    // 尋找路徑 / Find path
    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    TArray<FVector> FindPath(const FVector& Start, const FVector& End, EPathAlgorithm Algorithm = EPathAlgorithm::AStar);
    
    // 設置網格 / Set grid
    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    void SetNavigationGrid(const FNavigationGrid& Grid);
    
    // 更新障礙物 / Update obstacles
    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    void UpdateObstacles(const TArray<FVector>& ObstaclePositions);

private:
    UPROPERTY()
    FNavigationGrid NavigationGrid;
    
    // A* 算法實現 / A* algorithm implementation
    TArray<FVector> FindPathAStar(const FVector& Start, const FVector& End);
    
    // Dijkstra 算法實現 / Dijkstra algorithm implementation
    TArray<FVector> FindPathDijkstra(const FVector& Start, const FVector& End);
    
    // 路徑優化 / Path optimization
    TArray<FVector> OptimizePath(const TArray<FVector>& Path);
    
    // 障礙物檢測 / Obstacle detection
    bool IsObstacle(const FVector& Position);
    bool IsValidPosition(const FVector& Position);
};
```

### 經濟系統開發 / Economic System Development

#### 資源管理器開發 / Resource Manager Development
```cpp
// MingRTSResourceManager.h
USTRUCT(BlueprintType)
struct FResourceData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EResourceType ResourceType;
    
    UPROPERTY(BlueprintReadWrite)
    float Amount;
    
    UPROPERTY(BlueprintReadWrite)
    float ProductionRate;
    
    UPROPERTY(BlueprintReadWrite)
    float ConsumptionRate;

    FResourceData()
    {
        ResourceType = EResourceType::Gold;
        Amount = 0.0f;
        ProductionRate = 0.0f;
        ConsumptionRate = 0.0f;
    }
};

UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSResourceManager : public UObject
{
    GENERATED_BODY()

public:
    // 添加資源 / Add resource
    UFUNCTION(BlueprintCallable, Category = "Resource Management")
    void AddResource(EResourceType ResourceType, float Amount);
    
    // 消耗資源 / Consume resource
    UFUNCTION(BlueprintCallable, Category = "Resource Management")
    bool ConsumeResource(EResourceType ResourceType, float Amount);
    
    // 獲取資源數量 / Get resource amount
    UFUNCTION(BlueprintPure, Category = "Resource Management")
    float GetResourceAmount(EResourceType ResourceType) const;
    
    // 設置生產率 / Set production rate
    UFUNCTION(BlueprintCallable, Category = "Resource Management")
    void SetProductionRate(EResourceType ResourceType, float Rate);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    TMap<EResourceType, FResourceData> Resources;
    
    // 資源更新邏輯 / Resource update logic
    void UpdateResourceProduction(float DeltaTime);
    void UpdateResourceConsumption(float DeltaTime);
    
    // 資源平衡 / Resource balance
    void CheckResourceBalance();
    void HandleResourceShortage(EResourceType ResourceType);
};
```

### AI 系統開發 / AI System Development

#### AI 控制器開發 / AI Controller Development
```cpp
// MingRTSAIController.h
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API AMingRTSAIController : public AAIController
{
    GENERATED_BODY()

public:
    // 設置 AI 類型 / Set AI type
    UFUNCTION(BlueprintCallable, Category = "AI")
    void SetAIType(EAIType NewAIType);
    
    // 執行 AI 決策 / Execute AI decision
    UFUNCTION(BlueprintCallable, Category = "AI")
    void ExecuteAIDecision();
    
    // 評估威脅 / Assess threat
    UFUNCTION(BlueprintPure, Category = "AI")
    float AssessThreat(AActor* Target) const;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void OnPossess(APawn* InPawn) override;

private:
    UPROPERTY()
    EAIType AIType;
    
    UPROPERTY()
    EAIState CurrentAIState;
    
    UPROPERTY()
    TArray<AActor*> KnownEnemies;
    
    UPROPERTY()
    TArray<AActor*> KnownAllies;
    
    // AI 決策系統 / AI decision system
    void MakeStrategicDecision();
    void MakeTacticalDecision();
    void MakeEconomicDecision();
    
    // 威脅評估系統 / Threat assessment system
    void UpdateThreatAssessment();
    float CalculateThreatLevel(AActor* Actor) const;
    
    // 行為執行系統 / Behavior execution system
    void ExecuteCombatBehavior();
    void ExecuteDefensiveBehavior();
    void ExecuteEconomicBehavior();
};
```

---

## 🎨 內容開發 / Content Development

### 藍圖開發 / Blueprint Development

#### 藍圖組織 / Blueprint Organization
```
Content/Blueprints/
├── Characters/          # 角色藍圖 / Character Blueprints
├── Units/              # 單位藍圖 / Unit Blueprints
├── Buildings/          # 建築藍圖 / Building Blueprints
├── UI/                 # UI 藍圖 / UI Blueprints
├── GameModes/          # 遊戲模式藍圖 / Game Mode Blueprints
└── Utilities/          # 工具藍圖 / Utility Blueprints
```

#### 藍圖最佳實踐 / Blueprint Best Practices
```cpp
// 藍圖函數組織 / Blueprint function organization
1. 輸入驗證 / Input validation
2. 數據處理 / Data processing
3. 條件邏輯 / Conditional logic
4. 執行操作 / Execute operations
5. 輸出結果 / Output results

// 變量命名 / Variable naming
- 使用描述性名稱 / Use descriptive names
- 包含類型前綴 / Include type prefixes
- 避免縮寫 / Avoid abbreviations

// 注釋規範 / Comment standards
- 函數說明 / Function description
- 複雜邏輯解釋 / Complex logic explanation
- 性能注意事項 / Performance considerations
```

### 資產創建 / Asset Creation

#### 3D 模型創建 / 3D Model Creation
```cpp
// 模型導入規範 / Model import standards
- 文件格式 / File format: FBX, OBJ
- 多邊形數量 / Polygon count: 根據用途調整 / Adjust based on usage
- 材質命名 / Material naming: 使用描述性名稱 / Use descriptive names
- 碰撞體 / Collision: 適當設置碰撞體 / Set appropriate collision

// 材質創建 / Material creation
- 基礎材質 / Base material: M_BaseMaterial
- 實例材質 / Instance material: 根據對象命名 / Name based on object
- 參數設置 / Parameter settings: 一致性設置 / Consistent settings
```

#### 音頻資產創建 / Audio Asset Creation
```cpp
// 音頻文件規範 / Audio file standards
- 文件格式 / File format: WAV, OGG
- 采樣率 / Sample rate: 44.1kHz
- 位深度 / Bit depth: 16-bit
- 聲道 / Channels: 單聲道或立體聲 / Mono or stereo

// 音頻分類 / Audio classification
- 音樂 / Music: 背景音樂 / Background music
- 音效 / Sound effects: 遊戲音效 / Game sound effects
- 語音 / Voice: 角色語音 / Character voice
- 環境音 / Ambient: 環境音效 / Environmental sounds
```

---

## 🔍 測試與調試 / Testing and Debugging

### 單元測試 / Unit Testing
```cpp
// 測試類示例 / Test class example
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingRTSUnitSelectorTest, "MingGoRTS.UnitSelector", 
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMingRTSUnitSelectorTest::RunTest(const FString& Parameters)
{
    // 創建測試對象 / Create test object
    UMingRTSUnitSelector* UnitSelector = NewObject<UMingRTSUnitSelector>();
    
    // 測試單位選擇 / Test unit selection
    TArray<AActor*> TestUnits;
    TestUnits.Add(CreateTestUnit());
    TestUnits.Add(CreateTestUnit());
    
    UnitSelector->SelectUnits(TestUnits, ESelectionType::Multiple);
    
    // 驗證結果 / Verify results
    TArray<AActor*> SelectedUnits = UnitSelector->GetSelectedUnits();
    TestEqual(TEXT("Should select 2 units"), SelectedUnits.Num(), 2);
    
    return true;
}
```

### 集成測試 / Integration Testing
```cpp
// 集成測試示例 / Integration test example
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMingRTSIntegrationTest, "MingGoRTS.Integration", 
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMingRTSIntegrationTest::RunTest(const FString& Parameters)
{
    // 創建測試環境 / Create test environment
    UWorld* World = GetTestWorld();
    
    // 測試系統集成 / Test system integration
    UMingRTSUnitManager* UnitManager = World->GetSubsystem<UMingRTSUnitManager>();
    UMingRTSResourceManager* ResourceManager = World->GetSubsystem<UMingRTSResourceManager>();
    
    // 創建測試單位 / Create test units
    AActor* TestUnit = CreateTestUnit(World);
    
    // 測試單位與資源管理器交互 / Test unit and resource manager interaction
    bool bCanTrain = ResourceManager->ConsumeResource(EResourceType::Gold, 100.0f);
    TestTrue(TEXT("Should be able to train unit"), bCanTrain);
    
    return true;
}
```

### 性能測試 / Performance Testing
```cpp
// 性能測試示例 / Performance test example
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMingRTSPerformanceTest, "MingGoRTS.Performance", 
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMingRTSPerformanceTest::RunTest(const FString& Parameters)
{
    // 性能測試設置 / Performance test setup
    const int32 TestUnitCount = 1000;
    const float TestDuration = 60.0f; // 60 seconds
    
    // 創建大量單位 / Create many units
    TArray<AActor*> TestUnits;
    for (int32 i = 0; i < TestUnitCount; ++i)
    {
        AActor* Unit = CreateTestUnit();
        TestUnits.Add(Unit);
    }
    
    // 性能監控 / Performance monitoring
    double StartTime = FPlatformTime::Seconds();
    float InitialFPS = GEngine->GetAverageFPS();
    
    // 運行測試 / Run test
    for (float ElapsedTime = 0.0f; ElapsedTime < TestDuration; ElapsedTime += GetWorld()->GetDeltaSeconds())
    {
        // 模擬遊戲邏輯 / Simulate game logic
        UpdateTestUnits(TestUnits);
        
        // 檢查性能 / Check performance
        float CurrentFPS = GEngine->GetAverageFPS();
        if (CurrentFPS < 30.0f) // FPS below 30 is considered poor performance
        {
            UE_LOG(LogTemp, Warning, TEXT("Performance test failed: FPS dropped to %f"), CurrentFPS);
            return false;
        }
    }
    
    double EndTime = FPlatformTime::Seconds();
    float AverageFPS = GEngine->GetAverageFPS();
    
    // 驗證性能 / Verify performance
    TestTrue(TEXT("Average FPS should be above 30"), AverageFPS >= 30.0f);
    TestTrue(TEXT("Test should complete within reasonable time"), (EndTime - StartTime) < TestDuration * 1.5f);
    
    return true;
}
```

---

## 🚀 部署與發布 / Deployment and Release

### 構建配置 / Build Configuration

#### 開發構建 / Development Build
```xml
<!-- MingGoRTS.Build.cs -->
PublicDependencyModuleNames.AddRange(new string[] 
{
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "EnhancedInput",
    "GameplayTags",
    "NavigationSystem",
    "AIModule",
    "UMG",
    "Slate",
    "SlateCore",
    "Json",
    "JsonUtilities"
});

PrivateDependencyModuleNames.AddRange(new string[] 
{
    "Slate",
    "SlateCore"
});
```

#### 發布構建 / Shipping Build
```xml
<!-- MingGoRTS.Build.cs -->
// 發布構建移除調試模塊 / Remove debug modules for shipping build
#if UE_BUILD_SHIPPING
    // 發布版本特定配置 / Shipping version specific configuration
#endif
```

### 打包設置 / Packaging Settings

#### 項目設置 / Project Settings
```cpp
// DefaultEngine.ini
[/Script/EngineSettings.GameMapsSettings]
GameDefaultMap=/Game/Maps/MainMenu
GameInstanceClass=/Game/Blueprints/GameInstances/MingGoRTSGameInstance.MingGoRTSGameInstance_C

[/Script/Engine.Engine]
bUseVSync=False
bUseDynamicResolution=False
FrameRateLimit=60.000000
```

#### 平台特定設置 / Platform Specific Settings
```cpp
// Android 設置 / Android settings
[/Script/AndroidRuntimeSettings.AndroidRuntimeSettings]
bPackageDataInsideApk=false
bUseOpenGL_ES3=true
bSupportVulkan=true
TextureFormatName=ASTC

// iOS 設置 / iOS settings
[/Script/IOSRuntimeSettings.IOSRuntimeSettings]
bSupportsMetal=true
bUseMetalShaderCompiler=true
MinimumIOSVersion=12.0
```

### 版本管理 / Version Management

#### 版本號設置 / Version Numbering
```cpp
// MingGoRTS.Build.cs
// 版本格式：Major.Minor.Patch
// Version format: Major.Minor.Patch
#define MING_GORTS_VERSION_MAJOR 1
#define MING_GORTS_VERSION_MINOR 0
#define MING_GORTS_VERSION_PATCH 0

#define MING_GORTS_VERSION_STRING TEXT("1.0.0")
```

#### 熱更新設置 / Hot Update Settings
```cpp
// 熱更新配置 / Hot update configuration
struct FMingGoRTSHotUpdateConfig
{
    FString ServerURL;
    FString VersionCheckURL;
    FString AssetBundleURL;
    bool bEnableHotUpdate;
    float UpdateCheckInterval;
};
```

---

## 📚 文檔與資源 / Documentation and Resources

### API 文檔生成 / API Documentation Generation
```cpp
// 使用 Doxygen 風格註釋 / Use Doxygen style comments
/**
 * @brief RTS 單位選擇器 / RTS Unit Selector
 * @details 提供多種單位選擇方式和智能過濾功能 / Provides various unit selection methods and intelligent filtering
 * @author MingGoRTS Team
 * @version 1.0
 * @date 2026-03-22
 */
class MINGCORE_API UMingRTSUnitSelector : public UObject
{
    /**
     * @brief 選擇單位 / Select units
     * @param Units 要選擇的單位數組 / Array of units to select
     * @param SelectionType 選擇類型 / Selection type
     * @return 是否成功選擇 / Whether selection was successful
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Selection")
    bool SelectUnits(const TArray<AActor*>& Units, ESelectionType SelectionType);
};
```

### 技術文檔 / Technical Documentation

#### 系統架構文檔 / System Architecture Documentation
```markdown
# 系統架構 / System Architecture

## 核心組件 / Core Components

### 1. 單位管理系統 / Unit Management System
- **功能 / Function**: 管理所有遊戲單位 / Manage all game units
- **接口 / Interface**: UMingRTSUnitManager
- **依賴 / Dependencies**: AI系統、戰鬥系統 / AI System, Combat System

### 2. 經濟系統 / Economic System
- **功能 / Function**: 管理資源生產和分配 / Manage resource production and allocation
- **接口 / Interface**: UMingRTSEconomicSystem
- **依賴 / Dependencies**: 建築系統、貿易系統 / Building System, Trade System
```

#### 性能優化指南 / Performance Optimization Guide
```markdown
# 性能優化指南 / Performance Optimization Guide

## 單位管理優化 / Unit Management Optimization

### 對象池 / Object Pooling
```cpp
// 使用對象池減少內存分配 / Use object pool to reduce memory allocation
class MINGCORE_API UMingUnitPool : public UObject
{
private:
    TArray<AActor*> AvailableUnits;
    TArray<AActor*> ActiveUnits;
    
public:
    AActor* GetUnit();
    void ReturnUnit(AActor* Unit);
};
```

### 批量處理 / Batch Processing
```cpp
// 批量更新單位狀態 / Batch update unit states
void UMingRTSUnitManager::BatchUpdateUnits(const TArray<AActor*>& Units, float DeltaTime)
{
    for (AActor* Unit : Units)
    {
        UpdateUnit(Unit, DeltaTime);
    }
}
```
```

---

## 🤝 社區與協作 / Community and Collaboration

### 貢獻指南 / Contribution Guidelines

#### 代碼貢獻 / Code Contribution
1. **Fork 項目 / Fork Project**: 在 GitHub 上 fork 項目 / Fork project on GitHub
2. **創建分支 / Create Branch**: 創建功能分支 / Create feature branch
3. **編寫代碼 / Write Code**: 遵循編碼規範 / Follow coding standards
4. **測試代碼 / Test Code**: 確保測試通過 / Ensure tests pass
5. **提交 Pull Request / Submit Pull Request**: 提交代碼審查 / Submit code review

#### 文檔貢獻 / Documentation Contribution
1. **識別缺口 / Identify Gaps**: 找到文檔缺失部分 / Find missing documentation
2. **編寫文檔 / Write Documentation**: 使用標準格式 / Use standard format
3. **審查文檔 / Review Documentation**: 確保準確性 / Ensure accuracy
4. **提交文檔 / Submit Documentation**: 通過適當渠道提交 / Submit through appropriate channels

### 問題報告 / Issue Reporting

#### Bug 報告 / Bug Report
```markdown
## Bug 標題 / Bug Title
簡短描述問題 / Brief description of issue

### 環境信息 / Environment Information
- 操作系統 / Operating System: Windows 11
- 遊戲版本 / Game Version: v1.0.0
- 硬件配置 / Hardware: Intel i7-12700K, RTX 3070

### 問題描述 / Problem Description
詳細描述問題現象 / Detailed description of issue

### 重現步驟 / Reproduction Steps
1. 步驟 1 / Step 1
2. 步驟 2 / Step 2
3. 步驟 3 / Step 3

### 期望行為 / Expected Behavior
描述應該發生的情況 / Describe what should happen

### 實際行為 / Actual Behavior
描述實際發生的情況 / Describe what actually happens

### 附加信息 / Additional Information
截圖、日誌、視頻等 / Screenshots, logs, videos, etc.
```

#### 功能請求 / Feature Request
```markdown
## 功能標題 / Feature Title
簡短描述功能需求 / Brief description of feature request

### 功能描述 / Feature Description
詳細描述功能需求 / Detailed description of feature request

### 使用場景 / Use Cases
描述功能的使用場景 / Describe use cases for feature

### 實現建議 / Implementation Suggestions
建議的實現方式 / Suggested implementation approach

### 優先級 / Priority
低 / 中 / 高 / Low / Medium / High
```

---

## 🔧 故障排除 / Troubleshooting

### 常見開發問題 / Common Development Issues

#### 編譯錯誤 / Compilation Errors
```cpp
// 常見錯誤 1: 缺少頭文件 / Common error 1: Missing header file
// 解決方案 / Solution: 添加必要的頭文件 / Add necessary header files
#include "CoreMinimal.h"
#include "Engine/Engine.h"

// 常見錯誤 2: 鏈接錯誤 / Common error 2: Linking error
// 解決方案 / Solution: 檢查 Build.cs 文件依賴 / Check Build.cs file dependencies
PublicDependencyModuleNames.AddRange(new string[] 
{
    "Core",
    "Engine",
    "UMG"
});
```

#### 運行時錯誤 / Runtime Errors
```cpp
// 常見錯誤 1: 空指針異常 / Common error 1: Null pointer exception
// 解決方案 / Solution: 添加空指針檢查 / Add null pointer checks
if (Unit != nullptr)
{
    Unit->Update();
}

// 常見錯誤 2: 數組越界 / Common error 2: Array out of bounds
// 解決方案 / Solution: 添加邊界檢查 / Add boundary checks
if (Index >= 0 && Index < Units.Num())
{
    return Units[Index];
}
```

### 性能問題 / Performance Issues

#### 內存泄漏 / Memory Leaks
```cpp
// 使用智能指針避免內存泄漏 / Use smart pointers to avoid memory leaks
TSharedPtr<AActor> UnitPtr = MakeShared<AActor>();

// 使用 UE 對象系統 / Use UE object system
UPROPERTY()
TObjectPtr<AActor> Unit;
```

#### 幀率問題 / Frame Rate Issues
```cpp
// 使用時間切片 / Use time slicing
void UMingRTSUnitManager::Tick(float DeltaTime)
{
    // 每幀只處理部分單位 / Process only some units per frame
    const int32 UnitsPerFrame = 100;
    static int32 CurrentIndex = 0;
    
    for (int32 i = 0; i < UnitsPerFrame && CurrentIndex < Units.Num(); ++i)
    {
        UpdateUnit(Units[CurrentIndex], DeltaTime);
        CurrentIndex = (CurrentIndex + 1) % Units.Num();
    }
}
```

---

## 📈 性能監控 / Performance Monitoring

### 性能指標 / Performance Metrics

#### 關鍵指標 / Key Metrics
- **幀率 / Frame Rate**: 目標 60 FPS / Target 60 FPS
- **內存使用 / Memory Usage**: 目標 < 4GB / Target < 4GB
- **加載時間 / Load Time**: 目標 < 30 秒 / Target < 30 seconds
- **網絡延遲 / Network Latency**: 目標 < 100ms / Target < 100ms

#### 監控工具 / Monitoring Tools
```cpp
// 性能監控類 / Performance monitoring class
class MINGCORE_API UMingPerformanceMonitor : public UObject
{
public:
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetFrameRate() const;
    
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetMemoryUsage() const;
    
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetNetworkLatency() const;
    
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartPerformanceCapture();
    
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopPerformanceCapture();
    
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void ExportPerformanceData(const FString& FilePath);
};
```

---

## 🎯 未來發展 / Future Development

### 技術路線圖 / Technical Roadmap

#### 短期目標 / Short-term Goals (3個月 / 3 months)
- [ ] 完善現有系統 / Improve existing systems
- [ ] 優化性能 / Optimize performance
- [ ] 修復已知問題 / Fix known issues
- [ ] 增加測試覆蓋 / Increase test coverage

#### 中期目標 / Medium-term Goals (6個月 / 6 months)
- [ ] 添加新功能 / Add new features
- [ ] 支持新平台 / Support new platforms
- [ ] 改進用戶界面 / Improve user interface
- [ ] 增強 AI 系統 / Enhance AI system

#### 長期目標 / Long-term Goals (12個月 / 12 months)
- [ ] 模組化重構 / Modular refactoring
- [ ] 雲端功能 / Cloud features
- [ ] VR/AR 支持 / VR/AR support
- [ ] 社區功能 / Community features

### 技術債務 / Technical Debt

#### 待解決問題 / Issues to Resolve
- [ ] 重構舊代碼 / Refactor old code
- [ ] 改進錯誤處理 / Improve error handling
- [ ] 優化數據結構 / Optimize data structures
- [ ] 統一編碼風格 / Unify coding style

---

## 📞 支持與聯繫 / Support and Contact

### 技術支持 / Technical Support
- **郵件 / Email**: dev@minggorts.com
- **GitHub**: [MingGoRTS Repository](https://github.com/minggorts)
- **Discord**: [MingGoRTS Discord](https://discord.gg/minggorts)
- **文檔 / Documentation**: [MingGoRTS Docs](https://docs.minggorts.com)

### 社區資源 / Community Resources
- **論壇 / Forum**: [MingGoRTS Forum](https://forum.minggorts.com)
- **Wiki**: [MingGoRTS Wiki](https://wiki.minggorts.com)
- **教程 / Tutorials**: [MingGoRTS Tutorials](https://tutorials.minggorts.com)
- **示例 / Examples**: [MingGoRTS Examples](https://examples.minggorts.com)

---

*最後更新 / Last Updated: 2026年3月22日*  
*版本 / Version: v1.0.0*  
*語言 / Language: 繁體中文 / English*
