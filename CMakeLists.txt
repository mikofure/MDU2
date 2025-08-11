cmake_minimum_required(VERSION 3.16)
project(mdu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Release by default if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.5
)
FetchContent_MakeAvailable(SDL2)

# Nuklear GUI
FetchContent_Declare(
    nuklear
    GIT_REPOSITORY https://github.com/Immediate-Mode-UI/Nuklear
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

# FFmpeg (AVCodec)
FetchContent_Declare(
    ffmpeg
    GIT_REPOSITORY https://github.com/FFmpeg/FFmpeg.git
    GIT_TAG n6.0
)
FetchContent_MakeAvailable(ffmpeg)

if(WIN32)
    add_definitions(-DPLATFORM_WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
elseif(UNIX AND NOT APPLE)
    add_definitions(-DPLATFORM_LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(X11 REQUIRED x11)
endif()

# Source files
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.c)

add_executable(mdu ${SRC_FILES})

target_include_directories(mdu PRIVATE
    ${SDL2_SOURCE_DIR}/include
    ${nuklear_SOURCE_DIR}
    ${httplib_SOURCE_DIR}
    ${ffmpeg_SOURCE_DIR}/libavcodec
    ${ffmpeg_SOURCE_DIR}/libavformat
    ${ffmpeg_SOURCE_DIR}/libavutil
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/win32
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/linux
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/config
)

if(WIN32)
    target_link_libraries(mdu
        SDL2main
        SDL2-static
        dwmapi
        # Add other dependencies as needed
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(mdu
        SDL2main
        SDL2-static
        ${X11_LIBRARIES}
        # Add other dependencies as needed
    )
else()
    target_link_libraries(mdu
        SDL2main
        SDL2-static
        # Add other dependencies as needed
    )
endif()

# Output directory for Release builds
set_target_properties(mdu PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/release"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/debug"
)

# Copy font resources to build directory
add_custom_command(TARGET mdu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:mdu>/fonts
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts/InterVariable.ttf
        $<TARGET_FILE_DIR:mdu>/fonts/InterVariable.ttf
    COMMENT "Copying font resources to build directory"
)
