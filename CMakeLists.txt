cmake_minimum_required(VERSION 3.14)
project(cursed_chat C)

set(CMAKE_C_STANDARD 11)

add_executable(cursed_chat Sources/main.c Sources/Server/server.c Headers/core_server.h Sources/log_file.c Headers/log_file.h)

IF (WIN32)
    target_link_libraries(cursed_chat ws2_32)
ENDIF (WIN32)