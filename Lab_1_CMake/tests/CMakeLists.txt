cmake_minimum_required(VERSION 3.14)

# Собираем все .cpp из директории tests автоматически
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Создаём тестовый исполняемый файл (цель называется mytests)
add_executable(mytests ${TEST_SOURCES})
target_include_directories(mytests PRIVATE ${CMAKE_SOURCE_DIR}/include)
# Подключаем общие include (common) и gtest (gtest и gtest_main должны
# быть доступны, т.к. мы вызвали FetchContent_MakeAvailable в корне)
target_link_libraries(mytests PRIVATE common gtest gtest_main)

# Регистрируем тесты для ctest / Visual Studio Test Explorer
include(GoogleTest)
gtest_discover_tests(mytests)

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS mytests
    COMMENT "Running Google Tests..."
)