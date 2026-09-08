cmake_minimum_required(VERSION 3.23)

# Use folder to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Currently disabled, used to move the files to their respective folders
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.LIB)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.DLL)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.EXE)

project(Helios-Renderer LANGUAGES CXX)

include_directories(ext/directx)

# Libraries to use
link_libraries(
    D3D12.lib
    dxgi.lib
    dxguid.lib
    d3dcompiler.lib
)

add_executable(HeliosRenderer WIN32)

# All src files to use
target_sources(HeliosRenderer
    PRIVATE
        src/main.cpp
        src/application.cpp
        
    PUBLIC
        FILE_SET renderHeaders
        TYPE HEADERS
        BASE_DIRS
            inc
        FILES
            inc/application.hpp
)

target_precompile_headers(HeliosRenderer
    PRIVATE
        <Windows.h>
        <shellapi.h>
        <wrl.h>

        <algorithm>
        <cassert>
        <chrono>
)

target_compile_definitions(HeliosRenderer
    PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

# Set HeliosRenderer as start project in VS
set_directory_properties( PROPERTIES
    VS_STARTUP_PROJECT HeliosRenderer
)