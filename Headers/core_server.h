//
// Created by Valentin on 10/24/2019.
//

#ifndef CURSED_CHAT_CORE_SERVER_H
#define CURSED_CHAT_CORE_SERVER_H

#include <winsock2.h>
#include "log_file.h"
#include "shared_header.h"

#define LOCAL_HOST "127.0.0.1"
#define LOCAL_PORT 10001

#define SERVER_MAJOR 0
#define SERVER_MINOR 1
#define SERVER_PATCH 0

typedef struct {
	SOCKET sock;
	struct sockaddr_in name;
} serv_core_t;

int server_shutdown(serv_core_t *server); // MAY GO STATIC
int server_startup(void);

#endif //CURSED_CHAT_CORE_SERVER_H
