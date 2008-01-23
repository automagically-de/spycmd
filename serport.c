#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

int serport_connect(const char *device)
{
	int ser, linebits;
	struct termios tios;

	ser = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(ser < 0)
	{
		perror(device);
		return -1;
	}

	if(tcgetattr(ser, &tios) < 0)
	{
		perror(device);
		return -1;
	}

	tios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	tios.c_cflag &= ~(CSIZE | CSTOPB | OPOST);
	tios.c_cflag |= (PARENB | PARODD);
	tios.c_cflag |= (CS8);

	tios.c_cc[VMIN] = 1;
	tios.c_cc[VTIME] = 0;

	if(cfsetispeed(&tios, B4800) < 0)
	{
		perror(device);
		return -1;
	}

	if(tcsetattr(ser, TCSAFLUSH, &tios) < 0)
	{
		perror(device);
		return -1;
	}

	ioctl(ser, TIOCMGET, &linebits);
	linebits |= TIOCM_DTR; /* enable DTR */
	linebits &= ~TIOCM_RTS; /* disable RTS */
	ioctl(ser, TIOCMSET, &linebits);

	return ser;
}

