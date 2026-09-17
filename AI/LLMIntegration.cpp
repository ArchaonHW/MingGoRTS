/**
 * Potato Engine LLM Integration System Implementation
 */

#include "LLMIntegration.h"
#include "Serialization/JsonParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <algorithm>
#include <regex>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winhttp.h>
#if defined(_MSC_VER)
#pragma comment(lib, "winhttp.lib")
#endif
#endif

namespace Potato {
namespace AI {

namespace {

#ifdef _WIN32
// UTF-8 → UTF-16（WinHTTP API 皆為寬字元）
std::wstring Utf8ToWide(const std::string& s) {
    if (s.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    if (n <= 0) return {};
    std::wstring w((size_t)n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &w[0], n);
    return w;
}

// 解析 scheme://host[:port][/path]
struct ParsedUrl {
    bool secure = true;
    std::wstring host;
    INTERNET_PORT port = 443;
    std::wstring path = L"/";
};

bool ParseHttpUrl(const std::string& url, ParsedUrl& out) {
    std::string u = url;
    if (u.rfind("https://", 0) == 0) { out.secure = true;  u.erase(0, 8); }
    else if (u.rfind("http://", 0) == 0) { out.secure = false; u.erase(0, 7); }
    else return false;
    size_t slash = u.find('/');
    std::string hostport = (slash == std::string::npos) ? u : u.substr(0, slash);
    std::string path = (slash == std::string::npos) ? "/" : u.substr(slash);
    size_t colon = hostport.rfind(':');
    if (colon != std::string::npos) {
        int p = std::atoi(hostport.substr(colon + 1).c_str());
        if (p <= 0 || p > 65535) return false;
        out.port = (INTERNET_PORT)p;
        hostport.erase(colon);
    } else {
        out.port = out.secure ? 443 : 80;
    }
    if (hostport.empty() || hostport.size() > 253) return false;
    out.host = Utf8ToWide(hostport);
    out.path = Utf8ToWide(path);
    return !out.host.empty();
}

// HTTPS POST（WinHTTP）。成功回 true 並填 outBody；失敗回 false + errMsg。
// headers 為 "Name: Value" 列表。回應體上限 16MB 防 OOM。
bool HttpPostWinHttp(const std::string& url,
                     const std::vector<std::string>& headers,
                     const std::string& body,
                     int& outStatus, std::string& outBody,
                     std::string& errMsg) {
    ParsedUrl pu;
    if (!ParseHttpUrl(url, pu)) {
        errMsg = "invalid URL: " + url;
        return false;
    }
    HINTERNET session = WinHttpOpen(L"PotatoEngine-LLM/1.0",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) { errMsg = "WinHttpOpen failed"; return false; }
    // LLM 回應可能較慢：resolve/connect 10s、send 30s、receive 120s
    WinHttpSetTimeouts(session, 10000, 10000, 30000, 120000);
    bool ok = false;
    HINTERNET connect = WinHttpConnect(session, pu.host.c_str(), pu.port, 0);
    if (connect) {
        HINTERNET req = WinHttpOpenRequest(connect, L"POST", pu.path.c_str(),
            nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
            pu.secure ? WINHTTP_FLAG_SECURE : 0);
        if (req) {
            for (const auto& h : headers) {
                std::wstring wh = Utf8ToWide(h);
                if (!wh.empty())
                    WinHttpAddRequestHeaders(req, wh.c_str(), (DWORD)-1,
                        WINHTTP_ADDREQ_FLAG_ADD);
            }
            if (WinHttpSendRequest(req, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                    (LPVOID)body.data(), (DWORD)body.size(),
                    (DWORD)body.size(), 0) &&
                WinHttpReceiveResponse(req, nullptr)) {
                DWORD status = 0, sz = sizeof(status);
                WinHttpQueryHeaders(req,
                    WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                    WINHTTP_HEADER_NAME_BY_INDEX, &status, &sz,
                    WINHTTP_NO_HEADER_INDEX);
                outStatus = (int)status;
                outBody.clear();
                char buf[16384];
                DWORD read = 0;
                const size_t kMaxBody = 16u * 1024 * 1024;
                while (WinHttpReadData(req, buf, sizeof(buf), &read) && read > 0) {
                    outBody.append(buf, read);
                    if (outBody.size() > kMaxBody) {
                        errMsg = "response body exceeds 16MB limit";
                        WinHttpCloseHandle(req);
                        WinHttpCloseHandle(connect);
                        WinHttpCloseHandle(session);
                        return false;
                    }
                }
                ok = true;
            } else {
                errMsg = "WinHTTP send/receive failed (err=" +
                         std::to_string(GetLastError()) + ")";
            }
            WinHttpCloseHandle(req);
        } else {
            errMsg = "WinHttpOpenRequest failed";
        }
        WinHttpCloseHandle(connect);
    } else {
        errMsg = "WinHttpConnect failed (err=" +
                 std::to_string(GetLastError()) + ")";
    }
    WinHttpCloseHandle(session);
    return ok;
}
#endif // _WIN32

} // anonymous namespace


// 跳脫 JSON 字串中的特殊字元，防止產生不合法 JSON 與注入
static std::string EscapeJson(const std::string& input) {
    std::string out;
    out.reserve(input.size() + 8);
    for (char c : input) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b";  break;
            case '\f': out += "\\f";  break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char buf[8];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    out += buf;
                } else {
                    out += c;
                }
        }
    }
    return out;
}

