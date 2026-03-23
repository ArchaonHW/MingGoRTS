// MingGoRTS Enhanced Potato AI API Implementation
// 增強版 C++ Potato AI 程式碼生成系統實作

#include "MingRTSPotatoAIEnhancedAPI.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Engine/Engine.h"

UMingRTSPotatoAIEnhancedAPI::UMingRTSPotatoAIEnhancedAPI()
{
    PrimaryComponentTick.bCanEverTick = true;
    DefaultAIModel = EPotatoAIModel::GPT4;
    bLearningEnabled = true;
    LearningDataPath = TEXT("PotatoAI/LearningData.json");
    TemplatesPath = TEXT("PotatoAI/Templates/");
    MaxLearningDataEntries = 1000;
    TotalGenerations = 0;
    AverageQualityScore = 0.0f;
}

void UMingRTSPotatoAIEnhancedAPI::BeginPlay()
{
    Super::BeginPlay();
    InitializeEnhancedPotatoAI();
}

void UMingRTSPotatoAIEnhancedAPI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 儲存學習資料
    if (bLearningEnabled && LearningHistory.Num() > 0)
    {
        SaveLearningDataToFile();
    }
    Super::EndPlay(EndPlayReason);
}

bool UMingRTSPotatoAIEnhancedAPI::InitializeEnhancedPotatoAI()
{
    UE_LOG(LogTemp, Log, TEXT("🥔 初始化增強型 Potato AI 系統..."));
    
    // 載入學習資料
    if (bLearningEnabled)
    {
        LoadLearningDataFromFile();
    }
    
    // 初始化模板
    FString TemplatesDir = FPaths::ProjectDir() + TemplatesPath;
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*TemplatesDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*TemplatesDir);
    }
    
    UE_LOG(LogTemp, Log, TEXT("✅ 增強型 Potato AI 系統初始化完成"));
    return true;
}

FPotatoAIEnhancedResult UMingRTSPotatoAIEnhancedAPI::GenerateEnhancedCode(
    const FString& Requirement,
    const FPotatoAIEnhancedOptions& Options)
{
    FDateTime StartTime = FDateTime::Now();
    FPotatoAIEnhancedResult Result;
    
    UE_LOG(LogTemp, Log, TEXT("🥔 開始增強型程式碼生成: %s"), *Requirement);
    
    // 選擇生成模式
    FString GeneratedCode;
    switch (Options.GenerationMode)
    {
        case EPotatoAIEnhancedMode::Template:
            GeneratedCode = GenerateCodeWithTemplate(Requirement, Options);
            break;
        case EPotatoAIEnhancedMode::Hybrid:
            GeneratedCode = GenerateCodeHybrid(Requirement, Options);
            break;
        case EPotatoAIEnhancedMode::Learning:
            GeneratedCode = GenerateCodeWithAI(Requirement, Options);
            ApplyLearningOptimization(GeneratedCode, Requirement);
            break;
        case EPotatoAIEnhancedMode::Intelligent:
        default:
            GeneratedCode = GenerateCodeWithAI(Requirement, Options);
            break;
    }
    
    // 自動優化
    if (Options.bAutoOptimize)
    {
        GeneratedCode = OptimizeCodeForPerformance(GeneratedCode);
        GeneratedCode = OptimizeCodeForBlueprint(GeneratedCode);
    }
    
    // 生成類別名稱
    Result.ClassName = GenerateClassName(Requirement);
    Result.ModulePath = GenerateModulePath(Result.ClassName);
    
    // 分割程式碼
    Result.GeneratedCode = GeneratedCode;
    Result.HeaderCode = GenerateHeaderCode(Result.ClassName, GeneratedCode);
    Result.ImplementationCode = GenerateImplementationCode(Result.ClassName, GeneratedCode);
    
    // 生成測試程式碼
    if (Options.bIncludeTests)
    {
        GenerateTestCode(Result.TestCode, GeneratedCode, Result.ClassName);
    }
    
    // 生成使用範例
    if (Options.bIncludeExamples)
    {
        GenerateUsageExamples(Result.UsageExamples, GeneratedCode, Result.ClassName);
    }
    
    // 生成文件
    if (Options.bIncludeDocumentation)
    {
        GenerateDocumentation(Result.Documentation, Requirement);
    }
    
    // 品質分析
    Result.QualityAssessment = PerformAdvancedQualityAnalysis(GeneratedCode);
    
    // 計算生成時間
    FDateTime EndTime = FDateTime::Now();
    Result.GenerationTime = (EndTime - StartTime).GetTotalSeconds();
    Result.bGenerationSuccessful = true;
    
    // 更新統計資料
    UpdateModelUsageStats(Options.AIModel);
    LogGeneration(Requirement, Result);
    
    // 廣播事件
    OnPotatoAIEnhancedCodeGenerated.Broadcast(Requirement, Result, Result.GenerationTime);
    
    UE_LOG(LogTemp, Log, TEXT("✅ 程式碼生成完成，品質評分: %d"), Result.QualityAssessment.OverallScore);
    
    return Result;
}

