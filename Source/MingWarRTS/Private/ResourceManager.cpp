#include "ResourceManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h" // Needed to find PlayerController
#include "GameFramework/PlayerController.h"

UResourceManager::UResourceManager()
{
	PrimaryComponentTick.bCanEverTick = true; // Can be false if not needed every frame
}

void UResourceManager::BeginPlay()
{
	Super::BeginPlay();
	LoadInitialResources();

    // 嘗試從 PlayerController 獲取 ModifierManager (假設它被作為 Component 添加到了 PlayerController)
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        ModifierManagerRef = PC->FindComponentByClass<UModifierManager>();
        
        // 如果找不到 Component，嘗試找找看它是不是一個屬性 (雖然我們之前設計的是 UObject)
        // 為了穩健起見，我們先綁定
        if (ModifierManagerRef)
        {
            ModifierManagerRef->OnModifierApplied.AddDynamic(this, &UResourceManager::OnModifierApplied);
            UE_LOG(LogTemp, Log, TEXT("ResourceManager successfully bound to ModifierManager."));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ResourceManager could not find ModifierManager on PlayerController."));
        }
    }
}

void UResourceManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Unbind to prevent memory leaks or calling deleted objects
    if (ModifierManagerRef)
    {
        ModifierManagerRef->OnModifierApplied.RemoveDynamic(this, &UResourceManager::OnModifierApplied);
    }
    Super::EndPlay(EndPlayReason);
}

void UResourceManager::OnModifierApplied(FName ModifierID, float Value)
{
    // 在這裡處理收到的 Modifier
    // 例如，如果我們收到了一個 "res_political_power_gain" 的 modifier
    // 我們可能需要更新一個專門儲存「資源獲取率」的結構，而不是直接修改當前資源量
    
    UE_LOG(LogTemp, Log, TEXT("ResourceManager received Modifier: %s with value: %f"), *ModifierID.ToString(), Value);
    
    // 一個非常簡化的例子：如果 ModifierID 直接對應一個資源，我們就給它加上這個值 (假設 Value 是一個整數)
    if (PlayerResources.Contains(ModifierID))
    {
        PlayerResources[ModifierID] += FMath::RoundToInt(Value);
        UE_LOG(LogTemp, Log, TEXT("Updated Resource %s to %d"), *ModifierID.ToString(), PlayerResources[ModifierID]);
    }
}

void UResourceManager::LoadInitialResources()
{
    const FString FilePath = FPaths::ProjectContentDir() + TEXT("Data/resources.csv");

    TArray<FString> Lines;
    if (!FFileHelper::LoadFileToStringArray(Lines, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load resource file: %s"), *FilePath);
        return;
    }

    // Skip header line
    for (int32 i = 1; i < Lines.Num(); ++i)
    {
        FString& Line = Lines[i];
        TArray<FString> Fields;
        Line.ParseIntoArray(Fields, TEXT(","), true);

        if (Fields.Num() == 3)
        {
            FName ResourceID = FName(*Fields[0]);
            int32 StartingAmount = FCString::Atoi(*Fields[2]);
            PlayerResources.Add(ResourceID, StartingAmount);

            UE_LOG(LogTemp, Log, TEXT("Loaded Resource: %s with amount %d"), *ResourceID.ToString(), StartingAmount);
        }
    }
}

bool UResourceManager::HasEnoughResources(const TMap<FName, int32>& Costs) const
{
    for (const auto& Cost : Costs)
    {
        const int32* CurrentAmount = PlayerResources.Find(Cost.Key);
        // If we don't even have an entry for the resource, or we have less than needed
        if (!CurrentAmount || *CurrentAmount < Cost.Value)
        {
            return false;
        }
    }
    // If we passed all checks, we have enough
    return true;
}

bool UResourceManager::ConsumeResources(const TMap<FName, int32>& Costs)
{
    // IMPORTANT: First, verify we have enough of EVERYTHING before consuming ANYTHING.
    // This prevents partial consumption if a later check fails.
    if (!HasEnoughResources(Costs))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to consume resources: Insufficient funds."));
        return false;
    }

    // We have enough of everything, now consume them.
    for (const auto& Cost : Costs)
    {
        int32& CurrentAmount = PlayerResources.FindOrAdd(Cost.Key); // Use FindOrAdd for safety, though HasEnoughResources guarantees existence.
        CurrentAmount -= Cost.Value;
        UE_LOG(LogTemp, Log, TEXT("Consumed %d %s. Remaining: %d"), Cost.Value, *Cost.Key.ToString(), CurrentAmount);
    }
    
    return true;
}

void UResourceManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}