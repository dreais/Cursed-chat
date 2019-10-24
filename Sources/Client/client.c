//
// Created by Valentin on 10/24/2019.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "../../Headers/log_file.h"
#include "../../Headers/core_client.h"

int client_run(int client_s)
{
	
	return 0;
}

int client_initialize(void)
{
	int client = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client_name;
	int result = 0;

	if (client == -1) {
		output_logs_str(PREFIX_ERROR, "Client's socket couldn't be opened. Returning with %d.\n", client);
		return client;
	}
	memset(&client_name, 0, sizeof(struct sockaddr_in));
	client_name.sin_family = AF_INET;
	client_name.sin_port = htons(LOCAL_PORT);
	inet_aton(LOCAL_HOST, &client_name.sin_addr);
	result = connect(client, (struct sockaddr *) &client_name, sizeof(struct sockaddr_in));
	if (result == -1) {
		output_logs_str(PREFIX_ERROR, "Client couldn't connect to server. Returning with %d.\n", result);
		return result;
	}
	client_run(client);
	return result;
}