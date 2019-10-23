cmake_minimum_required(VERSION 3.14)
project(cursed_chat C)

set(CMAKE_C_STANDARD 11)

add_executable(cursed_chat Sources/main.c Sources/Server/server.c Headers/core_server.h Sources/log_file.c Headers/log_file.h Sources/inet_pton.c Headers/shared_header.h Sources/Server/poll_collector.c)

IF (WIN32)
    target_link_libraries(cursed_chat wsock32 ws2_32)
    # line below may be broken, you can delete it and try again
    target_compile_options(cursed_chat PRIVATE -D_WIN32_WINNT=0x0600)
ENDIF (WIN32)