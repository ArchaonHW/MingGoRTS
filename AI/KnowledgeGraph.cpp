/**
 * Potato Engine Knowledge Graph Implementation
 */

#include "KnowledgeGraph.h"
#include <sstream>
#include <algorithm>
#include <queue>
#include <chrono>
#include <cmath>

namespace Potato {
namespace AI {

KnowledgeGraph::KnowledgeGraph() = default;
KnowledgeGraph::~KnowledgeGraph() = default;

uint64_t KnowledgeGraph::Now() const {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
}

// ============================================================================
// Node operations
// ============================================================================

uint64_t KnowledgeGraph::AddNode(const std::string& name, const std::string& type,
                                 const std::map<std::string, std::string>& attributes) {
    std::lock_guard<std::mutex> lock(mutex);

    KGNode node;
    node.id = nextNodeId++;
    node.name = name;
    node.type = type;
    node.attributes = attributes;
    node.createdAt = Now();

    uint64_t id = node.id;
    nodes[id] = node;
    nameIndex[name].push_back(id);
    return id;
}

bool KnowledgeGraph::RemoveNode(uint64_t id) {
    std::lock_guard<std::mutex> lock(mutex);

    auto it = nodes.find(id);
    if (it == nodes.end()) return false;

    // 移除相連邊
    std::vector<uint64_t> toRemove;
    auto outIt = outIndex.find(id);
    if (outIt != outIndex.end()) toRemove = outIt->second;
    auto inIt = inIndex.find(id);
    if (inIt != inIndex.end()) {
        toRemove.insert(toRemove.end(), inIt->second.begin(), inIt->second.end());
    }
    for (uint64_t eid : toRemove) {
        auto eIt = edges.find(eid);
        if (eIt != edges.end()) {
            UnindexEdge(eIt->second);
            edges.erase(eIt);
        }
    }

    // 移除名稱索引
    auto nIt = nameIndex.find(it->second.name);
    if (nIt != nameIndex.end()) {
        auto& v = nIt->second;
        v.erase(std::remove(v.begin(), v.end(), id), v.end());
        if (v.empty()) nameIndex.erase(nIt);
    }

    nodes.erase(it);
    return true;
}

const KGNode* KnowledgeGraph::GetNode(uint64_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = nodes.find(id);
    if (it == nodes.end()) return nullptr;
    it->second.accessCount++;
    return &it->second;
}

std::vector<const KGNode*> KnowledgeGraph::FindNodesByName(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<const KGNode*> result;
    auto it = nameIndex.find(name);
    if (it != nameIndex.end()) {
        for (uint64_t id : it->second) {
            auto nIt = nodes.find(id);
            if (nIt != nodes.end()) result.push_back(&nIt->second);
        }
    }
    return result;
}

std::vector<const KGNode*> KnowledgeGraph::FindNodesByType(const std::string& type) {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<const KGNode*> result;
    for (auto& [id, node] : nodes) {
        if (node.type == type) result.push_back(&node);
    }
    return result;
}

bool KnowledgeGraph::HasNode(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mutex);
    return nameIndex.find(name) != nameIndex.end() && !nameIndex.at(name).empty();
}

// ============================================================================
// Edge operations
// ============================================================================

void KnowledgeGraph::IndexEdge(const KGEdge& e) {
    outIndex[e.source].push_back(e.id);
    inIndex[e.target].push_back(e.id);
}

void KnowledgeGraph::UnindexEdge(const KGEdge& e) {
    auto oIt = outIndex.find(e.source);
    if (oIt != outIndex.end()) {
        auto& v = oIt->second;
        v.erase(std::remove(v.begin(), v.end(), e.id), v.end());
        if (v.empty()) outIndex.erase(oIt);
    }
    auto iIt = inIndex.find(e.target);
    if (iIt != inIndex.end()) {
        auto& v = iIt->second;
        v.erase(std::remove(v.begin(), v.end(), e.id), v.end());
        if (v.empty()) inIndex.erase(iIt);
    }
}

bool KnowledgeGraph::HasEdge(uint64_t source, uint64_t target,
                             const std::string& relation) const {
    auto it = outIndex.find(source);
    if (it == outIndex.end()) return false;
    for (uint64_t eid : it->second) {
        auto eIt = edges.find(eid);
        if (eIt != edges.end() && eIt->second.target == target &&
            eIt->second.relation == relation) {
            return true;
        }
    }
    return false;
}

uint64_t KnowledgeGraph::AddRelation(uint64_t source, uint64_t target,
                                     const std::string& relation, float confidence) {
    std::lock_guard<std::mutex> lock(mutex);
    if (nodes.find(source) == nodes.end() || nodes.find(target) == nodes.end()) {
        return 0;
    }

    // 已存在則更新置信度
    auto oIt = outIndex.find(source);
    if (oIt != outIndex.end()) {
        for (uint64_t eid : oIt->second) {
            auto eIt = edges.find(eid);
            if (eIt != edges.end() && eIt->second.target == target &&
                eIt->second.relation == relation) {
                eIt->second.confidence = std::max(eIt->second.confidence, confidence);
                eIt->second.timestamp = Now();
                return eid;
            }
        }
    }

    KGEdge edge;
    edge.id = nextEdgeId++;
    edge.source = source;
    edge.target = target;
    edge.relation = relation;
    edge.confidence = confidence;
    edge.timestamp = Now();

    uint64_t id = edge.id;
    edges[id] = edge;
    IndexEdge(edges[id]);
    return id;
}

uint64_t KnowledgeGraph::AddRelationByName(const std::string& sourceName,
                                           const std::string& relation,
                                           const std::string& targetName,
                                           float confidence) {
    // 鎖在 AddNode/AddRelation 內部取得；先找/建節點
    uint64_t src, dst;
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto sIt = nameIndex.find(sourceName);
        src = (sIt != nameIndex.end() && !sIt->second.empty()) ? sIt->second[0] : 0;
        auto tIt = nameIndex.find(targetName);
        dst = (tIt != nameIndex.end() && !tIt->second.empty()) ? tIt->second[0] : 0;
    }
    if (src == 0) src = AddNode(sourceName);
    if (dst == 0) dst = AddNode(targetName);
    return AddRelation(src, dst, relation, confidence);
}

