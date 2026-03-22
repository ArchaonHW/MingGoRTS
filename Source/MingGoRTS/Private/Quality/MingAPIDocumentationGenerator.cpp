// Copyright (c) 2026 MingGoRTS. All rights reserved.
// API Documentation Generator Implementation - B3-4

#include "Quality/MingAPIDocumentationGenerator.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogAPIDoc, Log, All);

UMingAPIDocumentationGenerator::UMingAPIDocumentationGenerator()
{
}

void UMingAPIDocumentationGenerator::InitializeGenerator(const FDocumentationConfig& Config)
{
    this->Config = Config;

    UE_LOG(LogAPIDoc, Log, TEXT("API Documentation Generator initialized"));
    UE_LOG(LogAPIDoc, Log, TEXT("Output format: %s"), *UEnum::GetValueAsString(Config.OutputFormat));
    UE_LOG(LogAPIDoc, Log, TEXT("Output path: %s"), *Config.OutputPath);

    // Initialize developer guide structure
    DeveloperGuide.GuideTitle = FString::Printf(TEXT("%s Developer Guide"), *Config.ProjectName);
    DeveloperGuide.Introduction = TEXT("Welcome to the developer guide.");
}

void UMingAPIDocumentationGenerator::ShutdownGenerator()
{
    CancelScheduledGeneration();
    UE_LOG(LogAPIDoc, Log, TEXT("API Documentation Generator shutdown"));
}

bool UMingAPIDocumentationGenerator::GenerateDocumentation()
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating documentation..."));

    ParseSourceFiles();

    bool bSuccess = true;

    if (Config.DocumentTypes.Contains(EDocumentType::APICode))
    {
        bSuccess &= GenerateAPIDocumentation();
    }

    if (Config.DocumentTypes.Contains(EDocumentType::DeveloperGuide))
    {
        bSuccess &= GenerateDeveloperGuide();
    }

    if (Config.DocumentTypes.Contains(EDocumentType::UserManual))
    {
        bSuccess &= GenerateUserManual();
    }

    if (bSuccess)
    {
        OnDocumentationGenerated.Broadcast(Config.OutputPath);
        UE_LOG(LogAPIDoc, Log, TEXT("Documentation generated successfully"));
    }
    else
    {
        UE_LOG(LogAPIDoc, Error, TEXT("Documentation generation failed"));
    }

    return bSuccess;
}

bool UMingAPIDocumentationGenerator::GenerateAPIDocumentation()
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating API documentation..."));

    FString OutputPath = FPaths::Combine(Config.OutputPath, TEXT("API"));
    IFileManager::Get().MakeDirectory(*OutputPath, true);

    switch (Config.OutputFormat)
    {
    case EDocumentationFormat::Markdown:
        GenerateMarkdownOutput(OutputPath);
        break;
    case EDocumentationFormat::HTML:
        GenerateHTMLOutput(OutputPath);
        break;
    case EDocumentationFormat::PDF:
        GeneratePDFOutput(OutputPath);
        break;
    case EDocumentationFormat::XML:
        GenerateXMLOutput(OutputPath);
        break;
    case EDocumentationFormat::JSON:
        GenerateJSONOutput(OutputPath);
        break;
    case EDocumentationFormat::Doxygen:
        GenerateDoxygenOutput(OutputPath);
        break;
    default:
        GenerateMarkdownOutput(OutputPath);
        break;
    }

    if (Config.bGenerateIndex)
    {
        CreateIndexFile(OutputPath);
    }

    UE_LOG(LogAPIDoc, Log, TEXT("API documentation generated at: %s"), *OutputPath);

    return true;
}

bool UMingAPIDocumentationGenerator::GenerateDeveloperGuide()
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating developer guide..."));

    FString OutputPath = FPaths::Combine(Config.OutputPath, TEXT("DeveloperGuide.md"));

    GenerateDeveloperGuideMarkdown(OutputPath);

    UE_LOG(LogAPIDoc, Log, TEXT("Developer guide generated at: %s"), *OutputPath);

    return true;
}

