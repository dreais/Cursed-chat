//
// Created by Valentin on 10/24/2019.
//

#include "../../Headers/core_server.h"

int winsock_initialize(void)
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

int server_socket_initialize(void)
{
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int optval = 0;

	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(int));
	return 0;
}