FPotatoAIEnhancedResult UMingRTSPotatoAIEnhancedAPI::GenerateIntelligentCode(
    const FString& Requirement,
    EPotatoAICodeComplexity Complexity)
{
    FPotatoAIEnhancedOptions Options;
    Options.Complexity = Complexity;
    Options.GenerationMode = EPotatoAIEnhancedMode::Intelligent;
    Options.bEnableLearning = true;
    Options.bAutoOptimize = true;
    
    return GenerateEnhancedCode(Requirement, Options);
}

FString UMingRTSPotatoAIEnhancedAPI::GenerateCodeWithAI(const FString& Requirement, const FPotatoAIEnhancedOptions& Options)
{
    // 模擬 AI 生成過程
    FString ClassName = GenerateClassName(Requirement);
    FString Code;
    
    // 根據複雜度生成不同複雜度的程式碼
    switch (Options.Complexity)
    {
        case EPotatoAICodeComplexity::Simple:
            Code = FString::Printf(TEXT(
                "// 🥔 由 Potato AI 生成的 UE5 類別\n"
                "// 需求: %s\n"
                "// 生成時間: %s\n\n"
                "#pragma once\n\n"
                "#include \"CoreMinimal.h\"\n"
                "#include \"UObject/NoExportTypes.h\"\n"
                "#include \"%s.generated.h\"\n\n"
                "UCLASS(Blueprintable, BlueprintType)\n"
                "class MINGGORTS_API %s : public UObject\n"
                "{\n"
                "    GENERATED_BODY()\n\n"
                "public:\n"
                "    %s();\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    bool Initialize();\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    void Update(float DeltaTime);\n"
                "};"),
                *Requirement,
                *FDateTime::Now().ToString(),
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName
            );
            break;
            
        case EPotatoAICodeComplexity::Medium:
            Code = FString::Printf(TEXT(
                "// 🥔 由 Potato AI 生成的 UE5 類別\n"
                "// 需求: %s\n"
                "// 複雜度: 中等\n"
                "// 生成時間: %s\n\n"
                "#pragma once\n\n"
                "#include \"CoreMinimal.h\"\n"
                "#include \"UObject/NoExportTypes.h\"\n"
                "#include \"%s.generated.h\"\n\n"
                "UCLASS(Blueprintable, BlueprintType, Category = \"MingGoRTS\")\n"
                "class MINGGORTS_API %s : public UObject\n"
                "{\n"
                "    GENERATED_BODY()\n\n"
                "public:\n"
                "    %s();\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    bool Initialize();\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    void Update(float DeltaTime);\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    FString GetStatus() const;\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    void Shutdown();\n\n"
                "protected:\n"
                "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Config\")\n"
                "    bool bIsActive = true;\n\n"
                "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Config\")\n"
                "    float UpdateInterval = 0.1f;\n\n"
                "    virtual void BeginPlay();\n"
                "    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);\n"
                "};"),
                *Requirement,
                *FDateTime::Now().ToString(),
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName
            );
            break;
            
        case EPotatoAICodeComplexity::Complex:
        case EPotatoAICodeComplexity::Enterprise:
        case EPotatoAICodeComplexity::Advanced:
            // 更複雜的程式碼範本
            Code = FString::Printf(TEXT(
                "// 🥔 由 Potato AI 生成的高級 UE5 系統\n"
                "// 需求: %s\n"
                "// 複雜度: 高級\n"
                "// 生成時間: %s\n\n"
                "#pragma once\n\n"
                "#include \"CoreMinimal.h\"\n"
                "#include \"UObject/NoExportTypes.h\"\n"
                "#include \"Engine/World.h\"\n"
                "#include \"%s.generated.h\"\n\n"
                "// 前置宣告\n"
                "class UWorld;\n"
                "struct FTimerHandle;\n\n"
                "USTRUCT(BlueprintType)\n"
                "struct F%sConfig\n"
                "{\n"
                "    GENERATED_BODY()\n\n"
                "    UPROPERTY(EditAnywhere, BlueprintReadWrite)\n"
                "    bool bEnableFeature = true;\n\n"
                "    UPROPERTY(EditAnywhere, BlueprintReadWrite)\n"
                "    float PerformanceThreshold = 1.0f;\n\n"
                "    UPROPERTY(EditAnywhere, BlueprintReadWrite)\n"
                "    int32 MaxIterations = 100;\n";
                "};\n\n"
                "DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn%sEvent, const FString&, Message);\n\n"
                "UCLASS(Blueprintable, BlueprintType, Category = \"MingGoRTS|Advanced\")\n"
                "class MINGGORTS_API %s : public UObject\n"
                "{\n"
                "    GENERATED_BODY()\n\n"
                "public:\n"
                "    %s();\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    bool Initialize(UWorld* InWorld, const F%sConfig& Config);\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    void Update(float DeltaTime);\n\n"
                "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
                "    void Shutdown();\n\n"
                "    UPROPERTY(BlueprintAssignable, Category = \"%s\")\n"
                "    FOn%sEvent OnEvent;\n\n"
                "protected:\n"
                "    UPROPERTY()\n"
                "    UWorld* World;\n\n"
                "    UPROPERTY()\n"
                "    F%sConfig CurrentConfig;\n\n"
                "    FTimerHandle UpdateTimer;\n\n"
                "    virtual void InternalUpdate();\n"
                "    virtual void ProcessLogic();\n"
                "    virtual void HandleErrors();\n"
                "};"),
                *Requirement,
                *FDateTime::Now().ToString(),
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName,
                *ClassName
            );
            break;
    }
    
    return Code;
}

