Add-Type -AssemblyName System.Drawing

$rawDir = "C:\HW\MingGoRTS\RawUIAssets"
New-Item -ItemType Directory -Force -Path $rawDir | Out-Null

function Make-Image($name, $w, $h, $bgHex, $borderHex) {
    $bmp = New-Object System.Drawing.Bitmap $w, $h
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    
    $bgColor = [System.Drawing.ColorTranslator]::FromHtml($bgHex)
    $borderColor = [System.Drawing.ColorTranslator]::FromHtml($borderHex)
    
    $g.Clear($bgColor)
    $pen = New-Object System.Drawing.Pen $borderColor, 4
    $g.DrawRectangle($pen, 2, 2, $w - 4, $h - 4)
    
    $filePath = Join-Path -Path $rawDir -ChildPath "$name.png"
    $bmp.Save($filePath, [System.Drawing.Imaging.ImageFormat]::Png)
    
    $pen.Dispose()
    $g.Dispose()
    $bmp.Dispose()
    
    Write-Host "Created: $filePath"
}

# Military / Mercenary Theme Colors
# Normal: Olive Drab Background with Khaki border
Make-Image "Btn_Normal" 256 64 "#2B3324" "#4A5D3A"
# Hover: Brighter Green Background with Gold/Khaki highlight border
Make-Image "Btn_Hover" 256 64 "#3D4833" "#748C5B"
# Pressed: Dark indent look
Make-Image "Btn_Pressed" 256 64 "#1A1F16" "#2B3324"
# Background: Gunmetal / Dark gray-green
Make-Image "BG_Military" 1920 1080 "#161814" "#2A2E26"

# Create proper UE5 folder structure in Content
New-Item -ItemType Directory -Force -Path "C:\HW\MingGoRTS\Content\UI" | Out-Null
New-Item -ItemType Directory -Force -Path "C:\HW\MingGoRTS\Content\UI\Textures" | Out-Null
New-Item -ItemType Directory -Force -Path "C:\HW\MingGoRTS\Content\UI\Widgets" | Out-Null
New-Item -ItemType Directory -Force -Path "C:\HW\MingGoRTS\Content\UI\Fonts" | Out-Null

Write-Host "All UI assets created successfully."