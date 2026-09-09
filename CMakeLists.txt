cmake_minimum_required(VERSION 3.23)

# Use folder to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Currently disabled, used to move the files to their respective folders
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.LIB)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.DLL)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.EXE)

# Use vcpkg dir
set(CMAKE_TOOLCHAIN_FILE "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(Helios-Renderer LANGUAGES CXX)

find_package(directx-dxc CONFIG REQUIRED)
find_package(directx-headers CONFIG REQUIRED)
find_package(directxmath CONFIG REQUIRED)
find_package(directxtex CONFIG REQUIRED)

add_executable(HeliosRenderer WIN32)

target_link_libraries(HeliosRenderer PUBLIC
    D3D12.lib
    dxgi.lib
    dxguid.lib
    d3dcompiler.lib
)

target_link_libraries(HeliosRenderer PUBLIC
    Microsoft::DirectXShaderCompiler 
    Microsoft::DirectX-Headers
    Microsoft::DirectX-Guids
    Microsoft::DirectXTex 
    Microsoft::DirectXMath 
)

# All src files to use
target_sources(HeliosRenderer
    PRIVATE
        src/main.cpp
        src/application.cpp
        src/window.cpp
        src/commandqueue.cpp
        
    PUBLIC
        FILE_SET renderHeaders
        TYPE HEADERS
        BASE_DIRS
            inc
        FILES
            inc/application.hpp
            inc/window.hpp
            inc/commandqueue.hpp
)

target_precompile_headers(HeliosRenderer
    PRIVATE
        <Windows.h>
        <shellapi.h>
        <wrl.h>

        <algorithm>
        <cassert>
        <chrono>
        <memory>

        inc/globalhelper.hpp
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