// ============================================================================
// OpenAI Client Implementation
// ============================================================================

OpenAIClient::OpenAIClient(const std::string& key)
    : apiKey(key)
    , baseURL("https://api.openai.com/v1") {
}

OpenAIClient::~OpenAIClient() {
}

LLMResponse OpenAIClient::ChatCompletion(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config) {
    
    LLMResponse response;
    
    // Build JSON request
    std::stringstream json;
    json << "{";
    json << "\"model\":\"" << EscapeJson(config.model) << "\",";
    json << "\"messages\":[";
    
    for (size_t i = 0; i < messages.size(); i++) {
        if (i > 0) json << ",";
        json << "{";
        json << "\"role\":\"";
        switch (messages[i].role) {
            case MessageRole::System: json << "system"; break;
            case MessageRole::User: json << "user"; break;
            case MessageRole::Assistant: json << "assistant"; break;
            case MessageRole::Tool: json << "tool"; break;
        }
        json << "\",";
        json << "\"content\":\"" << EscapeJson(messages[i].content) << "\"";
        json << "}";
    }
    
    json << "],";
    json << "\"temperature\":" << config.temperature << ",";
    json << "\"max_tokens\":" << config.maxTokens << ",";
    json << "\"top_p\":" << config.topP;
    json << "}";
    
    // Make request (placeholder - would use HTTP client in real implementation)
    std::string jsonResponse = MakeRequest("/chat/completions", json.str());
    response = ParseResponse(jsonResponse);
    
    return response;
}

LLMResponse OpenAIClient::ChatCompletionWithTools(
    const std::vector<ChatMessage>& messages,
    const std::vector<ToolDefinition>& tools,
    const LLMConfig& config) {
    
    LLMResponse response;
    
    // Build JSON request with tools
    std::stringstream json;
    json << "{";
    json << "\"model\":\"" << EscapeJson(config.model) << "\",";
    json << "\"messages\":[";
    
    for (size_t i = 0; i < messages.size(); i++) {
        if (i > 0) json << ",";
        json << "{";
        json << "\"role\":\"";
        switch (messages[i].role) {
            case MessageRole::System: json << "system"; break;
            case MessageRole::User: json << "user"; break;
            case MessageRole::Assistant: json << "assistant"; break;
            case MessageRole::Tool: json << "tool"; break;
        }
        json << "\",";
        json << "\"content\":\"" << EscapeJson(messages[i].content) << "\"";
        json << "}";
    }
    
    json << "],";
    json << "\"tools\":[";
    
    for (size_t i = 0; i < tools.size(); i++) {
        if (i > 0) json << ",";
        json << "{";
        json << "\"type\":\"function\",";
        json << "\"function\":{";
        json << "\"name\":\"" << EscapeJson(tools[i].name) << "\",";
        json << "\"description\":\"" << EscapeJson(tools[i].description) << "\",";
        json << "\"parameters\":" << tools[i].parametersSchema;
        json << "}";
        json << "}";
    }
    
    json << "],";
    json << "\"temperature\":" << config.temperature << ",";
    json << "\"max_tokens\":" << config.maxTokens;
    json << "}";
    
    std::string jsonResponse = MakeRequest("/chat/completions", json.str());
    response = ParseResponse(jsonResponse);
    
    return response;
}

