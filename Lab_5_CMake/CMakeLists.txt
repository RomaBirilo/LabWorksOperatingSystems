cmake_minimum_required(VERSION 3.16)
project(Lab4 LANGUAGES CXX)

# --- Включаем C++17 ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Общие исходники в библиотеку ---
set(COMMON_SOURCES
    resourcesServer.h
    resourcesServer.cpp
    resourcesClient.h
    resourcesClient.cpp
)

add_library(common STATIC ${COMMON_SOURCES})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- Исполняемый файл Server ---
add_executable(Server Server.cpp Server.h)
target_link_libraries(Server PRIVATE common)


# --- Исполняемый файл Client ---
add_executable(Client Client.cpp Client.h)
target_link_libraries(Client PRIVATE common)



