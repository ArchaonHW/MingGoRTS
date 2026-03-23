# MingGoRTS C++ Script Execution Engine

## Overview

The MingGoRTS C++ Script Execution Engine is a comprehensive replacement for all PowerShell and Python scripts in the project. It provides a unified, high-performance C++ implementation that integrates seamlessly with Unreal Engine 5.

## Features

### 🚀 Core Capabilities
- **Unified Script Management**: Centralized script registry and execution
- **Multiple Execution Modes**: Sequential, Parallel, Pipeline, Conditional, Event-Driven, Smart
- **Real-time Progress Tracking**: Live execution progress and status updates
- **Comprehensive Error Handling**: Robust error recovery and reporting
- **Blueprint Integration**: Full Blueprint support for all functionality
- **Event-Driven Architecture**: Extensive event system for notifications

### 📂 Script Categories
- **AI Scripts**: Asset generation, music creation, epic content generation
- **Build Scripts**: Project compilation, syntax error fixing, health checking
- **API Scripts**: Documentation generation, API enhancement, script integration
- **Testing Scripts**: Comprehensive test suite execution
- **Maintenance Scripts**: Encoding fixes, text cleanup, convention checking

### ⚡ Performance Features
- **Asynchronous Execution**: Non-blocking script execution
- **Parallel Processing**: Multi-threaded script execution
- **Smart Caching**: Avoid redundant executions
- **Resource Management**: Optimized memory and CPU usage
- **Progressive Loading**: Load scripts on-demand

## Architecture

### Core Components

#### UMingScriptExecutionEngine
The main engine class that orchestrates all script execution.

```cpp
// Initialize the engine
UMingScriptExecutionEngine* ScriptEngine = NewObject<UMingScriptExecutionEngine>();
ScriptEngine->InitializeScriptEngine();

// Execute a script
FString Result = ScriptEngine->ExecuteScript(TEXT("CompileProject"));

// Execute with parameters
TArray<FString> Parameters;
Parameters.Add(TEXT("Development"));
FString Result = ScriptEngine->ExecuteScript(TEXT("CompileProject"), Parameters);
```

#### Script Registry
Central repository for all available scripts with metadata.

```cpp
// Get scripts by category
TArray<FScriptMetadata> BuildScripts = ScriptEngine->GetScriptsByCategory(EScriptCategory::Build);

// Get specific script metadata
FScriptMetadata Metadata = ScriptEngine->GetScriptMetadata(TEXT("CompileProject"));

// Check if script is enabled
bool bEnabled = ScriptEngine->IsScriptEnabled(TEXT("CompileProject"));
```

#### Execution Tracking
Real-time tracking of script execution with detailed results.

```cpp
// Get execution status
FScriptExecutionResult Status = ScriptEngine->GetExecutionStatus(TaskID);

// Check if execution is complete
bool bComplete = ScriptEngine->IsExecutionComplete(TaskID);

// Get execution progress
float Progress = ScriptEngine->GetExecutionProgress(TaskID);
```

## Script Types

### AI Asset Generation Scripts

#### GenerateAIMusic
Generates AI music based on description and duration.

```cpp
FString Description = TEXT("Epic Ming dynasty battle music");
float Duration = 120.0f;
FString Result = ScriptEngine->GenerateAIMusic(Description, Duration);
```

#### GenerateAIAsset
Generates various types of AI assets.

```cpp
FString AssetType = TEXT("Image");
FString Description = TEXT("Ming dynasty palace interior");
FString OutputPath = TEXT("/Game/Generated/Assets/");
FString Result = ScriptEngine->GenerateAIAsset(AssetType, Description, OutputPath);
```

#### GenerateEpicAssets
Generates comprehensive epic-level asset sets.

```cpp
FString EpicID = TEXT("Epic001");
TArray<FString> AssetTypes = { TEXT("Image"), TEXT("Music"), TEXT("Video") };
FString Result = ScriptEngine->GenerateEpicAssets(EpicID, AssetTypes);
```

### Build System Scripts

#### CompileProject
Compiles the Unreal Engine project with specified configuration.

```cpp
FString Configuration = TEXT("Development");
FString Result = ScriptEngine->CompileProject(Configuration);
```

#### FixSyntaxErrors
Automatically fixes common syntax errors in the codebase.

```cpp
FString Result = ScriptEngine->FixSyntaxErrors();
```

#### CheckProjectHealth
Performs comprehensive project health analysis.

```cpp
FString Result = ScriptEngine->CheckProjectHealth();
```

### API Management Scripts

#### GenerateAPIDocumentation
Generates comprehensive API documentation.

```cpp
FString Result = ScriptEngine->GenerateAPIDocumentation();
```

#### EnhanceAPI
Enhances the API with additional features.

```cpp
FString EnhancementType = TEXT("BlueprintIntegration");
FString Result = ScriptEngine->EnhanceAPI(EnhancementType);
```

