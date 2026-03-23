# MingGoRTS 多工作室協作架構與中央資源庫方案

**設計日期**: 2026-03-24  
**版本**: v1.0  
**適用範圍**: 多工作室協作開發環境

---

## 🎯 執行摘要

### 設計目標
- 支持 **2-10 個本地工作室** 協作開發
- 建立 **中央資源庫** 統一管理共享資源
- 實現 **資源版本同步** 和 **衝突解決機制**
- 確保 **代碼一致性** 和 **開發效率**

### 核心組件
- **中央資源庫 (Central Repository)**: 存儲共享資源
- **本地工作室節點 (Local Studio Nodes)**: 各自獨立開發
- **資源同步閘道 (Sync Gateway)**: 管理資源流動
- **版本協調器 (Version Coordinator)**: 解決版本衝突

---

## 🏗️ 一、系統架構設計

### 1.1 整體架構圖

```
┌─────────────────────────────────────────────────────────────────┐
│                        中央資源庫 (Central Hub)                   │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │   代碼倉庫    │  │   資源庫      │  │   文檔庫      │          │
│  │  (Git Server)│  │   (NAS/雲端) │  │  (Wiki/Docs) │          │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘          │
│         │                 │                 │                  │
│  ┌──────▼─────────────────▼─────────────────▼──────┐          │
│  │              資源同步閘道 (Sync Gateway)          │          │
│  │     - 權限管理  - 版本控制  - 衝突解決           │          │
│  └──────┬─────────────────────────────────┬─────────┘          │
└─────────┼─────────────────────────────────┼────────────────────┘
          │         網絡連接 (LAN/VPN)      │
          ▼                                 ▼
┌─────────────────┐              ┌─────────────────┐
│   工作室 A      │              │   工作室 B       │
│ (開發工作站群)  │◄────────────►│ (開發工作站群)  │
│ ┌────┐ ┌────┐ │   點對點同步   │ ┌────┐ ┌────┐ │
│ │WS1 │ │WS2 │ │                │ │WS3 │ │WS4 │ │
│ └────┘ └────┘ │                │ └────┘ └────┘ │
└─────────────────┘              └─────────────────┘
          │                                 │
          ▼                                 ▼
┌─────────────────┐              ┌─────────────────┐
│   工作室 C      │              │   工作室 D       │
│ (開發工作站群)  │              │ (開發工作站群)  │
└─────────────────┘              └─────────────────┘
```

### 1.2 組件詳細設計

#### 中央資源庫 (Central Repository)

| 組件 | 類型 | 容量 | 用途 |
|------|------|------|------|
| **Git 中央倉庫** | Git Server | 500GB | 代碼版本管理 |
| **資源存儲** | NAS / 雲端 | 2TB+ | 大文件資源 |
| **文檔系統** | Wiki / Docs | 100GB | 協作文檔 |
| **構建伺服器** | CI/CD | - | 自動化構建 |

#### 本地工作室節點 (Local Studio Node)

每個工作室包含：
- **開發工作站**: 3-10 台 (按團隊規模)
- **本地 NAS**: 10TB+ (資源緩存)
- **同步客戶端**: 資源同步工具
- **開發工具鏈**: UE5、VS2022、PowerShell 7.6+

---

## 🔄 二、資源同步機制

### 2.1 資源分類與同步策略

#### A 類：代碼資源 (Git 管理)
| 資源類型 | 同步頻率 | 存儲位置 | 管理方式 |
|---------|----------|----------|----------|
| C++ 源代碼 | 即時 | Git 中央倉庫 | 標準 Git 工作流 |
| 配置文件 | 即時 | Git 中央倉庫 | 版本控制 |
| 腳本文件 | 即時 | Git 中央倉庫 | 版本控制 |

#### B 類：大文件資源 (Git LFS + NAS)
| 資源類型 | 同步頻率 | 存儲位置 | 管理方式 |
|---------|----------|----------|----------|
| 3D 模型 (*.fbx, *.obj) | 每日 | NAS + LFS | 版本 + 鎖定 |
| 材質貼圖 (*.png, *.tga) | 每日 | NAS + LFS | 版本 + 鎖定 |
| 音效文件 (*.wav, *.mp3) | 每日 | NAS + LFS | 版本 + 鎖定 |
| 動畫資源 | 每日 | NAS + LFS | 版本 + 鎖定 |