bool KnowledgeGraph::RemoveEdge(uint64_t edgeId) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = edges.find(edgeId);
    if (it == edges.end()) return false;
    UnindexEdge(it->second);
    edges.erase(it);
    return true;
}

bool KnowledgeGraph::RemoveRelation(uint64_t source, uint64_t target,
                                    const std::string& relation) {
    std::lock_guard<std::mutex> lock(mutex);
    auto oIt = outIndex.find(source);
    if (oIt == outIndex.end()) return false;
    for (uint64_t eid : oIt->second) {
        auto eIt = edges.find(eid);
        if (eIt != edges.end() && eIt->second.target == target &&
            eIt->second.relation == relation) {
            UnindexEdge(eIt->second);
            edges.erase(eIt);
            return true;
        }
    }
    return false;
}

// ============================================================================
// Query
// ============================================================================

std::vector<KGTriple> KnowledgeGraph::Query(const std::string& subject,
                                            const std::string& predicate,
                                            const std::string& object) const {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<KGTriple> result;

    for (const auto& [eid, edge] : edges) {
        auto sIt = nodes.find(edge.source);
        auto tIt = nodes.find(edge.target);
        if (sIt == nodes.end() || tIt == nodes.end()) continue;

        if (!subject.empty() && sIt->second.name != subject) continue;
        if (!predicate.empty() && edge.relation != predicate) continue;
        if (!object.empty() && tIt->second.name != object) continue;

        KGTriple t;
        t.subject = sIt->second.name;
        t.predicate = edge.relation;
        t.object = tIt->second.name;
        t.confidence = edge.confidence;
        result.push_back(t);
    }
    return result;
}

std::vector<const KGNode*> KnowledgeGraph::GetNeighbors(uint64_t id,
                                                      const std::string& relation,
                                                      bool outgoing) {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<const KGNode*> result;

    const auto& index = outgoing ? outIndex : inIndex;
    auto it = index.find(id);
    if (it == index.end()) return result;

    for (uint64_t eid : it->second) {
        auto eIt = edges.find(eid);
        if (eIt == edges.end()) continue;
        if (!relation.empty() && eIt->second.relation != relation) continue;

        uint64_t neighborId = outgoing ? eIt->second.target : eIt->second.source;
        auto nIt = nodes.find(neighborId);
        if (nIt != nodes.end()) result.push_back(&nIt->second);
    }
    return result;
}

