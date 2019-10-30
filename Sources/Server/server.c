//
// Created by Valentin on 10/24/2019.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <signal.h>
#include "../../Headers/core_server.h"

#ifdef _WIN32 // winsock_initialize for windows
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
#endif

static bool emergency_stop = false;

static void handle_sigint(int sig __attribute__((unused)))
{
	output_logs_str(PREFIX_ERROR, "Got a SIGINT signal - emergency stop.\n");
	fprintf(stderr, "\nStopping server\n");
	emergency_stop = true;
}

static int server_socket_initialize(serv_core_t *server)
{
	int optval = 0;

	server->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server->sock < 0) {
		output_logs_str(PREFIX_ERROR, "Server socket couldn't open, returning.\n");
		return(1);
	}
	output_logs_str(PREFIX_INFO, "Server socket opened at %d\n", server->sock);
	setsockopt(server->sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(int));
	memset((char *) &server->name, 0, sizeof(struct sockaddr_in));
	server->name = (struct sockaddr_in) {.sin_family = AF_INET, .sin_port = htons(LOCAL_PORT)};
	bind(server->sock, (struct sockaddr *) &server->name, sizeof(struct sockaddr_in));
#ifdef _WIN32
	inet_pton(2, LOCAL_HOST, (char *) &server->name.sin_addr);
#else
	inet_aton(LOCAL_HOST, &server->name.sin_addr);
#endif
	listen(server->sock, ACCEPTED_BACKLOG);
	server->stop_server = false;
	return 0;
}

static void socket_pop(serv_core_t *server, const unsigned int index)
{
	shutdown(server->fd_pool.fds[index].fd, 2);
	close(server->fd_pool.fds[index].fd);
	free(server->fd_pool.name[index]);
	if ((unsigned int) index < (server->fd_pool.fds_n - 1)) { // don't count the server's socket
		for (unsigned int i = index; i < (server->fd_pool.fds_n - 1); i++) {
			server->fd_pool.fds[i] = server->fd_pool.fds[i + 1];
			server->fd_pool.name[i] = server->fd_pool.name[i + 1];
		}
	}
	server->fd_pool.fds_n--;
}

static int handle_socket(serv_core_t *server)
{
	client_socket new;
	int result = 0;
	char buffer[BUFFER_SIZE];

	if (server->fd_pool.fds[0].revents & POLLIN) { // data is ready to read from the server
		new.fd = accept(server->fd_pool.fds[0].fd, (struct sockaddr *) &new.socket_name, &new.addr_len);
		result = pollc_push_back(&server->fd_pool, new);
		output_logs_str(PREFIX_DEBUG, "New connection on %s.\n", server->fd_pool.name[server->fd_pool.fds_n - 1]);
		return result;
	}
	for (unsigned int i = 0; i < server->fd_pool.fds_n; i++) {
		if (server->fd_pool.fds[i].revents & POLLIN) {
			printf("%sGot data from %s\n", SERVER_NAME, server->fd_pool.name[i]);
			result = read_data(server->fd_pool.fds[i].fd, buffer);
			if (result == -1) {
				output_logs_str(PREFIX_ERROR, "Recv couldn't receive message. Returning with %d.\n", result);
				return result;
			} else if (result == 0) {
				output_logs_str(PREFIX_WARNING, "Client disconnected.\n");
				socket_pop(server, i);
				return result;
			}
			printf("%sMessage received: %s\n", SERVER_NAME, buffer);
			memset(&buffer, '\0', BUFFER_SIZE);
		}
	}
	return result;
}

static int run_server(serv_core_t *server)
{
	int ret_value;

	while (emergency_stop == false && server->stop_server == false) {
		ret_value = poll(server->fd_pool.fds, server->fd_pool.fds_n, 5);
		if (emergency_stop == false && ret_value == -1) {
			output_logs_str(PREFIX_ERROR, "Poll failed, returning %d.\n", ret_value);
			return ret_value;
		}
		if (ret_value > 0) {
			ret_value = handle_socket(server);
		}
		if (ret_value < 0) {
			output_logs_str(PREFIX_ERROR, "Got an error, code %d.\n", ret_value);
			return ret_value;
		}
	}
	if (emergency_stop == true) {
		return 1;
	}
	return 0;
}

int server_startup(void)
{
	serv_core_t server;
	int result = 0;

#ifdef _WIN32
	winsock_initialize();
#endif
	signal(SIGINT, handle_sigint);
	result = server_socket_initialize(&server);
	if (result > 0) {
		output_logs_str(PREFIX_ERROR, "server_socket_initialize failed and returned %d. Exiting.\n", result);
		exit(result);
	}
	result = create_poll(&server);
	if (result > 0) {
		output_logs_str(PREFIX_ERROR, "create_poll failed and returned %d. Exiting.\n", result);
		exit(result);
	}
	result = run_server(&server);
	server_shutdown(&server);
	return result;
}

int server_shutdown(serv_core_t *server)
{
	output_logs_str(PREFIX_INFO, "Server closing..\n");
	shutdown(server->sock, 2);
	close(server->sock);
	return 0;
}