#### IntegrateAllScripts
Integrates all script systems into a unified whole.

```cpp
FString Result = ScriptEngine->IntegrateAllScripts();
```

### Testing Scripts

#### RunTests
Executes comprehensive test suites.

```cpp
FString TestCategory = TEXT("All");
FString Result = ScriptEngine->RunTests(TestCategory);
```

#### RunCharacterCreationTests
Executes character creation system tests.

```cpp
FString Result = ScriptEngine->RunCharacterCreationTests();
```

#### RunDialogueSystemTests
Executes dialogue system tests.

```cpp
FString Result = ScriptEngine->RunDialogueSystemTests();
```

#### RunRelationshipNetworkTests
Executes relationship network tests.

```cpp
FString Result = ScriptEngine->RunRelationshipNetworkTests();
```

#### RunHistoricalRoleplayTests
Executes historical roleplay tests.

```cpp
FString Result = ScriptEngine->RunHistoricalRoleplayTests();
```

#### RunRTScoreTests
Executes RTS core system tests.

```cpp
FString Result = ScriptEngine->RunRTScoreTests();
```

#### RunSkillSystemTests
Executes skill system tests.

```cpp
FString Result = ScriptEngine->RunSkillSystemTests();
```

### Maintenance Scripts

#### FixEncodingIssues
Fixes encoding issues in source files.

```cpp
FString Result = ScriptEngine->FixEncodingIssues();
```

#### FixGarbledText
Fixes garbled text in project files.

```cpp
FString Result = ScriptEngine->FixGarbledText();
```

#### CheckNamingConventions
Checks and enforces naming conventions.

```cpp
FString Result = ScriptEngine->CheckNamingConventions();
```

## Execution Modes

### Sequential Execution
Scripts execute one after another. If one fails, execution stops.

```cpp
FString TaskID = ScriptEngine->ExecuteScriptsByCategory(EScriptCategory::Build, EScriptExecutionMode::Sequential);
```

### Parallel Execution
Scripts execute simultaneously for maximum efficiency.

```cpp
FString TaskID = ScriptEngine->ExecuteScriptsByCategory(EScriptCategory::Testing, EScriptExecutionMode::Parallel);
```

### Pipeline Execution
Output of one script becomes input for the next.

```cpp
FString TaskID = ScriptEngine->ExecuteScriptsByCategory(EScriptCategory::AI, EScriptExecutionMode::Pipeline);
```

### Smart Execution
Automatically determines optimal execution strategy.

```cpp
FString TaskID = ScriptEngine->ExecuteAllScripts(EScriptExecutionMode::Smart);
```

## Event System

### Script Events
The engine provides comprehensive event notifications:

```cpp
// Bind to events
ScriptEngine->OnScriptStarted.AddDynamic(this, &AMyActor::OnScriptStarted);
ScriptEngine->OnScriptCompleted.AddDynamic(this, &AMyActor::OnScriptCompleted);
ScriptEngine->OnScriptFailed.AddDynamic(this, &AMyActor::OnScriptFailed);
ScriptEngine->OnExecutionProgress.AddDynamic(this, &AMyActor::OnExecutionProgress);
ScriptEngine->OnExecutionSummary.AddDynamic(this, &AMyActor::OnExecutionSummary);

// Event handlers
UFUNCTION()
void AMyActor::OnScriptStarted(const FString& ScriptName, const FScriptMetadata& Metadata, const FString& TaskID);

UFUNCTION()
void AMyActor::OnScriptCompleted(const FString& ScriptName, const FScriptExecutionResult& Result, const FString& TaskID);

UFUNCTION()
void AMyActor::OnScriptFailed(const FString& ScriptName, const FString& ErrorMessage, const FString& TaskID);

UFUNCTION()
void AMyActor::OnExecutionProgress(const FString& TaskID, float Progress);

UFUNCTION()
void AMyActor::OnExecutionSummary(const FScriptExecutionSummary& Summary);
```

## Configuration

### Engine Configuration
Configure the execution engine behavior:

```cpp
// Set maximum concurrent executions
ScriptEngine->SetMaxConcurrentExecutions(8);

// Set default timeout
ScriptEngine->SetDefaultTimeout(600.0f);

// Set default execution mode
ScriptEngine->SetExecutionMode(EScriptExecutionMode::Smart);
```

### Script Configuration
Enable/disable specific scripts:

```cpp
// Check if script is enabled
bool bEnabled = ScriptEngine->IsScriptEnabled(TEXT("CompileProject"));

// Enable/disable script
ScriptEngine->SetScriptEnabled(TEXT("CompileProject"), true);
```

## Reporting

### Execution Reports
Generate detailed HTML reports of script execution:

```cpp
// Generate execution report
FString TaskID = ScriptEngine->ExecuteAllScripts();
FString ReportContent = ScriptEngine->GenerateExecutionReport(TaskID);

// Save report to file
FString LogPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Logs/ExecutionReport.html"));
ScriptEngine->SaveExecutionLog(TaskID, LogPath);
```