bool UMingAPIDocumentationGenerator::GenerateUserManual()
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating user manual..."));

    FString OutputPath = FPaths::Combine(Config.OutputPath, TEXT("UserManual.md"));

    FString Content = TEXT("# User Manual\n\n");
    Content += FString::Printf(TEXT("## %s v%s\n\n"), *Config.ProjectName, *Config.ProjectVersion);
    Content += TEXT("### Table of Contents\n\n");
    Content += TEXT("1. Getting Started\n");
    Content += TEXT("2. Installation\n");
    Content += TEXT("3. Basic Usage\n");
    Content += TEXT("4. Advanced Features\n");
    Content += TEXT("5. Troubleshooting\n\n");

    Content += TEXT("## Getting Started\n\n");
    Content += TEXT("Welcome to MingGoRTS! This guide will help you get started.\n\n");

    Content += TEXT("## Installation\n\n");
    Content += TEXT("1. Download the latest release\n");
    Content += TEXT("2. Run the installer\n");
    Content += TEXT("3. Follow the setup wizard\n\n");

    FFileHelper::SaveStringToFile(Content, *OutputPath);

    UE_LOG(LogAPIDoc, Log, TEXT("User manual generated at: %s"), *OutputPath);

    return true;
}

void UMingAPIDocumentationGenerator::AddDocumentSection(const FDocumentSection& Section)
{
    DeveloperGuide.Sections.Add(Section);
    OnGuideSectionAdded.Broadcast(Section);

    UE_LOG(LogAPIDoc, Log, TEXT("Added document section: %s"), *Section.SectionTitle);
}

void UMingAPIDocumentationGenerator::AddCodeExample(const FCodeExample& Example)
{
    CodeExamples.Add(Example);
    DeveloperGuide.Examples.Add(Example);
    OnExampleAdded.Broadcast(Example);

    UE_LOG(LogAPIDoc, Log, TEXT("Added code example: %s"), *Example.Title);
}

FAPIDocumentation UMingAPIDocumentationGenerator::ExtractDocumentation(const FString& FilePath, const FString& SymbolName)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return FAPIDocumentation();
    }

    FAPIDocumentation Doc;
    Doc.SymbolName = SymbolName;
    Doc.FilePath = FilePath;
    Doc.Description = TEXT("Documentation extracted from source code.");

    // Extract comments above the symbol
    TArray<FString> Lines;
    FileContent.ParseIntoArray(Lines, TEXT("\n"), true);

    for (int32 i = 0; i < Lines.Num(); ++i)
    {
        if (Lines[i].Contains(SymbolName))
        {
            Doc.LineNumber = i + 1;
            FString Comments = ExtractComments(FileContent, i);
            Doc.Description = ParseDescription(Comments);
            Doc.DetailedDescription = ParseDetailedDescription(Comments);
            Doc.Parameters = ParseParameters(Comments);
            Doc.ReturnDescription = ParseReturnDescription(Comments);
            Doc.Examples = ParseExamples(Comments);
            Doc.SeeAlso = ParseSeeAlso(Comments);

            // Detect symbol type
            if (Lines[i].Contains(TEXT("class ")) || Lines[i].Contains(TEXT("UCLASS")))
            {
                Doc.Type = ESymbolType::Class;
            }
            else if (Lines[i].Contains(TEXT("struct ")) || Lines[i].Contains(TEXT("USTRUCT")))
            {
                Doc.Type = ESymbolType::Struct;
            }
            else if (Lines[i].Contains(TEXT("enum ")) || Lines[i].Contains(TEXT("UENUM")))
            {
                Doc.Type = ESymbolType::Enum;
            }
            else if (Lines[i].Contains(TEXT("UFUNCTION")) || (Lines[i].Contains(TEXT("void ")) && Lines[i].Contains(TEXT("("))))
            {
                Doc.Type = ESymbolType::Function;
            }

            break;
        }
    }

    DocumentedSymbols.Add(Doc);
    OnSymbolDocumented.Broadcast(Doc);

    return Doc;
}

