#cmake -G "Visual Studio 15 2017" -A Win32
#cmake -G "Visual Studio 16 2019" -A Win32
#cmake -G "Visual Studio 17 2022" -A Win32
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(uncompress_cc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(uncompress_cc
  "uncompress_cc.cpp"
)  
