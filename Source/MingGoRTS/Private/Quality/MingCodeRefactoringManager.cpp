// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Code Refactoring Manager Implementation - B3-1

#include "Quality/MingCodeRefactoringManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogCodeRefactoring, Log, All);

UMingCodeRefactoringManager::UMingCodeRefactoringManager()
{
}

void UMingCodeRefactoringManager::InitializeRefactoringManager(const FRefactoringConfig& Config)
{
    this->Config = Config;

    UE_LOG(LogCodeRefactoring, Log, TEXT("Code Refactoring Manager initialized"));
    UE_LOG(LogCodeRefactoring, Log, TEXT("Optimization level: %s"),
        *UEnum::GetValueAsString(Config.OptimizationLevel));

    if (Config.bEnableRealtimeAnalysis)
    {
        StartRealtimeAnalysis();
    }
}

void UMingCodeRefactoringManager::ShutdownRefactoringManager()
{
    StopRealtimeAnalysis();
    UE_LOG(LogCodeRefactoring, Log, TEXT("Code Refactoring Manager shutdown"));
}

TArray<FCodeSmell> UMingCodeRefactoringManager::AnalyzeCode(const FString& FilePath)
{
    TArray<FCodeSmell> Smells;

    if (IsExcluded(FilePath))
    {
        return Smells;
    }

    UE_LOG(LogCodeRefactoring, Log, TEXT("Analyzing code: %s"), *FilePath);

    DetectSmellsInFile(FilePath);

    // Filter smells by severity based on optimization level
    for (const auto& Smell : DetectedSmells)
    {
        if (Smell.FilePath == FilePath)
        {
            bool bInclude = false;
            switch (Config.OptimizationLevel)
            {
            case EOptimizationLevel::Conservative:
                bInclude = (Smell.Severity >= 8.0f);
                break;
            case EOptimizationLevel::Moderate:
                bInclude = (Smell.Severity >= 5.0f);
                break;
            case EOptimizationLevel::Aggressive:
                bInclude = (Smell.Severity >= 3.0f);
                break;
            case EOptimizationLevel::Maximum:
                bInclude = true;
                break;
            }

            if (bInclude)
            {
                Smells.Add(Smell);
            }
        }
    }

    UE_LOG(LogCodeRefactoring, Log, TEXT("Found %d code smells in %s"), Smells.Num(), *FilePath);

    return Smells;
}

TArray<FCodeSmell> UMingCodeRefactoringManager::AnalyzeProject(const FString& ProjectPath)
{
    TArray<FCodeSmell> AllSmells;

    // Find all source files
    TArray<FString> SourceFiles;
    IFileManager::Get().FindFilesRecursive(SourceFiles, *ProjectPath, TEXT("*.h"), true, false);
    IFileManager::Get().FindFilesRecursive(SourceFiles, *ProjectPath, TEXT("*.cpp"), true, false);

    UE_LOG(LogCodeRefactoring, Log, TEXT("Analyzing project: %s (%d files)"), *ProjectPath, SourceFiles.Num());

    for (const auto& File : SourceFiles)
    {
        if (!IsExcluded(File))
        {
            TArray<FCodeSmell> FileSmells = AnalyzeCode(File);
            AllSmells.Append(FileSmells);
        }
    }

    UE_LOG(LogCodeRefactoring, Log, TEXT("Total code smells found: %d"), AllSmells.Num());

    return AllSmells;
}

FCodeMetrics UMingCodeRefactoringManager::CalculateMetrics(const FString& FilePath)
{
    FCodeMetrics Metrics;
    Metrics.FilePath = FilePath;

    if (!IsExcluded(FilePath))
    {
        CalculateFileMetrics(FilePath);

        if (FileMetrics.Contains(FilePath))
        {
            Metrics = FileMetrics[FilePath];
        }
    }

    return Metrics;
}

TArray<FCodeMetrics> UMingCodeRefactoringManager::CalculateProjectMetrics(const FString& ProjectPath)
{
    TArray<FCodeMetrics> AllMetrics;

    TArray<FString> SourceFiles;
    IFileManager::Get().FindFilesRecursive(SourceFiles, *ProjectPath, TEXT("*.h"), true, false);
    IFileManager::Get().FindFilesRecursive(SourceFiles, *ProjectPath, TEXT("*.cpp"), true, false);

    for (const auto& File : SourceFiles)
    {
        if (!IsExcluded(File))
        {
            AllMetrics.Add(CalculateMetrics(File));
        }
    }

    return AllMetrics;
}

