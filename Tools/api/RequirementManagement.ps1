# MingGoRTS 需求單管理系統
# Requirement Management System for MingGoRTS

param(
    [string]$Action = "list",
    [string]$RequirementID = "",
    [string]$Title = "",
    [string]$Description = "",
    [string]$Category = "",
    [string]$Priority = "",
    [string]$Complexity = "",
    [string]$Assignee = "",
    [string]$Status = "",
    [switch]$Interactive = $false,
    [switch]$Verbose = $false,
    [string]$OutputPath = "C:\HW\MingGoRTS\Requirements"
)

# 配置
$ProjectRoot = "C:\HW\MingGoRTS"
$RequirementsPath = "$OutputPath\Requirements.json"
$TemplatesPath = "$OutputPath\Templates"
$ReportsPath = "$OutputPath\Reports"

# 需求單系統配置
$RequirementConfig = @{
    # 優先級定義
    Priorities = @{
        "Critical" = @{
            Level = 1
            Color = "Red"
            Description = "關鍵需求，必須立即處理"
            SLA = "24小時內回應"
        }
        "High" = @{
            Level = 2
            Color = "Orange"
            Description = "高優先級，應優先處理"
            SLA = "3天內處理"
        }
        "Medium" = @{
            Level = 3
            Color = "Yellow"
            Description = "中等優先級，正常排程處理"
            SLA = "1週內處理"
        }
        "Low" = @{
            Level = 4
            Color = "Green"
            Description = "低優先級，可延後處理"
            SLA = "2週內處理"
        }
        "Backlog" = @{
            Level = 5
            Color = "Gray"
            Description = "待辦清單，有時間處理"
            SLA = "無固定時間"
        }
    }
    
    # 複雜度定義
    Complexities = @{
        "Simple" = @{
            Level = 1
            EstimatedHours = 4
            Description = "簡單任務，1-4小時"
            Risk = "Low"
        }
        "Medium" = @{
            Level = 2
            EstimatedHours = 16
            Description = "中等複雜度，1-3天"
            Risk = "Medium"
        }
        "Complex" = @{
            Level = 3
            EstimatedHours = 40
            Description = "複雜任務，1週"
            Risk = "High"
        }
        "VeryComplex" = @{
            Level = 4
            EstimatedHours = 80
            Description = "非常複雜，2週以上"
            Risk = "VeryHigh"
        }
    }
    
    # 狀態定義
    Statuses = @{
        "New" = @{
            Level = 1
            Description = "新建需求"
        }
        "Analysis" = @{
            Level = 2
            Description = "需求分析中"
        }
        "Approved" = @{
            Level = 3
            Description = "需求已批准"
        }
        "InProgress" = @{
            Level = 4
            Description = "開發中"
        }
        "Testing" = @{
            Level = 5
            Description = "測試中"
        }
        "Review" = @{
            Level = 6
            Description = "代碼審查中"
        }
        "Completed" = @{
            Level = 7
            Description = "已完成"
        }
        "Rejected" = @{
            Level = 8
            Description = "已拒絕"
        }
        "OnHold" = @{
            Level = 9
            Description = "暫停"
        }
    }
    
    # 分類定義
    Categories = @{
        "Feature" = "新功能開發"
        "BugFix" = "錯誤修復"
        "Enhancement" = "功能增強"
        "Optimization" = "性能優化"
        "Documentation" = "文檔更新"
        "Testing" = "測試相關"
        "UI" = "用戶界面"
        "API" = "API開發"
        "Infrastructure" = "基礎設施"
        "Security" = "安全相關"
        "Research" = "研究調研"
        "Integration" = "系統集成"
        "Refactoring" = "代碼重構"
    }
}

# 初始化目錄
function Initialize-RequirementSystem {
    Write-Host "初始化需求單系統..." -ForegroundColor Green
    
    $Directories = @(
        $OutputPath,
        $TemplatesPath,
        $ReportsPath,
        "$OutputPath\Attachments",
        "$OutputPath\Comments",
        "$OutputPath\History"
    )
    
    foreach ($Dir in $Directories) {
        if (-not (Test-Path $Dir)) {
            New-Item -ItemType Directory -Path $Dir -Force | Out-Null
            Write-Host "創建目錄: $Dir" -ForegroundColor Cyan
        }
    }
    
    # 初始化需求檔案
    if (-not (Test-Path $RequirementsPath)) {
        $InitialData = @{
            Requirements = @()
            Metadata = @{
                CreatedAt = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
                LastUpdated = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
                Version = "1.0.0"
                TotalRequirements = 0
            }
        }
        $InitialData | ConvertTo-Json -Depth 10 | Set-Content $RequirementsPath -Encoding UTF8
        Write-Host "創建需求檔案: $RequirementsPath" -ForegroundColor Green
    }
    
    # 創建模板
    New-RequirementTemplates
}

