cmake_minimum_required(VERSION 3.16)
project(mdu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.5 # Use a stable tag
)
FetchContent_MakeAvailable(SDL2)

# Nuklear GUI
FetchContent_Declare(
    nuklear
    GIT_REPOSITORY https://github.com/vurtun/nuklear.git
    GIT_TAG master
)
FetchContent_MakeAvailable(nuklear)

# cpp-httplib (HTTP library)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(httplib)

# FFmpeg (AVCodec) - Use https://github.com/FFmpeg/FFmpeg but building FFmpeg from source is complex.
# For demonstration, fetch the source but you may need to link to system-installed FFmpeg libs.
FetchContent_Declare(
    ffmpeg
    GIT_REPOSITORY https://github.com/FFmpeg/FFmpeg.git
    GIT_TAG n6.0
)
FetchContent_MakeAvailable(ffmpeg)

# Win32 is available by default on Windows
if(WIN32)
    add_definitions(-DPLATFORM_WIN32)
endif()

# Source files
file(GLOB SRC_FILES src/*.cpp src/*.c)

add_executable(mdu ${SRC_FILES})

target_include_directories(mdu PRIVATE
    ${SDL2_SOURCE_DIR}/include
    ${nuklear_SOURCE_DIR}
    ${httplib_SOURCE_DIR}
    ${ffmpeg_SOURCE_DIR}/libavcodec
    ${ffmpeg_SOURCE_DIR}/libavformat
    ${ffmpeg_SOURCE_DIR}/libavutil
)

target_link_libraries(mdu
    SDL2-static
    # You may need to manually link FFmpeg built libs here if built as part of your project
    # Or link to system FFmpeg if available
    # Add other dependencies as needed
)