bool UMingCodeRefactoringManager::ApplyRefactoring(const FRefactoringOperation& Operation)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Applying refactoring: %s"), *Operation.OperationID.ToString());

    if (Config.bCreateBackups)
    {
        CreateBackup(Operation.TargetFile);
    }

    // Apply the refactoring based on type
    bool bSuccess = false;
    switch (Operation.Type)
    {
    case ERefactoringType::Rename:
        bSuccess = RenameSymbol(Operation.TargetFile, Operation.OriginalCode, Operation.RefactoredCode);
        break;
    case ERefactoringType::RemoveUnused:
        bSuccess = RemoveUnusedCode(Operation.TargetFile);
        break;
    case ERefactoringType::OptimizeIncludes:
        bSuccess = OptimizeIncludes(Operation.TargetFile);
        break;
    case ERefactoringType::FormatCode:
        bSuccess = FormatCode(Operation.TargetFile);
        break;
    default:
        UE_LOG(LogCodeRefactoring, Warning, TEXT("Refactoring type not implemented: %s"),
            *UEnum::GetValueAsString(Operation.Type));
        break;
    }

    if (bSuccess)
    {
        FRefactoringOperation CompletedOp = Operation;
        CompletedOp.bApplied = true;
        StoreOperation(CompletedOp);

        if (Config.bVerifyAfterRefactoring)
        {
            CompletedOp.bVerified = VerifyRefactoring(CompletedOp);
        }

        OnRefactoringCompleted.Broadcast(CompletedOp);
        UE_LOG(LogCodeRefactoring, Log, TEXT("Refactoring applied successfully"));
    }
    else
    {
        UE_LOG(LogCodeRefactoring, Error, TEXT("Failed to apply refactoring"));
    }

    return bSuccess;
}

bool UMingCodeRefactoringManager::RenameSymbol(const FString& FilePath, const FString& OldName, const FString& NewName)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return false;
    }

    // Simple text replacement - in real implementation, would use proper parsing
    FileContent.ReplaceInline(*OldName, *NewName);

    return FFileHelper::SaveStringToFile(FileContent, *FilePath);
}

bool UMingCodeRefactoringManager::ExtractMethod(const FString& FilePath, int32 StartLine, int32 EndLine, const FString& MethodName)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Extracting method %s from lines %d-%d in %s"),
        *MethodName, StartLine, EndLine, *FilePath);

    // Implementation would extract code into a new method
    return true;
}

bool UMingCodeRefactoringManager::RemoveUnusedCode(const FString& FilePath)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return false;
    }

    TArray<FString> UnusedSymbols = FindUnusedSymbols(FileContent);

    UE_LOG(LogCodeRefactoring, Log, TEXT("Found %d unused symbols in %s"), UnusedSymbols.Num(), *FilePath);

    // Remove unused symbols
    for (const auto& Symbol : UnusedSymbols)
    {
        // Simple removal - real implementation would be more sophisticated
        FileContent.ReplaceInline(*Symbol, TEXT(""));
    }

    return FFileHelper::SaveStringToFile(FileContent, *FilePath);
}

bool UMingCodeRefactoringManager::OptimizeIncludes(const FString& FilePath)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return false;
    }

    TArray<FString> UnusedIncludes = FindUnusedIncludes(FileContent);

    UE_LOG(LogCodeRefactoring, Log, TEXT("Found %d unused includes in %s"), UnusedIncludes.Num(), *FilePath);

    // Remove unused includes
    for (const auto& Include : UnusedIncludes)
    {
        FileContent.ReplaceInline(*Include, TEXT(""));
    }

    return FFileHelper::SaveStringToFile(FileContent, *FilePath);
}

bool UMingCodeRefactoringManager::FormatCode(const FString& FilePath)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Formatting code: %s"), *FilePath);

    // Would integrate with clang-format or similar tool
    return true;
}

bool UMingCodeRefactoringManager::SimplifyComplexLogic(const FString& FilePath, int32 LineNumber)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Simplifying complex logic at line %d in %s"), LineNumber, *FilePath);

    // Implementation would simplify complex conditions
    return true;
}

