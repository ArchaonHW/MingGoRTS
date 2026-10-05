#pragma once

/**
 * Potato Engine Knowledge Graph - 知識圖譜與長期記憶
 *
 * 為 AI Agent 提供結構化的長期知識存儲：
 * - 實體/關係三元組 (subject-predicate-object)
 * - 置信度與時效性追蹤
 * - 圖遍歷與路徑查詢
 * - 傳遞關係推理（is-a / part-of 閉包）
 * - 簡單序列化格式
 *
 * 設計原則：
 * - 自包含：僅依賴標準庫
 * - 線程安全：所有公開方法內部加鎖
 * - 與 AIAgentSystem 記憶系統互補（語義結構 vs 情節記憶）
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <mutex>
#include <cstdint>
#include <optional>

namespace Potato {
namespace AI {

/**
 * 知識圖譜節點（實體）
 */
struct KGNode {
    uint64_t id = 0;
    std::string name;                                  // 實體名稱
    std::string type;                                  // 實體類型（如 "class", "function", "concept"）
    std::map<std::string, std::string> attributes;     // 屬性鍵值對
    uint64_t createdAt = 0;                            // 創建時間戳
    uint32_t accessCount = 0;                          // 訪問次數（用於重要性/遺忘）
};

/**
 * 知識圖譜邊（關係）
 */
struct KGEdge {
    uint64_t id = 0;
    uint64_t source = 0;                               // 源節點 id
    uint64_t target = 0;                               // 目標節點 id
    std::string relation;                              // 關係謂詞（如 "is-a", "calls", "depends-on"）
    float confidence = 1.0f;                           // 置信度 [0,1]
    uint64_t timestamp = 0;                            // 建立/更新時間
    bool inferred = false;                             // 是否為推理得出的邊
};

/**
 * 三元組（查詢/結果用）
 */
struct KGTriple {
    std::string subject;
    std::string predicate;
    std::string object;
    float confidence = 1.0f;
};

/**
 * 路徑查詢結果
 */
struct KGPath {
    std::vector<uint64_t> nodeIds;
    std::vector<std::string> relations;
    float score = 0.0f;                                // 路徑上置信度的乘積
};

/**
 * 知識圖譜
 */
class KnowledgeGraph {
public:
    KnowledgeGraph();
    ~KnowledgeGraph();

    // ---- 節點操作 ----
    uint64_t AddNode(const std::string& name, const std::string& type = "",
                     const std::map<std::string, std::string>& attributes = {});
    bool RemoveNode(uint64_t id);                      // 同時移除相連邊
    const KGNode* GetNode(uint64_t id);                // 訪問計數+1
    std::vector<const KGNode*> FindNodesByName(const std::string& name);
    std::vector<const KGNode*> FindNodesByType(const std::string& type);
    bool HasNode(const std::string& name) const;

    // ---- 邊操作 ----
    uint64_t AddRelation(uint64_t source, uint64_t target, const std::string& relation,
                         float confidence = 1.0f);
    uint64_t AddRelationByName(const std::string& sourceName, const std::string& relation,
                               const std::string& targetName, float confidence = 1.0f);
    bool RemoveEdge(uint64_t edgeId);
    bool RemoveRelation(uint64_t source, uint64_t target, const std::string& relation);

    // ---- 查詢 ----
    // 查詢三元組，任一字段為空字串表示通配
    std::vector<KGTriple> Query(const std::string& subject,
                                const std::string& predicate,
                                const std::string& object) const;
    // 某實體的相鄰實體（可限定關係方向/類型）
    std::vector<const KGNode*> GetNeighbors(uint64_t id, const std::string& relation = "",
                                            bool outgoing = true);
    // BFS 最短路徑（返回得分最高的路徑）
    std::optional<KGPath> FindPath(uint64_t from, uint64_t to, size_t maxDepth = 6) const;

    // ---- 推理 ----
    // 對傳遞關係計算閉包：(A,r,B) ∧ (B,r,C) ⇒ (A,r,C)
    // 返回新增的推論邊數量
    size_t InferTransitive(const std::string& relation, float decay = 0.9f);
    // 概念繼承：若 A is-a B，把 B 的 (B,r,X) 推廣為 (A,r,X)（弱化置信度）
    size_t InheritThroughIsA(float confidenceScale = 0.8f);

    // ---- 維護 ----
    // 移除置信度低於閾值的邊（含推論邊）
    size_t Prune(float minConfidence);
    // 訪問衰減：長期未訪問節點的邊置信度乘以 factor
    void Decay(uint64_t olderThan, float factor);

    // ---- 序列化 ----
    std::string Serialize() const;                     // 簡單 JSON
    bool Deserialize(const std::string& data);

    // ---- 統計 ----
    size_t NodeCount() const;
    size_t EdgeCount() const;
    std::vector<std::string> GetRelationTypes() const;

private:
    uint64_t nextNodeId = 1;
    uint64_t nextEdgeId = 1;

    std::unordered_map<uint64_t, KGNode> nodes;
    std::unordered_map<uint64_t, KGEdge> edges;
    // 索引：name -> node ids, source -> edge ids, target -> edge ids
    std::unordered_map<std::string, std::vector<uint64_t>> nameIndex;
    std::unordered_map<uint64_t, std::vector<uint64_t>> outIndex;
    std::unordered_map<uint64_t, std::vector<uint64_t>> inIndex;

    mutable std::mutex mutex;

    uint64_t Now() const;
    void IndexEdge(const KGEdge& e);
    void UnindexEdge(const KGEdge& e);
    bool HasEdge(uint64_t source, uint64_t target, const std::string& relation) const;
};

} // namespace AI
} // namespace Potato
