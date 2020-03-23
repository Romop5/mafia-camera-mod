cmake_minimum_required(VERSION 3.11)
project(Angelo)

add_executable(angelo
    src/dllmain.cpp
)
target_link_libraries(angelo AngeloFramework)