### Report Contents
HTML reports include:
- Execution timeline
- Script results and status
- Performance metrics
- Error details
- Success/failure statistics

## Integration Examples

### Game Mode Integration
Integrate the script engine into your game mode:

```cpp
class AMingGameMode : public AGameModeBase
{
protected:
    UPROPERTY()
    TObjectPtr<UMingScriptExecutionEngine> ScriptEngine;

    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        
        // Initialize script engine
        ScriptEngine = NewObject<UMingScriptExecutionEngine>();
        ScriptEngine->InitializeScriptEngine();
        
        // Bind events
        ScriptEngine->OnScriptCompleted.AddDynamic(this, &AMingGameMode::OnScriptCompleted);
    }

    UFUNCTION()
    void OnScriptCompleted(const FString& ScriptName, const FScriptExecutionResult& Result, const FString& TaskID)
    {
        // Handle script completion
        UE_LOG(LogTemp, Log, TEXT("Script completed: %s"), *ScriptName);
    }

public:
    // Generate game content
    UFUNCTION(BlueprintCallable)
    void GenerateGameContent()
    {
        // Generate epic assets
        ScriptEngine->GenerateEpicAssets(TEXT("MainCampaign"), { TEXT("Image"), TEXT("Music") });
        
        // Run tests
        ScriptEngine->RunTests(TEXT("All"));
        
        // Check project health
        ScriptEngine->CheckProjectHealth();
    }
};
```

### Blueprint Integration
Use the script engine in Blueprints:

1. Add Script Engine component to your Actor
2. In the Construction Script, initialize the engine
3. Use the script execution functions in event graphs
4. Bind to script events for notifications

## Performance Considerations

### Optimization Tips
- Use parallel execution for independent scripts
- Enable smart execution for automatic optimization
- Monitor execution progress to avoid blocking
- Use appropriate timeout values for long-running scripts

### Resource Management
- The engine automatically manages memory and resources
- Script results are cached when appropriate
- Concurrent executions are limited to prevent resource exhaustion
- Background tasks don't block the main thread

## Migration Guide

### From PowerShell
Replace PowerShell script calls with C++ equivalents:

```cpp
// Old: .\Tools\build\compile_project.ps1
// New: ScriptEngine->CompileProject(TEXT("Development"));

// Old: .\Tools\ai\generate_music.ps1 -Preset "Epic"
// New: ScriptEngine->GenerateAIMusic(TEXT("Epic battle music"), 120.0f);
```

### From Python
Replace Python script calls with C++ equivalents:

```cpp
// Old: python Tools\ai\epic_asset_generator.py --epic "Epic001"
// New: ScriptEngine->GenerateEpicAssets(TEXT("Epic001"));

// Old: python Tools\build\project_health_check.py
// New: ScriptEngine->CheckProjectHealth();
```

## Troubleshooting

### Common Issues
1. **Scripts not found**: Ensure the script engine is initialized
2. **Execution timeouts**: Increase timeout values for long operations
3. **Memory issues**: Limit concurrent executions
4. **Permission errors**: Ensure proper file access rights

### Debug Information
Enable verbose logging for detailed execution information:

```cpp
// Engine logs provide detailed information about script execution
// Check the output log for:
// - Script discovery results
// - Execution progress
// - Error details
// - Performance metrics
```

## Best Practices

### Script Organization
- Group related scripts by category
- Use descriptive script names
- Provide clear descriptions
- Set appropriate priorities

### Error Handling
- Always check execution results
- Implement proper error recovery
- Use event notifications for async operations
- Log important execution events

### Performance
- Use parallel execution when possible
- Monitor resource usage
- Implement proper cleanup
- Cache results when appropriate

## Future Enhancements

### Planned Features
- **Web Interface**: Browser-based script management
- **Distributed Execution**: Multi-machine script execution
- **Advanced Scheduling**: Time-based script scheduling
- **Machine Learning**: ML-powered script optimization
- **Plugin System**: Extensible plugin architecture

### API Extensions
- REST API for remote script execution
- WebSocket support for real-time updates
- GraphQL interface for complex queries
- gRPC for high-performance communication

## Conclusion

The MingGoRTS C++ Script Execution Engine provides a comprehensive, high-performance replacement for all PowerShell and Python scripts in the project. It offers:

- **Unified Interface**: Single point of access for all script functionality
- **High Performance**: Native C++ execution with optimized resource management
- **Full Integration**: Seamless integration with Unreal Engine 5
- **Extensive Features**: Comprehensive script management and execution capabilities
- **Future-Proof**: Extensible architecture for future enhancements

This engine represents a significant improvement in maintainability, performance, and reliability over the previous script-based approach.

---

**Last Updated**: 2026-03-23  
**Version**: 1.0  
**Status**: Production Ready
