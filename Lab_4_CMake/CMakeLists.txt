cmake_minimum_required(VERSION 3.16)
project(Lab4 LANGUAGES CXX)

# Включаем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Общие файлы
set(COMMON_SOURCES
    ProcessLauncher.cpp
    ProcessLauncher.h
    BufferClass.h
    BufferClass.cpp
)

# --- Исполняемый файл Receiver ---
add_executable(Receiver
    Receiver.cpp          # Твой основной файл Receiver
    ${COMMON_SOURCES}     # Общие исходники
    Receiver.h
 )

# --- Исполняемый файл Sender ---
add_executable(Sender
    Sender.cpp            # Твой основной файл Sender
    ${COMMON_SOURCES}     # Общие исходники
    Sender.h
 )

# Если нужно, можно добавить опции компилятора для Windows
if(MSVC)
    target_compile_options(Receiver PRIVATE /W4 /EHsc)
    target_compile_options(Sender   PRIVATE /W4 /EHsc)
else()
    target_compile_options(Receiver PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(Sender   PRIVATE -Wall -Wextra -pedantic)
endif()
