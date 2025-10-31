# CMakeList.txt: проект CMake для Lab3; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Lab3")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Lab3   "main.cpp" "ThreadData.h" "marker.cpp" "marker.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lab3 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
﻿cmake_minimum_required(VERSION 3.8)
project(Lab3)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Общая библиотека с кодом
add_library(marker STATIC marker.cpp marker.h ThreadData.h)

# Основная программа
add_executable(Lab3 main.cpp)
target_link_libraries(Lab3 PRIVATE marker)

# Подключение GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Тесты
add_executable(main_tests tests.cpp)
target_link_libraries(main_tests PRIVATE marker gtest_main)

include(GoogleTest)
gtest_discover_tests(main_tests)

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS main_tests
    COMMENT "Running GoogleTest suite..."
)
add_dependencies(Lab3 check)
