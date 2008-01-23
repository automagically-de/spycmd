#include "spylib.h"

int nqc_Alive(int fd)
{
	unsigned char message = 0x10;
	return spy_send_command(fd, &message, 1);
}

int nqc_Set(int fd, unsigned short dst, unsigned short src)
{
	unsigned char message[6] = { 0x05, 0x00, 0x00, 0x02, 0x00, 0x00 };
	message[1] = dst >> 8;
	message[2] = dst & 0xFF;
	message[4] = src & 0xFF;
	message[5] = src >> 8;
	nqc_Alive(fd);
	return spy_send_command(fd, message, 6);
}

int nqc_SetLED(int fd, unsigned char mode, unsigned short v)
{
	return nqc_Set(fd, 0x1600 + mode, v);
}

int nqc_SetOutput(int fd, unsigned char o, unsigned char m)
{
	unsigned char message[2] = { 0x21, 0x00 };
	message[1] = o + m;
	nqc_Alive(fd);
	return spy_send_command(fd, message, 2);
}

int nqc_SetDirection(int fd, unsigned char o, unsigned char d)
{
	unsigned char message[2] = { 0xe1, 0x00 };
	message[1] = o + d;
	return spy_send_command(fd, message, 2);
}

int nqc_SetPower(int fd, unsigned char o, unsigned char p)
{
	unsigned char message[4] = { 0x13, 0x00, 0x02, 0x00 };
	message[1] = o;
	message[3] = p;
	return spy_send_command(fd, message, 4);
}

int nqc_PlaySound(int fd, unsigned s)
{
	unsigned char message[2] = { 0x51, 0x00 };
	message[1] = s;
	nqc_Alive(fd);
	return spy_send_command(fd, message, 2);
}
