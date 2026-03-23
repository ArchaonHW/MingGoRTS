// Copyright (c) 2026 MingGoRTS. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MingGoRTSGameInstance.generated.h"

/**
 * MingGoRTS 遊戲實例
 * 處理遊戲啟動、初始化、和 UI/UX 自動生成
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class UMingGoRTSGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UMingGoRTSGameInstance();

    virtual void Init() override;
    virtual void Shutdown() override;

    /** 啟動時自動初始化 UI/UX 系統 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Init")
    void AutoInitializeUIUX();

    /** 檢查是否需要生成預設 UI 資產 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Init")
    bool ShouldGenerateDefaultUIAssets() const;

    /** 生成預設 UI/UX 畫面 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Init")
    void GenerateDefaultUIUXScreens();

    /** 載入啟動關卡 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Init")
    void LoadStartupMap();

    /** 獲取預設 Widget 類別 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS Init")
    TSubclassOf<class UUserWidget> GetDefaultMainMenuClass() const { return DefaultMainMenuClass; }

    UFUNCTION(BlueprintPure, Category = "MingGoRTS Init")
    TSubclassOf<class UUserWidget> GetDefaultGameHUDClass() const { return DefaultGameHUDClass; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS UI Classes")
    TSubclassOf<class UUserWidget> DefaultMainMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS UI Classes")
    TSubclassOf<class UUserWidget> DefaultGameHUDClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS UI Classes")
    TSubclassOf<class UUserWidget> DefaultLoadingScreenClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS Maps")
    FString StartupMapName;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS Maps")
    FString MainMenuMapName;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS Maps")
    FString GameMapName;

    /** 初始化預設配置 */
    void InitializeDefaultSettings();

    /** 預載入必要資源 */
    void PreloadEssentialAssets();

    /** 設置默認輸入配置 */
    void SetupDefaultInputSettings();

    /** 驗證 UI 類別配置 */
    void ValidateUIClassConfiguration();
};
