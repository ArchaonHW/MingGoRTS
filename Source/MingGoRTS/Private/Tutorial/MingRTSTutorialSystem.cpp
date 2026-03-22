#include "Tutorial/MingRTSTutorialSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UMingRTSTutorialSystem::UMingRTSTutorialSystem()
    : CurrentStage(ETutorialStage::None)
    , CurrentStepIndex(0)
    , bTutorialEnabled(true)
    , CurrentTutorialWidget(nullptr)
{
}

void UMingRTSTutorialSystem::InitializeTutorialSystem()
{
    LoadStageConfigs();
    LoadTutorialProgress();
    
    UE_LOG(LogTemp, Log, TEXT("Tutorial System Initialized. Completed stages: %d"), CompletedStages.Num());
}

void UMingRTSTutorialSystem::StartTutorialStage(ETutorialStage Stage)
{
    if (!bTutorialEnabled)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tutorial system is disabled"));
        return;
    }
    
    if (IsStageCompleted(Stage))
    {
        UE_LOG(LogTemp, Log, TEXT("Stage %d already completed"), (int32)Stage);
        return;
    }
    
    CurrentStage = Stage;
    CurrentStepIndex = 0;
    
    // 找到對應的階段配置
    FTutorialStageConfig* StageConfig = StageConfigs.FindByPredicate([Stage](const FTutorialStageConfig& Config) {
        return Config.Stage == Stage;
    });
    
    if (StageConfig)
    {
        OnTutorialStageStarted.Broadcast(Stage, StageConfig->StageName);
        
        if (StageConfig->Steps.Num() > 0)
        {
            ShowCurrentStep();
        }
        else
        {
            // 沒有步驟，直接完成
            OnTutorialStageCompleted.Broadcast(Stage);
            CompletedStages.AddUnique(Stage);
            SaveTutorialProgress();
        }
    }
}

void UMingRTSTutorialSystem::EndCurrentTutorial()
{
    if (CurrentTutorialWidget)
    {
        CurrentTutorialWidget->RemoveFromParent();
        CurrentTutorialWidget = nullptr;
    }
    
    ClearHighlight();
    
    GetWorld()->GetTimerManager().ClearTimer(AutoAdvanceTimerHandle);
    
    CurrentStage = ETutorialStage::None;
    CurrentStepIndex = 0;
}

void UMingRTSTutorialSystem::CompleteCurrentStep()
{
    FTutorialStageConfig* StageConfig = StageConfigs.FindByPredicate([this](const FTutorialStageConfig& Config) {
        return Config.Stage == CurrentStage;
    });
    
    if (!StageConfig)
    {
        return;
    }
    
    if (CurrentStepIndex < StageConfig->Steps.Num() - 1)
    {
        CurrentStepIndex++;
        ShowCurrentStep();
    }
    else
    {
        // 階段完成
        OnTutorialStageCompleted.Broadcast(CurrentStage);
        CompletedStages.AddUnique(CurrentStage);
        SaveTutorialProgress();
        
        // 檢查是否全部完成
        bool bAllCompleted = true;
        for (const FTutorialStageConfig& Config : StageConfigs)
        {
            if (Config.Stage != ETutorialStage::None && Config.Stage != ETutorialStage::Completed)
            {
                if (!CompletedStages.Contains(Config.Stage))
                {
                    bAllCompleted = false;
                    break;
                }
            }
        }
        
        if (bAllCompleted)
        {
            OnTutorialCompleted.Broadcast();
        }
        
        EndCurrentTutorial();
    }
}

void UMingRTSTutorialSystem::SkipTutorial()
{
    EndCurrentTutorial();
    OnTutorialSkipped.Broadcast();
}

bool UMingRTSTutorialSystem::IsStageCompleted(ETutorialStage Stage) const
{
    return CompletedStages.Contains(Stage);
}

ETutorialStage UMingRTSTutorialSystem::GetNextUncompletedStage() const
{
    for (const FTutorialStageConfig& Config : StageConfigs)
    {
        if (Config.Stage != ETutorialStage::None && 
            Config.Stage != ETutorialStage::Completed &&
            !CompletedStages.Contains(Config.Stage))
        {
            return Config.Stage;
        }
    }
    
    return ETutorialStage::Completed;
}

