// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Auto-generated Asset-Blueprint Bridge: BP_MingWarrior
// Generated: 03/23/2026 02:58:27
// 蝝????誨蝣潮???- 鞈撘????摰頂蝯?
#pragma once

#include "CoreMinimal.h"
#include "AssetBlueprintBridge.generated.h"

USTRUCT(BlueprintType)
struct FAssetReference
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Reference")
    FString AssetName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Reference")
    FString AssetPath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Reference")
    TSoftObjectPtr<UObject> AssetSoftRef;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Reference")
    ELoadPriority LoadPriority;
};

UENUM(BlueprintType)
enum class ELoadPriority : uint8
{
    Low UMETA(DisplayName = "雿?? - Low"),
    Normal UMETA(DisplayName = "?桅?- Normal"),
    High UMETA(DisplayName = "擃?? - High"),
    Critical UMETA(DisplayName = "? - Critical")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UAssetBlueprintBridge : public UActorComponent
{
    GENERATED_BODY()

public:
    UAssetBlueprintBridge();

    UFUNCTION(BlueprintCallable, Category = "Asset Blueprint Integration | 鞈???")
    void InitializeAssetReferences(const FString& BlueprintName);
    
    UFUNCTION(BlueprintCallable, Category = "Asset Blueprint Integration | 鞈???")
    void LoadAssetsAsync();
    
    UFUNCTION(BlueprintCallable, Category = "Asset Blueprint Integration | 鞈???")
    bool IsAssetLoaded(const FString& AssetName) const;
    
    UFUNCTION(BlueprintCallable, Category = "Asset Blueprint Integration | 鞈???")
    UObject* GetLoadedAsset(const FString& AssetName);
    
    UFUNCTION(BlueprintCallable, Category = "Asset Blueprint Integration | 鞈???")
    void BindAssetToBlueprint(UObject* Asset, const FString& BlueprintPropertyName);

    // Asset reference configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset References | 鞈撘")
    TArray<FAssetReference> AssetReferences;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset References | 鞈撘")
    FString TargetBlueprintName;

protected:
    UPROPERTY()
    TMap<FString, TObjectPtr<UObject>> LoadedAssets;
    
    UPROPERTY()
    bool bAssetsLoaded;
    
    void OnAssetLoaded(FString AssetName, UObject* LoadedAsset);
};

// Implementation template (save to .cpp file)
/*
UAssetBlueprintBridge::UAssetBlueprintBridge()
    : bAssetsLoaded(false)
{
}

void UAssetBlueprintBridge::InitializeAssetReferences(const FString& BlueprintName)
{
    TargetBlueprintName = BlueprintName;
    
    // Load asset reference configuration from JSON
    FString ConfigPath = FPaths::ProjectContentDir() + TEXT("Generated/AssetBlueprintIntegration/") + BlueprintName + TEXT("_AssetRefConfig.json");
    FString JsonString;
    if (FFileHelper::LoadFileToString(JsonString, *ConfigPath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            // Parse asset references from JSON
            const TArray<TSharedPtr<FJsonValue>>* AssetRefs;
            if (JsonObject->TryGetArrayField(TEXT("AssetReferences"), AssetRefs))
            {
                for (const auto& Ref : *AssetRefs)
                {
                    TSharedPtr<FJsonObject> RefObj = Ref->AsObject();
                    if (RefObj.IsValid())
                    {
                        FAssetReference AssetRef;
                        AssetRef.AssetName = RefObj->GetStringField(TEXT("AssetName"));
                        AssetRef.AssetPath = RefObj->GetStringField(TEXT("AssetPath"));
                        // Parse load priority
                        FString PriorityStr = RefObj->GetStringField(TEXT("LoadPriority"));
                        AssetRef.LoadPriority = StaticCast<ELoadPriority>(StaticCast<int32>(PriorityStr));
                        AssetReferences.Add(AssetRef);
                    }
                }
            }
        }
    }
}

void UAssetBlueprintBridge::LoadAssetsAsync()
{
    // Async load all referenced assets
    for (const FAssetReference& Ref : AssetReferences)
    {
        FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
        FSoftObjectPath AssetPath(Ref.AssetPath);
        
        Streamable.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateLambda([this, Ref]()
        {
            UObject* Asset = Cast<UObject>(AssetPath.TryLoad());
            if (Asset)
            {
                OnAssetLoaded(Ref.AssetName, Asset);
            }
        }));
    }
}

bool UAssetBlueprintBridge::IsAssetLoaded(const FString& AssetName) const
{
    return LoadedAssets.Contains(AssetName);
}

UObject* UAssetBlueprintBridge::GetLoadedAsset(const FString& AssetName)
{
    if (LoadedAssets.Contains(AssetName))
    {
        return LoadedAssets[AssetName];
    }
    return nullptr;
}

void UAssetBlueprintBridge::BindAssetToBlueprint(UObject* Asset, const FString& BlueprintPropertyName)
{
    // Bind loaded asset to Blueprint property
    AActor* Owner = GetOwner();
    if (Owner)
    {
        UClass* OwnerClass = Owner->GetClass();
        FProperty* Property = OwnerClass->FindPropertyByName(FName(*BlueprintPropertyName));
        
        if (Property)
        {
            // Set property value
            FObjectProperty* ObjProp = CastField<FObjectProperty>(Property);
            if (ObjProp)
            {
                ObjProp->SetObjectPropertyValue(ObjProp->ContainerPtrToValuePtr<void>(Owner), Asset);
                UE_LOG(LogTemp, Log, TEXT("Bound asset %s to property %s"), *Asset->GetName(), *BlueprintPropertyName);
            }
        }
    }
}

void UAssetBlueprintBridge::OnAssetLoaded(FString AssetName, UObject* LoadedAsset)
{
    LoadedAssets.Add(AssetName, LoadedAsset);
    UE_LOG(LogTemp, Log, TEXT("Asset loaded: %s"), *AssetName);
    
    // Check if all assets are loaded
    if (LoadedAssets.Num() == AssetReferences.Num())
    {
        bAssetsLoaded = true;
        UE_LOG(LogTemp, Log, TEXT("All assets loaded for blueprint: %s"), *TargetBlueprintName);
        
        // Notify Blueprint that assets are ready
        // This would typically call a Blueprint-implementable event
    }
}
*/
