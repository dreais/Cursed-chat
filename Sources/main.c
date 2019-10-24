#include <stdio.h>
#include "../Headers/core_server.h"

int main(void)
{
	get_log_file();
	output_logs_str(PREFIX_INFO, "Running server version %d.%d.%d\n", SERVER_MAJOR, SERVER_MINOR, SERVER_PATCH);
	server_startup();
	close_file();
	return 0;
}