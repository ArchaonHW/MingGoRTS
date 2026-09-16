// One-shot replica of bmad-method installer's _cleanupSkillDirs():
// removes redundant skill source dirs inside _bmad/ (rendered copies live in .agents/skills).
// Retries on EBUSY/EPERM via fs.rmSync's built-in retry (Windows file-lock race).
const fs = require('fs');
const path = require('path');
const csv = require('C:/Users/potat/AppData/Local/npm-cache/_npx/de806ab887efbc31/node_modules/csv-parse/dist/cjs/sync.cjs');

const projectRoot = process.cwd();
const bmadDir = path.join(projectRoot, '_bmad');
const csvPath = path.join(bmadDir, '_config', 'skill-manifest.csv');
const records = csv.parse(fs.readFileSync(csvPath, 'utf8'), { columns: true, skip_empty_lines: true });

const bmadPrefix = '_bmad/';
let removed = 0, skipped = 0, failed = 0;

function removeEmptyParents(dir) {
  let current = dir;
  while (true) {
    const rel = path.relative(bmadDir, current);
    if (rel === '' || rel.startsWith('..') || path.isAbsolute(rel)) break;
    try {
      if (fs.readdirSync(current).length > 0) break;
      fs.rmdirSync(current);
    } catch { break; }
    current = path.dirname(current);
  }
}

for (const r of records) {
  if (!r.path) continue;
  const rel = r.path.startsWith(bmadPrefix) ? r.path.slice(bmadPrefix.length) : r.path;
  const sourceDir = path.dirname(path.join(bmadDir, rel));
  if (!sourceDir.startsWith(bmadDir)) { skipped++; continue; }
  if (!fs.existsSync(sourceDir)) { skipped++; continue; }
  try {
    fs.rmSync(sourceDir, { recursive: true, force: true, maxRetries: 20, retryDelay: 500 });
    removed++;
    removeEmptyParents(path.dirname(sourceDir));
  } catch (e) {
    failed++;
    console.error(`FAILED ${sourceDir}: ${e.code || e.message}`);
  }
}
console.log(`removed=${removed} skipped=${skipped} failed=${failed}`);
process.exit(failed ? 1 : 0);
