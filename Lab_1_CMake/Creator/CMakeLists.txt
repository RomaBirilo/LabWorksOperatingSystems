# CMakeList.txt: проект CMake для Main; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Creator "Creator.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Creator PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(Creator PRIVATE ${CMAKE_SOURCE_DIR}/include)


# TODO: Добавьте тесты и целевые объекты, если это необходимо.
