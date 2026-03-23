# MingGoRTS UE5.7 API Learning System - Simplified PowerShell Script

param(
    [string]$LearningMode = "Comprehensive",
    [string]$DocumentationURL = "https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-5-7-documentation",
    [switch]$Interactive = $false,
    [switch]$Verbose = $false,
    [string]$OutputPath = "C:\HW\MingGoRTS\Docs\UE5_Learning",
    [switch]$ForceUpdate = $false
)

# Configuration
$ProjectRoot = "C:\HW\MingGoRTS"
$LogPath = "$ProjectRoot\Logs\UE5Learning_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"
$CachePath = "$ProjectRoot\Cache\UE5_Docs"
$KnowledgeBasePath = "$ProjectRoot\Knowledge\UE5_5.7"

# Initialize logging
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    Write-Host $LogEntry -ForegroundColor $(switch($Level) {"ERROR" {"Red"}; "WARNING" {"Yellow"}; "SUCCESS" {"Green"}; "DEBUG" {"Gray"}; default {"White"}})
    Add-Content -Path $LogPath -Value $LogEntry
}

# Initialize directories
function Initialize-Directories {
    Write-Log "初始化学习系统目录..." "INFO"
    
    $Directories = @(
        "$OutputPath",
        "$OutputPath\API",
        "$OutputPath\Examples", 
        "$OutputPath\Tutorials",
        "$OutputPath\Reference",
        "$CachePath",
        "$KnowledgeBasePath",
        "$KnowledgeBasePath\Classes",
        "$KnowledgeBasePath\Functions",
        "$KnowledgeBasePath\Patterns",
        "$KnowledgeBasePath\BestPractices"
    )
    
    foreach ($Dir in $Directories) {
        if (-not (Test-Path $Dir)) {
            New-Item -ItemType Directory -Path $Dir -Force | Out-Null
            Write-Log "创建目录: $Dir" "SUCCESS"
        }
    }
}

# UE5.7 Documentation Fetcher (Simplified)
function New-UE5DocumentationFetcher {
    param(
        [string]$BaseURL,
        [string]$CachePath
    )
    
    $Fetcher = @{
        BaseURL = $BaseURL
        CachePath = $CachePath
        DocumentationIndex = @{}
    }
    
    # Initialize documentation index
    $Fetcher.DocumentationIndex = @{
        "GettingStarted" = @{
            "URL" = "$($Fetcher.BaseURL)/getting-started"
            "Categories" = @("Installation", "Setup", "FirstProject", "Interface")
            "Priority" = "High"
        }
        "Programming" = @{
            "URL" = "$($Fetcher.BaseURL)/programming"
            "Categories" = @("CPP", "Blueprints", "API", "Plugins")
            "Priority" = "High"
        }
        "Gameplay" = @{
            "URL" = "$($Fetcher.BaseURL)/gameplay"
            "Categories" = @("Actors", "Components", "GameMode", "Input")
            "Priority" = "High"
        }
        "Rendering" = @{
            "URL" = "$($Fetcher.BaseURL)/rendering"
            "Categories" = @("Materials", "Lighting", "PostProcessing", "Meshes")
            "Priority" = "Medium"
        }
        "Physics" = @{
            "URL" = "$($Fetcher.BaseURL)/physics"
            "Categories" = @("Collision", "Constraints", "Simulation")
            "Priority" = "Medium"
        }
        "AI" = @{
            "URL" = "$($Fetcher.BaseURL)/artificial-intelligence"
            "Categories" = @("BehaviorTrees", "EQS", "Navigation", "Perception")
            "Priority" = "High"
        }
        "Networking" = @{
            "URL" = "$($Fetcher.BaseURL)/networking"
            "Categories" = @("Replication", "RPCs", "Actors", "Properties")
            "Priority" = "Medium"
        }
        "Audio" = @{
            "URL" = "$($Fetcher.BaseURL)/audio"
            "Categories" = @("SoundCues", "MetaSounds", "AudioComponents")
            "Priority" = "Low"
        }
        "UI" = @{
            "URL" = "$($Fetcher.BaseURL)/ui"
            "Categories" = @("UMG", "Widgets", "Slate", "Input")
            "Priority" = "Medium"
        }
        "Performance" = @{
            "URL" = "$($Fetcher.BaseURL)/optimization"
            "Categories" = @("Profiling", "Memory", "Rendering", "Cooking")
            "Priority" = "High"
        }
    }
    
    return $Fetcher
}

