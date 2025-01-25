# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

# vim: syntax=cmake
if(NOT CMAKE_BUILD_TYPE)
    # default to Release build for GCC builds
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()
message(STATUS "cmake version ${CMAKE_VERSION}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")


cmake_minimum_required(VERSION 3.10)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCXXCompilerFlag)

# Declares and names the project.

project(webrtc_libs LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        webrtcLibs

        # Sets the library as a shared library.
        # SHARED
        STATIC

        # Provides a relative path to your source file(s).
        # main_jni.cpp
	api/video/color_space.cc
	api/video/hdr_metadata.cc
	common_video/h264/h264_bitstream_parser.cc
	common_video/h264/h264_common.cc
	common_video/h264/pps_parser.cc
	common_video/h264/sps_parser.cc
	common_video/h264/sps_vui_rewriter.cc
        common_video/h265/h265_bitstream_parser.cc
        common_video/h265/h265_common.cc
        common_video/h265/h265_inline.cc
        common_video/h265/h265_pps_parser.cc
        common_video/h265/h265_sps_parser.cc
        common_video/h265/h265_vps_parser.cc
	rtc_base/bit_buffer.cc
	rtc_base/bitstream_reader.cc
	rtc_base/zero_memory.cc
	rtc_base/checks.cc
	rtc_base/logging.cc
	rtc_base/time_utils.cc
	rtc_base/platform_thread_types.cc
	rtc_base/system_time.cc
	rtc_base/string_encode.cc
	rtc_base/string_utils.cc
	rtc_base/string_to_number.cc
	rtc_base/strings/string_builder.cc
	system_wrappers/source/metrics.cc
        )


include_directories(
        .
        third_party/abseil-cpp/
    )

# add_library(
#         absl
#         STATIC
#         IMPORTED )

# set_target_properties(
#         absl
#         PROPERTIES 
#         IMPORTED_LOCATION
#         ${CMAKE_SOURCE_DIR}/third_party/abseil-cpp/bulid/android/aarch64/install/libabsl.a
#     )

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pipe -fPIC -Wall -fmessage-length=0 -std=gnu++11")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# 移除调试符号
target_compile_options(webrtcLibs PRIVATE "$<$<CXX_COMPILER_ID:GNU,Clang>:-s>")
target_link_options(webrtcLibs PRIVATE "$<$<CXX_COMPILER_ID:GNU,Clang>:-s>")

target_compile_options(webrtcLibs PRIVATE
    -DWEBRTC_ANDROID
    -DWEBRTC_POSIX
    -DWEBRTC_LINUX
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                        webrtcLibs
                        # stdc++
                        # android
                        # z
                        # absl
                        # Links the target library to the log library
                        # included in the NDK.
                        ${log-lib}
                        )

#----------------------------

# add third_party
add_subdirectory(third_party/abseil-cpp)

#----------------------------
# add testdemo
add_subdirectory(testdemo)

add_subdirectory(jni)


# ----------------------------------------------------------------------------
#  install headers
# ----------------------------------------------------------------------------

# 配置安装规则
# 安装动态库到 release/lib 目录
install(TARGETS webrtcLibs
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

# 安装头文件到 release/include 目录，并保持目录结构
install(DIRECTORY api common_video rtc_base system_wrappers third_party
        DESTINATION include
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.hxx")

# 如果需要安装 JNI 相关文件，可以单独处理
install(DIRECTORY jni
        DESTINATION include
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.java")
