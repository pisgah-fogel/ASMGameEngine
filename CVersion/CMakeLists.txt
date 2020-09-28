cmake_minimum_required(VERSION 2.8.4)

project(CVersion)

set(SDL2_INCLUDE_DIR C:/SDL/SDL2-2.0.3/include/SDL2)
set(SDL2_LIB_DIR C:/SDL/SDL2-2.0.3/lib/x64)
add_compile_definitions(ERROR_HANDLING)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

set(SOURCE_FILES main.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_image chipmunk)