#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDeploymentManager.generated.h"

UENUM(BlueprintType)
enum class EMingBuildConfiguration : uint8
{
    Development UMETA(DisplayName = "Development"),
    Test UMETA(DisplayName = "Test"),
    Shipping UMETA(DisplayName = "Shipping"),
    Debug UMETA(DisplayName = "Debug")
};

UENUM(BlueprintType)
enum class EMingPlatformTarget : uint8
{
    Windows UMETA(DisplayName = "Windows"),
    Android UMETA(DisplayName = "Android"),
    iOS UMETA(DisplayName = "iOS"),
    Linux UMETA(DisplayName = "Linux"),
    Mac UMETA(DisplayName = "Mac")
};

UENUM(BlueprintType)
enum class EMingDeploymentStatus : uint8
{
    NotStarted UMETA(DisplayName = "Not Started"),
    InProgress UMETA(DisplayName = "In Progress"),
    Packaging UMETA(DisplayName = "Packaging"),
    Testing UMETA(DisplayName = "Testing"),
    Deploying UMETA(DisplayName = "Deploying"),
    Completed UMETA(DisplayName = "Completed"),
    Failed UMETA(DisplayName = "Failed")
};

USTRUCT(BlueprintType)
struct FMingBuildSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingBuildConfiguration BuildConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingPlatformTarget TargetPlatform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BuildVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OutputPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIncludeDebugFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompressPackage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIncludeDLC;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AdditionalAssets;

    FMingBuildSettings()
        : BuildConfig(EMingBuildConfiguration::Shipping)
        , TargetPlatform(EMingPlatformTarget::Windows)
        , bIncludeDebugFiles(false)
        , bCompressPackage(true)
        , bIncludeDLC(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingDeploymentResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BuildID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDeploymentStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PackagePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BuildTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 PackageSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Warnings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Errors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MD5Hash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DeploymentURL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Notes;

    FMingDeploymentResult()
        : Status(EMingDeploymentStatus::NotStarted)
        , BuildTime(0.0f)
        , PackageSize(0)
        , Warnings(0)
        , Errors(0)
    {}
};

/**
 * Deployment Manager for MingGoRTS
 * Manages build packaging and deployment process
 */
UCLASS(ClassGroup = (Deployment), Blueprintable)
class MINGDEPLOYMENT_API UMingDeploymentManager : public UObject
{
    GENERATED_BODY()

public:
    UMingDeploymentManager();

    // Build Control
    UFUNCTION(BlueprintCallable, Category = "Build Control")
    void InitializeDeploymentManager();

    UFUNCTION(BlueprintCallable, Category = "Build Control")
    void StartBuildProcess(const FMingBuildSettings& Settings) {};

    UFUNCTION(BlueprintCallable, Category = "Build Control")
    void CancelBuild();

    UFUNCTION(BlueprintCallable, Category = "Build Control")
    bool IsBuildInProgress() const;

    UFUNCTION(BlueprintPure, Category = "Build Control")
    FMingDeploymentResult GetCurrentBuildStatus() const;

    // Build Steps
    UFUNCTION(BlueprintCallable, Category = "Build Steps")
    void ExecuteBuildSteps();

    UFUNCTION(BlueprintCallable, Category = "Build Steps")
    void PreBuildValidation();

    UFUNCTION(BlueprintCallable, Category = "Build Steps")
    void CompileProject();

    UFUNCTION(BlueprintCallable, Category = "Build Steps")
    void CookContent();

    UFUNCTION(BlueprintCallable, Category = "Build Steps")
    void PackageGame();

    UFUNCTION(BlueprintCallable, Category = "Build Steps")
    void PostBuildValidation();

    UFUNCTION(BlueprintCallable, Category = "Build Steps")
    void GenerateInstaller();

    // Platform Support
    UFUNCTION(BlueprintCallable, Category = "Platform Support")
    void ConfigureForWindows();

    UFUNCTION(BlueprintCallable, Category = "Platform Support")
    void ConfigureForAndroid();

    UFUNCTION(BlueprintCallable, Category = "Platform Support")
    void ConfigureForIOS();

    UFUNCTION(BlueprintCallable, Category = "Platform Support")
    void SetupCrossPlatformAssets();

    // Deployment
    UFUNCTION(BlueprintCallable, Category = "Deployment")
    void DeployToStaging();

    UFUNCTION(BlueprintCallable, Category = "Deployment")
    void DeployToProduction();

    UFUNCTION(BlueprintCallable, Category = "Deployment")
    void CreateDistributionPackage();

    UFUNCTION(BlueprintCallable, Category = "Deployment")
    void UploadToCDN();

    UFUNCTION(BlueprintCallable, Category = "Deployment")
    void UpdateVersionInfo();

    // Version Management
    UFUNCTION(BlueprintCallable, Category = "Version Management")
    void IncrementVersion();

    UFUNCTION(BlueprintCallable, Category = "Version Management")
    void SetVersionNumber(const FString& Version) {};

    UFUNCTION(BlueprintPure, Category = "Version Management")
    FString GetCurrentVersion() const;

    UFUNCTION(BlueprintCallable, Category = "Version Management")
    void GenerateReleaseNotes();

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetBuildConfigName(EMingBuildConfiguration Config};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetPlatformName(EMingPlatformTarget Platform};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetDeploymentStatusName(EMingDeploymentStatus Status};

    UFUNCTION(BlueprintCallable, Category = "Utility")
    void ExportBuildManifest();

    UFUNCTION(BlueprintCallable, Category = "Utility")
    void CleanupOldBuilds();

protected:
    UPROPERTY()
    FMingBuildSettings CurrentSettings;

    UPROPERTY()
    FMingDeploymentResult CurrentResult;

    UPROPERTY()
    bool bBuildInProgress;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeBuildEnvironment();
    void ValidateBuildSettings();
    void SetupBuildDirectories();
    void CleanupBuildArtifacts();

    // Build Step Implementations
    bool ValidateSourceCode();
    bool RunCompilation();
    bool CookGameContent();
    bool CreateGamePackage();
    bool ValidatePackage();
    bool CreateInstallerPackage();

    // Platform Configuration
    void SetupWindowsSpecifics();
    void SetupAndroidSpecifics();
    void SetupIOSSpecifics();
    void SetupCrossPlatformSupport();

    // Deployment Implementation
    bool UploadToStagingServer();
    bool UploadToProductionServer();
    bool UpdateCDNCache();
    bool NotifyDistributionPlatforms();

    // Helpers
    FString GenerateBuildID();
    int64 CalculatePackageSize();
    FString CalculateMD5Hash();
    void LogBuildEvent(const FString& Event, const FString& Details) {};
    void ReportBuildProgress(float Progress, const FString& Stage) {};
};