TArray<ETutorialStage> UMingRTSTutorialSystem::GetCompletedStages() const
{
    return CompletedStages;
}

void UMingRTSTutorialSystem::ResetTutorialProgress()
{
    CompletedStages.Empty();
    SaveTutorialProgress();
    
    UE_LOG(LogTemp, Log, TEXT("Tutorial progress reset"));
}

void UMingRTSTutorialSystem::SetTutorialEnabled(bool bEnabled)
{
    bTutorialEnabled = bEnabled;
}

void UMingRTSTutorialSystem::SaveTutorialProgress()
{
    // 使用遊戲實例或保存系統保存進度
    // 這裡提供基本實現框架
    
    TArray<FString> CompletedStageNames;
    for (ETutorialStage Stage : CompletedStages)
    {
        CompletedStageNames.Add(UEnum::GetValueAsString(Stage));
    }
    
    // 實際實現應該使用遊戲保存系統
    UE_LOG(LogTemp, Log, TEXT("Tutorial progress saved: %d stages completed"), CompletedStages.Num());
}

void UMingRTSTutorialSystem::LoadTutorialProgress()
{
    // 從保存系統載入進度
    // 這裡提供基本實現框架
    
    UE_LOG(LogTemp, Log, TEXT("Tutorial progress loaded"));
}

