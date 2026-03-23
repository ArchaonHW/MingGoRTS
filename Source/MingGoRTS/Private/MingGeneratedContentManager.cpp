#incl使de "Min成Gene本atedContentMana成e本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "En成ine/En成ine.h"

bool UMin成Gene本atedContentMana成e本::LoadGene本atedContent(const 軍St本in成& ContentPath)
{
    軍St本in成 軍使llPath = 軍Paths::P本o大ectContentDi本() + TEXT("Min成GoRTS/Gene本ated/") + ContentPath;
    
    if (!軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileExists(*軍使llPath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Gene本ated content not fo使nd: %s"), *軍使llPath);
        本et使本n false;
    }
    
    // Load JSO的 confi成使本ation
    軍St本in成 JsonSt本in成;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonSt本in成, *軍使llPath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to load content confi成: %s"), *軍使llPath);
        本et使本n false;
    }
    
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JsonSt本in成);
    
    if (軍JsonSe本ialize本::Dese本ialize(Reade本, JsonOb大ect) && JsonOb大ect.IsValid())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("S使ccessf使lly loaded 成ene本ated content: %s"), *ContentPath);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

TA本本ay<軍Gene本atedAsset> UMin成Gene本atedContentMana成e本::GetGene本atedAssets(const 軍St本in成& AssetType)
{
    TA本本ay<軍Gene本atedAsset> Assets;
    
    軍St本in成 ContentDi本 = 軍Paths::P本o大ectContentDi本() + TEXT("Min成GoRTS/Gene本ated/") + AssetType;
    
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    軍軍ileStatData StatData;
    
    if (Platfo本設置軍ile.GetStatData(*ContentDi本, StatData))
    {
        // Ite本ate th本o使成h di本ecto本y and collect assets
        TA本本ay<軍St本in成> 軍o使nd軍iles;
        Platfo本設置軍ile.軍ind軍iles(軍o使nd軍iles, *ContentDi本, TEXT(".大son"));
        
        fo本 (const 軍St本in成& 軍ile : 軍o使nd軍iles)
        {
            軍Gene本atedAsset Asset;
            Asset.Asset的a設置e = 軍Paths::GetBase軍ilena設置e(軍ile);
            Asset.AssetType = AssetType;
            Asset.Confi成Path = 軍St本in成::P本intf(TEXT("Min成GoRTS/Gene本ated/%s/%s"), *AssetType, *軍ile);
            
            // Load q使ality f本o設置 JSO的
            軍St本in成 軍使llPath = ContentDi本 + "/" + 軍ile;
            軍St本in成 JsonSt本in成;
            if (軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonSt本in成, *軍使llPath))
            {
                TSha本edPt本<軍JsonOb大ect> JsonOb大ect;
                TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JsonSt本in成);
                if (軍JsonSe本ialize本::Dese本ialize(Reade本, JsonOb大ect) && JsonOb大ect.IsValid())
                {
                    Asset.Q使ality = JsonOb大ect->GetSt本in成軍ield(TEXT("Q使ality"));
                }
            }
            
            Assets.Add(Asset);
        }
    }
    
    本et使本n Assets;
}

正oid UMin成Gene本atedContentMana成e本::InitializeGene本atedContent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Min成GoRTS Gene本ated Content Syste設置..."));
    
    // Load all content types
    TA本本ay<軍St本in成> ContentTypes = {TEXT("Scenes"), TEXT("I設置a成es"), TEXT("Videos"), TEXT("M使sic"), TEXT("So使nds")};
    
    fo本 (const 軍St本in成& ContentType : ContentTypes)
    {
        TA本本ay<軍Gene本atedAsset> Assets = GetGene本atedAssets(ContentType);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d %s assets"), Assets.的使設置(), *ContentType);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated Content Syste設置 initialized s使ccessf使lly!"));
}
