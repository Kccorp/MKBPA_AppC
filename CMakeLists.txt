cmake_minimum_required(VERSION 3.16)
project(OpenWeatherMap C)

set(CMAKE_C_STANDARD 99)

add_executable(OpenWeatherMap main.c)

find_package (CURL)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(OpenWeatherMap ${CURL_LIBRARIES})
else()
    MESSAGE(FATAL_ERROR "LibCurl is not found in your system.")
endif (CURL_FOUND)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON-C REQUIRED json-c)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${JSON-C_INCLUDE_DIRS})
link_directories(${JSON-C_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${JSON-C_CFLAGS_OTHER})
# Add an executable compiled from hello.c

# Link the target to the GTK+ libraries
target_link_libraries(OpenWeatherMap ${JSON-C_LIBRARIES})