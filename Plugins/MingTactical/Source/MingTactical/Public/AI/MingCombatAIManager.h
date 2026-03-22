#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCombatAIManager.generated.h"

// AI 難度等級
UENUM(BlueprintType)
enum class EMingAIDifficulty : uint8
{
    Recruit,        // 新兵
    Regular,        // 常規
    Veteran,        // 老兵
    Elite,          // 精銳
    Legendary       // 傳奇
};

// AI 節點結果
UENUM(BlueprintType)
enum class EMingAINodeResult : uint8
{
    Running,        // 運行中
    Success,        // 成功
    Failure         // 失敗
};

// AI 戰略偏好
UENUM(BlueprintType)
enum class EMingStrategicPreference : uint8
{
    Aggressive,     // 攻擊性
    Defensive,      // 防禦性
    Balanced,       // 平衡型
    Economic,       // 經濟型
    Diplomatic      // 外交型
};

// AI 戰術風格
UENUM(BlueprintType)
enum class EMingTacticalStyle : uint8
{
    DirectAssault,  // 直接攻擊
    Flanking,       // 側翼包抄
    Guerrilla,       // 游擊戰
    Siege,          // 圍攻戰
    Mobile          // 機動戰
};

// 軍閥類型
UENUM(BlueprintType)
enum class EMingWarlordType : uint8
{
    Northeastern,    // 東北軍閥
    Northwestern,    // 西北軍閥
    Southwestern,    // 西南軍閥
    Central,         // 中央軍閥
    Independent      // 獨立軍閥
};

// AI 統計信息
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalAIUnits;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveAIControllers;

    UPROPERTY(BlueprintReadOnly)
    float AverageDecisionTime;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalDecisionsMade;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulAttacks;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulDefenses;
};

// AI 上下文
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AController* AIController;

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> ControlledUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> EnemyUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> FriendlyUnits;

    UPROPERTY(BlueprintReadOnly)
    FVector CurrentTarget;

    UPROPERTY(BlueprintReadOnly)
    float CurrentTime;

    UPROPERTY(BlueprintReadOnly)
    EMingAIDifficulty Difficulty;
};

// AI 難度設置
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIDifficultySettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReactionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionAccuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TacticalSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CoordinationAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningCapability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MistakeRate;
};

/**
 * 戰鬥 AI 管理器
 * 負責管理所有戰鬥 AI 控制器和行為
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingCombatAIManager : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingCombatAIManager();

    // 初始化 AI 管理器
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void Initialize();

    // AI 更新主循環
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void UpdateCombatAI(float DeltaTime);

    // 註冊 AI 控制器
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void RegisterAIController(class AController* AIController);

    // 移除 AI 控制器
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void UnregisterAIController(class AController* AIController);

    // 設置全局 AI 難度
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetAIDifficulty(EMingAIDifficulty Difficulty);

    // 獲取 AI 難度設置
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIDifficultySettings GetDifficultySettings(EMingAIDifficulty Difficulty) const;

    // 獲取 AI 統計信息
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIStatistics GetAIStatistics() const;

    // 暫停 AI 更新
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void PauseAI();

    // 恢復 AI 更新
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ResumeAI();

    // 重置所有 AI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ResetAllAI();

protected:
    // AI 控制器列表
    TArray<TWeakObjectPtr<class AController>> AIControllers;

    // AI 更新間隔
    float AIUpdateInterval;

    // 當前難度設置
    EMingAIDifficulty CurrentDifficulty;

    // 是否暫停
    bool bIsPaused;

    // 統計數據
    FMingAIStatistics AIStatistics;

    // 難度設置映射
    TMap<EMingAIDifficulty, FMingAIDifficultySettings> DifficultySettings;

    // 初始化難度設置
    void InitializeDifficultySettings();

    // 更新統計信息
    void UpdateStatistics();

    // 獲取 AI 更新時間
    float GetAIUpdateTime() const;

private:
    // 是否已初始化
    bool bInitialized = false;

    // 累計時間
    float AccumulatedTime;
};