#### C 類：構建產物 (構建伺服器)
| 資源類型 | 同步頻率 | 存儲位置 | 管理方式 |
|---------|----------|----------|----------|
| 編譯輸出 | 觸發 | 構建伺服器 | 自動化構建 |
| 打包版本 | 手動 | 構建伺服器 | 版本標記 |
| 測試報告 | 觸發 | 構建伺服器 | 自動生成 |

### 2.2 同步流程設計

```
工作室 A (本地修改)
      │
      ▼
┌─────────────────┐
│ 1. 本地提交      │
│ 2. 代碼推送到中央 │ (Git Push)
│ 3. 資源標記待同步 │ (標記 B 類資源)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ 資源同步閘道     │
│ - 檢查衝突       │
│ - 驗證權限       │
│ - 觸發同步       │
└────────┬────────┘
         │
    ┌────┴────┐
    ▼         ▼
中央倉庫   中央 NAS
(Git)     (大文件)
    │         │
    └────┬────┘
         │
    通知其他工作室
         │
┌────────▼────────┐
│ 工作室 B/C/D... │
│ 自動拉取更新     │ (Git Pull)
│ 同步資源下載     │ (NAS Sync)
└─────────────────┘
```

### 2.3 衝突解決機制

#### 代碼衝突 (Git 標準流程)
```
發現衝突
    │
    ▼
自動通知相關開發者
    │
    ▼
本地解決衝突
    │
    ▼
重新提交並標記
    │
    ▼
中央倉庫合併
```

#### 資源文件衝突 (文件鎖定機制)
```
開發者 A 請求編輯資源 X
    │
    ▼
檢查資源 X 鎖定狀態
    │
    ▼
┌─────────────────────────┐
│ 未鎖定?                 │
│   是 → 鎖定並授權編輯    │
│   否 → 返回鎖定者信息    │
└─────────────────────────┘
    │
    ▼
編輯完成後解鎖並提交
```

---

## 🛡️ 三、權限管理與安全

### 3.1 角色權限矩陣

| 角色 | 代碼倉庫 | 資源庫 | 文檔庫 | 構建系統 |
|------|---------|--------|--------|----------|
| **專案負責人** | 讀寫 + 管理 | 讀寫 + 管理 | 讀寫 + 管理 | 管理 |
| **主程序** | 讀寫 + 審查 | 讀寫 | 讀寫 | 觸發 |
| **開發者** | 讀寫 (功能分支) | 讀寫 | 讀寫 | 讀取 |
| **美術** | 讀取 | 讀寫 (資源) | 讀取 | 讀取 |
| **測試** | 讀取 | 讀取 | 讀寫 | 讀取 |
| **訪客** | 讀取 (部分) | 無 | 讀取 | 無 |

### 3.2 工作室隔離策略

```
┌─────────────────────────────────────┐
│           中央資源庫                 │
│  ┌─────────────────────────────┐   │
│  │     公共資源區 (所有工作室)  │   │
│  │  - 核心框架代碼             │   │
│  │  - 通用工具庫               │   │
│  │  - 共享資源                 │   │
│  └─────────────────────────────┘   │
│            │                       │
│  ┌─────────┼─────────┐             │
│  ▼         ▼         ▼             │
│ ┌───┐   ┌───┐   ┌───┐             │
│ │A區│   │B區│   │C區│  工作室隔離區 │
│ └───┘   └───┘   └───┘             │
└─────────────────────────────────────┘
```

---

## 📦 四、中央資源庫實施方案

### 4.1 硬體配置建議

#### 小型配置 (2-3 工作室)
| 組件 | 規格 | 預估成本 |
|------|------|----------|
| **NAS 伺服器** | Synology DS920+ (4盤位) | $600 |
| **硬碟** | 4x 4TB NAS 硬碟 | $600 |
| **網絡設備** | 千兆交換機 | $100 |
| **UPS** | 1000VA 不斷電 | $150 |
| **總計** | - | ~$1,450 |

#### 中型配置 (4-6 工作室)
| 組件 | 規格 | 預估成本 |
|------|------|----------|
| **NAS 伺服器** | Synology DS1821+ (8盤位) | $1,000 |
| **硬碟** | 8x 8TB NAS 硬碟 | $2,000 |
| **SSD 緩存** | 2x 1TB NVMe SSD | $400 |
| **網絡設備** | 萬兆交換機 | $500 |
| **UPS** | 2000VA 不斷電 | $300 |
| **總計** | - | ~$4,200 |

