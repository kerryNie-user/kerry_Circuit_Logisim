#include "InteractionModule.hpp"
#include "Logger.hpp"
#include <iostream>
#include <filesystem>
#include <limits>

// ==========================
// Interaction Module Implementation
// ==========================

// 从指定目录查找所有 .asm 文件，返回完整路径列表
std::vector<std::string> findFileFromDirectory(const std::string& directory_path) {
    std::vector<std::string> files;
    if (!std::filesystem::exists(directory_path) || !std::filesystem::is_directory(directory_path)) {
        throw std::runtime_error("Directory not found: " + directory_path);
    } else {
        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            if (!entry.is_regular_file()) {
                continue;
            }

            if (entry.path().extension() == ".asm") {
                files.push_back(entry.path().string());
            }
        }
    }
    return files;
}

// 从完整路径中提取文件名
std::string extractFilename(const std::string& file_path) {
    return std::filesystem::path(file_path).filename().string();
}

// 交互式选择文件，返回选中的完整路径
std::string selectFile(const std::string& directory_path) {
    const std::vector<std::string> files = findFileFromDirectory(directory_path);   
    if (files.empty()) {
        throw std::runtime_error("No files found in directory: " + directory_path);
    }

    std::cout << "Total files: " << files.size() << std::endl;
    Logger::list(files);

    std::cout << "Waiting... Select file: ";
    int index;
    if (!(std::cin >> index)) {
        throw std::invalid_argument("Invalid input : Please input a number");
        return "";
    } else if (index < 0 || index >= static_cast<int>(files.size())) {
        throw std::out_of_range("Invalid input : Index out of range");
        return "";
    } else {
        return files[index];
    }
}