void OpenAIClient::ChatCompletionStream(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config,
    std::function<void(const std::string& chunk)> callback) {
    
    // Placeholder for streaming implementation
    // In real implementation, would use HTTP streaming
    LLMResponse response = ChatCompletion(messages, config);
    if (response.success) {
        callback(response.content);
    }
}

std::future<LLMResponse> OpenAIClient::ChatCompletionAsync(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config) {
    
    return std::async(std::launch::async, [this, messages, config]() {
        return ChatCompletion(messages, config);
    });
}

std::vector<float> OpenAIClient::GenerateEmbedding(
    const std::string& text,
    const std::string& model) {

    if (text.empty()) return {};
    std::string m = model.empty() ? "text-embedding-ada-002" : model;
    std::stringstream json;
    json << "{\"input\":\"" << EscapeJson(text)
         << "\",\"model\":\"" << EscapeJson(m) << "\"}";
    std::string resp = MakeRequest("/embeddings", json.str());
    if (resp.empty()) return {};

    JsonValue root;
    if (!JsonValue::ParseOk(resp, root)) return {};
    std::vector<float> emb;
    for (const JsonValue& v : root["data"][0]["embedding"].AsArray())
        emb.push_back(v.AsFloat());
    return emb;
}

std::vector<std::vector<float>> OpenAIClient::GenerateEmbeddings(
    const std::vector<std::string>& texts,
    const std::string& model) {
    
    std::vector<std::vector<float>> embeddings;
    for (const auto& text : texts) {
        embeddings.push_back(GenerateEmbedding(text, model));
    }
    return embeddings;
}

bool OpenAIClient::ValidateConfig(const LLMConfig& config) {
    return !config.apiKey.empty() && !config.model.empty();
}

std::string OpenAIClient::GetDefaultModel() {
    return "gpt-4";
}

std::string OpenAIClient::MakeRequest(const std::string& endpoint, const std::string& jsonBody) {
    lastTransportError.clear();
#ifdef _WIN32
    std::string body, err;
    int status = 0;
    std::vector<std::string> headers = {
        "Authorization: Bearer " + apiKey,
        "Content-Type: application/json"
    };
    if (!HttpPostWinHttp(baseURL + endpoint, headers, jsonBody,
                         status, body, err)) {
        lastTransportError = "HTTP transport failed: " + err;
        return {};
    }
    if (status < 200 || status >= 300) {
        // 非 2xx：body 仍可能是 JSON 錯誤物件，留給 ParseResponse 解析
        if (body.empty())
            lastTransportError = "HTTP " + std::to_string(status);
    }
    return body;
#else
    (void)endpoint;
    (void)jsonBody;
    lastTransportError = "HTTP transport unavailable on this platform";
    return {};
#endif
}

