# CMakeList.txt: проект CMake для Main; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Main "Main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Main PROPERTY CXX_STANDARD 20)
endif()

# Пути к exe других проектов
add_custom_target(copy_exes ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Creator> ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Reporter> ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(Main copy_exes Creator Reporter)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
