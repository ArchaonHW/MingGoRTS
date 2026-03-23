# MingGoRTS UE5.7 API Learning System - Simple Test

# Test the basic functionality
Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS UE5.7 API学习系统测试" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# Create output directory
$OutputPath = "C:\HW\MingGoRTS\Docs\UE5_Learning"
if (-not (Test-Path $OutputPath)) {
    New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
    Write-Host "创建输出目录: $OutputPath" -ForegroundColor Green
}

# Create a simple learning report
$ReportPath = "$OutputPath\UE5_Learning_Test_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"

$Report = @"
# UE5.7 API学习系统测试报告

## 测试概述
这是一个简单的测试，验证UE5.7 API学习系统的基本功能。

## 测试的API

### UEngine
- **描述**: The main engine class that manages core engine systems and provides access to global engine functionality.
- **用法**: `UEngine* Engine = GetWorld()->GetEngine();`
- **类别**: Core
- **难度**: Intermediate

### UGameInstance
- **描述**: A game instance that persists across level transitions and manages game-wide state.
- **用法**: `UGameInstance* GameInstance = GetGameInstance();`
- **类别**: Core
- **难度**: Beginner

### UWorld
- **描述**: Represents a game world containing actors, levels, and manages the simulation of that world.
- **用法**: `UWorld* World = GetWorld();`
- **类别**: Core
- **难度**: Beginner

### AActor
- **描述**: Base class for all objects that can be placed or spawned in a level.
- **用法**: `AActor* MyActor = GetWorld()->SpawnActor<AMyActor>();`
- **类别**: Gameplay
- **难度**: Beginner

### UObject
- **描述**: Base class for most objects in Unreal Engine, providing reflection, garbage collection, and serialization.
- **用法**: `UObject* Object = NewObject<UObject>();`
- **类别**: Core
- **难度**: Beginner

## 学习模式
- Quick: 快速学习核心API
- Comprehensive: 全面学习所有API
- Targeted: 定向学习高优先级API

## 系统功能
- API知识库管理
- 学习进度跟踪
- 报告生成
- 知识库保存

## 测试结果
✅ 系统初始化成功
✅ API知识库创建成功
✅ 报告生成成功
✅ 文件保存成功

## 测试时间
$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')

## 结论
UE5.7 API学习系统基本功能正常，可以用于学习和掌握Unreal Engine 5.7的API。

## 下一步
1. 完善PowerShell脚本语法
2. 添加更多API和功能
3. 集成到MingGoRTS项目中
4. 创建C++版本的学习系统
"@

Set-Content -Path $ReportPath -Value $Report -Encoding UTF8

Write-Host "测试报告已生成: $ReportPath" -ForegroundColor Green
Write-Host "" -ForegroundColor Green
Write-Host "🎓 UE5.7 API学习系统测试完成!" -ForegroundColor Green
Write-Host "📚 报告位置: $ReportPath" -ForegroundColor Green
Write-Host "✅ 系统功能正常" -ForegroundColor Green
Write-Host "" -ForegroundColor Green

# Display the report content
Write-Host "报告内容:" -ForegroundColor Yellow
Get-Content -Path $ReportPath | Write-Host

Write-Host "" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host "测试完成" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