LLMResponse OpenAIClient::ParseResponse(const std::string& jsonResponse) {
    LLMResponse response;

    if (jsonResponse.empty()) {
        response.success = false;
        response.error = lastTransportError.empty()
            ? "LLM request failed (OpenAI)"
            : "LLM request failed (OpenAI): " + lastTransportError;
        return response;
    }

    JsonValue root;
    if (!JsonValue::ParseOk(jsonResponse, root)) {
        response.success = false;
        response.error = "LLM response parse failed (OpenAI): invalid JSON";
        return response;
    }

    const JsonValue& err = root["error"]["message"];
    if (err.IsString()) {
        response.success = false;
        response.error = err.AsString();
        return response;
    }

    const JsonValue& choice = root["choices"][0];
    response.content = choice["message"]["content"].AsString();
    if (response.content.empty()) {
        response.success = false;
        response.error = "LLM response missing content (OpenAI)";
        return response;
    }

    response.success = true;
    response.finishReason = choice["finish_reason"].AsString("stop");
    response.promptTokens = root["usage"]["prompt_tokens"].AsInt();
    response.completionTokens = root["usage"]["completion_tokens"].AsInt();
    response.totalTokens = root["usage"]["total_tokens"].AsInt();
    return response;
}

// ============================================================================
// Anthropic Client Implementation
// ============================================================================

AnthropicClient::AnthropicClient(const std::string& key)
    : apiKey(key)
    , baseURL("https://api.anthropic.com") {
}

AnthropicClient::~AnthropicClient() {
}

LLMResponse AnthropicClient::ChatCompletion(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config) {
    
    LLMResponse response;
    
    // Build JSON request for Anthropic API
    std::stringstream json;
    json << "{";
    json << "\"model\":\"" << EscapeJson(config.model) << "\",";
    json << "\"max_tokens\":" << config.maxTokens << ",";
    json << "\"messages\":[";
    
    for (size_t i = 0; i < messages.size(); i++) {
        if (i > 0) json << ",";
        json << "{";
        json << "\"role\":\"";
        switch (messages[i].role) {
            case MessageRole::User: json << "user"; break;
            case MessageRole::Assistant: json << "assistant"; break;
            default: json << "user"; break;
        }
        json << "\",";
        json << "\"content\":\"" << EscapeJson(messages[i].content) << "\"";
        json << "}";
    }
    
    json << "]}";
    
    std::string jsonResponse = MakeRequest("/v1/messages", json.str());
    response = ParseResponse(jsonResponse);
    
    return response;
}

LLMResponse AnthropicClient::ChatCompletionWithTools(
    const std::vector<ChatMessage>& messages,
    const std::vector<ToolDefinition>& tools,
    const LLMConfig& config) {
    
    // Anthropic tool calling implementation
    return ChatCompletion(messages, config);
}

void AnthropicClient::ChatCompletionStream(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config,
    std::function<void(const std::string& chunk)> callback) {
    
    LLMResponse response = ChatCompletion(messages, config);
    if (response.success) {
        callback(response.content);
    }
}

std::future<LLMResponse> AnthropicClient::ChatCompletionAsync(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config) {
    
    return std::async(std::launch::async, [this, messages, config]() {
        return ChatCompletion(messages, config);
    });
}

std::vector<float> AnthropicClient::GenerateEmbedding(
    const std::string& text,
    const std::string& model) {

    // HTTP transport 未接入——回傳空向量（同 OpenAIClient 的說明）
    (void)text;
    (void)model;
    return {};
}

std::vector<std::vector<float>> AnthropicClient::GenerateEmbeddings(
    const std::vector<std::string>& texts,
    const std::string& model) {
    
    std::vector<std::vector<float>> embeddings;
    for (const auto& text : texts) {
        embeddings.push_back(GenerateEmbedding(text, model));
    }
    return embeddings;
}

bool AnthropicClient::ValidateConfig(const LLMConfig& config) {
    return !config.apiKey.empty() && !config.model.empty();
}

std::string AnthropicClient::GetDefaultModel() {
    return "claude-3-opus-20240229";
}

std::string AnthropicClient::MakeRequest(const std::string& endpoint, const std::string& jsonBody) {
    lastTransportError.clear();
#ifdef _WIN32
    std::string body, err;
    int status = 0;
    std::vector<std::string> headers = {
        "x-api-key: " + apiKey,
        "anthropic-version: 2023-06-01",
        "Content-Type: application/json"
    };
    if (!HttpPostWinHttp(baseURL + endpoint, headers, jsonBody,
                         status, body, err)) {
        lastTransportError = "HTTP transport failed: " + err;
        return {};
    }
    if ((status < 200 || status >= 300) && body.empty())
        lastTransportError = "HTTP " + std::to_string(status);
    return body;
#else
    (void)endpoint;
    (void)jsonBody;
    lastTransportError = "HTTP transport unavailable on this platform";
    return {};
#endif
}

