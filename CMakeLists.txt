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

find_package(JSON)
if (JSON_FOUND)
    include_directories(${JSON_INCLUDE_DIRS})
    target_link_libraries(OpenWeatherMap ${JSON_LIBRARIES})
else()
    MESSAGE(FATAL_ERROR "JSON is not found in your system.")
endif (JSON_FOUND)


