//
// Created by Valentin on 10/24/2019.
//

#ifndef CURSED_CHAT_CORE_SERVER_H
#define CURSED_CHAT_CORE_SERVER_H

/**
 * --------- INCLUDES ---------
 */
#ifdef _WIN32
#include <winsock2.h>
#endif
#include <stdbool.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "log_file.h"
#include "shared_header.h"

/** --------------------------- */


/**
 * --------- DEFINES ----------
 */
 // addresses
#define LOCAL_HOST "127.0.0.1"
#define LOCAL_PORT 10001

#define ACCEPTED_BACKLOG 1

#define SERVER_NAME "[SERVER]"

// versions
#define SERVER_MAJOR 0
#define SERVER_MINOR 2
#define SERVER_PATCH 0

// str to send to clients
#define CONNECTION_SUCCESSFUL "Sucessfully established a connection.\n"

/** --------------------------- */
#ifdef _WIN32 // struct pollfd_t defined below for windows
typedef struct {
	SOCKET fd;
	SHORT events;
	SHORT revents;
} pollfd_t;
#else
typedef struct pollfd pollfd_t;
#endif

typedef struct {
	pollfd_t *fds;
	unsigned int fds_n;
	char **name;
	bool *first_data;
} poll_collect_t;

typedef struct {
	int fd;
	struct sockaddr_in socket_name;
	unsigned int addr_len;
} client_socket;

typedef struct {
	int sock;
	struct sockaddr_in name;
	poll_collect_t fd_pool;
	bool stop_server;
} serv_core_t;

int pollc_push_back(poll_collect_t *pollc, client_socket new);
int create_poll(serv_core_t *server);

int server_shutdown(serv_core_t *server); // MAY GO STATIC
int server_startup(void);

#endif //CURSED_CHAT_CORE_SERVER_H