LLMResponse AnthropicClient::ParseResponse(const std::string& jsonResponse) {
    LLMResponse response;

    if (jsonResponse.empty()) {
        response.success = false;
        response.error = lastTransportError.empty()
            ? "LLM request failed (Anthropic)"
            : "LLM request failed (Anthropic): " + lastTransportError;
        return response;
    }

    JsonValue root;
    if (!JsonValue::ParseOk(jsonResponse, root)) {
        response.success = false;
        response.error = "LLM response parse failed (Anthropic): invalid JSON";
        return response;
    }

    const JsonValue& err = root["error"]["message"];
    if (err.IsString()) {
        response.success = false;
        response.error = err.AsString();
        return response;
    }

    // Anthropic 回應格式: content 為 [{type:"text", text:"..."}] 陣列
    for (const JsonValue& block : root["content"].AsArray()) {
        if (block["type"].AsString() == "text") {
            response.content += block["text"].AsString();
        }
    }
    if (response.content.empty()) {
        response.success = false;
        response.error = "LLM response missing content (Anthropic)";
        return response;
    }

    response.success = true;
    response.finishReason = root["stop_reason"].AsString("end_turn");
    response.promptTokens = root["usage"]["input_tokens"].AsInt();
    response.completionTokens = root["usage"]["output_tokens"].AsInt();
    response.totalTokens = response.promptTokens + response.completionTokens;
    return response;
}

// ============================================================================
// Local Model Client Implementation
// ============================================================================

LocalModelClient::LocalModelClient(const std::string& path)
    : modelPath(path)
    , modelHandle(nullptr) {
}

LocalModelClient::~LocalModelClient() {
    UnloadModel();
}

LLMResponse LocalModelClient::ChatCompletion(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config) {
    
    LLMResponse response;
    
    // Build prompt from messages
    std::stringstream prompt;
    for (const auto& msg : messages) {
        switch (msg.role) {
            case MessageRole::System:
                prompt << "System: " << msg.content << "\n";
                break;
            case MessageRole::User:
                prompt << "User: " << msg.content << "\n";
                break;
            case MessageRole::Assistant:
                prompt << "Assistant: " << msg.content << "\n";
                break;
            default:
                break;
        }
    }
    prompt << "Assistant:";
    
    std::string baseURL = config.baseURL.empty()
        ? "http://localhost:11434" : config.baseURL;
    std::string model = config.model.empty() ? modelPath : config.model;
    std::string localResponse = MakeLocalRequest(prompt.str(), baseURL, model);
    response = ParseLocalResponse(localResponse);
    
    return response;
}

LLMResponse LocalModelClient::ChatCompletionWithTools(
    const std::vector<ChatMessage>& messages,
    const std::vector<ToolDefinition>& tools,
    const LLMConfig& config) {
    
    return ChatCompletion(messages, config);
}

void LocalModelClient::ChatCompletionStream(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config,
    std::function<void(const std::string& chunk)> callback) {
    
    LLMResponse response = ChatCompletion(messages, config);
    if (response.success) {
        callback(response.content);
    }
}

std::future<LLMResponse> LocalModelClient::ChatCompletionAsync(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config) {
    
    return std::async(std::launch::async, [this, messages, config]() {
        return ChatCompletion(messages, config);
    });
}

