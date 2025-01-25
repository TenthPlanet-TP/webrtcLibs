# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 添加子目录名称作为项目名称
project(webrtcLibsJni)

# 指定 C++ 标准（根据需要选择合适的版本）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含根目录的头文件路径（如果需要）
# include_directories(${CMAKE_SOURCE_DIR}/include)

# 包含当前目录的头文件路径（如果有）
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 移除调试符号
# target_compile_options(webrtcLibsJni PRIVATE "$<$<CXX_COMPILER_ID:GNU,Clang>:-s>")
# target_link_options(webrtcLibsJni PRIVATE "$<$<CXX_COMPILER_ID:GNU,Clang>:-s>")

# 移除调试符号
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")

add_compile_options(
    -DWEBRTC_ANDROID
    -DWEBRTC_POSIX
    -DWEBRTC_LINUX
    )

# 添加测试目标
add_library(webrtcLibsJni SHARED webrtcLibsJni.cpp)

# 链接到根目录生成的动态库 webrtcLibs
# 假设 webrtcLibs 是在根目录的 CMakeLists.txt 中生成的
target_link_libraries(webrtcLibsJni webrtcLibs)


