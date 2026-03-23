# CppReference 整合使用指南

## 概述

Coding API 現已整合 CppReference_zh_TW 文檔，可以利用繁體中文 C++ 參考資料來增強代碼生成和分析功能。

## 新增功能

### 1. 載入 C++ Reference 文檔

```cpp
UMingRTSCodingAPI* CodingAPI = UMingRTSCodingAPI::GetCodingAPI();
CodingAPI->InitializeCodingAPI();

// 載入 C++ Reference 文檔
bool bLoaded = CodingAPI->LoadCppReferenceDocumentation();
if (bLoaded)
{
    UE_LOG(LogTemp, Log, TEXT("C++ Reference 文檔已載入"));
}
```

### 2. 基於 Reference 生成代碼

```cpp
// 使用 C++ Reference 知識生成代碼
FString GeneratedCode = CodingAPI->GenerateCodeWithReference(
    ECodingAPIDomain::CodeGeneration,
    TEXT("vector"),
    TEXT("Create a dynamic array for game units")
);

UE_LOG(LogTemp, Log, TEXT("Generated:\n%s"), *GeneratedCode);
```

輸出示例：
```cpp
// 基於 C++ Reference (container) 生成
// 上下文: Create a dynamic array for game units

#include <vector>
std::vector<int> MyVector;
MyVector.push_back(42);
```

### 3. 獲取相關主題

```cpp
// 根據任務類型獲取相關 C++ Reference 主題
TArray<FString> Topics = CodingAPI->GetCppReferenceTopics(ECodingTaskType::FeatureDevelopment);

for (const FString& Topic : Topics)
{
    UE_LOG(LogTemp, Log, TEXT("相關主題: %s"), *Topic);
}
```

## 支援的 C++ Reference 類別

| 類別 | 中文名稱 | 主要主題 | 適用 Domain |
|------|----------|----------|-------------|
| language | 語言功能 | class, template, lambda, constexpr | CodeGeneration |
| container | 容器函式庫 | vector, map, unordered_map, set | CodeAnalysis |
| algorithm | 演算法函式庫 | sort, find, transform, for_each | Performance |
| memory | 記憶體函式庫 | unique_ptr, shared_ptr, weak_ptr | CodeGeneration |
| thread | 執行緒函式庫 | thread, mutex, future, async | Concurrency |
| chrono | 時間函式庫 | duration, time_point, system_clock | Utility |
| utility | 工具函式庫 | pair, tuple, optional, variant | Utility |

## 整合範例

### 完整開發工作流

```cpp
void DevelopFeatureWithReference(const FString& FeatureDescription)
{
    UMingRTSCodingAPI* CodingAPI = UMingRTSCodingAPI::GetCodingAPI();
    
    // 1. 初始化並載入 Reference
    CodingAPI->InitializeCodingAPI();
    CodingAPI->LoadCppReferenceDocumentation();
    
    // 2. 獲取相關主題
    TArray<FString> Topics = CodingAPI->GetCppReferenceTopics(ECodingTaskType::FeatureDevelopment);
    
    // 3. 基於 Reference 生成代碼
    for (const FString& Topic : Topics)
    {
        FString Code = CodingAPI->GenerateCodeWithReference(
            ECodingAPIDomain::CodeGeneration,
            Topic,
            FeatureDescription
        );
        
        // 4. 分析生成的代碼
        FCodingAPIResult Analysis = CodingAPI->AnalyzeCode(Code, ECodingAPIDomain::CodeAnalysis);
        
        if (Analysis.bSuccess)
        {
            // 5. 保存結果
            UE_LOG(LogTemp, Log, TEXT("Generated and validated code for topic: %s"), *Topic);
        }
    }
}
```

### 智能代碼補全

```cpp
// 當使用者輸入 STL 相關代碼時，自動提供 Reference 資訊
FString ProvideSTLReference(const FString& UserInput)
{
    UMingRTSCodingAPI* CodingAPI = UMingRTSCodingAPI::GetCodingAPI();
    
    if (UserInput.Contains(TEXT("std::vector")))
    {
        return CodingAPI->GenerateCodeWithReference(
            ECodingAPIDomain::CodeGeneration,
            TEXT("vector"),
            TEXT("User requested vector usage")
        );
    }
    else if (UserInput.Contains(TEXT("std::map")))
    {
        return CodingAPI->GenerateCodeWithReference(
            ECodingAPIDomain::CodeGeneration,
            TEXT("map"),
            TEXT("User requested map usage")
        );
    }
    
    return FString();
}
```

## 檔案結構

```
Source/MingGoRTS/
├── Public/API/
│   ├── MingRTSCppReferenceIntegration.h    # CppReference 整合類
│   └── MingRTSCodingAPI.h                 # 主 Coding API (已整合)
└── Private/API/
    ├── MingRTSCppReferenceIntegration.cpp # 實現
    └── MingRTSCodingAPI.cpp               # 主實現 (已整合)

docs/CppReference_zh_TW/
├── README.md                              # 主索引
├── language/README.md                     # 語言功能
├── container/README.md                    # 容器函式庫
├── algorithm/README.md                    # 演算法函式庫
├── memory/README.md                       # 記憶體函式庫
├── thread/README.md                       # 執行緒函式庫
├── chrono/README.md                       # 時間函式庫
└── utility/README.md                      # 工具函式庫
```

## 性能考量

- **懶加載**: CppReference 文檔在首次調用時載入
- **緩存**: 載入後的類別資訊會被緩存
- **記憶體使用**: 僅載入必要的類別資訊

## 未來擴展

1. **深度整合**: 自動從 markdown 提取詳細 API 資訊
2. **智能搜索**: 支援模糊匹配相關主題
3. **代碼範例**: 提供完整的可執行範例
4. **版本管理**: 支援多版本 C++ 標準

## API 參考

### UMingRTSCppReferenceIntegration

| 方法 | 描述 |
|------|------|
| `LoadCppReferenceDocs()` | 載入所有 CppReference 文檔 |
| `GetAllCategories()` | 獲取所有類別 |
| `GenerateCodeFromReference(Category, Topic)` | 基於 Reference 生成代碼 |
| `GetRelevantTopics(TaskType)` | 獲取任務相關主題 |
| `CreateCodeTemplate(Domain, Topic)` | 創建代碼模板 |

### UMingRTSCodingAPI (新增)

| 方法 | 描述 |
|------|------|
| `LoadCppReferenceDocumentation()` | 載入並整合 CppReference |
| `GenerateCodeWithReference(Domain, Topic, Context)` | 使用 Reference 知識生成代碼 |
| `GetCppReferenceTopics(TaskType)` | 獲取相關 C++ 主題 |

---

*最後更新: 2026-03-23*
