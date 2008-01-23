#ifndef _SPYLIB_H
#define _SPYLIB_H

#include <unistd.h>

int spy_send_command(int fd, unsigned char *cmd, size_t len);

#endif