TArray<FAPIDocumentation> UMingAPIDocumentationGenerator::ExtractAllDocumentation(const FString& SourcePath)
{
    TArray<FAPIDocumentation> AllDocs;

    TArray<FString> SourceFiles;
    IFileManager::Get().FindFilesRecursive(SourceFiles, *SourcePath, TEXT("*.h"), true, false);

    UE_LOG(LogAPIDoc, Log, TEXT("Extracting documentation from %d files"), SourceFiles.Num());

    int32 Progress = 0;
    for (const auto& File : SourceFiles)
    {
        FString FileContent;
        if (FFileHelper::LoadFileToString(FileContent, *File))
        {
            // Find all UCLASS, USTRUCT, UENUM, UFUNCTION declarations
            TArray<FString> Lines;
            FileContent.ParseIntoArray(Lines, TEXT("\n"), true);

            for (int32 i = 0; i < Lines.Num(); ++i)
            {
                if (Lines[i].Contains(TEXT("UCLASS")) || Lines[i].Contains(TEXT("USTRUCT")) ||
                    Lines[i].Contains(TEXT("UENUM")) || Lines[i].Contains(TEXT("UFUNCTION")))
                {
                    // Extract symbol name from next line
                    if (i + 1 < Lines.Num())
                    {
                        FString SymbolLine = Lines[i + 1];
                        // Simple parsing to extract name
                        FString SymbolName = SymbolLine.TrimStartAndEnd();

                        FAPIDocumentation Doc = ExtractDocumentation(File, SymbolName);
                        if (!Doc.SymbolName.IsEmpty())
                        {
                            AllDocs.Add(Doc);
                        }
                    }
                }
            }
        }

        Progress++;
        float Percent = static_cast<float>(Progress) / SourceFiles.Num() * 100.0f;
        NotifyProgress(Percent);
    }

    UE_LOG(LogAPIDoc, Log, TEXT("Extracted %d documented symbols"), AllDocs.Num());

    return AllDocs;
}

bool UMingAPIDocumentationGenerator::ExportToFormat(const FString& OutputPath, EDocumentationFormat Format)
{
    EDocumentationFormat OldFormat = Config.OutputFormat;
    Config.OutputFormat = Format;

    bool bSuccess = GenerateDocumentation();

    Config.OutputFormat = OldFormat;

    return bSuccess;
}

void UMingAPIDocumentationGenerator::SetTemplate(const FString& TemplatePath)
{
    Config.TemplatePath = TemplatePath;
    UE_LOG(LogAPIDoc, Log, TEXT("Template set: %s"), *TemplatePath);
}

void UMingAPIDocumentationGenerator::UpdateConfiguration(const FDocumentationConfig& NewConfig)
{
    Config = NewConfig;
    UE_LOG(LogAPIDoc, Log, TEXT("Configuration updated"));
}

FString UMingAPIDocumentationGenerator::GenerateQuickReference()
{
    FString QuickRef = TEXT("# Quick Reference\n\n");

    QuickRef += TEXT("## Common Classes\n\n");
    for (const auto& Doc : DocumentedSymbols)
    {
        if (Doc.Type == ESymbolType::Class && !Doc.bDeprecated)
        {
            QuickRef += FString::Printf(TEXT("### %s\n"), *Doc.SymbolName);
            QuickRef += FString::Printf(TEXT("%s\n\n"), *Doc.Description);
        }
    }

    QuickRef += TEXT("## Common Functions\n\n");
    for (const auto& Doc : DocumentedSymbols)
    {
        if (Doc.Type == ESymbolType::Function && !Doc.bDeprecated)
        {
            QuickRef += FString::Printf(TEXT("### %s\n"), *Doc.SymbolName);
            QuickRef += FString::Printf(TEXT("%s\n\n"), *Doc.Description);
        }
    }

    return QuickRef;
}

FString UMingAPIDocumentationGenerator::GenerateTroubleshootingGuide()
{
    FString Guide = TEXT("# Troubleshooting Guide\n\n");

    Guide += TEXT("## Common Issues\n\n");
    Guide += TEXT("### Build Errors\n\n");
    Guide += TEXT("**Problem:** Build fails with linker errors\n");
    Guide += TEXT("**Solution:** Ensure all dependencies are properly linked. Check module dependencies in Build.cs files.\n\n");

    Guide += TEXT("### Runtime Errors\n\n");
    Guide += TEXT("**Problem:** Null pointer exceptions\n");
    Guide += TEXT("**Solution:** Always check for null before dereferencing. Use UE_LOG for debugging.\n\n");

    Guide += TEXT("### Performance Issues\n\n");
    Guide += TEXT("**Problem:** Low frame rate\n");
    Guide += TEXT("**Solution:** Use profiling tools to identify bottlenecks. Consider object pooling and LOD systems.\n\n");

    Guide += TEXT("## Getting Help\n\n");
    Guide += TEXT("- Check the developer documentation\n");
    Guide += TEXT("- Review code examples\n");
    Guide += TEXT("- Contact the development team\n");

    return Guide;
}

void UMingAPIDocumentationGenerator::ValidateDocumentation()
{
    UE_LOG(LogAPIDoc, Log, TEXT("Validating documentation..."));

    int32 CompleteCount = 0;
    int32 IncompleteCount = 0;

    for (const auto& Doc : DocumentedSymbols)
    {
        if (IsDocumentationComplete(Doc))
        {
            CompleteCount++;
        }
        else
        {
            IncompleteCount++;
            UE_LOG(LogAPIDoc, Warning, TEXT("Incomplete documentation: %s"), *Doc.SymbolName);
        }
    }

    UE_LOG(LogAPIDoc, Log, TEXT("Documentation validation: %d complete, %d incomplete"),
        CompleteCount, IncompleteCount);
}

