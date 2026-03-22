# 安裝本機 AI 圖像生成工具
# Stable Diffusion WebUI 安裝腳本

param(
    [string]$InstallPath = "C:\AI_Tools",
    [string]$Tool = "stable-diffusion-webui",  # stable-diffusion-webui, comfyui, fooocus
    [switch]$AutoInstall = $false
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "    本機 AI 圖像生成工具安裝" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# 檢查系統要求
function Test-SystemRequirements {
    Write-Host "`n🔍 檢查系統要求..." -ForegroundColor Yellow
    
    # 檢查 Python
    try {
        $pythonVersion = python --version 2>$null
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ Python: $pythonVersion" -ForegroundColor Green
        } else {
            Write-Host "❌ Python 未安裝" -ForegroundColor Red
            return $false
        }
    } catch {
        Write-Host "❌ Python 檢查失敗" -ForegroundColor Red
        return $false
    }
    
    # 檢查 Git
    try {
        $gitVersion = git --version 2>$null
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ Git: $gitVersion" -ForegroundColor Green
        } else {
            Write-Host "❌ Git 未安裝" -ForegroundColor Red
            Write-Host "請先安裝 Git: https://git-scm.com/" -ForegroundColor Yellow
            return $false
        }
    } catch {
        Write-Host "❌ Git 檢查失敗" -ForegroundColor Red
        return $false
    }
    
    # 檢查硬碟空間
    $drive = Get-PSDrive -Name C
    $freeSpaceGB = [math]::Round($drive.Free / 1GB, 2)
    if ($freeSpaceGB -gt 20) {
        Write-Host "✅ 硬碟空間: $freeSpaceGB GB 可用" -ForegroundColor Green
    } else {
        Write-Host "⚠️ 硬碟空間不足: 僅剩 $freeSpaceGB GB" -ForegroundColor Yellow
    }
    
    # 檢查 GPU
    try {
        $gpu = Get-WmiObject -Class Win32_VideoController | Select-Object Name
        Write-Host "🎮 GPU: $($gpu.Name)" -ForegroundColor Green
    } catch {
        Write-Host "⚠️ GPU 檢查失敗" -ForegroundColor Yellow
    }
    
    return $true
}

# 安裝 Stable Diffusion WebUI
function Install-StableDiffusionWebUI {
    Write-Host "`n🚀 安裝 Stable Diffusion WebUI..." -ForegroundColor Green
    
    $webuiPath = "$InstallPath\stable-diffusion-webui"
    
    # 創建安裝目錄
    if (!(Test-Path $InstallPath)) {
        New-Item -ItemType Directory -Path $InstallPath -Force | Out-Null
        Write-Host "📁 創建安裝目錄: $InstallPath" -ForegroundColor Green
    }
    
    # 克隆倉庫
    if (!(Test-Path $webuiPath)) {
        Write-Host "📥 下載 Stable Diffusion WebUI..." -ForegroundColor Yellow
        Set-Location $InstallPath
        git clone https://github.com/AUTOMATIC1111/stable-diffusion-webui.git
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ 下載完成" -ForegroundColor Green
        } else {
            Write-Host "❌ 下載失敗" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "📁 Stable Diffusion WebUI 已存在" -ForegroundColor Yellow
    }
    
    # 下載模型
    Write-Host "📥 下載推薦模型..." -ForegroundColor Yellow
    Set-Location $webuiPath
    
    # 創建模型目錄
    $modelDir = "$webuiPath\models\Stable-diffusion"
    if (!(Test-Path $modelDir)) {
        New-Item -ItemType Directory -Path $modelDir -Force | Out-Null
    }
    
    # 下載 Deliberate 模型 (適合歷史風格)
    $modelUrl = "https://huggingface.co/cyberdelia/Deliberate/resolve/main/Deliberate_v2.safetensors"
    $modelPath = "$modelDir\Deliberate_v2.safetensors"
    
    if (!(Test-Path $modelPath)) {
        Write-Host "📥 下載 Deliberate 模型 (適合歷史風格)..." -ForegroundColor Yellow
        try {
            # 使用 Invoke-WebRequest 下載
            Invoke-WebRequest -Uri $modelUrl -OutFile $modelPath
            Write-Host "✅ Deliberate 模型下載完成" -ForegroundColor Green
        } catch {
            Write-Host "⚠️ 模型下載失敗，請手動下載" -ForegroundColor Yellow
            Write-Host "下載地址: $modelUrl" -ForegroundColor Yellow
        }
    }
    
    return $true
}

