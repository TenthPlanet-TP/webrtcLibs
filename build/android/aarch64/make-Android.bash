#!/bin/bash

BUILD_TYPE="Release"
ANDROID_ABI="arm64-v8a"

#Specify Android NDK path if needed
ANDROID_NDK=~/work/tools/android-ndk-r21d

#Specify cmake if needed
CMAKE_PROGRAM=cmake

source ../env_setup.sh

echo "--------------option------------------"
echo TOOLCHAIN_FILE=${TOOLCHAIN_FILE}
echo BUILD_TYPE=${BUILD_TYPE}
echo PLATFORM=${PLATFORM}
echo ANDROID_ABI=${ANDROID_ABI}
echo TOOLCHAIN_NAME=${TOOLCHAIN_NAME}
echo NATIVE_API_LEVEL=${NATIVE_API_LEVEL}
echo "--------------end------------------"

${CMAKE_PROGRAM} -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE}                   \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE}                                      \
      -DCMAKE_MAKE_PROGRAM=${MAKE_PROGRAM}                                  \
      -DANDROID_FORCE_ARM_BUILD=ON                                          \
      -DANDROID_NDK=${ANDROID_NDK}                                          \
      -DANDROID_SYSROOT=${PLATFORM}                                         \
      -DANDROID_ABI=${ANDROID_ABI}                                          \
      -DANDROID_TOOLCHAIN_NAME=${TOOLCHAIN_NAME}                            \
      -DANDROID_NATIVE_API_LEVEL=${NATIVE_API_LEVEL}                        \
      -DANDROID_STL=c++_static                                              \
      -DCMAKE_INSTALL_PREFIX=install                                        \
      ../../../

if [ "${CMAKE_PARALLEL_ENABLE}" = "0" ]; then
    ${CMAKE_PROGRAM} --build .
else
    ${CMAKE_PROGRAM} --build . -j20
fi
