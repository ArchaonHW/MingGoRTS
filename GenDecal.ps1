Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName System.Drawing.Drawing2D

$rawDir = "C:\HW\MingGoRTS\RawUIAssets"
if (!(Test-Path $rawDir)) { New-Item -ItemType Directory -Force -Path $rawDir | Out-Null }

$w = 256
$h = 256
$imgPath = Join-Path -Path $rawDir -ChildPath "T_SelectionDecal.png"

$bmp = New-Object System.Drawing.Bitmap $w, $h
$g = [System.Drawing.Graphics]::FromImage($bmp)
$g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias

# Transparent background
$g.Clear([System.Drawing.Color]::Transparent)

# Draw a glowing green circle (similar to classic RTS games like C&C or AoE)
$rect = New-Object System.Drawing.Rectangle 8, 8, ($w-16), ($h-16)
$greenColor = [System.Drawing.Color]::FromArgb(200, 50, 255, 50) # Bright Green
$pen = New-Object System.Drawing.Pen $greenColor, 8
$g.DrawEllipse($pen, $rect)

# Inner faint glow
$faintGreenColor = [System.Drawing.Color]::FromArgb(60, 50, 255, 50)
$brush = New-Object System.Drawing.SolidBrush $faintGreenColor
$g.FillEllipse($brush, $rect)

$bmp.Save($imgPath, [System.Drawing.Imaging.ImageFormat]::Png)

$pen.Dispose()
$brush.Dispose()
$g.Dispose()
$bmp.Dispose()

Write-Host "Created Selection Decal Texture: $imgPath"