TArray<FString> UMingAPIDocumentationGenerator::FindUndocumentedSymbols()
{
    TArray<FString> Undocumented;

    for (const auto& Doc : DocumentedSymbols)
    {
        if (Doc.Description.IsEmpty() || Doc.Description == TEXT("Documentation extracted from source code."))
        {
            Undocumented.Add(Doc.SymbolName);
        }
    }

    UE_LOG(LogAPIDoc, Log, TEXT("Found %d undocumented symbols"), Undocumented.Num());

    return Undocumented;
}

TArray<FString> UMingAPIDocumentationGenerator::FindOutdatedDocumentation()
{
    TArray<FString> Outdated;

    // Would check if code has changed since documentation was generated
    UE_LOG(LogAPIDoc, Log, TEXT("Checking for outdated documentation..."));

    return Outdated;
}

void UMingAPIDocumentationGenerator::ScheduleAutoGeneration(float IntervalHours)
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            AutoGenerationTimer,
            this,
            &UMingAPIDocumentationGenerator::GenerateDocumentation,
            IntervalHours * 3600.0f,
            true
        );

        UE_LOG(LogAPIDoc, Log, TEXT("Scheduled auto-generation every %.1f hours"), IntervalHours);
    }
}

void UMingAPIDocumentationGenerator::CancelScheduledGeneration()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(AutoGenerationTimer);
    }

    UE_LOG(LogAPIDoc, Log, TEXT("Cancelled scheduled generation"));
}

void UMingAPIDocumentationGenerator::PreviewDocumentation()
{
    UE_LOG(LogAPIDoc, Log, TEXT("Previewing documentation..."));

    // Would launch a preview server or open in browser
    FString PreviewPath = FPaths::Combine(Config.OutputPath, TEXT("index.html"));
    if (FPaths::FileExists(PreviewPath))
    {
        UE_LOG(LogAPIDoc, Log, TEXT("Documentation preview available at: %s"), *PreviewPath);
    }
    else
    {
        UE_LOG(LogAPIDoc, Warning, TEXT("No preview available. Generate documentation first."));
    }
}

int32 UMingAPIDocumentationGenerator::GetDocumentedSymbolCount() const
{
    return DocumentedSymbols.Num();
}

int32 UMingAPIDocumentationGenerator::GetUndocumentedSymbolCount() const
{
    return FindUndocumentedSymbols().Num();
}

void UMingAPIDocumentationGenerator::ParseSourceFiles()
{
    UE_LOG(LogAPIDoc, Log, TEXT("Parsing source files..."));

    for (const auto& SourcePath : Config.SourcePaths)
    {
        ExtractAllDocumentation(SourcePath);
    }

    UE_LOG(LogAPIDoc, Log, TEXT("Parsed %d symbols"), DocumentedSymbols.Num());
}

void UMingAPIDocumentationGenerator::ParseFile(const FString& FilePath)
{
    UE_LOG(LogAPIDoc, Verbose, TEXT("Parsing file: %s"), *FilePath);
    // Implementation in ExtractDocumentation
}

FAPIDocumentation UMingAPIDocumentationGenerator::ExtractSymbolDocumentation(
    const FString& FileContent, const FString& SymbolName, ESymbolType Type)
{
    return ExtractDocumentation(TEXT(""), SymbolName);
}

FString UMingAPIDocumentationGenerator::ExtractComments(const FString& FileContent, int32 LineNumber)
{
    TArray<FString> Lines;
    FileContent.ParseIntoArray(Lines, TEXT("\n"), true);

    FString Comments;

    // Go backwards to find comment block
    for (int32 i = LineNumber - 1; i >= 0; --i)
    {
        FString Line = Lines[i].TrimStartAndEnd();

        if (Line.StartsWith(TEXT("//")))
        {
            Comments = Line.RightChop(2).TrimStart() + TEXT(" ") + Comments;
        }
        else if (Line.StartsWith(TEXT("*")))
        {
            Comments = Line.RightChop(1).TrimStart() + TEXT(" ") + Comments;
        }
        else if (Line.StartsWith(TEXT("/*")))
        {
            Comments = Line.RightChop(2).TrimStart() + TEXT(" ") + Comments;
            break;
        }
        else if (!Line.IsEmpty())
        {
            break;
        }
    }

    return Comments.TrimStartAndEnd();
}

