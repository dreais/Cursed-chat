#include <stdio.h>
#include <string.h>
#include "../Headers/core_server.h"
#include "../Headers/core_client.h"

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
	get_log_file();
	if (argc == 1) {
		printf("Opening logfile named %s\n", LOG_NAME);
		output_logs_str(PREFIX_INFO, "Server mode of Cursed chat.\n");
		output_logs_str(PREFIX_INFO, "Running server version %d.%d.%d\n", SERVER_MAJOR, SERVER_MINOR, SERVER_PATCH);
		server_startup();
	} else {
		if (strcmp(argv[1], "client") == 0) {
			output_logs_str(PREFIX_INFO, "Client mode of Cursed chat.\n");
			output_logs_str(PREFIX_INFO, "Running client version %d.%d.%d\n", CLIENT_MAJOR, CLIENT_MINOR, CLIENT_PATCH);
			client_initialize();
		}
	}
	close_file();
	return 0;
}