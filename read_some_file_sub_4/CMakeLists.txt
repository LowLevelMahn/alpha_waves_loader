#cmake -G "Visual Studio 15 2017" -A Win32
#cmake -G "Visual Studio 16 2019" -A Win32
#cmake -G "Visual Studio 17 2022" -A Win32

cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 14)

project(read_some_file_sub_4)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-operator-names" ) # emu used "and" "or" "not" names

add_executable(read_some_file_sub_4 
  "emu.hpp"
  "emu.cpp"
  "helper.hpp"
  "helper.cpp"
  "types.hpp"
  "port.cpp"
  "original_port.cpp"
  "cleanup_port.cpp"
  "main.cpp"
  "protection_tests.cpp"
  "example.cpp"
  "cc1_reader.cpp"
)  