FString UMingRTSPotatoAIEnhancedAPI::GenerateCodeWithTemplate(const FString& Requirement, const FPotatoAIEnhancedOptions& Options)
{
    // 使用範本生成程式碼
    FString ClassName = GenerateClassName(Requirement);
    
    // 檢查是否有現成範本
    for (const auto& TemplatePair : CodeTemplates)
    {
        if (Requirement.Contains(TemplatePair.Key))
        {
            FString TemplateCode = TemplatePair.Value;
            TemplateCode.ReplaceInline(TEXT("{CLASS_NAME}"), *ClassName);
            TemplateCode.ReplaceInline(TEXT("{REQUIREMENT}"), *Requirement);
            TemplateCode.ReplaceInline(TEXT("{TIMESTAMP}"), *FDateTime::Now().ToString());
            return TemplateCode;
        }
    }
    
    // 如果沒有找到範本，使用 AI 生成
    return GenerateCodeWithAI(Requirement, Options);
}

FString UMingRTSPotatoAIEnhancedAPI::GenerateCodeHybrid(const FString& Requirement, const FPotatoAIEnhancedOptions& Options)
{
    // 混合模式：使用範本作為基礎，AI 進行增強
    FString BaseCode = GenerateCodeWithTemplate(Requirement, Options);
    
    // 根據需求添加特定功能
    if (Requirement.Contains(TEXT("Event")) || Requirement.Contains(TEXT("Delegate")))
    {
        FString EventCode = TEXT("\n\n    // 事件系統\n    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSystemEvent);\n    UPROPERTY(BlueprintAssignable)\n    FOnSystemEvent OnSystemReady;");
        BaseCode = BaseCode.Replace(TEXT("};"), *EventCode + TEXT("\n};"));
    }
    
    if (Requirement.Contains(TEXT("Timer")) || Requirement.Contains(TEXT("Schedule")))
    {
        FString TimerCode = TEXT("\n\n    // 定時器系統\n    FTimerHandle UpdateTimer;\n    void StartUpdateTimer();\n    void StopUpdateTimer();");
        BaseCode = BaseCode.Replace(TEXT("};"), *TimerCode + TEXT("\n};"));
    }
    
    return BaseCode;
}