# 創建需求模板
function New-RequirementTemplates {
    Write-Host "創建需求模板..." -ForegroundColor Cyan
    
    # 功能需求模板
    $FeatureTemplate = @{
        Type = "Feature"
        Title = "[功能需求] 新功能名稱"
        Description = @"
## 需求描述
詳細描述新功能的需求和目標

## 功能需求
1. 功能點1
2. 功能點2
3. 功能點3

## 非功能需求
- 性能要求
- 安全要求
- 兼容性要求

## 驗收標準
- [ ] 功能1正常工作
- [ ] 功能2正常工作
- [ ] 通過相關測試

## 技術要求
- 使用技術棧
- 依賴項目
- 限制條件
"@
        AcceptanceCriteria = @()
        TechnicalRequirements = @()
        Dependencies = @()
    }
    
    # Bug修復模板
    $BugFixTemplate = @{
        Type = "BugFix"
        Title = "[Bug修復] 問題簡述"
        Description = @"
## 問題描述
詳細描述遇到的問題

## 重現步驟
1. 步驟1
2. 步驟2
3. 步驟3

## 預期行為
描述正確的行為應該是什麼

## 實際行為
描述當前的錯誤行為

## 環境信息
- 操作系統
- 瀏覽器版本
- 相關版本號

## 錯誤日誌
```
錯誤日誌內容
```
"@
        StepsToReproduce = @()
        ExpectedBehavior = ""
        ActualBehavior = ""
        Environment = @{}
    }
    
    # 保存模板
    $FeatureTemplate | ConvertTo-Json -Depth 10 | Set-Content "$TemplatesPath\Feature.json" -Encoding UTF8
    $BugFixTemplate | ConvertTo-Json -Depth 10 | Set-Content "$TemplatesPath\BugFix.json" -Encoding UTF8
    
    Write-Host "模板創建完成" -ForegroundColor Green
}

# 載入需求數據
function Load-Requirements {
    try {
        if (Test-Path $RequirementsPath) {
            return Get-Content $RequirementsPath -Raw | ConvertFrom-Json
        } else {
            return @{
                Requirements = @()
                Metadata = @{
                    CreatedAt = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
                    LastUpdated = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
                    Version = "1.0.0"
                    TotalRequirements = 0
                }
            }
        }
    } catch {
        Write-Host "載入需求數據失敗: $($_.Exception.Message)" -ForegroundColor Red
        return $null
    }
}

