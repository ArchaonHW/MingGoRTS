Add-Type -AssemblyName System.Drawing

$rawDir = "C:\HW\MingGoRTS\RawUIAssets"
if (!(Test-Path $rawDir)) { New-Item -ItemType Directory -Force -Path $rawDir | Out-Null }

function Make-VintageButton($name, $w, $h, $colorBg, $borderColor, $highlightColor) {
    $bmp = New-Object System.Drawing.Bitmap $w, $h
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::None
    
    $cBg = [System.Drawing.ColorTranslator]::FromHtml($colorBg)
    $cBorder = [System.Drawing.ColorTranslator]::FromHtml($borderColor)
    $cHigh = [System.Drawing.ColorTranslator]::FromHtml($highlightColor)

    # Background
    $g.Clear($cBg)

    # Border (Outer)
    $penBorder = New-Object System.Drawing.Pen $cBorder, 4
    $g.DrawRectangle($penBorder, 2, 2, $w - 4, $h - 4)

    # Highlight (Inner Bevel effect)
    $penHigh = New-Object System.Drawing.Pen $cHigh, 2
    $g.DrawRectangle($penHigh, 5, 5, $w - 10, $h - 10)

    $filePath = Join-Path -Path $rawDir -ChildPath "$name.png"
    $bmp.Save($filePath, [System.Drawing.Imaging.ImageFormat]::Png)

    $penBorder.Dispose()
    $penHigh.Dispose()
    $g.Dispose()
    $bmp.Dispose()

    Write-Host "Created Button: $filePath"
}

# 1. Normal State: Vintage Brass / Dark Bronze (暗銅色)
Make-VintageButton "Btn_MingWar_Normal" 256 64 "#4A3B2C" "#8B6508" "#5E4A37"

# 2. Hover State: Bright Gold / Nationalist Blue Hint (青天白日藍 + 黃金邊框)
Make-VintageButton "Btn_MingWar_Hover" 256 64 "#1B3045" "#DAA520" "#284A6A"

# 3. Pressed State: Dark Iron / Blood Red Hint (暗鐵紅 + 鏽鐵邊框)
Make-VintageButton "Btn_MingWar_Pressed" 256 64 "#2A1616" "#5C3A21" "#1A0E0E"

Write-Host "All MingWar vintage buttons generated successfully."