cmake_minimum_required(VERSION 3.16)
project(Lab4 LANGUAGES CXX)

# --- Включаем C++17 ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Общие исходники в библиотеку ---
set(COMMON_SOURCES
    Process.cpp
    Process.h
    BufferClass.cpp
    BufferClass.h
)

add_library(common STATIC ${COMMON_SOURCES})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- Исполняемый файл Receiver ---
add_executable(Receiver Receiver.cpp Receiver.h)
target_link_libraries(Receiver PRIVATE common)

# --- Исполняемый файл Sender ---
add_executable(Sender Sender.cpp Sender.h)
target_link_libraries(Sender PRIVATE common)

# --- Подключение GoogleTest ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Тесты ---
add_executable(main_tests tests.cpp)
target_link_libraries(main_tests PRIVATE common gtest_main)

include(GoogleTest)
gtest_discover_tests(main_tests)

# --- Компиляторские опции ---
if(MSVC)
    target_compile_options(Receiver PRIVATE /W4 /EHsc)
    target_compile_options(Sender   PRIVATE /W4 /EHsc)
    target_compile_options(main_tests PRIVATE /W4 /EHsc)
else()
    target_compile_options(Receiver PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(Sender   PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(main_tests PRIVATE -Wall -Wextra -pedantic)
endif()

# --- Цель check: сначала тесты, потом сборка основной программы ---
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS main_tests
    COMMENT "Running GoogleTest suite..."
)

# --- Зависимости: сборка основной программы после успешных тестов ---
add_dependencies(Receiver check)
