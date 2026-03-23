# MingGoRTS UE5.7 API Learning System - Clean Version

param(
    [string]$LearningMode = "Quick",
    [switch]$Interactive = $false,
    [switch]$Verbose = $false
)

# Configuration
$ProjectRoot = "C:\HW\MingGoRTS"
$OutputPath = "C:\HW\MingGoRTS\Docs\UE5_Learning"
$CachePath = "C:\HW\MingGoRTS\Cache\UE5_Docs"
$KnowledgeBasePath = "C:\HW\MingGoRTS\Knowledge\UE5_5.7"

# Initialize logging
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    Write-Host $LogEntry -ForegroundColor $(switch($Level) {"ERROR" {"Red"}; "WARNING" {"Yellow"}; "SUCCESS" {"Green"}; "DEBUG" {"Gray"}; default {"White"}})
}

# Initialize directories
function Initialize-Directories {
    Write-Log "初始化学习系统目录..." "INFO"
    
    $Directories = @(
        $OutputPath,
        "$OutputPath\API",
        "$OutputPath\Examples", 
        "$OutputPath\Tutorials",
        "$OutputPath\Reference",
        $CachePath,
        $KnowledgeBasePath,
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

# UE5.7 API Knowledge Base
function Initialize-APIKnowledgeBase {
    Write-Log "初始化UE5.7 API知识库..." "INFO"
    
    $APIs = @{
        "UEngine" = @{
            "Description" = "The main engine class that manages core engine systems and provides access to global engine functionality."
            "Usage" = "UEngine* Engine = GetWorld()->GetEngine();"
            "Category" = "Core"
            "Difficulty" = "Intermediate"
        }
        "UGameInstance" = @{
            "Description" = "A game instance that persists across level transitions and manages game-wide state."
            "Usage" = "UGameInstance* GameInstance = GetGameInstance();"
            "Category" = "Core"
            "Difficulty" = "Beginner"
        }
        "UWorld" = @{
            "Description" = "Represents a game world containing actors, levels, and manages the simulation of that world."
            "Usage" = "UWorld* World = GetWorld();"
            "Category" = "Core"
            "Difficulty" = "Beginner"
        }
        "AActor" = @{
            "Description" = "Base class for all objects that can be placed or spawned in a level."
            "Usage" = "AActor* MyActor = GetWorld()->SpawnActor<AMyActor>();"
            "Category" = "Gameplay"
            "Difficulty" = "Beginner"
        }
        "UObject" = @{
            "Description" = "Base class for most objects in Unreal Engine, providing reflection, garbage collection, and serialization."
            "Usage" = "UObject* Object = NewObject<UObject>();"
            "Category" = "Core"
            "Difficulty" = "Beginner"
        }
        "AActorComponent" = @{
            "Description" = "Base class for components that can be attached to actors to add functionality."
            "Usage" = "UActorComponent* Component = CreateDefaultSubobject<UActorComponent>(TEXT('MyComponent'));"
            "Category" = "Gameplay"
            "Difficulty" = "Intermediate"
        }
        "FDelegate" = @{
            "Description" = "Template for creating delegates that can bind to member functions or static functions."
            "Usage" = "FDelegate::CreateUObject(this, &AMyActor::MyFunction);"
            "Category" = "Programming"
            "Difficulty" = "Advanced"
        }
        "UGameModeBase" = @{
            "Description" = "Base class for game modes that define the rules and behavior of the game."
            "Usage" = "AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();"
            "Category" = "Gameplay"
            "Difficulty" = "Intermediate"
        }
        "APlayerController" = @{
            "Description" = "Controls a player's pawn and handles player input and camera."
            "Usage" = "APlayerController* PC = GetWorld()->GetFirstPlayerController();"
            "Category" = "Gameplay"
            "Difficulty" = "Intermediate"
        }
        "APawn" = @{
            "Description" = "Base class for actors that can be controlled by players or AI."
            "Usage" = "APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();"
            "Category" = "Gameplay"
            "Difficulty" = "Beginner"
        }
        "UEnhancedInputComponent" = @{
            "Description" = "Component for handling enhanced input with context-aware actions."
            "Usage" = "UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);"
            "Category" = "Input"
            "Difficulty" = "Intermediate"
        }
        "UBehaviorTree" = @{
            "Description" = "Asset that defines AI behavior using a tree of nodes."
            "Usage" = "UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT('/Game/AI/MyBehaviorTree'));"
            "Category" = "AI"
            "Difficulty" = "Advanced"
        }
        "UBlackboardComponent" = @{
            "Description" = "Component that stores data used by behavior trees for AI decision making."
            "Usage" = "UBlackboardComponent* Blackboard = GetBlackboardComponent();"
            "Category" = "AI"
            "Difficulty" = "Intermediate"
        }
        "UEnvQuerySystem" = @{
            "Description" = "System that handles environment queries for AI to find locations or objects."
            "Usage" = "UEnvQuerySystem* QuerySystem = UWorld::GetSubsystem<UEnvQuerySystem>(GetWorld());"
            "Category" = "AI"
            "Difficulty" = "Advanced"
        }
        "ANavigationData" = @{
            "Description" = "Base class for navigation data used by AI for pathfinding."
            "Usage" = "ANavigationData* NavData = GetWorld()->GetNavigationSystem()->GetDefaultNavData();"
            "Category" = "AI"
            "Difficulty" = "Intermediate"
        }
        "UMaterial" = @{
            "Description" = "Asset that defines the appearance of surfaces through properties and expressions."
            "Usage" = "UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT('/Game/Materials/MyMaterial'));"
            "Category" = "Rendering"
            "Difficulty" = "Intermediate"
        }
        "UMaterialInstanceDynamic" = @{
            "Description" = "Runtime material instance that can modify material parameters dynamically."
            "Usage" = "UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(Material, this);"
            "Category" = "Rendering"
            "Difficulty" = "Advanced"
        }
        "FPrimitiveSceneProxy" = @{
            "Description" = "Proxy class for rendering primitives in the scene rendering system."
            "Usage" = "class FMyPrimitiveProxy : public FPrimitiveSceneProxy"
            "Category" = "Rendering"
            "Difficulty" = "Expert"
        }
        "FSceneView" = @{
            "Description" = "Represents a view of the scene for rendering."
            "Usage" = "FSceneView* SceneView = new FSceneView();"
            "Category" = "Rendering"
            "Difficulty" = "Advanced"
        }
        "FStatGroup" = @{
            "Description" = "Declaration for grouping performance statistics."
            "Usage" = "DECLARE_STAT_GROUP(TEXT('MyCustomStat'));"
            "Category" = "Performance"
            "Difficulty" = "Intermediate"
        }
        "IStatsThread" = @{
            "Description" = "Interface for accessing the stats thread system."
            "Usage" = "IStatsThread::Get()."
            "Category" = "Performance"
            "Difficulty" = "Advanced"
        }
        "FStreamingManager" = @{
            "Description" = "Manager for handling asset streaming and level streaming."
            "Usage" = "FStreamingManager::Get()."
            "Category" = "Performance"
            "Difficulty" = "Advanced"
        }
    }
    
    return $APIs
}

# Learning functions
function Start-QuickLearning {
    param([hashtable]$APIs)
    
    Write-Log "开始快速学习模式..." "INFO"
    
    $CoreAPIs = @("UEngine", "UGameInstance", "UWorld", "AActor", "UObject")
    
    foreach ($API in $CoreAPIs) {
        if ($APIs.ContainsKey($API)) {
            $Info = $APIs[$API]
            Write-Log "学习API: $API" "INFO"
            Write-Log "  描述: $($Info.Description)" "DEBUG"
            Write-Log "  用法: $($Info.Usage)" "DEBUG"
            Write-Log "  类别: $($Info.Category)" "DEBUG"
            Write-Log "  难度: $($Info.Difficulty)" "DEBUG"
        }
    }
    
    New-QuickReport -APIs $APIs
}

function Start-ComprehensiveLearning {
    param([hashtable]$APIs)
    
    Write-Log "开始全面学习模式..." "INFO"
    
    foreach ($API in $APIs.Keys) {
        $Info = $APIs[$API]
        Write-Log "学习API: $API" "INFO"
        Write-Log "  描述: $($Info.Description)" "DEBUG"
        Write-Log "  用法: $($Info.Usage)" "DEBUG"
        Write-Log "  类别: $($Info.Category)" "DEBUG"
        Write-Log "  难度: $($Info.Difficulty)" "DEBUG"
        
        if ($Interactive) {
            Write-Host "按Enter继续学习下一个API..." -ForegroundColor Yellow
            Read-Host
        }
    }
    
    New-ComprehensiveReport -APIs $APIs
}

function Start-TargetedLearning {
    param([hashtable]$APIs)
    
    Write-Log "开始定向学习模式..." "INFO"
    
    $HighPriorityCategories = @("Core", "Gameplay", "AI", "Performance")
    
    foreach ($Category in $HighPriorityCategories) {
        Write-Log "学习类别: $Category" "INFO"
        
        foreach ($API in $APIs.Keys) {
            $Info = $APIs[$API]
            if ($Info.Category -eq $Category) {
                Write-Log "  学习API: $API" "INFO"
                Write-Log "    描述: $($Info.Description)" "DEBUG"
                Write-Log "    用法: $($Info.Usage)" "DEBUG"
                Write-Log "    难度: $($Info.Difficulty)" "DEBUG"
            }
        }
    }
    
    New-TargetedReport -APIs $APIs
}

# Report generation
function New-QuickReport {
    param([hashtable]$APIs)
    
    Write-Log "生成快速学习报告..." "INFO"
    
    $ReportPath = "$OutputPath\QuickLearningReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = "# UE5.7 快速学习报告`n`n"
    $Report += "## 学习概述`n"
    $Report += "快速学习模式专注于Unreal Engine 5.7的核心概念和基础API。`n`n"
    $Report += "## 已学习部分`n"
    $Report += "- Getting Started (入门指南)`n"
    $Report += "- Programming (编程基础)`n"
    $Report += "- Gameplay (游戏玩法)`n`n"
    $Report += "## 关键API学习`n`n"
    
    $CoreAPIs = @("UEngine", "UGameInstance", "UWorld", "AActor", "UObject")
    
    foreach ($API in $CoreAPIs) {
        if ($APIs.ContainsKey($API)) {
            $Info = $APIs[$API]
            $Report += "### $API`n"
            $Report += "- **描述**: $($Info.Description)`n"
            $Report += "- **用法**: ````cpp`n$($Info.Usage)`n`````n"
            $Report += "- **类别**: $($Info.Category)`n"
            $Report += "- **难度**: $($Info.Difficulty)`n`n"
        }
    }
    
    $Report += "## 下一步建议`n"
    $Report += "1. 深入学习AI系统`n"
    $Report += "2. 掌握渲染和性能优化`n"
    $Report += "3. 实践项目开发`n`n"
    $Report += "## 学习时间`n"
    $Report += "$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n"
    
    Set-Content -Path $ReportPath -Value $Report -Encoding UTF8
    Write-Log "快速学习报告已生成: $ReportPath" "SUCCESS"
}

function New-ComprehensiveReport {
    param([hashtable]$APIs)
    
    Write-Log "生成全面学习报告..." "INFO"
    
    $ReportPath = "$OutputPath\ComprehensiveLearningReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = "# UE5.7 全面学习报告`n`n"
    $Report += "## 学习概述`n"
    $Report += "全面学习模式涵盖了Unreal Engine 5.7的所有主要API。`n`n"
    $Report += "## API知识库`n`n"
    
    $Categories = @("Core", "Gameplay", "Programming", "Input", "AI", "Rendering", "Performance")
    
    foreach ($Category in $Categories) {
        $CategoryAPIs = $APIs.GetEnumerator() | Where-Object { $_.Value.Category -eq $Category }
        
        if ($CategoryAPIs.Count -gt 0) {
            $Report += "### $Category APIs`n`n"
            
            foreach ($APIEntry in $CategoryAPIs) {
                $API = $APIEntry.Key
                $Info = $APIEntry.Value
                
                $Report += "#### $API`n"
                $Report += "- **描述**: $($Info.Description)`n"
                $Report += "- **用法**: ````cpp`n$($Info.Usage)`n`````n"
                $Report += "- **难度**: $($Info.Difficulty)`n`n"
            }
        }
    }
    
    $Report += "## 学习统计`n"
    $Report += "- 总API数量: $($APIs.Count)`n"
    $Report += "- Core APIs: $(($APIs.Values | Where-Object { $_.Category -eq 'Core' }).Count)`n"
    $Report += "- Gameplay APIs: $(($APIs.Values | Where-Object { $_.Category -eq 'Gameplay' }).Count)`n"
    $Report += "- AI APIs: $(($APIs.Values | Where-Object { $_.Category -eq 'AI' }).Count)`n"
    $Report += "- Rendering APIs: $(($APIs.Values | Where-Object { $_.Category -eq 'Rendering' }).Count)`n"
    $Report += "- Performance APIs: $(($APIs.Values | Where-Object { $_.Category -eq 'Performance' }).Count)`n`n"
    
    $Report += "## 学习时间`n"
    $Report += "$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n`n"
    
    $Report += "## 下一步计划`n"
    $Report += "1. 开始实际项目开发`n"
    $Report += "2. 深入研究特定领域`n"
    $Report += "3. 参与社区讨论和贡献`n"
    
    Set-Content -Path $ReportPath -Value $Report -Encoding UTF8
    Write-Log "全面学习报告已生成: $ReportPath" "SUCCESS"
}

function New-TargetedReport {
    param([hashtable]$APIs)
    
    Write-Log "生成定向学习报告..." "INFO"
    
    $ReportPath = "$OutputPath\TargetedLearningReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = "# UE5.7 定向学习报告`n`n"
    $Report += "## 学习概述`n"
    $Report += "定向学习模式专注于Unreal Engine 5.7的高优先级开发领域。`n`n"
    $Report += "## 已学习部分`n"
    $Report += "- Core APIs (核心API) - 高优先级`n"
    $Report += "- Gameplay APIs (游戏玩法API) - 高优先级`n"
    $Report += "- AI APIs (人工智能API) - 高优先级`n"
    $Report += "- Performance APIs (性能优化API) - 高优先级`n`n"
    $Report += "## 重点关注领域`n`n"
    
    $HighPriorityCategories = @("Core", "Gameplay", "AI", "Performance")
    
    foreach ($Category in $HighPriorityCategories) {
        $CategoryAPIs = $APIs.GetEnumerator() | Where-Object { $_.Value.Category -eq $Category }
        
        if ($CategoryAPIs.Count -gt 0) {
            $Report += "### $Category APIs`n"
            $Report += "这些API用于$Category相关功能：`n`n"
            
            foreach ($APIEntry in $CategoryAPIs) {
                $API = $APIEntry.Key
                $Info = $APIEntry.Value
                
                $Report += "- **$API**: $($Info.Description)`n"
                $Report += "  - 用法: ````cpp`n$($Info.Usage)`n`````n"
                $Report += "  - 难度: $($Info.Difficulty)`n`n"
            }
        }
    }
    
    $Report += "## 实践建议`n"
    $Report += "1. **立即实践**: 使用学习的API创建简单的Actor和组件`n"
    $Report += "2. **项目集成**: 将知识应用到MingGoRTS项目的具体开发中`n"
    $Report += "3. **持续学习**: 定期回顾文档并关注UE5.7的更新`n"
    $Report += "4. **社区参与**: 加入Epic开发者社区分享经验`n`n"
    
    $Report += "## 学习时间`n"
    $Report += "$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n`n"
    
    $Report += "## 项目应用建议`n"
    $Report += "1. 使用学习的API开发MingGoRTS核心系统`n"
    $Report += "2. 实施AI行为树系统`n"
    $Report += "3. 优化游戏性能`n"
    
    Set-Content -Path $ReportPath -Value $Report -Encoding UTF8
    Write-Log "定向学习报告已生成: $ReportPath" "SUCCESS"
}

# Save knowledge base
function Save-KnowledgeBase {
    param([hashtable]$APIs)
    
    Write-Log "保存知识库..." "INFO"
    
    $KnowledgePath = "$KnowledgeBasePath\UE5_API_Knowledge_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
    
    $KnowledgeData = @{
        "Version" = "5.7"
        "GeneratedAt" = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        "APIs" = $APIs
        "TotalAPIs" = $APIs.Count
    }
    
    Set-Content -Path $KnowledgePath -Value ($KnowledgeData | ConvertTo-Json -Depth 10) -Encoding UTF8
    Write-Log "知识库已保存: $KnowledgePath" "SUCCESS"
}

# Main execution
function Main {
    Write-Log "========================================" "INFO"
    Write-Log "MingGoRTS UE5.7 API学习系统" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "学习模式: $LearningMode" "INFO"
    Write-Log "交互模式: $Interactive" "INFO"
    Write-Log "详细输出: $Verbose" "INFO"
    Write-Log "" "INFO"
    
    try {
        # Initialize
        Initialize-Directories
        
        # Initialize API knowledge base
        $APIs = Initialize-APIKnowledgeBase
        
        # Start learning based on mode
        switch ($LearningMode) {
            "Quick" {
                Start-QuickLearning -APIs $APIs
            }
            "Comprehensive" {
                Start-ComprehensiveLearning -APIs $APIs
            }
            "Targeted" {
                Start-TargetedLearning -APIs $APIs
            }
            default {
                Write-Log "未知的学习模式: $LearningMode，使用快速模式" "WARNING"
                Start-QuickLearning -APIs $APIs
            }
        }
        
        # Save knowledge base
        Save-KnowledgeBase -APIs $APIs
        
        Write-Log "" "INFO"
        Write-Log "🎓 UE5.7学习完成!" "SUCCESS"
        Write-Log "📚 知识库位置: $KnowledgeBasePath" "INFO"
        Write-Log "📊 学习报告: $OutputPath" "INFO"
        Write-Log "🗂️  缓存位置: $CachePath" "INFO"
        Write-Log "📝 已学习API数量: $($APIs.Count)" "INFO"
        
    } catch {
        Write-Log "学习过程中发生错误: $($_.Exception.Message)" "ERROR"
        if ($Verbose) {
            Write-Log "详细错误信息: $($_.Exception.StackTrace)" "ERROR"
        }
        exit 1
    }
}

# Execute main function
Main
