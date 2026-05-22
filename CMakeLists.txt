cmake_minimum_required(VERSION 3.20)
project(VIDEOGIOCO)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.2
)
FetchContent_MakeAvailable(SDL2)

# Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.5
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

# File sorgente di ImGui
set(IMGUI_SRCS
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

add_executable(VIDEOGIOCO 
    main.cpp 
    ${IMGUI_SRCS}
    src/core/WindowAdapter.cpp
    src/graphics/Map.cpp
    src/graphics/Raycaster.cpp
    src/editor/Editor.cpp
)

# Includi le cartelle necessarie
target_include_directories(VIDEOGIOCO PRIVATE 
    include 
    ${imgui_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}/backends
)

# Collega SDL2
target_link_libraries(VIDEOGIOCO PRIVATE SDL2::SDL2main SDL2::SDL2)

# Su Windows copia la DLL di SDL2
if(WIN32)
    add_custom_command(TARGET VIDEOGIOCO POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:VIDEOGIOCO>
    )
endif()
