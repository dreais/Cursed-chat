//
// Created by Valentin on 10/24/2019.
//

#ifndef CURSED_CHAT_CORE_SERVER_H
#define CURSED_CHAT_CORE_SERVER_H

#include <winsock2.h>
#include <stdbool.h>
#include "log_file.h"
#include "shared_header.h"

#define LOCAL_HOST "127.0.0.1"
#define LOCAL_PORT 10001

#define ACCEPTED_BACKLOG 1

#define SERVER_NAME "[SERVER]"

#define SERVER_MAJOR 0
#define SERVER_MINOR 2
#define SERVER_PATCH 0

typedef struct {
	SOCKET fd;
	SHORT events;
	SHORT revents;
} pollfd_t;

typedef struct {
	pollfd_t *fds;
	unsigned int fds_n;
	char **name;
	bool *first_data;
} poll_collect_t;

typedef struct {
	SOCKET sock;
	struct sockaddr_in name;
	poll_collect_t fd_pool;
} serv_core_t;


int create_poll(serv_core_t *server);


int server_shutdown(serv_core_t *server); // MAY GO STATIC
int server_startup(void);

#endif //CURSED_CHAT_CORE_SERVER_H
