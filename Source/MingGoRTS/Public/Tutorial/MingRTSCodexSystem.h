#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCodexSystem.generated.h"

/**
 * ????條目類??
 */
UENUM(BlueprintType)
enum class ECodexEntryType : uint8
{
    GameBasics           UMETA(DisplayName = "??戲????"),
    FactionInfo          UMETA(DisplayName = "????介紹"),
    UnitInfo             UMETA(DisplayName = "???X???"),
    BuildingInfo         UMETA(DisplayName = "建??說??"),
    Technology           UMETA(DisplayName = "科????),
    Historical           UMETA(DisplayName = "歷史??景"),
    Tutorial             UMETA(DisplayName = "???X?學"),
    Strategy             UMETA(DisplayName = "????策略"),
    Achievement          UMETA(DisplayName = "??就說??"),
    Lore                 UMETA(DisplayName = "世??觀")
};

/**
 * ????條目????
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
 * ????章??
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
 * ???X?書系統 - ??戲??知識庫
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSCodexSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSCodexSystem(};
    
    // ???X    UFUNCTION(BlueprintCallable, Category = "Codex")
    void InitializeCodexSystem(};
    
    // ???X?????節
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexSection> GetAllSections() const;
    
    // ???X???類??條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetEntriesByType(ECodexEntryType Type) const;
    
    // ???X?個?X    UFUNCTION(BlueprintCallable, Category = "Codex")
    bool GetEntry(FName EntryID, FCodexEntry& OutEntry) const;
    
    // ??索條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> SearchEntries(const FString& SearchQuery) const;
    
    // ????條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    void ViewEntry(FName EntryID};
    
    // 檢查條目??否已解X    UFUNCTION(BlueprintCallable, Category = "Codex")
    bool IsEntryUnlocked(FName EntryID) const;
    
    // ?X條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    void UnlockEntry(FName EntryID};
    
    // ???X?薦條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRecommendedEntries() const;
    
    // ???X?近查X    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRecentlyViewed(int32 Count = 10) const;
    
    // ???X???條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetPopularEntries(int32 Count = 10) const;
    
    // ????籤????    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetEntriesByTag(const FString& Tag) const;
    
    // ???X???條目
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRelatedEntries(FName EntryID) const;
    
    // 保??/載入????歷史
    void SaveViewHistory(};
    void LoadViewHistory(};
    
    // 事件委??
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntryViewed, FName, EntryID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntryUnlocked, FName, EntryID};
    
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
    
    void LoadCodexData(};
    void InitializeGameBasicsSection(};
    void InitializeFactionSection(};
    void InitializeUnitSection(};
    void InitializeBuildingSection(};
    void InitializeTechnologySection(};
    void InitializeHistoricalSection(};
    void InitializeStrategySection(};
    void InitializeLoreSection(};
    
    FCodexEntry CreateEntry(ECodexEntryType Type, const FString& ID, const FText& Title, 
                           const FText& Description, const TArray<FString>& Tags};
};

