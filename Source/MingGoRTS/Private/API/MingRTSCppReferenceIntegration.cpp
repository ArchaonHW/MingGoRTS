// MingRTSCppReferenceIntegration.cpp - Implementation
#include "API/MingRTSCppReferenceIntegration.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

UMingRTSCppReferenceIntegration::UMingRTSCppReferenceIntegration()
{
    DocsBasePath = FPaths::ProjectDir() / TEXT("docs/CppReference_zh_TW/");
    InitializeDefaultCategories();
}

void UMingRTSCppReferenceIntegration::InitializeDefaultCategories()
{
    TArray<FCppReferenceCategory> Categories;
    
    FCppReferenceCategory Language;
    Language.CategoryName = TEXT("language");
    Language.ChineseTitle = TEXT("語言功能");
    Language.Description = TEXT("C++語言核心功能：類別、模板、Lambda、智能指標");
    Language.BaseUrl = TEXT("https://cppreference.com/w/cpp/language");
    Language.Topics = {TEXT("class"), TEXT("template"), TEXT("lambda"), TEXT("constexpr"), TEXT("move_semantics")};
    Categories.Add(Language);
    
    FCppReferenceCategory Container;
    Container.CategoryName = TEXT("container");
    Container.ChineseTitle = TEXT("容器函式庫");
    Container.Description = TEXT("STL容器：vector, map, unordered_map, set");
    Container.BaseUrl = TEXT("https://cppreference.com/w/cpp/container");
    Container.Topics = {TEXT("vector"), TEXT("map"), TEXT("unordered_map"), TEXT("set"), TEXT("array")};
    Categories.Add(Container);
    
    FCppReferenceCategory Algorithm;
    Algorithm.CategoryName = TEXT("algorithm");
    Algorithm.ChineseTitle = TEXT("演算法函式庫");
    Algorithm.Description = TEXT("STL演算法：sort, find, transform, for_each");
    Algorithm.BaseUrl = TEXT("https://cppreference.com/w/cpp/algorithm");
    Algorithm.Topics = {TEXT("sort"), TEXT("find"), TEXT("transform"), TEXT("for_each"), TEXT("reduce")};
    Categories.Add(Algorithm);
    
    FCppReferenceCategory Memory;
    Memory.CategoryName = TEXT("memory");
    Memory.ChineseTitle = TEXT("記憶體函式庫");
    Memory.Description = TEXT("智能指標和記憶體管理");
    Memory.BaseUrl = TEXT("https://cppreference.com/w/cpp/memory");
    Memory.Topics = {TEXT("unique_ptr"), TEXT("shared_ptr"), TEXT("weak_ptr"), TEXT("make_unique"), TEXT("allocator")};
    Categories.Add(Memory);
    
    FCppReferenceCategory Thread;
    Thread.CategoryName = TEXT("thread");
    Thread.ChineseTitle = TEXT("執行緒函式庫");
    Thread.Description = TEXT("多執行緒和並發：thread, mutex, future, async");
    Thread.BaseUrl = TEXT("https://cppreference.com/w/cpp/thread");
    Thread.Topics = {TEXT("thread"), TEXT("mutex"), TEXT("future"), TEXT("async"), TEXT("atomic")};
    Categories.Add(Thread);
    
    FCppReferenceCategory Chrono;
    Chrono.CategoryName = TEXT("chrono");
    Chrono.ChineseTitle = TEXT("時間函式庫");
    Chrono.Description = TEXT("時間和日期處理");
    Chrono.BaseUrl = TEXT("https://cppreference.com/w/cpp/chrono");
    Chrono.Topics = {TEXT("duration"), TEXT("time_point"), TEXT("system_clock"), TEXT("steady_clock")};
    Categories.Add(Chrono);
    
    FCppReferenceCategory Utility;
    Utility.CategoryName = TEXT("utility");
    Utility.ChineseTitle = TEXT("工具函式庫");
    Utility.Description = TEXT("通用工具：pair, tuple, optional, variant");
    Utility.BaseUrl = TEXT("https://cppreference.com/w/cpp/utility");
    Utility.Topics = {TEXT("pair"), TEXT("tuple"), TEXT("optional"), TEXT("variant"), TEXT("any")};
    Categories.Add(Utility);
    
    for (const auto& Cat : Categories)
    {
        CategoryMap.Add(Cat.CategoryName, Cat);
    }
}

