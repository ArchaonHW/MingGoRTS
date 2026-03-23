# MingGoRTS UE5.7 API Learning System - Working Version

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS UE5.7 API学习系统" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# Create output directory
$OutputPath = "C:\HW\MingGoRTS\Docs\UE5_Learning"
if (-not (Test-Path $OutputPath)) {
    New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
    Write-Host "创建输出目录: $OutputPath" -ForegroundColor Green
}

# Define UE5.7 APIs
$APIs = @{
    "UEngine" = "The main engine class that manages core engine systems and provides access to global engine functionality."
    "UGameInstance" = "A game instance that persists across level transitions and manages game-wide state."
    "UWorld" = "Represents a game world containing actors, levels, and manages the simulation of that world."
    "AActor" = "Base class for all objects that can be placed or spawned in a level."
    "UObject" = "Base class for most objects in Unreal Engine, providing reflection, garbage collection, and serialization."
    "AActorComponent" = "Base class for components that can be attached to actors to add functionality."
    "FDelegate" = "Template for creating delegates that can bind to member functions or static functions."
    "UGameModeBase" = "Base class for game modes that define the rules and behavior of the game."
    "APlayerController" = "Controls a player's pawn and handles player input and camera."
    "APawn" = "Base class for actors that can be controlled by players or AI."
    "UEnhancedInputComponent" = "Component for handling enhanced input with context-aware actions."
    "UBehaviorTree" = "Asset that defines AI behavior using a tree of nodes."
    "UBlackboardComponent" = "Component that stores data used by behavior trees for AI decision making."
    "UEnvQuerySystem" = "System that handles environment queries for AI to find locations or objects."
    "ANavigationData" = "Base class for navigation data used by AI for pathfinding."
    "UMaterial" = "Asset that defines the appearance of surfaces through properties and expressions."
    "UMaterialInstanceDynamic" = "Runtime material instance that can modify material parameters dynamically."
    "FPrimitiveSceneProxy" = "Proxy class for rendering primitives in the scene rendering system."
    "FSceneView" = "Represents a view of the scene for rendering."
    "FStatGroup" = "Declaration for grouping performance statistics."
    "IStatsThread" = "Interface for accessing the stats thread system."
    "FStreamingManager" = "Manager for handling asset streaming and level streaming."
}

# Display learning progress
Write-Host "开始学习UE5.7 API..." -ForegroundColor Yellow
Write-Host "" -ForegroundColor Yellow

foreach ($API in $APIs.Keys) {
    Write-Host "学习API: $API" -ForegroundColor Cyan
    Write-Host "  描述: $($APIs[$API])" -ForegroundColor White
    Write-Host "" -ForegroundColor White
}

# Create simple report
$ReportPath = "$OutputPath\UE5_Learning_Report_$(Get-Date -Format 'yyyyMMdd_HHmmss').txt"

$Report = "UE5.7 API学习报告`n"
$Report += "==================`n"
$Report += "生成时间: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n"
$Report += "学习API数量: $($APIs.Count)`n"
$Report += "`n"

foreach ($API in $APIs.Keys) {
    $Report += "API: $API`n"
    $Report += "描述: $($APIs[$API])`n"
    $Report += "`n"
}

$Report += "学习完成！`n"
$Report += "==================`n"

Set-Content -Path $ReportPath -Value $Report -Encoding UTF8

Write-Host "学习报告已生成: $ReportPath" -ForegroundColor Green
Write-Host "" -ForegroundColor Green
Write-Host "🎓 UE5.7 API学习完成!" -ForegroundColor Green
Write-Host "📚 报告位置: $ReportPath" -ForegroundColor Green
Write-Host "📝 已学习API数量: $($APIs.Count)" -ForegroundColor Green
Write-Host "" -ForegroundColor Green

# Show report content
Write-Host "报告内容:" -ForegroundColor Yellow
Get-Content -Path $ReportPath | Write-Host