# 保存需求數據
function Save-Requirements {
    param([object]$Data)
    
    try {
        $Data.Metadata.LastUpdated = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        $Data.Metadata.TotalRequirements = $Data.Requirements.Count
        $Data | ConvertTo-Json -Depth 10 | Set-Content $RequirementsPath -Encoding UTF8
        return $true
    } catch {
        Write-Host "保存需求數據失敗: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# 生成需求ID
function New-RequirementID {
    $Timestamp = Get-Date -Format "yyyyMMdd"
    $Random = Get-Random -Minimum 1000 -Maximum 9999
    return "REQ-$Timestamp-$Random"
}

# 計算優先級分數
function Get-PriorityScore {
    param(
        [string]$Priority,
        [string]$Complexity,
        [string]$Status
    )
    
    $PriorityWeight = $RequirementConfig.Priorities[$Priority].Level
    $ComplexityWeight = $RequirementConfig.Complexities[$Complexity].Level
    $StatusWeight = if ($Status -eq "New" -or $Status -eq "Analysis") { 1 } else { 0.5 }
    
    return ($PriorityWeight * 10) + ($ComplexityWeight * 5) + ($StatusWeight * 2)
}

# 創建需求單
function New-Requirement {
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "創建需求單" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    $Data = Load-Requirements
    if (-not $Data) {
        return $false
    }
    
    # 交互式輸入
    if ($Interactive -or [string]::IsNullOrEmpty($Title)) {
        $Title = Read-Host "請輸入需求標題"
        $Description = Read-Host "請輸入需求描述"
        $Category = Read-Host "請輸入需求分類 (Feature/BugFix/Enhancement/Optimization/Documentation)"
        $Priority = Read-Host "請輸入優先級 (Critical/High/Medium/Low/Backlog)"
        $Complexity = Read-Host "請輸入複雜度 (Simple/Medium/Complex/VeryComplex)"
        $Assignee = Read-Host "請輸入負責人 (可選)"
    }
    
    # 驗證輸入
    if (-not $RequirementConfig.Categories.ContainsKey($Category)) {
        Write-Host "錯誤: 無效的分類 '$Category'" -ForegroundColor Red
        return $false
    }
    
    if (-not $RequirementConfig.Priorities.ContainsKey($Priority)) {
        Write-Host "錯誤: 無效的優先級 '$Priority'" -ForegroundColor Red
        return $false
    }
    
    if (-not $RequirementConfig.Complexities.ContainsKey($Complexity)) {
        Write-Host "錯誤: 無效的複雜度 '$Complexity'" -ForegroundColor Red
        return $false
    }
    
    # 創建需求對象
    $Requirement = @{
        ID = New-RequirementID
        Title = $Title
        Description = $Description
        Category = $Category
        Priority = $Priority
        Complexity = $Complexity
        Status = "New"
        Assignee = $Assignee
        CreatedAt = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        UpdatedAt = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        CreatedBy = "System"
        PriorityScore = Get-PriorityScore -Priority $Priority -Complexity $Complexity -Status "New"
        EstimatedHours = $RequirementConfig.Complexities[$Complexity].EstimatedHours
        Tags = @()
        Attachments = @()
        Comments = @()
        History = @()
    }
    
    # 添加歷史記錄
    $Requirement.History += @{
        Action = "Created"
        Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        User = "System"
        Description = "需求單創建"
    }
    
    # 添加到數據中
    $Data.Requirements += $Requirement
    
    # 保存數據
    if (Save-Requirements -Data $Data) {
        Write-Host "需求單創建成功!" -ForegroundColor Green
        Write-Host "需求ID: $($Requirement.ID)" -ForegroundColor Cyan
        Write-Host "標題: $($Requirement.Title)" -ForegroundColor White
        Write-Host "優先級: $($Requirement.Priority) (分數: $($Requirement.PriorityScore))" -ForegroundColor Yellow
        Write-Host "複雜度: $($Requirement.Complexity) (預估: $($Requirement.EstimatedHours)小時)" -ForegroundColor Cyan
        return $true
    } else {
        Write-Host "需求單創建失敗!" -ForegroundColor Red
        return $false
    }
}

# 列出需求單
function Get-Requirements {
    param(
        [string]$FilterCategory = "",
        [string]$FilterPriority = "",
        [string]$FilterStatus = "",
        [string]$FilterAssignee = "",
        [switch]$SortByPriority,
        [switch]$SortByComplexity,
        [switch]$SortByDate
    )
    
    $Data = Load-Requirements
    if (-not $Data) {
        return $false
    }
    
    $Requirements = $Data.Requirements
    
    # 應用篩選
    if ($FilterCategory) {
        $Requirements = $Requirements | Where-Object { $_.Category -eq $FilterCategory }
    }
    
    if ($FilterPriority) {
        $Requirements = $Requirements | Where-Object { $_.Priority -eq $FilterPriority }
    }
    
    if ($FilterStatus) {
        $Requirements = $Requirements | Where-Object { $_.Status -eq $FilterStatus }
    }
    
    if ($FilterAssignee) {
        $Requirements = $Requirements | Where-Object { $_.Assignee -like "*$FilterAssignee*" }
    }
    
    # 排序
    if ($SortByPriority) {
        $Requirements = $Requirements | Sort-Object { $_.PriorityScore }
    } elseif ($SortByComplexity) {
        $Requirements = $Requirements | Sort-Object { $RequirementConfig.Complexities[$_.Complexity].Level }
    } elseif ($SortByDate) {
        $Requirements = $Requirements | Sort-Object { $_.CreatedAt } -Descending
    }
    
    # 顯示結果
    if ($Requirements.Count -eq 0) {
        Write-Host "沒有找到符合條件的需求單" -ForegroundColor Yellow
        return $true
    }
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "需求單列表 ($($Requirements.Count) 個)" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    foreach ($Req in $Requirements) {
        $PriorityColor = $RequirementConfig.Priorities[$Req.Priority].Color
        
        Write-Host "[$($Req.ID)] $($Req.Title)" -ForegroundColor White
        Write-Host "  分類: $($Req.Category) | 優先級: $($Req.Priority) | 複雜度: $($Req.Complexity)" -ForegroundColor $PriorityColor
        Write-Host "  狀態: $($Req.Status) | 負責人: $(if ($Req.Assignee) { $Req.Assignee } else { '未分配' })" -ForegroundColor Cyan
        Write-Host "  創建時間: $($Req.CreatedAt) | 分數: $($Req.PriorityScore)" -ForegroundColor Gray
        $Description = $Req.Description
        if ($Description.Length -gt 100) {
            $Description = $Description.Substring(0, 100) + "..."
        }
        Write-Host "  描述: $Description" -ForegroundColor Gray
        Write-Host ""
    }
    
    return $true
}

# 更新需求單
function Update-Requirement {
    param([string]$RequirementID)
    
    if ([string]::IsNullOrEmpty($RequirementID)) {
        $RequirementID = Read-Host "請輸入需求ID"
    }
    
    $Data = Load-Requirements
    if (-not $Data) {
        return $false
    }
    
    $Requirement = $Data.Requirements | Where-Object { $_.ID -eq $RequirementID }
    if (-not $Requirement) {
        Write-Host "找不到需求ID: $RequirementID" -ForegroundColor Red
        return $false
    }
    
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "更新需求單: $RequirementID" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    Write-Host "當前信息:" -ForegroundColor Cyan
    Write-Host "標題: $($Requirement.Title)" -ForegroundColor White
    Write-Host "狀態: $($Requirement.Status)" -ForegroundColor White
    Write-Host "優先級: $($Requirement.Priority)" -ForegroundColor White
    Write-Host "複雜度: $($Requirement.Complexity)" -ForegroundColor White
    Write-Host "負責人: $($Requirement.Assignee)" -ForegroundColor White
    Write-Host ""
    
    # 交互式更新
    $NewTitle = Read-Host "新標題 (當前: $($Requirement.Title))"
    $NewStatus = Read-Host "新狀態 (當前: $($Requirement.Status))"
    $NewPriority = Read-Host "新優先級 (當前: $($Requirement.Priority))"
    $NewComplexity = Read-Host "新複雜度 (當前: $($Requirement.Complexity))"
    $NewAssignee = Read-Host "新負責人 (當前: $($Requirement.Assignee))"
    
    # 更新字段
    if ($NewTitle -and $NewTitle -ne $Requirement.Title) {
        $Requirement.Title = $NewTitle
        $Requirement.History += @{
            Action = "Updated"
            Field = "Title"
            OldValue = $Requirement.Title
            NewValue = $NewTitle
            Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
            User = "System"
            Description = "更新標題"
        }
    }
    
    if ($NewStatus -and $NewStatus -ne $Requirement.Status) {
        if ($RequirementConfig.Statuses.ContainsKey($NewStatus)) {
            $Requirement.Status = $NewStatus
            $Requirement.History += @{
                Action = "Updated"
                Field = "Status"
                OldValue = $Requirement.Status
                NewValue = $NewStatus
                Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
                User = "System"
                Description = "更新狀態"
            }
        } else {
            Write-Host "錯誤: 無效的狀態 '$NewStatus'" -ForegroundColor Red
        }
    }
    
    if ($NewPriority -and $NewPriority -ne $Requirement.Priority) {
        if ($RequirementConfig.Priorities.ContainsKey($NewPriority)) {
            $Requirement.Priority = $NewPriority
            $Requirement.PriorityScore = Get-PriorityScore -Priority $NewPriority -Complexity $Requirement.Complexity -Status $Requirement.Status
            $Requirement.History += @{
                Action = "Updated"
                Field = "Priority"
                OldValue = $Requirement.Priority
                NewValue = $NewPriority
                Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
                User = "System"
                Description = "更新優先級"
            }
        } else {
            Write-Host "錯誤: 無效的優先級 '$NewPriority'" -ForegroundColor Red
        }
    }
    
    if ($NewComplexity -and $NewComplexity -ne $Requirement.Complexity) {
        if ($RequirementConfig.Complexities.ContainsKey($NewComplexity)) {
            $Requirement.Complexity = $NewComplexity
            $Requirement.EstimatedHours = $RequirementConfig.Complexities[$NewComplexity].EstimatedHours
            $Requirement.PriorityScore = Get-PriorityScore -Priority $Requirement.Priority -Complexity $NewComplexity -Status $Requirement.Status
            $Requirement.History += @{
                Action = "Updated"
                Field = "Complexity"
                OldValue = $Requirement.Complexity
                NewValue = $NewComplexity
                Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
                User = "System"
                Description = "更新複雜度"
            }
        } else {
            Write-Host "錯誤: 無效的複雜度 '$NewComplexity'" -ForegroundColor Red
        }
    }
    
    if ($NewAssignee -and $NewAssignee -ne $Requirement.Assignee) {
        $Requirement.Assignee = $NewAssignee
        $Requirement.History += @{
            Action = "Updated"
            Field = "Assignee"
            OldValue = $Requirement.Assignee
            NewValue = $NewAssignee
            Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
            User = "System"
            Description = "更新負責人"
        }
    }
    
    $Requirement.UpdatedAt = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
    
    # 保存數據
    if (Save-Requirements -Data $Data) {
        Write-Host "需求單更新成功!" -ForegroundColor Green
        return $true
    } else {
        Write-Host "需求單更新失敗!" -ForegroundColor Red
        return $false
    }
}

# 刪除需求單
function Remove-Requirement {
    param([string]$RequirementID)
    
    if ([string]::IsNullOrEmpty($RequirementID)) {
        $RequirementID = Read-Host "請輸入要刪除的需求ID"
    }
    
    $Data = Load-Requirements
    if (-not $Data) {
        return $false
    }
    
    $Requirement = $Data.Requirements | Where-Object { $_.ID -eq $RequirementID }
    if (-not $Requirement) {
        Write-Host "找不到需求ID: $RequirementID" -ForegroundColor Red
        return $false
    }
    
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "刪除需求單: $RequirementID" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "標題: $($Requirement.Title)" -ForegroundColor White
    Write-Host "描述: $($Requirement.Description)" -ForegroundColor Gray
    Write-Host ""
    
    $Confirm = Read-Host "確認刪除? (y/N)"
    if ($Confirm -eq 'y' -or $Confirm -eq 'Y') {
        $Data.Requirements = $Data.Requirements | Where-Object { $_.ID -ne $RequirementID }
        
        if (Save-Requirements -Data $Data) {
            Write-Host "需求單刪除成功!" -ForegroundColor Green
            return $true
        } else {
            Write-Host "需求單刪除失敗!" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "取消刪除" -ForegroundColor Yellow
        return $true
    }
}

# 生成統計報告
function New-RequirementReport {
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "需求統計報告" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    $Data = Load-Requirements
    if (-not $Data) {
        return $false
    }
    
    $Requirements = $Data.Requirements
    $TotalRequirements = $Requirements.Count
    
    if ($TotalRequirements -eq 0) {
        Write-Host "沒有需求單數據" -ForegroundColor Yellow
        return $true
    }
    
    # 統計各類別數量
    $CategoryStats = @{}
    $PriorityStats = @{}
    $ComplexityStats = @{}
    $StatusStats = @{}
    $AssigneeStats = @{}
    
    foreach ($Req in $Requirements) {
        # 分類統計
        if (-not $CategoryStats.ContainsKey($Req.Category)) {
            $CategoryStats[$Req.Category] = 0
        }
        $CategoryStats[$Req.Category]++
        
        # 優先級統計
        if (-not $PriorityStats.ContainsKey($Req.Priority)) {
            $PriorityStats[$Req.Priority] = 0
        }
        $PriorityStats[$Req.Priority]++
        
        # 複雜度統計
        if (-not $ComplexityStats.ContainsKey($Req.Complexity)) {
            $ComplexityStats[$Req.Complexity] = 0
        }
        $ComplexityStats[$Req.Complexity]++
        
        # 狀態統計
        if (-not $StatusStats.ContainsKey($Req.Status)) {
            $StatusStats[$Req.Status] = 0
        }
        $StatusStats[$Req.Status]++
        
        # 負責人統計
        if ($Req.Assignee) {
            if (-not $AssigneeStats.ContainsKey($Req.Assignee)) {
                $AssigneeStats[$Req.Assignee] = 0
            }
            $AssigneeStats[$Req.Assignee]++
        }
    }
    
    # 顯示統計結果
    Write-Host "總需求單數量: $TotalRequirements" -ForegroundColor White
    Write-Host ""
    
    Write-Host "按分類統計:" -ForegroundColor Cyan
    foreach ($Category in $CategoryStats.GetEnumerator() | Sort-Object Value -Descending) {
        Write-Host "  $($Category.Key): $($Category.Value)" -ForegroundColor White
    }
    Write-Host ""
    
    Write-Host "按優先級統計:" -ForegroundColor Cyan
    foreach ($Priority in $PriorityStats.GetEnumerator() | Sort-Object { $RequirementConfig.Priorities[$_.Key].Level }) {
        $Color = $RequirementConfig.Priorities[$Priority.Key].Color
        Write-Host "  $($Priority.Key): $($Priority.Value)" -ForegroundColor $Color
    }
    Write-Host ""
    
    Write-Host "按複雜度統計:" -ForegroundColor Cyan
    foreach ($Complexity in $ComplexityStats.GetEnumerator() | Sort-Object { $RequirementConfig.Complexities[$_.Key].Level }) {
        $Color = if ($Complexity.Key -eq "Simple") { "Green" } 
                elseif ($Complexity.Key -eq "Medium") { "Yellow" }
                elseif ($Complexity.Key -eq "Complex") { "Orange" }
                else { "Red" }
        Write-Host "  $($Complexity.Key): $($Complexity.Value) (預估 $([int]$RequirementConfig.Complexities[$Complexity.Key].EstimatedHours * $Complexity.Value)小時)" -ForegroundColor $Color
    }
    Write-Host ""
    
    Write-Host "按狀態統計:" -ForegroundColor Cyan
    foreach ($Status in $StatusStats.GetEnumerator() | Sort-Object { $RequirementConfig.Statuses[$_.Key].Level }) {
        Write-Host "  $($Status.Key): $($Status.Value)" -ForegroundColor White
    }
    Write-Host ""
    
    if ($AssigneeStats.Count -gt 0) {
        Write-Host "按負責人統計:" -ForegroundColor Cyan
        foreach ($Assignee in $AssigneeStats.GetEnumerator() | Sort-Object Value -Descending) {
            Write-Host "  $($Assignee.Key): $($Assignee.Value)" -ForegroundColor White
        }
        Write-Host ""
    }
    
    # 計算工作負載
    $TotalEstimatedHours = 0
    foreach ($Req in $Requirements) {
        $TotalEstimatedHours += $RequirementConfig.Complexities[$Req.Complexity].EstimatedHours
    }
    
    Write-Host "工作負載分析:" -ForegroundColor Cyan
    Write-Host "  總預估工時: $TotalEstimatedHours 小時" -ForegroundColor White
    Write-Host "  平均每個需求: $([math]::Round($TotalEstimatedHours / $TotalRequirements, 1)) 小時" -ForegroundColor White
    Write-Host "  按標準工作日計算: $([math]::Round($TotalEstimatedHours / 8, 1)) 天" -ForegroundColor White
    Write-Host ""
    
    # 生成報告檔案
    $ReportContent = @"
# MingGoRTS 需求統計報告

## 基本統計
- 總需求單數量: $TotalRequirements
- 總預估工時: $TotalEstimatedHours 小時
- 平均每個需求: $([math]::Round($TotalEstimatedHours / $TotalRequirements, 1)) 小時
- 按標準工作日計算: $([math]::Round($TotalEstimatedHours / 8, 1)) 天

## 按分類統計
$($CategoryStats.GetEnumerator() | ForEach-Object { "- $($_.Key): $($_.Value)" }) -join "`n"

## 按優先級統計
$($PriorityStats.GetEnumerator() | ForEach-Object { "- $($_.Key): $($_.Value)" }) -join "`n"

## 按複雜度統計
$($ComplexityStats.GetEnumerator() | ForEach-Object { "- $($_.Key): $($_.Value) (預估 $([int]$RequirementConfig.Complexities[$_.Key].EstimatedHours * $_.Value)小時)" }) -join "`n"

## 按狀態統計
$($StatusStats.GetEnumerator() | ForEach-Object { "- $($_.Key): $($_.Value)" }) -join "`n"

## 按負責人統計
$($AssigneeStats.GetEnumerator() | ForEach-Object { "- $($_.Key): $($_.Value)" }) -join "`n"

## 報告生成時間
$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
"@
    
    $ReportPath = "$ReportsPath\RequirementReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    $ReportContent | Set-Content $ReportPath -Encoding UTF8
    Write-Host "報告已保存: $ReportPath" -ForegroundColor Green
    
    return $true
}

# 顯示幫助
function Show-Help {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "MingGoRTS 需求單管理系統" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "用法:" -ForegroundColor Yellow
    Write-Host "  .\RequirementManagement.ps1 -Action <動作> [參數]" -ForegroundColor White
    Write-Host ""
    Write-Host "可用動作:" -ForegroundColor Yellow
    Write-Host "  create     - 創建需求單" -ForegroundColor White
    Write-Host "  list       - 列出需求單" -ForegroundColor White
    Write-Host "  update     - 更新需求單" -ForegroundColor White
    Write-Host "  delete     - 刪除需求單" -ForegroundColor White
    Write-Host "  report     - 生成統計報告" -ForegroundColor White
    Write-Host "  help       - 顯示幫助" -ForegroundColor White
    Write-Host ""
    Write-Host "參數:" -ForegroundColor Yellow
    Write-Host "  -Interactive          - 交互式模式" -ForegroundColor White
    Write-Host "  -Verbose              - 顯示詳細信息" -ForegroundColor White
    Write-Host "  -FilterCategory       - 篩選分類" -ForegroundColor White
    Write-Host "  -FilterPriority       - 篩選優先級" -ForegroundColor White
    Write-Host "  -FilterStatus         - 篩選狀態" -ForegroundColor White
    Write-Host "  -FilterAssignee       - 篩選負責人" -ForegroundColor White
    Write-Host "  -SortByPriority       - 按優先級排序" -ForegroundColor White
    Write-Host "  -SortByComplexity     - 按複雜度排序" -ForegroundColor White
    Write-Host "  -SortByDate           - 按日期排序" -ForegroundColor White
    Write-Host ""
    Write-Host "範例:" -ForegroundColor Yellow
    Write-Host "  .\RequirementManagement.ps1 -Action create -Interactive" -ForegroundColor White
    Write-Host "  .\RequirementManagement.ps1 -Action list -SortByPriority" -ForegroundColor White
    Write-Host "  .\RequirementManagement.ps1 -Action list -FilterPriority High" -ForegroundColor White
    Write-Host "  .\RequirementManagement.ps1 -Action update -RequirementID REQ-20260323-1234" -ForegroundColor White
    Write-Host "  .\RequirementManagement.ps1 -Action report" -ForegroundColor White
    Write-Host ""
    Write-Host "優先級:" -ForegroundColor Yellow
    Write-Host "  Critical - 關鍵需求，必須立即處理" -ForegroundColor Red
    Write-Host "  High     - 高優先級，應優先處理" -ForegroundColor Orange
    Write-Host "  Medium   - 中等優先級，正常排程處理" -ForegroundColor Yellow
    Write-Host "  Low      - 低優先級，可延後處理" -ForegroundColor Green
    Write-Host "  Backlog  - 待辦清單，有時間處理" -ForegroundColor Gray
    Write-Host ""
    Write-Host "複雜度:" -ForegroundColor Yellow
    Write-Host "  Simple      - 簡單任務，1-4小時" -ForegroundColor Green
    Write-Host "  Medium      - 中等複雜度，1-3天" -ForegroundColor Yellow
    Write-Host "  Complex     - 複雜任務，1週" -ForegroundColor Orange
    Write-Host "  VeryComplex - 非常複雜，2週以上" -ForegroundColor Red
}

# 主執行函數
function Main {
    # 初始化系統
    Initialize-RequirementSystem
    
    # 根據動作執行相應功能
    switch ($Action.ToLower()) {
        "create" {
            New-Requirement
        }
        "list" {
            Get-Requirements -FilterCategory $Category -FilterPriority $Priority -FilterStatus $Status -FilterAssignee $Assignee -SortByPriority:$SortByPriority -SortByComplexity:$SortByComplexity -SortByDate:$SortByDate
        }
        "update" {
            Update-Requirement -RequirementID $RequirementID
        }
        "delete" {
            Remove-Requirement -RequirementID $RequirementID
        }
        "report" {
            New-RequirementReport
        }
        "help" {
            Show-Help
        }
        default {
            Write-Host "未知動作: $Action" -ForegroundColor Red
            Show-Help
        }
    }
}

# 執行主函數
Main
