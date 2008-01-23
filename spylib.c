#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define _XOPEN_SOURCE 500
#include <unistd.h>

int spy_send_command(int fd, unsigned char *cmd, size_t len)
{
	int i, n;
	unsigned char buffer[1024];
	unsigned char *sendbuf;
	int sum = 0;

	sendbuf = calloc(sizeof(unsigned char), len + 2);
	sendbuf[0] = 0x98;
	memcpy(sendbuf + 1, cmd, len);
	for(i = 0; i <= len; i ++)
		sum += sendbuf[i];
	sendbuf[len + 1] = 256 - (sum % 256);

	printf("Tx: ");
	for(i = 0; i < len + 2; i ++)
		printf("%02X ", sendbuf[i]);
	printf("\n");

	write(fd, sendbuf, len + 2);

	usleep(50000);
	n = read(fd, buffer, 1024);
	printf("Rx: ");
	for(i = 0; i < n; i ++)
		printf("%02X ", buffer[i]);
	printf("\n");

	return 0;
}
