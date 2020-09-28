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
    set(EMSCRIPTEN_VERSION 2.0.4)
    set(EMSDK_DIR ../emsdk)
    set(EMSDK_CC "${CMAKE_CURRENT_SOURCE_DIR}/${EMSDK_DIR}/upstream/emscripten/emcc")
    get_filename_component(_fullpath "${CMAKE_CURRENT_SOURCE_DIR}/${EMSDK_DIR}/emsdk" REALPATH)
    if (NOT EXISTS "${_fullpath}")
        message(FATAL_ERROR "EMSDK directory is empty, please use 'git submodule update --init --recursive'")
    endif()

    get_filename_component(_fullpath2 "${EMSDK_CC}" REALPATH)
    if (NOT EXISTS "${_fullpath2}")
        message(WARNING "EMSDK is not compiled yet")
        execute_process(
            COMMAND ./emsdk install latest
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${EMSDK_DIR}
        )
        execute_process(
            COMMAND ./emsdk activate latest
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${EMSDK_DIR}
            )
    endif()

    set(CMAKE_C_COMPILER "${EMSDK_CC}")
    set(CMAKE_CXX_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/${EMSDK_DIR}/upstream/emscripten/em++")

    include("${CMAKE_CURRENT_SOURCE_DIR}/${EMSDK_DIR}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
	#set_target_properties(client ${PROJECT_NAME} LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --preload-file assets --bind")
    #set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Os -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s USE_GLFW=3 -s TOTAL_MEMORY=16777216 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 --preload-file resources -s ASSERTIONS=1 --profiling --shell-file $(RAYLIB_PATH)/src/shell.html")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' --preload-file ../assets")
	#target_link_libraries(*.bc)
	#link_directories("../emsdk/upstream/emscripten/cache/wasm/")
	#target_link_libraries(${PROJECT_NAME} PUBLIC gl)
    message(AUTHOR_WARNING "Debug: Target properties is set to match emscripten requirements")

    get_target_property(TMPNAME ${PROJECT_NAME} OUTPUT_NAME )
    get_target_property(TMPSUFFIX ${PROJECT_NAME} SUFFIX )

    add_custom_target(server)
    add_custom_command(TARGET server
        POST_BUILD
        COMMAND echo "Visit http://localhost:8080/${TMPNAME}${TMPSUFFIX}"
        COMMAND python3 -m http.server 8080
        COMMENT "Run a python3 web server"
    )

else()
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-g")
    target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_image chipmunk)
endif()

