//
// Created by Valentin on 10/24/2019.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../Headers/log_file.h"
#include "../../Headers/core_client.h"

static void remove_newline(char *buff, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++) {
		if (buff[i] == '\n') {
			buff[i] = '\0';
			return;
		}
	}
}

static int submit_nickname(user_t *user)
{
	char *protocol;

	read(STDIN_FILENO, user->nickname, MAX_NICK_LENGTH);
	fflush(stdin);
	remove_newline(user->nickname, MAX_NICK_LENGTH);
	protocol = malloc(sizeof(char) * (strlen(PROTOC_NICK) + strlen(user->nickname)) + 1);
	protocol[strlen(PROTOC_NICK) + strlen(user->nickname)] = '\0';
	strcat(protocol, PROTOC_NICK);
	strcat(protocol, user->nickname);
	output_logs_str(PREFIX_INFO, "PROTOCOL NICK SENT WITH %s AS VALUE\n", user->nickname);
	write(user->serv_sock, protocol, strlen(protocol));
	return 0;
}

static int protoc_msg(user_t *user)
{
	char *protocol = malloc(sizeof(char) * (strlen(PROTOC_SEND_MSG) + strlen(user->mesg)) + 1);

	if (protocol == NULL) {
		return 1;
	}
	protocol[strlen(PROTOC_SEND_MSG) + strlen(user->mesg)] = '\0';
	strcat(protocol, PROTOC_SEND_MSG);
	strcat(protocol, user->mesg);
	output_logs_str(PREFIX_INFO, "PROTOCOL MSG SENT WITH %s AS VALUE\n", user->mesg);
	write(user->serv_sock, protocol, strlen(protocol));
	return 0;
}

int client_run(int client_s)
{
	user_t user = {.serv_sock = client_s};
	bool connected = true;

	submit_nickname(&user);
	while (connected) {
		write(1, "> ", 2);
		read(STDIN_FILENO, user.mesg, MAX_MSG_LENGTH);
		remove_newline(user.mesg, MAX_MSG_LENGTH);
		protoc_msg(&user);
		connected = false;
	}
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