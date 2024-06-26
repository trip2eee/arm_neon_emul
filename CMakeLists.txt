cmake_minimum_required(VERSION 3.5)

project(neon_emul CXX)

set(CMAKE_CXX_FLAGS "-g -march=armv8.2-a+fp16")

add_executable(ut_fp_operation
    unittests/ut_fp_operation.cpp
    neon_emul/fp16_operation.cpp
    neon_emul/fp32_operation.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY})