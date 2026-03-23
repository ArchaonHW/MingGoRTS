from flask import Flask, jsonify, request
from flask_cors import CORS
from datetime import datetime

app = Flask(__name__)
CORS(app)

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
<html>
<head>
    <title>MingGoRTS Office</title>
    <style>
        body { font-family: monospace; background: #1a1a1a; color: #0f0; text-align: center; padding: 50px; }
        .office { border: 3px solid #0f0; padding: 30px; margin: 20px; background: #0a0a0a; display: inline-block; }
        .status { font-size: 28px; margin: 20px; padding: 15px; border: 2px dashed #0f0; }
        .desc { font-size: 16px; color: #ff0; margin: 10px; }
        .avatar { font-size: 64px; margin: 20px; }
    </style>
</head>
<body>
    <div class="office">
        <h1>MingGoRTS 办公界面</h1>
        <div class="avatar">👨‍💻</div>
        <div class="status" id="s">Loading...</div>
        <div class="desc" id="d">Connecting...</div>
        <p style="color:#888;font-size:12px;">Star-Office-UI Style Pixel Office</p>
    </div>
    <script>
        const emojis = {'idle':'😴 待机','writing':'📝 记录','researching':'🔍 研究',
            'executing':'⚔️ 执行','syncing':'🔄 同步','error':'❌ 错误'};
        function upd() {
            fetch('/status').then(r=>r.json()).then(d=>{
                document.getElementById('s').textContent = emojis[d.state] || d.state;
                document.getElementById('d').textContent = d.description || 'Ready';
            }).catch(e=>{ document.getElementById('s').textContent = '❌ Error'; });
        }
        upd();
        setInterval(upd, 3000);
    </script>
</body>
</html>"""

if __name__ == '__main__':
    print("MingGoRTS Office UI: http://127.0.0.1:19000")
    app.run(host='127.0.0.1', port=19000, debug=False)
