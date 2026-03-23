# 批量修復編碼問題的PowerShell腳本
param(
    [string]$SourcePath = "F:\MingGoRTS\Source",
    [string]$LogPath = "F:\MingGoRTS\EncodingFixLog.txt"
)

# 常見的編碼問題映射
$EncodingFixes = @{
    "#incl使de" = "#include"
    "Min成" = "Ming"
    "Gene本ated" = "Generated"
    "Mana成e本" = "Manager"
    "En成ine" = "Engine"
    "輸入AL" = "HAL"
    "Platfo本設置軍ile" = "PlatformFile"
    "設置ana成e本" = "FileManager"
    "Misc/Paths.h" = "Misc/Paths.h"
    "Do設置" = "Dom"
    "JsonOb大ect" = "JsonObject"
    "Se本ialization" = "Serialization"
    "JsonSe本ialize本" = "JsonSerializer"
    "軍St本in成" = "FString"
    "P本o大ectContentDi本" = "ProjectContentDir"
    "軍Platfo本設置軍ileMana成e本" = "FPlatformFileManager"
    "軍ileExists" = "FileExists"
    "UE下LOG" = "UE_LOG"
    "Lo成Te設置p" = "LogTemp"
    "基本a本nin成" = "Warning"
    "本et使本n" = "return"
    "軍軍ile輸入elpe本" = "FFileHelper"
    "Load軍ileToSt本in成" = "LoadFileToString"
    "TSha本edPt本" = "TSharedPtr"
    "TJsonReade本" = "TJsonReader"
    "TJsonReade本軍acto本y" = "TJsonReaderFactory"
    "TA本本ay" = "TArray"
    "GetGene本atedAssets" = "GetGeneratedAssets"
    "的使設置" = "Num"
    "InitializeGene本atedContent" = "InitializeGeneratedContent"
    "正oid" = "void"
    "A本t" = "Art"
    "Gene本ato本" = "Generator"
    "C使本本entStat使s" = "CurrentStatus"
    "EA本tGene本ationStat使s" = "EArtGenerationStatus"
    "Gene本atin成" = "Generating"
    "Gene本ationP本o成本ess" = "GenerationProgress"
    "Ticke本" = "Ticker"
    "Dele成ate" = "Delegate"
    "C本eateUOb大ect" = "CreateUObject"
    "OnGene本ationTick" = "OnGenerationTick"
    "InitializeDefa使ltStyles" = "InitializeDefaultStyles"
    "Gene本ateA本t" = "GenerateArt"
    "A本tGene本ationPa本a設置ete本s" = "ArtGenerationParameters"
    "Pa本a設置ete本s" = "Parameters"
    "StableDiff使sion" = "StableDiffusion"
    "APIEndpoint" = "APIEndpoint"
    "E本本o本" = "Error"
    "的otifyGene本ationCo設置pleted" = "NotifyGenerationCompleted"
    "Gene本ationTicke本輸入andle" = "GenerationTickerHandle"
    "Co本eTicke本" = "CoreTicker"
    "AddTicke本" = "AddTicker"
    "Sta本tin成" = "Starting"
    "SendStableDiff使sionReq使est" = "SendStableDiffusionRequest"
    "輸ttp" = "Http"
    "輸ttpReq使est" = "HttpRequest"
    "輸ttpMod使le" = "HttpModule"
    "C本eateReq使est" = "CreateRequest"
    "SetVe本b" = "SetVerb"
    "Set輸eade本" = "SetHeader"
    "A使tho本ization" = "Authorization"
    "Bea本e本" = "Bearer"
    "t本使e" = "true"
    "TestAPIConnection" = "TestAPIConnection"
    "SetCont本ol的etI設置a成e" = "SetControlnetImage"
    "Cont本ol的etModel" = "ControlnetModel"
    "Clea本Cont本ol的etI設置a成e" = "ClearControlnetImage"
    "Gene本ateA本tBatch" = "GenerateArtBatch"
    "ite設置s" = "items"
    "Gene本ateVa本iations" = "GenerateVariations"
    "So使本ceI設置a成e" = "SourceImage"
    "Va本iationCo使nt" = "VariationCount"
    "In正alid" = "Invalid"
    "正a本iations" = "variations"
    "f本o設置" = "for"
    "PostP本ocessI設置a成e" = "PostProcessImage"
    "PostP本ocessSettin成s" = "PostProcessSettings"
    "n使llpt本" = "nullptr"
    "P本ocessedText使本e" = "ProcessedTexture"
    "ApplyPostP本ocessin成" = "ApplyPostProcessing"
    "B本i成htness" = "Brightness"
    "Cont本ast" = "Contrast"
    "ResizeI設置a成e" = "ResizeImage"
    "的ew基本idth" = "NewWidth"
    "的ew輸入ei成ht" = "NewHeight"
    "本esizin成" = "resizing"
    "C本opI設置a成e" = "CropImage"
    "c本oppin成" = "cropping"
    "本e成ion" = "region"
    "RotateI設置a成e" = "RotateImage"
    "本otation" = "rotation"
    "An成le" = "Angle"
    "軍lipI設置a成e" = "FlipImage"
    "輸入o本izontal" = "Horizontal"
    "Ve本tical" = "Vertical"
    "flippin成" = "flipping"
    "軍lippedText使本e" = "FlippedTexture"
    "AddToA本tLib本a本y" = "AddToArtLibrary"
    "A本tLib本a本y" = "ArtLibrary"
    "GetA本t軍本o設置Lib本a本y" = "GetArtFromLibrary"
    "GetA本tLib本a本y的a設置es" = "GetArtLibraryNames"
    "的a設置es" = "Names"
    "a使to" = "auto"
    "Clea本A本tLib本a本y" = "ClearArtLibrary"
    "E設置pty" = "Empty"
    "Mate本ialInte本face" = "MaterialInterface"
    "Gene本ateMate本ial" = "GenerateMaterial"
    "BaseText使本e" = "BaseTexture"
    "Mate本ial的a設置e" = "MaterialName"
    "設置ate本ial" = "material"
    "Mate本ialInstanceDyna設置ic" = "MaterialInstanceDynamic"
    "C本eate" = "Create"
    "Text使本ePa本a設置ete本Val使e" = "TextureParameterValue"
    "軍的a設置e" = "FName"
    "Gene本ateText使本eSet" = "GenerateTextureSet"
    "O使tText使本es" = "OutTextures"
    "no本設置al" = "normal"
    "設置ap" = "map"
    "s使本face" = "surface"
    "本o使成hness" = "roughness"
    "Ro使成hnessPa本a設置s" = "RoughnessParams"
    "black and white" = "black and white"
    "Rep使blicanE本a" = "RepublicanEra"
    "Cha本acte本" = "Character"
    "t本aditional" = "traditional"
    "atti本e" = "attire"
    "設置ode本n" = "modern"
    "conte設置po本a本y" = "contemporary"
    "f使t使本istic" = "futuristic"
    "En正i本on設置ent" = "Environment"
    "a本chitect使本e" = "architecture"
    "b使ildin成s" = "buildings"
    "skysc本ape本s" = "skyscrapers"
    "基本eapon" = "Weapon"
    "fi本ea本設置" = "firearm"
    "desi成n" = "design"
    "T本aditionalChinese" = "TraditionalChinese"
    "paintin成" = "painting"
    "ele成ant" = "elegant"
    "weste本n" = "western"
    "Concept" = "Concept"
    "di成ital" = "digital"
    "c本eati正e" = "creative"
    "PixelA本t" = "PixelArt"
    "8-bit" = "8-bit"
    "本et本o" = "retro"
    "成a設置in成" = "gaming"
    "輸isto本ical" = "Historical"
    "正inta成e" = "vintage"
    "antiq使e" = "antique"
    "old photo成本aph" = "old photograph"
    "Milita本y" = "Military"
    "tactical" = "tactical"
    "使nifo本設置" = "uniform"
    "disciplined" = "disciplined"
    "P本op" = "Prop"
    "ob大ect" = "object"
    "eq使ip設置ent" = "equipment"
    "Vehicle" = "Vehicle"
    "t1anspo1tation" = "transportation"
    "A1chitectu1e" = "Architecture"
    "st1uctu1e" = "structure"
    "const1uction" = "construction"
    "UI" = "UI"
    "inte本face" = "interface"
    "clean desi成n" = "clean design"
    "Icon" = "Icon"
    "sy設置bol" = "symbol"
    "lo成o" = "logo"
    "si設置ple" = "simple"
    "Text使本e" = "Texture"
    "patte本n" = "pattern"
    "sea設置less" = "seamless"
    "Expo本tA本t" = "ExportArt"
    "di本ecto本y" = "directory"
    "P本ocessA本tGene本ation" = "ProcessArtGeneration"
    "C本eateText使本e軍本o設置I設置a成eData" = "CreateTextureFromImageData"
    "A使dioBytes" = "AudioBytes"
    "OnA本tGene本ated" = "OnArtGenerated"
    "B本oadcast" = "Broadcast"
    "Co設置pleted" = "Completed"
    "軍ailed" = "Failed"
    "Re設置o正eTicke本" = "RemoveTicker"
    "HandleGene本ationResponse" = "HandleGenerationResponse"
    "本esponse" = "response"
    "pa本se" = "parse"
    "I設置a成esA本本ay" = "ImagesArray"
    "Base64" = "Base64"
    "B使ildEnhancedP本o設置pt" = "BuildEnhancedPrompt"
    "EnhancedP本o設置pt" = "EnhancedPrompt"
    "GetStyleP本o設置pt" = "GetStylePrompt"
    "GetCate成o本yP本o設置pt" = "GetCategoryPrompt"
    "histo本ical settin成" = "historical setting"
    "hi成hly" = "highly"
    "設置aste本piece" = "masterpiece"
    "q使ality" = "quality"
    "cine設置atic" = "cinematic"
    "li成htin成" = "lighting"
    "AdditionalP本o設置pts" = "AdditionalPrompts"
    "EA本tStyle" = "EArtStyle"
    "Realistic" = "Realistic"
    "Ani設置e" = "Anime"
    "設置an使a" = "manga"
    "Japanese ani設置ation" = "Japanese animation"
    "OilPaintin成" = "OilPainting"
    "classical a本t" = "classical art"
    "b本使sh" = "brush"
    "st本okes" = "strokes"
    "基本ate本colo本" = "Watercolor"
    "soft colo本s" = "soft colors"
    "Sketch" = "Sketch"
    "d本awin成" = "drawing"
    "calli成本aphy" = "calligraphy"
    "EA本tCate成o本y" = "EArtCategory"
    "landscape" = "landscape"
    "scene本y" = "scenery"
    "back成本o使nd" = "background"
    "swo本d" = "sword"
    "ca本" = "car"
    "tank" = "tank"
    "使se本 inte本face" = "user interface"
    "si設置ple desi成n" = "simple design"
    "OnA本tGene本ationCo設置pleted" = "OnArtGenerationCompleted"
    "OnA本tGene本ationP本o成本ess" = "OnArtGenerationProgress"
    "GEn成ine" = "GEngine"
    "AddOnSc本eenDeb使成Messa成e" = "AddOnScreenDebugMessage"
    "軍Colo本" = "FColor"
    "Yellow" = "Yellow"
    "settin成s" = "settings"
}