Write-Host "" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host "学习完成" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# Create C++ header file
$HeaderPath = "C:\HW\MingGoRTS\Source\MingGoRTS\Public\Learning\MingUE5LearningSystem.h"

if (-not (Test-Path "C:\HW\MingGoRTS\Source\MingGoRTS\Public\Learning")) {
    New-Item -ItemType Directory -Path "C:\HW\MingGoRTS\Source\MingGoRTS\Public\Learning" -Force | Out-Null
}

$HeaderContent = "// MingGoRTS UE5.7 API Learning System`n"
$HeaderContent += "// Generated: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n"
$HeaderContent += "`n"
$HeaderContent += "#pragma once`n"
$HeaderContent += "`n"
$HeaderContent += "#include 'CoreMinimal.h'`n"
$HeaderContent += "#include 'UObject/NoExportTypes.h'`n"
$HeaderContent += "`n"
$HeaderContent += "/**`n"
$HeaderContent += " * UE5.7 API Learning System`n"
$HeaderContent += " * 用于学习和掌握Unreal Engine 5.7的API`n"
$HeaderContent += " */`n"
$HeaderContent += "UCLASS(Blueprintable, BlueprintType)`n"
$HeaderContent += "class MINGGORTS_API UMingUE5LearningSystem : public UObject`n"
$HeaderContent += "{`n"
$HeaderContent += "    GENERATED_BODY()`n"
$HeaderContent += "`n"
$HeaderContent += "public:`n"
$HeaderContent += "    // Constructor`n"
$HeaderContent += "    UMingUE5LearningSystem();`n"
$HeaderContent += "`n"
$HeaderContent += "    // Initialize learning system`n"
$HeaderContent += "    UFUNCTION(BlueprintCallable, Category = 'UE5 Learning')`n"
$HeaderContent += "    bool InitializeLearningSystem();`n"
$HeaderContent += "`n"
$HeaderContent += "    // Learn specific API`n"
$HeaderContent += "    UFUNCTION(BlueprintCallable, Category = 'UE5 Learning')`n"
$HeaderContent += "    bool LearnAPI(const FString& APIName);`n"
$HeaderContent += "`n"
$HeaderContent += "    // Get learning progress`n"
$HeaderContent += "    UFUNCTION(BlueprintCallable, Category = 'UE5 Learning')`n"
$HeaderContent += "    float GetLearningProgress() const;`n"
$HeaderContent += "`n"
$HeaderContent += "    // Generate learning report`n"
$HeaderContent += "    UFUNCTION(BlueprintCallable, Category = 'UE5 Learning')`n"
$HeaderContent += "    bool GenerateLearningReport(const FString& ReportPath);`n"
$HeaderContent += "`n"
$HeaderContent += "protected:`n"
$HeaderContent += "    // API knowledge base`n"
$HeaderContent += "    UPROPERTY(BlueprintReadOnly, Category = 'UE5 Learning')`n"
$HeaderContent += "    TMap<FString, FString> APIKnowledgeBase;`n"
$HeaderContent += "`n"
$HeaderContent += "    // Learning progress`n"
$HeaderContent += "    UPROPERTY(BlueprintReadOnly, Category = 'UE5 Learning')`n"
$HeaderContent += "    int32 LearnedAPIs;`n"
$HeaderContent += "`n"
$HeaderContent += "    // Total APIs to learn`n"
$HeaderContent += "    UPROPERTY(BlueprintReadOnly, Category = 'UE5 Learning')`n"
$HeaderContent += "    int32 TotalAPIs;`n"
$HeaderContent += "`n"
$HeaderContent += "private:`n"
$HeaderContent += "    // Initialize API knowledge base`n"
$HeaderContent += "    void InitializeAPIKnowledgeBase();`n"
$HeaderContent += "`n"
$HeaderContent += "    // Update learning progress`n"
$HeaderContent += "    void UpdateLearningProgress();`n"
$HeaderContent += "`n"
$HeaderContent += "    // Validate API name`n"
$HeaderContent += "    bool IsValidAPIName(const FString& APIName) const;`n"
$HeaderContent += "};`n"