FString UMingAPIDocumentationGenerator::ParseDescription(const FString& Comment)
{
    // First sentence is the brief description
    int32 PeriodIndex = Comment.Find(TEXT("."));
    if (PeriodIndex != INDEX_NONE)
    {
        return Comment.Left(PeriodIndex + 1);
    }
    return Comment;
}

FString UMingAPIDocumentationGenerator::ParseDetailedDescription(const FString& Comment)
{
    // Everything after first sentence
    int32 PeriodIndex = Comment.Find(TEXT("."));
    if (PeriodIndex != INDEX_NONE && PeriodIndex + 1 < Comment.Len())
    {
        return Comment.Mid(PeriodIndex + 1).TrimStart();
    }
    return TEXT("");
}

TMap<FString, FString> UMingAPIDocumentationGenerator::ParseParameters(const FString& Comment)
{
    TMap<FString, FString> Params;

    // Look for @param tags
    TArray<FString> Lines;
    Comment.ParseIntoArray(Lines, TEXT("\n"), true);

    for (const auto& Line : Lines)
    {
        if (Line.Contains(TEXT("@param")))
        {
            FString ParamText = Line;
            ParamText.ReplaceInline(TEXT("@param"), TEXT(""));
            ParamText = ParamText.TrimStart();

            int32 SpaceIndex = ParamText.Find(TEXT(" "));
            if (SpaceIndex != INDEX_NONE)
            {
                FString ParamName = ParamText.Left(SpaceIndex);
                FString ParamDesc = ParamText.Mid(SpaceIndex + 1);
                Params.Add(ParamName, ParamDesc);
            }
        }
    }

    return Params;
}

FString UMingAPIDocumentationGenerator::ParseReturnDescription(const FString& Comment)
{
    // Look for @return tag
    TArray<FString> Lines;
    Comment.ParseIntoArray(Lines, TEXT("\n"), true);

    for (const auto& Line : Lines)
    {
        if (Line.Contains(TEXT("@return")))
        {
            FString ReturnText = Line;
            ReturnText.ReplaceInline(TEXT("@return"), TEXT(""));
            return ReturnText.TrimStart();
        }
    }

    return TEXT("");
}

TArray<FString> UMingAPIDocumentationGenerator::ParseExamples(const FString& Comment)
{
    TArray<FString> Examples;

    // Look for @example tags
    TArray<FString> Lines;
    Comment.ParseIntoArray(Lines, TEXT("\n"), true);

    for (const auto& Line : Lines)
    {
        if (Line.Contains(TEXT("@example")))
        {
            FString ExampleText = Line;
            ExampleText.ReplaceInline(TEXT("@example"), TEXT(""));
            Examples.Add(ExampleText.TrimStart());
        }
    }

    return Examples;
}

TArray<FString> UMingAPIDocumentationGenerator::ParseSeeAlso(const FString& Comment)
{
    TArray<FString> SeeAlso;

    // Look for @see tags
    TArray<FString> Lines;
    Comment.ParseIntoArray(Lines, TEXT("\n"), true);

    for (const auto& Line : Lines)
    {
        if (Line.Contains(TEXT("@see")))
        {
            FString SeeText = Line;
            SeeText.ReplaceInline(TEXT("@see"), TEXT(""));
            SeeAlso.Add(SeeText.TrimStart());
        }
    }

    return SeeAlso;
}

bool UMingAPIDocumentationGenerator::IsDocumentationComplete(const FAPIDocumentation& Doc)
{
    return !Doc.Description.IsEmpty() && !Doc.SymbolName.IsEmpty();
}

void UMingAPIDocumentationGenerator::GenerateMarkdownOutput(const FString& OutputPath)
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating Markdown output..."));

    for (const auto& Doc : DocumentedSymbols)
    {
        FString FileName = FString::Printf(TEXT("%s.md"), *Doc.SymbolName);
        FString FilePath = FPaths::Combine(OutputPath, FileName);

        FString Content = GenerateSymbolMarkdown(Doc);
        FFileHelper::SaveStringToFile(Content, *FilePath);
    }
}

