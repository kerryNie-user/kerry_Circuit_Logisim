cmake_minimum_required(VERSION 3.10)
project(Logisim CXX)

# 编译标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 源文件
aux_source_directory(src SRC_LIST)

# 添加 include 目录（项目自己的）
include_directories(include)

# 创建可执行文件
add_executable(Logisim ${SRC_LIST})
