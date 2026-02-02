cmake_minimum_required(VERSION 3.22)
project(obscura LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(OBSCURA_BUILD_DEMOS "Build interactive demos" ON)
option(OBSCURA_BUILD_TESTS "Build unit tests" ON)
option(OBSCURA_SANITIZERS "Enable ASan/UBSan in Debug" ON)
option(OBSCURA_TRACE "Enable runtime instrumentation overlays" ON)

add_library(obscura
        src/term/Terminal.cpp
        src/term/Input.cpp
        src/term/Screen.cpp
        src/term/Renderer.cpp

        src/ecology/World.cpp
        src/ecology/CellSettle.cpp
        src/ecology/Instrumentation.cpp

        src/organisms/Menu.cpp
        src/organisms/Box.cpp
        src/organisms/Statusline.cpp
)

target_include_directories(obscura PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if (OBSCURA_TRACE)
    target_compile_definitions(obscura PUBLIC OBSCURA_TRACE=1)
endif()

if (UNIX)
    target_compile_definitions(obscura PRIVATE OBSCURA_UNIX=1)
endif()

if (NOT MSVC)
    target_compile_options(obscura PRIVATE -Wall -Wextra -Wpedantic)
    if (OBSCURA_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(obscura PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(obscura PRIVATE -fsanitize=address,undefined)
    endif()
endif()

if (OBSCURA_BUILD_DEMOS)
    add_subdirectory(demos)
endif()

if (OBSCURA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
