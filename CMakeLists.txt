cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(yamz_editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/yamz_engine)

file(GLOB_RECURSE SOURCES 
    src/*.cxx
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    yamz_engine
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        external/yamz_engine/include
)