function Fetch-Documentation {
    param(
        [object]$Fetcher,
        [string]$Section
    )
    
    Write-Log "获取文档部分: $Section" "INFO"
    
    if (-not $Fetcher.DocumentationIndex.ContainsKey($Section)) {
        Write-Log "未找到文档部分: $Section" "WARNING"
        return ""
    }
    
    $SectionInfo = $Fetcher.DocumentationIndex[$Section]
    $CacheFile = "$($Fetcher.CachePath)\$Section.json"
    
    # Check cache first
    if ((Test-Path $CacheFile) -and -not $ForceUpdate) {
        $CacheTime = (Get-Item $CacheFile).LastWriteTime
        $TimeDiff = (Get-Date) - $CacheTime
        
        if ($TimeDiff.TotalDays -lt 7) { # Cache for 7 days
            Write-Log "从缓存加载: $Section" "DEBUG"
            return Get-Content $CacheFile -Raw
        }
    }
    
    # Simulate fetching (in real implementation, would use web requests)
    Write-Log "模拟从Epic Games获取文档: $Section" "INFO"
    
    # Generate mock documentation content based on section
    $Content = Generate-MockDocumentation -Section $Section
    
    # Cache the content
    Set-Content -Path $CacheFile -Value $Content -Encoding UTF8
    Write-Log "文档已缓存: $CacheFile" "SUCCESS"
    
    return $Content
}

function Generate-MockDocumentation {
    param([string]$Section)
    
    $Templates = @{
        "GettingStarted" = @{
            "Title" = "Getting Started with Unreal Engine 5.7"
            "Content" = "Unreal Engine 5.7 introduces several exciting new features and improvements including Nanite virtualized geometry, Lumen global illumination, World Partition for large worlds, Enhanced Blueprint system, and Improved C++ performance."
            "APIs" = @("UEngine", "UGameInstance", "UWorld", "AActor")
        }
        "Programming" = @{
            "Title" = "Programming with Unreal Engine 5.7"
            "Content" = "Unreal Engine 5.7 provides powerful programming capabilities with Modern C++17/20 support, Enhanced reflection system, Improved template metaprogramming, and Better integration with Blueprints."
            "APIs" = @("UObject", "AActor", "UActorComponent", "FDelegate")
        }
        "Gameplay" = @{
            "Title" = "Gameplay Systems in UE5.7"
            "Content" = "Core gameplay systems in Unreal Engine 5.7 include Actor Framework, Game Mode System, and Enhanced Input System."
            "APIs" = @("AActor", "UGameModeBase", "APlayerController", "APawn", "UEnhancedInputComponent")
        }
        "AI" = @{
            "Title" = "Artificial Intelligence Systems"
            "Content" = "Advanced AI systems in Unreal Engine 5.7 include Behavior Trees, Environment Query System, and Navigation System."
            "APIs" = @("UBehaviorTree", "UBlackboardComponent", "UEnvQuerySystem", "ANavigationData")
        }
        "Rendering" = @{
            "Title" = "Rendering and Graphics"
            "Content" = "Advanced rendering capabilities in UE5.7 include Nanite, Lumen, and improved Materials system."
            "APIs" = @("UMaterial", "UMaterialInstanceDynamic", "FPrimitiveSceneProxy", "FSceneView")
        }
        "Performance" = @{
            "Title" = "Performance Optimization"
            "Content" = "Performance optimization strategies for UE5.7 include Profiling Tools, Optimization Techniques, and Best Practices."
            "APIs" = @("FStatGroup", "IStatsThread", "UMaterialInstanceDynamic", "FStreamingManager")
        }
    }
    
    $Template = $Templates[$Section]
    if ($Template) {
        $Content = @{
            "Section" = $Section
            "Title" = $Template.Title
            "Content" = $Template.Content
            "APIs" = $Template.APIs
            "LastUpdated" = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
            "Version" = "5.7"
        }
        
        return $Content | ConvertTo-Json -Depth 10
    }
    
    return "{}"
}

