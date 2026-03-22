# 蝝????誨蝣潮???- 鞈撘????摰頂蝯?# Asset-Blueprint Integration Usage Example
# Generated: 03/23/2026 02:58:27

## 蝜?銝剜?隤芣? (Traditional Chinese)

### 蝟餌絞璁膩
?祉頂蝯勗祕?曆?鞈撘????蝬?璈嚗?閮梧?
1. 雿輻頠???(Soft References) 蝞∠?鞈
2. ??甇亥??亥??Ｖ誑?踹??餃?
3. ??蝬?鞈?啗??惇??4. 摰???Ｚ??亦??蕭頩?
### 雿輻甇仿?

#### 1. ??C++ 銝剖?憪?鞈撘
`cpp
UAssetBlueprintBridge* Bridge = NewObject<UAssetBlueprintBridge>(this);
Bridge->InitializeAssetReferences(TEXT("BP_MingWarrior"));
Bridge->LoadAssetsAsync();
`

#### 2. ?刻??葉雿輻
1. 瘛餃? AssetBlueprintBridge 蝯辣??Actor
2. 隤輻 InitializeAssetReferences ?喳 Blueprint ?迂
3. 隤輻 LoadAssetsAsync ????甇亥???4. ?? OnAssetsLoaded 鈭辣
5. 雿輻 GetLoadedAsset ?脣?撌脰??亦?鞈

#### 3. 鞈蝬?
鞈頛敺??芸?蝬??啗???撠?撅祆改??⊿???????
### 鞈頛?芸?蝝?- Critical (?): 蝡頛嚗憛?
- High (擃?: ?芸???甇亥???- Normal (?桅?: 璅???甇亥???- Low (雿?: ?頛嚗撱園

### ?蔭?辣隤芣?
JSON ?蔭?辣摰儔鈭????Ｗ??剁?
- AssetName: 鞈?迂
- AssetPath: 鞈頝臬? (Soft Object Path)
- Type: 鞈憿?
- LoadPriority: 頛?芸?蝝?
## English Documentation

### System Overview
This system implements asset reference and Blueprint binding:
1. Use soft references for asset management
2. Async asset loading to prevent blocking
3. Dynamic asset binding to Blueprint properties
4. Complete asset loading state tracking

### Usage Steps

#### 1. Initialize in C++
`cpp
UAssetBlueprintBridge* Bridge = NewObject<UAssetBlueprintBridge>(this);
Bridge->InitializeAssetReferences(TEXT("BP_MingWarrior"));
Bridge->LoadAssetsAsync();
`

#### 2. In Blueprint
1. Add AssetBlueprintBridge component to Actor
2. Call InitializeAssetReferences with Blueprint name
3. Call LoadAssetsAsync to start async loading
4. Listen to OnAssetsLoaded event
5. Use GetLoadedAsset to retrieve loaded assets

#### 3. Asset Binding
Assets are automatically bound to corresponding Blueprint properties after loading.

### Load Priorities
- Critical: Immediate, blocking load
- High: Priority async load
- Normal: Standard async load
- Low: Background load, can be delayed

---
Integration ID: INT_20260323_025826_BP_MingWarrior
Asset Type: Unit
Blueprint: BP_MingWarrior
