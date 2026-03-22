#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCodexSystem.generated.h"

/**
 * ?æÁ?Ê¢ùÁõÆÈ°ûÂ?
 */
UENUM(BlueprintType)
enum class ECodexEntryType : uint8
{
    GameBasics           UMETA(DisplayName = "?äÊà≤?∫Á?"),
    FactionInfo          UMETA(DisplayName = "?¢Â?‰ªãÁ¥π"),
    UnitInfo             UMETA(DisplayName = "?Æ‰??ñÈ?"),
    BuildingInfo         UMETA(DisplayName = "Âª∫Á?Ë™™Ê?"),
    Technology           UMETA(DisplayName = "ÁßëÊ?Ê®?),
    Historical           UMETA(DisplayName = "Ê≠∑Âè≤?åÊôØ"),
    Tutorial             UMETA(DisplayName = "?≤È??ôÂ≠∏"),
    Strategy             UMETA(DisplayName = "?∞Ë?Á≠ñÁï•"),
    Achievement          UMETA(DisplayName = "?êÂ∞±Ë™™Ê?"),
    Lore                 UMETA(DisplayName = "‰∏ñÁ?ËßÄ")
};

/**
 * ?æÁ?Ê¢ùÁõÆ?∏Ê?
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
 * ?æÁ?Á´†Á?
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
 * ?æÁ??®Êõ∏Á≥ªÁµ± - ?äÊà≤?ßÁü•Ë≠òÂ∫´
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSCodexSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSCodexSystem(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Codex")
    void InitializeCodexSystem(};
    
    // ?≤Â??Ä?âÁ?ÁØÄ
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexSection> GetAllSections() const;
    
    // ?≤Â??πÂ?È°ûÂ?Ê¢ùÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetEntriesByType(ECodexEntryType Type) const;
    
    // ?≤Â??ÆÂÄãÊ???    UFUNCTION(BlueprintCallable, Category = "Codex")
    bool GetEntry(FName EntryID, FCodexEntry& OutEntry) const;
    
    // ?úÁ¥¢Ê¢ùÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> SearchEntries(const FString& SearchQuery) const;
    
    // ?•Á?Ê¢ùÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Codex")
    void ViewEntry(FName EntryID};
    
    // Ê™¢Êü•Ê¢ùÁõÆ?ØÂê¶Â∑≤Ëß£??    UFUNCTION(BlueprintCallable, Category = "Codex")
    bool IsEntryUnlocked(FName EntryID) const;
    
    // Ëß??Ê¢ùÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Codex")
    void UnlockEntry(FName EntryID};
    
    // ?≤Â??®Ëñ¶Ê¢ùÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRecommendedEntries() const;
    
    // ?≤Â??ÄËøëÊü•??    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRecentlyViewed(int32 Count = 10) const;
    
    // ?≤Â??±È?Ê¢ùÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetPopularEntries(int32 Count = 10) const;
    
    // ?âÊ?Á±§È?Êø?    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetEntriesByTag(const FString& Tag) const;
    
    // ?≤Â??∏È?Ê¢ùÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Codex")
    TArray<FCodexEntry> GetRelatedEntries(FName EntryID) const;
    
    // ‰øùÂ?/ËºâÂÖ•?±Ë?Ê≠∑Âè≤
    void SaveViewHistory(};
    void LoadViewHistory(};
    
    // ‰∫ã‰ª∂ÂßîË?
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

