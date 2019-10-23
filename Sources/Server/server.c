//
// Created by Valentin on 10/24/2019.
//

#include <io.h>
#include "../../Headers/core_server.h"

static int winsock_initialize(void)
{
	WSADATA wsa_data;
	int result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != NO_ERROR) {
		output_logs_str(PREFIX_ERROR, "WSA couldn't initialize. Exiting with error code %d\n", result);
		exit(result);
	}
	return result;
}

static int server_socket_initialize(serv_core_t *server)
{
	int optval = 0;

	server->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server->sock < 0) {
		output_logs_str(PREFIX_ERROR, "Server socket couldn't open, exiting.\n");
		exit(1);
	}
	output_logs_str(PREFIX_INFO, "Server socket opened at %d\n", server->sock);
	setsockopt(server->sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(int));
	memset((char *) &server->name, 0, sizeof(struct sockaddr_in));
	server->name = (struct sockaddr_in) {.sin_family = AF_INET, .sin_port = htons(LOCAL_PORT)};
	bind(server->sock, (struct sockaddr *) &server->name, sizeof(struct sockaddr_in));
	inet_pton(2, LOCAL_HOST, (char *) &server->name.sin_addr);
	listen(server->sock, 1);
	return 0;
}

int server_startup(void)
{
	serv_core_t server;

	winsock_initialize();
	server_socket_initialize(&server);
}

int server_shutdown(serv_core_t *server)
{
	shutdown(server->sock, 2);
	close(server->sock);
}