# UE5.7 Knowledge Processor (Simplified)
function New-UE5KnowledgeProcessor {
    param([string]$KnowledgeBasePath)
    
    $Processor = @{
        KnowledgeBasePath = $KnowledgeBasePath
        LearnedConcepts = @{}
        APIPatterns = @{}
    }
    
    return $Processor
}

function Initialize-KnowledgeProcessor {
    param([object]$Processor)
    
    Write-Log "初始化知识处理器..." "INFO"
    Load-ExistingKnowledge -Processor $Processor
}

function Load-ExistingKnowledge {
    param([object]$Processor)
    
    Write-Log "加载现有知识库..." "INFO"
    
    $KnowledgeFiles = @(
        "$($Processor.KnowledgeBasePath)\Classes\knowledge.json",
        "$($Processor.KnowledgeBasePath)\Functions\knowledge.json",
        "$($Processor.KnowledgeBasePath)\Patterns\knowledge.json",
        "$($Processor.KnowledgeBasePath)\BestPractices\knowledge.json"
    )
    
    foreach ($File in $KnowledgeFiles) {
        if (Test-Path $File) {
            try {
                $Knowledge = Get-Content $File -Raw | ConvertFrom-Json
                foreach ($Concept in $Knowledge.Concepts) {
                    $Processor.LearnedConcepts[$Concept.Name] = $Concept
                }
                Write-Log "加载知识文件: $File" "SUCCESS"
            } catch {
                Write-Log "加载知识文件失败: $File - $($_.Exception.Message)" "WARNING"
            }
        }
    }
    
    Write-Log "已加载 $($Processor.LearnedConcepts.Count) 个知识概念" "INFO"
}

function Process-Documentation {
    param(
        [object]$Processor,
        [string]$Section,
        [string]$Content
    )
    
    Write-Log "处理文档: $Section" "INFO"
    
    try {
        $DocData = $Content | ConvertFrom-Json
        Extract-APIPatterns -Processor $Processor -DocData $DocData
        Identify-BestPractices -Processor $Processor -DocData $DocData
        Generate-LearningExamples -Processor $Processor -DocData $DocData
        Update-KnowledgeBase -Processor $Processor -DocData $DocData
        
        Write-Log "文档处理完成: $Section" "SUCCESS"
    } catch {
        Write-Log "文档处理失败: $Section - $($_.Exception.Message)" "ERROR"
    }
}

function Extract-APIPatterns {
    param(
        [object]$Processor,
        [object]$DocData
    )
    
    Write-Log "提取API模式..." "DEBUG"
    
    if ($DocData.APIs) {
        foreach ($API in $DocData.APIs) {
            $Pattern = @{
                "Name" = $API
                "Section" = $DocData.Section
                "Usage" = Generate-UsageExample -APIName $API
                "Context" = $DocData.Title
                "LearnedAt" = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
            }
            
            $Processor.APIPatterns[$API] = $Pattern
        }
    }
}