void UMingRTSTutorialSystem::LoadStageConfigs()
{
    StageConfigs.Empty();
    
    // 基礎操作階段
    {
        FTutorialStageConfig Config;
        Config.Stage = ETutorialStage::BasicControls;
        Config.StageName = FText::FromString(TEXT("基礎操作教學"));
        Config.StageDescription = FText::FromString(TEXT("學習遊戲的基本操作"));
        Config.EstimatedDurationMinutes = 10.0f;
        
        // 步驟1: 移動視角
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("MoveCamera"));
            Step.Title = FText::FromString(TEXT("移動視角"));
            Step.Description = FText::FromString(TEXT("使用 WASD 鍵或鼠標右鍵拖動來移動視角，觀察戰場"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        // 步驟2: 選擇單位
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("SelectUnit"));
            Step.Title = FText::FromString(TEXT("選擇單位"));
            Step.Description = FText::FromString(TEXT("左鍵點擊單位來選擇，按住左鍵拖動可以框選多個單位"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        // 步驟3: 移動命令
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("MoveCommand"));
            Step.Title = FText::FromString(TEXT("下達移動命令"));
            Step.Description = FText::FromString(TEXT("選擇單位後，右鍵點擊地面下達移動命令"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        // 步驟4: 攻擊命令
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("AttackCommand"));
            Step.Title = FText::FromString(TEXT("攻擊敵人"));
            Step.Description = FText::FromString(TEXT("選擇單位後，右鍵點擊敵人下達攻擊命令，或使用A鍵移動攻擊"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        StageConfigs.Add(Config);
    }
    
    // 資源管理階段
    {
        FTutorialStageConfig Config;
        Config.Stage = ETutorialStage::ResourceManagement;
        Config.StageName = FText::FromString(TEXT("資源管理教學"));
        Config.StageDescription = FText::FromString(TEXT("學習資源採集和管理"));
        Config.EstimatedDurationMinutes = 15.0f;
        Config.UnlockCondition = TEXT("Complete BasicControls");
        
        // 步驟1: 資源類型
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("ResourceTypes"));
            Step.Title = FText::FromString(TEXT("認識資源"));
            Step.Description = FText::FromString(TEXT("遊戲中有三種主要資源：糧食、金錢、鋼鐵。它們顯示在屏幕右上角"));
            Step.bRequiresAction = false;
            Step.AutoAdvanceDelay = 5.0f;
            Config.Steps.Add(Step);
        }
        
        // 步驟2: 建造採集建築
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("BuildGatherer"));
            Step.Title = FText::FromString(TEXT("建造採集建築"));
            Step.Description = FText::FromString(TEXT("選擇工人，點擊建造菜單，選擇農場/礦場/工廠來採集資源"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        // 步驟3: 訓練單位
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("TrainUnits"));
            Step.Title = FText::FromString(TEXT("訓練單位"));
            Step.Description = FText::FromString(TEXT("選擇兵營，點擊訓練按鈕生產新單位"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        StageConfigs.Add(Config);
    }
    
    // 戰鬥基礎階段
    {
        FTutorialStageConfig Config;
        Config.Stage = ETutorialStage::CombatBasics;
        Config.StageName = FText::FromString(TEXT("戰鬥基礎教學"));
        Config.StageDescription = FText::FromString(TEXT("學習戰鬥技巧"));
        Config.EstimatedDurationMinutes = 20.0f;
        Config.UnlockCondition = TEXT("Complete ResourceManagement");
        
        // 步驟1: 編隊
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("ControlGroups"));
            Step.Title = FText::FromString(TEXT("單位編隊"));
            Step.Description = FText::FromString(TEXT("選擇單位後，按 Ctrl+數字鍵 創建編隊，之後按數字鍵快速選擇"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        // 步驟2: 微操作
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("MicroControl"));
            Step.Title = FText::FromString(TEXT("微操作"));
            Step.Description = FText::FromString(TEXT("在戰鬥中，選擇受傷單位後撤，讓健康單位承受傷害"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        // 步驟3: 使用技能
        {
            FTutorialStep Step;
            Step.StepID = FName(TEXT("UseAbilities"));
            Step.Title = FText::FromString(TEXT("使用技能"));
            Step.Description = FText::FromString(TEXT("選擇英雄單位，點擊技能圖標或按快捷鍵使用特殊能力"));
            Step.bRequiresAction = true;
            Config.Steps.Add(Step);
        }
        
        StageConfigs.Add(Config);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Loaded %d tutorial stage configs"), StageConfigs.Num());
}

void UMingRTSTutorialSystem::ShowCurrentStep()
{
    FTutorialStageConfig* StageConfig = StageConfigs.FindByPredicate([this](const FTutorialStageConfig& Config) {
        return Config.Stage == CurrentStage;
    });
    
    if (!StageConfig || !StageConfig->Steps.IsValidIndex(CurrentStepIndex))
    {
        return;
    }
    
    const FTutorialStep& Step = StageConfig->Steps[CurrentStepIndex];
    
    // 廣播步驟變化事件
    OnTutorialStepChanged.Broadcast(CurrentStepIndex, Step.Title);
    
    // 高亮UI元素
    for (const FString& Element : Step.HighlightUIElements)
    {
        HighlightGameElement(Element);
    }
    
    // 如果需要暫停遊戲
    if (Step.bPauseGame)
    {
        // 暫停遊戲邏輯
    }
    
    // 自動前進定時器
    if (Step.AutoAdvanceDelay > 0.0f && !Step.bRequiresAction)
    {
        GetWorld()->GetTimerManager().SetTimer(AutoAdvanceTimerHandle, [this]() {
            CompleteCurrentStep();
        }, Step.AutoAdvanceDelay, false);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Showing tutorial step: %s"), *Step.Title.ToString());
}

void UMingRTSTutorialSystem::AdvanceToNextStep()
{
    CompleteCurrentStep();
}

void UMingRTSTutorialSystem::OnStepActionCompleted(FName StepID)
{
    FTutorialStageConfig* StageConfig = StageConfigs.FindByPredicate([this](const FTutorialStageConfig& Config) {
        return Config.Stage == CurrentStage;
    });
    
    if (StageConfig && StageConfig->Steps.IsValidIndex(CurrentStepIndex))
    {
        if (StageConfig->Steps[CurrentStepIndex].StepID == StepID)
        {
            CompleteCurrentStep();
        }
    }
}

void UMingRTSTutorialSystem::HighlightGameElement(const FString& ElementTag)
{
    // 實現UI高亮邏輯
    UE_LOG(LogTemp, Log, TEXT("Highlighting element: %s"), *ElementTag);
}

void UMingRTSTutorialSystem::ClearHighlight()
{
    // 清除所有高亮
}
