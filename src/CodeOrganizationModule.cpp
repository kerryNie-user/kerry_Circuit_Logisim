#include "CodeOrganizationModule.hpp"
#include "CodeCompileModule.hpp"
#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>

// ==========================
// Code Organization Module Implementation
// ==========================

#include <sstream>
#include <iomanip>
#include <string>

// 将 uint32_t 转换为 8 位十六进制字符串
std::string toHex(uint32_t value) {
    std::ostringstream oss;
    oss << std::hex << std::setw(8) << std::setfill('0') << value;
    return oss.str();
}

// 去除首位空格
std::string removeSpace(const std::string& line) {
    auto start = std::find_if_not(line.begin(), line.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    auto end = std::find_if_not(line.rbegin(), line.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();

    if (start >= end) {
        return "";
    }
    return std::string(start, end);
}

// 从文件中提取未编译代码，每行作为一个字符串存入 vector
std::vector<std::string> extractUncompiledCodeFromFile(const std::string& file_path) {
    std::vector<std::string> uncompiled_code;
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }
    std::string line;
    while (std::getline(file, line)) {
        // 行内注释处理：截断 '#' 或 "//"
        size_t posHash = line.find('#');
        size_t posSlash = line.find("//");

        if (posHash != std::string::npos || posSlash != std::string::npos) {
            size_t pos = (posHash == std::string::npos) ? posSlash :
                        (posSlash == std::string::npos) ? posHash :
                        std::min(posHash, posSlash);
            line = line.substr(0, pos);
        }

        line = removeSpace(line);
        if (!line.empty()) {
            uncompiled_code.push_back(line);
        }
    }
    file.close();

    Logger::info("File " + std::filesystem::path(file_path).filename().string() + " has been read successfully (" + std::to_string(uncompiled_code.size()) + " lines)");
    if (uncompiled_code.empty()) {
        throw std::runtime_error("File is empty.");
    }

    Logger::debug("Uncompiled code:");
    Logger::list(uncompiled_code);

    return uncompiled_code;
}

// 预处理代码，收集标签并替换跳转指令
std::vector<std::string> preprocessCode(std::vector<std::string>& uncompiled_code) {
    LabelTable label_table;
    // 第一遍：收集标签
    size_t address = 0;  // 指令地址
    for (size_t i = 0; i < uncompiled_code.size(); ++i) {
        std::string& line = uncompiled_code[i];

        // 检查是否是标签行
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string label = line.substr(0, pos);
            label = removeSpace(label);
            label_table.addLabel(label, static_cast<uint32_t>(address));

            // 标签行不计入编译指令
            uncompiled_code.erase(uncompiled_code.begin() + i);
            i--; 
        } else {
            // 普通指令计入地址
            address++;
        }
    }

    // 第二遍：替换 jump 标签为数字地址
    for (auto& line : uncompiled_code) {
        if (line.size() < 5) {
            continue; // jump 最短长度是 5
        }

        // 提取开头 5 个字符并转小写
        std::string prefix = line.substr(0, 5);
        std::transform(prefix.begin(), prefix.end(), prefix.begin(), [](unsigned char c){ return std::tolower(c); });

        if (prefix == "jump ") {
            std::string label = removeSpace(line.substr(5));
            if (auto addr = label_table.getAddress(label)) {
                line = "JUMP " + std::to_string(*addr);  // 替换成数字形式
            } else {
                throw std::runtime_error("Undefined label: " + label);
            }
        }
    }
    Logger::debug("Preprocessed code :");
    Logger::list(uncompiled_code);
    return uncompiled_code;
}

// 将编译后的二进制代码写入指定文件
void buildCompiledFile(const std::vector<uint32_t>& compiled_code, const std::filesystem::path& file_name) {
    std::filesystem::path compiled_dir = std::filesystem::current_path() / "output";
    if (!std::filesystem::exists(compiled_dir)) {
        std::filesystem::create_directories(compiled_dir);
    }

    std::string base_name = std::filesystem::path(file_name).stem().string();
    std::filesystem::path file_path = compiled_dir / (base_name + ".txt");
    
    std::ofstream outFile(file_path);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open or build file: " + file_path.string());
    }

    // Logisim 识别标志
    outFile <<"v2.0 raw" << std::endl;
    Logger::debug("Compiled code:");
    for (uint32_t code : compiled_code) {
        Logger::debug(toHex(code));
        outFile << toHex(code) << " ";
    }

    outFile.close();
    Logger::info("[INFO] Compiled code written to " + file_path.string());
}

// 编译整个文件的代码，并写入 output 文件夹
std::vector<uint32_t> compileCode(const std::vector<std::string>& uncompiled_code, const std::string& file_name) {
    std::vector<uint32_t> compiled_code;
    Logger::debug("Executed code (" + std::to_string(uncompiled_code.size()) + " lines)");
    for (size_t i = 0; i < uncompiled_code.size(); ++i) {
        try {
            CodeCompiler compiler(uncompiled_code[i]);
            uint32_t binary_code = compiler.compile();
            compiled_code.push_back(binary_code);
        } catch (const std::exception& e) {
            throw std::runtime_error("Compile interrupted at line " + std::to_string(i + 1) + ": " + uncompiled_code[i]);
        }
        Logger::logTheProgress(static_cast<int>(i), static_cast<int>(uncompiled_code.size()), "Compiling ... " + file_name, "Compiled successfully: " + file_name);
    }

    buildCompiledFile(compiled_code, file_name);
    return compiled_code;
}