cmake_minimum_required(VERSION 3.15)
project(MyProject C)

add_executable(myapp src/main.c)

target_include_directories(myapp PRIVATE include)
