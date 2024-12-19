cmake_minimum_required(VERSION 3.10)

# Название проекта
project(FileAccessSystem)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Для Windows добавляем библиотеку kernel32
if(WIN32)
    set(EXTRA_LIBS kernel32)
endif()

# Добавление серверной части
add_executable(server server.cpp)
target_link_libraries(server PRIVATE ${EXTRA_LIBS})

# Добавление клиентской части
add_executable(client client.cpp)
target_link_libraries(client PRIVATE ${EXTRA_LIBS})