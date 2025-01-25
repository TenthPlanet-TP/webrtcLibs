# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 添加子目录名称作为项目名称
project(testdemo)

# 指定 C++ 标准（根据需要选择合适的版本）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含根目录的头文件路径（如果需要）
# include_directories(${CMAKE_SOURCE_DIR}/include)

# 包含当前目录的头文件路径（如果有）
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options(
    -DWEBRTC_ANDROID
    -DWEBRTC_POSIX
    -DWEBRTC_LINUX
    )

# 添加测试目标
add_executable(h264_bitstream_parser_unittest h264_bitstream_parser_unittest.cc)
add_executable(pps_parser_unittest pps_parser_unittest.cc)
add_executable(sps_parser_unittest sps_parser_unittest.cc)
add_executable(sps_vui_rewriter_unittest sps_vui_rewriter_unittest.cc)

# 链接到根目录生成的动态库 webrtcLibs
# 假设 webrtcLibs 是在根目录的 CMakeLists.txt 中生成的
target_link_libraries(h264_bitstream_parser_unittest webrtcLibs)
target_link_libraries(pps_parser_unittest webrtcLibs)
target_link_libraries(sps_parser_unittest webrtcLibs)
target_link_libraries(sps_vui_rewriter_unittest webrtcLibs)

