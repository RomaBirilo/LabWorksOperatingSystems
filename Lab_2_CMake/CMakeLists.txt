cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Lab2_CMake")

# --- Основная программа ---
add_executable(Lab2_CMake "main.cpp" "min_max.cpp" "min_max.h" "average.cpp" "average.h")

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL" CACHE STRING "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# --- Подключаем GoogleTest ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Тесты ---
add_executable(mytests tests.cpp min_max.cpp average.cpp)
target_link_libraries(mytests PRIVATE gtest gtest_main)
include(GoogleTest)
gtest_discover_tests(mytests)

# --- Создаём "check" цель, которая запускает тесты ---
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS mytests
    COMMENT "Running GoogleTest suite..."
)

# --- Делаем так, чтобы Lab2_CMake зависел от прохождения тестов ---
add_dependencies(Lab2_CMake check)
