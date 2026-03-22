# OPTIMIZED: Script uses buffered logging for better performance
# Original had multiple Write-Host calls that can slow execution

$Script:LogBuffer = @()
function Write-BufferedLog {
    param([string]$Message)
    $Script:LogBuffer += "[03:06:25] $Message"
    if ($Script:LogBuffer.Count -ge 100) { Flush-LogBuffer }
}
function Flush-LogBuffer {
    $Script:LogBuffer | ForEach-Object { Write-Host $_ }
    $Script:LogBuffer = @()
}

# --- ORIGINAL SCRIPT BELOW ---
# 摰??祆? AI ????撌亙
# Stable Diffusion WebUI 摰??單

param(
    [string]$InstallPath = "C:\AI_Tools",
    [string]$Tool = "stable-diffusion-webui",  # stable-diffusion-webui, comfyui, fooocus
    [switch]$AutoInstall = $false
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "    ?祆? AI ????撌亙摰?" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# 瑼Ｘ蝟餌絞閬?
function Test-SystemRequirements {
    Write-Host "`n?? 瑼Ｘ蝟餌絞閬?..." -ForegroundColor Yellow
    
    # 瑼Ｘ Python
    try {
        $pythonVersion = python --version 2>$null
        if ($LASTEXITCODE -eq 0) {
            Write-Host "??Python: $pythonVersion" -ForegroundColor Green
        } else {
            Write-Host "??Python ?芸?鋆? -ForegroundColor Red
            return $false
        }
    } catch {
        Write-Host "??Python 瑼Ｘ憭望?" -ForegroundColor Red
        return $false
    }
    
    # 瑼Ｘ Git
    try {
        $gitVersion = git --version 2>$null
        if ($LASTEXITCODE -eq 0) {
            Write-Host "??Git: $gitVersion" -ForegroundColor Green
        } else {
            Write-Host "??Git ?芸?鋆? -ForegroundColor Red
            Write-Host "隢?摰? Git: https://git-scm.com/" -ForegroundColor Yellow
            return $false
        }
    } catch {
        Write-Host "??Git 瑼Ｘ憭望?" -ForegroundColor Red
        return $false
    }
    
    # 瑼Ｘ蝖祉?蝛粹?
    $drive = Get-PSDrive -Name C
    $freeSpaceGB = [math]::Round($drive.Free / 1GB, 2)
    if ($freeSpaceGB -gt 20) {
        Write-Host "??蝖祉?蝛粹?: $freeSpaceGB GB ?舐" -ForegroundColor Green
    } else {
        Write-Host "?? 蝖祉?蝛粹?銝雲: ? $freeSpaceGB GB" -ForegroundColor Yellow
    }
    
    # 瑼Ｘ GPU
    try {
        $gpu = Get-WmiObject -Class Win32_VideoController | Select-Object Name
        Write-Host "? GPU: $($gpu.Name)" -ForegroundColor Green
    } catch {
        Write-Host "?? GPU 瑼Ｘ憭望?" -ForegroundColor Yellow
    }
    
    return $true
}

# 摰? Stable Diffusion WebUI
function Install-StableDiffusionWebUI {
    Write-Host "`n?? 摰? Stable Diffusion WebUI..." -ForegroundColor Green
    
    $webuiPath = "$InstallPath\stable-diffusion-webui"
    
    # ?萄遣摰??桅?
    if (!(Test-Path $InstallPath)) {
        New-Item -ItemType Directory -Path $InstallPath -Force | Out-Null
        Write-Host "?? ?萄遣摰??桅?: $InstallPath" -ForegroundColor Green
    }
    
    # ???澈
    if (!(Test-Path $webuiPath)) {
        Write-Host "? 銝? Stable Diffusion WebUI..." -ForegroundColor Yellow
        Set-Location $InstallPath
        git clone https://github.com/AUTOMATIC1111/stable-diffusion-webui.git
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "??銝?摰?" -ForegroundColor Green
        } else {
            Write-Host "??銝?憭望?" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "?? Stable Diffusion WebUI 撌脣??? -ForegroundColor Yellow
    }
    
    # 銝?璅∪?
    Write-Host "? 銝??刻璅∪?..." -ForegroundColor Yellow
    Set-Location $webuiPath
    
    # ?萄遣璅∪??桅?
    $modelDir = "$webuiPath\models\Stable-diffusion"
    if (!(Test-Path $modelDir)) {
        New-Item -ItemType Directory -Path $modelDir -Force | Out-Null
    }
    
    # 銝? Deliberate 璅∪? (?拙?甇瑕憸冽)
    $modelUrl = "https://huggingface.co/cyberdelia/Deliberate/resolve/main/Deliberate_v2.safetensors"
    $modelPath = "$modelDir\Deliberate_v2.safetensors"
    
    if (!(Test-Path $modelPath)) {
        Write-Host "? 銝? Deliberate 璅∪? (?拙?甇瑕憸冽)..." -ForegroundColor Yellow
        try {
            # 雿輻 Invoke-WebRequest 銝?
            Invoke-WebRequest -Uri $modelUrl -OutFile $modelPath
            Write-Host "??Deliberate 璅∪?銝?摰?" -ForegroundColor Green
        } catch {
            Write-Host "?? 璅∪?銝?憭望?嚗???銝?" -ForegroundColor Yellow
            Write-Host "銝??啣?: $modelUrl" -ForegroundColor Yellow
        }
    }
    
    return $true
}

# 摰? ComfyUI
function Install-ComfyUI {
    Write-Host "`n?? 摰? ComfyUI..." -ForegroundColor Green
    
    $comfyuiPath = "$InstallPath\ComfyUI"
    
    # ?萄遣摰??桅?
    if (!(Test-Path $InstallPath)) {
        New-Item -ItemType Directory -Path $InstallPath -Force | Out-Null
    }
    
    # ???澈
    if (!(Test-Path $comfyuiPath)) {
        Write-Host "? 銝? ComfyUI..." -ForegroundColor Yellow
        Set-Location $InstallPath
        git clone https://github.com/comfyanonymous/ComfyUI.git
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "??銝?摰?" -ForegroundColor Green
        } else {
            Write-Host "??銝?憭望?" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "?? ComfyUI 撌脣??? -ForegroundColor Yellow
    }
    
    return $true
}

# 摰? Fooocus
function Install-Fooocus {
    Write-Host "`n?? 摰? Fooocus..." -ForegroundColor Green
    
    $fooocusPath = "$InstallPath\Fooocus"
    
    # ?萄遣摰??桅?
    if (!(Test-Path $InstallPath)) {
        New-Item -ItemType Directory -Path $InstallPath -Force | Out-Null
    }
    
    # ???澈
    if (!(Test-Path $fooocusPath)) {
        Write-Host "? 銝? Fooocus..." -ForegroundColor Yellow
        Set-Location $InstallPath
        git clone https://github.com/lllyasviel/Fooocus.git
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "??銝?摰?" -ForegroundColor Green
        } else {
            Write-Host "??銝?憭望?" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "?? Fooocus 撌脣??? -ForegroundColor Yellow
    }
    
    return $true
}

# ?萄遣???單
function New-StartupScripts {
    Write-Host "`n?? ?萄遣???單..." -ForegroundColor Green
    
    $scriptsDir = "$InstallPath\Scripts"
    New-Item -ItemType Directory -Path $scriptsDir -Force | Out-Null
    
    # Stable Diffusion WebUI ???單
    $webuiScript = @"
@echo off
echo ?? Stable Diffusion WebUI...
cd /d "$InstallPath\stable-diffusion-webui"
webui-user.bat
pause
"@
    $webuiScript | Out-File -FilePath "$scriptsDir\Start_StableDiffusion.bat" -Encoding ASCII
    
    # ComfyUI ???單
    $comfyuiScript = @"
@echo off
echo ?? ComfyUI...
cd /d "$InstallPath\ComfyUI"
python main.py
pause
"@
    $comfyuiScript | Out-File -FilePath "$scriptsDir\Start_ComfyUI.bat" -Encoding ASCII
    
    # Fooocus ???單
    $fooocusScript = @"
@echo off
echo ?? Fooocus...
cd /d "$InstallPath\Fooocus"
python launch.py
pause
"@
    $fooocusScript | Out-File -FilePath "$scriptsDir\Start_Fooocus.bat" -Encoding ASCII
    
    Write-Host "?????單撌脣撱箏 $scriptsDir" -ForegroundColor Green
}

# ?萄遣敹急?孵?
function New-Shortcuts {
    Write-Host "`n?? ?萄遣獢敹急?孵?..." -ForegroundColor Green
    
    $desktop = [Environment]::GetFolderPath("Desktop")
    
    # Stable Diffusion WebUI 敹急?孵?
    $shortcutPath = "$desktop\Stable Diffusion WebUI.lnk"
    $shell = New-Object -ComObject WScript.Shell
    $shortcut = $shell.CreateShortcut($shortcutPath)
    $shortcut.TargetPath = "$scriptsDir\Start_StableDiffusion.bat"
    $shortcut.WorkingDirectory = "$InstallPath\stable-diffusion-webui"
    $shortcut.Description = "?? Stable Diffusion WebUI"
    $shortcut.Save()
    
    Write-Host "??獢敹急?孵?撌脣撱? -ForegroundColor Green
}

# 憿舐內雿輻隤芣?
function Show-UsageInstructions {
    Write-Host @"
========================================
? 雿輻隤芣?
========================================

1. 擐活??:
   - ??獢敹急?孵? "Stable Diffusion WebUI"
   - 蝑??芸?銝?靘陷 (擐活??頛)
   - ?汗?冽??芸??? WebUI 隞

2. MingGoRTS ?內閰?
   - 雿輻?????內閰?
   - 閮剔蔭閫??摨? 1920x1080
   - ?豢? Deliberate 璅∪?
   - 隤踵??脣??雿單???

3. 璅∪??刻:
   - Deliberate_v2: ?拙?甇瑕憸冽
   - Realistic Vision: ?拙?撖怠祕憸冽
   - Dreamshaper: ?拙???憸冽

4. ?撱箄降:
   - Sampling Steps: 30-50
   - CFG Scale: 7-10
   - Sampler: DPM++ 2M Karras
   - Resolution: 1920x1080

========================================
?? 摰?頝臬?: $InstallPath
========================================
"@ -ForegroundColor Cyan
}

# 銝餃?鋆?蝔?
function Start-Installation {
    Write-Host "??摰??祆? AI ????撌亙..." -ForegroundColor Green
    
    # 瑼Ｘ蝟餌絞閬?
    if (!(Test-SystemRequirements)) {
        Write-Host "??蝟餌絞閬?銝遛頞喉?摰?蝯迫" -ForegroundColor Red
        return
    }
    
    # ?寞??豢??極?琿脰?摰?
    switch ($Tool) {
        "stable-diffusion-webui" {
            if (!(Install-StableDiffusionWebUI)) {
                Write-Host "??Stable Diffusion WebUI 摰?憭望?" -ForegroundColor Red
                return
            }
        }
        "comfyui" {
            if (!(Install-ComfyUI)) {
                Write-Host "??ComfyUI 摰?憭望?" -ForegroundColor Red
                return
            }
        }
        "fooocus" {
            if (!(Install-Fooocus)) {
                Write-Host "??Fooocus 摰?憭望?" -ForegroundColor Red
                return
            }
        }
        default {
            Write-Host "??銝?渡?撌亙: $Tool" -ForegroundColor Red
            return
        }
    }
    
    # ?萄遣???單
    New-StartupScripts
    
    # ?萄遣敹急?孵?
    New-Shortcuts
    
    # 憿舐內雿輻隤芣?
    Show-UsageInstructions
    
    Write-Host "?? 摰?摰?嚗? -ForegroundColor Green
}

# ?瑁?摰?
if ($AutoInstall) {
    Start-Installation
} else {
    Write-Host @"
========================================
?豢?閬?鋆? AI 撌亙:

1. Stable Diffusion WebUI (?刻)
2. ComfyUI (?脤?)
3. Fooocus (蝪⊥?)

雿輻?孵?:
.\Install_Local_AI.ps1 -Tool stable-diffusion-webui -AutoInstall
.\Install_Local_AI.ps1 -Tool comfyui -AutoInstall
.\Install_Local_AI.ps1 -Tool fooocus -AutoInstall

?芸?蝢拙?鋆楝敺?
.\Install_Local_AI.ps1 -Tool stable-diffusion-webui -InstallPath "D:\AI_Tools" -AutoInstall
"@ -ForegroundColor Cyan
    
    Write-Host "`n??Enter 蝜潛?..." -ForegroundColor Yellow
    Read-Host
}

