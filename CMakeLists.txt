cmake_minimum_required(VERSION 3.5)

project(JudgeCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(JudgeCore main.cpp JudgeCore.cpp Parser.cpp Compiler.cpp Runner.cpp SyscallChecker.cpp SyscallChecker.h Runner.h Utils.h Compiler.h JudgeCore.h Parser.h)