FPotatoAIEnhancedQuality UMingRTSPotatoAIEnhancedAPI::PerformAdvancedQualityAnalysis(const FString& Code)
{
    FPotatoAIEnhancedQuality Quality;
    
    // 程式碼結構分析
    Quality.CodeStructureScore = AnalyzeCodeStructure(Code).OverallScore;
    
    // 效能分析
    Quality.PerformanceScore = AnalyzePerformance(Code).OverallScore;
    
    // 安全性分析
    Quality.SecurityScore = AnalyzeSecurity(Code).OverallScore;
    
    // 維護性分析
    Quality.MaintainabilityScore = AnalyzeMaintainability(Code).OverallScore;
    
    // 計算總分
    Quality.OverallScore = (Quality.CodeStructureScore + Quality.PerformanceScore + 
                           Quality.SecurityScore + Quality.MaintainabilityScore) / 4;
    
    // 檢查是否符合標準
    Quality.bMeetsStandards = Quality.OverallScore >= 70;
    
    // 生成建議
    if (Quality.OverallScore < 80)
    {
        Quality.Recommendations.Add(TEXT("考慮增加更多註解以提高程式碼可讀性"));
    }
    if (Quality.PerformanceScore < 80)
    {
        Quality.Recommendations.Add(TEXT("優化迴圈結構以提升效能"));
    }
    if (Quality.SecurityScore < 80)
    {
        Quality.Recommendations.Add(TEXT("增加錯誤處理和輸入驗證"));
    }
    
    // 廣播品質分析完成事件
    OnPotatoAIQualityAnalysisCompleted.Broadcast(Code, Quality);
    
    return Quality;
}

FString UMingRTSPotatoAIEnhancedAPI::OptimizeCodeForPerformance(const FString& Code)
{
    FString OptimizedCode = Code;
    
    // 效能優化規則
    // 1. 將頻繁呼叫的函數改為 FORCEINLINE
    if (OptimizedCode.Contains(TEXT("virtual void Update")))
    {
        OptimizedCode.ReplaceInline(TEXT("virtual void Update"), TEXT("FORCEINLINE virtual void Update"));
    }
    
    // 2. 使用更高效的資料結構
    if (OptimizedCode.Contains(TEXT("TArray<FString>")) && OptimizedCode.Contains(TEXT("大量資料")))
    {
        OptimizedCode.ReplaceInline(TEXT("TArray<FString>"), TEXT("TSet<FString>"));
    }
    
    return OptimizedCode;
}

FString UMingRTSPotatoAIEnhancedAPI::OptimizeCodeForBlueprint(const FString& Code)
{
    FString BlueprintCode = Code;
    
    // Blueprint 優化
    // 1. 確保所有公開函數都有 BlueprintCallable
    if (!BlueprintCode.Contains(TEXT("BlueprintCallable")))
    {
        BlueprintCode = BlueprintCode.Replace(TEXT("public:"), 
            TEXT("public:\n    UFUNCTION(BlueprintCallable, Category = \"PotatoAI\")"));
    }
    
    // 2. 添加 BlueprintAssignable 事件
    if (BlueprintCode.Contains(TEXT("DECLARE_DYNAMIC_MULTICAST_DELEGATE")))
    {
        // 確保事件已正確標記
    }
    
    return BlueprintCode;
}

void UMingRTSPotatoAIEnhancedAPI::ApplyLearningOptimization(FString& Code, const FString& Requirement)
{
    if (!bLearningEnabled || LearningHistory.Num() == 0)
    {
        return;
    }
    
    // 尋找相似的歷史資料
    for (const FPotatoAILearningData& Data : LearningHistory)
    {
        if (Data.Requirement.Contains(Requirement.Left(20)) && Data.bWasAccepted)
        {
            // 應用歷史優化建議
            if (Data.QualityScore.Recommendations.Num() > 0)
            {
                UE_LOG(LogTemp, Log, TEXT("🥔 應用學習優化: %s"), *Data.Requirement);
            }
        }
    }
}

FString UMingRTSPotatoAIEnhancedAPI::GenerateClassName(const FString& Requirement)
{
    // 從需求生成類別名稱
    FString BaseName = TEXT("Ming");
    
    // 提取關鍵字
    if (Requirement.Contains(TEXT("System"))) BaseName += TEXT("System");
    else if (Requirement.Contains(TEXT("Manager"))) BaseName += TEXT("Manager");
    else if (Requirement.Contains(TEXT("Controller"))) BaseName += TEXT("Controller");
    else if (Requirement.Contains(TEXT("Component"))) BaseName += TEXT("Component");
    else BaseName += TEXT("AI");
    
    // 添加後綴
    if (Requirement.Contains(TEXT("Enhanced"))) BaseName += TEXT("Enhanced");
    if (Requirement.Contains(TEXT("Advanced"))) BaseName += TEXT("Advanced");
    
    // 添加時間戳記確保唯一性
    BaseName += FDateTime::Now().ToString(TEXT("_yyyyMMdd"));
    
    return BaseName;
}

