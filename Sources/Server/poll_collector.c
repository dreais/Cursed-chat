//
// Created by Valentin on 10/24/2019.
//

#include <stdlib.h>
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