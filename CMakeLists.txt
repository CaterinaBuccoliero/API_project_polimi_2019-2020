cmake_minimum_required(VERSION 3.17)
project(ProjectAPI C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjectAPI main.c commands.c commands.h definitions.h reading.c reading.h data_structure.c data_structure.h)