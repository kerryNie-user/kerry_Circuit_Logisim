#include "CodeCompileModule.hpp"
#include "Logger.hpp"
#include <regex>
#include <stdexcept>
#include <iostream>

// ==========================
// CodeCompiler 类实现
// ==========================

// 构造函数
CodeCompiler::CodeCompiler(const std::string& uncompiled_code)
    : uncompiled_code_(uncompiled_code) {}

// compile 接口
uint32_t CodeCompiler::compile() {
    compile_encode_();
    compile_decode_();
    return compiled_code_;
}

// extractNumbers
std::vector<int> CodeCompiler::extractNumbers(const std::smatch& match) {
    std::vector<int> result;
    for (size_t i = 1; i < match.size(); ++i) {
        if (match[i].matched) {
            result.push_back(std::stoi(match[i].str()));
        }
    }
    return result;
}

// compile_encode_
void CodeCompiler::compile_encode_() {
    std::regex pattern_SW(R"(SW R(\d+),\s*(\d+)\(R(\d+)\))", std::regex_constants::icase);
    std::regex pattern_LW(R"(LW R(\d+),\s*(\d+)\(R(\d+)\))", std::regex_constants::icase);
    std::regex pattern_ADDI(R"(ADDI R(\d+),\s*R(\d+),\s*(\d+))", std::regex_constants::icase);
    std::regex pattern_SUBI(R"(SUBI R(\d+),\s*R(\d+),\s*(\d+))", std::regex_constants::icase);
    std::regex pattern_MOVEI(R"(MOVI R(\d+),\s*(\d+))", std::regex_constants::icase);
    std::regex pattern_JUMP_NUM(R"(JUMP (\d+))", std::regex_constants::icase);

    std::smatch match;
    if (std::regex_match(uncompiled_code_, match, pattern_SW)) {
        auto nums = extractNumbers(match);
        storeWord(nums[0], nums[1], nums[2]);
    } else if (std::regex_match(uncompiled_code_, match, pattern_LW)) {
        auto nums = extractNumbers(match);
        loadWord(nums[0], nums[1], nums[2]);
    } else if (std::regex_match(uncompiled_code_, match, pattern_ADDI)) {
        auto nums = extractNumbers(match);
        addI(nums[0], nums[1], nums[2]);
    } else if (std::regex_match(uncompiled_code_, match, pattern_SUBI)) {
        auto nums = extractNumbers(match);
        subI(nums[0], nums[1], nums[2]);
    } else if (std::regex_match(uncompiled_code_, match, pattern_MOVEI)) {
        auto nums = extractNumbers(match);
        moveI(nums[0], nums[1]);
    } else if (std::regex_match(uncompiled_code_, match, pattern_JUMP_NUM)) {
        auto nums = extractNumbers(match);
        jump(nums[0]);
    } else {
        throw std::runtime_error(
            "Invalid instruction: " + uncompiled_code_ +
            " (Expected: SW/LW/ADDI/SUBI/MOVI)"
        );
    }
}

// compile_decode_
void CodeCompiler::compile_decode_() {
    compiled_code_ = 0;

    if (compiling_code_.ALU_in_immediate > 0xFF)
        throw std::runtime_error("Invalid ALU_in_immediate: " + uncompiled_code_);
    if (compiling_code_.Data_of_RAM_or_Write_to_Register_Group_Address > 0x1F)
        throw std::runtime_error("Invalid RAM/Register address: " + uncompiled_code_);
    if (compiling_code_.ALU_in_register > 0x1F)
        throw std::runtime_error("Invalid ALU_in_register: " + uncompiled_code_);
    if (compiling_code_.jump) {
        compiled_code_ |= (compiling_code_.jump & 0x1) << 28;
        compiled_code_ |= (compiling_code_.jump_address & 0xFF) << 0;
    } else {
        compiled_code_ |= (compiling_code_.LU & 0x1) << 0;
        compiled_code_ |= (compiling_code_.AU & 0x1) << 1;
        compiled_code_ |= (compiling_code_.MUX & 0x1) << 2;
        compiled_code_ |= (compiling_code_.ALU_in_immediate & 0xFF) << 6;
        compiled_code_ |= (compiling_code_.Data_of_RAM_or_Write_to_Register_Group_Address & 0x1F) << 16;
        compiled_code_ |= (compiling_code_.ALU_in_register & 0x1F) << 21;
        compiled_code_ |= (compiling_code_.RAM & 0x1) << 29;
        compiled_code_ |= (compiling_code_.write_to_register & 0x1) << 30;
        compiled_code_ |= (compiling_code_.enable_write_to_register & 0x1) << 31;
    }
}

