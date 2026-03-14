import { run } from 'file:///C:/Users/potat/.openclaw/workspace/skills/tripo-3d-generation/index.mjs';
import fs from 'fs';
import https from 'https';

async function pollAndDownload() {
  const taskId = '45d0fd2c-0e99-4cf9-b832-97661364dc63';
  let status = 'IN_PROGRESS';
  while(status !== 'SUCCESS') {
    const res = await run({ action: 'status', task_id: taskId }, {});
    status = res.status;
    if (status === 'SUCCESS') {
      console.log('Finished generation!');
      const downloadRes = await run({ action: 'download', task_id: taskId }, {});
      const glbUrl = downloadRes.urls.glb;
      console.log('Downloading GLB from:', glbUrl);
      const file = fs.createWriteStream('C:/HW/MingGoRTS/影片素材庫/infantry_model.glb');
      https.get(glbUrl, function(response) {
        response.pipe(file);
        file.on('finish', function() {
          file.close();
          console.log('Download completed to C:/HW/MingGoRTS/影片素材庫/infantry_model.glb');
        });
      });
      break;
    } else if (status === 'FAILED') {
      console.error('Generation failed!', res);
      break;
    }
    console.log('Progress:', res.progress);
    await new Promise(r => setTimeout(r, 10000));
  }
}
pollAndDownload();