#### 大型配置 (7-10 工作室)
| 組件 | 規格 | 預估成本 |
|------|------|----------|
| **機架式 NAS** | Synology RS1221RP+ | $2,000 |
| **硬碟** | 12x 12TB 企業級 | $6,000 |
| **SSD 緩存** | 4x 2TB NVMe SSD | $1,600 |
| **網絡設備** | 萬兆核心交換機 | $1,500 |
| **備份設備** | LTO-9 磁帶機 | $5,000 |
| **UPS** | 3000VA 機架式 | $800 |
| **總計** | - | ~$16,900 |

### 4.2 軟體配置

#### 資源同步軟體選型
| 方案 | 優點 | 缺點 | 適用場景 |
|------|------|------|----------|
| **Synology Drive** | 易用、整合好 | 閉源 | 小型團隊 |
| **Resilio Sync** | P2P 快速 | 付費 | 中大型團隊 |
| **Nextcloud** | 開源、功能多 | 配置複雜 | 需要自定義 |
| **自研工具** | 完全符合需求 | 開發成本 | 特殊需求 |

#### 推薦方案：混合架構
```
中央 NAS (Synology)
    ├── Git Server (Gitea/GitLab)
    ├── 資源同步 (Resilio Sync)
    ├── 文檔系統 (BookStack)
    └── CI/CD (Jenkins/GitLab CI)
```

---

## 🚀 五、實施路線圖

### Phase 1: 基礎設施 (1-2 週)
- [ ] 採購並部署 NAS 設備
- [ ] 配置網絡基礎設施
- [ ] 安裝 Git Server
- [ ] 配置 Git LFS

### Phase 2: 工具部署 (2-3 週)
- [ ] 部署資源同步工具
- [ ] 配置權限管理系統
- [ ] 設置 CI/CD 流程
- [ ] 測試同步機制

### Phase 3: 試點運行 (2-4 週)
- [ ] 選擇 2 個工作室試點
- [ ] 完整測試協作流程
- [ ] 收集反饋並優化
- [ ] 培訓團隊使用

### Phase 4: 全面推廣 (1-2 週)
- [ ] 擴展到所有工作室
- [ ] 監控系統運行狀態
- [ ] 建立支持流程
- [ ] 文檔化操作手冊

---

## 🛠️ 六、工具與腳本

### 6.1 資源同步腳本

```powershell
# Sync-StudioResources.ps1
# 工作室資源同步腳本

param(
    [string]$StudioID,
    [string]$CentralRepo = "\\central-nas\MingGoRTS",
    [string]$LocalCache = "D:\\MingGoRTS\\Cache",
    [ValidateSet("Pull", "Push", "Sync")]
    [string]$Mode = "Sync"
)

# 資源類型配置
$ResourceTypes = @{
    "Code" = @{
        Path = "Source"
        SyncMethod = "Git"
        Remote = "https://git.central.local/minggorts"
    }
    "Assets" = @{
        Path = "Content"
        SyncMethod = "Robocopy"
        Exclude = @("*.tmp", "*.cache")
    }
    "Builds" = @{
        Path = "Build"
        SyncMethod = "Manual"
        ReadOnly = $true
    }
}

function Sync-Code {
    git fetch origin
    git pull origin development
}

function Sync-Assets {
    robocopy $CentralRepo\Content $LocalCache\Content /MIR /R:3 /W:5 /XD Temp /XF *.tmp
}

# 主執行流程
switch ($Mode) {
    "Pull" { 
        Sync-Code
        Sync-Assets
    }
    "Push" {
        # 推送本地修改到中央
    }
    "Sync" {
        Sync-Code
        Sync-Assets
    }
}
```

### 6.2 監控與報告

```powershell
# New-CollaborationReport.ps1
# 生成多工作室協作報告

function Get-StudioStatus {
    param([string]$StudioID)
    
    return @{
        StudioID = $StudioID
        LastSync = Get-Date
        PendingChanges = Get-PendingChangesCount
        ResourceUsage = Get-DiskUsage
        ActiveDevelopers = Get-ActiveDevelopers
        BuildStatus = Get-LastBuildStatus
    }
}

function New-CollaborationDashboard {
    $Studios = @("Studio-A", "Studio-B", "Studio-C")
    $Report = foreach ($Studio in $Studios) {
        Get-StudioStatus -StudioID $Studio
    }
    
    # 生成 HTML 報告
    $Report | ConvertTo-Html -Title "MingGoRTS 協作狀態" | 
        Set-Content "Reports\CollaborationDashboard.html"
}
```

---

## 📊 七、效能指標與監控

### 7.1 關鍵效能指標 (KPI)