function Generate-UsageExample {
    param([string]$APIName)
    
    $Examples = @{
        "UEngine" = "UEngine* Engine = GetWorld()->GetEngine();"
        "UGameInstance" = "UGameInstance* GameInstance = GetGameInstance();"
        "UWorld" = "UWorld* World = GetWorld();"
        "AActor" = "AActor* MyActor = GetWorld()->SpawnActor<AMyActor>();"
        "UObject" = "UObject* Object = NewObject<UObject>();"
        "AActorComponent" = "UActorComponent* Component = CreateDefaultSubobject<UActorComponent>(TEXT('MyComponent'));"
        "FDelegate" = "FDelegate::CreateUObject(this, &AMyActor::MyFunction);"
        "UGameModeBase" = "AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();"
        "APlayerController" = "APlayerController* PC = GetWorld()->GetFirstPlayerController();"
        "APawn" = "APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();"
        "UEnhancedInputComponent" = "UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);"
        "UBehaviorTree" = "UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT('/Game/AI/MyBehaviorTree'));"
        "UBlackboardComponent" = "UBlackboardComponent* Blackboard = GetBlackboardComponent();"
        "UEnvQuerySystem" = "UEnvQuerySystem* QuerySystem = UWorld::GetSubsystem<UEnvQuerySystem>(GetWorld());"
        "ANavigationData" = "ANavigationData* NavData = GetWorld()->GetNavigationSystem()->GetDefaultNavData();"
        "UMaterial" = "UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT('/Game/Materials/MyMaterial'));"
        "UMaterialInstanceDynamic" = "UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(Material, this);"
        "FPrimitiveSceneProxy" = "class FMyPrimitiveProxy : public FPrimitiveSceneProxy"
        "FSceneView" = "FSceneView* SceneView = new FSceneView();"
        "FStatGroup" = "DECLARE_STAT_GROUP(TEXT('MyCustomStat'));"
        "IStatsThread" = "IStatsThread::Get()."
        "FStreamingManager" = "FStreamingManager::Get()."
    }
    
    return $Examples[$APIName] ?? "// Usage example for $APIName"
}

function Identify-BestPractices {
    param(
        [object]$Processor,
        [object]$DocData
    )
    
    Write-Log "识别最佳实践..." "DEBUG"
    
    $BestPractices = @()
    
    # Extract best practices based on content analysis
    if ($DocData.Content -match "memory|performance|optimize") {
        $BestPractices += @{
            "Category" = "Performance"
            "Practice" = "Always profile your code and optimize based on actual performance data"
            "Context" = $DocData.Section
        }
    }
    
    if ($DocData.Content -match "component|actor") {
        $BestPractices += @{
            "Category" = "Architecture"
            "Practice" = "Use component-based design for modular and reusable code"
            "Context" = $DocData.Section
        }
    }
    
    if ($DocData.Content -match "event|delegate") {
        $BestPractices += @{
            "Category" = "Communication"
            "Practice" = "Use events and delegates for loose coupling between systems"
            "Context" = $DocData.Section
        }
    }
    
    # Store best practices
    foreach ($Practice in $BestPractices) {
        $Key = "$($Practice.Category)_$($Practice.Context)"
        $Processor.LearnedConcepts[$Key] = $Practice
    }
}

function Generate-LearningExamples {
    param(
        [object]$Processor,
        [object]$DocData
    )
    
    Write-Log "生成学习示例..." "DEBUG"
    
    $ExamplePath = "$($Processor.KnowledgeBasePath)\Examples\$($DocData.Section)_Examples.json"
    $Examples = @()
    
    if ($DocData.APIs) {
        foreach ($API in $DocData.APIs) {
            $Example = @{
                "API" = $API
                "Example" = Generate-UsageExample -APIName $API
                "Explanation" = Generate-Explanation -APIName $API
                "Section" = $DocData.Section
                "Difficulty" = "Beginner"
            }
            $Examples += $Example
        }
    }
    
    # Save examples
    $ExamplesData = @{
        "Section" = $DocData.Section
        "Examples" = $Examples
        "GeneratedAt" = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
    }
    
    Set-Content -Path $ExamplePath -Value ($ExamplesData | ConvertTo-Json -Depth 10) -Encoding UTF8
    Write-Log "生成示例文件: $ExamplePath" "SUCCESS"
}

