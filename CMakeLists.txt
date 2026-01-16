cmake_minimum_required(VERSION 3.10)

project(MyProgram LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем, где лежат заголовочные файлы
include_directories(include)

# Создаём исполняемый файл
add_executable(MyProgram
    src/main.cpp
    src/wbi_incides.cpp
    src/file_processor.cpp
)
