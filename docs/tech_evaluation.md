# 技術評估報告

## Unreal Engine 5 適配性
- ✅ C++ + Blueprint 混合開發，適合 RTS
- ✅ Nanite 可處理大量單位渲染
- ✅ 內置 replication 系統，便於後續多人開發
- ⚠️ 手機適配需要仔細優化渲染設置

## AI 工具鏈
- Stable Diffusion：本地部署需顯存 8GB+，生成時間 ~10s/張
- ControlNet：poses 用於角色，canny 用於建築，depth 用於場景
- AIVA API：免費版有限額，商用需要訂閱

## 構建與部署
- Windows：Unreal 直接打包
- Android：需要 Android SDK，測試性能
- iOS：需要 Mac 和 Apple Developer 賬戶