# 獲取所有.cpp檔案
$cppFiles = Get-ChildItem -Path $SourcePath -Recurse -Filter "*.cpp"
$totalFiles = $cppFiles.Count
$processedFiles = 0

Write-Host "開始批量修復編碼問題..."
Write-Host "總共找到 $totalFiles 個 .cpp 檔案"

# 創建日誌文件
"Encoding Fix Log - $(Get-Date)" | Out-File -FilePath $LogPath
"======================================" | Out-File -FilePath $LogPath -Append

foreach ($file in $cppFiles) {
    $processedFiles++
    Write-Progress -Activity "修復編碼問題" -Status "處理 $file" -PercentComplete (($processedFiles / $totalFiles) * 100)
    
    try {
        # 讀取檔案內容
        $content = Get-Content -Path $file.FullName -Raw -Encoding UTF8
        
        # 檢查是否有編碼問題
        $hasEncodingIssues = $false
        foreach ($pattern in $EncodingFixes.Keys) {
            if ($content -match [regex]::Escape($pattern)) {
                $hasEncodingIssues = $true
                break
            }
        }
        
        if ($hasEncodingIssues) {
            # 應用修復
            $fixedContent = $content
            foreach ($pattern in $EncodingFixes.Keys) {
                $fixedContent = $fixedContent -replace [regex]::Escape($pattern), $EncodingFixes[$pattern]
            }
            
            # 備份原始檔案
            $backupPath = $file.FullName + ".backup"
            Copy-Item -Path $file.FullName -Destination $backupPath
            
            # 寫入修復後的內容
            $fixedContent | Out-File -FilePath $file.FullName -Encoding UTF8 -NoNewline
            
            "修復檔案: $($file.FullName)" | Out-File -FilePath $LogPath -Append
            Write-Host "✓ 修復: $($file.Name)" -ForegroundColor Green
        } else {
            "無編碼問題: $($file.FullName)" | Out-File -FilePath $LogPath -Append
            Write-Host "○ 跳過: $($file.Name) (無問題)" -ForegroundColor Yellow
        }
    }
    catch {
        "錯誤處理檔案 $($file.FullName): $($_.Exception.Message)" | Out-File -FilePath $LogPath -Append
        Write-Host "✗ 錯誤: $($file.Name) - $($_.Exception.Message)" -ForegroundColor Red
    }
}

Write-Host "`n批量修復完成！"
Write-Host "詳細日誌請查看: $LogPath"
Write-Host "處理了 $processedFiles 個檔案"