std::vector<float> LocalModelClient::GenerateEmbedding(
    const std::string& text,
    const std::string& model) {

    // Ollama /api/embeddings；無 HTTP 層的平台回傳空向量，
    // RAG 層對空 embedding 已降級處理。
    if (text.empty()) return {};
#ifdef _WIN32
    std::string m = model.empty()
        ? (modelPath.empty() ? "nomic-embed-text" : modelPath) : model;
    std::stringstream json;
    json << "{\"model\":\"" << EscapeJson(m)
         << "\",\"prompt\":\"" << EscapeJson(text) << "\"}";
    std::string body, err;
    int status = 0;
    std::vector<std::string> headers = {"Content-Type: application/json"};
    if (!HttpPostWinHttp("http://localhost:11434/api/embeddings",
                         headers, json.str(), status, body, err))
        return {};
    JsonValue root;
    if (!JsonValue::ParseOk(body, root)) return {};
    std::vector<float> emb;
    for (const JsonValue& v : root["embedding"].AsArray())
        emb.push_back(v.AsFloat());
    return emb;
#else
    (void)model;
    return {};
#endif
}

std::vector<std::vector<float>> LocalModelClient::GenerateEmbeddings(
    const std::vector<std::string>& texts,
    const std::string& model) {
    
    std::vector<std::vector<float>> embeddings;
    for (const auto& text : texts) {
        embeddings.push_back(GenerateEmbedding(text, model));
    }
    return embeddings;
}

bool LocalModelClient::ValidateConfig(const LLMConfig& config) {
    return !config.model.empty();
}

std::string LocalModelClient::GetDefaultModel() {
    return "llama-2-7b";
}

bool LocalModelClient::LoadModel(const std::string& modelPath) {
    // Ollama 模式：modelPath 即伺服器端模型名（如 "llama3"），
    // 不做檔案檢查——模型存不存在由伺服器在推論時回報。
    if (modelPath.empty()) return false;
    this->modelPath = modelPath;
    return true;
}

void LocalModelClient::UnloadModel() {
    if (modelHandle) {
        // Placeholder for model unloading
        modelHandle = nullptr;
    }
}

std::string LocalModelClient::MakeLocalRequest(const std::string& prompt,
                                               const std::string& baseURL,
                                               const std::string& model) {
    lastLocalError.clear();
    if (model.empty()) {
        lastLocalError = "no model specified";
        return {};
    }
#ifdef _WIN32
    // Ollama /api/generate（stream=false 回單一 JSON）
    std::stringstream json;
    json << "{\"model\":\"" << EscapeJson(model)
         << "\",\"prompt\":\"" << EscapeJson(prompt)
         << "\",\"stream\":false}";
    std::string body, err;
    int status = 0;
    std::vector<std::string> headers = {"Content-Type: application/json"};
    if (!HttpPostWinHttp(baseURL + "/api/generate", headers, json.str(),
                         status, body, err)) {
        lastLocalError = "ollama unreachable: " + err;
        return {};
    }
    JsonValue root;
    if (!JsonValue::ParseOk(body, root)) {
        lastLocalError = "invalid ollama response";
        return {};
    }
    const JsonValue& e = root["error"];
    if (e.IsString()) {
        lastLocalError = e.AsString();
        return {};
    }
    return root["response"].AsString();
#else
    (void)prompt;
    (void)baseURL;
    lastLocalError = "HTTP transport unavailable on this platform";
    return {};
#endif
}

LLMResponse LocalModelClient::ParseLocalResponse(const std::string& response) {
    LLMResponse resp;
    if (response.empty()) {
        resp.success = false;
        resp.error = lastLocalError.empty()
            ? "Local model inference unavailable"
            : "Local model inference failed: " + lastLocalError;
        return resp;
    }
    resp.content = response;
    resp.success = true;
    resp.finishReason = "stop";
    return resp;
}

// ============================================================================
// LLM Manager Implementation
// ============================================================================

LLMManager::LLMManager()
    : defaultProvider(LLMProvider::OpenAI)
    , fallbackProvider(LLMProvider::Anthropic)
    , fallbackEnabled(false)
    , rateLimitingEnabled(false)
    , rateLimit(60)
    , cachingEnabled(false) {
    
    memset(&stats, 0, sizeof(stats));
}

LLMManager::~LLMManager() {
}

void LLMManager::RegisterClient(LLMProvider provider, std::unique_ptr<ILLMClient> client) {
    clients[provider] = std::move(client);
}

void LLMManager::UnregisterClient(LLMProvider provider) {
    clients.erase(provider);
}