# 安裝 ComfyUI
function Install-ComfyUI {
    Write-Host "`n🚀 安裝 ComfyUI..." -ForegroundColor Green
    
    $comfyuiPath = "$InstallPath\ComfyUI"
    
    # 創建安裝目錄
    if (!(Test-Path $InstallPath)) {
        New-Item -ItemType Directory -Path $InstallPath -Force | Out-Null
    }
    
    # 克隆倉庫
    if (!(Test-Path $comfyuiPath)) {
        Write-Host "📥 下載 ComfyUI..." -ForegroundColor Yellow
        Set-Location $InstallPath
        git clone https://github.com/comfyanonymous/ComfyUI.git
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ 下載完成" -ForegroundColor Green
        } else {
            Write-Host "❌ 下載失敗" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "📁 ComfyUI 已存在" -ForegroundColor Yellow
    }
    
    return $true
}

# 安裝 Fooocus
function Install-Fooocus {
    Write-Host "`n🚀 安裝 Fooocus..." -ForegroundColor Green
    
    $fooocusPath = "$InstallPath\Fooocus"
    
    # 創建安裝目錄
    if (!(Test-Path $InstallPath)) {
        New-Item -ItemType Directory -Path $InstallPath -Force | Out-Null
    }
    
    # 克隆倉庫
    if (!(Test-Path $fooocusPath)) {
        Write-Host "📥 下載 Fooocus..." -ForegroundColor Yellow
        Set-Location $InstallPath
        git clone https://github.com/lllyasviel/Fooocus.git
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ 下載完成" -ForegroundColor Green
        } else {
            Write-Host "❌ 下載失敗" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "📁 Fooocus 已存在" -ForegroundColor Yellow
    }
    
    return $true
}

# 創建啟動腳本
function New-StartupScripts {
    Write-Host "`n📝 創建啟動腳本..." -ForegroundColor Green
    
    $scriptsDir = "$InstallPath\Scripts"
    New-Item -ItemType Directory -Path $scriptsDir -Force | Out-Null
    
    # Stable Diffusion WebUI 啟動腳本
    $webuiScript = @"
@echo off
echo 啟動 Stable Diffusion WebUI...
cd /d "$InstallPath\stable-diffusion-webui"
webui-user.bat
pause
"@
    $webuiScript | Out-File -FilePath "$scriptsDir\Start_StableDiffusion.bat" -Encoding ASCII
    
    # ComfyUI 啟動腳本
    $comfyuiScript = @"
@echo off
echo 啟動 ComfyUI...
cd /d "$InstallPath\ComfyUI"
python main.py
pause
"@
    $comfyuiScript | Out-File -FilePath "$scriptsDir\Start_ComfyUI.bat" -Encoding ASCII
    
    # Fooocus 啟動腳本
    $fooocusScript = @"
@echo off
echo 啟動 Fooocus...
cd /d "$InstallPath\Fooocus"
python launch.py
pause
"@
    $fooocusScript | Out-File -FilePath "$scriptsDir\Start_Fooocus.bat" -Encoding ASCII
    
    Write-Host "✅ 啟動腳本已創建在 $scriptsDir" -ForegroundColor Green
}

