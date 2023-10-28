cmake_minimum_required(VERSION 3.5)

project(cmd-ls-l)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/auxf.c ${PROJECT_SOURCE_DIR}/src/main.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
