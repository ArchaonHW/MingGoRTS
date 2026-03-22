# MingGoRTS 安全編碼指南

## 🛡️ UObject 生命週期安全

### 正確的初始化模式
```cpp
// ❌ 錯誤 - 在建構函式中初始化
UMingGoRTS::UMingGoRTS()
    : bIsGameInitialized(false)  // 危險！
{
}

// ✅ 正確 - 使用 PostInitProperties
virtual void PostInitProperties() override
{
    Super::PostInitProperties();
    bIsGameInitialized = false;  // 安全
}
```

### UCLASS 安全配置
```cpp
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingGoRTS : public UObject
```

### 記憶體安全原則
1. **永不在建構函式中初始化 UPROPERTY**
2. **使用 PostInitProperties() 進行初始化**
3. **始終調用 Super::PostInitProperties()**
4. **避免在析構函式中訪問其他 UObject**

## 🚨 常見錯誤

### EXCEPTION_ACCESS_VIOLATION
- **原因**: 記憶體訪問違規
- **解決**: 檢查 UObject 初始化順序
- **預防**: 使用安全的初始化模式

### UHT 錯誤
- **原因**: Unreal Header Tool 解析問題
- **解決**: 檢查 UCLASS/UFUNCTION 語法
- **預防**: 保持標準 UE5 語法

## 📊 系統監控

### 性能指標
- CPU 使用率 < 20%
- 記憶體使用 < 2GB
- 進程響應時間 < 1秒

### 崩潰檢測
- 監控 EXCEPTION_ACCESS_VIOLATION
- 檢查 CoreUObject.dll 錯誤
- 記錄崩潰日誌

---
**更新時間**: 2026-03-22 07:37
**狀態**: 已修復致命錯誤