std::optional<KGPath> KnowledgeGraph::FindPath(uint64_t from, uint64_t to,
                                               size_t maxDepth) const {
    std::lock_guard<std::mutex> lock(mutex);
    if (nodes.find(from) == nodes.end() || nodes.find(to) == nodes.end()) {
        return std::nullopt;
    }

    // BFS，記錄路徑與得分
    struct State {
        uint64_t node;
        std::vector<uint64_t> path;
        std::vector<std::string> rels;
        float score;
    };
    std::queue<State> q;
    std::unordered_set<uint64_t> visited;
    q.push({from, {from}, {}, 1.0f});
    visited.insert(from);

    std::optional<KGPath> best;

    while (!q.empty()) {
        State cur = q.front();
        q.pop();
        if (cur.path.size() > maxDepth) continue;

        if (cur.node == to) {
            KGPath p;
            p.nodeIds = cur.path;
            p.relations = cur.rels;
            p.score = cur.score;
            if (!best || p.score > best->score) best = p;
            continue;
        }

        auto oIt = outIndex.find(cur.node);
        if (oIt == outIndex.end()) continue;
        for (uint64_t eid : oIt->second) {
            auto eIt = edges.find(eid);
            if (eIt == edges.end()) continue;
            uint64_t next = eIt->second.target;
            if (visited.count(next)) continue;
            visited.insert(next);
            State ns;
            ns.node = next;
            ns.path = cur.path;
            ns.path.push_back(next);
            ns.rels = cur.rels;
            ns.rels.push_back(eIt->second.relation);
            ns.score = cur.score * eIt->second.confidence;
            q.push(ns);
        }
    }
    return best;
}

// ============================================================================
// Inference
// ============================================================================

size_t KnowledgeGraph::InferTransitive(const std::string& relation, float decay) {
    std::lock_guard<std::mutex> lock(mutex);
    size_t added = 0;

    // 收集該關係的所有邊 (a -> b)
    std::vector<std::pair<uint64_t, uint64_t>> rel;
    for (const auto& [eid, e] : edges) {
        if (e.relation == relation) rel.emplace_back(e.source, e.target);
    }

    // (a,r,b) ∧ (b,r,c) ⇒ (a,r,c)
    std::vector<KGEdge> newEdges;
    for (auto [a, b] : rel) {
        for (auto [b2, c] : rel) {
            if (b != b2 || a == c) continue;
            if (HasEdge(a, c, relation)) continue;

            float conf = 1.0f;
            // 取兩邊置信度乘積再衰減
            for (const auto& [eid, e] : edges) {
                if (e.source == a && e.target == b && e.relation == relation)
                    conf *= e.confidence;
                if (e.source == b && e.target == c && e.relation == relation)
                    conf *= e.confidence;
            }
            conf *= decay;

            KGEdge e;
            e.id = nextEdgeId++;
            e.source = a;
            e.target = c;
            e.relation = relation;
            e.confidence = conf;
            e.timestamp = Now();
            e.inferred = true;
            newEdges.push_back(e);
        }
    }

    for (const auto& e : newEdges) {
        edges[e.id] = e;
        IndexEdge(edges[e.id]);
        added++;
    }
    return added;
}

size_t KnowledgeGraph::InheritThroughIsA(float confidenceScale) {
    std::lock_guard<std::mutex> lock(mutex);
    size_t added = 0;

    // 收集所有 is-a 邊
    std::vector<std::pair<uint64_t, uint64_t>> isA;
    for (const auto& [eid, e] : edges) {
        if (e.relation == "is-a") isA.emplace_back(e.source, e.target);
    }

    std::vector<KGEdge> newEdges;
    for (auto [child, parent] : isA) {
        auto pOut = outIndex.find(parent);
        if (pOut == outIndex.end()) continue;
        for (uint64_t eid : pOut->second) {
            auto eIt = edges.find(eid);
            if (eIt == edges.end()) continue;
            const KGEdge& pe = eIt->second;
            if (pe.relation == "is-a") continue;          // 不繼承 is-a 本身
            if (pe.target == child) continue;
            if (HasEdge(child, pe.target, pe.relation)) continue;

            KGEdge e;
            e.id = nextEdgeId++;
            e.source = child;
            e.target = pe.target;
            e.relation = pe.relation;
            e.confidence = pe.confidence * confidenceScale;
            e.timestamp = Now();
            e.inferred = true;
            newEdges.push_back(e);
        }
    }

    for (const auto& e : newEdges) {
        edges[e.id] = e;
        IndexEdge(edges[e.id]);
        added++;
    }
    return added;
}

// ============================================================================
// Maintenance
// ============================================================================

size_t KnowledgeGraph::Prune(float minConfidence) {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<uint64_t> toRemove;
    for (const auto& [eid, e] : edges) {
        if (e.confidence < minConfidence) toRemove.push_back(eid);
    }
    for (uint64_t eid : toRemove) {
        auto it = edges.find(eid);
        UnindexEdge(it->second);
        edges.erase(it);
    }
    return toRemove.size();
}

void KnowledgeGraph::Decay(uint64_t olderThan, float factor) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& [eid, e] : edges) {
        if (e.timestamp < olderThan) e.confidence *= factor;
    }
}

// ============================================================================
// Serialization (簡單 JSON)
// ============================================================================