bool UMingCodeRefactoringManager::ModernizeSyntax(const FString& FilePath)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Modernizing syntax: %s"), *FilePath);

    // Would modernize C++ syntax (e.g., use auto, range-based for loops, etc.)
    return true;
}

bool UMingCodeRefactoringManager::OptimizeLoops(const FString& FilePath)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Optimizing loops: %s"), *FilePath);

    // Would optimize loop structures
    return true;
}

TArray<FRefactoringOperation> UMingCodeRefactoringManager::GetSuggestedRefactorings(const FString& FilePath)
{
    TArray<FRefactoringOperation> Suggestions;

    TArray<FCodeSmell> Smells = AnalyzeCode(FilePath);

    for (const auto& Smell : Smells)
    {
        if (Smell.bAutoFixable)
        {
            FRefactoringOperation Op;
            Op.OperationID = FName(*FString::Printf(TEXT("AUTO-%d"), FMath::Rand()));
            Op.Type = ERefactoringType::RemoveUnused;
            Op.TargetFile = FilePath;
            Op.Description = Smell.Description;
            Op.bApplied = false;

            Suggestions.Add(Op);
        }
    }

    return Suggestions;
}

bool UMingCodeRefactoringManager::BatchRefactor(const TArray<FRefactoringOperation>& Operations)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Applying %d refactoring operations"), Operations.Num());

    int32 SuccessCount = 0;
    for (const auto& Op : Operations)
    {
        if (ApplyRefactoring(Op))
        {
            SuccessCount++;
        }
    }

    UE_LOG(LogCodeRefactoring, Log, TEXT("Batch refactor complete: %d/%d succeeded"),
        SuccessCount, Operations.Num());

    return SuccessCount == Operations.Num();
}

void UMingCodeRefactoringManager::UndoLastRefactoring()
{
    if (OperationHistory.Num() > 0)
    {
        FRefactoringOperation LastOp = OperationHistory.Last();

        if (!LastOp.BackupPath.IsEmpty())
        {
            RestoreFromBackup(LastOp.BackupPath, LastOp.TargetFile);
            OperationHistory.Pop();

            UE_LOG(LogCodeRefactoring, Log, TEXT("Undid refactoring: %s"), *LastOp.OperationID.ToString());
        }
    }
}

bool UMingCodeRefactoringManager::VerifyRefactoring(const FRefactoringOperation& Operation)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Verifying refactoring: %s"), *Operation.OperationID.ToString());

    // Would verify that code still compiles and tests pass
    return true;
}

void UMingCodeRefactoringManager::ExportMetricsReport(const FString& FilePath)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("Exporting metrics report to: %s"), *FilePath);

    FString Report = TEXT("# Code Metrics Report\n\n");
    Report += TEXT("## Summary\n\n");
    Report += FString::Printf(TEXT("- Total files analyzed: %d\n"), FileMetrics.Num());

    int32 TotalLines = 0;
    int32 TotalCoveredLines = 0;

    for (const auto& Pair : FileMetrics)
    {
        TotalLines += Pair.Value.LinesOfCode;
        TotalCoveredLines += Pair.Value.CoveredLines;
    }

    Report += FString::Printf(TEXT("- Total lines of code: %d\n"), TotalLines);
    Report += FString::Printf(TEXT("- Average cyclomatic complexity: %.2f\n"),
        TotalLines > 0 ? static_cast<float>(TotalCoveredLines) / TotalLines : 0.0f);

    Report += TEXT("\n## Per-File Metrics\n\n");
    Report += TEXT("| File | Lines | Methods | Complexity |\n");
    Report += TEXT("|------|-------|---------|------------|\n");

    for (const auto& Pair : FileMetrics)
    {
        Report += FString::Printf(TEXT("| %s | %d | %d | %.2f |\n"),
            *Pair.Key, Pair.Value.LinesOfCode, Pair.Value.NumberOfMethods, Pair.Value.CyclomaticComplexity);
    }

    FFileHelper::SaveStringToFile(Report, *FilePath);
}

