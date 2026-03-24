# MingGoRTS Office UI Launcher - Simplified
param([switch]$Start, [switch]$Stop, [switch]$Status)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "   MingGoRTS Office UI 模拟启动器" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$Port = 19000
$Url = "http://127.0.0.1:$Port"

if ($Start) {
    Write-Host "`n[INIT] 初始化办公界面服务..." -ForegroundColor Yellow
    
    # 创建目录结构
    $Paths = @(
        "Tools/office-ui/backend",
        "Tools/office-ui/config",
        "Tools/office-ui/frontend"
    )
    foreach ($Path in $Paths) {
        if (!(Test-Path $Path)) {
            New-Item -ItemType Directory -Path $Path -Force | Out-Null
            Write-Host "  [OK] 创建: $Path" -ForegroundColor Gray
        }
    }
    
    # 创建requirements.txt
    "Flask==2.3.3" | Out-File "Tools/office-ui/backend/requirements.txt" -Encoding UTF8
    Write-Host "  [OK] 创建依赖配置" -ForegroundColor Gray
    
    # 创建简化版Flask后端
    $AppPy = @'
from flask import Flask, jsonify, request
from flask_cors import CORS
import json
from datetime import datetime
import os

app = Flask(__name__)
CORS(app)

# 状态存储
STATE = {
    'state': 'idle',
    'description': 'MingGoRTS Office UI Ready',
    'updated_at': datetime.now().isoformat()
}

@app.route('/health')
def health():
    return jsonify({'status': 'ok', 'service': 'MingGoRTS-OfficeUI'})

@app.route('/status')
def status():
    STATE['updated_at'] = datetime.now().isoformat()
    return jsonify(STATE)

@app.route('/set_state', methods=['POST'])
def set_state():
    global STATE
    data = request.get_json()
    STATE['state'] = data.get('state', 'idle')
    STATE['description'] = data.get('description', '')
    STATE['updated_at'] = datetime.now().isoformat()
    return jsonify({'success': True, 'state': STATE})

@app.route('/')
def index():
    return """<!DOCTYPE html>
<html><head><title>MingGoRTS Office</title>
<style>
body{font-family:monospace;background:#1a1a1a;color:#0f0;text-align:center;padding:50px;}
.office{border:3px solid #0f0;padding:30px;margin:20px;background:#0a0a0a;display:inline-block;}
.status{font-size:28px;margin:20px;padding:15px;border:2px dashed #0f0;}
.desc{font-size:16px;color:#ff0;margin:10px;}
.avatar{font-size:64px;margin:20px;}
</style></head>
<body>
<div class="office">
<h1>MingGoRTS 办公界面</h1>
<div class="avatar">👨‍💻</div>
<div class="status" id="s">🟡 Loading...</div>
<div class="desc" id="d">Connecting...</div>
<p style="color:#888;font-size:12px;">Star-Office-UI Style Pixel Office</p>
</div>
<script>
const emojis={'idle':'😴 待机','writing':'📝 记录','researching':'🔍 研究',
'executing':'⚔️ 执行','syncing':'🔄 同步','error':'❌ 错误'};
function upd(){fetch('/status').then(r=>r.json()).then(d=>{
document.getElementById('s').textContent=emojis[d.state]||d.state;
document.getElementById('d').textContent=d.description||'Ready';
}).catch(e=>{document.getElementById('s').textContent='❌ Error';});}
upd();setInterval(upd,3000);
</script></body></html>"""

if __name__ == '__main__':
    print("MingGoRTS Office UI: http://127.0.0.1:19000")
    app.run(host='127.0.0.1', port=19000, debug=False)
'@
    
    $AppPy | Out-File "Tools/office-ui/backend/app.py" -Encoding UTF8
    Write-Host "  [OK] 创建后端服务" -ForegroundColor Gray
    
    # 创建状态文件
    $StateJson = @{
        state = "idle"
        description = "MingGoRTS Office UI 已启动"
        updated_at = (Get-Date -Format "yyyy-MM-ddTHH:mm:ss")
    } | ConvertTo-Json -Compress
    
    $StateJson | Out-File "Tools/office-ui/config/state.json" -Encoding UTF8
    Write-Host "  [OK] 创建状态文件" -ForegroundColor Gray
    
    # 尝试启动服务
    Write-Host "`n[START] 启动办公界面服务..." -ForegroundColor Yellow
    try {
        $PythonCmd = Get-Command python3 -ErrorAction SilentlyContinue
        if (!$PythonCmd) { $PythonCmd = Get-Command python -ErrorAction SilentlyContinue }
        
        if ($PythonCmd) {
            Start-Process $PythonCmd.Source -ArgumentList "Tools/office-ui/backend/app.py" -WindowStyle Hidden
            Start-Sleep -Seconds 3
            Write-Host "  [OK] 服务进程已启动" -ForegroundColor Green
        } else {
            Write-Host "  [WARN] 未找到Python，跳过服务启动" -ForegroundColor Yellow
        }
    } catch {
        Write-Host "  [WARN] 服务启动失败: $_" -ForegroundColor Yellow
    }
    
    Write-Host "`n========================================" -ForegroundColor Cyan
    Write-Host "  MingGoRTS Office UI 模拟完成!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "`n✅ 已创建文件结构:" -ForegroundColor White
    Write-Host "   Tools/office-ui/backend/app.py" -ForegroundColor Gray
    Write-Host "   Tools/office-ui/config/state.json" -ForegroundColor Gray
    Write-Host "`n📍 服务地址: http://127.0.0.1:19000" -ForegroundColor Yellow
    Write-Host "`n🔧 手动启动命令:" -ForegroundColor White
    Write-Host "   cd Tools/office-ui/backend" -ForegroundColor Gray
    Write-Host "   python3 app.py" -ForegroundColor Gray
    
} elseif ($Stop) {
    Write-Host "`n[STOP] 停止办公界面服务..." -ForegroundColor Yellow
    Get-Process python* | Where-Object {$_.CommandLine -match "app.py"} | Stop-Process -Force
    Write-Host "  [OK] 服务已停止" -ForegroundColor Green
    
} elseif ($Status) {
    Write-Host "`n[STATUS] 检查办公界面状态..." -ForegroundColor Yellow
    try {
        $Resp = Invoke-WebRequest "$Url/health" -TimeoutSec 3
        if ($Resp.StatusCode -eq 200) {
            Write-Host "  [OK] 服务运行正常" -ForegroundColor Green
            $StatusResp = Invoke-WebRequest "$Url/status"
            $State = $StatusResp.Content | ConvertFrom-Json
            Write-Host "  State: $($State.state)" -ForegroundColor Cyan
            Write-Host "  Desc: $($State.description)" -ForegroundColor Cyan
        }
    } catch {
        Write-Host "  [ERROR] 服务未运行或无法访问" -ForegroundColor Red
    }
    
} else {
    Write-Host "`n用法:" -ForegroundColor Yellow
    Write-Host "  Start-MingOfficeUI.ps1 -Start   # 启动服务" -ForegroundColor White
    Write-Host "  Start-MingOfficeUI.ps1 -Stop    # 停止服务" -ForegroundColor White
    Write-Host "  Start-MingOfficeUI.ps1 -Status  # 检查状态" -ForegroundColor White
}

Write-Host "`n========================================`n" -ForegroundColor Cyan
