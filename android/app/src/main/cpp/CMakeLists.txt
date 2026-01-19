cmake_minimum_required(VERSION 3.22.1)

project(audiostream C)

# Opus ayarları (C ONLY)
set(OPUS_ENABLE_FLOAT_API OFF CACHE BOOL "" FORCE)
set(OPUS_ENABLE_FIXED_POINT ON CACHE BOOL "" FORCE)
set(OPUS_ENABLE_ASSERTIONS OFF CACHE BOOL "" FORCE)
set(OPUS_ENABLE_RTCD OFF CACHE BOOL "" FORCE)
set(OPUS_BUILD_SHARED_LIBRARY OFF CACHE BOOL "" FORCE)
set(OPUS_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(OPUS_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(OPUS_STACK_PROTECTOR OFF CACHE BOOL "" FORCE)

# Opus'u ekle
#add_subdirectory(opus2)

# Senin kütüphanen
add_library(
        audiostream
        SHARED
        audiostream.c
)

# Opus header
target_include_directories(
        audiostream
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/opus2/include/opus
)


# Android log
find_library(
        log-lib
        log
)

# Link
target_link_libraries(
        audiostream
        opus
        ${log-lib}
)