bool UMingRTSCppReferenceIntegration::LoadCppReferenceDocs()
{
    UE_LOG(LogTemp, Log, TEXT("Loading C++ Reference docs from: %s"), *DocsBasePath);
    
    for (const auto& Pair : CategoryMap)
    {
        FString Content = ReadDocFile(Pair.Key);
        if (!Content.IsEmpty())
        {
            FCppReferenceCategory Updated = Pair.Value;
            Updated.Topics = ParseTopics(Content);
            CategoryMap[Pair.Key] = Updated;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Loaded %d C++ Reference categories"), CategoryMap.Num());
    return true;
}

FString UMingRTSCppReferenceIntegration::ReadDocFile(const FString& CategoryName)
{
    FString FilePath = DocsBasePath / CategoryName / TEXT("README.md");
    FString Content;
    
    if (FFileHelper::LoadFileToString(Content, *FilePath))
    {
        return Content;
    }
    
    return FString();
}

TArray<FString> UMingRTSCppReferenceIntegration::ParseTopics(const FString& Content)
{
    TArray<FString> Topics;
    // Extract topic references from markdown content
    // Look for function names, class names, or links
    return Topics;
}

TArray<FCppReferenceCategory> UMingRTSCppReferenceIntegration::GetAllCategories() const
{
    TArray<FCppReferenceCategory> Result;
    CategoryMap.GenerateValueArray(Result);
    return Result;
}

FString UMingRTSCppReferenceIntegration::GenerateCodeFromReference(const FString& Category, const FString& Topic)
{
    const FCppReferenceCategory* Cat = CategoryMap.Find(Category);
    if (!Cat)
    {
        return FString();
    }
    
    // Generate code template based on category and topic
    if (Category == TEXT("container"))
    {
        if (Topic == TEXT("vector"))
        {
            return TEXT("#include <vector>\nstd::vector<int> MyVector;\nMyVector.push_back(42);");
        }
        if (Topic == TEXT("map"))
        {
            return TEXT("#include <map>\nstd::map<FString, int> MyMap;\nMyMap[TEXT(\"key\")] = 42;");
        }
    }
    else if (Category == TEXT("memory"))
    {
        if (Topic == TEXT("unique_ptr"))
        {
            return TEXT("#include <memory>\nauto MyPtr = std::make_unique<MyClass>();\n// Automatic cleanup when out of scope");
        }
    }
    
    return FString::Printf(TEXT("// Reference: %s - %s\n// See: %s"), *Cat->ChineseTitle, *Topic, *Cat->BaseUrl);
}

TArray<FString> UMingRTSCppReferenceIntegration::GetRelevantTopics(ECodingTaskType TaskType) const
{
    TArray<FString> RelevantTopics;
    
    switch (TaskType)
    {
    case ECodingTaskType::FeatureDevelopment:
        RelevantTopics = {TEXT("language"), TEXT("container"), TEXT("utility")};
        break;
    case ECodingTaskType::Optimization:
        RelevantTopics = {TEXT("algorithm"), TEXT("memory")};
        break;
    case ECodingTaskType::Concurrency:
        RelevantTopics = {TEXT("thread"), TEXT("memory")};
        break;
    case ECodingTaskType::Refactoring:
        RelevantTopics = {TEXT("language"), TEXT("utility")};
        break;
    default:
        RelevantTopics = {TEXT("language"), TEXT("container")};
    }
    
    return RelevantTopics;
}

FString UMingRTSCppReferenceIntegration::CreateCodeTemplate(ECodingAPIDomain Domain, const FString& Topic)
{
    FString Template;
    
    switch (Domain)
    {
    case ECodingAPIDomain::CodeGeneration:
        Template = FString::Printf(TEXT(
            "// C++ Reference: %s\n"
            "// Generated for UE5 development\n\n"
            "#pragma once\n\n"
            "class F%sHelper\n"
            "{\n"
            "public:\n"
            "    static void Example();\n"
            "};"), *Topic, *Topic);
        break;
        
    case ECodingAPIDomain::CodeAnalysis:
        Template = FString::Printf(TEXT(
            "// Analysis template for %s\n"
            "// Check: STL usage, performance, memory safety\n"), *Topic);
        break;
        
    default:
        Template = FString::Printf(TEXT("// Template for %s in domain %d"), *Topic, (int32)Domain);
    }
    
    return Template;
}
