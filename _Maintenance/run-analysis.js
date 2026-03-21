#!/usr/bin/env node
// run-analysis.js - 手動啟動 BMAD 分析工作流
const { readdirSync, readFileSync, writeFileSync, existsSync } = require('fs');
const { join } = require('path');

const projectDir = process.cwd();
const bmadDir = join(projectDir, '_bmad');
const workflowsDir = join(bmadDir, 'workflows');

// 讀取分析工作流定義
const workflow = JSON.parse(readFileSync(join(workflowsDir, 'analysis.json'), 'utf8'));
console.log(`?? Starting analysis workflow: ${workflow.name}`);

// 簡單的執行邏輯：依次處理每個步驟
for (const step of workflow.steps) {
    console.log(`\n??  Step: ${step.name} (agent: ${step.agent})`);
    // TODO: 實際調用 BMAD agent 系統
    console.log(`   Output: ${step.output}`);
}

console.log('\n? Analysis workflow completed!');