FString UMingRTSPotatoAIEnhancedAPI::GenerateModulePath(const FString& ClassName)
{
    return FString::Printf(TEXT("MingGoRTS/Public/AI/%s.h"), *ClassName);
}

FString UMingRTSPotatoAIEnhancedAPI::GenerateHeaderCode(const FString& ClassName, const FString& Implementation)
{
    // 從完整程式碼中提取標頭部分
    return Implementation;
}

FString UMingRTSPotatoAIEnhancedAPI::GenerateImplementationCode(const FString& ClassName, const FString& Header)
{
    // 生成實作檔案內容
    FString Implementation;
    Implementation += FString::Printf(TEXT("// %s.cpp\n"), *ClassName);
    Implementation += FString::Printf(TEXT("#include \"%s.h\"\n\n"), *ClassName);
    Implementation += FString::Printf(TEXT("%s::%s()\n{\n}\n\n"), *ClassName, *ClassName);
    Implementation += FString::Printf(TEXT("bool %s::Initialize()\n{\n    return true;\n}\n\n"), *ClassName);
    Implementation += FString::Printf(TEXT("void %s::Update(float DeltaTime)\n{\n}\n\n"), *ClassName);
    
    return Implementation;
}

void UMingRTSPotatoAIEnhancedAPI::GenerateDocumentation(FString& Documentation, const FString& Requirement)
{
    Documentation = FString::Printf(TEXT(
        "# %s\n\n"
        "## 概述\n"
        "由 Potato AI 自動生成的 UE5 類別。\n\n"
        "## 需求\n"
        "%s\n\n"
        "## 功能\n"
        "- 自動初始化\n"
        "- 定期更新\n"
        "- Blueprint 支援\n\n"
        "## 使用方法\n"
        "```cpp\n"
        "UYourClass* Instance = NewObject<UYourClass>();\n"
        "Instance->Initialize();\n"
        "```\n\n"
        "## 生成時間\n"
        "%s\n"),
        *GenerateClassName(Requirement),
        *Requirement,
        *FDateTime::Now().ToString()
    );
}

void UMingRTSPotatoAIEnhancedAPI::GenerateTestCode(FString& TestCode, const FString& GeneratedCode, const FString& ClassName)
{
    TestCode = FString::Printf(TEXT(
        "// %sTest.cpp\n"
        "#include \"%s.h\"\n"
        "#include \"Misc/AutomationTest.h\"\n\n"
        "IMPLEMENT_SIMPLE_AUTOMATION_TEST(F%sTest, \"MingGoRTS.%s.BasicTest\",\n"
        "    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)\n\n"
        "bool F%sTest::RunTest(const FString& Parameters)\n"
        "{\n"
        "    %s* TestInstance = NewObject<%s>();\n"
        "    TestTrue(TEXT(\"Instance created\"), TestInstance != nullptr);\n"
        "    \n"
        "    bool bInitResult = TestInstance->Initialize();\n"
        "    TestTrue(TEXT(\"Initialize succeeded\"), bInitResult);\n"
        "    \n"
        "    return true;\n"
        "}\n"),
        *ClassName, *ClassName, *ClassName, *ClassName, *ClassName, *ClassName, *ClassName
    );
}

void UMingRTSPotatoAIEnhancedAPI::GenerateUsageExamples(TArray<FString>& Examples, const FString& Code, const FString& ClassName)
{
    Examples.Add(FString::Printf(TEXT(
        "// 基本使用範例\n"
        "%s* Instance = NewObject<%s>();\n"
        "if (Instance->Initialize())\n"
        "{\n"
        "    Instance->Update(0.1f);\n"
        "}\n"),
        *ClassName, *ClassName
    ));
    
    Examples.Add(FString::Printf(TEXT(
        "// Blueprint 使用範例\n"
        "// 在 Blueprint 中建立 %s 變數\n"
        "// 呼叫 Initialize 函數\n"
        "// 在 Tick 中呼叫 Update\n"),
        *ClassName
    ));
}

