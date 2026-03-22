#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "MingUISystem.generated.h"

UENUM(BlueprintType)
enum class EMingUIScreen: uint8 {
    MainMenu UMETA(DisplayName = "Main Menu"),
    CampaignMap UMETA(DisplayName = "Campaign Map"),
    BattleHUD UMETA(DisplayName = "Battle HUD"),
    CharacterPanel UMETA(DisplayName = "Character Panel"),
    ResearchTree UMETA(DisplayName = "Research Tree"),
    Settings UMETA(DisplayName = "Settings"),
    Loading UMETA(DisplayName = "Loading"),
    Victory UMETA(DisplayName = "Victory"),
    Defeat UMETA(DisplayName = "Defeat"),
    Pause UMETA(DisplayName = "Pause")
};

UENUM(BlueprintType)
enum class EMingUIAnimationType: uint8 {
    Fade UMETA(DisplayName = "Fade"),
    Slide UMETA(DisplayName = "Slide"),
    Scale UMETA(DisplayName = "Scale"),
    Rotate UMETA(DisplayName = "Rotate"),
    Bounce UMETA(DisplayName = "Bounce"),
    Elastic UMETA(DisplayName = "Elastic")
};

USTRUCT(BlueprintType)
struct FMingUIScreenData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUIScreen ScreenType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ScreenName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsModal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDestroyOnClose;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUIAnimationType OpenAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUIAnimationType CloseAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AnimationDuration;

    FMingUIScreenData()
        : ScreenType(EMingUIScreen::MainMenu)
        , bIsModal(false)
        , bDestroyOnClose(true)
        , OpenAnimation(EMingUIAnimationType::Fade)
        , CloseAnimation(EMingUIAnimationType::Fade)
        , AnimationDuration(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FMingUIStyle
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor PrimaryColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor SecondaryColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor AccentColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor TextColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor BackgroundColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor BorderColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSlateFontInfo MainFont;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSlateFontInfo TitleFont;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSlateFontInfo ButtonFont;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BorderRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BorderThickness;

    FMingUIStyle()
        : PrimaryColor(FLinearColor(0.2f, 0.4f, 0.8f, 1.0f))
        , SecondaryColor(FLinearColor(0.1f, 0.2f, 0.4f, 1.0f))
        , AccentColor(FLinearColor(0.8f, 0.6f, 0.2f, 1.0f))
        , TextColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
        , BackgroundColor(FLinearColor(0.05f, 0.05f, 0.1f, 0.9f))
        , BorderColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f))
        , BorderRadius(8.0f)
        , BorderThickness(2.0f)
    {}
};

/**
 * UI System for MingGoRTS
 * Manages all UI screens, animations, and styling
 */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingUISystem : public UObject
{
    GENERATED_BODY()

public:
    UMingUISystem();

    // Screen Management
    UFUNCTION(BlueprintCallable, Category = "UI")
    void InitializeUI();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowScreen(EMingUIScreen ScreenType);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideScreen(EMingUIScreen ScreenType);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseScreen(EMingUIScreen ScreenType);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseAllScreens();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowModalScreen(EMingUIScreen ScreenType);

    UFUNCTION(BlueprintPure, Category = "UI")
    bool IsScreenVisible(EMingUIScreen ScreenType) const;

    UFUNCTION(BlueprintPure, Category = "UI")
    UUserWidget* GetCurrentScreen() const;

    UFUNCTION(BlueprintPure, Category = "UI")
    TArray<UUserWidget*> GetVisibleScreens() const;

    // Navigation
    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void NavigateToScreen(EMingUIScreen ScreenType);

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void GoBack();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void GoForward();

    UFUNCTION(BlueprintPure, Category = "Navigation")
    bool CanGoBack() const;

    UFUNCTION(BlueprintPure, Category = "Navigation")
    bool CanGoForward() const;

    // Animations
    UFUNCTION(BlueprintCallable, Category = "Animations")
    void PlayScreenAnimation(EMingUIScreen ScreenType, EMingUIAnimationType AnimationType, float Duration = 0.5f);

    UFUNCTION(BlueprintCallable, Category = "Animations")
    void FadeScreen(EMingUIScreen ScreenType, float TargetAlpha = 1.0f, float Duration = 0.5f);

    UFUNCTION(BlueprintCallable, Category = "Animations")
    void SlideScreen(EMingUIScreen ScreenType, FVector2D Direction, float Duration = 0.5f);

    UFUNCTION(BlueprintCallable, Category = "Animations")
    void ScaleScreen(EMingUIScreen ScreenType, float Scale, float Duration = 0.5f);

    // Styling
    UFUNCTION(BlueprintCallable, Category = "Styling")
    void SetUIStyle(const FMingUIStyle& NewStyle);

    UFUNCTION(BlueprintPure, Category = "Styling")
    FMingUIStyle GetCurrentStyle() const;

    UFUNCTION(BlueprintCallable, Category = "Styling")
    void ApplyTheme(const FString& ThemeName);

    UFUNCTION(BlueprintPure, Category = "Styling")
    TArray<FString> GetAvailableThemes() const;

    // Screen Data
    UFUNCTION(BlueprintCallable, Category = "Screen Data")
    void RegisterScreen(const FMingUIScreenData& ScreenData);

    UFUNCTION(BlueprintPure, Category = "Screen Data")
    FMingUIScreenData GetScreenData(EMingUIScreen ScreenType) const;

    UFUNCTION(BlueprintPure, Category = "Screen Data")
    TArray<FMingUIScreenData> GetAllScreenData() const;

    // Widget Management
    UFUNCTION(BlueprintCallable, Category = "Widgets")
    UUserWidget* CreateWidget(EMingUIScreen ScreenType);

    UFUNCTION(BlueprintCallable, Category = "Widgets")
    void DestroyWidget(EMingUIScreen ScreenType);

    UFUNCTION(BlueprintPure, Category = "Widgets")
    UUserWidget* GetWidget(EMingUIScreen ScreenType) const;

    // Event Delegates
    
    
    

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnScreenShown OnScreenShown;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnScreenHidden OnScreenHidden;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnUIScreenChanged OnUIScreenChanged;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetScreenName(EMingUIScreen ScreenType);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetAnimationTypeName(EMingUIAnimationType AnimationType);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveUISettings() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadUISettings(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<EMingUIScreen, FMingUIScreenData> ScreenDataMap;

    UPROPERTY()
    TMap<EMingUIScreen, UUserWidget*> ActiveWidgets;

    UPROPERTY()
    TArray<EMingUIScreen> ScreenHistory;

    UPROPERTY()
    int32 HistoryIndex;

    UPROPERTY()
    FMingUIStyle CurrentStyle;

    UPROPERTY()
    TMap<FString, FMingUIStyle> ThemeStyles;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeDefaultScreens();
    void InitializeDefaultThemes();
    void SetupScreenHistory();
    void UpdateScreenStack(EMingUIScreen ScreenType);

    // Animation Helpers
    void PlayAnimationInternal(UUserWidget* Widget, EMingUIAnimationType AnimationType, float Duration);
    void CleanupAnimation(UUserWidget* Widget);

    // Widget Helpers
    UUserWidget* CreateWidgetInternal(const FMingUIScreenData& ScreenData);
    void DestroyWidgetInternal(EMingUIScreen ScreenType);
    void PositionWidget(UUserWidget* Widget, EMingUIScreen ScreenType);

    // Navigation Helpers
    void AddToHistory(EMingUIScreen ScreenType);
    EMingUIScreen GetPreviousScreen() const;
    EMingUIScreen GetNextScreen() const;
};