# 創建快捷方式
function New-Shortcuts {
    Write-Host "`n🔗 創建桌面快捷方式..." -ForegroundColor Green
    
    $desktop = [Environment]::GetFolderPath("Desktop")
    
    # Stable Diffusion WebUI 快捷方式
    $shortcutPath = "$desktop\Stable Diffusion WebUI.lnk"
    $shell = New-Object -ComObject WScript.Shell
    $shortcut = $shell.CreateShortcut($shortcutPath)
    $shortcut.TargetPath = "$scriptsDir\Start_StableDiffusion.bat"
    $shortcut.WorkingDirectory = "$InstallPath\stable-diffusion-webui"
    $shortcut.Description = "啟動 Stable Diffusion WebUI"
    $shortcut.Save()
    
    Write-Host "✅ 桌面快捷方式已創建" -ForegroundColor Green
}

# 顯示使用說明
function Show-UsageInstructions {
    Write-Host @"
========================================
🎯 使用說明
========================================

1. 首次啟動:
   - 雙擊桌面快捷方式 "Stable Diffusion WebUI"
   - 等待自動下載依賴 (首次啟動較慢)
   - 瀏覽器會自動開啟 WebUI 介面

2. MingGoRTS 提示詞:
   - 使用我們準備的提示詞
   - 設置解析度: 1920x1080
   - 選擇 Deliberate 模型
   - 調整參數獲得最佳效果

3. 模型推薦:
   - Deliberate_v2: 適合歷史風格
   - Realistic Vision: 適合寫實風格
   - Dreamshaper: 適合藝術風格

4. 參數建議:
   - Sampling Steps: 30-50
   - CFG Scale: 7-10
   - Sampler: DPM++ 2M Karras
   - Resolution: 1920x1080

========================================
📁 安裝路徑: $InstallPath
========================================
"@ -ForegroundColor Cyan
}

# 主安裝流程
function Start-Installation {
    Write-Host "開始安裝本機 AI 圖像生成工具..." -ForegroundColor Green
    
    # 檢查系統要求
    if (!(Test-SystemRequirements)) {
        Write-Host "❌ 系統要求不滿足，安裝終止" -ForegroundColor Red
        return
    }
    
    # 根據選擇的工具進行安裝
    switch ($Tool) {
        "stable-diffusion-webui" {
            if (!(Install-StableDiffusionWebUI)) {
                Write-Host "❌ Stable Diffusion WebUI 安裝失敗" -ForegroundColor Red
                return
            }
        }
        "comfyui" {
            if (!(Install-ComfyUI)) {
                Write-Host "❌ ComfyUI 安裝失敗" -ForegroundColor Red
                return
            }
        }
        "fooocus" {
            if (!(Install-Fooocus)) {
                Write-Host "❌ Fooocus 安裝失敗" -ForegroundColor Red
                return
            }
        }
        default {
            Write-Host "❌ 不支援的工具: $Tool" -ForegroundColor Red
            return
        }
    }
    
    # 創建啟動腳本
    New-StartupScripts
    
    # 創建快捷方式
    New-Shortcuts
    
    # 顯示使用說明
    Show-UsageInstructions
    
    Write-Host "🎉 安裝完成！" -ForegroundColor Green
}

# 執行安裝
if ($AutoInstall) {
    Start-Installation
} else {
    Write-Host @"
========================================
選擇要安裝的 AI 工具:

1. Stable Diffusion WebUI (推薦)
2. ComfyUI (進階)
3. Fooocus (簡易)

使用方式:
.\Install_Local_AI.ps1 -Tool stable-diffusion-webui -AutoInstall
.\Install_Local_AI.ps1 -Tool comfyui -AutoInstall
.\Install_Local_AI.ps1 -Tool fooocus -AutoInstall

自定義安裝路徑:
.\Install_Local_AI.ps1 -Tool stable-diffusion-webui -InstallPath "D:\AI_Tools" -AutoInstall
"@ -ForegroundColor Cyan
    
    Write-Host "`n按 Enter 繼續..." -ForegroundColor Yellow
    Read-Host
}
