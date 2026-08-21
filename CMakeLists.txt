cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(yamz_editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/yamz_engine)

#ImGui
#//////////////////////////////////////////////////////////////////////////////
set(IMGUI_SRC
    external/ImGui/imgui.cpp
    external/ImGui/imgui_demo.cpp
    external/ImGui/imgui_draw.cpp
    external/ImGui/imgui_tables.cpp
    external/ImGui/imgui_widgets.cpp
    external/ImGui/backends/imgui_impl_opengl3.cpp
    external/ImGui/backends/imgui_impl_glfw.cpp
)

set(IMGUI_INCLUDES
    external/ImGui/imgui.h
    external/ImGui/backends/imgui_impl_opengl3.h
    external/ImGui/backends/imgui_impl_glfw.h
)

add_library(ImGui STATIC
    ${IMGUI_INCLUDES}
    ${IMGUI_SRC}
)

target_link_libraries(ImGui
    yamz_engine
)

target_include_directories(ImGui
    PUBLIC
        external/ImGui
        external/ImGui/backends
)
#//////////////////////////////////////////////////////////////////////////////

file(GLOB_RECURSE SOURCES 
    src/*.cxx
    src/*.cpp
)

file(GLOB_RECURSE YAMZ_PRV_INCLUDES
    include/*.hpp
    include/*.ipp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${YAMZ_PRV_INCLUDES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

target_link_libraries(${PROJECT_NAME}
    yamz_engine
    ImGui
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)