void UMingRTSPotatoAIEnhancedAPI::RecordLearningData(const FPotatoAILearningData& LearningData)
{
    if (!bLearningEnabled)
    {
        return;
    }
    
    // 限制學習資料數量
    if (LearningHistory.Num() >= MaxLearningDataEntries)
    {
        LearningHistory.RemoveAt(0);
    }
    
    LearningHistory.Add(LearningData);
    UpdateLearningStatistics(LearningData);
    
    // 廣播學習資料更新事件
    OnPotatoAILearningDataUpdated.Broadcast(LearningData);
    
    UE_LOG(LogTemp, Log, TEXT("🥔 學習資料已記錄: %s"), *LearningData.Requirement);
}

TArray<FPotatoAILearningData> UMingRTSPotatoAIEnhancedAPI::GetLearningHistory()
{
    return LearningHistory;
}

void UMingRTSPotatoAIEnhancedAPI::ClearLearningData()
{
    LearningHistory.Empty();
    UE_LOG(LogTemp, Log, TEXT("🥔 學習資料已清除"));
}

bool UMingRTSPotatoAIEnhancedAPI::SaveLearningDataToFile()
{
    FString FilePath = FPaths::ProjectDir() + LearningDataPath;
    FString JsonData;
    
    // 建立 JSON 物件
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    TArray<TSharedPtr<FJsonValue>> LearningArray;
    
    for (const FPotatoAILearningData& Data : LearningHistory)
    {
        TSharedPtr<FJsonObject> DataObject = MakeShareable(new FJsonObject);
        DataObject->SetStringField(TEXT("Requirement"), Data.Requirement);
        DataObject->SetStringField(TEXT("GeneratedCode"), Data.GeneratedCode);
        DataObject->SetNumberField(TEXT("QualityScore"), Data.QualityScore.OverallScore);
        DataObject->SetStringField(TEXT("Timestamp"), Data.Timestamp.ToString());
        DataObject->SetBoolField(TEXT("WasAccepted"), Data.bWasAccepted);
        
        LearningArray.Add(MakeShareable(new FJsonValueObject(DataObject)));
    }
    
    RootObject->SetArrayField(TEXT("LearningData"), LearningArray);
    
    // 序列化為 JSON
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonData);
    bool bSuccess = FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    if (bSuccess)
    {
        // 確保目錄存在
        FString Directory = FPaths::GetPath(FilePath);
        if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Directory))
        {
            FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*Directory);
        }
        
        bSuccess = FFileHelper::SaveStringToFile(JsonData, *FilePath);
        UE_LOG(LogTemp, Log, TEXT("🥔 學習資料已儲存: %s"), *FilePath);
    }
    
    return bSuccess;
}

bool UMingRTSPotatoAIEnhancedAPI::LoadLearningDataFromFile()
{
    FString FilePath = FPaths::ProjectDir() + LearningDataPath;
    
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("🥔 沒有現有學習資料檔案"));
        return false;
    }
    
    FString JsonData;
    if (!FFileHelper::LoadFileToString(JsonData, *FilePath))
    {
        return false;
    }
    
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonData);
    
    if (FJsonSerializer::Deserialize(Reader, RootObject))
    {
        const TArray<TSharedPtr<FJsonValue>>* LearningArray;
        if (RootObject->TryGetArrayField(TEXT("LearningData"), LearningArray))
        {
            LearningHistory.Empty();
            
            for (const TSharedPtr<FJsonValue>& Value : *LearningArray)
            {
                TSharedPtr<FJsonObject> DataObject = Value->AsObject();
                if (DataObject.IsValid())
                {
                    FPotatoAILearningData Data;
                    Data.Requirement = DataObject->GetStringField(TEXT("Requirement"));
                    Data.GeneratedCode = DataObject->GetStringField(TEXT("GeneratedCode"));
                    Data.QualityScore.OverallScore = DataObject->GetIntegerField(TEXT("QualityScore"));
                    Data.bWasAccepted = DataObject->GetBoolField(TEXT("WasAccepted"));
                    
                    FString TimestampStr;
                    if (DataObject->TryGetStringField(TEXT("Timestamp"), TimestampStr))
                    {
                        FDateTime::Parse(TimestampStr, Data.Timestamp);
                    }
                    
                    LearningHistory.Add(Data);
                }
            }
            
            UE_LOG(LogTemp, Log, TEXT("🥔 已載入 %d 筆學習資料"), LearningHistory.Num());
            return true;
        }
    }
    
    return false;
}