ILLMClient* LLMManager::GetClient(LLMProvider provider) {
    auto it = clients.find(provider);
    if (it != clients.end()) {
        return it->second.get();
    }
    return nullptr;
}

void LLMManager::SetDefaultProvider(LLMProvider provider) {
    defaultProvider = provider;
}

LLMResponse LLMManager::Chat(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config) {
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    stats.totalRequests++;
    
    // Check rate limit
    if (rateLimitingEnabled && !CheckRateLimit()) {
        LLMResponse response;
        response.success = false;
        response.error = "Rate limit exceeded";
        stats.failedRequests++;
        return response;
    }
    
    // Check cache
    if (cachingEnabled) {
        std::string cacheKey = GenerateCacheKey(messages, config);
        LLMResponse* cached = GetFromCache(cacheKey);
        if (cached) {
            stats.successfulRequests++;
            return *cached;
        }
    }
    
    // Get client
    ILLMClient* client = GetClient(defaultProvider);
    if (!client) {
        stats.failedRequests++;
        LLMResponse response;
        response.success = false;
        response.error = "No client available for default provider";
        return response;
    }
    
    // Make request
    LLMResponse response = client->ChatCompletion(messages, config);
    
    // Fallback if enabled and failed
    if (!response.success && fallbackEnabled) {
        ILLMClient* fallbackClient = GetClient(fallbackProvider);
        if (fallbackClient) {
            response = fallbackClient->ChatCompletion(messages, config);
        }
    }
    
    // Update stats
    if (response.success) {
        stats.successfulRequests++;
        stats.totalTokens += response.totalTokens;
        
        // Add to cache
        if (cachingEnabled) {
            std::string cacheKey = GenerateCacheKey(messages, config);
            AddToCache(cacheKey, response);
        }
    } else {
        stats.failedRequests++;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    stats.averageResponseTime = (stats.averageResponseTime * (stats.totalRequests - 1) + duration.count() / 1000.0f) / stats.totalRequests;
    
    return response;
}

LLMResponse LLMManager::ChatWithTools(
    const std::vector<ChatMessage>& messages,
    const std::vector<ToolDefinition>& tools,
    const LLMConfig& config) {
    
    ILLMClient* client = GetClient(defaultProvider);
    if (!client) {
        LLMResponse response;
        response.success = false;
        response.error = "No client available";
        return response;
    }
    
    return client->ChatCompletionWithTools(messages, tools, config);
}

void LLMManager::ChatStream(
    const std::vector<ChatMessage>& messages,
    const LLMConfig& config,
    std::function<void(const std::string& chunk)> callback) {
    
    ILLMClient* client = GetClient(defaultProvider);
    if (client) {
        client->ChatCompletionStream(messages, config, callback);
    }
}

std::vector<float> LLMManager::GenerateEmbedding(
    const std::string& text,
    const std::string& model) {
    
    ILLMClient* client = GetClient(defaultProvider);
    if (client) {
        return client->GenerateEmbedding(text, model);
    }
    return std::vector<float>();
}

std::vector<std::vector<float>> LLMManager::GenerateEmbeddings(
    const std::vector<std::string>& texts,
    const std::string& model) {
    
    ILLMClient* client = GetClient(defaultProvider);
    if (client) {
        return client->GenerateEmbeddings(texts, model);
    }
    return std::vector<std::vector<float>>();
}

void LLMManager::EnableFallback(bool enable) {
    fallbackEnabled = enable;
}

void LLMManager::SetFallbackProvider(LLMProvider provider) {
    fallbackProvider = provider;
}

void LLMManager::SetRateLimit(int requestsPerMinute) {
    rateLimit = requestsPerMinute;
}

void LLMManager::EnableRateLimiting(bool enable) {
    rateLimitingEnabled = enable;
}

void LLMManager::EnableCaching(bool enable) {
    cachingEnabled = enable;
}

void LLMManager::ClearCache() {
    cache.clear();
}

LLMManager::LLMStats LLMManager::GetStats() const {
    return stats;
}

void LLMManager::ResetStats() {
    memset(&stats, 0, sizeof(stats));
}

bool LLMManager::CheckRateLimit() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    // Remove old timestamps (older than 1 minute)
    requestTimestamps.erase(
        std::remove_if(requestTimestamps.begin(), requestTimestamps.end(),
            [now_ms](uint64_t ts) { return now_ms - ts > 60000; }),
        requestTimestamps.end());
    
    // Check if under limit
    if (requestTimestamps.size() >= static_cast<size_t>(rateLimit)) {
        return false;
    }
    
    requestTimestamps.push_back(now_ms);
    return true;
}

