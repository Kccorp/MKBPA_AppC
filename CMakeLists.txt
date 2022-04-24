cmake_minimum_required(VERSION 3.16)
project(OpenWeatherMap C)

set(CMAKE_C_STANDARD 99)

add_executable(OpenWeatherMap test.c)

find_package (CURL)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(OpenWeatherMap ${CURL_LIBRARIES})
else()
    MESSAGE(FATAL_ERROR "LibCurl is not found in your system.")
endif (CURL_FOUND)

find_package(PkgConfig REQUIRED)

pkg_check_modules(JSON-C REQUIRED json-c)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${JSON-C_INCLUDE_DIRS})
link_directories(${JSON-C_LIBRARY_DIRS})

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${JSON-C_CFLAGS_OTHER})
add_definitions(${GTK3_CFLAGS_OTHER})
# Add an executable compiled from hello.c

# Link the target to the GTK+ libraries
target_link_libraries(OpenWeatherMap ${JSON-C_LIBRARIES})
target_link_libraries(OpenWeatherMap ${GTK3_LIBRARIES})

#add mysql library
target_link_libraries(OpenWeatherMap libmysqlclient.so)