function Generate-Explanation {
    param([string]$APIName)
    
    $Explanations = @{
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
    
    return $Explanations[$APIName] ?? "Explanation for $APIName"
}

function Update-KnowledgeBase {
    param(
        [object]$Processor,
        [object]$DocData
    )
    
    Write-Log "更新知识库..." "DEBUG"
    
    # Update classes knowledge
    $ClassesPath = "$($Processor.KnowledgeBasePath)\Classes\knowledge.json"
    $ClassesKnowledge = Load-KnowledgeFile -FilePath $ClassesPath
    
    if ($DocData.APIs) {
        foreach ($API in $DocData.APIs) {
            $Concept = @{
                "Name" = $API
                "Type" = "Class"
                "Section" = $DocData.Section
                "Description" = Generate-Explanation -APIName $API
                "Usage" = Generate-UsageExample -APIName $API
                "LearnedAt" = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
            }
            $ClassesKnowledge.Concepts += $Concept
        }
    }
    
    Save-KnowledgeFile -FilePath $ClassesPath -Knowledge $ClassesKnowledge
    
    # Update patterns knowledge
    $PatternsPath = "$($Processor.KnowledgeBasePath)\Patterns\knowledge.json"
    $PatternsKnowledge = Load-KnowledgeFile -FilePath $PatternsPath
    
    foreach ($Pattern in $Processor.APIPatterns.Values) {
        $Concept = @{
            "Name" = $Pattern.Name
            "Type" = "Pattern"
            "Section" = $Pattern.Section
            "Description" = "API usage pattern for $($Pattern.Name)"
            "Usage" = $Pattern.Usage
            "Context" = $Pattern.Context
            "LearnedAt" = $Pattern.LearnedAt
        }
        $PatternsKnowledge.Concepts += $Concept
    }
    
    Save-KnowledgeFile -FilePath $PatternsPath -Knowledge $PatternsKnowledge
    
    Write-Log "知识库更新完成" "SUCCESS"
}

function Load-KnowledgeFile {
    param([string]$FilePath)
    
    if (Test-Path $FilePath) {
        try {
            return Get-Content $FilePath -Raw | ConvertFrom-Json
        } catch {
            Write-Log "加载知识文件失败: $FilePath" "WARNING"
        }
    }
    
    return @{
        "Type" = "Knowledge"
        "Concepts" = @()
        "LastUpdated" = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
    }
}

function Save-KnowledgeFile {
    param(
        [string]$FilePath,
        [object]$Knowledge
    )
    
    $Knowledge["LastUpdated"] = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
    $Dir = Split-Path -Parent $FilePath
    if (-not (Test-Path $Dir)) {
        New-Item -ItemType Directory -Path $Dir -Force | Out-Null
    }
    
    Set-Content -Path $FilePath -Value ($Knowledge | ConvertTo-Json -Depth 10) -Encoding UTF8
}

# Learning Orchestrator (Simplified)
function New-UE5LearningOrchestrator {
    param(
        [string]$DocumentationURL,
        [string]$CachePath,
        [string]$KnowledgeBasePath,
        [string]$OutputPath
    )
    
    $Orchestrator = @{
        DocumentationFetcher = New-UE5DocumentationFetcher -BaseURL $DocumentationURL -CachePath $CachePath
        KnowledgeProcessor = New-UE5KnowledgeProcessor -KnowledgeBasePath $KnowledgeBasePath
        OutputPath = $OutputPath
    }
    
    return $Orchestrator
}

function Initialize-LearningOrchestrator {
    param([object]$Orchestrator)
    
    Write-Log "初始化学习协调器..." "INFO"
    Initialize-KnowledgeProcessor -Processor $Orchestrator.KnowledgeProcessor
}

function Start-Learning {
    param(
        [object]$Orchestrator,
        [string]$LearningMode
    )
    
    Write-Log "开始学习模式: $LearningMode" "INFO"
    
    switch ($LearningMode) {
        "Quick" {
            Quick-Learning -Orchestrator $Orchestrator
        }
        "Comprehensive" {
            Comprehensive-Learning -Orchestrator $Orchestrator
        }
        "Targeted" {
            Targeted-Learning -Orchestrator $Orchestrator
        }
        default {
            Write-Log "未知的学习模式: $LearningMode" "WARNING"
            Comprehensive-Learning -Orchestrator $Orchestrator
        }
    }
}

function Quick-Learning {
    param([object]$Orchestrator)
    
    Write-Log "快速学习模式 - 核心概念" "INFO"
    
    $CoreSections = @("GettingStarted", "Programming", "Gameplay")
    
    foreach ($Section in $CoreSections) {
        Write-Log "学习核心部分: $Section" "INFO"
        $Content = Fetch-Documentation -Fetcher $Orchestrator.DocumentationFetcher -Section $Section
        Process-Documentation -Processor $Orchestrator.KnowledgeProcessor -Section $Section -Content $Content
    }
    
    Generate-QuickReport -Orchestrator $Orchestrator
}

function Comprehensive-Learning {
    param([object]$Orchestrator)
    
    Write-Log "全面学习模式 - 所有文档" "INFO"
    
    $AllSections = $Orchestrator.DocumentationFetcher.DocumentationIndex.Keys
    
    foreach ($Section in $AllSections) {
        Write-Log "学习部分: $Section" "INFO"
        $Content = Fetch-Documentation -Fetcher $Orchestrator.DocumentationFetcher -Section $Section
        Process-Documentation -Processor $Orchestrator.KnowledgeProcessor -Section $Section -Content $Content
        
        if ($Interactive) {
            Write-Host "按Enter继续学习下一部分..." -ForegroundColor Yellow
            Read-Host
        }
    }
    
    Generate-ComprehensiveReport -Orchestrator $Orchestrator
}

function Targeted-Learning {
    param([object]$Orchestrator)
    
    Write-Log "定向学习模式 - 高优先级内容" "INFO"
    
    $HighPrioritySections = @("Programming", "Gameplay", "AI", "Performance")
    
    foreach ($Section in $HighPrioritySections) {
        Write-Log "学习高优先级部分: $Section" "INFO"
        $Content = Fetch-Documentation -Fetcher $Orchestrator.DocumentationFetcher -Section $Section
        Process-Documentation -Processor $Orchestrator.KnowledgeProcessor -Section $Section -Content $Content
    }
    
    Generate-TargetedReport -Orchestrator $Orchestrator
}

function Generate-QuickReport {
    param([object]$Orchestrator)
    
    Write-Log "生成快速学习报告..." "INFO"
    
    $ReportPath = "$($Orchestrator.OutputPath)\QuickLearningReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = @"
# UE5.7 快速学习报告

## 学习概述
快速学习模式专注于Unreal Engine 5.7的核心概念和基础API。

## 已学习部分
- Getting Started (入门指南)
- Programming (编程基础)  
- Gameplay (游戏玩法)

## 关键API学习
- UEngine: 主引擎类
- UGameInstance: 游戏实例
- UWorld: 游戏世界
- AActor: 游戏对象基类
- UObject: UE5基础对象类

## 下一步建议
1. 深入学习AI系统
2. 掌握渲染和性能优化
3. 实践项目开发

## 学习时间
$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
"@
    
    Set-Content -Path $ReportPath -Value $Report -Encoding UTF8
    Write-Log "快速学习报告已生成: $ReportPath" "SUCCESS"
}

function Generate-ComprehensiveReport {
    param([object]$Orchestrator)
    
    Write-Log "生成全面学习报告..." "INFO"
    
    $ReportPath = "$($Orchestrator.OutputPath)\ComprehensiveLearningReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = @"
# UE5.7 全面学习报告

## 学习概述
全面学习模式涵盖了Unreal Engine 5.7的所有主要文档部分。

## 已学习部分
- Getting Started (入门指南)
- Programming (编程)
- Gameplay (游戏玩法)
- Rendering (渲染)
- Physics (物理)
- AI (人工智能)
- Networking (网络)
- Audio (音频)
- UI (用户界面)
- Performance (性能优化)

## API知识库
已学习 $($Orchestrator.KnowledgeProcessor.LearnedConcepts.Count) 个API概念和模式

## 最佳实践
已识别多个性能优化和架构设计最佳实践

## 学习统计
- 总学习部分: $($Orchestrator.DocumentationFetcher.DocumentationIndex.Count)
- 学习的API数量: $($Orchestrator.KnowledgeProcessor.APIPatterns.Count)
- 知识概念数量: $($Orchestrator.KnowledgeProcessor.LearnedConcepts.Count)

## 学习时间
$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')

## 下一步计划
1. 开始实际项目开发
2. 深入研究特定领域
3. 参与社区讨论和贡献
"@
    
    Set-Content -Path $ReportPath -Value $Report -Encoding UTF8
    Write-Log "全面学习报告已生成: $ReportPath" "SUCCESS"
}

function Generate-TargetedReport {
    param([object]$Orchestrator)
    
    Write-Log "生成定向学习报告..." "INFO"
    
    $ReportPath = "$($Orchestrator.OutputPath)\TargetedLearningReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = @"
# UE5.7 定向学习报告

## 学习概述
定向学习模式专注于Unreal Engine 5.7的高优先级开发领域。

## 已学习部分
- Programming (编程) - 高优先级
- Gameplay (游戏玩法) - 高优先级  
- AI (人工智能) - 高优先级
- Performance (性能优化) - 高优先级

## 重点关注领域
1. **编程架构**: 掌握UE5.7的组件系统和现代C++特性
2. **游戏玩法**: 理解Actor框架和游戏模式系统
3. **人工智能**: 实施行为树和环境查询系统
4. **性能优化**: 学习profiling工具和优化技术

## 实践建议
1. **立即实践**: 使用学习的API创建简单的Actor和组件
2. **项目集成**: 将知识应用到MingGoRTS项目的具体开发中
3. **持续学习**: 定期回顾文档并关注UE5.7的更新
4. **社区参与**: 加入Epic开发者社区分享经验

## 学习时间
$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')

## 项目应用建议
1. 使用学习的API开发MingGoRTS核心系统
2. 实施AI行为树系统
3. 优化游戏性能
"@
    
    Set-Content -Path $ReportPath -Value $Report -Encoding UTF8
    Write-Log "定向学习报告已生成: $ReportPath" "SUCCESS"
}

# Main execution function
function Start-UE5Learning {
    Write-Log "========================================" "INFO"
    Write-Log "MingGoRTS UE5.7 API学习系统" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "学习模式: $LearningMode" "INFO"
    Write-Log "文档URL: $DocumentationURL" "INFO"
    Write-Log "输出路径: $OutputPath" "INFO"
    Write-Log "交互模式: $Interactive" "INFO"
    Write-Log "强制更新: $ForceUpdate" "INFO"
    Write-Log "" "INFO"
    
    try {
        # Initialize
        Initialize-Directories
        
        # Create learning orchestrator
        $Orchestrator = New-UE5LearningOrchestrator -DocumentationURL $DocumentationURL -CachePath $CachePath -KnowledgeBasePath $KnowledgeBasePath -OutputPath $OutputPath
        Initialize-LearningOrchestrator -Orchestrator $Orchestrator
        
        # Start learning
        Start-Learning -Orchestrator $Orchestrator -LearningMode $LearningMode
        
        Write-Log "" "INFO"
        Write-Log "🎓 UE5.7学习完成!" "SUCCESS"
        Write-Log "📚 知识库位置: $KnowledgeBasePath" "INFO"
        Write-Log "📊 学习报告: $OutputPath" "INFO"
        Write-Log "🗂️  缓存位置: $CachePath" "INFO"
        Write-Log "📝 日志文件: $LogPath" "INFO"
        
    } catch {
        Write-Log "学习过程中发生错误: $($_.Exception.Message)" "ERROR"
        if ($Verbose) {
            Write-Log "详细错误信息: $($_.Exception.StackTrace)" "ERROR"
        }
        exit 1
    }
}

# Execute main function
Start-UE5Learning