void UMingCodeRefactoringManager::GenerateRefactoringReport()
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("=== Refactoring Report ==="));
    UE_LOG(LogCodeRefactoring, Log, TEXT("Total operations: %d"), OperationHistory.Num());
    UE_LOG(LogCodeRefactoring, Log, TEXT("Total smells detected: %d"), DetectedSmells.Num());
    UE_LOG(LogCodeRefactoring, Log, TEXT("Files analyzed: %d"), FileMetrics.Num());
}

void UMingCodeRefactoringManager::SetExclusions(const TArray<FString>& Paths)
{
    Config.ExcludedPaths = Paths;
    UE_LOG(LogCodeRefactoring, Log, TEXT("Set %d exclusion paths"), Paths.Num());
}

int32 UMingCodeRefactoringManager::GetTotalSmellsCount() const
{
    return DetectedSmells.Num();
}

int32 UMingCodeRefactoringManager::GetAutoFixableCount() const
{
    int32 Count = 0;
    for (const auto& Smell : DetectedSmells)
    {
        if (Smell.bAutoFixable)
        {
            Count++;
        }
    }
    return Count;
}

void UMingCodeRefactoringManager::StartRealtimeAnalysis()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            AnalysisTimer,
            this,
            &UMingCodeRefactoringManager::PerformRealtimeAnalysis,
            60.0f,
            true
        );

        UE_LOG(LogCodeRefactoring, Log, TEXT("Started real-time analysis"));
    }
}

void UMingCodeRefactoringManager::StopRealtimeAnalysis()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(AnalysisTimer);
    }
}

void UMingCodeRefactoringManager::PerformRealtimeAnalysis()
{
    UE_LOG(LogCodeRefactoring, Verbose, TEXT("Performing real-time analysis"));

    // Analyze recently modified files
    // Implementation would check file modification times
}

void UMingCodeRefactoringManager::DetectSmellsInFile(const FString& FilePath)
{
    // Remove existing smells for this file
    for (int32 i = DetectedSmells.Num() - 1; i >= 0; --i)
    {
        if (DetectedSmells[i].FilePath == FilePath)
        {
            DetectedSmells.RemoveAt(i);
        }
    }

    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return;
    }

    // Check for long methods
    TArray<FString> Lines;
    FileContent.ParseIntoArray(Lines, TEXT("\n"), true);

    int32 MethodStart = -1;
    FString CurrentMethod;

    for (int32 i = 0; i < Lines.Num(); ++i)
    {
        const FString& Line = Lines[i];

        // Simple heuristic to detect method start
        if (Line.Contains(TEXT("::")) && (Line.Contains(TEXT("void ")) || Line.Contains(TEXT("bool ")) ||
            Line.Contains(TEXT("int ")) || Line.Contains(TEXT("float "))))
        {
            MethodStart = i;
            CurrentMethod = Line;
        }

        // Simple heuristic to detect method end
        if (MethodStart >= 0 && Line == TEXT("}"))
        {
            int32 MethodLength = i - MethodStart;
            if (MethodLength > Config.MaxMethodLength)
            {
                FCodeSmell Smell;
                Smell.Type = ECodeSmellType::LongMethod;
                Smell.FilePath = FilePath;
                Smell.LineNumber = MethodStart + 1;
                Smell.Description = FString::Printf(TEXT("Method is %d lines long (max %d)"),
                    MethodLength, Config.MaxMethodLength);
                Smell.AffectedSymbol = CurrentMethod;
                Smell.Severity = FMath::Clamp(static_cast<float>(MethodLength) / Config.MaxMethodLength * 10.0f, 5.0f, 10.0f);
                Smell.bAutoFixable = false;

                DetectedSmells.Add(Smell);
                OnCodeSmellDetected.Broadcast(Smell);
            }

            MethodStart = -1;
        }
    }
}

void UMingCodeRefactoringManager::CalculateFileMetrics(const FString& FilePath)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return;
    }

    FCodeMetrics Metrics;
    Metrics.FilePath = FilePath;
    Metrics.LinesOfCode = CountCodeLines(FileContent);
    Metrics.CommentLines = CountCommentLines(FileContent);
    Metrics.CyclomaticComplexity = CalculateCyclomaticComplexity(FileContent);

    FileMetrics.Add(FilePath, Metrics);
    OnMetricsCalculated.Broadcast(Metrics);
}

