#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.generated.h"

// Á∑®È??Ω‰ª§È°ûÂ?
UENUM(BlueprintType)
enum class EMingFormationCommand : uint8
{
    CreateFormation,    // ?µÂª∫Á∑®È?
    ChangeFormation,     // ËÆäÊõ¥Á∑®È?
    DisbandFormation,   // Ëß?ï£Á∑®È?
    MergeFormations,     // ?à‰ΩµÁ∑®È?
    SplitFormation,     // ?ÜË?Á∑®È?
    RotateFormation,     // ?ãË?Á∑®È?
    ScaleFormation,      // Á∏ÆÊîæÁ∑®È?
    MirrorFormation     // ?°Â?Á∑®È?
};

// Á∑®È??Ä??UENUM(BlueprintType)
enum class EMingFormationStatus : uint8
{
    Forming,           // Á∑®È?‰∏?    Active,            // Ê¥ªË?
    Transitioning,      // ËΩâÊ?‰∏?    Disrupted,         // Ë¢´Ê?‰∫?    Disbanded          // Â∑≤Ëß£??};

// Á∑®È??™Â?Á¥?UENUM(BlueprintType)
enum class EMingFormationPriority : uint8
{
    Low,               // ‰ΩéÂÑ™?àÁ?
    Normal,            // ?ÆÈÄöÂÑ™?àÁ?
    High,              // È´òÂÑ™?àÁ?
    Critical           // ?úÈçµ?™Â?Á¥?};

// Á∑®È??Æ‰?ËßíËâ≤
UENUM(BlueprintType)
enum class EMingUnitRole : uint8
{
    Leader,            // ?òÈ?
    Vanguard,          // ?àÈ?
    Flanker,           // ?¥Áøº
    Support,           // ?ØÊè¥
    Rearguard,         // ÂæåË?
    Reserve,           // ?êÂ???    Artillery,         // ?≤ÂÖµ
    Scout,             // ?µÂ?
    Medic              // ?´Á???};

// Á∑®È??∏Ê?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationData
{
    GENERATED_BODY()

    // Á∑®È?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // Á∑®È??çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationName;

    // Á∑®È?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // Á∑®È??Ä??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationStatus FormationStatus;

    // Á∑®È??™Â?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationPriority Priority;

    // Á∑®È?‰∏≠Â?Èª?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector FormationCenter;

    // Á∑®È??πÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator FormationRotation;

    // Á∑®È?Ë¶èÊ®°
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationScale;

    // ?Æ‰??óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnitIDs;

    // ?Æ‰?ËßíËâ≤?†Â?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, EMingUnitRole> UnitRoles;

    // Á∑®È?‰ΩçÁΩÆ?∏Á?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> FormationPositions;

    // Á∑®È??†Ê?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationBonuses;

    // Á∑®È?Ê∏õÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationPenalties;

    // ?µÂª∫?ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CreationTime;

    // ?ÄÂæåÊõ¥?∞Ê???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastUpdateTime;

    // Á∑®È??áÊèÆÂÆòID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommanderID;

    // ?ôÁî®?áÊèÆÂÆòID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BackupCommanderID;

    // Á∑®È?Á∂ìÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationExperience;

    // Á∑®È?Â£´Ê∞£
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationMorale;

    // Á∑®È?Á¥ÄÂæ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationDiscipline;

    // ?ØÂê¶?∫È??ãÁ∑®??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsStatic;

    // ?ØÂê¶?∫Ëá™?ïÁ∑®??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAutoManaged;

    FMingFormationData()
    {
        FormationID = -1;
        FormationName = TEXT("");
        FormationType = EMingFormationType::Line;
        FormationStatus = EMingFormationStatus::Forming;
        Priority = EMingFormationPriority::Normal;
        FormationCenter = FVector::ZeroVector;
        FormationRotation = FRotator::ZeroRotator;
        FormationScale = 1.0f;
        UnitIDs.Empty();
        UnitRoles.Empty();
        FormationPositions.Empty();
        FormationBonuses.Empty();
        FormationPenalties.Empty();
        CreationTime = FDateTime::Now();
        LastUpdateTime = FDateTime::Now();
        CommanderID = -1;
        BackupCommanderID = -1;
        FormationExperience = 0.0f;
        FormationMorale = 75.0f;
        FormationDiscipline = 70.0f;
        bIsStatic = false;
        bIsAutoManaged = false;
    }
};

// Á∑®È??Ω‰ª§?∏Ê?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationCommand
{
    GENERATED_BODY()

    // ?Ω‰ª§ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandID;

    // ?Ω‰ª§È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationCommand CommandType;

    // ?ÆÊ?Á∑®È?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetFormationID;

    // ?Ω‰ª§?ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CommandParameters;

    // ?Ω‰ª§?™Â?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationPriority Priority;

    // ?∑Ë??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // ?Ω‰ª§?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CommandDescription;

    FMingFormationCommand()
    {
        CommandID = -1;
        CommandType = EMingFormationCommand::CreateFormation;
        TargetFormationID = -1;
        CommandParameters.Empty();
        Priority = EMingFormationPriority::Normal;
        ExecutionTime = 0.0f;
        CommandDescription = TEXT("");
    }
};

// Á∑®È?Ê®°Êùø
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationTemplate
{
    GENERATED_BODY()

    // Ê®°ÊùøID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TemplateID;

    // Ê®°Êùø?çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateName;

    // Ê®°Êùø?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateDescription;

    // Á∑®È?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // ?®Ëñ¶?Æ‰?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingUnitType> RecommendedUnitTypes;

    // ?ÄÂ∞èÂñÆ‰ΩçÊï∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinUnitCount;

    // ?ÄÂ§ßÂñÆ‰ΩçÊï∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUnitCount;

    // Ê®ôÊ??ìË?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardSpacing;

    // Ê®ôÊ?Ê∑±Â∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardDepth;

    // Ê®ôÊ?ÂØ¨Â∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardWidth;

    // Ê®°Êùø?†Ê?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> TemplateBonuses;

    // Ê®°Êùø‰ΩøÁî®Ê¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UsageConditions;

    // ?ØÂê¶?∫Ê≠∑?≤Á∑®??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHistorical;

    // Ê≠∑Âè≤?åÊôØ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    FMingFormationTemplate()
    {
        TemplateID = -1;
        TemplateName = TEXT("");
        TemplateDescription = TEXT("");
        FormationType = EMingFormationType::Line;
        RecommendedUnitTypes.Empty();
        MinUnitCount = 1;
        MaxUnitCount = 100;
        StandardSpacing = 100.0f;
        StandardDepth = 200.0f;
        StandardWidth = 300.0f;
        TemplateBonuses.Empty();
        UsageConditions.Empty();
        bIsHistorical = false;
        HistoricalBackground = TEXT("");
    }
};

// Á∑®È?ÁÆ°Á?‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFormationCreated, const FMingFormationData&, Formation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationChanged, int32, FormationID, EMingFormationType, NewFormationType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationDisbanded, int32, FormationID, const TArray<int32>&, ReleasedUnits);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFormationUnitAdded, int32, FormationID, int32, UnitID, EMingUnitRole, Role);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationUnitRemoved, int32, FormationID, int32, UnitID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFormationCommandExecuted, const FMingFormationCommand&, Command);

/**
 * Á∑®È?ÁÆ°Á??? * Ë≤†Ë≤¨ÁÆ°Á??∞Ë?Á∑®È??ÑÂâµÂª∫„ÄÅË??¥Â?Á∂≠Ë≠∑
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingFormationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFormationManager();

    // ?ùÂ??ñÁ∑®?äÁÆ°?ÜÂô®
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool InitializeFormationManager();

    // ?µÂª∫Á∑®È?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    int32 CreateFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType, const FString& FormationName);

    // Ëß?ï£Á∑®È?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool DisbandFormation(int32 FormationID);

    // ËÆäÊõ¥Á∑®È?È°ûÂ?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ChangeFormationType(int32 FormationID, EMingFormationType NewFormationType);

    // Ê∑ªÂ??Æ‰??∞Á∑®??    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool AddUnitToFormation(int32 FormationID, int32 UnitID, EMingUnitRole Role);

    // ÂæûÁ∑®?äÁßª?§ÂñÆ‰Ω?    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RemoveUnitFromFormation(int32 FormationID, int32 UnitID);

    // Ë®≠ÁΩÆÁ∑®È??áÊèÆÂÆ?    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool SetFormationCommander(int32 FormationID, int32 CommanderID);

    // ?ãË?Á∑®È?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RotateFormation(int32 FormationID, const FRotator& NewRotation);

    // Á∏ÆÊîæÁ∑®È?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ScaleFormation(int32 FormationID, float NewScale);

    // ÁßªÂ?Á∑®È?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool MoveFormation(int32 FormationID, const FVector& TargetLocation);

    // ?≤Â?Á∑®È?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    FMingFormationData GetFormationInfo(int32 FormationID) const;

    // ?≤Â?Á∑®È??óË°®
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TArray<int32> GetFormationList() const;

    // ?≤Â?Á∑®È?Ê®°Êùø
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TArray<FMingFormationTemplate> GetFormationTemplates() const;

    // ?âÁî®Á∑®È?Ê®°Êùø
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ApplyFormationTemplate(int32 FormationID, int32 TemplateID);

    // ?≤Â?Á∑®È?Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TMap<EMingFormationType, int32> GetFormationStatistics() const;

    // Ë®àÁ?Á∑®È??àÁ?
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    float CalculateFormationEfficiency(int32 FormationID) const;

    // Ê™¢Êü•Á∑®È?ÂÆåÊï¥??    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool CheckFormationIntegrity(int32 FormationID);

    // ‰øÆÂæ©Á∑®È?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RepairFormation(int32 FormationID);

    // ‰øùÂ?Á∑®È??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool SaveFormationData();

    // ËºâÂÖ•Á∑®È??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool LoadFormationData();

    // Ê∏ÖÈô§?Ä?âÁ∑®??    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    void ClearAllFormations();

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable)
    FOnFormationCreated OnFormationCreated;

    UPROPERTY(BlueprintAssignable)
    FOnFormationChanged OnFormationChanged;

    UPROPERTY(BlueprintAssignable)
    FOnFormationDisbanded OnFormationDisbanded;

    UPROPERTY(BlueprintAssignable)
    FOnFormationUnitAdded OnFormationUnitAdded;

    UPROPERTY(BlueprintAssignable)
    FOnFormationUnitRemoved OnFormationUnitRemoved;

    UPROPERTY(BlueprintAssignable)
    FOnFormationCommandExecuted OnFormationCommandExecuted;

protected:
    // Á∑®È??∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingFormationData> FormationDatabase;

    // Á∑®È?Ê®°Êùø?∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingFormationTemplate> FormationTemplateDatabase;

    // Á∑®È??Ω‰ª§?äÂ?
    UPROPERTY()
    TArray<FMingFormationCommand> FormationCommandQueue;

    // ?∂Â?Ê¥ªË?Á∑®È?
    UPROPERTY()
    TArray<int32> ActiveFormations;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠Á∑®È?Ê®°Êùø
    void LoadDefaultFormationTemplates();

    // ?üÊ?Á∑®È?‰ΩçÁΩÆ
    TArray<FVector> GenerateFormationPositions(EMingFormationType FormationType, int32 UnitCount, float Scale) const;

    // Ë®àÁ?Á∑®È??†Ê?
    TMap<FString, float> CalculateFormationBonuses(int32 FormationID) const;

    // Ë®àÁ?Á∑®È?Ê∏õÁ?
    TMap<FString, float> CalculateFormationPenalties(int32 FormationID) const;

    // ?¥Êñ∞Á∑®È??Ä??    void UpdateFormationStatus(int32 FormationID, EMingFormationStatus NewStatus);

    // ?ïÁ?Á∑®È??Ω‰ª§
    void ProcessFormationCommand(const FMingFormationCommand& Command);

    // È©óË?Á∑®È??∏Ê?
    bool ValidateFormationData(const FMingFormationData& Formation) const;

    // ?âÁî®Á∑®È??†Ê??∞ÂñÆ‰Ω?    void ApplyFormationBonusesToUnits(int32 FormationID);

    // ÁßªÈô§Á∑®È??†Ê?ÂæûÂñÆ‰Ω?    void RemoveFormationBonusesFromUnits(int32 FormationID);

    // ?™Â?‰øÆÂæ©Á∑®È?
    void AutoRepairFormation(int32 FormationID);

    // Ë®àÁ??Ä‰Ω≥Á∑®?äÈ???    EMingFormationType CalculateOptimalFormationType(const TArray<int32>& UnitIDs) const;

    // Ê™¢Êü•Á∑®È?Ë°ùÁ?
    bool CheckFormationConflict(int32 FormationID1, int32 FormationID2) const;

    // Ëß?±∫Á∑®È?Ë°ùÁ?
    void ResolveFormationConflict(int32 FormationID1, int32 FormationID2);
};