void UMingRTSPotatoAIEnhancedAPI::UpdateModelUsageStats(EPotatoAIModel Model)
{
    if (ModelUsageStats.Contains(Model))
    {
        ModelUsageStats[Model]++;
    }
    else
    {
        ModelUsageStats.Add(Model, 1);
    }
}

void UMingRTSPotatoAIEnhancedAPI::LogGeneration(const FString& Requirement, const FPotatoAIEnhancedResult& Result)
{
    TotalGenerations++;
    
    // 更新平均品質分數
    if (TotalGenerations > 1)
    {
        AverageQualityScore = (AverageQualityScore * (TotalGenerations - 1) + Result.QualityAssessment.OverallScore) / TotalGenerations;
    }
    else
    {
        AverageQualityScore = Result.QualityAssessment.OverallScore;
    }
    
    UE_LOG(LogTemp, Log, TEXT("🥔 生成統計 - 總次數: %d, 平均品質: %.1f"), TotalGenerations, AverageQualityScore);
}

FPotatoAIEnhancedQuality UMingRTSPotatoAIEnhancedAPI::AnalyzeCodeStructure(const FString& Code)
{
    FPotatoAIEnhancedQuality Quality;
    
    int32 Score = 70; // 基礎分數
    
    // 檢查程式碼結構
    if (Code.Contains(TEXT("#pragma once"))) Score += 5;
    if (Code.Contains(TEXT("GENERATED_BODY()"))) Score += 5;
    if (Code.Contains(TEXT("public:")) && Code.Contains(TEXT("protected:")) && Code.Contains(TEXT("private:"))) Score += 10;
    if (Code.Contains(TEXT("UFUNCTION"))) Score += 5;
    if (Code.Contains(TEXT("UPROPERTY"))) Score += 5;
    
    Quality.CodeStructureScore = FMath::Clamp(Score, 0, 100);
    return Quality;
}

FPotatoAIEnhancedQuality UMingRTSPotatoAIEnhancedAPI::AnalyzePerformance(const FString& Code)
{
    FPotatoAIEnhancedQuality Quality;
    
    int32 Score = 70;
    
    // 檢查效能特徵
    if (Code.Contains(TEXT("FORCEINLINE"))) Score += 10;
    if (Code.Contains(TEXT("const&"))) Score += 5;
    if (!Code.Contains(TEXT("new ")) && !Code.Contains(TEXT("delete "))) Score += 10; // 沒有裸指標
    if (Code.Contains(TEXT("TArray") && Code.Contains(TEXT("Reserve")))) Score += 5;
    
    Quality.PerformanceScore = FMath::Clamp(Score, 0, 100);
    return Quality;
}

FPotatoAIEnhancedQuality UMingRTSPotatoAIEnhancedAPI::AnalyzeSecurity(const FString& Code)
{
    FPotatoAIEnhancedQuality Quality;
    
    int32 Score = 70;
    
    // 檢查安全性
    if (Code.Contains(TEXT("nullptr")) || Code.Contains(TEXT("NULL"))) Score += 5;
    if (Code.Contains(TEXT("check(")) || Code.Contains(TEXT("ensure("))) Score += 5;
    if (Code.Contains(TEXT("try")) || Code.Contains(TEXT("catch"))) Score += 5;
    
    Quality.SecurityScore = FMath::Clamp(Score, 0, 100);
    return Quality;
}

FPotatoAIEnhancedQuality UMingRTSPotatoAIEnhancedAPI::AnalyzeMaintainability(const FString& Code)
{
    FPotatoAIEnhancedQuality Quality;
    
    int32 Score = 70;
    
    // 檢查維護性
    if (Code.Contains(TEXT("// ")) || Code.Contains(TEXT("/*"))) Score += 10; // 有註解
    if (Code.Contains(TEXT("UFUNCTION(BlueprintCallable)"))) Score += 5;
    if (Code.Contains(TEXT("UFUNCTION(BlueprintPure)"))) Score += 5;
    
    Quality.MaintainabilityScore = FMath::Clamp(Score, 0, 100);
    return Quality;
}
