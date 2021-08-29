#------------------------------
# GLFW
# Desktop Window Library
#------------------------------
if(NOT TARGET glfw AND NOT PLATFORM_ANDROID)
    set(GLFW_INSTALL OFF)
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(BUILD_SHARED_LIBS OFF)
    set(GLFW_USE_HYBRID_HPG ON)
    add_subdirectory(glfw)
    set_property(TARGET glfw PROPERTY FOLDER "GameLib")
endif()

#------------------------------
# GLM
# Game mathematics library
#------------------------------
if(NOT TARGET glm)
    add_subdirectory(glm)
    set_property(TARGET glm PROPERTY FOLDER "GameLib")
endif()

#------------------------------
# zpl-c/enet
# ENet reliable UDP networking library
#------------------------------
if(NOT TARGET enet)
    add_subdirectory(enet)
    set_property(TARGET enet PROPERTY FOLDER "GameLib")
endif()

#------------------------------
# GLAD
# OpenGL
#------------------------------
if(NOT TARGET glad AND NOT PLATFORM_ANDROID)
    add_subdirectory(glad)
    set_property(TARGET glad PROPERTY FOLDER "GameLib")
endif()

#------------------------------
# Lua
# lua lib
#------------------------------
if(NOT TARGET lua)
    project(lua VERSION 4.6 LANGUAGES C)
    file(GLOB PROJECT_SOURCE lua/*.h lua/*.c)
    add_library(lua STATIC ${PROJECT_SOURCE})
    target_include_directories(lua PUBLIC lua/)
endif()

#------------------------------
# GLAD
# OpenGL
#------------------------------
if(NOT TARGET sol2)
    set(SOL2_ENABLE_INSTALL OFF)
    set(SOL2_TESTS_SINGLE OFF)
    set(SOL2_TESTS_EXAMPLES OFF)
    set(SOL2_TESTS_INTEROP_EXAMPLES OFF)
    set(BUILD_LUA_AS_DLL OFF)
    set(SOL2_BUILD_LUA ON)
    add_subdirectory(sol2)
    set_property(TARGET sol2 PROPERTY FOLDER "GameLib")
endif()

#------------------------------
# ASSIMP
# Model loading library
#------------------------------
if(NOT TARGET assimp)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BUILD_SHARED_LIBS ON)
    else()
        set(BUILD_SHARED_LIBS OFF)
    endif()

    set(BUILD_SHARED_LIBS ON)

    cmake_policy(SET CMP0077 NEW) # set() overrides option()

    set(ASM686 OFF)
    set(AMD64 ON)
    set(ASSIMP_BUILD_ZLIB ON)
    set(ASSIMP_BUILD_TESTS OFF)
    set(ZLIB_AMD64 ON)
    set(ASSIMP_NO_EXPORT ON)
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
    #set(ASSIMP_BUILD_FBX_IMPORTER ON)

    set(ASSIMP_BUILD_B3D_IMPORTER ON)
    set(ASSIMP_BUILD_X3D_IMPORTER ON)

    set(ASSIMP_BUILD_DRACO OFF)
    add_subdirectory(assimp)

    set_property(TARGET assimp PROPERTY FOLDER "GameLib")

    if(BUILD_SHARED_LIBS)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            if(WIN32)
                if(MSVC)
                    if(CMAKE_GENERATOR MATCHES "Visual Studio")
                        set(ZEPHA_DLL_TO_COPY
                                ${ZEPHA_DLL_TO_COPY}
                                ${CMAKE_CURRENT_BINARY_DIR}/assimp/bin/Debug/assimp-vc142-mtd.dll
                                CACHE INTERNAL "ZEPHA_DLL_TO_COPY")
                    else()
                        set(ZEPHA_DLL_TO_COPY
                                ${ZEPHA_DLL_TO_COPY}
                                ${CMAKE_CURRENT_BINARY_DIR}/assimp/bin/assimp-vc142-mtd.dll
                                CACHE INTERNAL "ZEPHA_DLL_TO_COPY")
                    endif()
                else(MSVC)
                    set(ZEPHA_DLL_TO_COPY
                            ${ZEPHA_DLL_TO_COPY}
                            ${CMAKE_CURRENT_BINARY_DIR}/assimp/bin/libassimpd.dll
                            CACHE INTERNAL "ZEPHA_DLL_TO_COPY")
                endif(MSVC)
            else(WIN32)
                set(ZEPHA_DLL_TO_COPY
                        ${ZEPHA_DLL_TO_COPY}
                        ${CMAKE_CURRENT_BINARY_DIR}/assimp/bin/libassimpd.so
                        CACHE INTERNAL "ZEPHA_DLL_TO_COPY")
            endif(WIN32)
        else()
            if(WIN32)
                if(MSVC)
                    if(CMAKE_GENERATOR MATCHES "Visual Studio")
                        set(ZEPHA_DLL_TO_COPY
                                ${ZEPHA_DLL_TO_COPY}
                                ${CMAKE_CURRENT_BINARY_DIR}/assimp/bin/Release/assimp-vc142-mt.dll
                                CACHE INTERNAL "ZEPHA_DLL_TO_COPY")
                    else()
                        set(ZEPHA_DLL_TO_COPY
                                ${ZEPHA_DLL_TO_COPY}
                                ${CMAKE_CURRENT_BINARY_DIR}/assimp/bin/assimp-vc142-mt.dll
                                CACHE INTERNAL "ZEPHA_DLL_TO_COPY")
                    endif()
                else(MSVC)
                    set(ZEPHA_DLL_TO_COPY
                            ${ZEPHA_DLL_TO_COPY}
                            ${CMAKE_CURRENT_BINARY_DIR}/assimp/bin/libassimp.dll
                            CACHE INTERNAL "ZEPHA_DLL_TO_COPY")
                endif(MSVC)
            else(WIN32)
                set(ZEPHA_DLL_TO_COPY
                        ${ZEPHA_DLL_TO_COPY}
                        ${CMAKE_CURRENT_BINARY_DIR}/assimp/bin/libassimp.so
                        CACHE INTERNAL "ZEPHA_DLL_TO_COPY")
            endif(WIN32)
        endif()
    endif()

    set(BUILD_SHARED_LIBS OFF)
endif()

#------------------------------
# FastNoise2
# Modular node based noise generation library using SIMD, C++17 and templates
#------------------------------
if(NOT TARGET FastNoise)
    set(FASTNOISE2_NOISETOOL OFF)
    add_subdirectory(FastNoise2)
endif()

#------------------------------
# nothings stb
# multi util library
#------------------------------
if(NOT TARGET nothings-stb)
    project(nothings-stb VERSION 4.6 LANGUAGES C)
    file(GLOB PROJECT_SOURCE nothings-stb/*.h nothings-stb/*.c)
    add_library(nothings-stb STATIC ${PROJECT_SOURCE})
    target_include_directories(nothings-stb PUBLIC nothings-stb/)
endif()

#------------------------------
# GZIP
# compress library
#------------------------------
if(NOT TARGET gzip)
    project(gzip)
    add_library(gzip INTERFACE)
    target_include_directories(gzip INTERFACE gzip-hpp/include)
endif()

#------------------------------
# cute_headers
# Collection of cross-platform one-file C/C++ libraries with no dependencies, primarily used for games
#------------------------------
if(NOT TARGET cute_headers)
    project(cute_headers)
    add_library(cute_headers INTERFACE)
    target_include_directories(cute_headers INTERFACE cute_headers)
endif()

#--------------------------------
# nlohmann JSON
# JSON for Modern C++
#--------------------------------
if(NOT TARGET nlohmann_json)
    set(JSON_BuildTests OFF)
    set(JSON_Install OFF)
    add_subdirectory(nlohmann_json)
    set_property(TARGET nlohmann_json PROPERTY FOLDER "GameLib")
endif()