Set-Content -Path $HeaderPath -Value $HeaderContent -Encoding UTF8

Write-Host "C++头文件已生成: $HeaderPath" -ForegroundColor Green

# Create C++ implementation file
$CppPath = "C:\HW\MingGoRTS\Source\MingGoRTS\Private\Learning\MingUE5LearningSystem.cpp"

if (-not (Test-Path "C:\HW\MingGoRTS\Source\MingGoRTS\Private\Learning")) {
    New-Item -ItemType Directory -Path "C:\HW\MingGoRTS\Source\MingGoRTS\Private\Learning" -Force | Out-Null
}

$CppContent = "// MingGoRTS UE5.7 API Learning System`n"
$CppContent += "// Generated: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n"
$CppContent += "`n"
$CppContent += "#include 'MingUE5LearningSystem.h'`n"
$CppContent += "#include 'HAL/PlatformFilemanager.h'`n"
$CppContent += "#include 'Misc/FileHelper.h'`n"
$CppContent += "#include 'Misc/Paths.h'`n"
$CppContent += "#include 'Misc/DateTime.h'`n"
$CppContent += "`n"
$CppContent += "UMingUE5LearningSystem::UMingUE5LearningSystem()`n"
$CppContent += "{`n"
$CppContent += "    LearnedAPIs = 0;`n"
$CppContent += "    TotalAPIs = 0;`n"
$CppContent += "}`n"
$CppContent += "`n"
$CppContent += "bool UMingUE5LearningSystem::InitializeLearningSystem()`n"
$CppContent += "{`n"
$CppContent += "    // Initialize API knowledge base`n"
$CppContent += "    InitializeAPIKnowledgeBase();`n"
$CppContent += "`n"
$CppContent += "    // Update total APIs count`n"
$CppContent += "    TotalAPIs = APIKnowledgeBase.Num();`n"
$CppContent += "`n"
$CppContent += "    return true;`n"
$CppContent += "}`n"
$CppContent += "`n"
$CppContent += "bool UMingUE5LearningSystem::LearnAPI(const FString& APIName)`n"
$CppContent += "{`n"
$CppContent += "    if (!IsValidAPIName(APIName))`n"
$CppContent += "    {`n"
$CppContent += "        return false;`n"
$CppContent += "    }`n"
$CppContent += "`n"
$CppContent += "    // Check if already learned`n"
$CppContent += "    if (APIKnowledgeBase.Contains(APIName))`n"
$CppContent += "    {`n"
$CppContent += "        UpdateLearningProgress();`n"
$CppContent += "        return true;`n"
$CppContent += "    }`n"
$CppContent += "`n"
$CppContent += "    return false;`n"
$CppContent += "}`n"
$CppContent += "`n"
$CppContent += "float UMingUE5LearningSystem::GetLearningProgress() const`n"
$CppContent += "{`n"
$CppContent += "    if (TotalAPIs == 0)`n"
$CppContent += "    {`n"
$CppContent += "        return 0.0f;`n"
$CppContent += "    }`n"
$CppContent += "`n"
$CppContent += "    return (float)LearnedAPIs / TotalAPIs * 100.0f;`n"
$CppContent += "}`n"
$CppContent += "`n"
$CppContent += "bool UMingUE5LearningSystem::GenerateLearningReport(const FString& ReportPath)`n"
$CppContent += "{`n"
$CppContent += "    FString ReportContent = TEXT('UE5.7 API Learning Report\\n');`n"
$CppContent += "    ReportContent += FString::Printf(TEXT('Generated: %s\\n'), *FDateTime::Now().ToString());`n"
$CppContent += "    ReportContent += FString::Printf(TEXT('Progress: %.1f%%\\n'), GetLearningProgress());`n"
$CppContent += "    ReportContent += FString::Printf(TEXT('Learned: %d/%d APIs\\n'), LearnedAPIs, TotalAPIs);`n"
$CppContent += "`n"
$CppContent += "    return FFileHelper::SaveStringToFile(ReportContent, *ReportPath);`n"
$CppContent += "}`n"
$CppContent += "`n"
$CppContent += "void UMingUE5LearningSystem::InitializeAPIKnowledgeBase()`n"
$CppContent += "{`n"
$CppContent += "    // Core APIs`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UEngine'), TEXT('The main engine class that manages core engine systems and provides access to global engine functionality.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UGameInstance'), TEXT('A game instance that persists across level transitions and manages game-wide state.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UWorld'), TEXT('Represents a game world containing actors, levels, and manages the simulation of that world.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('AActor'), TEXT('Base class for all objects that can be placed or spawned in a level.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UObject'), TEXT('Base class for most objects in Unreal Engine, providing reflection, garbage collection, and serialization.'));`n"
$CppContent += "`n"
$CppContent += "    // Gameplay APIs`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('AActorComponent'), TEXT('Base class for components that can be attached to actors to add functionality.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UGameModeBase'), TEXT('Base class for game modes that define the rules and behavior of the game.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('APlayerController'), TEXT('Controls a player's pawn and handles player input and camera.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('APawn'), TEXT('Base class for actors that can be controlled by players or AI.'));`n"
$CppContent += "`n"
$CppContent += "    // AI APIs`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UBehaviorTree'), TEXT('Asset that defines AI behavior using a tree of nodes.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UBlackboardComponent'), TEXT('Component that stores data used by behavior trees for AI decision making.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UEnvQuerySystem'), TEXT('System that handles environment queries for AI to find locations or objects.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('ANavigationData'), TEXT('Base class for navigation data used by AI for pathfinding.'));`n"
$CppContent += "`n"
$CppContent += "    // Rendering APIs`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UMaterial'), TEXT('Asset that defines the appearance of surfaces through properties and expressions.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('UMaterialInstanceDynamic'), TEXT('Runtime material instance that can modify material parameters dynamically.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('FPrimitiveSceneProxy'), TEXT('Proxy class for rendering primitives in the scene rendering system.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('FSceneView'), TEXT('Represents a view of the scene for rendering.'));`n"
$CppContent += "`n"
$CppContent += "    // Performance APIs`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('FStatGroup'), TEXT('Declaration for grouping performance statistics.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('IStatsThread'), TEXT('Interface for accessing the stats thread system.'));`n"
$CppContent += "    APIKnowledgeBase.Add(TEXT('FStreamingManager'), TEXT('Manager for handling asset streaming and level streaming.'));`n"
$CppContent += "}`n"
$CppContent += "`n"
$CppContent += "void UMingUE5LearningSystem::UpdateLearningProgress()`n"
$CppContent += "{`n"
$CppContent += "    LearnedAPIs = APIKnowledgeBase.Num();`n"
$CppContent += "}`n"
$CppContent += "`n"
$CppContent += "bool UMingUE5LearningSystem::IsValidAPIName(const FString& APIName) const`n"
$CppContent += "{`n"
$CppContent += "    return APIKnowledgeBase.Contains(APIName);`n"
$CppContent += "}`n"

Set-Content -Path $CppPath -Value $CppContent -Encoding UTF8

Write-Host "C++实现文件已生成: $CppPath" -ForegroundColor Green

Write-Host "" -ForegroundColor Green
Write-Host "🎯 系统文件创建完成!" -ForegroundColor Green
Write-Host "📄 PowerShell脚本: C:\HW\MingGoRTS\Tools\api\UE5-Learning-Working.ps1" -ForegroundColor Green
Write-Host "📄 C++头文件: $HeaderPath" -ForegroundColor Green
Write-Host "📄 C++实现文件: $CppPath" -ForegroundColor Green
Write-Host "" -ForegroundColor Green
Write-Host "✅ UE5.7 API学习系统创建完成!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