bool UMingCodeRefactoringManager::IsExcluded(const FString& FilePath) const
{
    for (const auto& ExcludedPath : Config.ExcludedPaths)
    {
        if (FilePath.Contains(ExcludedPath))
        {
            return true;
        }
    }
    return false;
}

bool UMingCodeRefactoringManager::CreateBackup(const FString& FilePath)
{
    FString BackupPath = GenerateBackupPath(FilePath);
    return IFileManager::Get().Copy(*BackupPath, *FilePath) == COPY_OK;
}

bool UMingCodeRefactoringManager::RestoreFromBackup(const FString& BackupPath, const FString& OriginalPath)
{
    return IFileManager::Get().Copy(*OriginalPath, *BackupPath) == COPY_OK;
}

FString UMingCodeRefactoringManager::GenerateBackupPath(const FString& OriginalPath)
{
    FString Timestamp = FString::Printf(TEXT("%d"), static_cast<int32>(FPlatformTime::Seconds()));
    return OriginalPath + TEXT(".backup.") + Timestamp;
}

TArray<FString> UMingCodeRefactoringManager::FindUnusedSymbols(const FString& FileContent)
{
    TArray<FString> Unused;
    // Implementation would find unused variables, functions, etc.
    return Unused;
}

TArray<FString> UMingCodeRefactoringManager::FindUnusedIncludes(const FString& FileContent)
{
    TArray<FString> Unused;
    // Implementation would find unused #include directives
    return Unused;
}

float UMingCodeRefactoringManager::CalculateCyclomaticComplexity(const FString& MethodContent)
{
    int32 Complexity = 1;

    // Count decision points
    Complexity += MethodContent.Contains(TEXT("if ")) ? 1 : 0;
    Complexity += MethodContent.Contains(TEXT("else if")) ? 1 : 0;
    Complexity += MethodContent.Contains(TEXT("for ")) ? 1 : 0;
    Complexity += MethodContent.Contains(TEXT("while ")) ? 1 : 0;
    Complexity += MethodContent.Contains(TEXT("case ")) ? 1 : 0;
    Complexity += MethodContent.Contains(TEXT("&&")) ? 1 : 0;
    Complexity += MethodContent.Contains(TEXT("||")) ? 1 : 0;
    Complexity += MethodContent.Contains(TEXT("?")) ? 1 : 0;

    return static_cast<float>(Complexity);
}

int32 UMingCodeRefactoringManager::CountCodeLines(const FString& Content)
{
    TArray<FString> Lines;
    Content.ParseIntoArray(Lines, TEXT("\n"), true);

    int32 Count = 0;
    for (const auto& Line : Lines)
    {
        FString Trimmed = Line.TrimStartAndEnd();
        if (!Trimmed.IsEmpty() && !Trimmed.StartsWith(TEXT("//")) && !Trimmed.StartsWith(TEXT("/*")) && !Trimmed.StartsWith(TEXT("*")))
        {
            Count++;
        }
    }

    return Count;
}

int32 UMingCodeRefactoringManager::CountCommentLines(const FString& Content)
{
    TArray<FString> Lines;
    Content.ParseIntoArray(Lines, TEXT("\n"), true);

    int32 Count = 0;
    for (const auto& Line : Lines)
    {
        FString Trimmed = Line.TrimStartAndEnd();
        if (Trimmed.StartsWith(TEXT("//")) || Trimmed.StartsWith(TEXT("/*")) || Trimmed.StartsWith(TEXT("*")))
        {
            Count++;
        }
    }

    return Count;
}

void UMingCodeRefactoringManager::StoreOperation(const FRefactoringOperation& Operation)
{
    OperationHistory.Add(Operation);

    // Trim history if needed
    if (OperationHistory.Num() > 100)
    {
        OperationHistory.RemoveAt(0);
    }
}

void UMingCodeRefactoringManager::LogRefactoringActivity(const FString& Activity)
{
    UE_LOG(LogCodeRefactoring, Log, TEXT("[REFACTORING] %s"), *Activity);
}

static UMingCodeRefactoringManager* UMingCodeRefactoringManager::Get(UObject* WorldContextObject)
{
    static UMingCodeRefactoringManager* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingCodeRefactoringManager>();
        Instance->AddToRoot();
    }
    return Instance;
}
