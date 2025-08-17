#ifndef CODE_ORGANIZATION_MODULE_HPP
#define CODE_ORGANIZATION_MODULE_HPP

#include "Logger.hpp"

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <unordered_map>
#include <optional>
#include <sstream>
#include <iomanip>

// ==========================
// 工具函数声明
// ==========================
std::string toHex(uint32_t value);
std::string removeSpace(const std::string& line);

// ==========================
// 标签表类
// ==========================
class LabelTable {
private:
    std::unordered_map<std::string, uint32_t> table_;

public:
    // 添加标签
    void addLabel(const std::string& name, uint32_t address) {
        if (table_.find(name) != table_.end()) {
            throw std::runtime_error("Duplicate label: " + name);
        }
        table_[name] = address;
    }

    // 查询标签地址
    std::optional<uint32_t> getAddress(const std::string& name) const {
        auto it = table_.find(name);
        if (it != table_.end()) {
            return it->second;
        } else {
            return std::nullopt;
        }
    }

    // 判断标签是否存在
    bool hasLabel(const std::string& name) const {
        return table_.find(name) != table_.end();
    }

    // 获取标签数量
    size_t size() const {
        return table_.size();
    }

    // 调试打印所有标签
    void debugPrint() const {
        for (const auto& [name, addr] : table_) {
            Logger::debug(name + " -> 0x" + toHex(addr));
        }
    }
};

// ==========================
// Code Organization Module 接口
// ==========================

// 从文件中提取未编译代码，每行作为一个字符串存入 vector
std::vector<std::string> extractUncompiledCodeFromFile(const std::string& file_path);

// 预处理代码：收集标签、替换跳转指令
std::vector<std::string> preprocessCode(std::vector<std::string>& uncompiled_code);

// 将编译后的二进制代码写入指定文件
void buildCompiledFile(const std::vector<uint32_t>& compiled_code, const std::filesystem::path& file_path);

// 编译整个文件，并写入 output 文件夹
std::vector<uint32_t> compileCode(const std::vector<std::string>& uncompiled_code, const std::string& file_name);

#endif // CODE_ORGANIZATION_MODULE_HPP