void UMingAPIDocumentationGenerator::GenerateHTMLOutput(const FString& OutputPath)
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating HTML output..."));

    // Generate HTML documentation
    for (const auto& Doc : DocumentedSymbols)
    {
        FString FileName = FString::Printf(TEXT("%s.html"), *Doc.SymbolName);
        FString FilePath = FPaths::Combine(OutputPath, FileName);

        FString Content = GenerateSymbolHTML(Doc);
        FFileHelper::SaveStringToFile(Content, *FilePath);
    }
}

void UMingAPIDocumentationGenerator::GeneratePDFOutput(const FString& OutputPath)
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating PDF output (requires external tool)..."));
    // Would require external tool like pandoc
}

void UMingAPIDocumentationGenerator::GenerateXMLOutput(const FString& OutputPath)
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating XML output..."));

    FString XML = TEXT("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<api>\n");

    for (const auto& Doc : DocumentedSymbols)
    {
        XML += FString::Printf(TEXT("  <symbol name=\"%s\" type=\"%s\">\n"),
            *Doc.SymbolName, *UEnum::GetValueAsString(Doc.Type));
        XML += FString::Printf(TEXT("    <description>%s</description>\n"), *Doc.Description);
        XML += FString::Printf(TEXT("    <file>%s</file>\n"), *Doc.FilePath);
        XML += FString::Printf(TEXT("    <line>%d</line>\n"), Doc.LineNumber);
        XML += TEXT("  </symbol>\n");
    }

    XML += TEXT("</api>\n");

    FString FilePath = FPaths::Combine(OutputPath, TEXT("api.xml"));
    FFileHelper::SaveStringToFile(XML, *FilePath);
}

void UMingAPIDocumentationGenerator::GenerateJSONOutput(const FString& OutputPath)
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating JSON output..."));

    FString JSON = TEXT("{\n  \"api\": {\n");
    JSON += FString::Printf(TEXT("    \"project\": \"%s\",\n"), *Config.ProjectName);
    JSON += FString::Printf(TEXT("    \"version\": \"%s\",\n"), *Config.ProjectVersion);
    JSON += TEXT("    \"symbols\": [\n");

    for (int32 i = 0; i < DocumentedSymbols.Num(); ++i)
    {
        const auto& Doc = DocumentedSymbols[i];
        JSON += TEXT("      {\n");
        JSON += FString::Printf(TEXT("        \"name\": \"%s\",\n"), *Doc.SymbolName);
        JSON += FString::Printf(TEXT("        \"type\": \"%s\",\n"), *UEnum::GetValueAsString(Doc.Type));
        JSON += FString::Printf(TEXT("        \"description\": \"%s\"\n"), *Doc.Description);
        JSON += TEXT("      }");
        JSON += (i < DocumentedSymbols.Num() - 1) ? TEXT(",\n") : TEXT("\n");
    }

    JSON += TEXT("    ]\n  }\n}\n");

    FString FilePath = FPaths::Combine(OutputPath, TEXT("api.json"));
    FFileHelper::SaveStringToFile(JSON, *FilePath);
}

void UMingAPIDocumentationGenerator::GenerateDoxygenOutput(const FString& OutputPath)
{
    UE_LOG(LogAPIDoc, Log, TEXT("Generating Doxygen configuration..."));

    // Generate Doxyfile configuration
    FString Doxyfile = TEXT("PROJECT_NAME = \"" + Config.ProjectName + TEXT("\"\n"));
    Doxyfile += TEXT("OUTPUT_DIRECTORY = \"" + OutputPath + TEXT("\"\n"));
    Doxyfile += TEXT("INPUT = ");

    for (const auto& SourcePath : Config.SourcePaths)
    {
        Doxyfile += SourcePath + TEXT(" ");
    }

    Doxyfile += TEXT("\n");
    Doxyfile += TEXT("RECURSIVE = YES\n");
    Doxyfile += TEXT("EXTRACT_ALL = YES\n");
    Doxyfile += TEXT("GENERATE_HTML = YES\n");
    Doxyfile += TEXT("GENERATE_LATEX = NO\n");

    FString FilePath = FPaths::Combine(OutputPath, TEXT("Doxyfile"));
    FFileHelper::SaveStringToFile(Doxyfile, *FilePath);
}

