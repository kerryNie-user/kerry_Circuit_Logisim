#include "CodeOrganizationModule.hpp"
#include "InteractionModule.hpp"
#include "Logger.hpp"
#include <iostream>

const std::string RES_DIRECTORY_PATH = "res";

int main() {
    // Logger::setLevel(LogLevel::DEBUG);
    Logger::info("Compiler has been started");

    std::string file_path;
    std::string file_name;
    try {
        file_path = selectFile(RES_DIRECTORY_PATH);
        file_name = extractFilename(file_path);
    } catch (const std::exception& e) {
        Logger::error("Select file failed : " + std::string(e.what()));
        return 1;
    }
    
    Logger::info("Start compile: " + file_name);

    std::vector<std::string> uncompiled_code;
    std::vector<std::string> preprocessed_code;
    std::vector<uint32_t> compiled_code;
    try {
        uncompiled_code = extractUncompiledCodeFromFile(file_path);
        preprocessed_code = preprocessCode(uncompiled_code);
        compiled_code = compileCode(preprocessed_code, file_name);
    } catch (const std::exception& e) {
        Logger::error("Compile failed : " + std::string(e.what()));
        return 1;
    }

    Logger::info(file_name + " has been compiled successfully");
    return 0;
}