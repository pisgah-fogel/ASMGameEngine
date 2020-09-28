cmake_minimum_required(VERSION 2.8.4)

project(CVersion)

IF (WIN32)
    set(SDL2_INCLUDE_DIR C:/SDL/SDL2-2.0.3/include/SDL2)
    set(SDL2_LIB_DIR C:/SDL/SDL2-2.0.3/lib/x64)
    include_directories(${SDL2_INCLUDE_DIR})
    link_directories(${SDL2_LIB_DIR})
    message(AUTHOR_WARNING "Debug: You are running under Windows")
ELSE()
  # set stuff for other systems
ENDIF()

add_compile_definitions(ERROR_HANDLING)

set(SOURCE_FILES main.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(EMSCRIPTEN)
    set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES 
        OUTPUT_NAME "${PROJECT_NAME}"
        SUFFIX ".html"
    )
endif()


if(EMSCRIPTEN)
    set(EMSCRIPTEN_VERSION 1.0)
    set(EMSDK_DIR ../emsdk)
    set(CC "${EMSDK_DIR}/upstream/emscripten/emcc")
    set(CXX "${EMSDK_DIR}/upstream/emscripten/em++")
    include("${EMSDK_DIR}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
	#set_target_properties(client ${PROJECT_NAME} LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --preload-file assets --bind")
    #set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Os -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s USE_GLFW=3 -s TOTAL_MEMORY=16777216 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 --preload-file resources -s ASSERTIONS=1 --profiling --shell-file $(RAYLIB_PATH)/src/shell.html")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' --preload-file ../assets")
	#target_link_libraries(*.bc)
	#link_directories("../emsdk/upstream/emscripten/cache/wasm/")
	#target_link_libraries(${PROJECT_NAME} PUBLIC gl)
    message(AUTHOR_WARNING "Debug: Target properties is set to match emscripten requirements")
else()
    target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_image chipmunk)
endif()