std::string KnowledgeGraph::Serialize() const {
    std::lock_guard<std::mutex> lock(mutex);
    std::stringstream ss;
    ss << "{\"nodes\":[";
    bool first = true;
    for (const auto& [id, n] : nodes) {
        if (!first) ss << ",";
        first = false;
        ss << "{\"id\":" << n.id << ",\"name\":\"" << n.name
           << "\",\"type\":\"" << n.type << "\"}";
    }
    ss << "],\"edges\":[";
    first = true;
    for (const auto& [id, e] : edges) {
        if (!first) ss << ",";
        first = false;
        ss << "{\"source\":" << e.source << ",\"target\":" << e.target
           << ",\"relation\":\"" << e.relation << "\",\"confidence\":" << e.confidence
           << ",\"inferred\":" << (e.inferred ? "true" : "false") << "}";
    }
    ss << "]}";
    return ss.str();
}

bool KnowledgeGraph::Deserialize(const std::string& data) {
    // 簡易解析：僅支援 Serialize() 產生的格式
    std::lock_guard<std::mutex> lock(mutex);
    nodes.clear(); edges.clear();
    nameIndex.clear(); outIndex.clear(); inIndex.clear();
    nextNodeId = 1; nextEdgeId = 1;

    auto extractArray = [&](const std::string& key) -> std::string {
        std::string k = "\"" + key + "\":[";
        auto pos = data.find(k);
        if (pos == std::string::npos) return "";
        size_t start = pos + k.size();
        int depth = 1;
        size_t i = start;
        for (; i < data.size() && depth > 0; i++) {
            if (data[i] == '[') depth++;
            else if (data[i] == ']') depth--;
        }
        return data.substr(start, i - start - 1);
    };

    auto parseObjects = [](const std::string& arr) -> std::vector<std::string> {
        std::vector<std::string> objs;
        int depth = 0;
        size_t start = 0;
        for (size_t i = 0; i < arr.size(); i++) {
            if (arr[i] == '{') { if (depth == 0) start = i; depth++; }
            else if (arr[i] == '}') { if (--depth == 0) objs.push_back(arr.substr(start, i - start + 1)); }
        }
        return objs;
    };

    auto getStr = [](const std::string& obj, const std::string& key) -> std::string {
        std::string k = "\"" + key + "\":\"";
        auto pos = obj.find(k);
        if (pos == std::string::npos) return "";
        size_t start = pos + k.size();
        auto end = obj.find("\"", start);
        return obj.substr(start, end - start);
    };
    auto getNum = [](const std::string& obj, const std::string& key) -> double {
        std::string k = "\"" + key + "\":";
        auto pos = obj.find(k);
        if (pos == std::string::npos) return 0;
        return std::stod(obj.substr(pos + k.size()));
    };

    std::unordered_map<uint64_t, uint64_t> idRemap;
    for (const auto& obj : parseObjects(extractArray("nodes"))) {
        KGNode n;
        n.id = nextNodeId++;
        n.name = getStr(obj, "name");
        n.type = getStr(obj, "type");
        n.createdAt = Now();
        idRemap[static_cast<uint64_t>(getNum(obj, "id"))] = n.id;
        nodes[n.id] = n;
        nameIndex[n.name].push_back(n.id);
    }
    for (const auto& obj : parseObjects(extractArray("edges"))) {
        uint64_t src = idRemap[static_cast<uint64_t>(getNum(obj, "source"))];
        uint64_t dst = idRemap[static_cast<uint64_t>(getNum(obj, "target"))];
        if (!src || !dst) continue;
        KGEdge e;
        e.id = nextEdgeId++;
        e.source = src;
        e.target = dst;
        e.relation = getStr(obj, "relation");
        e.confidence = static_cast<float>(getNum(obj, "confidence"));
        e.inferred = obj.find("\"inferred\":true") != std::string::npos;
        e.timestamp = Now();
        edges[e.id] = e;
        IndexEdge(edges[e.id]);
    }
    return !nodes.empty();
}

// ============================================================================
// Statistics
// ============================================================================

size_t KnowledgeGraph::NodeCount() const {
    std::lock_guard<std::mutex> lock(mutex);
    return nodes.size();
}

size_t KnowledgeGraph::EdgeCount() const {
    std::lock_guard<std::mutex> lock(mutex);
    return edges.size();
}

std::vector<std::string> KnowledgeGraph::GetRelationTypes() const {
    std::lock_guard<std::mutex> lock(mutex);
    std::unordered_set<std::string> types;
    for (const auto& [id, e] : edges) types.insert(e.relation);
    return {types.begin(), types.end()};
}

} // namespace AI
} // namespace Potato
