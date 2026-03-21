Add-Type -AssemblyName System.Drawing

$rawDir = "C:\HW\MingGoRTS\RawUIAssets"
$imgPath = Join-Path -Path $rawDir -ChildPath "BG_MingWar.png"

$w = 1920
$h = 1080
$bmp = New-Object System.Drawing.Bitmap $w, $h
$g = [System.Drawing.Graphics]::FromImage($bmp)
$g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias

# Faded Nationalist Blue (青天白日暗藍)
$colorBlue = [System.Drawing.ColorTranslator]::FromHtml("#1A2B3C") 

# Faded Communist Red (戰火暗紅)
$colorRed = [System.Drawing.ColorTranslator]::FromHtml("#3C1515")

# Draw a diagonal split to symbolize the conflict
$brushBlue = New-Object System.Drawing.SolidBrush($colorBlue)
$brushRed = New-Object System.Drawing.SolidBrush($colorRed)

$g.FillRectangle($brushRed, 0, 0, $w, $h)

$pts = @(
    (New-Object System.Drawing.Point(0, 0)),
    (New-Object System.Drawing.Point($w, 0)),
    (New-Object System.Drawing.Point(0, $h))
)
$g.FillPolygon($brushBlue, $pts)

# Add some "old map / sepia" overlay lines to make it look tactical/historical
$pen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(30, 255, 235, 200)), 2
for ($i = 0; $i -lt $w; $i += 50) {
    $g.DrawLine($pen, $i, 0, $i, $h)
}
for ($j = 0; $j -lt $h; $j += 50) {
    $g.DrawLine($pen, 0, $j, $w, $j)
}

$bmp.Save($imgPath, [System.Drawing.Imaging.ImageFormat]::Png)

$pen.Dispose()
$brushBlue.Dispose()
$brushRed.Dispose()
$g.Dispose()
$bmp.Dispose()

Write-Host "Created historical themed background: $imgPath"