void UMingAPIDocumentationGenerator::GenerateDeveloperGuideMarkdown(const FString& OutputPath)
{
    FString Content = TEXT("# " + DeveloperGuide.GuideTitle + TEXT("\n\n"));
    Content += DeveloperGuide.Introduction + TEXT("\n\n");

    // Sort sections by order
    DeveloperGuide.Sections.Sort([](const FDocumentSection& A, const FDocumentSection& B) {
        return A.Order < B.Order;
    });

    for (const auto& Section : DeveloperGuide.Sections)
    {
        Content += FString::Printf(TEXT("## %s\n\n"), *Section.SectionTitle);
        Content += Section.SectionContent + TEXT("\n\n");
    }

    if (DeveloperGuide.Examples.Num() > 0)
    {
        Content += TEXT("## Code Examples\n\n");

        for (const auto& Example : DeveloperGuide.Examples)
        {
            Content += FString::Printf(TEXT("### %s\n\n"), *Example.Title);
            Content += Example.Description + TEXT("\n\n");
            Content += TEXT("```") + Example.Language + TEXT("\n");
            Content += Example.Code + TEXT("\n");
            Content += TEXT("```\n\n");
        }
    }

    FFileHelper::SaveStringToFile(Content, *OutputPath);
}

FString UMingAPIDocumentationGenerator::GenerateSymbolMarkdown(const FAPIDocumentation& Doc)
{
    FString Content = FString::Printf(TEXT("# %s\n\n"), *Doc.SymbolName);

    Content += FString::Printf(TEXT("**Type:** %s\n\n"), *UEnum::GetValueAsString(Doc.Type));
    Content += FString::Printf(TEXT("**Defined in:** %s (line %d)\n\n"), *Doc.FilePath, Doc.LineNumber);

    if (Doc.bDeprecated)
    {
        Content += FString::Printf(TEXT("> **Deprecated:** %s\n\n"), *Doc.DeprecatedMessage);
    }

    if (Doc.bExperimental)
    {
        Content += TEXT("> **Experimental:** This feature is experimental and subject to change.\n\n");
    }

    Content += FString::Printf(TEXT("## Description\n\n%s\n\n"), *Doc.Description);

    if (!Doc.DetailedDescription.IsEmpty())
    {
        Content += FString::Printf(TEXT("%s\n\n"), *Doc.DetailedDescription);
    }

    if (Doc.Parameters.Num() > 0)
    {
        Content += TEXT("## Parameters\n\n");
        Content += TEXT("| Parameter | Description |\n");
        Content += TEXT("|-----------|-------------|\n");

        for (const auto& Param : Doc.Parameters)
        {
            Content += FString::Printf(TEXT("| %s | %s |\n"), *Param, *Doc.ParameterDescriptions.FindRef(Param));
        }

        Content += TEXT("\n");
    }

    if (!Doc.ReturnDescription.IsEmpty())
    {
        Content += FString::Printf(TEXT("## Returns\n\n%s\n\n"), *Doc.ReturnDescription);
    }

    if (Doc.Examples.Num() > 0)
    {
        Content += TEXT("## Examples\n\n");

        for (const auto& Example : Doc.Examples)
        {
            Content += FString::Printf(TEXT("```cpp\n%s\n```\n\n"), *Example);
        }
    }

    if (Doc.SeeAlso.Num() > 0)
    {
        Content += TEXT("## See Also\n\n");

        for (const auto& See : Doc.SeeAlso)
        {
            Content += FString::Printf(TEXT("- %s\n"), *See);
        }

        Content += TEXT("\n");
    }

    return Content;
}

