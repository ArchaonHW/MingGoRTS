#pragma once

#include <string>
#include <vector>
#include <map>

namespace MingGoRTS {

// 响应状态
enum class ResponseStatus {
    Success,
    Error,
    Pending,
    Timeout,
    InvalidCommand
};

// 响应 - 后端向前端返回的结果
class Response {
public:
    Response();
    explicit Response(ResponseStatus status);
    ~Response();
    
    // 状态
    ResponseStatus GetStatus() const { return status_; }
    void SetStatus(ResponseStatus status) { status_ = status; }
    
    bool IsSuccess() const { return status_ == ResponseStatus::Success; }
    bool IsError() const { return status_ == ResponseStatus::Error; }
    
    // 数据
    void SetData(const std::string& data) { data_ = data; }
    const std::string& GetData() const { return data_; }
    
    // 错误信息
    void SetErrorMessage(const std::string& message) { errorMessage_ = message; }
    const std::string& GetErrorMessage() const { return errorMessage_; }
    
    // 辅助方法
    void AddField(const std::string& name, const std::string& value);
    void AddField(const std::string& name, int value);
    void AddField(const std::string& name, float value);
    void AddField(const std::string& name, bool value);
    
    std::string GetField(const std::string& name) const;
    int GetIntField(const std::string& name, int defaultValue = 0) const;
    float GetFloatField(const std::string& name, float defaultValue = 0.0f) const;
    bool GetBoolField(const std::string& name, bool defaultValue = false) const;
    bool HasField(const std::string& name) const;
    
    // 序列化
    std::string Serialize() const;
    bool Deserialize(const std::string& data);
    
    // 快速创建成功/失败响应
    static Response CreateSuccess(const std::string& data = "");
    static Response CreateError(const std::string& message);

private:
    ResponseStatus status_;
    std::string data_;
    std::string errorMessage_;
    std::map<std::string, std::string> fields_;
};

} // namespace MingGoRTS
