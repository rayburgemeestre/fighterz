#include "../server/common.h"

int fucking_dead = 0;
int theSocket = 0;
int myid = 0;

int main(int argc, char *argv[])
{
struct sockaddr_in addr;
struct data *me;

	if ((theSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket error!\r\n");
		exit(EXIT_FAILURE);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	if (connect(theSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("Connect failed: %m\n");
		exit(EXIT_FAILURE);
	}

	/* [setsockopt] */
	set_sockopts(theSocket);

	me = malloc(sizeof(user));
	memset(me, 0, sizeof(user));
	me->buffer = malloc(8192);
	memset(me->buffer, 'X', 8192);
	
	while (1)
	{
		readpacket(me, theSocket);
		if (IsDead(me))
			break;
		usleep(500 * 1000);
		printf("."); fflush(stdout);
	}

	printf("\nEOFFffffffffffff\n");	
	close(theSocket);
	return 1;
}
