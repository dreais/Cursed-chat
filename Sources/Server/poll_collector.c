//
// Created by Valentin on 10/24/2019.
//

#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../Headers/core_server.h"

int create_poll(serv_core_t *server)
{
	poll_collect_t new;
	char serv_name[] = SERVER_NAME;
	size_t fd_initial_size = 0;

	new.fds_n = 1;
	fd_initial_size = sizeof(pollfd_t) * new.fds_n;
	new.fds = malloc(fd_initial_size);
	new.name = malloc(sizeof(char *) * 1);
	if (new.fds == NULL) {
		output_logs_str(PREFIX_ERROR, "Couldn't allocate poll collector's file descriptor. Returning.\n");
		return(1);
	}
	if (new.name == NULL) {
		output_logs_str(PREFIX_ERROR, "Couldn't allocate server's name. Returning.\n");
		return(1);
	}
	new.name[0] = strdup(serv_name);
	if (new.name == NULL) {
		output_logs_str(PREFIX_ERROR, "Couldn't copy server's name. Returning.\n");
		return(1);
	}
	new.fds[0].fd = server->sock;
	new.fds[0].events = POLLIN;
	server->fd_pool = new;
	return 0;
}

int pollc_push_back(poll_collect_t *pollc, client_socket new)
{
	char *new_addr = inet_ntoa(new.socket_name.sin_addr);

	if (new_addr == NULL) {
		return 1;
	}
	pollc->fds_n++;
	pollc->name = realloc(pollc->name, sizeof(char *) * (pollc->fds_n + 1));
	pollc->name[pollc->fds_n - 1] = strdup(new_addr);

	pollc->fds = realloc(pollc->fds, sizeof(struct pollfd) * pollc->fds_n);
	pollc->fds[pollc->fds_n - 1].fd = new.fd;
	pollc->fds[pollc->fds_n - 1].events = POLLIN;
	write(pollc->fds[pollc->fds_n - 1].fd, CONNECTION_SUCCESSFUL, strlen(CONNECTION_SUCCESSFUL));
	return 0;
}

int read_data(int sender, char *buffer)
{
	int ret_value = 0;

	ret_value = recv(sender, buffer, BUFFER_SIZE, MSG_DONTWAIT);
	return ret_value;
}