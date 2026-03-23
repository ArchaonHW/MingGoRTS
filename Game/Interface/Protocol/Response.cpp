#include "Response.h"
#include <sstream>

namespace MingGoRTS {

Response::Response() : status_(ResponseStatus::Success) {
}

Response::Response(ResponseStatus status) : status_(status) {
}

Response::~Response() = default;

void Response::AddField(const std::string& name, const std::string& value) {
    fields_[name] = value;
}

void Response::AddField(const std::string& name, int value) {
    fields_[name] = std::to_string(value);
}

void Response::AddField(const std::string& name, float value) {
    fields_[name] = std::to_string(value);
}

void Response::AddField(const std::string& name, bool value) {
    fields_[name] = value ? "true" : "false";
}

std::string Response::GetField(const std::string& name) const {
    auto it = fields_.find(name);
    if (it != fields_.end()) {
        return it->second;
    }
    return "";
}

int Response::GetIntField(const std::string& name, int defaultValue) const {
    std::string value = GetField(name);
    if (value.empty()) return defaultValue;
    try {
        return std::stoi(value);
    } catch (...) {
        return defaultValue;
    }
}

float Response::GetFloatField(const std::string& name, float defaultValue) const {
    std::string value = GetField(name);
    if (value.empty()) return defaultValue;
    try {
        return std::stof(value);
    } catch (...) {
        return defaultValue;
    }
}

bool Response::GetBoolField(const std::string& name, bool defaultValue) const {
    std::string value = GetField(name);
    if (value.empty()) return defaultValue;
    return value == "true" || value == "1" || value == "yes";
}

bool Response::HasField(const std::string& name) const {
    return fields_.find(name) != fields_.end();
}

std::string Response::Serialize() const {
    std::stringstream ss;
    
    // 状态
    switch (status_) {
        case ResponseStatus::Success: ss << "SUCCESS"; break;
        case ResponseStatus::Error: ss << "ERROR"; break;
        case ResponseStatus::Pending: ss << "PENDING"; break;
        case ResponseStatus::Timeout: ss << "TIMEOUT"; break;
        case ResponseStatus::InvalidCommand: ss << "INVALID"; break;
        default: ss << "UNKNOWN"; break;
    }
    
    ss << "|";
    
    // 错误信息
    if (!errorMessage_.empty()) {
        ss << "error=" << errorMessage_ << "&";
    }
    
    // 主数据
    if (!data_.empty()) {
        ss << "data=" << data_ << "&";
    }
    
    // 其他字段
    for (const auto& [name, value] : fields_) {
        ss << name << "=" << value << "&";
    }
    
    return ss.str();
}

bool Response::Deserialize(const std::string& data) {
    size_t pos = data.find('|');
    if (pos == std::string::npos) {
        status_ = ResponseStatus::Error;
        return false;
    }
    
    std::string statusStr = data.substr(0, pos);
    if (statusStr == "SUCCESS") status_ = ResponseStatus::Success;
    else if (statusStr == "ERROR") status_ = ResponseStatus::Error;
    else if (statusStr == "PENDING") status_ = ResponseStatus::Pending;
    else if (statusStr == "TIMEOUT") status_ = ResponseStatus::Timeout;
    else if (statusStr == "INVALID") status_ = ResponseStatus::InvalidCommand;
    else status_ = ResponseStatus::Error;
    
    std::string params = data.substr(pos + 1);
    fields_.clear();
    
    size_t start = 0;
    while (start < params.length()) {
        size_t end = params.find('&', start);
        if (end == std::string::npos) end = params.length();
        
        std::string param = params.substr(start, end - start);
        size_t eqPos = param.find('=');
        
        if (eqPos != std::string::npos) {
            std::string name = param.substr(0, eqPos);
            std::string value = param.substr(eqPos + 1);
            
            if (name == "error") {
                errorMessage_ = value;
            } else if (name == "data") {
                data_ = value;
            } else {
                fields_[name] = value;
            }
        }
        
        start = end + 1;
    }
    
    return true;
}

Response Response::CreateSuccess(const std::string& data) {
    Response response(ResponseStatus::Success);
    response.SetData(data);
    return response;
}

Response Response::CreateError(const std::string& message) {
    Response response(ResponseStatus::Error);
    response.SetErrorMessage(message);
    return response;
}

} // namespace MingGoRTS