std::string LLMManager::GenerateCacheKey(const std::vector<ChatMessage>& messages, const LLMConfig& config) {
    std::stringstream key;
    key << config.model << "|";
    for (const auto& msg : messages) {
        key << static_cast<int>(msg.role) << ":" << msg.content << "|";
    }
    return key.str();
}

LLMResponse* LLMManager::GetFromCache(const std::string& key) {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    for (auto& entry : cache) {
        if (entry.key == key && (now_ms - entry.timestamp) < 3600000) {  // 1 hour TTL
            return &entry.response;
        }
    }
    return nullptr;
}

void LLMManager::AddToCache(const std::string& key, const LLMResponse& response) {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    CacheEntry entry;
    entry.key = key;
    entry.response = response;
    entry.timestamp = now_ms;
    
    cache.push_back(entry);
    
    // Limit cache size
    if (cache.size() > 1000) {
        cache.erase(cache.begin());
    }
}

// ============================================================================
// LLM Utilities Implementation
// ============================================================================

namespace LLMUtils {

std::vector<ChatMessage> FormatSystemPrompt(const std::string& systemPrompt) {
    std::vector<ChatMessage> messages;
    messages.emplace_back(MessageRole::System, systemPrompt);
    return messages;
}

std::vector<ChatMessage> AddUserMessage(const std::vector<ChatMessage>& messages, const std::string& content) {
    auto result = messages;
    result.emplace_back(MessageRole::User, content);
    return result;
}

std::vector<ChatMessage> AddAssistantMessage(const std::vector<ChatMessage>& messages, const std::string& content) {
    auto result = messages;
    result.emplace_back(MessageRole::Assistant, content);
    return result;
}

std::string FormatToolCall(const ToolCall& call) {
    std::stringstream ss;
    ss << "{\"id\":\"" << EscapeJson(call.id) << "\",\"name\":\"" << EscapeJson(call.name) << "\",\"arguments\":" << call.arguments << "}";
    return ss.str();
}

ToolCall ParseToolCall(const std::string& json) {
    ToolCall call;
    // Placeholder for JSON parsing
    return call;
}

std::string ExtractContent(const LLMResponse& response) {
    return response.content;
}

std::vector<ToolCall> ExtractToolCalls(const LLMResponse& response) {
    // Placeholder for tool call extraction
    return std::vector<ToolCall>();
}

LLMConfig CreateOpenAIConfig(const std::string& apiKey, const std::string& model) {
    LLMConfig config;
    config.provider = LLMProvider::OpenAI;
    config.apiKey = apiKey;
    config.model = model;
    return config;
}

LLMConfig CreateAnthropicConfig(const std::string& apiKey, const std::string& model) {
    LLMConfig config;
    config.provider = LLMProvider::Anthropic;
    config.apiKey = apiKey;
    config.model = model;
    return config;
}

LLMConfig CreateLocalConfig(const std::string& modelPath) {
    LLMConfig config;
    config.provider = LLMProvider::Local;
    config.model = modelPath;
    return config;
}

bool ValidateMessages(const std::vector<ChatMessage>& messages) {
    return !messages.empty();
}

bool ValidateToolDefinitions(const std::vector<ToolDefinition>& tools) {
    for (const auto& tool : tools) {
        if (tool.name.empty() || tool.description.empty()) {
            return false;
        }
    }
    return true;
}

} // namespace LLMUtils

} // namespace AI
} // namespace Potato
