#cmake -G "Visual Studio 15 2017" -A Win32
#cmake -G "Visual Studio 16 2019" -A Win32
#cmake -G "Visual Studio 17 2022" -A Win32

cmake_minimum_required(VERSION 2.20)

project(read_some_file_sub_4)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(read_some_file_sub_4 
  "emu.hpp"
  "emu.cpp"
  "helper.hpp"
  "helper.cpp"
  "types.hpp"
  "port.cpp"
  "main.cpp"
)  

