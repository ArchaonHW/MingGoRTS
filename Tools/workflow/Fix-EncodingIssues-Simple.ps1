# 簡化版編碼修復腳本
param(
    [string]$SourcePath = "F:\MingGoRTS\Source",
    [string]$LogPath = "F:\MingGoRTS\EncodingFixLog.txt"
)

# 基本編碼修復映射
$BasicFixes = @{
    "#incl使de" = "#include"
    "Min成" = "Ming"
    "Gene本ated" = "Generated"
    "Mana成e本" = "Manager"
    "En成ine" = "Engine"
    "軍St本in成" = "FString"
    "UE下LOG" = "UE_LOG"
    "Lo成Te設置p" = "LogTemp"
    "本et使本n" = "return"
    "TA本本ay" = "TArray"
    "正oid" = "void"
    "A本t" = "Art"
    "Gene本ato本" = "Generator"
    "Pa本a設置ete本s" = "Parameters"
    "E本本o本" = "Error"
    "t本使e" = "true"
    "輸ttp" = "Http"
    "n使llpt本" = "nullptr"
    "f本o設置" = "for"
    "的使設置" = "Num"
    "設置" = "m"
    "成" = "g"
    "本" = "n"
    "使" = "s"
    "軍" = "F"
    "下" = "T"
    "大" = "j"
    "的" = "d"
    "輸" = "H"
    "設" = "s"
    "置" = "z"
    "正" = "v"
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
        foreach ($pattern in $BasicFixes.Keys) {
            if ($content -match [regex]::Escape($pattern)) {
                $hasEncodingIssues = $true
                break
            }
        }
        
        if ($hasEncodingIssues) {
            # 應用修復
            $fixedContent = $content
            foreach ($pattern in $BasicFixes.Keys) {
                $fixedContent = $fixedContent -replace [regex]::Escape($pattern), $BasicFixes[$pattern]
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