// 指令机器码填充函数（附调试日志）
void CodeCompiler::storeWord(int num1, int num2, int num3) {
    compiling_code_ = {};
    compiling_code_.LU = 0;
    compiling_code_.AU = 1;
    compiling_code_.MUX = 1;
    compiling_code_.ALU_in_register = num3;
    compiling_code_.ALU_in_immediate = num2;
    compiling_code_.Data_of_RAM_or_Write_to_Register_Group_Address = num1;
    compiling_code_.RAM = 1;
    compiling_code_.enable_write_to_register = 0;
    compiling_code_.write_to_register = 0;

    Logger::debug("SW R" + std::to_string(num1) + ",(" + std::to_string(num2) + ")R" + std::to_string(num3));
}

void CodeCompiler::loadWord(int num1, int num2, int num3) {
    compiling_code_ = {};
    compiling_code_.LU = 0;
    compiling_code_.AU = 1;
    compiling_code_.MUX = 1;
    compiling_code_.ALU_in_register = num3;
    compiling_code_.ALU_in_immediate = num2;
    compiling_code_.Data_of_RAM_or_Write_to_Register_Group_Address = num1;
    compiling_code_.RAM = 0;
    compiling_code_.enable_write_to_register = 1;
    compiling_code_.write_to_register = 1;

    Logger::debug("LW R" + std::to_string(num1) + ",(" + std::to_string(num2) + ")R" + std::to_string(num3));
}

void CodeCompiler::addI(int num1, int num2, int num3) {
    compiling_code_ = {};
    compiling_code_.LU = 0;
    compiling_code_.AU = 1;
    compiling_code_.MUX = 1;
    compiling_code_.ALU_in_register = num2;
    compiling_code_.ALU_in_immediate = num3;
    compiling_code_.Data_of_RAM_or_Write_to_Register_Group_Address = num1;
    compiling_code_.RAM = 0;
    compiling_code_.enable_write_to_register = 1;
    compiling_code_.write_to_register = 0;

    Logger::debug("ADDI R" + std::to_string(num1) + ",R" + std::to_string(num2) + "," + std::to_string(num3));
}

void CodeCompiler::subI(int num1, int num2, int num3) {
    compiling_code_ = {};
    compiling_code_.LU = 0;
    compiling_code_.AU = 0;
    compiling_code_.MUX = 1;
    compiling_code_.ALU_in_register = num2;
    compiling_code_.ALU_in_immediate = num3;
    compiling_code_.Data_of_RAM_or_Write_to_Register_Group_Address = num1;
    compiling_code_.RAM = 0;
    compiling_code_.enable_write_to_register = 1;
    compiling_code_.write_to_register = 0;

    Logger::debug("SUBI R" + std::to_string(num1) + ",R" + std::to_string(num2) + "," + std::to_string(num3));
}

void CodeCompiler::moveI(int num1, int num2) {
    addI(num1, REGISTER_GROUP_SIZE - 1, num2);
}

void CodeCompiler::jump(int num1) {
    compiling_code_ = {};
    compiling_code_.jump = 1;
    compiling_code_.jump_address = num1;
    Logger::debug("JUMP " + std::to_string(num1));
}
