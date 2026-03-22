#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCodexSystem.generated.h"

/**
 * 百科條目類型
 */
UENUM(BlueprintType)
enum class ECodexEntryType : uint8
{
    GameBasics           UMETA(DisplayName = "遊戲基礎"),
    FactionInfo          UMETA(DisplayName = "勢力介紹"),
    UnitInfo             UMETA(DisplayName = "單位圖鑑"),
    BuildingInfo         UMETA(DisplayName = "建築說明"),
    Technology           UMETA(DisplayName = "科技樹"),
    Historical           UMETA(DisplayName = "歷史背景"),
    Tutorial             UMETA(DisplayName = "進階教學"),
    Strategy             UMETA(DisplayName = "戰術策略"),
    Achievement          UMETA(DisplayName = "成就說明"),
    Lore                 UMETA(DisplayName = "世界觀")
};

/**
 * 百科條目數據
 */
USTRUCT(BlueprintType)
struct FCodexEntry
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName EntryID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECodexEntryType EntryType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Subtitle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString IconPath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImagePath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Tags;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> RelatedEntries;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsLocked;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnlockCondition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ViewCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LastViewedTime;
    
    FCodexEntry()
        : EntryType(ECodexEntryType::GameBasics)
        , bIsLocked(false)
        , ViewCount(0)
    {}
};

/**
 * 百科章節
 */
USTRUCT(BlueprintType)
struct FCodexSection
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECodexEntryType SectionType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SectionName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SectionDescription;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SectionIcon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FCodexEntry> Entries;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SortOrder;
    
    FCodexSection()
        : SectionType(ECodexEntryType::GameBasics)
        , SortOrder(0)
    {}
};

/**
 * 百科全書系統 - 遊戲內知識庫
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSCodexSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSCodexSystem();
    
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Codex")
    void InitializeCodexSystem();
    
    // 獲取所有章節
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexSection> GetAllSections() const;
    
    // 獲取特定類型條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetEntriesByType(ECodexEntryType Type) const;
    
    // 獲取單個條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    bool GetEntry(FName EntryID, FCodexEntry& OutEntry) const;
    
    // 搜索條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> SearchEntries(const FString& SearchQuery) const;
    
    // 查看條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    void ViewEntry(FName EntryID);
    
    // 檢查條目是否已解鎖
    UFUNCTION(BlueprintCallable, Category = "Codex")
    bool IsEntryUnlocked(FName EntryID) const;
    
    // 解鎖條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    void UnlockEntry(FName EntryID);
    
    // 獲取推薦條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRecommendedEntries() const;
    
    // 獲取最近查看
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRecentlyViewed(int32 Count = 10) const;
    
    // 獲取熱門條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetPopularEntries(int32 Count = 10) const;
    
    // 按標籤過濾
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetEntriesByTag(const FString& Tag) const;
    
    // 獲取相關條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRelatedEntries(FName EntryID) const;
    
    // 保存/載入閱讀歷史
    void SaveViewHistory();
    void LoadViewHistory();
    
    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntryViewed, FName, EntryID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntryUnlocked, FName, EntryID);
    
    UPROPERTY(BlueprintAssignable, Category = "Codex|Events")
    FOnEntryViewed OnEntryViewed;
    
    UPROPERTY(BlueprintAssignable, Category = "Codex|Events")
    FOnEntryUnlocked OnEntryUnlocked;
    
private:
    UPROPERTY()
    TArray<FCodexSection> Sections;
    
    UPROPERTY()
    TMap<FName, int32> ViewHistory; // EntryID -> ViewCount
    
    UPROPERTY()
    TMap<FName, FString> ViewTimestamps; // EntryID -> LastViewedTime
    
    void LoadCodexData();
    void InitializeGameBasicsSection();
    void InitializeFactionSection();
    void InitializeUnitSection();
    void InitializeBuildingSection();
    void InitializeTechnologySection();
    void InitializeHistoricalSection();
    void InitializeStrategySection();
    void InitializeLoreSection();
    
    FCodexEntry CreateEntry(ECodexEntryType Type, const FString& ID, const FText& Title, 
                           const FText& Description, const TArray<FString>& Tags);
};