FString UMingAPIDocumentationGenerator::GenerateSymbolHTML(const FAPIDocumentation& Doc)
{
    FString Content = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    Content += TEXT("<title>") + Doc.SymbolName + TEXT(" - API Documentation</title>\n");
    Content += TEXT("<style>\n");
    Content += TEXT("body { font-family: Arial, sans-serif; margin: 40px; }\n");
    Content += TEXT("h1 { color: #333; }\n");
    Content += TEXT(".deprecated { background: #ffcccc; padding: 10px; border-left: 4px solid #cc0000; }\n");
    Content += TEXT(".experimental { background: #fff3cd; padding: 10px; border-left: 4px solid #ffc107; }\n");
    Content += TEXT("code { background: #f4f4f4; padding: 2px 5px; }\n");
    Content += TEXT("pre { background: #f4f4f4; padding: 15px; overflow-x: auto; }\n");
    Content += TEXT("table { border-collapse: collapse; width: 100%; }\n");
    Content += TEXT("th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n");
    Content += TEXT("th { background-color: #f2f2f2; }\n");
    Content += TEXT("</style>\n</head>\n<body>\n");

    Content += FString::Printf(TEXT("<h1>%s</h1>\n"), *Doc.SymbolName);
    Content += FString::Printf(TEXT("<p><strong>Type:</strong> %s</p>\n"), *UEnum::GetValueAsString(Doc.Type));
    Content += FString::Printf(TEXT("<p><strong>Defined in:</strong> %s (line %d)</p>\n"), *Doc.FilePath, Doc.LineNumber);

    if (Doc.bDeprecated)
    {
        Content += FString::Printf(TEXT("<div class=\"deprecated\"><strong>Deprecated:</strong> %s</div>\n"), *Doc.DeprecatedMessage);
    }

    if (Doc.bExperimental)
    {
        Content += TEXT("<div class=\"experimental\"><strong>Experimental:</strong> This feature is experimental and subject to change.</div>\n");
    }

    Content += FString::Printf(TEXT("<h2>Description</h2>\n<p>%s</p>\n"), *Doc.Description);

    if (!Doc.DetailedDescription.IsEmpty())
    {
        Content += FString::Printf(TEXT("<p>%s</p>\n"), *Doc.DetailedDescription);
    }

    if (Doc.Parameters.Num() > 0)
    {
        Content += TEXT("<h2>Parameters</h2>\n<table>\n<tr><th>Parameter</th><th>Description</th></tr>\n");

        for (const auto& Param : Doc.Parameters)
        {
            Content += FString::Printf(TEXT("<tr><td>%s</td><td>%s</td></tr>\n"),
                *Param, *Doc.ParameterDescriptions.FindRef(Param));
        }

        Content += TEXT("</table>\n");
    }

    if (!Doc.ReturnDescription.IsEmpty())
    {
        Content += FString::Printf(TEXT("<h2>Returns</h2>\n<p>%s</p>\n"), *Doc.ReturnDescription);
    }

    if (Doc.Examples.Num() > 0)
    {
        Content += TEXT("<h2>Examples</h2>\n");

        for (const auto& Example : Doc.Examples)
        {
            Content += FString::Printf(TEXT("<pre><code>%s</code></pre>\n"), *Example);
        }
    }

    if (Doc.SeeAlso.Num() > 0)
    {
        Content += TEXT("<h2>See Also</h2>\n<ul>\n");

        for (const auto& See : Doc.SeeAlso)
        {
            Content += FString::Printf(TEXT("<li>%s</li>\n"), *See);
        }

        Content += TEXT("</ul>\n");
    }

    Content += TEXT("</body>\n</html>");

    return Content;
}

void UMingAPIDocumentationGenerator::CreateIndexFile(const FString& OutputPath)
{
    FString IndexContent = TEXT("# API Index\n\n");
    IndexContent += TEXT("## Classes\n\n");

    for (const auto& Doc : DocumentedSymbols)
    {
        if (Doc.Type == ESymbolType::Class)
        {
            IndexContent += FString::Printf(TEXT("- [%s](%s.md) - %s\n"),
                *Doc.SymbolName, *Doc.SymbolName, *Doc.Description.Left(100));
        }
    }

    IndexContent += TEXT("\n## Functions\n\n");

    for (const auto& Doc : DocumentedSymbols)
    {
        if (Doc.Type == ESymbolType::Function)
        {
            IndexContent += FString::Printf(TEXT("- [%s](%s.md) - %s\n"),
                *Doc.SymbolName, *Doc.SymbolName, *Doc.Description.Left(100));
        }
    }

    FString IndexPath = FPaths::Combine(OutputPath, TEXT("index.md"));
    FFileHelper::SaveStringToFile(IndexContent, *IndexPath);

    UE_LOG(LogAPIDoc, Log, TEXT("Created index file: %s"), *IndexPath);
}

void UMingAPIDocumentationGenerator::CopyAssets(const FString& OutputPath)
{
    if (!Config.LogoPath.IsEmpty() && FPaths::FileExists(Config.LogoPath))
    {
        FString DestPath = FPaths::Combine(OutputPath, TEXT("logo.png"));
        IFileManager::Get().Copy(*DestPath, *Config.LogoPath);
    }
}

void UMingAPIDocumentationGenerator::NotifyProgress(float Progress)
{
    OnGenerationProgress.Broadcast(Progress);
}

void UMingAPIDocumentationGenerator::LogGenerationActivity(const FString& Activity)
{
    UE_LOG(LogAPIDoc, Log, TEXT("[DOC] %s"), *Activity);
}

static UMingAPIDocumentationGenerator* UMingAPIDocumentationGenerator::Get(UObject* WorldContextObject)
{
    static UMingAPIDocumentationGenerator* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingAPIDocumentationGenerator>();
        Instance->AddToRoot();
    }
    return Instance;
}
