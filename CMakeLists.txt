cmake_minimum_required(VERSION 3.14)
project(cursed_chat C)

set(CMAKE_C_STANDARD 11)

add_executable(cursed_chat Sources/main.c Sources/Server/server.c)

IF (WIN32)
    target_link_libraries(cursed_chat ws2_32)
ENDIF (WIN32)