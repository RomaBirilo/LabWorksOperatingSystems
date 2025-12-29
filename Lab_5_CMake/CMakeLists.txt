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

add_library(common STATIC ${COMMON_SOURCES} )
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- Исполняемый файл Server ---
add_executable(Server Server.cpp Server.h )
target_link_libraries(Server PRIVATE common)


# --- Исполняемый файл Client ---
add_executable(Client Client.cpp Client.h )
target_link_libraries(Client PRIVATE common)

# --- Подключение GoogleTest ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Тесты ---
add_executable(server_tests serverTests.cpp)
target_link_libraries(server_tests PRIVATE common gtest_main)

include(GoogleTest)
gtest_discover_tests(server_tests)

# --- Компиляторские опции ---
if(MSVC)
    target_compile_options(Server PRIVATE /W4 /EHsc)
    target_compile_options(Client   PRIVATE /W4 /EHsc)
    target_compile_options(server_tests PRIVATE /W4 /EHsc)
else()
    target_compile_options(Server PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(Client  PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(server_tests PRIVATE -Wall -Wextra -pedantic)
endif()

# --- Цель check: сначала тесты, потом сборка основной программы ---
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS server_tests
    COMMENT "Running GoogleTest suite..."
)

# --- Зависимости: сборка основной программы после успешных тестов ---
add_dependencies(Server check)


