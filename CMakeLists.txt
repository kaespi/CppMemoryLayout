cmake_minimum_required(VERSION 3.0)
project(CppMemoryLayoutInvestigation)

set(TARGET "memlayout")
add_executable(${TARGET}
    memory_layout.cpp
    class_types.h
    address_helper.h
    type_helper.h)

set (CMAKE_CXX_STANDARD 11)

# note: we do some rather nasty cast from a function pointer to a void pointer. This is not
#       accepted usually. Therefore this needs to be suppressed, hence -Wno-pmf-conversion.
target_compile_options(${TARGET} PUBLIC -O0 -Wall -Wextra -Wno-pmf-conversions)
