cmake_minimum_required(VERSION 3.22)
project(EpromEmulator)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

set(VCPKG_TARGET_TRIPLET "x64-windows")
set(VCPKG_OVERLAY_PORTS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_ports)
set(VCPKG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg)

set(CMAKE_CXX_STANDARD 17)

# build for x86_64, because that is how the brew package is compiled
set(CMAKE_OSX_ARCHITECTURES "x86_64")

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(EpromEmulator ${SRCS})
target_include_directories(EpromEmulator PUBLIC include)

FIND_PACKAGE(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(EpromEmulator ${OpenCV_LIBS})