| 指標 | 目標值 | 測量方式 |
|------|--------|----------|
| **資源同步延遲** | < 5 分鐘 | 提交到全工作室可用 |
| **代碼合併衝突率** | < 5% | 月度統計 |
| **構建成功率** | > 95% | CI/CD 統計 |
| **資源命中率** | > 80% | 本地緩存命中 |
| **工作室間協作頻率** | > 10 次/日 | 跨工作室提交 |

### 7.2 監控項目

```yaml
# monitoring-config.yaml
monitoring:
  infrastructure:
    - nas_disk_usage
    - network_bandwidth
    - sync_queue_length
    - error_rate
  
  collaboration:
    - active_studios
    - concurrent_developers
    - resource_lock_wait_time
    - merge_conflict_frequency
  
  alerts:
    disk_usage: 
      warning: 80%
      critical: 90%
    sync_latency:
      warning: 5min
      critical: 15min
```

---

## 🎯 八、風險評估與應對

### 8.1 風險矩陣

| 風險 | 可能性 | 影響 | 應對措施 |
|------|--------|------|----------|
| 中央 NAS 故障 | 低 | 高 | RAID + 異地備份 |
| 網絡中斷 | 中 | 中 | 本地緩存 + VPN |
| 版本衝突 | 高 | 低 | 文件鎖定 + 自動合併 |
| 權限洩漏 | 低 | 高 | 審計日誌 + 定期審查 |
| 資源誤刪 | 中 | 中 | 版本歷史 + 回收站 |

### 8.2 災難恢復計劃

```
災難場景: 中央 NAS 完全故障

RTO (恢復時間目標): 4 小時
RPO (恢復點目標): 1 小時

恢復步驟:
1. 啟動備用 NAS (15 分鐘)
2. 從異地備份恢復數據 (2 小時)
3. 重新配置同步服務 (30 分鐘)
4. 驗證所有工作室連接 (1 小時)
```

---

## 📋 九、操作手冊摘要

### 快速開始

#### 新工作室加入流程
```bash
# 1. 工作站配置
& "C:\Program Files\PowerShell\7\pwsh.exe" -File "Tools\workflow\Setup-GitWorkflow.ps1" -Init -ConfigureLFS -SetupHooks -SetupBranches

# 2. 連接中央資源庫
.\Tools\workflow\Connect-CentralRepository.ps1 -StudioID "Studio-B" -CentralURL "https://central.minggorts.local"

# 3. 首次同步
.\Tools\workflow\Sync-StudioResources.ps1 -Mode Pull

# 4. 驗證連接
.\Tools\workflow\Test-CollaborationSetup.ps1
```

#### 日常操作流程
```bash
# 開始工作前 - 獲取最新更新
.\Tools\workflow\Sync-StudioResources.ps1 -Mode Pull

# 工作完成後 - 提交修改
# 1. 提交代碼 (標準 Git 流程)
git add .
git commit -m "[🎉] 實現新功能"
git push origin feature/xxx

# 2. 標記資源變更 (如果有)
.\Tools\workflow\Sync-StudioResources.ps1 -Mode Push -ResourceTypes "Assets"

# 結束工作前 - 同步狀態
.\Tools\workflow\Sync-StudioResources.ps1 -Mode Sync
```

---

## 📝 十、總結與建議

### 方案優勢
1. **可擴展性**: 支持 2-10 個工作室，可按需擴展
2. **容錯性**: 多層備份 + 本地緩存，單點故障不影響全局
3. **效能**: 智能同步策略，減少網絡傳輸
4. **安全**: 細粒度權限控制 + 審計追蹤

### 實施建議
1. **分階段實施**: 從小型配置開始，逐步擴展
2. **試點先行**: 選擇 2 個工作室試點，驗證後全面推廣
3. **培訓配套**: 制定詳細的操作手冊和培訓計劃
4. **監控到位**: 建立完整的監控和報警機制

### 預估投資回報
- **初期投資**: $1,450 - $16,900 (根據規模)
- **維護成本**: 年度 20% 初期投資
- **效率提升**: 30-50% 協作效率提升
- **回報周期**: 6-12 個月

---

**設計文件**: `docs/ARCHITECTURE/MultiStudio_Collaboration_Architecture.md`  
**實施工具**: `Tools\workflow\Connect-CentralRepository.ps1`  
**監控面板**: `http://central.minggorts.local/dashboard`  

*設計: MingGoRTS 架構團隊*  
*版本: v1.0*  
*日期: 2026-03-24*
