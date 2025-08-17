#ifndef INTERACTION_MODULE_HPP
#define INTERACTION_MODULE_HPP

#include <string>
#include <vector>

// ==========================
// Interaction Module Interface
// ==========================

// 从指定目录查找所有匹配文件（例如 *.asm），返回完整路径列表
std::vector<std::string> findFileFromDirectory(const std::string& directory_path);

// 从完整路径中提取文件名（去掉路径）
std::string extractFilename(const std::string& file_path);

// 交互式选择文件，返回选中的完整路径
std::string selectFile(const std::string& directory_path);

// ==========================

#endif // INTERACTION_MODULE_HPP