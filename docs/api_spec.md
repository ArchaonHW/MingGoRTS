# API 設計規範（多人模式）

## 概述
本遊戲採用 Unreal Engine replication 系統，無需傳統 REST API。如需外部 AI 服務集成，定義如下：

## 外部 AI API

### Stable Diffusion 生成介面
```
POST http://localhost:7860/sdapi/v1/txt2img
Content-Type: application/json

Request:
{
  "prompt": "string",
  "negative_prompt": "string",
  "steps": 30,
  "width": 512,
  "height": 768,
  "sampler_name": "DPM++ 2M Karras"
}

Response:
{
  "images": ["base64_string"],
  "parameters": {},
  "info": "json_string"
}
```

### AIVA 音樂生成介面
```
POST https://api.aiva.ai/v1/compositions
Authorization: Bearer <API_KEY>
Content-Type: application/json

Request:
{
  "title": "MingWar Main Theme",
  "style": "chinese-orchestral-epic",
  "duration": 180,
  "mood": "dramatic"
}

Response:
{
  "id": "string",
  "status": "processing/completed",
  "download_url": "string"
}
```

## 內部遊戲事件
使用 Unreal 委派（Delegates）和 Gameplay Tags 傳遞事件的訊息。