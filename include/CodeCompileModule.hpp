#ifndef CODE_COMPILE_MODULE_HPP
#define CODE_COMPILE_MODULE_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <regex>

// 寄存器组大小
#define REGISTER_GROUP_SIZE 8

// ==========================
// 机器码结构体
// ==========================
struct machinecode {
    bool LU;                                            // 0     : LU, 1 -> And, 0 -> Or
    bool AU;                                            // 1     : AU, 1 -> Add, 0 -> Sub
    bool MUX;                                           // 2     : MUX, 1 -> AU, 0 -> LU
    int ALU_in_register;                                // 21-25 : ALU Input from Register Group
    int ALU_in_immediate;                               // 6-13  : ALU Immediate
    bool RAM;                                           // 29    : RAM, 1 -> Write, 0 -> Read
    int Data_of_RAM_or_Write_to_Register_Group_Address; // 16-20 : reading RAM -> RAM address ; writing -> Register group address
    bool enable_write_to_register;                      // 31    : 1 -> RAM, 0 -> ALU
    bool write_to_register;                             // 30    : 1 -> Yes, 0 -> No
    bool jump;                                          // 28    : 1 -> Yes, 0 -> No
    int jump_address;                                  // 0-7   : Jump to code address
    machinecode() 
        : LU(0), AU(0), MUX(0), ALU_in_register(0), ALU_in_immediate(0), RAM(0),
          Data_of_RAM_or_Write_to_Register_Group_Address(0), enable_write_to_register(0),
          write_to_register(0), jump(0), jump_address(0) {}
};

// ==========================
// CodeCompiler 类声明
// ==========================
class CodeCompiler {
private:
    std::string uncompiled_code_;  // 原始指令字符串
    machinecode compiling_code_;    // 当前解析的指令对应的机器码结构体
    uint32_t compiled_code_ = 0;   // 最终生成的 32 位机器码
public:
    explicit CodeCompiler(const std::string& uncompiled_code);
    ~CodeCompiler() = default;

    // 编译接口：解析字符串并生成机器码
    uint32_t compile();
private:
    // 数字提取辅助函数
    std::vector<int> extractNumbers(const std::smatch& match);

    // 指令解析
    void compile_encode_();

    // 编码生成
    void compile_decode_();

    // 各条指令对应的机器码填充
    void storeWord(int num1, int num2, int num3);
    void loadWord(int num1, int num2, int num3);
    void addI(int num1, int num2, int num3);
    void subI(int num1, int num2, int num3);
    void moveI(int num1, int num2);
    void jump(int num1);
};

#endif // CODE_COMPILE_MODULE_HPP