import requests, json, sys, base64

api_key = "AIzaSyBLEy7ZqeH3LAtvleEIrwNlD8WupMzhjzI"
prompt = "A highly detailed, top-down vintage military strategic map from the 1930s Republic of China era, featuring old Chinese calligraphy, troop movement arrows in faded red and blue, scattered sepia-toned intelligence photographs, placed on a dark wooden table with dim cinematic lighting, realistic, 8k resolution, suitable for an RTS game main menu background --ar 16:9"
url = f"https://generativelanguage.googleapis.com/v1beta/models/gemini-3.1-flash-image-preview:generateContent?key={api_key}"

headers = {"Content-Type": "application/json"}
payload = {
    "contents": [{"role": "user", "parts": [{"text": prompt}]}]
}

response = requests.post(url, headers=headers, json=payload)
if response.status_code == 200:
    data = response.json()
    try:
        b64_img = data["candidates"][0]["content"]["parts"][0]["inlineData"]["data"]
        with open(r"C:\HW\MingGoRTS\RawUIAssets\BG_MingWar_AI.png", "wb") as f:
            f.write(base64.b64decode(b64_img))
        print("Success")
    except KeyError as e:
        print("KeyError", e, data)
else:
    print(response.status_code, response.text)